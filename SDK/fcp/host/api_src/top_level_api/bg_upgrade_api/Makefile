################################################################################
## needs to be defined by relative way.
################################################################################
HOST_TOP := ../../..
CUR_DIR  := $(HOST_TOP)/api_src/top_level_api/bg_upgrade_api

################################################################################
## General
################################################################################

TARGET                  := libbg_upgrade.a
SRCS_C                  := $(wildcard $(CUR_DIR)/*.*c) 

FIND_SENDCMD_COMMON     := $(if $(findstring AudioSmart_sendcmd_common.c,$(SRCS_C)),"Y","")
ifeq ($(FIND_SENDCMD_COMMON),"")
SRCS_C                  +=$(HOST_TOP)/api_src/top_level_api/dsp_ctrl_status_api/AudioSmart_sendcmd_common.c
endif 

CPPFLAGS                := -c -fpermissive

LDFLAGS                 = -lc 

CPPFLAGS  += -I$(HOST_TOP)/api/mid_level_api/sendcmd/include \
             -I$(HOST_TOP)/api/top_level_api/bg_upgrade_api/include \
             -I$(HOST_TOP)/api_src/top_level_api/bg_upgrade_api \
             -I$(HOST_TOP)/api_src/top_level_api/include \
             -I$(HOST_TOP)/api_src/include \
             -I$(HOST_TOP)/api/include

ifeq ($(CONFIG_TARGET_TAHITI),y)
CPPFLAGS += -I$(HOST_TOP) -DCONFIG_TARGET_TAHITI
endif

ifeq ($(CONFIG_TARGET_KATANA),y)
CPPFLAGS += -I$(HOST_TOP) -DCONFIG_TARGET_KATANA
endif

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


TARGET_PATH             = $(HOST_TOP)/api/top_level_api/bg_upgrade_api/lib/$(VARIANT)
include $(HOST_TOP)/etc/Makefile_host_code.base
