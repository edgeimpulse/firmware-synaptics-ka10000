
#ifdef __cplusplus
extern "C"
{
#endif

#define ID_CTRL       _ID('C','T','R','L')
#define ID_CTRL_A     _ID('<','A','M','>')
#define ID_CTRL_B     _ID('<','B','M','>')
#define ID_INDX_FILE  _ID('I','N','D','X')
#define ID_EOF        _ID('E','O','F','0')
#define ID_CAIO       _ID('<','A','W','>')  //core A IO
#define ID_CBIO       _ID('<','B','W','>')  //core B IO
typedef enum {
  CONTROL_MGR_VERSION            =  3,
  CONTROL_MGR_EXEC_FILE          =  4,
//CONTROL_MGR_EVENT_PARAM - Command data is an odd number of uint32_t.
// SET: The first parameter is a bit-field mask that indicates the events to trigger,
// followed by pairs of parameter ID and parameter value for parameterized scripts.
  CONTROL_MGR_EVENT_PARAM        = 13,
  CONTROL_MGR_MODE_ID            = 14,
  CONTROL_MGR_PARAMETER_VALUE    = 37,
  CONTROL_MGR_SCRIPT_DATA        = 84,
  // CONTROL_MGR_TUNED_MODES
  // GET: This command returns with a list of available tuned modes (modes from ext_scripts folder)
  //      This command has only one argument which is the index to start reading the modes from.
  //      e.g.: To start reading the modes, send this command with an input argument of 0
  //            If after reading 13 modes, the host can ask to read more from index 13, by
  //            sending this command again with the input argument of 13
  CONTROL_MGR_TUNED_MODES        = 85,
} ControlMgrCmdCode;

typedef enum {
  BASE_CONFIG_SCRIPT_ID     =  0,
  BASE_PLBK_PROFILE_ID      =  1,
  BASE_ANC_PROFILE_ID       =  2,
  BASE_PSAP_PROFILE_ID      =  3,
  BASE_NS_PROFILE_ID        =  4,
  MAX_NUM_SCRIPT_DATA       
} ControlMgrScriptData;
typedef enum {
  EVENT_PAR_ANC_STATE          = 27,
  EVENT_PAR_AF_STATE           = 28,
  EVENT_PAR_ENC_STATE          = 31,
  PAR_INDEX_EVENT_ARG0         = 32
} ParProcessIndex;

typedef enum {
  EVENT_PAR_ANC_FF_CALIB_GAIN           = PAR_INDEX_EVENT_ARG0 + 21,
  EVENT_PAR_ANC_FB_CALIB_GAIN           = PAR_INDEX_EVENT_ARG0 + 22,
  EVENT_PAR_ANC_PLBK_CANC_CALIB_GAIN    = PAR_INDEX_EVENT_ARG0 + 23,
  EVENT_PAR_AMB_INCL_CALIB_GAIN         = PAR_INDEX_EVENT_ARG0 + 24,
  EVENT_PAR_PLBK_CALIB_GAIN             = PAR_INDEX_EVENT_ARG0 + 25,
  EVENT_PAR_PSAP_CALIB_GAIN             = PAR_INDEX_EVENT_ARG0 + 26,
  EVENT_PAR_EAR_ROLE                    = PAR_INDEX_EVENT_ARG0 + 27,
  EVENT_PAR_CUSTOM_0                    = PAR_INDEX_EVENT_ARG0 + 34,
  EVENT_PAR_CUSTOM_1                    = PAR_INDEX_EVENT_ARG0 + 35,
  EVENT_PAR_CUSTOM_2                    = PAR_INDEX_EVENT_ARG0 + 36,
  EVENT_PAR_CUSTOM_3                    = PAR_INDEX_EVENT_ARG0 + 37,
  EVENT_PAR_KEY_FUNC_STATE              = PAR_INDEX_EVENT_ARG0 + 38,
  EVENT_PAR_EXT_ECHO_REFERENCE          = PAR_INDEX_EVENT_ARG0 + 39,
  EVENT_PAR_TRIG_DETECTED_INDEX         = PAR_INDEX_EVENT_ARG0 + 40,
  EVENT_PAR_ANC_RIGHT_FF_CALIB_GAIN     = PAR_INDEX_EVENT_ARG0 + 75,
  EVENT_PAR_ANC_RIGHT_FB_CALIB_GAIN     = PAR_INDEX_EVENT_ARG0 + 76,
  EVENT_PAR_ANC_RIGHT_PLBK_CANC_CALIB_GAIN = PAR_INDEX_EVENT_ARG0 + 77,
  EVENT_PAR_AMB_RIGHT_INCL_CALIB_GAIN   = PAR_INDEX_EVENT_ARG0 + 78,
  EVENT_PAR_PLBK_RIGHT_CALIB_GAIN       = PAR_INDEX_EVENT_ARG0 + 79,
  EVENT_PAR_PSAP_RIGHT_CALIB_GAIN       = PAR_INDEX_EVENT_ARG0 + 80,
  EVENT_PAR_SIDETONE_ENABLE             = PAR_INDEX_EVENT_ARG0 + 81,
} EventParIndex;
typedef enum {
  EVENT_SAMPLE_RATE_CHANGE              = 1 << 0,    //used by software caf 
  EVENT_USB_RECORD_STARTSTOP            = 1 << 1,
  EVENT_CADENCE_END                     = 1 << 2,
  EVENT_VOLUME_CHANGE_MUTE_USB_RECORD   = 1 << 3,
  EVENT_VOLUME_CHANGE_USB_RECORD        = 1 << 4,
  EVENT_USB_PLAYBACK_STARTSTOP          = 1 << 5,
  EVENT_FAST_EXEC_END                   = 1 << 6,
  EVENT_VOLUME_CHANGE_USB_PLAYBACK      = 1 << 7,
  EVENT_TRIG_DETECTED                   = 1 << 8,
  EVENT_CMND_DETECTED                   = 1 << 9,
  EVENT_LP_TRIG_DETECTED                = 1 << 10,
  EVENT_TRIG_VAD_HANGOVER               = 1 << 11,
  EVENT_AVAD_CONTROL                    = 1 << 12,
  EVENT_CUSTOM_0                        = 1 << 13,
  EVENT_CUSTOM_1                        = 1 << 14,
  EVENT_CUSTOM_2                        = 1 << 15,
  EVENT_CUSTOM_3                        = 1 << 16,
  EVENT_UART_TX_RECD_STARTSTOP          = 1 << 17,
  EVENT_SPIM_TX_RECD_STARTSTOP          = 1 << 18,
  EVENT_ANC_CONTROL                     = 1 << 19,
  EVENT_ENC_CONTROL                     = 1 << 20,
  EVENT_VOICE_PROMPT_CONTROL            = 1 << 21,
  EVENT_AF_CONTROL                      = 1 << 22,
  EVENT_LP_TRIG_CONTROL                 = 1 << 23,
  EVENT_KEY_DETECTED                    = 1 << 24,
  EVENT_UART_RX_PLBK_STARTSTOP          = 1 << 25,

  EVENT_HPM_CONTROL                     = 1 << 27,
} EventId;

#ifdef __cplusplus
}
#endif

