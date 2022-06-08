Thin Client
===========

Description:
-----------
This tool that translates golem commands received via TCP/IP interface and sends
the appropriate control commands to Synaptics AudioSmart DSP Codec 
chip


-----------
Usage:
=====
  ./caf_thin_client -h 
  Options:
  -------
  -h:  Help  

Note: When this caf_thin_client is executed, the TCP-IP IP and port to connect to
      it are displayed on the console.


Build: 
====
Building caf_thin_client from Linux host
====================================
(Tested on Raspberry Pi with kernel 4.4.19)

Linux host can connect to Synaptics AudioSmart DSP Codec via I2C using standard i2c driver.
And for USB control from the Linux Driver, the host demo code utilizes the
libudev-dev and libusb-1.0 libraries (which for Raspbian can be installed using 
sudo apt-get install libudev-dev
sudo apt-get install libusb-1.0.0-dev)

Transfer the entire host_demo_code directory over to the Raspberry pi 

Compiling:
---------
make clean; make EXTRA_CPPFLAGS=<1st Options> EXTRA_CPPFLAGS+=<2nd Option> 

Options:
-------
For controlling Synaptics AudioSmart DSP Codec using Standard I2C control   : -DENABLE_I2C_CONTROL
For controlling Synaptics AudioSmart DSP Codec using USB control            : -DENABLE_USB_CONTROL


Execute to connect via I2C interface:
------------------------------------
1) Compile the host libs 
#build libi2c_interface.a, after build it will located in the directory host/api/interface_api/lib
cd host/api_src/interface_api/i2c_interface/i2c_dev
make 
#build libsendcmd_i2c.a ,after build it will located in the directory host/api/mid_level_api/sendcmd/lib
cd host/api_src/mid_level_api/sendcmd_i2c
make 

2) Compile the application ,after build, you can find the build out in the directory host/application/api_caf_thin_client/linux_rpi/I2C
cd host/application/api_caf_thin_client 
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

2)Compile the host application ,after build, you can find the build out in the directory host/application/api_caf_thin_client/linux_rpi/USB 
cd host/application/api_caf_thin_client 
make clean
make EXTRA_CPPFLAGS=-DENABLE_USB_CONTROL

---------------------------






