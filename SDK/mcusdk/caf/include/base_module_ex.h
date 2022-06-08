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

  File Name: base_module_ex.h

  Description: A short-circuit that serves as base-object for module interface
               Common types used in interface to modules

--------------------------------------------------------------------------------

Typical usage is as follows:
  1. Call BaseModuleCommand(...STRUCT_INT16_SIZE_OF) to get required memory size
  2. Allocate the number of words of memory specified.
  3. Call BaseModuleCommand(...INIT_DATA) to set parameters affecting extra memory
  4. Call BaseModuleCommand(...EXTRA_INT16_SIZE_OF) to get required extra size.
  5. Allocate the number of words of extra memory specified.
  6. Call BaseModuleSetup  (...INITIALIZE) with pointers to the allocated memory
  7. Call BaseModuleCommand() to set up all relevant parameters.
  8. Call BaseModuleApply() for each processing frame to process samples.

------------------------------------------------------------------------------*/

#ifndef CAF_INCLUDE_BASE_MODULE_EX_H_
#define CAF_INCLUDE_BASE_MODULE_EX_H_

#include "cape_types.h"

/*------------------------------------------------------------------------------
  Type:       AbstractModule

  Purpose:    Type-casting of module-pointers to an abstract type
------------------------------------------------------------------------------*/
typedef struct AbstractModuleTag {
  int64_t value64 ;
} AbstractModule ;


/*------------------------------------------------------------------------------
  Prototypes of module interface functions
------------------------------------------------------------------------------*/
typedef int  (ModuleCommand)(      AbstractModule * module_state      ,
                             const uint32_t         command_code      ,
                                   void           * command_data      ,
                             const uint32_t         int32_size_of_data );

typedef void (ModuleApply )(AbstractModule * module_state, int num_out_samples);

/*------------------------------------------------------------------------------
  Type:       ModuleTypeAPI

  Purpose:    API function pointers 
------------------------------------------------------------------------------*/
typedef struct {
  ModuleCommand     *setup;
  ModuleCommand     *command;
  ModuleApply       *apply;
} ModuleTypeAPI;

#define MODULE_API_DECLARATION(module)  \
  ModuleCommand module##Setup;          \
  ModuleCommand module##Command;        \
  ModuleApply   module##Apply

#define MODULE_API_FUNCTIONS(module) \
  {module##Setup, module##Command, module##Apply}

#define MODULE_API_FUNCTIONS_NULL {NULL, NULL, NULL}

/*------------------------------------------------------------------------------
  Type:       ModuleInstance

  Purpose:    Module instance inforamtion
------------------------------------------------------------------------------*/
typedef struct {
  int             module_type;     // Module type index
  AbstractModule* module_state;    // pointer to module instance data
} ModuleInstance;

// Indicates an invalid or unused ModuleInstance object
#define MODULE_TYPE_NONE        -1
#define MODULE_TYPE_RESERVED    -2
/****************************************************************************/

/*------------------------------------------------------------------------------
  Type:       ModuleReturnCode

  Purpose:    Return codes of module's functions
------------------------------------------------------------------------------*/
#define MODULE_RC_FIRST -1024

typedef enum ModuleReturnCodeTag {
  MODULE_RC_SUCCESS                = 0,

  MODULE_RC_GENERIC_FAILURE        = MODULE_RC_FIRST- 1, // -1025
  MODULE_RC_ALREADY_EXISTS         = MODULE_RC_FIRST- 2, // -1026
  MODULE_RC_NULL_APP_POINTER       = MODULE_RC_FIRST- 3, // -1027
  MODULE_RC_NULL_MODULE_POINTER    = MODULE_RC_FIRST- 4, // -1028
  MODULE_RC_NULL_STREAM_POINTER    = MODULE_RC_FIRST- 5, // -1029
  MODULE_RC_NULL_POINTER           = MODULE_RC_FIRST- 6, // -1030

  MODULE_RC_BAD_APP_ID             = MODULE_RC_FIRST- 7, // -1031
  MODULE_RC_MODULE_ID_OUT_OF_RANGE = MODULE_RC_FIRST- 8, // -1032
  MODULE_RC_MODULE_ID_NOT_CREATED  = MODULE_RC_FIRST- 9, // -1033
  MODULE_RC_MODULE_TYPE_HAS_NO_API = MODULE_RC_FIRST-10, // -1034
  MODULE_RC_STREAM_ID_OUT_OF_RANGE = MODULE_RC_FIRST-11, // -1035
  MODULE_RC_STREAM_ID_NOT_CREATED  = MODULE_RC_FIRST-12, // -1036
  MODULE_RC_BAD_SIGNAL_TYPE        = MODULE_RC_FIRST-13, // -1037
  MODULE_RC_BAD_IO_TYPE            = MODULE_RC_FIRST-14, // -1038
  MODULE_RC_MODULE_LIB_NOT_LOADED  = MODULE_RC_FIRST-15, // -1039

  MODULE_RC_ALLOC_APP_FAILURE      = MODULE_RC_FIRST-17, // -1041
  MODULE_RC_ALLOC_HEAP_FAILURE     = MODULE_RC_FIRST-18, // -1042
  MODULE_RC_ALLOC_MODULE_FAILURE   = MODULE_RC_FIRST-19, // -1043
  MODULE_RC_ALLOC_STREAM_FAILURE   = MODULE_RC_FIRST-20, // -1044
  MODULE_RC_ALLOC_FAILURE          = MODULE_RC_FIRST-25, // -1049
  MODULE_RC_FILE_OPEN_ERROR        = MODULE_RC_FIRST-26, // -1050
  MODULE_RC_BAD_MEM_TYPE           = MODULE_RC_FIRST-27, // -1051
  MODULE_RC_BAD_MAGIC_NUMBER       = MODULE_RC_FIRST-28, // -1052

  // Codes specific to module's Command()
  MODULE_RC_CMD_MODE_UNSUPPORTED   = MODULE_RC_FIRST-32, // -1056 Unsupported mode (SET or GET)
  MODULE_RC_CMD_CODE_UNSUPPORTED   = MODULE_RC_FIRST-33, // -1057 Unknown value of command_code
  MODULE_RC_CMD_DATA_SIZE_WRONG    = MODULE_RC_FIRST-34, // -1058 Wrong value of int32_size_of_data
  MODULE_RC_CMD_DATA_INCORRECT     = MODULE_RC_FIRST-35, // -1059 Incorrect data in (*command_data)

  MODULE_RC_VALUE_OUT_OF_RANGE     = MODULE_RC_FIRST-36, // -1060
  MODULE_RC_INDEX_OUT_OF_BOUNDS    = MODULE_RC_FIRST-37, // -1061
  MODULE_RC_ARRAY_IS_FULL          = MODULE_RC_FIRST-38, // -1062

  MODULE_RC_TUNNEL_INVALID_ID      = MODULE_RC_FIRST-45, // -1069
  MODULE_RC_TUNNEL_NOT_READY       = MODULE_RC_FIRST-46, // -1070
  MODULE_RC_TUNNEL_INVALID_DEVICE  = MODULE_RC_FIRST-47, // -1071
  MODULE_RC_TUNNEL_DATA_UNALIGNED  = MODULE_RC_FIRST-48, // -1072
  MODULE_RC_FILE_NOT_PRESENT       = MODULE_RC_FIRST-49, // -1073

  MODULE_RC_ALLOC_HEAP_SEG0_FAILED = MODULE_RC_FIRST-50, // -1074
  MODULE_RC_ALLOC_HEAP_SEG1_FAILED = MODULE_RC_FIRST-51, // -1075
  MODULE_RC_ALLOC_HEAP_SEG2_FAILED = MODULE_RC_FIRST-52, // -1076
  MODULE_RC_ALLOC_HEAP_SEG3_FAILED = MODULE_RC_FIRST-53, // -1077
  MODULE_RC_ALLOC_HEAP_SEG4_FAILED = MODULE_RC_FIRST-54, // -1078
  MODULE_RC_ALLOC_HEAP_SEG5_FAILED = MODULE_RC_FIRST-55, // -1079
  MODULE_RC_ALLOC_HEAP_SEG6_FAILED = MODULE_RC_FIRST-56, // -1080
  MODULE_RC_ALLOC_HEAP_SEG7_FAILED = MODULE_RC_FIRST-57, // -1081
  MODULE_RC_ALLOC_HEAP_SEG8_FAILED = MODULE_RC_FIRST-58, // -1082
  MODULE_RC_ALLOC_HEAP_SEG9_FAILED = MODULE_RC_FIRST-59  // -1083
} ModuleReturnCode ;

/****************************************************************************/
/*------------------------------------------------------------------------------
  Common command codes for ModuleCommand
------------------------------------------------------------------------------*/
// Code MODULE_CMD_RESERVED is reserved for system. Do not use.
#define MODULE_CMD_RESERVED              0x0000

// Code MODULE_CMD_STRUCT_INT16_SIZE_OF - command data is 1 uint32_t.
// GET: The module fills in its data-structure size (in "int16_t" elements).
#define MODULE_CMD_STRUCT_INT16_SIZE_OF  0x0001

// Code MODULE_CMD_INIT_DATA - command data is one ModuleInitData with
// variable-size "specific[]" array.
// GET: The module fills in its Init data-structure.
#define MODULE_CMD_INIT_DATA             0x0002

// Code MODULE_CMD_EXTRA_INT16_SIZE_OF - command data is one uint32_t.
// GET: The data serves as an input argument of type HeapMemSegmentId identifying
// a certain memory section. The module fills in the extra size that it needs in
// that memory section (in "int16_t" elements).
#define MODULE_CMD_EXTRA_INT16_SIZE_OF   0x0003

// Code MODULE_CMD_INITIALIZE - command data is one ModuleExtraMem.
// SET: The module performs initialization.
#define MODULE_CMD_INITIALIZE            0x0004

// Code MODULE_CMD_CLEAR_STATE - command data is empty
// SET: The module reset all the state variables (clears its history)
#define MODULE_CMD_CLEAR_STATE           0x0005

// Code MODULE_CMD_DEFAULT_CONFIG - command data is empty
// SET: The module changes all configurations to default
#define MODULE_CMD_DEFAULT_CONFIG        0x0006

// Code MODULE_CMD_ACTIVE - command data is one ModuleActive.
#define MODULE_CMD_ACTIVE                0x0007

// Code MODULE_CMD_SAMPLE_RATE - command data is one uint32_t representing
// the processing sample rate enumerated according to SampleRateCode.
#define MODULE_CMD_SAMPLE_RATE           0x0008

// Code MODULE_CMD_INPUT_RATE - command data is one uint32_t representing
// the input sample rate enumerated according to SampleRateCode.
#define MODULE_CMD_INPUT_RATE            0x0009

// Code MODULE_CMD_FULL_CONFIG - command data is the entire configuration
// of the specific module, except for the Init data.
// GET/SET are used when the system goes to suspend/resume correspondingly.
#define MODULE_CMD_FULL_CONFIG           0x000A

// Code MODULE_CMD_MODULE_TYPE - command data is one ModuleType.
// GET: The module fills in its corresponding type.
#define MODULE_CMD_MODULE_TYPE           0x000B

// Code MODULE_CMD_VERSION_ID - command data is one uint32_t.
// GET: The module fills in the version identifier.
#define MODULE_CMD_VERSION_ID            0x000C

// Code MODULE_CMD_DEBUG_STREAM - command data is one (SignalStream *).
// GET: The data is used for connecting the debug output to subsequent modules.
#define MODULE_CMD_DEBUG_STREAM          0x000E

// Code MODULE_CMD_OUTPUT_STREAM - command data is one (SignalStream *).
// SET: The data is used for initializing the module's output stream.
// GET: The data is used for connecting the module to subsequent modules.
#define MODULE_CMD_OUTPUT_STREAM         0x000F

// Code MODULE_CMD_INPUT_STREAM# - command data is one (SignalStream *).
// SET: The data is used for establishing an input connection of the module.
#define MODULE_CMD_INPUT_STREAM0         0x0010
#define MODULE_CMD_INPUT_STREAM1         0x0011
#define MODULE_CMD_INPUT_STREAM2         0x0012
#define MODULE_CMD_INPUT_STREAM3         0x0013
#define MODULE_CMD_INPUT_STREAM4         0x0014
#define MODULE_CMD_INPUT_STREAM5         0x0015
#define MODULE_CMD_INPUT_STREAM6         0x0016
#define MODULE_CMD_INPUT_STREAM7         0x0017
#define MODULE_CMD_INPUT_STREAM8         0x0018
#define MODULE_CMD_INPUT_STREAM9         0x0019

//MODULE_CMD_INPUT_STATUS - command data is one uint32_t.
//SET: Clears the status indicators
//GET: The module fills its status bit-field flags for the input stream -
//bit  0 indicates stall condition occured due to lack of input samples
//bit  1 indicates overrun  (write crossed read ) occured in the input buffer
//bit  2 indicates underrun (read  crossed write) occured in the input buffer
//but 31 indicates this command is not applicable
#define MODULE_CMD_INPUT_STATUS          0x0020

// Code MODULE_CMD_COMMAND_CALLBACK - command data is one CommandCallback.
// SET: The data holds pointers to the command function to call and to the
// related data-structure.
#define MODULE_CMD_COMMAND_CALLBACK      0x0025

// Code MODULE_CMD_SCRATCH_ALIGNMENT_OF - command data is uint32_t.
// GET: The module fills in its required allignment of scratch memory 
//      (0: 16-bit, 1: 32-bit, etc).
#define MODULE_CMD_SCRATCH_ALIGNMENT_OF  0x0026

// Code MODULE_CMD_SCRATCH_INT16_SIZE_OF - command data is uint32_t.
// GET: The module's required size of scratch memory (in "int16_t" elements).
#define MODULE_CMD_SCRATCH_INT16_SIZE_OF 0x0027

// Code MODULE_CMD_SCRATCH_PTR - command data is one (int64_t *).
// SET: The data contains the address of the scratch memory.
#define MODULE_CMD_SCRATCH_PTR           0x0028

// Code MODULE_CMD_LOGGING_INIT - command data is one uint32_t.
// SET: Initializes the logging nodes associated with this module. 
//      the data is the ID of the app containing this module
#define MODULE_CMD_LOGGING_INIT          0x0029

// Code MODULE_CMD_LOGGING_ENABLE - command data is one ModuleEnableLogging.
// SET: Enables or disables logging of specific nodes. 
#define MODULE_CMD_LOGGING_ENABLE        0x002A

// Code MODULE_CMD_TIMING_INFO - command data is one AsyncSrcTimingInfo.
// SET: The data is used for connecting the information for timing tracking.
#define MODULE_CMD_TIMING_INFO           0x002B

// Code MODULE_CMD_FRAME_CYCLES - command data is one uint32_t.
// GET: The data is the number of CPU clock cycles for processing the last frame.
#define MODULE_CMD_FRAME_CYCLES          0x002C

// Code MODULE_CMD_SAVE_RESTORE   - command data is one uint32_t.
// GET: Triplets of each block address, length in words and type of memory
#define MODULE_CMD_SAVE_RESTORE          0x002D

#define STANDARD_CMD_CODES(name)                                     \
  name##_CMD_RESERVED             = MODULE_CMD_RESERVED             ,\
  name##_CMD_STRUCT_INT16_SIZE_OF = MODULE_CMD_STRUCT_INT16_SIZE_OF ,\
  name##_CMD_INIT_DATA            = MODULE_CMD_INIT_DATA            ,\
  name##_CMD_EXTRA_INT16_SIZE_OF  = MODULE_CMD_EXTRA_INT16_SIZE_OF  ,\
  name##_CMD_INITIALIZE           = MODULE_CMD_INITIALIZE           ,\
  name##_CMD_CLEAR_STATE          = MODULE_CMD_CLEAR_STATE          ,\
  name##_CMD_DEFAULT_CONFIG       = MODULE_CMD_DEFAULT_CONFIG       ,\
  name##_CMD_ACTIVE               = MODULE_CMD_ACTIVE               ,\
  name##_CMD_SAMPLE_RATE          = MODULE_CMD_SAMPLE_RATE          ,\
  name##_CMD_INPUT_RATE           = MODULE_CMD_INPUT_RATE           ,\
  name##_CMD_FULL_CONFIG          = MODULE_CMD_FULL_CONFIG          ,\
  name##_CMD_MODULE_TYPE          = MODULE_CMD_MODULE_TYPE          ,\
  name##_CMD_VERSION_ID           = MODULE_CMD_VERSION_ID           ,\
  name##_CMD_DEBUG_STREAM         = MODULE_CMD_DEBUG_STREAM         ,\
  name##_CMD_OUTPUT_STREAM        = MODULE_CMD_OUTPUT_STREAM        ,\
  name##_CMD_INPUT_STREAM0        = MODULE_CMD_INPUT_STREAM0        ,\
  name##_CMD_INPUT_STREAM1        = MODULE_CMD_INPUT_STREAM1        ,\
  name##_CMD_INPUT_STREAM2        = MODULE_CMD_INPUT_STREAM2        ,\
  name##_CMD_INPUT_STREAM3        = MODULE_CMD_INPUT_STREAM3        ,\
  name##_CMD_INPUT_STREAM4        = MODULE_CMD_INPUT_STREAM4        ,\
  name##_CMD_INPUT_STREAM5        = MODULE_CMD_INPUT_STREAM5        ,\
  name##_CMD_INPUT_STREAM6        = MODULE_CMD_INPUT_STREAM6        ,\
  name##_CMD_INPUT_STREAM7        = MODULE_CMD_INPUT_STREAM7        ,\
  name##_CMD_INPUT_STREAM8        = MODULE_CMD_INPUT_STREAM8        ,\
  name##_CMD_INPUT_STREAM9        = MODULE_CMD_INPUT_STREAM9        ,\
  name##_CMD_INPUT_STATUS         = MODULE_CMD_INPUT_STATUS         ,\
  name##_CMD_COMAMND_CALLBACK     = MODULE_CMD_COMMAND_CALLBACK     ,\
  name##_CMD_SCRATCH_ALIGNMENT_OF = MODULE_CMD_SCRATCH_ALIGNMENT_OF ,\
  name##_CMD_SCRATCH_INT16_SIZE_OF= MODULE_CMD_SCRATCH_INT16_SIZE_OF,\
  name##_CMD_SCRATCH_PTR          = MODULE_CMD_SCRATCH_PTR          ,\
  name##_CMD_LOGGING_INIT         = MODULE_CMD_LOGGING_INIT         ,\
  name##_CMD_LOGGING_ENABLE       = MODULE_CMD_LOGGING_ENABLE       ,\
  name##_CMD_TIMING_INFO          = MODULE_CMD_TIMING_INFO          ,\
  name##_CMD_FRAME_CYCLES         = MODULE_CMD_FRAME_CYCLES         ,\
  name##_CMD_SAVE_RESTORE         = MODULE_CMD_SAVE_RESTORE

//------------------------------------------------------------------------------

#define MODULE_CMD_GET_STRUCT_INT16_SIZE_OF  CMD_GET(MODULE_CMD_STRUCT_INT16_SIZE_OF)
#define MODULE_CMD_GET_INIT_DATA             CMD_GET(MODULE_CMD_INIT_DATA    )
#define MODULE_CMD_GET_EXTRA_INT16_SIZE_OF   CMD_GET(MODULE_CMD_EXTRA_INT16_SIZE_OF)
#define MODULE_CMD_GET_INITIALIZE            CMD_GET(MODULE_CMD_INITIALIZE   )
#define MODULE_CMD_SET_ACTIVE                CMD_SET(MODULE_CMD_ACTIVE       )
#define MODULE_CMD_GET_ACTIVE                CMD_GET(MODULE_CMD_ACTIVE       )
#define MODULE_CMD_SET_SAMPLE_RATE           CMD_SET(MODULE_CMD_SAMPLE_RATE  )
#define MODULE_CMD_GET_SAMPLE_RATE           CMD_GET(MODULE_CMD_SAMPLE_RATE  )
#define MODULE_CMD_SET_INPUT_RATE            CMD_SET(MODULE_CMD_INPUT_RATE   )
#define MODULE_CMD_GET_INPUT_RATE            CMD_GET(MODULE_CMD_INPUT_RATE   )
#define MODULE_CMD_GET_MODULE_TYPE           CMD_GET(MODULE_CMD_MODULE_TYPE  )
#define MODULE_CMD_GET_VERSION_ID            CMD_GET(MODULE_CMD_VERSION_ID   )
#define MODULE_CMD_GET_DEBUG_STREAM          CMD_GET(MODULE_CMD_DEBUG_STREAM )
#define MODULE_CMD_SET_OUTPUT_STREAM         CMD_SET(MODULE_CMD_OUTPUT_STREAM)
#define MODULE_CMD_GET_OUTPUT_STREAM         CMD_GET(MODULE_CMD_OUTPUT_STREAM)
                                             
#define MODULE_CMD_SET_INPUT_STREAM0         CMD_SET(MODULE_CMD_INPUT_STREAM0)
#define MODULE_CMD_SET_INPUT_STREAM1         CMD_SET(MODULE_CMD_INPUT_STREAM1)
#define MODULE_CMD_SET_INPUT_STREAM2         CMD_SET(MODULE_CMD_INPUT_STREAM2)
#define MODULE_CMD_SET_INPUT_STREAM3         CMD_SET(MODULE_CMD_INPUT_STREAM3)
#define MODULE_CMD_SET_INPUT_STREAM4         CMD_SET(MODULE_CMD_INPUT_STREAM4)
#define MODULE_CMD_SET_INPUT_STREAM5         CMD_SET(MODULE_CMD_INPUT_STREAM5)
#define MODULE_CMD_SET_INPUT_STREAM6         CMD_SET(MODULE_CMD_INPUT_STREAM6)
#define MODULE_CMD_SET_INPUT_STREAM7         CMD_SET(MODULE_CMD_INPUT_STREAM7)
#define MODULE_CMD_SET_INPUT_STREAM8         CMD_SET(MODULE_CMD_INPUT_STREAM8)
#define MODULE_CMD_SET_INPUT_STREAM9         CMD_SET(MODULE_CMD_INPUT_STREAM9)

#define MODULE_CMD_SET_INPUT_STATUS          CMD_SET(MODULE_CMD_INPUT_STATUS )
#define MODULE_CMD_GET_INPUT_STATUS          CMD_GET(MODULE_CMD_INPUT_STATUS )

#define MODULE_CMD_SET_COMMAND_CALLBACK      CMD_SET(MODULE_CMD_COMMAND_CALLBACK)
#define MODULE_CMD_GET_SCRATCH_ALIGNMENT_OF  CMD_GET(MODULE_CMD_SCRATCH_ALIGNMENT_OF)
#define MODULE_CMD_GET_SCRATCH_INT16_SIZE_OF CMD_GET(MODULE_CMD_SCRATCH_INT16_SIZE_OF)

#define MODULE_CMD_GET_FRAME_CYCLES          CMD_GET(MODULE_CMD_FRAME_CYCLES )

#define MODULE_CMD_GET_SAVE_RESTORE          CMD_GET(MODULE_CMD_SAVE_RESTORE )

/****************************************************************************/

/*------------------------------------------------------------------------------
  Type:       ModuleInitData

  Purpose:    Parameters for module creation
------------------------------------------------------------------------------*/
typedef struct ModuleInitCommonTag {
  uint32_t  module_id    ; // Unique numeric identifier of the module's instance
  int32_t   module_type  ; // Module type according to enum ModuleType
  uint32_t  out_sig_type ; // Output stream type according to enum SignalType
  uint32_t  out_buf_size ; // Output buffer size in samples per channel
} ModuleInitCommon;

typedef struct ModuleInitDataTag {
  ModuleInitCommon common     ; // parameters common to all modules
  int32_t          specific[1]; // module-specific parameters for memory alloc.
                           // NOTE: Actual array length may be different than 1!
} ModuleInitData;

/*------------------------------------------------------------------------------
  Type:       ModuleExtraSize

  Purpose:    Size of module data in addition to its main data-structure.
------------------------------------------------------------------------------*/
typedef enum HeapMemSegmentIdTag {
  HEAP_SEGMENT0 = 0,
  HEAP_SEGMENT1    ,
  HEAP_SEGMENT2    ,
  HEAP_SEGMENT3    , 
  HEAP_SEGMENT4    ,
  HEAP_SEGMENT5    ,
  HEAP_SEGMENT6    ,
  HEAP_SEGMENT7    ,
  HEAP_SEGMENT8    ,
  HEAP_SEGMENT9
} HeapMemSegmentId;

typedef enum {
  MEM_TYPE_X = 0,
  MEM_TYPE_Y = 1,
} MemType;

/*------------------------------------------------------------------------------
  Type:       ModuleExtraMem

  Purpose:    Pointers for module data in addition to its main data-structure.
------------------------------------------------------------------------------*/
typedef struct ModuleExtraMemTag {
#ifdef CONFIG_NUM_HEAP_MEMORY_SEGMENTS
  int64_t        * ptr[CONFIG_NUM_HEAP_MEMORY_SEGMENTS]; // Pointers to module data in heap memory
#else
  int64_t        * ptr[0]; // Pointers to module data in heap memory
#endif
} ModuleExtraMem;

/*------------------------------------------------------------------------------
  Type:       ModuleActive

  Purpose:    Enumerates start-stop options
------------------------------------------------------------------------------*/
typedef enum ModuleActiveTag {
  MODULE_BYPASSED = 0,
  MODULE_ACTIVE   = 1,
  MODULE_DIRECTLY_RETURN = 2
} ModuleActive;

// Status indicators for input stream (command MODULE_CMD_INPUT_STATUS)
#define MODULE_CMD_INPUT_STALL_OCCURED    (1UL<< 0)
#define MODULE_CMD_INPUT_OVERRUN_OCCURED  (1UL<< 1)
#define MODULE_CMD_INPUT_UNDERRUN_OCCURED (1UL<< 2)
#define MODULE_CMD_INPUT_STATUS_NA        (1UL<<31)

/*------------------------------------------------------------------------------
  Type:       ModuleEnableLogging

  Purpose:    Data structure to enable/disbale logging of a specific node
------------------------------------------------------------------------------*/
#define LOGGING_TYPE_SCR  0
#define LOGGING_TYPE_DMP  1

#define LEAF_ID_BROADCAST 0xFFFFFFFF

typedef struct ModuleEnableLoggingTag {
  uint32_t  type    ; // 0 for SCR, 1 for DMP
  uint32_t  leaf_id ; // ID of the leaf to enable
  int32_t   enable  ; // 1 Enable/ 0 Disable logging.
} ModuleEnableLogging;

/*------------------------------------------------------------------------------
  Type:       ModuleCmdCode

  Purpose:    Enumerated commands for ModuleCommand
------------------------------------------------------------------------------*/
typedef enum BaseModuleCmdCodeTag {
  STANDARD_CMD_CODES(BASE_MODULE)  ,

  BASE_MODULE_CMD_SPECIFIC = 0x0040
} BaseModuleCmdCode;

/****************************************************************************/

/*------------------------------------------------------------------------------
  Function:   BaseModuleSetup()

  Purpose:    Configures or queries BaseModule with data that involves pointers

  Inputs:     base_module_state  - Pointer to BaseModule data structure
              command_code       - ModuleCmdCode enumerated command
              command_data       - Pointer to buffer of command arguments
              int32_size_of_data - Number of 32-bit elements in the data buffer

  Outputs:    MODULE_RC_SUCCESS if successful, else ModuleReturnCode error
------------------------------------------------------------------------------*/
int    BaseModuleSetup  (      AbstractModule * base_module_state ,
                         const uint32_t         command_code      ,
                               void           * command_data      ,
                         const uint32_t         int32_size_of_data );

/*------------------------------------------------------------------------------
  Function:   BaseModuleCommand()

  Purpose:    Configures parameters of BaseModule or triggers actions

  Inputs:     base_module_state  - Pointer to BaseModule data structure
              command_code       - ModuleCmdCode enumerated command
              command_data       - Pointer to buffer of command arguments
              int32_size_of_data - Number of 32-bit elements in the data buffer

  Outputs:    MODULE_RC_SUCCESS if successful, else ModuleReturnCode error
------------------------------------------------------------------------------*/
int    BaseModuleCommand(      AbstractModule * base_module_state ,
                         const uint32_t         command_code      ,
                               void           * command_data      ,
                         const uint32_t         int32_size_of_data );

/*------------------------------------------------------------------------------
  Function:   BaseModuleApply()

  Purpose:    Runs the BaseModule to process one frame of samples

  Inputs:     base_module_state  - Pointer to BaseModule data structure
              num_samples        - Samples per channel (Only for Edge modules)

  Outputs:    Nothing
------------------------------------------------------------------------------*/
void   BaseModuleApply  (AbstractModule * base_module_state , int num_samples);

/****************************************************************************/

/*------------------------------------------------------------------------------
  Type:       ModuleCallback

  Purpose:    Allow trigerring an external event from within a module
------------------------------------------------------------------------------*/
typedef struct ModuleCallbackTag
{
  ModuleCommand  * func;   // Pointing to the function to be called
  AbstractModule * object; // Pointing to the data-structure for the function
} ModuleCallback ;

typedef int (CmdCallbackFunc)(AbstractModule * module_state, void * command_data);

typedef struct CommandCallbackTag
{
  CmdCallbackFunc * func;   // Pointing to the function to be called
  AbstractModule  * object; // Pointing to the data-structure for the function
} CommandCallback ;

/*------------------------------------------------------------------------------
  Type:       IndexedVolume

  Purpose:    Allow configuring the volume of a specific channel out of
              multiple channels or of a specific stream out of multiple streams.
------------------------------------------------------------------------------*/
typedef struct IndexedVolumeTag
{
  int32_t index   ; /* Identifies the channel or stream that the gain is for */
#ifdef __cplusplus
  q8_23_t gain_dB ;
#else
  int32_t gain_dB ;
#endif /* __cplusplus */
} IndexedVolume ;

/*------------------------------------------------------------------------------
  Type:       BiquadFilterType

  Purpose:    Enumerated filter-types for 2nd order IIR (biquad) filter design
------------------------------------------------------------------------------*/
typedef enum BiquadFilterTypeTag
{
  BIQUAD_PEAKING_FILTER    = 0 ,
  BIQUAD_LOW_SHELF_FILTER      ,
  BIQUAD_HIGH_SHELF_FILTER     ,
  BIQUAD_LOW_PASS_FILTER       ,
  BIQUAD_HIGH_PASS_FILTER      ,
  BIQUAD_ALL_PASS_FILTER       ,
  BIQUAD_CUSTOM_COEFF

} BiquadFilterType ;

/*------------------------------------------------------------------------------
  Type:       AsyncSrcTimingInfo

  Purpose:    Data structure for the command MODULE_CMD_TIMING_INFO
------------------------------------------------------------------------------*/
typedef struct AsyncSrcTimingInfoTag {
  int32_t   is_for_output; // The timing info is for: 0 - input; 1 - output
  int64_t * info_address ; // pointing to the count of clock cycles and count of
                           // samples per channel at the low/high parts
} AsyncSrcTimingInfo;

/*------------------------------------------------------------------------------
  Function:   TimingInfoDecompose()

  Purpose:    Decomposes packed timing-information into its 2 components

  Inputs:     t_info             - Packed form of the timing information

  Outputs:    clock_cycles       - Count of clock cycles
              num_samples        - Count of samples per channel
------------------------------------------------------------------------------*/
inline void TimingInfoDecompose(int64_t   t_info,
                                uint32_t* num_samp, uint32_t* clock_cyc) {
  uint64_t cc, ns;
  cc = (uint64_t)t_info & (uint64_t)0x00000000FFFFFFFF;
  ns = (uint64_t)t_info >> 32;
  *clock_cyc = (uint32_t)(cc);
  *num_samp  = (uint32_t)(ns);
}

/*------------------------------------------------------------------------------
  Function:   TimingInfoConstruct()

  Purpose:    Constructs packed timing-information from its 2 components

  Inputs:     clock_cycles       - Count of clock cycles
              num_samples        - Count of samples per channel

  Outputs:    t_info             - Packed form of the timing information
------------------------------------------------------------------------------*/
inline int64_t TimingInfoConstruct(uint32_t num_samp, uint32_t clock_cyc) {
  return (((uint64_t)num_samp << 32) | (uint64_t)clock_cyc);
}

#endif  // CAF_INCLUDE_BASE_MODULE_EX_H_
