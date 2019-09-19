# SPDX-License-Identifier: GPL-2.0+
#

obj-$(CONFIG_AML_VPU) += cmd_vpu.o
obj-$(CONFIG_AML_VPP) += cmd_vpp.o
obj-$(CONFIG_AML_VOUT) += cmd_vout.o
obj-$(CONFIG_AML_CVBS) += cmd_cvbs.o
obj-$(CONFIG_AML_HDMITX) += cmd_hdmitx.o
obj-$(CONFIG_AML_LCD) += cmd_lcd.o
obj-$(CONFIG_AML_OSD) += cmd_osd.o
obj-$(CONFIG_CMD_PLLTEST) += cmd_plltest.o

obj-$(CONFIG_AML_STORE_WRAPPER) += store_wrapper.o
obj-$(CONFIG_AML_IMG_READ) += imgread.o
obj-$(CONFIG_AML_DEFENV) += defenv_without.o

#obj-y += cmd_reboot.o
obj-$(CONFIG_CMD_RSVMEM) += cmd_rsvmem.o

ifdef CONFIG_BOOTLOADER_CONTROL_BLOCK
obj-y += cmd_bcb.o
obj-y += get_bcb_boot_part.o
ifdef CONFIG_CMD_BOOTCTOL_AVB
obj-y += cmd_bootctl_avb.o
else
obj-y += cmd_bootctl.o
endif
endif
ifndef CONFIG_AML_RPMB_DISABLE
#obj-y += cmd_rpmb.o
endif
obj-$(CONFIG_CMD_BOOTCTOL_AVB) += cmd_avb.o
obj-$(CONFIG_CMD_DDR_D2PLL) += cmd_d2pll.o
obj-$(CONFIG_CMD_DDR_TEST) += cmd_ddr_test_g12.o

obj-$(CONFIG_CMD_OS_IDENT) += os_ident.o
