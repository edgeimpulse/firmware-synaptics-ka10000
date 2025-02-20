/* The Clear BSD License
 *
 * Copyright (c) 2025 EdgeImpulse Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 *   * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#define PRINT_LEVEL_SET ASSERT_LEVEL_DBG

/* Include ----------------------------------------------------------------- */
#include "ei_device_synaptics_ka10000.h"
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "edge-impulse-sdk/dsp/numpy.hpp"
#include "firmware-sdk/at_base64_lib.h"
#include "ei_camera.h"
#include "mcu.h"
#include "firmware-sdk/jpeg/encode_as_jpg.h"
#include "edge-impulse-sdk/porting/ei_classifier_porting.h"
extern "C" {
#include "mem_alloc_al.h"
}

#if defined(EI_CLASSIFIER_SENSOR) && EI_CLASSIFIER_SENSOR == EI_CLASSIFIER_SENSOR_CAMERA

void run_nn(bool debug, int delay_ms, bool use_max_baudrate) {

    bool stop_inferencing = false;

    // summary of inferencing settings (from model_metadata.h)
    ei_printf("Inferencing settings:\n");
    ei_printf("\tImage resolution: %dx%d\n", EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT);
    ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));

    for (size_t ix = 0; ix < ei_dsp_blocks_size; ix++) {
        ei_model_dsp_t block = ei_dsp_blocks[ix];
        if (block.extract_fn == &extract_image_features) {
            ei_dsp_config_image_t config = *((ei_dsp_config_image_t*)block.config);
            int16_t channel_count = strcmp(config.channels, "Grayscale") == 0 ? 1 : 3;
            if (channel_count == 3) {
                ei_printf("WARN: You've deployed a color model, but the Synaptics KA10000 only has a monochrome image sensor. Set your DSP block to 'Grayscale' for best performance.\r\n");
                break; // only print this once
            }
        }
    }

    if (ei_camera_init() == false) {
        ei_printf("ERR: Failed to initialize image sensor\r\n");
        return;
    }

    if (use_max_baudrate) {
        // sleep a little to let the daemon attach on the new baud rate...
        ei_printf("OK\r\n");
        ei_sleep(100);

        EiDevice.set_max_data_output_baudrate();
    }

    while(stop_inferencing == false) {
        if (delay_ms != 0) {
            ei_printf("Starting inferencing in %d seconds...\n", delay_ms / 1000);

            // instead of wait_ms, we'll wait on the signal, this allows threads to cancel us...
            if (ei_sleep(delay_ms) != EI_IMPULSE_OK) {
                break;
            }
        }

        ei::signal_t signal;
        signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
        signal.get_data = &ei_camera_cutout_get_data;

        ei_printf("Taking photo...\n");

        if (ei_camera_capture((size_t)EI_CLASSIFIER_INPUT_WIDTH, (size_t)EI_CLASSIFIER_INPUT_HEIGHT, NULL) == false) {
            ei_printf("Failed to capture image\r\n");
            break;
        }

        // run the impulse: DSP, neural network and the Anomaly algorithm
        ei_impulse_result_t result = { 0 };

        EI_IMPULSE_ERROR ei_error = run_classifier(&signal, &result, false);
        if (ei_error != EI_IMPULSE_OK) {
            ei_printf("Failed to run impulse (%d)\n", ei_error);
            break;
        }

        if (debug) {
            ei_printf("Begin output\n");
            ei_printf("Framebuffer: ");
            size_t out_size;
            int x = encode_bw_signal_as_jpg_and_output_base64(&signal, EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT);
            if (x != 0) {
                ei_printf("Failed to encode frame as JPEG (%d)\n", x);
                break;
            }
            ei_printf("\r\n");
        }

        display_results(&ei_default_impulse, &result);

        if (debug) {
            ei_printf("End output\n");
        }

        if(ei_user_invoke_stop()) {
            ei_printf("Inferencing stopped by user\r\n");
            EiDevice.set_state(eiStateIdle);
            break;
        }
    }

    if (use_max_baudrate) {
        // lower baud rate
        ei_printf("OK\r\n");
        EiDevice.set_default_data_output_baudrate();

        // sleep a little to let the daemon attach on baud rate 115200 again...
        ei_sleep(100);
    }

    ei_camera_deinit();
}

#else
void run_nn(bool debug, int delay_ms, bool use_max_baudrate) {}
#error "EI_CLASSIFIER_SENSOR not configured, cannot configure `run_nn`"

#endif // EI_CLASSIFIER_SENSOR

void run_nn_normal(void) {
    run_nn(false, 2000, false);
}

void run_nn_debug(char *baudrate_s) {

    bool use_max_baudrate = false;
    if (baudrate_s[0] == 'y') {
       use_max_baudrate = true;
    }

#if defined(EI_CLASSIFIER_SENSOR) && EI_CLASSIFIER_SENSOR == EI_CLASSIFIER_SENSOR_CAMERA
    run_nn(true, 0, use_max_baudrate);
#else
    run_nn(true, 2000, use_max_baudrate);
#endif

}

void run_nn_continuous_normal()
{
#if defined(EI_CLASSIFIER_SENSOR) && EI_CLASSIFIER_SENSOR == EI_CLASSIFIER_SENSOR_CAMERA
    run_nn(false, 0, false);
#else
    ei_printf("Error no continuous classification available for current model\r\n");
#endif
}
