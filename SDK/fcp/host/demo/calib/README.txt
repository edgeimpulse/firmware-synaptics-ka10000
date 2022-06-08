CALIBRATION DEMO CODE
=====================

Description:
-----------
This demo code contains methods to call Calibration API for AS33970 via either I2C or using CX31993 
or CY7C68013A (Windows PC only) for USB to I2C conversion.
This host demo code can be compiled both for the Windows PC as well as on a linux host (ARM/Intel).

Usage:
=====
  calib_demo.exe <Options> <Commands Arguments>
  Options:
---------
  -h:  Help
  -p <HEX>: PID of the device to connect to via USB-HID
  -v <HEX>: VID of the device to connect to via USB-HID
  
  
Build: 
=====
(1) When the Host is a Windows PC:
    =============================
Double click on calib_demo.sln to open up the Visual Studio project using Visual Studio 17.
Select the Soluction Configuration as Release_I2C or Release_I2SF3 depending on whether 
AS33970 is connected to the Windows PC via CY7C68013A or CX31993 respectively.
Compile it by pressing F7. The executable is built in <Configuration>/calib_demo.exe

To control AS33970 via I2C (from USB->CY7C68013A->I2C), it has to be compiled with Release_I2C Configuration 
To control AS33970 via I2C (from USB->CX31993->I2C), it has to be compiled with Release_I2SF3 Configuration


(2) When the Host is a Linux x86 PC:
    ===============================
Host can connect to AS33970 from the LInux X86 PC via USB using libusb driver. 
For libusb please refer to https://libusb.info/
And for USB control from the Linux Driver, the host demo code utilizes the
libudev-dev and libusb-1.0 libraries (which can be installed using 
sudo apt-get install libudev-dev
sudo apt-get install libusb-1.0.0-dev   )

1) Compile the Host API libraries 

  a. Build Inteface Level API library - libusb_interface.a.
     $ cd host/api_src/interface_api/usb_interface/libusb
     $ make
     After building it will located in the directory host/api/interface_api/usb_interface/lib/linux_x86
     
  b. Build Mid-Level API library - libsendcmd_i2c-f3.a (for USB->CX31993->I2C)
     $ cd host/api_src/mid_level_api/sendcmd_i2c-f3
     $ make 
     After building it will located in the directory host/api/mid_level_api/sendcmd/lib/linux_x86

  c. Build Top-level DSP Control and Status API library - libdsp_ctrl_status.a
     $ cd host/api_src/top_level_api/dsp_ctrl_status_api
     $ make 
     After building it will located in the directory host/api/top_level_api/dsp_ctrl_status_api/lib/linux_x86

2) Compile the Calibration Demo Application
  $ cd host/demo/calib 
  $ make clean
  $ make EXTRA_CPPFLAGS=-DENABLE_I2CF3_CONTROL
