#define PRINT_LEVEL_SET ASSERT_LEVEL_ERR
#include "mcu.h"
#include "fileSystem.h"

int readFromFileSystem(uint32_t fileId, uint8_t* buf, uint16_t length)
{
    sfs_file_info_t info;
    int err = 0;
    uint8_t* fileName = (uint8_t*)&fileId;

    if(length %4 != 0)
    {
        Print_ERR("Error: Sfs read length has to be a multiple of 4\n");
        return err;
    }

    memset(&info, 0, sizeof(sfs_file_info_t));
    err = sfs_fopen(fileId, &info);
    if (err != 0)
    {
        Print_ERR("Error: Sfs open error:%d\n", err);
        return err;
    }

    err = sfs_fread(&info, buf, length);
    if (err != length)
    {
        Print_ERR("Error: Sfs read error:%d\n", err);
        return err;
    }

    err = sfs_fclose(&info);
    if (err != 0)
    {
        Print_ERR("Error: Sfs close error:%d\n", err);
        return err;
    }

    return err;
}