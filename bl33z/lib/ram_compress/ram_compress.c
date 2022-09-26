// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <string.h>
#include <serial.h>
#include <lz4.h>
#include <platform_def.h>
#include <ram_compress.h>
#include <arch.h>
#include <stdio.h>
#include <timer.h>
#include <watchdog.h>
#include <string.h>
#include <io.h>
#include <cache_v8.h>

/* a must be order of 2 */
#define __ALIGN(x, mask)		(((x) + (mask)) & ~(mask))
#define ALIGN(x, a)			__ALIGN(x, a - 1)
typedef void (*cb)(unsigned long processed, unsigned long total);

extern unsigned char debug_enable;
//unsigned char debug_enable = 0;
typedef unsigned int  u32;
typedef unsigned long u64;
static inline u64 readq(void *reg)
{
	return ((u64)readl((u64)reg + 0x4UL) << 32) | (u64)readl((u64)reg);
}

static inline void writeq(u64 val, void *reg)
{
	writel(val & 0xffffffff, (u64)reg);
	writel(val >> 32, (u64)reg + 0x4UL);
}

static void run_callback(cb cbf, unsigned long processed, unsigned long total)
{
	if (cbf)
		cbf(processed, total);
}

static int check_over_flow(struct ram_compress_info    *rci,
			   unsigned int wrap_size)
{
	signed long total_process;
	signed long total_used;

	total_used    = rci->total_zip_size +
			rci->curr_zip_size  +
			wrap_size;
	total_process = rci->total_processed +
			rci->curr_processed;

	if (total_used >= total_process) {
		if (debug_enable) {
			serial_puts("==> Full processed:0x");
			serial_put_hex(rci->total_processed, 64);
			serial_puts(", cur processed:0x");
			serial_put_hex(rci->curr_processed, 64);
			serial_puts("\n");
			serial_puts("==> Full compress:0x");
			serial_put_hex(rci->total_zip_size, 64);
			serial_puts(", cur compress:0x");
			serial_put_hex(rci->curr_zip_size, 64);
			serial_puts(", reserved:0x");
			serial_put_hex(wrap_size, 32);
			serial_puts(",  --- OVER FLOW!!!\n");
		}
		return -1;
	}
	return 0;
}

static unsigned char *get_stride(unsigned long *stride,
				 struct ram_compress_info *rci,
				 struct ram_compress_section *rcs,
				 unsigned int *avail)
{
	unsigned int mem_avail = 0;
	unsigned char *out_buf = NULL;
	unsigned int pad_size = 2 * sizeof(unsigned int);

	mem_avail = (rci->total_processed - rci->total_zip_size) +
		    (rci->curr_processed  - rci->curr_zip_size);

	*avail = mem_avail;
	/*
	 * if existing memory gap (space between the end of compressed
	 * content and the beginning address of the ram that is going to
	 * be compressed next) is less than 3MBytes, then use default
	 * compress buffer; otherwise, use this memory gap as much as
	 * possible for compress
	 */
	if (mem_avail <= SELF_COMPRESS_THRESHOLD) {
		/*
		 * leave somespace to keep safe if compressed size
		 * is larger than input size
		 */
		*stride = rci->rcwi->output_buffer_size - 16 * 1024;
		out_buf = rci->rcwi->temp_output_buffer;
	} else {
		*stride = (mem_avail / SZ_1M) * SZ_1M - 256 * 1024;
		out_buf = rci->output_fp + rci->curr_zip_size + pad_size;
	}
	/*
	 * Limit stride to max section size. If some section
	 * of compressed data is polluted by out coming process,
	 * then we can still have other compressed block data to use.
	 * Other wize the whole section may not be used.
	 */
	if (*stride > rci->rcwi->max_zip_section_size)
		*stride = rci->rcwi->max_zip_section_size;

	/* last block to compress */
	if ((*stride + rci->curr_processed) > rcs->section_size)
		*stride = rcs->section_size - rci->curr_processed;

	return out_buf;
}

static int zip_one_section(struct ram_compress_info *rci,
			   struct ram_compress_section *rcs,
			   int wrap_size, unsigned int total_ram)
{
	unsigned char *srcaddr, *dstaddr, *out_buf;
	size_t stride, out_size;
	int ret, slice = 0;
	int pad_size = sizeof(unsigned int);
	unsigned int mem_avail = 0;

	/* start_phyaddr can be 0 */
	if (!rci || !rci->rcwi ||
	    !rci->rcwi->temp_output_buffer ||
	    !rci->rcwi->temp_work_buffer) {
		return EINVALID_ARG;
	}
	serial_puts("####### run zip section start. ########\n");
	rci->curr_processed = 0;
	rci->curr_zip_size  = 0;
	while (rci->curr_processed < rcs->section_size) {
		srcaddr = (unsigned char *)rcs->phy_addr + rci->curr_processed;
		dstaddr = rci->output_fp + rci->curr_zip_size;
		out_buf = get_stride(&stride, rci, rcs, &mem_avail);
		memset(out_buf, 0, stride);
		/* tell system we are alive */
		if (rci->rcwi->hear_beat)
			rci->rcwi->hear_beat();

		if (debug_enable) {
			serial_puts("compressing address:0x");
			serial_put_hex((unsigned long)srcaddr, 64);
			serial_puts(", free space:0x");
			serial_put_hex(mem_avail, 32);
			serial_puts(", out_buf:0x");
			serial_put_hex((unsigned long)out_buf, 64);
			serial_puts(" ,stride:0x");
			serial_put_hex(stride, 32);
		}
		ret = lz4_compress(srcaddr, stride, out_buf,
				   &out_size, rci->rcwi->temp_work_buffer);
		if (debug_enable) {
			serial_puts(" -> 0x");
			serial_put_hex(out_size, 32);
			serial_puts("\n");
		}
		if (ret < 0) {
			/* compress failed, return immediately */
			if (debug_enable) {
				serial_puts("compress ");
				serial_put_hex((unsigned long)srcaddr, 32);
				serial_puts(" failed:");
				serial_put_hex(ret, 32);
				serial_puts("\n");
			}
			return ECOMPRESS_FAILE;
		}

		rci->curr_processed += stride;
		rci->curr_zip_size  += out_size + 2 * pad_size;
		if (check_over_flow(rci, wrap_size)) {
			if (!slice)
				return NEED_RETRY;
			else
				return ECOMPRESS_OVERFLOW;
		}

		slice++;
		/*
		 * copy compressed length info and stride to destination
		 */
		memcpy(dstaddr, &out_size, pad_size);
		memcpy(dstaddr + pad_size, &(stride), pad_size);
		/*
		 * copy compressed content if not in-place compression
		 */
		if (out_buf == rci->rcwi->temp_output_buffer)
			memcpy(dstaddr + 2 * pad_size, out_buf, out_size);

		/*
		 *update progress bar
		 */
		run_callback(rci->rcwi->progress_update,
			     rci->total_processed + rci->curr_processed,
			     total_ram);
	}

	return slice;
}

unsigned long ram_compress_all(struct ram_compress_full *rcf,
		     struct ram_compress_work_info *rcwi)
{
	unsigned char *ramfile_base = NULL;
	unsigned char *current_fp  = NULL;

	struct ram_compress_info            rci;
	struct ram_compress_section        *rcs;
	struct ram_compress_file_header    *rcfh;
	struct ram_compress_section_header *rcsh;

	unsigned int i = 0, wrap_size = 0;
	int ret, sz_rcsh, sz_rcfh;

	if (!rcf || !rcf->section_count)
		return EINVALID_ARG;

	sz_rcsh = sizeof(struct ram_compress_section_header);
	sz_rcfh = sizeof(struct ram_compress_file_header);

	/* setup ram_compress_info */
	memset(&rci, 0, sizeof(rci));
	rci.rcwi = rcwi;

	/* skip the header */
	ramfile_base = rcf->store_phy_addr;
	current_fp   = ramfile_base + sz_rcfh;

	/* going through all ram segments and compress them one by one */
	for (i = 0; i < rcf->section_count; i++) {
		rcs = &rcf->sections[i];
		if (debug_enable) {
			serial_puts("\n");
			serial_puts("process seg:");
			serial_put_hex(i, 8);
			serial_puts(", section index:");
			serial_put_hex(rcs->section_index, 8);
			serial_puts(", phy_addr:");
			serial_put_hex((unsigned long)rcs->phy_addr, 64);

			if (rcs->section_size == 0) {
				serial_puts(" Section size = 0, skip!\n");
				continue;
			}
		}

		rcsh = (struct ram_compress_section_header *)current_fp;
		memset(rcsh, 0, sz_rcsh);
		wrap_size += sz_rcsh;
		switch (rcs->compress_type) {
		case RAM_COMPRESS_SET:
			rcsh->section_index = rcs->section_index;
			rcsh->raw_size      = rcs->section_size;
			rcsh->compress_type = rcs->compress_type;
			/*
			 * fill up certain region with same value (in byte)
			 */
			rcsh->set_value     = rcs->set_value;
			rcsh->zip_size      = sz_rcsh;
			serial_puts(" type: method:RAM_COMPRESS_SET to 0\n");
			break;

		case RAM_COMPRESS_COPY:
			/*
			 * no compression, just simple mem to mem copy
			 * Can memcpy handle mem overlay case???
			 */
			//memcpy((current_fp + sz_rcsh), rcs->phy_addr,rcs->section_size);
			memcpy((current_fp + sz_rcsh), rcs->copy_dest_addr, rcs->section_size);
			rcsh->section_index   = rcs->section_index;
			rcsh->raw_size        = rcs->section_size;
			rcsh->compress_type   = rcs->compress_type;
			rcsh->zip_size        = rcs->section_size + sz_rcsh;
			break;
			/* data was copy to anther addr in bl2 stage.*/
			//rcs->phy_addr = rcs->copy_dest_addr;

		case RAM_COMPRESS_NORMAL:
			/*
			 * need to perform compression on this mem region
			 * build up compress request
			 */
			if (rcs->copy_dest_addr != 0) {
				/* data was copy to anther addr in bl2 stage.*/
				serial_puts(" method:RAM_COMPRESS_COPY ");
				rcs->phy_addr = rcs->copy_dest_addr;
			}
			if (!i)
				wrap_size += sz_rcfh;

			if (debug_enable) {
				serial_puts(", wrap_size:");
				serial_put_hex(wrap_size, 32);
				serial_puts("\n");
			}

			rci.output_fp = current_fp + sz_rcsh;
			ret = zip_one_section(&rci, rcs, wrap_size,
					      rcf->full_memsize);
			if (ret < 0) {
				if (debug_enable) {
					unsigned long base;

					base = (unsigned long)rcs->phy_addr;
					serial_puts("compress:");
					serial_put_hex(base, 32);
					serial_puts(" size:");
					serial_put_hex(rcs->section_size, 32);
					serial_puts(" ret:");
					serial_put_hex(ret, 32);
					serial_puts(" failed\n");
				}
				/*
				 * can retry another start physical address if
				 * first compress failed
				 */
				if (ret == NEED_RETRY && !i)
					return ret;
				return ECOMPRESS_FAILE;
			}
			/*
			 * We should NOT modify the region until it is
			 * compressed. Otherwise, it will corrupt the original
			 * content if start_phyaddr == store_phy_addr
			 */
			rcsh->section_index = rcs->section_index;
			rcsh->raw_size      = rcs->section_size;
			rcsh->compress_type = rcs->compress_type;
			rcsh->set_value     = ret;
			rcsh->zip_size      = sz_rcsh + rci.curr_zip_size;
			break;

		default:
			if (debug_enable) {
				serial_puts("unknown compress type:");
				serial_put_hex(rcs->compress_type, 32);
			}
			return ECOMPRESS_FAILE;
		}

		//if (debug_enable)
		//	serial_puts("\n");
		/*
		 * make sure each segment starts with 64bit aligned address
		 */
		rcsh->zip_size = ALIGN(rcsh->zip_size, 8);

		/*
		 * ready to move to next segment
		 */
		current_fp          += rcsh->zip_size;
		rci.total_zip_size   = current_fp - ramfile_base;
		rci.total_processed += rcs->section_size;
		if (debug_enable) {
			serial_puts("compress orig:");
			serial_put_hex(rcs->section_size, 32);
			serial_puts(" to size:");
			serial_put_hex(rcsh->zip_size, 32);
			serial_puts(" done!\n");
		}

		/* update progress bar */
		run_callback(rci.rcwi->progress_update,
			     rci.total_processed,
			     rcf->full_memsize);
	}

	/* setup the compress file head */
	memset(ramfile_base, 0, sz_rcfh);
	rcfh = (struct ram_compress_file_header *)ramfile_base;
	memcpy(rcfh->ram_compress_tag, COMPRESS_TAG_HEAD, COMPRESS_TAG_SIZE);
	rcfh->section_count = rcf->section_count;
	rcfh->file_size     = current_fp - ramfile_base + COMPRESS_TAG_SIZE;

	/* add tail signature */
	memset(current_fp, 0, COMPRESS_TAG_SIZE);
	memcpy(current_fp, COMPRESS_TAG_TAIL, COMPRESS_TAG_SIZE);

	return rcfh->file_size;
}

void watchdog_ping(void)
{
	//serial_puts("watchdog_ping\n");
}

void print_process(unsigned long processed, unsigned long total)
{
	//serial_puts("print_process\n");
}

#ifdef CONFIG_ENABLE_DUMP
static void dump_info(unsigned long addr, unsigned long size, const char *info)
{
	serial_puts("\ndump ");
	serial_puts(info);
	serial_puts(" [0x");
	serial_put_hex(addr, 32);
	serial_puts("]:\n");
	for (unsigned long i = 0; i < size; i += 4) {
		if ((0 == (i % 32)) && (i != 0))
			serial_puts("\n");
		serial_puts("0x");
		serial_put_hex(readl(addr + i), 32);
		serial_puts(" ");
	}
	serial_puts("\n\n");
}
#endif /* CONFIG_ENABLE_DUMP */

void print_acs_ramdump_para(struct ram_compress_full *rcf)
{
#ifdef CONFIG_ENABLE_DUMP
	dump_info((unsigned long)rcf, sizeof(struct ram_compress_full), "RCF INFO:");
	serial_puts("\n[ACS] mdump rcf ------------\n");
	serial_puts("rcf addr: 0x");
	serial_put_hex((unsigned long)rcf, 32);
	serial_puts(", store_phy_addr: 0x");
	serial_put_hex((unsigned long)rcf->store_phy_addr, 64);
	serial_puts(", full_memsize: 0x");
	serial_put_hex((unsigned long)rcf->full_memsize, 64);
	serial_puts(", section_count: ");
	serial_put_dec((unsigned int)rcf->section_count);
	serial_puts("\n\n");
#endif /* CONFIG_ENABLE_DUMP */
}

unsigned int get_reboot_mode(void)
{
	//uint32_t reboot_mode_val = ((readl(AO_RTI_STATUS_REG3)) & 0xf);
	//serial_puts("REBOOT MODE REG: 0x");
	//serial_put_hex((unsigned long)AO_SEC_SD_CFG15, 32);
	//serial_puts("\n");
	uint32_t reboot_mode_val = ((readl(AO_SEC_SD_CFG15) >> 12) & 0xf);
	return reboot_mode_val;
}

int check_reboot_reason(unsigned long *mem_size)
{
	int reboot_reason;
	unsigned long cfg;

	/* dump when kernel panic or watch dog */
	reboot_reason = get_reboot_mode();
	serial_puts("reboot reason: 0x");
	serial_put_hex(reboot_reason, 8);
	serial_puts("\n");

	/*
	 * using sticky register to save memory config and boot status in kernel
	 */
	cfg = readl(PREG_STICKY_REG8);
	serial_puts("CFG:0x");
	serial_put_hex(cfg, 32);
	serial_puts("\n");

	if (!(cfg & AMLOGIC_KERNEL_BOOTED))
		return 0;

	unsigned long nTotal = cfg & AMLOGIC_USERAM_MASK;
	*mem_size = (((nTotal - 1) / 512 + 1) * 512) << 20;
	serial_puts("mem size:0x");
	serial_put_hex(*mem_size, 64);
	serial_puts("\n");

	switch (reboot_reason) {
	case AMLOGIC_KERNEL_PANIC:
	case AMLOGIC_WATCHDOG_REBOOT:
		break;
	default:
		reboot_reason = 0;
		break;
	}
	return reboot_reason;
}

void save_ramp_dump(unsigned long addr, long size)
{
	unsigned long addr_tmp, size_tmp;

	if (size > 0) {
		if (addr >= REG_SPACE_END_ADDR) {
			/* real size = size[31:0] + addr[6:0]<<32, size should be less than 128G. */
			size_tmp = size & 0xffffffff;
			/* real addr = addr[31:8]<<8, addr should be aligned with 256B at least. */
			addr_tmp = ((addr >> 8) & 0xffffff00) | REG_BIG_ADDR_FLAG | (size >> 32);
		} else {
			/* if addr < 4G, then size < 4G */
			addr_tmp = addr;
			size_tmp = size;
		}
		serial_puts("\n==> RAM COMPRESS OK, ADDR:0x");
		serial_put_hex(addr, 64);
		serial_puts(", SIZE:0x");
		serial_put_hex(size, 64);
		serial_puts("\n");
	} else {
		addr_tmp = 0;
		size_tmp = 0;
		serial_puts("\n==> RAM COMPRESS Failed, set 0.");
	}

	writel(addr_tmp, P_AO_SEC_GP_CFG12);
	writel(size_tmp, P_AO_SEC_GP_CFG13);
}

static struct ram_compress_work_info rcwi = {
	.progress_update      = NULL,
	.hear_beat            = NULL,
	.temp_work_buffer     = (void *)CONFIG_RAMDUMP_TMPWORK_ADDR,
	.temp_output_buffer   = (void *)CONFIG_RAMDUMP_TMPOUT_ADDR,
	.work_buffer_size     = CONFIG_RAMDUMP_TMPWORK_SIZE,
	.output_buffer_size   = CONFIG_RAMDUMP_TMPOUT_SIZE,
	.max_zip_section_size = 32 * SZ_1M,
};

static void fix_up_ddr_size(struct ram_compress_full *rcf,
			    unsigned long size)
{

	if (size < CONFIG_DDR_SIZE_RANGE1) {
		/* 0 ~ 3.5G, use 5 sections */
		serial_puts("ddr size range 1: 0 ~ 3.5G\n");
		rcf->store_phy_addr = (void *)CONFIG_SMALL_COMPRESS_ADDR;
		rcf->full_memsize   = size;
		rcf->sections[0].section_size = size - CONFIG_SMALL_COMPRESS_ADDR;
		rcf->sections[5].section_size = 0;
		rcf->sections[6].section_size = 0;
	} else if (size < CONFIG_DDR_SIZE_RANGE3) {
		/* 3.5G ~ 7.0G, use 7 sections */
		serial_puts("ddr size range 2: 3.5 ~ 7.0G\n");
		rcf->store_phy_addr = (void *)CONFIG_SMALL_COMPRESS_ADDR;
		rcf->full_memsize   = size + REG_SPACE_RANGE_SIZE;
		rcf->sections[0].section_size = REG_SPACE_START_ADDR - CONFIG_SMALL_COMPRESS_ADDR;
		rcf->sections[5].section_size = REG_SPACE_RANGE_SIZE;
		rcf->sections[6].section_size = size - CONFIG_BIG_COMPRESS_ADDR;
	} else {
		/* > 7.0G ~, use 7 sections */
		serial_puts("ddr size range 3: 7.0G ~\n");
		rcf->store_phy_addr = (void *)CONFIG_BIG_COMPRESS_ADDR;
		rcf->full_memsize   = size + REG_SPACE_RANGE_SIZE;
		rcf->sections[0].section_size = REG_SPACE_START_ADDR - CONFIG_BIG_COMPRESS_ADDR;
		rcf->sections[5].section_size = REG_SPACE_RANGE_SIZE;
		rcf->sections[6].section_size = size - CONFIG_BIG_COMPRESS_ADDR;
	}

	if (debug_enable) {
		serial_puts("Fix up store_phy_addr:0x");
		serial_put_hex((unsigned long)rcf->store_phy_addr, 64);
		serial_puts(", first segment size:0x");
		serial_put_hex(rcf->sections[0].section_size, 64);
		serial_puts("\n");
	}
}

static void find_adj(struct ram_compress_full     *rcf,
		     struct ram_compress_section **adj_seg,
		     struct ram_compress_section **first_seg)
{
	int i;
	unsigned long start_addr;
	unsigned long end_addr;
	unsigned long tmp;

	start_addr = (unsigned long)rcf->store_phy_addr;
	for (i = 0; i < rcf->section_count; i++) {
		tmp      = (unsigned long)rcf->sections[i].phy_addr;
		end_addr = rcf->sections[i].section_size + tmp;
		if (tmp  == start_addr) {
			*first_seg = &rcf->sections[i];
			if (debug_enable) {
				serial_puts("First seg:0x");
				serial_put_hex((unsigned long)*first_seg, 32);
				serial_puts("\n");
			}
		}
		if (end_addr == start_addr) {
			*adj_seg = &rcf->sections[i];
			if (debug_enable) {
				serial_puts("Adj seg:0x");
				serial_put_hex((unsigned long)*adj_seg, 32);
				serial_puts("\n");
			}
		}
	}
}

static void try_other_addr(struct ram_compress_full    *rcf,
			   struct ram_compress_section *adj_seg,
			   struct ram_compress_section *first_seg,
			   int retry)
{
	unsigned long start_addr;

	start_addr = (unsigned long)rcf->store_phy_addr;
	start_addr += RETRY_STEP;

	rcf->store_phy_addr = (void *)start_addr;
	first_seg->phy_addr = (void *)start_addr;
	first_seg->section_size -= RETRY_STEP;
	adj_seg->section_size   += RETRY_STEP;

	if (debug_enable) {
		serial_puts("Try other start addr:0x");
		serial_put_hex(start_addr, 32);
		serial_puts(", retry:0x");
		serial_put_hex(retry, 8);
		serial_puts("\n");
	}
}

unsigned long ramdump_compress_all(struct ram_compress_full *rcf,
			 void *arg,
			 unsigned long flag,
			 unsigned long ctrl,
			 unsigned long size)
{
	unsigned long ret = 0, retry = 0;

	unsigned long rc = 0;
	struct ram_compress_section *adj_sec   = NULL;
	struct ram_compress_section *first_sec = NULL;

	if (debug_enable) {
		serial_puts("\n[RAM DUMP], DDR SIZE:0x");
		serial_put_hex(size, 64);
		serial_puts("\n");
	}
	/* these parameters are not configured by bl33 */
	serial_puts("rcf->full_memsize: 0x");
	serial_put_hex((unsigned long)(rcf->full_memsize), 64);
	serial_puts("\n");
	/* update rcf with size */
	if (size && size != rcf->full_memsize) {
		serial_puts("fix_up_ddr_size.\n");
		fix_up_ddr_size(rcf, size);
	}
	serial_puts("find_adj.\n");
	find_adj(rcf, &adj_sec, &first_sec);
	while (retry < MAX_RETRY) {
		serial_puts("run ram_compress_all, retry: 0x");
		serial_put_hex((unsigned int)retry, 32);
		serial_puts("\n");
		rc = ram_compress_all(rcf, &rcwi);
		if (rc != NEED_RETRY)
			break;
		if (!first_sec || !adj_sec)
			break;
		serial_puts("try_other_addr.\n");
		try_other_addr(rcf, adj_sec, first_sec, retry);
		retry++;
	}

	if (rc > 0)
		ret = rc;
	else {
		if (debug_enable) {
			serial_puts("compress failed:");
			serial_put_hex(rc, 32);
			serial_puts("\n");
		}
	}

	return ret;
}

/*******************************************************************************
 * The only thing to do in BL2 is to load further images and pass control to
 * BL31. The memory occupied by BL2 will be reclaimed by BL3_x stages. BL2 runs
 * entirely in S-EL1. Since arm standard c libraries are not PIC, printf et al
 * are not available. We rely on assertions to signal error conditions
 ******************************************************************************/
void aml_ramdump_compress(struct ram_compress_full *rcf,
	       void *arg, unsigned long flag, unsigned long ctrl)
{
	unsigned long ret = 0;
	unsigned long size = 0;

	ret = check_reboot_reason(&size); /* AMLOGIC_WATCHDOG_REBOOT && AMLOGIC_KERNEL_PANIC */
	if ((ret != 0) && size) {
		watchdog_disable();
		serial_puts("aml log : watch dog is disabled!\n");
		serial_puts("STACK ADDR:0x");
		serial_put_hex(PLATFORM_STACK_POSI, 32);
		serial_puts("\n");

		/* show args */
		print_acs_ramdump_para(rcf);
		if (rcf->section_count != CONFIG_COMPRESS_SECTION) {
			serial_puts("rcf error! exit!\n");
			return;
		}

		serial_puts("timer start.\n");
		timer_start();
		enable_caches(size);

		if (0) {
			serial_puts("compress start, get SCTLR_EL2:0x");
			serial_put_hex((unsigned long)__asm_get_sctlr(), 32);
			serial_puts("\n");
		}

		ret = ramdump_compress_all(rcf, arg, flag, ctrl, size);
		save_ramp_dump((unsigned long)rcf->store_phy_addr, ret);

		disable_caches();
		enable_icache();
		timer_end("\nMEM DUMP USED");

		if (0) {
			serial_puts("\ncompress end, get SCTLR_EL2:0x");
			serial_put_hex((unsigned long)__asm_get_sctlr(), 32);
			serial_puts("\n");
		}
	} else {
		serial_puts("normal boot.\n");
	}
}