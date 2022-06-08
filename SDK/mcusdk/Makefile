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
RM                      := rm -f

include $(CAFBASEROOT)/platform/src/include/auto.conf
include $(CAFBASEROOT)/platform/src/include/user.conf

#------------ LPAI ------------
#Tensai default setup 
TENSAI_COMPILER=$(TENSAI_COMPILER_DIR)/tensaicc
TENSAI_OUTPUT=platform/tahiti/m33/app/tensaiflow/output/
TENSAI_CONFIG=platform/tahiti/m33/app/tensaiflow/core_config.ini

#Setup the target Tensor model if CONFIG_TENSAI_COMPILE_MODEL not define
#User should define correct CONFIG_TENSAI_COMPILE_MODEL by "make config"
ifeq ($(CONFIG_TENSAI_COMPILE_MODEL),)
CONFIG_TENSAI_COMPILE_MODEL=tensaiflow/models/model_kpd.tflite
endif


all: tensai_make
	make -C platform/tahiti/m33/
	 
clean:
	make -C platform/tahiti/m33/ clean
ifeq ($(CONFIG_FIRMWARE),y)
else ifeq ($(CONFIG_TENSAIFLOW),y)
	@$(RM) -rf $(TENSAI_OUTPUT)
endif


tensai_make:
ifeq ($(CONFIG_FIRMWARE),y)
else ifeq ($(CONFIG_TENSAIFLOW),y)
ifeq ($(wildcard platform/tahiti/m33/app/tensaiflow/output/src/infer.c),)
	@echo "Compile Model.."
	@mkdir -p $(TENSAI_OUTPUT)/include 
	@mkdir -p $(TENSAI_OUTPUT)/src
	$(TENSAI_COMPILER)	--model=$(CONFIG_TENSAI_COMPILE_MODEL) --target_config=$(TENSAI_CONFIG) --app_dir=$(TENSAI_OUTPUT) --nodata_io

	@echo "Generating blob file.."

	@mkdir -p $(TENSAI_OUTPUT)/gen
	./tensaiflow/tools/preAllocExe --builddir=$(TENSAI_OUTPUT)/gen --blobdir=$(TENSAI_OUTPUT)/gen --infer=$(TENSAI_OUTPUT)/src/infer.c --config=platform/tahiti/m33/app/tensaiflow/inc/config.h --prealloc=tensaiflow/tools/preAlloc --a2bin=tensaiflow/tools/array2bin.c --output=$(TENSAI_OUTPUT)/gen --data=platform/tahiti/m33/app/tensaiflow/inc/input_data.h

	@echo "Copy Blob File.."
	if [ -e $(TENSAI_OUTPUT)/gen/*.blob ]; then	\
		cp $(TENSAI_OUTPUT)/gen/*.blob ../fcp/scripts/;	\
	fi

	@echo "Copy Header.."
	@cp $(TENSAI_OUTPUT)/gen/preAlloc.h $(TENSAI_OUTPUT)/include/

endif #end of exist infer.c
endif

.PHONY: all clean
