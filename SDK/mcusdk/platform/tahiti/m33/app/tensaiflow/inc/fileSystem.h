#include "sfs_drv.h"

#define ALIGN_UP_TO_4(x) (((x) + 3) & ~0x03)
#define ALIGN_DOWN_TO_4(x) ((x) & ~0x03)

#define FILE_ID2(a,b) a ## b
#define FILE_ID(x) FILE_ID2(x,_fileId)

int readFromFileSystem(uint32_t fileId, uint8_t* buf, uint16_t length);