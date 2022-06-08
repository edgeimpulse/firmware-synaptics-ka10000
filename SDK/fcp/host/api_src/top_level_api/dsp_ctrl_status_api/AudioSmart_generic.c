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

  File Name: AudioSmart_generic.c

------------------------------------------------------------------------------*/
#include "AudioSmart_definitions.h"
#include "AudioSmart_generic.h"
#include "AudioSmart_sendcmd_common.h"

#define hex2id0(id)  ((id>>8 )&0x3f)+0x20
#define hex2id1(id)  ((id>>14)&0x3f)+0x20
#define hex2id2(id)  ((id>>20)&0x3f)+0x20
#define hex2id3(id)  ((id>>26)&0x3f)+0x20
#define hex2id(id)   (((hex2id3(id))<<24) |((hex2id2(id))<<16) |((hex2id1(id))<<8) |(hex2id0(id)))

#define CAPT_MIC_IN_MODULE_ID		4
#define MODULE_CMD_ACTIVE			0x0007
#define AMPLIFIER_CMD_VOLUME_LIST   0x0040

#define Q31_T(val)					(((val)>=  1.f)?((1U<<31)-1):(((val)<  -1.f)?(1<<31U):((int32_t)((val)*2*(float)(1<<30)))<<0))
#define Q23_T(val)					(((val)>=  1.f)?((1U<<31)-1):(((val)<  -1.f)?(1<<31U):((int32_t)((val)*(float)(1<<23)))<<8))
#define Q8_23_T(val)				(((val)>=256.f)?((1U<<31)-1):(((val)<-256.f)?(1<<31U):((int32_t)((val)*(float)(1<<23)))<<0))

#define FLOAT_FROM_Q31_T(val)		((float)(val)*(0.5f/(float)(1<<30)))
#define FLOAT_FROM_Q23_T(val)		((float)(val)*(0.5f/(float)(1<<30)))
#define FLOAT_FROM_Q8_23_T(val)		((float)(val)*(1.0f/(float)(1<<23)))

int32_t AudioSmart_get_fw_version(uint32_t *v1_ptr, uint32_t *v2_ptr, uint32_t *v3_ptr, uint32_t *v4_ptr)
{
  Command cmd = {0};
  int32_t ret_val = SENDCMD(&cmd, (ID_MM|FWID_SYS), CMD_GET(SYS_CMD_VERSION));

  if (ret_val < 0)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "ERROR: Failed to get FW version\n");
    return ret_val;
  }

  *v1_ptr = cmd.data[0];
  *v2_ptr = cmd.data[1];
  *v3_ptr = cmd.data[2];
  *v4_ptr = cmd.data[3];

  return ret_val;
}

int32_t AudioSmart_get_build_version(uint8_t *ptr, uint32_t byte_size)
{
  Command cmd = {0};
  uint8_t *tmp_ptr = (uint8_t *)(&(cmd.data[0]));
  uint32_t bsz = 0;
  int32_t ret_val = SENDCMD(&cmd, (ID_MM | FWID_SYS), CMD_GET(SYS_CMD_SVN_VERSION));

  if (ret_val < 0)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "ERROR: Failed to get build FW version\n");
    return ret_val;
  }
  else
  {
    bsz = (uint32_t)(ret_val * 4);
    bsz = (bsz > byte_size) ? byte_size : bsz;
    for (uint32_t i = 0; i < bsz; i++)
    {
      *(ptr + i) = *(tmp_ptr + i);
    }
  }
  return (int32_t)bsz;
}

int32_t AudioSmart_get_list_of_tuned_modes(int8_t **modes)
{
  Command cmd = {0};

  int32_t index = 0;
  int32_t count = 0;
  int32_t num = 0;
  int32_t ret_val = 0;
  int32_t i;
  uint32_t id;

  do
  {
    ret_val = SENDCMD(&cmd, (ID_MM|FWID_SW), CMD_GET(SW_SFS_CMD_TUNED_MODES), index);
    if (ret_val < 0)
    {
      AUDIOSMART_DEBUG_PRINT(stderr, "ERROR: Failed to get a list of modes\n");
      return ret_val;
    }
    count = cmd.head.num_32b_words;
    if ((count > 0) && (count <= MAX_COMMAND_SIZE))
    {
      for (i = 0; i < count; i++)
      {
        id = (unsigned int)cmd.data[i];
		if (hex2id0(id) == 'Z' || hex2id0(id) == 'Y')
		{
		  modes[num][0] = hex2id0(id);
          modes[num][1] = hex2id1(id);
          modes[num][2] = hex2id2(id);
          modes[num][3] = hex2id3(id);
		  num++;
		}
      }
      index += count;
    }
  } while (count == MAX_COMMAND_SIZE);

  return num;
}

int32_t AudioSmart_get_num_of_tuned_modes(int32_t *num_of_mode)
{
  Command cmd = {0};

  int32_t index = 0;
  int32_t count = 0;
  int32_t num = 0;
  int32_t ret_val = 0;
  int32_t i;

  do
  {
    ret_val = SENDCMD(&cmd, (ID_MM|FWID_SW), CMD_GET(SW_SFS_CMD_TUNED_MODES), index);
    if (ret_val < 0)
    {
      AUDIOSMART_DEBUG_PRINT(stderr, "ERROR: Failed to get a list of modes\n");
      return ret_val;
    }
    count = cmd.head.num_32b_words;
    if ((count > 0) && (count <= MAX_COMMAND_SIZE))
    {
      for (i = 0; i < count; i++)
      {
		if (hex2id0(cmd.data[i]) == 'Z' || hex2id0(cmd.data[i]) == 'Y')
		{
		  num++;
		}
      }
      index += count;
    }
  } while (count == MAX_COMMAND_SIZE);

  *num_of_mode = num;

  return 0;
}

int32_t AudioSmart_get_current_mode(int8_t *mode)
{
  Command cmd = {0};
  int32_t ret_val = SENDCMD(&cmd, (ID_MM|FWID_SYS), CMD_GET(SYS_CMD_MODE_ID));

  if (ret_val < 0)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "ERROR: Failed to get the current mode\n");
    return ret_val;
  }

  uint32_t id = cmd.data[0];

  mode[0] = hex2id0(id);
  mode[1] = hex2id1(id);
  mode[2] = hex2id2(id);
  mode[3] = hex2id3(id);

  return 0;
}

int32_t AudioSmart_set_current_mode(int8_t *mode)
{
  Command cmd = {0};
  int32_t id = ID(mode[0], mode[1], mode[2], mode[3]);
  int32_t ret_val = 0;

  ret_val = SENDCMD(&cmd, (ID_MM|FWID_SYS), SYS_CMD_EXEC_FILE, id);

  if (ret_val == 0)
  {
	ret_val = SENDCMD(&cmd, (ID_MM|FWID_SYS), CMD_GET(SYS_CMD_MODE_ID));
    if ((ret_val == 0) && ((int)(cmd.data[0]) == id))
    {
      //Success
    }
    else
    {
      AUDIOSMART_DEBUG_PRINT(stderr, "ERROR: Failed to confirm switching to mode: %d %x %x\n", ret_val, id, cmd.data[0]);
    }
  }
  else
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "ERROR: Failed to switch to mode: %d %x %x\n", ret_val, id, cmd.data[0]);
  }
  return ret_val;
}

int32_t AudioSmart_get_parameter_value(int32_t evnt_par, int32_t *value)
{
  int32_t ret_val = 0;
  Command cmd = {0};

  ret_val = SENDCMD(&cmd, (ID_MM|FWID_SYS), CMD_GET(SYS_CMD_PARAMETER_VALUE), evnt_par);
  if (ret_val >= 0 && cmd.head.num_32b_words >= 2)
  {
    *value = cmd.data[1];
    ret_val = 0;
  }
  else
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "ERROR: Failed to get parameter value: %d %x\n", ret_val, evnt_par);
  }
  return ret_val;
}

int32_t AudioSmart_reset_device(void)
{
  int32_t status = 0;
  int32_t err_no = CXERR_NOERR;
  Command cmd = { 0 };

  err_no = SENDCMD(&cmd, (ID_MM | FWID_SYS), SYS_CMD_SOFTRESET);

  return err_no;
}

int32_t AudioSmart_gpio_reset(int32_t peripheral_index)
{
  Command cmd = {0};
  int32_t ret_val = 0;

  ret_val = SENDCMD(&cmd, (ID_MM | FWID_SYS), SYS_CMD_GPIO_RESET, peripheral_index); //You need to set the gpios by SYS_CMD_GPIO_RESET_CONFIG before using this.

  if (ret_val != 0)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "Reset : %08x failed\n", ret_val);
  }
  return ret_val;
}

int32_t AudioSmart_get_key_events(uint32_t* key_id, uint32_t* key_state, uint32_t* num_keys)
{
  uint32_t data[MAX_COMMAND_SIZE], i, size;
  int32_t  ret_val = 0;
  
  ret_val = AudioSmart_get_ipc_msg( data, num_keys);
  if (ret_val != 0)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "ERROR: Failed to get Keys Pressed, error: %08x failed\n", ret_val);
  }

  size = *num_keys;
  *num_keys = 0;
  for (i=0; i<size; i++){
    if( (data[i] & 0xFFFF) != 0 ){
      key_id[*num_keys] = data[i] & 0xFF;
      key_state[*num_keys] = (data[i] & 0xFF00) >> 8;
      (*num_keys)++;
    }
  }

  return ret_val;
}

int32_t AudioSmart_send_custom0_event_with_param(int32_t par)
{
  Command cmd = {0};
  int32_t ret_val = 0;

  ret_val = SENDCMD(&cmd, ID_MM, CMD_SET(SYS_CMD_EVENT_PARAM), EVENT_CUSTOM_0, EVENT_PAR_CUSTOM_0, par);

  if (ret_val != 0)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "Command to Send Custom 0 event : %08x failed\n", ret_val);
  }
  return ret_val;
}

int32_t AudioSmart_send_custom1_event_with_param(int32_t par)
{
  Command cmd = {0};
  int32_t ret_val = 0;

  ret_val = SENDCMD(&cmd, ID_MM, CMD_SET(SYS_CMD_EVENT_PARAM), EVENT_CUSTOM_1, EVENT_PAR_CUSTOM_1, par);

  if (ret_val != 0)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "Command to Send Custom 1 event : %08x failed\n", ret_val);
  }
  return ret_val;
}

int32_t AudioSmart_send_custom2_event_with_param(int32_t par)
{
  Command cmd = {0};
  int32_t ret_val = 0;

  ret_val = SENDCMD(&cmd, ID_MM, CMD_SET(SYS_CMD_EVENT_PARAM), EVENT_CUSTOM_2, EVENT_PAR_CUSTOM_2, par);

  if (ret_val != 0)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "Command to Send Custom 2 event : %08x failed\n", ret_val);
  }
  return ret_val;
}

int32_t AudioSmart_send_custom3_event_with_param(int32_t par)
{
  Command cmd = {0};
  int32_t ret_val = 0;

  ret_val = SENDCMD(&cmd, ID_MM, CMD_SET(SYS_CMD_EVENT_PARAM), EVENT_CUSTOM_3, EVENT_PAR_CUSTOM_3, par);

  if (ret_val != 0)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "Command to Send Custom 3 event : %08x failed\n", ret_val);
  }
  return ret_val;
}

/*
 * This is basic function by which command send out
 */
#include "AudioSmart_sendcmd_common.h"


#if 1 //host code will use this file, so always build it 

int32_t AudioSmart_sendcmd(Command *cmd, uint32_t mgr_module_id, uint32_t command_id, uint32_t num_32b_words, ...)
{
#if 1  
  va_list  args;
  uint32_t n;

  va_start(args, num_32b_words);

  // at least two words of header
  if (num_32b_words > MAX_COMMAND_SIZE)
  {
    va_end(args);
    return (SENDCMD_RC_INVALID_ARG_NUM);
  }
  cmd->head.num_32b_words = (command_id&CMD_GET(0)) ? MAX_COMMAND_SIZE : num_32b_words;
  cmd->head.command_id = command_id;
  cmd->head.reply = 0;
  cmd->head.mgr_module_id = mgr_module_id;

  for (n = 0; n < num_32b_words; n++)
  {
    cmd->data[n] = va_arg(args, int32_t);
  }
  va_end(args);
#endif
  return AudioSmart_mid_sendcmd((uint32_t *)cmd);
}

int32_t AudioSmart_sendcmd_sync(Command *cmd) {
  return AudioSmart_mid_sendcmd((uint32_t *)cmd);
}

int32_t AudioSmart_sendcmd_async(Command *cmd, sendcmd_cb cb_func){
  if (cb_func != NULL) {
    return AudioSmart_mid_sendcmd_async((uint32_t *)cmd, CONFIG_DEFAULT_WAIT_TIMEOUT, cb_func);
  }
  else {
    return AudioSmart_mid_sendcmd_async((uint32_t *)cmd, 0, cb_func);
  }
}



#endif





#include "AudioSmart_definitions.h"
#include "AudioSmart_generic.h"
#include "AudioSmart_sendcmd_common.h"

int32_t SYNA_EXPORT_DLL AudioSmart_get_ipc_event(int32_t* event)
{
	Command cmd = { 0 };
	int32_t ret_val = 0;

	ret_val = SENDCMD(&cmd, ID_MM, CMD_GET(SYS_CMD_IPC_CMD), EIPC_REASON);

	if (ret_val != 0)
	{
		AUDIOSMART_DEBUG_PRINT(stderr, "ERROR: Failed to get IPC event, error: %08x failed\n", ret_val);
	} 
    else 
    {
        *event = cmd.data[0];    
    }

	return ret_val;
}

int32_t SYNA_EXPORT_DLL AudioSmart_get_ipc_msg_size(uint32_t *msg_size)
{
	Command cmd = { 0 };
	int32_t ret_val = 0;

	ret_val = SENDCMD(&cmd, ID_MM, CMD_GET(SYS_CMD_IPC_CMD), EIPC_MESSAGE_SIZE);

	if (ret_val != 0)
	{
		AUDIOSMART_DEBUG_PRINT(stderr, "ERROR: Failed to get IPC msg size, error: %08x failed\n", ret_val);
	} 
    else 
    {
        *msg_size = cmd.data[0];    
    }

	return ret_val;
}

int32_t SYNA_EXPORT_DLL AudioSmart_get_ipc_msg(uint32_t *msg, uint32_t *msg_size)
{
	Command cmd = { 0 };
	int32_t ret_val = 0;
    int i = 0;

    if (*msg_size < sizeof(int32_t) * MAX_COMMAND_SIZE)
    {
		AUDIOSMART_DEBUG_PRINT(stderr, "ERROR: Buffer size for message is smaller than %d\n", sizeof(int32_t) * MAX_COMMAND_SIZE);
        return -1;
	}

	ret_val = SENDCMD(&cmd, ID_MM, CMD_GET(SYS_CMD_IPC_MSG));

	if (ret_val != 0)
	{
		AUDIOSMART_DEBUG_PRINT(stderr, "ERROR: Failed to get IPC message, error: %08x failed\n", ret_val);
	} 
    else 
    {
        *msg_size = cmd.head.num_32b_words;
        for (i = 0; i < cmd.head.num_32b_words; i++)
        {
            msg[i] = cmd.data[i];
        }
    }

	return ret_val;
}

#include "AudioSmart_definitions.h"
#include "AudioSmart_generic.h"
#include "AudioSmart_sendcmd_common.h"

int32_t SYNA_EXPORT_DLL AudioSmart_playback_startstop(int32_t startstop, int32_t sampleRate, int32_t sampleWidth, int32_t frameSize)
{
    Command cmd = {0};
    int32_t ret_val = 0;

    if (startstop == 1)         //Playback start.
    {
        ret_val = SENDCMD(&cmd, ID_MM, CMD_SET(SYS_CMD_EVENT_PARAM), EVENT_USB_PLAYBACK_STARTSTOP,
                          EVENT_PAR_USB_PLAYBACK_STATE, 1,
                          EVENT_PAR_RATE_MAIN_INPUT, sampleRate,
                          PAR_INDEX_I2S_TX_WIDTH, sampleWidth,
                          PAR_INDEX_I2S_TX_NUM_OF_BITS, frameSize);
    }
    else if (startstop == 2)    //Playback start with enabling sending external echo reference on I2STX(DSP View)
    {
        ret_val = SENDCMD(&cmd, ID_MM, CMD_SET(SYS_CMD_EVENT_PARAM), EVENT_USB_PLAYBACK_STARTSTOP,
                          EVENT_PAR_USB_PLAYBACK_STATE, 1,
                          EVENT_PAR_RATE_MAIN_INPUT, sampleRate,
                          PAR_INDEX_I2S_TX_WIDTH, sampleWidth,
                          PAR_INDEX_I2S_TX_NUM_OF_BITS, frameSize,
                          EVENT_PAR_EXT_ECHO_REFERENCE, 1);
    }                           //Playback stop.
    else
    {
        ret_val = SENDCMD(&cmd, ID_MM, CMD_SET(SYS_CMD_EVENT_PARAM), EVENT_USB_PLAYBACK_STARTSTOP, EVENT_PAR_USB_PLAYBACK_STATE, 0);
    }

    if (ret_val != 0)
    {
        AUDIOSMART_DEBUG_PRINT(stderr, "ERROR:%s:startstop=%d, %d\n", __FUNCTION__, startstop, ret_val);
    }
    return ret_val;
}

int32_t SYNA_EXPORT_DLL AudioSmart_get_playback_startstop_state(int32_t *pPlbkState)
{
    return AudioSmart_get_parameter_value(EVENT_PAR_USB_PLAYBACK_STATE, pPlbkState);
}

int32_t SYNA_EXPORT_DLL AudioSmart_set_playback_volume(uint32_t volLeft, uint32_t volRight)
{
    Command cmd = {0};
    int32_t ret_val = 0;

    ret_val = SENDCMD(&cmd,
                      ID_MM,
                      CMD_SET(SYS_CMD_EVENT_PARAM),
                      EVENT_VOLUME_CHANGE_USB_PLAYBACK,
                      EVENT_PAR_VOL_USB_PLAYBACK_CH0,
                      volLeft,
                      EVENT_PAR_VOL_USB_PLAYBACK_CH1,
                      volRight);

    if (ret_val != 0)
    {
        AUDIOSMART_DEBUG_PRINT(stderr, "ERROR:%s:%d\n", __FUNCTION__, ret_val);
    }
    return ret_val;
}

int32_t SYNA_EXPORT_DLL AudioSmart_set_preset_user_eq_index(int32_t UserEqInx)
{
    int32_t ret_val = -1;
    if (UserEqInx >= 0 && UserEqInx <= 15)
    {
        Command cmd = {0};
        int32_t id = ID('E', 'Q', '0', (UserEqInx + 0x30));

        ret_val = SENDCMD(&cmd, (ID_MM | FWID_SYS), SYS_CMD_EXEC_FILE, id);
        if (ret_val != 0)
        {
            AUDIOSMART_DEBUG_PRINT(stderr, "ERROR:%s:%d\n", __FUNCTION__, ret_val);
        }
    }
    return ret_val;
}

int32_t SYNA_EXPORT_DLL AudioSmart_get_preset_user_eq_index(int32_t *pPreSetUserEQIdx)
{
    return AudioSmart_get_parameter_value(EVENT_PAR_PRE_SET_USER_EQ_INDEX, pPreSetUserEQIdx);
}

int32_t SYNA_EXPORT_DLL AudioSmart_record_startstop(int32_t startstop, int32_t sampleRate, int32_t sampleWidth, int32_t frameSize)
{
    Command cmd = {0};
    int32_t ret_val = 0;

    if (startstop)
    {
        ret_val = SENDCMD(&cmd, ID_MM, CMD_SET(SYS_CMD_EVENT_PARAM), EVENT_USB_RECORD_STARTSTOP,
                            EVENT_PAR_USB_RECORD_STATE, 1,
                            EVENT_PAR_RATE_HOST_RECORD, sampleRate,
                            PAR_INDEX_I2S_RX_WIDTH, sampleWidth,
                            PAR_INDEX_I2S_RX_NUM_OF_BITS, frameSize);
    }
    else
    {
        ret_val = SENDCMD(&cmd, ID_MM, CMD_SET(SYS_CMD_EVENT_PARAM), EVENT_USB_RECORD_STARTSTOP, EVENT_PAR_USB_RECORD_STATE, 0);
    }

    if (ret_val != 0)
    {
        AUDIOSMART_DEBUG_PRINT(stderr, "ERROR:%s:%d\n", __FUNCTION__, ret_val);
    }
    return ret_val;
}

int32_t SYNA_EXPORT_DLL AudioSmart_get_record_startstop_state(int32_t *pRecdState)
{
    return AudioSmart_get_parameter_value(EVENT_PAR_USB_RECORD_STATE, pRecdState);
}

int32_t SYNA_EXPORT_DLL AudioSmart_set_record_volume(uint32_t volLeft, uint32_t volRight)
{
    Command cmd = {0};
    int32_t ret_val = 0;

    ret_val = SENDCMD(&cmd,
                      ID_MM,
                      CMD_SET(SYS_CMD_EVENT_PARAM),
                      EVENT_VOLUME_CHANGE_USB_RECORD,
                      EVENT_PAR_VOL_USB_RECORD_CH0,
                      volLeft,
                      EVENT_PAR_VOL_USB_RECORD_CH1,
                      volRight);

    if (ret_val != 0)
    {
        AUDIOSMART_DEBUG_PRINT(stderr, "ERROR:%s:%d\n", __FUNCTION__, ret_val);
    }
    return ret_val;
}

int32_t SYNA_EXPORT_DLL AudioSmart_set_voice_call_mode(eVoiceCallMode vc_mode)
{
    Command cmd = {0};
    int32_t ret_val = 0;

    ret_val = SENDCMD(&cmd, ID_MM, SYS_CMD_EXEC_FILE, ID('V', 'O', 'C', 'C'), vc_mode);
    if (ret_val != 0)
    {
        AUDIOSMART_DEBUG_PRINT(stderr, "ERROR:%s:%d\n", __FUNCTION__, ret_val);
    }
    return ret_val;
}

int32_t SYNA_EXPORT_DLL AudioSmart_enable_enc(int32_t enable)
{
    Command cmd = {0};
    int32_t ret_val = 0;

    ret_val = SENDCMD(&cmd, ID_MM, CMD_SET(SYS_CMD_EVENT_PARAM), EVENT_ENC_CONTROL, EVENT_PAR_ENC_STATE, enable);

    if (ret_val != 0)
    {
        AUDIOSMART_DEBUG_PRINT(stderr, "ERROR:%s:%d\n", __FUNCTION__, ret_val);
    }
    return ret_val;
}

int32_t SYNA_EXPORT_DLL AudioSmart_get_enc_enable_state(int32_t *pEncState)
{
    return AudioSmart_get_parameter_value(EVENT_PAR_ENC_STATE, pEncState);
}

int32_t SYNA_EXPORT_DLL AudioSmart_enable_sidetone(uint32_t enable)
{
    Command cmd = {0};
    int32_t ret_val = 0;

    ret_val = SENDCMD(&cmd, ID_MM, CMD_SET(SYS_CMD_SIDETONE_ENABLE), enable);

    if (ret_val != 0)
    {
        AUDIOSMART_DEBUG_PRINT(stderr, "ERROR:%s:%d\n", __FUNCTION__, ret_val);
    }
    return ret_val;
}

int32_t SYNA_EXPORT_DLL AudioSmart_set_sidetone_volume(uint32_t volSidetone)
{
    Command cmd = {0};
    int32_t ret_val = 0;

    ret_val = SENDCMD(&cmd, ID_MM, CMD_SET(SYS_CMD_SIDETONE_VOLUME), volSidetone);

    if (ret_val != 0)
    {
        AUDIOSMART_DEBUG_PRINT(stderr, "ERROR:%s:%d\n", __FUNCTION__, ret_val);
    }
    return ret_val;
}

int32_t SYNA_EXPORT_DLL AudioSmart_enable_voice_prompt(int32_t enable)
{
    Command cmd = {0};
    int32_t ret_val = 0;

    ret_val = SENDCMD(&cmd, ID_MM, CMD_SET(SYS_CMD_EVENT_MASK), !enable, 0, EVENT_VOICE_PROMPT_CONTROL);

    if (ret_val != 0)
    {
        AUDIOSMART_DEBUG_PRINT(stderr, "ERROR:%s:%d\n", __FUNCTION__, ret_val);
    }
    return ret_val;
}

int32_t SYNA_EXPORT_DLL AudioSmart_get_voice_prompt_enable_state(int32_t *pVptEnState)
{
    Command cmd = {0};
    int32_t ret_val = 0;

    ret_val = SENDCMD(&cmd, ID_MM, CMD_GET(SYS_CMD_EVENT_MASK), 0, EVENT_VOICE_PROMPT_CONTROL);

    if (ret_val != 0)
    {
        AUDIOSMART_DEBUG_PRINT(stderr, "ERROR:%s:%d\n", __FUNCTION__, ret_val);
        return ret_val;
    }
    else
    {
        *pVptEnState = !(cmd.data[0] & EVENT_VOICE_PROMPT_CONTROL);
        return 0;
    }
}

int32_t SYNA_EXPORT_DLL AudioSmart_voice_prompt_startstop(int8_t *file_id, int32_t startstop)
{
    Command cmd = {0};
    int32_t id = ID(file_id[0], file_id[1], file_id[2], file_id[3]);
    int32_t ret_val = 0;

    ret_val = SENDCMD(&cmd, ID_MM, SYS_CMD_EVENT_PARAM, EVENT_VOICE_PROMPT_CONTROL, EVENT_PAR_VOICE_PROMPT_STATE, !!startstop, EVENT_PAR_VOICE_PROMPT_FILE, id);

    if (ret_val != 0)
    {
        AUDIOSMART_DEBUG_PRINT(stderr, "ERROR:%s:%c%c%c%c,%d\n", __FUNCTION__, file_id[0], file_id[1], file_id[2], file_id[3], ret_val);
    }
    return ret_val;
}

int32_t SYNA_EXPORT_DLL AudioSmart_get_voice_prompt_startstop_state(int32_t *pVpStartStopState)
{
    return AudioSmart_get_parameter_value(EVENT_PAR_VOICE_PROMPT_STATE, pVpStartStopState);
}

