////////////////////////////////////////////////////////////////////////////////
//
// NDA AND NEED-TO-KNOW REQUIRED
//
// Copyright (C) 2021 Synaptics Incorporated. All rights reserved.
//
// This file contains information that is proprietary to Synaptics
// Incorporated ("Synaptics"). The holder of this file shall treat all
// information contained herein as confidential, shall use the
// information only for its intended purpose, and shall not duplicate,
// disclose, or disseminate any of this information in any manner
// unless Synaptics has otherwise provided express, written
// permission.
//
// Use of the materials may require a license of intellectual property
// from a third party or from Synaptics. This file conveys no express
// or implied licenses to any intellectual property rights belonging
// to Synaptics.
//
// INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
// SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
// INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
// INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
// CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
// OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
// BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
// COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
// DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
// TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
//
////////////////////////////////////////////////////////////////////////////////
/* ---------------------------------------------------------------------------------------------
    Below Macro 'PRINT_LEVEL_SET', Should be first line in this .c file, before any includes.
        Valid options are:
            > ASSERT_DISABLE
            > ASSERT_LEVEL_ERR
            > ASSERT_LEVEL_WARN
            > ASSERT_LEVEL_INFO
            > ASSERT_LEVEL_ALL
*/
#define PRINT_LEVEL_SET ASSERT_LEVEL_DBG
//-------------------------------- includes --------------------------------------
#include "mcu.h"
#include "camera_drv.h"
#ifdef CONFIG_CAMERA_SELECT_HM01B0
#include "hm01b0.h"
#endif
#include "i2spcm_drv.h"
#include "gpio_drv.h"
#include "adma_drv.h"
#include "uart_drv.h"
#include "tahiti_register_ahb.h"
#if defined(CONFIG_PRINT_TO_BT)
#include "bluetooth_drv.h"
#endif

//-------------------------------- Defines ---------------------------------------
#ifdef CONFIG_MCU_DRV_CAMERA

#define GPIO_TRIGGER        9
#define GPIO_VSYNC          11
#define GPIO_CAMERA_INT     10

#define EDGE_FALLING        0
#define EDGE_RISING         1

#define SEARCH_BUFFER_SIZE  1024


#define M33_BARRIER()       __DMB();    \
                            __DSB();    \
                            __ISB()

#define CAMERA_FLIP_CHECK   1
//#define CAMERA_LOCAL_TEST   1
//#define CAMERA_GPIO_DEBUG   1

#ifdef CAMERA_GPIO_DEBUG   
    #define NOTIFY_IO(x)    gpio_set(6, x)
#else
    #define NOTIFY_IO(x)    
#endif

//-------------------------------- Variables ---------------------------------------
static uint32_t i2s0_buffer[] = {0xffffff00, 0x0};
//static uint32_t i2s1_buffer[SEARCH_BUFFER_SIZE + 1];

const uint32_t dma_tx_dev_list[2] = {DEV_ADMA3_I2S_PCM0_TX_L, DEV_ADMA3_I2S_PCM0_TX_R};
#ifdef CONFIG_CAMERA_SERIAL_MODE
const uint32_t dma_rx_dev_list[2] = {DEV_ADMA3_I2S_PCM1_RX_L, DEV_ADMA3_I2S_PCM1_RX_R};
#else
const uint32_t dma_rx_dev_list[1] = {DEV_ADMA3_I2S_PCM1_RX_L};
#endif

volatile int dma_running = 0;

struct camera_ctrl_s
{
    uint32_t            init_sync;
    uint32_t            inited;
    uint32_t            ready;
    camera_callback     callback;
    uint32_t            *rx_buffer;
    uint32_t            buffer_size_dw;
    os_SemHandle	    wait_dma;
    uint32_t            motion_detection_en;
    uint32_t            resolution;
    uint32_t            sample_swap;
}camera_ctrl;

//-------------------------------- functions --------------------------------------

static int wait_for_vsync(int edge)
{
    int cnt = 0;

    int expect = (edge == EDGE_FALLING) ? 0 : 1;
    int read_val = 0;
    int last_time = 0;

    last_time = gpio_get(GPIO_VSYNC);
    //TODO: consider using semaphore
    while(1)
    {
        read_val = gpio_get(GPIO_VSYNC);
        if((last_time == !expect) && (read_val == expect))
            return 0;
        last_time = read_val;
    }

    return -1;
}

static void wait_vsync_level(int level, int cnt)
{
    int read_val = 0;
    int i = 0;

    while(1)
    {
        read_val = gpio_get(GPIO_VSYNC);
        if(read_val == level)
            break;
        if(cnt != 0)
        {
            if(cnt <= i++)
            {
                Print_ERR("ERR:NO VBLANK\n");
                break;
            }
        }
    }
}

static void chicken_run(int start)
{
    int read_val = 0;
    int cnt = 0;

    /*  while (1)
    {
        read_val = gpio_get(GPIO_VSYNC);
        if (read_val == 0)
            break;

        if(24576000 <= cnt++)
        {
            Print_ERR("ERR:NO VBLANK\n");
            break;
        }
    } */
    //wait_vsync_level(0, 245760);
    gpio_set(GPIO_TRIGGER, !!start);
}

void adma_rx_cb(void *data)
{
#ifdef CONFIG_CAMERA_SERIAL_MODE
    adma_enable(2, dma_rx_dev_list, ADMA_CH_DISABLE);
#else
    adma_enable(1, dma_rx_dev_list, ADMA_CH_DISABLE);
#endif
    dma_running = 0;

    if (camera_ctrl.ready == 0)
    {
        NOTIFY_IO(0);
        os_SemGive(camera_ctrl.wait_dma);
    }
    else
    {
        if (camera_ctrl.callback != NULL)
            camera_ctrl.callback(camera_ctrl.rx_buffer);
    }
}


static int trigger_dma(uint32_t buffer, uint32_t size)
{
    int err = 0;

    adma_channel_ctrl adma_ctrl;
    ADMA_START_ADDRESS Start_Address;

#ifdef CONFIG_CAMERA_SERIAL_MODE
    adma_ctrl.buffer_size               = size/2;
#else
    adma_ctrl.buffer_size               = size;
#endif
    adma_ctrl.ctrl.bf.samp_size         = ADMA_SAMPLE_SIZE_32D;
    adma_ctrl.ctrl.bf.s_cnt             = 0;
#ifdef CONFIG_CAMERA_SERIAL_MODE
    adma_ctrl.ctrl.bf.step_ofst         = 1;
#else
    adma_ctrl.ctrl.bf.step_ofst         = 0;
#endif
    adma_ctrl.ctrl.bf.address_direction = 1;
    adma_ctrl.ctrl.bf.burst_size        = 0;
    adma_ctrl.ctrl.bf.block_interrupt   = 1;

    Start_Address.dw = (uint32_t)buffer;
    adma_set_channel_ctrl(DEV_ADMA3_I2S_PCM1_RX_L, &adma_ctrl, &Start_Address);

#ifdef CONFIG_CAMERA_SERIAL_MODE
    Start_Address.dw = (uint32_t)buffer + 4;
    adma_set_channel_ctrl(DEV_ADMA3_I2S_PCM1_RX_R, &adma_ctrl, &Start_Address);
#endif

    int cnt = 0;
    while(1)
    {
        if(dma_running == 0)
            break;
        Print_DBG("DMA running!\n");
        if(2000 < cnt++)
        {
            Print_ERR("ERR:DMA LOCK\n");
        }
    }

    dma_running = 1;
#ifdef CONFIG_CAMERA_SERIAL_MODE
    err = adma_enable(2, dma_rx_dev_list, ADMA_CH_ENABLE);
#else
    err = adma_enable(1, dma_rx_dev_list, ADMA_CH_ENABLE);
#endif
    return err;
}

void Stop_DMA()
{
#ifdef CONFIG_CAMERA_SERIAL_MODE
    adma_enable(2, dma_rx_dev_list, ADMA_CH_DISABLE);
#else
    adma_enable(1, dma_rx_dev_list, ADMA_CH_DISABLE);
#endif
    dma_running = 0;
}

#if defined(CONFIG_CAMERA_SERIAL_MODE) && defined(CONFIG_CAMERA_SOFT_SERIAL_MODE)
/**
 * @brief Shift i2s1 ws to align hsync
 * 
 * @param right : rifht value pointer
 * @param left : left value pointer
 * @param obsv : value in buffer
 * @param dir : 0: shift left, 1 shift right
 */
static void shift_ws(uint32_t *left, uint32_t *right, uint32_t obsv, int dir)
{
    int i = 0;
    int cnt = 0;
    uint32_t data_left  = 0;
    uint32_t data_right = 0;
    uint64_t i2s_val    = 0x0000ffffff000000;
    uint64_t i2s_marker = 0x0001000000000000;

    int offset = dir ? ((~obsv) & 0xffffff00) : obsv;

    cnt = __builtin_popcount(offset);
    if(dir == 0)
    {
        for (i = 0; i < cnt; i++)
        {
            i2s_val <<= 1;
            if ((i2s_val & i2s_marker) != 0x00)
            {
                i2s_val |= 0x01;
            }
        }
    }
    else
    {
        for (i = 0; i < cnt; i++)
        {
            if ((i2s_val & 0x01) != 0x00)
            {
                i2s_val |= i2s_marker;
            }
            i2s_val >>= 1;
        }
    }
    i2s_val &= 0x0000ffffffffffff;

    data_right = ((i2s_val & 0x00ffffff) << 8) & 0xffffff00;
    data_left = (i2s_val >> 16) & 0xffffff00;

    *left = data_left;
    *right = data_right;

    Print_ERR("Shift(Dir-%d:%d), 0x%x, 0x%x\n", dir, cnt, *left, *right);

}


static int error_pattern_search(uint32_t *buf, int size)
{
    int i = 0;
    int find = 0;
    for(i=0;i<size-3;i++)
    {
        if((buf[i]!=0x0) && (buf[i+1]==0x0) && (buf[i+2]!=0x0))
        {
            find = 1;
            break;
        }
        if((buf[i]==0) && (buf[i+1]!=0) && (buf[i+2]==0))
        {
            find = 1;
            break;
        }
    }
    if(find == 1)
    {
        Print_ERR("Error Pattern Found(0x%x, 0x%x, 0x%x)\n", buf[i], buf[i+1], buf[i+2]);
        Print_DBG("Probe:\n");
        for (i = 0; i < SEARCH_BUFFER_SIZE; i += 4)
        {
            Print_DBG("%x, %x, %x, %x\n", buf[i], buf[i + 1], buf[i + 2], buf[i + 3]);
        }
        return i;
    }
    return -1;
}

/**
 * @brief Search data pattern to find hsync rising stage
 * 
 * @param buffer : buffer pointer
 * @param size : buffer size
 * @return int 
 */
static int search_hsync_rising(uint32_t *buffer, int size)
{
    int i = 0;
    int j = 0;

    i = error_pattern_search(buffer, size);
    if (i >= 0)
    {
        return -5;
    }

    for (i = 0; i < size; i++)
    {
        if (buffer[i] == 0x0)
            break;
    }
    if (i >= size)
    {
        return -3;
    }

    for (j = i; j < size; j++)
    {
        if (buffer[j] != 0x0)
            break;
    }
    if (j >= size)
    {
        return -4;
    }

    return j;
}


/**
 * @brief Search pattern and adjust the ws with hsync
 * 
 * @return int: error code:
 *                  0   success
 *                  -1  capture vsync error
 *                  -2  no pattern find
 *                  -3  no low hsync
 *                  -4  no rising hsync
 *                  -5  wrong buffer pattern
 */
static int sync_ws_hsync()
{
    int i = 0;
    int err = 0;
    
    if (camera_ctrl.init_sync != 0)
    {
        wait_vsync_level(0, 0);
        set_work_mode(0);
    }
    else
    {
        camera_ctrl.init_sync = 1;
    }

    //*((volatile uint32_t *)0x42005c00) = '<';
    int val = wait_for_vsync(EDGE_RISING);
    if (val != 0)
    {
        Print_ERR("VSYNC CAPTURE\n");
        return -1;
    }
    //*((volatile uint32_t *)0x42005c00) = '>';

    trigger_dma((uint32_t)camera_ctrl.rx_buffer, SEARCH_BUFFER_SIZE);
    NOTIFY_IO(1);

    //*((volatile uint32_t *)0x42005c00) = '{';
    os_SemTake(camera_ctrl.wait_dma, OS_SEM_WAIT_FOREVER);
    //*((volatile uint32_t *)0x42005c00) = '}';
    

#if 0
    Print_ERR("Probe:\n");
    for (i = 0; i < SEARCH_BUFFER_SIZE; i += 4)
    {
        Print_ERR("%x, %x, %x, %x\n", camera_ctrl.rx_buffer[i], camera_ctrl.rx_buffer[i + 1], camera_ctrl.rx_buffer[i + 2], camera_ctrl.rx_buffer[i + 3]);
    }
#endif

    i = search_hsync_rising(camera_ctrl.rx_buffer, SEARCH_BUFFER_SIZE);
    if(i < 0)
    {
        //return i;
        err = i;
        goto enable_workmode;
    }

    Print_DBG("Pattern(%d): 0x%x, 0x%x, 0x%x\n", i, camera_ctrl.rx_buffer[i - 1], camera_ctrl.rx_buffer[i], camera_ctrl.rx_buffer[i + 1]);

    uint32_t new_left = 0;
    uint32_t new_right = 0;

    if ((camera_ctrl.rx_buffer[i] == 0x00) || (camera_ctrl.rx_buffer[i] == 0xffffff00))
    {
        //Nothing really need to change
        Print_DBG("Already aligned\n");
    }
    else
    {
        shift_ws(&new_left, &new_right, camera_ctrl.rx_buffer[i], ((i - 1) % 2));

        //Update I2S1
        i2s0_buffer[0] = new_left;
        i2s0_buffer[1] = new_right;
    }

    NOTIFY_IO(1);

#ifdef CAMERA_FLIP_CHECK
    Print_DBG("Flip Check\n");
    os_TaskSleepms(1);

    NOTIFY_IO(0);

    trigger_dma((uint32_t)camera_ctrl.rx_buffer, SEARCH_BUFFER_SIZE*2);
    NOTIFY_IO(1);
    os_SemTake(camera_ctrl.wait_dma, OS_SEM_WAIT_FOREVER);

#if 0
    Print_ERR("Probe2:\n");
    for (i = 0; i < 128; i += 4)
    {
        Print_ERR("%x, %x, %x, %x\n", camera_ctrl.rx_buffer[i], camera_ctrl.rx_buffer[i + 1], camera_ctrl.rx_buffer[i + 2], camera_ctrl.rx_buffer[i + 3]);
    }
#endif

    i = search_hsync_rising(&camera_ctrl.rx_buffer[SEARCH_BUFFER_SIZE], SEARCH_BUFFER_SIZE);
    if (i < 0)
    {
        err = i;
    }
    else
    {
        M33_BARRIER();
        if ((i - 1) % 2 == 0)
        {
            //flip
            i2s0_buffer[0] = (~new_left);
            i2s0_buffer[1] = (~new_right);
            camera_ctrl.sample_swap = 1;
            Print_DBG("Warn: Recover from fliped!0x%x, 0x%x\n", i2s0_buffer[0], i2s0_buffer[1]);
        }
    }

    //Print_DBG("Pattern2(%d): 0x%x, 0x%x, 0x%x\n", i, camera_ctrl.rx_buffer[i - 1], camera_ctrl.rx_buffer[i], camera_ctrl.rx_buffer[i + 1]);

#endif

enable_workmode:
    wait_vsync_level(0, 0);
    set_work_mode(1);
    return err;
}
#endif

//-------------------------------  Interface API ----------------------------------
int _camera_sample_check()
{
    return camera_ctrl.sample_swap;
}

#if defined(CONFIG_PRINT_TO_UART)

void print_dw(uint32_t dw, int pixel)
{
    int i = 0;
    union
    {
        uint32_t data;
        uint8_t bytes[4];
    } cvt;

    cvt.data = dw;

    if (pixel == 0)
    {
        for (i = 0; i < 4; i++)
        {
            uart_putchar(cvt.bytes[3 - i]);
        }
    }
    else
    {
        for (i = 0; i < 3; i++)
        {
            uart_putchar(cvt.bytes[3 - i]);
        }
    }
}

void _print_image(uint32_t *buf, int dw_size)
{
    uint32_t *ptr;
    ptr = (uint32_t *)buf;
    int i = 0;

    Print_ERR("[$MAGIC~IMAGE!$]");

    //TODO: disable printf
    M33_BARRIER();
    print_enable(0);
    //M33_BARRIER();
#ifdef CONFIG_CAMERA_SERIAL_MODE
    //Under serial mode, for soft-serial mode, first 6 pixels are from last frame so should cut first 6 pixel and 
    //add 6 0s on the bottom
    //for hard-serial mode, if ws_gen_polarity=0 and ws_gen_mode = 0 the first 3 pixels are missed so should added 3 pixel at start
    //if ws_gen_polarity-0 and ws_gen_mode =1, first 2 pixels are missed so should added 2 pixel at start

    uint32_t cnt = dw_size * 3;
    print_dw(cnt, 0);
    for (i = 2; i < dw_size; i++)
    {
        print_dw(ptr[i], 1);
    }

    print_dw(0x0, 1);
    print_dw(0x0, 1);

#else
    uint32_t cnt = dw_size * 4; //total bytes need to transfer
    print_dw(cnt, 0);
    
    for(i = 0; i < dw_size; i++)
    {
        print_dw(ptr[i], 0);
    }

#endif
    //TODO enable printf
    print_enable(1);
}

#elif defined(CONFIG_PRINT_TO_BT) 

#define BT_IMAGE_FRAME_PAYLOAD_LEN  240
#pragma pack(push, 1)
typedef struct 
{
    uint16_t full_page_size;    //Total pages for a image
    uint16_t page_id;           //Current page id
    uint16_t num_of_sample;     //How many sample in payload
    uint16_t reserved;          //not used
    uint8_t data[BT_IMAGE_FRAME_PAYLOAD_LEN]; //payload
} bt_image_frame_t;
#pragma pack(pop)

bt_image_frame_t __ALIGNED(4) image_frame;


void print_dw_to_ptr(uint8_t **ptr, uint32_t dw, int pixel)
{
    int i = 0;
    union
    {
        uint32_t data;
        uint8_t bytes[4];
    }cvt;

    cvt.data = dw;

    if(pixel == 0)
    {
        for (i = 0; i < 4; i++)
        {
            **ptr = (cvt.bytes[3 - i]);
            (*ptr)++;
        }
    }
    else
    {
        for (i = 0; i < 3; i++)
        {
            **ptr = (cvt.bytes[3 - i]);
            (*ptr)++;
        }
    }

}

void _print_image(uint32_t *buf, int dw_size)
{
    int i, j, page_num;
    uint8_t *ptr;

#ifdef CONFIG_CAMERA_SERIAL_MODE
    //Under serial mode, for soft-serial mode, first 6 pixels are from last frame so should cut first 6 pixel and
    //add 6 0s on the bottom
    //for hard-serial mode, if ws_gen_polarity=0 and ws_gen_mode = 0 the first 3 pixels are missed so should added 3 pixel at start
    //if ws_gen_polarity-0 and ws_gen_mode =1, first 2 pixels are missed so should added 2 pixel at start
    //For BT, the basic format is
    int valid_pixel_cnt = dw_size * 3;
    int frame_cnt = BT_IMAGE_FRAME_PAYLOAD_LEN / 3;
#else
    int valid_pixel_cnt = dw_size * 4;
    int frame_cnt = BT_IMAGE_FRAME_PAYLOAD_LEN / 4;
#endif

    int last_samples_cnt = valid_pixel_cnt % BT_IMAGE_FRAME_PAYLOAD_LEN;

    page_num = valid_pixel_cnt / BT_IMAGE_FRAME_PAYLOAD_LEN;

    if (last_samples_cnt)
        page_num++;
    else
        last_samples_cnt = BT_IMAGE_FRAME_PAYLOAD_LEN;

    
    image_frame.full_page_size = page_num;
    
#ifdef CONFIG_CAMERA_SERIAL_MODE
    int idx = 2;
    for(i = 0; i < page_num; i++)
    {
        image_frame.page_id = i;
        memset(image_frame.data, 0, BT_IMAGE_FRAME_PAYLOAD_LEN);
        ptr = image_frame.data;

        if ((i + 1) >= page_num)
        {
            image_frame.num_of_sample = last_samples_cnt;
            for (j = 0; j < last_samples_cnt/3 - 2; j++)
            {
                print_dw_to_ptr(&ptr, buf[idx++], 1);
            }
            print_dw_to_ptr(&ptr, 0, 1);
            print_dw_to_ptr(&ptr, 0, 1);
        }
        else
        {
            image_frame.num_of_sample = BT_IMAGE_FRAME_PAYLOAD_LEN;
            for (j = 0; j < frame_cnt; j++)
            {
                print_dw_to_ptr(&ptr, buf[idx++], 1);
            }
        }

        bt_sendmsg(BT_MSG_CH_IMAGE, BT_MSG_TYPE_NOTIFY, (uint8_t *)&image_frame, sizeof(bt_image_frame_t));
    }
#else
    int idx = 0;
    for (i = 0; i < page_num; i++)
    {
        image_frame.page_id = i;
        image_frame.num_of_sample = (((i + 1) >= page_num) ? last_samples_cnt : BT_IMAGE_FRAME_PAYLOAD_LEN);
        memset(image_frame.data, 0, BT_IMAGE_FRAME_PAYLOAD_LEN);
        ptr = iamge_frame.data;

        for (j = 0; j < image_frame.num_of_sample / 4; j++)
        {
            print_dw_to_ptr(&ptr, buf[idx++], 0);
        }

        bt_sendmsg(BT_MSG_CH_IMAGE, BT_MSG_TYPE_NOTIFY, (uint8_t *)&image_frame, sizeof(bt_image_frame_t));
    }
#endif
}
#endif

int _camera_capture(uint32_t *buf, uint32_t buf_size_dw)
{
    int err = 0;

    if (camera_ctrl.inited == 0)
    {
        Print_ERR("Camera not inited:%d\n", __LINE__);
        return -1;
    }

    camera_ctrl.rx_buffer = buf;
    camera_ctrl.buffer_size_dw = buf_size_dw;

    if (camera_ctrl.rx_buffer == NULL)
        return -2;

    //Sync
    M33_BARRIER();
#if defined(CONFIG_CAMERA_SERIAL_MODE) && defined(CONFIG_CAMERA_SOFT_SERIAL_MODE)
    if(camera_ctrl.buffer_size_dw <= SEARCH_BUFFER_SIZE)
        return -3;
    
    if (camera_ctrl.ready == 0)
    {
        err = sync_ws_hsync();
        if (err != 0)
        {
            Print_ERR("ERR:Sync:%d\n", err);
            return -4;
        }
        else
        {
            camera_ctrl.ready = 1;
        }
    }
#endif
    //Stop DMA
    Stop_DMA();

    //Wait for vsync falling
    int val = wait_for_vsync(EDGE_FALLING);
    if (val != 0)
    {
        Print_ERR("VSYNC CAPTURE\n");
        return -5;
    }

#if defined(CONFIG_CAMERA_SERIAL_MODE) && defined(CONFIG_CAMERA_HARD_SERIAL_MODE)
    volatile I2S_PCM_s *i2s_reg = (volatile I2S_PCM_s *)I2S_PCM_s_BASE_ADDRESS;
    i2s_reg->i2s1_pcm_reg14.dw = 0x06;
    i2s_reg->i2s1_pcm_reg14.dw = 0x0E;
#endif

    //Start capture
    trigger_dma((uint32_t)camera_ctrl.rx_buffer, camera_ctrl.buffer_size_dw);

}

int _camera_reset()
{
    Stop_DMA();
    camera_ctrl.ready = 0;
}

int _camera_setup(camera_setup_t *setup)
{
    int err = 0;

    if(camera_ctrl.inited == 0)
    {
        Print_ERR("Camera not inited:%d\n", __LINE__);
        return -1;
    }

    camera_ctrl.callback            = setup->done_cb;
    camera_ctrl.motion_detection_en = setup->motion_detection_en;
    camera_ctrl.rx_buffer           = setup->buffer;
    camera_ctrl.buffer_size_dw      = setup->buffer_size;

#ifdef CONFIG_CAMERA_SELECT_HM01B0
#ifdef CONFIG_CAMERA_SERIAL_MODE
    #define SENSOR_MINIMAL_FRAME_SIZE   34992
#else
    #define SENSOR_MINIMAL_FRAME_SIZE   26244
#endif
    if (camera_ctrl.buffer_size_dw < SENSOR_MINIMAL_FRAME_SIZE) //at least to hold 324*324
    {
        Print_ERR("ERR:Buffer size\n");
        return -2;
    }
#endif

#if defined(CONFIG_CAMERA_SERIAL_MODE) && defined(CONFIG_CAMERA_SOFT_SERIAL_MODE)
    camera_ctrl.ready = 0;
    M33_BARRIER();

    err = sync_ws_hsync();
    if (err != 0)
    {
        Print_ERR("ERR:Sync:%d\n", err);
    }
#endif

    camera_ctrl.ready = 1;

    return 0;
}

#ifdef CAMERA_LOCAL_TEST
void user_callback(void *msg)
{
    Print_ERR("Captured Image!!0x%x\n", (uint32_t)msg);

#ifdef CONFIG_CAMERA_SERIAL_MODE
    print_image(msg, 34992);
#else
    print_image(msg, 26244);
#endif
}
#endif

#ifdef CONFIG_CAMERA_SERIAL_MODE
int _camera_drv_init()
{
    int err = 0;
    memset(&camera_ctrl, 0, sizeof(camera_ctrl));

    //Setup GPIO
    gpiomux_select_gpio(GPIO_TRIGGER,       IO_MUX_ENABLE, OUT_DIR, DRV_CUR_2mA_FAST, PULL_DISABLED);
    gpiomux_select_gpio(GPIO_VSYNC,         IO_MUX_ENABLE, IN_DIR, DRV_CUR_2mA_FAST, PULL_DISABLED);
    gpiomux_select_gpio(GPIO_CAMERA_INT,    IO_MUX_ENABLE, IN_DIR, DRV_CUR_2mA_FAST, PULL_DISABLED);
    gpio_set(GPIO_TRIGGER, 0);

    *((volatile uint32_t *)PADCTRL_MB_SNS_ADDRESS) &= 0xfffffeff;
    *((volatile uint32_t *)PADCTRL_MB_SNS_ADDRESS) |= 0x80;

#ifdef CAMERA_GPIO_DEBUG
    gpiomux_select_gpio(6, IO_MUX_ENABLE, OUT_DIR, DRV_CUR_2mA_FAST, PULL_DISABLED);
    gpio_set(6, 0);
#endif

    //--------------  TX ------------------
    //Setup I2S0
    i2s_config_t cfg;
    cfg.tx_rx          = I2S_PCM_TX;
    cfg.sample_rate    = 512000;
    cfg.justified      = I2S_JUSTIFIED_LEFT;
    cfg.dstart_delay   = I2S_DELAY_0_BIT;
    cfg.lrclk_pol      = I2S_LRCLK_POL_LEFT_HIGH;
    cfg.lrck_low_first = I2S_LRCK_FIRST_HIGH;
    i2s_pcm_config_i2s(DEV_I2S0, &cfg);

    i2s_pcm_en_t i2s_cfg;
    i2s_cfg.tx_rx        = I2S_PCM_TX;
    i2s_cfg.sample_width = I2S_PCM_SAMPLE_WIDTH_24_BIT;
    i2s_cfg.framelength  = 48;
    i2s_cfg.channels     = 2;
    i2s_cfg.flag         = I2SPCM_EN_FLAG_WITH_RST_TX | I2SPCM_EN_FLAG_WITHOUT_EN;
    i2s_pcm_enable(DEV_I2S0, (i2s_pcm_en_t *)&i2s_cfg);

    //Enable I2S0
    i2s_cfg.flag = 0;
    i2s_pcm_enable(DEV_I2S0, (i2s_pcm_en_t *)&i2s_cfg);

    //Setup Tx DMA 

    struct adma_callback cb;
    cb.callback = NULL;

#if defined(CONFIG_CAMERA_SERIAL_MODE) && defined(CONFIG_CAMERA_SOFT_SERIAL_MODE)
    adma_set_irq_cb(2, dma_tx_dev_list, &cb, 0);

    adma_channel_ctrl adma_ctrl;
    adma_ctrl.buffer_size               = 1;
    adma_ctrl.ctrl.bf.samp_size         = ADMA_SAMPLE_SIZE_32D;
    adma_ctrl.ctrl.bf.s_cnt             = 0;
    adma_ctrl.ctrl.bf.step_ofst         = 1;
    adma_ctrl.ctrl.bf.address_direction = 1;
    adma_ctrl.ctrl.bf.burst_size        = 0;
    adma_ctrl.ctrl.bf.block_interrupt   = 0;

    ADMA_START_ADDRESS Start_Address;
    Start_Address.dw = (uint32_t)i2s0_buffer;
    adma_set_channel_ctrl(DEV_ADMA3_I2S_PCM0_TX_L, &adma_ctrl, &Start_Address);

    Start_Address.dw = (uint32_t)i2s0_buffer + 4;
    adma_set_channel_ctrl(DEV_ADMA3_I2S_PCM0_TX_R, &adma_ctrl, &Start_Address);

    adma_enable(2, dma_tx_dev_list, ADMA_CH_ENABLE);
#endif
  
    //--------------  RX ------------------
    //Setup I2S1
    cfg.tx_rx                = I2S_PCM_RX;
    cfg.sample_rate          = 0;
    cfg.justified            = I2S_JUSTIFIED_LEFT;
    cfg.dstart_delay         = I2S_DELAY_0_BIT;
    cfg.lrclk_pol            = I2S_LRCLK_POL_LEFT_HIGH;
    cfg.lrck_low_first       = I2S_LRCK_FIRST_HIGH;
    cfg.rx_en_by_sync_module = 0;
    i2s_pcm_config_i2s(DEV_I2S1, &cfg);

    i2s_cfg.tx_rx        = I2S_PCM_RX;
    i2s_cfg.sample_width = I2S_PCM_SAMPLE_WIDTH_24_BIT;
    i2s_cfg.framelength  = 48;
    i2s_cfg.channels     = 2;
    i2s_cfg.flag         = I2SPCM_EN_FLAG_WITH_RST_RX | I2SPCM_EN_FLAG_WITHOUT_EN;
    i2s_pcm_enable(DEV_I2S1, (i2s_pcm_en_t *)&i2s_cfg);
    i2s_cfg.flag = 0;
    i2s_pcm_enable(DEV_I2S1, (i2s_pcm_en_t *)&i2s_cfg);

    //Print_ERR("PAD=0x%x\n", *((volatile uint32_t *)0x42023470));
    //FIXME: Pad control need driver support
    *((volatile uint32_t *)PADCTRL_I2S0_BCLK_ADDRESS)   = 0x3180;
    *((volatile uint32_t *)PADCTRL_I2S0_TX_DAT_ADDRESS) = 0x3104;
    //*((volatile uint32_t *)0x42023444) = 0x3108;
    //*((volatile uint32_t *)0x4202344c) = 0x3100;


    //Setup Rx DMA
    cb.callback = adma_rx_cb;
    cb.data = NULL;
    adma_set_irq_cb(2, dma_rx_dev_list, &cb, 1);

    os_SemInit SemInit;
    SemInit.uSemType     = SEM_BINARY;
    camera_ctrl.wait_dma = os_SemCreate(&SemInit);

    //---------------- Init Sensor --------------------------

    int val = gpio_get(GPIO_VSYNC);
    if (val == 1)
    {
        Print_ERR("Warn:VSYNC HIGH\n");
    }

#ifdef CONFIG_CAMERA_SELECT_HM01B0
    err = init_hm01b0();
#endif
    if(err != 0)
    {
        Print_ERR("ERR:Init Camera\n");
        return -1;
    }

    *((volatile uint32_t *)PADCTRL_MB_SNS_ADDRESS) &= 0xffffff7f;

    os_TaskSleepms(1);
    chicken_run(1);

    //FIXME: remove after debug
    //----------------- debug ----------------------
#ifdef CAMERA_LOCAL_TEST
    int i = 0;
    camera_ctrl.rx_buffer = (uint32_t *)0x41510000;
    //camera_ctrl.rx_buffer = (uint32_t *)os_Malloc((SEARCH_BUFFER_SIZE + 1) * 4);
    camera_ctrl.buffer_size_dw = 34992;

    uint32_t *ptr;
    //ptr = (uint32_t *)0x41510000;
    //for (i = 0; i < 49200; i++)
    //    ptr[i] = 0x55aa55aa;


    M33_BARRIER();

    err = sync_ws_hsync();
    if (err != 0)
    {
        Print_ERR("ERR:Sync:%d\n", err);
    }
    camera_ctrl.ready = 1;
    camera_ctrl.rx_buffer = (uint32_t *)0x41510000;
    camera_ctrl.callback = user_callback;

    ptr = (uint32_t *)0x41510000;
    
    for(i = 0; i<49200; i++)
        ptr[i] = 0x55aa55aa;

    camera_ctrl.inited = 1;
    camera_capture(camera_ctrl.rx_buffer, camera_ctrl.buffer_size_dw);

#endif

    camera_ctrl.inited = 1;
    //--------------- debug end --------------------
    Print_ERR("Camera Inited\n");
    return 0;
}

#else

int _camera_drv_init()
{
    int err = 0;
    memset(&camera_ctrl, 0, sizeof(camera_ctrl));

    Print_ERR("Camera Driver Start..\n");
    //TODO: init GPIO pinmux
    //Setup GPIO
    gpiomux_select_gpio(GPIO_TRIGGER, IO_MUX_ENABLE, OUT_DIR, DRV_CUR_2mA_FAST, PULL_DISABLED);
    gpio_set(GPIO_TRIGGER, 0);

    //TODO: Enable I2S0 BCLK to provide MCLK
    //Setup I2S0 --- Temp for FPGA, should switch to another pin
    i2s_config_t cfg;
    cfg.tx_rx          = I2S_PCM_TX;
    cfg.sample_rate    = 256000;
    cfg.justified      = I2S_JUSTIFIED_LEFT;
    cfg.dstart_delay   = I2S_DELAY_0_BIT;
    cfg.lrclk_pol      = I2S_LRCLK_POL_LEFT_HIGH;
    cfg.lrck_low_first = I2S_LRCK_FIRST_HIGH;
    i2s_pcm_config_i2s(DEV_I2S0, &cfg);

    i2s_pcm_en_t i2s_cfg;
    i2s_cfg.tx_rx        = I2S_PCM_TX;
    i2s_cfg.sample_width = I2S_PCM_SAMPLE_WIDTH_24_BIT;
    i2s_cfg.framelength  = 48;
    i2s_cfg.channels     = 2;
    i2s_cfg.flag         = I2SPCM_EN_FLAG_WITH_RST_TX | I2SPCM_EN_FLAG_WITHOUT_EN;
    i2s_pcm_enable(DEV_I2S0, (i2s_pcm_en_t *)&i2s_cfg);

    //Enable I2S0
    i2s_cfg.flag = 0;
    i2s_pcm_enable(DEV_I2S0, (i2s_pcm_en_t *)&i2s_cfg);

    *((volatile uint32_t *)PADCTRL_I2S0_BCLK_ADDRESS) = 0x3180;
    *((volatile uint32_t *)PADCTRL_I2S0_TX_DAT_ADDRESS) = 0x3104;
    *((volatile uint32_t *)0x42023478) = 0x00001180;

    //TODO: hack for pinmux
    //#Set GPs for parallel camera inputs 4bit
    //set_reg 0x420234b8 0x00000408; #PADCTRL_MIC1N
    //set_reg 0x42023480 0x00008008; #PADCTRL_I2S0_RX_DAT
    //set_reg 0x420234bc 0x00008028; #PADCTRL_TEMP_SNS
    //set_reg 0x420234c0 0x0000c028; #PADCTRL_MB_SNS
    //set_reg 0x42023448 0x0000C008; #PADCTRL_I2S1_TX_WS
    //set_reg 0x4202344c 0x0000C008; #PADCTRL_I2S1_RX_WS
    //set_reg 0x4202343c 0x0000C008; #PADCTRL_I2S1_BCLK
    //set_reg 0x42023440 0x0000c008; #PADCTRL_I2S1_TX_DAT
    //set_reg 0x42023444 0x0000C008; #PADCTRL_I2S1_RX_DAT
#if defined(CONFIG_CAMERA_PARALLEL_WIDTH_8)
    *((volatile uint32_t *)0x42023430) = 0x00000200;    //#GPIO-4 as JTAG
#endif
    *((volatile uint32_t *)0x420234b8) = 0x408;         //PADCTRL_MIC1N
    *((volatile uint32_t *)0x42023480) = 0x00008008;    //PADCTRL_I2S0_RX_DAT
    *((volatile uint32_t *)0x420234bc) = 0x00008028;    //PADCTRL_TEMP_SNS
    *((volatile uint32_t *)0x420234c0) = 0xC228;        //PADCTRL_MB_SNS//Ted:0x0000c028;
    *((volatile uint32_t *)0x42023448) = 0x0000C008;    //PADCTRL_I2S1_TX_WS
    *((volatile uint32_t *)0x4202344c) = 0x0000C008;    //PADCTRL_I2S1_RX_WS
    *((volatile uint32_t *)0x4202343c) = 0x0000C008;    //PADCTRL_I2S1_BCLK
    *((volatile uint32_t *)0x42023440) = 0x0000C008;    //PADCTRL_I2S1_TX_DAT
    *((volatile uint32_t *)0x42023444) = 0x0000C008;    //PADCTRL_I2S1_RX_DAT
#if defined(CONFIG_CAMERA_PARALLEL_WIDTH_8)
    *((volatile uint32_t *)0x42023420) = 0x0000c008;    //PADCTRL_GPIO0 D4
    *((volatile uint32_t *)0x42023424) = 0x00014008;    //PADCTRL_GPIO0 D5
    *((volatile uint32_t *)0x42023428) = 0x0000c008;    //PADCTRL_GPIO0 D6
    *((volatile uint32_t *)0x4202342C) = 0x00014008;    //PADCTRL_GPIO0 D7
#endif

    //TODO: Enable clock
    volatile CR_CLK_ISP_s *isp_clk = (volatile CR_CLK_ISP_s *)CR_CLK_ISP_ADDRESS;
    isp_clk->dw                   = 0x0;
    isp_clk->bf.isp_clk_div       = 0;
    isp_clk->bf.isp_clk_div_chng  = 1;
    isp_clk->bf.isp_rx_ahb_clk_en = 1;
    isp_clk->bf.isp_rx_clk_en     = 1;
    isp_clk->bf.isp_mst_clk_en    = 1;

    //TODO: init sensor
    //TODO: how to read vsync status??

#ifdef CONFIG_CAMERA_SELECT_HM01B0
    err = init_hm01b0();
#endif
    if (err != 0)
    {
        Print_ERR("ERR:Init Camera\n");
        return -1;
    }

    Print_ERR("ISP CLK=0x%x\n", isp_clk->dw);
    //Reset ISP
    *((volatile uint32_t *)0x42007808) = 0xC000;
    *((volatile uint32_t *)0x42007808) = 0x0000;

    //TODO: init ISP
    volatile MISC_s *isp_reg = (volatile MISC_s *)MISC_s_OFFSET;
#if defined(CONFIG_CAMERA_PARALLEL_WIDTH_1)
    //1 Bit Parallel
    isp_reg->isp_ctrl0.bf.isp_hend       = 0xA20;
    isp_reg->isp_ctrl0.bf.isp_hstart     = 1;
    isp_reg->isp_ctrl1.bf.isp_vend       = 0x144;
    isp_reg->isp_ctrl1.bf.isp_vstart     = 1;
    isp_reg->isp_ctrl2.dw                = 0x0;
    isp_reg->isp_ctrl2.bf.isp_byte_order = 1;
    isp_reg->isp_ctrl2.bf.isp_mode       = 0x0;
    isp_reg->isp_ctrl2.bf.isp_bitwidth   = 0;
#elif defined(CONFIG_CAMERA_PARALLEL_WIDTH_2)
    #error Not support 2-Bits mode for now
#elif defined(CONFIG_CAMERA_PARALLEL_WIDTH_4)
    //4 Bit Parallel
    isp_reg->isp_ctrl0.bf.isp_hend       = 0x288;
    isp_reg->isp_ctrl0.bf.isp_hstart     = 1;
    isp_reg->isp_ctrl1.bf.isp_vend       = 0x144;
    isp_reg->isp_ctrl1.bf.isp_vstart     = 1;
    isp_reg->isp_ctrl2.dw                = 0x0;
    isp_reg->isp_ctrl2.bf.isp_byte_order = 1;
    isp_reg->isp_ctrl2.bf.isp_mode       = 0x0;
    isp_reg->isp_ctrl2.bf.isp_bitwidth   = 0x2;
#elif defined(CONFIG_CAMERA_PARALLEL_WIDTH_8)
    isp_reg->isp_ctrl0.bf.isp_hend       = 0x144;
    isp_reg->isp_ctrl0.bf.isp_hstart     = 1;
    isp_reg->isp_ctrl1.bf.isp_vend       = 0x144;
    isp_reg->isp_ctrl1.bf.isp_vstart     = 1;
    isp_reg->isp_ctrl2.dw                = 0x0;
    isp_reg->isp_ctrl2.bf.isp_byte_order = 1;
    isp_reg->isp_ctrl2.bf.isp_mode       = 0x0;
    isp_reg->isp_ctrl2.bf.isp_bitwidth   = 0x3;
#endif
    isp_reg->isp_ctrl2.bf.isp_or_i2s     = 1;
    isp_reg->isp_ctrl2.bf.isp_enable     = 1;

    //Print_ERR("CTRL0=0x%x\n", isp_reg->isp_ctrl0.dw);
    //Print_ERR("CTRL1=0x%x\n", isp_reg->isp_ctrl1.dw);
    //Print_ERR("CTRL2=0x%x\n", isp_reg->isp_ctrl2.dw);
    //TODO: enable test
    //Setup ADMA
    struct adma_callback cb;
    cb.callback = adma_rx_cb;
    cb.data     = NULL;
    adma_set_irq_cb(1, dma_rx_dev_list, &cb, 1);

    os_SemInit SemInit;
    SemInit.uSemType = SEM_BINARY;
    camera_ctrl.wait_dma = os_SemCreate(&SemInit);

    os_TaskSleepms(1);
    chicken_run(1);

#ifdef CAMERA_LOCAL_TEST
    int i = 0;
    camera_ctrl.rx_buffer = (uint32_t *)0x41510000;
    //camera_ctrl.rx_buffer = (uint32_t *)os_Malloc((SEARCH_BUFFER_SIZE + 1) * 4);
    camera_ctrl.buffer_size_dw = 26244;

    uint32_t *ptr;
    //ptr = (uint32_t *)0x41510000;
    //for (i = 0; i < 49200; i++)
    //    ptr[i] = 0x55aa55aa;

    M33_BARRIER();

    camera_ctrl.ready     = 1;
    camera_ctrl.rx_buffer = (uint32_t *)0x41510000;
    camera_ctrl.callback  =  user_callback;

    ptr = (uint32_t *)0x41510000;

    for (i = 0; i < 49200; i++)
        ptr[i] = 0x55aa55aa;

    camera_ctrl.inited = 1;
    _camera_capture();
    //trigger_dma((uint32_t)camera_ctrl.rx_buffer, camera_ctrl.buffer_size_dw);
    //Print_ERR("S9\n");
    //os_SemTake(camera_ctrl.wait_dma, OS_SEM_WAIT_FOREVER);
#endif

    camera_ctrl.inited = 1;
    //--------------- debug end --------------------
    Print_ERR("Camera Inited\n");
    return err;
}

#endif

int _camera_hw_write(uint16_t addr, uint8_t data)
{
    camera_set_reg(addr, data);
    return 0;
}

int _camera_hw_read(uint16_t addr, uint8_t *data)
{
    *data = camera_get_reg(addr);
    return 0;
}

int _camera_init(void) {return 0;}
int _camera_open(uint32_t dev, uint32_t flags) { return 0; }
int _camera_close(uint32_t dev) { return 0; }
int _camera_ioctl(uint32_t dev, uint32_t cmd, void *param) { return 0; }

static struct camera_ops_s camera_ops = {
    DEFINE_STANDARD_DEVICE_API(_camera),
    _camera_sample_check,
    _print_image,
    _camera_setup,
    _camera_drv_init,
    _camera_capture,
    _camera_reset,
    _camera_hw_write,
    _camera_hw_read
};

//
//  Register the driver.
//
int drv_camera_start(void)
{
    int ret;
    ret = os_register_driver(DEVICE0(CAMERA), (struct drvr_s *)&camera_ops);
    if (ret != 0)
        Print_ERR("ERR: %s()\n", __func__);

    return (ret);
}

#endif //CONFIG_MCU_DRV_CAMERA
