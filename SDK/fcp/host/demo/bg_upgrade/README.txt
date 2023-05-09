BG_UPGRADE
==============

Description:
=====

An Windows/Linux RPi based tool can download FW in the background (not at boot time) through i2c interface or usb interface  

-----------
Usage:
=====
  bg_upgrade.exe 
  Options:
  -------
  -h:  Help  

Build: 
=====

(1) When the Host is a Windows PC:
=============================

Double click on bg_upgrade.sln with VS17 to open up the Visual Studio project.
Select the Soluction Configuration as Release_I2C or Release_USB , Compile it by pressing F7.

Execute to connect via I2C interface:
------------------------------------
To control Synaptics AudioSmart DSP Codec via I2C (actually from USB->Siena->I2C), it has to be compiled with Soluction Configuration as Release_I2C

Execute to connect via USB interface:
------------------------------------
To control Synaptics AudioSmart DSP Codec via USB (using windows hid library), it has to be compiled with Soluction Configuration as Release_USB


(2) When the Host is a Raspberry Pi 
=====================================

(Tested on Raspberry Pi with kernel 4.4.19)
Host can connect to CX2092x from the raspberry pi  via I2C using i2c-dev driver or via USB using Libusb driver
For libusb please refer to https://libusb.info/
And for USB control from the Linux Driver, the host demo code utilizes the
libudev-dev and libusb-1.0 libraries (which for Raspbian can be installed using 
sudo apt-get install libudev-dev
sudo apt-get install libusb-1.0.0-dev   )

Execute to connect via I2C interface:
------------------------------------
1) Compile the host libs 
#build libi2c_interface.a, after build it will located in the directory host/api/interface_api/lib
cd host/api_src/interface_api/i2c_interface/i2c_dev
make 
#build libsendcmd_i2c.a ,after build it will located in the directory host/api/mid_level_api/sendcmd/lib
cd host/api_src/mid_level_api/sendcmd_i2c
make 
#pre-compiled libbg_upgrade.a is provided by Synaptics, located in the directory /host/api/top_level_api/bg_upgrade_api/lib/linux_rpi


2) Compile the host application
cd host/demo/bg_upgrade 
make clean
make EXTRA_CPPFLAGS=-DENABLE_I2C_CONTROL
---------------------------

Execute to connect via USB interface:
------------------------------------
1) Compile the host libs 
#build libusb_interface.a , after build it will located in the directory host/api/interface_api/lib
cd host/api_src/interface_api/usb_interface/libusb
make 
#build libsendcmd_usb.a  ,after build it will located in the directory host/api/mid_level_api/sendcmd/lib
cd host/api_src/mid_level_api/sendcmd_usb
make 
#pre-compiled libbg_upgrade.a is provided by Synaptics, located in the directory /host/api/top_level_api/bg_upgrade_api/lib/linux_rpi


2)Compile the host application
cd host/demo/bg_upgrade 
make clean
make EXTRA_CPPFLAGS=-DENABLE_USB_CONTROL
---------------------------



(3)When the Host is a Linux x86 PC
=====================================
(Tested on Ubuntu 16.04.1)
Host can connect to CX2092x from the LInux X86 PC via USB using Libusb driver.(Please note I2C interfce can't be support on Linux PC) 
For libusb please refer to https://libusb.info/
And for USB control from the Linux Driver, the host demo code utilizes the
libudev-dev and libusb-1.0 libraries (which for Raspbian can be installed using 
sudo apt-get install libudev-dev
sudo apt-get install libusb-1.0.0-dev   )

Execute to connect via USB interface:
------------------------------------
1) Compile the host libs 
#build libusb_interface.a , after build it will located in the directory host/api/interface_api/lib
cd host/api_src/interface_api/usb_interface/libusb
make 
#build libsendcmd_usb.a  ,after build it will located in the directory host/api/mid_level_api/sendcmd/lib
cd host/api_src/mid_level_api/sendcmd_usb
make 
#pre-compiled libbg_upgrade.a is provided by Synaptics, located in the directory /host/api/top_level_api/bg_upgrade_api/lib/linux_x86


2)Compile the host application
cd host/demo/bg_upgrade 
make clean
make EXTRA_CPPFLAGS=-DENABLE_USB_CONTROL
---------------------------

(4)When the Host is an Andorid Device 
===================================== 
User can use standalone toolchain or ndk-build to cross-build our host code.
For creating Standalone Toolchain, please refer to android official webpage  https://developer.android.com/ndk/guides/standalone_toolchain .
The NDK provides the make_standalone_toolchain.py script to allow you to perform a customized toolchain installation from the command line.
An example of the use of this script appears below:
$NDK/build/tools/make_standalone_toolchain.py \
    --arch arm --api 21 --install-dir /tmp/hdc_toolchains/
Assume you had put the standalone toolchain located in  /tmp/hdc_toolchains/arm-linux-androideabi-4.9/
if you put the toolchain to any customized folder, then you need to update path of CROSS_COMPILE in the makefile: host/etc/make_host_code.base, host code using VARIANT to distinguish arm and arm64 
if arch is arm ,then VARIANT=android_armv7-a , if arch is arm64 , then VARIANT=android64_armv8-a 
Host can connect to CX2092x from the andorid device via I2C using i2c-dev driver or via USB using Libusb driver
And for USB control from the Linux Driver, the host demo code utilizes the libusb libraries, For libusb please refer to https://libusb.info/ for more 

Execute to connect via I2C interface:
------------------------------------
1) Compile the host libs 
#build libi2c_interface.a, after build it will located in the directory host/api/interface_api/lib
cd host/api_src/interface_api/i2c_interface/i2c_dev
make VARIANT=android_armv7-a
#build libsendcmd_i2c.a ,after build it will located in the directory host/api/mid_level_api/sendcmd/lib
cd host/api_src/mid_level_api/sendcmd_i2c
make VARIANT=android_armv7-a
#pre-compiled libbg_upgrade.a is provided by Synaptics, located in the directory /host/api/top_level_api/bg_upgrade_api/lib/android_armv7-a


2) Compile the host application
cd host/demo/bg_upgrade 
make VARIANT=android_armv7-a clean
make VARIANT=android_armv7-a EXTRA_CPPFLAGS=-DENABLE_I2C_CONTROL
---------------------------

Execute to connect via USB interface:
------------------------------------
1) Compile the host libs 
#build libusb_interface.a , after build it will located in the directory host/api/interface_api/lib
cd host/api_src/interface_api/usb_interface/libusb
make VARIANT=android_armv7-a
#build libsendcmd_usb.a  ,after build it will located in the directory host/api/mid_level_api/sendcmd/lib
cd host/api_src/mid_level_api/sendcmd_usb
make VARIANT=android_armv7-a
#pre-compiled libbg_upgrade.a is provided by Synaptics, located in the directory /host/api/top_level_api/bg_upgrade_api/lib/android_armv7-a


2)Compile the host application
cd host/demo/bg_upgrade 
make VARIANT=android_armv7-a clean
make VARIANT=android_armv7-a EXTRA_CPPFLAGS=-DENABLE_USB_CONTROL
---------------------------



(4)When Cross-compile
===================================== 
User can cross compile the host code by setting up cross compile toolchain, and add a new variant. 
User can refer the Makefile_host_code.base for more ,let us take VARIANT=linux_armv7-a as example, user  need to define the CROSS_COMPILE and CPPFLAGS  (show as below), after that user can use make VARIANT=linux_armv7-a to cross-compile the host code  

################################################################################
## Specific for each VARIANT.
################################################################################

ifeq ($(VARIANT), linux_armv7-a)
ifeq ($(IS_CYGWIN),"Y")
  CROSS_COMPILE           ?= /tmp/hdc_toolchains/gcc-linaro-4.9.4-2017.01-i686-mingw32_arm-linux-gnueabi/bin/arm-linux-gnueabi-
else
  CROSS_COMPILE           ?= /tmp/hdc_toolchains/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabi/bin/arm-linux-gnueabi-
endif
  CPPFLAGS                += -march=armv7-a
endif

------------------------------------












