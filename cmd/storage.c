#include "storage.h"
#include "../drivers/nand/include/phynand.h"

extern void store_register(struct storage_t *store_dev);
extern int storage_dev_is_mlc(u8 flag);

int emmc_pre(void)
{
	return 1;
}

int amlnf_pre(void)
{
	char ret = 0xff;

	ret = storage_dev_is_mlc(0);

	return ret;
}

int nand_pre(void)
{
	return 1;
}
int spi_nand_pre(void)
{
	return 1;
}
int spi_nor_pre(void)
{
	return 1;
}
int sdcard_pre(void)
{
	return 1;
}

int emmc_probe(uint32_t init_flag)
{
	return 1;
}

#if 1
extern int amlnf_init(unsigned char flag);
extern struct amlnand_chip *aml_nand_chip;

extern uint32_t amlnf_get_rsv_size(const char *name);
extern int amlnf_read_rsv(const char *name, size_t size, void *buf);
extern int amlnf_write_rsv(const char *name, size_t size, void *buf);
extern int amlnf_erase_rsv(const char *name);
extern u8 amlnf_boot_cpys(const char *part_name);
extern u64 amlnf_boot_copy_size(const char *part_name);
extern u64 amlnf_get_size(const char *part_name);
extern int amlnf_read(const char *part_name, loff_t off, size_t size,void *dest);
extern int amlnf_write(const char *part_name, loff_t off, size_t size, void *source);
extern int amlnf_erase(const char *part_name, loff_t off, size_t size, int scrub_flag);
extern int amlnf_boot_read(const char *part_name, uint8_t copy, size_t size, void *buf);
extern int amlnf_boot_write(const char *part_name, uint8_t copy, size_t size, void *buf);
extern int amlnf_boot_erase(const char *part_name, uint8_t copy);
extern int amlnf_rsv_protect(const char *name, bool ops);
#endif

int amlnf_probe(uint32_t init_flag)
{
	char ret = 0xff;
	static struct storage_t *storage_dev = NULL;
	struct amlnand_chip *aml_chip = NULL;
	/*struct store_operation *storage_opera = NULL;*/

	storage_dev = kzalloc(sizeof(struct storage_t), GFP_KERNEL);
	if (storage_dev == NULL) {
		printf("malloc failed for storage_dev\n");
		ret = -1;
		goto exit_error;
	}

	ret = amlnf_init(init_flag); /*flag 0*/
	if (ret) {
		printf("amlnf init failed ret:%x\n", ret);
		goto exit_error;
	}
	/******basic info*******/
	storage_dev->init_flag = init_flag;
	storage_dev->type = BOOT_MLC;

	printf("store flag: %d, types: %d\n",storage_dev->init_flag,storage_dev->type);
	if (aml_nand_chip == NULL) {
		printf("error aml_nand_chip is NULL\n");
		ret = -1;
		goto exit_error;
	}
	aml_chip = aml_nand_chip;
	/*storage_opera = &(aml_chip->storage_opera);*/
	memcpy((char *)(storage_dev->info.name), (char *)(aml_chip->flash.name),
		32*sizeof(char));

	memcpy((char *)(storage_dev->info.id), (char *)(aml_chip->flash.id),
		8*sizeof(char));
	storage_dev->info.read_unit = aml_chip->flash.pagesize;
	storage_dev->info.write_unit = aml_chip->flash.pagesize;
	storage_dev->info.erase_unit = aml_chip->flash.blocksize;
	storage_dev->info.caps = aml_chip->flash.chipsize;
	storage_dev->info.mode = COMPACT_BOOTLOADER;
	printf("page size: 0x%x\n",storage_dev->info.read_unit);

#if 0

#else
	storage_dev->get_part_size = amlnf_get_size;
	storage_dev->read = amlnf_read;
	storage_dev->write = amlnf_write;
	storage_dev->erase = amlnf_erase;

	storage_dev->get_copies = amlnf_boot_cpys;
	storage_dev->get_copy_size = amlnf_boot_copy_size;
	storage_dev->boot_read = amlnf_boot_read;
	storage_dev->boot_write = amlnf_boot_write;
	storage_dev->boot_erase = amlnf_boot_erase;

	storage_dev->get_rsv_size = amlnf_get_rsv_size;
	storage_dev->read_rsv = amlnf_read_rsv;
	storage_dev->write_rsv = amlnf_write_rsv;
	storage_dev->erase_rsv = amlnf_erase_rsv;
	storage_dev->protect_rsv = amlnf_rsv_protect;
#endif
	if (storage_dev->init_flag < NAND_BOOT_ERASE_PROTECT_CACHE)
		store_register(storage_dev);
	printf("amlnf probe success\n");
exit_error:
	return ret;
	return 0;
}
int nand_probe(uint32_t init_flag)
{
	return 0;
}
int spi_nand_probe(uint32_t init_flag)
{
	return 0;
}
int spi_nor_probe(uint32_t init_flag)
{
	return 0;
}
int sdcard_probe(uint32_t init_flag)
{
	return 0;
}

static struct device_node_t {
	enum boot_type_e index;
	int (*probe)(uint32_t init_flag);
};

static struct device_node_t device_list[] = {
	{BOOT_EMMC, emmc_probe},
	{BOOT_SD, sdcard_probe},
	{BOOT_MLC, amlnf_probe},
	{BOOT_SLC, nand_probe},
	{BOOT_SNAND, spi_nand_probe},
	{BOOT_SNOR, spi_nor_probe}
};

static struct storage_t *store_current;

void store_register(struct storage_t *store_dev)
{
	if (!store_current) {
		INIT_LIST_HEAD(&store_dev->list);
		store_current = store_dev;
	} else {
		list_add_tail(&store_dev->list, &store_current->list);
		if (store_dev->type != BOOT_SD)
			store_current = store_dev;
	}
}

void store_unregister(struct storage_t *store_dev)
{
	if (store_dev == store_current) {
		if (list_empty_careful(&store_dev->list))
			store_current = NULL;
		else {
			store_current = list_entry((store_current->list).next, struct storage_t, list);
			list_del_init(&store_dev->list);
		}
	} else {
		list_del_init(&store_dev->list);
	}
}

uint8_t store_device_valid(enum boot_type_e type)
{
	struct list_head *entry;
	struct storage_t *dev;

	if (!store_current)
		return 0;
	if (store_current->type == type)
		return 1;
	list_for_each(entry, &store_current->list) {
		dev = list_entry(entry, struct storage_t, list);
		if (dev->type == type)
			return 1;
	}
	return 0;
}

static uint32_t store_scan(void)
{
	uint32_t ret = 0;
	if (!emmc_pre())
		ret |= BOOT_EMMC;
	if (!sdcard_pre())
		ret |= BOOT_SD;
	if (!amlnf_pre())
		ret |= BOOT_MLC;
	if (!nand_pre())
		ret |= BOOT_SLC;
	if (!spi_nand_pre())
		ret |= BOOT_SNAND;
	if (!spi_nor_pre())
		ret |= BOOT_SNOR;
	return ret;
}

int store_init(uint32_t init_flag)
{
	uint32_t dev_list = 0;
	int ret = 1, i;
	dev_list = store_scan();
	if (!dev_list)
		return ret;
	printf("device_list cnt: %d, %ld\n", dev_list, ARRAY_SIZE(device_list));
	for (i = 0; i < ARRAY_SIZE(device_list); i++) {
		if (dev_list & device_list[i].index) {
			if (!store_device_valid(device_list[i].index)) {
				printf("store no probe,first probe\n");
				ret = device_list[i].probe(init_flag);
				if (ret)
					pr_info("%s %d %d device probe fail\n",
							__func__, __LINE__, device_list[i].index);
			}
		}
	}

	return ret;
}

static struct storage_t *store_get_current(void)
{
	return store_current;
}

int store_set_device(enum boot_type_e type)
{
	struct list_head *entry;
	struct storage_t *dev, *store_dev = store_get_current();

	if (!store_dev) {
		pr_info("%s %d no current device\n", __func__, __LINE__);
		return 1;
	}
	if (store_dev->type == type)
		return 0;
	list_for_each(entry, &store_dev->list) {
		dev = list_entry(entry, struct storage_t, list);
		if (dev->type == type) {
			store_current = dev;
			return 0;
		}
	}
	pr_info("%s %d please confirm the %d device is valid\n",
			__func__, __LINE__, type);
	return 0;
}

enum boot_type_e store_get_type(void)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return BOOT_NONE;
	}

	return store->type;
}

int store_get_device_info(struct storage_info_t *info)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}

	memcpy((char *)info, (char *)&(store->info), sizeof(struct storage_info_t));
	return 0;
}

int store_read(const char *name, loff_t off, size_t size, void *buf)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->read(name, off, size, buf);
}

int store_write(const char *name, loff_t off, size_t size, void *buf)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->write(name, off, size, buf);
}

int store_erase(const char *name, loff_t off, size_t size, int scrub)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->erase(name, off, size, scrub);
}

uint64_t store_part_size(const char *name)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->get_part_size(name);
}

uint8_t store_boot_copy_num(const char *name)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->get_copies(name);
}

uint64_t store_boot_copy_size(const char *name)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->get_copy_size(name);
}

int store_boot_read(const char *name, uint8_t copy, size_t size, void *buf)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->boot_read(name, copy, size, buf);
}

int store_boot_write(const char *name, uint8_t copy, size_t size, void *buf)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->boot_write(name, copy, size, buf);
}

int store_boot_erase(const char *name, uint8_t copy)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->boot_erase(name, copy);
}

uint32_t store_rsv_size(const char *name)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->get_rsv_size(name);
}

int store_rsv_read(const char *name, size_t size, void *buf)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->read_rsv(name, size, buf);
}

int store_rsv_write(const char *name, size_t size, void *buf)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->write_rsv(name, size, buf);
}

int store_rsv_erase(const char *name)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->erase_rsv(name);
}

int store_rsv_protect(const char *name, bool ops)
{
	struct storage_t *store = store_get_current();
	if (!store) {
		pr_info("%s %d please init storage device first\n",
				__func__, __LINE__);
		return 1;
	}
	return store->protect_rsv(name, ops);
}

static int do_store_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint32_t init_flag = 1;
	if (unlikely((argc != 2) && (argc != 3)))
		return CMD_RET_USAGE;

	if (argc == 3)
		init_flag = simple_strtoul(argv[2], NULL, 10);
	return store_init(init_flag);
}

static int do_store_scrub(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long offset;
	size_t size;
	char *name = NULL;

	if (!store) {
		printf("%s %d please init your storage device first!\n",
				__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely((argc != 4) && (argc != 5)))
		return CMD_RET_USAGE;

	size = (size_t)simple_strtoul(argv[argc - 1], NULL, 16);
	offset = simple_strtoul(argv[argc - 2], NULL, 16);
	if (argc == 5)
		name = argv[2];

	printf ("%s, name %s,size: %ld",__func__, name,size);
	return store->erase(name, offset, size, 1);
}

static int do_store_erase(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long offset;
	size_t size;
	char *name = NULL;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
				__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	printf("mode :%d",store->info.mode);
	if (unlikely((argc != 4) && (argc != 5)))
		return CMD_RET_USAGE;

	size = (size_t)simple_strtoul(argv[argc - 1], NULL, 16);
	offset = simple_strtoul(argv[argc - 2], NULL, 16);
	if (argc == 5)
		name = argv[2];

	return store->erase(name, offset, size, 0);
}

static int do_store_read(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long offset, addr;
	size_t size;
	char *name = NULL;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
				__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely((argc != 5) && (argc != 6)))
		return CMD_RET_USAGE;

	addr = simple_strtoul(argv[2], NULL, 16);
	size = (size_t)simple_strtoul(argv[argc - 1], NULL, 16);
	offset = simple_strtoul(argv[argc - 2], NULL, 16);
	if (argc == 6)
		name = argv[3];
	return store->read(name, offset, size, (u_char *)addr);
}

static int do_store_write(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long offset, addr;
	size_t size;
	char *name = NULL;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
				__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely((argc != 5) && (argc != 6)))
		return CMD_RET_USAGE;

	addr = simple_strtoul(argv[2], NULL, 16);
	offset = simple_strtoul(argv[argc - 2], NULL, 16);
	size = (size_t)simple_strtoul(argv[argc - 1], NULL, 16);
	if (argc == 6)
		name = argv[3];

	return store->write(name, offset, size, (u_char *)addr);
}

static int do_store_boot_read(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long addr;
	size_t size;
	uint8_t cpy;
	char *name;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
				__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely(argc != 6))
		return CMD_RET_USAGE;

	name = argv[2];
	addr = (unsigned long)simple_strtoul(argv[3], NULL, 16);
	cpy = (uint8_t)simple_strtoul(argv[4], NULL, 16);
	size = (size_t)simple_strtoul(argv[5], NULL, 16);

	return store->boot_read(name, cpy, size, (u_char *)addr);
}

static int do_store_boot_write(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long addr;
	size_t size;
	uint8_t cpy = BOOT_OPS_ALL;
	char *name;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
				__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely((argc != 5) && (argc != 6)))
		return CMD_RET_USAGE;

	name = argv[2];
	addr = (unsigned long)simple_strtoul(argv[3], NULL, 16);
	size = (size_t)simple_strtoul(argv[argc - 1], NULL, 16);
	if (argc == 6)
		cpy = (uint8_t)simple_strtoul(argv[4], NULL, 16);

	return store->boot_write(name, cpy, size, (u_char *)addr);
}

static int do_store_boot_erase(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	uint8_t cpy = BOOT_OPS_ALL;
	char *name;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
				__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely((argc != 3) && (argc != 4)))
		return CMD_RET_USAGE;

	name = argv[2];
	if (argc == 4)
		cpy = (uint8_t)simple_strtoul(argv[3], NULL, 16);

	return store->boot_erase(name, cpy);
}

static int do_store_rsv_ops(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	char *name = NULL;

	if (!store) {
		printf("%s %d please init your storage device first!\n",
				__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (!strcmp(argv[2], "erase")) {
		if (argc == 3)
			;
		else if (argc == 4)
			name = argv[3];
		else
			return CMD_RET_USAGE;
		return store->erase_rsv(name);
	} else if (!strcmp(argv[2], "read") ||
			   !strcmp(argv[2], "write")) {
		uint8_t cmd = strcmp(argv[2], "read") ? 0 : 1;
		unsigned long addr = simple_strtoul(argv[4], NULL, 16);
		size_t size = (size_t)simple_strtoul(argv[5], NULL, 16);
		name = argv[3];

		if (unlikely(argc != 6))
			return CMD_RET_USAGE;
		if (cmd)
			return store->read_rsv(name, size, (u_char *)addr);
		else
			return store->write_rsv(name, size, (u_char *)addr);
	} else if (!strcmp(argv[2], "protect")) {
		bool flag = false;
		char *ops;

		if (unlikely((argc != 4) && (argc != 5)))
			return CMD_RET_USAGE;

		name = (argc == 4) ? NULL : argv[3];
		ops = argv[argc - 1];
		if (!strcmp(ops, "on"))
			flag = true;
		else if (!strcmp(ops, "off"))
			flag = false;
		return store->protect_rsv(name, flag);
	}
	return CMD_RET_USAGE;
}

static cmd_tbl_t cmd_store_sub[] = {
	U_BOOT_CMD_MKENT(init,			4, 0, do_store_init, "", ""),
	U_BOOT_CMD_MKENT(scrub,			5, 0, do_store_scrub, "", ""),
	U_BOOT_CMD_MKENT(erase,			5, 0, do_store_erase, "", ""),
	U_BOOT_CMD_MKENT(read,			6, 0, do_store_read, "", ""),
	U_BOOT_CMD_MKENT(write,			7, 0, do_store_write, "", ""),
	U_BOOT_CMD_MKENT(boot_read,		6, 0, do_store_boot_read, "", ""),
	U_BOOT_CMD_MKENT(boot_write,	6, 0, do_store_boot_write, "", ""),
	U_BOOT_CMD_MKENT(boot_erase,	4, 0, do_store_boot_erase, "", ""),
	U_BOOT_CMD_MKENT(rsv,			6, 0, do_store_rsv_ops, "", ""),
};

static int do_store(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	cmd_tbl_t *c;

	if (argc < 2)
		return CMD_RET_USAGE;

	c = find_cmd_tbl(argv[1], cmd_store_sub, ARRAY_SIZE(cmd_store_sub));

	if (c) {
		return c->cmd(cmdtp, flag, argc, argv);
	}

	return CMD_RET_USAGE;
}

U_BOOT_CMD(store, CONFIG_SYS_MAXARGS, 1, do_store,
	"STORE sub-system",
	"store init [flag]\n"
	"	init storage device\n"
	"store read addr [partition name] off size\n"
	"	read 'size' bytes from offset 'off'\n"
	"	of device/partition 'partition name' to.\n"
	"	address 'addr' of memory.\n"
	"store write addr [partition name] off size\n"
	"	write 'size' bytes to offset 'off' of\n"
	"	device/partition [partition name] from\n"
	"	address 'addr' of memory.\n"
	"store erase [partition name] off size.\n"
	"	erase 'size' bytes from offset 'off'\n"
	"	of device/partition [partition name]\n"
	"store scrub [partition name] off size.\n"
	"	erase 'size' bytes from offset 'off'\n"
	"	of device/partition [partition name]\n"
	"	includes oob area if the device has.\n"
	"store boot_read bootloader/tpl addr cpy size\n"
	"	read 'size' bytes from 'cpy'th backup\n"
	"	of 'bootloader/tpl' partition.\n"
	"store boot_write bootloader/tpl addr [cpy] size\n"
	"	write 'size' bytes to 'cpy' of\n"
	"	bootloader/tpl partition from address\n"
	"	'addr' of memory. when the optional 'cpy'\n"
	"	is null, it will writes to all copies\n"
	"store boot_erase bootloader/tpl [cpy]\n"
	"	when the optional 'cpy' is null, it\n"
	"	will erase all copies.\n"
	"store rsv read name addr size\n"
	"	read 'size' bytes 'name' rsv info\n"
	"	to address 'addr' of memory\n"
	"	'name' could be key/dtb/env etc..."
	"store rsv write name addr size\n"
	"	write 'size' bytes 'name' rsv info\n"
	"	from address 'addr' of memory\n"
	"store rsv erase [name]\n"
	"	erase 'name' rsv info it will erase\n"
	"	all rsv info when [name] is null\n"
	"store rsv protect [name] on/off\n"
	"	turn on/off the rsv info protection\n"
	"	it will operates all when [name] is null\n"
);
