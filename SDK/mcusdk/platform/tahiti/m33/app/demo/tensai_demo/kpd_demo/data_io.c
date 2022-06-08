#define PRINT_LEVEL_SET ASSERT_LEVEL_DBG
#include "stdint.h"
#include "config.h"
#include "userconf.h"
#include "usr_demo.h"
#include "mcu.h"
#include "workQ_common.h"
#include "executor_types.h"
#include "executor_config.h"
#include "executor_common.h"
#include "executor_public.h"
#include "mem_alloc_al.h"
#include "eta_m33_nnfunctions.h"
#include "input_data.h"
#include "fileSystem.h"
#ifdef CONFIG_MCU_SDK_CAMERA_ENABLE
#include "img_process.h"
#endif
#include "camera_drv.h"
#if (defined(MCU_SDK_KPD_DEMO) && (MCU_SDK_KPD_DEMO == 1))
#include "kpd.h"
typedef struct kpd_demo_t
{
    char demo_name[4];
    int pre_init;
    int post_init;
    float *scores_out;
    BoxCornerFloat *boxes_out;
    KPDPreShareInfo *pre_share_info;
    PostShareInfo *post_share_info;
    uint16_t nn_image_row;
    uint16_t nn_image_col;
} kpd_demo;
static kpd_demo kpd = {'K', 'P', 'D', ' ', 0, 0, NULL, NULL, NULL, NULL, 0, 0};

int kpd_post_processing_init(int process_id, int8_t *out_buf_0, int8_t *out_buf_1)
{
    int ret = 0;
    float scales = 0.08325857669115067;
    float scaleb = 0.036149077117443085;
    int8_t shifts = 64;
    int8_t shiftb = 0;

    kpd.boxes_out = (BoxCornerFloat *)os_MallocAlign(sizeof(BoxCornerFloat) * 10, 8);
    kpd.scores_out = (float *)os_MallocAlign(sizeof(float) * 10, 8);
    if ((kpd.boxes_out == NULL) || (kpd.scores_out == NULL))
        return -ENOMEM;
    kpd.post_share_info = (PostShareInfo *)katana_post_process_init(process_id);
    if (kpd.post_share_info == NULL)
        return -EFAULT;
    ret = katana_post_process_nn_quantization_par_set(process_id, scales, scaleb, shifts, shiftb);
    ret = katana_post_process_training_scale_shift_set(process_id, 0.2, 0.0);
    ret = katana_post_process_height_width_set(process_id, kpd.nn_image_row, kpd.nn_image_col); //KPD model V2_alpha/V2_0 need 80*160, before V2 needs 160*320
    ret = katana_post_process_buf_set(process_id, 1 /*buf in2post*/, (int32_t)out_buf_0 /*scores*/, (int32_t)out_buf_1 /*boxes*/, (int32_t)NULL);
    ret = katana_post_process_buf_set(process_id, 0 /*buf post2out*/, (int32_t)kpd.scores_out /*scores*/, (int32_t)kpd.boxes_out /*boxes*/, (int32_t)NULL);

    return ret;
}

void kpd_pre_processing(void *data)
{
    kpd.pre_share_info->pre_updated = 0;
    katana_pre_process_send_tick_to_HIFI();
    while (1)
    {
        if (kpd.pre_share_info->pre_updated)
        {
            Print_DBG("Pre process Done\n");
            kpd.pre_share_info->pre_updated = 0;
            break;
        }
        os_TaskSleep(1);
    }
}

void kpd_pre_processing_setup(void *data)
{
    int resize_enable = 1;
    preProcessParams_t *preProcessParams = (preProcessParams_t *)data;

    kpd.pre_share_info = (KPDPreShareInfo *)katana_pre_process_init(resize_enable);
    katana_pre_process_buf_set(1 /*buf in pre*/, (int32_t)((uint32_t)preProcessParams->src + IMAGE_HEADER_SIZE));
    katana_pre_process_buf_set(0 /*buf out pre*/, (int32_t)preProcessParams->dst);
    katana_pre_process_pic_in_row_clo(PADDED_ROW_SIZE, PADDED_COL_SIZE);
    if (resize_enable)
    { //v2.0 MODEL
        katana_pre_process_crop_dst_row_clo(160, 320);
        katana_pre_process_resize_row_clo(kpd.nn_image_row, kpd.nn_image_col);
    }
    else
    {
        katana_pre_process_crop_dst_row_clo(kpd.nn_image_row, kpd.nn_image_col);
    }
    katana_pre_process_crop_mode_set(CROP_LEFT_BOTTOM_JUSTIFIED);
}

void kpd_pre_processing_init(uint16_t nn_image_row_t, uint16_t nn_image_col_t)
{
    kpd.nn_image_row = nn_image_row_t;
    kpd.nn_image_col = nn_image_col_t;
    setImagePreProcessingSetup(&kpd_pre_processing_setup);
    setImagePreProcessing(&kpd_pre_processing);
}

void get_data(int8_t *in_buf_0, uint16_t in_buf_0_dim_0, uint16_t in_buf_0_dim_1, uint16_t in_buf_0_dim_2)
{

#ifdef CONFIG_MCU_SDK_CAMERA_ENABLE
    if (!kpd.pre_init)
    {
        kpd_pre_processing_init(in_buf_0_dim_0, in_buf_0_dim_1);
        kpd.pre_init = 1;
    }
    captureAndProcessImage(in_buf_0, in_buf_0_dim_0, in_buf_0_dim_1);
#else
    readFromFileSystem(FILE_ID(pIn), in_buf_0, ALIGN_DOWN_TO_4(IN_SIZE));
#endif
}

void post_process(int8_t *out_buf_0, int8_t *out_buf_1)
{
    int diff, maxDiff, nDiff;
    int sumDiff;
    maxDiff = 0;
    nDiff = 0;
    sumDiff = 0;
    int ret = 0;
    int process_id = KATANA_KPD_POST_PROCESS;
#ifdef CONFIG_MCU_SDK_CAMERA_ENABLE //TD: Need change to post process related config
    if (!kpd.post_init)
    {
        ret = kpd_post_processing_init(process_id, out_buf_0, out_buf_1);
        if (ret < 0)
            Print_ERR("KPD post process init fail\n");
        kpd.post_init = 1;
    }
    //Send Tick to HIFI
    katana_post_process_send_tick_to_HIFI(process_id, OUT1_SIZE);
    //check HIFI process status
    //Print_DBG("Pshare 0x%x 0x%x\n", kpd.post_share_info, kpd.post_share_info->updated);
    while (1)
    {
        if (kpd.post_share_info->updated)
        {
            Print_DBG("Get %s counted people number: %d\n", kpd.demo_name, kpd.post_share_info->rt_box_num);
            Print_DBG("Scores:\n");
            for (int i = 0; i < kpd.post_share_info->rt_box_num; i++)
                Print_DBG("%f ", *(kpd.scores_out + i));
            Print_DBG("\nBoxes:\n");
            for (int i = 0; i < kpd.post_share_info->rt_box_num; i++)
                Print_DBG("[%3.8f %3.8f %3.8f %3.8f]\n", (kpd.boxes_out + i)->x1, (kpd.boxes_out + i)->y1, (kpd.boxes_out + i)->x2, (kpd.boxes_out + i)->y2);
            kpd.post_share_info->updated = 0;
            break;
        }
        os_TaskSleep(1);
    }
#else
    for (int32_t i = 0; i < OUT1_SIZE - 1; ++i)
    {
        //    Print_DBG(" output[%d] = %d", i, out_buf_0[i]);
        Print_DBG("%d, ", out_buf_0[i]);
    }
    Print_DBG("%d ", out_buf_0[OUT1_SIZE - 1]);

    Print_DBG("\r\n}\r\n\r\n//----Output 1x5x10x36 ----\r\n{\r\n");

    for (int32_t i = 0; i < OUT2_SIZE - 1; ++i)
    {
        //    Print_DBG("%d, ", out_buf_0[OUT1_SIZE+i]);
        Print_DBG("%d, ", out_buf_1[i]);
    }
    Print_DBG("%d ", out_buf_1[OUT2_SIZE - 1]);
    Print_DBG("\r\n}\r\n\r\n");

#ifdef SHOWEXPECT
    int8_t *pExpect = allocMem(SRAM, OUT_SIZE);
    readFromFileSystem(FILE_ID(pExpect), in_buf_0, ALIGN_DOWN_TO_4(IN_SIZE));
    for (int32_t i = 0; i < OUT_SIZE; ++i)
    {
        Print_DBG("output[%d] = %d\r\n", i, out_buf_0[i]);
        diff = out_buf_0[i] - pExpect[i];
        if (diff)
        {
            nDiff++;
            diff = diff >= 0 ? diff : -diff;
            sumDiff += diff;
            if (diff > maxDiff)
                maxDiff = diff;
        }
    }

    Print_DBG("Max Diff: %d, avg Diff: %d, nDiff = %d/%d\r\n",
              maxDiff, sumDiff / nDiff, nDiff, OUT_SIZE);
#endif

#endif //CONFIG_MCU_SDK_CAMERA_ENABLE
}

#endif //#if (defined(MCU_SDK_KPD_DEMO) && (MCU_SDK_KPD_DEMO == 1))
