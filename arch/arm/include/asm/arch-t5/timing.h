/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/arch-t5/timing.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __AML_TIMING_H_
#define __AML_TIMING_H_
#include <asm/arch/ddr_define.h>


#define BL2_INIT_STAGE_0			0
#define BL2_INIT_STAGE_1			1
#define BL2_INIT_STAGE_2			2
#define BL2_INIT_STAGE_3			3
#define BL2_INIT_STAGE_4			4
#define BL2_INIT_STAGE_5			5
#define BL2_INIT_STAGE_6			6
#define BL2_INIT_STAGE_7			7
#define BL2_INIT_STAGE_8			8
#define BL2_INIT_STAGE_9			9

#define BL2_INIT_STAGE_PWM_PRE_INIT		0x81
#define BL2_INIT_STAGE_PWM_CHK_HW		0x82
#define BL2_INIT_STAGE_PWM_CFG_GROUP	0x83
#define BL2_INIT_STAGE_PWM_INIT			0xC0
/*bl2 efuse val flag*/
#define BL2_INIT_STAGE_VDDCORE_CONFIG_1		0x84
#define BL2_INIT_STAGE_VDDCORE_CONFIG_2		0x85
#define BL2_INIT_STAGE_VDDCORE_CONFIG_3		0x86

typedef struct bl2_reg {
	unsigned	int		reg;
	unsigned	int		value;
	unsigned	int		mask;
	unsigned	short	udelay;
	unsigned	char	flag;
	unsigned	char	rsv_0;
}__attribute__ ((packed)) bl2_reg_t;

typedef struct ddr_reg {
	unsigned	int		reg;
	unsigned	int		value;
	unsigned	int		mask;
	unsigned	short	udelay;
	unsigned	char	flag;
	unsigned	char	rsv_0;
}__attribute__ ((packed)) ddr_reg_t;

typedef struct training_delay_set_ps{
	unsigned	char	ac_trace_delay[10];
	unsigned	char	ac_trace_delay_rev[2];
	unsigned	char	read_dqs_delay[16];
	unsigned	char	read_dq_bit_delay[72];
	unsigned	short	write_dqs_delay[16];
//	*/
	unsigned	short	write_dq_bit_delay[72];
	unsigned	short	read_dqs_gate_delay[16];
	unsigned	char	soc_bit_vref[36];
	unsigned	char	dram_bit_vref[32];
	///*
	unsigned	char	rever1;//read_dqs  read_dq,write_dqs, write_dq
	unsigned	char	dfi_mrl;
	unsigned	char	dfi_hwtmrl;
	unsigned	char	ARdPtrInitVal;
	unsigned	short	csr_vrefinglobal;
	unsigned	short	csr_dqsrcvcntrl[4];
	unsigned	short	csr_pptdqscntinvtrntg0[4];
	unsigned	short	csr_pptdqscntinvtrntg1[4];
	unsigned	short	csr_seq0bgpr[9];
	unsigned	short	csr_dllgainctl;
	unsigned	short	csr_dlllockpara;
//	unsigned	short	rever2;
}__attribute__ ((packed)) training_delay_set_ps_t;

typedef struct ddr_mrs_reg {
	unsigned	int		cfg_ddr_mr[8];
	unsigned	int		cfg_ddr_mr11;
	unsigned	int		cfg_ddr_mr12;
	unsigned	int		cfg_ddr_mr13;
	unsigned	int		cfg_ddr_mr14;
	unsigned	int		cfg_ddr_mr16;
	unsigned	int		cfg_ddr_mr17;
	unsigned	int		cfg_ddr_mr22;
	unsigned	int		cfg_ddr_mr24;
}__attribute__ ((packed)) ddr_mrs_reg_t;

typedef struct ddr_timing{
	unsigned	int		identifier;
	unsigned	int		cfg_ddr_mrd;
	unsigned	int		cfg_ddr_rfcab;
	unsigned	int		cfg_ddr_rfcpb;
	unsigned	int		cfg_ddr_rpab;
	unsigned	int		cfg_ddr_rppb;
	unsigned	int		cfg_ddr_rtw;
	unsigned	int		cfg_ddr_rl;
	unsigned	int		cfg_ddr_wl;
	unsigned	int		cfg_ddr_ras;
	unsigned	int		cfg_ddr_rc;
	unsigned	int		cfg_ddr_rcd;
	unsigned	int		cfg_ddr_rrds;
	unsigned	int		cfg_ddr_rrdl;
	unsigned	int		cfg_ddr_faw;
	unsigned	int		cfg_ddr_rtp;
	unsigned	int		cfg_ddr_wr;
	unsigned	int		cfg_ddr_wtrs;
	unsigned	int		cfg_ddr_wtrl;
	unsigned	int		cfg_ddr_ccds;
	unsigned	int		cfg_ddr_ccdl;
	unsigned	int		cfg_ddr_exsr;
	unsigned	int		cfg_ddr_xs;
	unsigned	int		cfg_ddr_xp;
	unsigned	int		cfg_ddr_xpdll;
	unsigned	int		cfg_ddr_zqcs;
	unsigned	int		cfg_ddr_cksre;
	unsigned	int		cfg_ddr_cksrx;
	unsigned	int		cfg_ddr_cke;
	unsigned	int		cfg_ddr_mod;
	unsigned	int		cfg_ddr_dqs;
	unsigned	int		cfg_ddr_rstl;
	unsigned	int		cfg_ddr_zqlat;
	unsigned	int		cfg_ddr_mrr;
	unsigned	int		cfg_ddr_ckesr;
	unsigned	int		cfg_ddr_dpd;
	unsigned	int		cfg_ddr_ckeck;
	unsigned	int		cfg_ddr_refi;
	unsigned	int		cfg_ddr_sr;
	unsigned	int		cfg_ddr_ccdmw;
	unsigned	int		cfg_ddr_escke;
	unsigned	int		cfg_ddr_refi_ddr3;
	unsigned	int		cfg_ddr_dfictrldelay;
	unsigned	int		cfg_ddr_dfiphywrdata;
	unsigned	int		cfg_ddr_dfiphywrlat;
	unsigned	int		cfg_ddr_dfiphyrddataen;
	unsigned	int		cfg_ddr_dfiphyrdlat;
	unsigned	int		cfg_ddr_dfictrlupdmin;
	unsigned	int		cfg_ddr_dfictrlupdmax;
	unsigned	int		cfg_ddr_dfimstrresp;
	unsigned	int		cfg_ddr_dfirefmski;
	unsigned	int		cfg_ddr_dfictrlupdi;
	unsigned	int		cfg_ddr_dfidramclk;
	unsigned	int		cfg_ddr_dfilpresp;
	unsigned	int		cfg_ddr_dfiphymstr;
	unsigned	int		cfg_ddr_rtodt;
	unsigned	int		cfg_ddr_wlmrd;
	unsigned	int		cfg_ddr_wlo;
	unsigned	int		cfg_ddr_al;
	unsigned	int		cfg_ddr_zqcl;
	unsigned	int		cfg_ddr_zqcsi;
	unsigned	int		cfg_ddr_zqreset;
	unsigned	int		cfg_ddr_tdqsck_min;
	unsigned	int		cfg_ddr_tdqsck_max;
	//training_delay_set_ps_t	cfg_ddr_training_delay_ps;
	ddr_mrs_reg_t		cfg_ddr_mrs_reg_ps[2];
	unsigned	int		dfi_odt1_config_ps[2];
	//ddr_mrs_reg_t		cfg_ddr_mrs_reg_ps1;
	#if 0
	unsigned	int		cfg_ddr_mr[8];
	unsigned	int		cfg_ddr_mr11;
	unsigned	int		cfg_ddr_mr12;
	unsigned	int		cfg_ddr_mr13;
	unsigned	int		cfg_ddr_mr14;
	unsigned	int		cfg_ddr_mr16;
	unsigned	int		cfg_ddr_mr17;
	unsigned	int		cfg_ddr_mr22;
	unsigned	int		cfg_ddr_mr24;
	unsigned	int		cfg_ddr_reserve[5];
	#endif
}__attribute__ ((packed)) ddr_timing_t;

typedef struct board_common_setting
{
	unsigned	int		timming_magic;
	unsigned	short	timming_max_valid_configs;
	unsigned	short	timming_struct_version;
	unsigned	short	timming_struct_org_size;
	unsigned	short	timming_struct_real_size;
	unsigned	char	fast_boot[4];// 0   fastboot enable  1 window test margin  2 auto offset after window test 3 auto window test enable
	unsigned	int		ddr_func;
	unsigned	char	board_id;
	unsigned	char	DramType;
	unsigned	char	dram_rank_config;
	unsigned	char	DisabledDbyte;
	unsigned	int		dram_cs0_base_add;
	unsigned	int		dram_cs1_base_add;
	unsigned	short	dram_cs0_size_MB;
	unsigned	short	dram_cs1_size_MB;
	unsigned	char	dram_x4x8x16_mode;
	unsigned	char	Is2Ttiming;
	unsigned	char	log_level;
	unsigned	char	ddr_rdbi_wr_enable;
	unsigned	int		pll_ssc_mode;
	unsigned	short	org_tdqs2dq;
	unsigned	char	reserve1_test_function[2];
	unsigned	int		ddr_dmc_remap[5];
	unsigned	char	ac_pinmux[35];
	unsigned	char	ddr_dqs_swap;
	unsigned	char	ddr_dq_remap[36];
	unsigned	int		ddr_vddee_setting[4];//add,default-value,default-voltage,step
}__attribute__ ((packed)) board_common_setting_t;

typedef struct board_SI_setting_ps
{
	unsigned	short	DRAMFreq;
	unsigned	char	PllBypassEn;
	unsigned	char	training_SequenceCtrl;
	unsigned	short	ddr_odt_config;
	unsigned	char	clk_drv_ohm;
	unsigned	char	cs_drv_ohm;
	unsigned	char	ac_drv_ohm;
	unsigned	char	soc_data_drv_ohm_p;
	unsigned	char	soc_data_drv_ohm_n;
	unsigned	char	soc_data_odt_ohm_p;
	unsigned	char	soc_data_odt_ohm_n;
	unsigned	char	dram_data_drv_ohm;
	unsigned	char	dram_data_odt_ohm;
	unsigned	char	dram_data_wr_odt_ohm;
	unsigned	char	dram_ac_odt_ohm;
	unsigned	char	dram_data_drv_pull_up_calibration_ohm;
	unsigned	char	lpddr4_dram_vout_voltage_range_setting;
	unsigned	char	reserve2;
	unsigned	short	vref_ac_permil; //phy
	unsigned	short	vref_soc_data_permil; //soc
	unsigned	short	vref_dram_data_permil;
	unsigned	short	max_core_timmming_frequency;
	unsigned	short	training_phase_parameter[2];
	unsigned	short	ac_trace_delay_org[36];
}__attribute__ ((packed)) board_SI_setting_ps_t;

typedef struct board_phase_setting_ps
{
	unsigned	short	ac_trace_delay[36];
	unsigned	short	write_dqs_delay[8];
	unsigned	short	write_dq_bit_delay[72];
	unsigned	short	read_dqs_gate_delay[8];
	unsigned	char	read_dqs_delay[8];
	unsigned	char	read_dq_bit_delay[72];
	unsigned	char	soc_bit_vref[44];
	unsigned	char	dram_bit_vref[36];
	unsigned	char	reserve_training_parameter[16];//0-7 read dqs offset,8-15 write dqs offset,MSB bit 7 use 0 mean right offset
}__attribute__ ((packed)) board_phase_setting_ps_t;

typedef struct ddr_set
{
	board_common_setting_t		cfg_board_common_setting	;
	board_SI_setting_ps_t		cfg_board_SI_setting_ps[2]	;
	board_phase_setting_ps_t	cfg_ddr_training_delay_ps[2]	;
}__attribute__ ((packed)) ddr_set_t;

#define DDR_FW_TOTAL_OFFSET		0
#define DDR_FW_TOTAL_SIZE		2
#define DDR_FW_TOTAL_VERSION		3
#define DDR_FW_BIN_OFFSET		4
#define DDR_FW_BIN_SIZE		6
#define DDR_FW_VERSION	7
#define DDR_ACS_BIN_OFFSET		8
#define DDR_ACS_BIN_SIZE		10
#define DDR_ACS_VERSION		11
#define DDR_FAST_BOOT_DATA_OFFSET		12
#define DDR_FAST_BOOT_DATA_SIZE		14
#define DDR_FAST_BOOT_DATA_VERSION		15
#define DDR_STICKY_REG_ADD_OFFSET		16
#define DDR_STICKY_REG_SIZE_OFFSET		18
#define DDR_STICKY_REG_VERSION_OFFSET		19

//#define DDR_FW_HEAD_SIZE		(32<<2)//>96
//#define DDR_BL2_DDR_FW_HEAD_SIZE					256
typedef struct ddr_fw_head_struct{

	unsigned int 	ddr_all_fw_add;
	unsigned int 	ddr_all_fw_size;
	unsigned int 	ddr_all_fw_version;

	unsigned int 	ddr_fw_add;
	unsigned int 	ddr_fw_size;
	unsigned int 	ddr_fw_version;

	unsigned int 	ddr_acs_bin_add;
	unsigned int 	ddr_acs_bin_size;
	unsigned int 	ddr_acs_bin_version;

	unsigned int 	ddr_fast_boot_data_add;
	unsigned int 	ddr_fast_boot_data_size;
	unsigned int 	ddr_fast_boot_data_version;

	unsigned int 	ddr_sticky_add;
	unsigned int 	ddr_sticky_size;
	unsigned int 	ddr_sticky_version;

	//void (*log_info)(log_chl chl,const char *fmt, ...);
	int (*serial_puts)(const char *s);
	void (*serial_put_hex)(unsigned long data, int bitlen);
	void (*ddr_init_return)(void);

	unsigned int	 ddr_bl2_ddr_fw_mail_message[12];

} ddr_fw_head_struct_t;

typedef struct pll_ctrl {
	unsigned short 	delay_u;
	unsigned short 	flag;
	unsigned int 	clkset;
	unsigned int	pll_para[10];
	unsigned int 	reserve;

}__attribute__ ((packed)) pll_ctrl_t;

typedef struct pll_set_new {
	pll_ctrl_t		sys_pll_ctrl;
	pll_ctrl_t		fix_pll_cntl;
	pll_ctrl_t		gp0_pll_cntl;
	pll_ctrl_t		gp1_pll_cntl;
	pll_ctrl_t		hifi_pll_cntl;

}__attribute__ ((packed)) pll_set_t_new;

typedef struct pll_set{
	unsigned	short	cpu_clk;
	unsigned	short	pxp;
	unsigned	int		spi_ctrl;
	unsigned	short	vddee;
	unsigned	short	vcck;
	unsigned	char	low_console_baud;
	unsigned	char	szPad[3];

	unsigned	long long	lCustomerID;
	unsigned	char		debug_mode;
	unsigned	char		log_chl;
	unsigned	char		log_ctrl;
	unsigned	char		ddr_timming_save_mode;
	unsigned	int			nCFGTAddr;
	/* align 8Byte */

	unsigned	int		sys_pll_cntl[8];
	unsigned	int		ddr_pll_cntl[8];
	unsigned	int		fix_pll_cntl[8];
}__attribute__ ((packed)) pll_set_t;

typedef struct dwc_apb {
	unsigned int      addr;
	unsigned short    val;
} dwc_apb_t;

#endif //__AML_TIMING_H_
