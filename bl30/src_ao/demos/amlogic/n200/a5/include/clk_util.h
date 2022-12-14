#ifndef CLK_UTIL_H
#define CLK_UTIL_H

//#include "define.h"
//#include "common_util.h"

#define PLL_LOCK_BIT_OFFSET  31
#define PLL_LOCK_CHECK_LOOP  50

#define ENC_SEL_I 1
#define ENC_SEL_P 2
#define ENC_SEL_L 0
#define ENC_SEL_T 3
#define ENC_SEL_ALL -1
/*
typedef enum {
	PLL_SYS,
	PLL_GP0,
	PLL_GP1,
	PLL_FIX,
	PLL_HIFI,
	PLL_RTC,
	PLL_DDR
} PLL_TYPE;
*/
typedef enum {
	A55_CLK_PCLK,
	A55_CLK_ACLKM,
	A55_CLK_ATCLK,
	A55_CLK_TRACECLK,
	A55_CLK_GICCLK,
	A55_CLK_PERIPHCLK,
	A55_CLK_CNTCLK,
	A55_CLK_TSCLK
} A55_CLK_ITEMS;

typedef enum {
	A53_CLK_PCLK,
	A53_CLK_ACLKM,
	A53_CLK_ATCLK,
	A53_CLK_CNTCLK,
	A53_CLK_TRACECLK,
	A73_CLK_CORE0,
	A73_CLK_CORE1,
	A73_CLK_CORE2,
	A73_CLK_CORE3,
} A53_CLK_ITEMS;

void set_dsu_pll_clk(uint32_t freq);
void set_sys_pll_clk(uint32_t freq);
void set_fix_pll_clk(void);
void set_ddr_pll_clk(uint32_t freq);
void set_cts_sys_clk(int freq_sel);
void set_cts_axi_clk(int freq_sel);
void set_rama_div_clk(int sel, int div);
void clocks_set_dsu_clk(uint32_t freq);
void clocks_set_cpu_core_clk(uint32_t cpu_id, uint32_t freq);
void clocks_set_a55_div_clk_rate(int clk_idx, int ratio);
void clocks_set_sys_defaults(void);
void switch_cpu_clk_to_pll(void);

void clk_util_set_mpeg_clock( uint32_t hiu_reg, uint32_t xd );
void clk_util_set_audio_clock( uint32_t hiu_reg, uint32_t xd );
void clk_util_set_audio_clk_for_hdmi(uint32_t clk_in_mhz);
void clk_util_set_audio_amclk_iec958(uint32_t clk_in_mhz);
//void clk_util_set_video_clock( uint32_t pll_reg, uint32_t vid_div_reg,
//														uint32_t xd );
//void clk_util_clocks_set_video_other(uint32_t vclk3_freq,
//					uint32_t vclk2_ratio, uint32_t vclk1_ratio);
//void clk_util_setup_lvds_clock( uint32_t vid_pll_clk_sel,
//						uint32_t phy_clk_div2, uint32_t vid_pll_hiu_reg,
//           uint32_t vid_pll_xd, uint32_t digital_clk_sel, uint32_t perform_cal );

//define the expected or deriving frequency value and position in clk_to_msr_in
//frequency value
#define    SYS_OSC_CLK_VAL          24000000     //sys_osc_clk 24Mhz
#define    FCLK_DIV2_VAL            1000000000    //fclk_div2 1Ghz
#define    FCLK_DIV2P5_VAL          800000000    //fclk_div2p5 800Mhz
#define    FCLK_DIV3_VAL            666666667    //fclk_div3 666.6Mhz
#define    FCLK_DIV4_VAL            500000000    //fclk_div4 500Mhz
#define    FCLK_DIV5_VAL            400000000    //fclk_div5 400Mhz
#define    FCLK_DIV7_VAL            285714286    //fclk_div7 285.7Mhz
//clk_fromPad 25Mhz used to test the FEC_clk,gpio_clk_msr,
//i2s_clk_in_src0 and rmii_from_pad
#define    CLK_FROMPAD_VAL          25000000
#define    MP0_CLK_OUT_VAL          175000000    //mp0_clk_out 500Mhz
#define    MP1_CLK_OUT_VAL          220000000    //mp1_clk_out 500Mhz
#define    MP2_CLK_OUT_VAL          25000000    //mp2_clk_out 500Mhz
#define    MP3_CLK_OUT_VAL          500000000    //mp3_clk_out 500Mhz
#define    MPLL_CLK_TEST_VAL        400000000    //mpll_clk_test_out 400Mhz
#define    GP1_PLL_CLK_VAL          596000000   //gp0_pll_clk 500Mhz
#define    GP0_PLL_CLK_VAL          596000000   //gp0_pll_clk 596Mhz
#define    HIFI_PLL_CLK_VAL         596000000   //hifi_pll_clk 596Mhz
#define    MPLL_CLK_50M_VAL         50000000   //mpll_clk_50m 50Mhz
#define    VID_PLL_CLK_VAL          600000000    //Vid_pll_clk
#define    VID2_PLL_CLK_VAL         274000000    //Vid2_pll_clk
#define    SYS_PLL_CLK_VAL          1290000000    //Vid2_pll_clk
#define    DDR_PLL_CLK_VAL          456000000    //ddr_pll_clk
#define    MMC_CLK_VAL              DDR_PLL_CLK_VAL //mmc_clk
#define    PCIE_PLL_CLK_VAL         100000000   //pcie_pll_clk 100Mhz
#define    ETH_MPLL_50M_CKOUT_VAL   50000000   //eth_mppll_50m_ckout 50Mhz
#define    ETH_CLK50OUT_VAL         50000000   //eth_clk50out 50Mhz
#define    ETH_CLK125Mhz            125000000  //cts_eth_clk125Mhz 125Mhz
#define    CLK81_VAL                166666666

#define    AM_RING_OSC_CLK_LOW_VAL      10400000 //10.4M
#define    AM_RING_OSC_CLK_HIGH_VAL     15600000 //15.6M

#define    RNG_RING_OSC_FRQ_0_VAL     166600000
#define    RNG_RING_OSC_FRQ_1_VAL     142800000
#define    RNG_RING_OSC_FRQ_2_VAL     120500000
#define    RNG_RING_OSC_FRQ_3_VAL     111100000
#define    RNG_RING_OSC_FRQ_4_VAL     100000000
#define    RNG_RING_OSC_FRQ_5_VAL     90900000
#define    RNG_RING_OSC_FRQ_6_VAL     83300000
#define    RNG_RING_OSC_FRQ_7_VAL     76900000
#define    RNG_RING_OSC_FRQ_8_VAL     71400000
#define    RNG_RING_OSC_FRQ_9_VAL     66600000
#define    RNG_RING_OSC_FRQ_10_VAL    62500000
#define    RNG_RING_OSC_FRQ_11_VAL    58800000
#define    RNG_RING_OSC_FRQ_12_VAL    55500000
#define    RNG_RING_OSC_FRQ_13_VAL    52600000
#define    RNG_RING_OSC_FRQ_14_VAL    50000000
#define    RNG_RING_OSC_FRQ_15_VAL    47600000

#define    CLK_FEC_VAL              25000000 //clk_fromPad 25Mhz
#define    CLK_I2S_IN_SRC0_VAL      25000000 //clk_fromPad 25Mhz
#define    ETH_RX_CLK_RMII_VAL      25000000 //clk_fromPad 25Mhz
#define    GPIO_CLK_VAL             25000000

#define    HDMI_PLL_CLK_VAL         274000000//274M
#define    HDMI_PLL_CLK2_VAL        274000000//274M

#define    ADC_DPLL_INTCLK_VAL      50000000
#define    ADC_DPLL_CLK_A2_VAL      50000000
#define    ADC_DPLL_CLK_B2_VAL      50000000
#define    ADC_DPLL_CLK_B3_VAL      50000000

#define    VIRTUAL_CLK_VAL          100000000 //100Mhz

#define    HDMIRX_AUD_PLL_CLK_VAL   VIRTUAL_CLK_VAL

//clock position in clk_to_msr_in
//
// Modified by guangyin.hu  2017/11/7 , for testing clock in G12A
#define AM_RING_OSC_CLK_0_POS         0
#define AM_RING_OSC_CLK_1_POS         1
#define AM_RING_OSC_CLK_2_POS         2
#define AM_RING_OSC_CLK_3_POS         3
#define GP0_PLL_CLK_POS               4
#define GP1_PLL_CLK_POS               5
#define CTS_ENCI_CLK_POS              6
#define CLK81_POS                     7
#define CTS_ENCP_CLK_POS              8
#define CTS_ENCL_CLK_POS              9
#define CTS_VDAC_CLK_POS              10
#define RGMII_TX_CLK_TO_PHY_POS       11
#define HIFI_PLL_CLK_POS              12
#define TCON_CLKO_POS                 13
#define CTS_FEC_CLK_0_POS             14
#define CTS_FEC_CLK_1_POS             15
#define CTS_FEC_CLK_2_POS             16
#define SYS_PLL_CLK_DIV16_POS         17
#define SYS_CPU_CLK_DIV16_POS         18
#define LCD_AN_CLK_PH2_POS            19

#define LCD_AN_CLK_PH3_POS            21
#define ETH_PHY_REF_CLK_POS           22
#define MPLL_CLK_50M_POS              23
#define CTS_ETH_CLK125MHZ_POS         24
#define CTS_ETH_CLK_RMII_POS          25
#define SC_CLK_INT_POS                26
#define CO_CLKIN_TO_MAC_CLK_POS       27
#define CTS_SAR_ADC_CLK_POS           28
#define PCIE_CLK_INP_POS              29
#define PCIE_CLK_INN_POS              30
#define MPLL_CLK_TEST_OUT_POS         31
#define CTS_VDEC_CLK_POS              32
//#define SYS_CPU_RING_OSC_CLK_1_POS    33
#define ETH_MPPLL_50M_CKOUT_POS       34
#define CTS_MALI_CLK_POS              35
#define CTS_HDMI_TX_PIXEL_CLK_POS     36
#define CTS_CDAC_CLK_POS              37
#define CTS_VDIN_MEAS_CLK_POS         38
#define CTS_BT656_CLK0_POS            39
#define AM_RING_OSC_CLK_4_POS         40
//#define CTS_PCM_MCLK_POS            40
#define ETH_RX_CLK_OR_CLK_RMII_POS    41
#define MP0_CLK_OUT_POS               42
#define FCLK_DIV5_POS                 43
#define CTS_PWM_B_CLK_POS             44
#define CTS_PWM_A_CLK_POS             45
#define CTS_VPU_CLK_POS               46
#define DDR_DPLL_PT_CLK_POS           47
#define MP1_CLK_OUT_POS               48
#define MP2_CLK_OUT_POS               49
#define MP3_CLK_OUT_POS               50
//#define CTS_NAND_CORE_CLK_POS       51
#define CTS_SD_EMMC_CLK_C_POS         51
#define CTS_SD_EMMC_CLK_B_POS         52
#define CTS_SD_EMMC_CLK_A_POS         53
#define CTS_VPU_CLKC_POS              54
#define VID_PLL_DIV_CLK_OUT_POS       55
#define CTS_WAVE420L_ACLK_POS         56
#define CTS_WAVE420L_CCLK_POS         57
#define CTS_WAVE420L_BCLK_POS         58

#define CTS_HCODEC_CLK_POS            59
//#define ALT_32K_CLK_POS             60
#define AM_RING_OSC_CLK_5_POS         60
#define GPIO_CLK_MSR_POS              61
#define CTS_HEVCB_CLK_POS             62
#define CTS_DSI_MEAS_CLK_POS          63
#define CTS_SPICC_1_CLK_POS           64
#define CTS_SPICC_0_CLK_POS           65
#define CTS_VID_LOCK_CLK_POS          66
#define CTS_DSI_PHY_CLK_POS           67
#define CTS_HDCP22_ESMCLK_POS         68
#define CTS_HDCP22_SKPCLK_POS         69
#define CTS_PWM_F_CLK_POS             70
#define CTS_PWM_E_CLK_POS             71
#define CTS_PWM_D_CLK_POS             72
#define CTS_PWM_C_CLK_POS             73
//#define CTS_ATV_DMD_VDAC_CLK_POS    74
#define AM_RING_OSC_CLK_6_POS         74
#define CTS_HEVCF_CLK_POS             75
//#define CTS_AOCLK_INT_POS           76
#define AM_RING_OSC_CLK_7_POS         76
#define RNG_RING_OSC_0_POS            77
#define RNG_RING_OSC_1_POS            78
#define RNG_RING_OSC_2_POS            79
#define RNG_RING_OSC_3_POS            80
#define CTS_VAPB_CLK_POS              81
#define CTS_GE2D_CLK_POS              82
#define CO_RX_CLK_POS                 83
#define CO_TX_CLK_POS                 84
//#define HDMIRX_PHY_DTB2_POS         85
#define AM_RING_OSC_CLK_8_POS         85
//#define HDMIRX_PHY_DTB3_POS         86
#define AM_RING_OSC_CLK_9_POS         86
//#define LVDS_FIFO_CLK_POS           87
//#define HDMIRX_VID_CLK_POS          88
#define CTS_MIPI_CSI_PHY_CLK_POS      87
#define CTS_CSI2_ADAPT_CORE_CLK_POS   88
#define HDMITX_TMDS_CLK_POS           89
#define CTS_HDMITX_SYS_CLK_POS        90
//#define CTS_AUDIN_MCLK_POS          91
//#define CTS_AUDIN_SCLK_POS          92
//#define CTS_AUDIN_LRCLK_POS         93
#define CTS_VIPNANOQ_CORE_CLK_POS     91
#define CTS_VIPNANOQ_AXI_CLK_POS      92
#define AUDIO_VAD_CLK_POS             93
#define ETH_PHY_RXCLK_POS             94
#define ETH_PHY_PLLTXCLK_POS          95
#define CTS_VPU_CLKB_POS              96
#define CTS_VPU_CLKB_TMP_POS          97
#define CTS_TS_CLK_POS                98
#define AM_RING_OSC_CLK_10_POS        99
#define AM_RING_OSC_CLK_11_POS        100
#define SYS_CPU_RING_OSC_CLK_0_POS    101
#define SYS_CPU_RING_OSC_CLK_1_POS    102
#define SYS_CPU_RING_OSC_CLK_2_POS    103
#define SYS_CPU_RING_OSC_CLK_3_POS    104

#define EPHY_TEST_CLK_POS             106
#define AU_DAC_CLK_G128X_POS          107
#define C_ALOCKER_IN_CLK_POS          108
#define C_ALOCKER_OUT_CLK_POS         109
#define AUDIO_TDMOUT_C_SCLK_POS       110
#define AUDIO_TDMOUT_B_SCLK_POS       111
#define AUDIO_TDMOUT_A_SCLK_POS       112
#define AUDIO_TDMIN_LB_SCLK_POS       113
#define AUDIO_TDMIN_C_SCLK_POS        114
#define AUDIO_TDMIN_B_SCLK_POS        115
#define AUDIO_TDMIN_A_SCLK_POS        116
#define AUDIO_RESAMPLEA_CLK_POS       117
#define AUDIO_PDM_SYSCLK_POS          118
#define AUDIO_SPDIFOUT_B_CLK_POS      119
#define AUDIO_SPDIFOUT_CLK_POS        120
#define AUDIO_SPDIFIN_CLK_POS         121
#define AUDIO_PDM_DCLK_POS            122
#define AUDIO_RESAMPLEB_CLK_POS       123
#define I_EARCRX_PLL_DMAC_CKOUT_POS   124
#define EARCRX_PLL_CKOUT_POS          125
#define MIPI_CSI_PHY0_CLK_OUT_POS     126
#define CLK_CSI2_DATA_POS             127

#define CTS_ENCT_CLK_POS              130
#define CTS_VDAC_CLK_1_POS            131
#define CTS_ATV_DMD_VDAC_CLK_POS      132
#define CTS_ADC_DPLL_CLK_A2_POS       133
#define CTS_ADC_DPLL_CLK_B2_POS       134
#define CTS_ADC_DPLL_CLK_B3_POS       135
#define CTS_ADC_DPLL_INTCLK_POS       136
#define I2S_CLK_IN_SRC0_POS           213
#define MMC_CLK_POS                   138
#define HDMIRX_AUD_CLK_POS            139
#define CTS_HEVC_CLK_POS              140
#define CTS_TVFE_SAMPLE_CLK_POS       141
#define HDMIRX_PHY_DTB0_POS           142
#define HDMIRX_PHY_DTB1_POS           143
#define HDMIRX_PHY_DTB2_POS           144
#define HDMIRX_PHY_DTB3_POS           145
#define HDMIRX_VID_CLK_POS            146
#define CTS_AUDIN_MCLK_POS            147
#define CTS_AUDIN_SCLK_POS            148
#define CTS_AUDIN_LRCLK_POS           149
#define ETHPHY_TEST_CLK_OUT_POS       150
#define HDMI_CLK_TODIG_POS            151

#endif

