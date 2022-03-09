/*
 * arch/arm/cpu/armv8/gx12b/sound.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the named License,
 * or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>
#include <common.h>
#include <amlogic/auge_sound.h>


struct aiu_958_channel_status {
	unsigned short chstat0_l;
	unsigned short chstat1_l;
	unsigned short chstat0_r;
	unsigned short chstat1_r;
};

static int audiobus_read(unsigned long reg, unsigned int *val)
{
	*val = readl(reg);

	return 0;
}

static void audiobus_write(unsigned long reg, unsigned int val)
{
	writel(val, reg);
}

static int audiobus_update_bits(unsigned int reg, unsigned int mask, unsigned int value)
{
	bool change;
	unsigned int old, new;

	audiobus_read(reg, &old);

	new = (old & ~mask) | (value & mask);
	change = old != new;
	if (change)
		audiobus_write(reg, new);

	return change;
}

static void aml_set_audio_spdif_clk(void)
{
	/*mpll0: 25m*/
	audiobus_write(HHI_MPLL_CNTL0, 0x543);
	audiobus_write(HHI_MPLL_CNTL1, 0xC5101856);
	audiobus_write(HHI_MPLL_CNTL2, 0x40000033);

	/* audio clk gate */
	audiobus_write(EE_AUDIO_CLK_GATE_EN,
		       1 << 17 /* spdifout */
		       | 1 << 9 /* frddra */
		       | 1 << 0 /* ddr_arb */);

	/*SPDIFOUT_CTRL clk:6m*/
	audiobus_write(EE_AUDIO_CLK_SPDIFOUT_CTRL,
		       1 << 31    /* enable */
		       | 0 << 24 /* mpll0 */
		       | 3 << 0  /* dividor */);
}

static void aml_spdif_fifo_ctrl(void)
{
	/* reg mute val */
	audiobus_write(EE_AUDIO_SPDIFOUT_MUTE_VAL, 0x0);

	/* mask lane0 L/R, lsb first,  insert data from 31bits */
	audiobus_update_bits(EE_AUDIO_SPDIFOUT_CTRL0,
			     0x1<<20 | 0x1<<19 | 0xff << 4,
			     0x0<<20 | 0x0<<19 | 0x3 << 4);

	/* split 64bits ddr data to 2 sample */
	/* msb position of data */
	/* frddr_A */
	audiobus_update_bits(EE_AUDIO_SPDIFOUT_CTRL1,
			     0x3 << 24 | 0x1f << 8 | 0x7 << 4,
			     0 << 24 | (32 - 1) << 8 | 3<<4);

	audiobus_write(EE_AUDIO_SPDIFOUT_SWAP, 0x1 << 4);
}

static void spdifout_set_pcm_chsts(struct aiu_958_channel_status *set)
{
	if (!set) {
		printf("spdifout_set_pcm_chsts, error set NULL point\n");
		return;
	}

	audiobus_write(EE_AUDIO_SPDIFOUT_CHSTS0, set->chstat1_l << 16 | set->chstat0_l);
	audiobus_write(EE_AUDIO_SPDIFOUT_CHSTS6, set->chstat1_r << 16 | set->chstat0_r);
}

void frddr_init_without_mngr(void)
{
	unsigned int start_addr, end_addr, int_addr;
	static int buf[256];

	memset(buf, 0x0, sizeof(buf));
	start_addr = virt_to_phys(buf);
	end_addr = start_addr + sizeof(buf) - 1;
	int_addr = sizeof(buf) / 64;

	audiobus_write(EE_AUDIO_ARB_CTRL, 0x800000ff);

	audiobus_write(EE_AUDIO_FRDDR_A_START_ADDR, start_addr);

	audiobus_write(EE_AUDIO_FRDDR_A_INIT_ADDR, start_addr);

	audiobus_write(EE_AUDIO_FRDDR_A_FINISH_ADDR, end_addr);

	audiobus_write(EE_AUDIO_FRDDR_A_INT_ADDR, int_addr);

	audiobus_write(EE_AUDIO_FRDDR_A_CTRL1,
		       (0x40 - 1) << 24 | (0x20 - 1) << 16 | 2 << 8 | 0 << 0);

	audiobus_write(EE_AUDIO_FRDDR_A_CTRL0,
		       1 << 31
		       | 0 << 24
		       | 4 << 16
		       | 1 << 3 /* src0 enable */
		       | 3 << 0 /* src0 select spdifout*/
	);
}

static void aml_spdif_enable(void)
{
	/* reset */
	audiobus_update_bits(EE_AUDIO_SPDIFOUT_CTRL0, 3<<28, 0);
	audiobus_update_bits(EE_AUDIO_SPDIFOUT_CTRL0, 1<<29, 1<<29);
	audiobus_update_bits(EE_AUDIO_SPDIFOUT_CTRL0, 1<<28, 1<<28);
	/* enable */
	audiobus_update_bits(EE_AUDIO_SPDIFOUT_CTRL0, 1 << 31, 1 << 31);

	/* tohdmitx enable */
	audiobus_write(EE_AUDIO_TOHDMITX_CTRL0,
		       1 << 31
		       | 1 << 3 /* spdif_clk_cap_inv */
		       | 0 << 2 /* spdif_clk_inv */
		       | 0 << 1 /* spdif_out */
		       | 0 << 0 /* spdif_clk */
	);
}

static void aml_spdif_play(void)
{
	struct aiu_958_channel_status chstat;

	chstat.chstat0_l = 0x0100;
	chstat.chstat0_r = 0x0100;
	chstat.chstat1_l = 0X200;
	chstat.chstat1_r = 0X200;

	aml_spdif_fifo_ctrl();
	spdifout_set_pcm_chsts(&chstat);
	frddr_init_without_mngr();
	aml_spdif_enable();
}

int aml_audio_init(void)
{
	printf("aml_audio_init\n");

	aml_set_audio_spdif_clk();
	aml_spdif_play();

	return 0;
}
