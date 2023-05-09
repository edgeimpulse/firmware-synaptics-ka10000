################################################################################
## needs to be defined by relative way.
################################################################################
HOST_TOP := ../..
CUR_DIR  := $(HOST_TOP)/application/api_caf_thin_client

################################################################################
## General
################################################################################
TARGET                  := caf_thin_client
SRCS_CPP                := $(CUR_DIR)/thin_client.cpp

CPPFLAGS                := -c

LDFLAGS                 = -lc

ifeq ($(VARIANT), linux_armv7-a)
  LDFLAGS          += -lpthread
endif

ifeq ($(VARIANT), linux64_armv8-a)
  LDFLAGS          += -lpthread
endif

ifeq ($(VARIANT), linux_rpi)
  LDFLAGS          += -lpthread
endif

ifeq ($(VARIANT), linux_x86)
  LDFLAGS          += -lpthread
endif

ifeq ($(VARIANT),)
  LDFLAGS          += -lpthread
endif

ENABLE_I2C_CONTROL      := $(if $(findstring -DENABLE_I2C_CONTROL,     $(EXTRA_CPPFLAGS)),"Y","")
ENABLE_USB_CONTROL      := $(if $(findstring -DENABLE_USB_CONTROL,     $(EXTRA_CPPFLAGS)),"Y","")

ifeq ($(ENABLE_I2C_CONTROL),"")
ifeq ($(ENABLE_USB_CONTROL),"")
$(error either I2C or USB must be enabled, enable one of them with EXTRA_CPPFLAGS=-DENABLE_I2C_CONTROL or EXTRA_CPPFLAGS=-DENABLE_USB_CONTROL)
endif 
endif
ifeq ($(ENABLE_I2C_CONTROL),"Y")
ifeq ($(ENABLE_USB_CONTROL),"Y")
$(error I2C and USB can't be enabled at the same time)
endif
endif

CPPFLAGS  += -I$(HOST_TOP)/api/mid_level_api/sendcmd/include \
             -I$(HOST_TOP)/api_src/include \
             -I$(HOST_TOP)/api/include \

LDFLAGS   += -L$(HOST_TOP)/api/mid_level_api/sendcmd/lib/$(VARIANT) \

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
               -lsendcmd_usb \
               -lusb_interface \
               -lusb-1.0
endif


ifneq ($(ENABLE_I2C_CONTROL),"")
TARGET_PATH             = $(CUR_DIR)/$(VARIANT)/I2C
endif 

ifneq ($(ENABLE_USB_CONTROL),"")
TARGET_PATH             = $(CUR_DIR)/$(VARIANT)/USB
endif 



include $(HOST_TOP)/etc/Makefile_host_code.base
