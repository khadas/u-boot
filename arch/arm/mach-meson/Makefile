# SPDX-License-Identifier: GPL-2.0+
#
# Copyright (c) 2016 Beniamino Galvani <b.galvani@gmail.com>

#obj-y += board-common.o sm.o
obj-y += cpu_id.o
obj-$(CONFIG_MESON_GX) += board-gx.o
obj-$(CONFIG_MESON_AXG) += board-axg.o

obj-$(CONFIG_ARCH_MESON) += $(SOC)/

obj-$(CONFIG_AML_HW_SHA2) += hard_sha2.o
obj-$(CONFIG_CMD_PLLTEST) += clk-pll.o clk-mpll.o

ifdef CONFIG_AML_AVB2_ANTIROLLBACK
obj-y += avb2-anti-rollback.o
else
obj-$(CONFIG_AML_ANTIROLLBACK) += anti-rollback.o
endif
