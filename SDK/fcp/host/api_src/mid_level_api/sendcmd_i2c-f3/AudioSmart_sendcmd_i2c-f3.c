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

  File Name: AudioSmart_sendcmd_usb.c

------------------------------------------------------------------------------*/
#include "AudioSmart_definitions.h"
#include "AudioSmart_sendcmd.h"
#include "AudioSmart_usb_interface_api.h"
#include <string.h>

#define REPLY_POLL_INTERVAL_MSEC  1
#define REPLY_POLL_TIMEOUT_MSEC   5000
#define READY_POOLING_INTERVAL    10 /*10 ms*/
#define READY_POOLING_LOOP        (6000/READY_POOLING_INTERVAL) /*6 s*/
#define BADBEFF                   0X8BADD00D

typedef void (*typedef_delay_fn_ptr)(uint32_t);
typedef_delay_fn_ptr g_delay_fn_ptr = NULL; /*global delay function pointer*/

int32_t AudioSmart_mid_level_init(void (*delay_fn_ptr)(uint32_t delay_in_ms))
{
  if (delay_fn_ptr)
  {
    g_delay_fn_ptr = delay_fn_ptr;
    return SENDCMD_RC_SUCCESS;
  }
  else
  {
    return SENDCMD_RC_INVALID_ARG_NUM;
  }
}

int32_t AudioSmart_mid_sendcmd(uint32_t *cmd_ptr)
{
  Command *cmd = (Command *)cmd_ptr;
  int32_t err = SENDCMD_RC_INVALID_ARG_NUM;

  int32_t elapsed_ms = 0;

  if (g_delay_fn_ptr == NULL)
  {
    return SENDCMD_RC_INVALID_ARG_NUM;
  }

  int32_t num_32b_words = cmd->head.num_32b_words;
  cmd->head.num_32b_words = (cmd->head.command_id & CMD_GET(0)) ? MAX_COMMAND_SIZE : num_32b_words;
  cmd->head.command_id = CMD_FWD(cmd->head.command_id);

  while ((elapsed_ms < REPLY_POLL_TIMEOUT_MSEC) && (err < 0)) //if there is an error with sending to USB retry every 1ms up to 2 seconds before giving up.
  {
    if ((err = AudioSmart_usb_interface_write((unsigned int *)cmd, sizeof(Command))) < 0)
    {
      (*g_delay_fn_ptr)(REPLY_POLL_INTERVAL_MSEC);
      //sys_mdelay(REPLY_POLL_INTERVAL_MSEC);
      elapsed_ms += REPLY_POLL_INTERVAL_MSEC;
    }
  }

  if (err < 0) // if error after 2000 tries give up.
    return (SENDCMD_RC_FAILED_SEND);

  elapsed_ms = 0;

  while (elapsed_ms < REPLY_POLL_TIMEOUT_MSEC)
  {
    // only read the first word and check the reply bit
    if ((err = AudioSmart_usb_interface_read((uint32_t *)cmd, sizeof(Command))) < 0)
      return (SENDCMD_RC_FAILED_SEND);

    if (cmd->head.reply == 1)
    {
      if (cmd->head.num_32b_words >= 0)
        return (SENDCMD_RC_SUCCESS);
      else
        return (SENDCMD_RC_CMD_ERROR);
    }

    (*g_delay_fn_ptr)(REPLY_POLL_INTERVAL_MSEC);
    //sys_mdelay(REPLY_POLL_INTERVAL_MSEC);

    elapsed_ms += REPLY_POLL_INTERVAL_MSEC;
  }
  return (SENDCMD_RC_REPLY_TIMEOUT);
}

int32_t AudioSmart_mid_sendcmd_async(uint32_t *cmd, uint32_t wait_timeout, sendcmd_cb cb_func){
  //these interfaces don't support non-blocking API, so ignore these parameters
  return AudioSmart_mid_sendcmd(cmd);
}

#define MAX_OFFSET_LEN 4
#define MAX_PACKET_SIZE (40 + MAX_OFFSET_LEN)
#define MAX_READ_PACKET_SIZE 40
#define I2C_SLAVE_ADDR 0X41
#define I2C_SUB_ADDR_SIZE 2

int32_t raw_i2c_read(int32_t address, uint32_t *buffer, int32_t size, void(*delay_fn_ptr)(uint32_t delay_in_ms))
{
  int nRet=0;
  uint8_t SubAddr[4]={0};
  uint32_t SubAddress=0;
  int32_t i;
  Command Cmd = {0};
  int32_t elapsed_ms = 0;
  int32_t err = SENDCMD_RC_INVALID_ARG_NUM;

  address &= 0xFFFFFF;
  uint8_t offset[MAX_OFFSET_LEN] = {0};

  int32_t len = size * 4;

  if (delay_fn_ptr == NULL)
  {
    return SENDCMD_RC_INVALID_ARG_NUM;
  }

  if (len > MAX_READ_PACKET_SIZE)
  {
    return -1;
  }

  for (i = 0; i < I2C_SUB_ADDR_SIZE; i++)
    offset[i] = (address >> ((I2C_SUB_ADDR_SIZE - 1 - i) * 8)) & 0xFF;

  memcpy(&SubAddr, (uint8_t *)offset, I2C_SUB_ADDR_SIZE);
  for (i = 0; i < I2C_SUB_ADDR_SIZE; i++) {
    SubAddress = SubAddress | *(offset+i);
    if((i+1)<I2C_SUB_ADDR_SIZE)
      SubAddress = SubAddress<<8;
  }

  Cmd.head.num_32b_words = MAX_COMMAND_SIZE;
  Cmd.head.command_id = CMD_GET(MCU_RAW_I2C);
  Cmd.head.mgr_module_id = ID_CTRL;
  Cmd.data[0] = I2C_SLAVE_ADDR | (I2C_SUB_ADDR_SIZE<<8) | (400<<16);//device address b'0:7; adr_len b'8:10; speed b'16:31
  Cmd.data[1] = SubAddress;//subaddress
  Cmd.data[2] = len;//I2C read in bytes

  while ((elapsed_ms < REPLY_POLL_TIMEOUT_MSEC) && (err < 0)) //if there is an error with sending to USB retry every 1ms up to 2 seconds before giving up.
  {
    if ((err = AudioSmart_usb_interface_write((unsigned int *)&Cmd, sizeof(Command))) < 0)
    {
      (*delay_fn_ptr)(REPLY_POLL_INTERVAL_MSEC);
      //sys_mdelay(REPLY_POLL_INTERVAL_MSEC);
      elapsed_ms += REPLY_POLL_INTERVAL_MSEC;
    }
  }

  if (err < 0) // if error after 2000 tries give up.
    return (SENDCMD_RC_FAILED_SEND);

  elapsed_ms = 0;

  while (elapsed_ms < REPLY_POLL_TIMEOUT_MSEC)
  {
    // only read the first word and check the reply bit
    if ((err = AudioSmart_usb_interface_read((uint32_t *)&Cmd, sizeof(Command))) < 0)
      return (SENDCMD_RC_FAILED_SEND);

    if (Cmd.head.reply == 1)
    {
      if (Cmd.head.num_32b_words >= 0)
      {
        memcpy((void *)buffer, (void *)&(Cmd.data), Cmd.head.num_32b_words * 4);
        return (SENDCMD_RC_SUCCESS);
      }
      else
        return (SENDCMD_RC_CMD_ERROR);
    }

    (*delay_fn_ptr)(REPLY_POLL_INTERVAL_MSEC);
    //sys_mdelay(REPLY_POLL_INTERVAL_MSEC);

    elapsed_ms += REPLY_POLL_INTERVAL_MSEC;
  }
  return (SENDCMD_RC_REPLY_TIMEOUT);
}

int32_t AudioSmart_i2c_wait_until_bootup(void (*delay_fn_ptr)(uint32_t delay_in_ms))
{
  int32_t check_loop = READY_POOLING_LOOP;
  int32_t i;
  uint32_t val = 0;
  int32_t err_no = CXERR_NOERR;

  for (i = 0; i < check_loop; i++)
  {
    (*delay_fn_ptr)(READY_POOLING_INTERVAL);
    err_no = raw_i2c_read(4, &val, 1, delay_fn_ptr);
    if (BADBEFF == val)
    {
      err_no = CXERR_NOERR;
      break;
    }
  }
  if (BADBEFF != val)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "\nERROR: Failed to reboot!\n");
    err_no = -CXERR_FW_CANNOT_BOOT;
  }
  return err_no;
}
