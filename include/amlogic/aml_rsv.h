/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __MESON_RSV_H_
#define __MESON_RSV_H_

#define NAND_RSV_BLOCK_NUM 48

#define NAND_GAP_BLOCK_NUM 4
#define NAND_BBT_BLOCK_NUM 4
#define NAND_ENV_BLOCK_NUM 8
#define NAND_KEY_BLOCK_NUM 8
#define NAND_DTB_BLOCK_NUM 4
#define NAND_DDR_BLOCK_NUM 2

#define BBT_NAND_MAGIC	"nbbt"
#define ENV_NAND_MAGIC	"nenv"
#define KEY_NAND_MAGIC	"nkey"
#define SEC_NAND_MAGIC	"nsec"
#define DTB_NAND_MAGIC	"ndtb"
#define DDR_NAND_MAGIC	"nddr"

/*define abnormal state for reserved area*/
#define POWER_ABNORMAL_FLAG	0x01
#define ECC_ABNORMAL_FLAG	0x02
#define DDR_PARA_SIZE 2048

struct meson_rsv_info_t {
	struct mtd_info *mtd;
	struct valid_node_t *nvalid;
	struct free_node_t *nfree;
	unsigned int start;
	unsigned int end;
	unsigned int size;
	char name[8];
	u_char valid;
	u_char init;
	void *handler;
};

struct valid_node_t {
	s16 ec;
	s16	blk_addr;
	s16	page_addr;
	int timestamp;
	s16 status;
};

struct free_node_t {
	unsigned int index;
	s16 ec;
	s16	blk_addr;
	int dirty_flag;
	struct free_node_t *next;
};

struct oobinfo_t {
	char name[4];
	s16 ec;
	unsigned timestamp:15;
	unsigned status_page:1;
};

struct meson_rsv_handler_t {
	struct mtd_info *mtd;
	unsigned long long fn_bitmask;
	struct free_node_t *free_node[NAND_RSV_BLOCK_NUM];
	struct meson_rsv_info_t *bbt;
#ifndef CONFIG_ENV_IS_IN_NAND
	struct meson_rsv_info_t *env;
#endif
	struct meson_rsv_info_t *key;
	struct meson_rsv_info_t *dtb;
	struct meson_rsv_info_t *ddr_para;
	void *priv;
};

int meson_rsv_bbt_read(u_char *dest, size_t size);
int meson_rsv_key_read(u_char *dest, size_t size);
int meson_rsv_env_read(u_char *dest, size_t size);
int meson_rsv_dtb_read(u_char *dest, size_t size);
int meson_rsv_ddr_para_read(u_char *dest, size_t size);
/* do not provide bbt write operates */
int meson_rsv_key_write(u_char *source, size_t size);
int meson_rsv_env_write(u_char *source, size_t size);
int meson_rsv_dtb_write(u_char *source, size_t size);
int meson_rsv_ddr_para_write(u_char *source, size_t size);
uint32_t meson_rsv_bbt_size(void);
uint32_t meson_rsv_key_size(void);
uint32_t meson_rsv_env_size(void);
uint32_t meson_rsv_dtb_size(void);
u32 meson_rsv_ddr_para_size(void);
/* do not provide bbt erase operates */
int meson_rsv_key_erase(void);
int meson_rsv_env_erase(void);
int meson_rsv_dtb_erase(void);
int meson_rsv_ddr_para_erase(void);
int meson_rsv_check(struct meson_rsv_info_t *rsv_info);
int meson_rsv_init(struct mtd_info *mtd, struct meson_rsv_handler_t *handler);
int meson_rsv_scan(struct meson_rsv_info_t *rsv_info);
int meson_rsv_read(struct meson_rsv_info_t *rsv_info, u_char *buf);
int meson_rsv_save(struct meson_rsv_info_t *rsv_info, u_char *buf);
int meson_rsv_write(struct meson_rsv_info_t *rsv_info, u_char *buf);
int meson_rsv_erase_protect(struct meson_rsv_handler_t *handler,
uint32_t block_addr);
#endif/* __MESON_RSV_H_ */
