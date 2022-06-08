////////////////////////////////////////////////////////////////////////////////
//
//  SYNAPTICS INC. CONFIDENTIAL AND PROPRIETARY
//
//  The information contained in this source code file
//  is strictly confidential and proprietary to Synaptics, Inc.
//  ("Synaptics")
//
//  No part of this file may be possessed, reproduced or distributed, in
//  any form or by any means for any purpose, without the express written
//  permission of Synaptics Inc.
//
//  Except as otherwise specifically provided through an express agreement
//  with Synaptics that governs the confidentiality, possession, use
//  and distribution of the information contained in this file, SYNAPTICS
//  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
//  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
//  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
//  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
//  AND REPRESENTATIONS.  IN NO EVENT WILL SYNAPTICS BE LIABLE FOR ANY DAMAGES
//  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
//
//  Copyright (C) 2019 Synaptics Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
/**
 * \file sfs_drv.h
 * \brief Those APIs will be used for file operation such as Open, Read/Write and Close.
 * \defgroup Programming_API_SFS
 * \ingroup Programming_API_SFS
 * @{
 *
 *
 * sfs_drv.h
 * 
 * \section Introduction
 * The purpose of those APIs is used for file operation by which customer can store some key information. \n
 * Such as the setting or configuration from end user.
 * 
 * \section Usage
 * The usage for those APIs is very similar to general C code style function for file operation. \n 
 * Please pay attention to the file name in MCU SDK, it is four upper letter such as _ID('T','E','S','F'), \n
 * and it will be converted to int32 format type. \n
 * \b Step 1:
 * Calling sfs_fopen to open one existed file, if this file is not existed, then return error, \n
 * If that, you can call sfs_fcreate to create one new file.
 * 
 * \b Step 2: 
 * Calling sfs_fwrite or sfs_fread to write / read buffer data to / from this opened file.
 * 
 * \b Step 3:
 * After finish the file write / read, please calling sfs_fclose to close this file handle.
 *
 * \section Example
 * Please refer to sfs_demo.c under m33/app/demo for more detail example code.
 * 
 *
 */
 
/** @} */ 
#ifndef _MCU_DRV_SFS_H_
#define _MCU_DRV_SFS_H_



#define SFS_LOG2BLKSIZ          12
#define SFS_BLK2BYTE(n)         ((n)<<SFS_LOG2BLKSIZ)
#define SFS_BLOCKSIZE           4096

#define SFS_MAGIC(a,b,c,d)	    (((a)<<(0*8))|((b)<<(1*8))|((c)<<(2*8))|((d)<<(3*8)))
#define SFS_MAGIC_FILE		    SFS_MAGIC('S','F','S','F')
#define SFS_MAGIC_FDEL          SFS_MAGIC('S','F','S','N')
#define SFS_MAGIC_SCFILE        SFS_MAGIC('E','M','E','T')
#define SFS_MAGIC_TEMP			SFS_MAGIC('S','F','S','N')
#define SFS_MAGIC_DELETED		SFS_MAGIC('S','F','S','@')

#ifdef CONFIG_M33_SFS_TO_SHARED_SYSMEM_MEM
#define SFS_RELPOS_IN_SM(n)   ((n) - CONFIG_M33_SFS_IN_SHARED_SYSMEM_MEM_BASE)//relative address in share mem
#define SFS_ABSPOS_IN_SM(n)   ((n) + CONFIG_M33_SFS_IN_SHARED_SYSMEM_MEM_BASE)//absolute posisiton in share mem
#endif
/**
 * \enum sfs_file_hdr_t
 * \brief This enum is used for file handler.
 */
typedef struct _sfs_file_s
{
    uint32_t magic;             /// 0 magic to specify the contents. Normally it will be SFSF.
    uint32_t asize;             /// 4 allocated size of memory in blocks when blk alignment. size in bytes when bytes alignment.
    uint32_t id;                /// 8 name of the file.
}sfs_file_hdr_t;

/**
 * \enum ConfigScFileHeader
 * \brief This enum is used for file handler configuration.
 */
typedef struct
{
    uint32_t magic;             ///EMET, specify this is a script file.
    uint32_t data_len;          ///Real data length of the file.
    uint32_t checksum;          ///Checksum of the contents.
}ConfigScFileHeader;

/**
 * \enum sfs_file_info_t
 * \brief This enum is used for storing position informaion of file operation.
 */
typedef struct sfs_file_info_s
{
    uint32_t sta;       /// starting address of the file in FLASH.
    uint32_t fsz;       /// file size in bytes.
    uint32_t pos;       /// current relative position within the file, in byte.
    uint32_t part_num;  /// partition number
}sfs_file_info_t;

#define DEFINE_PARTITION_MAIN_FILE_INFO(name)       sfs_file_info_t name = {.part_num = PARTITION_MAIN}
#define DEFINE_PARTITION_PERSIST_FILE_INFO(name)    sfs_file_info_t name = {.part_num = PARTITION_PERSIST}
#define DEFINE_PARTITION_CUSTOM0_FILE_INFO(name)    sfs_file_info_t name = {.part_num = PARTITION_CUSTOM0}
typedef enum
{
    STORE_IN_SPI = 0,
    STORE_IN_SHARE_MEM,
}sfs_store_t;

/**
 * \enum sfs_partition_info_t
 * \brief This enum is used for partition information of file system.
 */
typedef struct sfs_partition_info_s
{
#define PARTITION_ATTR_READ         (1 << 0)
#define PARTITION_ATTR_WRITE        (1 << 1)
#define PARTITION_ATTR_BYTES_ALIGN  (1 << 2)    //By default, it is block alignment.
                                                //So no attribute macro for block alignment.
    uint32_t partition_attr;
    uint32_t partition_start_addr;  ///Need to 4K alignment
    uint32_t partition_size;        ///Need to 4K alignment
    int16_t sfs_partition_store;
} sfs_partition_info_t;

/**
 * \enum drv_sfs_ops_t
 * \brief This enum is used for file operations API structure.
 */
typedef struct drv_sfs_ops_s
{
    DECLARE_STANDARD_DEVICE_API;

    int (*_sfs_config)(uint32_t partition, sfs_partition_info_t *ppart_info);        /// file configuration information
    int (*_get_info)(uint32_t partition, sfs_partition_info_t *ppart_info);      /// get file configuration information
#define PARTITION_MAIN          0   //System default file system.
#define PARTITION_PERSIST       1   //Extend FS like for persistent storage.
#ifdef CONFIG_MCU_DRV_SFS_CUSTOM0_PARTITION
#define PARTITION_CUSTOM0        2   //Custom FS for Voice Prompt files
#endif
    int (*_fcreate) (uint32_t file_id, uint32_t fsize_in_nr, sfs_file_info_t *pinfo);   /// create file
    int (*_fopen)   (uint32_t file_id, sfs_file_info_t *pinfo);                      /// open file
    int (*_fclose)  (sfs_file_info_t *pinfo);                                        /// close file
    int (*_fread)   (sfs_file_info_t *pinfo, uint8_t *buffer, uint32_t len_in_byte); /// read file
    int (*_fwrite)  (sfs_file_info_t *pinfo, uint8_t *buffer, uint32_t len_in_byte); /// write file
    int (*_fdelete) (uint32_t file_id, uint32_t which_part);                         /// delete file
#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2
    int (*_fseek)   (sfs_file_info_t *pinfo, int32_t offset,  uint32_t fromwhere);  /// seek file
} drv_sfs_ops_t;
extern drv_sfs_ops_t _sfs_ops;

int32_t drv_sfs_start(void);

__STATIC_FORCEINLINE int32_t sfs_config(uint32_t partition, sfs_partition_info_t *ppart_info)
{
    return _sfs_ops._sfs_config(partition, ppart_info);
}

__STATIC_FORCEINLINE int32_t sfs_get_info(uint32_t partition, sfs_partition_info_t *ppart_info)
{
    return _sfs_ops._get_info(partition, ppart_info);
}

/**
 * \fn int32_t sfs_fcreate(uint32_t file_id, uint32_t fsize_in_nr, sfs_file_info_t *pinfo)
 * \brief This function create a file with specified parameters. \n
 *        Caller has to specify the partition number in pinfo->part_num. \n
 *        DEFINE_PARTITION_*_FILE_INFO are the helper definitions to declare a variable. \n
 *        More usage information please refer to sfs_demo.c.
 * \param file_id file name.
 * \param fsize_in_nr allocated size of memory in blocks.
 * \param pinfo specify file information such as partition number.
 * \return status int32_t representing the status of the function.\n
 *               = 0 if successful\n
 *              != 0 if error
 */
__STATIC_FORCEINLINE int32_t sfs_fcreate(uint32_t file_id, uint32_t fsize_in_nr, sfs_file_info_t *pinfo)
{
    return _sfs_ops._fcreate(file_id, fsize_in_nr, pinfo);
}

/**
 * \fn int32_t sfs_fopen(uint32_t file_id, sfs_file_info_t *pinfo)
 * \brief This function open a file with specified parameters. \n
 *        Caller has to specify the partition number in pinfo->part_num. \n
 *        DEFINE_PARTITION_*_FILE_INFO are the helper definitions to declare a variable. \n
 *        More usage information please refer to sfs_demo.c.
 * \param file_id file name.
 * \param pinfo specify file information such as partition number.
 * \return status int32_t representing the status of the function.\n
 *               = 0 if successful\n
 *              != 0 if error
 */
__STATIC_FORCEINLINE int32_t sfs_fopen(uint32_t file_id, sfs_file_info_t *pinfo)
{
    return _sfs_ops._fopen(file_id, pinfo);
}

/**
 * \fn int32_t sfs_fclose(sfs_file_info_t *pinfo)
 * \brief This function close a file with specified parameters. \n
 *        More usage information please refer to sfs_demo.c.
 * \param pinfo specify file information such as partition number.
 * \return status int32_t representing the status of the function.\n
 *               = 0 if successful\n
 *              != 0 if error
 */
__STATIC_FORCEINLINE int32_t sfs_fclose(sfs_file_info_t *pinfo)
{
    return _sfs_ops._fclose(pinfo);
}

/**
 * \fn int32_t sfs_fread(sfs_file_info_t *pinfo, uint8_t *buffer, uint32_t len_in_byte)
 * \brief This function read the specified data size from a file. \n
 *        More usage information please refer to sfs_demo.c.
 * \param pinfo use the same information return by sfs_fcreate or sfs_fopen.
 * \param buffer used for storing data read from file.
 * \param len_in_byte the data size read from file.
 * \return status int32_t representing the status of the function.\n
 *               >= 0 if successful \n
 *               < 0 if error
 */
__STATIC_FORCEINLINE int32_t sfs_fread(sfs_file_info_t *pinfo, uint8_t *buffer, uint32_t len_in_byte)
{
    return _sfs_ops._fread(pinfo, buffer, len_in_byte);
}

/**
 * \fn int32_t sfs_fwrite(sfs_file_info_t *pinfo, uint8_t *buffer, uint32_t len_in_byte)
 * \brief This function write specified size of data into a file. \n
 *        More usage information please refer to sfs_demo.c.
 * \param pinfo use the same information return by sfs_fcreate or sfs_fopen.
 * \param buffer used for storing data write to file.
 * \param len_in_byte the data size write to file.
 * \return status int32_t representing the status of the function.\n
 *              >= 0 if successful\n
 *              < 0 if error
 */
__STATIC_FORCEINLINE int32_t sfs_fwrite(sfs_file_info_t *pinfo, uint8_t *buffer, uint32_t len_in_byte)
{
    return _sfs_ops._fwrite(pinfo, buffer, len_in_byte);
}

/**
 * \fn int32_t sfs_fdelete(uint32_t file_id, uint32_t which_part)
 * \brief This function delete a file with specified parameters. \n
 *        More usage information please refer to sfs_demo.c.
 * \param file_id the file name.
 * \param which_part use the same partition number as sfs_fcreate or sfs_fopen.
 * \return status int32_t representing the status of the function.\n
 *               = 0 if successful\n
 *              != 0 if error
 */
__STATIC_FORCEINLINE int32_t sfs_fdelete(uint32_t file_id, uint32_t which_part)
{
    return _sfs_ops._fdelete(file_id, which_part);
}

/**
 * \fn int32_t sfs_fseek(sfs_file_info_t *pinfo, int32_t offset,  uint32_t fromwhere)
 * \brief This function seek to specified position of a file. \n
 *        More usage information please refer to sfs_demo.c.
 * \param pinfo use the same information return by sfs_fcreate or sfs_fopen.
 * \param offset the end position.
 * \param fromwhere the begin position.
 * \return status int32_t representing the status of the function.\n
 *               = 0 if successful\n
 *              != 0 if error
 */
__STATIC_FORCEINLINE int32_t sfs_fseek(sfs_file_info_t *pinfo, int32_t offset,  uint32_t fromwhere)
{
    return _sfs_ops._fseek(pinfo, offset, fromwhere);
}

#endif
