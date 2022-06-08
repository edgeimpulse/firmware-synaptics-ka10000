/*------------------------------------------------------------------------------
  Copyright (C) 2019 Synaptics Inc.
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

  File Name: AudioSmart_i2c_dev.c

------------------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>

#include "AudioSmart_i2c_interface_api.h"

#if defined (CONFIG_TARGET_OS_ANDROID) || (CONFIG_TARGET_OS_LINUX)
#include <dirent.h>
#endif

#if defined CONFIG_TARGET_OS_ANDROID
  #define LOG_TAG "AudioSmart_i2c_interface_api"
  #define LOG_NDEBUG 0  //0 turns debugging on!
#endif
#define SLAVE_ADDRESS            0x41
#define ADDRESS_LENGTH           2
#define DEF_MAX_I2C_WRITE_LEN    120 /*MUST equal to or bigger than the 3rd parameters of SetupI2cWriteMemCallback.*/
#define DEF_MAX_I2C_READ_LEN     16   /*MUST equal to or bigger than the 3rd parameters of SetupI2cReadMemCallback.*/
#define REPLY_POLL_INTERVAL_MSEC 1

#if defined (CONFIG_TARGET_OS_ANDROID) || (CONFIG_TARGET_OS_LINUX)
enum FIND_GPIO_PATH
{
    FIND_GPIO_PAHT_FALSE = 0,    
    FIND_GPIO_PAHT_TRUE = 1,
};
#endif

static int32_t  g_hI2cDevice;
static int8_t   g_devname[20] = "/dev/i2c-1";
static int8_t   *g_i2c_buf = NULL;
static uint8_t  g_bChipAddress = 0; /*Specify the i2c chip address*/
static uint8_t  g_bAddressOffset = 0;
static uint32_t g_cbMaxI2cWrite;
static uint32_t g_cbMaxI2cRead;

#if defined (CONFIG_TARGET_OS_ANDROID) || (CONFIG_TARGET_OS_LINUX)
static FIND_GPIO_PATH is_find_reset_gpio_path = FIND_GPIO_PAHT_FALSE;
#ifndef I2C_BUS_DSP_FIND_I2C_NAME_AUTOMATICLLY
#define I2C_BUS_DSP_REST_PATH "/sys/bus/i2c/devices/3-0041/reset_dsp"
#endif
struct syna_rest_gpio_buffer {
	char   *path;
	int  path_len;
};
struct syna_rest_gpio_buffer rest_gpio_buffer;
#endif

#ifdef I2C_DUMP
static int32_t debugflag = 1;
#else
static int32_t debugflag = 0;
#endif
#if defined (CONFIG_TARGET_OS_ANDROID) || (CONFIG_TARGET_OS_LINUX)
FIND_GPIO_PATH AudioSmart_find_i2c_rest_pin_path(struct syna_rest_gpio_buffer *st_rest_gpio_buffer);
#endif
int32_t AudioSmart_i2c_interface_init (uint32_t i2c_address, int32_t offset_len, int8_t *card_name)
{
    int32_t i2cdev = 0;
    int8_t *devname = NULL;

    g_bChipAddress = i2c_address;
    g_bAddressOffset = offset_len;
    g_cbMaxI2cWrite = DEF_MAX_I2C_WRITE_LEN;
    g_cbMaxI2cRead = DEF_MAX_I2C_READ_LEN;

    if (card_name != NULL)
    {
        devname = card_name;
    }
    else
    {
        devname = g_devname;
    }
    i2cdev = open((const char *)devname, O_RDWR);

    logInfo("DEV I2C: open i2c device from %s\n", devname);

    if (i2cdev < 0)
    {
        logError("I2c device [%s] is not present.\n", devname);
        return -ENODEV;
    }
#if defined (CONFIG_TARGET_OS_ANDROID) || (CONFIG_TARGET_OS_LINUX)	
#ifdef I2C_BUS_DSP_FIND_I2C_NAME_AUTOMATICLLY
	is_find_reset_gpio_path = AudioSmart_find_i2c_rest_pin_path(&rest_gpio_buffer);
#else
	rest_gpio_buffer.path = I2C_BUS_DSP_REST_PATH;
	rest_gpio_buffer.path_len = strlen(I2C_BUS_DSP_REST_PATH);
	is_find_reset_gpio_path = FIND_GPIO_PAHT_TRUE;
#endif	
#endif
    g_hI2cDevice = i2cdev;
    return 0;
}

int32_t AudioSmart_i2c_interface_write (int32_t address, uint32_t *data, int32_t size)
{
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg msg[1];
    uint8_t *write_buf = (uint8_t *)data;
    uint32_t write_len = size * 4;
    int32_t fd         = g_hI2cDevice;
    int32_t ret        = 0;
    int32_t i;

    if (g_i2c_buf == NULL)
    {
        g_i2c_buf = (int8_t *)malloc(g_cbMaxI2cWrite + g_bAddressOffset);
        if (g_i2c_buf == NULL)
        {
            return -1;
        }
    }

    for (i = 0; i < g_bAddressOffset; i++)
        g_i2c_buf[i] = (uint8_t)(address >> ((g_bAddressOffset - i - 1) * 8));

    memcpy(&g_i2c_buf[i], write_buf, write_len);

    msg[0].addr = g_bChipAddress;
    msg[0].flags = 0; //7 bits
    msg[0].len = write_len + g_bAddressOffset;
    msg[0].buf = (uint8_t *)g_i2c_buf;

    packets.msgs = msg;
    packets.nmsgs = 1;

    if (debugflag)
    {
        logInfo("I2C: <START> %02X", g_bChipAddress << 1);
        for (i = 0; i < msg[0].len; i++)
        {
            logInfo(" %02X", (0xff & g_i2c_buf[i]));
        }
        logInfo(" <STOP>\n");
    }

    ret = ioctl(fd, I2C_RDWR, &packets);

    if (ret < 0)
    {
        logError(" Failed: i2c_write error = %d\n", ret);
        return -1;
    }
    return 0;
}

int32_t AudioSmart_i2c_interface_read (int32_t address, uint32_t *data, int32_t size)
{
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg msg[2];
    uint8_t *read_buf = (uint8_t *)data;
    uint32_t read_len = size * 4;
    int32_t fd        = g_hI2cDevice;
	uint8_t be_sub_addr[4];
    int32_t ret = 0;
    uint8_t i;

    msg[0].addr = g_bChipAddress;
    msg[0].flags = 0; //7 bits
    msg[0].len = g_bAddressOffset;
    msg[0].buf = &be_sub_addr[0];
	

    for (i = 0; i < g_bAddressOffset; i++)
        msg[0].buf[i] = (uint8_t)(address >> ((g_bAddressOffset - i - 1) * 8));

    msg[1].addr = g_bChipAddress;
    msg[1].flags = I2C_M_RD; //read flag
    msg[1].len = read_len;
    msg[1].buf = read_buf;

    packets.msgs = msg;
    packets.nmsgs = 2;

    ret = ioctl(fd, I2C_RDWR, &packets);
	
    if (debugflag)
    {
        logInfo("I2C: <START> %02X", g_bChipAddress << 1);
        for (i = 0; i < msg[0].len; i++)
            logInfo(" %02X", msg[0].buf[i]);

        logInfo(" <RESTART> %02X [", g_bChipAddress << 1 | 1);

        for (i = 0; i < msg[1].len; i++)
        {
            logInfo("%02X", msg[1].buf[i]);
        }
        logInfo("] <STOP>\n");
    }

    if (ret < 0)
    {
        //logError(" Failed: i2c_read error = %d\n", ret);
        return -1;
    }
    return 0;
}

int32_t AudioSmart_i2c_interface_write_bytes (int32_t address, uint8_t *data, int32_t size)
{
  struct i2c_rdwr_ioctl_data packets;
  struct i2c_msg msg[1];
  uint8_t *write_buf = (uint8_t *)data;
  uint32_t write_len = (uint32_t)size;
  int32_t fd         = g_hI2cDevice;
  int32_t ret        = 0;
  int32_t i;

  if (g_i2c_buf == NULL)
  {
      g_i2c_buf = (int8_t *)malloc(g_cbMaxI2cWrite + g_bAddressOffset);
      if (g_i2c_buf == NULL)
      {
          return -1;
      }
  }

  for (i = 0; i < g_bAddressOffset; i++)
      g_i2c_buf[i] = (uint8_t)(address >> ((g_bAddressOffset - i - 1) * 8));

  memcpy(&g_i2c_buf[i], write_buf, write_len);

  msg[0].addr = g_bChipAddress;
  msg[0].flags = 0; //7 bits
  msg[0].len = write_len + g_bAddressOffset;
  msg[0].buf = (uint8_t *)g_i2c_buf;

  packets.msgs = msg;
  packets.nmsgs = 1;

  if (debugflag)
  {
      logInfo("I2C: <START> %02X", g_bChipAddress << 1);
      for (i = 0; i < msg[0].len; i++)
      {
          logInfo(" %02X", (0xff & g_i2c_buf[i]));
      }
      logInfo(" <STOP>\n");
  }

  ret = ioctl(fd, I2C_RDWR, &packets);

  if (ret < 0)
  {
      logError(" Failed: i2c_write error = %d\n", ret);
      return -1;
  }
  return 0;

}

int32_t AudioSmart_i2c_interface_read_bytes (int32_t address, uint8_t *data, int32_t size)
{
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg msg[2];
    uint8_t *read_buf = (uint8_t *)data;
    uint32_t read_len = (uint32_t)size;
    int32_t fd        = g_hI2cDevice;
    uint8_t be_sub_addr[4];
    int32_t ret = 0;
    uint8_t i;

    msg[0].addr = g_bChipAddress;
    msg[0].flags = 0; //7 bits
    msg[0].len = g_bAddressOffset;
    msg[0].buf = &be_sub_addr[0];

    for (i = 0; i < g_bAddressOffset; i++)
        msg[0].buf[i] = (uint8_t)(address >> ((g_bAddressOffset - i - 1) * 8));

    msg[1].addr = g_bChipAddress;
    msg[1].flags = I2C_M_RD; //read flag
    msg[1].len = read_len;
    msg[1].buf = read_buf;

    packets.msgs = msg;
    packets.nmsgs = 2;

    ret = ioctl(fd, I2C_RDWR, &packets);

    if (debugflag)
    {
        logInfo("I2C: <START> %02X", g_bChipAddress << 1);
        for (i = 0; i < msg[0].len; i++)
            logInfo(" %02X", msg[0].buf[i]);

        logInfo(" <RESTART> %02X [", g_bChipAddress << 1 | 1);

        for (i = 0; i < msg[1].len; i++)
        {
            logInfo("%02X", msg[1].buf[i]);
        }
        logInfo("] <STOP>\n");
    }

    if (ret < 0)
    {
        logError(" Failed: i2c_read error = %d\n", ret);
        return -1;
    }
    return 0;
}

int32_t AudioSmart_i2c_interface_close (void)
{
    close(g_hI2cDevice);

    if (g_i2c_buf)
    {
        free(g_i2c_buf);
        g_i2c_buf = NULL;
    }
#if defined (CONFIG_TARGET_OS_ANDROID) || (CONFIG_TARGET_OS_LINUX)	
#ifdef I2C_BUS_DSP_FIND_I2C_NAME_AUTOMATICLLY
	if(rest_gpio_buffer.path != NULL) {
		free(rest_gpio_buffer.path);
		rest_gpio_buffer.path = NULL;
	}
#endif
	rest_gpio_buffer.path_len = 0;
#endif

    return 0;
}
#if defined (CONFIG_TARGET_OS_ANDROID) || (CONFIG_TARGET_OS_LINUX)
#ifdef I2C_BUS_DSP_FIND_I2C_NAME_AUTOMATICLLY
FIND_GPIO_PATH AudioSmart_find_i2c_rest_pin_path(struct syna_rest_gpio_buffer *st_rest_gpio_buffer){
	FIND_GPIO_PATH ret = FIND_GPIO_PAHT_FALSE;
	int retValue = 0, name_path_len = 0;
	struct dirent *p;
	char name_path[256] = {0};
	char fcontent[20] = {0}, *match_addr = NULL;
	FILE *fp;
	
	DIR *dirp=opendir(I2C_BUS_DEIVCE_PATH);
	if(dirp==NULL){
		logError("%s opendir %s Error %s \n",__func__,I2C_BUS_DEIVCE_PATH,strerror(errno));
		return FIND_GPIO_PAHT_FALSE;
	}
	while((p=readdir(dirp))!=NULL){
		match_addr = strstr(p->d_name,I2C_BUS_SLAVE_ADDR_NAME_DEC);
		if(match_addr != NULL) {
			name_path_len = (strlen(I2C_BUS_DEIVCE_PATH) + strlen(p->d_name) + strlen("/") +strlen(I2C_BUS_CODEC_NAME_T));
			if(name_path_len > sizeof(name_path)) {
				logError("%s Error: name_path_len %d > sizeof(name_path) %d \n",__func__,name_path_len,sizeof(name_path));
				goto EXIT_OUIT;
			}
			sprintf(name_path,"%s%s/%s",I2C_BUS_DEIVCE_PATH,p->d_name,I2C_BUS_CODEC_NAME_T);

			fp=fopen(name_path,"r");
			if(fp==NULL){
				logError("%s opendir %s Error %s \n",__func__,name_path,strerror(errno));
				goto EXIT_OUIT;
			}
			fread(fcontent,1,strlen(I2C_DEIVCE_CODEC_NAME),fp);
			retValue = memcmp(fcontent,I2C_DEIVCE_CODEC_NAME,strlen(I2C_DEIVCE_CODEC_NAME));
			if(retValue == 0) {
				st_rest_gpio_buffer->path_len = (strlen(I2C_BUS_DEIVCE_PATH) + strlen(p->d_name) + strlen("/") + strlen(I2C_BUS_DSP_REST_NAME));
				st_rest_gpio_buffer->path = (char *) calloc(1,st_rest_gpio_buffer->path_len);
				if(st_rest_gpio_buffer->path == NULL) {
					logError(" %s calloc error path_len  %d\n",__func__,st_rest_gpio_buffer->path_len);
					goto EXIT_OUIT;
				}
				sprintf(st_rest_gpio_buffer->path,"%s%s/%s",I2C_BUS_DEIVCE_PATH,p->d_name,I2C_BUS_DSP_REST_NAME);
				ret = FIND_GPIO_PAHT_TRUE;
				break;
			}
			if(fp!=NULL) {
				fclose(fp);
				fp = NULL;
			}
			memset(name_path,0,sizeof(name_path));
		}
	}

	logInfo("%s rest_gpio path %s \n",__func__,st_rest_gpio_buffer->path);
EXIT_OUIT:
	if(ret == FIND_GPIO_PAHT_FALSE) {
		if(st_rest_gpio_buffer->path != NULL) {
			free(st_rest_gpio_buffer->path);
			st_rest_gpio_buffer->path = NULL;
		}
	}
	if(fp!=NULL) {		
		fclose(fp);
		fp = NULL;
	}
	
	if(dirp!=NULL) {
		closedir(dirp);
		dirp = NULL;
	}
	return ret;
}
#endif
int set_reset_pin(int32_t high) {
	int ret = 0;
	FILE *fp;
	char fcontent[1] = {0};
	
	if(rest_gpio_buffer.path == NULL) {
		logError("%s reset_gpio_buffer == NULL \n",__func__);
		ret = -ENOENT;
		return ret;
	}

	if(rest_gpio_buffer.path_len == 0) {
		logError("%s reset_gpio path_len == 0 \n",__func__);
		ret = -ENOENT;
		return ret;
	}

	if(high == 1) {
		fcontent[0] = 1; 
	} else if (high == 0) {
		fcontent[0] = 0; 
	}
	logInfo("%s rest_gpio_buffer path %s \n",__func__,rest_gpio_buffer.path);
	
	fp=fopen(rest_gpio_buffer.path,"w");
	if(fp==NULL){
		logError("%s opendir %s Error %s errno %d \n",__func__,rest_gpio_buffer.path,strerror(errno),errno);
		ret = -errno;
		return ret;
	}
	//logInfo("%s fcontent %s strlen(fcontent) %d\n",__func__,fcontent,strlen(fcontent));

	ret = fwrite((void *)fcontent,strlen(fcontent),1,fp);
	if(ret != 1) {
		logError("%s fw fwrite failed \n",__func__);
		ret= -1;
	}else {
		logInfo("%s fw is Succussfully. \n",__func__);
		ret = 1;
	}
	
	fclose(fp);
	
	return ret;
}
#endif

int32_t AudioSmart_set_gpio_reset(int32_t high) {
	int32_t ret = 0;
#if defined (CONFIG_TARGET_OS_ANDROID) || (CONFIG_TARGET_OS_LINUX)
	
	if(is_find_reset_gpio_path == FIND_GPIO_PAHT_FALSE) {
		logError("%s can't find i2c rest path in %s  \n",__func__,I2C_BUS_DEIVCE_PATH);
		ret = -ENOENT;
		return ret;
	}
	if(rest_gpio_buffer.path == NULL) {
		logError("%s g_rest_gpio_path == NULL \n",__func__);
		ret = -ENOENT;
		return ret;
	}

	if(rest_gpio_buffer.path_len == 0) {
		logError("%s path_len == 0 \n",__func__);
		ret = -ENOENT;
		return ret;
	}

	ret = set_reset_pin(high);
#else
/*Need user to set gpio high or low voltage
	if (high)
	   set set gpio high voltage;
	 else
	   set set gpio low voltage;
	*/
#endif
	return ret;
}
int32_t AudioSmart_i2c_set_speed(bool b400Khz){
	int32_t ret = 0;
	return ret;
}

