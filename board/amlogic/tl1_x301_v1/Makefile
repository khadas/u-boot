
obj-y += $(BOARD).o eth_setup.o
obj-$(CONFIG_AML_LCD) += lcd.o
obj-$(CONFIG_AML_LCD_TCON) += lcd_tconless_spi_data.o

ifeq ($(CONFIG_MTK_BT_USB),y)
obj-y += ./mtk-bt/LD_btmtk_usb.o
obj-y += ./mtk-bt/LD_usbbt.o
obj-y += ./mtk-bt/cmd_btmtk.o
obj-y += ./mtk-bt/fs_platform.o
endif