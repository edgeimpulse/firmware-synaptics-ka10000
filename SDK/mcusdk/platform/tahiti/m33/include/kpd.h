#ifndef _USR_KPD_H_
#define _USR_KPD_H_

#include "mcu.h"
#include "img_process.h"
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef CONFIG_MCU_KPD_PROCESS
//*********************************Post Process****************************************//
typedef struct BoxCornerFloatTag {
    float x1;
    float y1;
    float x2;
    float y2;
} BoxCornerFloat;

typedef struct PostShareInfoTag {
    int rt_box_num;
    int updated;
} PostShareInfo;

typedef enum
{
    KATANA_KPD_POST_PROCESS = 0,
    KATANA_PRE_PROCESS,
    KATANA_KMR_POST_PROCESS,
    KATANA_PROCESS_LAST,
} KPD_PROCESS_ID;

typedef struct PostProcessAnchorsTag {
    KPD_PROCESS_ID post_process_id;
    uint32_t anchors_file_id;
    uint32_t box_number;
} PostProcessAnchors;

int katana_post_process_init(int post_process_id);
int katana_post_process_share_info_set(int post_process_id, int kpd_share_info);
int katana_post_process_buf_set(int post_process_id, int dir, int scores, int boxes, int lable);
int katana_post_process_nn_quantization_par_set(int post_process_id, float scales, float scaleb, int8_t shifts, int8_t shiftb);
int katana_post_process_detect_threshold_par_set(int post_process_id, float detect_threshold);
int katana_post_process_iou_threshold_par_set(int post_process_id, float iou_threshold);
int katana_post_process_training_scale_shift_set(int post_process_id, float training_scale, float training_shift);
int katana_post_process_height_width_set(int post_process_id, int height, int width);
int katana_post_process_num_digtits_set(int post_process_id, int num_digtits);
int katana_post_process_accepted_misalignment_set(int post_process_id, float accepted_misalignment);
int katana_post_process_send_tick_to_HIFI(int post_process_id, int box_num);

//*********************************Pre Process****************************************//
typedef struct KPDPreShareInfoTag {
    int pre_updated;
} KPDPreShareInfo;

int katana_pre_process_init(uint reszie_enable);
int katana_pre_process_buf_set(int dir, int buff_addr);
int katana_pre_process_send_tick_to_HIFI(void);
int katana_pre_process_pic_in_row_clo(int row, int col);
int katana_pre_process_resize_row_clo(int row, int col);
int katana_pre_process_crop_dst_row_clo(int row, int col);
int katana_pre_process_crop_mode_set(int crop_mode);
#endif //CONFIG_MCU_KPD_PROCESS

#ifdef __cplusplus
}
#endif

#endif // _USR_KPD_H_
