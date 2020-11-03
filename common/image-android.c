// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2011 Sebastian Andrzej Siewior <bigeasy@linutronix.de>
 */

#include <common.h>
#include <image.h>
#include <android_image.h>
#include <malloc.h>
#include <errno.h>
static const unsigned char lzop_magic[] = {
	0x89, 0x4c, 0x5a, 0x4f, 0x00, 0x0d, 0x0a, 0x1a, 0x0a
};

#define ANDROID_IMAGE_DEFAULT_KERNEL_ADDR	0x10008000
static const unsigned char gzip_magic[] = {
	0x1f, 0x8b
};

static char andr_tmp_str[ANDR_BOOT_ARGS_SIZE + 1];

static struct {
	const unsigned char * szID;
	int   nIDLength;
	ulong nCompID;
} arrComp[] = {
	{lzop_magic,9,IH_COMP_LZO},
	{gzip_magic,2,IH_COMP_GZIP},
};

/* android R*/
static int   android_image_get_kernel_v3(const boot_img_hdr_v3_t *hdr, int verify, ulong *os_data, ulong *os_len);
static int   android_image_need_move_v3(ulong *img_addr,const boot_img_hdr_v3_t *hdr);
static ulong android_image_get_kload_v3(const boot_img_hdr_v3_t *hdr);
static ulong android_image_get_comp_v3(const boot_img_hdr_v3_t *hdr);
static ulong android_image_get_end_v3(const boot_img_hdr_v3_t *hdr);

#ifdef CONFIG_OF_LIBFDT_OVERLAY
static int save_dtbo_idx(const char *cmdline)
{
	char *dtbo_chosen_idx_start = NULL;
	char *dtbo_chosen_idx_end = NULL;
	char *dtbo_idx = NULL;

	if (!env_get("androidboot.dtbo_idx")) {
		if (!cmdline)
			return -1;

		dtbo_chosen_idx_start = strstr(cmdline, "androidboot.dtbo_idx");
		if (!dtbo_chosen_idx_start) {
			printf("No androidboot.dtbo_idx configured");
			return -1;
		}

		dtbo_chosen_idx_end = strchr(dtbo_chosen_idx_start, ' ');
		if (dtbo_chosen_idx_end) {
			dtbo_idx = malloc(dtbo_chosen_idx_end -
					dtbo_chosen_idx_start + 1);
			if (!dtbo_idx) {
				printf("dtbo out of memory\n");
				return -1;
			}
			memset(dtbo_idx, 0x00,
			       dtbo_chosen_idx_end - dtbo_chosen_idx_start + 1);
			strncpy(dtbo_idx, dtbo_chosen_idx_start,
				dtbo_chosen_idx_end - dtbo_chosen_idx_start);
		} else
			strncpy(dtbo_idx, dtbo_chosen_idx_start,
				strlen(dtbo_chosen_idx_start));

		env_set("androidboot.dtbo_idx",
		       dtbo_idx + strlen("androidboot.dtbo_idx="));
	}

	free(dtbo_idx);
	return 0;
}
#endif

int is_android_r_image(void *img_addr)
{
	p_boot_img_hdr_v3_t pHDR = (p_boot_img_hdr_v3_t)(img_addr);
	return (ANDROID_R_IMG_VER == pHDR->header_version);
}

static ulong android_image_get_kernel_addr(const  boot_img_hdr_t *hdr)
{
	/*
	 * All the Android tools that generate a boot.img use this
	 * address as the default.
	 *
	 * Even though it doesn't really make a lot of sense, and it
	 * might be valid on some platforms, we treat that adress as
	 * the default value for this field, and try to execute the
	 * kernel in place in such a case.
	 *
	 * Otherwise, we will return the actual value set by the user.
	 */
	if (hdr->kernel_addr == ANDROID_IMAGE_DEFAULT_KERNEL_ADDR)
		return (ulong)hdr + hdr->page_size;

	return hdr->kernel_addr;
}

/**
 * android_image_get_kernel() - processes kernel part of Android boot images
 * @hdr:	Pointer to image header, which is at the start
 *			of the image.
 * @verify:	Checksum verification flag. Currently unimplemented.
 * @os_data:	Pointer to a ulong variable, will hold os data start
 *			address.
 * @os_len:	Pointer to a ulong variable, will hold os data length.
 *
 * This function returns the os image's start address and length. Also,
 * it appends the kernel command line to the bootargs env variable.
 *
 * Return: Zero, os start address and length on success,
 *		otherwise on failure.
 */
int android_image_get_kernel(const  boot_img_hdr_t *hdr, int verify,
			     ulong *os_data, ulong *os_len)
{
	if (is_android_r_image((void*)hdr))
		return android_image_get_kernel_v3((void*)hdr,verify,os_data,os_len);

	u32 kernel_addr = android_image_get_kernel_addr(hdr);

	/*
	 * Not all Android tools use the id field for signing the image with
	 * sha1 (or anything) so we don't check it. It is not obvious that the
	 * string is null terminated so we take care of this.
	 */
	strncpy(andr_tmp_str, hdr->name, ANDR_BOOT_NAME_SIZE);
	andr_tmp_str[ANDR_BOOT_NAME_SIZE] = '\0';
	if (strlen(andr_tmp_str))
		printf("Android's image name: %s\n", andr_tmp_str);

	printf("Kernel load addr 0x%08x size %u KiB\n",
	       kernel_addr, DIV_ROUND_UP(hdr->kernel_size, 1024));

	int len = 0;
	if (*hdr->cmdline) {
		printf("Kernel command line: %s\n", hdr->cmdline);
		len += strlen(hdr->cmdline);
	}

#ifdef CONFIG_OF_LIBFDT_OVERLAY
	save_dtbo_idx(hdr->cmdline);
#endif
	char *bootargs = env_get("bootargs");
	if (bootargs)
		len += strlen(bootargs);

	char *newbootargs = malloc(len + 2);
	if (!newbootargs) {
		puts("Error: malloc in android_image_get_kernel failed!\n");
		return -ENOMEM;
	}
	*newbootargs = '\0';

	if (bootargs) {
		strcpy(newbootargs, bootargs);
		strcat(newbootargs, " ");
	}
	if (*hdr->cmdline)
		strcat(newbootargs, hdr->cmdline);

	env_set("bootargs", newbootargs);

	if (os_data) {
		*os_data = (ulong)hdr;
		*os_data += hdr->page_size;
	}
	if (os_len)
		*os_len = hdr->kernel_size;

#if defined(CONFIG_ANDROID_BOOT_IMAGE)
			ulong end;
			images.ft_len = (ulong)(hdr->second_size);
			end = (ulong)hdr;
			end += hdr->page_size;
			end += ALIGN(hdr->kernel_size, hdr->page_size);
			images.rd_start = end;
			end += ALIGN(hdr->ramdisk_size, hdr->page_size);
			images.ft_addr = (char *)end;
#endif

	return 0;
}

int android_image_check_header(const  boot_img_hdr_t *hdr)
{
	return memcmp(ANDR_BOOT_MAGIC, hdr->magic, ANDR_BOOT_MAGIC_SIZE);
}

int vendor_boot_image_check_header(const vendor_boot_img_hdr_t * hdr)
{
	return memcmp(VENDOR_BOOT_MAGIC, hdr->magic, VENDOR_BOOT_MAGIC_SIZE);
}

ulong android_image_get_end(const  boot_img_hdr_t *hdr)
{
	if (is_android_r_image((void*)hdr))
		return android_image_get_end_v3((void*)hdr);

	ulong end;
	/*
	 * The header takes a full page, the remaining components are aligned
	 * on page boundary
	 */
	end = (ulong)hdr;
	end += hdr->page_size;
	end += ALIGN(hdr->kernel_size, hdr->page_size);
	end += ALIGN(hdr->ramdisk_size, hdr->page_size);
	end += ALIGN(hdr->second_size, hdr->page_size);

	return end;
}

ulong android_image_get_kload(const  boot_img_hdr_t *hdr)
{
	if (is_android_r_image((void*)hdr))
		return android_image_get_kload_v3((void*)hdr);
	else
		return android_image_get_kernel_addr(hdr);
}

int android_image_get_ramdisk(const  boot_img_hdr_t *hdr,
			      ulong *rd_data, ulong *rd_len)
{
	if (is_android_r_image((void*)hdr))
		return android_image_get_ramdisk_v3((void*)hdr,rd_data,rd_len);

	if (!hdr->ramdisk_size) {
		*rd_data = *rd_len = 0;
		return -1;
	}

	printf("RAM disk load addr 0x%08x size %u KiB\n",
	       hdr->ramdisk_addr, DIV_ROUND_UP(hdr->ramdisk_size, 1024));

	*rd_data = (unsigned long)hdr;
	*rd_data += hdr->page_size;
	*rd_data += ALIGN(hdr->kernel_size, hdr->page_size);

	*rd_len = hdr->ramdisk_size;
	return 0;
}

int android_image_get_second(const  boot_img_hdr_t *hdr,
			      ulong *second_data, ulong *second_len)
{
	if (is_android_r_image((void*)hdr))
		return ;

	if (!hdr->second_size) {
		*second_data = *second_len = 0;
		return -1;
	}

	*second_data = (unsigned long)hdr;
	*second_data += hdr->page_size;
	*second_data += ALIGN(hdr->kernel_size, hdr->page_size);
	*second_data += ALIGN(hdr->ramdisk_size, hdr->page_size);

	printf("second address is 0x%lx\n",*second_data);

	*second_len = hdr->second_size;
	return 0;
}

#if !defined(CONFIG_SPL_BUILD)
/**
 * android_print_contents - prints out the contents of the Android format image
 * @hdr: pointer to the Android format image header
 *
 * android_print_contents() formats a multi line Android image contents
 * description.
 * The routine prints out Android image properties
 *
 * returns:
 *     no returned results
 */
void android_print_contents(const  boot_img_hdr_t *hdr)
{
	const char * const p = IMAGE_INDENT_STRING;
	/* os_version = ver << 11 | lvl */
	u32 os_ver = hdr->os_version >> 11;
	u32 os_lvl = hdr->os_version & ((1U << 11) - 1);

	printf("%skernel size:      %x\n", p, hdr->kernel_size);
	printf("%skernel address:   %x\n", p, hdr->kernel_addr);
	printf("%sramdisk size:     %x\n", p, hdr->ramdisk_size);
	printf("%sramdisk addrress: %x\n", p, hdr->ramdisk_addr);
	printf("%ssecond size:      %x\n", p, hdr->second_size);
	printf("%ssecond address:   %x\n", p, hdr->second_addr);
	printf("%stags address:     %x\n", p, hdr->tags_addr);
	printf("%spage size:        %x\n", p, hdr->page_size);
	/* ver = A << 14 | B << 7 | C         (7 bits for each of A, B, C)
	 * lvl = ((Y - 2000) & 127) << 4 | M  (7 bits for Y, 4 bits for M) */
	printf("%sos_version:       %x (ver: %u.%u.%u, level: %u.%u)\n",
	       p, hdr->os_version,
	       (os_ver >> 7) & 0x7F, (os_ver >> 14) & 0x7F, os_ver & 0x7F,
	       (os_lvl >> 4) + 2000, os_lvl & 0x0F);
	printf("%sname:             %s\n", p, hdr->name);
	printf("%scmdline:          %s\n", p, hdr->cmdline);
}
#endif

ulong android_image_get_comp(const  boot_img_hdr_t *os_hdr)
{
	if (is_android_r_image((void*)os_hdr))
		return android_image_get_comp_v3((void*)os_hdr);

	int i;
	unsigned char *src = (unsigned char *)os_hdr + os_hdr->page_size;
	for (i = 0;i< ARRAY_SIZE(arrComp);++i)
	{
		if (!memcmp(arrComp[i].szID,src,arrComp[i].nIDLength))
			return arrComp[i].nCompID;
	}

	return IH_COMP_NONE;
}
int android_image_need_move(ulong *img_addr, const  boot_img_hdr_t *hdr)
{
	if (is_android_r_image((void*)hdr))
		return android_image_need_move_v3(img_addr,(void*)hdr);

	ulong kernel_load_addr = android_image_get_kload(hdr);
	ulong img_start = *img_addr;
	ulong val = 0;
	if (kernel_load_addr > img_start)
		val = kernel_load_addr - img_start;
	else
		val = img_start - kernel_load_addr;
	if (android_image_get_comp(hdr) == IH_COMP_NONE)
		return 0;
	if (val < 32*1024*1024) {
		ulong total_size = android_image_get_end(hdr)-(ulong)hdr;
		void *reloc_addr = malloc(total_size);
		if (!reloc_addr) {
			puts("Error: malloc in  android_image_need_move failed!\n");
			return -ENOMEM;
		}
		printf("reloc_addr =%lx\n", (ulong)reloc_addr);
		memset(reloc_addr, 0, total_size);
		memmove(reloc_addr, hdr, total_size);
		*img_addr = (ulong)reloc_addr;
		printf("copy done\n");
	}
	return 0;
}

/*Android R boot func*/
/* definition of vendor_boot partition structure */
p_vendor_boot_img_t p_vender_boot_img = 0;
static int android_image_get_kernel_v3(const boot_img_hdr_v3_t *hdr, int verify,
			     ulong *os_data, ulong *os_len)
{
	/*
	 * Not all Android tools use the id field for signing the image with
	 * sha1 (or anything) so we don't check it. It is not obvious that the
	 * string is null terminated so we take care of this.
	 */
	/*check vendor boot image first*/
	if (!p_vender_boot_img)
	{
		if (os_data)
			*os_data = 0;
		if (os_len)
			*os_len = 0;
		goto exit;
	}

	ulong end;
	p_vendor_boot_img_hdr_t vb_hdr = &p_vender_boot_img->hdr;
	char boot_name[ANDR_BOOT_NAME_SIZE + 8];
	memset(boot_name,0,sizeof(boot_name));
	strncpy(boot_name, (char *)vb_hdr->name, ANDR_BOOT_NAME_SIZE);
	if (strlen(boot_name))
		printf("Android's image name: %s\n", boot_name);

	//debug("Kernel load addr 0x%08x size %u KiB\n",
	//      hdr_v3->kernel_addr, DIV_ROUND_UP(hdr->kernel_size, 1024));

	int len = 0;
	ulong dtb_size = vb_hdr->dtb_size;
	ulong dtb_addr = vb_hdr->dtb_addr;

	if (*vb_hdr->cmdline) {
		printf("Kernel command line: %s\n", vb_hdr->cmdline);
		len += strlen(vb_hdr->cmdline);
	}

	#ifdef CONFIG_OF_LIBFDT_OVERLAY
	save_dtbo_idx(vb_hdr->cmdline);
	#endif

	char *pbootargs = env_get("bootargs");
	if (pbootargs) {
		int nlen = strlen(pbootargs) + len + 2;
		char *pnewbootargs = malloc(nlen);
		if (pnewbootargs) {
			memset((void *)pnewbootargs,0,nlen);
			sprintf(pnewbootargs,"%s %s",pbootargs,vb_hdr->cmdline);
			env_set("bootargs",pnewbootargs);
			free(pnewbootargs);
			pnewbootargs = NULL;
		}
		else {
			puts("Error: malloc in pnewbootargs failed!\n");
		}
	}
	else {
		puts("Error: add kernel command line in bootargs failed!\n");
	}

	if (os_data) {
		*os_data = (ulong)hdr;
		*os_data += 0x1000; //android R header size
	}
	if (os_len)
		*os_len = hdr->kernel_size;

#if defined(CONFIG_ANDROID_IMG)
	images.ft_len = dtb_size;
	end = dtb_addr;
	images.ft_addr = (char *)end;
#endif

exit:

	return 0;
}


void aml_u8_printf(void *pBuffer, int nSize)
{
	printf("aml log : dump buffer from addr=0x%x\n",(unsigned int)(unsigned long)pBuffer);

	unsigned char *pData = (unsigned char *)pBuffer;
	int nIndex = 0;

	for (nIndex = 0; nIndex < nSize;++nIndex)
	{
		printf("%02x%s",pData[nIndex],
			(nIndex + 1)%16 ? " ":"\n");
	}

	printf("\n");
}

static ulong android_image_get_end_v3(const boot_img_hdr_v3_t *hdr)
{
	if (!p_vender_boot_img)
		return 0;

	/* copy dtb to dtb_addr */
	p_vendor_boot_img_hdr_t vb_hdr = &p_vender_boot_img->hdr;
	unsigned int dtb_offset;
	dtb_offset = (DIV_ROUND_UP(vb_hdr->vendor_ramdisk_size,vb_hdr->page_size)) * vb_hdr->page_size;

	memmove((void*)(unsigned long)vb_hdr->dtb_addr,p_vender_boot_img->szData + dtb_offset,vb_hdr->dtb_size);

	/*??*/
	ulong end;
	/*
	 * The header takes a full page, the remaining components are aligned
	 * on page boundary
	 */

	end = (ulong)hdr;
	end += 0x1000;
	end += ALIGN(hdr->kernel_size, 0x1000);
	end += ALIGN(hdr->ramdisk_size, 0x1000);
	return end;
}

static  ulong android_image_get_kload_v3(const boot_img_hdr_v3_t *hdr)
{
	if (p_vender_boot_img)
		return p_vender_boot_img->hdr.kernel_addr;
	else
		return 0;
}

int android_image_get_ramdisk_v3(const boot_img_hdr_v3_t *hdr,
			      ulong *rd_data, ulong *rd_len)
{
	/*boot image must contain ramdisk*/
	if (!hdr->ramdisk_size ||!p_vender_boot_img)
	{
		if (rd_data)
			*rd_data = 0;
		if (rd_len)
			*rd_len  = 0;
		return -1;
	}

	p_vendor_boot_img_hdr_t vb_hdr = &p_vender_boot_img->hdr;

	debug("RAM disk load addr 0x%08x size %u KiB\n",
	       vb_hdr->ramdisk_addr, DIV_ROUND_UP(hdr->ramdisk_size, 1024));

	/*ramdisk offset of android R boot image*/
	int nOffset = (DIV_ROUND_UP(hdr->kernel_size,4096)+1)*4096;

	unsigned char *pRAMdisk = (unsigned char *)(unsigned long)vb_hdr->ramdisk_addr;

	/* copy ramdisk to ramdisk_addr */
	memmove(pRAMdisk, (char*)(unsigned long)(vb_hdr->kernel_addr + nOffset),hdr->ramdisk_size);
	memmove(pRAMdisk + hdr->ramdisk_size, p_vender_boot_img->szData,vb_hdr->vendor_ramdisk_size);
	//aml_u8_printf(pRAMdisk, 128);
	//aml_u8_printf(pRAMdisk + hdr->ramdisk_size, 128);
	if (rd_data)
		*rd_data = vb_hdr->ramdisk_addr;

	if (rd_len)
		*rd_len  = hdr->ramdisk_size + vb_hdr->vendor_ramdisk_size;

	return 0;
}

static ulong android_image_get_comp_v3(const boot_img_hdr_v3_t *os_hdr)
{
	int i;
	unsigned char *src = (unsigned char *)os_hdr + 0x1000;

	for (i = 0;i< ARRAY_SIZE(arrComp);++i)
	{
		if (!memcmp(arrComp[i].szID,src,arrComp[i].nIDLength))
			return arrComp[i].nCompID;
	}

	return IH_COMP_NONE;
}

static int android_image_need_move_v3(ulong *img_addr, const boot_img_hdr_v3_t *hdr)
{
	ulong kernel_load_addr = android_image_get_kload_v3(hdr);
	ulong img_start = *img_addr;
	ulong val = 0;
	if (kernel_load_addr > img_start)
		val = kernel_load_addr - img_start;
	else
		val = img_start - kernel_load_addr;

	if (android_image_get_comp_v3(hdr) == IH_COMP_NONE)
		return 0;

	if (val < 32*1024*1024) {
		ulong total_size = android_image_get_end_v3(hdr)-(ulong)hdr;
		void *reloc_addr = malloc(total_size);
		if (!reloc_addr) {
			puts("Error: malloc in  android_image_need_move failed!\n");
			return -ENOMEM;
		}
		printf("reloc_addr =%lx\n", (ulong)reloc_addr);
		memset(reloc_addr, 0, total_size);
		memmove(reloc_addr, hdr, total_size);
		*img_addr = (ulong)reloc_addr;
		printf("Copy done\n");
	}
	return 0;
}
