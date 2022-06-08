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
#ifdef CONFIG_MCU_DRV_BLUETOOTH
#include "bluetooth_drv.h"
#endif
#if (defined(MCU_SDK_KMR_DEMO) && (MCU_SDK_KMR_DEMO == 1))
#include "kpd.h"
typedef struct kmr_demo_t
{
    char demo_name[4];
    int post_init;
    int pre_init;
    BoxCornerFloat *boxes_out;
    uint8_t *label_out;
    float *scores_out;
    PostShareInfo *post_share_info;
    KPDPreShareInfo *pre_share_info;
    uint16_t nn_image_row;
    uint16_t nn_image_col;
} kmr_demo;
static kmr_demo kmr = {'K', 'M', 'R', ' ', 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0};

int kmr_post_processing_init(int process_id, int8_t *out_buf_0, int8_t *out_buf_1)
{
    int ret = 0;
    /*(shifts, shiftb) (scaleb, scales)
     *KMR V0p2:
     *The boxes and scores quantization shifts are (7, 77)
     *The boxes and scores quantization scales are (0.03166044130921364, 0.1000121682882309)
     *V0p3:
     *The boxes and scores quantization shifts are (-6, 94)
     *The boxes and scores quantization scales are (0.028298066928982735, 0.07950646430253983)
     */
    float scales = 0.07950646430253983;
    float scaleb = 0.028298066928982735;
    int8_t shifts = 94;
    int8_t shiftb = -6;

    kmr.boxes_out = (BoxCornerFloat *)os_MallocAlign(sizeof(BoxCornerFloat) * 16, 8);
    kmr.label_out = (uint8_t *)os_MallocAlign(sizeof(uint8_t) * 16, 8);
    kmr.scores_out = (float *)os_MallocAlign(sizeof(float) * 16, 8);
    if ((kmr.boxes_out == NULL) || (kmr.label_out == NULL) || (kmr.scores_out == NULL)) {
        Print_ERR("KMR output buffer mallco fail\n");
        return -ENOMEM;
    }
    Print_INFO("score 0x%x box 0x%x label 0x%x\n", kmr.scores_out, kmr.boxes_out, kmr.label_out);
    kmr.post_share_info = (PostShareInfo *)katana_post_process_init(process_id);
    if (kmr.post_share_info == NULL)
        return -EFAULT;

    ret = katana_post_process_nn_quantization_par_set(process_id, scales, scaleb, shifts, shiftb);
    ret = katana_post_process_training_scale_shift_set(process_id, 0.2, 0.0);
    ret = katana_post_process_num_digtits_set(process_id, 6);
    ret = katana_post_process_height_width_set(process_id, kmr.nn_image_row, kmr.nn_image_col); //KMR model V0p2 need 240*320
    ret = katana_post_process_iou_threshold_par_set(process_id, 0.3);
    ret = katana_post_process_detect_threshold_par_set(process_id, -0.85);
    ret = katana_post_process_buf_set(process_id, 1 /*buf in2post*/, (int32_t)out_buf_0 /*scores*/, (int32_t)out_buf_1 /*boxes*/, (int32_t)NULL);
    ret = katana_post_process_buf_set(process_id, 0 /*buf post2out*/, (int32_t)kmr.scores_out /*scores*/, (int32_t)kmr.boxes_out /*boxes*/, (int32_t)kmr.label_out);

    return ret;
}

void kmr_pre_processing(void *data)
{
    kmr.pre_share_info->pre_updated = 0;
    katana_pre_process_send_tick_to_HIFI();
    while (1)
    {
        if (kmr.pre_share_info->pre_updated)
        {
            Print_DBG("Pre process Done\n");
            kmr.pre_share_info->pre_updated = 0;
            break;
        }
        os_TaskSleep(1);
    }
}

void kmr_pre_processing_setup(void *data)
{
    int resize_enable = 0;
    preProcessParams_t *preProcessParams = (preProcessParams_t *)data;

    kmr.pre_share_info = (KPDPreShareInfo *)katana_pre_process_init(resize_enable);
    katana_pre_process_buf_set(1 /*buf in pre*/, (int32_t)((uint32_t)preProcessParams->src + IMAGE_HEADER_SIZE));
    katana_pre_process_buf_set(0 /*buf out pre*/, (int32_t)preProcessParams->dst);
    katana_pre_process_pic_in_row_clo(PADDED_ROW_SIZE, PADDED_COL_SIZE);
    if (resize_enable)
    { //v2.0 MODEL
        katana_pre_process_crop_dst_row_clo(240, 320);
        katana_pre_process_resize_row_clo(kmr.nn_image_row, kmr.nn_image_col);
    }
    else
    {
        katana_pre_process_crop_dst_row_clo(kmr.nn_image_row, kmr.nn_image_col);
    }
    katana_pre_process_crop_mode_set(CROP_LEFT_TOP_JUSTIFIED);
}

void kmr_pre_processing_init(uint16_t nn_image_row_t, uint16_t nn_image_col_t)
{
    kmr.nn_image_row = nn_image_row_t;
    kmr.nn_image_col = nn_image_col_t;
    setImagePreProcessingSetup(&kmr_pre_processing_setup);
    setImagePreProcessing(&kmr_pre_processing);
}

void kmr_result_post_to_uart(void)
{
    Print_DBG("Get %s counted label number: %d\n", kmr.demo_name, kmr.post_share_info->rt_box_num);
    Print_DBG("\nLabel:\n");
    for (int i = 0; i < kmr.post_share_info->rt_box_num; i++)
        Print_DBG("%d ", *(kmr.label_out + i));
    Print_DBG("\nScore:\n");
    for (int i = 0; i < kmr.post_share_info->rt_box_num; i++)
        Print_DBG("%f ", *(kmr.scores_out + i));
    Print_DBG("\nBoxes:\n");
    for (int i = 0; i < kmr.post_share_info->rt_box_num; i++)
        Print_DBG("[%3.8f %3.8f %3.8f %3.8f]\n", (kmr.boxes_out + i)->x1, (kmr.boxes_out + i)->y1, (kmr.boxes_out + i)->x2, (kmr.boxes_out + i)->y2);
}

#ifdef CONFIG_MCU_DRV_BLUETOOTH
void kmr_result_post_to_bt(void)
{
    uint8_t *frame_data;
    int num_data = 0;
    int len = 0;
    bt_app_frame_t frame;
    bt_app_frame_t *pframe;
    pframe = &frame;

    frame.type = BT_APP_MSG_TYPE_DEMO;
    pframe->reserved = 0;
    //frame data: demo_name/box_num/label
    //demo name
    frame_data = (uint8_t *)&kmr.demo_name;
    for (int i = 0; i < sizeof(kmr.demo_name); i++, num_data++)
        pframe->data[num_data] = *(frame_data + i);
    //box num
    frame_data = (uint8_t *)&(kmr.post_share_info->rt_box_num);
    for (int i = 0; i < sizeof(kmr.post_share_info->rt_box_num); i++, num_data++)
        pframe->data[num_data] = *(frame_data + i);
    //label
    frame_data = kmr.label_out;
    for (int i = 0; i < (kmr.post_share_info->rt_box_num * sizeof(uint8_t)); i++, num_data++)
        pframe->data[num_data] = *(frame_data + i);
    pframe->data_len = num_data;
    len = BT_APP_FRAME_FULL_SIZE(pframe);
    bt_sendmsg(BT_MSG_CH_APP, BT_MSG_TYPE_INDICATE, (uint8_t *)pframe, len);
}
#endif

void get_data(int8_t *in_buf_0, uint16_t in_buf_0_dim_0, uint16_t in_buf_0_dim_1, uint16_t in_buf_0_dim_2)
{

#ifdef CONFIG_MCU_SDK_CAMERA_ENABLE
    if (!kmr.pre_init)
    {
        kmr_pre_processing_init(in_buf_0_dim_0, in_buf_0_dim_1);
        kmr.pre_init = 1;
    }
    captureAndProcessImage(in_buf_0, in_buf_0_dim_0, in_buf_0_dim_1);
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
    int process_id = KATANA_KMR_POST_PROCESS;

#ifdef CONFIG_MCU_SDK_CAMERA_ENABLE //TD: Need change to post process related config
    if (!kmr.post_init)
    {
        ret = kmr_post_processing_init(process_id, out_buf_0, out_buf_1);
        if (ret < 0)
            Print_ERR("KMR post process init fail\n");
        kmr.post_init = 1;
    }
    //Send Tick to HIFI
    ret = katana_post_process_send_tick_to_HIFI(process_id, CONFIG_KMR_BOX_NUM);
    if (ret < 0)
        Print_ERR("KMR send tick fail\n");
    //check HIFI process status
    //Print_DBG("Pshare 0x%x 0x%x\n", kmr.post_share_info, kmr.post_share_info->updated);
    while (1)
    {
        if (kmr.post_share_info->updated)
        {
#ifdef CONFIG_MCU_DRV_BLUETOOTH
            kmr_result_post_to_bt();
#else
            kmr_result_post_to_uart();
#endif
            kmr.post_share_info->updated = 0;
            break;
        }
        os_TaskSleep(1);
    }

#endif //CONFIG_MCU_SDK_CAMERA_ENABLE
}

#endif //#if (defined(MCU_SDK_KMR_DEMO) && (MCU_SDK_KMR_DEMO == 1))
