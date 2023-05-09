/*------------------------------------------------------------------------------
  Copyright (C) 2020 Synaptics Inc.
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

  File Name: AudioSmart_generic.h

------------------------------------------------------------------------------*/
/**
 * \file AudioSmart_generic.h
 * \note
 *     - The \ref DspCtrlStatusAPI can be used not just for communicating between Host and Synaptics AudioSmart DSP Codecs, but also from
 *      one of the cores of the Synaptics AudioSmart DSP Codec to the main controller of that chip - for e.g. HiFi3 of AS33970 to M33 of
 *      AS33970.
 *
 *
 * \addtogroup DspCtrlStatusAPI
 * @{
 *    \defgroup GenericCtrlStatusAPI Generic Control and Status APIs
 *    \brief
 *      This is Generic AudioSmart DSP Codec Control and Status API.
 *    \addtogroup GenericCtrlStatusAPI
 *    @{
 */
#ifndef AudioSmart_GENERIC_H_
#define AudioSmart_GENERIC_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "syna_types.h"

/**
 * \fn int32_t AudioSmart_get_fw_version (uint32_t *v1_ptr, uint32_t *v2_ptr, uint32_t *v3_ptr, uint32_t *v4_ptr)
 * \brief This function returns the version number of the Synaptics Audio DSP's firmware.
 *
 * \param v1_ptr uint32_t pointer to memory where the first major firmware version
 *               would be returned from the DSP
 * \param v2_ptr uint32_t pointer to memory where the second major firmware version
 *               would be returned from the DSP
 * \param v3_ptr uint32_t pointer to memory where the third major firmware version
 *               would be returned from the DSP
 * \param v4_ptr uint32_t pointer to memory where the minor firmware version
 *               would be returned from the DSP
 * \return status int32_t representing the status of the function.\n
 *               = 0 if successful\n
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_fw_version (uint32_t *v1_ptr, uint32_t *v2_ptr, uint32_t *v3_ptr, uint32_t *v4_ptr);



/**
 * \fn int32_t AudioSmart_get_build_version(uint8_t *ptr, uint32_t byte_size)
 * \brief This function returns the build version number of the Synaptics Audio DSP's firmware.
 *
 * \param ptr       uint8_t pointer to memory where the buffer to store the string for build version 
 * \param byte_size uint32_t size of buffer (bytes) to store the string for build version, typical size is 16 bytes
 * \return status int32_t representing the status of the function.\n
 *               >= 0 number of bytes of string for build version 
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL  AudioSmart_get_build_version(uint8_t *ptr, uint32_t byte_size);

/**
 * \fn int32_t AudioSmart_get_num_of_tuned_modes(int32_t* num_of_mode)
 * \brief This function gets the number of all available Tuned modes in the
 *        Synaptics Audio Device. This function currently is available only for HW CAF.
 *
 * \param num_of_mode  pointer to number of modes 
 * \return status int32_t representing the status of the function.\n
 *               = 0 if successful\n
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_num_of_tuned_modes(int32_t* num_of_mode);


/**
 * \fn int32_t AudioSmart_get_list_of_tuned_modes(int8_t **modes)
 * \brief This function gets the names of all available Tuned modes in the
 *        Synaptics DSP, and it is capture mode if the first letter is 'Z' and
 *        it is playback mode if the first letter is 'Y'.
 *
 * \param modes  pointer to int8_t[n][4] array, where n is equal to or greater than the
 *               maximum number of tuned modes possible in a Synaptics Audio DSP. Generally
 *               15 or so is sufficient, however this might need to change depending on if the
 *               firmware is known to have many more modes.
 *               Returns the 4-character names of all the available modes in
 *               the memory pointed to by this pointer.
 * \return status int32_t representing the status of the function.\n
 *               > 0 the number of modes\n
 *               = 0 if successful\n
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_list_of_tuned_modes(int8_t **modes);

/**
 * \fn int32_t AudioSmart_get_current_mode(int8_t *mode)
 * \brief This function returns the current mode of the Synaptics Audio device.
 *
 * \param mode   Pointer to an int8_t[4] array which will return the 4-character name of the current mode of
 *               the Synaptics Audio DSP.
 * \return status int32_t representing the status of the function.\n
 *               = 0 if successful\n
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_current_mode(int8_t *mode);

/**
 * \fn int32_t AudioSmart_set_current_mode(int8_t *mode)
 * \brief This function sets the current mode of the Synaptics Audio device
 *
 * \param mode   Pointer to an int8_t[4] array containing the mode to set
 *               the Synaptics Audio DSP to.
 * \return status int32_t representing the status of the function.\n
 *               = 0 if successful\n
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_set_current_mode(int8_t *mode);

/**
 * \fn int32_t AudioSmart_get_parameter_value(int32_t evnt_par, int32_t *value)
 * \brief This function gets a given parameter value of the Synaptics Audio device.
 *
 * \param evnt_par int32_t representing the event parameter to get EventParIndex.
 * \param value int32_t* pointer to store the returned value
 * \return status int32_t representing the status of the function.\n
 *               = 0 if successful\n
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_parameter_value(int32_t evnt_par, int32_t *value);

/**
 * \fn int32_t AudioSmart_reset_device(void)
 *
 * \brief This function soft resets the Audio DSP
 *
 * \return status integer representing the status of the function.
 *               = CXERR_NOERR if successful, or error-code
 */
int32_t SYNA_EXPORT_DLL AudioSmart_reset_device(void);

/**
 * \fn int32_t AudioSmart_send_custom0_event_with_param (int32_t par)
 * \brief This function sends custom0 event to the Synaptics Audio DSP along with a parameter.
 *        The definition of what this event does is defined by Synaptics Audio DSP for 
 *        customer specific requirement or generated by FCP.
 *
 * \param par int32_t the integer value set in custom0 event's parameter.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_send_custom0_event_with_param (int32_t par);

/**
 * \fn int32_t AudioSmart_gpio_reset(int32_t peripheral_index)
 * \brief This function reset the specified external device.
 *
 * \param peripheral_index int32_t the integer value specifies the external device index.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_gpio_reset(int32_t peripheral_index);

/**
 * \fn int32_t Audiosmart_get_key_events(uint32_t* key_id, uint32_t* key_state, uint32_t* num_keys);
 * \brief This function get the key events detected.
 *
 * \param key_id - uint32_t* Pointer to buffer to hold the eKEY_ID of buttons pressed.
 * \param key_state - uint32_t* Pointer to buffer to hold the corresponding eKEY_EVENT of buttons pressed.
 * \param num_keys - uint32_t* Pointer to buffer to hold the key event buffers size. When call the function,
 *                   it specify the buffer size pointed by key_id and key_state. On return, it hold the actual
 *                   number of key press events detected.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL Audiosmart_get_key_events(uint32_t* key_id, uint32_t* key_state, uint32_t* num_keys);

/**
 * \fn int32_t AudioSmart_send_custom1_event_with_param (int32_t par)
 * \brief This function sends custom1 event to the Synaptics Audio DSP along with a parameter.
 *        The definition of what this event does is defined by Synaptics Audio DSP for 
 *        customer specific requirement or generated by FCP.
 *
 * \param par int32_t the integer value set in custom1 event's parameter.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_send_custom1_event_with_param (int32_t par);

/**
 * \fn int32_t AudioSmart_send_custom2_event_with_param (int32_t par)
 * \brief This function sends custom2 event to the Synaptics Audio DSP along with a parameter.
 *        The definition of what this event does is defined by Synaptics Audio DSP for 
 *        customer specific requirement or generated by FCP.
 *
 * \param par int32_t the integer value set in custom2 event's parameter.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_send_custom2_event_with_param (int32_t par);

/**
 * \fn int32_t AudioSmart_send_custom3_event_with_param (int32_t par)
 * \brief This function sends custom3 event to the Synaptics Audio DSP along with a parameter.
 *        The definition of what this event does is defined by Synaptics Audio DSP for 
 *        customer specific requirement or generated by FCP.
 *
 * \param par int32_t the integer value set in custom3 event's parameter.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_send_custom3_event_with_param (int32_t par);

#ifdef __cplusplus
}
#endif 


#endif /* AudioSmart_GENERIC_H_ */

/*@}*/
/*@}*/
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

  File Name: AudioSmart_ipc.h

------------------------------------------------------------------------------*/
/**
 * \file AudioSmart_ipc.h
 *
 * \addtogroup DspCtrlStatusAPI
 * @{
 *    \defgroup GenericCtrlStatusAPI Fast I2S Control and Status APIs
 *    \brief
 *      This is Generic AudioSmart DSP Codec Control and Status API.
 *    \addtogroup GenericCtrlStatusAPI
 *    @{
 *
 */
#ifndef AudioSmart_IPC_H_
#define AudioSmart_IPC_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "syna_types.h"

/**
 * \fn int32_t AudioSmart_get_ipc_event(int32_t* event)
 * \brief Get event value to see which event is signaled
 *
 * \param event - int32_t* Pointer to a 32bit bitwise value to indicate an event occur with corresponding
 *                bit be set as 1.
 *                Bit 0: IPC message
 *                Bit 1: Aurora trigger
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_ipc_event(int32_t *event);

/**
 * \fn int32_t AudioSmart_get_ipc_msg_size(uint32_t *msg_size)
 * \brief Get the available IPC message size in 32bit.
 *
 * \param msg_size - int32_t* Pointer to a 32bit value to hold the IPC message size in 32bit.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_ipc_msg_size(uint32_t *msg_size);

/**
 * \fn int32_t AudioSmart_get_ipc_msg(uint32_t *msg, uint32_t *msg_size)
 * \brief Read the IPC message out. Max read size is 13 dword each time
 *
 * \param msg - int32_t* Pointer to buffe to hold the IPC message.
 * \param msg_size - int32_t* Pointer to buffer to hold the IPC message size. When call the function,
 *                   it specify the buffer size pointed by msg. On return, it hold the actual data lenght
 *                   in msg.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_ipc_msg(uint32_t *msg, uint32_t *msg_size);

#ifdef __cplusplus
}
#endif 


#endif /* AudioSmart_IPC_H_ */

/*@}*/
/*@}*/
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

  File Name: AudioSmart_generic.h

------------------------------------------------------------------------------*/
/**
 * \file AudioSmart_generic.h
 *
 * \addtogroup DspCtrlStatusAPI
 * @{
 *    \defgroup GenericCtrlStatusAPI Control and Status APIs
 *    \brief
 *      This is Generic AudioSmart DSP Codec Control and Status API.
 *    \addtogroup GenericCtrlStatusAPI
 *    @{
 *
 */
#ifndef AudioSmart_CONTROL_H_
#define AudioSmart_CONTROL_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "syna_types.h"

/**
 * \fn int32_t AudioSmart_playback_startstop(int32_t startstop, int32_t sampleRate, int32_t sampleWidth, int32_t frameSize)
 * \brief Start or stop playback path.
 *
 * \param startstop - int32_t the state of playback path: 
 *                    0 means stop.
 *                    1 means start with playback path only.
 *                    2 means start playback path along with echo reference signal when MUSIC playback.
 *                      If host need echo reference signal like for AEC, enable echo reference signal sending back to 
 *                      host on i2stx when MUSIC playback start. To differ music playback or voice call playback, 
 *                      DSP will clear echo reference setting automatically when playback stop.
 * \param sampleRate - int32_t sample rate of I2STX(Host View).(e.g 16000, 44100, 48000)
 * \param sampleWidth - int32_t sample width index of I2STX(Host View), 0 is for 8bit, 1 is for 16bit, 2 is for 24bit, 3 is for 32bit
 * \param frameSize - int32_t frame size of I2STX(Host View). (e.g 32, 64)
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_playback_startstop(int32_t startstop, int32_t sampleRate, int32_t sampleWidth, int32_t frameSize);

/**
 * \fn int32_t AudioSmart_get_playback_startstop_state(int32_t *pPlbkState)
 * \brief get the state of playback path. 0 means stopped, 1 means started.
 *
 * \param pPlbkState - int32_t * the pointer of state of playback path
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_playback_startstop_state(int32_t *pPlbkState);

/**
 * \fn int32_t AudioSmart_set_playback_volume(uint32_t volLeft, uint32_t volRight);
 * \brief Set playback volume level. 
 *
 * \param volLeft - uint32_t volume of Left channel in Q8.23 format.
 * \param volRight - uint32_t volume of Right channel in Q8.23 format. Will not use it when TWS application.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_set_playback_volume(uint32_t volLeft, uint32_t volRight);

/**
 * \fn int32_t AudioSmart_set_preset_user_eq_index(int32_t UserEqInx)
 * \brief This function sets preset User EQ type with UserEqIdx. If customers want to have preset User EQ type,
 *        normally customer's tuning engineers can use Synaptics' caf_tuning tool or Synaptics' Audio Stutio Tool 
 *        to tune the preset User EQ and save as prefix_EQ0x_free_text.tcl.
 *        This API is applicable when DSP provides playback ability.
 *
 * \param UserEqInx - int32_t Pre_Set User EQ index which valid scope is 0~15(Maximum preset EQ index supported).
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_set_preset_user_eq_index(int32_t UserEqInx);

/**
 * \fn int32_t AudioSmart_get_preset_user_eq_index(int32_t *pPreSetUserEQIdx)
 * \brief This function gets preset User EQ index which starting from 0.
 *        This API is applicable when DSP provides playback ability.
 *
 * \param pPreSetUserEQIdx - int32_t * the pointer to store index of preset User EQ.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_preset_user_eq_index(int32_t *pPreSetUserEQIdx);

/**
 * \fn int32_t AudioSmart_record_startstop(int32_t startstop, int32_t sampleRate, int32_t sampleWidth, int32_t frameSize)
 * \brief Start or stop record path
 *
 * \param startstop - int32_t the state of record path: 0 means stop, 1 means start.
 * \param sampleRate - int32_t sample rate of I2SRX(Host View).(e.g 16000, 44100, 48000)
 * \param sampleWidth - int32_t sample width index of I2SRX(Host View), 0 is for 8bit, 1 is for 16bit, 2 is for 24bit, 3 is for 32bit
 * \param frameSize - int32_t frame size of I2SRX(Host View). (e.g 32, 64)
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_record_startstop(int32_t startstop, int32_t sampleRate, int32_t sampleWidth, int32_t frameSize);

/**
 * \fn int32_t AudioSmart_get_record_startstop_state(int32_t *pRecdState)
 * \brief get the state of record path. 0 means stopped, 1 means started.
 *
 * \param pRecdState - int32_t * the pointer of state of record path
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_record_startstop_state(int32_t *pRecdState);

/**
 * \fn int32_t AudioSmart_set_record_volume(uint32_t volLeft, uint32_t volRight);
 * \brief Set record volume level.
 *
 * \param volLeft - uint32_t volume of Left channel in Q8.23 format. DSP outputs bi-mono by default and will use volLeft
 * \param volRight - uint32_t volume of Right channel in Q8.23 format.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_set_record_volume(uint32_t volLeft, uint32_t volRight);

typedef enum
{
  VOICE_CALL_OFF,
  VOICE_CALL_NORMAL,
  VOICE_CALL_HEARING_PROTECT,
} eVoiceCallMode;

/**
 * \fn int32_t AudioSmart_set_voice_call_mode(int vc_mode)
 * \brief This function sets DSP voice call mode. By default, DSP is in VOICE_CALL_OFF mode if MUSIC playback start. By default DSP is in VOICE_CALL_NORMAL if playback/record start.
 *        If DSP provides Hearing protection voice call integration mode, use this API to set between VOICE_CALL_OFF(MUSIC mode)/VOICE_CALL_NORMAL/VOICE_CALL_HEARING_PROTECT.
 *
 * \param vc_mode - eVoiceCallMode Voice call mode
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_set_voice_call_mode(eVoiceCallMode vc_mode);

/**
 * \fn int32_t AudioSmart_enable_enc(int32_t enable)
 * \brief This function enables/disables Synaptics ENC. Normally host doesn't need to control ENC dynamially.
 *
 * \param enable  - uint32_t the state of ENC: 0 means disable, 1 means enable.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_enable_enc(int32_t enable);

/**
 * \fn int32_t AudioSmart_get_enc_enable_state(int32_t *pEncState)
 * \brief This function gets the enable state of ENC. 0 means disabled, 1 means enabled.
 *
 * \param pEncState - int32_t * the pointer of state of ENC
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_enc_enable_state(int32_t *pEncState);

/**
 * \fn int32_t AudioSmart_enable_sidetone(uint32_t enable);
 * \brief Enable or disable hardware sidetone
 *
 * \param enable - uint32_t the state of sidetone: 0 means disable, 1 means enable.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_enable_sidetone(uint32_t enable);

/**
 * \fn int32_t AudioSmart_set_sidetone_volume(uint32_t volSidetone);
 * \brief Set playback volume level
 *
 * \param vol - volume of sidetone in Q8.23 format (range is -6dB to -60dB)
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_set_sidetone_volume(uint32_t volSidetone);

/**
 * \fn int32_t AudioSmart_enable_voice_prompt (int32_t enable)
 * \brief This function enables/disables the Voice Prompt playback feature. 
 *        It will ignore the command of AudioSmart_voice_prompt_startstop when disable voice prompt playback feature.
 *
 * \param enable  - uint32_t the state of Voice Prompt feature: 0 means disable, 1 means enable.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_enable_voice_prompt(int32_t enable);

/**
 * \fn int32_t AudioSmart_get_voice_prompt_enable_state(int32_t *pVptEnState)
 * \brief get the enable state of voice prompt.
 *
 * \param pVptEnState - int32_t * the pointer of voice prompt enable state.  0 means disabled, 1 means enabled.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_voice_prompt_enable_state(int32_t *pVptEnState);

/**
 * \fn int32_t AudioSmart_voice_prompt_startstop (int8_t *file_id, int32_t startstop)
 * \brief This function starts or stops playing an audio file as a Voice Prompt which stores in DSP's flash.
 *
 * \param file_id - int8_t[4] Pointer to an int8_t[4] array containing the file ID of the voice prompt to be played.
 * \param startstop  - uint32_t Represent the action to be executed with the audio file: 0 means stop, 1 means start.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_voice_prompt_startstop(int8_t *file_id, int32_t startstop);

/**
 * \fn int32_t SYNA_EXPORT_DLL AudioSmart_get_voice_prompt_startstop_state(int32_t *pVpStartStopState)
 * \brief This function gets voice prompt startstop state. 0: stopped. 1: started.
 *
 * \param pVpStartStopState - int32 * pointer to get voice prompt start stop state.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t SYNA_EXPORT_DLL AudioSmart_get_voice_prompt_startstop_state(int32_t *pVpStartStopState);

#ifdef __cplusplus
}
#endif 


#endif /* AudioSmart_GENERIC_H_ */

/*@}*/
/*@}*/
