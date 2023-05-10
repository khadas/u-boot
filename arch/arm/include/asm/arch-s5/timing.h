
/*
 * arch/arm/include/asm/arch-p1/timing.h
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef __AML_TIMING_H_
#define __AML_TIMING_H_

#include <asm/arch/ddr_define.h>
#include <asm/arch/types.h>
#include <asm/arch/mnPmuSramMsgBlock_ddr3.h>
#include <asm/arch/mnPmuSramMsgBlock_ddr4.h>
#include <asm/arch/mnPmuSramMsgBlock_ddr4_2d.h>
#include <asm/arch/mnPmuSramMsgBlock_lpddr3.h>
#include <asm/arch/mnPmuSramMsgBlock_lpddr4.h>
#include <asm/arch/mnPmuSramMsgBlock_lpddr4_2d.h>
#include <asm/arch/dev_parameter.h>

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

typedef struct bl2_reg {
	unsigned int	reg;
	unsigned int	value;
	unsigned int	mask;
	unsigned int	flag;
	unsigned short	udelay;
	unsigned short	rsv_0;
}   bl2_reg_t;

typedef struct ddr_reg {
	unsigned int	reg;
	unsigned int	value;
	unsigned int	mask;
	unsigned short	udelay;
	unsigned char	flag;
	unsigned char	rsv_0;
}   ddr_reg_t;

typedef struct ddr_offset {
	unsigned short	read_dq_delay_t[72];
	unsigned short	read_dq_delay_c[72];
	unsigned char	soc_bit_vref0[36];
	unsigned char	soc_bit_vref1[36];
	unsigned char	soc_bit_vref2[36];
	unsigned char	soc_bit_vref3[36];
}   ddr_offset_t;

//typedef struct training_delay_set_ps {
//	unsigned char ac_trace_delay[11];
//	unsigned char ac_trace_delay_rev[5];
//	unsigned	short	read_dq_delay_t[72];
//	unsigned	short	read_dq_delay_c[72];
//	unsigned	short	read_dqs_dig_delay[72];
//	unsigned	short	write_dqs_delay[8];
//	unsigned	short	write_wck_delay[8];
//	unsigned	short	wdq_delay[72];
//	unsigned	short	read_dqs_gate_delay[8];
//	unsigned	char	soc_bit_vref0[36];
//	unsigned	char	soc_bit_vref1[36];
//	unsigned	char	soc_bit_vref2[36];
//	unsigned	char	soc_bit_vref3[36];
//	unsigned	char	dram_vref[32];
//
//	unsigned	short	dca_wck_tx_t[8];//t and c
//	unsigned	short	dca_wck_rx_t[8];//t and c
//	unsigned	short	dca_dqs_tx_t[8];//t and c
//	unsigned	short	dca_wck_tx_c[8];//t and c
//	unsigned	short	dca_wck_rx_c[8];//t and c
//	unsigned	short	dca_dqs_tx_c[8];//t and c
//	unsigned	short	dca_dq_tx[8];//common
//
//	unsigned char dfi_mrl[4];
//	//ac dfi_mrl data dfi_mrl
//	//unsigned char dfi_hwtmrl;
//	//unsigned char ARdPtrInitVal;
//	//unsigned short csr_vrefinglobal;
//	//unsigned short csr_dqsrcvcntrl[4];
//	unsigned short pptdqscnttg0[4];
//	unsigned short pptdqscnttg1[4];
//	//unsigned short csr_seq0bgpr[9];
//	//unsigned short csr_dllgainctl;
//	//unsigned short csr_dlllockpara;
//	//	unsigned	short	rever2;
//}   training_delay_set_ps_t;

typedef struct ddr_mrs_reg {
	unsigned short ddr_mr[52];
}   ddr_mrs_reg_t;

typedef struct ddr_timing {
	unsigned int	identifier;
	unsigned int	ddr_mrd;
	unsigned int	cfg_ddr_rfcab;
	unsigned int	cfg_ddr_rfcpb;
	unsigned int	cfg_ddr_pbr2act;
	unsigned int	cfg_ddr_pbr2pbr;
	unsigned int	cfg_ddr_ppd;
	unsigned int	cfg_ddr_rpab;
	unsigned int	cfg_ddr_rppb;
	unsigned int	cfg_ddr_rtw;
	unsigned int	cfg_ddr_rl;
	unsigned int	cfg_ddr_wl;
	//unsigned	int		cfg_ddr_nrbtp;
	unsigned int	cfg_ddr_ras;
	unsigned int	cfg_ddr_rc;
	unsigned int	cfg_ddr_rcd;
	unsigned int	cfg_ddr_rrds;
	unsigned int	cfg_ddr_rrdl;
	unsigned int	cfg_ddr_faw;
	unsigned int	cfg_ddr_rtp;
	unsigned int	cfg_ddr_wr;
	unsigned int	cfg_ddr_wtrs;
	unsigned int	cfg_ddr_wtrl;
	unsigned int	cfg_ddr_ccds;
	unsigned int	cfg_ddr_ccdl;
	unsigned int	cfg_ddr_exsr;
	unsigned int	cfg_ddr_xs;
	unsigned int	cfg_ddr_xp;
	unsigned int	cfg_ddr_xpdll;
	unsigned int	cfg_ddr_zqcs;
	unsigned int	cfg_ddr_cksre;
	unsigned int	cfg_ddr_cksrx;
	unsigned int	cfg_ddr_cke;
	unsigned int	cfg_ddr_mod;
	unsigned int	cfg_ddr_dqs;
	unsigned int	cfg_ddr_rstl;
	unsigned int	cfg_ddr_zqlat;
	unsigned int	ddr_mrr;
	unsigned int	cfg_ddr_ckesr;
	unsigned int	cfg_ddr_dpd;
	unsigned int	cfg_ddr_ckeck;
	unsigned int	cfg_ddr_refi;
	unsigned int	cfg_ddr_sr;
	unsigned int	cfg_ddr_ccdmw;
	unsigned int	cfg_ddr_escke;
	unsigned int	cfg_ddr_refi_ddr3;
	unsigned int	cfg_ddr_dfictrldelay;
	unsigned int	cfg_ddr_dfiphywrdata;
	unsigned int	cfg_ddr_dfiphywrlat;
	unsigned int	cfg_ddr_dfiphyrddataen;
	unsigned int	cfg_ddr_dfiphyrdlat;
	unsigned int	cfg_ddr_dfiphywrlatcsn;
	unsigned int	cfg_ddr_dfiphyrddatacsn;
	unsigned int	cfg_ddr_dfictrlupdmin;
	unsigned int	cfg_ddr_dfictrlupdmax;
	unsigned int	cfg_ddr_dfimstrresp;
	unsigned int	cfg_ddr_dfirefmski;
	unsigned int	cfg_ddr_dfictrlupdi;
	unsigned int	cfg_ddr_dfidramclk;
	unsigned int	cfg_ddr_dfilpresp;
	unsigned int	cfg_ddr_dfiphymstr;
	unsigned int	cfg_ddr_rtodt;
	unsigned int	cfg_ddr_wlmrd;
	unsigned int	cfg_ddr_wlo;
	unsigned int	cfg_ddr_al;
	unsigned int	cfg_ddr_zqcl;
	unsigned int	cfg_ddr_zqcsi;
	unsigned int	cfg_ddr_zqreset;
	unsigned int	cfg_ddr_tdqsck_min;
	unsigned int	cfg_ddr_tdqsck_max;
	//uint32_t addition_cl;
	uint32_t	dfi_wrlat;
	uint32_t	dfi_rdlat;
	//training_delay_set_ps_t	cfg_ddr_training_delay_ps;
	ddr_mrs_reg_t	ddr_mrs_reg_ps[2];
	unsigned int	dfi_odt1_config_ps[2];
	//ddr_mrs_reg_t		ddr_mrs_reg_ps1;
#if PSEUDO_DISABLE
	unsigned int	ddr_mr[8];
	unsigned int	ddr_mr11;
	unsigned int	ddr_mr12;
	unsigned int	ddr_mr13;
	unsigned int	ddr_mr14;
	unsigned int	ddr_mr16;
	unsigned int	ddr_mr17;
	unsigned int	ddr_mr22;
	unsigned int	ddr_mr24;
	unsigned int	cfg_ddr_reserve[5];
#endif
}  ddr_timing_t;

typedef struct board_common_setting {
	unsigned int	timming_magic;
	unsigned short	timming_max_valid_configs;
	unsigned short	timming_struct_version;
	unsigned short	timming_struct_org_size;
	unsigned short	timming_struct_real_size;
	unsigned char	fast_boot[4];
	// 0   fastboot enable  1 window test margin
	// 2 auto offset after window test 3 auto window test enable
	unsigned int	ddr_func;
	unsigned char	board_id;
	unsigned char	DramType;
	//support DramType should confirm with amlogic
	//#define CONFIG_DDR_TYPE_DDR3				0
	//#define CONFIG_DDR_TYPE_DDR4				1
	//#define CONFIG_DDR_TYPE_LPDDR4				2
	//#define CONFIG_DDR_TYPE_LPDDR3				3
	//#define CONFIG_DDR_TYPE_LPDDR2				4
	//#define CONFIG_DDR_TYPE_LPDDR4x
	unsigned char dram_rank_config;
	//support Dram connection type should confirm with amlogic
	//#define CONFIG_DDR0_16BIT_CH0				0x1  //dram total bus width 16bit only use cs0
	//#define CONFIG_DDR0_16BIT_RANK01_CH0		0x4  //dram total bus width 16bit  use cs0 cs1
	//#define CONFIG_DDR0_32BIT_RANK0_CH0			0x2  //dram total bus width 32bit  use cs0
	//#define CONFIG_DDR0_32BIT_RANK01_CH01		0x3    //only for lpddr4,dram total bus width 32bit  use channel a cs0 cs1 channel b cs0 cs1
	//#define CONFIG_DDR0_32BIT_16BIT_RANK0_CH0		0x5    //dram total bus width 32bit only use cs0,but high address use 16bit mode
	//#define CONFIG_DDR0_32BIT_16BIT_RANK01_CH0	0x6   //dram total bus width 32bit  use cs0 cs1,but cs1 use 16bit mode ,current phy not support reserve
	//#define CONFIG_DDR0_32BIT_RANK01_CH0		0x7       //dram total bus width 32bit  use cs0 cs1
	//#define CONFIG_DDR0_32BIT_RANK0_CH01		0x8     //only for lpddr4,dram total bus width 32bit  use channel a cs0  channel b cs0

	/* rsv_char0. update for diagnose type define */
	unsigned char	rsv_char0;

	unsigned char	DisabledDbyte[2];            //ch0 and ch1
	//use for dram bus 16bit or 32bit,if use 16bit mode ,should disable bit 2,3
	//bit 0 ---cs0 use byte 0 ,1 disable byte 0,
	//bit 1 ---cs0 use byte 1 ,1 disable byte 1,
	//bit 2 ---cs0 use byte 2 ,1 disable byte 2,
	//bit 3 ---cs0 use byte 3 ,1 disable byte 3,
	//bit 4 ---cs1 use byte 0 ,1 disable byte 0,
	//bit 5 ---cs1 use byte 1 ,1 disable byte 1,
	//bit 6 ---cs1 use byte 2 ,1 disable byte 2,
	//bit 7 ---cs1 use byte 3 ,1 disable byte 3,
	unsigned short	dram_ch0_size_MB;
	//config cs0 dram size ,like 1G DRAM ,setting 1024
	unsigned short	dram_ch1_size_MB; //
	//config cs1 dram size,like 512M DRAM ,setting 512
	/* align8 */

	unsigned char	dram_x4x8x16_mode;
	unsigned char	Is2Ttiming;
	unsigned char	log_level;
	unsigned char	dbi_enable;
	//system reserve,do not modify
	unsigned char	ddr_rfc_type;
	//config dram rfc type,according dram type,also can use same dram type max config
	//#define DDR_RFC_TYPE_DDR3_512Mbx1				0
	//#define DDR_RFC_TYPE_DDR3_512Mbx2				1
	//#define DDR_RFC_TYPE_DDR3_512Mbx4				2
	//#define DDR_RFC_TYPE_DDR3_512Mbx8				3
	//#define DDR_RFC_TYPE_DDR3_512Mbx16				4
	//#define DDR_RFC_TYPE_DDR4_2Gbx1					5
	//#define DDR_RFC_TYPE_DDR4_2Gbx2					6
	//#define DDR_RFC_TYPE_DDR4_2Gbx4					7
	//#define DDR_RFC_TYPE_DDR4_2Gbx8					8
	//#define DDR_RFC_TYPE_LPDDR4_2Gbx1				9
	//#define DDR_RFC_TYPE_LPDDR4_3Gbx1				10
	//#define DDR_RFC_TYPE_LPDDR4_4Gbx1				11
	unsigned char	enable_lpddr4x_mode;
	//system reserve,do not modify
	/* align8 */

	unsigned int	pll_ssc_mode;
	//
	//pll ssc config:
	//
	//  pll_ssc_mode = (1<<20) | (1<<8) | ([strength] << 4) | [mode],
	//     ppm = strength * 500
	//     mode: 0=center, 1=up, 2=down
	//
	//  eg:
	//    1. config 1000ppm center ss. then mode=0, strength=2
	//       .pll_ssc_mode = (1<<20) | (1<<8) | (2 << 4) | 0,
	//    2. config 3000ppm down ss. then mode=2, strength=6
	//       .pll_ssc_mode = (1<<20) | (1<<8) | (6 << 4) | 2,
	//
	unsigned short	org_tdqs2dq;
	unsigned char	reserve1_test[2];
	unsigned int	ddr_dmc_remap[5];
	unsigned char	lpddr34_ca_remap[4];
	unsigned char	ddr_dq_remap[36];
	unsigned char	ac_pinmux[DWC_AC_PINMUX_TOTAL]; //24 35
	unsigned char	dfi_pinmux[DWC_DFI_PINMUX_TOTAL];
	unsigned char	ddr_dqs_swap;
	unsigned char	rsv_char1;
	unsigned char	rsv_char2;
	unsigned char	rsv_char3;
	unsigned int	ddr_vddee_setting[4];   // add,default-value,default-voltage,
}   board_common_setting_t;
typedef struct board_SI_setting_ps {
	unsigned short	DRAMFreq;
	unsigned char	PllBypassEn;
	unsigned char	training_SequenceCtrl;
	unsigned int	dfi_odt_config;
	//normal go status od config,use for normal status
	//bit 12.  rank1 ODT default. default vulue for ODT[1] pins if theres
	//no read/write activity.
	//bit 11.  rank1 ODT write sel.  enable ODT[1] if there's write  occurred in rank1.
	//bit 10.  rank1 ODT write nsel. enable ODT[1] if theres's write  occurred in rank0.
	//bit 9.   rank1 odt read sel.   enable ODT[1] if there's read  occurred in rank1.
	//bit 8.   rank1 odt read nsel.  enable ODT[1] if there's read  occurrede in rank0.
	//bit 4.   rank0 ODT default.    default vulue for ODT[0] pins if
	//theres no read/write activity.
	//bit 3.   rank0 ODT write sel.  enable ODT[0] if there's write  occurred in rank0.
	//bit 2.   rank0 ODT write nsel. enable ODT[0] if theres's write  occurred in rank1.
	//bit 1.   rank0 odt read sel.   enable ODT[0] if there's read  occurred in rank0.
	//bit 0.   rank0 odt read nsel.  enable ODT[0] if there's read  occurrede in rank1.
	unsigned char	phy_odt_config_rank[2];
	unsigned short	clk_drv_ohm;
	//config soc clk pin signal driver strength ,select 20,30,40,60ohm
	unsigned short	cs_drv_ohm;
	//config soc cs0 cs1 pin signal driver strength ,select 20,30,40,60ohm
	unsigned short	ac_drv_ohm;
	//config soc  normal address command pin driver strength ,select 20,30,40,60ohm
	unsigned short	soc_data_drv_ohm_p;
	//config soc data pin pull up driver strength,
	//select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short	soc_data_drv_ohm_n;
	//config soc data pin pull down driver strength,
	//select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short	soc_data_odt_ohm_p;
	//config soc data pin odt pull up strength,
	//select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short	soc_data_odt_ohm_n;
	//config soc data pin odt pull down strength,
	//select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short	dram_data_drv_ohm;
	//config dram data pin pull up pull down driver strength,
	//ddr3 select 34,40ohm,ddr4 select 34,48ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned short	dram_data_odt_ohm;
	//config dram data pin odt pull up down strength,ddr3 select 40,60,120ohm,
	//ddr4 select 34,40,48,60,120,240ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned short	dram_data_wr_odt_ohm;
	//config dram data pin odt pull up down strength,ddr3 select 40,60,120ohm,
	//ddr4 select 34,40,48,60,120,240ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned short	dram_ac_odt_ohm;
	//config dram ac pin odt pull up down strength,use for lpddr4, select 40,48,60,80,120,240ohm
	unsigned short	soc_clk_slew_rate;
	//system reserve,do not modify
	unsigned short	soc_cs_slew_rate;
	//system reserve,do not modify
	unsigned short	soc_ac_slew_rate;
	//system reserve,do not modify
	unsigned short	soc_data_slew_rate;
	//system reserve,do not modify
	unsigned char	dram_drv_pull_up_cal_ohm;
	//config soc data pin odt pull up strength,select 40,60,80,120ohm
	unsigned char	lpddr4_dram_vout_range;
	//use for lpddr4 read vout voltage  setting 0 --->2/5VDDQ ,1--->1/3VDDQ
	unsigned char	char_rev0;                      //dfe_offset_value;       //char_rev1;
	unsigned char	char_rev1;                      //training_offset;        //char_rev2;

	unsigned short	vref_ac_permil;
	//soc init dram ac vref ,config like 500 means 0.5VDDQ,take care ,please follow SI
	unsigned short	vref_soc_data_permil;
	//soc init SOC receiver vref ,config like 500 means 0.5VDDQ,take care ,please follow SI
	unsigned short	vref_dram_data_permil;
	//soc init DRAM receiver vref ,config like 500 means 0.5VDDQ,take care ,please follow SI
	unsigned short	max_core_timmming_frequency;
	//use for limited ddr speed core timmming parameter,
	//for some old dram maybe have no over speed register
	//unsigned short training_phase_parameter[2];
	//unsigned short ac_trace_delay_org[36];
}   board_SI_setting_ps_t;
typedef struct board_phase_setting_ps {
	unsigned short	ac_trace_delay[DWC_AC_PINMUX_TOTAL];
	//unsigned char ac_trace_delay_rev[5];
	unsigned short	read_dq_delay_t[72];
	unsigned short	read_dq_delay_c[72];
	unsigned short	read_dqs_delay[8];
	unsigned short	write_dqs_delay[8];
	unsigned short	write_wck_delay[8];
	unsigned short	wdq_delay[72];
	unsigned short	read_dqs_gate_delay[8];
	unsigned char	soc_bit_vref0[36];
	unsigned char	soc_bit_vref1[36];
	unsigned char	soc_bit_vref2[36];
	unsigned char	soc_bit_vref3[36];
	unsigned char	dram_vref[32];

	unsigned short	dca_wck_tx_t[8];        //t and c
	unsigned short	dca_wck_rx_t[8];        //t and c
	unsigned short	dca_dqs_tx_t[8];        //t and c
	unsigned short	dca_wck_tx_c[8];        //t and c
	unsigned short	dca_wck_rx_c[8];        //t and c
	unsigned short	dca_dqs_tx_c[8];        //t and c
	unsigned short	dca_dq_tx[8];           //common

	unsigned char	dfi_mrl[4];
	unsigned char	dfi_hwtmrl;
	unsigned char	csr_hwtctrl;
	unsigned char	rever1;
	unsigned char	rever2;
	//unsigned	char	ARdPtrInitVal;
	//unsigned	short	csr_vrefinglobal;
	//unsigned	short	csr_dqsrcvcntrl[4];
	unsigned short	pptdqscnttg0[4];
	unsigned short	pptdqscnttg1[4];
	unsigned short	PptWck2DqoCntTg0[4];
	unsigned short	PptWck2DqoCntTg1[4];
	unsigned short	RxReplicaPhase[4][5];
	//unsigned	short	csr_RxReplicaCtl03[4];
	//unsigned	short	csr_seq0bgpr[9];
	//unsigned	short	csr_dllgainctl;
	//unsigned	short	csr_dlllockpara;
	unsigned char	dac_offset[4];          //bit 7 offset direction 0 ++  1 --
	unsigned char	rx_offset[2];           //bit 7 offset direction 0 ++  1 --
	unsigned char	tx_offset[2];           //bit 7 offset direction 0 ++  1 --
	unsigned char	reserve_para[16];
	// 0-7 write dqs offset,8-15 read dqs offset,MSB bit 7 use 0 mean right offset
}   board_phase_setting_ps_t;
typedef struct ddr_set {
	board_common_setting_t		cfg_board_common_setting;
	board_SI_setting_ps_t		cfg_board_SI_setting_ps[2];
	board_phase_setting_ps_t	cfg_ddr_training_delay_ps[2];
}   ddr_set_t;

typedef struct ddr_set_ps0_only {
	board_common_setting_t		cfg_board_common_setting;
	board_SI_setting_ps_t		cfg_board_SI_setting_ps;
	board_phase_setting_ps_t	cfg_ddr_training_delay_ps;
}   ddr_set_ps0_only_t;

typedef struct board_clk_set{
	unsigned	short	cpu_clk;
	unsigned 	short	dsu_clk;
	unsigned	short	vddee;
	unsigned	short	vcck;
	unsigned	short	pxp;
	unsigned    char 	low_console_baud;
	unsigned	char	szPad[1];
}  board_clk_set_t;

typedef struct pll_ctrl{
	/*Enable flag: 0xa1:need set pll in bl2
	               0xa2:need set pll in bl2x
	               other: no need
	*/
	unsigned	char		flag;
	/* Delay time for timing sequence in 10us, range [0 .. 255]*/
	unsigned	char		delay_10u;
	/* PLL clock
	 * syspll	[93 .. 6000]
	 * fixpll	[93 .. 6000]
	 * gp0pll	[93 .. 6000]
	 * gp1pll	[93 .. 6000]
	 * hifipll	[375 .. 6000]
	 */
	unsigned	short	clkset;
	unsigned	int		pll_para[8];
	unsigned	int		reserve;
}   pll_ctrl_t;

typedef struct pll_set{
	/*new struct for sc2*/
	pll_ctrl_t      sys_pll_ctrl;
	pll_ctrl_t      sys1_pll_ctrl;
	pll_ctrl_t      fix_pll_ctrl;
	pll_ctrl_t      gp0_pll_ctrl;
	pll_ctrl_t      gp1_pll_ctrl;
	pll_ctrl_t      hifi_pll_ctrl;
}  pll_set_t;

typedef struct dwc_apb {
	unsigned int      addr;
	unsigned short    val;
} dwc_apb_t;

typedef struct bl2_sec_parameter{
	/*new struct for sc2*/
	uint32_t		version;
	uint32_t		bl31_region_start;
	uint32_t		bl31_region_size;
	uint32_t		bl32_region_start;
	uint32_t		bl32_region_size;

	uint32_t		RFU[27];
}  sec_parameter_t;

#endif //__AML_TIMING_H_
