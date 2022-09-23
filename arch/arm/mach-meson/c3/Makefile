
obj-y	:= sdio.o pinctrl_init.o
obj-y   += bl31_apis.o
obj-y	+= cpu_reset.o
obj-y	+= timer.o
obj-y  += stick_mem.o
obj-y	+= mailbox.o
#obj-y   += gate_init.o
#obj-y   += power_cal.o
obj-$(CONFIG_CMD_PLLTEST)	+= pll.o
obj-$(CONFIG_CMD_AML_MTEST) += core.o
obj-y += usb.o
obj-$(CONFIG_AML_TSENSOR) += tsensor.o
obj-$(CONFIG_AML_OSCRING) += oscring.o
obj-$(CONFIG_AML_HDMITX20) += hdmitx_hw.o hdmitx_clk.o
obj-y += aml_efuse.o
obj-$(CONFIG_SECURE_POWER_CONTROL) += pwrc.o
