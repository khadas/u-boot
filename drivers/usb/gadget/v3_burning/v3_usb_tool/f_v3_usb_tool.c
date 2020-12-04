/*
 * \file        f_v3_usb_tool.c
 * \brief
 *
 * \version     1.0.0
 * \date        2018/04/08
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2018 Amlogic. All Rights Reserved.
 *
 */
//declares from f_fastboot.c
#include <config.h>
#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/usb/composite.h>
#include <linux/compiler.h>
#include <version.h>
#include <g_dnl.h>
#include <asm/arch/cpu.h>
#include <partition_table.h>
#include <android_image.h>
#include <image.h>
#include <amlogic/cpu_id.h>
#include "../include/v3_tool_def.h"
DECLARE_GLOBAL_DATA_PTR;
static void cb_aml_media_write(struct usb_ep *ep, struct usb_request *req);
static void cb_aml_media_read(struct usb_ep *outep, struct usb_request *outreq);
static void cb_oem_cmd(struct usb_ep *ep, struct usb_request *req);

#define DNL_PROTOCOL_VERSION		"0.1"
#define FASTBOOT_INTERFACE_CLASS	0xff
#define FASTBOOT_INTERFACE_SUB_CLASS	0x42
#define FASTBOOT_INTERFACE_PROTOCOL	0x03

#define DEVICE_PRODUCT	"amlogic"
#define DEVICE_SERIAL	"1234567890"

#define CONFIG_USB_FASTBOOT_BUF_ADDR V3_DOWNLOAD_EP_DATA

/* The 64 defined bytes plus \0 */

static struct {
    int         hadDown;    //already downloaded to mem
    unsigned    imgSize;      //size of dtb.img
}_memDtbImg = {0};

struct f_fastboot {
	struct usb_function usb_function;

	/* IN/OUT EP's and corresponding requests */
	struct usb_ep *in_ep, *out_ep;
	struct usb_request *in_req, *out_req;
};

static inline struct f_fastboot *func_to_fastboot(struct usb_function *f)
{
	return container_of(f, struct f_fastboot, usb_function);
}

static struct f_fastboot *fastboot_func;
static unsigned int download_size;
static unsigned int download_bytes;

static struct usb_interface_descriptor interface_desc = {
	.bLength		= USB_DT_INTERFACE_SIZE,
	.bDescriptorType	= USB_DT_INTERFACE,
	.bInterfaceNumber	= 0x00,
	.bAlternateSetting	= 0x00,
	.bNumEndpoints		= 0x02,
	.bInterfaceClass	= FASTBOOT_INTERFACE_CLASS,
	.bInterfaceSubClass	= FASTBOOT_INTERFACE_SUB_CLASS,
	.bInterfaceProtocol	= FASTBOOT_INTERFACE_PROTOCOL,
};

static struct usb_endpoint_descriptor fs_ep_in = {
	.bLength            = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType    = USB_DT_ENDPOINT,
	.bEndpointAddress   = USB_DIR_IN,
	.bmAttributes       = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize     = cpu_to_le16(64),
};

static struct usb_endpoint_descriptor fs_ep_out = {
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_OUT,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= cpu_to_le16(64),
};

static struct usb_endpoint_descriptor hs_ep_in = {
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_IN,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= cpu_to_le16(512),
};

static struct usb_endpoint_descriptor hs_ep_out = {
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_OUT,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= cpu_to_le16(512),
};


static struct usb_descriptor_header *fb_fs_function[] = {
	(struct usb_descriptor_header *)&interface_desc,
	(struct usb_descriptor_header *)&fs_ep_in,
	(struct usb_descriptor_header *)&fs_ep_out,
};

static struct usb_descriptor_header *fb_hs_function[] = {
	(struct usb_descriptor_header *)&interface_desc,
	(struct usb_descriptor_header *)&hs_ep_in,
	(struct usb_descriptor_header *)&hs_ep_out,
	NULL,
};

static struct usb_endpoint_descriptor *
fb_ep_desc(struct usb_gadget *g, struct usb_endpoint_descriptor *fs,
	    struct usb_endpoint_descriptor *hs)
{
	if (gadget_is_dualspeed(g) && g->speed == USB_SPEED_HIGH)
		return hs;
	return fs;
}

/*
 * static strings, in UTF-8
 */
static const char fastboot_name[] = "Amlogic DNL";

static struct usb_string fastboot_string_defs[] = {
	[0].s = fastboot_name,
	{  }			/* end of list */
};

static struct usb_gadget_strings stringtab_fastboot = {
	.language	= 0x0409,	/* en-us */
	.strings	= fastboot_string_defs,
};

static struct usb_gadget_strings *fastboot_strings[] = {
	&stringtab_fastboot,
	NULL,
};

#if 0
#define DRAM_UBOOT_RESERVE		0x01000000
static unsigned int ddr_size_usable(unsigned int addr_start)
{
	unsigned int ddr_size=0;
	unsigned int free_size = 0;
	int i;

	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++)
		ddr_size += gd->bd->bi_dram[i].size;

	free_size = (ddr_size - DRAM_UBOOT_RESERVE - addr_start - CONFIG_SYS_MALLOC_LEN - CONFIG_SYS_MEM_TOP_HIDE);
#if defined CONFIG_FASTBOOT_MAX_DOWN_SIZE
	if (free_size > CONFIG_FASTBOOT_MAX_DOWN_SIZE)
		free_size = CONFIG_FASTBOOT_MAX_DOWN_SIZE;
#endif
	return free_size;
}
#endif

static void rx_handler_command(struct usb_ep *ep, struct usb_request *req);

static char response_str[RESPONSE_LEN + 1];

static void fastboot_fail(const char *s)
{
	strncpy(response_str, "FAIL", 4);
	if (s)strncat(response_str, s, RESPONSE_LEN - 4 - 1) ;
}

static void fastboot_okay(const char *s)
{
	strncpy(response_str, "OKAY", 4);
	if (s)strncat(response_str, s, RESPONSE_LEN - 4 - 1) ;
}

static void fastboot_busy(const char* s)
{
	strncpy(response_str, "INFO", 4 + 1);//add terminated 0
	if (s)strncat(response_str, s, RESPONSE_LEN - 4 - 1) ;
}
static int fastboot_is_busy(void)
{
	return !strncmp("INFO", response_str, strlen("INFO"));
}

//cb for bulk in_req->complete
static void fastboot_complete(struct usb_ep *ep, struct usb_request *req)
{
	int status = req->status;

	if ( fastboot_is_busy() && fastboot_func) {
		struct usb_ep* out_ep = fastboot_func->out_ep;
		struct usb_request* out_req = fastboot_func->out_req;
		rx_handler_command(out_ep, out_req);
		return;
	}
	if (!status)
		return;
	printf("status: %d ep '%s' trans: %d\n", status, ep->name, req->actual);
}

static int fastboot_bind(struct usb_configuration *c, struct usb_function *f)
{
	int id;
	struct usb_gadget *gadget = c->cdev->gadget;
	struct f_fastboot *f_fb = func_to_fastboot(f);

	/* DYNAMIC interface numbers assignments */
	id = usb_interface_id(c, f);
	if (id < 0)
		return id;

	interface_desc.bInterfaceNumber = id;

	id = usb_string_id(c->cdev);
	if (id < 0)
		return id;

	fastboot_string_defs[0].id = id;
	interface_desc.iInterface = id;

	f_fb->in_ep = usb_ep_autoconfig(gadget, &fs_ep_in);
	if (!f_fb->in_ep)
		return -ENODEV;

	f_fb->in_ep->driver_data = c->cdev;

	f_fb->out_ep = usb_ep_autoconfig(gadget, &fs_ep_out);
	if (!f_fb->out_ep)
		return -ENODEV;

	f_fb->out_ep->driver_data = c->cdev;

	f->descriptors = fb_fs_function;

	if (gadget_is_dualspeed(gadget)) {
		/* Assume endpoint addresses are the same for both speeds */
		hs_ep_in.bEndpointAddress = fs_ep_in.bEndpointAddress;
		hs_ep_out.bEndpointAddress = fs_ep_out.bEndpointAddress;
		/* copy HS descriptors */
		f->hs_descriptors = fb_hs_function;
	}

	return 0;
}

static void fastboot_unbind(struct usb_configuration *c, struct usb_function *f)
{
	memset(fastboot_func, 0, sizeof(*fastboot_func));
}

static void fastboot_disable(struct usb_function *f)
{
	struct f_fastboot *f_fb = func_to_fastboot(f);

	usb_ep_disable(f_fb->out_ep);
	usb_ep_disable(f_fb->in_ep);

	if (f_fb->out_req) {
		usb_ep_free_request(f_fb->out_ep, f_fb->out_req);
		f_fb->out_req = NULL;
	}
	if (f_fb->in_req) {
		usb_ep_free_request(f_fb->in_ep, f_fb->in_req);
		f_fb->in_req = NULL;
	}
}

#define EP_CMD_LEN_MAX 256
static char EP_CMD_BUF[EP_CMD_LEN_MAX*2];
static struct usb_request *fastboot_start_ep(struct usb_ep *ep)
{
	struct usb_request *req;
	const char* epName = ep->name;

	req = usb_ep_alloc_request(ep, 0);
	if (!req)
		return NULL;

	const int isBulkOut = strnlen(epName, 12) == strlen("ep1out");
	/*req->length = EP_BUFFER_SIZE;*/
	/*req->buf = isBulkOut ? (char*)V3_DOWNLOAD_EP_OUT : (char*)V3_DOWNLOAD_EP_IN;*/
	req->buf = isBulkOut ? &EP_CMD_BUF[0] : &EP_CMD_BUF[EP_CMD_LEN_MAX];
	req->length = EP_CMD_LEN_MAX;
	FB_DBG("start %s EP, [%p]\n", isBulkOut ? "OUT" : "IN", req->buf);

	memset(req->buf, 0, req->length);
	return req;
}

extern unsigned int adnl_enum_timeout;
extern unsigned int adnl_identify_timeout;

static int fastboot_set_alt(struct usb_function *f,
			    unsigned interface, unsigned alt)
{
	int ret;
	struct usb_composite_dev *cdev = f->config->cdev;
	struct usb_gadget *gadget = cdev->gadget;
	struct f_fastboot *f_fb = func_to_fastboot(f);
	const struct usb_endpoint_descriptor *d;

	debug("%s: func: %s intf: %d alt: %d\n",
	      __func__, f->name, interface, alt);

	/* make sure we don't enable the ep twice */
	d = fb_ep_desc(gadget, &fs_ep_out, &hs_ep_out);
	ret = usb_ep_enable(f_fb->out_ep, d);
	if (ret) {
		puts("failed to enable out ep\n");
		return ret;
	}

	f_fb->out_req = fastboot_start_ep(f_fb->out_ep);
	if (!f_fb->out_req) {
		puts("failed to alloc out req\n");
		ret = -EINVAL;
		goto err;
	}
	f_fb->out_req->complete = rx_handler_command;

	d = fb_ep_desc(gadget, &fs_ep_in, &hs_ep_in);
	ret = usb_ep_enable(f_fb->in_ep, d);
	if (ret) {
		puts("failed to enable in ep\n");
		goto err;
	}

	f_fb->in_req = fastboot_start_ep(f_fb->in_ep);
	if (!f_fb->in_req) {
		puts("failed alloc req in\n");
		ret = -EINVAL;
		goto err;
	}
	f_fb->in_req->complete = fastboot_complete;

	ret = usb_ep_queue(f_fb->out_ep, f_fb->out_req, 0);
	if (ret)
		goto err;

	adnl_enum_timeout = 0;
	adnl_identify_timeout = get_timer(0);

	return 0;
err:
	fastboot_disable(f);
	return ret;
}

static int  fastboot_setup(struct usb_function *f,
	const struct usb_ctrlrequest *ctrl)
{
	int value = -EOPNOTSUPP;
	struct f_fastboot *f_fb = func_to_fastboot(f);

	/* composite driver infrastructure handles everything; interface
	 * activation uses set_alt().
	 */
	if (((ctrl->bRequestType & USB_RECIP_MASK) == USB_RECIP_ENDPOINT)
		&& (ctrl->bRequest == USB_REQ_CLEAR_FEATURE)
		&& (ctrl->wValue== USB_ENDPOINT_HALT)) {
		switch (ctrl->wIndex & 0xfe) {
		case USB_DIR_OUT:
			value = ctrl->wLength;
			usb_ep_clear_halt(f_fb->out_ep);
			break;

		case USB_DIR_IN:
			value = ctrl->wLength;
			usb_ep_clear_halt(f_fb->in_ep);
			break;
		default:
			printf("unknown usb_ctrlrequest\n");
			break;
		}
	}

	return value;
}

static int fastboot_add(struct usb_configuration *c)
{
	struct f_fastboot *f_fb = fastboot_func;
	int status;

	if (!f_fb) {
		f_fb = memalign(CONFIG_SYS_CACHELINE_SIZE, sizeof(*f_fb));
		if (!f_fb)
			return -ENOMEM;

		fastboot_func = f_fb;
		memset(f_fb, 0, sizeof(*f_fb));
	}

	f_fb->usb_function.name = "f_aml_dnl";
	/*f_fb->usb_function.hs_descriptors = fb_runtime_descs;*/
	f_fb->usb_function.bind = fastboot_bind;
	f_fb->usb_function.unbind = fastboot_unbind;
	f_fb->usb_function.set_alt = fastboot_set_alt;
	f_fb->usb_function.disable = fastboot_disable;
	f_fb->usb_function.strings = fastboot_strings;
	f_fb->usb_function.setup = fastboot_setup;

	status = usb_add_function(c, &f_fb->usb_function);
	if (status) {
		free(f_fb);
		fastboot_func = f_fb;
	}

	return status;
}
DECLARE_GADGET_BIND_CALLBACK(usb_dnl_amlogic, fastboot_add);

static int fastboot_tx_write(const char *buffer, unsigned int buffer_size)
{
	struct usb_request *in_req = fastboot_func->in_req;
	int ret;

	memcpy(in_req->buf, buffer, buffer_size);
	in_req->length = buffer_size;
	ret = usb_ep_queue(fastboot_func->in_ep, in_req, 0);
	if (ret)
		printf("Error %d on queue\n", ret);
	return 0;
}

static int fastboot_tx_write_str(const char *buffer)
{
	return fastboot_tx_write(buffer, strlen(buffer));
}

static void compl_do_reset(struct usb_ep *ep, struct usb_request *req)
{
    f_dwc_otg_pullup(0);//disconnect before reboot/plugin to enhance pc compatibility
    udelay(2*1000*1000);
	do_reset(NULL, 0, 0, NULL);
}

static void compl_do_reboot_bootloader(struct usb_ep *ep, struct usb_request *req)
{
	run_command("reboot bootloader", 0);
}
static void compl_do_reboot_bl1usb(struct usb_ep *ep, struct usb_request *req)
{
    f_dwc_otg_pullup(0);//disconnect before reboot/plugin to enhance pc compatibility
    udelay(2*1000*1000);
    optimus_erase_bootloader("usb");//skip to bl1 usb rom driver
	do_reset(NULL, 0, 0, NULL);
}

static void cb_reboot(struct usb_ep *ep, struct usb_request *req)
{
	char *cmd = req->buf;

	printf("cmd cb_reboot is %s\n", cmd);

    void (*do_after_bulk_in)(struct usb_ep*, struct usb_request*) = NULL;
	strsep(&cmd, "-");
	if (!cmd) {
		do_after_bulk_in = compl_do_reset;
	} else if (!strcmp("bootloader", cmd)) {
        do_after_bulk_in = compl_do_reboot_bootloader;
    } else if (!strcmp("romusb", cmd)) {
        do_after_bulk_in = compl_do_reboot_bl1usb;
    } else {
        fastboot_fail("unsupported reboot cmd");
        fastboot_tx_write_str(response_str);
        return;
    }

    printf("reboot subcmd %s\n", cmd);
	fastboot_func->in_req->complete = do_after_bulk_in;
	fastboot_tx_write_str("OKAY");
}

static int strcmp_l1(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return -1;
	return strncmp(s1, s2, strlen(s1));
}

static const char* getvar_list[] = {
	"version", "serialno", "product", "erase-block-size", "secure",
};
static const char* getvar_list_ab[] = {
	"version", "serialno", "product", "erase-block-size",
	"secure", "slot-count", "slot-suffixes","current-slot",
};

extern unsigned int adnl_identify_timeout;

static void cb_getvar(struct usb_ep *ep, struct usb_request *req)
{
	char *cmd = req->buf;
	char cmdBuf[RESPONSE_LEN];
	char* response = response_str;
	size_t chars_left;
	int replyLen = 0;

	strcpy(response, "OKAY");
	chars_left = sizeof(response_str) - strlen(response) - 1;

	memcpy(cmdBuf, cmd, strnlen(cmd, RESPONSE_LEN-1)+1);
	cmd = cmdBuf;
	strsep(&cmd, ":");
	printf("cb_getvar: %s\n", cmd);
	if (!cmd) {
		FBS_ERR(response, "FAILmissing var\n");
		fastboot_tx_write_str(response);
		return;
	}
	if (!strncmp(cmd, "all", 3)) {
		static int cmdIndex = 0;
		int getvar_num;
		if (has_boot_slot == 1) {
			strcpy(cmd, getvar_list_ab[cmdIndex]);
			getvar_num = (sizeof(getvar_list_ab) / sizeof(getvar_list_ab[0]));
		} else {
			strcpy(cmd, getvar_list[cmdIndex]);//only support no-arg cmd
			getvar_num = (sizeof(getvar_list) / sizeof(getvar_list[0]));
		}
		printf("getvar_num: %d\n", getvar_num);
		if ( ++cmdIndex >= getvar_num) cmdIndex = 0;
		else fastboot_busy(NULL);
		FB_MSG("all cmd:%s\n", cmd);
		strncat(response, cmd, chars_left);
		strncat(response, ":", 1);
		chars_left -= strlen(cmd) + 1;
	}

	if (!strcmp_l1("version", cmd)) {
		strncat(response, DNL_PROTOCOL_VERSION, chars_left);
	} else if (!strcmp_l1("bootloader-version", cmd)) {
		strncat(response, U_BOOT_VERSION, chars_left);
	} else if (!strcmp_l1("burnsteps", cmd)) {
		unsigned* steps = (unsigned*)(response + 4);
		FB_DBG("SYSCTRL_STICKY_REG2 addr 0x%x\n", P_PREG_STICKY_REG2);
		*steps = readl(P_PREG_STICKY_REG2);
		fastboot_tx_write(response, 4 + sizeof(unsigned));
		return;
	} else if (!strcmp_l1("identify", cmd)) {
		const int identifyLen = 8;
		char fwVer[] = {5, 0, 0, 16, 0, 0, 0, 0};
		cpu_id_t cpuid = get_cpu_id();
		if (cpuid.family_id >= MESON_CPU_MAJOR_ID_SC2) fwVer[0] = 6;
		memcpy(response + 4, fwVer, identifyLen);
		replyLen = 4 + identifyLen;
		adnl_identify_timeout = 0;
	} else if (!strcmp_l1("secureboot", cmd)) {
		unsigned securebootEnable = 0;
#ifdef CONFIG_EFUSE
		securebootEnable = IS_FEAT_BOOT_VERIFY() ? 1 : 0;
#else
		FB_MSG("Configure efuse not enabled\n");
#endif//#ifdef CONFIG_EFUSE
		memcpy(response + 4, &securebootEnable, sizeof(unsigned));
		replyLen = 4 + sizeof(unsigned);
	} else if (!strcmp_l1("serialno", cmd)) {
		extern const char * get_usid_string(void);
		const char* usid = get_usid_string();
		if (usid) strncat(response, usid, chars_left);
		else strncat(response, DEVICE_SERIAL, chars_left);
	} else if (!strcmp_l1("soctype", cmd)) {
		cpu_id_t cpuid = get_cpu_id();
		*(unsigned*)(response+4) = cpuid.family_id;
		*(unsigned*)(response+8) = cpuid.chip_rev;
		FB_DBG("soctype 0x%08x, %08x\n", cpuid.family_id, cpuid.chip_rev);
		replyLen = 4 + 8;
	} else if (!strcmp_l1("product", cmd)) {
		char* s1 = DEVICE_PRODUCT;
		strncat(response, s1, chars_left);
	} else if (!strcmp_l1("slot-count", cmd)) {
		strncat(response, "2", chars_left);
	} else if (!strcmp_l1("erase-block-size", cmd)) {
		strncat(response, "2000", chars_left);
	} else {
		FB_ERR("unknown variable: %s\n", cmd);
		strcpy(response, "FAILVariable not implemented");
	}

	replyLen = replyLen ? replyLen : strlen(response) + 1; //+1 means plus '\0'
	fastboot_tx_write(response, replyLen);
}

static unsigned int rx_bytes_expected(void)
{
	int rx_remain = download_size - download_bytes;
	if (rx_remain < 0)
		return 0;
	if (rx_remain > EP_BUFFER_SIZE)
		return EP_BUFFER_SIZE;
	return rx_remain;
}

#define BYTES_PER_DOT	0x20000
static void rx_handler_dl_image(struct usb_ep *ep, struct usb_request *req)
{
	char response[RESPONSE_LEN];
	unsigned int transfer_size = download_size - download_bytes;
	const unsigned char *buffer = req->buf;
	unsigned int buffer_size = req->actual;
	unsigned int pre_dot_num, now_dot_num;

	if (req->status != 0) {
		printf("Bad status: %d\n", req->status);
		return;
	}

	if (buffer_size < transfer_size)
		transfer_size = buffer_size;

	memcpy((void *)CONFIG_USB_FASTBOOT_BUF_ADDR + download_bytes,
	       buffer, transfer_size);

	pre_dot_num = download_bytes / BYTES_PER_DOT;
	download_bytes += transfer_size;
	now_dot_num = download_bytes / BYTES_PER_DOT;

	if (pre_dot_num != now_dot_num) {
		putc('.');
		if (!(now_dot_num % 74))
			putc('\n');
	}

	/* Check if transfer is done */
	if (download_bytes >= download_size) {
		/*
		 * Reset global transfer variable, keep download_bytes because
		 * it will be used in the next possible flashing command
		 */
		download_size = 0;
		req->complete = rx_handler_command;
		/*req->length = EP_BUFFER_SIZE;*/
		req->length = EP_CMD_LEN_MAX;
		req->buf	= &EP_CMD_BUF[0];

		sprintf(response, "OKAY");
		fastboot_tx_write_str(response);

		printf("\ndownloading of %d bytes finished\n", download_bytes);
	} else {
		req->length = rx_bytes_expected();
		if (req->length < ep->maxpacket)
			req->length = ep->maxpacket;
	}

	req->actual = 0;
	usb_ep_queue(ep, req, 0);
}

static void cb_download(struct usb_ep *ep, struct usb_request *req)
{
	char *cmd = req->buf;
	char response[RESPONSE_LEN];

	printf("cmd cb_download is %s\n", cmd);

	strsep(&cmd, ":");
	download_size = simple_strtoul(cmd, NULL, 16);
	download_bytes = 0;

	printf("Starting download of %d bytes\n", download_size);

	if (0 == download_size) {
		sprintf(response, "FAILdata invalid size");
	} else if (download_size > V3_DOWNLOAD_MEM_SIZE/*ddr_size_usable(CONFIG_USB_FASTBOOT_BUF_ADDR)*/) {
		download_size = 0;
		sprintf(response, "FAILdata too large");
	} else {
		sprintf(response, "DATA%08x", download_size);
		req->complete = rx_handler_dl_image;
		req->buf	  = (char*)V3_DOWNLOAD_EP_OUT;
		req->length = rx_bytes_expected();
		if (req->length < ep->maxpacket)
			req->length = ep->maxpacket;
	}
	fastboot_tx_write_str(response);
}

typedef boot_img_hdr_t boot_img_hdr;

static void do_bootm_on_complete(struct usb_ep *ep, struct usb_request *req)
{
	char boot_addr_start[12];
	unsigned    kernel_size;
	unsigned    ramdisk_size;
	boot_img_hdr *hdr_addr = NULL;
	int genFmt = 0;
	unsigned actualBootImgSz = 0;
	unsigned dtbSz = 0;
	unsigned char* loadaddr = 0;

	puts("Booting kernel...\n");

	sprintf(boot_addr_start, "bootm 0x%lx", load_addr);
	printf("boot_addr_start %s\n", boot_addr_start);

	loadaddr = (unsigned char*)CONFIG_USB_FASTBOOT_BUF_ADDR;
	hdr_addr = (boot_img_hdr*)loadaddr;

	genFmt = genimg_get_format(hdr_addr);
	if (IMAGE_FORMAT_ANDROID != genFmt) {
		printf("Fmt unsupported!genFmt 0x%x != 0x%x\n", genFmt, IMAGE_FORMAT_ANDROID);
		return;
	}

	kernel_size     =(hdr_addr->kernel_size + (hdr_addr->page_size-1)+hdr_addr->page_size)&(~(hdr_addr->page_size -1));
	ramdisk_size    =(hdr_addr->ramdisk_size + (hdr_addr->page_size-1))&(~(hdr_addr->page_size -1));
	dtbSz           = hdr_addr->second_size;
	actualBootImgSz = kernel_size + ramdisk_size + dtbSz;
	printf("kernel_size 0x%x, page_size 0x%x, totalSz 0x%x\n", hdr_addr->kernel_size, hdr_addr->page_size, kernel_size);
	printf("ramdisk_size 0x%x, totalSz 0x%x\n", hdr_addr->ramdisk_size, ramdisk_size);
	printf("dtbSz 0x%x, Total actualBootImgSz 0x%x\n", dtbSz, actualBootImgSz);

	memcpy((void *)load_addr, (void *)CONFIG_USB_FASTBOOT_BUF_ADDR, actualBootImgSz);

	flush_cache(load_addr,(unsigned long)actualBootImgSz);

	run_command(boot_addr_start, 0);

	/* This only happens if image is somehow faulty so we start over */
	do_reset(NULL, 0, 0, NULL);
}

static void cb_boot(struct usb_ep *ep, struct usb_request *req)
{
	fastboot_func->in_req->complete = do_bootm_on_complete;
	fastboot_tx_write_str("OKAY");
}

static void do_exit_on_complete(struct usb_ep *ep, struct usb_request *req)
{
	puts("Booting kernel..\n");
	run_command("run storeboot", 0);

	/* This only happens if image is somehow faulty so we start over */
	do_reset(NULL, 0, 0, NULL);
}


static void cb_continue(struct usb_ep *ep, struct usb_request *req)
{
	fastboot_func->in_req->complete = do_exit_on_complete;
	fastboot_tx_write_str("OKAY");
}

static void cb_set_active(struct usb_ep *ep, struct usb_request *req)
{
	char *cmd = req->buf;
	//char response[RESPONSE_LEN];
	int ret = 0;
	char str[128];

	printf("cmd cb_set_active is %s\n", cmd);
	strsep(&cmd, ":");
	if (!cmd) {
		FB_ERR("missing slot name\n");
		fastboot_tx_write_str("FAILmissing slot name");
		return;
	}

	sprintf(str, "set_active_slot %s", cmd);
	printf("command:    %s\n", str);
	ret = run_command(str, 0);
	printf("ret = %d\n", ret);
	if (ret == 0)
		fastboot_tx_write_str("OKAY");
	else
		fastboot_tx_write_str("FAILset slot error");
}

static void cb_devices(struct usb_ep *ep, struct usb_request *req)
{
	char response[RESPONSE_LEN];
	char *cmd = req->buf;

	printf("cmd is %s\n", cmd);

	strcpy(response, "AMLOGIC");

	fastboot_tx_write_str(response);
}

struct cmd_dispatch_info {
	char *cmd;
	void (*cb)(struct usb_ep *ep, struct usb_request *req);
};

static const struct cmd_dispatch_info cmd_dispatch_info[] = {
	{
		.cmd = "reboot",
		.cb = cb_reboot,
	}, {
		.cmd = "getvar:",
		.cb = cb_getvar,
	}, {
		.cmd = "download:",
		.cb = cb_download,
	}, {
		.cmd = "boot",
		.cb = cb_boot,
	}, {
		.cmd = "continue",
		.cb = cb_continue,
	},
	{
		.cmd = "update",
		.cb = cb_download,
	},
	{
		.cmd = "devices",
		.cb = cb_devices,
	},
	{
		.cmd = "reboot-bootloader",
		.cb = cb_reboot,
	},
	{
		.cmd = "set_active",
		.cb = cb_set_active,
	},
    {
		.cmd = "mwrite",
        .cb = cb_aml_media_write,
	}, {
		.cmd = "mread",
		.cb  = cb_aml_media_read,
    }, {
		.cmd = "oem",
		.cb  = cb_oem_cmd,
	}
};

//cb for out_req->complete
static void rx_handler_command(struct usb_ep *ep, struct usb_request *req)
{
	char *cmdbuf = req->buf;
	void (*func_cb)(struct usb_ep *ep, struct usb_request *req) = NULL;
	int i;

	for (i = 0; i < ARRAY_SIZE(cmd_dispatch_info); i++) {
		if (!strcmp_l1(cmd_dispatch_info[i].cmd, cmdbuf)) {
			func_cb = cmd_dispatch_info[i].cb;
			break;
		}
	}

	if (!func_cb) {
		FB_ERR("unknown command: %s\n", cmdbuf);
		fastboot_tx_write_str("FAILunknown command");
	} else {
		if (req->actual < req->length) {
			u8 *buf = (u8 *)req->buf;
			buf[req->actual] = 0;
			func_cb(ep, req);
		} else {
			FB_ERR("buffer overflow\n");
			fastboot_tx_write_str("FAILbuffer overflow");
		}
	}

	if (req->status == 0 && !fastboot_is_busy()) {
		*cmdbuf = '\0';
		req->actual = 0;
		usb_ep_queue(ep, req, 0);
	}
}

//following extended amlogic commands
//
char* fb_response_str = &response_str[4];

enum {
    MWRITE_DATA_CHECK_ALG_NONE  = 0, //not need check sum
    MWRITE_DATA_CHECK_ALG_ADDSUM,
    MWRITE_DATA_CHECK_ALG_CRC32,
};
static struct {
    unsigned totalBytes;
    unsigned transferredBytes; //transferredBytes <= totalBytes
    unsigned dataCheckAlg;
    void*    priv;//now for backup req->buf
}_mwriteInfo = {0}, _mreadInfo = {0};


static UsbDownInf* _pUsbDownInf = NULL;
static UsbUpInf* _pUsbUpInf = NULL;

static void rx_handler_mwrite(struct usb_ep *ep, struct usb_request *req)
{
    const unsigned int transfer_size = req->actual;
    char* dataBuf = _pUsbDownInf->dataBuf;
    int ret = 0;

    if (req->status != 0) {
        printf("Bad status: %d\n", req->status);
        return;
    }

    _mwriteInfo.transferredBytes += transfer_size;

    /* Check if transfer is done */
    if ((_mwriteInfo.transferredBytes == _mwriteInfo.totalBytes) &&
            (_mwriteInfo.dataCheckAlg > MWRITE_DATA_CHECK_ALG_NONE)) {
        req->length = 4;//rx addsum
        req->buf   += transfer_size;//remove copy
        if ((int64_t)req->buf & 7) {//address not align 8
            req->buf = (char*)((((int64_t)req->buf + 7)>>3)<<3);
        }
    }else if (_mwriteInfo.transferredBytes >= _mwriteInfo.totalBytes) {
        fastboot_okay(NULL);
        response_str[4] = 0;
        //forward to hold on long-time wait and not need use driver api directly
        fastboot_tx_write_str(response_str);//response_str will update following
        if (MWRITE_DATA_CHECK_ALG_ADDSUM == _mwriteInfo.dataCheckAlg) {
            const unsigned dataLen = _mwriteInfo.totalBytes;
            const unsigned gensum = add_sum(dataBuf, dataLen);
            const unsigned orisum = *(unsigned*)(((int64_t)(dataBuf + dataLen + 7)>>3)<<3);
            if ( gensum != orisum ) {
                FB_MSG("dataLen 0x%x, origsum 0x%x, 0x%x\n", dataLen, orisum, _mwriteInfo.transferredBytes);
                FBS_ERR(response_str, "FAIL;gensum(0x%x) != origsum(0x%x)", gensum, orisum);
                ret = -__LINE__;
            }
        }
        if ( !ret ) {//no err
            ret = v3tool_buffman_data_complete_download(_pUsbDownInf);
        }
        ret ? fastboot_fail(NULL) : fastboot_okay(NULL);
        fastboot_tx_write_str(response_str);//just update tx buffer
        FB_DBG("mwrite 0x%x bytes [%s]\n", _mwriteInfo.transferredBytes, ret ? "FAILED" : "OK");

        req->complete = rx_handler_command;//mwrite ended and return to receive command
        req->length = EP_CMD_LEN_MAX;
        if (_mwriteInfo.priv)req->buf    = (char*) _mwriteInfo.priv;

    } else {
        const unsigned leftLen = _mwriteInfo.totalBytes - _mwriteInfo.transferredBytes;
        req->length = DWC_BLK_LEN(leftLen);
        req->buf   += transfer_size;//remove copy
    }

    req->actual = 0;
    usb_ep_queue(ep, req, 0);
}

//[fastboot mwrite:verify=addsum]
static void cb_aml_media_write(struct usb_ep *ep, struct usb_request *req)
{
    char *cmd = req->buf;
    FB_DBG("cmd cb_mwrite[%s]\n", cmd);
    strsep(&cmd, ":");
    int ret = -__LINE__;
    const char* field = cmd;
    response_str[4] = 0;//clear for fastboot_tx_write_str

    //default attributes for mwrite
    _mwriteInfo.dataCheckAlg = MWRITE_DATA_CHECK_ALG_NONE;//default no transfer verify
    _mwriteInfo.transferredBytes = 0;
    for (strsep(&cmd, "="); cmd; ) {
        if (!strcmp(field,"verify")) {
            if (!strcmp("addsum", cmd)) {
                _mwriteInfo.dataCheckAlg = MWRITE_DATA_CHECK_ALG_ADDSUM;
            } else {
                /**endptr = '\0';*/
                FBS_ERR(_ACK, "unsupported dataCheckAlg %s", cmd);
                fastboot_fail(NULL);
                return;
            }
        } else {
            sprintf(response_str, "FAILunknown field[%s]\n", field);
            FB_ERR(response_str);
            goto _exit;
        }

        strsep(&cmd, ",");
        strsep(&cmd, "=");
    }

    ret = v3tool_buffman_next_download_info(&_pUsbDownInf);
    if ( ret || NULL == _pUsbDownInf) {
        FBS_ERR(_ACK, "Fail in buffman get, ret %d", ret);
        goto _exit;
    }
    if (NULL == _pUsbDownInf) {
        FBS_ERR(_ACK, "in get next img info");
        goto _exit;
    }
    _mwriteInfo.totalBytes = _pUsbDownInf->dataSize;

    ret = 0;
_exit:
    if (ret) {
        fastboot_fail(NULL);
    }else if(0 == _pUsbDownInf->dataSize){
        fastboot_okay(NULL);
		FB_MSG("OK in Partition Image\n\n");
    }else {
        sprintf(response_str, "DATAOUT0x%x:0x%llx", _pUsbDownInf->dataSize, _pUsbDownInf->fileOffset);
        req->complete = rx_handler_mwrite;//handle for download complete
        const unsigned leftLen = _mwriteInfo.totalBytes - _mwriteInfo.transferredBytes;
        req->length = DWC_BLK_LEN(leftLen);
        if (!_mwriteInfo.priv) _mwriteInfo.priv = req->buf;
        req->buf = _pUsbDownInf->dataBuf;//to remove copy
    }
    fastboot_tx_write_str(response_str);

    return;
}

static int v3tool_bl33_setvar_burnsteps(const int argc, char* const argv[])
{
    if ( 3 != argc ) {
        FB_EXIT("err setvar argc %d\n", argc);
    }
    unsigned long reg2 = simple_strtoul(argv[2], NULL, 0);
    if ( reg2 >> 32 ) {
        FB_EXIT("argv[1](%s) too big for 32bits reg\n", argv[2]);
    }
    writel((unsigned)reg2, P_PREG_STICKY_REG2);
    return 0;
}

static int v3tool_bl33_setvar(const int argc, char* const argv[])
{
    if ( 2 > argc ) {
        FB_EXIT("too few setvar argc %d\n", argc);
    }
    const char* subcmd = argv[1];
    if ( !strcmp("burnsteps", subcmd) ) {
        return v3tool_bl33_setvar_burnsteps(argc, argv);
    } else {
        FB_EXIT("unsupported setvar cmd[%s]\n", subcmd);
    }
    return 0;
}

//forward declare for cb_oem_cmd
static int _mwrite_cmd_parser(const int argc, char* argv[], char* ack);
static int _verify_partition_img(const int argc, char* argv[], char* ack);
static int _mread_cmd_parser(const int argc, char* argv[], char* ack);
int __attribute__((weak)) sheader_need(void) { FB_WRN("sheader_need undefined\n"); return 0;}

static void cb_oem_cmd(struct usb_ep *ep, struct usb_request *req)
{
	int ret = 0;
	char tmp[RESPONSE_LEN + 1];
	char* cmd = req->buf;
	char* ack = response_str + 4;

	ack[0] = '\0';//set err for which buf not setted
	char* cmdBuf = tmp;
	memcpy(cmdBuf, cmd, strnlen(cmd, RESPONSE_LEN)+1);//+1 to terminate str
	strsep(&cmdBuf, " ");
	printf("OEM cmd[%s]\n", cmdBuf);
	response_str[4] = 0;

	int argc = 33;
	char *argv[CONFIG_SYS_MAXARGS + 1];
	argc = cli_simple_parse_line(cmdBuf, argv);
	if (argc == 0) {
		fastboot_fail("oem no command at all");
		FB_ERR("%s\n", response_str);;
		return;
	}

	if ( !strcmp("mwrite", argv[0]) ) {
		ret = _mwrite_cmd_parser(argc, argv, ack);
	} else if( !strcmp("verify", argv[0]) ){
		ret = _verify_partition_img(argc, argv, ack);
		if (fastboot_is_busy()) {
			fastboot_tx_write_str(response_str);
			return;
		}
	} else if( !strcmp("mread", argv[0]) ){
		ret = _mread_cmd_parser(argc, argv, ack);
#ifdef CONFIG_V3_KEY_BURNING_SUPPORT
	} else if( !strcmp("key", argv[0]) ){
		ret = v2_key_command(argc, argv, ack);
#endif//#ifdef CONFIG_V3_KEY_BURNING_SUPPORT
	} else if( !strcmp("disk_initial", argv[0]) ){
		int toErase = argc > 1 ? simple_strtoul(argv[1], NULL, 0) : 0;
		int dtbImgSz = (0x1b8e == _memDtbImg.hadDown) ? _memDtbImg.imgSize : 0;
		ret = v3tool_storage_init(toErase, dtbImgSz);
		memset(&_memDtbImg, 0, sizeof(_memDtbImg));
	} else if( !strcmp("save_setting", argv[0]) ){
#if defined(CONFIG_CMD_SAVEENV) && !defined(CONFIG_ENV_IS_NOWHERE)
		env_set("firstboot", "1");
		env_set("upgrade_step", "1");
		ret = run_command("store rsv erase env", 0);
		ret = run_command("saveenv", 0);
#else
		FB_MSG("saveenv not implemented\n");
		ret = 0;
#endif//#if defined(CONFIG_CMD_SAVEENV) && !defined(CONFIG_ENV_IS_NOWHERE)
	} else if( !strcmp("setvar", argv[0]) ){
		ret = v3tool_bl33_setvar(argc, argv);
	} else if( !strcmp("sheader_need", argv[0]) ){
		ret = sheader_need() ? 0 : ret;
	} else if( !strcmp("test", argv[0]) ){
		static int i = 0;
		if ( ++i < 20 ) {
			fastboot_busy(NULL);
			fastboot_tx_write_str(response_str);
			return;
		}
	} else {
		strsep(&cmd, " ");
		ret = run_command(cmd, 0);
		if ( ret ) {
			FBS_ERR(ack,"fail in cmd,ret %d", ret);
		}
	}

	ret ? fastboot_fail(NULL) :fastboot_okay(NULL);
	fastboot_tx_write_str(response_str);
	FB_MSG("response[%d][%s]\n", ret, response_str);
	return ;
}

const char* _imgFmt[] = {"normal", "sparse", "ubifs"};
const char* _mediatype[] = {"store", "mem", "key", "mmc"};

static int _verify_partition_img(const int argc, char* argv[], char* ack)
{
	int ret = -__LINE__;
	if (3 > argc) {
		FBS_ERR(ack, "argc(%d) < 3 invalid\n", argc);
		return -__LINE__;
	}
	const char* vryAlg = argv[1];
	const char* origsumStr = argv[2];
	unsigned char gensum[SHA1_SUM_LEN];
	char gensumStr[SHA1_SUM_LEN*2+1];

	ret = strcmp(vryAlg, "sha1sum");
	if (ret) {
		FBS_ERR(ack, "vryAlg[%s] unsupported\n", vryAlg);
		return -__LINE__;
	}
	if (40 != strnlen(origsumStr,48)) {
		FBS_ERR(ack, "err vrySum in len for vryAlg[%s]\n", vryAlg);
		return -__LINE__;
	}
	ret = v3tool_buffman_img_verify_sha1sum(gensum);
	if (v3tool_media_is_busy()) {
		return 0;
	}
	if ( ret ) {
		FB_ERR("Fail in gen sha1sum,ret=%d", ret);
		return -__LINE__;
	}
	ret = optimus_hex_data_2_ascii_str(gensum, SHA1_SUM_LEN, gensumStr, sizeof(gensumStr)/sizeof(gensumStr[0]));
	if ( ret ) {
		FBS_ERR(ack, "Fail in pass hex to str,ret %d", ret);
		return -__LINE__;
	}
	ret = strncmp(gensumStr, origsumStr, 40);
	if ( ret ) {
		/*FBS_ERR(ack, "gensum[%s] NOT match orisum[%s]", gensumStr, origsumStr);*/
		FBS_ERR(ack, "gensum[%s] NOT match, origsum[%s]", gensumStr, origsumStr);
		return -__LINE__;
	}
	FB_MSG("Verify finish and successful!\n");
	return 0;
}

//[mwrite] $imgSize $imgFmt $mediaType $partition <$partOffset>
static int _mwrite_cmd_parser(const int argc, char* argv[], char* ack)
{
	int i = 0, ret = 0;
	if ( 5 > argc ) {
		sprintf(ack, "argc(%d) too few for mwrite", argc);
		FB_ERR("%s\n", ack);
		return -__LINE__;
	}
	const int64_t imgSize = simple_strtoull(argv[1], NULL, 0);
	const char* imgFmt  = argv[2];
	const char* media = argv[3];
	const char* partition = argv[4];
	const int64_t partOff = argc > 5 ? simple_strtoull(argv[5], NULL, 0) : 0;
	ImgTransPara imgTransPara;
	ImgDownloadPara* imgDownloadPara = &imgTransPara.download;
	ImgCommonPara* commonInf      = &imgDownloadPara->commonInf;

	memset(&imgTransPara, 0 , sizeof(imgTransPara));
	int imgFmtInt = -1;
	for (; i < sizeof(_imgFmt)/sizeof(_imgFmt[0]); ++i) {
		if (!strcmp(_imgFmt[i],imgFmt)) imgFmtInt = V3TOOL_PART_IMG_FMT_RAW + i;
	}
	if ( imgFmtInt == -1 ) {
		FBS_ERR(ack, "illegal imgFmt %s", imgFmt);
		return -__LINE__;
	}
	int mediaType = -1;
	for (i=0; i < sizeof(_mediatype)/sizeof(_mediatype[0]); ++i) {
		if (strcmp(_mediatype[i], media)) continue;
		mediaType = V3TOOL_MEDIA_TYPE_STORE + i;
		break;
	}
	if ( -1 == mediaType ) {
		FBS_ERR(ack, "unsupprted media %s", media);
		return -__LINE__;
	}

	imgDownloadPara->imgFmt  = imgFmtInt;
	commonInf->imgSzTotal = imgSize;
	commonInf->mediaType = mediaType;
	commonInf->partStartOff = partOff;
	strncpy(commonInf->partName, partition,V3_PART_NAME_LEN);
	switch (mediaType)
	{
		case V3TOOL_MEDIA_TYPE_MEM:
			{
				if (!strcmp("dtb", partition)) {
					commonInf->partStartOff += V3_DTB_LOAD_ADDR;
					_memDtbImg.hadDown  = 0x1b8e;
					_memDtbImg.imgSize  = imgSize;
				} else if (!strcmp("sheader", partition)) {
					commonInf->partStartOff += V3_PAYLOAD_LOAD_ADDR;
				} else {
					commonInf->partStartOff += simple_strtoull(partition, NULL, 0);
				}
				FB_MSG("mem base %llx\n", commonInf->partStartOff);
			} break;
		case V3TOOL_MEDIA_TYPE_STORE:
		case V3TOOL_MEDIA_TYPE_MMC:
			{ }break;
		case V3TOOL_MEDIA_TYPE_UNIFYKEY:
			{
				if ( imgSize >= _UNIFYKEY_MAX_SZ ) {
					FBS_ERR(ack, "key sz 0x%llx too large\n", imgSize);
					return -__LINE__;
				}
			}break;
		default:
			FBS_ERR(ack, "unsupported meida %s", media);
			return -__LINE__;
	}
	ret = v3tool_buffman_img_init(&imgTransPara, 1);
	if ( ret ) {
		FB_ERR("Fail in buffman init, ret %d\n", ret);
		return -__LINE__;
	}
	printf("Flash 0x%08llx Bytes %s img to %s:%s at off 0x%llx\n", imgSize, imgFmt, media, partition, partOff);

	return ret;
}

//[mread] $imgSize $imgFmt $mediaType $partition <$partOffset>
static int _mread_cmd_parser(const int argc, char* argv[], char* ack)
{
	int i = 0, ret = 0;
	if ( 5 > argc ) {
		sprintf(ack, "argc(%d) too few for mwrite", argc);
		FB_ERR("%s\n", ack);
		return -__LINE__;
	}
	const int64_t imgSize = simple_strtoull(argv[1], NULL, 0);
	const char* imgFmt  = argv[2];
	const char* media = argv[3];
	const char* partition = argv[4];
	const int64_t partOff = argc > 5 ? simple_strtoull(argv[5], NULL, 0) : 0;
	ImgTransPara imgTransPara;
	ImgUploadPara* imgUploadPara = &imgTransPara.upload;
	ImgCommonPara* commonInf      = &imgUploadPara->commonInf;

	memset(&imgTransPara, 0 , sizeof(imgTransPara));
	int imgFmtInt = -1;
	for (; i < sizeof(_imgFmt)/sizeof(_imgFmt[0]); ++i) {
		if (!strcmp(_imgFmt[i],imgFmt)) imgFmtInt = V3TOOL_PART_IMG_FMT_RAW + i;
	}
	if ( imgFmtInt == -1 ) {
		FBS_ERR(ack, "illegal imgFmt %s", imgFmt);
		return -__LINE__;
	}
	if (imgFmtInt != V3TOOL_PART_IMG_FMT_RAW) {
		FBS_ERR(ack, "oops, only support normal fmt in upload mode");
		return -__LINE__;
	}
	int mediaType = -1;
	for (i=0; i < sizeof(_mediatype)/sizeof(_mediatype[0]); ++i) {
		if (strcmp(_mediatype[i], media)) continue;
		mediaType = V3TOOL_MEDIA_TYPE_STORE + i;
		break;
	}
	if ( -1 == mediaType ) {
		FBS_ERR(ack, "unsupprted media %s", media);
		return -__LINE__;
	}

	commonInf->imgSzTotal = imgSize;
	commonInf->mediaType = mediaType;
	commonInf->partStartOff = partOff;
	switch (mediaType)
	{
		case V3TOOL_MEDIA_TYPE_MEM:
			{
				commonInf->partStartOff += simple_strtoull(partition, NULL, 0);
				FB_MSG("partStartOff 0x%llx\n", commonInf->partStartOff);
			} break;
		case V3TOOL_MEDIA_TYPE_STORE:
		case V3TOOL_MEDIA_TYPE_UNIFYKEY:
		case V3TOOL_MEDIA_TYPE_MMC:
			{
				strncpy(commonInf->partName, partition,V3_PART_NAME_LEN);
			}break;
		default:
			FBS_ERR(ack, "unsupported meida %s", media);
			return -__LINE__;
	}
	ret = v3tool_buffman_img_init(&imgTransPara, 0);
	if ( ret ) {
		FB_ERR("Fail in buffman init, ret %d\n", ret);
		return -__LINE__;
	}

	return ret;
}

static void tx_handler_mread(struct usb_ep* inep, struct usb_request* inreq)
{
	const unsigned int transfer_size = inreq->actual;

	if (inreq->status != 0) {
		printf("in req Bad status: %d\n", inreq->status);
		return;
	}
	if ( fastboot_func->in_req != inreq ) {
		FB_ERR("exception, bogus req\n");
		return ;
	}
	_mreadInfo.transferredBytes += transfer_size;

	/* Check if transfer is done */
	if (_mreadInfo.transferredBytes >= _mreadInfo.totalBytes) {
		FB_DBG("mread 0x%x bytes end\n", _mreadInfo.transferredBytes);

		inreq->complete = fastboot_complete;//mwrite ended and return to receive command
		inreq->length = EP_BUFFER_SIZE;
		if (_mreadInfo.priv)inreq->buf  = (char*) _mreadInfo.priv;
		//should return to rx next command
		v3tool_buffman_data_complete_upload(_pUsbUpInf);
	} else {
		const unsigned leftLen = _mreadInfo.totalBytes - _mreadInfo.transferredBytes;
		inreq->length = DWC_BLK_LEN(leftLen);
		inreq->buf   += transfer_size;//remove copy

		inreq->actual = 0;
		usb_ep_queue(inep, inreq, 0);
	}

	return;
}

enum {
    MREAD_STATUS_REQUEST        = 0xee,
    MREAD_STATUS_UPLOAD               ,
    MREAD_STATUS_FINISH               ,
};

//[fastboot mread boot.img.dump]
void cb_aml_media_read(struct usb_ep *outep, struct usb_request *outreq)
{
	char *cmd = outreq->buf;
	FB_DBG("cmd cb_mread[%s]\n", cmd);
	strsep(&cmd, ":");
	int ret = -__LINE__;
	int staMread = 0;

	//default attributes for mwrite
	_mwriteInfo.dataCheckAlg = MWRITE_DATA_CHECK_ALG_NONE;//default no transfer verify
	_mwriteInfo.transferredBytes = 0;

	const char* field = cmd;
	strsep(&cmd, "=");
	/*const int64_t val = simple_strtoull(cmd, &endptr, 0);*/
	if (!strcmp(field,"status")) {
		if (!strcmp("request", cmd)) {
			staMread = MREAD_STATUS_REQUEST;
		} else if(!strcmp("upload", cmd)){
			staMread = MREAD_STATUS_UPLOAD;
		} else if(!strcmp("finish", cmd)){
			staMread = MREAD_STATUS_FINISH;
		} else {
			FBS_ERR(_ACK, "unsupported mread status %s", cmd);
			fastboot_fail(NULL);
			return;
		}
	}

	switch (staMread)
	{
		case MREAD_STATUS_REQUEST:
			{
				//default attributes for mwrite
				_mreadInfo.transferredBytes = 0;
				_mreadInfo.totalBytes = 0;

				ret = v3tool_buffman_next_upload_info(&_pUsbUpInf);
				if ( ret || NULL == _pUsbUpInf) {
					FBS_ERR(_ACK, "Fail in buffman get, ret %d", ret);
					fastboot_fail(NULL);
					fastboot_tx_write_str(response_str);
					return;
				}
				if ( 0 == _pUsbUpInf->dataSize ) {
					fastboot_okay(NULL); response_str[4] = 0;//add NULL terminated
					FB_MSG("OKAY in Upload Data\n\n");
				} else {
					_mreadInfo.totalBytes = _pUsbUpInf->dataSize;
					sprintf(response_str, "DATAIN0x%x", _pUsbUpInf->dataSize);
				}
				fastboot_tx_write(response_str, strnlen(response_str, RESPONSE_LEN) + 1);//add 0 ternimated
				FB_DBG("_pUsbUpInf %p,sz %d\n", _pUsbUpInf->dataBuf, _mreadInfo.totalBytes);
				return ;
			}
		case MREAD_STATUS_UPLOAD:
			{
				struct usb_ep* inep = fastboot_func->in_ep;
				struct usb_request* inreq = fastboot_func->in_req;
				inreq->complete = tx_handler_mread;//handle for download complete
				const unsigned leftLen = _mreadInfo.totalBytes - _mreadInfo.transferredBytes;
				inreq->length = DWC_BLK_LEN(leftLen);
				if (!_mreadInfo.priv) _mreadInfo.priv = inreq->buf;//backup command buf
				inreq->buf = _pUsbUpInf->dataBuf;//to remove copy
				FB_DBG("upload buf=%p, leftLen 0x%x, req len 0x%x\n",
						inreq->buf, leftLen, inreq->length);
				ret = usb_ep_queue(inep, inreq, 0);
				if (ret)
					FB_ERR("Error %d on queue\n", ret);
				return;
			}
		case MREAD_STATUS_FINISH:
			{
				const int uploadOk = (_mreadInfo.totalBytes == _mreadInfo.transferredBytes);
				const char* ack = uploadOk ? "OKAY" : "FAIL";
				fastboot_tx_write_str(ack);
			} break;//just reuturn
	}

	return;
}

