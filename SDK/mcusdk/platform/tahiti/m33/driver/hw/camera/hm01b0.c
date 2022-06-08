////////////////////////////////////////////////////////////////////////////////
//
// NDA AND NEED-TO-KNOW REQUIRED
//
// Copyright (C) 2021 Synaptics Incorporated. All rights reserved.
//
// This file contains information that is proprietary to Synaptics
// Incorporated ("Synaptics"). The holder of this file shall treat all
// information contained herein as confidential, shall use the
// information only for its intended purpose, and shall not duplicate,
// disclose, or disseminate any of this information in any manner
// unless Synaptics has otherwise provided express, written
// permission.
//
// Use of the materials may require a license of intellectual property
// from a third party or from Synaptics. This file conveys no express
// or implied licenses to any intellectual property rights belonging
// to Synaptics.
//
// INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
// SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
// INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
// INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
// CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
// OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
// BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
// COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
// DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
// TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
//
////////////////////////////////////////////////////////////////////////////////
/* ---------------------------------------------------------------------------------------------
    Below Macro 'PRINT_LEVEL_SET', Should be first line in this .c file, before any includes.
        Valid options are:
            > ASSERT_DISABLE
            > ASSERT_LEVEL_ERR
            > ASSERT_LEVEL_WARN
            > ASSERT_LEVEL_INFO
            > ASSERT_LEVEL_ALL
*/
#define PRINT_LEVEL_SET ASSERT_LEVEL_ERR
//-------------------------------- includes --------------------------------------
#include "mcu.h"
#include "hm01b0_hw.h"
#include "hm01b0.h"
#include "i3cm_drv.h"

#ifdef CONFIG_CAMERA_SELECT_HM01B0
//-------------------------------- defines ---------------------------------------
enum SENSOR_REG_CMD 
{
    CAMERA_CMD_EXEC,
    CAMERA_CMD_COMP,
    CAMERA_CMD_DELAY,
    CAMERA_CMD_END = 0xFF
};

#pragma pack(push, 1)
struct camera_hm01b0_reg_setups
{
    uint8_t cmd;
    uint16_t addr;
    uint8_t  value;
};
#pragma pack(pop)

struct camera_hm01b0_reg_setups hm01b0_regs[] = {
#ifdef CONFIG_CAMERA_SERIAL_MODE
    CAMERA_CMD_EXEC, 0x0100, 0x00, //Power Up
//Analog Settings

#if defined(CONFIG_CAMERA_SOFT_SERIAL_MODE)
    CAMERA_CMD_EXEC, 0x1003, 0xFF, //BLC target :8  at 8 bit mode
    CAMERA_CMD_EXEC, 0x1007, 0xFF, //BLI target :8  at 8 bit mode
#elif defined(CONFIG_CAMERA_HARD_SERIAL_MODE)
    CAMERA_CMD_EXEC, 0x1003, 0x10, //BLC target :8  at 8 bit mode
    CAMERA_CMD_EXEC, 0x1007, 0x10, //BLI target :8  at 8 bit mode
#else
    #error Invalid serial mode config
#endif

    CAMERA_CMD_EXEC, 0x3044, 0x0A, //Increase CDS time for settling
    CAMERA_CMD_EXEC, 0x3045, 0x00, //Make symetric for cds_tg and rst_tg
    CAMERA_CMD_EXEC, 0x3047, 0x0A, //Increase CDS time for settling
    CAMERA_CMD_EXEC, 0x3050, 0xC0, //Make negative offset up to 4x
    CAMERA_CMD_EXEC, 0x3051, 0x42, //Make negative offset up to 4x
    CAMERA_CMD_EXEC, 0x3052, 0x50, //Make negative offset up to 4x
    CAMERA_CMD_EXEC, 0x3053, 0x00, //Make negative offset up to 4x
    CAMERA_CMD_EXEC, 0x3054, 0x03, //tuning sf sig clamping as lowest
    CAMERA_CMD_EXEC, 0x3055, 0xF7, //tuning dsun
    CAMERA_CMD_EXEC, 0x3056, 0xF8, //increase adc nonoverlap clk
    CAMERA_CMD_EXEC, 0x3057, 0x29, //increase adc pwr for missing code
    CAMERA_CMD_EXEC, 0x3058, 0x1F, //turn on dsun
    CAMERA_CMD_EXEC, 0x3059, 0x1E, //
    CAMERA_CMD_EXEC, 0x3064, 0x00, //Trigger SYNC mode disable
    CAMERA_CMD_EXEC, 0x3065, 0x04, //pad pull 0

    CAMERA_CMD_EXEC, 0x3067, 0x02, //Trigger signal sync with MCLK falling edge, MCLK mode en

    //Black Level Control
    CAMERA_CMD_EXEC, 0x1000, 0x43, //BLC_on, IIR
    CAMERA_CMD_EXEC, 0x1001, 0x40, //[6] : BLC dithering en
    CAMERA_CMD_EXEC, 0x1002, 0x32, //blc_darkpixel_thd
    CAMERA_CMD_EXEC, 0x0350, 0x7F, //Dgain Control
    CAMERA_CMD_EXEC, 0x1006, 0x01, //[0] : bli enable

    //DPC
    CAMERA_CMD_EXEC, 0x1008, 0x00, //[2:0] : DPC option 0: DPC off   1 : mono   3 : bayer1   5 : bayer2
    CAMERA_CMD_EXEC, 0x1009, 0xA0, //cluster hot pixel th
    CAMERA_CMD_EXEC, 0x100A, 0x60, //cluster cold pixel th

    CAMERA_CMD_EXEC, 0x100B, 0x90, //single hot pixel th
    CAMERA_CMD_EXEC, 0x100C, 0x40, //single cold pixel th
    CAMERA_CMD_EXEC, 0x1012, 0x07, //Sync. shift enables

    CAMERA_CMD_EXEC, 0x2000, 0x07, //[0] : AE stat en	[1] : MD LROI stat en	[2] : MD GROI stat en	[3] : RGB stat ratio en	[4] : IIR selection (1 -> 16, 0 -> 8)
    CAMERA_CMD_EXEC, 0x2003, 0x00, //MD GROI 0 y start HB
    CAMERA_CMD_EXEC, 0x2004, 0x1C, //MD GROI 0 y start LB
    CAMERA_CMD_EXEC, 0x2007, 0x00, //MD GROI 1 y start HB
    CAMERA_CMD_EXEC, 0x2008, 0x58, //MD GROI 1 y start LB
    CAMERA_CMD_EXEC, 0x200B, 0x00, //MD GROI 2 y start HB
    CAMERA_CMD_EXEC, 0x200C, 0x7A, //MD GROI 2 y start LB
    CAMERA_CMD_EXEC, 0x200F, 0x00, //MD GROI 3 y start HB
    CAMERA_CMD_EXEC, 0x2010, 0xB8, //MD GROI 3 y start LB
    CAMERA_CMD_EXEC, 0x2013, 0x00, //MD GROI 3 y start LB
    CAMERA_CMD_EXEC, 0x2014, 0x58, //MD GROI 3 y start LB
    CAMERA_CMD_EXEC, 0x2017, 0x00, //MD LROI y end HB
    CAMERA_CMD_EXEC, 0x2018, 0x9B, //MD LROI y end LB

#if defined(CONFIG_CAMERA_SOFT_SERIAL_MODE)
    CAMERA_CMD_EXEC, 0x2100, 0x00, //AE off
#elif defined(CONFIG_CAMERA_HARD_SERIAL_MODE)
    CAMERA_CMD_EXEC, 0x2100, 0x01, //AE on
#endif

    CAMERA_CMD_EXEC, 0x2104, 0x07, //converge out th
    CAMERA_CMD_EXEC, 0x2105, 0x02, //max INTG Hb
    CAMERA_CMD_EXEC, 0x2106, 0x12, //max INTG Lb
    CAMERA_CMD_EXEC, 0x2108, 0x03, //max AGain in full
    CAMERA_CMD_EXEC, 0x2109, 0x03, //max AGain in bin2
    CAMERA_CMD_EXEC, 0x210B, 0x80, //max DGain
    CAMERA_CMD_EXEC, 0x210F, 0x00, //FS 60Hz Hb
    CAMERA_CMD_EXEC, 0x2110, 0x42, //FS 60Hz Lb
    CAMERA_CMD_EXEC, 0x2111, 0x00, //Fs 50Hz Hb
    CAMERA_CMD_EXEC, 0x2112, 0x50, //FS 50Hz Lb

    //Motion Detection
    CAMERA_CMD_EXEC, 0x2150, 0x03, //[0] : MD LROI en	[1] : MD GROI en

    //Frame size to 384*384
    CAMERA_CMD_EXEC, 0x0202, 0x01,
    CAMERA_CMD_EXEC, 0x0203, 0x08,

    CAMERA_CMD_EXEC, 0x0340, 0x01,
    CAMERA_CMD_EXEC, 0x0341, 0x80,
    CAMERA_CMD_EXEC, 0x0342, 0x01,
    CAMERA_CMD_EXEC, 0x0343, 0x80,

    CAMERA_CMD_EXEC, 0x3010, 0x00, //[0] : window mode	0 : full frame 324x324	1 : QVGA
    CAMERA_CMD_EXEC, 0x0383, 0x01,
    CAMERA_CMD_EXEC, 0x0387, 0x01,
    CAMERA_CMD_EXEC, 0x0390, 0x00,

    //Bit width
    CAMERA_CMD_EXEC, 0x3011, 0x70,
    CAMERA_CMD_EXEC, 0x3022, 0x00, //Advance VSYNC field from 0 to 20 (1LSB = 1 row period)
    CAMERA_CMD_EXEC, 0x3023, 0x00, //Advance HSYNC field from 0 to 20 (1LSB = 1 pixel clock)

    CAMERA_CMD_EXEC, 0x3064, 0x04, //[2]: Trigger En
    CAMERA_CMD_EXEC, 0x3059, 0x22, //[7]: Self OSC En, [6]: 4bit mode, [5]: serial mode, [4:0]: keep value as 0x02
#if defined(CONFIG_CAMERA_SOFT_SERIAL_MODE)
    CAMERA_CMD_EXEC, 0x3060, 0x14, //[5]: gated_clock, [4]: msb first,  not gated clock vt_reg_div 1/8 vt_sys_div 1/8
                                   //[3:2]: vt_reg_div -> div by 4/8/1/2
                                   //[1; #0]: vt_sys_div -> div by 8/4/2/1
#elif defined(CONFIG_CAMERA_HARD_SERIAL_MODE)
    CAMERA_CMD_EXEC, 0x3060, 0x34, //[5]: gated_clock, [4]: msb first,  not gated clock vt_reg_div 1/8 vt_sys_div 1/8
                                   //[3:2]: vt_reg_div -> div by 4/8/1/2
                                   //[1; #0]: vt_sys_div -> div by 8/4/2/1
#endif
    CAMERA_CMD_EXEC, 0x3062, 0x11, //increase slew rate

    //CMU update
    CAMERA_CMD_EXEC, 0x0104, 0xfe, //increase slew rate

    //Turn on rolling shutter
    CAMERA_CMD_EXEC, 0x0100, 0x05, //mode_select 00 : standby - wait fir I2C SW trigger     01 : streaming     03 : output "N" frame, then enter standby     04 : standby - wait for HW trigger (level), then continuous video out til HW TRIG goes off    06 : standby - wait for HW trigger (edge), then output "N" frames then enter standby

    CAMERA_CMD_EXEC, 0x3023, 0x0,  //Offset to align WS rising edge with HSYNC rising edege to catch first bit
    CAMERA_CMD_EXEC, 0x3068, 0x21, //PCLKO falling edge so Tahiti can strobe data correctly

#else
    //1bit parallel qvga 1bit msb
    CAMERA_CMD_EXEC, 0x0100, 0x0, //Power up

    //Analog
    CAMERA_CMD_EXEC, 0x1003, 0x10, //BLC
    CAMERA_CMD_EXEC, 0x1007, 0x10, //BLI

    CAMERA_CMD_EXEC, 0x3044, 0x0a, //Increase CDS time for settling
    CAMERA_CMD_EXEC, 0x3045, 0x00, //# Make symetric for cds_tg and rst_tg
    CAMERA_CMD_EXEC, 0x3047, 0x0A, //# Increase CDS time for settling
    CAMERA_CMD_EXEC, 0x3050, 0xC0, //# Make negative offset up to 4x
    CAMERA_CMD_EXEC, 0x3051, 0x42, //#
    CAMERA_CMD_EXEC, 0x3052, 0x50, //#
    CAMERA_CMD_EXEC, 0x3053, 0x00, //#
    CAMERA_CMD_EXEC, 0x3054, 0x03, //#  tuning sf sig clamping as lowest
    CAMERA_CMD_EXEC, 0x3055, 0xF7, //#  tuning dsun
    CAMERA_CMD_EXEC, 0x3056, 0xF8, //#  increase adc nonoverlap clk
    CAMERA_CMD_EXEC, 0x3057, 0x29, //#  increase adc pwr for missing code
    CAMERA_CMD_EXEC, 0x3058, 0x1F, //#  turn on dsun
    CAMERA_CMD_EXEC, 0x3059, 0x1E, //#
    CAMERA_CMD_EXEC, 0x3064, 0x00, //#
    CAMERA_CMD_EXEC, 0x3065, 0x04, //#  pad pull 0

    //Digital function

    //#Trigger sync with MCLK falling edge
    CAMERA_CMD_EXEC, 0x3067, 0x02, //#

    //# BLC
    CAMERA_CMD_EXEC, 0x1000, 0x43, //# BLC_on, IIR
    CAMERA_CMD_EXEC, 0x1001, 0x40, //# [6] : BLC dithering en
    CAMERA_CMD_EXEC, 0x1002, 0x32, //# #  blc_darkpixel_thd

    //# Dgain
    CAMERA_CMD_EXEC, 0x0350, 0x7F, // # Dgain Control

    //# BLI
    CAMERA_CMD_EXEC, 0x1006, 0x01, //# [0] : bli enable

    //# DPC
    CAMERA_CMD_EXEC, 0x1008, 0x00, //# [2:0] : DPC option 0: DPC off   1 : mono   3 : bayer1   5 : bayer2
    CAMERA_CMD_EXEC, 0x1009, 0xA0, //# cluster hot pixel th
    CAMERA_CMD_EXEC, 0x100A, 0x60, //# cluster cold pixel th
    CAMERA_CMD_EXEC, 0x100B, 0x90, //# single hot pixel th
    CAMERA_CMD_EXEC, 0x100C, 0x40, //# single cold pixel th

    //#set_hm01b0_reg 0x1012 0x00; # Sync. shift disable
    CAMERA_CMD_EXEC, 0x1012, 0x07, //# Sync. shift enables

    //# ROI Stattistic
    CAMERA_CMD_EXEC, 0x2000, 0x07, //# [0] : AE stat en	[1] : MD LROI stat en	[2] : MD GROI stat en	[3] : RGB stat ratio en	[4] : IIR selection (1 -> 16, 0 -> 8)
    CAMERA_CMD_EXEC, 0x2003, 0x00, //# MD GROI 0 y start HB
    CAMERA_CMD_EXEC, 0x2004, 0x1C, //# MD GROI 0 y start LB
    CAMERA_CMD_EXEC, 0x2007, 0x00, //# MD GROI 1 y start HB
    CAMERA_CMD_EXEC, 0x2008, 0x58, //# MD GROI 1 y start LB
    CAMERA_CMD_EXEC, 0x200B, 0x00, //# MD GROI 2 y start HB
    CAMERA_CMD_EXEC, 0x200C, 0x7A, //# MD GROI 2 y start LB
    CAMERA_CMD_EXEC, 0x200F, 0x00, //# MD GROI 3 y start HB
    CAMERA_CMD_EXEC, 0x2010, 0xB8, //# MD GROI 3 y start LB

    CAMERA_CMD_EXEC, 0x2013, 0x00, //# MD LRIO y start HB
    CAMERA_CMD_EXEC, 0x2014, 0x58, //# MD LROI y start LB
    CAMERA_CMD_EXEC, 0x2017, 0x00, //# MD LROI y end HB
    CAMERA_CMD_EXEC, 0x2018, 0x9B, //# MD LROI y end LB

    //# AE
    CAMERA_CMD_EXEC, 0x2100, 0x01, //# [0]: AE control enable on
    //#set_hm01b0_reg 0x2100 0x00; # [0]: AE control enable off
    CAMERA_CMD_EXEC, 0x2101, 0x80, //#Exposure target avg default 0x3C too dark
    CAMERA_CMD_EXEC, 0x2102, 0x10, //#Exposure target min default 0x0A
    CAMERA_CMD_EXEC, 0x2104, 0x07, //# converge out th
    CAMERA_CMD_EXEC, 0x2105, 0x02, //# max INTG Hb
    CAMERA_CMD_EXEC, 0x2106, 0x12, //# max INTG Lb
    CAMERA_CMD_EXEC, 0x2108, 0x03, //# max AGain in full
    CAMERA_CMD_EXEC, 0x2109, 0x03, //# max AGain in bin2
    CAMERA_CMD_EXEC, 0x210B, 0x80, //# max DGain
    CAMERA_CMD_EXEC, 0x210F, 0x00, //# FS 60Hz Hb
    CAMERA_CMD_EXEC, 0x2110, 0x42, //# FS 60Hz Lb
    CAMERA_CMD_EXEC, 0x2111, 0x00, //# Fs 50Hz Hb
    CAMERA_CMD_EXEC, 0x2112, 0x50, //# FS 50Hz Lb

    //# MD
    CAMERA_CMD_EXEC, 0x2150, 0x03, //# [0] : MD LROI en	[1] : MD GROI en

    //#---------------------------------------------------
    //# frame rate : 15 FPS
    //#---------------------------------------------------
    //#set_hm01b0_reg 0x0340 0x02; # smia frame length Hb		532
    //#set_hm01b0_reg 0x0341 0x14; # smia frame length Lb
    //#set_hm01b0_reg 0x0341 0x10; # smia frame length Lb  528 to make it multiple of 48
    CAMERA_CMD_EXEC, 0x0202, 0x01, //# Coarse integration time in lines high byte
    CAMERA_CMD_EXEC, 0x0203, 0x08, //# Coarse integration time in lines low byte
    //#set_hm01b0_reg 0x0342 0x01; # smia line length Hb
    //#set_hm01b0_reg 0x0343 0x78; # smia line length Lb		376
    //#set_hm01b0_reg 0x0343 0x80; # smia line length Lb		384 to make multiple of 48

    //#Ted's setting
    //#324x324
    //#set_hm01b0_reg 0x0340 0x01
    //#set_hm01b0_reg 0x0341 0x44
    //#set_hm01b0_reg 0x0342 0x01
    //##set_hm01b0_reg 0x0343 0x44
    //#set_hm01b0_reg 0x0343 0x74
    //#12.288MHz MCLK div 8 = 1.536MHz pix_clk  10.416 FPS
    CAMERA_CMD_EXEC, 0x0340, 0x01,
    CAMERA_CMD_EXEC, 0x0341, 0x80,
    CAMERA_CMD_EXEC, 0x0342, 0x01,
    CAMERA_CMD_EXEC, 0x0343, 0x80,

    //#---------------------------------------------------
    //# Resolution : QVGA or full 324x244
    //#---------------------------------------------------
    //#QVGA
    //#set_hm01b0_reg 0x3010 0x01; # [0] : window mode	0 : full frame 324x324	1 : QVGA
    //#Full frame
    CAMERA_CMD_EXEC, 0x3010, 0x00, //# [0] : window mode	0 : full frame 324x324	1 : QVGA
    CAMERA_CMD_EXEC, 0x0383, 0x01, //#
    CAMERA_CMD_EXEC, 0x0387, 0x01, //#
    CAMERA_CMD_EXEC, 0x0390, 0x00, //#

    //#---------------------------------------------------
    //# bit width Selection
    //#---------------------------------------------------
    CAMERA_CMD_EXEC, 0x3011, 0x70, //# [0] : 6 bit mode enable
    CAMERA_CMD_EXEC, 0x3022, 0x00, //# Advance VSYNC field from 0 to 20 (1LSB = 1 row period)
    CAMERA_CMD_EXEC, 0x3023, 0x00, //# Advance HSYNC field from 0 to 20 (1LSB = 1 pixel clock)

    CAMERA_CMD_EXEC, 0x3064, 0x04, //# [2]: Trigger En
#if defined(CONFIG_CAMERA_PARALLEL_WIDTH_1)
    CAMERA_CMD_EXEC, 0x3059, 0x22, //# [7]: Self OSC En, [6]: 4bit mode, [5]: serial mode, [4:0]: keep value as 0x02
#elif defined(CONFIG_CAMERA_PARALLEL_WIDTH_2)

#elif defined(CONFIG_CAMERA_PARALLEL_WIDTH_4)
    CAMERA_CMD_EXEC, 0x3059, 0x42, //# [7]: Self OSC En, [6]: 4bit mode, [5]: serial mode, [4:0]: keep value as 0x02
#elif defined(CONFIG_CAMERA_PARALLEL_WIDTH_8)
    CAMERA_CMD_EXEC, 0x3059, 0x02,  //; # [7]: Self OSC En, [6]: 4bit mode, [5]: serial mode, [4:0]: keep value as 0x02
#endif
    //#set_hm01b0_reg 0x3060 0x37; # [5]: gated_clock, [4]: msb first, vt_reg_div 1/8 vt_sys_div 1/1
    //#set_hm01b0_reg 0x3060 0x38; # [5]: gated_clock, [4]: msb first, vt_reg_div 1/1 vt_sys_div 1/8
    //#set_hm01b0_reg 0x3060 0x39; # [5]: gated_clock, [4]: msb first, vt_reg_div 1/1 vt_sys_div 1/8
    //#set_hm01b0_reg 0x3060 0x15; # [5]: gated_clock, [4]: msb first, not gated vt_reg_div 1/8 vt_sys_div 1/4
    //#set_hm01b0_reg 0x3060 0x35; # [5]: gated_clock, [4]: msb first, vt_reg_div 1/8 vt_sys_div 1/4
    //#set_hm01b0_reg 0x3060 0x36; # [5]: gated_clock, [4]: msb first,  vt_reg_div 1/8 vt_sys_div 1/2
    //#set_hm01b0_reg 0x3060 0x34; # [5]: gated_clock, [4]: msb first,  vt_reg_div 1/8 vt_sys_div 1/8
    CAMERA_CMD_EXEC, 0x3060, 0x14, //# [5]: gated_clock, [4]: msb first,  not gated clock vt_reg_div 1/8 vt_sys_div 1/8
                                   //# [3:2]: vt_reg_div -> div by 4/8/1/2
                                   //# [1; #0]: vt_sys_div -> div by 8/4/2/1

    CAMERA_CMD_EXEC, 0x3062, 0x11, //#  increase slew rate

    //#---------------------------------------------------
    //# CMU update
    //#---------------------------------------------------

    CAMERA_CMD_EXEC, 0x0104, 0xfe, //# was 0100

    //#---------------------------------------------------
    //# Turn on rolling shutter
    //#---------------------------------------------------
    CAMERA_CMD_EXEC, 0x0100, 0x05, //# was 0005; # mode_select 00 : standby - wait fir I2C SW trigger     01 : streaming     03 : output "N" frame, then enter standby     04 : standby - wait for HW trigger (level), then continuous video out til HW TRIG goes off    06 : standby - wait for HW trigger (edge), then output "N" frames then enter standby
    
    CAMERA_CMD_EXEC, 0x3023, 0x0,  //Offset to align WS rising edge with HSYNC rising edege to catch first bit
    CAMERA_CMD_EXEC, 0x3068, 0x20, //PCLKO rising edge

#endif
};


//-------------------------------- functions --------------------------------------

uint8_t camera_get_reg(uint16_t addr)
{
    uint8_t reg_addr[2] = {0, 0};
    uint8_t value = 0;
    reg_addr[1] = addr & 0x00ff;
    reg_addr[0] = (addr >> 8) & 0x00ff;

    i2cm_xfer_write(DEV_I3CM0, HM01B0_SLV_ADDR, 100000, reg_addr, 2);
    i2cm_xfer_read(DEV_I3CM0, HM01B0_SLV_ADDR, 100000, &value, 1);

    return value;
}

void camera_set_reg(uint16_t addr, uint8_t data)
{
	uint8_t reg_addr[3] = {0, 0, 0};
	uint8_t value = data;
	reg_addr[1] = addr & 0x00ff;
	reg_addr[0] = (addr >> 8) & 0x00ff;
	reg_addr[2] = data;

	i2cm_xfer_write(DEV_I3CM0, HM01B0_SLV_ADDR, 100000, reg_addr, 3);
    volatile uint8_t get_data = 0;

    get_data = camera_get_reg(addr);
    //if(get_data != data)
    //{
    //    Print_ERR("Error: Set 0x%x, Expect 0x%x, Get 0x%x\n", addr, data, get_data);
    //}
}


int hm01b0_execute_scripts(struct camera_hm01b0_reg_setups *regs,  int size)
{
    int err = 0;
    int i = 0;

    for (i = 0; i < size; i++)
    {
        if(CAMERA_CMD_EXEC == regs[i].cmd)
        {
            camera_set_reg(regs[i].addr, regs[i].value);
        }
        else if(CAMERA_CMD_COMP == regs[i].cmd)
        {
            uint8_t value = camera_get_reg(regs[i].addr);
            if (value != regs[i].value)
            {
                err = -(1000 + i);
                break;
            }
        }
        else if(CAMERA_CMD_DELAY == regs[i].cmd)
        {
            os_TaskSleep(regs[i].value);
        }
        else if(CAMERA_CMD_END == regs[i].cmd)
        {
            break;
        }
        else
        {
            err = -2000;
            break;
        }
    }

    return err;
}

int reset_hm01b0()
{
    uint8_t version_h, version_l;
    camera_set_reg(REG_SW_RESET, 0x00);

    version_h  = camera_get_reg(REG_MODEL_ID_H);
    version_l = camera_get_reg(REG_MODEL_ID_L);
    if (version_h != 0x01 || version_l != 0xb0)
    {
        Print_ERR("ERR:01B0:%d\n", __LINE__);
        return -1;
    }

    return 0;
}


int init_hm01b0()
{
    int err = 0;
    if (reset_hm01b0() < 0)
    {
        return -1;
    }

    err = hm01b0_execute_scripts(hm01b0_regs, sizeof(hm01b0_regs) / sizeof(struct camera_hm01b0_reg_setups));

    //Print_ERR("Execute:%d\n", err);

    return err;
}

void hm01b0_dump_reg(uint16_t addr)
{
    int val = 0;
    val = camera_get_reg(addr);
    Print_ERR("0x%x = 0x%x\n", addr, val);
}

void hm01b0_dump(struct camera_hm01b0_reg_setups *regs, int size)
{
    int i = 0;

    for(i = 0; i<size;i++)
    {
        if(regs[i].cmd == CAMERA_CMD_EXEC || regs[i].cmd == CAMERA_CMD_COMP)
        {
            hm01b0_dump_reg(regs[i].addr);
        }
        
        if(regs[i].cmd == CAMERA_CMD_END)
            break;
    }
}

#if defined(CONFIG_CAMERA_SOFT_SERIAL_MODE)
/**
 * @brief Set the work mode 
 * 
 * @param mode : 0 All white mode
 *               1 Normal mode
 */
void set_work_mode(int mode)
{
    if(mode == 0)
    {
        camera_set_reg(REG_BLC_TGT,         0xFF);
        camera_set_reg(REG_BLC2_TGT,        0xFF);
        camera_set_reg(REG_AE_TARGET_MEAN,  0x3C);
        camera_set_reg(REG_AE_MIN_MEAN,     0x0A);
        camera_set_reg(REG_AE_CTRL,         0x00);
        camera_set_reg(REG_OSC_CLK_DIV,     0x14);
    }
    else
    {
        camera_set_reg(REG_BLC_TGT,         0x10);
        camera_set_reg(REG_BLC2_TGT,        0x10);
        camera_set_reg(REG_AE_TARGET_MEAN,  0x80);
        camera_set_reg(REG_AE_MIN_MEAN,     0x10);
        camera_set_reg(REG_AE_CTRL,         0x01);
        camera_set_reg(REG_OSC_CLK_DIV,     0x34);
    }
}
#endif

#endif //CONFIG_CAMERA_SELECT_HM01B0
