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

/**
 * \file camera_drv.h
 * \name Camera Driver APIs
 * \defgroup Camera_API Camera Driver API
 * \ingroup HW_DRV_API
 * @{
 *
 *
 * camera_drv.h
 * 
 * \brief Camera Driver API
 * 
 * 
 *
 */
/** @} */

/**
* \file camera_drv.h
* \brief Those APIs can be used for init camera and capture image
*/
#ifndef _M33_DRV_CAMERA_H_
#define _M33_DRV_CAMERA_H_
#include "driver.h"
#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//-----------------------------------------------------------
#ifdef CONFIG_MCU_DRV_CAMERA

/**
 * @brief Callback format
 * 
 */
typedef void (*camera_callback)(void *msg);

/**
 * @brief Camera setup structure
 * 
 */
typedef struct camera_setup_s
{
    uint32_t        *buffer;                //Buffer pointer
    uint32_t        buffer_size;            //Buffer size in DWORD, for HM01B0 at least be 34992
    camera_callback done_cb;                //Capture done callback
    uint32_t        motion_detection_en;    //! Not Used
}camera_setup_t;

struct camera_ops_s
{
    DECLARE_STANDARD_DEVICE_API;
    int (*_camera_sample_check)();
    void (*_print_image)(uint32_t *buf, int dw_size);
    int (*_camera_setup)(camera_setup_t *setup);
    int (*_camera_drv_init)();
    int (*_camera_capture)(uint32_t *buf, uint32_t buf_size_dw);
    int (*_camera_reset)();
    int (*_camera_hw_write)(uint16_t addr, uint8_t data);
    int (*_camera_hw_read)(uint16_t addr, uint8_t *data);
};

STATIC_INLINE struct camera_ops_s *camera_driver_info()
{
    return (struct camera_ops_s *)os_get_driver_info(DEV_CAMERA);
}

/**
 * @brief Sampel flip check
 * 
 * @return int : 1 : Sample fliped,  0: Sample not fliped
 */
STATIC_INLINE int camera_sample_check()
{
    struct camera_ops_s *op;
    if ((op = camera_driver_info()) != NULL)
        return op->_camera_sample_check();
    return -ENODEV;
}

/**
 * @brief Print out image buffer to UART, same time M33 printf will be mute
 * 
 * @param buf :     buffer pointer
 * @param dw_size : buffer size in DWORD
 */
STATIC_INLINE int print_image(uint32_t *buf, int dw_size)
{
    struct camera_ops_s *op;
    if ((op = camera_driver_info()) != NULL)
    {
        op->_print_image(buf, dw_size);
        return 0;
    }
    
    return -ENODEV;
}


/**
 * @brief Camera setup function
 * 
 * @param setup : Setup structure pointer
 */
STATIC_INLINE int camera_setup(camera_setup_t *setup)
{
    struct camera_ops_s *op;
    if ((op = camera_driver_info()) != NULL)
        return op->_camera_setup(setup);
    return -ENODEV;
}


/**
 * @brief Camera init 
 *          CAUTION: This function should only be executed after power cycle
 * 
 */
STATIC_INLINE int camera_drv_init()
{
    struct camera_ops_s *op;
    if ((op = camera_driver_info()) != NULL)
        return op->_camera_drv_init();
    return -ENODEV;
}

/**
 * @brief Capture one frame
 * 
 * @param buf : Capture buffer address pointer
 * @param buf_size_dw : Capture buffer size in DWord
 * @return STATIC_INLINE int 0: success, negative value: failure
 */
STATIC_INLINE int camera_capture(uint32_t *buf, uint32_t buf_size_dw)
{
    struct camera_ops_s *op;
    if ((op = camera_driver_info()) != NULL)
        return op->_camera_capture(buf, buf_size_dw);

    return -ENODEV;
}


/**
 * @brief Camera softreset
 * 
 * @return int 0: success, negative value: faulure
 */
STATIC_INLINE int camera_reset()
{
    struct camera_ops_s *op;
    if ((op = camera_driver_info()) != NULL)
        return op->_camera_reset();

    return -ENODEV;
}


//--------------------- For Debug Only ---------------------------
STATIC_INLINE int camera_hw_write(uint16_t addr, uint8_t data)
{
    struct camera_ops_s *op;
    if ((op = camera_driver_info()) != NULL)
        return op->_camera_hw_write(addr, data);

    return -ENODEV;
}

STATIC_INLINE int camera_hw_read(uint16_t addr, uint8_t *data)
{
    struct camera_ops_s *op;
    if ((op = camera_driver_info()) != NULL)
        return op->_camera_hw_read(addr, data);

    return -ENODEV;
}

int drv_camera_start(void);


#endif

//-----------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // _M33_DRV_CAMERA_H_
