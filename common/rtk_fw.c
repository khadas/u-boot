/******************************************************************************
*    Copyright (c) 2009-2015 by Hisi.
*    All rights reserved.
*
*    Create by Chenzetian. 2015-11-16
*
******************************************************************************/

#include <common.h>
#include <command.h>
#include <asm/processor.h>
#include <linux/compiler.h>
#include <linux/ctype.h>
#include <asm/byteorder.h>
#include <asm/unaligned.h>

#include <malloc.h>
#include <usb.h>
#include <rtk_fw.h>

#define USB_ERROR(fmt, args...) printf(fmt, ##args)
#define USB_DEBUG(fmt, args...) printf(fmt, ##args)
#define CHIP_TYPE_RTL8723BU "fw_8723b_b.txt"
#define CHIP_TYPE_RTL8723DU "fw_8723du_c.txt"
#define CODE_MAXSIZE_24K    (1024*24)   //24K
#define CODE_MAXSIZE_40K    (1024*40)   //40K
int fw_8723bu=0;
int fw_8723du=0;

//patch txt formate: total length(2 bytes) || fw patch || config patch
static unsigned char lu8Code_RTL8723BU[CODE_MAXSIZE_24K] = {
#include CHIP_TYPE_RTL8723BU
};
static unsigned char lu8Code_RTL8723DU[CODE_MAXSIZE_40K] = {
#include CHIP_TYPE_RTL8723DU
};

firmware_info *firmware_info_init(struct usb_device *dev)
{
	struct usb_device *udev = dev;
	firmware_info *fw_info;

	USB_DEBUG("%s[%d]: start.\n", __func__, __LINE__);

	fw_info = malloc(sizeof(*fw_info));
    memset(fw_info,0,sizeof(*fw_info));

	if (!fw_info)
		return NULL;

    if (fw_8723bu) {
        fw_info->fw_data = malloc(CODE_MAXSIZE_24K);
        memset(fw_info->fw_data,0,CODE_MAXSIZE_24K);
    }

    if (fw_8723du) {
        fw_info->fw_data = malloc(CODE_MAXSIZE_40K);
        memset(fw_info->fw_data,0,CODE_MAXSIZE_40K);
    }

	if (!fw_info->fw_data) {
		free(fw_info);
		return NULL;
	}

	fw_info->send_pkt = malloc(PKT_LEN);
    memset(fw_info->send_pkt,0,PKT_LEN);
	if (!fw_info->send_pkt) {
        free(fw_info->fw_data);
		free(fw_info);
		return NULL;
	}

	fw_info->rcv_pkt = malloc(PKT_LEN);
    memset(fw_info->rcv_pkt,0,PKT_LEN);
	if (!fw_info->rcv_pkt) {
        free(fw_info->fw_data);
		free(fw_info->send_pkt);
		free(fw_info);
		return NULL;
	}

	fw_info->patch_entry = NULL;


	fw_info->intf = NULL;
	fw_info->udev = udev;
	fw_info->pipe_in = usb_rcvintpipe(fw_info->udev, INTR_EP);
	fw_info->pipe_out = usb_sndctrlpipe(fw_info->udev, CTRL_EP);
	fw_info->cmd_hdr = (struct hci_command_hdr *)(fw_info->send_pkt);
	fw_info->evt_hdr = (struct hci_event_hdr *)(fw_info->rcv_pkt);
	fw_info->cmd_cmp = (struct hci_ev_cmd_complete *)(fw_info->rcv_pkt + EVT_HDR_LEN);
	fw_info->req_para = fw_info->send_pkt + CMD_HDR_LEN;
	fw_info->rsp_para = fw_info->rcv_pkt + EVT_HDR_LEN + CMD_CMP_LEN;

	return fw_info;
}

void firmware_info_destroy(firmware_info *fw_info)
{
    free(fw_info->fw_data);
	free(fw_info->rcv_pkt);
	free(fw_info->send_pkt);
	free(fw_info);
}
int load_firmware(firmware_info *fw_info, unsigned char **buff)
{
    unsigned int size;

    if (fw_8723bu) {
        size = lu8Code_RTL8723BU[0] | (lu8Code_RTL8723BU[1] << 8);
        USB_DEBUG("%s,size is %d\n",__func__, size);

        if (size >= CODE_MAXSIZE_24K) {
            USB_ERROR("%s,size is %d firmware is larger than %d\n",__func__, size, CODE_MAXSIZE_24K);
            return -1;
        }

        memcpy(*buff, &lu8Code_RTL8723BU[2],size);
        fw_8723bu = 0;
        return size;
    }

    if (fw_8723du) {
        size = lu8Code_RTL8723DU[0] | (lu8Code_RTL8723DU[1] << 8);
        USB_DEBUG("%s,size is %d\n",__func__, size);

        if (size >= CODE_MAXSIZE_40K) {
            USB_ERROR("%s,size is %d firmware is larger than %d\n",__func__, size, CODE_MAXSIZE_40K);
            return -1;
        }

        memcpy(*buff, &lu8Code_RTL8723DU[2],size);
        fw_8723du= 0;
        return size;
    }
    return 0;
}
int send_hci_cmd(firmware_info *fw_info)
{
	int ret_val;

	ret_val = usb_control_msg(
		fw_info->udev, fw_info->pipe_out,
		0, USB_TYPE_CLASS, 0, 0,
		(void *)(fw_info->send_pkt),
		fw_info->pkt_len, MSG_TO);

	return ret_val;
}


int rcv_hci_evt(firmware_info *fw_info)
{
	int /*ret_len = 0,*/ ret_val = 0;
	int i;

	while (1) {
		for (i = 0; i < 5; i++) {
            ret_val = usb_submit_int_msg(
                fw_info->udev, fw_info->pipe_in,
                (void *)(fw_info->rcv_pkt), PKT_LEN,
                MSG_TO);
                if (ret_val >= 0)
                    break;

		}
    /*USB_DEBUG("fw_info->rcv_pkt : 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \n",
        fw_info->rcv_pkt[0],
        fw_info->rcv_pkt[1],
        fw_info->rcv_pkt[2],
        fw_info->rcv_pkt[3],
        fw_info->rcv_pkt[4],
        fw_info->rcv_pkt[5],
        fw_info->rcv_pkt[6]);*/
		if (ret_val < 0)
			return ret_val;

		if (CMD_CMP_EVT == fw_info->evt_hdr->evt) {
			if (fw_info->cmd_hdr->opcode == fw_info->cmd_cmp->opcode)
				return 0;
		}
	}
}

int get_firmware(firmware_info *fw_info)
{
    /* patch_info *patch_entry = fw_info->patch_entry; */

    fw_info->fw_len = load_firmware(fw_info, &fw_info->fw_data);
    USB_DEBUG("%s[%d]: fw_len=%d\n", __func__, __LINE__, fw_info->fw_len);
    if (fw_info->fw_len <= 0)
        return -1;

	return 0;
}
int reset_controller(firmware_info* fw_info)
{
    int ret_val;
    USB_DEBUG("reset_controller \n");
    if (!fw_info)
        return -ENODEV;
    fw_info->cmd_hdr->opcode = cpu_to_le16(HCI_VENDOR_FORCE_RESET_AND_PATCHABLE);
    fw_info->cmd_hdr->plen = 0;
    fw_info->pkt_len = CMD_HDR_LEN;
    ret_val = send_hci_cmd(fw_info);
    if (ret_val < 0) {
        USB_DEBUG("%s: Failed to send hci cmd 0x%04x, errno %d \n",
                __func__, fw_info->cmd_hdr->opcode, ret_val);
        return ret_val;
    }
    mdelay(200);
    USB_DEBUG("%s: Wait fw reset for 200ms \n",__func__);
    return ret_val;
}

int download_data(firmware_info *fw_info)
{
	download_cp *cmd_para;
	download_rp *evt_para;
	unsigned char *pcur;
	int pkt_len, frag_num, frag_len;
	int i, ret_val;
	int ncmd = 1, step = 1;

	//USB_DEBUG("%s: start\n", __func__);

	cmd_para = (download_cp *)fw_info->req_para;
	evt_para = (download_rp *)fw_info->rsp_para;
	pcur = fw_info->fw_data;
	pkt_len = CMD_HDR_LEN + sizeof(download_cp);
	frag_num = fw_info->fw_len / PATCH_SEG_MAX + 1;
	frag_len = PATCH_SEG_MAX;

	cmd_para->index = 0;
	for (i = 0; i < frag_num; i++) {
		if (i == (frag_num - 1)) {
			cmd_para->index |= DATA_END;
			frag_len = fw_info->fw_len % PATCH_SEG_MAX;
			pkt_len -= (PATCH_SEG_MAX - frag_len);
		}
		fw_info->cmd_hdr->opcode = cpu_to_le16(DOWNLOAD_OPCODE);
		fw_info->cmd_hdr->plen = sizeof(unsigned char) + frag_len;
		fw_info->pkt_len = pkt_len;
		memcpy(cmd_para->data, pcur, frag_len);

		if (step > 0) {
			ret_val = send_hci_cmd(fw_info);
			if (ret_val < 0) {
				//USB_ERROR("%s: Failed to send frag num %d\n", __func__, cmd_para->index);
				return ret_val;
			} else
				//USB_DEBUG("%s: Send frag num %d\n", __func__, cmd_para->index);

			if (--step > 0 && i < frag_num - 1) {
				//USB_DEBUG("%s: Continue to send frag num %d\n", __func__, cmd_para->index + 1);
				pcur += PATCH_SEG_MAX;
				continue;
			}
		}

		while (ncmd > 0) {
			ret_val = rcv_hci_evt(fw_info);
			if (ret_val < 0) {
				//USB_ERROR("%s: rcv_hci_evt err %d\n", __func__, ret_val);
				return ret_val;
			} else {
				//USB_DEBUG("%s: Receive acked frag num %d\n", __func__, evt_para->index);
				ncmd--;
			}

			if (0 != evt_para->status) {
				//USB_ERROR("%s: Receive acked frag num %d, err status %d\n",
				//		__func__, ret_val, evt_para->status);
				return -1;
			}

			if ((evt_para->index & DATA_END) || (evt_para->index == frag_num - 1)) {
				//USB_DEBUG("%s: Receive last acked index %d\n", __func__, evt_para->index);
				goto end;
			}
		}

		cmd_para->index ++;
		if (cmd_para->index == 0x80)
			cmd_para->index = 1;
		ncmd = step = fw_info->cmd_cmp->ncmd;
		pcur += PATCH_SEG_MAX;
		//USB_DEBUG("%s: HCI command packet num %d\n", __func__, ncmd);
	}

	/*
	 * It is tricky that Host cannot receive DATA_END index from BT
	 * controller, at least for 8723au. We are doomed if failed.
	 */

end:
	// USB_DEBUG("%s: done, sent %d frag pkts, received %d frag events\n",
	//		__func__, cmd_para->index, evt_para->index);
	return fw_info->fw_len;
}

int set_wakeup_device(firmware_info* fw_info)
{
	struct rtk_eversion_evt *ever_evt;
	int ret_val;

    static unsigned char bdaddr[7] = {0x00, 0xd7, 0xe3,0x3e,0xf8, 0xe6,0xa0};
	if (!fw_info)
		return -ENODEV;

	fw_info->cmd_hdr->opcode = cpu_to_le16(HCI_VENDOR_SET_WAKE_UP_DEVICE);
	fw_info->cmd_hdr->plen = 7;
    memcpy(fw_info->req_para, bdaddr, 7);
	fw_info->pkt_len = CMD_HDR_LEN + 7;

	ret_val = send_hci_cmd(fw_info);
	if (ret_val < 0) {
		USB_ERROR("%s: Failed to send hci cmd 0x%04x, errno %d\n",
				__func__, fw_info->cmd_hdr->opcode, ret_val);
		return ret_val;
	}

	ret_val = rcv_hci_evt(fw_info);
	if (ret_val < 0) {
		USB_ERROR("%s: Failed to receive hci event, errno %d\n",
				__func__, ret_val);
		return ret_val;
	}

	ever_evt = (struct rtk_eversion_evt *)(fw_info->rsp_para);

	USB_DEBUG("%s: status %d, eversion %d", __func__, ever_evt->status, ever_evt->version);

	return ret_val;
}


int check_chip_fw_version(firmware_info* fw_info)
{
    struct rtk_localversion_evt *ever_evt;
	int ret_val;

	if (!fw_info)
		return -ENODEV;

    fw_info->cmd_hdr->opcode = cpu_to_le16(HCI_VENDOR_READ_LMP_VERISION);
    fw_info->cmd_hdr->plen = 0;
    fw_info->pkt_len = CMD_HDR_LEN;

	ret_val = send_hci_cmd(fw_info);
	if (ret_val < 0) {
		USB_ERROR("%s: Failed to send hci cmd 0x%04x, errno %d\n",
				__func__, fw_info->cmd_hdr->opcode, ret_val);
		return ret_val;
	}

	ret_val = rcv_hci_evt(fw_info);
	if (ret_val < 0) {
		USB_ERROR("%s: Failed to receive hci event, errno %d\n",
				__func__, ret_val);
		return ret_val;
	}

    ever_evt = (struct rtk_localversion_evt *)(fw_info->rsp_para);

    USB_DEBUG("%s: status %x \n", __func__, ever_evt->status);
    USB_DEBUG("%s: hci_version %x \n", __func__, ever_evt->hci_version);
    USB_DEBUG("%s: hci_revision %x \n", __func__, ever_evt->hci_revision);
    USB_DEBUG("%s: lmp_version %x \n", __func__, ever_evt->lmp_version);
    USB_DEBUG("%s: lmp_subversion %x \n", __func__, ever_evt->lmp_subversion);
    USB_DEBUG("%s: lmp_manufacture %x \n", __func__, ever_evt->lmp_manufacture);

	return ret_val;
}
#if 0 // just fro reference
void load_rtl_firmware(void){
    firmware_info *fw_info;
    int ret_val,i;
    struct usb_device *dev;

    usb_init();

   /* scan all USB Devices */
   dev = NULL;
   for (i=0;i<USB_MAX_DEVICE;i++) {
       dev=usb_get_dev_index(i); /* get device */
       if (dev == NULL)
            return;
       if (dev->devnum != -1) {
           if (!strcmp(dev->mf,"Realtek") && !strcmp(dev->prod,"Bluetooth Radio")) {
               /* Download BT Fireware */
               printf("Now Download BT Firmware.\n");
               fw_info = firmware_info_init(dev);
               get_firmware(fw_info);
                ret_val = download_data(fw_info);
				if (ret_val > 0)
                    printf("Download fw patch done, fw len %d\n",ret_val);
                mdelay(10);
                set_wakeup_device(fw_info);
 //               set_bt_wakeup(fw_info);
               firmware_info_destroy(fw_info);
               }
         }
    }
}
#endif
struct usb_device * get_rtl_dev(void){
	int i;
	struct usb_device *dev;
	/* scan all USB Devices */
	dev = NULL;
	for (i = 0; i < USB_MAX_DEVICE; i++) {
		dev=usb_get_dev_index(i); /* get device */
		if (dev == NULL) {
			printf("usb dev is null!!!!\n");
	        return NULL;
	    }
	    printf("i=%d dev->devnum:%d dev->mf:%s dev->prod:%s\n", i, dev->devnum, dev->mf, dev->prod);
	    printf("vid is %x, pid is %x \n",le16_to_cpu(dev->descriptor.idVendor), le16_to_cpu(dev->descriptor.idProduct));
	    /* AML change for haier to match 8723bu bt */
		if (dev->devnum != -1) {
			if (le16_to_cpu(dev->descriptor.idVendor) == 0x0bda
				&& le16_to_cpu(dev->descriptor.idProduct) == 0xb720){
				fw_8723bu = 1;
				return dev;
			}
			if (le16_to_cpu(dev->descriptor.idVendor) == 0x0bda
                && le16_to_cpu(dev->descriptor.idProduct) == 0xd723){
				fw_8723du = 1;
				return dev;
			}
		}
	}
	return NULL;
}
int load_rtl_firmware_dev(struct usb_device *dev)
{
#if 0
	char *reboot_mode = getenv("reboot_mode");
	if (reboot_mode == NULL)
		return 0;
	if (reboot_mode&& strcmp("cold_boot", reboot_mode))
		return 0;
#endif
    firmware_info *fw_info;
    int ret_val=-1;

    /* Download BT Fireware */
    USB_DEBUG("Now Download RTK BT Firmware. (version 20181220i_1)\n");

    fw_info = firmware_info_init(dev);
    if (NULL == fw_info)
        return -1;

    ret_val = get_firmware(fw_info);
    printf("++++MB:{%s}[%d]...(get_firmware = [%d]).\n", __FUNCTION__, __LINE__, ret_val);
    if (-1 == ret_val) {
        printf("++++MB:{%s}[%d]...get firmware, errno %d \n", __FUNCTION__, __LINE__, ret_val);
        return ret_val;
    }

    ret_val = check_chip_fw_version(fw_info);
    printf("++++MB:{%s}[%d]...(check_chip_fw_version = [%d]).\n", __FUNCTION__, __LINE__, ret_val);
    if (ret_val < 0) {
        printf("++++MB:{%s}[%d]...Failed to check chip fw version, errno %d \n", __FUNCTION__, __LINE__, ret_val);
        return ret_val;
    }

    ret_val = reset_controller(fw_info);
    printf("++++MB:{%s}[%d]...(reset_controller = [%d]).\n", __FUNCTION__, __LINE__, ret_val);
    if (ret_val < 0) {
        printf("++++MB:{%s}[%d]...(reset controller error = [%d]).\n", __FUNCTION__, __LINE__, ret_val);
        return ret_val;
    }

    ret_val = download_data(fw_info);
    printf("++++MB:{%s}[%d]...(download_data = [%d]).\n", __FUNCTION__, __LINE__, ret_val);
    if (ret_val > 0)
        printf("Download fw patch done, fw len %d\n",ret_val);
    else
    {
        firmware_info_destroy(fw_info);
        return ret_val;
    }
    mdelay(10);
    ret_val = set_wakeup_device(fw_info);
    printf("++++MB:{%s}[%d]...(set_wakeup_device = [%d]).\n", __FUNCTION__, __LINE__, ret_val);
    //set_bt_wakeup(fw_info);
    firmware_info_destroy(fw_info);
    return ret_val;
}

