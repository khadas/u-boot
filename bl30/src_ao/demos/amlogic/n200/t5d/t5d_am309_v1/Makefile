#
#  Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
#
#  All information contained herein is Amlogic confidential.
#
#  This software is provided to you pursuant to Software License Agreement
#  (SLA) with Amlogic Inc ("Amlogic"). This software may be used
#  only in accordance with the terms of this agreement.
#
#  Redistribution and use in source and binary forms, with or without
#  modification is strictly prohibited without prior written permission from
#  Amlogic.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
include config.mk

CUR_DIR := ${dir ${abspath ${lastword ${MAKEFILE_LIST}}}}
driver_base=$(shell cd ../../../;pwd)
export FREERTOS_ROOT_DIR := ../../../../..
export FREERTOS_VENDOR :=$(FREERTOS_ROOT_DIR)/demos/amlogic
export FREERTOS_VENDOR_DRIVER := $(driver_base)/driver
export FREERTOS_VENDOR_COMMON := $(FREERTOS_VENDOR)/common
export FREERTOS_LIB_DIR := $(FREERTOS_ROOT_DIR)/lib
export FREERTOS_CORE_DIR := $(FREERTOS_LIB_DIR)/FreeRTOS/portable/GCC/$(ARCH_CPU)
#export FREERTOS_LIB_THIRD_PARTY := $(FREERTOS_LIB_DIR)/third_party/amlogic/$(COMPILER)/libdrivers.a
#SCATTER_RTOSDemo=../standalone.ld
SCATTER_RTOSDemo=$(COMPILER)/lscript
BINNAME=RTOSDemo

#$(info $(FREERTOS_CORE_DIR) $(CUR_DIR) $(__GNUC__))
include $(FREERTOS_VENDOR)/makedefs

RTOS_SOURCE_DIR = ${FREERTOS_LIB_DIR}/FreeRTOS
OUTPUT := ./out_${SOC}

CFLAGS += $(WARNINGS) -DN200_REVA=1 -I . -I./$(FREERTOS_VENDOR)/include -I ${FREERTOS_VENDOR}/n200/include -I ${FREERTOS_VENDOR}/n200/include/${SOC} -I ${FREERTOS_CORE_DIR} -I $(FREERTOS_ROOT_DIR)/lib/include -I $(FREERTOS_ROOT_DIR)/lib/third_party/amlogic/include/drivers -I $(FREERTOS_ROOT_DIR)/lib/third_party/amlogic/include -I ${FREERTOS_LIB_DIR}/include/private

AFLAGS = $(CFLAGS) -D__ASM

CC_PATH = $(shell which $(CC) | xargs dirname )
CC_VERSION = $(shell $(CC) -dumpversion)

LDFLAGS += -L$(CC_PATH)/../lib/gcc/riscv-none-embed/$(CC_VERSION)/rv32im/ilp32 -lgcc --gc-sections
$(info LDFLAGS = $(LDFLAGS))
#LDFLAGS += -L/opt/riscv-none-gcc/7.2.0-4-20180606-1631/riscv-none-embed/lib/rv32im/ilp32 -lm -lc \
	-L/opt/riscv-none-gcc/7.2.0-4-20180606-1631/lib/gcc/riscv-none-embed/7.2.0/rv32im/ilp32 -lgcc

-include $(FREERTOS_VENDOR_DRIVER)/uart/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/mailbox/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/get_version/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/gpio/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/cec/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/pwm/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/leds/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/str/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/vrtc/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/timer_source/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/ir/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/saradc/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/ddr/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/eth/build.mk
-include $(FREERTOS_VENDOR_DRIVER)/stick_mem/build.mk

objs_from_dir=$(sort $(foreach obj, $($(2)-y), \
                $(COMPILER)/$(1)/$(firstword $($(2)-mock-$(PROJECT)-$(obj)) $(obj))))

dirs+=$(shell find $(FREERTOS_VENDOR_DRIVER) -type d)
_dir_create := $(foreach d,$(dirs),$(shell [ -d $(COMPILER)/$(d) ] || \
	mkdir -p $(COMPILER)/$(d)))

dirs=$(dir $(shell find $(FREERTOS_VENDOR_DRIVER) -name "build.mk"))
all-y +=  $(foreach d,$(dirs), $(call objs_from_dir,$(d),$(lastword $(subst /, ,$(d)))))
OBJS += $(all-y)

$(info SOC=${SOC} BOARD_DIR=$(BOARD_DIR) FREERTOS_ROOT_DIR=${FREERTOS_ROOT_DIR} RTOS_SOURCE_DIR=${RTOS_SOURCE_DIR})
VPATH += ${RTOS_SOURCE_DIR}:${RTOS_SOURCE_DIR}/portable/MemMang:${FREERTOS_CORE_DIR}:$(FREERTOS_VENDOR_COMMON):$(FREERTOS_VENDOR)/n200:$(FREERTOS_VENDOR)/n200/${SOC}

OBJS += ${COMPILER}/main.o	\
		${COMPILER}/power.o     \
		$(COMPILER)/fsm.o    \
		$(COMPILER)/list.o    \
		$(COMPILER)/queue.o   \
		$(COMPILER)/tasks.o   \
		$(COMPILER)/timers.o  \
		$(COMPILER)/port.o    \
		$(COMPILER)/portasm_reva.o \
		$(COMPILER)/heap_5.o \
		$(COMPILER)/n200_func.o \
		$(COMPILER)/handlers.o \
		$(COMPILER)/interrupt_control.o \
		$(COMPILER)/util.o  \
		$(COMPILER)/myprintf.o \
		$(COMPILER)/start.o \
		$(COMPILER)/vtable.o

#
# The default rule, which causes init to be built.
#
all: $(COMPILER)           \
	$(COMPILER)/$(BINNAME).axf
#	out

#
# The rule to clean out all the build products
#

clean:
	@rm -rf ${COMPILER} ${wildcard *.bin} RTOSDemo.axf

#
# The rule to create the target directory
#
${COMPILER}:
	@mkdir ${COMPILER}

${SCATTER_RTOSDemo}: ${FREERTOS_VENDOR}/n200/standalone.ld
	@${CC} -E -xc -P ${CFLAGS} -o ${@} ${<}

${COMPILER}/$(BINNAME).axf: ${OBJS} $(SCATTER_RTOSDemo)
	$(LD) $(OBJS) $(LDFLAGS) -T $(SCATTER_RTOSDemo) -Map $(COMPILER)/RTOSDemo.map -o $@
	@${OBJCOPY} -O binary ${@} ${@:.axf=.bin}
	@$(OBJDUMP) -D ${@} > ${@:.axf=.dis}
#	@$(FREERTOS_VENDOR)/tools/padfile.sh ${COMPILER}/$(BINNAME).bin
#	@$(FREERTOS_VENDOR)/tools/pack-secpu-loader.py --output=${COMPILER}/$(BINNAME)_pad.bin \
		--seg-id=0 --arb-vers=0 ${COMPILER}/$(BINNAME).bin


-include $(wildcard $(COMPILER)/*.d)
