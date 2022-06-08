i2c_flash
==============

Description:
=====

An Windows/Linux based tool can download Tahiti FW at boot time via I2C.
=====
Usage:
i2c_flash_f3 valid option
---------------------------
  -h                :  this help information.
  -s [path/*.sfs]   :  specify the sfs image.
  -b [path/*.bin]   :  specify the bootloader image.
  -c                :  simple console output  
  -e                :  erase flash device before flashing - parameter usage 0 = invalidate special locations only, 1 = full erase, 2 = chip erase
  -l                :  set loading offset location in the flash (0, 1, 2, 3).
  -n                :  no lock the device after flashing
  -k                :  keep cmd window alive after flashing
  -d                :  set device download mode, parameter usage 0 = erase header mode, 1 = DFU mode, default use DFU mode
  -rate             :  set F3 I2C rate
  -v                :  specify use the vendor id of device, work with -p parameter, default is 0x06CB
  -p                :  specify use the product id of device, work with -v parameter, default is 0x1494
  -g                :  get firmware version after download
---------------------------

Building i2c_flash
---------------------------
(1) When the Host is a Windows PC:
=============================
Double click on i2c_flash.sln with VS17 to open up the Visual Studio project.
Select the Soluction Configuration as Release or Release_F3 or Release_BT_SPP , Compile it by pressing F7.

Execute to connect via SEINA I2C interface:
------------------------------------
To control Synaptics AudioSmart DSP Codec via I2C (actually from USB->Siena->I2C), it has to be compiled with Soluction Configuration as Release
Depends on libi2c_siena.lib and libi2c_flash_api.lib
1. Open host/api_src/interface_api/i2c_interface/i2c_siena/buildwin/libi2c_siena.sln, build Configuration "Release" to build libi2c_siena.lib;
2. Open host/api_src/top_level_api/i2c_flash_api/buildwin/libi2c_flash_api.sln, build Configuration "Release" to build libi2c_flash_api.lib;
3. Open host/demo/i2c_flash/i2c_flash.sln, build Configuration "Release" to build i2c_flash.exe.

Execute to connect via Freeman3 I2C interface:
------------------------------------
To control Synaptics AudioSmart DSP Codec via I2C (actually from USB->F3->I2C), it has to be compiled with Soluction Configuration as Release_F3
Depends on libi2c_f3.lib and libi2c_flash_api.lib
1. Open host/api_src/interface_api/i2c_interface/i2c_f3/buildwin/libi2c_f3.sln, build Configuration "Release" to build libi2c_f3.lib;
2. Open host/api_src/top_level_api/i2c_flash_api/buildwin/libi2c_flash_api.sln, build Configuration "Release_F3" to build libi2c_flash_api.lib;
3. Open host/demo/i2c_flash/i2c_flash.sln, build Configuration "Release_F3" to build i2c_flash_f3.exe.

Execute to connect via Bluetooth SPP interface:
------------------------------------
To control Synaptics AudioSmart DSP Codec via Bluetooth SPP, it has to be compiled with Soluction Configuration as Release_BT_SPP
Depends on libi2c_bt_spp.lib and libi2c_flash_api.lib
1. Open host/api_src/interface_api/i2c_interface/i2c_bt_spp/buildwin/libi2c_bt_spp.sln, build Configuration "Release" to build libi2c_bt_spp.lib;
2. Open host/api_src/top_level_api/i2c_flash_api/buildwin/libi2c_flash_api.sln, build Configuration "Release_BT_SPP" to build libi2c_flash_api.lib;
3. Open host/demo/i2c_flash/i2c_flash.sln, build Configuration "Release_BT_SPP" to build i2c_flash_bt_spp.exe.

(2) When the Host is a Linux x86 PC
=====================================
(Tested on Ubuntu 20.04)
Host can connect device from the Linux X86 PC via USB using Libusb driver.
For libusb please refer to https://libusb.info/
And for USB control from the Linux Driver, the host demo code utilizes the
libudev-dev and libusb-1.0 libraries (which for Raspbian can be installed using 
sudo apt-get install g++
sudo apt-get install libudev-dev
sudo apt-get install libusb-1.0.0-dev   )

After all the packages are installed OK, compile with below command(Only tested Freeman3 interface so far):
Build libi2c_f3.lib: 
    cd host/api_src/interface_api/i2c_interface/i2c_f3/ 
    make 
Build libi2c_flash.lib:
    cd host/api_src/top_level_api/i2c_flash_api/
    make EXTRA_CPPFLAGS=-DINTERFACE_I2C_F3
Build i2c_flash_f3:
    cd host/demo/i2c_flash/
    make EXTRA_CPPFLAGS=-DINTERFACE_I2C_F3

Running i2c_flash
---------------------------

1. Setup connection between host and device successfully.
2. copy *.img (FW image) to the current work directory and rename it partition_flash.img. The following instructions assume that the work directory
   is where the executable are located. 
3. On Windows, open a windows console and run i2c_flash_f3.exe. it will download partition_flash.img and iflash.bin.
    Command prompt input below command:
        i2c_flash_f3.exe
    The other way is use command option "-s"/"-b" choose image/bootloader file which want download in Windows command console.
    Command prompt input below command:
        i2c_flash_f3.exe -s partition_flash.img -b iflash.bin
   On Linux, open a shell console and run lsusb to check USB driver whether exist or not. If yes, then run: 
        sudo ./i2c_flash_f3 -s partition_flash.img -b iflash.bin
     Use command option "-s"/"-b" choose image/bootloader file which want download.
     (it will download partition_flash.img and iflash.bin to device by default if not typr option -s/-b
     ex:
        sudo ./i2c_flash_f3
     )
4. On both platforms, the program will report its progress and a success/fail result.
5. On Linux, the default library libi2c_flash.lib is based on Linux. If it is not compatiable with customer's target platform. Please
Customer share the target toolchain to Conexant software engineer and we will generate the library for target platform.
6. Normally, The tool with Freeman3 interface have two ways to go download mode, one is DFU mode, another on is software mode, erase flash header and soft reset.
   DFU mode require GPIO toggel reset pin to device to go download mode.
   Soft mode require alive firmware in board, can receive command properly.
   default using DFU mode.
   If would like to use soft ware mode, use option "-d" with parameter 0 ("-d 0"), as below:
   i2c_flash_f3.exe -d 0 -s partition_flash.img



