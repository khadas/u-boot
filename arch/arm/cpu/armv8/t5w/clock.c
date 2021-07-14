/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/t5d/clock.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/clock.h>
#include <asm/arch/mailbox.h>
#include <asm/cpu_id.h>
#include <asm/arch/bl31_apis.h>

/*  !!must use nonzero value as default value. Otherwise it linked to bss segment,
    but this segment not cleared to zero while running "board_init_f" */
#define CLK_UNKNOWN (0xffffffff)

#define RING_PWM_VCCK 	(0xffd1b000 + (0x0001 << 2))
#define RING_PWM_EE	(0xffd1b000 + (0x0000 << 2))

#define T5_MSR_CLK_REG0		((0x6001 << 2) + 0xffd00000)
#define T5_P_MSR_CLK_REG0	(volatile unsigned int *)((0x6001 << 2) + 0xffd00000)
#define T5_MSR_CLK_REG2		((0x6003 << 2) + 0xffd00000)
#define T5_P_MSR_CLK_REG2	(volatile unsigned int *)((0x6003 << 2) + 0xffd00000)

#if 0
__u32 get_rate_xtal(void)
{
	unsigned long clk;
	clk = (readl(P_PREG_CTLREG0_ADDR) >> 4) & 0x3f;
	clk = clk * 1000 * 1000;
	return clk;
}


__u32 get_cpu_clk(void)
{
    static __u32 sys_freq=CLK_UNKNOWN;
    if (sys_freq == CLK_UNKNOWN)
    {
        sys_freq=(clk_util_clk_msr(SYS_PLL_CLK)*1000000);
    }
    return sys_freq;
}

__u32 get_clk_ddr(void)
{
   static __u32 freq=CLK_UNKNOWN;
	if (freq == CLK_UNKNOWN)
	{
	    freq=(clk_util_clk_msr(DDR_PLL_CLK)*1000000);
	}
    return freq;
}

__u32 get_clk_ethernet_pad(void)
{
    static __u32 freq=CLK_UNKNOWN;
    if (freq == CLK_UNKNOWN)
    {
        freq=(clk_util_clk_msr(MOD_ETH_CLK50_I)*1000000);
    }
    return freq;
}

__u32 get_clk_cts_eth_rmii(void)
{
    static __u32 freq=CLK_UNKNOWN;
    if (freq == CLK_UNKNOWN)
    {
        freq=(clk_util_clk_msr(CTS_ETH_RMII)*1000000);
    }
    return freq;
}

__u32 get_misc_pll_clk(void)
{
    static __u32 freq=CLK_UNKNOWN;
    if (freq == CLK_UNKNOWN)
    {
        freq=(clk_util_clk_msr(MISC_PLL_CLK)*1000000);
    }
    return freq;
}
#endif

__u32 get_clk81(void)
{
    static __u32 clk81_freq=CLK_UNKNOWN;
	if (clk81_freq == CLK_UNKNOWN)
    {
        clk81_freq=(clk_util_clk_msr(CLK81)*1000000);
    }
    return clk81_freq;
}

struct __clk_rate{
    unsigned clksrc;
    __u32 (*get_rate)(void);
};

struct __clk_rate clkrate[]={
    {
        .clksrc=CLK81,
        .get_rate=get_clk81,
    },
};

int clk_get_rate(unsigned clksrc)
{
	int i;
	for (i = 0; i < sizeof(clkrate)/sizeof(clkrate[0]); i++)
	{
		if (clksrc == clkrate[i].clksrc)
			return clkrate[i].get_rate();
	}
	return -1;
}

static const char * const t5_table[] = {
	[210] = "am_ring_osc_ee24",
	[209] = "am_ring_osc_ee23",
	[208] = "am_ring_osc_ee22",
	[207] = "am_ring_osc_ee21",
	[206] = "am_ring_osc_ee20",
	[205] = "am_ring_osc_ee19",
	[204] = "am_ring_osc_ee18",
	[203] = "am_ring_osc_ee17",
	[202] = "am_ring_osc_ee16",
	[201] = "am_ring_osc_ee15",
	[200] = "am_ring_osc_ee14",
	[199] = "am_ring_osc_ee13",
	[198] = "am_ring_osc_ee12",
	[197] = "sys_cpu_ring_osc27",
	[196] = "sys_cpu_ring_osc26",
	[195] = "sys_cpu_ring_osc25",
	[194] = "sys_cpu_ring_osc24",
	[193] = "sys_cpu_ring_osc23",
	[192] = "sys_cpu_ring_osc22",
	[191] = "sys_cpu_ring_osc21",
	[190] = "sys_cpu_ring_osc20",
	[189] = "sys_cpu_ring_osc19",
	[188] = "sys_cpu_ring_osc18",
	[187] = "sys_cpu_ring_osc17",
	[186] = "sys_cpu_ring_osc16",
	[185] = "sys_cpu_ring_osc15",
	[184] = "sys_cpu_ring_osc14",
	[183] = "sys_cpu_ring_osc13",
	[182] = "sys_cpu_ring_osc12",
	[181] = "sys_cpu_ring_osc11",
	[180] = "sys_cpu_ring_osc10",
	[179] = "sys_cpu_ring_osc9",
	[178] = "sys_cpu_ring_osc8",
	[177] = "sys_cpu_ring_osc7",
	[176] = "sys_cpu_ring_osc6",
	[175] = "sys_cpu_ring_osc5",
	[174] = "sys_cpu_ring_osc4",
	[173] = "adc_lr_outrdy",
	[172] = "atv_dmd_mono_out",
	[167] = "au_dac1l_en_dac_clk",
	[166] = "au_dac1r_en_dac_clk",
	[163] = "hdmirx_aud_sck",
	[157] = "vpu_dmc_clk",
	[156] = "p22_usb2_clkout",
	[155] = "p21_usb2_clkout",
	[154] = "p20_usb2_clkout",
	[153] = "c_alocker_out_clk",
	[152] = "c_alocker_in_clk",
	[151] = "dpll_intclk",
	[150] = "dpll_clk_a2",
	[149] = "dpll_clk_b2",
	[148] = "dpll_clk_b3",
	[144] = "ts_pll_clk",
	[143] = "mainclk",
	[142] = "demode_ts_clk",
	[140] = "audio_toacodec_bclk",
	[139] = "aud_adc_clk_g128x",
	[137] = "atv_dmd_i2c_sclk",
	[135] = "tvfe_sample_clk",
	[134] = "adc_extclk_in",
	[133] = "atv_dmd_mono_clk_32",
	[132] = "audio_toacode_mclk",
	[129] = "lvds_fifo_clk",
	[128] = "cts_tcon_pll_clk",
	[127] = "hdmirx_vid_clk",
	[125] = "cts_hdmi_axi_clk",
	[124] =	"cts_demod_core_clk",
	[123] =	"mod_audio_pdm_dclk_o",
	[122] =	"audio_spdifin_mst_clk",
	[121] = "audio_spdifout_mst_clk",
	[120] = "audio_spdifout_b_mst_clk",
	[119] = "audio_pdm_sysclk",
	[118] = "audio_resamplea_clk",
	[117] = "audio_resampleb_clk",
	[116] = "audio_tdmin_a_sclk",
	[115] = "audio_tdmin_b_sclk",
	[114] = "audio_tdmin_c_sclk",
	[113] = "audio_tdmin_lb_sclk",
	[112] = "audio_tdmout_a_sclk",
	[111] = "audio_tdmout_b_sclk",
	[110] = "audio_tdmout_c_sclk",
	[109] = "o_vad_clk",
	[108] = "acodec_i2sout_bclk",
	[107] = "au_dac_clk_g128x",
	[106] = "ephy_test_clk",
	[105] = "am_ring_osc_clk_out_ee[9]",
	[104] = "am_ring_osc_clk_out_ee[8]",
	[103] = "am_ring_osc_clk_out_ee[7]",
	[102] = "am_ring_osc_clk_out_ee[6]",
	[101] = "am_ring_osc_clk_out_ee[5]",
	[100] = "am_ring_osc_clk_out_ee[4]",
	[99] = "am_ring_osc_clk_out_ee[3]",
	[98] = "cts_ts_clk",
	[97] = "cts_vpu_clkb_tmp",
	[96] = "cts_vpu_clkb",
	[95] = "eth_phy_plltxclk",
	[94] = "eth_phy_exclk",
	[93] = "sys_cpu_ring_osc_clk[3]",
	[92] = "sys_cpu_ring_osc_clk[2]",
	[91] = "hdmirx_audmeas_clk",
	[90] = "am_ring_osc_clk_out_ee[11]",
	[89] = "am_ring_osc_clk_out_ee[10]",
	[88] = "cts_hdmirx_meter_clk",
	[86] = "cts_hdmirx_modet_clk",
	[85] = "cts_hdmirx_acr_ref_clk",
	[84] = "co_tx_cl",
	[83] = "co_rx_clk",
	[82] = "cts_ge2d_clk",
	[81] = "cts_vapbclk",
	[80] = "rng_ring_osc_clk[3]",
	[79] = "rng_ring_osc_clk[2]",
	[78] = "rng_ring_osc_clk[1]",
	[77] = "rng_ring_osc_clk[0]",
	[76] = "hdmix_aud_clk",
	[75] = "cts_hevcf_clk",
	[74] = "hdmirx_aud_pll_clk",
	[73] = "cts_pwm_C_clk",
	[72] = "cts_pwm_D_clk",
	[71] = "cts_pwm_E_clk",
	[70] = "cts_pwm_F_clk",
	[69] = "cts_hdcp22_skpclk",
	[68] = "cts_hdcp22_esmclk",
	[67] = "hdmirx_apll_clk_audio",
	[66] = "cts_vid_lock_clk",
	[65] = "cts_spicc_0_clk",
	[63] = "hdmirx_tmds_clk",
	[61] = "gpio_clk_msr",
	[60] = "cts_hdmirx_aud_pll_clk",
	[58] = "cts_vafe_datack",
	[57] = "cts_atv_dmd_vdac_clk",
	[56] = "cts_atv_dmd_sys_clk",
	[55] = "vid_pll_div_clk_out",
	[54] = "cts_vpu_clkc",
	[53] = "ddr_2xclk",
	[51] = "cts_sd_emmc_clk_C",
	[50] = "mp3_clk_out",
	[49] = "mp2_clk_out",
	[48] = "mp1_clk_out",
	[47] = "ddr_dpll_pt_clk",
	[46] = "cts_vpu_clk",
	[45] = "cts_pwm_A_clk",
	[44] = "cts_pwm_B_clk",
	[43] = "fclk_div5",
	[42] = "mp0_clk_out",
	[41] = "mac_eth_rx_clk_rmii",
	[40] = "cts_hdmirx_cfg_clk",
	[38] = "cts_vdin_meas_clk",
	[37] = "cts_cdac_clk_c",
	[35] = "cts_mali_clk",
	[34] = "eth_mppll_50m_ckout",
	[33] = "sys_cpu_ring_osc_clk[1]",
	[32] = "cts_vdec_clk",
	[31] = "mpll_clk_test_out",
	[30] = "hdmirx_cable_clk",
	[29] = "hdmirx_apll_clk_out_div",
	[28] = "cts_sar_adc_clk",
	[27] = "co_clkin_to_mac",
	[25] = "cts_eth_clk_rmii",
	[24] = "cts_eth_clk125Mhz",
	[23] = "mpll_clk_50m",
	[22] = "mac_eth_phy_ref_clk",
	[21] = "lcd_an_clk_ph3",
	[20] = "rtc_osc_clk_out",
	[19] = "lcd_an_clk_ph2",
	[18] = "sys_cpu_clk_div16",
	[17] = "sys_pll_div16",
	[16] = "cts_FEC_CLK_2",
	[15] = "cts_FEC_CLK_1",
	[14] = "cts_FEC_CLK_0",
	[13] = "mod_tcon_clko",
	[12] = "hifi_pll_clk",
	[11] = "mac_eth_tx_clk",
	[10] = "cts_vdac_clk",
	[9] = "cts_encl_clk",
	[8] = "cts_encp_clk",
	[7] = "clk81",
	[6] = "cts_enci_clk",
	[4] = "gp0_pll_clk",
	[3] = "sys_cpu_ring_osc_clk[0]",
	[2] = "am_ring_osc_clk_out_ee[2]",
	[1] = "am_ring_osc_clk_out_ee[1]",
	[0] = "am_ring_osc_clk_out_ee[0]",
};

unsigned long clk_util_ring_msr(unsigned long clk_mux)
{
	unsigned int regval = 0;

	writel(0, T5_MSR_CLK_REG0);
	/* Set the measurement gate to 64uS */
	clrbits_le32(T5_P_MSR_CLK_REG0, 0xffff);
	/* 64uS is enough for measure the frequence? */
	setbits_le32(T5_P_MSR_CLK_REG0, 10000 - 1);
	/* Disable continuous measurement */
	/* Disable interrupts */
	clrbits_le32(T5_P_MSR_CLK_REG0, 1 << 18 | 1 << 17);
	clrbits_le32(T5_P_MSR_CLK_REG0, 0x7f << 20);
	setbits_le32(T5_P_MSR_CLK_REG0, clk_mux << 20 | /*Select MUX*/
				     1 << 19 |	     /* enable the clock */
				     1 << 16);	     /* enable measuring */
	/* Wait for the measurement to be done */
	regval = readl(T5_MSR_CLK_REG0);
	do {
		regval = readl(T5_MSR_CLK_REG0);
	} while (regval & (1 << 31));

	/* Disable measuring */
	clrbits_le32(T5_P_MSR_CLK_REG0, (1 << 16));
	regval = (readl(T5_MSR_CLK_REG2) + 31) & 0x000FFFFF;

	return (regval / 10);
}

unsigned long clk_util_clk_msr(unsigned long clk_mux)
{
	unsigned int regval = 0;

	writel(0, T5_MSR_CLK_REG0);
	/* Set the measurement gate to 64uS */
	clrbits_le32(T5_P_MSR_CLK_REG0, 0xffff);
	/* 64uS is enough for measure the frequence? */
	setbits_le32(T5_P_MSR_CLK_REG0, 64 - 1);
	/* Disable continuous measurement */
	/* Disable interrupts */
	clrbits_le32(T5_P_MSR_CLK_REG0, 1 << 18 | 1 << 17);
	clrbits_le32(T5_P_MSR_CLK_REG0, 0x7f << 20);
	setbits_le32(T5_P_MSR_CLK_REG0, clk_mux << 20 | /*Select MUX*/
				     1 << 19 |	     /* enable the clock */
				     1 << 16);	     /* enable measuring */
	/* Wait for the measurement to be done */
	regval = readl(T5_MSR_CLK_REG0);
	do {
		regval = readl(T5_MSR_CLK_REG0);
	} while (regval & (1 << 31));

	/* Disable measuring */
	clrbits_le32(T5_P_MSR_CLK_REG0, (1 << 16));
	regval = (readl(T5_MSR_CLK_REG2) + 31) & 0x000FFFFF;

	return (regval >> 6);
}

int clk_msr(int index)
{
	unsigned int index_total = 0;
	int i;

	index_total = sizeof(t5_table) / sizeof(char *);
	if (index == 0xff) {
		for (i = 0; i < index_total; i++)
			printf("[%4d][%4ld MHz] %s\n", i, clk_util_clk_msr(i), t5_table[i]);
	}
	else {
		if (index >= index_total) {
			printf("clk msr legal range: [0-%d]\n", index_total-1);
			return -1;
		}
		printf("[%4d][%4ld MHz] %s\n", index, clk_util_clk_msr(index),  t5_table[index]);
	}

	return 0;
}

void ring_powerinit(void)
{
	writel(0x0018000A, RING_PWM_VCCK);/*set vcck 0.8v*/
	writel(0x000d0005, RING_PWM_EE);/*set ee 0.8v*/
}

#ifdef CONFIG_RING
int ring_msr(int index)
{
	const char* clk_table[] = {
			[56] = "am_ring_osc_clk_out_ee[24] " ,
			[55] = "am_ring_osc_clk_out_ee[23] " ,
			[54] = "am_ring_osc_clk_out_ee[22] " ,
			[53] = "am_ring_osc_clk_out_ee[21] " ,
			[52] = "am_ring_osc_clk_out_ee[20] " ,
			[51] = "am_ring_osc_clk_out_ee[19] " ,
			[50] = "am_ring_osc_clk_out_ee[18] " ,
			[49] = "am_ring_osc_clk_out_ee[17] " ,
			[48] = "am_ring_osc_clk_out_ee[16] " ,
			[47] = "am_ring_osc_clk_out_ee[15] " ,
			[46] = "am_ring_osc_clk_out_ee[14] " ,
			[45] = "am_ring_osc_clk_out_ee[13] " ,
			[44] = "am_ring_osc_clk_out_ee[12] " ,
			[43] = "sys_cpu_ring_osc_clk[27] " ,
			[42] = "sys_cpu_ring_osc_clk[26] " ,
			[41] = "sys_cpu_ring_osc_clk[25] " ,
			[40] = "sys_cpu_ring_osc_clk[24] " ,
			[39] = "sys_cpu_ring_osc_clk[23] " ,
			[38] = "sys_cpu_ring_osc_clk[22] " ,
			[37] = "sys_cpu_ring_osc_clk[21] " ,
			[36] = "sys_cpu_ring_osc_clk[20] " ,
			[35] = "sys_cpu_ring_osc_clk[19] " ,
			[34] = "sys_cpu_ring_osc_clk[18] " ,
			[33] = "sys_cpu_ring_osc_clk[17] " ,
			[32] = "sys_cpu_ring_osc_clk[16] " ,
			[31] = "sys_cpu_ring_osc_clk[15] " ,
			[30] = "sys_cpu_ring_osc_clk[14] " ,
			[29] = "sys_cpu_ring_osc_clk[13] " ,
			[28] = "sys_cpu_ring_osc_clk[12] " ,
			[27] = "sys_cpu_ring_osc_clk[11] " ,
			[26] = "sys_cpu_ring_osc_clk[10] " ,
			[25] = "sys_cpu_ring_osc_clk[9] " ,
			[24] = "sys_cpu_ring_osc_clk[8] " ,
			[23] = "sys_cpu_ring_osc_clk[7] " ,
			[22] = "sys_cpu_ring_osc_clk[6] " ,
			[21] = "sys_cpu_ring_osc_clk[5] " ,
			[20] = "sys_cpu_ring_osc_clk[4] " ,
			[19] = "am_ring_osc_clk_out_ee[9] " ,
			[18] = "am_ring_osc_clk_out_ee[8] " ,
			[17] = "am_ring_osc_clk_out_ee[7] " ,
			[16] = "am_ring_osc_clk_out_ee[6] " ,
			[15] = "am_ring_osc_clk_out_ee[5] " ,
			[14] = "am_ring_osc_clk_out_ee[4] " ,
			[13] = "am_ring_osc_clk_out_ee[3] " ,
			[12] = "sys_cpu_ring_osc_clk[3] " ,
			[11] = "sys_cpu_ring_osc_clk[2] " ,
			[10] = "am_ring_osc_clk_out_ee[11] " ,
			[9] = "am_ring_osc_clk_out_ee[10] " ,
			[8] = "rng_ring_osc_clk[3] " ,
			[7] = "rng_ring_osc_clk[2] " ,
			[6] = "rng_ring_osc_clk[1] " ,
			[5] = "rng_ring_osc_clk[0] " ,
			[4] = "sys_cpu_ring_osc_clk[1] " ,
			[3] = "sys_cpu_ring_osc_clk[0] " ,
			[2] = "am_ring_osc_clk_out_ee[2] " ,
			[1] = "am_ring_osc_clk_out_ee[1] " ,
			[0] = "am_ring_osc_clk_out_ee[0] " ,
		};
	const int tb[] = {0, 1, 2, 3, 33, 77, 78, 79, 80, 89, 90, 92, 93, 99, 100, 101, 102, 103,
			  104,105, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 184,
			  186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
			  200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210};
	unsigned long i;
	unsigned char efuseinfo[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	if ((index != 0xff) && (index != 0)) {
		if (bl31_get_cornerinfo(efuseinfo, sizeof(efuseinfo) / sizeof(uint8_t)) != 0) {
			printf("fail get efuse info\n");
			return 0;
		}
		if ((index >= 1) && (index <= 4))
			printf("%d\n", (efuseinfo[index] * 20));
		printf("%d\n", (efuseinfo[5] * 400));
		printf("%d\n", efuseinfo[6]);
		printf("%d\n", (efuseinfo[7] * 400));
		if ((index >= 8) && (index <= 11))
			printf("%d\n", efuseinfo[index]);
		if ((index < 0) || (index >= 12))
			printf("input data not support!\n");
		return 0;
	}
	ring_powerinit();
	/*RING_OSCILLATOR       0x7f: set slow ring*/
	writel(0x155, 0xff6345fc);
	writel(0x01555555, 0xff634600);
	for (i = 0; i < 57; i++) {
		printf("%s      :",clk_table[i]);
		printf("%ld     KHz",clk_util_ring_msr(tb[i]));
		printf("\n");
	}

	if (bl31_get_cornerinfo(efuseinfo, sizeof(efuseinfo) / sizeof(uint8_t)) != 0) {
		printf("fail get osc ring efuse info\n");
		return 0;
	}

	printf("osc ring efuse info:\n");
	for (i = 0; i <= 3; i++)
		printf("0x%x, ", efuseinfo[i]);
	printf("\n");

	/*efuse to test value*/

	printf("sys_cpu_osc_ring0(ULVT16)(KHz), IDD CPU(uA), IDD EE(uA)\n");
	printf("%d KHz, ", (efuseinfo[1] * 20));
	printf("%d uA, ", (efuseinfo[2] * 400));
	printf("%d uA, ", (efuseinfo[3] * 400));

	printf("\n");

	return 0;
}
#endif

#ifdef CONFIG_AML_SPICC
/* generic clock control for spicc1.
 * if deleted, you have to add it into all t5 board files as necessary.
 */
static int spicc_clk_rate[] = {
	24000000,	/* XTAL */
	166666666,	/* CLK81 */
	500000000,	/* FCLK_DIV4 */
	666666666,	/* FCLK_DIV3 */
	1000000000,	/* FCLK_DIV2 */
	400000000,	/* FCLK_DIV5 */
	285700000	/* FCLK_DIV7 */
};

static int spicc_clk_set_rate(int id, int rate)
{
	u32 regv;
	u8 mux, div = 0;
	u8 shift = (id == 0) ? 0 : 16;

	for (mux = 0; mux < ARRAY_SIZE(spicc_clk_rate); mux++)
		if (rate == spicc_clk_rate[mux])
			break;
	if (mux == ARRAY_SIZE(spicc_clk_rate))
		return -EINVAL;

	regv = readl(P_HHI_SPICC_CLK_CNTL);
	regv &= ~ (0x3ff << shift);
	regv |= div << (0 + shift);
	regv |= 1 << (6 + shift);
	regv |= mux << (7 + shift);
	writel(regv, P_HHI_SPICC_CLK_CNTL);

	return 0;
}

int spicc0_clk_set_rate(int rate)
{
	return spicc_clk_set_rate(0, rate);
}

static int spicc_clk_enable(int id, bool enable)
{
	u32 regv;
	u8 shift = (id == 0) ? 8: 14;

	regv = readl(P_HHI_GCLK_MPEG0);
	if (enable)
		regv |= 1 << shift;
	else
		regv &= ~(1 << shift);
	writel(regv, P_HHI_GCLK_MPEG0);

	return 0;
}

int spicc0_clk_enable(bool enable)
{
	return spicc_clk_enable(0, enable);
}
#endif /* CONFIG_AML_SPICC */
