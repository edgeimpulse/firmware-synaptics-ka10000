################################################################################
## needs to be defined by relative way.
################################################################################
HOST_TOP := ../../../..
CUR_DIR  := $(HOST_TOP)/api_src/interface_api/usb_interface/libusb


################################################################################
## General
################################################################################
TARGET             := libusb_interface.a
SRCS_C             := $(HOST_TOP)/api_src/interface_api/usb_interface/libusb/AudioSmart_usb_interface_api_libusb.c

CPPFLAGS           := -c \
					  -I$(HOST_TOP)/api_src/interface_api/usb_interface/libusb -lusb1 \
					  -I$(HOST_TOP)/api/interface_api/usb_interface/include \
					  -I$(HOST_TOP)/api/include

TARGET_PATH             = $(HOST_TOP)/api/interface_api/usb_interface/lib/$(VARIANT)
include $(HOST_TOP)/etc/Makefile_host_code.base