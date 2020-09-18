EXTRA_CFLAGS = -I./include

obj-$(CONFIG_AML_V3_FACTORY_BURN) += aml_v3_burning.o
obj-$(CONFIG_AML_V3_FACTORY_BURN) += store_wrapper.o
obj-$(CONFIG_AML_V3_FACTORY_BURN) += v3_common/
ifdef CONFIG_AML_V3_FACTORY_BURN
ifdef CONFIG_USBDOWNLOAD_GADGET
	obj-$(CONFIG_AML_V3_USB_TOOl) += v3_usb_tool/
endif# ifdef CONFIG_USBDOWNLOAD_GADGET
obj-$(CONFIG_AML_V3_SDC_BURN) += v3_sdc_burn/
obj-$(CONFIG_AML_V3_USB_BURN) += v3_usb_burn/
endif #ifdef CONFIG_AML_V3_FACTORY_BURN

