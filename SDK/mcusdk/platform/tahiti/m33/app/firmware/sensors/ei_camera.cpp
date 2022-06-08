/* Edge Impulse ingestion SDK
 * Copyright (c) 2020 EdgeImpulse Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define PRINT_LEVEL_SET ASSERT_LEVEL_DBG

#include "config.h"
#include "ei_camera.h"
#include "edge-impulse-sdk/dsp/image/image.hpp"
#include "firmware-sdk/at_base64_lib.h"
#include "camera_drv.h"
#include "mcu.h"
extern "C" {
#include "mem_alloc_al.h"
}

#define PADDED_IMAGE_SIZE (EI_CAMERA_RAW_FRAME_BUFFER_ROWS * EI_CAMERA_RAW_FRAME_BUFFER_COLS * 4 / 3)
#define PADDED_IMAGE_SIZE_IN_WORDS (PADDED_IMAGE_SIZE / 4)

os_SemHandle    cam_notify;
os_TaskHandle   cam_task_handle;

camera_setup_t cfg;

void user_callback(void *msg)
{
    //Print_ERR("Captured Image!!0x%x\n", (uint32_t)msg);
    //print_image(msg, 34992);
    os_SemGive(cam_notify);
}

extern int base64_encode(const char *input, size_t input_size, char *output, size_t output_size);

#define DWORD_ALIGN_PTR(a)   ((a & 0x3) ?(((uintptr_t)a + 0x4) & ~(uintptr_t)0x3) : a)

// static CameraClass cam;
static bool is_initialised = false;

/*
** @brief used to store the raw frame
*/
static uint8_t *ei_camera_frame_buffer;

/*
** @brief points to the output of the capture
*/
uint8_t *ei_camera_capture_out = NULL;


static bool prepare_snapshot(size_t width, size_t height, bool use_max_baudrate);
static bool take_snapshot(size_t width, size_t height, bool print_oks);
static void finish_snapshot();

/**
 * @brief      Convert monochrome data to rgb values
 *
 * @param[in]  mono_data  The mono data
 * @param      r          red pixel value
 * @param      g          green pixel value
 * @param      b          blue pixel value
 */
static inline void mono_to_rgb(uint8_t mono_data, uint8_t *r, uint8_t *g, uint8_t *b)
{
    uint8_t v = mono_data;
    *r = *g = *b = v;
}

/**
 * @brief      Determine whether to resize and to which dimension
 *
 * @param[in]  out_width     width of output image
 * @param[in]  out_height    height of output image
 * @param[out] resize_col_sz       pointer to frame buffer's column/width value
 * @param[out] resize_row_sz       pointer to frame buffer's rows/height value
 * @param[out] do_resize     returns whether to resize (or not)
 *
 */
static int calculate_resize_dimensions(uint32_t out_width, uint32_t out_height, uint32_t *resize_col_sz, uint32_t *resize_row_sz, bool *do_resize)
{
    const ei_device_resize_resolutions_t *list;
    size_t list_size;

    int dl = EiDevice.get_resize_list((const ei_device_resize_resolutions_t **)&list, &list_size);
    if (dl) { /* apparently false is OK here?! */
        ei_printf("ERR: Device has no image resize feature\n");
        return 1;
    }

    // (default) conditions
    *resize_col_sz = EI_CAMERA_RAW_FRAME_BUFFER_COLS;
    *resize_row_sz = EI_CAMERA_RAW_FRAME_BUFFER_ROWS;
    *do_resize = false;

    for (size_t ix = 0; ix < list_size; ix++) {
        if ((out_width <= list[ix].width) && (out_height <= list[ix].height)) {
            *resize_col_sz = list[ix].width;
            *resize_row_sz = list[ix].height;
            *do_resize = true;
            break;
        }
    }

    return 0;
}

/**
 * @brief   Setup image sensor & start streaming
 *
 * @retval  false if initialisation failed
 */
bool ei_camera_init(void)
{
    if (is_initialised)
        return true;

    camera_drv_init();

    cfg.buffer      = (uint32_t *)allocMem(SHM, PADDED_IMAGE_SIZE);
    cfg.buffer_size = PADDED_IMAGE_SIZE_IN_WORDS; //To make sure hold up 324*324
    cfg.done_cb     = user_callback;
    camera_setup(&cfg);

    os_SemInit SemInit;
    SemInit.uSemType = SEM_BINARY;
    cam_notify       = os_SemCreate(&SemInit);

    Print_ERR("Camera Wait\n");
    //os_TaskSleep(15000); //Wait other core to finish startup print


    ei_camera_frame_buffer = (uint8_t *)(cfg.buffer + 2); // the first 2 words (6 pixels) are discarded in the driver
    is_initialised = true;

    return true;
}

/**
 * @brief      Stop streaming of sensor data
 */
void ei_camera_deinit(void) {
    freeMem(cfg.buffer);
    is_initialised = false;
}

/**
 * @brief      Capture, rescale and crop image
 *
 * @param[in]  img_width     width of output image
 * @param[in]  img_height    height of output image
 * @param[in]  out_buf       pointer to store output image, NULL may be passed
 *                           when you want the result image to be kept in the
 *                           internal frame buffer.
 *
 * @retval     false if not initialised, image captured, rescaled or cropped failed
 *
 */
bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf) {
    bool do_resize = false;
    bool do_crop = false;

    if (!is_initialised) {
        ei_printf("ERR: Camera is not initialized\r\n");
        return false;
    }

    EiDevice.set_state(eiStateSampling);

    camera_capture(cfg.buffer, cfg.buffer_size);

    os_SemTake(cam_notify, OS_SEM_WAIT_FOREVER);

    // from (LE) [0 P1 P2 P3] [ 0 P4 P5 P6 ] to (LE) [P3 P2 P1] 1BP
    int out_idx = 0;
    uint32_t *pBuf = (uint32_t *) ei_camera_frame_buffer;
    for (int in_idx =0; in_idx < EI_CAMERA_RAW_FRAME_BUFFER_COLS * EI_CAMERA_RAW_FRAME_BUFFER_ROWS/3; in_idx++) {
        uint32_t pixel = pBuf[in_idx];
        ei_camera_frame_buffer[out_idx++] = pixel >> 24 & 0xff;
        ei_camera_frame_buffer[out_idx++] = pixel >> 16 & 0xff;
        ei_camera_frame_buffer[out_idx++] = pixel >>  8 & 0xff;
    }

    if ((img_width != EI_CAMERA_RAW_FRAME_BUFFER_COLS)
        || (img_height != EI_CAMERA_RAW_FRAME_BUFFER_ROWS)) {
        do_resize = true;
    }

    // The following variables should always be assigned
    // if this routine is to return true
    // cutout values
    ei_camera_capture_out = ei_camera_frame_buffer;

    if (do_resize) {

        // if only resizing then and out_buf provided then use itinstead.
        if (out_buf && !do_crop) ei_camera_capture_out = out_buf;

        //ei_printf("resize cols: %d, rows: %d\r\n", resize_col_sz,resize_row_sz);
        ei::image::processing::resize_image(
            ei_camera_frame_buffer,
            EI_CAMERA_RAW_FRAME_BUFFER_COLS,
            EI_CAMERA_RAW_FRAME_BUFFER_ROWS,
            ei_camera_capture_out,
            img_width,
            img_height,
            1); // bytes per pixel
    }

    EiDevice.set_state(eiStateIdle);

    return true;
}

/**
 * @brief      Takes a snapshot, base64 encodes and outputs it to uart
 *
 * @param[in]  img_width     width of output image
 * @param[in]  img_height    height of output image
 *
 * @retval     true if snapshot was taken successfully
 *
 */
bool ei_camera_take_snapshot_encode_and_output(size_t width, size_t height, bool use_max_baudrate)
{
    bool result = true;

    if (!prepare_snapshot(width, height, use_max_baudrate))
        result = false;

    if (result) {
        if (!take_snapshot(width, height, true)) {
            result = false;
        }
    }

    finish_snapshot();

    return result;
}

/**
 * @brief      Starts a snapshot stream, base64 encodes and outputs it to uart
 *
 * @param[in]  img_width     width of output image
 * @param[in]  img_height    height of output image
 *
 * @retval     true if successful and/or terminated gracefully
 *
 */
bool ei_camera_start_snapshot_stream_encode_and_output(size_t width, size_t height, bool use_max_baudrate)
{
    bool result = true;

    ei_printf("Starting snapshot stream...\r\n");

    if (!prepare_snapshot(width, height, use_max_baudrate))
        result = false;

    while (result) {
        if (!take_snapshot(width, height, true)) {
            result = false;
        }

        if (ei_user_invoke_stop()) {
            ei_printf("Snapshot streaming stopped by user\r\n");
            EiDevice.set_state(eiStateIdle);
            break;
        }
    }

    finish_snapshot();

    return result;
}

/**
 * @brief      Helper function: Takes a snapshot, base64 encodes and prints it to uart
 *
 * @param[in]  img_width     width of output image
 * @param[in]  img_height    height of output image
 * @param[in]  print_oks     whether to print OK indicator or not for CLI
 *
 * @retval     bool
 *
 * @note       Expects the camera and `ei_camera_frame_buffer` buffer to be
 * initialised
 */
static bool take_snapshot(size_t width, size_t height, bool print_oks)
{
    if (print_oks) {
        ei_printf("OK\r\n");
    }

    // passing NULL; capture will use the internal framebuffer
    if (ei_camera_capture(width, height, NULL) == false) {
        ei_printf("ERR: Failed to capture image\r\n");

        return false;
    }

    base64_encode(
        (const char *)ei_camera_capture_out,
        height * width,
        ei_putc);
    EiDevice.set_state(eiStateIdle);

    ei_printf("\r\n");
    if (print_oks) {
        ei_printf("OK\r\n");
    }

    return true;
}

static bool verify_inputs(size_t width, size_t height)
{
    const ei_device_snapshot_resolutions_t *list;
    size_t list_size;
    const char *color_depth;

    int dl = EiDevice.get_snapshot_list(
        (const ei_device_snapshot_resolutions_t **)&list,
        &list_size,
        &color_depth);
    if (dl) { /* apparently false is OK here?! */
        ei_printf("ERR: Device has no snapshot feature\r\n");
        return false;
    }

    bool found_res = false;
    for (size_t ix = 0; ix < list_size; ix++) {
        if (list[ix].width == width && list[ix].height == height) {
            found_res = true;
        }
    }

    if (!found_res) {
        ei_printf("ERR: Invalid resolution %ux%u\r\n", width, height);
        return false;
    }

    return true;
}

static bool prepare_snapshot(size_t width, size_t height, bool use_max_baudrate)
{
    if (!verify_inputs(width, height)) {
        return false;
    }

    if (ei_camera_init() == false) {
        ei_printf("ERR: Failed to initialize image sensor\r\n");
        return false;
    }

    // setup data output baudrate
    if (use_max_baudrate) {

        // sleep a little to let the daemon attach on the new baud rate...
        ei_printf("OK\r\n");
        ei_sleep(100);
        EiDevice.set_max_data_output_baudrate();
        ei_sleep(100);
    }

    return true;
}

static void finish_snapshot()
{
    // lower baud rate
    ei_printf("OK\r\n");
    ei_sleep(100);
    EiDevice.set_default_data_output_baudrate();
    // sleep a little to let the daemon attach on baud rate 115200 again...
    ei_sleep(100);
}

int ei_camera_cutout_get_data(size_t offset, size_t length, float *out_ptr)
{
    size_t pixels_left = length;
    size_t out_ptr_ix = 0;

    // read pixel for pixel
    while (pixels_left != 0) {

        // grab the value and convert to r/g/b
        uint8_t pixel = ei_camera_capture_out[offset];

        uint8_t r, g, b;
        mono_to_rgb(pixel, &r, &g, &b);

        // then convert to out_ptr format
        float pixel_f = (r << 16) + (g << 8) + b;
        out_ptr[out_ptr_ix] = pixel_f;

        // and go to the next pixel
        out_ptr_ix++;
        offset++;
        pixels_left--;
    }

    // and done!
    return 0;
}
