################################################################################
## needs to be defined by relative way.
################################################################################
HOST_TOP := ../../..
CUR_DIR  := $(HOST_TOP)/api_src/top_level_api/dsp_ctrl_status_api

################################################################################
## General
################################################################################
TARGET                  := libdsp_ctrl_status.a

ifeq ($(VARIANT), linux_armv7-a)
	SRCS_C                  := AudioSmart_generic.c AudioSmart_sendcmd_common.c
else ifeq ($(VARIANT), linux64_armv8-a)
	SRCS_C                  := AudioSmart_generic.c AudioSmart_sendcmd_common.c
else ifeq ($(VARIANT), android_armv7-a)
	SRCS_C                  := AudioSmart_generic.c AudioSmart_sendcmd_common.c
else ifeq ($(VARIANT), android64_armv8-a)
	SRCS_C                  := AudioSmart_generic.c AudioSmart_sendcmd_common.c
else
SRCS_C                  := $(wildcard $(CUR_DIR)/*.*c)
endif

CPPFLAGS                := -c 

LDFLAGS                 = -lc

CPPFLAGS  += -I. \
             -I$(HOST_TOP)/api/mid_level_api/sendcmd/include \
             -I$(HOST_TOP)/api/top_level_api/dsp_ctrl_status_api/include \
             -I$(HOST_TOP)/api/interface_api/usb_interface/include \
             -I$(HOST_TOP)/api_src/top_level_api/include \
             -I$(HOST_TOP)/api_src/include \
             -I$(HOST_TOP)/api/include \
             -I$(HOST_TOP)


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


TARGET_PATH             = $(HOST_TOP)/api/top_level_api/dsp_ctrl_status_api/lib/$(VARIANT)
include $(HOST_TOP)/etc/Makefile_host_code.base