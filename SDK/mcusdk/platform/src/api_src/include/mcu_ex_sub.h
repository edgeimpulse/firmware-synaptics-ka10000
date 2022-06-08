typedef enum {
  FWID_SYS = 0,   //Target to SysCmdCode which in mcu_ex.h
  FWID_SW  = 2,   //Target to SwCmdCode  which in mcu_ex.h
  FWID_EXT = 3,   //Target to SDK Extension commands  which in mcu_ex.h
} FwModuleId;

/*
 * Sys related Commands Codes which not suitable to group to HW/SW.
 */
typedef enum {
  SYS_CMD_VERSION                       = 1,
  SYS_CMD_SOFTRESET                     = 3,
  SYS_CMD_CONF_ERR_INFO                 = 8,
  SYS_CMD_EXEC_FILE                     = 9,
  SYS_CMD_PARAMETER_VALUE               = 14,

  SYS_CMD_EVENT_PARAM                   = 15,
  SYS_CMD_MODE_ID                       = 19,
  SYS_CMD_SCRIPT_DATA                   = 20,
  SYS_CMD_BL_VERSION                    = 26,
  SYS_CMD_CURRENT_SYS_PARTITION         = 28,
  SYS_CMD_IPC_CMD                       = 37,
  SYS_CMD_IPC_MSG                       = 38,
  SYS_CMD_EVENT_MASK                    = 39,
  SYS_CMD_SIDETONE_ENABLE               = 47,
  SYS_CMD_SIDETONE_VOLUME               = 48,
  SYS_CMD_START_BG_UPGRADE              = 54,
} SysCmdCode;


/*
 * SW Drv related commands code.
 */
typedef enum {
  SW_CMD_NONE                           = 0,

  SW_SPIMEM_TUNNEL_CMD                  = 1,
  SW_SPIMEM_TUNNEL_CMD_CONFIG           = 2,
  SW_SPIMEM_TUNNEL_CMD_DATA             = 3,
  SW_SPIMEM_TUNNEL_CMD_APPLY            = 4,
  SW_SPIMEM_TUNNEL_CMD_ERASE            = 5,
  SW_SPIMEM_TUNNEL_CMD_PROTECT          = 6,
  SW_SFS_CMD_TUNED_MODES                = 13,
  SW_SFS_PERSIST_CMD_FILE_CREATE        = 18,
  SW_SFS_PERSIST_CMD_FILE_OPEN          = 19,
  SW_SFS_PERSIST_CMD_FILE_CLOSE         = 20,
  SW_SFS_PERSIST_CMD_FILE_DELETE        = 21,
  SW_SFS_PERSIST_CMD_FILE_ACCESS        = 22,
  SW_SFS_PERSIST_CMD_FILE_SEEK          = 23,
  SW_SFS_CMD_CUSTOM_PARTITION_INFO      = 34,

  SW_SFWUG_CMD_ERASE                    = 35, 
  SW_SFWUG_CMD_WRITE                    = 36,
  SW_SFWUG_CMD_READ                     = 37,
  SW_SFWUG_CMD_CONFIG                   = 38,
  SW_SFWUG_CMD_STATUS                   = 39,
  SW_SFWUG_CMD_DATA                     = 40,
  SW_SFWUG_CMD_I2CS                     = 41,
  

} SwCmdCode;
