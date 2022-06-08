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

/* Include ----------------------------------------------------------------- */
#include "ei_device_synaptics_ka10000.h"
#include "ei_synaptics_fs_commands.h"

#include "ei_camera.h"
//#include "ei_inertialsensor.h"
//#include "ei_microphone.h"

#include "repl.h"
#include "uart_drv.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <math.h>

/** Max size for device id array */
#define DEVICE_ID_MAX_SIZE 32

/** Sensors */
typedef enum
{
    MICROPHONE = 0,
    ACCELEROMETER
} used_sensors_t;

/** Data Output Baudrate */
const ei_device_data_output_baudrate_t ei_dev_max_data_output_baudrate = {
    "1500000",
     1500000
};

const ei_device_data_output_baudrate_t ei_dev_default_data_output_baudrate = {
    "115200",
     115200
};

/** Device type */
static const char *ei_device_type = "SYNAPTICS_KA10000  ";

/** Device id array */
static char ei_device_id[DEVICE_ID_MAX_SIZE] = { "34:52:52:22:57:98" };

/** Device object, for this class only 1 object should exist */
EiDeviceSynapticsKa10000 EiDevice;

static tEiState ei_program_state = eiStateIdle;

/** UART used for edge impulse communication */

/* Private function declarations ------------------------------------------- */
static int get_id_c(uint8_t out_buffer[32], size_t *out_size);
static int get_type_c(uint8_t out_buffer[32], size_t *out_size);
static bool get_wifi_connection_status_c(void);
static bool get_wifi_present_status_c(void);
static void timer_callback(void *arg);
static bool read_sample_buffer(size_t begin, size_t length, void (*data_fn)(uint8_t *, size_t));
static int get_data_output_baudrate_c(ei_device_data_output_baudrate_t *baudrate);
static void set_max_data_output_baudrate_c();
static void set_default_data_output_baudrate_c();

/* Public functions -------------------------------------------------------- */

EiDeviceSynapticsKa10000::EiDeviceSynapticsKa10000(void)
{
}

/**
 * @brief      For the device ID, the BLE mac address is used.
 *             The mac address string is copied to the out_buffer.
 *
 * @param      out_buffer  Destination array for id string
 * @param      out_size    Length of id string
 *
 * @return     0
 */
int EiDeviceSynapticsKa10000::get_id(uint8_t out_buffer[32], size_t *out_size)
{
    return get_id_c(out_buffer, out_size);
}

/**
 * @brief      Gets the identifier pointer.
 *
 * @return     The identifier pointer.
 */
const char *EiDeviceSynapticsKa10000::get_id_pointer(void)
{
    return (const char *)ei_device_id;
}

/**
 * @brief      Copy device type in out_buffer & update out_size
 *
 * @param      out_buffer  Destination array for device type string
 * @param      out_size    Length of string
 *
 * @return     -1 if device type string exceeds out_buffer
 */
int EiDeviceSynapticsKa10000::get_type(uint8_t out_buffer[32], size_t *out_size)
{
    return get_type_c(out_buffer, out_size);
}

/**
 * @brief      Gets the type pointer.
 *
 * @return     The type pointer.
 */
const char *EiDeviceSynapticsKa10000::get_type_pointer(void)
{
    return (const char *)ei_device_type;
}

/**
 * @brief      No Wifi available for device.
 *
 * @return     Always return false
 */
bool EiDeviceSynapticsKa10000::get_wifi_connection_status(void)
{
    return false;
}

/**
 * @brief      No Wifi available for device.
 *
 * @return     Always return false
 */
bool EiDeviceSynapticsKa10000::get_wifi_present_status(void)
{
    return false;
}

/**
 * @brief      Create sensor list with sensor specs
 *             The studio and daemon require this list
 * @param      sensor_list       Place pointer to sensor list
 * @param      sensor_list_size  Write number of sensors here
 *
 * @return     False if all went ok
 */
bool EiDeviceSynapticsKa10000::get_sensor_list(
    const ei_device_sensor_t **sensor_list,
    size_t *sensor_list_size)
{
    /* Calculate number of bytes available on flash for sampling, reserve 1 block for header + overhead */
    uint32_t available_bytes = (ei_synaptics_fs_get_n_available_sample_blocks() - 1) *
        ei_synaptics_fs_get_block_size();

    //sensors[MICROPHONE].name = "Built-in microphone";
    //sensors[MICROPHONE].start_sampling_cb = &ei_microphone_sample_start;
    //sensors[MICROPHONE].max_sample_length_s = available_bytes / (16000 * 2);
    //sensors[MICROPHONE].frequencies[0] = 16000.0f;

    //sensors[ACCELEROMETER].name = "Built-in accelerometer";
    //sensors[ACCELEROMETER].start_sampling_cb = &ei_inertial_setup_data_sampling;
    //sensors[ACCELEROMETER].max_sample_length_s = available_bytes / (100 * SIZEOF_N_AXIS_SAMPLED);
    //sensors[ACCELEROMETER].frequencies[0] = 62.5f;
    //sensors[ACCELEROMETER].frequencies[1] = 250.0f;
    //sensors[ACCELEROMETER].frequencies[2] = 500.0f;

    *sensor_list = sensors;
    *sensor_list_size = EI_DEVICE_N_SENSORS;

    return false;
}

/**
 * @brief      Create resolution list for snapshot setting
 *             The studio and daemon require this list
 * @param      snapshot_list       Place pointer to resolution list
 * @param      snapshot_list_size  Write number of resolutions here
 *
 * @return     False if all went ok
 */
bool EiDeviceSynapticsKa10000::get_snapshot_list(
    const ei_device_snapshot_resolutions_t **snapshot_list,
    size_t *snapshot_list_size,
    const char **color_depth)
{
    snapshot_resolutions[0].width = EI_CAMERA_RAW_FRAME_BUFFER_COLS;
    snapshot_resolutions[0].height = EI_CAMERA_RAW_FRAME_BUFFER_ROWS;
    snapshot_resolutions[1].width = 160;
    snapshot_resolutions[1].height= 160;
    snapshot_resolutions[2].width =  96;
    snapshot_resolutions[2].height=  96;
    snapshot_resolutions[3].width =  64;
    snapshot_resolutions[3].height = 64;

#if defined(EI_CLASSIFIER_SENSOR) && EI_CLASSIFIER_SENSOR == EI_CLASSIFIER_SENSOR_CAMERA
    snapshot_resolutions[4].width = EI_CLASSIFIER_INPUT_WIDTH;
    snapshot_resolutions[4].height = EI_CLASSIFIER_INPUT_HEIGHT;
#endif

    *snapshot_list = snapshot_resolutions;
    *snapshot_list_size = EI_DEVICE_N_RESOLUTIONS;
    *color_depth = "Grayscale";

    return false;
}

/**
 * @brief      Create resolution list for resizing
 * @param      resize_list       Place pointer to resolution list
 * @param      resize_list_size  Write number of resolutions here
 *
 * @return     False if all went ok
 */
bool EiDeviceSynapticsKa10000::get_resize_list(
    const ei_device_resize_resolutions_t **resize_list,
    size_t *resize_list_size)
{
    resize_resolutions[0].width = 64;
    resize_resolutions[0].height = 64;

    resize_resolutions[1].width = 100;
    resize_resolutions[1].height = 100;

    resize_resolutions[2].width = 200;
    resize_resolutions[2].height = 200;

    resize_resolutions[3].width = 324;
    resize_resolutions[3].height = 324;

    *resize_list = resize_resolutions;
    *resize_list_size = EI_DEVICE_N_RESIZE_RESOLUTIONS;

    return false;
}

/**
 * @brief      Device specific delay ms implementation
 *
 * @param[in]  milliseconds  The milliseconds
 */
void EiDeviceSynapticsKa10000::delay_ms(uint32_t milliseconds)
{

}

void EiDeviceSynapticsKa10000::setup_led_control(void)
{

}

void EiDeviceSynapticsKa10000::set_state(tEiState state)
{
    ei_program_state = state;

    if (state == eiStateFinished) {

        ei_program_state = eiStateIdle;
    }
}

/**
 * @brief      Get the data output baudrate
 *
 * @param      baudrate    Baudrate used to output data
 *
 * @return     0
 */
int EiDeviceSynapticsKa10000::get_data_output_baudrate(ei_device_data_output_baudrate_t *baudrate)
{
    return get_data_output_baudrate_c(baudrate);
}

/**
 * @brief      Set output baudrate to max
 *
 */
void EiDeviceSynapticsKa10000::set_max_data_output_baudrate()
{
    set_max_data_output_baudrate_c();
}

/**
 * @brief      Set output baudrate to default
 *
 */
void EiDeviceSynapticsKa10000::set_default_data_output_baudrate()
{
    set_default_data_output_baudrate_c();
}

/**
 * @brief      Get a C callback for the get_id method
 *
 * @return     Pointer to c get function
 */
c_callback EiDeviceSynapticsKa10000::get_id_function(void)
{
//#define FLASH_INFO_OFFSET_UNIQUE_ID (0x1f0 >> 2)

    //uint32_t ui32Buffer[SYNAPTICS_CSP_FLASH_PAGE_SIZE_WORDS];

    ////SynapticsCspFlashInfoGet(0x01000000, (uint8_t *)ui32Buffer, SYNAPTICS_CSP_FLASH_PAGE_SIZE_BYTES);

    ///* Setup device ID */
    //snprintf(
    //    &ei_device_id[0],
    //    DEVICE_ID_MAX_SIZE,
    //    "%02X:%02X:%02X:%02X:%02X:%02X",
    //    (uint8_t)((ui32Buffer[FLASH_INFO_OFFSET_UNIQUE_ID + 0] >> 0) & 0xFF),
    //    (uint8_t)((ui32Buffer[FLASH_INFO_OFFSET_UNIQUE_ID + 0] >> 8) & 0xFF),
    //    (uint8_t)((ui32Buffer[FLASH_INFO_OFFSET_UNIQUE_ID + 0] >> 16) & 0xFF),
    //    (uint8_t)((ui32Buffer[FLASH_INFO_OFFSET_UNIQUE_ID + 0] >> 24) & 0xFF),
    //    (uint8_t)((ui32Buffer[FLASH_INFO_OFFSET_UNIQUE_ID + 1] >> 0) & 0xFF),
    //    (uint8_t)((ui32Buffer[FLASH_INFO_OFFSET_UNIQUE_ID + 1] >> 8) & 0xFF));

    return &get_id_c;
}

/**
 * @brief      Get a C callback for the get_type method
 *
 * @return     Pointer to c get function
 */
c_callback EiDeviceSynapticsKa10000::get_type_function(void)
{
    return &get_type_c;
}

/**
 * @brief      Get a C callback for the get_wifi_connection_status method
 *
 * @return     Pointer to c get function
 */
c_callback_status EiDeviceSynapticsKa10000::get_wifi_connection_status_function(void)
{
    return &get_wifi_connection_status_c;
}

/**
 * @brief      Get a C callback for the wifi present method
 *
 * @return     The wifi present status function.
 */
c_callback_status EiDeviceSynapticsKa10000::get_wifi_present_status_function(void)
{
    return &get_wifi_present_status_c;
}

/**
 * @brief      Get a C callback to the read sample buffer function
 *
 * @return     The read sample buffer function.
 */
c_callback_read_sample_buffer EiDeviceSynapticsKa10000::get_read_sample_buffer_function(void)
{
    return &read_sample_buffer;
}

/**
 * @brief      Get characters for uart pheripheral and send to repl
 */
void ei_command_line_handle()
{
}

/**
 * @brief      Call this function periocally during inference to 
 *             detect a user stop command
 *
 * @return     true if user requested stop
 */
bool ei_user_invoke_stop(void)
{
    bool stop_found = false;
    char data = uart_getchar();

    if(data == 'b') {
        stop_found = true;
        EiDevice.set_state(eiStateFinished);
    }

    return stop_found;
}

/**
 * @brief      Setup the serial port
 */
void ei_serial_setup(void)
{

}

/**
 * @brief      Write serial data with length to Serial output
 *
 * @param      data    The data
 * @param[in]  length  The length
 */
void ei_write_string(char *data, int length)
{
    for (int i = 0; i < length; i++) {
        ei_putchar(*(data++));
    }
}

/**
 * @brief      Write single character to serial output
 *
 * @param[in]  cChar  The character
 */
void ei_putc(char cChar)
{
    ei_putchar(cChar);
}

/* Private functions ------------------------------------------------------- */
static void timer_callback(void *arg)
{
    static char toggle = 0;

    if (toggle) {
        switch (ei_program_state) {
        case eiStateErasingFlash:
            break;
        case eiStateSampling:
            break;
        case eiStateUploading:
            break;
        default:
            break;
        }
    }
    else {
        //if (ei_program_state != eiStateFinished) {
        //}
    }
    toggle ^= 1;
}

static int get_id_c(uint8_t out_buffer[32], size_t *out_size)
{
    size_t length = strlen(ei_device_id);

    if (length < 32) {
        memcpy(out_buffer, ei_device_id, length);

        *out_size = length;
        return 0;
    }

    else {
        *out_size = 0;
        return -1;
    }
}

static int get_type_c(uint8_t out_buffer[32], size_t *out_size)
{
    size_t length = strlen(ei_device_type);

    if (length < 32) {
        memcpy(out_buffer, ei_device_type, length);

        *out_size = length;
        return 0;
    }

    else {
        *out_size = 0;
        return -1;
    }
}

static bool get_wifi_connection_status_c(void)
{
    return false;
}

static bool get_wifi_present_status_c(void)
{
    return false;
}

static int get_data_output_baudrate_c(ei_device_data_output_baudrate_t *baudrate)
{
    size_t length = strlen(ei_dev_max_data_output_baudrate.str);

    if (length < 32) {
        memcpy(baudrate, &ei_dev_max_data_output_baudrate, sizeof(ei_device_data_output_baudrate_t));
        return 0;
    }
    else {
        return -1;
    }
}

static void set_max_data_output_baudrate_c()
{
    uart_config(ei_dev_max_data_output_baudrate.val);
}

static void set_default_data_output_baudrate_c()
{
    uart_config(ei_dev_default_data_output_baudrate.val);
}

/**
 * @brief      Read samples from sample memory and send to data_fn function
 *
 * @param[in]  begin    Start address
 * @param[in]  length   Length of samples in bytes
 * @param[in]  data_fn  Callback function for sample data
 *
 * @return     false on flash read function
 */
static bool read_sample_buffer(size_t begin, size_t length, void (*data_fn)(uint8_t *, size_t))
{
    size_t pos = begin;
    size_t bytes_left = length;
    bool retVal;

    EiDevice.set_state(eiStateUploading);

    // we're encoding as base64 in AT+READFILE, so this needs to be divisable by 3
    uint8_t buffer[513];
    while (1) {
        size_t bytes_to_read = sizeof(buffer);
        if (bytes_to_read > bytes_left) {
            bytes_to_read = bytes_left;
        }
        if (bytes_to_read == 0) {
            retVal = true;
            break;
        }

        int r = ei_synaptics_fs_read_sample_data(buffer, pos, bytes_to_read);
        if (r != 0) {
            retVal = false;
            break;
        }
        data_fn(buffer, bytes_to_read);

        pos += bytes_to_read;
        bytes_left -= bytes_to_read;
    }

    EiDevice.set_state(eiStateFinished);

    return retVal;
}
