#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <common.h>
#include <command.h>
#include <console.h>
#include <watchdog.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <jffs2/jffs2.h>

enum boot_type_e {
	BOOT_EMMC = 1,
	BOOT_SD = 2,
	BOOT_MLC = 4,
	BOOT_SLC = 8,
	BOOT_SNAND = 16,/* spi nand */
	BOOT_SNOR = 32,/* spi nor */
	BOOT_NONE = 0xFF
};

#define RSV_KEY	"key"
#define RSV_ENV	"env"
#define RSV_DTB "dtb"
#define RSV_BBT "bbt"

#define DISCRETE_BOOTLOADER 1
#define COMPACT_BOOTLOADER 0

struct storage_info_t {
	uint8_t name[32];
	uint8_t id[8];
	uint32_t read_unit;
	uint32_t write_unit;
	uint32_t erase_unit;
	uint64_t caps;/* total size */
	uint8_t mode;/* bootloader mode, compact or  discrete */
};


struct storage_t {
	enum boot_type_e type;
	struct storage_info_t info;
	uint32_t init_flag;
	struct list_head list;
	/* when part_name is null, default to ops in whole chip */
	uint64_t (*get_part_size)(const char *part_name);
	int (*read)(const char *part_name, loff_t off, size_t size, void *dest);
	int (*write)(const char *part_name, loff_t off, size_t size, void *source);
	int (*erase)(const char *part_name, loff_t off, size_t size, int scrub_flag);

#define BOOT_OPS_ALL 0xff;/* for cpy parameter operats all copies */
	uint8_t (*get_copies)(const char *part_name);
	uint64_t (*get_copy_size)(const char *part_name);
	int (*boot_read)(const char *part_name, uint8_t cpy, size_t size, void *dest);
	int (*boot_write)(const char *part_name, uint8_t cpy, size_t size, void *source);
	int (*boot_erase)(const char *part_name, uint8_t cpy);

	uint32_t (*get_rsv_size)(const char *rsv_name);
	int (*read_rsv)(const char *rsv_name, size_t size, void *buf);
	/*write bbt is prohibited*/
	int (*write_rsv)(const char *rsv_name, size_t size, void *buf);
	int (*erase_rsv)(const char *rsv_name);
	int (*protect_rsv)(const char *rsv_name, bool ops);/*true:on false:off*/
};

/**
 * we use a valid device list to manage the storage devices,
 * and every type of device can only exist ONE in this list.
 * we will scan and add the valid storage device to the list
 * in the init process, and of cause you can register the device
 * by you own.
 */
#define NAND_BOOT_NORMAL				0
#define NAND_BOOT_UPGRATE				1
#define NAND_BOOT_ERASE_PROTECT_CACHE	2
#define NAND_BOOT_ERASE_ALL   			3
#define NAND_BOOT_SCRUB_ALL				4
#define NAND_SCAN_ID_INIT				5
/**
 * @usage: init all the valid storage device
 *
 * @init_flag: it's only works for MLC driver
 * 0 NAND_BOOT_NORMAL:normal init, but can't operats the phy data area
 * 1 NAND_BOOT_UPGRATE:same as 0, but operation on phy data area is allowed
 * 2 NAND_BOOT_ERASE_PROTECT_CACHE:only erase rsv area
 * 3 NAND_BOOT_ERASE_ALL:erase whole device
 * 4 NAND_BOOT_SCRUB_ALL:erase whole device
 * 5 NAND_SCAN_ID_INIT:only read nand id
 *
 * @return: init result
 * 		    0 = success
 * 		    other = fail
 */
int store_init(uint32_t init_flag);

/**
 * @usage: register a storage device to the valid list
 *
 * @store_dev: the description pointer of your storage device
 */
void store_register(struct storage_t *store_dev);

/**
 * @usage: unregister a storage device from the valid list
 *
 * @store_dev: the description pointer of your storage device
 */
void store_unregister(struct storage_t *store_dev);

/**
 * @usage: check the type of device is valid on this board
 *
 * @type: the device type that you want to check
 *
 * @return: is the device valid
 * 			0 = invalid
 * 			1 = valid
 */
uint8_t store_device_valid(enum boot_type_e type);

/**
 * @usage: set the 'type' device as current device, and you can operats it
 *
 * @type: the device type that you want to set
 *
 * @return: result of the operation
 * 			0 = success
 * 			other = fail
 */
int store_set_device(enum boot_type_e type);

/**
 * @usage: get the type of current storage device
 *
 * @return: storage device type
 */
enum boot_type_e store_get_type(void);

/**
 * @usage: get information about the current device
 *
 * @info: the pointer for the information
 *
 * @return: result of the operation
 * 			0 = success
 * 			other = fail
 */
int store_get_device_info(struct storage_info_t *info);

/**
 * @usage: read data from storage device
 *
 * @name: partition name, when it's null the target
 * 		  will regards as whole device.
 * @off: offset to the 0 address of partition/device
 * @size: the amount of bytes to read
 * @buf: pointer of target buffer
 *
 * @return: result of the operation
 * 			0 = success
 * 			other = fail
 */
int store_read(const char *name, loff_t off, size_t size, void *buf);

/**
 * @usage: write data to storage device
 *
 * @name: partition name, when it's null the target
 * 		  will regards as whole device.
 * @off: offset to the 0 address of partition/device
 * @size: the amount of bytes to write
 * @buf: pointer of source buffer
 *
 * @return: result of the operation
 * 			0 = success
 * 			other = fail
 */
int store_write(const char *name, loff_t off, size_t size, void *buf);

/**
 * @usage: erase the storage device
 *
 * @name: partition name, when it's null the target
 * 		  will regards as whole device.
 * @off: offset to the 0 address of partition/device
 * @size: the amount of bytes to erase
 * @scrub: scrub flag(scrub operats will works only when the device support)
 * 			0 = no scrub, just erase
 * 			1 = use scrub operats instead of erase
 * @return: result of the operation
 * 			0 = success
 * 			other = fail
 */
int store_erase(const char *name, loff_t off, size_t size, int scrub);

/**
 * @usage: get the partition size or capacity of device
 *
 * @name: partition name, when it's null the target
 * 		  will regards as whole device.
 *
 * @return: the amount of bytes to the partition or device size
 */
uint64_t store_part_size(const char *name);

/**
 * @usage: get the copy number of bootloader/tpl(fip)
 *
 * @name: only can be "bootloader" or "tpl"
 * @return: the copy number of the "bootloader" or "tpl"
 */
uint8_t store_boot_copy_num(const char *name);

/**
 * @usage: get the copy size of bootloader/tpl(fip)
 *
 * @name: only can be "bootloader" or "tpl"
 *
 * @return: the size of every copy
 */
uint64_t store_boot_copy_size(const char *name);

/**
 * @usage: read the bootloader/tpl(fip) data from storage device
 *
 * @name: only can be "bootloader" or "tpl"
 * @copy: which copy you want read
 * @size: the amount of bytes to read
 * @buf: pointer of the target buffer
 *
 * @return: result of the operation
 * 			0 = success
 * 			other = fail
 */
int store_boot_read(const char *name, uint8_t copy, size_t size, void *buf);

/**
 * @usage: write the bootloader/tpl(fip) data into storage device
 *
 * @name: only can be "bootloader" or "tpl"
 * @copy: which copy you want write,
 * 		  it will write to all copies when copy = BOOT_OPS_ALL
 * @size: the amount of bytes to write
 * @buf: pointer of the source buffer
 *
 * @return: result of the operation
 * 			0 = success
 * 			other = fail
 */
int store_boot_write(const char *name, uint8_t copy, size_t size, void *buf);

/**
 * @usage: erase the bootloader/tpl(fip) data
 *
 * @name: only can be "bootloader" or "tpl"
 * @copy: which copy you want erase,
 * 		  it will erase all copies when copy = BOOT_OPS_ALL
 *
 * @return: result of the operation
 * 			0 = success
 * 			other = fail
 */
int store_boot_erase(const char *name, uint8_t copy);

/**
 * @usage: get the rsv info size
 *
 * @name: rsv info name, please refer to
 * 		  RSV_KEY	"key"
 * 		  RSV_ENV	"env"
 * 		  RSV_DTB	"dtb"
 * 		  RSV_BBT	"bbt"
 *
 * @return: the amount bytes of the rsv info
 */
uint32_t store_rsv_size(const char *name);

/**
 * @usage: read the rsv info from storage device
 *
 * @name: rsv info name, please refer to
 * 		  RSV_KEY	"key"
 * 		  RSV_ENV	"env"
 * 		  RSV_DTB	"dtb"
 * 		  RSV_BBT	"bbt"
 * @size: the amount of bytes to read
 * @buf: pointer of the target buffer
 *
 * @return: result of the operation
 * 			0 = success
 * 			other = fail
 */
int store_rsv_read(const char *name, size_t size, void *buf);

/**
 * @usage: write the rsv info to the storage device
 *
 * @name: rsv info name, please refer to
 * 		  RSV_KEY	"key"
 * 		  RSV_ENV	"env"
 * 		  RSV_DTB	"dtb"
 * 		  RSV_BBT	"bbt"
 * @size: the amount of bytes to write
 * @buf: pointer of the source buffer
 *
 * @return: result of the operation
 * 			0 = success
 * 			other = fail
 */
int store_rsv_write(const char *name, size_t size, void *buf);

/**
 * @usage: erase the rsv info
 *
 * @name: rsv info name, please refer to
 * 		  RSV_KEY	"key"
 * 		  RSV_ENV	"env"
 * 		  RSV_DTB	"dtb"
 * 		  RSV_BBT	"bbt"
 *
 * @return: result of the operation
 * 			0 = success
 * 			other = fail
 */
int store_rsv_erase(const char *name);

/**
 * @usage: turn on/off the protection of rsv info
 *
 * @name: rsv info name, please refer to
 * 		  RSV_KEY	"key"
 * 		  RSV_ENV	"env"
 * 		  RSV_DTB	"dtb"
 * 		  RSV_BBT	"bbt"
 * @ops: turn on/off the rsv info protection
 * 		 true = turn on the protection
 * 		 flase = turn off the protection
 *
 * @return: result of the operation
 * 			0 = success
 * 			other = fail
 */
int store_rsv_protect(const char *name, bool ops);
#endif/* __STORAGE_H__ */
