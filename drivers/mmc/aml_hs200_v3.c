/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/mmc/aml_hs200_v3.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <errno.h>
#include <mmc.h>
#include <part.h>
#include <malloc.h>
#include <linux/list.h>
#include <div64.h>
#include "mmc_private.h"
#include <asm/arch/sd_emmc.h>
#include <asm/arch/secure_apb.h>
#include <emmc_partitions.h>
#include <asm/cpu_id.h>
#include <linux/compat.h>
#include <asm/arch/timer.h>
#include <asm/arch/cpu_sdio.h>

#define MMC_PATTERN_OFFSET ((SZ_1M * (36 + 3)) / 512)
#define MMC_MAGIC_OFFSET ((SZ_1M * (36 + 3)) / 512)
#define MMC_RANDOM_OFFSET ((SZ_1M * (36 + 3)) / 512)


#ifdef EMMC_DEBUG_ENABLE
	#define emmc_debug(a...) printf(a);
#else
	#define emmc_debug(a...)
#endif
#ifdef MMC_HS400_MODE
u64 align[10];

void print_all_reg(struct mmc *mmc) {
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;

	printf("sd_emmc_regs->gcfg is %x\n", sd_emmc_regs->gcfg);
	printf("sd_emmc_regs->gclock is %x\n", sd_emmc_regs->gclock);
	printf("sd_emmc_regs->gadjust is %x\n", sd_emmc_regs->gadjust);
	printf("sd_emmc_regs->gdelay is %x\n", sd_emmc_regs->gdelay);
	printf("sd_emmc_regs->gdelay1 is %x\n", sd_emmc_regs->gdelay1);
	printf("sd_emmc_regs->gintf3 is %x\n", sd_emmc_regs->gintf3);
	return;
}

static int fbinary(u64 x)
{
	int i;
	for (i = 0; i < 64; i++) {
		if ((x >> i) & 0x1)
			return i;
	}
	return -1;
}
//
//static int mmc_get_status(struct mmc *mmc, int timeout)
//{
//	struct mmc_cmd cmd;
//	int err, retries = 1;
//
//	cmd.cmdidx = MMC_CMD_SEND_STATUS;
//	cmd.resp_type = MMC_RSP_R1;
//	if (!mmc_host_is_spi(mmc))
//		cmd.cmdarg = mmc->rca << 16;
//
//	do {
//		err = mmc_send_cmd(mmc, &cmd, NULL);
//		if (!err) {
//			if ((cmd.response[0] & MMC_STATUS_RDY_FOR_DATA) &&
//			    (cmd.response[0] & MMC_STATUS_CURR_STATE) !=
//			     MMC_STATE_PRG)
//				break;
//			else if (cmd.response[0] & MMC_STATUS_MASK) {
//				return COMM_ERR;
//			}
//		} else if (--retries < 0)
//			return err;
//		udelay(1000);
//	} while (timeout--);
//
//	if (timeout <= 0) {
//		return TIMEOUT;
//	}
//	if (cmd.response[0] & MMC_STATUS_SWITCH_ERROR)
//		return SWITCH_ERR;
//
//	return 0;
//}

static int mmc_read_single_block(struct mmc *mmc, void *dst, lbaint_t start)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int ret = 0;

	cmd.cmdidx = MMC_CMD_READ_SINGLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * mmc->read_bl_len;

	cmd.resp_type = MMC_RSP_R1;

	data.dest = dst;
	data.blocks = 1;
	data.blocksize = mmc->read_bl_len;
	data.flags = MMC_DATA_READ;

	ret = mmc_send_cmd(mmc, &cmd, &data);
	return ret;
}

static long long para_checksum_calc(struct tuning_para *para)
{
	int i = 0;
	int size = sizeof(struct tuning_para) - 6 * sizeof(unsigned int);
	unsigned int *buffer;
	long long checksum = 0;

	size = size >> 2;
	buffer = (unsigned int *)para;
	while (i < size)
		checksum += buffer[i++];

	return checksum;
}

/*set para on controller register*/
int aml_set_tuning_para(struct mmc *mmc)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct tuning_para *para = aml_priv->para;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	int temp_index;
	u32 delay1, delay2, intf3;

	if (para->update == 1)
		return 0;

	temp_index = para->temperature / 10;
	delay1 = para->hs4[temp_index].delay1;
	delay2 = para->hs4[temp_index].delay2;
	intf3 = para->hs4[temp_index].intf3;

	sd_emmc_regs->gdelay = delay1;
	sd_emmc_regs->gdelay1 = delay2;
	sd_emmc_regs->gintf3 = intf3;
	sd_emmc_regs->gstatus = 0x1ff0000;
	sd_emmc_regs->girq_en = 0x23fff;
	return 1;
}

/*save parameter on mmc_host pdata*/
void aml_save_tuning_para(struct mmc *mmc)
{
	long long checksum;
	int temp_index;
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	struct tuning_para *para = aml_priv->para;

	u32 delay1 = sd_emmc_regs->gdelay;
	u32 delay2 = sd_emmc_regs->gdelay1;
	u32 intf3 = sd_emmc_regs->gintf3;

	temp_index = para->temperature / 10;
	if (temp_index < 0 || temp_index > 6) {
		para->update = 0;
		printf("temperature is out of index, can't be saved\n");
		return;
	}

	para->hs4[temp_index].delay1 = delay1;
	para->hs4[temp_index].delay2 = delay2;
	para->hs4[temp_index].intf3 = intf3;
	para->hs4[temp_index].flag = TUNED_FLAG;

	para->magic = TUNED_MAGIC; /*E~K\0*/
	para->version = TUNED_VERSION;

	checksum = para_checksum_calc(para);
	para->checksum = checksum;
	return;
}

#ifdef MMC_WRITE_TUNING_PARA
/*write tuning para on emmc, the offset is 0x14400*/
static int aml_write_tuning_para(struct mmc *mmc)
{
	unsigned int size;
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct tuning_para *parameter = aml_priv->para;
	unsigned int *buf;
	int para_size;
	int blocks;
	int cnt;
	int offset;

	offset = MMC_TUNING_OFFSET;
	para_size = sizeof(struct tuning_para);
	blocks = (para_size - 1)  / 512 + 1;
	size = blocks << 9;

	buf = (unsigned int *)malloc(size);
	if (!buf) {
		printf("malloc buf failed\n");
		return -1;
	}

	memset(buf, 0, size);
	memcpy(buf, parameter, sizeof(struct tuning_para));

	cnt = mmc->block_dev.block_write(1, offset, blocks, buf);
	if (cnt != blocks)
		printf("write parameter failed\n");

	free(buf);
	return 0;
}
#endif

int aml_emmc_hs200_tl1(struct mmc *mmc)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	struct sd_emmc_clock *vclkc = (struct sd_emmc_clock *)&sd_emmc_regs->gclock;

	u32 clk_bak = 0;
	u32 delay2 = 0, count = 0;
	int i, j, err = 0;
	int retry_times = 1;

	mmc->refix = 1;
//	clk_bak = vclkc;
	clk_bak = 0x10100002;

	sd_emmc_regs->gclock = 0x10100004;
	sd_emmc_regs->gintf3 |= 1 << 22;
	sd_emmc_regs->gcfg = 0x804892;
	sd_emmc_regs->gintf3 = 0x4000c8;

	printf("[%s][%d] clk config:0x%x\n",
		__func__, __LINE__, sd_emmc_regs->gclock);
	for (i = 0; i < 63; i++) {
		delay2 += (1 << 24);
		sd_emmc_regs->gdelay1 = delay2;
retry:
		err = emmc_eyetest_log(mmc, 9);
		if (err)
			continue;
		count = fbinary(align[9]);
		if (((count >= 14) && (count <= 20))
			|| ((count >= 48) && (count <= 54))) {
			if (retry_times != 3) {
				retry_times++;
				goto retry;
			} else
				break;
		}
	}

	if (i == 63) {
		for (j = 0; j < 6; j++) {
			vclkc->tx_delay++;
			err = emmc_eyetest_log(mmc, 9);
			if (err)
				continue;
			count = fbinary(align[9]);
			if (((count >= 14) && (count <= 20))
				|| ((count >= 48) && (count <= 54)))
				break;
		}
	}

	aml_priv->cmd_c = (delay2 >> 24);
	sd_emmc_regs->gdelay1 = 0;
	sd_emmc_regs->gclock = clk_bak;
	delay2 = 0;
	for (i = 0; i < 63; i++) {
		retry_times = 0;
		delay2 += (1 << 24);
		sd_emmc_regs->gdelay1 = delay2;
retry1:
		err = emmc_eyetest_log(mmc, 9);
		if (err)
			continue;

		count = fbinary(align[9]);
		if (count >= 8 && count <= 56) {
			if (retry_times != 3) {
				retry_times++;
				goto retry1;
			} else
				break;
		}
	}

	printf("[%s][%d] clk config:0x%x\n",
		__func__, __LINE__, sd_emmc_regs->gclock);
	mmc->refix = 0;
	return 0;
}

void select_clock_src(void) {
	void* buf;
	unsigned long  addr;
	int size;
	u64 writeval;
	unsigned int clock_src = 0;
	unsigned long byte;
	cpu_id_t cpu_id = get_cpu_id();

	if (cpu_id.family_id == MESON_CPU_MAJOR_ID_TL1
			|| cpu_id.family_id == MESON_CPU_MAJOR_ID_T5)
		clock_src = 0x880;
	else if (cpu_id.family_id >= MESON_CPU_MAJOR_ID_G12B)
		clock_src = 0x880;
	else
		clock_src = 0xe80;

	size = 4;
	byte = size;
	addr = CLKSRC_BASE + 0x25c;
	writeval = clock_src;
	buf = map_sysmem(addr, byte);
	*((u32 *)buf) = (u32)writeval;
	unmap_sysmem(buf);

	writel(0x17ff, PAD_PULL_UP_REG0);
	writel(0xffffffff, PAD_DS_REG0A);
}

int mmc_set_hs200_mode(struct mmc *mmc)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	int err = 0;

	if (aml_priv->sd_emmc_port != 2)
		return err;

	err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
			EXT_CSD_BUS_WIDTH, 2);
	if (err) {
		printf("mmc switch buswidth failed\n");
		return err;
	}

	mmc_set_bus_width(mmc, MMC_MODE_8BIT);

	err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
			EXT_CSD_HS_TIMING, EXT_CSD_TIMING_HS200);
	if (err) {
		printf("mmc switch HS200 failed\n");
		return err;
	}

	return err;
}

static int emmc_send_deselect(struct mmc *mmc)
{
	struct mmc_cmd cmd = {0};
	u32 err = 0;

	cmd.cmdidx = MMC_CMD_SELECT_CARD;
	cmd.cmdarg = 0;
	cmd.resp_type = MMC_RSP_NONE;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err) {
		printf("[%s][%d] cmd:0x%x send error\n",
				__func__, __LINE__, cmd.cmdidx);
		return err;
	}
	return err;
}

static int emmc_send_select(struct mmc *mmc)
{
	struct mmc_cmd cmd = {0};
	u32 err = 0;

	cmd.cmdidx = MMC_CMD_SELECT_CARD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 1 << 16;
	err = mmc_send_cmd(mmc, &cmd, NULL);

//	if (err) {
//		printf("[%s][%d] cmd:0x%x send error\n",
//				__func__, __LINE__, cmd.cmdidx);
//		return err;
//	}
	return err;
}

static int emmc_send_cid(struct mmc *mmc)
{
	struct mmc_cmd cmd = {0};
	u32 err = 0;

	cmd.cmdidx = MMC_CMD_SEND_CID;
	cmd.cmdarg = (1 << 16);
	cmd.resp_type = MMC_RSP_R2;

	err = mmc_send_cmd(mmc, &cmd, NULL);
//	if (err) {
//		printf("[%s][%d] cmd:0x%x send error\n",
//				__func__, __LINE__, cmd.cmdidx);
//		return err;
//	}
	return err;
}

static int aml_sd_emmc_cmd_v3(struct mmc *mmc)
{
	int i;
	int ret;
	ret = mmc_send_status(mmc, 1000);
	ret |= emmc_send_deselect(mmc);
	for (i = 0; i < 2; i++)
		ret |= emmc_send_cid(mmc);
	ret |= emmc_send_select(mmc);
	return ret;
}

static int emmc_detect_base_line(u64 *arr)
{
	u32 i = 0, first[10] = {0};
	u32  max = 0, l_max = 0xff;
	for (i = 0; i < 8; i++) {
		first[i] = fbinary(arr[i]);
		if (first[i] > max) {
			l_max = i;
			max = first[i];
		}
	}
	printf("%s [%d] detect line:%d, max: %u\n",
			__func__, __LINE__, l_max, max);
	return max;
}

/**************** start all data align ********************/
static int emmc_all_data_line_alignment(struct mmc *mmc)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	u32 delay1 = 0, delay2 = 0;
	int result;
	int temp = 0, base_line = 0, line_x = 0;

	base_line = emmc_detect_base_line(align);
	for (line_x = 0; line_x < 9; line_x++) {
		if (line_x == 8)
			continue;
		if (align[line_x] & 0xf)
			continue;
		temp = fbinary(align[line_x]);
		result = base_line - temp;
		emmc_debug("*****line_x: %d, result: %d\n",
				line_x, result);
	    if (line_x < 5)
			delay1 |= result << (6 * line_x);
	    else
			delay2 |= result << (6 * (line_x - 5));
	}
	sd_emmc_regs->gdelay += delay1;
	sd_emmc_regs->gdelay1 += delay2;
	emmc_debug("gdelay: 0x%x, gdelay1: 0x%x\n",
			sd_emmc_regs->gdelay, sd_emmc_regs->gdelay1);
	return 0;
}

int emmc_eyetest_log(struct mmc *mmc, u32 line_x)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;

	u32 adjust = sd_emmc_regs->gadjust;
	struct sd_emmc_adjust_v3 *gadjust =
		(struct sd_emmc_adjust_v3 *)&adjust;
	u32 eyetest_log = 0;
	struct eyetest_log *geyetest_log = (struct eyetest_log *)&(eyetest_log);
	u32 eyetest_out0 = 0, eyetest_out1 = 0;
	u32 intf3 = sd_emmc_regs->gintf3;
	struct intf3 *gintf3 = (struct intf3 *)&(intf3);
	/*u32 vcfg = sd_emmc_regs->gcfg;*/
	int retry = 3;
	u64 tmp = 0;
	unsigned long phy_addr = 0x1080000;
	void *addr =  (void*)phy_addr;
	int i;
	lbaint_t start = 1;
	//((SZ_1M*(26+3))/ 512);

	/****** calculate  line_x ***************************/
	/******* init eyetest register ************************/
	gadjust->cali_enable = 1;
	gadjust->cali_sel = line_x;
	sd_emmc_regs->gadjust = adjust;
	if (line_x < 9)
		gintf3->eyetest_exp = 7;
	else
		gintf3->eyetest_exp = 3;
RETRY:

	gintf3->eyetest_on = 1;
	sd_emmc_regs->gintf3 = intf3;

	/*****test start*************/
	udelay(5);
	if (line_x < 9)
		for (i = 0; i< 2; i++)
			mmc_bread(1, start, 256, addr);
	else
		aml_sd_emmc_cmd_v3(mmc);
	udelay(1);
	eyetest_log = sd_emmc_regs->geyetest_log;

	if (!(geyetest_log->eyetest_done & 0x1)) {
		printf("testint eyetest times: 0x%x, out: 0x%x, 0x%x\n",
				geyetest_log->eyetest_times,
				eyetest_out0, eyetest_out1);
		gintf3->eyetest_on = 0;
		sd_emmc_regs->gintf3 = intf3;
		retry--;

		if (retry == 0) {
			printf("[%s][%d] retry eyetest failed\n",
					__func__, __LINE__);
			return 1;
		}
		goto RETRY;
	}
	eyetest_out0 = sd_emmc_regs->geyetest_out0;
	eyetest_out1 = sd_emmc_regs->geyetest_out1;
	gintf3->eyetest_on = 0;
	sd_emmc_regs->gintf3 = intf3;
	align[line_x] = ((tmp | eyetest_out1) << 32) | eyetest_out0;
	printf("d1:0x%x,d2:0x%x,u64eyet:0x%016llx,l_x:%d\n",
			sd_emmc_regs->gdelay, sd_emmc_regs->gdelay1,
			align[line_x], line_x);
	return 0;
}

static int emmc_ds_data_alignment(struct mmc *mmc)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	u32 delay1 = sd_emmc_regs->gdelay;
	u32 delay2 = sd_emmc_regs->gdelay1;
	int i, line_x, temp = 0;

	for (line_x = 0; line_x < 8; line_x++) {
		temp = fbinary(align[line_x]);
		if (temp <= 4)
			continue;
		for (i = 0; i < 64; i++) {
			emmc_debug("i = %d,delay1:0x%x,delay2:0x%x\n",
				i, sd_emmc_regs->gdelay,
				sd_emmc_regs->gdelay1);
			if (line_x < 5)
				delay1 += 1<<(6*line_x);
			else
				delay2 += 1<<(6*(line_x-5));
			sd_emmc_regs->gdelay = delay1;
			sd_emmc_regs->gdelay1 = delay2;
			emmc_eyetest_log(mmc, line_x);
			if (align[line_x] & 0xf0)
				break;
		}
		if (i == 64) {
			printf("%s [%d] Don't find line delay which aligned with DS\n",
				__func__, __LINE__);
			return 1;
		}
	}
	return 0;
}

static unsigned int get_emmc_cmd_win(struct mmc *mmc)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	u32 delay2 = sd_emmc_regs->gdelay1;
	u32 max = 0, i, temp;
	u32 str[64] = {0};
	int best_start = -1, best_size = -1;
	int cur_start = -1, cur_size = 0;

	for (i = 0; i < 64; i++) {
		delay2 &= ~(0x3f << 24);
		delay2 |= (i << 24);
		sd_emmc_regs->gdelay1 = delay2;
		emmc_eyetest_log(mmc, 9);
		temp = fbinary(align[9]);
		str[i] = temp;
		if (max < temp)
			max = temp;
	}
	for (i = 0; i < 64; i++) {
		if (str[i] >= 4) {
			if (cur_start < 0)
				cur_start = i;
			cur_size++;
		} else {
			if (cur_start >= 0) {
				if (best_start < 0) {
					best_start = cur_start;
					best_size = cur_size;
				} else {
					if (best_size < cur_size) {
						best_start = cur_start;
						best_size = cur_size;
					}
				}
				cur_start = -1;
				cur_size = 0;
			}
		}
	}
	if (cur_start >= 0) {
		if (best_start < 0) {
			best_start = cur_start;
			best_size = cur_size;
		} else if (best_size < cur_size) {
			best_start = cur_start;
			best_size = cur_size;
		}
		cur_start = -1;
		cur_size = -1;
	}
	delay2 &= ~(0x3f << 24);
	delay2 |= ((best_start + (best_size * 3 / 4)) << 24);
	sd_emmc_regs->gdelay1 = delay2;
	emmc_eyetest_log(mmc, 9);
	return max;
}

/* first step*/
static int emmc_ds_core_align(struct mmc *mmc)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;

	u32 delay1 = sd_emmc_regs->gdelay;
	u32 delay2 = sd_emmc_regs->gdelay1;
	u32 delay2_bak = delay2;
	u32 count = 0, ds_count = 0, cmd_count = 0;
	ds_count = fbinary(align[8]);
	if (ds_count == 0)
		if ((align[8] & 0x1e0) == 0)
			goto out_cmd;
	emmc_debug("ds_count:%d,delay1:0x%x,delay2:0x%x\n",
			ds_count, sd_emmc_regs->gdelay, sd_emmc_regs->gdelay1);
	if (ds_count < 20) {
		delay2 += ((20 - ds_count) << 18);
		sd_emmc_regs->gdelay1 = delay2;
	} else
		sd_emmc_regs->gdelay1 += (1<<18);
	emmc_eyetest_log(mmc, 8);
	while (!(align[8] & 0xf)) {
		sd_emmc_regs->gdelay1 += (1<<18);
		emmc_eyetest_log(mmc, 8);
	}
	delay1 = sd_emmc_regs->gdelay;
	delay2 = sd_emmc_regs->gdelay1;
	count = ((delay2>>18) & 0x3f) - ((delay2_bak>>18) & 0x3f);

	delay1 += (count<<0)|(count<<6)|(count<<12)|(count<<18)|(count<<24);
	delay2 += (count<<0)|(count<<6)|(count<<12);

	sd_emmc_regs->gdelay = delay1;
	sd_emmc_regs->gdelay1 = delay2;
out_cmd:
	cmd_count = get_emmc_cmd_win(mmc);
	printf("ds_count %u, count: %d, cmd_count:%u\n", ds_count, count, cmd_count);

	return 0;
}

void update_all_line_eyetest(struct mmc *mmc)
{
	int line_x;

	for (line_x = 0; line_x < 10; line_x++) {
		emmc_eyetest_log(mmc, line_x);
	}
}


/*
static int emmc_test_bus(struct mmc *mmc)
{
	unsigned long phy_addr = 0x1080000;
	void * addr = (void*) phy_addr;
	int ret = 0;
	ret = mmc_bread(1, MMC_PATTERN_OFFSET, 40, addr);
	if (ret)
		return ret;
	ret = mmc_bread(1, MMC_MAGIC_OFFSET, 80, addr);
	if (ret)
		return ret;
	ret = mmc_bread(1, MMC_RANDOM_OFFSET, 40, addr);
	if (ret)
		return ret;
	return ret;
}
*/
int emmc_ds_manual_sht(struct mmc *mmc)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;

	u32 intf3 = sd_emmc_regs->gintf3;
	struct intf3 *gintf3 = (struct intf3 *)&(intf3);
	int i, cnt = 0;
	int match[64];
	int best_start = -1, best_size = -1;
	int cur_start = -1, cur_size = 0;
	unsigned long phy_addr = 0x1080000;
	void * addr = (void*) phy_addr;
	u32 begin_time, end_time;
	u32 capacity = mmc->capacity >> 9;
	u32 offset;

//	sd_emmc_regs->gintf3 = 0x4000c8;
	sd_emmc_regs->gintf3 = 0x400000;
	intf3 = sd_emmc_regs->gintf3;
	begin_time = get_time();
	gintf3->ds_sht_m = 0;
	offset = (u32)rand() % capacity;
	for (i = 0; i < 64; i++) {
		cnt = mmc_bread(1, offset, 200, addr);
		if (cnt == 200)
			match[i] = 0;
		else
			match[i] = 1;
		gintf3->ds_sht_m += 1;
		sd_emmc_regs->gintf3 = intf3;
	}
	end_time = get_time();
	printf("spend time is  %dus.\n", end_time - begin_time);

	for (i = 0; i < 64; i++) {
		if (match[i] == 0) {
			if (cur_start < 0)
				cur_start = i;
			cur_size++;
		} else {
			if (cur_start >= 0) {
				if (best_start < 0) {
					best_start = cur_start;
					best_size = cur_size;
				} else {
					if (best_size < cur_size) {
						best_start = cur_start;
						best_size = cur_size;
					}
				}
				cur_start = -1;
				cur_size = 0;
			}
		}
	}
	if (cur_start >= 0) {
		if (best_start < 0) {
			best_start = cur_start;
			best_size = cur_size;
		} else if (best_size < cur_size) {
			best_start = cur_start;
			best_size = cur_size;
		}
		cur_start = -1;
		cur_size = -1;
	}

	gintf3->ds_sht_m = best_start + best_size / 2;
	sd_emmc_regs->gintf3 = intf3;
	aml_priv->win_start = best_start;
	printf("ds_sht:%u, window:%d, intf3:0x%x, clock:0x%x, cfg: 0x%x\n",
			gintf3->ds_sht_m, best_size,
			sd_emmc_regs->gintf3,
			sd_emmc_regs->gclock,
			sd_emmc_regs->gcfg);
	return best_size;
}

static ulong mmc_write_single_blocks(struct mmc *mmc, lbaint_t start,
		const void *src)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int timeout = 1000;
	int ret;

	udelay(100);
	if ((start + 1) > mmc->block_dev.lba) {
		printf("MMC: block number 0x" LBAF " exceeds max(0x" LBAF ")\n",
		       start + 1, mmc->block_dev.lba);
		return 0;
	}

	cmd.cmdidx = MMC_CMD_WRITE_SINGLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * mmc->write_bl_len;

	cmd.resp_type = MMC_RSP_R1;

	data.src = src;
	data.blocks = 1;
	data.blocksize = mmc->write_bl_len;
	data.flags = MMC_DATA_WRITE;

	ret = mmc_send_cmd(mmc, &cmd, &data);
	if (ret) {
		goto _out;
	}

_out:
	/* Waiting for the ready status */
	if (mmc_send_status(mmc, timeout))
		return 0;
	if (ret)
		return 0;
	return 1;
}

int auto_set_tx_delay(struct mmc *mmc)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;

	u32 clock = sd_emmc_regs->gclock;
	struct sd_emmc_clock *gclock = (struct sd_emmc_clock *)&(clock);
	int i;
	int match[64];
	int best_start = -1, best_size = -1;
	int cur_start = -1, cur_size = 0;
	unsigned long phy_addr = 0x1080000;
	void * addr = (void*) phy_addr;
	lbaint_t start = ((SZ_1M*(36+3))/512);
	int success_count=0;
	int count;

	if (mmc_bread(1, start, 1, addr) != 1) {
		return 0;
	}

	gclock->tx_delay = 0;
	printf("tx sample result:");
	for (i = 0; i < 64; i++) {
		gclock->tx_delay = i;
		sd_emmc_regs->gclock = clock;
		success_count = 0;

		for (count = 0; count < 256; count++) {
			if (mmc_write_single_blocks(mmc, start, addr))
				success_count++;
		}
		if (success_count == 256)
			printf("Y");
		else
			printf("N");
		if (success_count == 256)
			match[i] = 0;
		else
			match[i] = -1;
	}
	printf("\n");
	for (i = 0; i < 64; i++) {
		if (match[i] == 0) {
			if (cur_start < 0)
				cur_start = i;
			cur_size++;
		} else {
			if (cur_start >= 0) {
				if (best_start < 0) {
					best_start = cur_start;
					best_size = cur_size;
				} else {
					if (best_size < cur_size) {
						best_start = cur_start;
						best_size = cur_size;
					}
				}
				cur_start = -1;
				cur_size = 0;
			}
		}
	}
	if (cur_start >= 0) {
		if (best_start < 0) {
			best_start = cur_start;
			best_size = cur_size;
		} else if (best_size < cur_size) {
			best_start = cur_start;
			best_size = cur_size;
		}
		cur_start = -1;
		cur_size = -1;
	}
	if (best_size == -1) {
		printf("meson-mmc: can not find tx_delay\n");
		return 0;
	}
	gclock->tx_delay = best_start + best_size / 2;
	sd_emmc_regs->gclock = clock;
	printf("meson-mmc: tx_delay:%u\n", gclock->tx_delay);
	return 0;
}

unsigned int aml_sd_emmc_clktest(struct mmc *mmc)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;

	u32 intf3 = sd_emmc_regs->gintf3;
	struct intf3 *gintf3 = (struct intf3 *)&(intf3);
	u32 clktest = 0, delay_cell = 0, clktest_log = 0, count = 0;
	u32 vcfg = sd_emmc_regs->gcfg;

	int i = 0;
	unsigned int cycle = 0;

	sd_emmc_regs->gadjust = 0;

	/* one cycle = xxx(ps) */
	cycle = (1000000000 / mmc->clock) * 1000;
	vcfg &= ~(1 << 23);
	vcfg = 0x4896;

	sd_emmc_regs->gcfg = vcfg;
	sd_emmc_regs->gdelay = 0;
	sd_emmc_regs->gdelay1 = 0;

	gintf3->clktest_exp = 8;
	gintf3->clktest_on_m = 1;
	sd_emmc_regs->gintf3 = intf3;

	clktest_log = sd_emmc_regs->gclktest_log;
	clktest = sd_emmc_regs->gclktest_out;;
	while (!(clktest_log & 0x80000000)) {
		mdelay(1);
		i++;
		clktest_log = sd_emmc_regs->gclktest_log;
		clktest = sd_emmc_regs->gclktest_out;
		if (i > 4) {
			printf("[%s] [%d] emmc clktest error\n",
				__func__, __LINE__);
			break;
		}
	}
	if (clktest_log & 0x80000000) {
		clktest = sd_emmc_regs->gclktest_out;
		count = clktest / (1 << 8);
		if (vcfg & 0x4)
			delay_cell = ((cycle / 2) / count);
		else
			delay_cell = (cycle / count);
	}
	printf("%s [%d] clktest : %u, delay_cell: %d, count: %u\n",
		__func__, __LINE__, clktest, delay_cell, count);
	intf3 = sd_emmc_regs->gintf3;
	gintf3->clktest_on_m = 0;
	sd_emmc_regs->gintf3 = intf3;
	vcfg = sd_emmc_regs->gcfg;
	vcfg |= (1 << 23);
	sd_emmc_regs->gcfg = vcfg;
	return count;
}

static void tl1_emmc_line_timing(struct mmc *mmc)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	u32 delay1 = 0, delay2 = 0, count = 12;

	delay2 = sd_emmc_regs->gdelay1;
	delay1  = (count<<0)|(count<<6)|(count<<12)
		|(count<<18)|(count<<24);
	delay2 |= (count<<0)|(count<<6)|(count<<12);
	sd_emmc_regs->gdelay = delay1;
	sd_emmc_regs->gdelay1 = delay2;
	return;

}

static u32 emmc_search_cmd_delay(char *str, int repeat_times)
{
	int best_start = -1, best_size = -1;
	int cur_start = -1, cur_size = 0;
	u32 cmd_delay;
	int i;

	for (i = 0; i < 64; i++) {
		if (str[i] == repeat_times) {
			cur_size += 1;
			if (cur_start == -1)
				cur_start = i;
		} else {
			cur_size = 0;
			cur_start = -1;
		}

		if (cur_size > best_size) {
			best_size = cur_size;
			best_start = cur_start;
		}
	}

	cmd_delay = (best_start + best_size / 2) << 24;
	printf("best_start 0x%x, best_size %d, cmd_delay is 0x%x\n",
			best_start, best_size, cmd_delay >> 24);
	return cmd_delay;
}

static void emmc_show_cmd_window(char *str, int repeat_times)
{
	int pre_status = 0;
	int status = 0;
	int single = 0;
	int start = 0;
	int i;

	printf(">>>>>>>>>>>>>>scan command window>>>>>>>>>>>>>>>\n");
	for (i = 0; i < 64; i++) {
		if (str[i] == repeat_times)
			status = 1;
		else
			status = -1;

		if (i != 0 && pre_status != status) {
			if (pre_status == 1 && single == 1)
				printf(">>cmd delay [ 0x%x ] is ok\n", i - 1);
			else if (pre_status == 1 && single != 1)
				printf(">>cmd delay [ 0x%x -- 0x%x ] is ok\n",
						start, i - 1);
			else if (pre_status != 1 &&  single == 1)
				printf(">>cmd delay [ 0x%x ] is nok\n", i - 1);
			else if (pre_status != 1 && single != 1)
				printf(">>cmd delay [ 0x%x -- 0x%x ] is nok\n",
						start, i - 1);
			start = i;
			single = 1;
		} else
			single++;

		if (i == 63) {
			if (status == 1 && pre_status == 1)
				printf(">>cmd delay [ 0x%x -- 0x%x ] is ok\n",
						start, i);
			else if (status != 1 && pre_status == -1)
				printf(">>cmd delay [ 0x%x -- 0x%x ] is nok\n",
						start, i);
			else if (status == 1 && pre_status != 1)
				printf(">>cmd delay [ 0x%x ] is ok\n", i);
			else if (status != 1 && pre_status == 1)
				printf(">>cmd delay [ 0x%x ] is nok\n", i);
		}
		pre_status = status;
	}
	printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
}

static u32 scan_emmc_cmd_win(struct mmc *mmc, int send_status)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	u32 delay2 = sd_emmc_regs->gdelay1;
	u32 cmd_delay = 0;
	u32 delay2_bak = delay2;
	u32 i, j, err;
	int repeat_times = 100;
	char str[64] = {0};
	u32 capacity = mmc->capacity >> 9;
	unsigned long phy_addr = 0x1080000;
	void * addr = (void*) phy_addr;
	u32 offset;
	u32 begin_time, end_time;

	addr = (void *)malloc(512 *sizeof(char));
	if (!addr)
		return -1;

	delay2 &= ~(0xff << 24);

	begin_time = get_time();

	for (i = 0; i < 64; i++) {
		sd_emmc_regs->gdelay1 = delay2;
		offset = (u32)rand() % capacity;
		for (j = 0; j < repeat_times; j++) {
			if (send_status)
				err = aml_sd_emmc_cmd_v3(mmc);
			else
				err = mmc_read_single_block(mmc, addr, offset);
			if (!err)
				str[i]++;
			else
				break;
		}
		delay2 += (1 << 24);
	}
	end_time = get_time();
	printf("spend time is  %dus.\n", end_time - begin_time);

	sd_emmc_regs->gdelay1 = delay2_bak;
	cmd_delay = emmc_search_cmd_delay(str, repeat_times);
	if (!send_status)
		emmc_show_cmd_window(str, repeat_times);

	return cmd_delay;
}

static void set_emmc_cmd_delay(struct mmc *mmc, int send_status)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	u32 delay2 = sd_emmc_regs->gdelay1;
	u32 cmd_delay = 0;

	delay2 &= ~(0xff << 24);
	cmd_delay = scan_emmc_cmd_win(mmc, send_status);
	delay2 |= cmd_delay;
	sd_emmc_regs->gdelay1 = delay2;
}

static void aml_emmc_hs400_tl1(struct mmc *mmc)
{
	set_emmc_cmd_delay(mmc, 1);
	tl1_emmc_line_timing(mmc);
	emmc_ds_manual_sht(mmc);
	set_emmc_cmd_delay(mmc, 0);
}

static void aml_emmc_hs400_general(struct mmc *mmc) {
	update_all_line_eyetest(mmc);
	emmc_ds_core_align(mmc);
	update_all_line_eyetest(mmc);
	emmc_all_data_line_alignment(mmc);
	update_all_line_eyetest(mmc);
	emmc_ds_data_alignment(mmc);
	update_all_line_eyetest(mmc);
	emmc_ds_manual_sht(mmc);
}

static int emmc_data_alignment(struct mmc *mmc, int best_size)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	u32 delay1 = sd_emmc_regs->gdelay;
	u32 delay2 = sd_emmc_regs->gdelay1;
	u32 intf3 = sd_emmc_regs->gintf3;
	struct intf3 *gintf3 = (struct intf3 *)&(intf3);
	u32 delay1_bak = delay1;
	u32 delay2_bak = delay2;
	u32 intf3_bak = intf3;
	int line_x, i, cnt = 0, win_new;
	u32 d[8];
	int blk = 200;
	unsigned long phy_addr = 0x1080000;
	void *addr =  (void*)phy_addr;
	lbaint_t offset = 0;
	//((SZ_1M*(26+3))/ 512);

	gintf3->ds_sht_m = aml_priv->win_start + 4;
	sd_emmc_regs->gintf3 = intf3;
	for (line_x = 0; line_x < 8; line_x++) {
		for (i = 0; i < 20; i++) {
			if (line_x < 5) {
				delay1 += (1 << 6 * line_x);
				sd_emmc_regs->gdelay = delay1;
			} else {
				delay2 += (1 << 6 * (line_x - 5));
				sd_emmc_regs->gdelay1 = delay2;
			}
			cnt = mmc_bread(1, offset, blk, addr);
			if (cnt != blk) {
		//		printf("[%s]adjust line_x[%d]:%d\n",
		//				__func__, line_x, i);
				d[line_x] = i;
				delay1 = delay1_bak;
				delay2 = delay2_bak;
				sd_emmc_regs->gdelay = delay1_bak;
				sd_emmc_regs->gdelay1 = delay2_bak;
				break;
			}
		}
		if (i == 20) {
			printf("[%s][%d] return set default value",
					__func__, __LINE__);
			sd_emmc_regs->gdelay = delay1_bak;
			sd_emmc_regs->gdelay1 = delay2_bak;
			sd_emmc_regs->gintf3 = intf3_bak;
			return -1;
		}
	}

	delay1 += (d[0]<<0)|(d[1]<<6)|(d[2]<<12)|(d[3]<<18)|(d[4]<<24);
	delay2 += (d[5]<<0)|(d[6]<<6)|(d[7]<<12);
	sd_emmc_regs->gdelay = delay1;
	sd_emmc_regs->gdelay1 = delay2;
	printf("delay1:0x%x, delay2:0x%x\n",
		sd_emmc_regs->gdelay,
		sd_emmc_regs->gdelay1);
	gintf3->ds_sht_m = 0;
	sd_emmc_regs->gintf3 = intf3;
	win_new = emmc_ds_manual_sht(mmc);
	if (win_new < best_size) {
		printf("[%s][%d] win_new:%d < win_old:%d,set default!",
			__func__, __LINE__, win_new, best_size);
		sd_emmc_regs->gdelay = delay1_bak;
		sd_emmc_regs->gdelay1 = delay2_bak;
		sd_emmc_regs->gintf3 = intf3_bak;
		printf("intf3:0x%x, delay1:0x%x, delay2:0x%x\n",
				sd_emmc_regs->gdelay = delay1_bak,
				sd_emmc_regs->gdelay1 = delay2_bak,
				sd_emmc_regs->gintf3 = intf3_bak);
	}

	return 0;
}

int aml_emmc_hs400_Revb(struct mmc *mmc) {
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	int win_size = 0;

	sd_emmc_regs->gintf3 = 0x4000c8;
	set_emmc_cmd_delay(mmc, 1);

	printf("[%s], delay1: 0x%x, delay2: 0x%x\n",
			__func__, sd_emmc_regs->gdelay,
			sd_emmc_regs->gdelay1);
	win_size = emmc_ds_manual_sht(mmc);
	emmc_data_alignment(mmc, win_size);
	set_emmc_cmd_delay(mmc, 0);
	return 0;
}

int aml_post_hs400_timming(struct mmc *mmc)
{
	int i;

	cpu_id_t cpu_id = get_cpu_id();
	for (i = 0; i < 9; i++)
		align[i] = 0;
	mmc->refix = 1;
	aml_sd_emmc_clktest(mmc);
#ifdef MMC_SET_TUNING_PARA
	if (aml_set_tuning_para(mmc))
		return 0;
#endif

	if (cpu_id.family_id == MESON_CPU_MAJOR_ID_G12B)
		aml_emmc_hs400_Revb(mmc);
	else if (cpu_id.family_id > MESON_CPU_MAJOR_ID_G12B)
		aml_emmc_hs400_tl1(mmc);
	else
		aml_emmc_hs400_general(mmc);

#ifdef MMC_WRITE_TUNING_PARA
	aml_save_tuning_para(mmc);
	aml_write_tuning_para(mmc);
#endif
	mmc->refix = 0;
	return 0;
}

void mmc_set_clock_phase(struct mmc *mmc, int hs_mode)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	u32 clock = sd_emmc_regs->gclock;
	struct sd_emmc_clock *gclock = (struct sd_emmc_clock *) &clock;

	if (hs_mode) {
		gclock->core_phase = MMC_HS_COPHASE;
		sd_emmc_regs->gclock = clock;
	} else {
		gclock->core_phase = MMC_HS4_COPHASE;
		gclock->tx_delay = MMC_HS400_TXDELAY;
		sd_emmc_regs->gclock = clock;
	}
	return;
}

void mmc_set_txdelay(struct mmc *mmc, unsigned int tx_delay)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;

	sd_emmc_regs->gclock &= ~(0x1f << Cfg_tx_delay);
	sd_emmc_regs->gclock |= (tx_delay << Cfg_tx_delay);

	return;
}
void mmc_set_ddr_mode(struct mmc *mmc)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	u8 clk_div;

	clk_div = 0x3f & sd_emmc_regs->gclock;
	if (clk_div & 1)
		 clk_div = (clk_div + 1) >> 1 ;
	else
		clk_div = clk_div >> 1;

	sd_emmc_regs->gclock &= ~(0x3f & sd_emmc_regs->gclock);
	sd_emmc_regs->gclock |= 0x3f & clk_div;

	sd_emmc_regs->gcfg |= 1 << 2;

	return;
}

void mmc_set_ds_enable(struct mmc *mmc)
{
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;

	sd_emmc_regs->gadjust |= 1 << 15;

	return ;
}

void mmc_set_clock_div(struct mmc *mmc, u32 src_speed) {
	struct aml_card_sd_info *aml_priv = mmc->priv;
	struct sd_emmc_global_regs *sd_emmc_regs = aml_priv->sd_emmc_reg;
	u32 clock = sd_emmc_regs->gclock;
	struct sd_emmc_clock *gclock = (struct sd_emmc_clock *) &clock;
	u32 clock_speed = src_speed / 4;

	cpu_id_t cpu_id = get_cpu_id();

	mmc_set_clock(mmc, clock_speed);

	if (cpu_id.family_id >= MESON_CPU_MAJOR_ID_G12B) {
		gclock->div = 2;
		gclock->tx_delay = MMC_HS400_TXDELAY;
	} else {
		gclock->div = 1;
		gclock->tx_delay =12;//MMC_HS400_TXDELAY;
	}
	gclock->core_phase = MMC_HS4_COPHASE;
	gclock->src = 0;
	sd_emmc_regs->gclock = clock;
	mmc->tran_speed = src_speed / 4;
	printf("clock is 0x%x\n", clock);

	return ;
}

/*
 * Function to enable HS400 mode
 * 1. Set the HS_TIMING on ext_csd 185 to 0x01
 * 2. Set the clock frequency to 52MHz
 * 3. Set the bus width to 8 bit DDR as supported by the target & host
 * 4. Set the HS_TIMING to 0x03
 * 5. Set the clock frequency to 200 MHZ
 */
uint32_t mmc_set_hs400_mode(struct mmc *mmc)
{
	uint32_t err = 0;
	struct aml_card_sd_info *aml_priv = mmc->priv;
	cpu_id_t cpu_id = get_cpu_id();

	if (aml_priv->sd_emmc_port != 2)
		return err;

	/* set HS_TIMING TO 0X01 */
	err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
			EXT_CSD_HS_TIMING, EXT_CSD_TIMING_HS);

	if (err) {
		printf("Switch cmd returned failure %d\n", __LINE__);
		return err;
	}
	/* Set Clock @ 52MHZ */
	mmc_set_clock(mmc, 50000000);

	/* Set 8 bit DDR bus width */
	err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
				      EXT_CSD_BUS_WIDTH, 6);

	if (err) {
		printf("Switch cmd returned failure %d\n", __LINE__);
		return err;
	}
	/* Setting HS400 in HS_TIMING using EXT_CSD (CMD6) */
	err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
				      EXT_CSD_HS_TIMING, 3);

	if (err) {
		printf("Switch cmd returned failure %d\n", __LINE__);
		return err;
	}

	mmc_set_ddr_mode(mmc);
	mmc_set_ds_enable(mmc);

	mmc_set_clock_phase(mmc, 0);

	select_clock_src();

	aml_priv->cfg.f_max = 200000000;
	/* Set Clock @ 400 MHZ */
	if (cpu_id.family_id == MESON_CPU_MAJOR_ID_TL1)
		mmc_set_clock_div(mmc, 792000000);
	else if (cpu_id.family_id == MESON_CPU_MAJOR_ID_T5 ||
			cpu_id.family_id == MESON_CPU_MAJOR_ID_T5D)
		mmc_set_clock_div(mmc, 768000000);
	else if (cpu_id.family_id >= MESON_CPU_MAJOR_ID_G12B)
		mmc_set_clock_div(mmc, 800000000);
	else
		mmc_set_clock_div(mmc, 400000000);

	if (cpu_id.family_id == MESON_CPU_MAJOR_ID_G12B)
		aml_emmc_hs200_tl1(mmc);
	aml_post_hs400_timming(mmc);

	return err;
}
#endif
