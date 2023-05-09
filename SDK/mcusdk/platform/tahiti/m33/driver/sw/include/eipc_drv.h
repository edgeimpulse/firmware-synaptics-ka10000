////////////////////////////////////////////////////////////////////////////////
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
#ifndef _EIPC_DRV_H
#define _EIPC_DRV_H

//-------------------------------- Structures ------------------------------------
enum EIPC_COMMAND_ID
{
    EIPC_FLUSH         = 0,    
    EIPC_SET_THRESHOLD = 1,
    EIPC_MESSAGE_SIZE  = 2,
    EIPC_REASON        = 3,
    EIPC_TEST          = 4,
    EIPC_ENABLE_PRINTF = 5,
};

enum EIPC_EVENT_ID
{
    EIPC_EVENT_USER_MSG    = 0,
    EIPC_EVENT_DEBUG_PRINT = 1,
    EIPC_EVENT_KEYPAD      = 2,
    EIPC_EVENT_LAST_ID,     
};

//-------------------------------- Functions ------------------------------------
/**
 * @brief Flush EIPC buffer
 * @param event_id : Refer to EIPC_EVENT_IDs
 * @return int 0 to success
 */
int eipc_flush_ext(int event_id);

/**
 * @brief Setup the event reigster， any new event will trigger the GPIO
 * 
 * @param event_id : Refer to EIPC_EVENT_ID
 * @param set : 0 for clear, 1 for set
 */
void eipc_set_event(int event_id, uint32_t set);


/**
 * @brief Read the event ID, and after read the event, the GPIO/event-id will be cleared
 * 
 * @return int : event id
 */
int eipc_read_event();


/**
 * @brief Read avaliable message size in the buffer
 * 
 * @param event_id : Refer to EIPC_EVENT_IDs
 * @return int Message size in buffer
 */
int eipc_create_buffer(int event_id);

/**
 * @brief Write message to EIPC buffer, and wait for host to extract
 * 
 * @param event_id : Refer to EIPC_EVENT_IDs
 * @param data  Data pointer to message
 * @param size  Message size
 * @return int  0 for success, negative for failure
 */
int eipc_write_message_ext(int event_id, uint32_t *data, uint32_t size);

/**
 * @brief Read message from EIPC buffer
 * 
 * @param event_id : Refer to EIPC_EVENT_ID
 * @param data  placeholder pointer for the message to read 
 * @param size  size to read
 * @return int  real size read in the buffer
 */
int eipc_read_message_ext(int event_id, uint32_t *data, uint32_t size);

/**
 * @brief Set/Get EIPC buffer threshold(in DWord), by default it 1
 * 
 * @param event_id : Refer to EIPC_EVENT_ID
 * @param cmd : 1 for setup, ohter for get
 * @param data : threshold value
 * @return int : returned threshold value
 */
int eipc_buffer_threshold_ext(int event_id, int cmd, int data);

/**
 * @brief get EIPC buffer used buffer size
 * @param event_id : Refer to EIPC_EVENT_ID
 * @return int : returned buffer size value
 */
int eipc_used_buffer_size_ext(int event_id);

void eipc_init(void);


#endif
