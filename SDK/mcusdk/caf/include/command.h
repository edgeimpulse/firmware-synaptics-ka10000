/*------------------------------------------------------------------------------
  INTERNAL USE ONLY

  Unpublished Work Copyright (C) 2010-2020 Synaptics Incorporated.

  All rights reserved.

  This file contains information that is proprietary to Synaptics Incorporated
  ("Synaptics"). The holder of this file shall treat all information contained
  herein as confidential, shall use the information only for its intended
  purpose, and shall not duplicate, disclose, or disseminate any of this
  information in any manner unless Synaptics has otherwise provided express,
  written permission.
  
  Use of the materials may require a license of intellectual property from a
  third party or from Synaptics. This file conveys no express or implied
  licenses to any intellectual property rights belonging to Synaptics.
--------------------------------------------------------------------------------

  File Name: command.h

  Description: The Command structure and related definitions

  Created Jun 27, 2012

-------------------------------------------------------------------------------



------------------------------------------------------------------------------*/

#ifndef COMMAND_H_
#define COMMAND_H_

typedef struct CommandHeaderTag {
  int32_t   num_32b_words:16;
  uint32_t  command_id   :15;
  uint32_t  reply        : 1;
  uint32_t  mgr_module_id   ;
} CommandHeader;

#define COMMAND_OF_SIZE(n)   \
struct {                     \
  CommandHeader head;        \
  uint32_t      data[n];     \
}

// The maximum number of 32-bit data elements that a command can contain
#define MAX_COMMAND_SIZE 13

#define CMD_SET(item)                  ((item) & ~0x0100)
#define CMD_GET(item)                  ((item) |  0x0100)
#define CMD_PARAM(item)                ((item) |  0x0200)
#define CMD_CLR_PARAM(item)            ((item) & ~0x0200)
#define CMD_FWD(item)                  ((item) |  0x0400)
#define CMD_FWD_MASK(item)             ((item) &  0x0400)
#define CMD_CLR_FWD(item)              ((item) & ~0x0400)
#define CMD_FWD_TO_2ND_DEV(item)       ((item) |  0x0800)
#define CMD_FWD_TO_2ND_DEV_MASK(item)  ((item) &  0x0800)
#define CMD_CLR_FWD_TO_2ND_DEV(item)   ((item) & ~0x0800)
#define CMD_MASK                       (~(CMD_SET(0)|CMD_GET(0)|CMD_PARAM(0)|CMD_FWD(0)|CMD_FWD_TO_2ND_DEV(0)))
#define CMD_ITEM(cmd)                  ((cmd) & CMD_MASK)
#define IS_CMD_GET(item)               ((item) & 0x0100) 

#define CMD_REPLY 1
#if !defined _ID
#define _ID(a,b,c,d)  ((uint32_t)((((a)-0x20)<<8)|(((b)-0x20)<<14)|(((c)-0x20)<<20)|(((d)-0x20)<<26)))
#endif

// Retrieve the app and module id from an mgr_module_id
#define GET_MGR_ID(mgr_module_id)    ((mgr_module_id)&~0xFF)
#define GET_APP_ID(mgr_module_id)    GET_MGR_ID(mgr_module_id)
#define GET_MODULE_ID(mgr_module_id) ((mgr_module_id)& 0xFF)

// Reserved App IDs
#define APP_ID_BROADCAST     0xFFFFFF00 // to broadcast commands to all apps

// Reserved module IDs
#define MODULE_ID_APP        0    // to send commands to the app
#define MODULE_ID_BROADCAST  0xFF // to broadcast commands to all modules

// The Command type may be used to point to commands of arbitrary
// sizes, for example:
// COMMAND_OF_SIZE(5) cmd
// Command *ptr = (Command *)&cmd;
typedef COMMAND_OF_SIZE(MAX_COMMAND_SIZE) Command ;
// sendcommand callback function
typedef void(*sendcmd_cb)(void *data);
//------------------------------------------------------------------------------

#define _NARGN(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, N, ...) N
#define _GRANN  13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#if defined(_MSC_VER)
#define LEFT_PAREN (
#define RIGHT_PAREN )
#define NARGS(...) (_NARGN LEFT_PAREN __VA_ARGS__##_GRANN, _GRANN RIGHT_PAREN)
#define FILL_COMMAND(mgr_module_id,cmd_id,...) \
    ((uint32_t)cmd_id)<<16 | NARGS(__VA_ARGS__), (uint32_t)(mgr_module_id), __VA_ARGS__
#elif defined(__GNUC__)

#define NARGS(...) (sizeof((int[]){0,##__VA_ARGS__})/sizeof(int)-1)
#define FILL_COMMAND(mgr_module_id,cmd_id,...) \
    ((uint32_t)cmd_id)<<16 | NARGS(__VA_ARGS__), (uint32_t)(mgr_module_id), ##__VA_ARGS__
#else
#define NARGS(...) \
              (sizeof(#__VA_ARGS__) == sizeof("")?0:_NARG_(__VA_ARGS__, _GRANN))

#define _NARG_(...) _NARGN(__VA_ARGS__)
#define FILL_COMMAND(mgr_module_id,cmd_id,...) \
    ((uint32_t)cmd_id)<<16 | NARGS(__VA_ARGS__), (uint32_t)(mgr_module_id), __VA_ARGS__
#endif

#endif // COMMAND_H_
