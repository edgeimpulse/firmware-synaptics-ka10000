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
//  Copyright (C) 2018 Synaptics Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCU_CTRL_H_
#define _MCU_CTRL_H_

#include "mcu.h"
#include "mem.h"
#include "command.h"
#include "mcu_cmds.h"
//Decoupled between ctrl_task and CAF.
//#include <base_module_ex.h>

#if defined CONFIG_MCU_DRV_USB
#include <pub.h>
#endif

#ifdef CONFIG_CAF
#define __stdcall
#include "control_ex.h"
#endif

#if defined CONFIG_MCU_CTRL_SCRIPT_PAR
#include "par_process_in.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__DOMAIN_NS)

#ifdef CONFIG_MCU_DRV_MBOX
#include "mbox_drv.h"
#endif

typedef struct ctrl_info_s
{
    uint32_t mcu_version;
    Command cmd_bufs[NUM_OF_CMD_MSG_BUFS];
} ctrl_info_t;



#ifdef CONFIG_MCU_CTRL_ELK_TUNNELING
typedef struct
{
	int32_t msg_type;
	int32_t msg_len;
	int32_t slv_addr;//for ELK_MSG_WRITE & ELK_MSG_READ
} elkxfer_msg_s;
enum elk_msg_type{
	ELK_MSG_CMD = 1,
	ELK_MSG_WRITE,
	ELK_MSG_READ
	//TB improvement
};
enum elk_tunneling_status{
	ELK_TUNNEL_IDLE = 1,
	ELK_TUNNEL_XFER_START,
	ELK_TUNNEL_XFER_DONE,
	ELK_TUNNEL_RECV_DONE,
	ELK_TUNNEL_WAIT_REPLY,
	ELK_TUNNEL_READ_REPLY,
	ELK_TUNNEL_READ_REPLY_DONE
};
#endif

int send_cmd_to_mcu_mgr(Command *cmd);
int send_cmd_to_mcu_mgr_by_event_task(Command *cmd);
int ctrl_mgr_init_start(void);
void mcu_start_ctrl_task(void *param);
int process_router_cmd(Command *cmd);
void process_cmd(Command *cmd);
void start_cmd_handler(void);
long ctrl_hook(void *pvParam);
int exec_conf_file(void);

#else // !__DOMAIN_NS

#ifdef CONFIG_MCU_DRV_MBOX
#include "mbox_drv.h"
#endif

typedef struct file_err_info_s
{
	CommandHeader file_err_cmd_header ; 
	uint32_t      file_id ;
	uint32_t      rsvd; 
} file_err_info_t;

struct ctrl_info
{
  uint32_t mcu_version;
  Command cmd_bufs[NUM_OF_CMD_MSG_BUFS];
  os_QHandle cmd_queue;
#ifdef CONFIG_MCU_CTRL_EVENT_HANDLER_SYNC_TASK
#define EHMD_BUFFER_SIZE		16
  os_QHandle ehmd_queue;
  Command ehmd_bufs[EHMD_BUFFER_SIZE];
#endif
#ifdef CONFIG_MCU_CTRL_SCRIPT_PAR
  ParProcessState par_process_data;
#endif
  CommandHeader conf_err_cmd_header;
  file_err_info_t last_file_err_info;
#ifdef CONFIG_MCU_MGR_MODES
  uint32_t      mode_id;
  uint32_t      script_data[MAX_NUM_SCRIPT_DATA];
#endif
#ifdef CONFIG_MCU_CTRL_SFS_MODES_ATBOOT
  uint32_t      mode_file_id[CONFIG_MCU_SIZE_MODES_ATBOOT];
#endif
#if defined(CONFIG_MCU_CTRL_GPIO_RESET_API) || \
    defined(CONFIG_MCU_CTRL_ENCODER_API)    || \
	defined(CONFIG_MCU_CTRL_SENSOR_MONITOR) || \
	defined(CONFIG_MCU_DRV_KEYPAD)          || \
	defined(CONFIG_MCU_DRV_SWPWM)
  uint32_t gpio_used;
#endif
  uint32_t bg_upgrade_status;
} ctrl;

#ifdef CONFIG_MCU_CTRL_ELK_TUNNELING
typedef struct
{
	int32_t msg_type;
	int32_t msg_len;
	int32_t slv_addr;//for ELK_MSG_WRITE & ELK_MSG_READ
} elkxfer_msg_s;
enum elk_msg_type{
	ELK_MSG_CMD = 1,
	ELK_MSG_WRITE,
	ELK_MSG_READ
	//TB improvement
};
enum elk_tunneling_status{
	ELK_TUNNEL_IDLE = 1,
	ELK_TUNNEL_XFER_START,
	ELK_TUNNEL_XFER_DONE,
	ELK_TUNNEL_RECV_DONE,
	ELK_TUNNEL_WAIT_REPLY,
	ELK_TUNNEL_READ_REPLY,
	ELK_TUNNEL_READ_REPLY_DONE
};
#endif

#define IO_TYPE_OFFSET   (0U)
#define IO_INDEX_OFFSET  (4U)
#define IO_TYPE_MASK     (0xF)
#define IO_INDEX_MASK    (0x1F)
#define IO_CH_NUM_OFFSET (9U)
#define IO_CH_NUM_MASK   (3U)

typedef union {
	struct {
		uint32_t io_type     : 4;	 
		uint32_t io_sub_type : 8;
		uint32_t io_index    : 5;
		uint32_t io_ch_num   : 2;
        uint32_t reserved3   : 13;
	} bf;
	uint32_t dw;
} io_info_s;

typedef struct io_adma_io_s {
  uint32_t sample_rate;
  io_info_s input_io_info;
  io_info_s output_io_info;
} io_adma_io_t;

typedef union perip_rst_s
{
	struct {
		uint32_t gpio_num : 5;
		uint32_t polarity : 1;
		uint32_t active_delay : 10;
		uint32_t reserved : 16;
	} conf;
	uint32_t full;
} perip_rst_t;

int send_cmd_to_mcu_mgr(Command *cmd);
int ctrl_mgr_init_start(void);
void mcu_start_ctrl_task(void *param);
#ifdef CONFIG_MCU_CTRL_EVENT_HANDLER_SYNC_TASK
void mcu_start_ehmd(void *param);
#endif
int process_router_cmd(Command *cmd);
void process_cmd(Command *cmd);
void start_cmd_handler(void);
long ctrl_hook(void *pvParam);
int exec_conf_file(void);
int process_adma_cmd(Command *cmd);
__STATIC_FORCEINLINE int32_t SEND_CMD_TO_MCU_S(Command *cmd) {
    extern int32_t process_cmd_s(Command *cmd);
    return process_cmd_s(cmd);
}

#ifdef CONFIG_MCU_MEM_BULCK_ACS
enum MemBulkAcsCtrl_E{
	MEM_BULK_CTL_VALID_ADDR = 1,
	MEM_BULK_CTL_INF,
	MEM_BULK_CTL_TYPE,  
}; 

enum MemBulkAcsCtrlType_E{
	MEM_TYPE_CAPE_A_X = 1,
	MEM_TYPE_CAPE_A_Y,
	MEM_TYPE_CAPE_B_X,
	MEM_TYPE_CAPE_B_Y,
	MEM_TYPE_HIFI3_SEG0,
	MEM_TYPE_HIFI3_SEG1,
}; 


typedef struct  MemBulkAcsCtrl_S
{
     volatile uint32_t   start_addr_limit  ;
     volatile uint32_t   end_addr_limit    ;
     volatile uint32_t*  cur_start_addr    ;//New setting will override the start address 
     volatile uint32_t   tgt_dat_cnt       ;
     volatile uint32_t   expect_chksum     ; 
     volatile uint32_t   cur_dat_cnt       ;
     volatile uint32_t   cur_calc_chksum   ;
	 volatile uint32_t   cur_mem_ctl_type  ; 
} MemBulkAcsCtrl_T;
#endif 

#endif // !__DOMAIN_NS

#ifdef __cplusplus
}
#endif

#endif // _MCU_CTRL_H_
