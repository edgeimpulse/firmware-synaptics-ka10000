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

  File Name: calib_demo.cpp

------------------------------------------------------------------------------*/
// TODO: needed for printf. if STDIO isn't available on your host, implement
// your own printf
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <linux/input.h>
#include <time.h>
#endif

#include "AudioSmart_definitions.h"
#ifdef ENABLE_I2C_CONTROL
#include "AudioSmart_i2c_interface_api.h"
#endif
#if defined(ENABLE_USB_CONTROL)||defined(ENABLE_I2CF3_CONTROL)
#include "AudioSmart_usb_interface_api.h"
#endif

#ifdef ENABLE_SOCKET_CONTROL
#include "AudioSmart_socket_interface_api.h"

#define SOCKET_MAX_ADDR_LEN (15)
#define SOCKET_DEFAULT_ADDR "127.0.0.1"
#define SOCKET_DEFAULT_PORT 7777

#endif


#include "AudioSmart_sendcmd.h"
#include "AudioSmart_generic.h"

#undef AUDIOSMART_DEBUG_PRINT
#define AUDIOSMART_DEBUG_PRINT(file, format, ...) fprintf(file, format, ...)

uint32_t current_mode_index = 0;
void sys_mdelay(uint32_t ms_delay)
{
#ifdef WIN32
  Sleep(ms_delay);
#else
  // TODO : To implement millisecond sleep function here.
  usleep(ms_delay * 1000);
#endif
}

#if defined(ENABLE_USB_CONTROL)||defined(ENABLE_I2CF3_CONTROL)
//uint32_t intf_pid_en = 0;
//uint32_t intf_vid_en = 0;
uint32_t intf_setpid = 0x1494;
uint32_t intf_setvid = 0x06cb;
#endif

#define DUMP_DATA(_cmd_, _msg_, ret)      \
  printf(_msg_);                          \
  for (int32_t i = 0; i < ret; i++)           \
  {                                       \
    printf("  0x%08x", (_cmd_)->data[i]); \
  }                                       \
  printf("\n");

#define DUMP_RESULT(_cmd_, _msg_, ret) \
  if (ret >= 0)                        \
  {                                    \
    DUMP_DATA(_cmd_, _msg_, ret);      \
  }                                    \
  else                                 \
  {                                    \
    printf("failed to " _msg_);        \
  }

#define SLAVE_ADDRESS 0x41

void print_main_menu(void)
{
  printf("+-----------+\n");
  printf("| Main Menu |\n");
  printf("+-----------+\n\n");
  printf("Selection to enter  Test to run\n");
  printf("==================  ===========\n");
#ifdef AudioSmart_GENERIC_H_
  printf("     g              Get Firmware Version\n");
  printf("     c              Get Current Mode\n");
#endif
#ifdef AudioSmart_ANC_H_
  printf("     n              Switch to Calibration Mode.\n");
  printf("     o              ANC on\n");
  printf("     f              ANC off\n");
  printf("     i              Ambient Inclusion on - only valid when ANC is ON\n");
  printf("     j              Ambient Inclusion off - only valid when ANC is ON\n");
  printf("     s              Save Calibration gains - These only get into effect after reseting the device\n");
#endif
#ifdef AudioSmart_TAHI_DSP_ANC_H_
  printf("     d              Get state of ANC paths\n");
  printf("     e              Set state of ANC paths\n");
  printf("     1              Get Ambient Inclusion Calibration Gain\n");
  printf("     2              Set Ambient Inclusion Calibration Gain\n");
  printf("     3              Get ANC Feedback Calibration Gain\n");
  printf("     4              Set ANC Feedback Calibration Gain\n");
  printf("     5              Get ANC Feedforward Calibration Gain\n");
  printf("     6              Set ANC Feedforward Calibration Gain\n");
  printf("     7              Get ANC Playback Cancellation Calibration Gain\n");
  printf("     8              Set ANC Playback Cancellation Calibration Gain\n");
  printf("     9              Get PSAP Calibration Gain\n");
  printf("     0              Set PSAP Calibration Gain\n");
#endif
#ifdef AudioSmart_TAHI_DSP_AMP_H_
  printf("     a              Get Playback Calibration Gain\n");
  printf("     b              Set Playback Calibration Gain\n");
#endif
#ifdef AudioSmart_TAHI_DSP_MUX_H_
  printf("     v              Get the Voice Proc Select Out channel selection.\n");
  printf("     w              Set the Voice Proc Select Out channel selection.\n");
#endif
#ifdef AudioSmart_TAHI_DSP_PLBK_SIG_GEN_H_
  printf("     y              Get the configuration of playback signal generator.\n");
  printf("     z              Set the configuration of playback signal generator.\n");
  printf("     ~              Enable playback signal generator.\n");
  printf("     ^              Disable playback signal generator.\n");
#endif
#ifdef AudioSmart_TAHI_DSP_CPTR_METER_H_
  printf("     x              Enable Capture Meter\n");
  printf("     p              Disable Capture Meter\n");
  printf("     u              Get Status of Capture Meter\n");
  printf("     m              Get Meter RMS Values from Capture Meter (Meter Needs to be enabled before using this)\n");
#endif
  printf("     t              Reset the Device.\n");
#if defined(ENABLE_I2C_CONTROL)||defined(ENABLE_I2CF3_CONTROL)
  printf("     r              Check the Device is bootup ready.\n ");
  printf("                    Note:This should be the first call after Device Reset! \n");
  printf("                         Keep the menu open,reset the device,check the bootup status\n");
#endif
  printf("     h              Help\n");
  printf("     q              Quit\n");
}

void print_help_menu()
{
  printf("Usage:\n");
  printf("=====\n");
  printf("  calib_demo.exe <Options> <Commands Arguments>\n");
  printf("  Options:\n");
  printf("---------\n");
  printf("  -h:  Help\n");
#ifdef ENABLE_I2C_CONTROL
  printf("  -d: I2C path for DEV I2C interface\n");
#elif defined(ENABLE_USB_CONTROL)||defined(ENABLE_I2CF3_CONTROL)
  printf("  -p <HEX>: PID of the device to connect to via USB-HID\n");
  printf("  -v <HEX>: VID of the device to connect to via USB-HID\n");
#elif ENABLE_SOCKET_CONTROL
  printf("  -d : Destination TCP/IP address of the device to connect to via Socket\n");
  printf("  -p : Destination TCP/IP port number of the device to connect to via Socket\n");
  printf("  eg. ./calib_demo -d 10.47.28.37 -p 7777 \n");
#endif
  printf("\n");
}
#ifdef AudioSmart_GENERIC_H_
void print_fw_version()
{
  uint32_t fwv1, fwv2, fwv3, fwv4;

  if (AudioSmart_get_fw_version(&fwv1, &fwv2, &fwv3, &fwv4) >= 0)
  {
    fprintf(stdout, "version number: %u.%u.%u.%u\n", fwv1, fwv2, fwv3, fwv4);
  }
  else
  {
    fprintf(stderr, "failed to get FW version\n");
  }
}
#endif

int32_t main(int32_t argc, char *argv[])
{
  int32_t rc;
  int32_t err_no = 0;
  int8_t *mode = NULL;
  bool    is_ote = false;
  int32_t type = 0;
  int32_t  ch = 0;
  char str[128];
  int i;
#ifdef ENABLE_I2C_CONTROL
  char *card_name = NULL;
#endif
#if defined(AudioSmart_ANC_H_)||defined(AudioSmart_TAHI_DSP_ANC_H_)
  int tws_earbud_role = 0;
  float feedforward_calib_gain[2], feedback_calib_gain[2], plbk_canc_calib_gain[2], amb_inclusion_calib_gain[2], psap_calib_gain[2];
#endif

#ifdef AudioSmart_TAHI_DSP_ANC_H_
  int32_t ff_active, fb_active, ai_active, psap_active;
#endif
#if defined(AudioSmart_ANC_H_)||defined(AudioSmart_TAHI_DSP_AMP_H_)
  float playback_calib_gain[2];
#endif
#ifdef AudioSmart_TAHI_DSP_CPTR_METER_H_
  float mics_rms_db[5], echo_refs_rms_db[2], processed_out_rms_db;
  int32_t num_mic_channels, num_echo_ref_channels;
#endif
#ifdef AudioSmart_TAHI_DSP_MUX_H_
  int32_t channel0, channel1;
#endif
#ifdef AudioSmart_TAHI_DSP_PLBK_SIG_GEN_H_
  int32_t signal_type, signal_param;
  float amplitude_dbfs;
#endif
#ifdef ENABLE_SOCKET_CONTROL
  char *des_ip_addr   = NULL;
  char *des_ip_port   = NULL;
  char socket_caf_connect_ip_addr[SOCKET_MAX_ADDR_LEN] = {0};
  uint16_t socket_caf_connect_port = SOCKET_DEFAULT_PORT;
  int user_set_socket_addr = 0;

#endif

  int32_t data = 0;
#if !defined(ENABLE_I2C_CONTROL) && !defined(ENABLE_USB_CONTROL) && !defined(ENABLE_SOCKET_CONTROL) && !defined(ENABLE_I2CF3_CONTROL)
  fprintf(stderr, "ERROR: Program needs to be built with either i2c, usb or socket enabled");
  return -1;
#elif defined(ENABLE_I2C_CONTROL) && defined(ENABLE_USB_CONTROL)
  fprintf(stderr, "ERROR: Program can't have both i2c and usb enabled");
  return -1;
#elif defined(ENABLE_I2C_CONTROL) && defined(ENABLE_SOCKET_CONTROL)
  fprintf(stderr, "ERROR: Program can't have both i2c and socket enabled");
  return -1;
#elif defined(ENABLE_USB_CONTROL) && defined(ENABLE_SOCKET_CONTROL)
  fprintf(stderr, "ERROR: Program can't have both usb and socket enabled");
  return -1;
#endif
  if (argc > 1)
  {
    /* parsing the command line arguments */
    for (int32_t i = 1; i < argc; i++)
    {
#if defined(ENABLE_USB_CONTROL)||defined(ENABLE_I2CF3_CONTROL)
      if (strcmp(argv[i], "-p") == 0)
      {
        intf_setpid = strtol(argv[++i], NULL, 16);
      }
      else if (strcmp(argv[i], "-v") == 0)
      {
        intf_setvid = strtol(argv[++i], NULL, 16);
      }
#elif ENABLE_I2C_CONTROL
      if (strcmp(argv[i], "-d") == 0)
      {
        if ((i = i + 1) >= argc)
        {
          fprintf(stderr, "ERROR: Please enter i2c dev path name\n");
          return -1;
        }
        else
        {
          card_name = argv[i];
        }
      }
#elif ENABLE_SOCKET_CONTROL
    if (strcmp(argv[i], "-d") == 0)
    {
      if ((i = i + 1) >= argc)
      {
        fprintf(stderr, "ERROR: Please enter socket ip address.\n");
        return -1;
      }
      else
      {
        if(strlen(argv[i]) > SOCKET_MAX_ADDR_LEN) {
          fprintf(stderr,"ERROR: Ip address is too long.\n");
          return -1;
        }
        memcpy(socket_caf_connect_ip_addr,argv[i],strlen(argv[i]));
        user_set_socket_addr = 1;
      }
    }
    if (strcmp(argv[i], "-p") == 0)
    {
      if ((i = i + 1) >= argc)
      {
        fprintf(stderr, "ERROR: Please enter socket ip pord number.\n");
        return -1;
      }
      else
      {
        socket_caf_connect_port = strtol(argv[i], NULL, 10);
      }
    }
#endif
      if (strcmp(argv[i], "-h") == 0)
      {
        print_help_menu();
        exit(0);
      }
    }
  }

#if defined(ENABLE_USB_CONTROL)||defined(ENABLE_I2CF3_CONTROL)
  if ((rc = AudioSmart_usb_interface_init(intf_setvid, intf_setpid)) < 0)
  { 
    fprintf(stdout, "Warning: failed to initialize usb interface (vid:0x%04x  pid:0x%04x).Error code:%d \n",intf_setvid,intf_setpid, rc);
    fprintf(stdout, "Try with (vid:0x%04x  pid:0xffff) \n",intf_setvid);
    if ((rc = AudioSmart_usb_interface_init(intf_setvid, 0xFFFF)) < 0)
    {
      fprintf(stderr, "Error: failed to initialize usb interface.  Error code:%d \n", rc);
      return rc;
    }
  }
#elif ENABLE_I2C_CONTROL
  if ((rc = AudioSmart_i2c_interface_init((uint32_t)SLAVE_ADDRESS, (int32_t)2, (int8_t *)card_name)) < 0)
  {
    fprintf(stderr, "Error: failed to initialize i2c interface.  Error code:%d \n", rc);
    return rc;
  }
 #elif ENABLE_SOCKET_CONTROL
 if(user_set_socket_addr == 0) {
    memcpy(socket_caf_connect_ip_addr,SOCKET_DEFAULT_ADDR,strlen(SOCKET_DEFAULT_ADDR));
  }
 if ((rc = AudioSmart_socket_interface_init(socket_caf_connect_ip_addr,socket_caf_connect_port)) < 0 ) {
  fprintf(stderr, "Error: failed to initialize socket interface.  Error code:%d \n", rc);
    return rc;
 }
#endif

  /* Note we need to  register the delay functions to initial midlle level api */
  if (AudioSmart_mid_level_init(&sys_mdelay))
    return rc;

#ifdef AudioSmart_GENERIC_H_
  print_fw_version();
#endif

  printf("\nType select(0:TWS  1:OTE):");
  scanf("%d", &type);

  if (type == 0)
    is_ote = false;
  else if (type == 1)
    is_ote = true;
  else
    return -1;

  if (is_ote) 
  {
    printf("\nSelect ch (0-left  1-right):");
    scanf("%d", &ch);
    if ((ch != 0) && (ch != 1))
      return -1;
  } 

  mode = (int8_t *)malloc(4);
  print_main_menu();
  while (fgets(str, sizeof(str), stdin) != NULL)
  {
    switch (str[0])
    {
#ifdef AudioSmart_GENERIC_H_
    case 'g':
      print_fw_version();
      break;
    case 'c':
      err_no = AudioSmart_get_current_mode(mode);
      if (err_no >= 0)
        fprintf(stdout, "Current mode is %c%c%c%c\n", mode[0], mode[1], mode[2], mode[3]);
      else
        fprintf(stdout, "Failed to get current mode\n");
      break;
#endif
    case 'q':
      printf("Goodbye...\n");
      break;
    case 'h':
      print_main_menu();
      break;

#ifdef AudioSmart_ANC_H_
    case 'o': //anc on
      err_no = AudioSmart_anc_enable(true);
      if (err_no >= 0)
        fprintf(stdout, "ANC ON\n");
      else
        fprintf(stdout, "ERROR: Failed to turn on ANC.\n");
      break;
    case 'f': //anc off
      err_no = AudioSmart_anc_enable(false);
      if (err_no >= 0)
        fprintf(stdout, "ANC OFF\n");
      else
        fprintf(stdout, "ERROR: Failed to turn off ANC.\n");
      break;
    case 'i': // ai on
      err_no = AudioSmart_ai_enable(true);
      if (err_no >= 0)
        fprintf(stdout, "Ambient Inclusion ON\n");
      else
        fprintf(stdout, "ERROR: Failed to turn on Ambient Inclusion.\n");
      break;
    case 'j': // ai on
      err_no = AudioSmart_ai_enable(false);
      if (err_no >= 0)
        fprintf(stdout, "Ambient Inclusion OFF\n");
      else
        fprintf(stdout, "ERROR: Failed to turn off Ambient Inclusion.\n");
      break;
    case 'n': // Switch to Calibration Mode
      err_no = AudioSmart_switch_to_calibration_mode();
      if (err_no >= 0)
        fprintf(stdout, "Switched to Calibration Mode\n");
      else
        fprintf(stdout, "ERROR: Failed to switch to calibration mode.\n");
      break;
    case 's': // Save Calibration gains
      if (false == is_ote) {
        printf("\nEnter TWS earbud role(0: Left, 1: Right):");
        scanf("%d", &tws_earbud_role);
      }
      if (is_ote) {
        printf("\nEnter ANC LEFT Feed Forward Calibration Gain (dB float point):");
        scanf("%f", &feedforward_calib_gain[0]);
        printf("\nEnter ANC RIGHT Feed Forward Calibration Gain (dB float point):");
        scanf("%f", &feedforward_calib_gain[1]);
      } else {
        printf("\nEnter ANC Feed Forward Calibration Gain (dB float point):");
        scanf("%f", &feedforward_calib_gain[0]);
      }

      if (is_ote) {
        printf("\nEnter ANC LEFT Feed Back Calibration Gain (dB float point):");
        scanf("%f", &feedback_calib_gain[0]);
        printf("\nEnter ANC RIGHT Feed Back Calibration Gain (dB float point):");
        scanf("%f", &feedback_calib_gain[1]);
      } else {
        printf("\nEnter ANC Feed Back Calibration Gain (dB float point):");
        scanf("%f", &feedback_calib_gain[0]);
      }

      if (is_ote) {
        printf("\nEnter ANC Playback LEFT Cancellation Calibration Gain (dB float point):");
        scanf("%f", &plbk_canc_calib_gain[0]);
        printf("\nEnter ANC Playback RIGHT Cancellation Calibration Gain (dB float point):");
        scanf("%f", &plbk_canc_calib_gain[1]);
      } else {
        printf("\nEnter ANC Playback Cancellation Calibration Gain (dB float point):");
        scanf("%f", &plbk_canc_calib_gain[0]);
      }

      if (is_ote) {
        printf("\nEnter Ambient LEFT Inclusion Calibration Gain (dB float point):");
        scanf("%f", &amb_inclusion_calib_gain[0]);
        printf("\nEnter Ambient RIGHT Inclusion Calibration Gain (dB float point):");
        scanf("%f", &amb_inclusion_calib_gain[1]);
      } else {
        printf("\nEnter Ambient Inclusion Calibration Gain (dB float point):");
        scanf("%f", &amb_inclusion_calib_gain[0]);
      }

      if (is_ote) {
        printf("\nEnter Playback LEFT Calibration Gain (dB float point):");
        scanf("%f", &playback_calib_gain[0]);
        printf("\nEnter Playback RIGHT Calibration Gain (dB float point):");
        scanf("%f", &playback_calib_gain[1]);
      } else {
        printf("\nEnter Playback Calibration Gain (dB float point):");
        scanf("%f", &playback_calib_gain[0]);
      }

      if (is_ote) {
        printf("\nEnter PSAP LEFT Calibration Gain (dB float point):");
        scanf("%f", &psap_calib_gain[0]);
        printf("\nEnter PSAP RIGHT Calibration Gain (dB float point):");
        scanf("%f", &psap_calib_gain[1]);
      } else {
        printf("\nEnter PSAP Calibration Gain (dB float point):");
        scanf("%f", &psap_calib_gain[0]);
      }

      err_no = AudioSmart_save_anc_calib_gains(is_ote, tws_earbud_role, feedforward_calib_gain, feedback_calib_gain, plbk_canc_calib_gain, amb_inclusion_calib_gain, playback_calib_gain, psap_calib_gain);
      if (err_no >= 0)
        fprintf(stdout, "Saved Calibration Data - These only get into effect after reseting the device\n");
      else
        fprintf(stdout, "ERROR: Failed to save Calibration Gains.\n");
      break;
#endif

#ifdef AudioSmart_TAHI_DSP_ANC_H_
    case 'd':
      err_no = AudioSmart_dsp_anc_active_get(&ff_active, &fb_active, &ai_active, &psap_active);
      if (err_no >= 0)
      {
        fprintf(stdout, "ANC path settings - 0 means mute, 1 means active (enabled), 2 means bypassed (disabled):\n");
        fprintf(stdout, "Fead-forward path: %d, Fead-back path: %d, Ambient Inclusion path: %d, PSAP path: %d\n", ff_active, fb_active, ai_active, psap_active);
      }
      else
        fprintf(stdout, "ERROR: Failed to read ANC path settings.\n");
      break;
    case 'e':
      printf("\nEnter ANC Path settings - 0 means mute, 1 means active (enabled), 2 means bypassed (disabled):");
      printf("\nEnter ANC Fead Forward path setting:");
      scanf("%d", &ff_active);
      printf("\nEnter ANC Feed Back path setting:");
      scanf("%d", &fb_active);
      printf("\nEnter Ambient Inclusion path setting:");
      scanf("%d", &ai_active);
      printf("\nEnter PSAP path setting:");
      scanf("%d", &psap_active);
      if ((ff_active >=0) && (ff_active < 3) && (fb_active >=0) && (fb_active < 3) && (ai_active >=0) && (ai_active < 3) && (psap_active >=0) && (psap_active < 3))
      {
        err_no = AudioSmart_dsp_anc_active_set(ff_active, fb_active, ai_active, psap_active);
        if (err_no >= 0)
        {
          fprintf(stdout, "ANC path set to - 0 means mute, 1 means active (enabled), 2 means bypassed (disabled): \n");
          fprintf(stdout, "Fead-forward path: %d, Fead-back path: %d, Ambient Inclusion path: %d, PSAP path: %d\n", ff_active, fb_active, ai_active, psap_active);
        }
        else
          fprintf(stdout, "ERROR: Failed to set ANC path settings.\n");
      } else {
        fprintf(stdout, "ERROR: Incorrect path settings - should be 0, 1 or 2.\n");
        fprintf(stdout, "Fead-forward path: %d, Fead-back path: %d, Ambient Inclusion path: %d, PSAP path: %d\n", ff_active, fb_active, ai_active, psap_active);
      }
      break;
    case '5':
      err_no = AudioSmart_dsp_anc_feedforward_calibration_gain_get(is_ote, ch, &feedforward_calib_gain[ch]);
      if (err_no >= 0)
        fprintf(stdout, "ANC Feed-forward Calibration gain is: %f dB\n", feedforward_calib_gain[ch]);
      else
        fprintf(stdout, "ERROR: Failed to read ANC Feed-forward Calibration gain.\n");
      break;
    case '6':
      printf("\nEnter ANC Feed Forward Calibration Gain (dB float point):");
      scanf("%f", &feedforward_calib_gain[ch]);
      err_no = AudioSmart_dsp_anc_feedforward_calibration_gain_set(is_ote, ch, feedforward_calib_gain[ch]);
      if (err_no >= 0)
        fprintf(stdout, "Set ANC Feed Forward Calibration Gain to %f dB\n", feedforward_calib_gain[ch]);
      else
        fprintf(stdout, "ERROR: Failed to Set ANC Feed Forward Calibration Gain.\n");
      break;
    case '3':
      err_no = AudioSmart_dsp_anc_feedback_calibration_gain_get(is_ote, ch, &feedback_calib_gain[ch]);
      if (err_no >= 0)
        fprintf(stdout, "ANC Feed-back Calibration gain is: %f dB\n", feedback_calib_gain[ch]);
      else
        fprintf(stdout, "ERROR: Failed to read ANCFeed-back Calibration gain.\n");
      break;
    case '4':
      printf("\nEnter ANC Feed Back Calibration Gain (dB float point):");
      scanf("%f", &feedback_calib_gain[ch]);
      err_no = AudioSmart_dsp_anc_feedback_calibration_gain_set(is_ote, ch, feedback_calib_gain[ch]);
      if (err_no >= 0)
        fprintf(stdout, "Set ANC Feed-back Calibration Gain to %f dB\n", feedback_calib_gain[ch]);
      else
        fprintf(stdout, "ERROR: Failed to Set ANC Feed-back Calibration Gain.\n");
      break;
    case '7':
      err_no = AudioSmart_dsp_anc_playback_cancellation_calibration_gain_get(is_ote, ch, &plbk_canc_calib_gain[ch]);
      if (err_no >= 0)
        fprintf(stdout, "ANC Playback Cancellation Calibration gain is: %f dB\n", plbk_canc_calib_gain[ch]);
      else
        fprintf(stdout, "ERROR: Failed to read ANC Playback Cancellation Calibration gain.\n");
      break;
    case '8':
      printf("\nEnter ANC Playback Cancellation Calibration Gain (dB float point):");
      scanf("%f", &plbk_canc_calib_gain[ch]);
      err_no = AudioSmart_dsp_anc_playback_cancellation_calibration_gain_set(is_ote, ch, plbk_canc_calib_gain[ch]);
      if (err_no >= 0)
        fprintf(stdout, "Set ANC Playback Cancellation Calibration Gain to %f dB\n", plbk_canc_calib_gain[ch]);
      else
        fprintf(stdout, "ERROR: Failed to Set ANC Playback Cancellation Calibration Gain.\n");
      break;
    case '1':
      err_no = AudioSmart_dsp_anc_amb_inclusion_calibration_gain_get(is_ote, ch, &amb_inclusion_calib_gain[ch]);
      if (err_no >= 0)
        fprintf(stdout, "Ambient Inclusion Calibration Gain is: %f dB\n", amb_inclusion_calib_gain[ch]);
      else
        fprintf(stdout, "ERROR: Failed to read Ambient Inclusion Calibration Gain.\n");
      break;
    case '2':
      printf("\nEnter Ambient Inclusion Calibration Gain (dB float point):");
      scanf("%f", &amb_inclusion_calib_gain[ch]);
      err_no = AudioSmart_dsp_anc_amb_inclusion_calibration_gain_set(is_ote, ch, amb_inclusion_calib_gain[ch]);
      if (err_no >= 0)
        fprintf(stdout, "Set Ambient Inclusion Calibration Gain to %f dB\n", amb_inclusion_calib_gain[ch]);
      else
        fprintf(stdout, "ERROR: Failed to Set Ambient Inclusion Calibration Gain.\n");
      break;
    case '9':
      err_no = AudioSmart_dsp_anc_psap_calibration_gain_get(is_ote, ch, &psap_calib_gain[ch]);
      if (err_no >= 0)
        fprintf(stdout, "PSAP Calibration Gain is: %f dB\n", psap_calib_gain[ch]);
      else
        fprintf(stdout, "ERROR: Failed to read PSAP Calibration Gain.\n");
      break;
    case '0':
      printf("\nEnter PSAP Calibration Gain (dB float point):");
      scanf("%f", &psap_calib_gain[ch]);
      err_no = AudioSmart_dsp_anc_psap_calibration_gain_set(is_ote, ch, psap_calib_gain[ch]);
      if (err_no >= 0)
        fprintf(stdout, "Set PSAP Calibration Gain to %f dB\n", psap_calib_gain[ch]);
      else
        fprintf(stdout, "ERROR: Failed to set PSAP Calibration Gain.\n");
      break;
#endif
#ifdef AudioSmart_TAHI_DSP_AMP_H_
    case 'a':
      err_no = AudioSmart_dsp_playback_calibration_gain_get(ch, &playback_calib_gain[ch]);
      if (err_no >= 0)
        fprintf(stdout, "Playback Calibration Gain is: %f dB\n", playback_calib_gain[ch]);
      else
        fprintf(stdout, "ERROR: Failed to read Playback Calibration Gain.\n");
      break;
    case 'b':
      printf("\nEnter Playback Calibration Gain (dB float point):");
      scanf("%f", &playback_calib_gain[ch]);
      err_no = AudioSmart_dsp_playback_calibration_gain_set(ch, playback_calib_gain[ch]);
      if (err_no >= 0)
        fprintf(stdout, "Set Playback Calibration Gain to %f dB\n", playback_calib_gain[ch]);
      else
        fprintf(stdout, "ERROR: Failed to Set Playback Calibration Gain.\n");
      break;
#endif
#ifdef AudioSmart_TAHI_DSP_CPTR_METER_H_
    case 'x':
      err_no = AudioSmart_dsp_set_cptr_meter_status(1);
      if (err_no >= 0)
        fprintf(stdout, "Capture Meter Enabled\n");
      else
        fprintf(stdout, "ERROR: Failed to enable Capture Meters.\n");
      break;
    case 'p':
      err_no = AudioSmart_dsp_set_cptr_meter_status(0);
      if (err_no >= 0)
        fprintf(stdout, "Capture Meter Disbled\n");
      else
        fprintf(stdout, "ERROR: Failed to disable Capture Meters.\n");
      break;
    case 'u':
      err_no = AudioSmart_dsp_get_cptr_meter_status(&data);
      if (err_no >= 0)
      {
        if (data == 0)
          fprintf(stdout, "Capture Meter is Disabled.\n");
        else
          fprintf(stdout, "Capture Meter is Enabled.\n");
      }
      else
        fprintf(stdout, "ERROR: Failed to get status of Capture Meters.\n");
      break;
    case 'm':
      printf("\nEnter number of microphones (<=5):");
      scanf("%d", &num_mic_channels);
      printf("\nEnter number of echo reference channels (<=2):");
      scanf("%d", &num_echo_ref_channels);

      if ( (num_mic_channels > 0) && (num_mic_channels <= 5) && (num_echo_ref_channels > 0) && (num_echo_ref_channels <= 2)  )
      {
        err_no = AudioSmart_dsp_cptr_meter_rms_get(&mics_rms_db[0], num_mic_channels, &echo_refs_rms_db[0],
            num_echo_ref_channels, &processed_out_rms_db);
        if (err_no >= 0)
        {
          fprintf(stdout, "Mic RMS powers are: ");
          for (i = 0; i < num_mic_channels; i++)
          {
            fprintf(stdout, "%f dB, ", mics_rms_db[i]);
          }
          fprintf(stdout, "Echo Reference RMS powers are: ");
          for (i = 0; i < num_echo_ref_channels; i++)
          {
            fprintf(stdout, "%f dB, ", echo_refs_rms_db[i]);
          }
          fprintf(stdout, "and RMS power of processed signal is %f dB\n", processed_out_rms_db);
        }
        else
          fprintf(stdout, "ERROR: Failed to read Capture Meters RMS power values (%d).\n", err_no);
      } else {
        fprintf(stdout, "ERROR: Wrong number of microphones (%d) or echo reference channels (%d).\n", num_mic_channels, num_echo_ref_channels);
      }
      break;
#endif

#ifdef AudioSmart_TAHI_DSP_MUX_H_
    case 'v':
      err_no = AudioSmart_dsp_voice_proc_select_out_get(&channel0, &channel1);
      if ( (err_no >= 0) && ( (channel0 & 0xff0) <= 0x2c0) && ((channel1 & 0xff0) <= 0x2c0) )
      {
        printf("Left Channel: ");
        switch (channel0 & 0xfc0)
        {
        case 0x0c0:
          printf("Voice Processing Out\n");
          break;
        case 0x1c0:
          printf("Microphone - %d\n", channel0 & 0xf);
          break;
        case 0x2c0:
          printf("Echo Reference - channel %d\n", channel0 & 0xf);
          break;
        }
        printf("Right Channel: ");
        switch (channel1 & 0xfc0)
        {
        case 0x0c0:
          printf("Voice Processing Out\n");
          break;
        case 0x1c0:
          printf("Microphone - %d\n", channel1 & 0xf);
          break;
        case 0x2c0:
          printf("Echo Reference - channel %d\n", channel1 & 0xf);
          break;
        }
      } else
        fprintf(stdout, "ERROR: Querying Voice Proc Select Out Channel Selector.\n");
      break;

      break;
    case 'w':
      printf("\n The format of channel configuration is 0xscd, where s is the stream index, c is the const value 0xc, d is the channel index.\n");
      printf("stream index  (bits 8-11) - 0: VP-Out, 1: Mic, 2: EchoRef.\n");
      printf("channel index (bits 0-3)  - 0 for VP-Out, 0/1/2 for Mic, 0/1 for EchoRef according to the number of channels in each.\n");
      printf("\nEnter the Voice Proc Select Out Channel 0 Selection:");
      scanf("%x", &channel0);
      printf("\nEnter the Voice Proc Select Out Channel 1 Selection:");
      scanf("%x", &channel1);
      err_no = AudioSmart_dsp_voice_proc_select_out_set(channel0, channel1);
      if (err_no >= 0)
        fprintf(stdout, "Voice Proc Select Out Channel Selector Set\n");
      else
        fprintf(stdout, "ERROR: Failed to set Voice Proc Select Out Channel Selector.\n");
      break;
#endif
#ifdef AudioSmart_TAHI_DSP_PLBK_SIG_GEN_H_
    case 'y':
      err_no = AudioSmart_dsp_plbk_sig_gen_config_get(&signal_type, &amplitude_dbfs, &signal_param);
      if (err_no >= 0)
      {
        fprintf(stdout, "Playback Signal Generator is configured to output:\n");
        if (signal_type == 1)
        {
          fprintf(stdout, "    Signal Type                 : Tone\n");
          fprintf(stdout, "    Amplitude (dBFs)            : %f\n", amplitude_dbfs);
          fprintf(stdout, "    Frequency of the Tone in Hz : %d\n", signal_param);
        } else if (signal_type == 2)
        {
          fprintf(stdout, "    Signal Type                 : Noise\n");
          fprintf(stdout, "    Amplitude (dBFs)            : %f\n", amplitude_dbfs);
          fprintf(stdout, "    Random Seed Value           : %d\n", signal_param);
        } else
        {
          fprintf(stdout, "    Signal Type: Different\n");
        }
      }
      else
        fprintf(stdout, "ERROR: Failed to reading Playback Signal Generator Configuration.\n");
      break;
    case 'z':
      printf("\nEnter Signal Type (1 = Tone, 2 = Noise):");
      scanf("%d", &signal_type);
      if ( (signal_type < 1) || (signal_type > 2) )
      {
        fprintf(stdout, "ERROR: Unrecognized Signal Type.\n");
      } else {
        printf("\nEnter Signal Amplitude in dBFs:");
        scanf("%f", &amplitude_dbfs);

        if (signal_type == 1)
        {
          printf("\nEnter Frequency of the Tone in Hz :\n");
        } else {
          printf("\nRandom Seed Value :\n");
        }
        scanf("%d", &signal_param);
        err_no = AudioSmart_dsp_plbk_sig_gen_config_set(signal_type, amplitude_dbfs, signal_param);
        if (err_no >= 0)
          fprintf(stdout, "Set Playback Signal Generator Configuration.\n");
        else
          fprintf(stdout, "ERROR: Failed to Set Playback Signal Generator Configuration.\n");
      }
      break;
    case '~':
      err_no = AudioSmart_dsp_plbk_sig_gen_enable(1);
      if (err_no >= 0)
        fprintf(stdout, "Playback Signal Generator Enabled\n");
      else
        fprintf(stdout, "ERROR: Failed to enable Playback Signal Generator.\n");
      break;
    case '^':
      err_no = AudioSmart_dsp_plbk_sig_gen_enable(0);
      if (err_no >= 0)
        fprintf(stdout, "Playback Signal Generator Disbled\n");
      else
        fprintf(stdout, "ERROR: Failed to disable Playback Signal Generator.\n");
      break;
#endif
#if defined(ENABLE_I2C_CONTROL)||defined(ENABLE_I2CF3_CONTROL)
    case 'r':
      rc = AudioSmart_i2c_wait_until_bootup(&sys_mdelay);
      if (rc == 0)
      {
        printf("\nFirmware boot up Sucessfully !\n");
      }
      else
      {
        printf("\nFirmware boot up Fail !\n");
      }
      break;
#endif
    case 't':
      err_no = AudioSmart_reset_device();
      if (err_no >= 0)
        fprintf(stdout, "Device Soft Reset.\n");
      else
        fprintf(stdout, "ERROR: Device Reset Failed.\n");
      break;

#ifdef AudioSmart_GENERIC_H_
    default:
      break;
#endif
    }

    if (str[0] == 'q')
      break;
  }

#if defined(ENABLE_USB_CONTROL)||defined(ENABLE_I2CF3_CONTROL)
  AudioSmart_usb_interface_close();
#elif ENABLE_I2C_CONTROL
  AudioSmart_i2c_interface_close();
#elif ENABLE_SOCKET_CONTROL
  AudioSmart_socket_interface_close();
#endif

  if (mode != NULL)
    free(mode);
  return (0);
}
