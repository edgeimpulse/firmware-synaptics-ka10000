#define PRINT_LEVEL_SET ASSERT_LEVEL_DBG
#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "userconf.h"
#include "usr_demo.h"
#ifdef CONFIG_MCU_SDK_CAMERA_ENABLE
#include "img_process.h"
#include "config.h"
#include "camera_drv.h"
#include "mem_alloc_al.h"
#include "kpd.h"

os_SemHandle cam_notify;
uint8_t printImage_enable = 0;
preProcessParams_t preProcessParams;
image_pre_processing_t image_pre_processing = NULL;
image_pre_processing_t image_pre_processing_setup = NULL;
void camera_capture_callback(void *msg)
{
    Print_DBG("Respsonse recieved\n");
    os_SemGive(cam_notify);
}

int printImageEnable(uint32_t *enable, uint8_t flag)
{
    if (flag)
        printImage_enable = *enable;
    else
    {
        *enable = printImage_enable;
        return 1;
    }
    return 0;
}

void printImage(uint8_t *imagePtr, uint16_t row, uint16_t col)
{
    // Print_DBG("---------------------------\n");
    Print_DBG("[\n");
    int8_t *ptr = imagePtr;
    for (uint16_t i = 0; i < row; i++)
    {
        for (uint16_t j = 0; j < col; j++)
            Print_DBG("%d,", *(ptr++));
        Print_DBG("\n\r");
    }
    // Print_DBG("---------------------------\n");
    Print_DBG("]");
    Print_DBG("\n");
}

void setImagePreProcessingSetup(image_pre_processing_t preProcessFunc)
{
    image_pre_processing_setup = preProcessFunc;
}

void setImagePreProcessing(image_pre_processing_t preProcessFunc)
{
    image_pre_processing = preProcessFunc;
}

void captureAndProcessImage(int8_t *in_buf_0, uint16_t nn_image_row, uint16_t nn_image_col)
{
    static bool cameraInited = false;
#ifdef CONFIG_MCU_SDK_CAMERA_ENABLE
    int8_t *buffer = preAllocMem(SHM, PADDED_IMAGE_SIZE);
#else
    int8_t *buffer = allocMem(SHM, PADDED_IMAGE_SIZE);
#endif
    if (cameraInited == false)
    {
        camera_setup_t cfg;
        int ret = 0;
        os_SemInit SemInit;
        SemInit.uSemType = SEM_BINARY;
        cam_notify = os_SemCreate(&SemInit);

        ret = camera_drv_init();
        cfg.buffer = (uint32_t *)buffer;
        cfg.buffer_size = PADDED_IMAGE_SIZE_IN_WORDS; //To make sure hold up 324*324
        cfg.done_cb = camera_capture_callback;
        camera_setup(&cfg);
        cameraInited = true;
        Print_DBG("Camera Initialized src 0x%x dst 0x%x\n\r", buffer, in_buf_0);
        preProcessParams.src = (uint8_t *)buffer;
        preProcessParams.dst = (uint8_t *)in_buf_0;
        if (image_pre_processing_setup)
            (*image_pre_processing_setup)((void *)&preProcessParams);
    }

    camera_capture(buffer, PADDED_IMAGE_SIZE_IN_WORDS);
    os_SemTake(cam_notify, OS_SEM_WAIT_FOREVER);
    if (printImage_enable)
        print_image((uint32_t *)buffer, PADDED_IMAGE_SIZE_IN_WORDS);
    if (image_pre_processing)
        (*image_pre_processing)(NULL);

#ifdef CONFIG_MCU_SDK_CAMERA_ENABLE
    preAllocFreeMem(buffer);
#else
    freeMem(buffer);
#endif
    Print_DBG("Image capture done\n\r");
}
#endif
