################################################################################
## needs to be defined by relative way.
################################################################################
HOST_TOP           := ../../../..
CUR_DIR            := $(HOST_TOP)/api_src/interface_api/i2c_interface/i2c_dev


################################################################################
## General
################################################################################
TARGET             := libi2c_interface.a
SRCS_C             := $(HOST_TOP)/api_src/interface_api/i2c_interface/i2c_dev/AudioSmart_i2c_dev.c

CPPFLAGS           += -c \
                      -I$(HOST_TOP)/api/interface_api/i2c_interface/include \
                      -I$(HOST_TOP)/api/include

TARGET_PATH        = $(HOST_TOP)/api/interface_api/i2c_interface/lib/$(VARIANT)

include $(HOST_TOP)/etc/Makefile_host_code.base
