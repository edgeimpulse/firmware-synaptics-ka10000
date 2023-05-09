################################################################################
#
# NDA AND NEED-TO-KNOW REQUIRED
#
# Copyright (C) 2021 Synaptics Incorporated. All rights reserved.
#
# This file contains information that is proprietary to Synaptics
# Incorporated ("Synaptics"). The holder of this file shall treat all
# information contained herein as confidential, shall use the
# information only for its intended purpose, and shall not duplicate,
# disclose, or disseminate any of this information in any manner
# unless Synaptics has otherwise provided express, written
# permission.
#
# Use of the materials may require a license of intellectual property
# from a third party or from Synaptics. This file conveys no express
# or implied licenses to any intellectual property rights belonging
# to Synaptics.
#
# INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
# SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
# INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
# INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
# CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
# OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
# BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
# COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
# DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
# TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
#
################################################################################

.ONESHELL:
.SHELLFLAGS = -ec

V:=@
SILENT:= --silent

# Define the root folder which is an absolute path
export CAFBASEROOT := $(CURDIR)/fcp

# Define the full path of the config folder at cafbase root, which is used 
# in Makefile under each individual configuration directory
export conf_dir_path := $(CAFBASEROOT)/config

export KCONFIG_AUTOCONFIG := $(CAFBASEROOT)/platform/src/include/user.conf
export KCONFIG_AUTOHEADER := $(CAFBASEROOT)/platform/src/include/userconf.h

include $(CAFBASEROOT)/platform/src/include/auto.conf
include $(CAFBASEROOT)/etc/Makefile.def


#define building sub-system
ifeq ($(or $(CONFIG_TARGET_TAHITI),$(CONFIG_TARGET_KATANA)),y)


sub_systems :=	$(if $(CONFIG_SDK_CAPE2),$(if $(wildcard cape2sdk),cape2sdk,),) \
				$(if $(CONFIG_SDK_MCU),$(if $(wildcard mcusdk),mcusdk,),) \
				$(if $(CONFIG_SDK_HIFI),$(if $(wildcard hifi3sdk),hifi3sdk,),) \
				$(if $(CONFIG_SDK_ASIP),$(if $(wildcard asipsdk),asipsdk,),)


clean_sub_systems :=	$(if $(CONFIG_SDK_CAPE2),$(if $(wildcard cape2sdk),clean-cape2sdk,),) \
						$(if $(CONFIG_SDK_MCU),$(if $(wildcard mcusdk),clean-mcusdk,),) \
						$(if $(CONFIG_SDK_HIFI),$(if $(wildcard hifi3sdk),clean-hifi3sdk,),) \
						$(if $(CONFIG_SDK_ASIP),$(if $(wildcard asipsdk),clean-asipsdk,),)



PLATFORM :="tahiti"
DEFS := MDL_LIBS="cnxt" FLAVOR="cape2" PLATFORM="tahiti"

endif

ifeq ($(UNAME_O),Cygwin)
	ECHO_ESC = -e
endif


clean help: 
	$(V)echo "Build Facility for building the firmware"
	$(V)echo "make mcusdk            : Builds only the M33 SDK into appropriate chunk file. This does not package the image." 
	$(V)echo "make hifi3sdk          : Builds only the HiFi3 SDK into appropriate chunk file. This does not package the image." 
	$(V)echo "make cape2sdk          : Builds only the CAPE2 SDK into appropriate chunk file. This does not package the image." 
	$(V)echo "make asipsdk            : Builds only the LLE SDK into appropriate chunk file. This does not package the image." 
	$(V)echo "make image             : Compiles the script files, packages the chunk files and creates an image which can be flashed onto the SPI flash"
	$(V)echo "make flash-image    	 : Flashes the image onto the SPI flash by bg_upgrade"
	$(V)echo "make everything        : Builds all the SDKs and also creates an image which can be flashed onto the SPI flash"
	$(V)echo "make clean-mcusdk      : Cleans MCU chunk file in mcusdk"
	$(V)echo "make clean-hifi3sdk    : Cleans HiFi3 chunk file in hifi3sdk"
	$(V)echo "make clean-cape2sdk    : Cleans CAPE2 chunk file in cape2sdk"
	$(V)echo "make clean-asipsdk      : Cleans LLE chunk file in asipsdk"
	$(V)echo "make clean-everything  : Cleans everything"
	$(V)echo "make clean-image       : Cleans only the image"

everything: all
all: $(sub_systems) image
		
$(sub_systems):
	$(V)echo $(ECHO_ESC) "\n\n====================================================="
	echo "Building firmware $(PLATFORM)/$@ "
	date
	echo $(ECHO_ESC) "=====================================================\n"
	$(MAKE) -C $@ $(SILENT) $(DEFS) DO=

$(clean_sub_systems):
	$(V)echo $(ECHO_ESC) "\n\n====================================================="; \
	echo "Cleaning firmware $(subst clean-, , $@) "; \
	date; \
	echo $(ECHO_ESC) "=====================================================\n"; \
	cd $(subst clean-, , $@) && $(MAKE) $(SILENT) $(DEFS) clean && cd ..; \
	echo "Done cleaning "$(subst clean-, , $@); \
	echo $(ECHO_ESC) ""
		
image: 
	$(V)echo $(ECHO_ESC) "\n\n====================================================="
	echo "Packaging ..."
	date
	echo $(ECHO_ESC) "=====================================================\n"
	$(V)cd fcp/config && $(MAKE) $(SILENT) $(DEFS) package CONFIG=$(config_dir_short)  SUBCONFIG=$(subconfig)

flash-image: 
	$(V)echo $(ECHO_ESC) "\n\n====================================================="
	echo "Flashing ..."	
	echo $(ECHO_ESC) "=====================================================\n"
	cd fcp/build
	./bg_upgrade -s partition_system.img

mcu-image: mcusdk image

config:
	echo "Make user config..."
	cd fcp/platform && $(call user_config) && cd ../..
ifeq ($(CONFIG_SDK_MCU),y)
	cp $(CAFBASEROOT)/platform/src/include/user* $(CAFBASEROOT)/../mcusdk/platform/src/include/
endif
ifeq ($(CONFIG_SDK_ASIP),y)
	cp $(CAFBASEROOT)/platform/src/include/user* $(CAFBASEROOT)/../asipsdk/platform/src/include/
endif

clean-image:
	$(V)$(RM) -R $(CAFBASEROOT)/build
	$(V)echo "Done clean-image"

	
clean-everything: $(clean_sub_systems) clean-image
	$(V)echo "Done clean-everything"

debug:
	echo "debug"
	$(V)echo $(prx)
	$(V)echo $(sub_systems)
	
.PHONY: help all $(sub_systems) $(clean_sub_systems) image clean-image everything clean-everything 
