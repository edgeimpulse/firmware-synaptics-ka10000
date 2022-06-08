################################################################################
## needs to be defined by relative way.
################################################################################
HOST_TOP := ../..
CUR_DIR  := $(HOST_TOP)/demo/i2c_flash

################################################################################
## General
################################################################################
INTERFACE_I2C_SIENA		:= $(if $(findstring -DINTERFACE_I2C_SIENA,	$(EXTRA_CPPFLAGS)),"Y","")
INTERFACE_I2C_F3     	:= $(if $(findstring -DINTERFACE_I2C_F3,    $(EXTRA_CPPFLAGS)),"Y","")
INTERFACE_I2C_BT_SPP    := $(if $(findstring -DINTERFACE_I2C_BT_SPP,$(EXTRA_CPPFLAGS)),"Y","")

ifeq ($(INTERFACE_I2C_SIENA),"")
ifeq ($(INTERFACE_I2C_F3),"")
ifeq ($(INTERFACE_I2C_BT_SPP),"")
$(error Either SIENA or F3 or BT_SPP must be enabled, enable them with EXTRA_CPPFLAGS=-DINTERFACE_I2C_SIENA or EXTRA_CPPFLAGS=-DINTERFACE_I2C_F3 or EXTRA_CPPFLAGS=-DINTERFACE_I2C_BT_SPP)
endif
endif
endif

ifeq ($(INTERFACE_I2C_SIENA),"Y")
ifeq ($(INTERFACE_I2C_F3),"Y")
ifeq ($(INTERFACE_I2C_BT_SPP),"Y")
$(error SIENA and F3 and BT_SPP can not be enabled at the same time)
endif
endif
endif

ifeq ($(INTERFACE_I2C_SIENA),"Y")
TARGET             := i2c_flash
endif
ifeq ($(INTERFACE_I2C_F3),"Y")
TARGET             := i2c_flash_f3
endif
ifeq ($(INTERFACE_I2C_BT_SPP),"Y")
TARGET             := i2c_flash_bt_spp
endif

SRCS_CPP           := $(CUR_DIR)/main.cpp

CPPFLAGS           := -c -fpermissive -DCONFIG_I2C_DISABLE_RESET  

CPPFLAGS			+= -I $(HOST_TOP)/api/top_level_api/i2c_flash_api/include \
					-I $(HOST_TOP)/api/interface_api/i2c_interface/include \
					-I $(HOST_TOP)/api/interface_api/misc_interface/include \
					-I $(HOST_TOP)/api/include

ifeq ($(INTERFACE_I2C_F3),"Y")
LDFLAGS				= -L./$(HOST_TOP)/api/top_level_api/i2c_flash_api/lib/$(VARIANT)/F3 -li2c_f3_interface \
					-L./$(HOST_TOP)/api/interface_api/i2c_interface/lib/$(VARIANT) -li2c_flash_api \
					-lusb-1.0
					#-L./lib/x86_64-linux-gnu -lusb-1.0
endif

#TBD: INTERFACE_I2C_SIENA/BT_SPP

ifeq ($(INTERFACE_I2C_SIENA),"Y")
TARGET_PATH             = $(CUR_DIR)/pre_compiled/$(VARIANT)/SIENA
endif 

ifeq ($(INTERFACE_I2C_F3),"Y")
TARGET_PATH             = $(CUR_DIR)/pre_compiled/$(VARIANT)/F3
endif

ifeq ($(INTERFACE_I2C_BT_SPP),"Y")
TARGET_PATH             = $(CUR_DIR)/pre_compiled/$(VARIANT)/BT_SPP
endif 

include $(HOST_TOP)/etc/Makefile_host_code.base

