################################################################################
## needs to be defined by relative way.
################################################################################
HOST_TOP := ../../..
CUR_DIR  := $(HOST_TOP)/api_src/mid_level_api/sendcmd_i2c-f3


################################################################################
## General
################################################################################
TARGET             := libsendcmd_i2c-f3.a
SRCS_C             := $(HOST_TOP)/api_src/mid_level_api/sendcmd_i2c-f3/AudioSmart_sendcmd_i2c-f3.c
CPPFLAGS           := -c \
                      -I. \
                      -I$(HOST_TOP)/api/mid_level_api/sendcmd/include \
                      -I$(HOST_TOP)/api_src/include \
                      -I$(HOST_TOP)/api/interface_api/usb_interface/include \
                      -I$(HOST_TOP)/api/include

TARGET_PATH             = $(HOST_TOP)/api/mid_level_api/sendcmd/lib/$(VARIANT)
include $(HOST_TOP)/etc/Makefile_host_code.base
