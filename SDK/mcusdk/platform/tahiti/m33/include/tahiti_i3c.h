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
//  Copyright (C) 2009-2013 Conexant Systems Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _MCU_DRV_I3C_HW_H_
#define _MCU_DRV_I3C_HW_H_

#ifdef __cplusplus
  extern "C" {
#endif

#if (defined CONFIG_MCU_DRV_I3CS) || (defined CONFIG_MCU_DRV_I3CM)

//PAD register for Tahiti  
//#define PADCTRL_I3C_I2C_SCK_ADDRESS  0x42023458
//#define PADCTRL_I3C_I2C_SDA_ADDRESS  0x4202345c
//Clock Control for Tahiti 
//#define CR_CLK_I3C_ADDRESS  0x4200787c
//I3C CONTROL Register 
//#define I3C_CTRL_ADDRESS  0x42006508
//#define I3C_SRAM_TIM_CTRL_ADDRESS  0x42006510
//#define I3C_DBGL_ADDRESS  0x42006590
//#define I3C_DBGH_ADDRESS  0x42006594

//I3C IP register for Tahiti
#define   I3C0_REGS_BASE_ADDR                                 0x42042000 
#define   MEMMAP_I3CTL0_REG_BASE                              I3C0_REGS_BASE_ADDR 


//------------------------------  REGISTER DEFINES  ------------------------------
/* DWC_MIPI_I3C register offsets */
#ifndef BIT
#define BIT(nr)			(1 << (nr))
#endif
#define FIFO_WIDTH (4) // bytes

#define I3C_REG_DEVICE_CTRL                           (0x0)

    #define DEV_CTRL_ENABLE (1 << 31)
    #define DEV_CTRL_RESUME (1 << 30)
    #define DEV_CTRL_ABORT (1 << 29)
    #define DEV_CTRL_DMA_EN (1 << 28)
    #define DEV_CTRL_ADAPTIVE_I2C_I3C (1 << 27)
    #define DEV_CTRL_HOT_JOIN (1 << 8)
    #define I2C_SLAVE_PRESENT (1 << 7)
#define I3C_REG_DEVICE_ADDR                           (0x4)
    #define DYNAMIC_ADDR_VALID (1 << 31)
    #define DYNAMIC_ADDR_MASK (0x3F)
    #define DYNAMIC_ADDR_LSB (16)

#define I3C_REG_HW_CAPABILITY                         (0x8)
    #define HDR_TS_EN_BIT (1 << 9)
    #define HDR_DDR_EN_BIT (1 << 8)
    #define DAA_EN_BIT (1)
#define I3C_REG_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD     (0xc)
    #define ROC_BIT (1 << 26)
    #define DEV_COUNT_MASK (0x1F)
    #define DEV_COUNT_LSB 21 

    #define DEV_INDX_MASK (0x1F)
    #define DEV_INDX_LSB 16 

    #define I3C_CMD_MASK (0xFF)
    #define CMD_LSB  7

    #define CMD_TID_MASK 0xF
    #define CMD_TID_LSB 3

    #define XFER_CMD 0
    #define XFER_ARG 1
    #define SHORT_DATA_ARG 2
    #define ADDR_ASSGN_CMD 3
    #define CMD_ATTR_LSB 0

#define I3C_REG_COMMAND_QUEUE_PORT_TRANSFER_COMMAND   (0xc)
    #define TOC_BIT (1 << 30)  // terminate after comd
    #define RNW_BIT (1 << 28)  // 0: write, 1: read
    #define SDAP_BIT (1 << 27) // 0: prior is transfer, 1: prior is short data
    #define SPEED_MASK 0x7
    #define SPEED_LSB 21
    #define CP_BIT (1 << 15)  // 0: priviate, 1:  ccc/hdr: 

#define I3C_REG_COMMAND_QUEUE_PORT_SHORT_DATA_ARG     (0xc)
    #define DATA_BYTE_LSB (8)
    #define BYTE_STRB_LSB (3)
#define I3C_REG_COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT  (0xc)
    #define DATA_LENGTH_LSB (16)
    #define DATA_LENGTH_MASK (0xFFFF)
    #define DMA_BIT (1 << 15)

#define I3C_REG_RESPONSE_QUEUE_PORT                   (0x10)
    #define RESP_ERR_MASK 0xF
    #define RESP_ERR_LSB 28
#define RESP_IS_RX (1 << 27)
    #define RESP_TID_MASK 0x7
    #define RESP_TID_LSB 24
    #define RESP_DATA_LEN_MASK 0xFFFF
    #define RESPONSE_QUEUE_PORT_ERR_STATUS	0xf0000000 /* Error Status */
    #define RESPONSE_QUEUE_PORT_DATA_LEN	0x0000ffff /* Data Length */

    #define RESPONSE_ERROR_CRC   1
    #define RESPONSE_ERROR_PARITY 2
    #define RESPONSE_ERROR_FRAME 3
    #define RESPONSE_ERROR_IBA_NACK		4 /* IBA Nack'ed */
    #define RESPONSE_ERROR_ADDRESS_NACK	5 /* Address Nack'ed */
    #define RESPONSE_ERROR_OVER_UNDER_FLOW	6 /* Receive Buffer Overflow/Transmit Buffer Underflow*/
    #define RESPONSE_ERROR_MAX_RETRY 7
    #define RESPONSE_ERROR_TRANSF_ABORT	8 /* Transfer Aborted */
    #define RESPONSE_ERROR_I2C_W_NACK_ERR	9 /* I2C Slave Write Data NACK Error */
    #define RESPONSE_ERROR_MASTER_EARLY_TERMINATED  10  /*master early terminated*/ 

#define I3C_REG_RX_DATA_PORT                          (0x14)
#define I3C_REG_TX_DATA_PORT                          (0x14)
#define I3C_REG_IBI_QUEUE_DATA                        (0x18)
#define I3C_REG_IBI_QUEUE_STATUS                      (0x18)

#define I3C_REG_QUEUE_THLD_CTRL                       (0x1c)
    #define IBI_STATUS_THLD_MASK 0xFF
    #define IBI_STATUS_THLD_LSB 24
    #define RESP_BUF_THLD_MASK 0xFF
    #define RESP_BUF_THLD_LSB 8
    #define CMD_EMPTY_BUF_THLD_MASK 0xFF
    #define CMD_EMPTY_BUF_THLD_LSB 0

#define I3C_REG_DATA_BUFFER_THLD_CTRL                 (0x20)
    #define RX_START_THLD_MASK 7
    #define RX_START_THLD_LSB 24
    #define TX_START_THLD_MASK 7
    #define TX_START_THLD_LSB 16
    #define RX_BUF_THLD_MASK 7
    #define RX_BUF_THLD_LSB 8
    #define TX_EMPTY_BUF_THLD_MASK 7
    #define TX_EMPTY_BUF_THLD_LSB 0

#define I3C_REG_IBI_QUEUE_CTRL                        (0x24)
//#define I3C_REG_IBI_MR_SEL                            (0x28)
#define I3C_REG_IBI_MR_REQ_REJECT                     (0x2c)
#define I3C_REG_IBI_SIR_REQ_REJECT                    (0x30)
    #define MR_SEL_MASTER_REQUEST		(1 << 1) /* In-Band Master request */
    #define IBI_SIR_REQ_REJECT_ALL		0xffffffff /* Nack all IBI SIR*/
    #define MR_REQ_REJECT_NACK		(1 << 1) /* NACK Master/P2P Request */

#define I3C_REG_RESET_CTRL                            (0x34)
    #define IBI_QUEUE_RST (1 << 5)
    #define RX_FIFO_RST (1 << 4)
    #define TX_FIFO_RST (1 << 3)
    #define RESP_QUEUE_RST (1 << 2)
    #define CMD_QUEUE_RST (1 << 1)
    #define SOFT_RST (1 << 0)
#define RESET_ALL_BUFFER (IBI_QUEUE_RST | RX_FIFO_RST |\
    TX_FIFO_RST | RESP_QUEUE_RST | CMD_QUEUE_RST)

#define I3C_REG_SLV_EVENT_CTRL                        (0x38)
    #define MWL_UPDATED (1 << 7)
    #define MRL_UPDATED (1 << 6)
    #define HJ_INTR_REQ (1 << 3)
    #define SLAVE_INTR_REQ (1 << 0)
#define I3C_REG_INTR_STATUS                           (0x3c)
#define I3C_REG_INTR_STATUS_EN                        (0x40)
    #define INTR_BUS_OWNER_UPDATED_BIT (1 << 13) // only for 2nd master
    #define INTR_IBI_UPDATED_BIT (1 << 12) // only for 2nd master or slave
    #define INTR_READ_REQ_RECV_BIT (1 << 11) // only for slave
    #define INTR_DEFSLV_BIT (1 << 10)  // only for 2nd master
    #define INTR_TRANSFER_ERR_BIT (1 << 9)
    #define INTR_DYN_ADDR_ASSGN_BIT (1 << 8) // only for slave
//    #define INTR_CMD_RTRY_FAIL_BIT (1 << 7)
    #define INTR_CCC_TABLE_UPDT_BIT (1 << 6) // only for slave
    #define INTR_TRANSFER_ABORT_BIT (1 << 5) // only for main master
    #define INTR_RESP_READY_BIT (1 << 4)
    #define INTR_CMD_QUEUE_READY_BIT (1 << 3)
    #define INTR_IBI_THLD_BIT (1 << 2) // only for main master
    #define INTR_RX_THLD_BIT (1 << 1)
    #define INTR_TX_THLD_BIT (1 << 0)

#define I3C_SLV_INTR_MASK (INTR_IBI_UPDATED_BIT |   \
    /*INTR_READ_REQ_RECV_BIT |               */     \
    INTR_TRANSFER_ERR_BIT |                         \
    INTR_DYN_ADDR_ASSGN_BIT |                       \
    INTR_CCC_TABLE_UPDT_BIT |                       \
    INTR_RESP_READY_BIT |                           \
    /*INTR_CMD_QUEUE_READY_BIT |                 */ \
    INTR_RX_THLD_BIT                                \
                           /*| INTR_TX_THLD_BIT */)
#define I3C_MST_INTR_MASK (INTR_BUS_OWNER_UPDATED_BIT |\
    INTR_IBI_UPDATED_BIT |                          \
    INTR_DEFSLV_BIT |                               \
    INTR_TRANSFER_ERR_BIT |                         \
    INTR_TRANSFER_ABORT_BIT |                       \
    INTR_RESP_READY_BIT |                           \
                           /*INTR_CMD_QUEUE_READY_BIT |*/   \
    INTR_IBI_THLD_BIT |                             \
    INTR_RX_THLD_BIT                                \
                           /*INTR_TX_THLD_BIT*/     \
    )

#define I3C_MST_INTR_MASK_WITH_DMA (INTR_BUS_OWNER_UPDATED_BIT |\
    INTR_IBI_UPDATED_BIT |                          \
    INTR_DEFSLV_BIT |                               \
    INTR_TRANSFER_ERR_BIT |                         \
    INTR_TRANSFER_ABORT_BIT |                       \
    INTR_RESP_READY_BIT |                           \
                           /*INTR_CMD_QUEUE_READY_BIT |*/   \
    INTR_IBI_THLD_BIT                              \
    )

#define INTR_DEV_CTRL_ENABLE_ALL			          0x07ff

#define I3C_REG_INTR_SIGNAL_EN                        (0x44)
#define I3C_REG_INTR_FORCE                            (0x48)
#define I3C_REG_QUEUE_STATUS_LEVEL                    (0x4c)
#define I3C_REG_DATA_BUFFER_STATUS_LEVEL              (0x50)
#define I3C_REG_PRESENT_STATE                         (0x54)
#define I3C_REG_SLV_CCC_DEVICE_STATUS                 (0x58)
#define I3C_REG_DEVICE_ADDR_TABLE_POINTER             (0x5c)
#define I3C_REG_DEV_CHAR_TABLE_POINTER                (0x60)
#define I3C_REG_VENDOR_SPECIFIC_REG_POINTER           (0x6c)
#define I3C_REG_SLV_PID_VALUE                         (0x74)
#define I3C_REG_SLV_CHAR_CTRL                         (0x78)
    #define  I3C_SLAVE_CHAR_MAX_DATA_SPEED_LIMIT_MASK   ( 1<<0 )  
#define I3C_REG_SLV_MAX_LEN                           (0x7c)
#define I3C_REG_MAX_READ_TURNAROUND                   (0x80)
#define I3C_REG_MAX_DATA_SPEED                        (0x84)
#define I3C_REG_SLV_DEBUG_STATUS                      (0x88)
#define I3C_REG_SLV_INTR_REQ                          (0x8C)
#define I3C_REG_SLV_TSX_SYMBL_TIMING                  (0x90)
#define I3C_REG_DEVICE_CTRL_EXTENDED                  (0xb0)
#define I3C_REG_SCL_I3C_OD_TIMING                       (0xb4)
    #define I3C_OD_HCNT_MASK 0xFF
    #define I3C_OD_HCNT_LSB  16
    #define I3C_OD_LCNT_MASK 0xFF
    #define I3C_OD_LCNT_LSB 0
#define I3C_REG_SCL_I3C_PP_TIMING                       (0xb8)
    #define I3C_PP_HCNT_MASK 0xFF
    #define I3C_PP_HCNT_LSB 16
    #define I3C_PP_LCNT_MASK 0xFF
    #define I3C_PP_LCNT_LSB 0
#define I3C_REG_SCL_I2C_FM_TIMING                       (0xbc)
    #define I2C_FM_HCNT_MASK 0xFFFF
    #define I2C_FM_HCNT_LSB  16
    #define I2C_FM_LCNT_MASK 0xFFFF
    #define I2C_FM_LCNT_LSB 0
#define I3C_REG_SCL_I2C_FMP_TIMING                       (0xc0)
    #define I2C_FMP_HCNT_MASK 0xFF
    #define I2C_FMP_HCNT_LSB  16
    #define I2C_FMP_LCNT_MASK 0xFFFF
    #define I2C_FMP_LCNT_LSB 0
#define I3C_REG_SCL_EXT_LCNT_TIMING                   (0xc8)
    #define I3C_EXT_LCNT_4_MASK 0xFF
    #define I3C_EXT_LCNT_4_LSB 24
    #define I3C_EXT_LCNT_3_MASK 0xFF
    #define I3C_EXT_LCNT_3_LSB 16
    #define I3C_EXT_LCNT_2_MASK 0xFF
    #define I3C_EXT_LCNT_2_LSB 8
    #define I3C_EXT_LCNT_1_MASK 0xFF
    #define I3C_EXT_LCNT_1_LSB 0

#define I3C_REG_SCL_EXT_TERMN_LCNT_TIMING             (0xcc)
#define I3C_REG_BUS_FREE_AVAIL_TIMING                       (0xd4)
#define I3C_REG_BUS_IDLE_TIMING                       (0xd8)
#define I3C_REG_I3C_VER_ID                            (0xe0)
#define I3C_REG_I3C_VER_TYPE                          (0xe4)
#define I3C_REG_EXTENDED_CAPABILITY                   (0xe8)
#define I3C_REG_SLAVE_CONFIG                          (0xec)
#define I3C_REG_DEV_CHAR_TABLE1_LOC1                  (0x200)
#define I3C_REG_DEV_CHAR_TABLE1_LOC2                  (0x204)
#define I3C_REG_DEV_CHAR_TABLE1_LOC3                  (0x208)
#define I3C_REG_DEV_CHAR_TABLE1_LOC4                  (0x20c)
#define I3C_REG_DEV_CHAR_TABLE2_LOC1                  (0x210)
#define I3C_REG_DEV_CHAR_TABLE2_LOC2                  (0x214)
#define I3C_REG_DEV_CHAR_TABLE2_LOC3                  (0x218)
#define I3C_REG_DEV_CHAR_TABLE2_LOC4                  (0x21c)
#define I3C_REG_DEV_CHAR_TABLE3_LOC1                  (0x220)
#define I3C_REG_DEV_CHAR_TABLE3_LOC2                  (0x224)
#define I3C_REG_DEV_CHAR_TABLE3_LOC3                  (0x228)
#define I3C_REG_DEV_CHAR_TABLE3_LOC4                  (0x22c)
#define I3C_REG_DEV_CHAR_TABLE4_LOC1                  (0x230)
#define I3C_REG_DEV_CHAR_TABLE4_LOC2                  (0x234)
#define I3C_REG_DEV_CHAR_TABLE4_LOC3                  (0x238)
#define I3C_REG_DEV_CHAR_TABLE4_LOC4                  (0x23c)
#define I3C_REG_DEV_CHAR_TABLE5_LOC1                  (0x240)
#define I3C_REG_DEV_CHAR_TABLE5_LOC2                  (0x244)
#define I3C_REG_DEV_CHAR_TABLE5_LOC3                  (0x248)
#define I3C_REG_DEV_CHAR_TABLE5_LOC4                  (0x24c)
#define I3C_REG_DEV_CHAR_TABLE6_LOC1                  (0x250)
#define I3C_REG_DEV_CHAR_TABLE6_LOC2                  (0x254)
#define I3C_REG_DEV_CHAR_TABLE6_LOC3                  (0x258)
#define I3C_REG_DEV_CHAR_TABLE6_LOC4                  (0x25c)
#define I3C_REG_DEV_CHAR_TABLE7_LOC1                  (0x260)
#define I3C_REG_DEV_CHAR_TABLE7_LOC2                  (0x264)
#define I3C_REG_DEV_CHAR_TABLE7_LOC3                  (0x268)
#define I3C_REG_DEV_CHAR_TABLE7_LOC4                  (0x26c)
#define I3C_REG_DEV_CHAR_TABLE8_LOC1                  (0x270)
#define I3C_REG_DEV_CHAR_TABLE8_LOC2                  (0x274)
#define I3C_REG_DEV_CHAR_TABLE8_LOC3                  (0x278)
#define I3C_REG_DEV_CHAR_TABLE8_LOC4                  (0x27c)
#define I3C_REG_DEV_ADDR_TABLE_LOC1                   (0x280)
    #define LEGACY_I2C_DEVICE (1 << 31)
    #define DEV_NACK_RETRY_CNT_MASK 3
    #define DEV_NACK_RETRY_CNT_LSB 29
    #define DEV_ADDR_PARITY (1 << 23)
    #define DEV_DYNAMIC_ADDR_MASK 0x7F
    #define DEV_DYNAMIC_ADDR_LSB 16
    #define DEV_STATIC_ADDR_MASK 0x7F
    #define DEV_STATIC_ADDR_LSB 0
#define I3C_REG_DEV_ADDR_TABLE_LOC2                   (0x284)
#define I3C_REG_DEV_ADDR_TABLE_LOC3                   (0x288)
#define I3C_REG_DEV_ADDR_TABLE_LOC4                   (0x28c)
#define I3C_REG_DEV_ADDR_TABLE_LOC5                   (0x290)
#define I3C_REG_DEV_ADDR_TABLE_LOC6                   (0x294)
#define I3C_REG_DEV_ADDR_TABLE_LOC7                   (0x298)
#define I3C_REG_DEV_ADDR_TABLE_LOC8                   (0x29c)


#define DEV_ADDR_DYNAMIC_ADDR_VALID	BIT(31) /* self-assign address */
#define DEV_ADDR_DYNAMIC		(0x08<<16) /* Master Dynamic address*/
#define IC_DEVICE_CTRL		0x000
#define IC_DEVICE_ADDR		0x004
#define IC_HW_CAPABILITY	0x008
#define IC_COMMAND_QUEUE_PORT	0x00c
#define IC_RESPONSE_QUEUE_PORT	0x010
#define IC_RX_TX_DATA_PORT	0x014
#define IC_QUEUE_THLD_CTRL	0x01c
#define IC_DATA_BUF_THLD_CTRL	0x020
#define IC_MR_SEL		0x028
#define IC_MR_REQ_REJECT	0x02c
#define IC_IBI_SIR_REQ_REJECT	0x030
#define IC_RESET_CTRL		0x034
#define IC_INTR_STATUS		0x03c
#define IC_INTR_STATUS_EN	0x040
#define IC_INTR_SIGNAL_EN	0x044
#define IC_DATA_BUF_STATUS_LVL	0x050
#define IC_DEV_ADDR_TAB_POINTER	0x05C
#define IC_DEVICE_CTRL_EXTENDED 0x0b0
#define IC_BUS_FREE_TIMING	0x0c0
#define IC_BUS_IDLE_TIMING	0x0c4
#define IC_MASTER_CONFIG	0x0e8
#define IC_SCL_HCNT_TIMING	0x0b4
#define IC_SCL_LCNT_TIMING	0x0b8
#define IC_I3C_VER_ID		0x0e0
#define IC_I3C_VER_TYPE		0x0e4


#define COMMAND_PORT_CMD_TOC		BIT(30) /* Termination on Completion */
#define COMMAND_PORT_CMD_READ_TRANSFER	BIT(28) /* I2C Read Transfer */
#define COMMAND_PORT_CMD_SDAP		BIT(27) /* Short Data Arg present */
#define COMMAND_PORT_CMD_ROC		BIT(26) /* Response On Completion */
#define COMMAND_PORT_CMD_CP		BIT(15) /* Command Present */
#define COMMAND_PORT_CMD_ADDR_ASSGN_CMD	0x03	/* Addr Assignment command*/
#define COMMAND_PORT_CMD_ATTR_SDA	BIT(1)  /* Short Data Arg command */
#define COMMAND_PORT_CMD_ATTR_TRANSFER	BIT(0)  /* Transfer Argument command */
#define COMMAND_PORT_TRANSF_ARG_LEN	0xffff0000 /* Transfer Arg Data Len */
#define COMMAND_PORT_SDP_BYTE_STRB_3	BIT(5)  /* Data Byte 3 qualifier */
#define COMMAND_PORT_SDP_BYTE_STRB_2	BIT(4)  /* Data Byte 2 qualifier */
#define COMMAND_PORT_SDP_BYTE_STRB_1	BIT(3)	/* Data Byte 1 qualifier */
#define COMMAND_PORT_SDP_DATA_BYTE_3	0xff000000
#define COMMAND_PORT_SDP_DATA_BYTE_2	0x00ff0000
#define COMMAND_PORT_SDP_DATA_BYTE_1	0x0000ff00

//#define MR_SEL_MASTER_REQUEST		BIT(1) /* In-Band Master request */
#define IBI_SIR_REQ_REJECT_ALL		0xffffffff /* Nack all IBI SIR*/
//#define MR_REQ_REJECT_NACK		BIT(1) /* NACK Master/P2P Request */

#define INTR_DYN_ADDR_ASSGN_ENABLE	BIT(8) /* Dynamic Add Assigned Enable */
#define INTR_RX_THLD_ENABLE		BIT(1) /* RX Buffer Threshold Enable */
#define INTR_TX_THLD_ENABLE		BIT(0) /* TX Buffer Threshold Enable */

#define DEV_ADDR_TABLE_LEGACY_I2C_DEV	BIT(31) /* Legacy I2C Device */
#define DEV_ADDR_TABLE_DEV_STATIC_ADDR	0x007f  /* Device Static Address Mask */

#define QUEUE_THLD_CTRL_RESP_BUF	0x0000ff00 /* Response Buff Threshold */
#define DATA_BUF_THLD_CTRL_RX_BUF	0x00000700 /* RX Buffer Threshold */
#define DATA_BUF_STATUS_LVL_RX_BLR	0x0000ff00 /* RX Buffer Level Value */

#define RESET_CTRL_SOFT_RST		0x01

/* Interrupt Status Register (IC_INTR_STATUS) fields */
#define INTR_ENABLE_ALL			0x07ff
#define INTR_STATUS_DEFSLV_P2PTRGT_STAT BIT(10) /* Define Slave,P2P Target */
#define INTR_STATUS_TRANSFER_ERR_STAT	BIT(9)  /* Transfer Error */
#define INTR_STATUS_DYN_ADDR_ASSGN_STAT BIT(8)  /* Dynamic Address Assigned */
#define INTR_STATUS_CMD_RTRY_FAIL_STAT	BIT(7)  /* Transfer Retry Failed */
#define INTR_STATUS_CCC_TABLE_UPDT_STAT BIT(6)  /* CCC Table Updated */
#define INTR_STATUS_TRANSFER_ABORT_STAT BIT(5)  /* Transfer Abort */
#define INTR_STATUS_RESP_READY_STAT	BIT(4)  /* Response Queue Ready */
#define INTR_STATUS_CMD_QUEUE_RDY_STAT	BIT(3)  /* Command Queue Ready */
#define INTR_STATUS_IBI_THLD_STAT	BIT(2)  /* IBI Buffer Threshold */
#define INTR_STATUS_RX_THLD_STAT	BIT(1)  /* RX Buffer Threshold */
#define INTR_STATUS_TX_THLD_STAT	BIT(0)  /* TX Buffer Threshold */


/* Defines to do initial reset on bus */
#define RESET_BROADCAST_COMMAND	0x40008300

//125Mhz-> 8ns per cycle * 156(0x9c) = 1248ns *2 -> 2,496 ns -> 400.6KHz
//200Mhz-> 5ns per cycle * 250(0xFA) = 1250ns *2 -> 2500 ns -> 400.6KHz
#define I3C_PP_HCN_INIT_RESET	0xFA
#define I3C_PP_LCN_INIT_RESET	0xFA

#define I3C_OD_HCN_INIT_RESET	0xFA
#define I3C_OD_LCN_INIT_RESET	0xFA

#define I2C_FM_HCN_INIT_RESET	0xFA
#define I2C_FM_LCN_INIT_RESET	0xFA

#define I2C_FMP_HCN_INIT_RESET	0xFA
#define I2C_FMP_LCN_INIT_RESET	0xFA



#define I3C_TIMEOUT_US (1000)
#define true (1)
#define false (0)


#define I3C_DEV_OPERATION_MODE_MASTER   0
#define I3C_DEV_OPERATION_MODE_SLAVE    1
/*
 * The I3C specification says the maximum number of devices connected on the
 * bus is 11, but this number depends on external parameters like trace length,
 * capacitive load per Device, and the types of Devices present on the Bus.
 * I3C master can also have limitations, so this number is just here as a
 * reference and should be adjusted on a per-controller/per-board basis.
 */

#define I3C_BUS_MAX_DEVS		6             //for Tahiti I3C IP configuration max on-line device : 6 
#define I3C_MAX_DAT_ENTRIES    (6)            //for Tahiti I3C IP configuration Address table number : 6 
#define I3C_BUS_MAX_I3C_SCL_RATE	12900000  
#define I3C_BUS_TYP_I3C_SCL_RATE	12500000
#define I3C_BUS_I2C_FM_PLUS_SCL_RATE	1000000
#define I3C_BUS_I2C_FM_SCL_RATE		400000
#define I3C_BUS_TLOW_OD_MIN_NS		200
#define I3C_MAX_DCT_ENTRIRES  (24)

#define MAX_I3C_CMD_SLOT (8)               //for Tahiti I3C IP command queue and resposne queue depth is 8 

/* I3C Broadcast Commom Command Code*/
#define ENEC		0x00 /* Enable Events Command */
#define DISEC		0x01 /* Disable Events Command */
#define ENTAS0		0x02 /* Enter Activity State 0 */
#define ENTAS1		0x03 /* Enter Activity State 1 */
#define ENTAS2		0x04 /* Enter Activity State 2 */
#define ENTAS3		0x05 /* Enter Activity State 3 */
#define RSTDAA		0x06 /* Reset Dynamic Address Assignment */
#define ENTDAA		0x07 /* Enter Dynamic Address Assignment */
#define DEFSLVS		0x08 /* Define List of Slaves */
#define SETMWL		0x09 /* Set Max Write Length */
#define SETMRL		0x0A /* Set Max Read Length */
#define ENTTM		0x0B /* Enter Test Mode */
#define ENTHDR0		0x20 /* Enter HDR Mode 0 */
#define ENTHDR1		0x21 /* Enter HDR Mode 1 */
#define ENTHDR2		0x22 /* Enter HDR Mode 2 */
#define ENTHDR3		0x23 /* Enter HDR Mode 3 */
#define ENTHDR4		0x24 /* Enter HDR Mode 4 */
#define ENTHDR5		0x25 /* Enter HDR Mode 5 */
#define ENTHDR6		0x26 /* Enter HDR Mode 6 */
#define ENTHDR7		0x27 /* Enter HDR Mode 7 */
#define SETXTIME	0x28 /* Exchange Timing Information */

#define ENEC_D		0x80
#define DISEC_D		0x81
#define ENTAS0_D	0x82
#define ENTAS1_D	0x83
#define ENTAS2_D	0x84
#define ENTAS3_D	0x85
#define RSTDAA_D	0x86
#define SETDASA		0X87 /* Set Dynamic Address from Static Address*/
#define SETNEWDA	0X88 /* Set New Dynamic Address*/
#define SETMWL_D	0x89
#define SETMRL_D	0x8A
#define GETMWL_D	0x8B
#define GETMRL_D 	0x8C
#define GETPID_D 	0x8D
#define GETBCR_D 	0x8E
#define GETDCR_D 	0x8F
#define GETSTATUS_D 	0x90
#define GETMXDS_D 	0x94
#define GETHDRCAP_D 	0x95
#define GETXTIME_D 	0x99

/* I3C transaction types */
#define I3C_DYN_ADDR_ASSGN	0x01
#define I3C_CCC_SET		0X02
#define I3C_CCC_GET		0X03
#define I3C_PRIVATE_WRITE	0X04
#define I3C_PRIVATE_READ	0x05

#define  MAX_I3C_INSTANCE (1)
#define  MAX_I2C_INSTANCE MAX_I3C_INSTANCE
#define I3C_ASSERT_ID(id) do {                             \
        if (id >= MAX_I3C_INSTANCE ) {                 \
        Print_DBG("Invalid I3C/I2C id:%d\n", id); \
        return -1; \
    } \
}while(0)


#define I3CM_IO_CTRL_SET_SDA_TX_HOLD_MAX         (7)

//------------------------------  REGISTER DEFINES  ------------------------------


typedef union {
    uint32_t u32;
    struct
    {
        uint32_t to_i3c_inst_id: 4;
        uint32_t to_i3c_static_addr:  7 ; 
        uint32_t to_i3c_static_addr_en : 1;  
        uint32_t to_i3c_pending_int: 4;
        uint32_t to_i3c_act_mode: 2; 
        uint32_t to_i3c_slv_test_mode: 1; 
        uint32_t rsvd  :14 ; 
    }b;
} Tahiti_I3C_CTL;  //specfic for Tahiti I3C 


typedef union {
    uint32_t u32;
    struct
    {
        uint32_t iba_include: 1;
        uint32_t rsvd1_6:  6 ; 
        uint32_t i2c_slave_present : 1;  
        uint32_t hot_join_ctrl: 1;
        uint32_t rsvd9_23: 15; 
        uint32_t idle_cnt_multplier: 2 ;
        uint32_t rsvd26: 1  ; 
        uint32_t adaptive_i2c_i3c: 1; 
        uint32_t dma_enable : 1; 
        uint32_t abort : 1;
        uint32_t resume : 1;
        uint32_t enable: 1;    
    }b;
} Tahiti_I3C_DEVICE_CTRL;


typedef union {
    uint32_t u32;
    struct
    {
        uint32_t static_addr   :7;
        uint32_t rsvd_14_7     :8; 
        uint32_t static_addr_valid: 1;
        uint32_t dynamic_addr   :7; 
        uint32_t rsvd_30_23     :8;  
        uint32_t dynamic_addr_valid: 1;
    }b;
} Tahiti_I3C_DEVICE_ADDR;

typedef union {
    uint32_t u32;
    struct
    {

        uint32_t device_role_config     :3;  
        uint32_t hdr_ddr_en: 1;
        uint32_t hdr_ts_en     :1;  
        uint32_t clock_period  :6; 
        uint32_t hdr_tx_clock_period :6; 
        uint32_t dma_en  :1 ;
        uint32_t slv_hj_cap: 1; 
        uint32_t slv_ibi_cap :1;  
        uint32_t rsvd   :12;
   
    }b;
} Tahiti_I3C_HW_CAPABILITY;

typedef union {
    uint32_t u32;
    struct
    {
        uint32_t command : 32;
    } b;
} Tahiti_I3C_COMMAND_QUEUE_PORT;


typedef union {
    uint32_t u32;
    struct
    {
        uint32_t response : 32;
    } b;
} Tahiti_I3C_RESPONSE_QUEUE_PORT;


typedef union {
    uint32_t u32;
    struct
    {
        uint32_t trx_data_port : 32;
    } b;
} Tahiti_I3C_TRX_DATA_PORT;




typedef union {
    uint32_t u32;
    struct
    {
        uint32_t ibi_data : 32;
    } b;
} Tahiti_I3C_IBI_QUEUE_DATA;

typedef union {
    uint32_t u32;
    struct
    {
        uint32_t data_length : 8;
        uint32_t ibi_id : 8;
        uint32_t rsvd_27_16 : 12;
        uint32_t ibi_sts : 4;

    } b;
} Tahiti_I3C_IBI_QUEUE_STATUS;

typedef union {
    Tahiti_I3C_IBI_QUEUE_DATA     I3C_IBI_QUEUE_DATA;
    Tahiti_I3C_IBI_QUEUE_STATUS   I3C_IBI_QUEUE_STATUS ;
} Tahiti_I3C_IBI_QUEUE_DATA_OR_STATUS;


typedef union {
    uint32_t u32;
    struct
    {
        uint32_t cmd_empty_buf_thld : 8;
        uint32_t resp_buf_thld : 8;
        uint32_t rsvd_23_16 : 8;
        uint32_t ibi_status_thld : 8;
    } b;
} Tahiti_I3C_QUEUE_THLD_CTRL;



typedef union {
    uint32_t u32;
    struct
    {
        uint32_t tx_empty_buf_thld : 3;
        uint32_t rsvd_3 : 1;
        uint32_t rsvd_7_4 : 4;
        uint32_t rx_buf_thld : 3;

        uint32_t rsvd_11 : 1;
        uint32_t rsvd_15_12 : 4;
        uint32_t tx_start_thld : 3;
        uint32_t rsvd_19 : 1;

        uint32_t rsvd_23_20 : 4; 
        uint32_t rx_start_thld : 3;
        uint32_t rsvd_27 : 1;
        uint32_t rsvd_31_28 :4;

    } b;
} Tahiti_I3C_DATA_BUFFER_THLD_CTRL;


typedef union {
    uint32_t u32;
    struct
    {

        uint32_t notify_hj_rejected : 1; 
        uint32_t notify_mr_rejected : 1; 
        uint32_t rsvd_2 : 1;
        uint32_t notify_sir_rejected : 1;
        uint32_t rsvd :28;

    } b;
} Tahiti_I3C_IBI_QUEUE_CTRL;




typedef union {
    uint32_t u32;
    struct
    {
        uint32_t mr_req_reject :32 ;
    } b;
} Tahiti_I3C_IBI_MR_REQ_REJECT;




typedef union {
    uint32_t u32;
    struct
    {
        uint32_t sir_req_reject :32 ;
    } b;
} Tahiti_I3C_IBI_SIR_REQ_REJECT;


typedef union {
    uint32_t u32;
    struct
    {
        uint32_t soft_rst :1 ;
        uint32_t cmd_queue_rst :1 ;
        uint32_t resp_queue_rst :1 ;
        uint32_t tx_fifo_rst :1 ;
        uint32_t rx_fifo_rst :1 ;
        uint32_t ibi_queue_rst :1 ;
        uint32_t rsvd :26 ;
    } b;
} Tahiti_I3C_RESET_CTRL;


typedef union {
    uint32_t u32;
    struct
    {
        uint32_t slave_intr_req :1 ;
        uint32_t master_intr_req :1 ;
        uint32_t rsvd_2 :1; 
        uint32_t hj_intr_req :1 ;
        uint32_t activity_state :2 ;
        uint32_t mrl_updated :1 ;
        uint32_t mwl_updated :1 ;
        uint32_t rsvd :24 ;
    } b;
} Tahiti_I3C_SLV_EVENT_CTRL;



typedef union {
    uint32_t u32;
    struct
    {
        uint32_t tx_thld_stat :1 ;
        uint32_t rx_thld_stat :1 ;
        uint32_t ibi_thld_stat :1; 
        uint32_t cmd_queue_ready_stat :1 ;
        uint32_t resp_ready_stat_intr :1 ;
        uint32_t transfer_abort_stat :1 ;
        uint32_t ccc_updated_stat :1 ;
        uint32_t rsvd_7 :1 ;
        uint32_t dyn_addr_assgn_stat :1 ;
        uint32_t transfer_err_stat :1; 
        uint32_t defslv_stat :1 ;
        uint32_t read_req_recv_stat :1 ;
        uint32_t ibi_updated_sta :1 ;
        uint32_t busowner_updated_stat :1 ;
        uint32_t rsvd :18 ;
    } b;
} Tahiti_I3C_INTR_STATUS;



typedef union {
    uint32_t u32;
    struct
    {
        uint32_t tx_thld_stat_en :1 ;
        uint32_t rx_thld_stat_en :1 ;
        uint32_t ibi_thld_stat_en :1; 
        uint32_t cmd_queue_ready_stat_en :1 ;
        uint32_t resp_ready_stat_intr_en :1 ;
        uint32_t transfer_abort_stat_en :1 ;
        uint32_t ccc_updated_stat_en :1 ;
        uint32_t rsvd_7 :1 ;
        uint32_t dyn_addr_assgn_stat_en :1 ;
        uint32_t transfer_err_stat_en :1; 
        uint32_t defslv_stat_en :1 ;
        uint32_t read_req_recv_stat_en :1 ;
        uint32_t ibi_updated_stat_en :1 ;
        uint32_t busowner_updated_stat_en :1 ;
        uint32_t rsvd :18 ;
    } b;
} Tahiti_I3C_INTR_STATUS_EN;



typedef union {
    uint32_t u32;
    struct
    {
        uint32_t tx_thld_signal_en :1 ;
        uint32_t rx_thld_signal_en :1 ;
        uint32_t ibi_thld_signal_en :1; 
        uint32_t cmd_queue_ready_signal_en :1 ;
        uint32_t resp_ready_signal_en :1 ;
        uint32_t transfer_abort_signal_en :1 ;
        uint32_t ccc_updated_signal_en :1 ;
        uint32_t rsvd_7 :1 ;
        uint32_t dyn_addr_assgn_signal_en :1 ;
        uint32_t transfer_err_signal_en :1; 
        uint32_t defslv_signal_en :1 ;
        uint32_t read_req_recv_signal_en :1 ;
        uint32_t ibi_updated_signal_en :1 ;
        uint32_t busowner_updated_signal_en :1 ;
        uint32_t rsvd :18 ;
    } b;
} Tahiti_I3C_INTR_SIGNAL_EN;



typedef union {
    uint32_t u32;
    struct
    {
        uint32_t tx_thld_force_en :1 ;
        uint32_t rx_thld_force_en :1 ;
        uint32_t ibi_thld_force_en :1; 
        uint32_t cmd_queue_ready_force_en :1 ;
        uint32_t resp_ready_force_en :1 ;
        uint32_t transfer_abort_force_en :1 ;
        uint32_t ccc_updated_force_en :1 ;
        uint32_t rsvd_7 :1 ;
        uint32_t dyn_addr_assgn_force_en :1 ;
        uint32_t transfer_err_force_en :1; 
        uint32_t defslv_force_en :1 ;
        uint32_t read_req_force_en :1 ;
        uint32_t ibi_updated_force_en :1 ;
        uint32_t busowner_updated_force_en :1 ;
        uint32_t rsvd :18 ;
    } b;
} Tahiti_I3C_INTR_FORCE;


typedef union {
    uint32_t u32;
    struct
    {
        uint32_t cmd_queue_empty_loc : 8 ;
        uint32_t resp_buf_blr :8 ;
        uint32_t ibi_buf_blr :8;
        uint32_t ibi_status_cnt :5 ;
        uint32_t rsvd:3;
    } b;
} Tahiti_I3C_QUEUE_STATUS_LEVEL;




typedef union {
    uint32_t u32;
    struct
    {
        uint32_t tx_buf_empty_loc : 8 ;
        uint32_t rsvd_15_8 : 8;
        uint32_t rx_buf_blr : 8 ;
        uint32_t rsvd_31_24 : 8;
    } b;
} Tahiti_I3C_DATA_BUFFER_STATUS_LEVEL;




typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t scl_line_signal_level : 1 ;
        uint32_t sda_line_signal_level : 1;
        uint32_t current_master : 1 ;
        uint32_t rsvd3_7 : 5;
        uint32_t cm_tfr_status : 6 ;
        uint32_t rsvd14_15 : 2;
        uint32_t cm_tfr_st_status : 6 ;
        uint32_t rsvd22_23 : 2;
        uint32_t cmd_tid : 4 ;
        uint32_t master_idle: 1;  //added for Tahiti I3C IP version : lca02
        uint32_t rsvd28_31 : 3;
    } b;
} Tahiti_I3C_PRESENT_STATE;


typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t pending_intr : 4 ;
        uint32_t rsvd_4 : 1;
        uint32_t protocol_err : 1 ;
        uint32_t activity_mode : 2;
        uint32_t underflow_err : 1 ;
        uint32_t slave_busy : 1;
        uint32_t overflow_err : 1 ;
        uint32_t data_not_ready : 1;
        uint32_t buffer_not_avail : 1 ;
        uint32_t rsvd : 19;
    } b;
} Tahiti_I3C_CCC_DEVICE_STATUS;


typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t p_dev_addr_table_start_addr : 16;
        uint32_t dev_addr_table_depth : 16 ;

    } b;
} Tahiti_I3C_DEVICE_ADDR_TABLE_POINTER;




typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t p_dev_char_table_start_addr : 12;
        uint32_t dev_char_table_depth : 7 ;
        uint32_t present_dev_char_table_indx : 5;
        uint32_t rsvd_31_24 : 8 ;

    } b;
} Tahiti_I3C_DEV_CHAR_TABLE_POINTER;



typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t p_vendor_reg_start_addr : 16;
        uint32_t rsvd : 16 ;

    } b;
} Tahiti_I3C_VENDOR_SPECIFIC_REG_POINTER;



typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t slv_pid_dcr : 12;
        uint32_t slv_inst_id : 4;
        uint32_t slv_part_id : 16 ;

    } b;
} Tahiti_I3C_SLV_PID_VALUE;



typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t max_data_speed_limit : 1;
        uint32_t ibi_request_capable :1 ;
        uint32_t ibi_payload  : 1; 
        uint32_t offline_capable :1;
        uint32_t bridge_identifier :1; 
        uint32_t hdr_capable : 1; 
        uint32_t device_role : 2; 
        uint32_t dcr : 8;
        uint32_t hdr_cap : 8;
        uint32_t rsvd : 8 ;

    } b;
} Tahiti_I3C_SLV_CHAR_CTRL; //update for Tahiti I3C IP ver: lca02 




typedef union {
    uint32_t u32;
    struct
    { 

        uint32_t mwl : 16;
        uint32_t mrl : 16 ;

    } b;
} Tahiti_I3C_SLV_MAX_LEN;



typedef union {
    uint32_t u32;
    struct
    { 

        uint32_t mxds_max_rd_turn : 24;
        uint32_t rsvd : 8 ;

    } b;
} Tahiti_I3C_MAX_READ_TURNAROUND;


typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t mxds_max_wr_speed : 3;
        uint32_t rsvd_7_3 : 5;
        uint32_t mxds_max_rd_speed : 3;
        uint32_t rsvd_15_11 : 5;
        uint32_t mxds_clk_data_turn : 3;
        uint32_t rsvd : 13;

    } b;
} Tahiti_I3C_MAX_DATA_SPEED;


typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t sir : 1;
        uint32_t sir_ctrl : 2;
        uint32_t mir : 1;
        uint32_t rsvd_7_4 : 4;
        uint32_t ibi_sts : 2;
        uint32_t rsvd_23_10 : 14;
        uint32_t rsvd_31_24 : 8;

    } b;
} Tahiti_I3C_SLV_INTR_REQ;


typedef union {
    uint32_t u32;
    struct
    { 

        uint32_t SLV_TSX_SYMBL_CNT : 6;
        uint32_t RSVD_31_6 : 26 ;

    } b;
} Tahiti_I3C_SLV_TSX_SYMBL_TIMING;




typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t dev_operation_mode : 2;
        uint32_t rsvd_2 : 1;
        uint32_t reqmst_ack_ctrl : 1;
        uint32_t rsvd : 28;

    } b;
} Tahiti_I3C_DEVICE_CTRL_EXTENDED;

typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t i3c_od_lcnt : 8;
        uint32_t rsvd_15_8 : 8;
        uint32_t i3c_od_hcnt : 8;
        uint32_t rsvd_31_24 : 8;

    } b;
} Tahiti_I3C_SCL_I3C_OD_TIMING;

typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t i3c_pp_lcnt : 8;
        uint32_t rsvd_15_8 : 8;
        uint32_t i3c_pp_hcnt : 8;
        uint32_t rsvd_31_24 : 8;

    } b;
} Tahiti_I3C_SCL_I3C_PP_TIMING;

typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t i2c_fm_lcnt : 16;
        uint32_t i2c_fm_hcnt : 16;

    } b;
} Tahiti_I3C_SCL_I2C_FM_TIMING;

typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t i2c_fmp_lcnt : 16;
        uint32_t i2c_fmp_hcnt : 8;
        uint32_t rsvd_31_24 : 8 ; 
    } b;
} Tahiti_I3C_SCL_I2C_FMP_TIMING;

typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t i3c_ext_lcnt_1 : 8;
        uint32_t i3c_ext_lcnt_2 : 8;
        uint32_t i3c_ext_lcnt_3 : 8;
        uint32_t i3c_ext_lcnt_4 : 8 ; 
    } b;
} Tahiti_I3C_SCL_EXT_LCNT_TIMING;


typedef union {
    uint32_t u32;
    struct
    { 
        uint32_t i3c_ext_termn_lcnt : 4;
        uint32_t rsvd_15_4 : 12;
        uint32_t i3c_ts_skew_cnt : 4;
        uint32_t rsvd_31_20 : 12 ; 
    } b;
} Tahiti_I3C_SCL_EXT_TERMN_LCNT_TIMING;

typedef union {
    uint32_t u32;
    struct
    { 

        uint32_t rsvd_15_0 : 16;
        uint32_t sda_tx_hold : 3;
        uint32_t rsvd_31_19 : 13 ; 
    } b;
} Tahiti_I3C_SDA_HOLD_DLY_TIMING; //add for Tahiti I3C IP ver: lca102



typedef union {
    uint32_t u32;
    struct
    { 

        uint32_t i3c_mst_free : 16;
        uint32_t i3c_ibi_free : 16 ; 
    } b;
} Tahiti_I3C_BUS_FREE_TIMING;



typedef union {
    uint32_t u32;
    struct
    {
        uint32_t i3c_free_time : 16 ;   //FOR MASTER
        uint32_t i3c_bus_available_time : 16; //FOR SLAVE
    } b;
} Tahiti_I3C_BUS_FREE_AVAIL_TIMING; //specific for tahiti I3C IP verson lca02

typedef union {
    uint32_t u32;
    struct
    { 

        uint32_t bus_idle_time : 20;
        uint32_t rsvd_31_20 : 12 ; 
    } b;
} Tahiti_I3C_BUS_IDLE_TIMING;


typedef union {
    uint32_t u32;
    struct
    {
        uint32_t i3c_ver_id : 32 ; 
    } b;
} Tahiti_I3C_I3C_VER_ID;

typedef union {
    uint32_t u32;
    struct
    {
        uint32_t i3c_ver_type : 32 ; 
    } b;
} Tahiti_I3C_I3C_VER_TYPE;

typedef union {
    uint32_t u32;
    struct
    {
        uint32_t tx_buf_size:4; 
        uint32_t rx_buf_size:4;
        uint32_t cmd_buf_size:4; 
        uint32_t resp_buf_size:4; 
        uint32_t ibi_buf_size :4;
        uint32_t rsvd : 12; 
    } b;
} Tahiti_I3C_QUEUE_SZIE_CAPABILITY;  //specific for tahiti I3C IP verson lca02

typedef union {
    uint32_t u32;
    struct
    {
        uint32_t app_if_mode : 2 ; 
        uint32_t rapp_if_data_widthsvd : 2 ; 
        uint32_t operation_mode : 2 ; 
        uint32_t clk_period : 6 ; 
        uint32_t rsvd : 20 ; 
    } b;
} Tahiti_I3C_EXTENDED_CAPABILITY;




typedef union {
    uint32_t u32;
    struct
    {

        uint32_t dma_en : 1 ; 
        uint32_t hj_cap : 1 ; 
        uint32_t clk_period : 14; 
        uint32_t rsvd : 16 ; 
    } b;
} Tahiti_I3C_SLAVE_CONFIG;


typedef union {
    uint32_t u32;
    struct
    {
        uint32_t rsvd : 32 ; 
    } b;
} Tahiti_I3C_RSVD;

typedef struct
{
    Tahiti_I3C_DEVICE_CTRL DEVICE_CTRL;                                 //1 Offset: 0x0
    Tahiti_I3C_DEVICE_ADDR DEVICE_ADDR;                                 //2 Offset: 0x4
    Tahiti_I3C_HW_CAPABILITY HW_CAPABILITY;                             //3 Offset: 0x8
    Tahiti_I3C_COMMAND_QUEUE_PORT COMMAND_QUEUE_PORT;                   //4 Offset: 0xc
    Tahiti_I3C_RESPONSE_QUEUE_PORT RESPONSE_QUEUE_PORT;                 //5 Offset: 0x10
    Tahiti_I3C_TRX_DATA_PORT TRX_DATA_PORT;                             //6/7 Offset: 0x14
    Tahiti_I3C_IBI_QUEUE_DATA_OR_STATUS IBI_QUEUE_DATA_OR_STATUS;       //8/9 Offset: 0x18
    Tahiti_I3C_QUEUE_THLD_CTRL QUEUE_THLD_CTRL;                         //10 Offset: 0x1c
    Tahiti_I3C_DATA_BUFFER_THLD_CTRL DATA_BUFFER_THLD_CTRL;             //11 Offset: 0x20
    Tahiti_I3C_IBI_QUEUE_CTRL IBI_QUEUE_CTRL;                           //12 Offset: 0x24
    Tahiti_I3C_RSVD I3C_RSVD_0X28;                                      //   Offset: 0x28
    Tahiti_I3C_IBI_MR_REQ_REJECT IBI_MR_REQ_REJECT;                     //13 Offset: 0x2c
    Tahiti_I3C_IBI_SIR_REQ_REJECT IBI_SIR_REQ_REJECT;                   //14 Offset: 0x30
    Tahiti_I3C_RESET_CTRL RESET_CTRL;                                   //15 Offset: 0x34
    Tahiti_I3C_SLV_EVENT_CTRL SLV_EVENT_CTRL;                           //16 Offset: 0x38
    Tahiti_I3C_INTR_STATUS INTR_STATUS;                                 //17 Offset: 0x3c
    Tahiti_I3C_INTR_STATUS_EN INTR_STATUS_EN;                           //18 Offset: 0x40
    Tahiti_I3C_INTR_SIGNAL_EN INTR_SIGNAL_EN;                           //19 Offset: 0x44
    Tahiti_I3C_INTR_FORCE INTR_FORCE;                                   //20 Offset: 0x48
    Tahiti_I3C_QUEUE_STATUS_LEVEL QUEUE_STATUS_LEVEL;                   //21 Offset: 0x4c
    Tahiti_I3C_DATA_BUFFER_STATUS_LEVEL DATA_BUFFER_STATUS_LEVEL;       //22 Offset: 0x50
    Tahiti_I3C_PRESENT_STATE PRESENT_STATE;                             //23 Offset: 0x54
    Tahiti_I3C_CCC_DEVICE_STATUS CCC_DEVICE_STATUS;                     //24 Offset: 0x58
    Tahiti_I3C_DEVICE_ADDR_TABLE_POINTER DEVICE_ADDR_TABLE_POINTER;     //25  Offset: 0x5c
    Tahiti_I3C_DEV_CHAR_TABLE_POINTER DEV_CHAR_TABLE_POINTER;           //26  Offset: 0x60
    Tahiti_I3C_RSVD I3C_RSVD_0X64;                                      //    Offset: 0x64
    Tahiti_I3C_RSVD I3C_RSVD_0X68;                                      //    Offset: 0x68
    Tahiti_I3C_VENDOR_SPECIFIC_REG_POINTER VENDOR_SPECIFIC_REG_POINTER; //27  Offset: 0x6c
    Tahiti_I3C_RSVD I3C_RSVD_0X70;                                      //    Offset: 0x70
    Tahiti_I3C_SLV_PID_VALUE SLV_PID_VALUE;                             //28  Offset: 0x74
    Tahiti_I3C_SLV_CHAR_CTRL SLV_CHAR_CTRL;                             //29  Offset: 0x78
    Tahiti_I3C_SLV_MAX_LEN SLV_MAX_LEN;                                 //30  Offset: 0x7c
    Tahiti_I3C_MAX_READ_TURNAROUND MAX_READ_TURNAROUND;                 //31  Offset: 0x80
    Tahiti_I3C_MAX_DATA_SPEED MAX_DATA_SPEED;                           //32  Offset: 0x84
    Tahiti_I3C_RSVD I3C_RSVD_0X88;                                      //    Offset: 0x88
    Tahiti_I3C_SLV_INTR_REQ SLV_INTR_REQ;                               //33  Offset: 0x8c
    Tahiti_I3C_SLV_TSX_SYMBL_TIMING SLV_TSX_SYMBL_TIMING;               //34  Offset: 0x90
    Tahiti_I3C_RSVD I3C_RSVD_0X94_ARY[7];                               //    Offset: 0x94 ~0xAF
    Tahiti_I3C_DEVICE_CTRL_EXTENDED DEVICE_CTRL_EXTENDED;               //35  Offset: 0xb0
    Tahiti_I3C_SCL_I3C_OD_TIMING SCL_I3C_OD_TIMING;                     //36  Offset: 0xb4
    Tahiti_I3C_SCL_I3C_PP_TIMING SCL_I3C_PP_TIMING;                     //37  Offset: 0xb8
    Tahiti_I3C_SCL_I2C_FM_TIMING SCL_I2C_FM_TIMING;                     //38  Offset: 0xbc
    Tahiti_I3C_SCL_I2C_FMP_TIMING SCL_I2C_FMP_TIMING;                   //39  Offset: 0xc0
    Tahiti_I3C_RSVD I3C_RSVD_0XC4;                                      //    Offset: 0xC4
    Tahiti_I3C_SCL_EXT_LCNT_TIMING SCL_EXT_LCNT_TIMING;                 //40  Offset: 0xc8
    Tahiti_I3C_SCL_EXT_TERMN_LCNT_TIMING SCL_EXT_TERMN_LCNT_TIMING;     //41  Offset: 0xcc
    Tahiti_I3C_SDA_HOLD_DLY_TIMING SDA_HOLD_DLY_TIMING;                 //42  Offset: 0xd0, specific for tahiti
    Tahiti_I3C_BUS_FREE_AVAIL_TIMING BUS_FREE_AVAIL_TIMING;             //43  Offset: 0xd4, specific for tahiti
    Tahiti_I3C_BUS_IDLE_TIMING BUS_IDLE_TIMING;                         //44  Offset: 0xd8
    Tahiti_I3C_RSVD I3C_RSVD_0XDC;                                      //    Offset: 0xdc
    Tahiti_I3C_I3C_VER_ID I3C_VER_ID;                                   //45  Offset: 0xe0
    Tahiti_I3C_I3C_VER_TYPE I3C_VER_TYPE;                               //46  Offset: 0xe4
    Tahiti_I3C_QUEUE_SZIE_CAPABILITY QUEUE_SZIE_CAPABILITY;             //47  Offset: 0xe8 ,,specific for tahiti
    //Tahiti_I3C_SLAVE_CONFIG   SLAVE_CONFIG ;  //47     Offset: 0xec  , remove for  tahiti
} Tahiti_I3C_s;

#endif // if (defined CONFIG_MCU_DRV_I3CS) || (defined CONFIG_MCU_DRV_I3CM)


#ifdef __cplusplus
}
#endif

#endif // _MCU_DRV_I3C_HW_H_
