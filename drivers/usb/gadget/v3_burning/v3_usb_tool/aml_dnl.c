/*
 * \file        aml_dnl.c
 * \brief       from g_dnl.c  USB Downloader Gadget
 *
 * \version     1.0.0
 * \date        2019/05/16
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2019 Amlogic. All Rights Reserved.
 *
 */
#include <common.h>
#include <malloc.h>

#include <mmc.h>
#include <part.h>
#include <usb.h>

#include <g_dnl.h>
#include <usb_mass_storage.h>
#include <dfu.h>
#include <thor.h>

#include <env_callback.h>

#include "../../gadget_chips.h"
///#include "composite.c" //not need include twice

/*
 * One needs to define the following:
 * CONFIG_USB_GADGET_VENDOR_NUM
 * CONFIG_USB_GADGET_PRODUCT_NUM
 * CONFIG_USB_GADGET_MANUFACTURER
 * at e.g. ./configs/<board>_defconfig
 */
#define CONFIG_USB_GADGET_VENDOR_NUM 0x1b8e
#define CONFIG_USB_GADGET_PRODUCT_NUM 0xc004
#define CONFIG_USB_GADGET_MANUFACTURER  "Amlogic Inc"

#define STRING_MANUFACTURER 25
#define STRING_PRODUCT 2
/* Index of String Descriptor describing this configuration */
#define STRING_USBDOWN 2
/* Index of String serial */
#define STRING_SERIAL  3
#define MAX_STRING_SERIAL	256
/* Number of supported configurations */
#define CONFIGURATION_NUMBER 1

#define DRIVER_VERSION		"aml dnl 1.0"

static const char product[] = "DNL";
static char g_dnl_serial[MAX_STRING_SERIAL];
static const char manufacturer[] = CONFIG_USB_GADGET_MANUFACTURER;

static struct usb_device_descriptor device_desc = {
	.bLength = sizeof device_desc,
	.bDescriptorType = USB_DT_DEVICE,

	.bcdUSB = __constant_cpu_to_le16(0x0200),
	.bDeviceClass = USB_CLASS_PER_INTERFACE,
	.bDeviceSubClass = 0, /*0x02:CDC-modem , 0x00:CDC-serial*/

	.idVendor = __constant_cpu_to_le16(CONFIG_USB_GADGET_VENDOR_NUM),
	.idProduct = __constant_cpu_to_le16(CONFIG_USB_GADGET_PRODUCT_NUM),
	/* .iProduct = DYNAMIC */
	/* .iSerialNumber = DYNAMIC */
	.bNumConfigurations = 1,
};

/*
 * static strings, in UTF-8
 * IDs for those strings are assigned dynamically at g_dnl_bind()
 */
static struct usb_string g_dnl_string_defs[] = {
	{.s = manufacturer},
	{.s = product},
	{.s = g_dnl_serial},
	{ }		/* end of list */
};

static struct usb_gadget_strings g_dnl_string_tab = {
	.language = 0x0409, /* en-us */
	.strings = g_dnl_string_defs,
};

static struct usb_gadget_strings *g_dnl_composite_strings[] = {
	&g_dnl_string_tab,
	NULL,
};

#include <amlogic/cpu_id.h>
const char * get_usid_string(void)
{
    static char chipid_str[32];
	unsigned char chipid[16];
	int ret = get_chip_id(chipid, 16);
	if ( ret ) {
		printf("_get_chipid %d", ret);
		return NULL;
	}
	char* buff = &chipid_str[0];
	buff[0] = buff[24] = '\0';
	int i = 0;
	for (; i < 12; ++i) {
		sprintf(buff, "%s%02x", buff, chipid[15-i]);
	}
	return buff;
}

static int g_dnl_unbind(struct usb_composite_dev *cdev)
{
	struct usb_gadget *gadget = cdev->gadget;

	debug("%s: calling usb_gadget_disconnect for "
			"controller '%s'\n", __func__, gadget->name);
	usb_gadget_disconnect(gadget);

	return 0;
}

static inline struct g_dnl_bind_callback *g_dnl_bind_callback_first(void)
{
	return ll_entry_start(struct g_dnl_bind_callback,
				g_dnl_bind_callbacks);
}

static inline struct g_dnl_bind_callback *g_dnl_bind_callback_end(void)
{
	return ll_entry_end(struct g_dnl_bind_callback,
				g_dnl_bind_callbacks);
}

static int g_dnl_do_config(struct usb_configuration *c)
{
	const char *s = c->cdev->driver->name;
	struct g_dnl_bind_callback *callback = g_dnl_bind_callback_first();

	debug("%s: configuration: 0x%p composite dev: 0x%p\n",
	      __func__, c, c->cdev);

	for (; callback != g_dnl_bind_callback_end(); callback++)
		if (!strcmp(s, callback->usb_function_name))
			return callback->fptr(c);
	return -ENODEV;
}

static int g_dnl_config_register(struct usb_composite_dev *cdev)
{
	struct usb_configuration *config;
	const char *name = "usb_dnload";

	config = memalign(CONFIG_SYS_CACHELINE_SIZE, sizeof(*config));
	if (!config)
		return -ENOMEM;

	memset(config, 0, sizeof(*config));

	config->label = name;
	config->bmAttributes = USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER;
	config->bConfigurationValue = CONFIGURATION_NUMBER;
	config->iConfiguration = STRING_USBDOWN;
	config->bind = g_dnl_do_config;

	return usb_add_config(cdev, config);
}

static int g_dnl_get_bcd_device_number(struct usb_composite_dev *cdev)
{
	struct usb_gadget *gadget = cdev->gadget;
	int gcnum;

	gcnum = usb_gadget_controller_number(gadget);
	if (gcnum > 0)
		gcnum += 0x200;

	/*return g_dnl_get_board_bcd_device_number(gcnum);*/
    return gcnum;
}

static int g_dnl_bind(struct usb_composite_dev *cdev)
{
	struct usb_gadget *gadget = cdev->gadget;
	int id, ret;
	int gcnum;

	debug("%s: gadget: 0x%p cdev: 0x%p\n", __func__, gadget, cdev);

	id = usb_string_id(cdev);

	if (id < 0)
		return id;
	g_dnl_string_defs[0].id = id;
	device_desc.iManufacturer = id;

	id = usb_string_id(cdev);
	if (id < 0)
		return id;

	g_dnl_string_defs[1].id = id;
	device_desc.iProduct = id;

	/*g_dnl_bind_fixup(&device_desc, cdev->driver->name);*/

    id = usb_string_id(cdev);
    if (id < 0)
        return id;
    g_dnl_string_defs[2].id = id;
    device_desc.iSerialNumber = id;

    const char* s = get_usid_string();
    if (s) strncpy(g_dnl_serial, s, strlen(s));
    else printf("Fail in get chipid\n");

	ret = g_dnl_config_register(cdev);
	if (ret)
		goto error;

	gcnum = g_dnl_get_bcd_device_number(cdev);
	if (gcnum >= 0)
		device_desc.bcdDevice = cpu_to_le16(gcnum);
	else {
		debug("%s: controller '%s' not recognized\n",
			__func__, gadget->name);
		device_desc.bcdDevice = __constant_cpu_to_le16(0x9999);
	}

	debug("%s: calling usb_gadget_connect for "
			"controller '%s'\n", __func__, gadget->name);
	usb_gadget_connect(gadget);

	return 0;

 error:
	g_dnl_unbind(cdev);
	return -ENOMEM;
}

static struct usb_composite_driver g_dnl_driver = {
	.name = NULL,
	.dev = &device_desc,
	.strings = g_dnl_composite_strings,

	.bind = g_dnl_bind,
	.unbind = g_dnl_unbind,
};

/*
 * NOTICE:
 * Registering via USB function name won't be necessary after rewriting
 * g_dnl to support multiple USB functions.
 */
int aml_dnl_register(const char *name)
{
	int ret;

	debug("%s: g_dnl_driver.name = %s\n", __func__, name);
	g_dnl_driver.name = name;

	ret = usb_composite_register(&g_dnl_driver);
	if (ret) {
		printf("%s: failed!, error: %d\n", __func__, ret);
		return ret;
	}
	return 0;
}

void aml_dnl_unregister(void)
{
	usb_composite_unregister(&g_dnl_driver);
}
