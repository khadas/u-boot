/*
 * This is from the Android Project,
 * Repository: https://android.googlesource.com/platform/bootable/bootloader/legacy
 * File: include/boot/bootimg.h
 * Commit: 4205b865141ff2e255fe1d3bd16de18e217ef06a
 *
 * Copyright (C) 2008 The Android Open Source Project
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _ANDROID_IMAGE_H_
#define _ANDROID_IMAGE_H_

#define _BA1_(cond, line) \
	extern int __build_assertion_ ## line[1 - 2*!(cond)] \
	__attribute__ ((unused))
#define _BA0_(c, x) _BA1_(c, x)
#define BUILD_ASSERT(cond) _BA0_(cond, __LINE__)

#define BOOT_IMG_HDR_SIZE         (0x800)
#define BOOT_IMG_V3_HDR_SIZE      (0x1000)
#define VENDOR_BOOT_IMG_HDR_SIZE  (0x1000)

#define ANDR_BOOT_MAGIC            "ANDROID!"
#define ANDR_BOOT_MAGIC_SIZE      (8)
#define ANDR_BOOT_NAME_SIZE       (16)
#define ANDR_BOOT_ARGS_SIZE       (512)

#define BOOT_MAGIC                "ANDROID!"
#define BOOT_MAGIC_SIZE           (8)
#define BOOT_NAME_SIZE            (16)
#define BOOT_ARGS_SIZE            (512)
#define BOOT_EXTRA_ARGS_SIZE      (1024)

#define VENDOR_BOOT_MAGIC         "VNDRBOOT"
#define VENDOR_BOOT_MAGIC_SIZE    (8)
#define VENDOR_BOOT_ARGS_SIZE     (2048)
#define VENDOR_BOOT_NAME_SIZE     (16)

#define VENDOR_RAMDISK_TYPE_NONE	0
#define VENDOR_RAMDISK_TYPE_PLATFORM	1
#define VENDOR_RAMDISK_TYPE_RECOVERY	2
#define VENDOR_RAMDISK_TYPE_DLKM	3
#define VENDOR_RAMDISK_NAME_SIZE	32
#define VENDOR_RAMDISK_TABLE_ENTRY_BOARD_ID_SIZE	16

/* Before android R boot.img header structure */
typedef struct {
	char magic[BOOT_MAGIC_SIZE]; /*"ANDROID!"*/

	u32 kernel_size;	/* size in bytes */
	u32 kernel_addr;	/* physical load addr */

	u32 ramdisk_size;	/* size in bytes */
	u32 ramdisk_addr;	/* physical load addr */

	u32 second_size;	/* size in bytes */
	u32 second_addr;	/* physical load addr */

	u32 tags_addr;		/* physical addr for kernel tags */
	u32 page_size;		/* flash page size we assume */

	u32 header_version;	/* highest byte: 1 = boot, 2 = recovery;
				   low three bytes: kernel version */

	/* operating system version and security patch level; for
	   * version "A.B.C" and patch level "Y-M-D":
	   * ver = A << 14 | B << 7 | C         (7 bits for each of A, B, C)
	   * lvl = ((Y - 2000) & 127) << 4 | M  (7 bits for Y, 4 bits for M)
	   * os_version = ver << 11 | lvl */
	uint32_t os_version;

	char     name[BOOT_NAME_SIZE]; /* asciiz product name */

	char     cmdline[BOOT_ARGS_SIZE];

	u32      id[8]; /* timestamp / checksum / sha1 / etc */

	/* Supplemental command line data; kept here to maintain
	     binary compatibility with older versions of mkbootimg.
	  */
	uint8_t       extra_cmdline[BOOT_EXTRA_ARGS_SIZE];
	unsigned char szReserved[BOOT_IMG_HDR_SIZE - 1632];       /*align to 2KB header,1632 is size before this*/
}boot_img_hdr_t, *p_boot_img_hdr_t;

/*compile check*/
BUILD_ASSERT(sizeof(boot_img_hdr_t) == BOOT_IMG_HDR_SIZE);


typedef struct {
	boot_img_hdr_t hdr;
	unsigned char  szData[1];
}boot_img_t, * p_boot_img_t;


/* When the boot image header has a version of 3, the structure of the boot
 * image is as follows:
 *
 * +-----------------+
 * | boot header     | 1 page
 * +-----------------+
 * | kernel          | n pages
 * +-----------------+
 * | ramdisk         | m pages
 * +-----------------+
 * | second stage    | o pages
 * +-----------------+
 *
 * n = (kernel_size + page_size - 1) / page_size
 * m = (ramdisk_size + page_size - 1) / page_size
 * o = (second_size + page_size - 1) / page_size
 *
 * 0. all entities are page_size aligned in flash
 * 1. kernel and ramdisk are required (size != 0)
 * 2. second is optional (second_size == 0 -> no second)
 * 3. load each element (kernel, ramdisk, second) at
 *    the specified physical address (kernel_addr, etc)
 * 4. prepare tags at tag_addr.  kernel_args[] is
 *    appended to the kernel commandline in the tags.
 * 5. r0 = 0, r1 = MACHINE_TYPE, r2 = tags_addr
 * 6. if second_size != 0: jump to second_addr
 *    else: jump to kernel_addr
 */


#define ANDROID_R_IMG_VER  (3)
#define ANDROID_S_IMG_VER  (4)

/* Android R boot.img and vendor_boot.img structure */
typedef struct {
	char 	magic[ANDR_BOOT_MAGIC_SIZE]; /*"ANDROID!"*/

	u32 	kernel_size;	/* size in bytes */
	u32 	ramdisk_size;	/* size in bytes */

    /* Operating system version and security patch level.
       For version "A.B.C" and patch level "Y-M-D":
       (7 bits for each of A, B, C; 7 bits for (Y-2000), 4 bits for M)
       os_version = A[31:25] B[24:18] C[17:11] (Y-2000)[10:4] M[3:0]
       */

	uint32_t os_version;
	uint32_t header_size;
	uint32_t reserved[4];

	uint32_t header_version;   /* Version of the boot image header */
	char     cmdline[BOOT_ARGS_SIZE + BOOT_EXTRA_ARGS_SIZE];
	unsigned char szReserved[BOOT_IMG_V3_HDR_SIZE - 1580];       /*align to 4KB header,1580 is size before this*/
}boot_img_hdr_v3_t, * p_boot_img_hdr_v3_t;


/*compile check*/
BUILD_ASSERT(sizeof(boot_img_hdr_v3_t) == BOOT_IMG_V3_HDR_SIZE);

typedef struct {
	boot_img_hdr_v3_t hdr;
	unsigned char     szData[1];
}boot_img_v3_t, * p_boot_img_v3_t;


/* When the boot image header has a version of 3, the structure of the boot
 * image is as follows:
 *
 * +---------------------+
 * | boot header         | 4096 bytes
 * +---------------------+
 * | kernel              | m pages
 * +---------------------+
 * | ramdisk             | n pages
 * +---------------------+
 *
 * m = (kernel_size + 4096 - 1) / 4096
 * n = (ramdisk_size + 4096 - 1) / 4096
 *
 * Note that in version 3 of the boot image header, page size is fixed at 4096 bytes.
 *
 * The structure of the vendor boot image (introduced with version 3 and
 * required to be present when a v3 boot image is used) is as follows:
 *
 * +---------------------+
 * | vendor boot header  | o pages
 * +---------------------+
 * | vendor ramdisk      | p pages
 * +---------------------+
 * | dtb                 | q pages
 * +---------------------+
 * o = (2112 + page_size - 1) / page_size
 * p = (vendor_ramdisk_size + page_size - 1) / page_size
 * q = (dtb_size + page_size - 1) / page_size
 *
 * 0. all entities in the boot image are 4096-byte aligned in flash, all
 *    entities in the vendor boot image are page_size (determined by the vendor
 *    and specified in the vendor boot image header) aligned in flash
 * 1. kernel, ramdisk, vendor ramdisk, and DTB are required (size != 0)
 * 2. load the kernel and DTB at the specified physical address (kernel_addr,
 *    dtb_addr) * 3. load the vendor ramdisk at ramdisk_addr
 * 4. load the generic ramdisk immediately following the vendor ramdisk in
 *    memory
 * 5. set up registers for kernel entry as required by your architecture
 * 6. if the platform has a second stage bootloader jump to it (must be
 *    contained outside boot and vendor boot partitions), otherwise
 *    jump to kernel_addr
 */


struct vendor_boot_img_hdr {
	char     magic[VENDOR_BOOT_MAGIC_SIZE];  /*"VNDRBOOT"*/

	uint32_t header_version;   				 /*Version of the vendor boot image header*/
	uint32_t page_size;		 				 /* flash page size we assume */

	uint32_t kernel_addr; 					 /* physical load addr */
	uint32_t ramdisk_addr; 					 /* physical load addr */

	uint32_t vendor_ramdisk_size; 			 /* size in bytes */

	char     cmdline[VENDOR_BOOT_ARGS_SIZE]; /*2048B*/

	uint32_t tags_addr; 					 /* physical addr for kernel tags (if required) */
	uint8_t  name[VENDOR_BOOT_NAME_SIZE]; 	 /* 16B asciiz product name */

	uint32_t header_size;

	uint32_t dtb_size; 						 /* size in bytes for DTB image */
	uint64_t dtb_addr;						 /* physical load address for DTB image */
	/* new for v4 */
	u32 vendor_ramdisk_table_size;/* size in bytes for the vendor ramdisk table */
	u32 vendor_ramdisk_table_entry_num;/* number of entries in the vendor ramdisk table */
	u32 vendor_ramdisk_table_entry_size;
	u32 vendor_bootconfig_size;/* size in bytes for bootconfig image */
	unsigned char szReserved[VENDOR_BOOT_IMG_HDR_SIZE - 2128];
};

typedef struct vendor_boot_img_hdr vendor_boot_img_hdr_t;
typedef struct vendor_boot_img_hdr *p_vendor_boot_img_hdr_t;

struct vendor_ramdisk_table_entry_v4 {
	u32 ramdisk_size;	/* size in bytes for the ramdisk image */
	u32 ramdisk_offset;	/* offset to the ramdisk image in vendor ramdisk section */
	u32 ramdisk_type;	/* type of the ramdisk */
	u8 ramdisk_name[VENDOR_RAMDISK_NAME_SIZE];	/* asciiz ramdisk name */
	// Hardware identifiers describing the board, soc or platform which this
	// ramdisk is intended to be loaded on.
	u32 board_id[VENDOR_RAMDISK_TABLE_ENTRY_BOARD_ID_SIZE];
};

typedef struct vendor_ramdisk_table_entry_v4 vendor_ramdisk_table_entry_v4_t;
typedef struct vendor_ramdisk_table_entry_v4 *p_vendor_ramdisk_table_entry_v4_t;

/*compile check*/
BUILD_ASSERT((sizeof(vendor_boot_img_hdr_t) == VENDOR_BOOT_IMG_HDR_SIZE));

typedef struct {
	vendor_boot_img_hdr_t hdr;
	unsigned char         szData[1];
}vendor_boot_img_t, * p_vendor_boot_img_t;


#endif
