////////////////////////////////////////////////////////////////////////////////
//
//  CONEXANT SYSTEMS, INC. CONFIDENTIAL AND PROPRIETARY
//
//  The information contained in this source code file
//  is strictly confidential and proprietary to Conexant Systems, Inc.
//  ("Conexant")
//
//  No part of this file may be possessed, reproduced or distributed, in
//  any form or by any means for any purpose, without the express written
//  permission of Conexant Systems Inc.
//
//  Except as otherwise specifically provided through an express agreement
//  with Conexant that governs the confidentiality, possession, use
//  and distribution of the information contained in this file, CONEXANT
//  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
//  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
//  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
//  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
//  AND REPRESENTATIONS.  IN NO EVENT WILL CONEXANT BE LIABLE FOR ANY DAMAGES
//  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
//
//  Copyright (C) 2009-2018 Conexant Systems Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ADC_DRV_H_
#define _ADC_DRV_H_

#include "driver.h"
#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if defined(CONFIG_MCU_DRV_ADC)

#define CHECK_RETURN(ret) if(ret < 0) {Print_ERR("ERR:%s, %d\n", __func__, __LINE__); return ret;}

//------ Flags ------//
#define ADC_FLAGS_NO                  (0x0U)
#define ADC_FLAGS_SET                 (0x01U)
#define ADC_FLAGS_GET                 (0x02U)
#define ADC_FLAGS_NO_WAIT             (0x04U)
#define ADC_FLAGS_NO_LOCK             (0x08U)
#define ADC_FLAGS_NO_BIAS_CHANGE      (0x10U)
#define ADC_FLAGS_1X                  (0x20U)
#define ADC_FLAGS_DECIMATOR           (0x40U)
#define ADC_FLAGS_AHEAD_OF_DECIMATOR  (0x80U)          //ahead of decimator means ADC analog part and DMIC


#define ADC_AMIC_CONFIG               (1U)
#define ADC_DMIC_CONFIG               (2U)

//------ Paramerters ------//
// Enable/Disable parameters
#define ADC_PAR_DISABLE (0U)
#define ADC_PAR_ENABLE  (1U)

// Sample rate parameters
#define ADC_PAR_SR_8K   (0U)
#define ADC_PAR_SR_16K  (1U)
#define ADC_PAR_SR_24K  (2U)
#define ADC_PAR_SR_32K  (3U)
#define ADC_PAR_SR_44K  (4U)
#define ADC_PAR_SR_48K  (5U)
#define ADC_PAR_SR_96K  (6U)
#define ADC_PAR_SR_192K (7U)
#define ADC_PAR_SR_384K (8U)

// ADC Channel parameters
#define ADC_PAR_CH_0         (1 << 0)
#define ADC_PAR_CH_1         (1 << 1)
#define ADC_PAR_CH_2         (1 << 2)
#define ADC_PAR_CH_3         (1 << 3)
#define ADC_PAR_CH_4         (1 << 4)
#define ADC_MAX_CHANNEL_NUM  (5U)
#define ADC_PAR_CH_MONO      (0x10000000U)
#define ADC_PAR_CH_BI_MONO   (0x20000000U)
#define ADC_PAR_CH_STEREO    (0x40000000U)
#define ADC_PAR_CH_ALL       (0x1FU)


#define  DMIC_0_CH_L     ADC_PAR_CH_0
#define  DMIC_0_CH_R     ADC_PAR_CH_1
#define  DMIC_1_CH_L     ADC_PAR_CH_2
#define  DMIC_1_CH_R     ADC_PAR_CH_3
#define  DMIC_2_CH_L     ADC_PAR_CH_4

//micbias
#define MAX_MICBIAS_NUM      (3U)

enum MIC_TYPE_TABLE
{
  MIC_TYPE_ALL_DMIC = 0,
  MIC_TYPE_ALL_AMIC,
  MIC_TYPE_MIXED_DDAAA,   //ADC_CH  0  1  2  3  4
  MIC_TYPE_MIXED_DDAAD,     //      D  D  A  A  D 
  MIC_TYPE_MIXED_DDDDA,
  MIC_TYPE_MIXED_DAAAA,
  MIC_TYPE_MIXED_DXDAD,    //special for google dmic
  MAX_MIC_TYPE_MIXED_NUM
};

#define CHANNEL_TYPE_AMIC     (0U)
#define CHANNEL_TYPE_DMIC     (1U)

#define ADC_MUTE              (1U)
#define ADC_UNMUTE            (0U)

#define DMIC_0                (0U)
#define DMIC_1                (1U)
#define DMIC_2                (2U)
#define MAX_DMIC_NUM          (3U)

#define ADC_VOLUME_MUTE       (0U)
// Volume parameters
#define ADC_PAR_VOL_FORMAT_Q_8_8  (1U)
#define ADC_PAR_VOL_FORMAT_Q_8_23 (2U)

typedef struct adc_hpf_cfg_s {
  dword channel;
  dword k1;
  dword k2;
  dword order;
#define ADC_HPF_ONE_ORDER  (1U) 
#define ADC_HPF_TWO_ORDER  (0U)
} adc_hpf_cfg_t;

#if 1
typedef struct adc_anc_hpf_cfg_s {
  dword channel;
  dword enable;
  //  dword k1;                             //k1 and k2 are fixed in anc path
  //  dword k2;
} adc_anc_hpf_cfg_t;
#endif

//mic conmon config
typedef struct mic_conmon_cfg_s {
  dword channel;
  dword low_high_perf_filter;
#define ADC_CFG_LOW_PERF_FILTER  (0U)
#define ADC_CFG_HIGH_PERF_FILTER (1U)
  dword micbias_output_voltage;
#define ADC_MICBIAS_OUPUT_1P3V (1U)
#define ADC_MICBIAS_OUPUT_1P4V (2U)
#define ADC_MICBIAS_OUPUT_1P5V (3U)
#define ADC_MICBIAS_OUPUT_1P6V (4U)
#define ADC_MICBIAS_OUPUT_1P7V (5U)
#define ADC_MICBIAS_OUPUT_1P8V (6U)
#define ADC_MICBIAS_OUPUT_1P9V (7U)
#define ADC_MICBIAS_OUPUT_2P0V (8U)
#define ADC_MICBIAS_OUPUT_2P1V (9U)
#define ADC_MICBIAS_OUPUT_2P2V (10U)
#define ADC_MICBIAS_OUPUT_2P3V (11U)
#define ADC_MICBIAS_OUPUT_2P4V (12U)
#define ADC_MICBIAS_OUPUT_2P5V (13U)
#define ADC_MICBIAS_OUPUT_2P6V (14U)
#define ADC_MICBIAS_OUPUT_2P7V (15U)
  dword control_by_sync_module;
} mic_conmon_cfg_t;

//dmic config
typedef struct dmic_cfg_s {
  dword channel;
  dword low_high_perf_filter;
  dword micbias_output_voltage;
  dword control_by_sync_module;
  dword input_phase;
  dword output_swap;
  dword output_polary;
  dword micbias_index;
} dmic_cfg_t;

//amic config
typedef struct amic_cfg_s {
  dword channel;
  dword low_high_perf_filter;
  dword micbias_output_voltage;
  dword control_by_sync_module;
} amic_cfg_t;

typedef union {
  mic_conmon_cfg_t mic;
  dmic_cfg_t       dmic;
  amic_cfg_t       amic;
} mic_cfg_u;

typedef struct llr_anc_latency_path_cfg_s
{
  union {
    struct
    {
      uint32_t anc_ch0_from_adc_ch_x : 4;
      uint32_t anc_ch1_from_adc_ch_x : 4;
      uint32_t anc_ch2_from_adc_ch_x : 4;
      uint32_t anc_ch3_from_adc_ch_x : 4;
      uint32_t reserved : 16;
    } bf;
    uint32_t dw;
  } adc_anc_channel_map;
  uint32_t llr_anc_cal_mode;
#define LLE_ANC_CAL_AUTO_MODE (1U)
#define LLE_ANC_CAL_MANUAL_MODE (0U)
  uint32_t lle_anc_latency_target;
  uint32_t anc_sample_rate;
} llr_anc_latency_path_cfg_t;

typedef struct avad_cfg_s {
  dword  vad_with_bias;
  dword  gain;
  dword  hpf;
  dword  thresh;
  dword	 beta1;
  dword  beta2;
}avad_cfg_t;

/*-------------------volume----------------------------*/
//codec volume config
typedef struct adc_vol_s {
  dword channels;
  uint  format;
  uint  vol_gain;
  uint  boost_gain;
  uint  balance_gain;
} adc_vol_t;

//vol
#define ADC_CTRL_ADC_VOL_MUTE (0U)                           // 0x00: mute
#define ADC_CTRL_ADC_VOL_MIN_MINUS_74_0dB (1U)               // 0x01: -74dB
#define ADC_CTRL_ADC_VOL_MAX_39_0dB (0xE3U)                  // 39dB
#define ADC_CTRL_ADC_VOL_MIN_MINUS_74_0dB_Q8_23 (-620756992) // -74dB
#define ADC_CTRL_ADC_VOL_MAX_39_0dB_Q8_23 (327155712)        // 39dB
#define ADC_CTRL_ADC_VOL_STEP_0_5dB (0.5F)                     // 0.5dB

#define ADC_CTRL_ADC_BOOST_MIN_0dB (0U)                      // 0x0: 0dB
#define ADC_CTRL_ADC_BOOST_MAX_24dB (0x4U)                   // 24dB
#define ADC_CTRL_ADC_BOOST_MIN_0dB_Q8_23 (0)                 // 0dB
#define ADC_CTRL_ADC_BOOST_MAX_24dB_Q8_23 (201326592)        // 24dB
#define ADC_CTRL_ADC_BOOST_STEP_6dB (6U)                     // 6dB

#define ADC_CTRL_ADC_BALANCE_MIN_MINUS_3_0dB (0U)               // 0x0: -3dB
#define ADC_CTRL_ADC_BALANCE_MAX_3_0dB (0xCU)                   // 3dB
#define ADC_CTRL_ADC_BALANCE_MIN_MINUS_3_0dB_Q8_23 (-25165824)  // -3dB
#define ADC_CTRL_ADC_BALANCE_MAX_3_0dB_Q8_23 (25165824)        // 3dB
#define ADC_CTRL_ADC_BALANCE_STEP_0_5dB (0.5F)                    // 0.5dB

//------ ADC API ------//
extern int drv_adc_start(void);

struct adc_ops_s {
  DECLARE_STANDARD_DEVICE_API

  int (*adc_config)(uint mic_type, mic_cfg_u* config, uint flags);
  int (*adc_hpf_config)(adc_hpf_cfg_t* config, uint flags);
  int (*adc_vol)(adc_vol_t* vol_par, uint flags);
  int (*adc_mute)(uint channel, uint mute_en, uint flags);

  int (*adc_enable)(uint mic_type, uint channel, uint sample_rate, uint is_enable, uint dmic_output_clok, uint flags);
  int (*adc_hpf_enable)(uint channel, uint is_enable, uint flags);

  //int (*anc_config)(adc_anc_cfg_t *config, uint flags);
  int (*adc_anc_hpf_config)(adc_anc_hpf_cfg_t* config, uint flags);
  int (*adc_anc_enable)(uint channel, uint sample_rate, uint is_enable, uint flags);
  int (*adc_anc_hpf_enable)(uint channel, uint is_enable, uint flags);

  int (*_llr_anc_latency_ch_enable)(uint is_enable, uint flags);
  int (*_llr_anc_latency_path_config)(llr_anc_latency_path_cfg_t* config, uint flag);
  int (*adc_avad_enable)(uint enable);
  int (*adc_avad_config)(avad_cfg_t* config, uint flag);
};

static inline struct adc_ops_s* adc_driver_info(uint dev)
{
  return (struct adc_ops_s*)os_get_driver_info(dev);
}

//=============================SET=============================================
static inline int llr_anc_latency_ch_set_enable(uint is_enable, uint flags)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }
  return ops->_llr_anc_latency_ch_enable(is_enable, flags);
}

static inline int llr_anc_latency_path_set_config(llr_anc_latency_path_cfg_t* config, uint flags)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }
  return ops->_llr_anc_latency_path_config(config, flags);
}

static inline int adc_set_config(uint mic_type, mic_cfg_u* config, uint flags)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }
  return ops->adc_config(mic_type, config, flags);
}

static inline int adc_hpf_set_config(adc_hpf_cfg_t* config, uint flags)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }
  return ops->adc_hpf_config(config, flags);
}

static inline int adc_set_vol(adc_vol_t* vol_par, uint flags)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }
  return ops->adc_vol(vol_par, flags | ADC_FLAGS_SET);
}

static inline int adc_set_mute(uint chs, uint mute_en, uint flags)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }

  return ops->adc_mute(chs, mute_en, flags);
}

static inline int adc_set_enable(uint mic_type, uint channel, uint sample_rate, uint is_enable, uint dmic_output_clock, uint flags)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }
  return ops->adc_enable(mic_type, channel, sample_rate, is_enable, dmic_output_clock, flags);
}

static inline int adc_hpf_set_enable(uint channel, uint is_enable, uint flags)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }
  return ops->adc_hpf_enable(channel, is_enable, flags);
}

#if 1
static inline int adc_anc_hpf_set_config(adc_anc_hpf_cfg_t* config, uint flags)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }
  return ops->adc_anc_hpf_config(config, flags);
}
#endif

static inline int adc_anc_set_enable(uint channel, uint sample_rate, uint is_enable, uint flags)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }
  return ops->adc_anc_enable(channel, sample_rate, is_enable, flags);
}

static inline int adc_anc_hpf_set_enable(uint channel, uint is_enable, uint flags)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }
  return ops->adc_anc_hpf_enable(channel, is_enable, flags  );
}

//============================Get=========================================
static inline int adc_get_vol(adc_vol_t* vol_par, uint flags)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }
  return ops->adc_vol(vol_par, flags | ADC_FLAGS_GET);
}

static inline int adc_avad_enable(uint enable)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }
  return ops->adc_avad_enable(enable);
}

static inline int adc_avad_set_config(avad_cfg_t* config, uint flags)
{
  struct adc_ops_s* ops = adc_driver_info(DEV_ADC);
  if (ops == NULL) {
    return -ENOENT;
  }
  return ops->adc_avad_config(config, flags);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //CONFIG_MCU_DRV_ADC

#endif // _ADC_DRV_H_
