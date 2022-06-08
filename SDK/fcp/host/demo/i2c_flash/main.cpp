/****************************************************************************************
*****************************************************************************************
***                                                                                   ***
***                                 Copyright (c) 2013-2018                           ***
***                                                                                   ***
***                                Conexant Systems, Inc.                             ***
***                                                                                   ***
***                                 All Rights Reserved                               ***
***                                                                                   ***
***                                    CONFIDENTIAL                                   ***
***                                                                                   ***
***               NO DISSEMINATION OR USE WITHOUT PRIOR WRITTEN PERMISSION            ***
***                                                                                   ***
*****************************************************************************************
**
**  File Name:
**      main.cpp
**
**  Abstract:
**      i2c_flash is a Windows console application to downloade firmware
**      to Tahiti.
**
**  Product Name:
**      Synaptics firmware.
**
********************************************************************************
*****************************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AudioSmart_libi2c_flash.h"
#include "AudioSmart_i2c_interface_api.h"
#ifdef INTERFACE_I2C_F3
#include "AudioSmart_i2c_f3_misc_interface_api.h"
#endif
#ifdef INTERFACE_I2C_BT_SPP
#include "AudioSmart_i2c_bt_spp_misc_interface_api.h"
#endif
#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#include <string>
#define STRSAFE_NO_DEPRECATE //For VS2010 strcpy C4995 warning.
#include <strsafe.h>
#include <conio.h>
#include "CxProgress.h"
#define DUMP printf
extern void set_txt_color(int color);
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#define DUMP(args...) printf(args)
#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))
#define timeGetTime(...) 0
#define set_txt_color(t)
typedef unsigned int DWORD;
typedef unsigned int ULONG;
typedef int BOOL;
#define FALSE 0
#define TRUE 1
#endif

#define I2C_SLAVE_ADDR 0X41
#define I2C_SUB_ADDR_SIZE 2
#define GPIO_RESET_PIN 21

#define COL_B 1
#define COL_R 4
#define COL_G 2
#define COL_I 8
#define COLOR_WARNING (COL_I + COL_R + COL_G)
#define COLOR_ERROR (COL_I + COL_R)
#define COLOR_PASS (COL_G)
#define COLOR_NORMAL (COL_R + COL_G + COL_B)

void ShowResult(bool bIsPass);
#ifdef _WINDOWS
uint16_t String2Number(wchar_t *string);
#else
uint16_t String2Number(char *string);
#endif

static uint8_t i2c_address[] = {
    0x41,
    0x47,
    0x4e,
};

typedef void *HANDLE;

static int g_hot_flash = 0;

#ifdef INTERFACE_I2C_BT_SPP
static uint32_t g_upgrade_primary = 1;
#endif

class CTimeoutTimer
{
public:
    CTimeoutTimer(DWORD dwTimeOut /*ms*/) : m_dwTimeOutValue(dwTimeOut),
                                            m_dwStartTime(0)
    {
    }

    // Specifies the timer out value.
    void SetTimeOutValue(DWORD dwTimeOut /*ms*/)
    {
        m_dwTimeOutValue = dwTimeOut;
    }

    // Resets the elapsed time.
    void Reset()
    {
        m_dwStartTime = timeGetTime();
    }

    // Return TRUE if time is up.
    BOOL IsTimeOut() const
    {
        BOOL bTimeOut = FALSE;
        DWORD dwCurTime = timeGetTime();
        if ((dwCurTime - m_dwStartTime) > m_dwTimeOutValue)
        {
            bTimeOut = TRUE;
        }
        return bTimeOut;
    }

    // Return delta time.
    DWORD GetDeltaTime() const
    {
        return timeGetTime() - m_dwStartTime;
    }

private:
    CTimeoutTimer();
    ULONG m_dwTimeOutValue;
    ULONG m_dwStartTime;
};

void sys_mdelay(unsigned int ms_delay)
{
#ifdef _WINDOWS
    Sleep(ms_delay);
#else
    // TODO : To implement millisecond sleep function here.
    usleep(ms_delay * 1000);
#endif
}

#ifdef _WINDOWS
#define set_gpio_value(gpio, value) \
    do                              \
    {                               \
    } while (0)
#define OpenGpioDevice(grp) (HANDLE *)grp
#define CloseGpioDevice(device) \
    do                          \
    {                           \
    } while (0)
#else
#define MAX_DEV_NAME 256
static char gpio_name[MAX_DEV_NAME];
//
// GPIO functions
//
/* export the gpio to user mode space */
static int export_gpio(int gpio)
{
    int err = 0;
    int fd;
    int wr_len;

    /* check if gpio is already exist or not */
    sprintf(gpio_name, "/sys/class/gpio/gpio%d/value", gpio);

    fd = open(gpio_name, O_RDONLY);
    if (fd >= 0)
    {
        /* gpio device has already exported */
        close(fd);
        return 0;
    }

    /* gpio device is no exported, export it */
    strcpy(gpio_name, "/sys/class/gpio/export");

    fd = open(gpio_name, O_WRONLY);

    if (fd < 0)
    {
        fprintf(stderr, "failed to open device: %s\n", gpio_name);
        return -ENOENT;
    }

    sprintf(gpio_name, "%d", gpio);
    wr_len = strlen(gpio_name);

    if (write(fd, gpio_name, wr_len) != wr_len)
    {
        fprintf(stderr, "failed to export gpio: %d\n", gpio);
        return -EACCES;
    }
    close(fd);

    return err;
}

static int free_gpio(int gpio)
{
    int err = 0;
    int fd;
    int wr_len;

    /* check if gpio exist */
    sprintf(gpio_name, "/sys/class/gpio/gpio%d/value", gpio);

    fd = open(gpio_name, O_RDONLY);
    if (fd < 0)
    {
        /* gpio device doesn't exist */
        return -ENOENT;
    }
    close(fd);

    /* gpio device is no exported, export it */
    strcpy(gpio_name, "/sys/class/gpio/unexport");

    fd = open(gpio_name, O_WRONLY);

    if (fd < 0)
    {
        fprintf(stderr, "failed to open device: %s\n", gpio_name);
        return -ENOENT;
    }

    sprintf(gpio_name, "%d", gpio);
    wr_len = strlen(gpio_name);

    if (write(fd, gpio_name, wr_len) != wr_len)
    {
        fprintf(stderr, "failed to free gpio: %d\n", gpio);
        return -EACCES;
    }
    close(fd);

    return err;
}

static int set_gpio_dir(int gpio)
{
    int err = 0;
    int fd;
    int wr_len;
    char str_dir[4];

    err = export_gpio(gpio);

    if (err)
        return err;

    sprintf(gpio_name, "/sys/class/gpio/gpio%d/direction", gpio);
    sys_mdelay(50);
    fd = open(gpio_name, O_WRONLY);

    if (fd < 0)
    {
        fprintf(stderr, "failed to open device: %s\n", gpio_name);
        err = -ENOENT;
        goto exit;
    }

    strcpy(str_dir, "out");
    wr_len = strlen(str_dir);
    if (write(fd, str_dir, wr_len) != wr_len)
    {
        fprintf(stderr, "Failed to set gpio %s direction to %s \n", gpio_name, str_dir);
        err = -EACCES;
        free_gpio(gpio);
        goto exit;
    }
    printf("set gpio %d direction to %s\n", gpio, str_dir);

exit:
    close(fd);
    return err;
}

static int set_gpio_value(int gpio, int value)
{
    int err = 0;
    int fd;

    if (err)
        return err;

    sprintf(gpio_name, "/sys/class/gpio/gpio%d/value", gpio);

    fd = open(gpio_name, O_WRONLY);

    if (fd < 0)
    {
        fprintf(stderr, "failed to open device: %s\n", gpio_name);
        err = -ENOENT;
        goto exit;
    }

    if (write(fd, value ? "1" : "0", 1) != 1)
    {
        fprintf(stderr, "Failed to set gpio %s to %d\n", gpio_name, value);
        err = -EACCES;
        goto exit;
    }
    //printf("set gpio %d to %d\n",gpio,value);

exit:
    close(fd);
    return err;
}

static HANDLE OpenGpioDevice(int gpio_reset_pin)
{
    if (set_gpio_dir(gpio_reset_pin) < 0)
    {
        return NULL;
    }
    return ((HANDLE *)gpio_reset_pin);
}

static void CloseGpioDevice(HANDLE device)
{
    free_gpio((long)device);
}
#endif
/*
 *  To control reset pin
 */
int SetResetPin(HANDLE i2c_dev, int bHigh)
{
    int ret = 0;
#ifdef _WINDOWS
#if INTERFACE_I2C_SIENA
    ret = AudioSmart_set_gpio_reset(bHigh);
#elif INTERFACE_I2C_F3
    //TD::
#elif INTERFACE_I2C_BT_SPP
    //operation TBD
#endif
    if (ret != CXERR_NOERR)
    {
        DUMP("I2C NACK\n");
        return ret;
    }
#else
    /*
// LINUX
    int fd = (size_t) i2c_dev;
    ret = ioctl(fd, CXDBG_IOCTL_RESET_PIN, &bHigh);
    ret = ret? -CXERR_FAILED : CXERR_NOERR;
*/
    ret = set_gpio_value(GPIO_RESET_PIN, bHigh);
    ret = ret ? -CXERR_FAILED : CXERR_NOERR;
#endif

    return ret;
}

/*
* Return the size of the specified file, in bytes.
*/
long file_length(FILE *f)
{
    long pos;
    long end;

    pos = ftell(f);
    fseek(f, 0, SEEK_END);
    end = ftell(f);
    fseek(f, pos, SEEK_SET);

    return end;
}
#ifdef _WINDOWS
int find_sfs_file(TCHAR *sfs_path, unsigned int sfs_path_len)
{
#define SFS_EXT_NAME _T("_flash.img")
    TCHAR szDir[MAX_PATH];
    const size_t cchExtName = (sizeof(SFS_EXT_NAME) / sizeof(TCHAR)) - 1;

    WIN32_FIND_DATA FindData;
    HANDLE hFind;

    GetCurrentDirectory(MAX_PATH, szDir);
    StringCchCat(szDir, sfs_path_len, TEXT("\\*"));

    sfs_path;
    sfs_path_len;

    hFind = FindFirstFile(szDir, &FindData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        return -1;
    }

    do
    {
        if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            size_t cchFileName = _tcslen(FindData.cFileName);
            TCHAR *szExt = FindData.cFileName + cchFileName - cchExtName;

            if (0 == _tcsnicmp(szExt, SFS_EXT_NAME, cchExtName))
            {
                StringCchCopy(sfs_path, sfs_path_len, FindData.cFileName);
            }
        }
    } while (FindNextFile(hFind, &FindData) != 0);

    FindClose(hFind);
    return 0;
}
#endif

/*
* Main enty point of program, it
*
* PARAMETERS
*      None
*
* RETURN
*      Return 0 if the operation is successful. Otherwise, return error code.
*/
#ifdef _WINDOWS
int _tmain(int argc, TCHAR *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    HANDLE i2c_dev = NULL; // Handle of I2C device.
    HANDLE hGpioDevice = NULL;
    unsigned char *boot = NULL;  // Pointer to boot loader data
    unsigned char *image = NULL; // Pointer to image data.
    unsigned long num_boot = 0;  // the size of boot loader data in byte
    unsigned long num_image = 0; // the size of image data in byte
    int ErrNo = CXERR_NOERR;
    FILE *hboot_boader = NULL;
    FILE *himage = NULL;
    void *buf = NULL;
    char sfs_path[260];
    char bl_path[260];
    int i2c_det_retry_cnt = 0;
    int flash_offset = 0;
    int erase_option = 0;
    char *err_str = NULL;

    CTimeoutTimer ttimer(0);
    uint32_t download_interval;

    unsigned char i2c_addr = I2C_SLAVE_ADDR;
    bool find_sfs_path = false;
    bool find_bl_path = false;
    bool force_booted = true;
    bool keep_window = false;
    bool download_mode = true;
    static char card_name[20] = "NULL";
    uint16_t vid = 0;
    uint16_t pid = 0;
    uint32_t i2c_rate = 400;

    setbuf(stdout, NULL);
    do
    {
        printf("Synaptics I2C firmware update demo tool\n\n");
        printf("Tahiti project\n\n");

#if defined(_WINDOWS)
        hGpioDevice = OpenGpioDevice(GPIO_RESET_PIN);
        if (hGpioDevice == NULL)
        {
            // GPIO is not available.
            printf("GPIO %d is not present.\n", GPIO_RESET_PIN);
            return -ENODEV;
        }
#endif
//
// Get image file path.
//
#if defined(_WINDOWS)
        TCHAR wszSfsPath[MAX_PATH];
        TCHAR wszBootloaderPath[MAX_PATH];
        TCHAR wszOffsetNumber[MAX_PATH];
#endif
        if (argc > 1)
        { //if has more than one command option, parse command
            for (int i = 1; i < argc; i++)
            {
#if defined(_WINDOWS)

                if (wcscmp(argv[i], L"-s") == 0 && find_sfs_path == false)
                {
                    //command option for add sfs file
                    StringCchCopy(wszSfsPath, MAX_PATH, argv[i + 1]);
                    find_sfs_path = true;
                }
                else if (wcsstr(argv[i], L".sfs") != NULL && find_sfs_path == false)
                {
                    //drag sfs file into i2c flash tool
                    StringCchCopy(wszSfsPath, MAX_PATH, argv[i]);
                    find_sfs_path = true;
                }
                else if (wcscmp(argv[i], L"-b") == 0)
                {
                    //command option for add bootloader file
                    StringCchCopy(wszBootloaderPath, MAX_PATH, argv[i + 1]);
                    WideCharToMultiByte(CP_ACP, 0, wszBootloaderPath, MAX_PATH, bl_path, MAX_PATH, NULL, NULL);
                    find_bl_path = true;
                }
                else if (wcscmp(argv[i], L"-l") == 0)
                {
                    StringCchCopy(wszOffsetNumber, MAX_PATH, argv[i + 1]);
                    // specify loading location in the flash device
                    flash_offset = (int)strtol((const char *)wszOffsetNumber, NULL, 0);
                    if ((flash_offset > 3) || (flash_offset < 0))
                    {
                        printf("Error: -l parameter. Use only 0, 1, 2, or 3");
                        break;
                    }
                    else
                    {
                        set_flash_offset_location(flash_offset);
                    }
                }
                else if (wcscmp(argv[i], L"-n") == 0)
                {
                    set_flag_flash_nolock(true);
                }
                else if (wcscmp(argv[i], L"-e") == 0)
                {
                    StringCchCopy(wszOffsetNumber, MAX_PATH, argv[i + 1]);
                    // specify erase method
                    erase_option = (int)strtol((const char *)wszOffsetNumber, NULL, 0);
                    if ((erase_option > 2) || (erase_option < 0))
                    {
                        printf("Error: -l parameter. Use only 0, 1, 2\n");
                        erase_option = 0;
                        break;
                    }
                    else
                    {
                        set_flag_flash_invalidating(true);
                        switch (erase_option)
                        {
                        // full erase flash device before flashing
                        case 0: // default - erase only special locations
                        case 1: // full erase with sector erase
                        case 2: // chip erase
                            set_option_flash_fullerase(erase_option);
                            break;
                        default:
                            // no such case
                            break;
                        }
                    }
                }
                else if (wcscmp(argv[i], L"-c") == 0)
                {
                    //simple_console_print = true;//for compatibility with Eclipse console
                    SetSimpleConsoleProgress(true);
                }
                else if (wcscmp(argv[i], L"-k") == 0)
                {
                    keep_window = true; //keep cmd window alive after flashing finished
                }
#ifdef INTERFACE_I2C_BT_SPP
                else if (wcscmp(argv[i], L"-pri") == 0)
                {
                    g_upgrade_primary = 1;
                }
                else if (wcscmp(argv[i], L"-sec") == 0)
                {
                    g_upgrade_primary = 0;
                }
#endif
#ifdef INTERFACE_I2C_F3
                else if (wcscmp(argv[i], L"-d") == 0)
                {
                    if ((wcscmp(argv[i + 1], L"0") == 0) || (wcscmp(argv[i + 1], L"1") == 0))
                    {
                        download_mode = String2Number(argv[i + 1]);
                    }
                    else
                    {
                        printf("Error: -d need parameter: 0 or 1\n");
                        download_mode = true;
                        break;
                    }
                }
                else if (wcscmp(argv[i], L"-rate") == 0)
                {
                    i2c_rate = String2Number(argv[i + 1]);
                }
                else if (wcscmp(argv[i], L"-v") == 0)
                {
                    vid = String2Number(argv[i + 1]);
                    printf("Vendor id is:  0x%04x\n", vid);
                }
                else if (wcscmp(argv[i], L"-p") == 0)
                {
                    pid = String2Number(argv[i + 1]);
                    printf("Product id is: 0x%04x\n", pid);
                }
                else if (wcscmp(argv[i], L"-g") == 0)
                {
                    get_fw_ver_after_boot(true);
                }
#endif
                else if (wcscmp(argv[i], L"-h") == 0)
                {
                    printf("Usage:\n");
                    printf("=====\n");
                    printf("  i2c_flash.exe <Options>\n");
                    printf("  Options:\n");
                    printf("---------\n");
                    printf("  -h    :  this help information.\n");
                    printf("  -s    :  specify the sfs image.\n");
                    printf("  -b    :  specify the bootloader image.\n");
                    printf("  -c    :  simple console output.\n");
                    printf("  -e    :  erase flash device before flashing - parameter usage 0 = invalidate special locations only, 1 = full erase, 2 = chip erase\n");
                    printf("  -l    :  set loading offset location in the flash (0, 1, 2, 3).\n");
                    printf("  -n    :  no lock the device after flashing\n");
                    printf("  -k    :  keep cmd window alive after flashing\n");
#ifdef INTERFACE_I2C_F3
                    printf("  -d    :  set device download mode, parameter usage 0 = erase header mode, 1 = DFU mode, default use DFU mode\n");
                    printf("  -rate :  set F3 I2C rate\n");
                    printf("  -v    :  specify the vendor id of device, work with -p parameter, default is 0x06CB\n");
                    printf("  -p    :  specify the product id of device, work with -v parameter, default is 0x1494\n");
                    printf("  -g    :  get firmware version after download\n");
#endif
                    exit(0);
                }
#else
                if (strcmp(argv[i], "-s") == 0)
                {
                    //command option for add sfs file
                    if (strlen(argv[i + 1]) < sizeof(sfs_path))
                    {
                        strcpy(sfs_path, argv[i + 1]);
                        find_sfs_path = true;
                    }
                    else
                    {
                        printf("Error: Too long sfs path parameter.");
                        break;
                    }
                }
                else if (strcmp(argv[i], "-b") == 0)
                {
                    //command option for add bootloader file
                    if (strlen(argv[i + 1]) < sizeof(bl_path))
                    {
                        strcpy(bl_path, argv[i + 1]);
                        find_bl_path = true;
                    }
                    else
                    {
                        printf("Error: Too long bootloader path parameter.");
                        break;
                    }
                }
                else if (strcmp(argv[i], "-l") == 0)
                {
                    // specify loading location in the flash device
                    flash_offset = strtol(argv[i + 1], NULL, 0);
                    if (err_str)
                    {
                        printf("Error: -l parameter.");
                        break;
                    }
                    else
                    {
                        set_flash_offset_location(flash_offset);
                    }
                }
                else if (strcmp(argv[i], "-n") == 0)
                {
                    set_flag_flash_nolock(true);
                }
                else if (strcmp(argv[i], "-e") == 0)
                {
                    // specify erase method
                    erase_option = strtol(argv[i + 1], NULL, 0);
                    if ((erase_option > 2) || (erase_option < 0))
                    {
                        printf("Error: -l parameter. Use only 0, 1, 2\n");
                        erase_option = 0;
                        break;
                    }
                    else
                    {
                        set_flag_flash_invalidating(true);
                        switch (erase_option)
                        {
                        // full erase flash device before flashing
                        case 0: // default - erase only special locations
                        case 1: // full erase with sector erase
                        case 2: // chip erase
                            set_option_flash_fullerase(erase_option);
                            break;
                        default:
                            // no such case
                            break;
                        }
                    }
                }
#ifdef INTERFACE_I2C_F3
                else if (strcmp(argv[i], "-d") == 0)
                { //This option can be extend to other interface, currently only F3 support it.
                    if ((strcmp(argv[i + 1], "0") == 0) || (strcmp(argv[i + 1], "1") == 0))
                    {
                        download_mode = String2Number(argv[i + 1]);
                    }
                    else
                    {
                        printf("Error: -d need parameter: 0 or 1\n");
                        download_mode = true;
                        break;
                    }
                }
                else if (strcmp(argv[i], "-rate") == 0)
                {
                    i2c_rate = String2Number(argv[i + 1]);
                }
                else if (strcmp(argv[i], "-v") == 0)
                {
                    vid = String2Number(argv[i + 1]);
                    printf("Vendor id is:  0x%04x\n", vid);
                }
                else if (strcmp(argv[i], "-p") == 0)
                {
                    pid = String2Number(argv[i + 1]);
                    printf("Product id is: 0x%04x\n", pid);
                }
                else if (strcmp(argv[i], "-g") == 0)
                {
                    get_fw_ver_after_boot(true);
                }
#endif
                else if (strcmp(argv[i], "-h") == 0)
                {
                    printf("Usage:\n");
                    printf("=====\n");
                    printf("  i2c_flash.exe <Options>\n");
                    printf("  Options:\n");
                    printf("---------\n");
                    printf("  -h    :  this help information.\n");
                    printf("  -s    :  specify the sfs image.\n");
                    printf("  -b    :  specify the bootloader image.\n");
                    printf("  -e    :  erase flash device before flashing\n");
                    printf("           parameter 0 = invalidating special locations only, 1 = full erase, 2 = chip erase\n");
                    printf("  -l    :  set loading offset location in the flash (0, 1, 2, 3).\n");
                    printf("  -n    :  no lock the device after flashing\n");
#ifdef INTERFACE_I2C_F3
                    printf("  -d    :  set device download mode, parameter usage 0 = Erase header mode, 1 = DFU mode, default use DFU mode\n");
                    printf("  -rate :  set F3 I2C rate\n");
                    printf("  -v    :  specify the vendor id of device, work with -p parameter, default is 0x06CB\n");
                    printf("  -p    :  specify the product id of device, work with -v parameter, default is 0x1494\n");
                    printf("  -g    :  get firmware version after download\n");
#endif

                    exit(0);
                }
#endif
            }
//didn't specify sfs file, find it from current directory
#if defined(_WINDOWS)
            if (find_sfs_path == false)
                find_sfs_file(wszSfsPath, MAX_PATH);
            WideCharToMultiByte(CP_ACP, 0, wszSfsPath, MAX_PATH, sfs_path, MAX_PATH, NULL, NULL);
#else
            if (find_sfs_path == false)
                strcpy(sfs_path, "partition_flash.img");
#endif
        }
        else
        {
//no specify command option, set to default
#if defined(_WINDOWS)
            //find sfs file from current directory
            find_sfs_file(wszSfsPath, MAX_PATH);
            WideCharToMultiByte(CP_ACP, 0, wszSfsPath, MAX_PATH, sfs_path, MAX_PATH, NULL, NULL);
#else
            //find sfs file from current directory
            strcpy(sfs_path, "partition_flash.img");
#endif
        }
        //if didn't specify bootloader file
        if (find_bl_path == false)
            strcpy(bl_path, "iflash.bin");

#ifdef INTERFACE_I2C_BT_SPP
        ErrNo = AudioSmart_i2c_bt_spp_interface_init(I2C_SLAVE_ADDR, I2C_SUB_ADDR_SIZE, g_upgrade_primary);
        if (ErrNo != CXERR_NOERR)
        {
            fprintf(stderr, "\n\nERROR: Cannot open I2C BT SPP interface. Error Number: %d\n", ErrNo);
            ErrNo = -CXERR_DEVICE_NOT_RESET;
        }
        printf("upgrade firmware for %s\n", g_upgrade_primary ? "primary" : "secondary");
        if (g_upgrade_primary == 0)
        {
            int32_t ret = AudioSmart_i2c_bt_spp_interface_switch_role_set();
            if (ret != CXERR_NOERR)
            {
                printf("AudioSmart_i2c_bt_spp_interface_switch_role_set  error %d\n", ret);
                return ret;
            }
            sys_mdelay(1000);
            ret = AudioSmart_i2c_bt_spp_interface_switch_role_get();
            if (ret != CXERR_NOERR)
            {
                printf("AudioSmart_i2c_bt_spp_interface_switch_role_get  error %d\n", ret);
                return ret;
            }
        }
#else
#ifdef INTERFACE_I2C_F3
        // Attempt to open i2c device.
        ErrNo = AudioSmart_i2c_f3_interface_init(I2C_SLAVE_ADDR, I2C_SUB_ADDR_SIZE, i2c_rate, (int16_t)vid, (int16_t)pid);
        if (ErrNo != CXERR_NOERR)
        {
            fprintf(stderr, "\n\nERROR: Cannot open I2C interface with F3 device. Error Number: %d\n", ErrNo);
            exit(EXIT_FAILURE);
        }
#else
        // Attempt to open i2c device.
        ErrNo = AudioSmart_i2c_interface_init(I2C_SLAVE_ADDR, I2C_SUB_ADDR_SIZE, (int8_t *)card_name);
        if (ErrNo != CXERR_NOERR)
        {
            fprintf(stderr, "\n\nERROR: Cannot open I2C interface. Error Number: %d\n", ErrNo);
            exit(EXIT_FAILURE);
        }
#endif
#endif // !INTERFACE_I2C_BT_SPP
#if INTERFACE_I2C_F3
        //Disable F3 event handler
        if ((ErrNo = AudioSmart_i2c_interface_f3_event_handler_ctrl(0)) < 0)
            printf("ERR: Fail to disable F3 event handler ERRNo: %d\n", ErrNo);
#endif

        //
        // Load Boot Loader from file to memory.
        //
        hboot_boader = fopen(bl_path, "rb");
        if (hboot_boader)
        {
            num_boot = (unsigned long)file_length(hboot_boader);
            boot = (unsigned char *)malloc(num_boot);
            if (fread(boot, sizeof(char), num_boot, hboot_boader) != num_boot)
            {
                printf("failed to read bootloader file\n");
                break;
            }
            fclose(hboot_boader);
            hboot_boader = NULL;
        }
        else
        {
            printf("Open iflash.bin failed[path:%s].\n", bl_path);
            break;
        }

        //
        // Load image from file to memory.
        //
        himage = fopen(sfs_path, "rb");
        if (himage == NULL)
        {
            printf("Open image file failed[path:%s].\n", sfs_path);
            break;
        }
        num_image = (unsigned long)file_length(himage);
        image = (unsigned char *)malloc(num_image);
        if (fread(image, sizeof(char), num_image, himage) != num_image)
        {
            printf("read bootload file failed.\n");
            break;
        }
        fclose(himage);
        himage = NULL;
#if INTERFACE_I2C_F3
        //Set device to download mode or erase header then reset device are
        //two different way to put device to downlaod mode, it's alternative option
        if (download_mode == true)
        {
            if ((ErrNo = AudioSmart_i2c_interface_set_to_download_mode()) < 0)
            {
                printf("ERR: Fail to enable download mode ERRNo: %d\n", ErrNo);
                break;
            }
            DeviceDownloadModeEn(download_mode);
        }
        else
        {
            //erase flash header information option
            if ((ErrNo = AudioSmart_i2c_interface_hdr_info(force_booted)) < 0)
            {
                printf("ERR: Fail to erase host header info ERRNo: %d\n", ErrNo);
                //break;
            }
        }
#elif INTERFACE_I2C_BT_SPP
        printf("erase header while setting device into download mode stage\n");
#endif

// setup the reset pin callback function.
#if defined(CONFIG_I2C_DISABLE_RESET)
        SetupSetResetPin((void *)i2c_dev, (fn_SetResetPin)NULL);
#else
        SetupSetResetPin((void *)i2c_dev, (fn_SetResetPin)SetResetPin);
#endif

        buf = malloc(GetSizeOfBuffer());

#if defined(I2C_ADDR_AUTO_DETECT) && (!defined INTERFACE_I2C_BT_SPP) //we will reset i2c slave device from bt device stack
    //
    //Scan the I2C chip address
    //
    rescan:
        int i = 0;
        for (i = 0; i < ARRAYSIZE(i2c_address); i++)
        {
#if INTERFACE_I2C_SIENA
            ErrNo = AudioSmart_i2c_interface_read(0x0, (unsigned int *)buf, 1);
#elif INTERFACE_I2C_F3
            ErrNo = AudioSmart_i2c_interface_read(0x0, (unsigned int *)buf, 1);
#endif
            ErrNo = (ErrNo >= 0) ? -CXERR_NOERR : -CXERR_I2C_ERROR; // convert boolean to error code.

            if (ErrNo == 0)
            {
                i2c_addr = i2c_address[i];
                break;
            }
        }
        if (i == ARRAYSIZE(i2c_address))
        {
            if (i2c_det_retry_cnt > 3)
            {
                set_txt_color(COLOR_WARNING);
                printf("Warning! can't detect i2c address, rescan i2c address againusing default address %08x instead!.\n", i2c_addr);
                set_txt_color(COLOR_NORMAL);
            }
            else
            {
                printf("Warning! can't detect i2c address 0x%x, retry !.\n", i2c_addr);

                /* The Hudson't I2C might not work properly if the firmware crash,
                  for this case, we reset the device to make sure the I2C interfac working
                  again. However, the I2C can die again after device load NOT working firmware.
                  So, we have to try i2c after device come out from reset state asap.
                  But here is a very small interval between RESET pin goes HIGH and device come out
                  from reset state, i2c won't working. Therefore we will try 3 times without different
                  sleep interval. Anyway, this is rare case.
                */
                SetResetPin(i2c_dev, 0);
                sys_mdelay(RESET_INTERVAL_MS);
                SetResetPin(i2c_dev, 1);
                /*Since windows is not a realtime os, we knowon know
                when the thread will be executed at the exact interval
                after sleep. So we try different sleep time interval for 3 times.
                */
                if (i2c_det_retry_cnt)
                    sys_mdelay(i2c_det_retry_cnt++);
                goto rescan;
            }
        }
#endif //#if defined(I2C_ADDR_AUTO_DETECT)

        //
        //  Download FW.
        //
        //If the operation completes successfully,the return value is zero. Otherwise,
        //return EERON_* error code. For more information about return code, please refer
        //to cxpump.h file.
        ttimer.Reset();
        // read an option for calculating download mode
        ErrNo = DownloadFW_eFlash(buf, boot, num_boot, image, num_image, i2c_addr, DOWNLOAD_FACTORY, g_hot_flash ? 0 : 1, 0);

        download_interval = ttimer.GetDeltaTime();
        ShowResult(ErrNo == CXERR_NOERR);
#if INTERFACE_I2C_F3
        int DwnErrNo = CXERR_NOERR;
        DwnErrNo = ErrNo;
        //Enable F3 event handler
        if ((ErrNo = AudioSmart_i2c_interface_f3_event_handler_ctrl(1)) < 0)
            printf("ERR: Fail to enable F3 event handler ERRNo: %d\n", ErrNo);
        ErrNo |= DwnErrNo;
#endif
        if (ErrNo == CXERR_NOERR)
        {
            set_txt_color(COLOR_PASS);
#ifdef _WINDOWS
            DUMP("\nFirmware Downloaded Successfully. Total time : %u ms \n", download_interval);
#else
            DUMP("\nFirmware Downloaded Successfully.\n");
#endif
        }
        else
        {
            set_txt_color(COLOR_ERROR);
            DUMP("\nFailed! error code = %d\n\n", ErrNo);
        }
    } while (0);

    // Clean up.
    if (image)
        free(image);
    if (boot)
        free(boot);
    if (hGpioDevice)
        CloseGpioDevice(hGpioDevice);
    if (hboot_boader)
        fclose(hboot_boader);
    if (himage)
        fclose(himage);
    if (buf)
        free(buf);
    AudioSmart_i2c_interface_close();
    set_txt_color(COLOR_NORMAL);
    //DUMP("Press any key to exit\n");
#if defined(_WINDOWS)
    if (keep_window)
        _getch();
#endif
    return ErrNo;
}

static const char *const szPass[] = {
#if defined(_WINDOWS)
    {"  ########     ###     ######   ######  \n"},
    {"  ##     ##   ## ##   ##    ## ##    ## \n"},
    {"  ##     ##  ##   ##  ##       ##       \n"},
    {"  ########  ##     ##  ######   ######  \n"},
    {"  ##        #########       ##       ## \n"},
    {"  ##        ##     ## ##    ## ##    ## \n"},
    {"  ##        ##     ##  ######   ######  \n"}
#endif
};

static const char *const szFail[] = {
#if defined(_WINDOWS)
    {"                          \n"},
    {"   ######   ##   # #      \n"},
    {"   #       #  #  # #      \n"},
    {"   #####  #    # # #      \n"},
    {"   #      ###### # #      \n"},
    {"   #      #    # # #      \n"},
    {"   #      #    # # ###### \n"},
#endif
};

void ShowResult(bool bIsPass)
{
    if (bIsPass)
    {
        set_txt_color(COLOR_PASS);
        for (int i = 0; i < ARRAYSIZE(szPass); i++)
        {
            DUMP("%s", szPass[i]);
        }
    }
    else
    {
        set_txt_color(COLOR_ERROR);
        for (int i = 0; i < ARRAYSIZE(szFail); i++)
        {
            DUMP("%s", szFail[i]);
        }
    }
    set_txt_color(COLOR_NORMAL);
}

#ifdef _WINDOWS
uint16_t String2Number(wchar_t *string)
{
    uint16_t num = 0;
    if (wcsstr(string, L"0x") == NULL)
    {
        num = _wtoi(string);
    }
    else
    {
        num = wcstol(&string[2], NULL, 16);
    }
    return num;
}
#else
uint16_t String2Number(char *string)
{
    uint16_t num = 0;
    if (strstr(string, "0x") == NULL)
    {
        num = atoi(string);
    }
    else
    {
        num = strtol(&string[2], NULL, 16);
    }
    return num;
}
#endif
