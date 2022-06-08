/*------------------------------------------------------------------------------
  Copyright (C) 2018-2020 Synaptics Inc.
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

  File Name: AudioSmart_libbg_upgrade.h

------------------------------------------------------------------------------*/
/**
 * \file AudioSmart_libbg_upgrade.h
 * \brief This file contains the API for controlling the Synaptics Audio
 *        Background Upgrade used to update the Synaptics firmware.
 *
 * \addtogroup BgUpgradeAPI
 * @{
 *
 */
#ifndef AudioSmart_LIBBG_UPGRADE_H
#define AudioSmart_LIBBG_UPGRADE_H

#ifdef __cplusplus
extern "C"{
#endif

#include "syna_types.h"

enum AudioSmart_BG_STAT 
{
  AudioSmart_BG_OK,
  AudioSmart_BG_ERR,
  AudioSmart_BG_RETRY,
  AudioSmart_BG_ERASE_CHIPS
};

typedef struct AudioSmart_img_descriptor_s {
  uint32_t fw_img_version[4];
  uint32_t bootloader_size;
  uint32_t fw_img_size;
  uint32_t padded_img:1;
  uint32_t dual_partition_img:1;
  uint32_t header_idx; 
  uint32_t fst; 
  uint32_t end; 
  uint32_t total;
  uint32_t curr_blk; 
} AudioSmart_img_descriptor;

#define MAX_SYS_PART_NUM  3

struct sys_part_s {
  uint16_t blk_start;
  uint16_t num_blks;
  uint8_t  attribute;
};

typedef struct AudioSmart_spi_flash_descriptor_s {
  uint32_t id                ;
  uint32_t status            ;
  uint16_t active_partition  ;
  uint16_t updating_partition;
  uint16_t fst     ;
  uint16_t end     ;
  uint16_t total   ;
  uint16_t curr_blk;
  uint16_t bootloader_hdr_blkno  ;
  uint32_t fw_ver[4];
  uint16_t num_sys_images    ;
  struct sys_part_s sys_part[MAX_SYS_PART_NUM];
#define AudioSmart_SPI_NAME_LEN 16
  uint8_t  spi_name_str[AudioSmart_SPI_NAME_LEN];
} AudioSmart_spi_flash_descriptor;

/**
 * \fn int32_t AudioSmart_bg_upgrade_reset_device(void)
 *
 * \brief This function soft resets the Audio DSP
 *
 * \return status integer representing the status of the function.
 *               = CXERR_NOERR if successful, or error-code
 */

int32_t AudioSmart_bg_upgrade_reset_device(void);

/**
 * \fn int32_t AudioSmart_bg_get_bl_version(uint32_t *ver_z, uint32_t *ver_z,uint32_t *ver_y,uint32_t *ver_x,uint32_t *ver_w);
 *
 * \brief This function gets the Bootloader version from the Audio DSP.
 *
 * \param bl_ver  A pointer to the Bootloader's version version
 * \param ver_z   A pointer to the firmware Major version when the bootloader was built (z)
 * \param ver_y   A pointer to the firmware Minor version when the bootloader was built   (y)
 * \param ver_x   A pointer to the firmware Variant version when the bootloader was built (x)
 * \param ver_w   A pointer to the firmware Build version when the bootloader was built   (w)
 * \return status integer representing the status of the function.
 *               = CXERR_NOERR if successful, or error-code
 */
int32_t AudioSmart_bg_get_bl_version(uint32_t *bl_ver, uint32_t *ver_z, uint32_t *ver_y, uint32_t *ver_x, uint32_t *ver_w);

/**
 * \fn int32_t AudioSmart_bg_get_firmware_version(uint32_t *ver_z,uint32_t *ver_y,uint32_t *ver_x,uint32_t *ver_w);
 *
 * \brief This function gets the firmware version from the Audio DSP.
 *
 * \param ver_z		A pointer to the firmware Major version    (z)
 * \param ver_y		A pointer to the firmware Minor version    (y)
 * \param ver_x		A pointer to the firmware Variant version  (x)
 * \param ver_w		A pointer to the firmware Build version    (w)
 * \return status integer representing the status of the function.
 *               = CXERR_NOERR if successful, or error-code
 */
int32_t AudioSmart_bg_get_firmware_version(uint32_t  *ver_z,uint32_t *ver_y,uint32_t *ver_x,uint32_t *ver_w);

/**
 * \fn int32_t AudioSmart_bg_get_current_sys_partition_number(void);
 *
 * \brief This function gets the Current System Partition Number from the Audio DSP.
 *
 * \return status integer > 0: representing the current system partition number
 *                        < 0: CXERR_NOERR if successful, or error-code
 */
int32_t AudioSmart_bg_get_current_sys_partition_number(void);

/**
 * \fn  int32_t AudioSmart_validate_fw_headers(const uint8_t *img, uint32_t image_size, uint16_t starting_spi_blk_num, uint16_t blk_num_offset,
 *                                             bool mn_checking, void (*delay_fn_ptr)(uint32_t delay_in_ms),
 *                                             AudioSmart_spi_flash_descriptor *spimem_descriptor, AudioSmart_img_descriptor *img_descriptor);
 *
 *
 * \brief This function checks whether the firmware image being updated matches the device being flashed.
 *
 * \param img                A pointer to the input buffer that contains the firmware image header (1 blocks)
 * \param image_size         Size of the System Partition Image
 * \param starting_spi_blk_num Starting SPI block number
 * \param blk_num_offset     Block number offset in case of sharing SPI flash (default should be 64)
 * \param mn_checking        Should we check Magic Number of the firmware image file and the one from the device, and continue only if they match
 * \param delay_fn_ptr       Function pointer to a delay function which takes number of ms as argument.
 * \param spimem_descriptor  Pointer to SPI-memory descriptor structure
 * \param img_descriptor     Pointer to image descriptor structure
 *
 * \return status integer representing the status of the function.
 *               = CXERR_NOERR if successful, or error-code
 */
int32_t AudioSmart_validate_fw_headers(const uint8_t *img, uint32_t image_size, uint16_t starting_spi_blk_num, uint16_t blk_num_offset,
                                       bool mn_checking, void (*delay_fn_ptr)(uint32_t delay_in_ms),
                                       AudioSmart_spi_flash_descriptor *spimem_descriptor, AudioSmart_img_descriptor *img_descriptor);

/**
 * \fn int32_t AudioSmart_start_fw_image_flashing(const uint8_t *img, void (*delay_fn_ptr)(uint32_t delay_in_ms),
 *                                                AudioSmart_img_descriptor *img_descriptor, AudioSmart_spi_flash_descriptor *spimem_descriptor);
 *
 * \brief This function starts flashing the firmware image to the Audio DSP.
 *
 *
 * \param img                A pointer to the input buffer that contains the firmware image header (1 blocks)
 * \param delay_fn_ptr       Function pointer to a delay function which takes number of ms as argument.
 * \param img_descriptor     Pointer to image descriptor structure
 * \param spimem_descriptor  Pointer to SPI-memory descriptor structure
 *
 * \return status integer representing the status of the function.
 *               = CXERR_NOERR if successful, or error-code
 */
int32_t AudioSmart_start_fw_image_flashing(const uint8_t *img, void (*delay_fn_ptr)(uint32_t delay_in_ms),
                                           AudioSmart_img_descriptor *img_descriptor, AudioSmart_spi_flash_descriptor *spimem_descriptor);

/**
 * \fn  AudioSmart_start_custom0_partition_flashing(uint32_t *cus0_partition_start, uint32_t *cus0_partition_size, uint32_t custom0_fsz_in_block);
 *
 *
 * \brief This function makes firmware prepare for flashing custom0 partition.
 *
 * \param cus0_partition_start  Pointer to return Custom0 Partition Start block address.
 * \param cus0_partition_size   Pointer to return Custom0 Partition Size.
 * \param custom0_fsz_in_block  Custom0 Partition Size in 4k blocks in file.
 *
 * \return status integer representing the status of the function.
 *               = CXERR_NOERR if successful, or error-code
 */
int32_t AudioSmart_start_custom0_partition_flashing(uint32_t *cus0_partition_start, uint32_t *cus0_partition_size, uint32_t custom0_fsz_in_block);

/**
 * \fn int32_t AudioSmart_flash_a_block_of_cus_partition(const uint8_t *block_buff, uint32_t block_number);
 *
 *
 * \brief This function flashes one block of a Custom Partition file to the Audio DSP.
 *
 * \param block_buff         A pointer to the input buffer that contains one block of firmware image data
 * \param block_number       Pointer to block number to flash
 *
 * \return status integer representing the status of the function.
 *               = CXERR_NOERR if successful, or error-code
 */
int32_t AudioSmart_flash_a_block_of_cus_partition(const uint8_t *block_buff, uint32_t block_number);

/**
 * \fn int32_t AudioSmart_flash_a_block(const uint8_t *block,
 *  AudioSmart_img_descriptor *img_descriptor, AudioSmart_spi_flash_descriptor *spimem_descriptor); 
 *
 *
 * \brief This function flashes one block of the firmware image to the Audio DSP.
 *
 * \param block              A pointer to the input buffer that contains one block of firmware image data
 * \param img_descriptor     Pointer to image descriptor structure
 * \param spimem_descriptor  Pointer to SPI-memory descriptor structure
 *
 * \return status integer representing the status of the function.
 *               = CXERR_NOERR if successful, or error-code
 */
int32_t AudioSmart_flash_a_block(const uint8_t *block,
                                 AudioSmart_img_descriptor *img_descriptor, AudioSmart_spi_flash_descriptor *spimem_descriptor);

/**
 * \fn int32_t AudioSmart_end_fw_image_flashing(const uint8_t *headers, void (*delay_fn_ptr)(uint32_t delay_in_ms),
 *  AudioSmart_img_descriptor *img_descriptor, AudioSmart_spi_flash_descriptor *spimem_descriptor); 
 * 
 * \brief This function ends the flashing of the firmware image to the Audio DSP.
 *        
 * \param headers            A pointer to the input buffer that contains firmware image data headers (3 blocks)
 * \param delay_fn_ptr       Function pointer to a delay function which takes number of ms as argument.
 * \param img_descriptor     Pointer to image descriptor structure
 * \param spimem_descriptor  Pointer to SPI-memory descriptor structure
 *
 * \return status integer representing the status of the function.
 *               = CXERR_NOERR if successful, or error-code
 */
int32_t AudioSmart_end_fw_image_flashing(const uint8_t *headers, void (*delay_fn_ptr)(uint32_t delay_in_ms),
    AudioSmart_img_descriptor *img_descriptor, AudioSmart_spi_flash_descriptor *spimem_descriptor); 

#ifdef __cplusplus
}
#endif 

#endif //AudioSmart_LIBBG_UPGRADE_H
/*@}*/
