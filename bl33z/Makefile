#
# Copyright (C) 2014-2017 Amlogic, Inc. All rights reserved.
#
# This software is provided to you pursuant to Software License Agreement
# (SLA) with Amlogic Inc ("Amlogic"). This software may be used
# only in accordance with the terms of this agreement.
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions and derivatives of the Software.
#
# Redistribution and use in source and binary forms, with or without
# modification is strictly prohibited without prior written permission from
# Amlogic.
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT # LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

CROSS_COMPILE ?=/opt/gcc-linaro-7.3.1-2018.05-i686_aarch64-elf/bin/aarch64-elf-
export CROSS_COMPILE
################################################################################
# Toolchain
################################################################################

CC			:=	${CROSS_COMPILE}gcc
CPP			:=	${CROSS_COMPILE}cpp
AS			:=	${CROSS_COMPILE}gcc
AR			:=	${CROSS_COMPILE}ar
LD			:=	${CROSS_COMPILE}ld
OC			:=	${CROSS_COMPILE}objcopy
OD			:=	${CROSS_COMPILE}objdump
NM			:=	${CROSS_COMPILE}nm
PP			:=	${CROSS_COMPILE}gcc -E

ASFLAGS_aarch64		=	-mgeneral-regs-only
TF_CFLAGS_aarch64	=	-mgeneral-regs-only -mstrict-align

ASFLAGS_aarch32		=	-march=armv8-a
TF_CFLAGS_aarch32	=	-march=armv8-a

ASFLAGS			+= 	-nostdinc -ffreestanding -Wa,--fatal-warnings	\
				-Werror -Wmissing-include-dirs			\
				-D__ASSEMBLY__ $(ASFLAGS_$(ARCH))		\
				${DEFINES} ${INCLUDES}

TF_CFLAGS		+= 	-nostdinc -ffreestanding -Wall			\
				-Werror -Wmissing-include-dirs			\
				-std=c99 -c -Os					\
				$(TF_CFLAGS_$(ARCH))				\
				${DEFINES} ${INCLUDES}
TF_CFLAGS		+=	-ffunction-sections -fdata-sections

LDFLAGS			+=	--fatal-warnings -O1
LDFLAGS			+=	--gc-sections


ARMCC_$(CONFIG_ENABLE_ARMCC_DEBUGROM)=$(shell echo __CC_ARM | armcc -E -  | tail -n 1)
ifneq ($(ARMCC_y),)
targetComplier=arm
else
targetComplier=gcc
endif

MAKEFLAGS += --no-print-directory
.PHONY: all clean realclean distclean

PLAT	=
$(info Building PLAT = ${PLAT})
SOC		=	$(shell echo ${PLAT} | cut -d '_' -f 1)

ifeq ($(SOC),s4d)
SOC		=	s4
endif
ifeq ($(SOC),t5d)
SOC		=	t5
endif
ifeq ($(SOC),t5w)
SOC		=	t5
endif

list="sc2 s4 t3 t7 p1 a1 a5"
ifeq ($(findstring $(SOC), $(list)), $(SOC))
$(info support the soc: $(SOC))
else
$(info soc support list: $(list))
$(error soc($(SOC)) is not support, exit!!!)
endif

all:
	$(Q)$(MAKE) -f Makefile.$(targetComplier) PLAT=${SOC}

realclean distclean:
	$(Q)$(MAKE) -f Makefile.$(targetComplier) PLAT=${PLAT} distclean
