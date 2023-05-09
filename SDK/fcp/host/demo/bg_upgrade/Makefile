################################################################################
## needs to be defined by relative way.
################################################################################
HOST_TOP := ../..
CUR_DIR  := $(HOST_TOP)/demo/bg_upgrade

################################################################################
## General
################################################################################
TARGET                  := bg_upgrade
SRCS_CPP                := main.cpp
CPPFLAGS                := -c -fpermissive 

LDFLAGS                 = -lc
LDFLAGS                 += -lbg_upgrade 


ENABLE_I2C_CONTROL     := $(if $(findstring -DENABLE_I2C_CONTROL,    $(EXTRA_CPPFLAGS)),"Y","")
ENABLE_USB_CONTROL     := $(if $(findstring -DENABLE_USB_CONTROL,    $(EXTRA_CPPFLAGS)),"Y","")

ifeq ($(ENABLE_I2C_CONTROL),"")
ifeq ($(ENABLE_USB_CONTROL),"")
$(error Either I2C or USB must be enabled, enable them with EXTRA_CPPFLAGS=-DENABLE_I2C_CONTROL or EXTRA_CPPFLAGS=-DENABLE_USB_CONTROL)
endif
endif

ifeq ($(ENABLE_I2C_CONTROL),"Y")
ifeq ($(ENABLE_USB_CONTROL),"Y")
$(error I2C and USB can't be enabled at the same time)
endif
endif

CPPFLAGS  += -I$(HOST_TOP)/api/mid_level_api/sendcmd/include \
             -I$(HOST_TOP)/api/top_level_api/bg_upgrade_api/include \
             -I$(HOST_TOP)/api_src/include \
             -I$(HOST_TOP)/api/include 

LDFLAGS   += -L$(HOST_TOP)/api/mid_level_api/sendcmd/lib/$(VARIANT) \
             -L$(HOST_TOP)/api/top_level_api/bg_upgrade_api/lib/$(VARIANT) 

ifeq ($(ENABLE_I2C_CONTROL),"Y")
CPPFLAGS  += -I$(HOST_TOP)/api/interface_api/i2c_interface/include 
LDFLAGS   += -L$(HOST_TOP)/api/interface_api/i2c_interface/lib/$(VARIANT) \
             -lsendcmd_i2c \
             -li2c_interface 
               
             
endif

ifeq ($(ENABLE_USB_CONTROL),"Y")
CPPFLAGS  += -I$(HOST_TOP)/api/interface_api/usb_interface/include \
             -I$(HOST_TOP)/api_src/interface_api/usb_interface/libusb
LDFLAGS   += -L$(HOST_TOP)/api/interface_api/usb_interface/lib/$(VARIANT) \
             -L$(HOST_TOP)/api/interface_api/usb_interface/lib/$(VARIANT) \
             -lsendcmd_i2c-f3 \
             -lusb_interface \
             -lusb-1.0

ifeq ($(VARIANT), linux_armv7-a)
  LDFLAGS               += -lpthread
endif

ifeq ($(VARIANT), linux64_armv8-a)
  LDFLAGS               += -lpthread
endif

ifeq ($(VARIANT), linux64_armv8-a)
  LDFLAGS               += -lpthread
endif

ifeq ($(VARIANT), linux_rpi)
  LDFLAGS               += -lpthread
endif

#For linux_rpi.
ifeq ($(VARIANT), )
  LDFLAGS               += -lpthread
endif

endif

# ifeq ($(ENABLE_ALSA_I2C_CONTROL),"Y")
#   SRCS_CPP              += $(HOST_TOP)/capehost_alsa_i2c.cpp
# ## Only tested on RPi 32bit image.
#   LDFLAGS               += /usr/lib/arm-linux-gnueabihf/libasound.so.2.0.0
#endif

ifeq ($(VARIANT),mac32)
  CPPFLAGS              += -m32
  LDFLAGS               += $(HOST_TOP)/api/interface_api/usb_interface/lib/libusb/Mac/libusb-1.0.dylib.32
endif

ifeq ($(VARIANT),mac64)
  LDFLAGS               += $(HOST_TOP)/api/interface_api/usb_interface/lib/libusb/Mac/libusb-1.0.dylib.64
endif

ifneq ($(ENABLE_I2C_CONTROL),"")
TARGET_PATH             = $(CUR_DIR)/$(VARIANT)/I2C
endif 

ifneq ($(ENABLE_USB_CONTROL),"")
TARGET_PATH             = $(CUR_DIR)/$(VARIANT)/USB
endif 

include $(HOST_TOP)/etc/Makefile_host_code.base