// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <config.h>
#include <common.h>
#include <image.h>
#include <linux/libfdt.h>
#include <android_image.h>
#if defined(CONFIG_ZIRCON_BOOT_IMAGE)
#include <zircon/image.h>
#endif
#include <asm/arch/bl31_apis.h>
#include <asm/arch/secure_apb.h>
#include <amlogic/store_wrapper.h>
#include <amlogic/aml_efuse.h>
#include <malloc.h>
#include <emmc_partitions.h>
#include <version.h>
#include <amlogic/image_check.h>
#include <fs.h>

#ifndef IS_FEAT_BOOT_VERIFY
//#define IS_FEAT_BOOT_VERIFY() 0 //always undefined as IS_FEAT_BOOT_VERIFY is function not marco
#endif// #ifndef IS_FEAT_BOOT_VERIFY
int __attribute__((weak)) store_logic_read(const char *name, loff_t off, size_t size, void *buf)
{ return store_read(name, off, size, buf);}

#define debugP(fmt...) //printf("[Dbg imgread]L%d:", __LINE__),printf(fmt)
#define errorP(fmt...) pr_err("Err imgread(L%d):", __LINE__); pr_err(fmt)
#define wrnP(fmt...)   pr_warning("wrn:" fmt)
#define MsgP(fmt...)   pr_info("[imgread]" fmt)

#define IMG_PRELOAD_SZ  (1U<<20) //Total read 1M at first to read the image header
#define PIC_PRELOAD_SZ  (8U<<10) //Total read 4k at first to read the image header
#define RES_OLD_FMT_READ_SZ (8U<<20)

typedef struct __aml_enc_blk{
        unsigned int  nOffset;
        unsigned int  nRawLength;
        unsigned int  nSigLength;
        unsigned int  nAlignment;
        unsigned int  nTotalLength;
        unsigned char szPad[12];
        unsigned char szSHA2IMG[32];
        unsigned char szSHA2KeyID[32];
}t_aml_enc_blk;

#define AML_SECU_BOOT_IMG_HDR_MAGIC        "AMLSECU!"
#define AML_SECU_BOOT_IMG_HDR_MAGIC_SIZE   (8)
#define AML_SECU_BOOT_IMG_HDR_VESRION      (0x0905)

typedef struct {

        unsigned char magic[AML_SECU_BOOT_IMG_HDR_MAGIC_SIZE];//magic to identify whether it is a encrypted boot image

        unsigned int  version;                  //version for this header struct
        unsigned int  nBlkCnt;

        unsigned char szTimeStamp[16];

        t_aml_enc_blk   amlKernel;
        t_aml_enc_blk   amlRamdisk;
        t_aml_enc_blk   amlDTB;

} amlencryptbootimginfo;

typedef struct _boot_img_hdr_secure_boot
{
    unsigned char           reserve4ImgHdr[1024];

	amlencryptbootimginfo   encrypteImgInfo;

}AmlSecureBootImgHeader;

typedef struct{
    unsigned char           reserve4ImgHdr[2048];

	amlencryptbootimginfo   encrypteImgInfo;

}AmlSecureBootImg9Header;


#define COMPILE_TYPE_ASSERT(expr, t)       typedef char t[(expr) ? 1 : -1]
COMPILE_TYPE_ASSERT(2048 >= sizeof(AmlSecureBootImgHeader), _cc);

static int is_andr_9_image(void* pBuffer)
{

    int nReturn = 0;

    if (!pBuffer)
        goto exit;

    boot_img_hdr_t *pAHdr = (boot_img_hdr_t*)(unsigned long)pBuffer;

    if (pAHdr->header_version)
        nReturn = 1;

exit:

    return nReturn;

}

static int _aml_get_secure_boot_kernel_size(const void *ploadaddr, u32 *ptotalenckernelsz)
{
	const amlencryptbootimginfo *amlencryptebootimginfo = 0;
	int rc = 0;
	u32 securekernelimgsz = 2048;
	u32 nblkcnt = 0;
	const t_aml_enc_blk *pblkinf = NULL;
	int secure_boot_enabled = 0;
	unsigned char *pandhdr = (unsigned char *)ploadaddr;

#ifdef CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK
	secure_boot_enabled = 0;//donnot decrypt kernel/dtb if avb2 enabled
#else
	secure_boot_enabled = IS_FEAT_BOOT_VERIFY();
#endif//#ifdef CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK

#ifdef CONFIG_IMAGE_CHECK
	rc = __LINE__;

	if (!ploadaddr || !ptotalenckernelsz)
		return rc;

	if (secure_boot_enabled) {
		struct aml_boot_header_t *hdr;
		ulong ncheckoffset = android_image_check_offset();

		hdr = (struct aml_boot_header_t *)(pandhdr + ncheckoffset
			- sizeof(struct aml_boot_header_t));
		if (hdr->magic != AML_BOOT_IMAGE_MAGIC || hdr->version != AML_BOOT_IMAGE_VERSION)
			return rc;
		*ptotalenckernelsz = hdr->img_size + ncheckoffset;
		return 0;
	}

	*ptotalenckernelsz = 0;
	return 0;
#endif
	if (is_andr_9_image(pandhdr))
		securekernelimgsz = 4096;

	amlencryptebootimginfo = (amlencryptbootimginfo *)(pandhdr + (securekernelimgsz >> 1));

	nblkcnt = amlencryptebootimginfo->nBlkCnt;

	*ptotalenckernelsz = 0;

	rc = memcmp(AML_SECU_BOOT_IMG_HDR_MAGIC, amlencryptebootimginfo->magic,
		AML_SECU_BOOT_IMG_HDR_MAGIC_SIZE);
	if (rc) { // img NOT signed
		if (secure_boot_enabled) {
			errorP("img NOT signed but secure boot enabled\n");
			return __LINE__;
		}
		*ptotalenckernelsz = 0;
		return 0;
	}
	//img signed
	if (!secure_boot_enabled) {
		errorP("Img signed but secure boot NOT enabled\n");
		return __LINE__;
	}

	if (amlencryptebootimginfo->version != AML_SECU_BOOT_IMG_HDR_VESRION) {
		errorP("magic ok but version err, err ver=0x%x\n", amlencryptebootimginfo->version);
		return __LINE__;
	}
	MsgP("szTimeStamp[%s]\n", (char *)&amlencryptebootimginfo->szTimeStamp);
	debugP("nblkcnt=%d\n", nblkcnt);

	for (pblkinf = &amlencryptebootimginfo->amlKernel; nblkcnt--; ++pblkinf) {
		const unsigned int thisblklen = pblkinf->nTotalLength;

		debugP("thisblklen=0x%x\n", thisblklen);
		securekernelimgsz += thisblklen;
	}

	*ptotalenckernelsz = securekernelimgsz;
	return 0;
}

static int do_image_read_dtb_from_knl(const char *partname,
	unsigned char *loadaddr, uint64_t lflashreadinitoff)
{
	int ret = __LINE__;
	unsigned int nflashloadlen = 0;
	u64 lflashreadoff = 0;
	const int preloadsz = 4096 * 2;
	int pagesz = 0;
	boot_img_hdr_t *hdr_addr = (boot_img_hdr_t *)loadaddr;

	lflashreadoff = lflashreadinitoff;
	nflashloadlen = preloadsz;//head info is one page size == 2k
	debugP("sizeof preloadsz=%u\n", nflashloadlen);
	ret = store_logic_read(partname, lflashreadoff, nflashloadlen, loadaddr);
	if (ret) {
		errorP("Fail to read 0x%xB from part[%s] at offset 0\n", nflashloadlen, partname);
		return __LINE__;
	}

	if (genimg_get_format(hdr_addr) != IMAGE_FORMAT_ANDROID) {
		errorP("Fmt unsupported! only support 0x%x\n", IMAGE_FORMAT_ANDROID);
		return __LINE__;
	}

	if (is_android_r_image((void *)hdr_addr)) {
		const int preloadsz_r = preloadsz;
		int rc_r = 0;
		char *slot_name;

		slot_name = env_get("slot-suffixes");
		if (strcmp(slot_name, "0") == 0)
			strcpy((char *)partname, "vendor_boot_a");
		else if (strcmp(slot_name, "1") == 0)
			strcpy((char *)partname, "vendor_boot_b");
		else
			strcpy((char *)partname, "vendor_boot");

		MsgP("partname = %s\n", partname);
		nflashloadlen = preloadsz_r;//head info is one page size == 4k
		debugP("sizeof preloadSz=%u\n", nflashloadlen);

		ret = store_logic_read(partname, lflashreadoff, nflashloadlen, loadaddr);
		if (ret) {
			errorP("Fail to read 0x%xB from part[%s] at offset 0\n",
				nflashloadlen, partname);
			return __LINE__;
		}

		p_vendor_boot_img_hdr_t pvendorimghdr = (p_vendor_boot_img_hdr_t)loadaddr;

		rc_r = vendor_boot_image_check_header(pvendorimghdr);
		if (!rc_r) {
			unsigned long ramdisk_size_r, dtb_size_r;

			pagesz = pvendorimghdr->page_size;

			/* Android R's vendor_boot partition include ramdisk and dtb */
			ramdisk_size_r = ALIGN(pvendorimghdr->vendor_ramdisk_size, pagesz);
			dtb_size_r = ALIGN(pvendorimghdr->dtb_size, pagesz);
			nflashloadlen = dtb_size_r;
			lflashreadoff = ramdisk_size_r + 0x1000;
			debugP("ramdisk_size_r 0x%x, totalSz 0x%lx\n",
				pvendorimghdr->vendor_ramdisk_size, ramdisk_size_r);
			debugP("dtb_size_r 0x%x, totalSz 0x%lx\n",
				pvendorimghdr->dtb_size, dtb_size_r);
			debugP("lflashreadoff=0x%llx\n", lflashreadoff);
			debugP("nflashloadlen=0x%x\n", nflashloadlen);
		} else {
			errorP("check vendor_boot header error\n");
			return __LINE__;
		}

	} else {
		pagesz = hdr_addr->page_size;
		lflashreadoff += pagesz;
		lflashreadoff += ALIGN(hdr_addr->kernel_size, pagesz);
		lflashreadoff += ALIGN(hdr_addr->ramdisk_size, pagesz);
		nflashloadlen  = ALIGN(hdr_addr->second_size, pagesz);
	}

	debugP("lflashreadoff=0x%llx, nflashloadlen=0x%x\n", lflashreadoff, nflashloadlen);
	debugP("page sz %u\n", hdr_addr->page_size);
	if (!nflashloadlen) {
		errorP("NO second part in kernel image\n");
		return __LINE__;
	}
	unsigned char *secondaddr = (unsigned char *)loadaddr + lflashreadoff;

	loff_t wroff = lflashreadoff;
	size_t wrsz  = nflashloadlen;
	unsigned char *wraddr = secondaddr;
#if !defined(CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK) && defined(CONFIG_IMAGE_CHECK)
	u32 securekernelimgsz = 0;

	ret = _aml_get_secure_boot_kernel_size(loadaddr, &securekernelimgsz);
	if (ret) {
		errorP("Fail in _aml_get_secure_boot_kernel_size, rc=%d\n", ret);
		return __LINE__;
	}

	if (securekernelimgsz) {
		debugP("secure kernel sz 0x%x\n", securekernelimgsz);
		wrsz  = securekernelimgsz - preloadsz;
		wroff = lflashreadinitoff + preloadsz;
		wraddr = (unsigned char *)loadaddr + preloadsz;
	}
#endif//#if !defined(CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK) && defined(CONFIG_IMAGE_CHECK)
	if ((BOOT_NAND_MTD == store_get_type() || BOOT_SNAND == store_get_type())) {
		unsigned notAlignSz = wroff & 0xfff;

		wroff -= notAlignSz;
		wrsz  += notAlignSz;
		secondaddr += notAlignSz;
		MsgP("not align dtb off 0x%llx\n", wroff);
	}
	ret = store_logic_read(partname, wroff, wrsz, wraddr);
	if (ret) {
		errorP("Fail to read 0x%xB from part[%s] at offset 0x%x\n",
			(unsigned int)wrsz, partname, (unsigned int)wroff);
		return __LINE__;
	}
#ifndef CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK
	if (IS_FEAT_BOOT_VERIFY()) {
#ifndef CONFIG_IMAGE_CHECK
		//because secure boot will use DMA which need disable MMU temp
		//here must update the cache, otherwise nand will fail (eMMC is OK)
		flush_cache((unsigned long)secondaddr, (unsigned long)nflashloadlen);

		ret = aml_sec_boot_check(AML_D_P_IMG_DECRYPT, (unsigned long)loadaddr,
			GXB_IMG_SIZE, GXB_IMG_DEC_DTB);
#else
		//because secure boot will use DMA which need disable MMU temp
		//here must update the cache, otherwise nand will fail (eMMC is OK)
		flush_cache((unsigned long)loadaddr, (unsigned long)securekernelimgsz);

		ret = secure_image_check((uint8_t *)(unsigned long)loadaddr,
			GXB_IMG_SIZE, GXB_IMG_DEC_DTB);
		secondaddr += android_image_check_offset();
#endif
		if (ret) {
			errorP("\n[dtb]aml log : Sig Check is %d\n", ret);
			return __LINE__;
		}
		MsgP("decrypted dtb sz 0x%x\n", nflashloadlen);
	}
#endif
	char *dtdestaddr = (char *)loadaddr;//simple_strtoull(getenv("dtb_mem_addr"), NULL, 0);

	memmove(dtdestaddr, secondaddr, nflashloadlen);

	return ret;
}

/*uint32_t store_rsv_size(const char *name);*/
static int do_image_read_dtb_from_rsv(unsigned char* loadaddr)
{
	const int dtbmaxsz = store_rsv_size("dtb");

	if (dtbmaxsz < 0x400) {
		errorP("dtbmaxsz(0x%x) invalid\n", dtbmaxsz);
		return -__LINE__;
	}
	int ret = store_rsv_read("dtb", dtbmaxsz, loadaddr);

	if (ret) {
		errorP("Fail read dtb from rsv with sz 0x%x\n", dtbmaxsz);
		return -__LINE__;
	}
#ifndef CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK
	if (IS_FEAT_BOOT_VERIFY()) {
		flush_cache((unsigned long)loadaddr, dtbmaxsz);
#ifndef CONFIG_IMAGE_CHECK
		ret = aml_sec_boot_check(AML_D_P_IMG_DECRYPT, (long)loadaddr, dtbmaxsz, 0);
#else
		ret = secure_image_check((uint8_t *)(unsigned long)loadaddr, dtbmaxsz, 0);
		memmove(loadaddr, (void *)(loadaddr + sizeof(struct aml_boot_header_t)), dtbmaxsz);
#endif
		if (ret) {
			MsgP("decrypt dtb: Sig Check %d\n", ret);
			return -__LINE__;
		}
	}
#endif
	return 0;
}

//imgread dtb boot ${dtb_mem_addr}
//imgread dtb rsv ${dtb_mem_addr}
static int do_image_read_dtb(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int iRet = 0;
    const char* const partName = argv[1];
    unsigned char* loadaddr = 0;
    uint64_t lflashReadOff = 0;
    if (2 < argc) {
        loadaddr = (unsigned char*)simple_strtoul(argv[2], NULL, 16);
    } else{
        loadaddr = (unsigned char*)simple_strtoul(env_get("loadaddr"), NULL, 16);
    }

    if (3 < argc) lflashReadOff = simple_strtoull(argv[3], NULL, 0) ;

    const int fromRsv = !strcmp("_aml_dtb", argv[1]);
    if ( fromRsv ) {
        iRet = do_image_read_dtb_from_rsv(loadaddr);
    } else {
        iRet = do_image_read_dtb_from_knl(partName, loadaddr, lflashReadOff);
    }
	if (iRet) {
		errorP("Fail read dtb from %s, ret %d\n", partName, iRet);
		return CMD_RET_FAILURE;
	}

    unsigned long fdtAddr = (unsigned long)loadaddr;
#ifdef CONFIG_MULTI_DTB
    extern unsigned long get_multi_dt_entry(unsigned long fdt_addr);
    fdtAddr = get_multi_dt_entry((unsigned long)fdtAddr);
    if (!fdtAddr) {
        errorP("Fail in get_multi_dt_entry\n");
        return __LINE__;
    }
#endif// #ifdef CONFIG_MULTI_DTB
    iRet = fdt_check_header((char*)fdtAddr);
    if (iRet) {
        errorP("Fail in fdt check header\n");
        return CMD_RET_FAILURE;
    }
    const unsigned fdtsz    = fdt_totalsize((char*)fdtAddr);
    memmove(loadaddr, (char*)fdtAddr, fdtsz);

    return iRet;
}

static int do_image_read_kernel(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned    kernel_size;
    unsigned    ramdisk_size;
    boot_img_hdr_t *hdr_addr = NULL;
    int genFmt = 0;
	u32 actualbootimgsz = 0;
	u32 dtbsz = 0;
	const char *const partname = argv[1];
    unsigned char* loadaddr = 0;
    int rc = 0;
	u64 flashreadoff = 0;
	u32 securekernelimgsz = 0;
	char *upgrade_step_s = NULL;
	bool cache_flag = false;

	if (argc > 2)
		loadaddr = (unsigned char *)simple_strtoul(argv[2], NULL, 16);
	else
		loadaddr = (unsigned char *)simple_strtoul(env_get("loadaddr"), NULL, 16);

	hdr_addr = (boot_img_hdr_t *)loadaddr;

	if (argc > 3)
		flashreadoff = simple_strtoull(argv[3], NULL, 0);

	upgrade_step_s = env_get("upgrade_step");
	if (upgrade_step_s && (strcmp(upgrade_step_s, "3") == 0) &&
		(strcmp(partname, "recovery") == 0)) {
		loff_t len_read;

		MsgP("read recovery.img from cache\n");
		rc = fs_set_blk_dev("mmc", "1:2", FS_TYPE_EXT);
		if (rc) {
			errorP("Fail to set blk dev cache\n");
			cache_flag = false;
		} else {
			fs_read("/recovery/recovery.img", (unsigned long)loadaddr,
					flashreadoff, IMG_PRELOAD_SZ, &len_read);
			if (IMG_PRELOAD_SZ != len_read) {
				errorP("Fail to read recovery.img from cache\n");
				cache_flag = false;
			} else {
				cache_flag = true;
			}
		}
	}

	if (!cache_flag) {
		MsgP("read from part: %s\n", partname);
		rc = store_logic_read(partname, flashreadoff, IMG_PRELOAD_SZ, loadaddr);
		if (rc) {
			errorP("Fail to read 0x%xB from part[%s] at offset 0\n",
				IMG_PRELOAD_SZ, partname);
			return __LINE__;
		}
	}
	flashreadoff += IMG_PRELOAD_SZ;

	if (!is_android_r_image((void *)hdr_addr)) {

		/*free vendor buffer first*/
		if (p_vender_boot_img) {
			free(p_vender_boot_img);
			p_vender_boot_img = 0;
		}
        genFmt = genimg_get_format(hdr_addr);
#if defined(CONFIG_ZIRCON_BOOT_IMAGE)
	if (IMAGE_FORMAT_ANDROID != genFmt && IMAGE_FORMAT_ZIRCON != genFmt) {
		errorP("Fmt 0x%x unsupported!, supported genFmt 0x%x or 0x%x\n", genFmt,
				IMAGE_FORMAT_ANDROID, IMAGE_FORMAT_ZIRCON);
#else
        if (IMAGE_FORMAT_ANDROID != genFmt) {
            errorP("Fmt unsupported!genFmt 0x%x != 0x%x\n", genFmt, IMAGE_FORMAT_ANDROID);
#endif
            return __LINE__;
        }

		//Check if encrypted image
		rc = _aml_get_secure_boot_kernel_size(loadaddr, &securekernelimgsz);
        if (rc) {
            errorP("Fail in _aml_get_secure_boot_kernel_size, rc=%d\n", rc);
            return __LINE__;
        }
		if (securekernelimgsz) {
			actualbootimgsz = securekernelimgsz;
			MsgP("securekernelimgsz=0x%x\n", actualbootimgsz);
		}
        else {
            kernel_size     =(hdr_addr->kernel_size + (hdr_addr->page_size-1)+hdr_addr->page_size)&(~(hdr_addr->page_size -1));
            ramdisk_size    =(hdr_addr->ramdisk_size + (hdr_addr->page_size-1))&(~(hdr_addr->page_size -1));
			dtbsz           = hdr_addr->second_size;
			actualbootimgsz = kernel_size + ramdisk_size + dtbsz;
            debugP("kernel_size 0x%x, page_size 0x%x, totalSz 0x%x\n", hdr_addr->kernel_size, hdr_addr->page_size, kernel_size);
            debugP("ramdisk_size 0x%x, totalSz 0x%x\n", hdr_addr->ramdisk_size, ramdisk_size);
			debugP("dtbSz 0x%x, Total actualbootimgsz 0x%x\n", dtbsz, actualbootimgsz);
        }

#if defined(CONFIG_ZIRCON_BOOT_IMAGE)
	if (genFmt == IMAGE_FORMAT_ZIRCON) {
		const zbi_header_t *zbi = (zbi_header_t *)hdr_addr;

		actualbootimgsz = zbi->length + sizeof(*zbi);
	}
#endif//#if defined(CONFIG_ZIRCON_BOOT_IMAGE)

		if (actualbootimgsz > IMG_PRELOAD_SZ) {
			const u32 leftsz = actualbootimgsz - IMG_PRELOAD_SZ;

			debugP("Left sz 0x%x\n", leftsz);
			rc = store_logic_read(partname, flashreadoff,
				leftsz, loadaddr + IMG_PRELOAD_SZ);
			if (rc) {
				errorP("Fail to read 0x%xB from part[%s] at offset 0x%x\n",
					leftsz, partname, IMG_PRELOAD_SZ);
				return __LINE__;
			}
		}
		debugP("totalSz=0x%x\n", actualbootimgsz);

		//because secure boot will use DMA which need disable MMU temp
		//here must update the cache, otherwise nand will fail (eMMC is OK)
		flush_cache((unsigned long)loadaddr, (unsigned long)actualbootimgsz);

		return 0;
    }
    else {
	char partname_init[32] = {0};
	u64 rc_init;
	char *slot_name;
	p_boot_img_hdr_v3_t hdr_addr_v3 = NULL;

	init_boot_ramdisk_size = 0;
	slot_name = env_get("slot-suffixes");
	if (slot_name && (strcmp(slot_name, "0") == 0))
		strcpy((char *)partname_init, "init_boot_a");
	else if (slot_name && (strcmp(slot_name, "1") == 0))
		strcpy((char *)partname_init, "init_boot_b");
	else
		strcpy((char *)partname_init, "init_boot");

	rc_init = store_part_size(partname_init);

        /*free vendor buffer first*/
	 if (p_vender_boot_img) {
            free(p_vender_boot_img);
            p_vender_boot_img = 0;
        }

        genFmt = genimg_get_format(hdr_addr);
	 if (IMAGE_FORMAT_ANDROID != genFmt) {
            errorP("Fmt unsupported!genFmt 0x%x != 0x%x\n", genFmt, IMAGE_FORMAT_ANDROID);
            return __LINE__;
        }

        //Check if encrypted image
		rc = _aml_get_secure_boot_kernel_size(loadaddr, &securekernelimgsz);
		if (rc) {
			errorP("Fail in _aml_get_secure_boot_kernel_size, rc=%d\n", rc);
			return __LINE__;
		}

		hdr_addr_v3 = (p_boot_img_hdr_v3_t)hdr_addr;
		kernel_size = ALIGN(hdr_addr_v3->kernel_size, 0x1000);

		ramdisk_size = ALIGN(hdr_addr_v3->ramdisk_size, 0x1000);

		MsgP("kernel_size 0x%x, totalSz 0x%x\n",
			hdr_addr_v3->kernel_size, kernel_size);
		MsgP("ramdisk_size 0x%x, totalSz 0x%x\n",
			hdr_addr_v3->ramdisk_size, ramdisk_size);
		MsgP("boot header_version = %d\n", hdr_addr_v3->header_version);
		if (securekernelimgsz) {
			actualbootimgsz = securekernelimgsz;
			MsgP("securekernelimgsz=0x%x\n", actualbootimgsz);
		} else {
			actualbootimgsz = kernel_size + ramdisk_size + 0x1000;
		}

		if (actualbootimgsz > IMG_PRELOAD_SZ) {
			const u32 leftsz = actualbootimgsz - IMG_PRELOAD_SZ;

			debugP("Left sz 0x%x\n", leftsz);

			if (upgrade_step_s && (strcmp(upgrade_step_s, "3") == 0) &&
				(strcmp(partname, "recovery") == 0)) {
				loff_t len_read;

				MsgP("read recovery.img from cache\n");
				rc = fs_set_blk_dev("mmc", "1:2", FS_TYPE_EXT);
				if (rc) {
					errorP("Fail to set blk dev cache\n");
					cache_flag = false;
				} else {
					fs_read("/recovery/recovery.img",
							(unsigned long)loadaddr,
							0, actualbootimgsz, &len_read);
					if (actualbootimgsz != len_read) {
						errorP("Fail to read recovery.img from cache\n");
						cache_flag = false;
					} else {
						cache_flag = true;
					}
				}
			}
			if (!cache_flag) {
				MsgP("read from part: %s\n", partname);
				rc = store_logic_read(partname, flashreadoff,
					leftsz, loadaddr + IMG_PRELOAD_SZ);
				if (rc) {
					errorP("Fail to read 0x%xB from part[%s] at offset 0x%x\n",
						leftsz, partname, IMG_PRELOAD_SZ);
					return __LINE__;
				}

				if (rc_init != -1) {
					MsgP("read header from part: %s\n", partname_init);
					unsigned int nflashloadlen_init = 0;
					const int preloadsz_init = 0x1000 * 2;
					unsigned char *pbuffpreload_init = 0;

					nflashloadlen_init = preloadsz_init;
					debugP("sizeof preloadSz=%u\n", nflashloadlen_init);

					pbuffpreload_init = malloc(preloadsz_init);

					if (!pbuffpreload_init) {
						printf("Fail to allocate memory for %s!\n",
							partname_init);
						return __LINE__;
					}

					rc = store_logic_read(partname_init, 0,
						nflashloadlen_init, pbuffpreload_init);
					if (rc) {
						errorP("Fail to read 0x%xB from part[%s]\n",
							nflashloadlen_init, partname_init);
						free(pbuffpreload_init);
						pbuffpreload_init = 0;
						return __LINE__;
					}

					p_boot_img_hdr_v3_t pinitbootimghdr;

					pinitbootimghdr = (p_boot_img_hdr_v3_t)pbuffpreload_init;

					ramdisk_size = ALIGN(pinitbootimghdr->ramdisk_size,
							0x1000);
					MsgP("ramdisk_size 0x%x, totalSz 0x%x\n",
						pinitbootimghdr->ramdisk_size, ramdisk_size);
					MsgP("init_boot header_version = %d\n",
						pinitbootimghdr->header_version);
					init_boot_ramdisk_size = pinitbootimghdr->ramdisk_size;

					if (init_boot_ramdisk_size != 0) {
						MsgP("read ramdisk from part: %s\n", partname_init);
						rc = store_logic_read(partname_init,
							BOOT_IMG_V3_HDR_SIZE,
							ramdisk_size,
							loadaddr + kernel_size
							+ BOOT_IMG_V3_HDR_SIZE);
						if (rc) {
							errorP("Fail to read 0x%xB from part[%s]\n",
								ramdisk_size, partname_init);
							free(pbuffpreload_init);
							pbuffpreload_init = 0;
							return __LINE__;
						}
					}
					free(pbuffpreload_init);
					pbuffpreload_init = 0;
				}
			}
		}
		debugP("totalSz=0x%x\n", actualbootimgsz);
		/*
		 *because secure boot will use DMA which need disable MMU temp
		 *here must update the cache, otherwise nand will fail (eMMC is OK)
		 */
		flush_cache((unsigned long)loadaddr, (unsigned long)actualbootimgsz);

		/*
		 *Android R need read vendor_boot partition
		 *define Android R variable add suffix xxx_r
		 */
		char partname_r[32] = {0};
		int ret_r = __LINE__;
		u64 lflashreadoff_r = 0;
		unsigned int nflashloadlen_r = 0;
		const int preloadsz_r = 0x1000 * 2;//4k not enough for signed
		unsigned char *pbuffpreload = 0;
		int rc_r = 0;

		if (strcmp(slot_name, "0") == 0)
			strcpy((char *)partname_r, "vendor_boot_a");
		else if (strcmp(slot_name, "1") == 0)
			strcpy((char *)partname_r, "vendor_boot_b");
		else
			strcpy((char *)partname_r, "vendor_boot");

		MsgP("partname_r = %s\n", partname_r);

		nflashloadlen_r = preloadsz_r;		//head info is one page size == 4k
		debugP("sizeof preloadSz=%u\n", nflashloadlen_r);

		pbuffpreload = malloc(preloadsz_r);

		if (!pbuffpreload) {
			printf("aml log : system error! Fail to allocate memory for %s!\n",
				partname_r);
			return __LINE__;
		}

		if (upgrade_step_s && (strcmp(upgrade_step_s, "3") == 0) &&
			(strcmp(partname_r, "vendor_boot") == 0)) {
			loff_t len_read;

			MsgP("read vendor_boot from cache\n");
			ret_r = fs_set_blk_dev("mmc", "1:2", FS_TYPE_EXT);
			if (ret_r) {
				errorP("Fail to set blk dev cache\n");
				cache_flag = false;
			} else {
				fs_read("/recovery/vendor_boot.img",
							(unsigned long)pbuffpreload,
							0, nflashloadlen_r, &len_read);
				if (nflashloadlen_r != len_read) {
					errorP("Fail to read vendor_boot.img from cache\n");
					cache_flag = false;
				} else {
					cache_flag = true;
				}
			}
		}
		if (!cache_flag) {
			MsgP("read from part: %s\n", partname_r);
			ret_r = store_logic_read(partname_r, lflashreadoff_r,
				nflashloadlen_r, pbuffpreload);
			if (ret_r) {
				errorP("Fail to read 0x%xB from part[%s] at offset 0\n",
					nflashloadlen_r, partname_r);
				free(pbuffpreload);
				pbuffpreload = 0;
				return __LINE__;
			}
		}
		p_vendor_boot_img_hdr_t pvendorimghdr = (p_vendor_boot_img_hdr_t)pbuffpreload;

		rc_r = vendor_boot_image_check_header(pvendorimghdr);
		if (!rc_r) {
			//Check if encrypted image
			rc_r = _aml_get_secure_boot_kernel_size(pbuffpreload, &securekernelimgsz);
			if (rc_r) {
				errorP("Fail in _aml_get_secure_boot_kernel_size, rc=%d\n", rc_r);
				free(pbuffpreload);
				pbuffpreload = 0;
				return __LINE__;
			}
			if (securekernelimgsz) {
				nflashloadlen_r = securekernelimgsz;
				MsgP("securekernelimgsz=0x%x\n", nflashloadlen_r);
			} else {
				unsigned long ramdisk_size_r, dtb_size_r;
				unsigned long ramdisk_table_size;
				const int pagesz_r = pvendorimghdr->page_size;

				/* Android R's vendor_boot partition include ramdisk and dtb */
				ramdisk_size_r = ALIGN(pvendorimghdr->vendor_ramdisk_size,
					pagesz_r);
				dtb_size_r      = ALIGN(pvendorimghdr->dtb_size, pagesz_r);
				nflashloadlen_r = ramdisk_size_r + dtb_size_r + 0x1000;
				MsgP("ramdisk_size_r 0x%x, totalSz 0x%lx\n",
					pvendorimghdr->vendor_ramdisk_size, ramdisk_size_r);
				MsgP("dtb_size_r 0x%x, totalSz 0x%lx\n",
					pvendorimghdr->dtb_size, dtb_size_r);
				MsgP("vendor_boot header_version = %d\n",
					pvendorimghdr->header_version);
				if (pvendorimghdr->header_version > 3) {
					MsgP("vendor_ramdisk_table_size: 0x%x\n",
					pvendorimghdr->vendor_ramdisk_table_size);
					MsgP("vendor_ramdisk_table_entry_num: 0x%x\n",
					pvendorimghdr->vendor_ramdisk_table_entry_num);
					MsgP("vendor_ramdisk_table_entry_size: 0x%x\n",
					pvendorimghdr->vendor_ramdisk_table_entry_size);
					MsgP("vendor_bootconfig_size: 0x%x\n",
					pvendorimghdr->vendor_bootconfig_size);
					ramdisk_table_size =
						ALIGN(pvendorimghdr->vendor_ramdisk_table_size,
						pagesz_r);
					nflashloadlen_r = nflashloadlen_r + ramdisk_table_size;

					MsgP("ramdisk table offset 0x%lx, nflashloadlen_r 0x%x\n",
					ramdisk_size_r + dtb_size_r + 0x1000, nflashloadlen_r);
				}
			}

			if (nflashloadlen_r > preloadsz_r) {
				free(pbuffpreload);
				pbuffpreload = malloc(nflashloadlen_r);
				if (!pbuffpreload)
					return __LINE__;
				if (upgrade_step_s && (strcmp(upgrade_step_s, "3") == 0) &&
					(strcmp(partname_r, "vendor_boot") == 0)) {
					loff_t len_read;

					MsgP("recovery mode, read vendor_boot from cache\n");
					ret_r = fs_set_blk_dev("mmc", "1:2", FS_TYPE_EXT);
					if (ret_r) {
						errorP("Fail to set blk dev cache\n");
						cache_flag = false;
					} else {
						fs_read("/recovery/vendor_boot.img",
								(unsigned long)pbuffpreload,
								lflashreadoff_r, nflashloadlen_r,
								&len_read);
						if (nflashloadlen_r != len_read) {
							errorP("Fail to read from cache\n");
							cache_flag = false;
						} else {
							cache_flag = true;
						}
					}
				}
				if (!cache_flag) {
					MsgP("read from part: %s\n", partname_r);
					rc_r = store_logic_read(partname_r, lflashreadoff_r,
						nflashloadlen_r, pbuffpreload);
					if (rc_r) {
						errorP("Fail read 0x%xB from %s at offset 0x%x\n",
							(unsigned int)nflashloadlen_r, partname_r,
							(unsigned int)lflashreadoff_r);
						free(pbuffpreload);
						pbuffpreload = 0;
						return __LINE__;
					}
				}
			}

			debugP("totalSz=0x%x\n", nflashloadlen_r);
			flush_cache((unsigned long)pbuffpreload, nflashloadlen_r);
#ifndef CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK
			if (IS_FEAT_BOOT_VERIFY()) {
#ifndef CONFIG_IMAGE_CHECK
				rc_r = aml_sec_boot_check(AML_D_P_IMG_DECRYPT,
						(unsigned long)pbuffpreload,
						GXB_IMG_SIZE, GXB_IMG_DEC_DTB);
#else
				rc_r = secure_image_check(pbuffpreload, GXB_IMG_SIZE,
						GXB_IMG_DEC_DTB);
				/*pbuffpreload += android_image_check_offset();*/
				memmove(pbuffpreload, pbuffpreload + android_image_check_offset(),
						nflashloadlen_r - android_image_check_offset());
#endif
				if (rc_r) {
					errorP("\n[vendor_boot]aml log : Sig Check is %d\n", rc_r);
					return __LINE__;
				}
				MsgP("vendor_boot decrypt at 0x%p\n", pbuffpreload);
			}
#endif//#ifndef CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK

			p_vender_boot_img = (p_vendor_boot_img_t)pbuffpreload;
		} else {
			free(pbuffpreload);
			pbuffpreload = 0;
		}
	} /*ANDROID R/S*/
    return 0;
}

#define AML_RES_IMG_VERSION_V1      (0x01)
#define AML_RES_IMG_VERSION_V2      (0x02)
#define AML_RES_IMG_V1_MAGIC_LEN    8
#define AML_RES_IMG_V1_MAGIC        "AML_RES!"//8 chars
#define AML_RES_IMG_ITEM_ALIGN_SZ   16
#define AML_RES_IMG_HEAD_SZ         (AML_RES_IMG_ITEM_ALIGN_SZ * 4)//64
#define AML_RES_ITEM_HEAD_SZ        (AML_RES_IMG_ITEM_ALIGN_SZ * 4)//64

//typedef for amlogic resource image
#pragma pack(push, 4)
typedef struct {
    __u32   crc;    //crc32 value for the resouces image
    __s32   version;//current version is 0x01

    __u8    magic[AML_RES_IMG_V1_MAGIC_LEN];  //resources images magic

    __u32   imgSz;  //total image size in byte
    __u32   imgItemNum;//total item packed in the image

    __u32   alignSz;//AML_RES_IMG_ITEM_ALIGN_SZ
    __u8    reserv[AML_RES_IMG_HEAD_SZ - 8 * 3 - 4];

}AmlResImgHead_t;
#pragma pack(pop)

#define LOGO_OLD_FMT_READ_SZ (8U<<20)//if logo format old, read 8M

static int img_res_check_log_header(const AmlResImgHead_t* pResImgHead)
{
    int rc = 0;

    rc = memcmp(pResImgHead->magic, AML_RES_IMG_V1_MAGIC, AML_RES_IMG_V1_MAGIC_LEN);
    if (rc) {
        debugP("Magic error for res\n");
        return 1;
    }
    if (AML_RES_IMG_VERSION_V2 != pResImgHead->version) {
        errorP("res version 0x%x != 0x%x\n", pResImgHead->version, AML_RES_IMG_VERSION_V2);
        return 2;
    }

    return 0;
}

static int do_image_read_res(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    const char* const partName = argv[1];
    unsigned char* loadaddr = 0;
    int rc = 0;
    AmlResImgHead_t* pResImgHead = NULL;
    unsigned totalSz    = 0;
    uint64_t flashReadOff = 0;

    if (2 < argc) {
        loadaddr = (unsigned char*)simple_strtoul(argv[2], NULL, 16);
    }
    else{
        loadaddr = (unsigned char*)simple_strtoul(env_get("loadaddr"), NULL, 16);
    }
    pResImgHead = (AmlResImgHead_t*)loadaddr;

    rc = store_logic_read(partName, flashReadOff, IMG_PRELOAD_SZ, loadaddr);
    if (rc) {
        errorP("Fail to read 0x%xB from part[%s] at offset 0\n", IMG_PRELOAD_SZ, partName);
        return __LINE__;
    }
    flashReadOff = IMG_PRELOAD_SZ;

    if (img_res_check_log_header(pResImgHead)) {
        errorP("Logo header err.\n");
        return __LINE__;
    }

    //Read the actual size of the new version res image
    totalSz = pResImgHead->imgSz;
    if (totalSz > IMG_PRELOAD_SZ )
    {
        const unsigned leftSz = totalSz - flashReadOff;

        rc = store_logic_read(partName, flashReadOff, leftSz, loadaddr + (unsigned)flashReadOff);
        if (rc) {
            errorP("Fail to read 0x%xB from part[%s] at offset 0x%x\n", leftSz, partName, IMG_PRELOAD_SZ);
            return __LINE__;
        }
    }
    debugP("totalSz=0x%x\n", totalSz);

    return 0;
}

#define IH_MAGIC	0x27051956	/* Image Magic Number		*/
#define IH_NMLEN		32	/* Image Name Length		*/

#pragma pack(push, 1)
typedef struct pack_header{
	unsigned int 	magic;	/* Image Header Magic Number	*/
	unsigned int 	hcrc;	/* Image Header CRC Checksum	*/
	unsigned int	size;	/* Image Data Size		*/
	unsigned int	start;	/* item data offset in the image*/
	unsigned int	end;		/* Entry Point Address		*/
	unsigned int	next;	/* Next item head offset in the image*/
	unsigned int	dcrc;	/* Image Data CRC Checksum	*/
	unsigned char	index;		/* Operating System		*/
	unsigned char	nums;	/* CPU architecture		*/
	unsigned char   type;	/* Image Type			*/
	unsigned char 	comp;	/* Compression Type		*/
	char 	name[IH_NMLEN];	/* Image Name		*/
}AmlResItemHead_t;
#pragma pack(pop)

#define CONFIG_MAX_PIC_LEN (12 << 20)
static const unsigned char gzip_magic[] = { 0x1f, 0x8b };

//uncompress known format for 'imgread pic'
static int imgread_uncomp_pic(unsigned char* srcAddr, const unsigned srcSz,
        unsigned char* dstAddr, const unsigned dstBufSz, unsigned long* dstDatSz)
{
    /*debugP("srcAddr[%x, %x]\n", srcAddr[0], srcAddr[1]);*/
    if (!memcmp(srcAddr, gzip_magic, sizeof(gzip_magic)))
    {
        *dstDatSz = srcSz;
        return gunzip(dstAddr, dstBufSz, srcAddr, dstDatSz);
    }

    return 0;
}

//[imgread pic] logo bootup $loadaddr_misc
static int do_image_read_pic(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    const char* const partName = argv[1];
    unsigned char* loadaddr = 0;
    int rc = 0;
    const AmlResImgHead_t* pResImgHead = NULL;
    //unsigned totalSz    = 0;
    uint64_t flashReadOff = 0;
    const unsigned PreloadSz = PIC_PRELOAD_SZ;//preload 8k, 124-1 pic header, If you need pack more than 123 items,  fix this
    unsigned itemIndex = 0;
    const AmlResItemHead_t* pItem = NULL;
    const char* picName = argv[2];

    loadaddr = (unsigned char*)simple_strtoul(argc > 3 ? argv[3] : env_get("loadaddr_misc"), NULL, 16);

    pResImgHead = (AmlResImgHead_t*)loadaddr;

    debugP("to read pic (%s)\n", picName);
    rc = store_logic_read(partName, flashReadOff, PreloadSz, loadaddr);
    if (rc) {
        errorP("Fail to read 0x%xB from part[%s] at offset 0\n", PreloadSz, partName);
        return __LINE__;
    }
    flashReadOff = PreloadSz;
    debugP("end read pic sz %d\n", PreloadSz);

    if (img_res_check_log_header(pResImgHead)) {
        errorP("Logo header err.\n");
        return __LINE__;
    }

    //correct bootup for mbox
    while (!strcmp("bootup", picName))
    {
            char* outputmode = env_get("outputmode");
            if (!outputmode)break;//not env outputmode

            rc = !strncmp("720", outputmode, 3) || !strncmp("576", outputmode, 3) || !strncmp("480", outputmode, 3);
            if (rc) {
                    picName = "bootup_720";
                    break;
            }

            picName = "bootup_1080";
            break;
    }

    pItem = (AmlResItemHead_t*)(pResImgHead + 1);
    for (itemIndex = 0; itemIndex < pResImgHead->imgItemNum; ++itemIndex, ++pItem)
    {
            if (IH_MAGIC != pItem->magic) {
                    errorP("item magic 0x%x != 0x%x\n", pItem->magic, IH_MAGIC);
                    return __LINE__;
            }
            if (!strcmp(picName, pItem->name) || !strcmp(argv[2], pItem->name))
            {
                    char env_name[IH_NMLEN*2];
                    char env_data[IH_NMLEN*2];
                    unsigned long picLoadAddr = (unsigned long)loadaddr + (unsigned)pItem->start;
                    int         itemSz      = pItem->size;
                    int         uncompSz    = 0;

                    if (pItem->start + itemSz > flashReadOff)
                    {
                        unsigned long rdOff = pItem->start;
                        unsigned long rdOffAlign = (rdOff >> 11) << 11;//align 2k page for mtd nand, 512 for emmc
                        rc = store_logic_read(partName, rdOffAlign, itemSz + (rdOff & 0x7ff),(char*)((picLoadAddr>>11)<<11));
                        if (rc) {
                            errorP("Fail to read pic at offset 0x%x\n", pItem->start);
                            return __LINE__;
                        }
                        debugP("pic sz 0x%x\n", itemSz);
                    }

                    //uncompress supported format
                    unsigned long uncompLoadaddr = picLoadAddr + itemSz + 7;
                    uncompLoadaddr &= ~(0x7U);
                    rc = imgread_uncomp_pic((unsigned char*)picLoadAddr, itemSz, (unsigned char*)uncompLoadaddr,
                            CONFIG_MAX_PIC_LEN, (unsigned long*)&uncompSz);
                    if (rc) {
                        errorP("Fail in uncomp pic,rc[%d]\n", rc);
                        return __LINE__;
                    }
                    if (uncompSz) {
                        itemSz      = uncompSz;
                        picLoadAddr = uncompLoadaddr;
                    }

                    sprintf(env_name, "%s_offset", argv[2]);//be bootup_offset ,not bootup_720_offset
                    sprintf(env_data, "0x%lx", picLoadAddr);
                    env_set(env_name, env_data);

                    sprintf(env_name, "%s_size", argv[2]);
                    sprintf(env_data, "0x%x", itemSz);
                    env_set(env_name, env_data);

                    debugP("end read pic[%s]\n", picName);
                    return 0;//success
            }
    }

    return __LINE__;//fail
}

static cmd_tbl_t cmd_imgread_sub[] = {
    U_BOOT_CMD_MKENT(kernel, 4, 0, do_image_read_kernel, "", ""),
    U_BOOT_CMD_MKENT(dtb,    4, 0, do_image_read_dtb, "", ""),
    U_BOOT_CMD_MKENT(res,    3, 0, do_image_read_res, "", ""),
    U_BOOT_CMD_MKENT(pic,    4, 0, do_image_read_pic, "", ""),
};

static int do_image_read(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

#ifdef CONFIG_PXP_EMULATOR
	printf("\naml log : PXP image all use preload\n");
	do { (void)cmd_imgread_sub[0]; } while(0);
	return 0;
#else
	cmd_tbl_t *c;

	/* Strip off leading 'imgread' command argument */
	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_imgread_sub[0], ARRAY_SIZE(cmd_imgread_sub));

	if (c) {
		return	c->cmd(cmdtp, flag, argc, argv);
	} else {
		cmd_usage(cmdtp);
		return 1;
	}
#endif //CONFIG_PXP_EMULATOR
}

U_BOOT_CMD(
   imgread,         //command name
   5,               //maxargs
   0,               //repeatable
   do_image_read,   //command function
   "Read the image from internal flash with actual size",           //description
   "    argv: <imageType> <part_name> <loadaddr> \n"   //usage
   "    - <image_type> Current support is kernel/res(ource).\n"
   "imgread kernel  --- Read image in fomart IMAGE_FORMAT_ANDROID\n"
   "imgread dtb     --- Read dtb in fomart IMAGE_FORMAT_ANDROID\n"
   "imgread res     --- Read image packed by 'Amlogic resource packer'\n"
   "imgread picture --- Read one picture from Amlogic logo"
   "    - e.g. \n"
   "        to read boot.img     from part boot     from flash: <imgread kernel boot loadaddr> \n"   //usage
   "        to read recovery.img from part recovery from flash: <imgread kernel recovery loadaddr $offset> \n"   //usage
   "        to read logo.img     from part logo     from flash: <imgread res    logo loadaddr> \n"   //usage
   "        to read one picture named 'bootup' from logo.img    from logo: <imgread pic logo bootup loadaddr> \n"   //usage
);

//[imgread pic] logo bootup $loadaddr_misc
static int do_unpackimg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned char* loadaddr = 0;
    const AmlResImgHead_t* pResImgHead = NULL;
    unsigned itemIndex = 0;
    const AmlResItemHead_t* pItem = NULL;

    loadaddr = (unsigned char*)simple_strtoul(argc > 1 ? argv[1] : env_get("loadaddr_misc"), NULL, 16);

    pResImgHead = (AmlResImgHead_t*)loadaddr;
	const int totalSz = pResImgHead->imgSz;
	unsigned long unCompressBuf = (long)loadaddr + totalSz;

    if (img_res_check_log_header(pResImgHead)) {
        errorP("Logo header err.\n");
        return __LINE__;
    }

    pItem = (AmlResItemHead_t*)(pResImgHead + 1);
    for (itemIndex = 0; itemIndex < pResImgHead->imgItemNum; ++itemIndex, ++pItem)
    {
        if (IH_MAGIC != pItem->magic) {
            errorP("item magic 0x%x != 0x%x\n", pItem->magic, IH_MAGIC);
            return __LINE__;
        }
        char env_name[IH_NMLEN*2];
        char env_data[IH_NMLEN*2];
        unsigned long picLoadAddr = (unsigned long)loadaddr + (unsigned)pItem->start;

		int itemSz = pItem->size;
		unsigned long uncompSz = 0;

		if (unCompressBuf & 0x7U)
			unCompressBuf = ((unCompressBuf + 8) >> 3) << 3;
		imgread_uncomp_pic((unsigned char *)picLoadAddr, pItem->size,
				(unsigned char *)unCompressBuf, CONFIG_MAX_PIC_LEN, &uncompSz);
		if (uncompSz) {
			picLoadAddr = (unsigned long)unCompressBuf;
			itemSz      = uncompSz;
			unCompressBuf += uncompSz;
		}
        sprintf(env_name, "%s_offset", pItem->name);//be bootup_offset ,not bootup_720_offset
        sprintf(env_data, "0x%lx", picLoadAddr);
        env_set(env_name, env_data);

        sprintf(env_name, "%s_size", pItem->name);
		sprintf(env_data, "0x%x", itemSz);
        env_set(env_name, env_data);
    }

    return 0;//success
}

U_BOOT_CMD(
   unpackimg,           //command name
   2,                   //maxargs
   0,                   //repeatable
   do_unpackimg,   //command function
   "un pack logo image into pictures",           //description
   "    argv: unpackimg <imgLoadaddr> \n"   //usage
   "    un pack the logo image, which already loaded at <imgLoadaddr>.\n"
);

#if defined(CONFIG_CMD_AUTOSCR)
/*
 * Keep for now for backward compatibility;
 * remove later when support for "autoscr" goes away.
 */
static int
do_autoscr (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	printf ("\n### WARNING ### "
			"\"autoscr\" is deprecated, use \"source\" instead ###\n\n");
	if (argc < 2) {
		printf("too few argc %d for %s\n", argc, argv[0]);
		return CMD_RET_FAILURE;
	}
	env_set("_src_addr", argv[1]);
	return run_command("echo _src_addr ${_src_addr}; source ${_src_addr}; env delete _src_addr", 0);
}

U_BOOT_CMD_COMPLETE(
	autoscr, 2, 0,	do_autoscr,
	"DEPRECATED - use \"source\" command instead",
	"	argv: autoscr script_mem_addr",
	var_complete
);
#endif//#if defined(CONFIG_CMD_AUTOSCR)

#if defined(CONFIG_CMD_EXT4) && defined(CONFIG_MMC_MESON_GX)
/*"if ext4load mmc 1:x ${dtb_mem_addr} /recovery/dtb.img; then echo cache dtb.img loaded; fi;"\*/
static int do_load_logo_from_ext4(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (3 > argc) {
        errorP("argc(%d) < 3 illegal\n", argc);
        return CMD_RET_USAGE;
    }
    int iRet = 0;
    const char* ext4Part = argv[1];
    void* loadaddr = (void*)simple_strtoul(argv[2], NULL, 16);
	int autoSelectSlot = 1;//auto detect if need add _a/_b

    if (argc > 3) {
        env_set("ext4LogoPath", argv[3]);
    } else {
        env_set("ext4LogoPath", "/logo_files/bootup.bmp");
    }
	if (argc > 4) {
		const char *paraAutoSel = argv[4];

		autoSelectSlot = !memcmp(paraAutoSel, "true", 5);
		if (!autoSelectSlot && strcmp(paraAutoSel, "false")) {
			errorP("illegal para4 %s\n", paraAutoSel);
			return CMD_RET_FAILURE;
		}
	}

    if (!loadaddr) {
        errorP("illegal loadaddr %s\n", argv[2]);
        return CMD_RET_FAILURE;
    }

    if (BOOT_EMMC != store_get_type() ) {
        errorP("only support emmc, but store type %d\n", store_get_type() );
        return CMD_RET_FAILURE;
    }

    env_set("bootLogoPart", ext4Part);
	if (autoSelectSlot)
		run_command("if test ${active_slot} != normal; then setenv bootLogoPart ${bootLogoPart}${active_slot}; printenv bootLogoPart; fi", 0);
    const int partIndex = get_partition_num_by_name(env_get("bootLogoPart"));
    if (partIndex < 0) {
        errorP("fail find part index for name(%s)\n", env_get("bootLogoPart")); return CMD_RET_FAILURE;
    }
    env_set_hex("logoPart", partIndex);
    env_set_hex("logoLoadAddr", (ulong)loadaddr);
    env_set("ext4logoLoadCmd", "ext4load mmc 1:${logoPart} ${logoLoadAddr} ${ext4LogoPath}");
    iRet = run_command("printenv ext4logoLoadCmd; run ext4logoLoadCmd", 0);
    if (iRet) {
        errorP("Fail in load logo cmd\n"); return CMD_RET_FAILURE;
    }
    MsgP("load bmp from ext4 part okay\n");
    run_command("setenv ext4LogoSz ${filesize}", 0);
    const int bmpSz = env_get_hex("filesize", 0);
    if (bmpSz <= 0) {
        errorP("err bmp sz\n"); return CMD_RET_FAILURE;
    }

#if defined(CONFIG_GZIP)
    if (memcmp(loadaddr, gzip_magic, sizeof(gzip_magic))) {
        return CMD_RET_SUCCESS;
    }
    MsgP("gunzip bmp logo\n");
    void* uncompress = (char*)loadaddr + (((bmpSz + 0xf)>>4)<<4);
    unsigned long uncompSz = 0;
    iRet = imgread_uncomp_pic((unsigned char*)loadaddr, bmpSz, (unsigned char*)uncompress,
            CONFIG_MAX_PIC_LEN, (unsigned long*)&uncompSz);
    if (iRet) {
        errorP("Fail in uncomp pic,rc[%d]\n", iRet); return __LINE__;
    }
    if (uncompSz <= 0) {
        errorP("Fail uncompress logo bmp\n"); return CMD_RET_FAILURE;
    }
    memmove(loadaddr, uncompress, uncompSz);
    env_set_hex("ext4LogoSz", uncompSz);
#endif//#if defined(CONFIG_GZIP)

    return CMD_RET_SUCCESS;
}

U_BOOT_CMD_COMPLETE(
   rdext4pic,                   //read ext4 picture
	5,                           //maxargs
   0,                           //repeatable
   do_load_logo_from_ext4,      //command function
   "read logo bmp from ext4 part",           //description
   "    argv: rdext4pic <partName> <memAddr> <logoPath>\n"   //usage
   "    load bmp picture from <logoPath> of <partName> to <memAddr>.\n",
   var_complete
);
#endif// #if defined(CONFIG_CMD_EXT4) && defined(CONFIG_MMC_MESON_GX)

