/*------------------------------------------------------------------------------
  Copyright (C) 2018 Synaptics Inc.
  All rights reserved.

  SYNAPTICS INC. CONFIDENTIAL AND PROPRIETARY

  The information contained in this source code file
  is strictly confidential and proprietary to Synaptics Inc.
  ("Synaptics")

  No part of this file may be possessed, reproduced or distributed, in
  any form or by any means for any purpose, without the express written
  permission of Synaptics Inc.

  Except as otherwise specifically provided through an express agreement
  with Synaptics that governs the confidentiality, possession, use
  and distribution of the information contained in this file, SYNAPTICS
  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
  AND REPRESENTATIONS.  IN NO EVENT WILL SYNAPTICS BE LIABLE FOR ANY DAMAGES
  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
--------------------------------------------------------------------------------

  File Name: AudioSmart_libi2c_flash.h
  
------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"{
#endif 

typedef int (*fn_SetResetPin) ( void* pCallbackContext,
                                int         bSet);
/*
 * Set the SetResetPin callback function.
 * 
 * PARAMETERS
 *  
 *    pCallbackContext    [in] - A pointer to a caller-defined structure of data items
 *                               to be passed as the context parameter of the callback
 *                               routine each time it is called. 
 *
 *    SetResetPinPtr      [in] - A pointer to a i2cwirte callback routine, which is to 
 *                               write I2C data. The callback routine must conform to 
 *                               the following prototype:
 *
 *                        int (*fn_SetResetPin)(  
 *                                  void * pCallbackContext,
 *                                  int    bSet );
 *
 *                        The callback routine parameters are as follows:
 *
 *                         pCallbackContext [in] - A pointer to a caller-supplied 
 *                                                 context area as specified in the
 *                                                 CallbackContext parameter of 
 *                                                 SetupI2cWriteMemCallback. 
 *                         bSet             [in] - Indicates whether to high or low the GPIO pin.
 *
 * RETURN
 *  
 *    If the operation completes successfully, the return value is CXERR_NOERR.
 *    Otherwise, return ERRON_* error code. 
 *
 */
void SetupSetResetPin(  void * pCallbackContext,
                        fn_SetResetPin  SetResetPinPtr);

/*
 * cx_get_buffer_size.
 *
 *  Calculates the buffer size required for firmware update processing.. 
 * 
 * PARAMETERS
 *    None
 *
 * RETURN
 *  
 *    return buffer size required for firmware update processing.. 
 *
 */
uint32_t GetSizeOfBuffer(void);
/*
 * DeviceDownloadModeEn.
 *
 *  Set device to  download mode, alternative with erase header
 * 
 * PARAMETERS
 *    None
 *
 * RETURN
 *  
 *    void
 *
 */
void DeviceDownloadModeEn(bool Enable);

void set_flag_flash_invalidating (bool flag);

void set_flag_flash_nolock (bool flag);

void set_option_flash_fullerase (int opt);

void set_flash_offset_location(int offset);

void get_fw_ver_after_boot (bool flag);
typedef enum _SFS_UPDATE_PARTITION
{
  SFS_UPDATE_AUTO          = 0,   /* update partition according to partition status.*/
  SFS_UPDATE_PARTITION_0   = 1,   /* force to update partition 0.*/
  SFS_UPDATE_PARTITION_1   = 2,   /* force to update partition 1.*/
  SFS_UPDATE_BOTH          = 3,   /* force to update both partitions.*/
  SFS_UPDATE_NONE          = -1,  /* do nothing*/ 
} SFS_UPDATE_PARTITION;

typedef enum _DOWNLOAD_MODE
{
  DOWNLOAD_FACTORY         = 0,   /* for Factory */
  DOWNLOAD_UPDATE          = 1,   /* for Update */
  DOWNLOAD_RECOVERY        = 2,   /* for Recovery */
} DOWNLOAD_MODE;

/*
 * Download Firmware to Hudson.
 * 
 * PARAMETERS
 *    
 *    pBuffer             [in] - A potinter to a buffer for firmware update processing.
 *    loader_data         [in] - A pointer to the input buffer that contains I2C bootloader data.
 *    loader_size         [in] - The size of Bootloader data in bytes.
 *    image_data          [in] - A pointer to the input buffer that contains Image data. 
 *    image_size          [in] - The size of Image data in bytes.
 *    slave_address       [in] - Specifies I2C chip address.
 *    eDownLoadMode       [in] - Specifies the download mode, Could be the follwoing one of values.(Reno only) 
 *                               0 :  auto 
 *                               1 :  flash partition 0
 *                               2 :  flash partition 1
 *                               3 :  flash both partitions

 * RETURN
 *  
 *    If the operation completes successfully, the return value is CXERR_NOERR.
 *    Otherwise, return ERRON_* error code. 
 * 
 * REMARKS
 *  
 *    You need to set up both I2cWrite and I2cWriteThenRead callback function by calling 
 *    SetupI2cWriteMemCallback and SetupI2cReadMemCallback before you call this function.
 */

int DownloadFW_eFlash(        void *                      pBuffer,
                       const unsigned char *       loader_data,
                       uint32_t               loader_size,
                       const unsigned char *       image_data,
                       uint32_t               image_size,
                       unsigned char               slave_address,
                       DOWNLOAD_MODE        eDownLoadMode,
                       int boot_time_upgrade,
                       int legacy_device);

/*
 * time delay function.
 * 
 * PARAMETERS
 *  
 *    ms_delay         [in] - Specifies millisecond delay
 * 
 *
 * RETURN
 *  
 *    None. 
 * 
 */
void sys_mdelay(unsigned int ms_delay);

uint32_t i2c_sub_read(uint32_t i2c_sub_addr, int *pErrNo);
int i2c_sub_write(uint32_t i2c_sub_addr, uint32_t i2c_data);
int i2c_sub_burst_write(uint32_t startAddr, uint32_t num_byte, const unsigned char *pData);
int i2c_sub_burst_read(uint32_t startAddr, uint32_t num_byte, uint8_t *pData);

#ifdef __cplusplus
}
#endif 


/*Error codes*/
#define CXERR_NOERR                 0
#define CXERR_MIS_ENDIANNESS        101
#define CXERR_WRITE_FAILED          102
#define CXERR_INVALID_DATA          103
#define CXERR_CHECKSUM_FAILED       104
#define CXERR_FAILED                105
#define CXERR_INVALID_PARAMETER     106
#define CXERR_NOMEM                 107
#define CXERR_I2CFUN_NOT_SET        108
#define CXERR_UPDATE_MEMORY_FAILED  109
#define CXERR_DEVICE_NOT_RESET      110
#define CXERR_DEVICE_OUT_OF_CONTROL 111
#define CXERR_UPDATE_EEPROM_FAILED  112
#define CXERR_INVALID_BOOTLOADER    113
#define CXERR_INVALID_IMAGE         114
#define CXERR_WAITING_RESET_TIMEOUT 115
#define CXERR_LOAD_IMG_TIMEOUT      116
#define CXERR_STATE_FATAL           117
#define CXERR_CRC_CHECK_ERROR       118
#define CXERR_I2C_ERROR             119
#define CXERR_BOOT_LOADER_NOT_FOND  120
#define CXERR_IMAGE_NOT_FOND        121
#define CXERR_WAITING_LOADER_TIMEOUT  123
#define CXERR_NULL_POINTER          124
#define CXERR_I2C_BLOCK_NR_ERROR    125
#define CXERR_SEND_I2C_ERROR        126
#define CXERR_NO_MORE_DATA          127
#define CXERR_UNSUPPORTED_FLASH_MEMORY    128

#define CXERR_I2CWRITE_DATA_MISALIGNMENT  201
#define CXERR_I2CWRITE_ADDR_MISALIGNEMNT  202
#define CXERR_I2CREAD_DATA_MISALIGNMENT   203
#define CXERR_I2CREAD_ADDR_MISALIGNEMNT   204


#define CXERR_FW_NO_RESPONSE        301
#define CXERR_FW_VER_INCORRECT      302
#define CXERR_FW_CANNOT_BOOT        303

/*Boot loader error code*/
#define BLERR_BLOCK_NR		-1
#define BLERR_CHECKSUM		-2
#define BLERR_LENGTH		-3
#define BLERR_TIMEOUT		-4
#define BLERR_NSUPP		-5
#define BLERR_NPROT		-6
#define BLERR_ERASE		-7
#define BLERR_WRITE		-8
#define BLERR_VERIFY		-9
#define BLERR_INVALID_CMD	-10
#define BLERR_NOTHING_TO_DO	-11
#define BLERR_INVALID_SFS_HDR	-12
#define BLERR_I2C_INTERNAL	-20

#define RESET_INTERVAL_MS    200 // 200 ms


