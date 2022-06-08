#ifndef __IMG_PROCESS_H
#define __IMG_PROCESS_H

#include "mcu.h"

#define IMAGE_HEADER_SIZE 8
#define PADDED_IMAGE_SIZE ((CONFIG_CAMERA_ROW_COUNT * CONFIG_CAMERA_COL_COUNT * 4) / 3)
#define PADDED_IMAGE_SIZE_IN_WORDS (PADDED_IMAGE_SIZE / 4)
#define PADDED_ROW_SIZE CONFIG_CAMERA_ROW_COUNT
#define PADDED_COL_SIZE (CONFIG_CAMERA_COL_COUNT * 4) / 3

typedef enum
{
    CROP_LEFT_TOP_JUSTIFIED,
    CROP_LEFT_CENTRE_JUSTIFIED,
    CROP_LEFT_BOTTOM_JUSTIFIED,

    CROP_MID_TOP_JUSTIFIED,
    CROP_MID_CENTRE_JUSTIFIED,
    CROP_MID_BOTTOM_JUSTIFIED,

    CROP_RIGHT_TOP_JUSTIFIED,
    CROP_RIGHT_CENTRE_JUSTIFIED,
    CROP_RIGHT_BOTTOM_JUSTIFIED
} cropMode_t;

typedef struct
{
    uint8_t *src;
    uint8_t *dst;
} preProcessParams_t;

typedef void (*image_pre_processing_t)(void *data);

int printImageEnable(uint32_t *enable, uint8_t flag);
void printImage(uint8_t *imagePtr, uint16_t row, uint16_t col);
void captureAndProcessImage(int8_t *in_buf_0, uint16_t nn_image_row_size, uint16_t nn_image_col_size);
void setImagePreProcessingSetup(image_pre_processing_t preProcessFunc);
void setImagePreProcessing(image_pre_processing_t preProcessFunc);
#endif
