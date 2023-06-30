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

V:=@

.ONESHELL:
.SHELLFLAGS=-ec

.PHONY : all post_build

-include ../../src/include/auto.conf
-include ../../src/include/user.conf

RM                      := rm -f
MKDIR                   := mkdir -p
srctree					:= .

PROGRAMFILES:="/cygdrive/c/Program\ Files\ \(x86\)/"

ifeq ($(or $(CONFIG_TARGET_TAHITI),$(CONFIG_TARGET_KATANA)),y)
##The Tahiti path to the cross compile tool chain must be defined
##in the enviroment variable: CROSS_TOOLS.  strcat "arm-none-eabi-" to the path.
#CROSS:="$(CROSS_TOOLS)"
ifeq ($(UNAME),Linux)
	CROSS:="/opt/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-"
else
	CROSS:="$(PROGRAMFILES)GNU Tools ARM Embedded/7 2017-q4-major/bin/arm-none-eabi-"
endif
endif

CHUNK_TOOLS := ../../../../fcp/platform/tahiti/tools/create_chunks.tcl
MCUCC=$(CROSS)gcc
MCUCXX=$(CROSS)g++
MCUAR=$(CROSS)ar
MCULD=$(CROSS)ld
MCUSIZE=$(CROSS)size
MCUOBJDUMP=$(CROSS)objdump
MCUOBJCOPY=$(CROSS)objcopy
MCUSTRIP=$(CROSS)strip


#------------ NONE_SECURE ------------
MCUCFLAGS_NS:= -mcpu=cortex-m33 -mthumb -specs=nano.specs -mfloat-abi=hard -mfpu=fpv5-sp-d16 -D__DOMAIN_NS=1 -D__STARTUP_CLEAR_BSS -DARMCM33_DSP_FP -D__START=main_ns
MCUCFLAGS_NS+= -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fstack-usage
MCUCFLAGS_NS+= -g3
#MCUCFLAGS_NS+= -Wno-implicit-function-declaration
MCUCFLAGS_NS+= $(CONFIG_MCU_TOOLCHAIN_GCC_O_LVL_NS)
MCUCFLAGS_NS+= -I ../../src/include
MCUCFLAGS_NS+= -I ../include
ifeq ($(CONFIG_CHIP_VERSION_Z1),y)
MCUCFLAGS_NS+= -I ../include/z1
endif
ifeq ($(CONFIG_CHIP_VERSION_A1),y)
MCUCFLAGS_NS+= -I ../include/a1
endif
ifeq ($(CONFIG_CHIP_VERSION_A0),y)
MCUCFLAGS_NS+= -I ../include/a0
endif
ifeq ($(CONFIG_CHIP_VERSION_B0),y)
MCUCFLAGS_NS+= -I ../include/b0
endif
MCUCFLAGS_NS+= -I $(srctree)/cnxt/ARMCM33/Include
MCUCFLAGS_NS+= -I $(srctree)/arm/CMSIS_5.3.0/Core/include
MCUCFLAGS_NS+= -I $(srctree)/include
# ifeq ($(CONFIG_MCU_RTOS_FRTOS),y)
MCUCFLAGS_NS+= -I $(srctree)/rtos/frtos/Source/include
MCUCFLAGS_NS+= -I $(srctree)/rtos/frtos/Source/portable/GCC/ARM_CM33F
MCUCFLAGS_NS+= -I $(srctree)/rtos/os_al
# endif
MCUCFLAGS_NS+= -I $(srctree)/rtos/os_al
MCUCFLAGS_NS+= -I $(srctree)/driver/hw/include
MCUCFLAGS_NS+= -I $(srctree)/driver/sw/include
MCUCFLAGS_NS+= -I $(srctree)/test

MCUCFLAGS_NS+= -I $(srctree)/app/ctrl/include
MCUCFLAGS_NS+= -I ../../../caf/
MCUCFLAGS_NS+= -I ../../../caf/include
MCUCFLAGS_NS+= -I ../../../platform/src/api_src/include

ifeq ($(CONFIG_FIRMWARE),y)
MCUCFLAGS_NS+= -I $(srctree)/app/tensaiflow/inc/
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/tensaiflow-model
MCUCFLAGS_NS+= -I $(srctree)/app/tensaiflow/cmsis
else ifeq ($(CONFIG_TENSAIFLOW),y)
MCUCFLAGS_NS+= -I $(srctree)/app/tensaiflow/inc/
MCUCFLAGS_NS+= -I $(srctree)/app/tensaiflow/output/include
MCUCFLAGS_NS+= -I $(srctree)/app/tensaiflow/cmsis
endif #CONFIG_TENSAIFLOW

ifeq ($(CONFIG_FIRMWARE),y)
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/classifier
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/porting
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/dsp
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/dsp/kissfft
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/dsp/dct
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/dsp/
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/third_party/flatbuffers/include/flatbuffers
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/third_party/gemmlowp
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/third_party/ruy
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/CMSIS/DSP/Include/
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/CMSIS/Core/Include/
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/tensorflow/
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/tensorflow/lite
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/tensorflow/lite/micro
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/tensorflow/lite/micro/kernels
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/tensorflow/lite/scheme
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/tensorflow/lite/c
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/tensorflow/lite/core/api/
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/model-parameters
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/sensors
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/ingestion-sdk-platform/synaptics
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/ingestion-sdk-c
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/repl
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/QCBOR/inc
MCUCFLAGS_NS+= -I $(srctree)/app/firmware/edge_impulse/mbedtls_hmac_sha256_sw

# CFLAGS
MCUCFLAGS_NS+= -DCONFIG_WCHAR_BUILTIN
MCUCFLAGS_NS+= -DCONFIG_HAVE_DOUBLE
MCUCFLAGS_NS+= -fmessage-length=0
MCUCFLAGS_NS+= -fno-exceptions
MCUCFLAGS_NS+= -fno-unwind-tables
MCUCFLAGS_NS+= -ffunction-sections
MCUCFLAGS_NS+= -fdata-sections
MCUCFLAGS_NS+= -funsigned-char
MCUCFLAGS_NS+= -mabi=aapcs
MCUCFLAGS_NS+= -mthumb
MCUCFLAGS_NS+= -mfloat-abi=hard
MCUCFLAGS_NS+= -Wall
MCUCFLAGS_NS+= -Wextra
MCUCFLAGS_NS+= -Wno-shadow
MCUCFLAGS_NS+= -Wno-vla
MCUCFLAGS_NS+= -Wno-strict-aliasing
MCUCFLAGS_NS+= -Wno-type-limits
MCUCFLAGS_NS+= -Wno-unused-parameter
MCUCFLAGS_NS+= -Wno-missing-field-initializers
MCUCFLAGS_NS+= -Wno-write-strings
MCUCFLAGS_NS+= -Wno-sign-compare
MCUCFLAGS_NS+= -Wno-unused-function
MCUCFLAGS_NS+= -Wno-unused-variable
MCUCFLAGS_NS+= -fno-delete-null-pointer-checks
MCUCFLAGS_NS+= -fomit-frame-pointer
MCUCFLAGS_NS+= -O2

# APP FLAGS
MCUCFLAGS_NS+=	-DEI_SENSOR_AQ_STREAM=FILE
MCUCFLAGS_NS+=	-DEI_PORTING_SYNAPTICS=1
MCUCFLAGS_NS+=	-DEIDSP_USE_CMSIS_DSP=1
MCUCFLAGS_NS+=  -DEI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN=1
MCUCFLAGS_NS+=  -DEI_CLASSIFIER_TFLITE_LOAD_CMSIS_NN_SOURCES=1
MCUCFLAGS_NS+=  -DEIDSP_LOAD_CMSIS_DSP_SOURCES=1
MCUCFLAGS_NS+=	-DEIDSP_QUANTIZE_FILTERBANK=0
MCUCFLAGS_NS+=	-DNDEBUG

# CXXFLAGS
MCUCXXFLAGS_NS+= $(MCUCFLAGS_NS)
MCUCXXFLAGS_NS+= -std=gnu++11
MCUCXXFLAGS_NS+= -fno-rtti
MCUCXXFLAGS_NS+= -fno-use-cxa-atexit
MCUCXXFLAGS_NS+= -fno-inline-functions

endif #CONFIG_FIRMWARE

MCULDFLAGS_NS:= -nostartfiles -specs=nosys.specs -Wl,--start-group -Wl,--print-memory-usage -lgcc -lc -lm -Wl,--end-group
MCULDFLAGS_NS+= -Xlinker --gc-sections

SRC := ./app/demo
SRC_CTRL := ./app/ctrl
SRC_USR  := ./app/usr
PREBUILT := ./build

bin-obj_ns:= $(PREBUILT)/mcu_ns.bin
map-obj_ns:= $(PREBUILT)/mcu_ns.map
siz-obj_ns:= $(PREBUILT)/mcu_ns.size

ifeq ($(or $(CONFIG_TARGET_TAHITI),$(CONFIG_TARGET_KATANA)),y)
ld-scripts_pre_ns:=$(srctree)/etc/ld/tahiti_gcc_arm_ns.ld.h
ld-scripts_ns:=$(PREBUILT)/tahiti_gcc_arm_ns.ld
endif

syslib-objs_ns:= $(PREBUILT)/libsys_ns.a
oslib-objs_ns:= $(PREBUILT)/libos_ns.a
ifeq ($(CONFIG_MCU_KPD_PROCESS),y)
usrkpdlib-objs_ns:= $(PREBUILT)/libusrkpd_ns.a
endif
usrlib-objs_ns:= $(PREBUILT)/libusr_ns.a

lib-objs_ns:= $(syslib-objs_ns)
lib-objs_ns+= $(oslib-objs_ns)
lib-objs_ns+= $(usrlib-objs_ns)
ifeq ($(CONFIG_MCU_KPD_PROCESS),y)
lib-objs_ns+= $(usrkpdlib-objs_ns)
endif

ifeq ($(CONFIG_MCU_NON_SECURE_MODE_ENABLE_TZ),y)
ifneq ($(CONFIG_MCU_M33_ROM),y)
cmse-lib-objs_ns:= $(PREBUILT)/cmse_implib.o
endif
endif

target-outputs_ns:= $(PREBUILT)/mcu_ns.elf
target-outputs_dis:= $(PREBUILT)/mcu_ns.dis

all: $(build-dir_ns) $(target-outputs_ns) $(bin-obj_ns) post_build
	@echo "done building " $^

$(PREBUILT)/%.o::$(srctree)/%.c
	@echo 'MCU CC -c $<'
	@$(MCUCC) $(MCUCFLAGS_NS) $(MCUEXTRA_CFLAGS) -MMD -MP -MF"$(@:%.o=%.dep)" -MT"$(@)" -c -o $@ $<

$(PREBUILT)/%.o::$(srctree)/%.cpp
	@echo 'MCU CXX -c $<'
	@$(MCUCXX) $(MCUCXXFLAGS_NS) $(MCUEXTRA_CFLAGS) -MMD -MP -MF"$(@:%.o=%.dep)" -MT"$(@)" -c -o $@ $<

$(PREBUILT)/%.o::$(srctree)/%.cc
	@echo 'MCU CXX -c $<'
	@$(MCUCXX) $(MCUCXXFLAGS_NS) $(MCUEXTRA_CFLAGS) -MMD -MP -MF"$(@:%.o=%.dep)" -MT"$(@)" -c -o $@ $<


ifeq ($(CONFIG_FIRMWARE),y)
usr-objs_ns+=$(srctree)/app/firmware/edge_impulse/tensaiflow-model/infer.o
else ifeq ($(CONFIG_TENSAIFLOW),y)
usr-objs_ns+=$(srctree)/app/demo/tensai_demo/app_start.o
ifeq ($(CONFIG_MCU_SDK_KPD_DEMO),y)
usr-objs_ns+=$(srctree)/app/demo/tensai_demo/kpd_demo/data_io.o
else ifeq ($(CONFIG_MCU_SDK_KMR_DEMO),y)
usr-objs_ns+=$(srctree)/app/demo/tensai_demo/kmr_demo/data_io.o
endif
usr-objs_ns+=$(srctree)/app/tensaiflow/output/src/infer.o
else
usr-objs_ns:=$(srctree)/app/demo/usr_demo.o
endif

usr-objs_ns+=$(srctree)/app/ctrl/usr_ctrl.o
usr-objs_ns+=$(srctree)/app/demo/demo_send_cmd.o
usr-objs_ns+=$(srctree)/app/usr/usr_init_ns.o
ifeq ($(CONFIG_MCU_G_ACCELOMETER),y)
usr-objs_ns+=$(srctree)/app/usr/usr_g_accelerometer.o
endif
ifeq ($(CONFIG_MCU_DRV_CAMERA),y)
usr-objs_ns+=$(srctree)/app/demo/camera_demo.o
endif
ifeq ($(CONFIG_MCU_DRV_ADC),y)
usr-objs_ns+=$(srctree)/app/demo/adc_demo.o
endif
ifeq ($(CONFIG_MCU_DRV_SFS),y)
usr-objs_ns+=$(srctree)/app/demo/sfs_demo.o
endif
ifeq ($(CONFIG_MCU_PM_LITE),y)
usr-objs_ns+=$(srctree)/app/demo/pm_demo.o
endif
ifeq ($(CONFIG_MCU_DRV_BLUETOOTH),y)
usr-objs_ns+=$(srctree)/app/demo/bluetooth_demo.o
endif

ifeq ($(CONFIG_SDK_MCU_EXPOSE_CAMERA),y)
ifeq ($(CONFIG_MCU_DRV_CAMERA),y)
ifeq ($(CONFIG_CAMERA_SELECT_HM01B0),y)
usr-objs_ns+=$(srctree)/driver/hw/camera/hm01b0.o
endif
usr-objs_ns+=$(srctree)/driver/hw/camera/camera.o
endif
endif

ifeq ($(CONFIG_TENSAIFLOW),y)
usr-objs_ns+=$(srctree)/app/tensaiflow/src/fileSystem.o
usr-objs_ns+=$(srctree)/app/tensaiflow/src/img_process.o
usr-objs_ns+=$(srctree)/app/tensaiflow/src/mem_alloc_al.o
endif #CONFIG_TENSAIFLOW

ifeq ($(CONFIG_FIRMWARE),y)

# SRC_C
SRC_C+=$(wildcard $(srctree)/app/firmware/*.c)
SRC_C+=$(wildcard $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/CMSIS/DSP/Source/TransformFunctions/*fft*.c)
SRC_C+=$(wildcard $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/CMSIS/DSP/Source/CommonTables/*.c)
SRC_C+=$(wildcard $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/CMSIS/DSP/Source/TransformFunctions/*bit*.c)
SRC_C+=$(wildcard $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/CMSIS/DSP/Source/MatrixFunctions/*.c)
SRC_C+=$(wildcard $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/CMSIS/DSP/Source/StatisticsFunctions/*.c)
SRC_C+=$(wildcard $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/CMSIS/NN/Source/ActivationFunctions/*.c)
SRC_C+=$(wildcard $(srctree)/app/firmware/edge_impulse/tensaiflow-model/*.c)
SRC_C+=$(wildcard $(srctree)/app/firmware/edge_impulse/mbedtls_hmac_sha256_sw/mbedtls/src/*.c)
SRC_C+=$(wildcard $(srctree)/app/firmware/edge_impulse/tensorflow/lite/c/*.c)
SRC_C+=$(wildcard $(srctree)/app/firmware/edge_impulse/QCBOR/src/*.c)


# SRC_CXX
SRC_CXX=$(wildcard $(srctree)/app/firmware/*.cpp)
SRC_CXX+=$(wildcard $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/porting/synaptics/*.cpp)
SRC_CXX+=$(wildcard $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/dsp/dct/*.cpp)
SRC_CXX+=$(wildcard $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/dsp/kissfft/*.cpp)
SRC_CXX+=$(wildcard $(srctree)/app/firmware/edge_impulse/edge-impulse-sdk/dsp/image/*.cpp)
SRC_CXX+=$(wildcard $(srctree)/app/firmware/edge_impulse/ingestion-sdk-platform/synaptics/*.cpp)
SRC_CXX+=$(wildcard $(srctree)/app/firmware/edge_impulse/ingestion-sdk-c/*.cpp)
SRC_CXX+=$(wildcard $(srctree)/app/firmware/edge_impulse/repl/*.cpp)
SRC_CXX+=$(wildcard $(srctree)/app/firmware/edge_impulse/firmware-sdk/*.cpp)
SRC_CXX+=$(wildcard $(srctree)/app/firmware/edge_impulse/firmware-sdk/jpeg/*.cpp)
SRC_CXX+=$(wildcard $(srctree)/app/firmware/sensors/*.cpp)

usr-objs_ns +=$(SRC_C:.c=.o)
usr-objs_ns +=$(SRC_CXX:.cpp=.o)
usr-objs_ns +=$(SRC_CC:.cc=.o)

endif #CONFIG_FIRMWARE

DEPS_ns:=$(usr-objs_ns:%.o=$(PREBUILT)/%.dep)
-include $(DEPS_ns)

usr-objs_ns:=$(patsubst $(srctree)/%.o, $(PREBUILT)/%.o, $(usr-objs_ns))


usrlib-objs_ns:= $(srctree)/build/libusr_ns.a
lib-objs_ns+= $(usrlib-objs_ns)
build-dir_ns := $(sort $(dir $(usr-objs_ns)))
clean-obj+= $(DEPS_ns) $(usrlib-objs_ns) $(target-outputs_ns) $(map-obj_ns) $(usr-objs_ns) $(bin-obj_ns) $(siz-obj_ns)

tensai_lib_ns:=
ifeq ($(CONFIG_TENSAIFLOW),y)
tensai_lib_ns+= $(srctree)/app/tensaiflow/lib/libCMSISNN.a
tensai_lib_ns+= $(srctree)/app/tensaiflow/lib/libETA_NN_M33.a
tensai_lib_ns+= $(srctree)/app/tensaiflow/lib/libexecutor.a
tensai_lib_ns+= $(srctree)/app/tensaiflow/lib/librpc.a
endif


$(usrlib-objs_ns): $(usr-objs_ns)
	@echo AR $@
	@$(MCUAR) crus $@ $^

# for synaptic demos
#@$(MCUCC) $(MCUCFLAGS_NS) $(MCUEXTRA_CFLAGS) $(MCULDFLAGS_NS) -Wl,-Map,"$(map-obj_ns)" -T $(ld-scripts_ns) $(cmse-lib-objs_ns) $(lib-objs_ns) $(tensai_lib_ns) -lgcc -lc -lm -o $@
# for edge impulse
#@$(MCUCXX) $(MCUCXXFLAGS_NS) $(MCUEXTRA_CFLAGS) $(MCULDFLAGS_NS) -Wl,-Map,"$(map-obj_ns)" -T $(ld-scripts_ns) $(cmse-lib-objs_ns) $(lib-objs_ns) $(tensai_lib_ns) -lgcc -lc -lm -o $@
$(target-outputs_ns): $(build-dir_ns) $(ld-scripts_ns) $(usrlib-objs_ns)
	@$(MCUCXX) $(MCUCXXFLAGS_NS) $(MCUEXTRA_CFLAGS) $(MCULDFLAGS_NS) -Wl,-Map,"$(map-obj_ns)" -T $(ld-scripts_ns) $(cmse-lib-objs_ns) $(lib-objs_ns) $(tensai_lib_ns) -lgcc -lc -lm -o $@
	$(CHUNK_TOOLS) -o $(PREBUILT) -ms $(target-outputs_ns) -p 0

$(siz-obj_ns): $(target-outputs_ns)
	@$(MCUSIZE) --format=berkeley $< > $@

$(bin-obj_ns): $(target-outputs_ns) $(siz-obj_ns)
	@$(MCUOBJDUMP) -D -S $(target-outputs_ns) > $(target-outputs_dis) 
	@$(MCUOBJCOPY) -v -S -g -j .text -j .reclaim -j .second_text -j .ARM.extab -j .ARM.exidx -j .gnu.sgstubs -j .system_layout -j .data -j .rodata -j .romtable -O binary  $(target-outputs_ns) $@
ifeq ($(CONFIG_M33_LINKING_SYSMEM),y)
	@$(MCUOBJCOPY) -v -S -g -j .sm_data -j .sm_rodata -O binary $(target-outputs_ns) $(PREBUILT)/mcu_ns2.bin
endif
ifeq ($(CONFIG_CHIP_VERSION_B0),y)
	@$(MCUOBJCOPY) -v -S -g -j .ext_text -O binary $(target-outputs_ns) $(PREBUILT)/mcu_ns3.bin
endif

$(build-dir_ns):
	@mkdir -p $@

post_build:
ifeq ($(CONFIG_SDK_MCU_EXPOSE_GDB),y)
	@python $(srctree)/etc/debug_script.py $(PREBUILT) gdb_init_noreset.ini $(CROSS)
endif

clean:
	@$(RM) -rf $(clean-obj)
	@$(RM) $(PREBUILT)/*.bin
