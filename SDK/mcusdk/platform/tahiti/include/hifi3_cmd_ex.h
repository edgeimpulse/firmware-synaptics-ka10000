/*------------------------------------------------------------------------------
  Unpublished Work Copyright � 2018-2019 Synaptics Incorporated.
  All rightsreserved.

  This file contains information that is proprietary to Synaptics Incorporated
  (�Synaptics�). The holder of this file shall treat all information contained
  herein as confidential, shall use the information only for its intended purpose,
  and shall not duplicate, disclose, or disseminate any of this information in
  any manner unless Synaptics has otherwise provided express, written permission.
  Use of the materials may require a license of intellectual property from a
  third party or from Synaptics. This file conveys no express or implied licenses
  to any intellectual property rights belonging to Synaptics.
--------------------------------------------------------------------------------

  File Name: hifi3_cmd_ex.h

  Description: struct and enumeration that are shared by supervisor and HiFi3

------------------------------------------------------------------------------*/

#ifndef AUDIOFW_CODE_INCLUDE_CMD_HIFI3_EX_H_
#define AUDIOFW_CODE_INCLUDE_CMD_HIFI3_EX_H_


#define PLBK_PROCESS_ID 1
#define CAPT_PROCESS_ID 2
#define TRIG_PROCESS_ID 3 

#ifdef CONFIG_CHIP_NAME_KATANA
//TICK format:   8bits tick_code + 4bits io_idx + 20bits num_samples
#define ID_TICK             (0xFF000000)
#define GET_FRAME_LEN(x)    (x&0x000FFFFF)
#define PROCESS_ID(x)       ((x>>20)&0x0000000F)

#else

#define ID_TICK        (0x0FFF<<16)
#define GET_FRAME_LEN(x)   (x&0x000000FF)
#define PROCESS_ID(x)  ((x>>8)&0x000000FF)

#endif//CHIP_NAME_KATANA

#ifdef SIMULATION
#define  XM_2_AHB(x)             (x)
#define  SYSM_2_AHB(x)           (x) 
#define  AHB_2_SYSM(x)           (x)
#else
#define  XM_2_AHB(x)             ((((size_t)(x)) & 0x000fffff) | 0x41200000) 
#define  SYSM_2_AHB(x)           ((((size_t)(x)) & 0x001fffff) | 0x41700000) 
#define  AHB_2_SYSM(x)           (((size_t)(x)) & 0x001fffff)
#endif 

/**
 * hifi3_cmd_ex.h
 *
 * 
 * \section Definition
 * \include CommandStructure.c
 * 
 * \section Description
 * 
 * The command can hold parameter values that are passed according to the specified target:
 * 
 * * mgr_module_id – A 32-bit variable holding a manager ID at the upper 24 bits and a module ID at the lower 8 bits.
 * 
 * * command_id – Identifies the command to the recipient. There are two types of commands: \n
 * SET command – Bit 8 in command_id is clear – The sender is writing data or signaling the recipient, and the payload of the reply is therefore assumed to be empty. \n
 * GET command – Bit 8 in command_id is set – The sender is retrieving data from the recipient and both incoming and outgoing commands can contain a data payload.
 *
 * * data – An optional set of 32-bit values, which is the payload of the command. Its size varies among commands and can be up to 13. Parameter values can be specified numerically or as variables or in some cases as C language pre-processor definitions. For example, it is possible to use 0 or MODULE_BYPASSED for setting a module to bypass mode and to use 1 or MODULE_ACTIVE for setting a module to active mode.
 *
 * * num_32b_words – The number of 32-bit elements in the command’s payload. \n
 * SET command – When the command is sent, num_32b_words holds the size of the data payload in 32-bit words. In the reply, it holds a non-positive return code. \n
 * GET command – When the command is sent, num_32b_words holds the size of the memory area that’s available to store the recipient’s data in 32-bit words. In the reply, num_32b_words holds the size of data stored in the payload in 32-bit words or a negative value in case of a failure .
 *
 * * reply – Identifies the direction of the command. If the bit is clear, the command is a request from the host to the device. The sender clears the bit before sending the command. If the bit is set, the command is a reply from the device to the host. The recipient sets the bit after it completed processing the command.
 *
 * \section Example
 * From Sculptor, we can send the following command to HiFi3 to set the amplifier factor of playback signal for captrue processing. \n
 * here, \c <HM> is mgr_module_id; \c SET_PARAMETER_VALUE is command_id; \c {...} is data part
 * \code golem::sendcmd <HM> SET_PARAMETER_VALUE {AMPLIFY_FACTOR_REF, 2, (q8_23_t)1.0} \endcode
 * 
 *
 * 
 * \defgroup CommandDataStructure Command Data Structure
 * \ingroup Programming_API_Command 
 */


/** 
 * hifi3_cmd_ex.h
 * 
 * \include CommandsInHiFi3.c
 *
 * * CONTROL_MGR_RESET \n
 * Set-mode only. Instructs HiFi3 to clean all buffers in IO queue and destroy all processing alogorithm.
 * 
 * * SYSTEM_MEMORY \n
 * Set shared system memory to HiFi3. 
 * 
 * * INIT_PROCESSING \n
 * Set-mode only. Instructs HiFi3 to create one processing instance and initilize it. Here, \c ThirdInitXXX is called. For example:
 * \code golem::sendcmd <AP> MODULE_APP_MODULE_CREATE {mdl::plbk_supervisor, MODULE_TYPE_HIFI3_SUPERVISOR, kSignalType2StereoCSS, [expr $prm::PLBK_FRAME*15], 0x1, 0x20, 0x30} \endcode  
 * It creates HiFi3Supervisor instance, then this supervisor sends several commands to HiFi3 to create one processing instance. \n 
 * Here, the parameters \c 0x1 is processing ID, ThirdParty can change it if necessary; \n  
 * \c 0x20 is shared system size in terms of 16-bit words for HiFi3Supervisor; \n
 * \c 0x30 is shared system size in terms of 16-bit words for the processing instance in HiFi3, ThirdParty can adjust it according to their request.  
 * 
 * * PARAMETER_VALUE \n
 * Instructs the Control Manager to destroy all Module Manager instances and all connecting streams. For example: 
 * \code golem::sendcmd <HM> SET_PARAMETER_VALUE {AMPLIFY_FACTOR_CAPT, 2, (q8_23_t)0.0, (q8_23_t)1.0} \endcode
 * It sets amplifier factor for the processing instacne whose ID is 2, mic signal factor is 0.0 and ref signal factor is 1.0.
 * @note ThirdParty can add more parameter types and define their value. The command data is \link HiFi3ThirdParameterTag HiFi3ThirdParameter \endlink Structure. 
 * 
 * \defgroup CommandsInHiFi3 Commands definition In HiFi3
 * \ingroup Programming_API_Command 
 */
/*------------------------------------------------------------------------------
  Type:       Hifi3CmdCode

  Purpose:    Enumerated commands for HiFi3SupervisorCommand
------------------------------------------------------------------------------*/

typedef enum Hifi3CmdCodeTag{
  HIFI3_CMD_RESERVED       =  0,
  SYSTEM_MEMORY            =  2,   //This is shared system memory, for example, 
                                   //HiFi3 can use it to access file system 
  INIT_PROCESSING          =  3,   /*!< init processing */
  PARAMETER_VALUE          =  4,
  HIFI3_PM_MSG             =  5,
/**
 *---------------------Katana Commands------------------------
 **/
  PROCESS_CREATE           =  100,
  PROCESS_PARAMETER_SETUP  =  101,
} Hifi3CmdCode;



/** 
 * hifi3_cmd_ex.h
 * 
 * In short, HiFi3 registered mailbox interrupt and mailbox callback is triggered if any core write any item into HiFi3's mailbox. 
 * The callback function \c MailbobxCallback pushes the item into corresponding queue. If the item is command, then push it to command queue; 
 * otherwise, the item is tick, then push it into IO queue.  \n
 * 
 * HiFi3 has CTRL task which keep waiting for any command in command queue, if any, handle it in \c CommandHandeler .

 *
 * * MailbobxCallback \n
 * Example code: 
 * \include MailbobxCallback.c
 * 
 * @note MailbobxCallback should return as soon as possible because it's in ISR context.
 * 
 * * CommandHandeler \n
 * Example code: 
 * \include CommandHandeler.c
 * 
 * \defgroup CommandsProcessingHiFi3 Commands processing In HiFi3
 * \ingroup Programming_API_Command 
 */

#define GET_SYSTEM_MEMORY                 CMD_GET(SYSTEM_MEMORY)
#define SET_SYSTEM_MEMORY                 CMD_SET(SYSTEM_MEMORY)

#define GET_PARAMETER_VALUE               CMD_GET(PARAMETER_VALUE)
#define SET_PARAMETER_VALUE               CMD_SET(PARAMETER_VALUE)

typedef enum {
 
  INPUT_STREAM                     = 1,
  OUTPUT_STREAM                    = 2,
  
  //ThirdParty puts parameter from here
  LIB_VERSION                      = 10,
  AMPLIFY_FACTOR_PLBK              = 11,
  AMPLIFY_FACTOR_CAPT              = 12,
  PROCESS_STATUS                   = 13,
  SELECT_CAPT_PROCESS              = 14,
  SET_WR_DELAY                     = 15,
} Hifi3ParameterIndex;


enum STREAM_INDEX_TABLE
{
    IDX_INPUT_STREAM    = 0, 
    IDX_OUTPUT_STREAM   = 1, 
    IDX_STREAM_MAX_NUM  = 2,
};

#if defined CONFIG_DSP_APP_SENS_TRIG 
typedef struct {
  int   trigger_detect;
  int   hit_num;
  int   map_changed;
  int   reserved0;
} SensTrggrCmd;
#endif //CONFIG_DSP_APP_SENS_TRIG

typedef struct HiFi3ShareBufTag{

  int  process_id;
  int  num_32b_words;    //size in bytes 
  void *p_buf ;          //points to parameter buffer

} HiFi3SystemBuf;

typedef struct HiFi3ParameterTag{
  int  param_index;  
  int  process_id;
  void *p_param_struct ;      //points to parameter struct

} HiFi3Parameter;

typedef struct HiFi3ThirdParameterTag {
  int  param_index;
  int  process_id;
  int  p_param_struct[5];  // the total size can not be more than command->data size

} HiFi3ThirdParameter;

void HiFi3CreateCtrlTask() ;
int HiFi3SendCommandToMCU(void *p_cmd, uint32_t wait_timeout);

#endif  // AUDIOFW_CODE_INCLUDE_CMD_HIFI3_EX_H_
