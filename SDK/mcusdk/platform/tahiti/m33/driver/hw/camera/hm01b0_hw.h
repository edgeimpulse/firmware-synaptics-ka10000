#ifndef _HM01B0_HW_H_
#define _HM01B0_HW_H_

#include "mcu.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef CONFIG_CAMERA_SELECT_HM01B0

#define HM01B0_SLV_ADDR             0x24


#define REG_MODEL_ID_H              0x0000      //RO: 0x01
#define REG_MODEL_ID_L              0x0001      //RO: 0xB0
#define REG_SILICON_REV             0x0002      //RO:
#define REG_FRAME_COUNT             0x0005      //RO: 0xFF
#define REG_PIXEL_ORDER             0x0006      //RO: 0x02

#define REG_MODE_SELECT             0x0100
#define REG_IMAGE_ORIENTATION       0x0101
#define REG_SW_RESET                0x0103
#define REG_GRP_PARAM_HOLD          0x0104

#define REG_INTERGRATION_H          0x0202
#define REG_INTEGRRATION_L          0x0203
#define REG_ANALOG_GAIN             0x0205
#define REG_DIGITAL_GAIN_H          0x020E
#define REG_DIGITAL_GAIN_L          0x020F

#define REG_FRAME_LENGTH_LINES_H    0x0340
#define REG_FRAME_LENGTH_LINES_L    0x0341
#define REG_LINE_LENGTH_PCK_H       0x0342
#define REG_LINE_LENGTH_PCK_L       0x0343

#define REG_READOUT_X               0x0383
#define REG_READOUT_Y               0x0387
#define REG_BINNING_MODE            0x0390

#define REG_TEST_PATTERN_MODE       0x0601

#define REG_BLC_CFG                 0x1000
#define REG_BLC_TGT                 0x1003
#define REG_BLI_EN                  0x1006
#define REG_BLC2_TGT                0x1007
#define REG_DPC_CTRL                0x1008
#define REG_SINGLE_THR_HOT          0x100B
#define REG_SINGLE_THR_COLD         0x100C
#define REG_VSYNC_HSYNC_PIXEL_SHIFT_EN  0x1012

#define REG_STATISTIC_CTRL          0x2000
#define REG_MD_LROI_X_START_H       0x2011
#define REG_MD_LROI_X_START_L       0x2012
#define REG_MD_LROI_Y_START_H       0x2013
#define REG_MD_LROI_Y_START_L       0x2014
#define REG_MD_LROI_X_END_H         0x2015
#define REG_MD_LROI_X_END_L         0x2016
#define REG_MD_LROI_Y_END_H         0x2017
#define REG_MD_LROI_Y_END_L         0x2018

#define REG_AE_CTRL                 0x2100
#define REG_AE_TARGET_MEAN          0x2101
#define REG_AE_MIN_MEAN             0x2102
#define REG_CONVERGE_IN_TH          0x2103
#define REG_CONVERGE_OUT_TH         0x2104
#define REG_MAX_INTG_H              0x2105
#define REG_MAX_INTG_L              0x2106
#define REG_MIN_INTG                0x2107
#define REG_MAX_AGAIN_FULL          0x2108
#define REG_MAX_AGAIN_BIN2          0x2109
#define REG_MIN_AGAIN               0x210A
#define REG_MAX_DGAIN               0x210B
#define REG_MIN_DGAIN               0x210C
#define REG_DAMPING_FACTOR          0x210D
#define REG_FS_CTRL                 0x210E
#define REG_FS_60HZ_H               0x210F
#define REG_FS_60HZ_L               0x2110
#define REG_FS_50HZ_H               0x2111
#define REG_FS_50HZ_L               0x2112
#define REG_FS_HYST_TH              0x2113

#define REG_MD_CTRL                 0x2150
#define REG_I2C_CLEAR               0x2153
#define REG_WMEAN_DIFF_TH_H         0x2155
#define REG_WMEAN_DIFF_TH_M         0x2156
#define REG_WMEAN_DIFF_TH_L         0x2157
#define REG_MD_THH                  0x2158
#define REG_MD_THM1                 0x2159
#define REG_MD_THM2                 0x215A
#define REG_MD_THL                  0x215B

#define REG_QVGA_WIN_EN             0x3010
#define REG_SIX_BIT_MODE_EN         0x3011
#define REG_PMU_PROGRAMMABLE_FRAMECNT   0x3020
#define REG_ADVANCE_VSYNC           0x3022
#define REG_ADVANCE_HSYNC           0x3023
#define REG_EARLY_GAIN              0x3035

#define REG_BIT_CONTROL             0x3059
#define REG_OSC_CLK_DIV             0x3060
#define REG_ANA_REGISTER_11         0x3061
#define REG_IO_DRIVE_STR            0x3062
#define REG_IO_DRIVE_STR2           0x3063
#define REG_ANA_REGISTER_14         0x3064
#define REG_OUTPUT_PIN_STATUS_CONTROL   0x3065
#define REG_ANA_REGISTER_17         0x3067
#define REG_PCLK_POLARITY           0x3068

#define REG_I2C_ID_SEL              0x3400
#define REG_I2C_ID_REG              0x3401


//------------- Values ------------
#define MODE_SELECT_STANDBY     0
#define MODE_SELECT_STREAMING   1
#define MODE_SELECT_STREAMING2  3
#define MODE_SELECT_STREAMING3  5


#endif

#ifdef __cplusplus
}
#endif

#endif // _HM01B0_HW_H_
