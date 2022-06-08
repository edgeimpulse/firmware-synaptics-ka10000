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

*
* File: main.cpp
*
* Description: Main function for bacground upgrade demo
*
------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <time.h> //for clock()
#define STRSAFE_NO_DEPRECATE //For VS2010 strcpy C4995 warning.
#include <strsafe.h>
#include <conio.h>
extern void set_txt_color(int color);
extern void ShowProgress(int curPos, int bForceRedraw, int eState, const int MaxPos);
extern void InitShowProgress(const int MaxPos);
#else
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))
#define set_txt_color(t)
#define InitShowProgress(MaxPos)  fprintf(stdout, "Downloading ... %3d%%", 0); fflush(stdout);
#define ShowProgress(curPos,  bForceRedraw, eState,MaxPos) fprintf(stdout, "\b\b\b\b%3d%%", (char) (((curPos) *100)/MaxPos) ); fflush(stdout);
#endif

#define InitShowBlocks(StartPos, total)   fprintf(stdout, "Downloading Block... %3d / %3u", StartPos, total); fflush(stdout);
#define ShowBlocks(blocks, total)  fprintf(stdout, "\b\b\b\b\b\b\b\b\b%3u / %3u", blocks, total ); fflush(stdout);
#define ShowBlocksSimple(blocks, total)  if (blocks%BLOCK_TO_SHOW==0) {fprintf(stdout, "\n %3u / %3u ", blocks, total ); fflush(stdout);} else {fprintf(stdout, "."); fflush(stdout);}

#include "syna_types.h"
#include "AudioSmart_definitions.h"
#if (defined ENABLE_I2C_CONTROL) || (defined INTERFACE_I2C_F3)
#include "AudioSmart_i2c_interface_api.h"
#elif ENABLE_USB_CONTROL
#include "AudioSmart_usb_interface_api.h"
#endif
#include "AudioSmart_sendcmd.h"

#include "AudioSmart_libbg_upgrade.h"

#define DUMP(f_, ...) printf((f_), __VA_ARGS__)


#define COL_B 1
#define COL_R 4
#define COL_G 2
#define COL_I 8
#define COLOR_WARNING (COL_I + COL_R + COL_G)
#define COLOR_ERROR (COL_I + COL_R)
#define COLOR_PASS (COL_G)
#define COLOR_NORMAL (COL_R + COL_G + COL_B)
#define CONTROL_MGR_CAPE_SOFTRESET 134

#ifdef _WINDOWS
uint16_t String2Number(wchar_t *string);
#else
uint16_t String2Number(char *string);
#endif

#define SLAVE_ADDRESS 0x41
#define I2C_SUB_ADDR_SIZE 2

#define BLK_SIZE  4096 
#define LOG2BLKSIZ  12
#define SFS_MAGIC_HDRPAD  0x4b534653
#define HEADERS_SIZE (3*BLK_SIZE)

#define BLOCK_TO_SHOW 5

/* Function Declaration */
void ShowResult(bool bIsPass);
void sys_mdelay(unsigned int ms_delay);
long file_length(FILE *f);
int32_t AudioSmart_flash_custom_partition(FILE *cus_image, uint32_t cust_part_start, uint32_t cust_part_size);


static const char *const szPass[] = {
  "  ########     ###     ######   ######  \n",
  "  ##     ##   ## ##   ##    ## ##    ## \n",
  "  ##     ##  ##   ##  ##       ##       \n",
  "  ########  ##     ##  ######   ######  \n",
  "  ##        #########       ##       ## \n",
  "  ##        ##     ## ##    ## ##    ## \n",
  "  ##        ##     ##  ######   ######  \n",
};

static const char *const szFail[] = {
  "                          \n",
  "   ######   ##   # #      \n",
  "   #       #  #  # #      \n",
  "   #####  #    # # #      \n",
  "   #      ###### # #      \n",
  "   #      #    # # #      \n",
  "   #      #    # # ###### \n",
};


/* ------------------------------ Local Functions ----------------------------- */
void ShowResult(bool bIsPass)
{
	if (bIsPass)
	{
		set_txt_color(COLOR_PASS);
		for (int i = 0; i < ARRAYSIZE(szPass); i++)
		{
			fprintf(stdout, "%s", szPass[i]);
			fflush(stdout);
		}
	}
	else
	{
		set_txt_color(COLOR_ERROR);
		for (int i = 0; i < ARRAYSIZE(szFail); i++)
		{
			fprintf(stdout, "%s", szFail[i]);
			fflush(stdout);
		}
	}
	set_txt_color(COLOR_NORMAL);
}


#if (defined _WIN32) && !(defined INTERFACE_I2C_F3) 
void wait_us(UINT us)
{
	static int GetFreqDone = 0;
	LARGE_INTEGER litmp;
	LONGLONG QPartStart, QPartStop;
	static  double dfFreq;
	LONGLONG  ticks_needed = 0;

	/*Step 1 Recording the time-stamp of the Start time  */
	//On systems that run Windows XP or later, the function will always succeed and will thus never return zero.
	QueryPerformanceCounter(&litmp);
	QPartStart = litmp.QuadPart;


	/*Step 2 Retrieves the frequency of the performance counter  */
	if (!GetFreqDone)
	{
		//On systems that run Windows XP or later, the function will always succeed and will thus never return zero.
		QueryPerformanceFrequency(&litmp);
		dfFreq = (double)litmp.QuadPart;//
		GetFreqDone = 0x01;

	}
	else
	{
		// do nothing 
	}

	/*Step 3 Calculate the count/ticks_needed for the delay/wait  */

	ticks_needed = (LONGLONG)((double)us*(double)dfFreq / 1000000.0);

	/*Step 4  Loop Query the Performance Counter until the counts meet the ticks_needed */
	do
	{
		QueryPerformanceCounter(&litmp);
		QPartStop = litmp.QuadPart;
	} while (QPartStop < (QPartStart + ticks_needed));


}

#endif 

void sys_mdelay(uint32_t ms_delay)
{
#ifdef _WINDOWS
#ifdef INTERFACE_I2C_F3
	Sleep(ms_delay);
#else
	wait_us(ms_delay * 1000);
#endif
#else
	// TODO : To implement millisecond sleep function here.
	usleep(ms_delay * 1000);
#endif
}

/*
* Return the size of the specified file, in bytes.
*/
long file_length(FILE *f)
{
	long pos;
	long end;

	pos = ftell(f);
	fseek(f, 0, SEEK_END);
	end = ftell(f);
	fseek(f, pos, SEEK_SET);

	return end;
}


#ifdef _WINDOWS
bool find_sfs_path = false;
int find_sfs_file(TCHAR * sfs_path, unsigned int sfs_path_len)
{
#define SFS_FILE_NAME _T("partition_system.img")
	TCHAR szDir[MAX_PATH];
	size_t cchExtName;
	cchExtName = (sizeof(SFS_FILE_NAME) / sizeof(TCHAR));
	
	WIN32_FIND_DATA FindData;
	HANDLE hFind;

	GetCurrentDirectory(MAX_PATH, szDir);
	StringCchCat(szDir, sfs_path_len, TEXT("\\*"));

	sfs_path; sfs_path_len;

	hFind = FindFirstFile(szDir, &FindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return -1;
	}

	do
	{
		if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			size_t cchFileName = _tcslen(FindData.cFileName);
			//TCHAR *szExt = FindData.cFileName + cchFileName -cchExtName;
			if (find_sfs_path == false) {
				if (0 == _tcsnicmp(FindData.cFileName, SFS_FILE_NAME, cchExtName))
				{
					StringCchCopy(sfs_path, sfs_path_len, FindData.cFileName);
					find_sfs_path = true;
				}
			}
		}
	} while (FindNextFile(hFind, &FindData) != 0);

	FindClose(hFind);
	return 0;
}
#endif



static void print_img_err_msg(int32_t err_no)
{
	switch (err_no) {
	case -CXERR_NOERR:
		break;
	case -CXERR_MIS_ENDIANNESS:
		fprintf(stderr, "\n\nERROR: CXERR_MIS_ENDIANNESS.\n");
		break;
	case -CXERR_WRITE_FAILED:
		fprintf(stderr, "\n\nERROR: Write Failed.\n");
		break;
	case -CXERR_INVALID_DATA:
		fprintf(stderr, "\n\nERROR: Invalid Data.\n");
		break;
	case -CXERR_CHECKSUM_FAILED:
		fprintf(stderr, "\n\nERROR: Checksum failed.\n");
		break;
	case -CXERR_FAILED:
		fprintf(stderr, "\n\nERROR: CXERR_FAILED.\n");
		break;
	case -CXERR_INVALID_PARAMETER:
		fprintf(stderr, "\n\nERROR: Invalid Parameter.\n");
		break;
	case -CXERR_NOMEM:
		fprintf(stderr, "\n\nERROR: CXERR_NOMEM.\n");
		break;
	case -CXERR_I2CFUN_NOT_SET:
		fprintf(stderr, "\n\nERROR: I2C function is not set.\n");
		break;
	case -CXERR_UPDATE_MEMORY_FAILED:
		fprintf(stderr, "\n\nERROR: Memory Update Failed.\n");
		break;
	case -CXERR_DEVICE_NOT_RESET:
		fprintf(stderr, "\n\nERROR: Device is not Reset.\n");
		break;
	case -CXERR_DEVICE_OUT_OF_CONTROL:
		fprintf(stderr, "\n\nERROR: CXERR_DEVICE_OUT_OF_CONTROL.\n");
		break;
	case -CXERR_UPDATE_EEPROM_FAILED:
		fprintf(stderr, "\n\nERROR: CXERR_UPDATE_EEPROM_FAILED.\n");
		break;
	case -CXERR_INVALID_BOOTLOADER:
		fprintf(stderr, "\n\nERROR: Invalid Bootloader.\n");
		break;
	case -CXERR_INVALID_IMAGE:
		fprintf(stderr, "\n\nERROR: Invalid Firmware Image.\n");
		break;
	case -CXERR_WAITING_RESET_TIMEOUT:
		fprintf(stderr, "\n\nERROR: Time-out waiting for Reset.\n");
		break;
	case -CXERR_LOAD_IMG_TIMEOUT:
		fprintf(stderr, "\n\nERROR: Aborting. Exceeded max number of retries.\n");
		break;
	case -CXERR_STATE_FATAL:
		fprintf(stderr, "\n\nERROR: Fatal error happened. Aborting.\n");
		break;
	case -CXERR_CRC_CHECK_ERROR:
		fprintf(stderr, "\n\nERROR: CRC Check Error.\n");
		break;
	case -CXERR_I2C_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_I2C_ERROR.\n");
		break;
	case -CXERR_BOOT_LOADER_NOT_FOND:
		fprintf(stderr, "\n\nERROR: Bootloader Not Found.\n");
		break;
	case -CXERR_IMAGE_NOT_FOND:
		fprintf(stderr, "\n\nERROR: Firmware Image Not Found.\n");
		break;
	case -CXERR_WAITING_LOADER_TIMEOUT:
		fprintf(stderr, "\n\nERROR: Aborting. timeout waiting for device\n");
		break;
	case -CXERR_NULL_POINTER:
		fprintf(stderr, "\n\nERROR: CXERR_NULL_POINTER\n");
		break;
	case -CXERR_I2C_BLOCK_NR_ERROR:
		fprintf(stderr, "\n\nERROR: Got I2C message error. \n");
		break;
	case -CXERR_SEND_I2C_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_SEND_I2C_ERROR\n");
		break;
	case -CXERR_NO_MORE_DATA:
		fprintf(stderr, "\n\nERROR: CXERR_NO_MORE_DATA\n");
		break;
	case -CXERR_UNSUPPORTED_FLASH_MEMORY:
		fprintf(stderr, "\n\nERROR: Unsupported flash memory!\n");
		break;
	case -CXERR_I2CWRITE_DATA_MISALIGNMENT:
		fprintf(stderr, "\n\nERROR: The data size for I2C write is NOT 4 bytes aligned \n");
		break;
	case -CXERR_I2CWRITE_ADDR_MISALIGNEMNT:
		fprintf(stderr, "\n\nERROR: The I2C write address is NOT 4 bytes aligned \n");
		break;
	case -CXERR_I2CREAD_DATA_MISALIGNMENT:
		fprintf(stderr, "\n\nERROR: The data size for I2C read is NOT 4 bytes aligned \n");
		break;
	case -CXERR_I2CREAD_ADDR_MISALIGNEMNT:
		fprintf(stderr, "\n\nERROR: The I2C read address is NOT 4 bytes aligned \n");
		break;
	case -CXERR_FW_NO_RESPONSE:
		fprintf(stderr, "\n\nERROR: CXERR_FW_NO_RESPONSE\n");
		break;
	case -CXERR_FW_VER_INCORRECT:
		fprintf(stderr, "\n\nERROR: In-correct Firmware Version \n");
		break;
	case -CXERR_FW_CANNOT_BOOT:
		fprintf(stderr, "\n\nERROR: Firmware Cannot Boot\n");
		break;
	case -CXERR_MAGIC_NUM_MISMATCH:
		fprintf(stderr, "\n\nWarning: Magic Number Mis-match\n");
		break;
	case -CXERR_PARTITION_SIZE_MISMATCH:
		fprintf(stderr, "\n\nWarning: Partition Sizes between the Firmware and the Image don't match\n");
		break;
	case -CXERR_SPI_PROTECT_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_SPI_PROTECT_ERROR\n");
		break;
	case -CXERR_SPI_MEM_READ_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_SPI_MEM_READ_ERROR\n");
		break;
	case -CXERR_SPI_CLOSE_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_SPI_CLOSE_ERROR\n");
		break;
	case -CXERR_SPI_READ_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_SPI_READ_ERROR\n");
		break;
	case -CXERR_SPI_WRITE_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_SPI_WRITE_ERROR\n");
		break;
	case -CXERR_SPI_ERASE_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_SPI_ERASE_ERROR\n");
		break;
	case -CXERR_SPI_PARTION_NUM_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_SPI_PARTION_NUM_ERROR\n");
		break;
	case -CXERR_SPI_OPEN_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_SPI_OPEN_ERROR\n");
		break;
	case -CXERR_SPI_GET_ID_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_SPI_GET_SPIID_ERROR\n");
		break;
	case -CXERR_SPI_GET_STATUS_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_SPI_GET_STATUS_ERROR\n");
		break;
	case -CXERR_MGR_RESET_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_MGR_RESET_ERROR\n");
		break;
	case -CXERR_SPI_VERIFY_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_SPI_VERIFY_ERROR\n");
		break;
	case -CXERR_INTERFACE_USB_OPEN_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_INTERFACE_USB_OPEN_ERROR\n");
		break;
	case -CXERR_INTERFACE_I2C_OPEN_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_INTERFACE_I2C_OPEN_ERROR\n");
		break;
	case -CXERR_IMAGE_CRC_CHECK_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_IMAGE_CRC_CHECK_ERROR\n");
		break;
	case -CXERR_DEVICE_CRC_CHECK_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_DEVICE_CRC_CHECK_ERROR\n");
		break;
	case -CXERR_MGR_CAPE_SOFT_RESET:
		fprintf(stderr, "\n\nERROR: CXERR_MGR_CAPE_SOFT_RESET\n");
		break;
	case -CXERR_I2C_SEND_HASH_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_I2C_SEND_HASH_ERROR           \n");
		break;
	case -CXERR_IMAGE_HEADERSIZE_MISMATCH:
		fprintf(stderr, "\n\nERROR: CXERR_IMAGE_HEADERSIZE_MISMATCH     \n");
		break;
	case -CXERR_FW_GET_CURMODE:
		fprintf(stderr, "\n\nERROR: CXERR_FW_GET_CURMODE                \n");
		break;
	case -CXERR_FW_EXEC_FILE:
		fprintf(stderr, "\n\nERROR: CXERR_FW_EXEC_FILE                  \n");
		break;
	case -CXERR_FIND_PERSISTENT_BLK_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_FIND_PERSISTENT_BLK_ERROR     \n");
		break;
	case -CXERR_CONTENT_VERIFY_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_CONTENT_VERIFY_ERROR          \n");
		break;
	case -CXERR_BL_VER_GET_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_BL_VER_GET_ERROR              \n");
		break;
	case -CXERR_PERISISTEN_ERASE_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_PERISISTEN_ERASE_ERROR        \n");
		break;
	case -CXERR_IMAGE_SIZE_TOO_BIG:
		fprintf(stderr, "\n\nERROR: CXERR_IMAGE_SIZE_TOO_BIG            \n");
		break;
	case -CXERR_MALLCO_FAIL:
		fprintf(stderr, "\n\nERROR: CXERR_MALLCO_FAIL                   \n");
		break;
	case -CXERR_FW_VER_GET_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_FW_VER_GET_ERROR\n");
		break;
	case -CXERR_FW_VER_CHECK_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_FW_VER_CHECK_ERROR\n");
		break;
	case -CXERR_FILE_SEEK_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_FILE_SEEK_ERROR\n");
		break;
	case -CXERR_FILE_READ_ERROR:
		fprintf(stderr, "\n\nERROR: CXERR_FILE_READ_ERROR\n");
		break;
	default:
		fprintf(stderr, "\n\nERROR: UNKNOWN ERROR CODE %d\n", err_no);
		break;
	}
}

int32_t AudioSmart_flash_custom_partition(FILE *cus_image, uint32_t cust_part_start, uint32_t cust_part_size)
{
	int32_t err_no = CXERR_NOERR;
	uint32_t block_num = cust_part_start;
	uint32_t end_block = cust_part_start + cust_part_size;
	unsigned char *block_buff; // block pointer (4KB)
	uint32_t len;

	InitShowBlocks(0, cust_part_size); // It will start always from block #0 of size
	block_buff = (unsigned char *)malloc(BLK_SIZE);
	if (block_buff == NULL)
	{
		err_no = -CXERR_MALLCO_FAIL;
		return err_no;
	}
	for (; block_num < end_block && (err_no == CXERR_NOERR); block_num++)
	{
		/* read one block from file */
		if (fread(block_buff, sizeof(char), BLK_SIZE, cus_image) != BLK_SIZE)
		{
			printf("ERROR: Failed to read image block #%u\n", block_num);
			err_no = -CXERR_FILE_READ_ERROR;
			break;
		}
		/* flash one block to device */
		err_no = AudioSmart_flash_a_block_of_cus_partition(block_buff, block_num);
		if (err_no == CXERR_NOERR)
		{
			ShowBlocks((block_num - cust_part_start), cust_part_size);
		}
		else
		{
			printf("ERROR: Failed to download image block #%u err:%d\n", block_num, err_no);
			break;
		}
	}

	return err_no;
}

#ifdef _WINDOWS
int _tmain(int argc, TCHAR * argv[])
#else
int main(int argc, char * argv[])
#endif
{

#define DEFAULT_FW_NAME     "firmware.sfs" //"evk-nebula-generic-7.19.0.0.sfs"
	int err_no = CXERR_NOERR;
	int ret = 0;
#ifdef INTERFACE_I2C_F3
	uint16_t vid = 0;
	uint16_t pid = 0;
	uint32_t i2c_rate = 1200; // 800; // 400;
	bool flag_pid = false;
	bool flag_vid = false;
	bool flag_custom = false;
	bool flag_custom_only = false;
	unsigned int block_sz_cus_partion;
	unsigned int start_blk_cus_partition;
#elif (defined ENABLE_USB_CONTROL)
	unsigned int vid = 0;
	unsigned int pid = 0;
	bool flag_pid = false;
	bool flag_vid = false;
#endif
	char sfs_path[260];
	char custom_path[260];
	unsigned char *image = NULL; // Pointer to image data.
	unsigned long num_image = 0; // the size of image data in byte
	unsigned char *block = NULL; // block pointer (4KB) 
	bool ignore_magic_num = false;
	bool disable_final_prompt = false;
	bool simple_console_print = false;
	FILE *himage = NULL;
	AudioSmart_img_descriptor img_descriptor;
	AudioSmart_spi_flash_descriptor spimem_descriptor;
	const int      little_endian = endianess_is_little();
	unsigned int ver_z;
	unsigned int ver_y;
	unsigned int ver_x;
	unsigned int ver_w;
	unsigned int bl_ver;
	uint32_t c;
	uint32_t block_num;
	int32_t  part_num;

#ifdef ENABLE_I2C_CONTROL
	char *card_name = NULL;
#endif

	do
	{
		fprintf(stdout, "AudioSmart BG firmware update demo tool\n\n");
		if (argc > 1)
		{
			for (int i = 1; i < argc; i++) {
				char *argu_opt = NULL;
#ifdef _WINDOWS
				char argv_local[260] = { 0 };
				WideCharToMultiByte(CP_ACP, 0, argv[i], MAX_PATH, argv_local, MAX_PATH, NULL, NULL);
				argu_opt = argv_local;
#else
				argu_opt = argv[i];
#endif

				if (strcmp(argu_opt, "-c") == 0)
				{
					disable_final_prompt = true;
				}
				else if (strcmp(argu_opt, "-cs") == 0)
				{
					simple_console_print = true;
				}
#ifdef INTERFACE_I2C_F3
				else if (wcscmp(argv[i], L"-v") == 0)
				{
					vid = String2Number(argv[i + 1]);
					printf("Vendor id is:  0x%04x\n", vid);

					flag_vid = true;
				}
				else if (wcscmp(argv[i], L"-p") == 0)
				{
					pid = String2Number(argv[i + 1]);
					printf("Product id is: 0x%04x\n", pid);

					flag_pid = true;
				}
				else if (strcmp(argu_opt, "-u") == 0)
				{
#ifdef _WINDOWS
					WideCharToMultiByte(CP_ACP, 0, argv[i + 1], MAX_PATH, custom_path, MAX_PATH, NULL, NULL);
#else
					strcpy(custom_path, argv[i + 1]);
#endif
					printf("Updating CUSTOM Partition\n");
					flag_custom = true;
				}
#elif (defined ENABLE_USB_CONTROL) //USB_CONTROL
				else if (strcmp(argu_opt, "-p") == 0)
				{
#ifdef _WINDOWS
					swscanf(argv[i + 1], L"%x", &pid);
#else
					sscanf(argv[i + 1], "%x", &pid);
#endif
					flag_pid = true;
				}
				else if (strcmp(argu_opt, "-v") == 0)
				{
#ifdef _WINDOWS
					swscanf(argv[i + 1], L"%x", &vid);
#else
					sscanf(argv[i + 1], "%x", &vid);
#endif
					flag_vid = true;
				}
#endif
				else if (strcmp(argu_opt, "-w") == 0)
				{
					ignore_magic_num = true;
				}
				else if (strcmp(argu_opt, "-s") == 0)
				{
#ifdef _WINDOWS
					WideCharToMultiByte(CP_ACP, 0, argv[i + 1], MAX_PATH, sfs_path, MAX_PATH, NULL, NULL);
#else
					strcpy(sfs_path, argv[i + 1]);
#endif
					find_sfs_path = true;
				}
				else if (strstr(argu_opt, ".img") != NULL)
				{
#ifdef _WINDOWS
					WideCharToMultiByte(CP_ACP, 0, argv[i], MAX_PATH, sfs_path, MAX_PATH, NULL, NULL);
#else
					strcpy(sfs_path, argv[i]);
#endif
					find_sfs_path = true;
				}
				else if (strcmp(argu_opt, "-h") == 0)
				{
					printf("Usage:\n");
					printf("=====\n");
					printf("  bg_upgrade.exe <Options>\n");
					printf("  Options:\n");
					printf("---------\n");
					printf("  -h    :  this help information.\n");
#ifdef ENABLE_USB_CONTROL
					printf("  -v vid:  input vid for opening usb device.\n");
					printf("  -p pid:  input pid for opening usb device.\n");
					printf("  If you don't input vid and pid, usb_upgrade will use the default VID(0x06cb) PID(0x1494)/PID(0xFFFF)\n");
#elif ENABLE_I2C_CONTROL

#endif
					printf("  -w    :  don't check magic number between firmware image file and firmware present in the device before downloading.\n");
					printf("  -s    :  specify the firmware image file.\n");
					printf("  -c    :  Disable final prompt to press a key.\n");
#ifdef INTERFACE_I2C_F3
					printf("  -u    :  Update Custom Partition \n");
#endif
					exit(0);
				}
			}
		}

#ifdef INTERFACE_I2C_F3
		if (find_sfs_path == false && flag_custom == true) {
			flag_custom_only = true;
		}
#endif
#ifdef _WINDOWS
		TCHAR wszSfsPath[MAX_PATH];
#ifdef INTERFACE_I2C_F3
		if (find_sfs_path == false && flag_custom_only == false)
#else
		if (find_sfs_path == false)
#endif
		{
			find_sfs_file(wszSfsPath, MAX_PATH);
			WideCharToMultiByte(CP_ACP, 0, wszSfsPath, MAX_PATH, sfs_path, MAX_PATH, NULL, NULL);
		}
#else
		if (find_sfs_path == false)
			strcpy(sfs_path, DEFAULT_FW_NAME);
#endif


#ifdef ENABLE_USB_CONTROL
		if (flag_pid && flag_vid)
		{
			printf("\n\n\t Use command line VID(0x%x) PID(0x%x)to open USB device.\n\n", vid, pid);
#ifdef INTERFACE_I2C_F3
			// Attempt to open i2c device.
			ret = AudioSmart_i2c_f3_interface_init(SLAVE_ADDRESS, I2C_SUB_ADDR_SIZE, i2c_rate, (int16_t)vid, (int16_t)pid);
			if (ret < 0)
			{
				printf("\n\nERROR: Cannot open I2C interface with F3 device. Error Number: %d\n", ret);
				exit(EXIT_FAILURE);
			}
			else
			{
				printf("PASS:AudioSmart_i2c_f3_interface_init %d  \r\n", ret);
			}
#else
			ret = AudioSmart_usb_interface_init(vid, pid);
			if (ret < 0)
			{
				printf("\nWarning: Fail to open with VID(0x%x) PID(0x%x)\n", vid, pid);
				printf("\nTry with (vid:0x%04x  pid:0xffff) \n", vid);
				if ((ret = AudioSmart_usb_interface_init(vid, 0xFFFF)) < 0)
				{
					printf("FAIL:AudioSmart_usb_interface_init %d  \r\n", ret);
					break;
				}
			}
			else
			{
				printf("PASS:AudioSmart_usb_interface_init %d  \r\n", ret);
			}
#endif
		}
		else
		{
#ifdef INTERFACE_I2C_F3
			printf("\n\n\t Use default VID(0x06cb) PID(0x1494) to open USB device.\n\n");
			// Attempt to open i2c device.
			ret = AudioSmart_i2c_f3_interface_init(SLAVE_ADDRESS, I2C_SUB_ADDR_SIZE, i2c_rate, (int16_t)0x06cb, (int16_t)0x1494);
			if (ret < 0)
			{
				printf("\n\nERROR: Cannot open I2C interface with F3 device. Error Number: %d\n", ret);
				exit(EXIT_FAILURE);
			}
			else
			{
				printf("PASS:AudioSmart_i2c_f3_interface_init %d  \r\n", ret);
			}
#else
			printf("\n\n\t Use default VID(0x06cb) PID(0x1494) to open USB device.\n\n");
			ret = AudioSmart_usb_interface_init(0x06cb, 0x1494);
			if (ret < 0)
			{
				vid = 0x06cb;
				pid = 0xFFFF;
				flag_pid = 1; //set flag as true for reading back the FW version at the end of FW upgrade
				flag_vid = 1;
				printf("\nWarning: Fail to open with VID(0x%04X) PID(0x%04X)\n", 0x06cb, 0x1494);
				printf("\nTry with (vid:0x%04X pid:0x%04X) \n", vid, pid);
				if ((ret = AudioSmart_usb_interface_init(vid, pid)) < 0)
				{
					printf("FAIL:AudioSmart_usb_interface_init %d  \r\n", ret);
					break;
				}
			}
			else
			{
				DUMP("PASS:AudioSmart_usb_interface_init %d  \r\n", ret);
			}
#endif
		}
#elif ENABLE_I2C_CONTROL
		ret = AudioSmart_i2c_interface_init((uint32_t)SLAVE_ADDRESS, (int32_t)2, (int8_t *)card_name);
		if (ret < 0)
		{
			DUMP("FAIL:AudioSmart_i2c_interface_init %d  \r\n", ret);
			break;
		}
		else
		{
			DUMP("PASS:AudioSmart_i2c_interface_init %d  \r\n", ret);
		}
#endif


		/* Note we need to  register the delay functions to initial midlle level api */
		err_no = AudioSmart_mid_level_init(&sys_mdelay); if (err_no != CXERR_NOERR) break;
		if (err_no != CXERR_NOERR) break;

		/* Get Bootloader Version */
		err_no = AudioSmart_bg_get_bl_version(&bl_ver, &ver_z, &ver_y, &ver_x, &ver_w);
		if (err_no != CXERR_NOERR) break;
		fprintf(stdout, "Read device Booltloader version : [%u] built with firmware version : [%u.%u.%u.%u]\n", bl_ver, ver_z,
			ver_y, ver_x, ver_w);


		/* Get FW version before FW upgrade */
		err_no = AudioSmart_bg_get_firmware_version(&ver_z, &ver_y, &ver_x, &ver_w);
		if (err_no != CXERR_NOERR) break;
		fprintf(stdout, "Read device OLD firmware version : [%u.%u.%u.%u]\n", ver_z,
			ver_y, ver_x, ver_w);

		//
		// Update Custom partition
		//
#ifdef INTERFACE_I2C_F3
		if (flag_custom) {
			fprintf(stdout, "Opening Custom Partition Image file:%s\n", custom_path);
			himage = fopen(custom_path, "rb");
			if (himage == NULL)
			{
				fprintf(stderr, "\n\nERROR: Opening of Custom Partition file failed[path:%s].\n", custom_path);
				exit(EXIT_FAILURE);
			}
			num_image = (unsigned long)file_length(himage);
			fprintf(stdout, "Custom Partition file, size:%d bytes   %d blocks\n", num_image, (num_image >> LOG2BLKSIZ));

			/* Validate whether the size of the system image file and the headers are valid */
			fprintf(stdout, "Start Validating whether input image is compatible with the device\n");
			err_no = AudioSmart_start_custom0_partition_flashing(&start_blk_cus_partition, &block_sz_cus_partion, (num_image >> LOG2BLKSIZ));
			if (err_no != CXERR_NOERR)
			{
				break;
			}

			fprintf(stdout, "\n\n\t Custom Partition description:\n");
			fprintf(stdout, "\t---------------------\n");
			fprintf(stdout, "\tPartition Start  : %08xh \n", start_blk_cus_partition);
			fprintf(stdout, "\tPartition Size  : %08xh \n", block_sz_cus_partion);
			fflush(stdout);

			/* Flash Custom Partition */
			fprintf(stdout, "Start Downloading the Custom Partition image to the device\n");
			err_no = AudioSmart_flash_custom_partition(himage, start_blk_cus_partition, block_sz_cus_partion);
			if (err_no != CXERR_NOERR)
			{
				break;
			}

			/* Finishing Custom Partition Flashing */
			fprintf(stdout, "\nFinishing Updating the Custom Partition\n");
			err_no = AudioSmart_bg_upgrade_reset_device();

			if (himage != NULL)
				fclose(himage);
			block = NULL;
			image = NULL;
			num_image = 0;
			spimem_descriptor = {0};
			flag_custom = false;

			if (flag_custom_only)
			{
				break;
			}
		}
#endif

		//
		// Load image from file to memory.
		//
		fprintf(stdout, "Opening Firmware Image file:%s\n", sfs_path);
		himage = fopen(sfs_path, "rb");
		if (himage == NULL)
		{
			fprintf(stderr, "\n\nERROR: Opening of firmware image file failed[path:%s].\n", sfs_path);
			exit(EXIT_FAILURE);
		}
		num_image = (unsigned long)file_length(himage);
		fprintf(stdout, "Firmware Image, size:%d bytes\n", num_image);

		/* Reading a block of data from the image */
		block = (unsigned char *)malloc(BLK_SIZE); if (block == NULL) { err_no = -CXERR_MALLCO_FAIL; break; }
		if (fread(block, sizeof(char), BLK_SIZE, himage) != BLK_SIZE)
		{
			printf("ERROR: Failed to read image block.\n");
			err_no = -CXERR_FILE_READ_ERROR;
			break;
		}

		/* Validate whether the size of the system image file and the headers are valid */
		fprintf(stdout, "Start Validating whether input image is compatible with the device\n");
		err_no = AudioSmart_validate_fw_headers((const uint8_t *)block, num_image, 0, 64, !ignore_magic_num,
			&sys_mdelay, &spimem_descriptor, &img_descriptor);
		if (err_no != CXERR_NOERR)
		{
			break;
		}

		/* Start flashing the various blocks, block by block */
		/* Start transferring image */
		fprintf(stdout, "Start transferring image \n");
		err_no = AudioSmart_start_fw_image_flashing((const uint8_t *)block, &sys_mdelay,
			&img_descriptor, &spimem_descriptor);
		if (err_no != CXERR_NOERR)
		{
			break;
		}

		// Dump the system environment
		fprintf(stdout, "\r\n\r\n\r\n");
		fprintf(stdout, "\tNew Firmware description:\n");
		fprintf(stdout, "\t------------------------\n");
		fprintf(stdout, "\tFirmware version    : %u.%u.%u.%u \n", img_descriptor.fw_img_version[0],
			img_descriptor.fw_img_version[1],
			img_descriptor.fw_img_version[2],
			img_descriptor.fw_img_version[3]);
		fprintf(stdout, "\tImage size          : %lxh bytes\n", num_image);

		fprintf(stdout, "\n\n\tSPI Flash description:\n");
		fprintf(stdout, "\t---------------------\n");
		fprintf(stdout, "\tSPI memory has ID   : %08xh \n", (uint32_t)spimem_descriptor.id);
		fprintf(stdout, "\tpartition %d         : Active\n", spimem_descriptor.active_partition);
		fprintf(stdout, "\tpartition %d         : Inactive\n", spimem_descriptor.updating_partition);
		fprintf(stdout, "\n\nUpdating partition [%d]\n", spimem_descriptor.updating_partition);
		fflush(stdout);

		/*Transferring image block by block */
		InitShowBlocks(0, spimem_descriptor.sys_part[spimem_descriptor.updating_partition].num_blks);
		//
// Load image from file to memory and flash to device block by block starting from the second block

		for (c = 1; c < spimem_descriptor.sys_part[spimem_descriptor.updating_partition].num_blks && (err_no == CXERR_NOERR); c++)
		{
			block_num = spimem_descriptor.sys_part[spimem_descriptor.updating_partition].blk_start + c;
			/* read one block from file */
			if (fread(block, sizeof(char), BLK_SIZE, himage) != BLK_SIZE)
			{
				printf("ERROR: Failed to read image block.\n");
				err_no = -CXERR_FILE_READ_ERROR;
				break;
			}
			/* flash one block to device */
			err_no = AudioSmart_flash_a_block(block, &img_descriptor, &spimem_descriptor);

			if (err_no < 0)
			{
				if (simple_console_print) {
					ShowBlocksSimple(c, spimem_descriptor.sys_part[spimem_descriptor.updating_partition].num_blks);
				} 
				else {
					ShowBlocks(c, spimem_descriptor.sys_part[spimem_descriptor.updating_partition].num_blks);
				}
				break;
			}
			else
			{
				if (simple_console_print) {
					ShowBlocksSimple(c, spimem_descriptor.sys_part[spimem_descriptor.updating_partition].num_blks);
				}
				else
				{
					ShowBlocks(c, spimem_descriptor.sys_part[spimem_descriptor.updating_partition].num_blks);
				}
			}
		}
		if (himage)
			fclose(himage);

		// Flashing the first block
		himage = fopen(sfs_path, "rb");
		if (himage == NULL)
		{
			fprintf(stderr, "\n\nERROR: Opening of firmware image file failed[path:%s].\n", sfs_path);
			exit(EXIT_FAILURE);
		}
		block_num = spimem_descriptor.sys_part[spimem_descriptor.updating_partition].blk_start;
		/* read first block from file */
		if (fread(block, sizeof(char), BLK_SIZE, himage) != BLK_SIZE)
		{
			printf("ERROR: Failed to read image block.\n");
			err_no = -CXERR_FILE_READ_ERROR;
			break;
		}
		/* flash first block to device */
		if (err_no != CXERR_NOERR) break;

		/*End of transferring image,will active the updating partition*/
		err_no = AudioSmart_end_fw_image_flashing((const uint8_t *)block, &sys_mdelay,
			&img_descriptor, &spimem_descriptor);
		if (err_no != CXERR_NOERR) break;

		fprintf(stdout, "\n\n");
		fprintf(stdout, "Mark partition [%d] as active...\n", spimem_descriptor.updating_partition);
		fprintf(stdout, "Mark partition [%d] as inactive...\n", 1 - spimem_descriptor.updating_partition);
		fprintf(stdout, "\nImage transfer completed successfully!\n");
		fflush(stdout);

		if (err_no == CXERR_NOERR)
		{
			fprintf(stdout, "\nFirmware Downloaded Successfully.");
		}
		else
		{
			fprintf(stdout, "\nFailed! error code = %d\n\n", err_no);
		}

		fprintf(stdout, "\nSoft Reset the Device - 1st time.");
		err_no = AudioSmart_bg_upgrade_reset_device();


		err_no = AudioSmart_i2c_wait_until_bootup(&sys_mdelay); /*waiting for the device boot-up*/
#ifdef ENABLE_I2C_CONTROL
		if (err_no == 0)
		{
			fprintf(stdout, "\nFirmware boot up Sucessfully !\n");
			/*reopen the interface to query the FW version */
			AudioSmart_i2c_interface_close();
			ret = AudioSmart_i2c_interface_init((uint32_t)SLAVE_ADDRESS, (int32_t)2, (int8_t *)card_name);
			if (ret < 0)
			{
				DUMP("FAIL:AudioSmart_i2c_interface_init %d  \r\n", ret);
				break;
			}
			else
			{
				DUMP("PASS:AudioSmart_i2c_interface_init %d  \r\n", ret);
			}
		}
		else
		{
			printf("\nFirmware boot up Fail !\n");
			break;
		}
#endif 


#ifdef ENABLE_USB_CONTROL
		if (err_no == 0)
		{
			fprintf(stdout, "\nFirmware boot up Sucessfully !\n");
		}
		else
		{
			printf("\nFirmware boot up Fail !\n");
			break;
		}
#endif


		err_no = AudioSmart_bg_get_firmware_version(&ver_z, &ver_y, &ver_x, &ver_w);
		if (err_no != CXERR_NOERR) break;

		part_num = AudioSmart_bg_get_current_sys_partition_number();
		if (part_num < 0) break;

		fprintf(stdout, "Read device NEW system partition %d firmware version : [%u.%u.%u.%u]\n", part_num, ver_z, ver_y, ver_x, ver_w);
		fprintf(stdout, "Read device NEW firmware version : [%u.%u.%u.%u]\n", ver_z, ver_y, ver_x, ver_w);
		if ((part_num == spimem_descriptor.updating_partition) &&
			(ver_z == img_descriptor.fw_img_version[0]) &&
			(ver_y == img_descriptor.fw_img_version[1]) &&
			(ver_x == img_descriptor.fw_img_version[2]) &&
			(ver_w == img_descriptor.fw_img_version[3]))
		{
			fprintf(stdout, "\nFirmware boot up Sucessfully !\n");
		}
		else {
			fprintf(stdout, "\nFirmware didnt' boot up Sucessfully !\n");
			err_no = CXERR_FAILED;
			break;
		}

		fprintf(stdout, "\nSoft Reset the Device - 2nd time for cleanup.");
		AudioSmart_bg_upgrade_reset_device(); if (err_no != CXERR_NOERR) break;
		err_no = CXERR_NOERR;

		break;
	} while (0);

	fprintf(stdout, "\n\n");
	ShowResult(err_no == CXERR_NOERR);
	fprintf(stdout, "\n\n");
	fflush(stdout);

#if (defined ENABLE_I2C_CONTROL) || (defined INTERFACE_I2C_F3)
	AudioSmart_i2c_interface_close();
#elif  ENABLE_USB_CONTROL
	AudioSmart_usb_interface_close();
#endif

	if (image)
		free(image);
	if (block)
		free(block);
	if (himage)
		fclose(himage);
	print_img_err_msg(err_no);
	if (!disable_final_prompt)
	{
		printf("\nPress any key to exit\n");
		getchar();
	}
	return err_no;
}

#ifdef INTERFACE_I2C_F3
uint16_t String2Number(wchar_t *string)
{
	uint16_t num = 0;
	if (wcsstr(string, L"0x") == NULL)
	{
		num = _wtoi(string);
	}
	else
	{
		num = wcstol(&string[2], NULL, 16);
	}
	return num;
}
#endif
