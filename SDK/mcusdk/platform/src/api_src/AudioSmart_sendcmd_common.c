/*
 * This is basic function by which command send out
 */
#include "AudioSmart_sendcmd_common.h"


#if 1 //host code will use this file, so always build it 

int32_t AudioSmart_sendcmd(Command *cmd, uint32_t mgr_module_id, uint32_t command_id, uint32_t num_32b_words, ...)
{
#if 1  
  va_list  args;
  uint32_t n;

  va_start(args, num_32b_words);

  // at least two words of header
  if (num_32b_words > MAX_COMMAND_SIZE)
  {
    va_end(args);
    return (SENDCMD_RC_INVALID_ARG_NUM);
  }
  cmd->head.num_32b_words = (command_id&CMD_GET(0)) ? MAX_COMMAND_SIZE : num_32b_words;
  cmd->head.command_id = command_id;
  cmd->head.reply = 0;
  cmd->head.mgr_module_id = mgr_module_id;

  for (n = 0; n < num_32b_words; n++)
  {
    cmd->data[n] = va_arg(args, int32_t);
  }
  va_end(args);
#endif
  return AudioSmart_mid_sendcmd((uint32_t *)cmd);
}

int32_t AudioSmart_sendcmd_sync(Command *cmd) {
  return AudioSmart_mid_sendcmd((uint32_t *)cmd);
}

int32_t AudioSmart_sendcmd_async(Command *cmd, sendcmd_cb cb_func){
  if (cb_func != NULL) {
    return AudioSmart_mid_sendcmd_async((uint32_t *)cmd, CONFIG_DEFAULT_WAIT_TIMEOUT, cb_func);
  }
  else {
    return AudioSmart_mid_sendcmd_async((uint32_t *)cmd, 0, cb_func);
  }
}



#endif





