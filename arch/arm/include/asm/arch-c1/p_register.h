/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifdef P_REGISTER_H
#else
#define P_REGISTER_H

//
// Reading file:  ./REG_LIST_RTL.h
//
// synopsys translate_off
// synopsys translate_on
//
// Reading file:  REG_LIST_AUDIO_RTL.h
//
//========================================================================
//  AUDIO - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe050000
// -----------------------------------------------
#define P_EE_AUDIO_CLK_GATE_EN0                    ((volatile uint32_t *)0xfe050000)
#define P_EE_AUDIO_CLK_GATE_EN1                    ((volatile uint32_t *)0xfe050004)
#define P_EE_AUDIO_MCLK_A_CTRL                     ((volatile uint32_t *)0xfe050008)
#define P_EE_AUDIO_MCLK_B_CTRL                     ((volatile uint32_t *)0xfe05000c)
#define P_EE_AUDIO_MCLK_C_CTRL                     ((volatile uint32_t *)0xfe050010)
#define P_EE_AUDIO_MCLK_D_CTRL                     ((volatile uint32_t *)0xfe050014)
#define P_EE_AUDIO_MCLK_E_CTRL                     ((volatile uint32_t *)0xfe050018)
#define P_EE_AUDIO_MCLK_F_CTRL                     ((volatile uint32_t *)0xfe05001c)
#define P_EE_AUDIO_PAD_CTRL0                       ((volatile uint32_t *)0xfe050020)
#define P_EE_AUDIO_PAD_CTRL1                       ((volatile uint32_t *)0xfe050024)
#define P_EE_AUDIO_SW_RESET0                       ((volatile uint32_t *)0xfe050028)
#define P_EE_AUDIO_SW_RESET1                       ((volatile uint32_t *)0xfe05002c)
#define P_EE_AUDIO_CLK81_CTRL                      ((volatile uint32_t *)0xfe050030)
#define P_EE_AUDIO_CLK81_EN                        ((volatile uint32_t *)0xfe050034)
#define P_EE_AUDIO_MST_A_SCLK_CTRL0                ((volatile uint32_t *)0xfe050040)
#define P_EE_AUDIO_MST_A_SCLK_CTRL1                ((volatile uint32_t *)0xfe050044)
#define P_EE_AUDIO_MST_B_SCLK_CTRL0                ((volatile uint32_t *)0xfe050048)
#define P_EE_AUDIO_MST_B_SCLK_CTRL1                ((volatile uint32_t *)0xfe05004c)
#define P_EE_AUDIO_MST_C_SCLK_CTRL0                ((volatile uint32_t *)0xfe050050)
#define P_EE_AUDIO_MST_C_SCLK_CTRL1                ((volatile uint32_t *)0xfe050054)
#define P_EE_AUDIO_MST_D_SCLK_CTRL0                ((volatile uint32_t *)0xfe050058)
#define P_EE_AUDIO_MST_D_SCLK_CTRL1                ((volatile uint32_t *)0xfe05005c)
#define P_EE_AUDIO_MST_E_SCLK_CTRL0                ((volatile uint32_t *)0xfe050060)
#define P_EE_AUDIO_MST_E_SCLK_CTRL1                ((volatile uint32_t *)0xfe050064)
#define P_EE_AUDIO_MST_F_SCLK_CTRL0                ((volatile uint32_t *)0xfe050068)
#define P_EE_AUDIO_MST_F_SCLK_CTRL1                ((volatile uint32_t *)0xfe05006c)
#define P_EE_AUDIO_MST_DLY_CTRL0                   ((volatile uint32_t *)0xfe050070)
#define P_EE_AUDIO_MST_DLY_CTRL1                   ((volatile uint32_t *)0xfe050074)
#define P_EE_AUDIO_CLK_TDMIN_A_CTRL                ((volatile uint32_t *)0xfe050080)
#define P_EE_AUDIO_CLK_TDMIN_B_CTRL                ((volatile uint32_t *)0xfe050084)
#define P_EE_AUDIO_CLK_TDMIN_C_CTRL                ((volatile uint32_t *)0xfe050088)
#define P_EE_AUDIO_CLK_TDMIN_LB_CTRL               ((volatile uint32_t *)0xfe05008c)
#define P_EE_AUDIO_CLK_TDMOUT_A_CTRL               ((volatile uint32_t *)0xfe050090)
#define P_EE_AUDIO_CLK_TDMOUT_B_CTRL               ((volatile uint32_t *)0xfe050094)
#define P_EE_AUDIO_CLK_TDMOUT_C_CTRL               ((volatile uint32_t *)0xfe050098)
#define P_EE_AUDIO_CLK_SPDIFIN_CTRL                ((volatile uint32_t *)0xfe05009c)
#define P_EE_AUDIO_CLK_SPDIFOUT_CTRL               ((volatile uint32_t *)0xfe0500a0)
#define P_EE_AUDIO_CLK_RESAMPLEA_CTRL              ((volatile uint32_t *)0xfe0500a4)
#define P_EE_AUDIO_CLK_LOCKER_CTRL                 ((volatile uint32_t *)0xfe0500a8)
#define P_EE_AUDIO_CLK_PDMIN_CTRL0                 ((volatile uint32_t *)0xfe0500ac)
#define P_EE_AUDIO_CLK_PDMIN_CTRL1                 ((volatile uint32_t *)0xfe0500b0)
#define P_EE_AUDIO_CLK_SPDIFOUT_B_CTRL             ((volatile uint32_t *)0xfe0500b4)
#define P_EE_AUDIO_CLK_RESAMPLEB_CTRL              ((volatile uint32_t *)0xfe0500b8)
#define P_EE_AUDIO_CLK_SPDIFIN_LB_CTRL             ((volatile uint32_t *)0xfe0500bc)
#define P_EE_AUDIO_CLK_EQDRC_CTRL0                 ((volatile uint32_t *)0xfe0500c0)
#define P_EE_AUDIO_VAD_CLK_CTRL                    ((volatile uint32_t *)0xfe0500c4)
#define P_EE_AUDIO_EARCTX_CMDC_CLK_CTRL            ((volatile uint32_t *)0xfe0500c8)
#define P_EE_AUDIO_EARCTX_DMAC_CLK_CTRL            ((volatile uint32_t *)0xfe0500cc)
#define P_EE_AUDIO_EARCRX_CMDC_CLK_CTRL            ((volatile uint32_t *)0xfe0500d0)
#define P_EE_AUDIO_EARCRX_DMAC_CLK_CTRL            ((volatile uint32_t *)0xfe0500d4)
#define P_EE_AUDIO_CLK_LOCKERB_CTRL                ((volatile uint32_t *)0xfe0500d8)
#define P_EE_AUDIO_TODDR_A_CTRL0                   ((volatile uint32_t *)0xfe050100)
#define P_EE_AUDIO_TODDR_A_CTRL1                   ((volatile uint32_t *)0xfe050104)
#define P_EE_AUDIO_TODDR_A_START_ADDR              ((volatile uint32_t *)0xfe050108)
#define P_EE_AUDIO_TODDR_A_FINISH_ADDR             ((volatile uint32_t *)0xfe05010c)
#define P_EE_AUDIO_TODDR_A_INT_ADDR                ((volatile uint32_t *)0xfe050110)
#define P_EE_AUDIO_TODDR_A_STATUS1                 ((volatile uint32_t *)0xfe050114)
#define P_EE_AUDIO_TODDR_A_STATUS2                 ((volatile uint32_t *)0xfe050118)
#define P_EE_AUDIO_TODDR_A_START_ADDRB             ((volatile uint32_t *)0xfe05011c)
#define P_EE_AUDIO_TODDR_A_FINISH_ADDRB            ((volatile uint32_t *)0xfe050120)
#define P_EE_AUDIO_TODDR_A_INIT_ADDR               ((volatile uint32_t *)0xfe050124)
#define P_EE_AUDIO_TODDR_A_CTRL2                   ((volatile uint32_t *)0xfe050128)
#define P_EE_AUDIO_TODDR_B_CTRL0                   ((volatile uint32_t *)0xfe050140)
#define P_EE_AUDIO_TODDR_B_CTRL1                   ((volatile uint32_t *)0xfe050144)
#define P_EE_AUDIO_TODDR_B_START_ADDR              ((volatile uint32_t *)0xfe050148)
#define P_EE_AUDIO_TODDR_B_FINISH_ADDR             ((volatile uint32_t *)0xfe05014c)
#define P_EE_AUDIO_TODDR_B_INT_ADDR                ((volatile uint32_t *)0xfe050150)
#define P_EE_AUDIO_TODDR_B_STATUS1                 ((volatile uint32_t *)0xfe050154)
#define P_EE_AUDIO_TODDR_B_STATUS2                 ((volatile uint32_t *)0xfe050158)
#define P_EE_AUDIO_TODDR_B_START_ADDRB             ((volatile uint32_t *)0xfe05015c)
#define P_EE_AUDIO_TODDR_B_FINISH_ADDRB            ((volatile uint32_t *)0xfe050160)
#define P_EE_AUDIO_TODDR_B_INIT_ADDR               ((volatile uint32_t *)0xfe050164)
#define P_EE_AUDIO_TODDR_B_CTRL2                   ((volatile uint32_t *)0xfe050168)
#define P_EE_AUDIO_TODDR_C_CTRL0                   ((volatile uint32_t *)0xfe050180)
#define P_EE_AUDIO_TODDR_C_CTRL1                   ((volatile uint32_t *)0xfe050184)
#define P_EE_AUDIO_TODDR_C_START_ADDR              ((volatile uint32_t *)0xfe050188)
#define P_EE_AUDIO_TODDR_C_FINISH_ADDR             ((volatile uint32_t *)0xfe05018c)
#define P_EE_AUDIO_TODDR_C_INT_ADDR                ((volatile uint32_t *)0xfe050190)
#define P_EE_AUDIO_TODDR_C_STATUS1                 ((volatile uint32_t *)0xfe050194)
#define P_EE_AUDIO_TODDR_C_STATUS2                 ((volatile uint32_t *)0xfe050198)
#define P_EE_AUDIO_TODDR_C_START_ADDRB             ((volatile uint32_t *)0xfe05019c)
#define P_EE_AUDIO_TODDR_C_FINISH_ADDRB            ((volatile uint32_t *)0xfe0501a0)
#define P_EE_AUDIO_TODDR_C_INIT_ADDR               ((volatile uint32_t *)0xfe0501a4)
#define P_EE_AUDIO_TODDR_C_CTRL2                   ((volatile uint32_t *)0xfe0501a8)
#define P_EE_AUDIO_FRDDR_A_CTRL0                   ((volatile uint32_t *)0xfe0501c0)
#define P_EE_AUDIO_FRDDR_A_CTRL1                   ((volatile uint32_t *)0xfe0501c4)
#define P_EE_AUDIO_FRDDR_A_START_ADDR              ((volatile uint32_t *)0xfe0501c8)
#define P_EE_AUDIO_FRDDR_A_FINISH_ADDR             ((volatile uint32_t *)0xfe0501cc)
#define P_EE_AUDIO_FRDDR_A_INT_ADDR                ((volatile uint32_t *)0xfe0501d0)
#define P_EE_AUDIO_FRDDR_A_STATUS1                 ((volatile uint32_t *)0xfe0501d4)
#define P_EE_AUDIO_FRDDR_A_STATUS2                 ((volatile uint32_t *)0xfe0501d8)
#define P_EE_AUDIO_FRDDR_A_START_ADDRB             ((volatile uint32_t *)0xfe0501dc)
#define P_EE_AUDIO_FRDDR_A_FINISH_ADDRB            ((volatile uint32_t *)0xfe0501e0)
#define P_EE_AUDIO_FRDDR_A_INIT_ADDR               ((volatile uint32_t *)0xfe0501e4)
#define P_EE_AUDIO_FRDDR_A_CTRL2                   ((volatile uint32_t *)0xfe0501e8)
#define P_EE_AUDIO_FRDDR_B_CTRL0                   ((volatile uint32_t *)0xfe050200)
#define P_EE_AUDIO_FRDDR_B_CTRL1                   ((volatile uint32_t *)0xfe050204)
#define P_EE_AUDIO_FRDDR_B_START_ADDR              ((volatile uint32_t *)0xfe050208)
#define P_EE_AUDIO_FRDDR_B_FINISH_ADDR             ((volatile uint32_t *)0xfe05020c)
#define P_EE_AUDIO_FRDDR_B_INT_ADDR                ((volatile uint32_t *)0xfe050210)
#define P_EE_AUDIO_FRDDR_B_STATUS1                 ((volatile uint32_t *)0xfe050214)
#define P_EE_AUDIO_FRDDR_B_STATUS2                 ((volatile uint32_t *)0xfe050218)
#define P_EE_AUDIO_FRDDR_B_START_ADDRB             ((volatile uint32_t *)0xfe05021c)
#define P_EE_AUDIO_FRDDR_B_FINISH_ADDRB            ((volatile uint32_t *)0xfe050220)
#define P_EE_AUDIO_FRDDR_B_INIT_ADDR               ((volatile uint32_t *)0xfe050224)
#define P_EE_AUDIO_FRDDR_B_CTRL2                   ((volatile uint32_t *)0xfe050228)
#define P_EE_AUDIO_FRDDR_C_CTRL0                   ((volatile uint32_t *)0xfe050240)
#define P_EE_AUDIO_FRDDR_C_CTRL1                   ((volatile uint32_t *)0xfe050244)
#define P_EE_AUDIO_FRDDR_C_START_ADDR              ((volatile uint32_t *)0xfe050248)
#define P_EE_AUDIO_FRDDR_C_FINISH_ADDR             ((volatile uint32_t *)0xfe05024c)
#define P_EE_AUDIO_FRDDR_C_INT_ADDR                ((volatile uint32_t *)0xfe050250)
#define P_EE_AUDIO_FRDDR_C_STATUS1                 ((volatile uint32_t *)0xfe050254)
#define P_EE_AUDIO_FRDDR_C_STATUS2                 ((volatile uint32_t *)0xfe050258)
#define P_EE_AUDIO_FRDDR_C_START_ADDRB             ((volatile uint32_t *)0xfe05025c)
#define P_EE_AUDIO_FRDDR_C_FINISH_ADDRB            ((volatile uint32_t *)0xfe050260)
#define P_EE_AUDIO_FRDDR_C_INIT_ADDR               ((volatile uint32_t *)0xfe050264)
#define P_EE_AUDIO_FRDDR_C_CTRL2                   ((volatile uint32_t *)0xfe050268)
#define P_EE_AUDIO_ARB_CTRL                        ((volatile uint32_t *)0xfe050280)
#define P_EE_AUDIO_LB_A_CTRL0                      ((volatile uint32_t *)0xfe0502c0)
#define P_EE_AUDIO_LB_A_CTRL1                      ((volatile uint32_t *)0xfe0502c4)
#define P_EE_AUDIO_LB_A_CTRL2                      ((volatile uint32_t *)0xfe0502c8)
#define P_EE_AUDIO_LB_A_CTRL3                      ((volatile uint32_t *)0xfe0502cc)
#define P_EE_AUDIO_LB_A_DAT_CH_ID0                 ((volatile uint32_t *)0xfe0502d0)
#define P_EE_AUDIO_LB_A_DAT_CH_ID1                 ((volatile uint32_t *)0xfe0502d4)
#define P_EE_AUDIO_LB_A_DAT_CH_ID2                 ((volatile uint32_t *)0xfe0502d8)
#define P_EE_AUDIO_LB_A_DAT_CH_ID3                 ((volatile uint32_t *)0xfe0502dc)
#define P_EE_AUDIO_LB_A_LB_CH_ID0                  ((volatile uint32_t *)0xfe0502e0)
#define P_EE_AUDIO_LB_A_LB_CH_ID1                  ((volatile uint32_t *)0xfe0502e4)
#define P_EE_AUDIO_LB_A_LB_CH_ID2                  ((volatile uint32_t *)0xfe0502e8)
#define P_EE_AUDIO_LB_A_LB_CH_ID3                  ((volatile uint32_t *)0xfe0502ec)
#define P_EE_AUDIO_LB_A_STS                        ((volatile uint32_t *)0xfe0502f0)
#define P_EE_AUDIO_TDMIN_A_CTRL                    ((volatile uint32_t *)0xfe050300)
#define P_EE_AUDIO_TDMIN_A_SWAP0                   ((volatile uint32_t *)0xfe050304)
#define P_EE_AUDIO_TDMIN_A_MASK0                   ((volatile uint32_t *)0xfe050308)
#define P_EE_AUDIO_TDMIN_A_MASK1                   ((volatile uint32_t *)0xfe05030c)
#define P_EE_AUDIO_TDMIN_A_MASK2                   ((volatile uint32_t *)0xfe050310)
#define P_EE_AUDIO_TDMIN_A_MASK3                   ((volatile uint32_t *)0xfe050314)
#define P_EE_AUDIO_TDMIN_A_STAT                    ((volatile uint32_t *)0xfe050318)
#define P_EE_AUDIO_TDMIN_A_MUTE_VAL                ((volatile uint32_t *)0xfe05031c)
#define P_EE_AUDIO_TDMIN_A_MUTE0                   ((volatile uint32_t *)0xfe050320)
#define P_EE_AUDIO_TDMIN_A_MUTE1                   ((volatile uint32_t *)0xfe050324)
#define P_EE_AUDIO_TDMIN_A_MUTE2                   ((volatile uint32_t *)0xfe050328)
#define P_EE_AUDIO_TDMIN_A_MUTE3                   ((volatile uint32_t *)0xfe05032c)
#define P_EE_AUDIO_TDMIN_B_CTRL                    ((volatile uint32_t *)0xfe050340)
#define P_EE_AUDIO_TDMIN_B_SWAP0                   ((volatile uint32_t *)0xfe050344)
#define P_EE_AUDIO_TDMIN_B_MASK0                   ((volatile uint32_t *)0xfe050348)
#define P_EE_AUDIO_TDMIN_B_MASK1                   ((volatile uint32_t *)0xfe05034c)
#define P_EE_AUDIO_TDMIN_B_MASK2                   ((volatile uint32_t *)0xfe050350)
#define P_EE_AUDIO_TDMIN_B_MASK3                   ((volatile uint32_t *)0xfe050354)
#define P_EE_AUDIO_TDMIN_B_STAT                    ((volatile uint32_t *)0xfe050358)
#define P_EE_AUDIO_TDMIN_B_MUTE_VAL                ((volatile uint32_t *)0xfe05035c)
#define P_EE_AUDIO_TDMIN_B_MUTE0                   ((volatile uint32_t *)0xfe050360)
#define P_EE_AUDIO_TDMIN_B_MUTE1                   ((volatile uint32_t *)0xfe050364)
#define P_EE_AUDIO_TDMIN_B_MUTE2                   ((volatile uint32_t *)0xfe050368)
#define P_EE_AUDIO_TDMIN_B_MUTE3                   ((volatile uint32_t *)0xfe05036c)
#define P_EE_AUDIO_TDMIN_C_CTRL                    ((volatile uint32_t *)0xfe050380)
#define P_EE_AUDIO_TDMIN_C_SWAP0                   ((volatile uint32_t *)0xfe050384)
#define P_EE_AUDIO_TDMIN_C_MASK0                   ((volatile uint32_t *)0xfe050388)
#define P_EE_AUDIO_TDMIN_C_MASK1                   ((volatile uint32_t *)0xfe05038c)
#define P_EE_AUDIO_TDMIN_C_MASK2                   ((volatile uint32_t *)0xfe050390)
#define P_EE_AUDIO_TDMIN_C_MASK3                   ((volatile uint32_t *)0xfe050394)
#define P_EE_AUDIO_TDMIN_C_STAT                    ((volatile uint32_t *)0xfe050398)
#define P_EE_AUDIO_TDMIN_C_MUTE_VAL                ((volatile uint32_t *)0xfe05039c)
#define P_EE_AUDIO_TDMIN_C_MUTE0                   ((volatile uint32_t *)0xfe0503a0)
#define P_EE_AUDIO_TDMIN_C_MUTE1                   ((volatile uint32_t *)0xfe0503a4)
#define P_EE_AUDIO_TDMIN_C_MUTE2                   ((volatile uint32_t *)0xfe0503a8)
#define P_EE_AUDIO_TDMIN_C_MUTE3                   ((volatile uint32_t *)0xfe0503ac)
#define P_EE_AUDIO_TDMIN_LB_CTRL                   ((volatile uint32_t *)0xfe0503c0)
#define P_EE_AUDIO_TDMIN_LB_SWAP0                  ((volatile uint32_t *)0xfe0503c4)
#define P_EE_AUDIO_TDMIN_LB_MASK0                  ((volatile uint32_t *)0xfe0503c8)
#define P_EE_AUDIO_TDMIN_LB_MASK1                  ((volatile uint32_t *)0xfe0503cc)
#define P_EE_AUDIO_TDMIN_LB_MASK2                  ((volatile uint32_t *)0xfe0503d0)
#define P_EE_AUDIO_TDMIN_LB_MASK3                  ((volatile uint32_t *)0xfe0503d4)
#define P_EE_AUDIO_TDMIN_LB_STAT                   ((volatile uint32_t *)0xfe0503d8)
#define P_EE_AUDIO_TDMIN_LB_MUTE_VAL               ((volatile uint32_t *)0xfe0503dc)
#define P_EE_AUDIO_TDMIN_LB_MUTE0                  ((volatile uint32_t *)0xfe0503e0)
#define P_EE_AUDIO_TDMIN_LB_MUTE1                  ((volatile uint32_t *)0xfe0503e4)
#define P_EE_AUDIO_TDMIN_LB_MUTE2                  ((volatile uint32_t *)0xfe0503e8)
#define P_EE_AUDIO_TDMIN_LB_MUTE3                  ((volatile uint32_t *)0xfe0503ec)
#define P_EE_AUDIO_SPDIFIN_CTRL0                   ((volatile uint32_t *)0xfe050400)
#define P_EE_AUDIO_SPDIFIN_CTRL1                   ((volatile uint32_t *)0xfe050404)
#define P_EE_AUDIO_SPDIFIN_CTRL2                   ((volatile uint32_t *)0xfe050408)
#define P_EE_AUDIO_SPDIFIN_CTRL3                   ((volatile uint32_t *)0xfe05040c)
#define P_EE_AUDIO_SPDIFIN_CTRL4                   ((volatile uint32_t *)0xfe050410)
#define P_EE_AUDIO_SPDIFIN_CTRL5                   ((volatile uint32_t *)0xfe050414)
#define P_EE_AUDIO_SPDIFIN_CTRL6                   ((volatile uint32_t *)0xfe050418)
#define P_EE_AUDIO_SPDIFIN_STAT0                   ((volatile uint32_t *)0xfe05041c)
#define P_EE_AUDIO_SPDIFIN_STAT1                   ((volatile uint32_t *)0xfe050420)
#define P_EE_AUDIO_SPDIFIN_STAT2                   ((volatile uint32_t *)0xfe050424)
#define P_EE_AUDIO_SPDIFIN_MUTE_VAL                ((volatile uint32_t *)0xfe050428)
#define P_EE_AUDIO_RESAMPLEA_CTRL0                 ((volatile uint32_t *)0xfe050440)
#define P_EE_AUDIO_RESAMPLEA_CTRL1                 ((volatile uint32_t *)0xfe050444)
#define P_EE_AUDIO_RESAMPLEA_CTRL2                 ((volatile uint32_t *)0xfe050448)
#define P_EE_AUDIO_RESAMPLEA_CTRL3                 ((volatile uint32_t *)0xfe05044c)
#define P_EE_AUDIO_RESAMPLEA_COEF0                 ((volatile uint32_t *)0xfe050450)
#define P_EE_AUDIO_RESAMPLEA_COEF1                 ((volatile uint32_t *)0xfe050454)
#define P_EE_AUDIO_RESAMPLEA_COEF2                 ((volatile uint32_t *)0xfe050458)
#define P_EE_AUDIO_RESAMPLEA_COEF3                 ((volatile uint32_t *)0xfe05045c)
#define P_EE_AUDIO_RESAMPLEA_COEF4                 ((volatile uint32_t *)0xfe050460)
#define P_EE_AUDIO_RESAMPLEA_STATUS1               ((volatile uint32_t *)0xfe050464)
#define P_EE_AUDIO_SPDIFOUT_STAT                   ((volatile uint32_t *)0xfe050480)
#define P_EE_AUDIO_SPDIFOUT_GAIN0                  ((volatile uint32_t *)0xfe050484)
#define P_EE_AUDIO_SPDIFOUT_GAIN1                  ((volatile uint32_t *)0xfe050488)
#define P_EE_AUDIO_SPDIFOUT_CTRL0                  ((volatile uint32_t *)0xfe05048c)
#define P_EE_AUDIO_SPDIFOUT_CTRL1                  ((volatile uint32_t *)0xfe050490)
#define P_EE_AUDIO_SPDIFOUT_PREAMB                 ((volatile uint32_t *)0xfe050494)
#define P_EE_AUDIO_SPDIFOUT_SWAP                   ((volatile uint32_t *)0xfe050498)
#define P_EE_AUDIO_SPDIFOUT_CHSTS0                 ((volatile uint32_t *)0xfe05049c)
#define P_EE_AUDIO_SPDIFOUT_CHSTS1                 ((volatile uint32_t *)0xfe0504a0)
#define P_EE_AUDIO_SPDIFOUT_CHSTS2                 ((volatile uint32_t *)0xfe0504a4)
#define P_EE_AUDIO_SPDIFOUT_CHSTS3                 ((volatile uint32_t *)0xfe0504a8)
#define P_EE_AUDIO_SPDIFOUT_CHSTS4                 ((volatile uint32_t *)0xfe0504ac)
#define P_EE_AUDIO_SPDIFOUT_CHSTS5                 ((volatile uint32_t *)0xfe0504b0)
#define P_EE_AUDIO_SPDIFOUT_CHSTS6                 ((volatile uint32_t *)0xfe0504b4)
#define P_EE_AUDIO_SPDIFOUT_CHSTS7                 ((volatile uint32_t *)0xfe0504b8)
#define P_EE_AUDIO_SPDIFOUT_CHSTS8                 ((volatile uint32_t *)0xfe0504bc)
#define P_EE_AUDIO_SPDIFOUT_CHSTS9                 ((volatile uint32_t *)0xfe0504c0)
#define P_EE_AUDIO_SPDIFOUT_CHSTSA                 ((volatile uint32_t *)0xfe0504c4)
#define P_EE_AUDIO_SPDIFOUT_CHSTSB                 ((volatile uint32_t *)0xfe0504c8)
#define P_EE_AUDIO_SPDIFOUT_MUTE_VAL               ((volatile uint32_t *)0xfe0504cc)
#define P_EE_AUDIO_TDMOUT_A_CTRL0                  ((volatile uint32_t *)0xfe050500)
#define P_EE_AUDIO_TDMOUT_A_CTRL1                  ((volatile uint32_t *)0xfe050504)
#define P_EE_AUDIO_TDMOUT_A_SWAP0                  ((volatile uint32_t *)0xfe050508)
#define P_EE_AUDIO_TDMOUT_A_MASK0                  ((volatile uint32_t *)0xfe05050c)
#define P_EE_AUDIO_TDMOUT_A_MASK1                  ((volatile uint32_t *)0xfe050510)
#define P_EE_AUDIO_TDMOUT_A_MASK2                  ((volatile uint32_t *)0xfe050514)
#define P_EE_AUDIO_TDMOUT_A_MASK3                  ((volatile uint32_t *)0xfe050518)
#define P_EE_AUDIO_TDMOUT_A_STAT                   ((volatile uint32_t *)0xfe05051c)
#define P_EE_AUDIO_TDMOUT_A_GAIN0                  ((volatile uint32_t *)0xfe050520)
#define P_EE_AUDIO_TDMOUT_A_GAIN1                  ((volatile uint32_t *)0xfe050524)
#define P_EE_AUDIO_TDMOUT_A_MUTE_VAL               ((volatile uint32_t *)0xfe050528)
#define P_EE_AUDIO_TDMOUT_A_MUTE0                  ((volatile uint32_t *)0xfe05052c)
#define P_EE_AUDIO_TDMOUT_A_MUTE1                  ((volatile uint32_t *)0xfe050530)
#define P_EE_AUDIO_TDMOUT_A_MUTE2                  ((volatile uint32_t *)0xfe050534)
#define P_EE_AUDIO_TDMOUT_A_MUTE3                  ((volatile uint32_t *)0xfe050538)
#define P_EE_AUDIO_TDMOUT_A_MASK_VAL               ((volatile uint32_t *)0xfe05053c)
#define P_EE_AUDIO_TDMOUT_B_CTRL0                  ((volatile uint32_t *)0xfe050540)
#define P_EE_AUDIO_TDMOUT_B_CTRL1                  ((volatile uint32_t *)0xfe050544)
#define P_EE_AUDIO_TDMOUT_B_SWAP0                  ((volatile uint32_t *)0xfe050548)
#define P_EE_AUDIO_TDMOUT_B_MASK0                  ((volatile uint32_t *)0xfe05054c)
#define P_EE_AUDIO_TDMOUT_B_MASK1                  ((volatile uint32_t *)0xfe050550)
#define P_EE_AUDIO_TDMOUT_B_MASK2                  ((volatile uint32_t *)0xfe050554)
#define P_EE_AUDIO_TDMOUT_B_MASK3                  ((volatile uint32_t *)0xfe050558)
#define P_EE_AUDIO_TDMOUT_B_STAT                   ((volatile uint32_t *)0xfe05055c)
#define P_EE_AUDIO_TDMOUT_B_GAIN0                  ((volatile uint32_t *)0xfe050560)
#define P_EE_AUDIO_TDMOUT_B_GAIN1                  ((volatile uint32_t *)0xfe050564)
#define P_EE_AUDIO_TDMOUT_B_MUTE_VAL               ((volatile uint32_t *)0xfe050568)
#define P_EE_AUDIO_TDMOUT_B_MUTE0                  ((volatile uint32_t *)0xfe05056c)
#define P_EE_AUDIO_TDMOUT_B_MUTE1                  ((volatile uint32_t *)0xfe050570)
#define P_EE_AUDIO_TDMOUT_B_MUTE2                  ((volatile uint32_t *)0xfe050574)
#define P_EE_AUDIO_TDMOUT_B_MUTE3                  ((volatile uint32_t *)0xfe050578)
#define P_EE_AUDIO_TDMOUT_B_MASK_VAL               ((volatile uint32_t *)0xfe05057c)
#define P_EE_AUDIO_TDMOUT_C_CTRL0                  ((volatile uint32_t *)0xfe050580)
#define P_EE_AUDIO_TDMOUT_C_CTRL1                  ((volatile uint32_t *)0xfe050584)
#define P_EE_AUDIO_TDMOUT_C_SWAP0                  ((volatile uint32_t *)0xfe050588)
#define P_EE_AUDIO_TDMOUT_C_MASK0                  ((volatile uint32_t *)0xfe05058c)
#define P_EE_AUDIO_TDMOUT_C_MASK1                  ((volatile uint32_t *)0xfe050590)
#define P_EE_AUDIO_TDMOUT_C_MASK2                  ((volatile uint32_t *)0xfe050594)
#define P_EE_AUDIO_TDMOUT_C_MASK3                  ((volatile uint32_t *)0xfe050598)
#define P_EE_AUDIO_TDMOUT_C_STAT                   ((volatile uint32_t *)0xfe05059c)
#define P_EE_AUDIO_TDMOUT_C_GAIN0                  ((volatile uint32_t *)0xfe0505a0)
#define P_EE_AUDIO_TDMOUT_C_GAIN1                  ((volatile uint32_t *)0xfe0505a4)
#define P_EE_AUDIO_TDMOUT_C_MUTE_VAL               ((volatile uint32_t *)0xfe0505a8)
#define P_EE_AUDIO_TDMOUT_C_MUTE0                  ((volatile uint32_t *)0xfe0505ac)
#define P_EE_AUDIO_TDMOUT_C_MUTE1                  ((volatile uint32_t *)0xfe0505b0)
#define P_EE_AUDIO_TDMOUT_C_MUTE2                  ((volatile uint32_t *)0xfe0505b4)
#define P_EE_AUDIO_TDMOUT_C_MUTE3                  ((volatile uint32_t *)0xfe0505b8)
#define P_EE_AUDIO_TDMOUT_C_MASK_VAL               ((volatile uint32_t *)0xfe0505bc)
//`define EE_AUDIO_POW_DET_CTRL0          10'h180
//`define EE_AUDIO_POW_DET_CTRL1          10'h181
//`define EE_AUDIO_POW_DET_TH_HI          10'h182
//`define EE_AUDIO_POW_DET_TH_LO          10'h183
//`define EE_AUDIO_POW_DET_VALUE          10'h184
#define P_EE_AUDIO_SECURITY_CTRL0                  ((volatile uint32_t *)0xfe050640)
#define P_EE_AUDIO_SECURITY_CTRL1                  ((volatile uint32_t *)0xfe050644)
#define P_EE_AUDIO_SPDIFOUT_B_STAT                 ((volatile uint32_t *)0xfe050680)
#define P_EE_AUDIO_SPDIFOUT_B_GAIN0                ((volatile uint32_t *)0xfe050684)
#define P_EE_AUDIO_SPDIFOUT_B_GAIN1                ((volatile uint32_t *)0xfe050688)
#define P_EE_AUDIO_SPDIFOUT_B_CTRL0                ((volatile uint32_t *)0xfe05068c)
#define P_EE_AUDIO_SPDIFOUT_B_CTRL1                ((volatile uint32_t *)0xfe050690)
#define P_EE_AUDIO_SPDIFOUT_B_PREAMB               ((volatile uint32_t *)0xfe050694)
#define P_EE_AUDIO_SPDIFOUT_B_SWAP                 ((volatile uint32_t *)0xfe050698)
#define P_EE_AUDIO_SPDIFOUT_B_CHSTS0               ((volatile uint32_t *)0xfe05069c)
#define P_EE_AUDIO_SPDIFOUT_B_CHSTS1               ((volatile uint32_t *)0xfe0506a0)
#define P_EE_AUDIO_SPDIFOUT_B_CHSTS2               ((volatile uint32_t *)0xfe0506a4)
#define P_EE_AUDIO_SPDIFOUT_B_CHSTS3               ((volatile uint32_t *)0xfe0506a8)
#define P_EE_AUDIO_SPDIFOUT_B_CHSTS4               ((volatile uint32_t *)0xfe0506ac)
#define P_EE_AUDIO_SPDIFOUT_B_CHSTS5               ((volatile uint32_t *)0xfe0506b0)
#define P_EE_AUDIO_SPDIFOUT_B_CHSTS6               ((volatile uint32_t *)0xfe0506b4)
#define P_EE_AUDIO_SPDIFOUT_B_CHSTS7               ((volatile uint32_t *)0xfe0506b8)
#define P_EE_AUDIO_SPDIFOUT_B_CHSTS8               ((volatile uint32_t *)0xfe0506bc)
#define P_EE_AUDIO_SPDIFOUT_B_CHSTS9               ((volatile uint32_t *)0xfe0506c0)
#define P_EE_AUDIO_SPDIFOUT_B_CHSTSA               ((volatile uint32_t *)0xfe0506c4)
#define P_EE_AUDIO_SPDIFOUT_B_CHSTSB               ((volatile uint32_t *)0xfe0506c8)
#define P_EE_AUDIO_SPDIFOUT_B_MUTE_VAL             ((volatile uint32_t *)0xfe0506cc)
#define P_EE_AUDIO_TORAM_CTRL0                     ((volatile uint32_t *)0xfe050700)
#define P_EE_AUDIO_TORAM_CTRL1                     ((volatile uint32_t *)0xfe050704)
#define P_EE_AUDIO_TORAM_START_ADDR                ((volatile uint32_t *)0xfe050708)
#define P_EE_AUDIO_TORAM_FINISH_ADDR               ((volatile uint32_t *)0xfe05070c)
#define P_EE_AUDIO_TORAM_INT_ADDR                  ((volatile uint32_t *)0xfe050710)
#define P_EE_AUDIO_TORAM_STATUS1                   ((volatile uint32_t *)0xfe050714)
#define P_EE_AUDIO_TORAM_STATUS2                   ((volatile uint32_t *)0xfe050718)
#define P_EE_AUDIO_TORAM_INIT_ADDR                 ((volatile uint32_t *)0xfe05071c)
#define P_EE_AUDIO_TOACODEC_CTRL0                  ((volatile uint32_t *)0xfe050740)
#define P_EE_AUDIO_TOHDMITX_CTRL0                  ((volatile uint32_t *)0xfe050744)
#define P_EE_AUDIO_TOVAD_CTRL0                     ((volatile uint32_t *)0xfe050748)
#define P_EE_AUDIO_FRATV_CTRL0                     ((volatile uint32_t *)0xfe05074c)
#define P_EE_AUDIO_RESAMPLEB_CTRL0                 ((volatile uint32_t *)0xfe050780)
#define P_EE_AUDIO_RESAMPLEB_CTRL1                 ((volatile uint32_t *)0xfe050784)
#define P_EE_AUDIO_RESAMPLEB_CTRL2                 ((volatile uint32_t *)0xfe050788)
#define P_EE_AUDIO_RESAMPLEB_CTRL3                 ((volatile uint32_t *)0xfe05078c)
#define P_EE_AUDIO_RESAMPLEB_COEF0                 ((volatile uint32_t *)0xfe050790)
#define P_EE_AUDIO_RESAMPLEB_COEF1                 ((volatile uint32_t *)0xfe050794)
#define P_EE_AUDIO_RESAMPLEB_COEF2                 ((volatile uint32_t *)0xfe050798)
#define P_EE_AUDIO_RESAMPLEB_COEF3                 ((volatile uint32_t *)0xfe05079c)
#define P_EE_AUDIO_RESAMPLEB_COEF4                 ((volatile uint32_t *)0xfe0507a0)
#define P_EE_AUDIO_RESAMPLEB_STATUS1               ((volatile uint32_t *)0xfe0507a4)
#define P_EE_AUDIO_SPDIFIN_LB_CTRL0                ((volatile uint32_t *)0xfe0507c0)
#define P_EE_AUDIO_SPDIFIN_LB_CTRL1                ((volatile uint32_t *)0xfe0507c4)
#define P_EE_AUDIO_SPDIFIN_LB_CTRL6                ((volatile uint32_t *)0xfe0507d8)
#define P_EE_AUDIO_SPDIFIN_LB_STAT0                ((volatile uint32_t *)0xfe0507dc)
#define P_EE_AUDIO_SPDIFIN_LB_STAT1                ((volatile uint32_t *)0xfe0507e0)
#define P_EE_AUDIO_SPDIFIN_LB_MUTE_VAL             ((volatile uint32_t *)0xfe0507e8)
#define P_EE_AUDIO_FRHDMIRX_CTRL0                  ((volatile uint32_t *)0xfe050800)
#define P_EE_AUDIO_FRHDMIRX_CTRL1                  ((volatile uint32_t *)0xfe050804)
#define P_EE_AUDIO_FRHDMIRX_CTRL2                  ((volatile uint32_t *)0xfe050808)
#define P_EE_AUDIO_FRHDMIRX_CTRL3                  ((volatile uint32_t *)0xfe05080c)
#define P_EE_AUDIO_FRHDMIRX_CTRL4                  ((volatile uint32_t *)0xfe050810)
#define P_EE_AUDIO_FRHDMIRX_CTRL5                  ((volatile uint32_t *)0xfe050814)
#define P_EE_AUDIO_FRHDMIRX_STAT0                  ((volatile uint32_t *)0xfe050828)
#define P_EE_AUDIO_FRHDMIRX_STAT1                  ((volatile uint32_t *)0xfe05082c)
#define P_EE_AUDIO_TODDR_D_CTRL0                   ((volatile uint32_t *)0xfe050840)
#define P_EE_AUDIO_TODDR_D_CTRL1                   ((volatile uint32_t *)0xfe050844)
#define P_EE_AUDIO_TODDR_D_START_ADDR              ((volatile uint32_t *)0xfe050848)
#define P_EE_AUDIO_TODDR_D_FINISH_ADDR             ((volatile uint32_t *)0xfe05084c)
#define P_EE_AUDIO_TODDR_D_INT_ADDR                ((volatile uint32_t *)0xfe050850)
#define P_EE_AUDIO_TODDR_D_STATUS1                 ((volatile uint32_t *)0xfe050854)
#define P_EE_AUDIO_TODDR_D_STATUS2                 ((volatile uint32_t *)0xfe050858)
#define P_EE_AUDIO_TODDR_D_START_ADDRB             ((volatile uint32_t *)0xfe05085c)
#define P_EE_AUDIO_TODDR_D_FINISH_ADDRB            ((volatile uint32_t *)0xfe050860)
#define P_EE_AUDIO_TODDR_D_INIT_ADDR               ((volatile uint32_t *)0xfe050864)
#define P_EE_AUDIO_TODDR_D_CTRL2                   ((volatile uint32_t *)0xfe050868)
#define P_EE_AUDIO_FRDDR_D_CTRL0                   ((volatile uint32_t *)0xfe050880)
#define P_EE_AUDIO_FRDDR_D_CTRL1                   ((volatile uint32_t *)0xfe050884)
#define P_EE_AUDIO_FRDDR_D_START_ADDR              ((volatile uint32_t *)0xfe050888)
#define P_EE_AUDIO_FRDDR_D_FINISH_ADDR             ((volatile uint32_t *)0xfe05088c)
#define P_EE_AUDIO_FRDDR_D_INT_ADDR                ((volatile uint32_t *)0xfe050890)
#define P_EE_AUDIO_FRDDR_D_STATUS1                 ((volatile uint32_t *)0xfe050894)
#define P_EE_AUDIO_FRDDR_D_STATUS2                 ((volatile uint32_t *)0xfe050898)
#define P_EE_AUDIO_FRDDR_D_START_ADDRB             ((volatile uint32_t *)0xfe05089c)
#define P_EE_AUDIO_FRDDR_D_FINISH_ADDRB            ((volatile uint32_t *)0xfe0508a0)
#define P_EE_AUDIO_FRDDR_D_INIT_ADDR               ((volatile uint32_t *)0xfe0508a4)
#define P_EE_AUDIO_FRDDR_D_CTRL2                   ((volatile uint32_t *)0xfe0508a8)
#define P_EE_AUDIO_LB_B_CTRL0                      ((volatile uint32_t *)0xfe0508c0)
#define P_EE_AUDIO_LB_B_CTRL1                      ((volatile uint32_t *)0xfe0508c4)
#define P_EE_AUDIO_LB_B_CTRL2                      ((volatile uint32_t *)0xfe0508c8)
#define P_EE_AUDIO_LB_B_CTRL3                      ((volatile uint32_t *)0xfe0508cc)
#define P_EE_AUDIO_LB_B_DAT_CH_ID0                 ((volatile uint32_t *)0xfe0508d0)
#define P_EE_AUDIO_LB_B_DAT_CH_ID1                 ((volatile uint32_t *)0xfe0508d4)
#define P_EE_AUDIO_LB_B_DAT_CH_ID2                 ((volatile uint32_t *)0xfe0508d8)
#define P_EE_AUDIO_LB_B_DAT_CH_ID3                 ((volatile uint32_t *)0xfe0508dc)
#define P_EE_AUDIO_LB_B_LB_CH_ID0                  ((volatile uint32_t *)0xfe0508e0)
#define P_EE_AUDIO_LB_B_LB_CH_ID1                  ((volatile uint32_t *)0xfe0508e4)
#define P_EE_AUDIO_LB_B_LB_CH_ID2                  ((volatile uint32_t *)0xfe0508e8)
#define P_EE_AUDIO_LB_B_LB_CH_ID3                  ((volatile uint32_t *)0xfe0508ec)
#define P_EE_AUDIO_LB_B_STS                        ((volatile uint32_t *)0xfe0508f0)
#define P_EE_AUDIO_TODDR_E_CTRL0                   ((volatile uint32_t *)0xfe050900)
#define P_EE_AUDIO_TODDR_E_CTRL1                   ((volatile uint32_t *)0xfe050904)
#define P_EE_AUDIO_TODDR_E_START_ADDR              ((volatile uint32_t *)0xfe050908)
#define P_EE_AUDIO_TODDR_E_FINISH_ADDR             ((volatile uint32_t *)0xfe05090c)
#define P_EE_AUDIO_TODDR_E_INT_ADDR                ((volatile uint32_t *)0xfe050910)
#define P_EE_AUDIO_TODDR_E_STATUS1                 ((volatile uint32_t *)0xfe050914)
#define P_EE_AUDIO_TODDR_E_STATUS2                 ((volatile uint32_t *)0xfe050918)
#define P_EE_AUDIO_TODDR_E_START_ADDRB             ((volatile uint32_t *)0xfe05091c)
#define P_EE_AUDIO_TODDR_E_FINISH_ADDRB            ((volatile uint32_t *)0xfe050920)
#define P_EE_AUDIO_TODDR_E_INIT_ADDR               ((volatile uint32_t *)0xfe050924)
#define P_EE_AUDIO_TODDR_E_CTRL2                   ((volatile uint32_t *)0xfe050928)
#define P_EE_AUDIO_FRDDR_E_CTRL0                   ((volatile uint32_t *)0xfe050940)
#define P_EE_AUDIO_FRDDR_E_CTRL1                   ((volatile uint32_t *)0xfe050944)
#define P_EE_AUDIO_FRDDR_E_START_ADDR              ((volatile uint32_t *)0xfe050948)
#define P_EE_AUDIO_FRDDR_E_FINISH_ADDR             ((volatile uint32_t *)0xfe05094c)
#define P_EE_AUDIO_FRDDR_E_INT_ADDR                ((volatile uint32_t *)0xfe050950)
#define P_EE_AUDIO_FRDDR_E_STATUS1                 ((volatile uint32_t *)0xfe050954)
#define P_EE_AUDIO_FRDDR_E_STATUS2                 ((volatile uint32_t *)0xfe050958)
#define P_EE_AUDIO_FRDDR_E_START_ADDRB             ((volatile uint32_t *)0xfe05095c)
#define P_EE_AUDIO_FRDDR_E_FINISH_ADDRB            ((volatile uint32_t *)0xfe050960)
#define P_EE_AUDIO_FRDDR_E_INIT_ADDR               ((volatile uint32_t *)0xfe050964)
#define P_EE_AUDIO_FRDDR_E_CTRL2                   ((volatile uint32_t *)0xfe050968)
#define P_EE_AUDIO_TDMIN_A_SWAP1                   ((volatile uint32_t *)0xfe050980)
#define P_EE_AUDIO_TDMIN_A_MASK4                   ((volatile uint32_t *)0xfe050984)
#define P_EE_AUDIO_TDMIN_A_MASK5                   ((volatile uint32_t *)0xfe050988)
#define P_EE_AUDIO_TDMIN_A_MASK6                   ((volatile uint32_t *)0xfe05098c)
#define P_EE_AUDIO_TDMIN_A_MASK7                   ((volatile uint32_t *)0xfe050990)
#define P_EE_AUDIO_TDMIN_A_MUTE4                   ((volatile uint32_t *)0xfe050994)
#define P_EE_AUDIO_TDMIN_A_MUTE5                   ((volatile uint32_t *)0xfe050998)
#define P_EE_AUDIO_TDMIN_A_MUTE6                   ((volatile uint32_t *)0xfe05099c)
#define P_EE_AUDIO_TDMIN_A_MUTE7                   ((volatile uint32_t *)0xfe0509a0)
#define P_EE_AUDIO_TDMIN_B_SWAP1                   ((volatile uint32_t *)0xfe0509c0)
#define P_EE_AUDIO_TDMIN_B_MASK4                   ((volatile uint32_t *)0xfe0509c4)
#define P_EE_AUDIO_TDMIN_B_MASK5                   ((volatile uint32_t *)0xfe0509c8)
#define P_EE_AUDIO_TDMIN_B_MASK6                   ((volatile uint32_t *)0xfe0509cc)
#define P_EE_AUDIO_TDMIN_B_MASK7                   ((volatile uint32_t *)0xfe0509d0)
#define P_EE_AUDIO_TDMIN_B_MUTE4                   ((volatile uint32_t *)0xfe0509d4)
#define P_EE_AUDIO_TDMIN_B_MUTE5                   ((volatile uint32_t *)0xfe0509d8)
#define P_EE_AUDIO_TDMIN_B_MUTE6                   ((volatile uint32_t *)0xfe0509dc)
#define P_EE_AUDIO_TDMIN_B_MUTE7                   ((volatile uint32_t *)0xfe0509e0)
#define P_EE_AUDIO_TDMIN_C_SWAP1                   ((volatile uint32_t *)0xfe050a00)
#define P_EE_AUDIO_TDMIN_C_MASK4                   ((volatile uint32_t *)0xfe050a04)
#define P_EE_AUDIO_TDMIN_C_MASK5                   ((volatile uint32_t *)0xfe050a08)
#define P_EE_AUDIO_TDMIN_C_MASK6                   ((volatile uint32_t *)0xfe050a0c)
#define P_EE_AUDIO_TDMIN_C_MASK7                   ((volatile uint32_t *)0xfe050a10)
#define P_EE_AUDIO_TDMIN_C_MUTE4                   ((volatile uint32_t *)0xfe050a14)
#define P_EE_AUDIO_TDMIN_C_MUTE5                   ((volatile uint32_t *)0xfe050a18)
#define P_EE_AUDIO_TDMIN_C_MUTE6                   ((volatile uint32_t *)0xfe050a1c)
#define P_EE_AUDIO_TDMIN_C_MUTE7                   ((volatile uint32_t *)0xfe050a20)
#define P_EE_AUDIO_TDMIN_LB_SWAP1                  ((volatile uint32_t *)0xfe050a40)
#define P_EE_AUDIO_TDMIN_LB_MASK4                  ((volatile uint32_t *)0xfe050a44)
#define P_EE_AUDIO_TDMIN_LB_MASK5                  ((volatile uint32_t *)0xfe050a48)
#define P_EE_AUDIO_TDMIN_LB_MASK6                  ((volatile uint32_t *)0xfe050a4c)
#define P_EE_AUDIO_TDMIN_LB_MASK7                  ((volatile uint32_t *)0xfe050a50)
#define P_EE_AUDIO_TDMIN_LB_MUTE4                  ((volatile uint32_t *)0xfe050a54)
#define P_EE_AUDIO_TDMIN_LB_MUTE5                  ((volatile uint32_t *)0xfe050a58)
#define P_EE_AUDIO_TDMIN_LB_MUTE6                  ((volatile uint32_t *)0xfe050a5c)
#define P_EE_AUDIO_TDMIN_LB_MUTE7                  ((volatile uint32_t *)0xfe050a60)
#define P_EE_AUDIO_TDMOUT_A_CTRL2                  ((volatile uint32_t *)0xfe050a80)
#define P_EE_AUDIO_TDMOUT_A_SWAP1                  ((volatile uint32_t *)0xfe050a84)
#define P_EE_AUDIO_TDMOUT_A_GAIN2                  ((volatile uint32_t *)0xfe050a88)
#define P_EE_AUDIO_TDMOUT_A_GAIN3                  ((volatile uint32_t *)0xfe050a8c)
#define P_EE_AUDIO_TDMOUT_A_MASK4                  ((volatile uint32_t *)0xfe050a90)
#define P_EE_AUDIO_TDMOUT_A_MASK5                  ((volatile uint32_t *)0xfe050a94)
#define P_EE_AUDIO_TDMOUT_A_MASK6                  ((volatile uint32_t *)0xfe050a98)
#define P_EE_AUDIO_TDMOUT_A_MASK7                  ((volatile uint32_t *)0xfe050a9c)
#define P_EE_AUDIO_TDMOUT_A_MUTE4                  ((volatile uint32_t *)0xfe050aa0)
#define P_EE_AUDIO_TDMOUT_A_MUTE5                  ((volatile uint32_t *)0xfe050aa4)
#define P_EE_AUDIO_TDMOUT_A_MUTE6                  ((volatile uint32_t *)0xfe050aa8)
#define P_EE_AUDIO_TDMOUT_A_MUTE7                  ((volatile uint32_t *)0xfe050aac)
#define P_EE_AUDIO_TDMOUT_B_CTRL2                  ((volatile uint32_t *)0xfe050ac0)
#define P_EE_AUDIO_TDMOUT_B_SWAP1                  ((volatile uint32_t *)0xfe050ac4)
#define P_EE_AUDIO_TDMOUT_B_GAIN2                  ((volatile uint32_t *)0xfe050ac8)
#define P_EE_AUDIO_TDMOUT_B_GAIN3                  ((volatile uint32_t *)0xfe050acc)
#define P_EE_AUDIO_TDMOUT_B_MASK4                  ((volatile uint32_t *)0xfe050ad0)
#define P_EE_AUDIO_TDMOUT_B_MASK5                  ((volatile uint32_t *)0xfe050ad4)
#define P_EE_AUDIO_TDMOUT_B_MASK6                  ((volatile uint32_t *)0xfe050ad8)
#define P_EE_AUDIO_TDMOUT_B_MASK7                  ((volatile uint32_t *)0xfe050adc)
#define P_EE_AUDIO_TDMOUT_B_MUTE4                  ((volatile uint32_t *)0xfe050ae0)
#define P_EE_AUDIO_TDMOUT_B_MUTE5                  ((volatile uint32_t *)0xfe050ae4)
#define P_EE_AUDIO_TDMOUT_B_MUTE6                  ((volatile uint32_t *)0xfe050ae8)
#define P_EE_AUDIO_TDMOUT_B_MUTE7                  ((volatile uint32_t *)0xfe050aec)
#define P_EE_AUDIO_TDMOUT_C_CTRL2                  ((volatile uint32_t *)0xfe050b00)
#define P_EE_AUDIO_TDMOUT_C_SWAP1                  ((volatile uint32_t *)0xfe050b04)
#define P_EE_AUDIO_TDMOUT_C_GAIN2                  ((volatile uint32_t *)0xfe050b08)
#define P_EE_AUDIO_TDMOUT_C_GAIN3                  ((volatile uint32_t *)0xfe050b0c)
#define P_EE_AUDIO_TDMOUT_C_MASK4                  ((volatile uint32_t *)0xfe050b10)
#define P_EE_AUDIO_TDMOUT_C_MASK5                  ((volatile uint32_t *)0xfe050b14)
#define P_EE_AUDIO_TDMOUT_C_MASK6                  ((volatile uint32_t *)0xfe050b18)
#define P_EE_AUDIO_TDMOUT_C_MASK7                  ((volatile uint32_t *)0xfe050b1c)
#define P_EE_AUDIO_TDMOUT_C_MUTE4                  ((volatile uint32_t *)0xfe050b20)
#define P_EE_AUDIO_TDMOUT_C_MUTE5                  ((volatile uint32_t *)0xfe050b24)
#define P_EE_AUDIO_TDMOUT_C_MUTE6                  ((volatile uint32_t *)0xfe050b28)
#define P_EE_AUDIO_TDMOUT_C_MUTE7                  ((volatile uint32_t *)0xfe050b2c)
//========================================================================
//  PDM - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe051000
// -----------------------------------------------
#define P_PDM_CTRL                                 ((volatile uint32_t *)0xfe051000)
//bit 31:   PDM enable.
//bit 30:   invert the PDM_DCLK.
//bit 29:   output mode:  1: 24bits. 0: 32 bits.
//bit 28:   bypass mode. 1: bypass all filter. directly output the PDM input to DDR. 0: normal mode.
//bit 27~9: not used.
//bit 16:.   PDM Asynchronous FIFO soft reset.  write 1 to soft reset AFIFO.
//bit 15:8   PDM channel reset.  0: to reset each PDM channel.  1: normal mode.
//bit 7:0.  PDM channel enable. each bit for one channel.
#define P_PDM_HCIC_CTRL1                           ((volatile uint32_t *)0xfe051004)
//bit 31      hcic filter enable.  1 use sinc filter. 0 bypass input to output.
//bit 29:24.  hcic final gain shift parameter.
//bit 23:16   hcic final gain multiplier.
//bit 8:4     hcic  down sample rate.
//bit 3:0     hcic  stage number. must be between 3 to 9.
#define P_PDM_HCIC_CTRL2                           ((volatile uint32_t *)0xfe051008)
//Not used.
#define P_PDM_F1_CTRL                              ((volatile uint32_t *)0xfe05100c)
//bit 31 .   filter 1 enable.
//bit 16:15. f1 round mode.  2'b00 : sign bit at bit 49.  28bits output [49:22] round at bit 21. 32bits output [49:18]. 24bits output [49:26]
//                    2'b01 : sign bit at bit 50.  28bits output [50:23] round at bit 22. 32bits output [49:18]. 24bits output [49:26]
//                    2'b10 : sign bit at bit 51.  28bits output [51:24] round at bit 23 32bits output [49:18]. 24bits output [49:26].
//bit 15:12. filter 1 down sample rate.
//bit 8:0.   filter 1 stage number.
#define P_PDM_F2_CTRL                              ((volatile uint32_t *)0xfe051010)
//bit 31 .   filter 2 enable.
//bit 16:15. f2 round mode.  2'b00 : round at bit 21. 2'b01 : round at bit 22.  2'b10 : round at bit 23.
//bit 15:12. filter 2 down sample rate.
//bit 8:0.   filter 2 stage number.
#define P_PDM_F3_CTRL                              ((volatile uint32_t *)0xfe051014)
//bit 31 .   filter 3 enable.
//bit 16:15. f3 round mode.  2'b00 : round at bit 21. 2'b01 : round at bit 22.  2'b10 : round at bit 23.
//bit 15:12. filter 3 down sample rate.
//bit 8:0.   filter 3 stage number.
#define P_PDM_HPF_CTRL                             ((volatile uint32_t *)0xfe051018)
//bit 31  High pass filter enable.
//bit 20:16 high pass filter shift steps. 6~19 steps.
//bit 15:0 high pass filter output factor.
#define P_PDM_CHAN_CTRL                            ((volatile uint32_t *)0xfe05101c)
//bit 31:24.  chan3 data sample pointer vs edge of the PDM_DCLK.
//bit 23:16   chan2 data sample pointer vs edge of the PDM_DCLK.
//bit 15:8.   chan1 data sample pointer vs edge of the PDM_DCLK.
//bit 7:0     chan0 data sample pointer vs edge of the PDM_DCLK.
#define P_PDM_CHAN_CTRL1                           ((volatile uint32_t *)0xfe051020)
//bit 31:24.  chan7 data sample pointer vs edge of the PDM_DCLK.
//bit 23:16   chan6 data sample pointer vs edge of the PDM_DCLK.
//bit 15:8.   chan5 data sample pointer vs edge of the PDM_DCLK.
//bit 7:0     chan4 data sample pointer vs edge of the PDM_DCLK.
#define P_PDM_COEFF_ADDR                           ((volatile uint32_t *)0xfe051024)
// address of the write/read of coeff data.
#define P_PDM_COEFF_DATA                           ((volatile uint32_t *)0xfe051028)
//write/read data to coeff memory.
#define P_PDM_CLKG_CTRL                            ((volatile uint32_t *)0xfe05102c)
// auto clock gating control.  1: disable the clock gating function. the clock will awlays enabled. 0 : use RTL auto clock gating.
//31:7 not used.
//bit 6  filt_ctrl module auto clock gating control.
//bit 5  sinc fifo module auto clock gating control.
//bit 4  filter module auto clock gating control.
//bit 3  apb module auto clock gating control.
//bit 2  coeff memory module auto clock gating control.
//bit 1  each channel module auto clock gating control.
//bit 0 cts_pdm_clk   auto clock gating control.
#define P_PDM_STS                                  ((volatile uint32_t *)0xfe051030)
//bit 1  HPF filter output overflow.  means the PCLK is too slow.
//bit 0  HCIC filter output overflow. means the CTS_PDM_CLK is too slow. can't finished the filter function.
#define P_PDM_MUTE_VALUE                           ((volatile uint32_t *)0xfe051034)
#define P_PDM_MASK_NUM                             ((volatile uint32_t *)0xfe051038)
//========================================================================
//  EQ DRC - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe052000
// -----------------------------------------------
#define P_AED_COEF_RAM_CNTL                        ((volatile uint32_t *)0xfe052000)
#define P_AED_COEF_RAM_DATA                        ((volatile uint32_t *)0xfe052004)
#define P_AED_EQ_EN                                ((volatile uint32_t *)0xfe052008)
#define P_AED_EQ_TAP_CNTL                          ((volatile uint32_t *)0xfe05200c)
#define P_AED_EQ_VOLUME                            ((volatile uint32_t *)0xfe052010)
#define P_AED_EQ_VOLUME_SLEW_CNT                   ((volatile uint32_t *)0xfe052014)
#define P_AED_MUTE                                 ((volatile uint32_t *)0xfe052018)
#define P_AED_DRC_CNTL                             ((volatile uint32_t *)0xfe05201c)
#define P_AED_DRC_RMS_COEF0                        ((volatile uint32_t *)0xfe052020)
#define P_AED_DRC_RMS_COEF1                        ((volatile uint32_t *)0xfe052024)
#define P_AED_DRC_THD0                             ((volatile uint32_t *)0xfe052028)
#define P_AED_DRC_THD1                             ((volatile uint32_t *)0xfe05202c)
#define P_AED_DRC_THD2                             ((volatile uint32_t *)0xfe052030)
#define P_AED_DRC_THD3                             ((volatile uint32_t *)0xfe052034)
#define P_AED_DRC_THD4                             ((volatile uint32_t *)0xfe052038)
#define P_AED_DRC_K0                               ((volatile uint32_t *)0xfe05203c)
#define P_AED_DRC_K1                               ((volatile uint32_t *)0xfe052040)
#define P_AED_DRC_K2                               ((volatile uint32_t *)0xfe052044)
#define P_AED_DRC_K3                               ((volatile uint32_t *)0xfe052048)
#define P_AED_DRC_K4                               ((volatile uint32_t *)0xfe05204c)
#define P_AED_DRC_K5                               ((volatile uint32_t *)0xfe052050)
#define P_AED_DRC_THD_OUT0                         ((volatile uint32_t *)0xfe052054)
#define P_AED_DRC_THD_OUT1                         ((volatile uint32_t *)0xfe052058)
#define P_AED_DRC_THD_OUT2                         ((volatile uint32_t *)0xfe05205c)
#define P_AED_DRC_THD_OUT3                         ((volatile uint32_t *)0xfe052060)
#define P_AED_DRC_OFFSET                           ((volatile uint32_t *)0xfe052064)
#define P_AED_DRC_RELEASE_COEF00                   ((volatile uint32_t *)0xfe052068)
#define P_AED_DRC_RELEASE_COEF01                   ((volatile uint32_t *)0xfe05206c)
#define P_AED_DRC_RELEASE_COEF10                   ((volatile uint32_t *)0xfe052070)
#define P_AED_DRC_RELEASE_COEF11                   ((volatile uint32_t *)0xfe052074)
#define P_AED_DRC_RELEASE_COEF20                   ((volatile uint32_t *)0xfe052078)
#define P_AED_DRC_RELEASE_COEF21                   ((volatile uint32_t *)0xfe05207c)
#define P_AED_DRC_RELEASE_COEF30                   ((volatile uint32_t *)0xfe052080)
#define P_AED_DRC_RELEASE_COEF31                   ((volatile uint32_t *)0xfe052084)
#define P_AED_DRC_RELEASE_COEF40                   ((volatile uint32_t *)0xfe052088)
#define P_AED_DRC_RELEASE_COEF41                   ((volatile uint32_t *)0xfe05208c)
#define P_AED_DRC_RELEASE_COEF50                   ((volatile uint32_t *)0xfe052090)
#define P_AED_DRC_RELEASE_COEF51                   ((volatile uint32_t *)0xfe052094)
#define P_AED_DRC_ATTACK_COEF00                    ((volatile uint32_t *)0xfe052098)
#define P_AED_DRC_ATTACK_COEF01                    ((volatile uint32_t *)0xfe05209c)
#define P_AED_DRC_ATTACK_COEF10                    ((volatile uint32_t *)0xfe0520a0)
#define P_AED_DRC_ATTACK_COEF11                    ((volatile uint32_t *)0xfe0520a4)
#define P_AED_DRC_ATTACK_COEF20                    ((volatile uint32_t *)0xfe0520a8)
#define P_AED_DRC_ATTACK_COEF21                    ((volatile uint32_t *)0xfe0520ac)
#define P_AED_DRC_ATTACK_COEF30                    ((volatile uint32_t *)0xfe0520b0)
#define P_AED_DRC_ATTACK_COEF31                    ((volatile uint32_t *)0xfe0520b4)
#define P_AED_DRC_ATTACK_COEF40                    ((volatile uint32_t *)0xfe0520b8)
#define P_AED_DRC_ATTACK_COEF41                    ((volatile uint32_t *)0xfe0520bc)
#define P_AED_DRC_ATTACK_COEF50                    ((volatile uint32_t *)0xfe0520c0)
#define P_AED_DRC_ATTACK_COEF51                    ((volatile uint32_t *)0xfe0520c4)
#define P_AED_DRC_LOOPBACK_CNTL                    ((volatile uint32_t *)0xfe0520c8)
#define P_AED_MDRC_CNTL                            ((volatile uint32_t *)0xfe0520cc)
#define P_AED_MDRC_RMS_COEF00                      ((volatile uint32_t *)0xfe0520d0)
#define P_AED_MDRC_RMS_COEF01                      ((volatile uint32_t *)0xfe0520d4)
#define P_AED_MDRC_RELEASE_COEF00                  ((volatile uint32_t *)0xfe0520d8)
#define P_AED_MDRC_RELEASE_COEF01                  ((volatile uint32_t *)0xfe0520dc)
#define P_AED_MDRC_ATTACK_COEF00                   ((volatile uint32_t *)0xfe0520e0)
#define P_AED_MDRC_ATTACK_COEF01                   ((volatile uint32_t *)0xfe0520e4)
#define P_AED_MDRC_THD0                            ((volatile uint32_t *)0xfe0520e8)
#define P_AED_MDRC_K0                              ((volatile uint32_t *)0xfe0520ec)
#define P_AED_MDRC_LOW_GAIN                        ((volatile uint32_t *)0xfe0520f0)
#define P_AED_MDRC_OFFSET0                         ((volatile uint32_t *)0xfe0520f4)
#define P_AED_MDRC_RMS_COEF10                      ((volatile uint32_t *)0xfe0520f8)
#define P_AED_MDRC_RMS_COEF11                      ((volatile uint32_t *)0xfe0520fc)
#define P_AED_MDRC_RELEASE_COEF10                  ((volatile uint32_t *)0xfe052100)
#define P_AED_MDRC_RELEASE_COEF11                  ((volatile uint32_t *)0xfe052104)
#define P_AED_MDRC_ATTACK_COEF10                   ((volatile uint32_t *)0xfe052108)
#define P_AED_MDRC_ATTACK_COEF11                   ((volatile uint32_t *)0xfe05210c)
#define P_AED_MDRC_THD1                            ((volatile uint32_t *)0xfe052110)
#define P_AED_MDRC_K1                              ((volatile uint32_t *)0xfe052114)
#define P_AED_MDRC_OFFSET1                         ((volatile uint32_t *)0xfe052118)
#define P_AED_MDRC_MID_GAIN                        ((volatile uint32_t *)0xfe05211c)
#define P_AED_MDRC_RMS_COEF20                      ((volatile uint32_t *)0xfe052120)
#define P_AED_MDRC_RMS_COEF21                      ((volatile uint32_t *)0xfe052124)
#define P_AED_MDRC_RELEASE_COEF20                  ((volatile uint32_t *)0xfe052128)
#define P_AED_MDRC_RELEASE_COEF21                  ((volatile uint32_t *)0xfe05212c)
#define P_AED_MDRC_ATTACK_COEF20                   ((volatile uint32_t *)0xfe052130)
#define P_AED_MDRC_ATTACK_COEF21                   ((volatile uint32_t *)0xfe052134)
#define P_AED_MDRC_THD2                            ((volatile uint32_t *)0xfe052138)
#define P_AED_MDRC_K2                              ((volatile uint32_t *)0xfe05213c)
#define P_AED_MDRC_OFFSET2                         ((volatile uint32_t *)0xfe052140)
#define P_AED_MDRC_HIGH_GAIN                       ((volatile uint32_t *)0xfe052144)
#define P_AED_ED_CNTL                              ((volatile uint32_t *)0xfe052148)
#define P_AED_DC_EN                                ((volatile uint32_t *)0xfe05214c)
#define P_AED_ND_LOW_THD                           ((volatile uint32_t *)0xfe052150)
#define P_AED_ND_HIGH_THD                          ((volatile uint32_t *)0xfe052154)
#define P_AED_ND_CNT_THD                           ((volatile uint32_t *)0xfe052158)
#define P_AED_ND_SUM_NUM                           ((volatile uint32_t *)0xfe05215c)
#define P_AED_ND_CZ_NUM                            ((volatile uint32_t *)0xfe052160)
#define P_AED_ND_SUM_THD0                          ((volatile uint32_t *)0xfe052164)
#define P_AED_ND_SUM_THD1                          ((volatile uint32_t *)0xfe052168)
#define P_AED_ND_CZ_THD0                           ((volatile uint32_t *)0xfe05216c)
#define P_AED_ND_CZ_THD1                           ((volatile uint32_t *)0xfe052170)
#define P_AED_ND_COND_CNTL                         ((volatile uint32_t *)0xfe052174)
#define P_AED_ND_RELEASE_COEF0                     ((volatile uint32_t *)0xfe052178)
#define P_AED_ND_RELEASE_COEF1                     ((volatile uint32_t *)0xfe05217c)
#define P_AED_ND_ATTACK_COEF0                      ((volatile uint32_t *)0xfe052180)
#define P_AED_ND_ATTACK_COEF1                      ((volatile uint32_t *)0xfe052184)
#define P_AED_ND_CNTL                              ((volatile uint32_t *)0xfe052188)
#define P_AED_MIX0_LL                              ((volatile uint32_t *)0xfe05218c)
#define P_AED_MIX0_RL                              ((volatile uint32_t *)0xfe052190)
#define P_AED_MIX0_LR                              ((volatile uint32_t *)0xfe052194)
#define P_AED_MIX0_RR                              ((volatile uint32_t *)0xfe052198)
#define P_AED_CLIP_THD                             ((volatile uint32_t *)0xfe05219c)
#define P_AED_CH1_ND_SUM_OUT                       ((volatile uint32_t *)0xfe0521a0)
#define P_AED_CH2_ND_SUM_OUT                       ((volatile uint32_t *)0xfe0521a4)
#define P_AED_CH1_ND_CZ_OUT                        ((volatile uint32_t *)0xfe0521a8)
#define P_AED_CH2_ND_CZ_OUT                        ((volatile uint32_t *)0xfe0521ac)
#define P_AED_NOISE_STATUS                         ((volatile uint32_t *)0xfe0521b0)
#define P_AED_POW_CURRENT_S0                       ((volatile uint32_t *)0xfe0521b4)
#define P_AED_POW_CURRENT_S1                       ((volatile uint32_t *)0xfe0521b8)
#define P_AED_POW_CURRENT_S2                       ((volatile uint32_t *)0xfe0521bc)
#define P_AED_POW_OUT0                             ((volatile uint32_t *)0xfe0521c0)
#define P_AED_POW_OUT1                             ((volatile uint32_t *)0xfe0521c4)
#define P_AED_POW_OUT2                             ((volatile uint32_t *)0xfe0521c8)
#define P_AED_POW_ADJ_INDEX0                       ((volatile uint32_t *)0xfe0521cc)
#define P_AED_POW_ADJ_INDEX1                       ((volatile uint32_t *)0xfe0521d0)
#define P_AED_POW_ADJ_INDEX2                       ((volatile uint32_t *)0xfe0521d4)
#define P_AED_DRC_GAIN_INDEX0                      ((volatile uint32_t *)0xfe0521d8)
#define P_AED_DRC_GAIN_INDEX1                      ((volatile uint32_t *)0xfe0521dc)
#define P_AED_DRC_GAIN_INDEX2                      ((volatile uint32_t *)0xfe0521e0)
#define P_AED_CH1_VOLUME_STATE                     ((volatile uint32_t *)0xfe0521e4)
#define P_AED_CH2_VOLUME_STATE                     ((volatile uint32_t *)0xfe0521e8)
#define P_AED_CH1_VOLUME_GAIN                      ((volatile uint32_t *)0xfe0521ec)
#define P_AED_CH2_VOLUME_GAIN                      ((volatile uint32_t *)0xfe0521f0)
#define P_AED_FULL_POW_CURRENT                     ((volatile uint32_t *)0xfe0521f4)
#define P_AED_FULL_POW_OUT                         ((volatile uint32_t *)0xfe0521f8)
#define P_AED_FULL_POW_ADJ                         ((volatile uint32_t *)0xfe0521fc)
#define P_AED_FULL_DRC_GAIN                        ((volatile uint32_t *)0xfe052200)
#define P_AED_MASTER_VOLUME_STATE                  ((volatile uint32_t *)0xfe052204)
#define P_AED_MASTER_VOLUME_GAIN                   ((volatile uint32_t *)0xfe052208)
#define P_AED_TOP_CTL0                             ((volatile uint32_t *)0xfe05220c)
#define P_AED_TOP_CTL1                             ((volatile uint32_t *)0xfe052210)
#define P_AED_TOP_CTL2                             ((volatile uint32_t *)0xfe052214)
//========================================================================
//  AUDIO locker - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe051400
// -----------------------------------------------
#define P_AUD_LOCK_EN                              ((volatile uint32_t *)0xfe051400)
#define P_AUD_LOCK_SW_RESET                        ((volatile uint32_t *)0xfe051404)
#define P_AUD_LOCK_SW_LATCH                        ((volatile uint32_t *)0xfe051408)
#define P_AUD_LOCK_HW_LATCH                        ((volatile uint32_t *)0xfe05140c)
#define P_AUD_LOCK_REFCLK_SRC                      ((volatile uint32_t *)0xfe051410)
#define P_AUD_LOCK_REFCLK_LAT_INT                  ((volatile uint32_t *)0xfe051414)
#define P_AUD_LOCK_IMCLK_LAT_INT                   ((volatile uint32_t *)0xfe051418)
#define P_AUD_LOCK_OMCLK_LAT_INT                   ((volatile uint32_t *)0xfe05141c)
#define P_AUD_LOCK_REFCLK_DS_INT                   ((volatile uint32_t *)0xfe051420)
#define P_AUD_LOCK_IMCLK_DS_INT                    ((volatile uint32_t *)0xfe051424)
#define P_AUD_LOCK_OMCLK_DS_INT                    ((volatile uint32_t *)0xfe051428)
#define P_AUD_LOCK_INT_CLR                         ((volatile uint32_t *)0xfe05142c)
#define P_AUD_LOCK_GCLK_CTRL                       ((volatile uint32_t *)0xfe051430)
#define P_AUD_LOCK_INT_CTRL                        ((volatile uint32_t *)0xfe051434)
#define P_RO_REF2IMCLK_CNT_L                       ((volatile uint32_t *)0xfe051440)
#define P_RO_REF2IMCLK_CNT_H                       ((volatile uint32_t *)0xfe051444)
#define P_RO_REF2OMCLK_CNT_L                       ((volatile uint32_t *)0xfe051448)
#define P_RO_REF2OMCLK_CNT_H                       ((volatile uint32_t *)0xfe05144c)
#define P_RO_IMCLK2REF_CNT_L                       ((volatile uint32_t *)0xfe051450)
#define P_RO_IMCLK2REF_CNT_H                       ((volatile uint32_t *)0xfe051454)
#define P_RO_OMCLK2REF_CNT_L                       ((volatile uint32_t *)0xfe051458)
#define P_RO_OMCLK2REF_CNT_H                       ((volatile uint32_t *)0xfe05145c)
#define P_RO_REFCLK_PKG_CNT                        ((volatile uint32_t *)0xfe051460)
#define P_RO_IMCLK_PKG_CNT                         ((volatile uint32_t *)0xfe051464)
#define P_RO_OMCLK_PKG_CNT                         ((volatile uint32_t *)0xfe051468)
#define P_RO_AUD_LOCK_INT_STATUS                   ((volatile uint32_t *)0xfe05146c)
//========================================================================
//  AUDIO VAD - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe051800
// -----------------------------------------------
//
// Reading file:  VAD_REG.h
//
#define P_VAD_TOP_CTRL0                            ((volatile uint32_t *)0xfe051800)
#define P_VAD_TOP_CTRL1                            ((volatile uint32_t *)0xfe051804)
#define P_VAD_TOP_CTRL2                            ((volatile uint32_t *)0xfe051808)
#define P_VAD_FIR_CTRL                             ((volatile uint32_t *)0xfe05180c)
#define P_VAD_FIR_EMP                              ((volatile uint32_t *)0xfe051810)
#define P_VAD_FIR_COEF0                            ((volatile uint32_t *)0xfe051814)
#define P_VAD_FIR_COEF1                            ((volatile uint32_t *)0xfe051818)
#define P_VAD_FIR_COEF2                            ((volatile uint32_t *)0xfe05181c)
#define P_VAD_FIR_COEF3                            ((volatile uint32_t *)0xfe051820)
#define P_VAD_FIR_COEF4                            ((volatile uint32_t *)0xfe051824)
#define P_VAD_FIR_COEF5                            ((volatile uint32_t *)0xfe051828)
#define P_VAD_FIR_COEF6                            ((volatile uint32_t *)0xfe05182c)
#define P_VAD_FIR_COEF7                            ((volatile uint32_t *)0xfe051830)
#define P_VAD_FIR_COEF8                            ((volatile uint32_t *)0xfe051834)
#define P_VAD_FIR_COEF9                            ((volatile uint32_t *)0xfe051838)
#define P_VAD_FIR_COEF10                           ((volatile uint32_t *)0xfe05183c)
#define P_VAD_FIR_COEF11                           ((volatile uint32_t *)0xfe051840)
#define P_VAD_FIR_COEF12                           ((volatile uint32_t *)0xfe051844)
#define P_VAD_FRAME_CTRL0                          ((volatile uint32_t *)0xfe051848)
#define P_VAD_FRAME_CTRL1                          ((volatile uint32_t *)0xfe05184c)
#define P_VAD_FRAME_CTRL2                          ((volatile uint32_t *)0xfe051850)
#define P_VAD_CEP_CTRL0                            ((volatile uint32_t *)0xfe051854)
#define P_VAD_CEP_CTRL1                            ((volatile uint32_t *)0xfe051858)
#define P_VAD_CEP_CTRL2                            ((volatile uint32_t *)0xfe05185c)
#define P_VAD_CEP_CTRL3                            ((volatile uint32_t *)0xfe051860)
#define P_VAD_CEP_CTRL4                            ((volatile uint32_t *)0xfe051864)
#define P_VAD_CEP_CTRL5                            ((volatile uint32_t *)0xfe051868)
#define P_VAD_DEC_CTRL                             ((volatile uint32_t *)0xfe05186c)
#define P_VAD_TOP_STS0                             ((volatile uint32_t *)0xfe051870)
#define P_VAD_TOP_STS1                             ((volatile uint32_t *)0xfe051874)
#define P_VAD_TOP_STS2                             ((volatile uint32_t *)0xfe051878)
#define P_VAD_FIR_STS0                             ((volatile uint32_t *)0xfe05187c)
#define P_VAD_FIR_STS1                             ((volatile uint32_t *)0xfe051880)
#define P_VAD_POW_STS0                             ((volatile uint32_t *)0xfe051884)
#define P_VAD_POW_STS1                             ((volatile uint32_t *)0xfe051888)
#define P_VAD_POW_STS2                             ((volatile uint32_t *)0xfe05188c)
#define P_VAD_FFT_STS0                             ((volatile uint32_t *)0xfe051890)
#define P_VAD_FFT_STS1                             ((volatile uint32_t *)0xfe051894)
#define P_VAD_SPE_STS0                             ((volatile uint32_t *)0xfe051898)
#define P_VAD_SPE_STS1                             ((volatile uint32_t *)0xfe05189c)
#define P_VAD_SPE_STS2                             ((volatile uint32_t *)0xfe0518a0)
#define P_VAD_SPE_STS3                             ((volatile uint32_t *)0xfe0518a4)
#define P_VAD_DEC_STS0                             ((volatile uint32_t *)0xfe0518a8)
#define P_VAD_DEC_STS1                             ((volatile uint32_t *)0xfe0518ac)
#define P_VAD_LUT_CTRL                             ((volatile uint32_t *)0xfe0518b0)
#define P_VAD_LUT_WR                               ((volatile uint32_t *)0xfe0518b4)
#define P_VAD_LUT_RD                               ((volatile uint32_t *)0xfe0518b8)
#define P_VAD_IN_SEL0                              ((volatile uint32_t *)0xfe0518bc)
#define P_VAD_IN_SEL1                              ((volatile uint32_t *)0xfe0518c0)
#define P_VAD_TO_DDR                               ((volatile uint32_t *)0xfe0518c4)
//
// Closing file:  VAD_REG.h
//
//========================================================================
//  AUDIO RESAMPLEA - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe051c00
// -----------------------------------------------
//
// Reading file:  RESAMPLE.h
//
#define P_AUDIO_RSAMP_CTRL0                        ((volatile uint32_t *)0xfe051c00)
//Bit   31:3      reserved
//Bit   2         reg_lock_rst      //unsigned  , default =0;
//Bit   1         reg_rsamp_rst     //unsigned  , default =0;
//Bit   0         reg_sw_rst        //unsigned  , default =0;
#define P_AUDIO_RSAMP_CTRL1                        ((volatile uint32_t *)0xfe051c04)
//Bit   31:26      reserved          //unsigned  , default =0;
//Bit   25         reg_rsamp_rst_sel //unsigned  , default =0;
//Bit   24         reg_module_bypas  //unsigned  , default =0;
//Bit   23:18      reg_gclk_ctrl     //unsigned  , default =0;
//Bit   17:13      reg_in_msb        //unsigned  , default =23;
//Bit   12         reg_output_en     //unsigned  , default =0;
//Bit   11         reg_rsamp_en      //unsigned  , default =0;
//Bit   10         reg_filt_en       //unsigned  , default =0;
//Bit   9          reg_post_en       //unsigned  , default =0;
//Bit   8          reg_inp_mux_mode  //unsigned  , default =0;
//Bit   7:4        reserved          //unsigned  , default =2;
//Bit   3:0        reg_inp_mux       //unsigned  , default =0;
#define P_AUDIO_RSAMP_CTRL2                        ((volatile uint32_t *)0xfe051c08)
//Bit 31:30    reserved              //unsigned  , default =0;
//Bit 29:24    reg_chx_size          //unsigned  , default =2;
//Bit 23:18    reserved              //unsigned  , default =0;
//Bit 17:16    reg_scl_step          //unsigned  , default =0; 0: 1/1  1: 1/2  2: 1/4
//Bit 15:8     reg_filt_tap          //unsigned  , default =63;
//Bit 7:0      reg_intp_tap          //unsigned  , default =63;
#define P_AUDIO_RSAMP_PHSINIT                      ((volatile uint32_t *)0xfe051c0c)
//Bit   31:28      reserved          //unsigned  , default = 0;
//Bit   27:0       reg_init_phs      //unsigned  , default = 0;
#define P_AUDIO_RSAMP_PHSSTEP                      ((volatile uint32_t *)0xfe051c10)
//Bit   31         reserved          //unsigned  , default = 0;
//Bit   30:0       reg_rsamp_step    //unsigned  , default = 134217728;//'h800_0000
#define P_AUDIO_RSAMP_SHIFT                        ((volatile uint32_t *)0xfe051c14)
//Bit   31:24       reg_rsft_iir    //unsigned  , default = 23;
//Bit   23:16       reg_rsft_blnd   //unsigned  , default = 21;
//Bit   15:8        reg_rsft_sinc   //unsigned  , default = 31;
//Bit   7:0         reg_rsft_aa     //unsigned  , default = 31;
#define P_AUDIO_RSAMP_ADJ_CTRL0                    ((volatile uint32_t *)0xfe051c18)
//Bit   31:3        reserved                //unsigned
//Bit   2           reg_rsamp_adj_out_inv   //unsigned , default = 0;
//Bit   1           reg_rsamp_adj_force_en  //unsigned , default = 0;
//Bit   0           reg_rsamp_adj_en        //unsigned , default = 0;
#define P_AUDIO_RSAMP_ADJ_CTRL1                    ((volatile uint32_t *)0xfe051c1c)
//Bit   31:16       reg_rsamp_adj_odet_step     //unsigned , default = 8;
//Bit   15:0        reg_rsamp_adj_kmax          //unsigned , default = 32768;
#define P_AUDIO_RSAMP_ADJ_SFT                      ((volatile uint32_t *)0xfe051c20)
//Bit   31:30       reserved                //unsigned , default = 0;
//Bit   29          reg_rsamp_adj_dif_sel   //unsigned , default = 0;
//Bit   28:24       reg_rsamp_adj_ki        //unsigned , default = 9;
//Bit   23:21       reserved                //unsigned , default = 0;
//Bit   20:16       reg_rsamp_adj_kp        //unsigned , default = 1;
//Bit   15:13       reserved                //unsigned , default = 0;
//Bit   12:8        reg_rsamp_adj_ki_sft    //unsigned , default = 6;
//Bit   7:6         reserved                //unsigned , default = 0;
//Bit   5:0         reg_rsamp_adj_out_sft   //unsigned , default = 12;
#define P_AUDIO_RSAMP_ADJ_IDET_LEN                 ((volatile uint32_t *)0xfe051c24)
//Bit   31:0       reg_rsamp_adj_idet_len       //unsigned , default = 10000;
#define P_AUDIO_RSAMP_ADJ_FORCE                    ((volatile uint32_t *)0xfe051c28)
//Bit   31:0       reg_rsamp_adj_force_err      //signed , default = 8;
#define P_AUDIO_RSAMP_ADJ_KI_FORCE                 ((volatile uint32_t *)0xfe051c2c)
//Bit   31:0       reg_rsamp_adj_ki_force //signed , default = 0;
#define P_AUDIO_RSAMP_RO_STATUS                    ((volatile uint32_t *)0xfe051c40)
//Bit   31:0       ro_rsamp_stat  //{din_chx_chk_err,is_idle_st,rsamp_fifo_over_cnt[7:0]}
#define P_AUDIO_RSAMP_RO_ADJ_FREQ                  ((volatile uint32_t *)0xfe051c44)
//Bit   31:0       ro_rsamp_adj_freq
#define P_AUDIO_RSAMP_RO_ADJ_DIFF_BAK              ((volatile uint32_t *)0xfe051c48)
//Bit   31:0       ro_det_diff_bak
#define P_AUDIO_RSAMP_RO_ADJ_DIFF_DLT              ((volatile uint32_t *)0xfe051c4c)
//Bit   31:0       ro_det_diff_dlt
#define P_AUDIO_RSAMP_RO_ADJ_PHS_ERR               ((volatile uint32_t *)0xfe051c50)
//Bit   31:0       ro_det_phase_err
#define P_AUDIO_RSAMP_RO_ADJ_KI_OUT                ((volatile uint32_t *)0xfe051c54)
//Bit   31:0       ro_rsamp_ki_out
#define P_AUDIO_RSAMP_RO_IN_CNT                    ((volatile uint32_t *)0xfe051c58)
//Bit   31:0       ro_rsamp_in_cnt
#define P_AUDIO_RSAMP_RO_OUT_CNT                   ((volatile uint32_t *)0xfe051c5c)
//Bit   31:0       ro_rsamp_out_cnt
#define P_AUDIO_RSAMP_POST_COEF0                   ((volatile uint32_t *)0xfe051c80)
//Bit   31:0       reg_post_coef0 //signed  , default = 0;
#define P_AUDIO_RSAMP_POST_COEF1                   ((volatile uint32_t *)0xfe051c84)
//Bit   31:0       reg_post_coef1 //signed  , default = 0;
#define P_AUDIO_RSAMP_POST_COEF2                   ((volatile uint32_t *)0xfe051c88)
//Bit   31:0       reg_post_coef2 //signed  , default = 0;
#define P_AUDIO_RSAMP_POST_COEF3                   ((volatile uint32_t *)0xfe051c8c)
//Bit   31:0       reg_post_coef3 //signed  , default = 0;
#define P_AUDIO_RSAMP_POST_COEF4                   ((volatile uint32_t *)0xfe051c90)
//Bit   31:0       reg_post_coef4 //signed  , default = 0;
#define P_AUDIO_RSAMP_AA_COEF_ADDR                 ((volatile uint32_t *)0xfe051cc0)
//Bit   31:0       reg_aa_coef_addr     //unsigned, default = 0;
#define P_AUDIO_RSAMP_AA_COEF_DATA                 ((volatile uint32_t *)0xfe051cc4)
//Bit   31:0       reg_aa_coef_data     //signed  , default = 0;
#define P_AUDIO_RSAMP_SINC_COEF_ADDR               ((volatile uint32_t *)0xfe051d00)
//Bit   31:0       reg_sinc_coef_addr   //unsigned, default = 0;
#define P_AUDIO_RSAMP_SINC_COEF_DATA               ((volatile uint32_t *)0xfe051d04)
//Bit   31:0       reg_sinc_coef_data   //signed  , default = 0;
//
// Closing file:  RESAMPLE.h
//
//========================================================================
//  AUDIO TOP_VAD - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe054800
// -----------------------------------------------
#define P_EE_AUDIO2_CLK81_CTRL                     ((volatile uint32_t *)0xfe054800)
#define P_EE_AUDIO2_CLK81_EN                       ((volatile uint32_t *)0xfe054804)
#define P_EE_AUDIO2_SW_RESET0                      ((volatile uint32_t *)0xfe054808)
#define P_EE_AUDIO2_CLK_GATE_EN0                   ((volatile uint32_t *)0xfe05480c)
#define P_EE_AUDIO2_SECURITY_CTRL0                 ((volatile uint32_t *)0xfe054810)
#define P_EE_AUDIO2_MCLK_VAD_CTRL                  ((volatile uint32_t *)0xfe054840)
#define P_EE_AUDIO2_VAD_CLK_CTRL                   ((volatile uint32_t *)0xfe054844)
#define P_EE_AUDIO2_MST_DLY_CTRL0                  ((volatile uint32_t *)0xfe054848)
#define P_EE_AUDIO2_MST_VAD_SCLK_CTRL0             ((volatile uint32_t *)0xfe05484c)
#define P_EE_AUDIO2_MST_VAD_SCLK_CTRL1             ((volatile uint32_t *)0xfe054850)
#define P_EE_AUDIO2_CLK_TDMIN_VAD_CTRL             ((volatile uint32_t *)0xfe054854)
#define P_EE_AUDIO2_CLK_PDMIN_CTRL0                ((volatile uint32_t *)0xfe054858)
#define P_EE_AUDIO2_CLK_PDMIN_CTRL1                ((volatile uint32_t *)0xfe05485c)
#define P_EE_AUDIO2_TOVAD_CTRL0                    ((volatile uint32_t *)0xfe054880)
#define P_EE_AUDIO2_TODDR_VAD_CTRL0                ((volatile uint32_t *)0xfe0548c0)
#define P_EE_AUDIO2_TODDR_VAD_CTRL1                ((volatile uint32_t *)0xfe0548c4)
#define P_EE_AUDIO2_TODDR_VAD_CTRL2                ((volatile uint32_t *)0xfe0548c8)
#define P_EE_AUDIO2_TODDR_VAD_START_ADDR           ((volatile uint32_t *)0xfe0548cc)
#define P_EE_AUDIO2_TODDR_VAD_INIT_ADDR            ((volatile uint32_t *)0xfe0548d0)
#define P_EE_AUDIO2_TODDR_VAD_FINISH_ADDR          ((volatile uint32_t *)0xfe0548d4)
#define P_EE_AUDIO2_TODDR_VAD_START_ADDRB          ((volatile uint32_t *)0xfe0548d8)
#define P_EE_AUDIO2_TODDR_VAD_FINISH_ADDRB         ((volatile uint32_t *)0xfe0548dc)
#define P_EE_AUDIO2_TODDR_VAD_INT_ADDR             ((volatile uint32_t *)0xfe0548e0)
#define P_EE_AUDIO2_TODDR_VAD_STATUS1              ((volatile uint32_t *)0xfe0548e4)
#define P_EE_AUDIO2_TODDR_VAD_STATUS2              ((volatile uint32_t *)0xfe0548e8)
#define P_EE_AUDIO2_TDMIN_VAD_CTRL                 ((volatile uint32_t *)0xfe054900)
#define P_EE_AUDIO2_TDMIN_VAD_SWAP0                ((volatile uint32_t *)0xfe054904)
#define P_EE_AUDIO2_TDMIN_VAD_SWAP1                ((volatile uint32_t *)0xfe054908)
#define P_EE_AUDIO2_TDMIN_VAD_MUTE_VAL             ((volatile uint32_t *)0xfe05490c)
#define P_EE_AUDIO2_TDMIN_VAD_STAT                 ((volatile uint32_t *)0xfe054910)
#define P_EE_AUDIO2_TDMIN_VAD_MUTE0                ((volatile uint32_t *)0xfe054940)
#define P_EE_AUDIO2_TDMIN_VAD_MUTE1                ((volatile uint32_t *)0xfe054944)
#define P_EE_AUDIO2_TDMIN_VAD_MUTE2                ((volatile uint32_t *)0xfe054948)
#define P_EE_AUDIO2_TDMIN_VAD_MUTE3                ((volatile uint32_t *)0xfe05494c)
#define P_EE_AUDIO2_TDMIN_VAD_MUTE4                ((volatile uint32_t *)0xfe054950)
#define P_EE_AUDIO2_TDMIN_VAD_MUTE5                ((volatile uint32_t *)0xfe054954)
#define P_EE_AUDIO2_TDMIN_VAD_MUTE6                ((volatile uint32_t *)0xfe054958)
#define P_EE_AUDIO2_TDMIN_VAD_MUTE7                ((volatile uint32_t *)0xfe05495c)
#define P_EE_AUDIO2_TDMIN_VAD_MASK0                ((volatile uint32_t *)0xfe054960)
#define P_EE_AUDIO2_TDMIN_VAD_MASK1                ((volatile uint32_t *)0xfe054964)
#define P_EE_AUDIO2_TDMIN_VAD_MASK2                ((volatile uint32_t *)0xfe054968)
#define P_EE_AUDIO2_TDMIN_VAD_MASK3                ((volatile uint32_t *)0xfe05496c)
#define P_EE_AUDIO2_TDMIN_VAD_MASK4                ((volatile uint32_t *)0xfe054970)
#define P_EE_AUDIO2_TDMIN_VAD_MASK5                ((volatile uint32_t *)0xfe054974)
#define P_EE_AUDIO2_TDMIN_VAD_MASK6                ((volatile uint32_t *)0xfe054978)
#define P_EE_AUDIO2_TDMIN_VAD_MASK7                ((volatile uint32_t *)0xfe05497c)
//
// Closing file:  REG_LIST_AUDIO_RTL.h
//
//
// Reading file:  ../spifc/rtl/spifc_reg.vh
//
//`ifdef SPIFC_REG_DEFINE
//`else
//`define SPIFC_REG_DEFINE
//`define SPIFC BASE ADDR      32'hfd000400
// -----------------------------------------------
// APB_BASE:  APB2_BASE_ADDR = 0xfd000400
// -----------------------------------------------
//AHB domain regsiter.
#define P_SPIFC_AHB_CTRL                           ((volatile uint32_t *)0xfd000400)
//bit 31   AHB BUS enable.  1 enable ahb request.  0: disable ahb request.
//bit 30.  decerr_en for (AXI->AHB bridge).
//bit 29.  force_incr.  for ( AXI->AHB bridge).
//bit 19.  critical word first.  CWF_EN.  1: enable.   0 : disable.
//bit 18:17 RDBUF_SIZE . 00: 64bytes. 01: 32 bytes. 10: 16 bytes. 11: reserved.
//bit 16  AHB MASTER enable.  if enabled, each master use one dedicated HRDATA buffer inside ahb2api model. total 3 HRDATA buffers inside ahb2spi.
//if disabled the 3 HRDATA buffer will random be used for all masters.
//bit 14  write 1 to clean the HRDATA buffer 2. read 0 finished clean operation.
//bit 13  write 1 to clean the HRDATA buffer 1. read 0 finished clean operation.
//bit 12  write 1 to clean the HRDATA buffer 0. read 0 finished clean operation.
//bit 11:0. not used.
#define P_SPIFC_CLK_CTRL                           ((volatile uint32_t *)0xfd000404)
//bit 14   asynchronous buffer ahb clock disable.  1 = disable. 0 = enable.
//bit 13   ahb2spi ahb clock disable.  1 = disable. 0 = enable.
//bit 12   ahb_arb ahb clock disable.  1 = disable. 0 = enable.
//bit 10   asynchronous buffer ahb clock auto gating enable.  1 = enable. 0 = disable.
//bit 9    ahb2spi ahb clock auto gating enable.  1 = enable. 0 = disable.
//bit 8    ahbarb  ahb clock auto gating enable.  1 = enable. 0 = disable.
//bit 2.    asynchronous AHB clock domain software reset.  1 = reset. 0 = normal working mode.
//bit 1.    ahb2spi ahb clock domain software reset.  1 = reset. 0 = normal working mode.
//bit 0.    not used.
#define P_SPIFC_SEC_CTRL                           ((volatile uint32_t *)0xfd000408)
//bit 31.  ADDRESS security range enable.  1 = enable; 0: disable.
//bit 14.  range6 enable:   1: enable; 0 : disable.
//bit 13.  range5 enable:   1: enable; 0 : disable.
//bit 12.  range4 enable:   1: enable; 0 : disable.
//bit 11.  range3 enable:   1: enable; 0 : disable.
//bit 10.  range2 enable:   1: enable; 0 : disable.
//bit 9.   range1 enable:   1: enable; 0 : disable.
//bit 8.   range0 enable:   1: enable; 0 : disable.
//bit 7:4  to control spi clock register security.  bit 7 = 1, APB bus can access spi clock domain register.  bit 7 == 0 : only PPROT[0] == bit 4, the APB can access spic clock domain register.
//bit 3:0  to control ahb clock domain register security. bit 3 = 1, APB bus can access ahb clock domain register.  bit 3 ==0; only PPROT[0] == bit 0, the APB can access ahb clock domain register.
#define P_SPIFC_RANGE0_STA                         ((volatile uint32_t *)0xfd000440)
//bit 28 :6.
//AHB security range 0  start address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE0_EDA                         ((volatile uint32_t *)0xfd000444)
//bit 28 :6.
//AHB security range 0  end address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE0_CTRL                        ((volatile uint32_t *)0xfd000448)
// AHB security range 0 control.
//bit 15:8 range 0 access control for DMA access.
//bit  15  range 0 access control for DMA thread 7
//bit  14  range 0 access control for DMA Thread 6
//bit  13  range 0 access control for DMA Thread 5.
//bit  12  range 0 access control for DMA Thread 4.
// bit 11. range 0 access control for DMA thread 3
// bit 10. range 0 access control for DMA Thread 2
// bit  9. range 0 access control for DMA Thread 1.
// bit  8. range 0 access control for DMA Thread 0.
// bit  7. range 0 access control for HMASTER == 7   Not used.
// bit  6. range 0 access control for HMASTEr == 6   //all other access that is not CPU DSP and DMA.
// bit  5. range 0 access control for HMASTER == 5   //DSPB non secure
// bit  4. range 0 access control for HMASTER == 4   //DSPB non secure
// bit  3. range 0 access control for HMASTER == 3   //DSPA secure
// bit  2. range 0 access control for HMASTER == 2   //DSPA non secure
// bit  1. range 0 access control for HMASTER == 1   //CPU secure access
// bit  0. range 0 access control for HMASTER == 0   //CPU non secure.
#define P_SPIFC_RANGE1_STA                         ((volatile uint32_t *)0xfd00044c)
//bit 28 :6.
//AHB security range 1  start address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE1_EDA                         ((volatile uint32_t *)0xfd000450)
//bit 28 :6.
//AHB security range 1  end address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE1_CTRL                        ((volatile uint32_t *)0xfd000454)
// AHB security range 1 control.
//bit 15:8 range 1 access control for DMA access.
//bit  15  range 1 access control for DMA thread 7
//bit  14  range 1 access control for DMA Thread 6
//bit  13  range 1 access control for DMA Thread 5.
//bit  12  range 1 access control for DMA Thread 4.
// bit 11. range 1 access control for DMA thread 3
// bit 10. range 1 access control for DMA Thread 2
// bit  9. range 1 access control for DMA Thread 1.
// bit  8. range 1 access control for DMA Thread 0.
// bit  7. range 1 access control for HMASTER == 7   Not used.
// bit  6. range 1 access control for HMASTEr == 6   //all other access that is not CPU DSP and DMA.
// bit  5. range 1 access control for HMASTER == 5   //DSPB non secure
// bit  4. range 1 access control for HMASTER == 4   //DSPB non secure
// bit  3. range 1 access control for HMASTER == 3   //DSPA secure
// bit  2. range 1 access control for HMASTER == 2   //DSPA non secure
// bit  1. range 1 access control for HMASTER == 1   //CPU secure access
// bit  0. range 1 access control for HMASTER == 0   //CPU non secure.
#define P_SPIFC_RANGE2_STA                         ((volatile uint32_t *)0xfd000458)
//bit 28 :6.
//AHB security range 2  start address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE2_EDA                         ((volatile uint32_t *)0xfd00045c)
//bit 28 :6.
//AHB security range 2  end address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE2_CTRL                        ((volatile uint32_t *)0xfd000460)
// AHB security range 2 control.
//bit 15:8 range 2 access control for DMA access.
//bit  15  range 2 access control for DMA thread 7
//bit  14  range 2 access control for DMA Thread 6
//bit  13  range 2 access control for DMA Thread 5.
//bit  12  range 2 access control for DMA Thread 4.
// bit 11. range 2 access control for DMA thread 3
// bit 10. range 2 access control for DMA Thread 2
// bit  9. range 2 access control for DMA Thread 1.
// bit  8. range 2 access control for DMA Thread 0.
// bit  7. range 2 access control for HMASTER == 7   Not used.
// bit  6. range 2 access control for HMASTEr == 6   //all other access that is not CPU DSP and DMA.
// bit  5. range 2 access control for HMASTER == 5   //DSPB non secure
// bit  4. range 2 access control for HMASTER == 4   //DSPB non secure
// bit  3. range 2 access control for HMASTER == 3   //DSPA secure
// bit  2. range 2 access control for HMASTER == 2   //DSPA non secure
// bit  1. range 2 access control for HMASTER == 1   //CPU secure access
// bit  0. range 2 access control for HMASTER == 0   //CPU non secure.
#define P_SPIFC_RANGE3_STA                         ((volatile uint32_t *)0xfd000464)
//bit 28 :6.
//AHB security range 3  start address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE3_EDA                         ((volatile uint32_t *)0xfd000468)
//bit 28 :6.
//AHB security range 3  end address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE3_CTRL                        ((volatile uint32_t *)0xfd00046c)
// AHB security range 3 control.
//bit 15:8 range 3 access control for DMA access.
//bit  15  range 3 access control for DMA thread 7
//bit  14  range 3 access control for DMA Thread 6
//bit  13  range 3 access control for DMA Thread 5.
//bit  12  range 3 access control for DMA Thread 4.
// bit 11. range 3 access control for DMA thread 3
// bit 10. range 3 access control for DMA Thread 2
// bit  9. range 3 access control for DMA Thread 1.
// bit  8. range 3 access control for DMA Thread 0.
// bit  7. range 3 access control for HMASTER == 7   Not used.
// bit  6. range 3 access control for HMASTEr == 6   //all other access that is not CPU DSP and DMA.
// bit  5. range 3 access control for HMASTER == 5   //DSPB non secure
// bit  4. range 3 access control for HMASTER == 4   //DSPB non secure
// bit  3. range 3 access control for HMASTER == 3   //DSPA secure
// bit  2. range 3 access control for HMASTER == 2   //DSPA non secure
// bit  1. range 3 access control for HMASTER == 1   //CPU secure access
// bit  0. range 3 access control for HMASTER == 0   //CPU non secure.
#define P_SPIFC_RANGE4_STA                         ((volatile uint32_t *)0xfd000470)
//bit 28 :6.
//AHB security range 4  start address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE4_EDA                         ((volatile uint32_t *)0xfd000474)
//bit 28 :6.
//AHB security range 4  end address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE4_CTRL                        ((volatile uint32_t *)0xfd000478)
// AHB security range 4 control.
//bit 15:8 range 4 access control for DMA access.
//bit  15  range 4 access control for DMA thread 7
//bit  14  range 4 access control for DMA Thread 6
//bit  13  range 4 access control for DMA Thread 5.
//bit  12  range 4 access control for DMA Thread 4.
// bit 11. range 4 access control for DMA thread 3
// bit 10. range 4 access control for DMA Thread 2
// bit  9. range 4 access control for DMA Thread 1.
// bit  8. range 4 access control for DMA Thread 0.
// bit  7. range 4 access control for HMASTER == 7   Not used.
// bit  6. range 4 access control for HMASTEr == 6   //all other access that is not CPU DSP and DMA.
// bit  5. range 4 access control for HMASTER == 5   //DSPB non secure
// bit  4. range 4 access control for HMASTER == 4   //DSPB non secure
// bit  3. range 4 access control for HMASTER == 3   //DSPA secure
// bit  2. range 4 access control for HMASTER == 2   //DSPA non secure
// bit  1. range 4 access control for HMASTER == 1   //CPU secure access
// bit  0. range 4 access control for HMASTER == 0   //CPU non secure.
#define P_SPIFC_RANGE5_STA                         ((volatile uint32_t *)0xfd00047c)
//bit 28 :6.
//AHB security range 5 start address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE5_EDA                         ((volatile uint32_t *)0xfd000484)
//bit 28 :6.
//AHB security range 5  end address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE5_CTRL                        ((volatile uint32_t *)0xfd000480)
// AHB security range 5 control.
//bit 15:8 range 5 access control for DMA access.
//bit  15  range 5 access control for DMA thread 7
//bit  14  range 5 access control for DMA Thread 6
//bit  13  range 5 access control for DMA Thread 5.
//bit  12  range 5 access control for DMA Thread 4.
// bit 11. range 5 access control for DMA thread 3
// bit 10. range 5 access control for DMA Thread 2
// bit  9. range 5 access control for DMA Thread 1.
// bit  8. range 5 access control for DMA Thread 0.
// bit  7. range 5 access control for HMASTER == 7   Not used.
// bit  6. range 5 access control for HMASTEr == 6   //all other access that is not CPU DSP and DMA.
// bit  5. range 5 access control for HMASTER == 5   //DSPB non secure
// bit  4. range 5 access control for HMASTER == 4   //DSPB non secure
// bit  3. range 5 access control for HMASTER == 3   //DSPA secure
// bit  2. range 5 access control for HMASTER == 2   //DSPA non secure
// bit  1. range 5 access control for HMASTER == 1   //CPU secure access
// bit  0. range 5 access control for HMASTER == 0   //CPU non secure.
#define P_SPIFC_RANGE6_STA                         ((volatile uint32_t *)0xfd000488)
//bit 28 :6.
//AHB security range 6 start address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE6_EDA                         ((volatile uint32_t *)0xfd00048c)
//bit 28 :6.
//AHB security range 6  end address in unit of 64Byte. related to HADDR bit 28:6.
//bit 5:0.  always 0.
#define P_SPIFC_RANGE6_CTRL                        ((volatile uint32_t *)0xfd000490)
// AHB security range 6 control.
//bit 15:8 range 6 access control for DMA access.
//bit  15  range 6 access control for DMA thread 7
//bit  14  range 6 access control for DMA Thread 6
//bit  13  range 6 access control for DMA Thread 5.
//bit  12  range 6 access control for DMA Thread 4.
// bit 11. range 6 access control for DMA thread 3
// bit 10. range 6 access control for DMA Thread 2
// bit  9. range 6 access control for DMA Thread 1.
// bit  8. range 6 access control for DMA Thread 0.
// bit  7. range 6 access control for HMASTER == 7   Not used.
// bit  6. range 6 access control for HMASTEr == 6   //all other access that is not CPU DSP and DMA.
// bit  5. range 6 access control for HMASTER == 5   //DSPB non secure
// bit  4. range 6 access control for HMASTER == 4   //DSPB non secure
// bit  3. range 6 access control for HMASTER == 3   //DSPA secure
// bit  2. range 6 access control for HMASTER == 2   //DSPA non secure
// bit  1. range 6 access control for HMASTER == 1   //CPU secure access
// bit  0. range 6 access control for HMASTER == 0   //CPU non secure.
#define P_SPIFC_RANGE7_CTRL                        ((volatile uint32_t *)0xfd000494)
// AHB security range 7( default range) control.
//bit 15:8 range 7 access control for DMA access.
//bit  15  range 7 access control for DMA thread 7
//bit  14  range 7 access control for DMA Thread 6
//bit  13  range 7 access control for DMA Thread 5.
//bit  12  range 7 access control for DMA Thread 4.
// bit 11. range 7 access control for DMA thread 3
// bit 10. range 7 access control for DMA Thread 2
// bit  9. range 7 access control for DMA Thread 1.
// bit  8. range 7 access control for DMA Thread 0.
// bit  7. range 7 access control for HMASTER == 7   Not used.
// bit  6. range 7 access control for HMASTEr == 6   //all other access that is not CPU DSP and DMA.
// bit  5. range 7 access control for HMASTER == 5   //DSPB non secure
// bit  4. range 7 access control for HMASTER == 4   //DSPB non secure
// bit  3. range 7 access control for HMASTER == 3   //DSPA secure
// bit  2. range 7 access control for HMASTER == 2   //DSPA non secure
// bit  1. range 7 access control for HMASTER == 1   //CPU secure access
// bit  0. range 7 access control for HMASTER == 0   //CPU non secure.
#define P_SPIFC_AHB_WTCH_CTRL                      ((volatile uint32_t *)0xfd000498)
//bit 15:0.   ahb bus watch dog timer. if AHB bus not response, the timer over this number, the SPIFC will send back  a error response.
#define P_SPIFC_SEC_VIO0                           ((volatile uint32_t *)0xfd00049c)
//bit 31:0  VIO ADDR
#define P_SPIFC_SEC_VIO1                           ((volatile uint32_t *)0xfd0004a0)
//bit  31.  AHB VIO status.  write 1 to clear the error info.
//bit 6:5  HMASTER.
//bit 4:2  HPROT.
//bit 1    HNONSEC.
//bit 0    HWRITE.
#define P_SPIFC_AHB_STS                            ((volatile uint32_t *)0xfd0004a4)
//bit 31.  AHB IDLE.  when ahb disabled, to show ahb status. 1,  no more ahb request or ahb frequent is holded.  0, ahb is still busy.
//bit 30.  AHB data cycle status, when ahb disabled, to show ahb data cycle status. 1, data cycle is holded,  0 : not in data cycle or data cycle is processing.
//bit 29.  AHB bus spi requst status. 1: still waiting for spi data. 0: idle.
//bit 28:0.  not used.
//SPI cts_spi_clk domain regsiter.
#define P_SPIFC_USER_CTRL0                         ((volatile uint32_t *)0xfd000600)
//bit 31.    user request enable.  write 1 to enable.  read 0, means user command accepted by the SPI_CTRL.
//bit 30.    user request finish bit.  write 0 when enable user request.  read  1 means SPI_CTRL finished this user command.
//bit 0.     user data updated.  write 0 when enable user request.   1 means there's datas/status read from SPI flash.
#define P_SPIFC_USER_CTRL1                         ((volatile uint32_t *)0xfd000604)
//bit 31.    not used.
//bit 30.    user command cycle enable.   1) enable.  0) don't send command in usr command period.
//bit 29:28. user command mode.  00 = SPI extend mode. 01 : dual mode.  10:  quad mode.
//bit 27:20. user command code. the code need to send in command period.
//bit 19.    user address cycle enable.
//bit 18:17  user address mode:  00 = SPI extend mode. 01 : dual mode.  10:  quad mode.
//bit 16:15  user address Byte length  00 = 1 byte.  01 = 2 bytes.  10 = 3 bytes. 11 = 4bytes.
//bit 14     user datout enable.
//bit 13     user dataout AES enable. 1: dataout through AES describe. 0 : original data.
//bit 12     user data output source. 0: from data buffer.  1: from status register.
//bit 11:10  user dataout mode: 00 SPI extend model. 01 : dual model. 10. quad mode.
//bit 9:0    User data out bytes num.
#define P_SPIFC_USER_CTRL2                         ((volatile uint32_t *)0xfd000608)
//bit 31 usr_dummy_en  1: enable dummy cycles.  0 : no dummy cycles.
//bit 30: 29 usr_dummy_mode  2'b00 = SPI extend mode. 01 dual mode. 10 Quad mode.
//bit 28:23  user dummy clock cycle number.
//bit 19:16. user command input/output direction control.  000000: right after the last output.
//          1: after the first cycle of dummy cycle.
//          .....
//          4'hf:  right before the datin cycle.
//bit 15:8.  data on the dummy cycle after the first byte.
//bit 7:0   The first bytes data showed on dummy cycle. some flash may need special data for enhance performance mode or XIP mode.
#define P_SPIFC_USER_CTRL3                         ((volatile uint32_t *)0xfd00060c)
//bit 31 usr_datin_en  1: datain cycle enabled.  0: no datain cycle.
//bit 30: datin_dest.  1: save the input data to STATUS register.  0 save the input data to data buffer.
//bit 29: 1: enable datain data AES describe. 0: not use AES.
//bit 28:27.  datin mode.  2'b00 SPI extend mode. 01: dual mode. 10 Quad mode.
//bit 25:16.  user data in data bytes.  how many bytes of user datain expected.
#define P_SPIFC_USER_ADDR                          ((volatile uint32_t *)0xfd000610)
//bit 31:0.  32bits user address.
#define P_SPIFC_AHB_REQ_CTRL                       ((volatile uint32_t *)0xfd000614)
//bit 31.  AHB request enable.
//bit 30   AHB cmd_en.  command cycle enable.
//bit 29:28. ahb command mode.   2'b00: spi mode. 01: dual mode. 10 Quad mode.
//bit 27:20:.AHB command code.
//bit 19.    ahb request address cycle enable.
//bit 18:17  AHB ADDRESS  mode. 2'b00: spi mode. 01: dual mode. 10 Quad mode.
//bit 16:15  AHB ADDRESS data width. 2'b00: 1byte. 2'b01 : 2bytes. 2'b10: 3bytes. 2'b11 4bytes.
//bit 13:10   AHB request spi bus input switch time. 0 : after address cycle. 1: the first dummy cycles. ... 0xf. after dummy cycle.
//bit 9:8     AHB DATAIN mode.   2'b00: spi mode. 01: dual mode. 10 Quad mode.
//bit 7       DATA IN AES enable. 1: enable 0: disable.
//bit 1:0     AHB REQ DATA size.  2'b00:  64bytes.  2'b01: 32 bytes. 2'b10 : 16 bytes. 2'b11: reserved. this bit setting should be same as RDBUF_SIZE setting.
#define P_SPIFC_AHB_REQ_CTRL1                      ((volatile uint32_t *)0xfd000618)
//bit 31.  ahb Dummy enable.
//bit 30:29.  ahb dummy mode.
//bit 28:23.  ahb request dummy clock cycles.
//bit 15:0.   ahb dummy period output data.
#define P_SPIFC_AHB_REQ_CTRL2                      ((volatile uint32_t *)0xfd00061c)
//bit 1:0  AHB address position. this setting depend on SPIFC_AHB_CTRL register CWF_EN bit and RDBUF_SIZE.
// if CWF_EN is enabled. these bit should be set to 2'b10 (16BYTE boundary).
// if CWF_EN is disabled. these bit should be set to same as the RDBUF_SIZE and AHB REQ DATA SIZE.
//00 :  64byte boundary.
//01 :  32byte boundary.
//10 :  16byte boundary.
//11 :  reserved.
#define P_SPIFC_ACTIMING0                          ((volatile uint32_t *)0xfd000620)
//bit 31:30.  tSLCH
//bit 29:28   tCLSH
//bit 20:16   tSHWL
//bit 15:12   tSHSL2
//bit 11:8    tSHSL1
//bit 7:0     tWHSL
#define P_SPIFC_ACTIMING1                          ((volatile uint32_t *)0xfd000624)
//bit 31.  D2 pin WP_n function enable.
//bit 30.  D2 Pin value in WP_n fucntion.
//bit 29.  D3 Pin HOLD_n function enable.
//bit 8.   DTR mode. not support.
//bit 6:4. Clock turn around delay. use it to set the system SPI clock read data delay.
//bit 3:0. not used.
#define P_SPIFC_ACTIMING2                          ((volatile uint32_t *)0xfd000628)
//bit 31.  spi clock input pin enable. SPIFC controller used this pin as clock to latch the input data.
//bit [3:0]  spi clock input pin delay adjustment.
//SPI DATA BUFFER.  There's a total 512Byte + 64byte(organized as 36x128bits) SRAM .
// since each APB access is 32bits. So we use word address as APB read/write address.
// But for SPI side we have to use 128bit boundary. That's the first program/read SPI with data buffer must start at data buffer 128bit boundary.  that means the SPIFC_USE_DBUF_ADDR last 2 bits must be 0.
#define P_SPIFC_DBUF_CTRL                          ((volatile uint32_t *)0xfd000640)
//bit 31.    1 = write DBUF.  0 : read DBUF.
//bit 30.    1. = auto update address. 0 don't change address.
//bit 7:0   DBUF address unit 32bits.
#define P_SPIFC_DBUF_DATA                          ((volatile uint32_t *)0xfd000644)
//bit 31:0.   if SPIFC_DBUF_CTRL bit 31 = 1.  write to this register will trigger one write to DBUF. and the data will be writen to DBUF.
#define P_SPIFC_USER_DBUF_ADDR                     ((volatile uint32_t *)0xfd000648)
//bit 7:0.  the DBUF ADDRESS used to send to or receive from SPI FLASH. the last 2 bits must be 0.
#define P_SPIFC_FLASH_STATUS                       ((volatile uint32_t *)0xfd000680)
#define P_SPIFC_STATUS                             ((volatile uint32_t *)0xfd000684)
//bit 1.  AES KEY valid. read only 1 : The aes key is ready to use. 0: the aes key is not ready.
//bit 0.  SPI CTRL STATE IDLE.  read only. 1 = idle; 0 = busy.
#define P_SPIFC_CTRL                               ((volatile uint32_t *)0xfd000688)
//bit 15:14. mempd for DBUF MEMORY.
//bit 13   spifc asynchronous buffer clock disable.  1 = disable. 0 = enable.
//bit 12.  spifc controller clock disable. 1 = disable. 0 = enable.
//bit 9.   spifc asynchronous bufer clock auto gate enable. 1 = enable. 0 = disable.
//bit 8.   spifc controller clock auto gate enable. 1 = enable. 0 = disable.
//bit 2.   spifc interface reset.  1 = RESET.  0 = normal working mode.
//bit 1.   spifc asynchronous buffer spi clock side reset. 1 = RESET.  0 : normal working mode.
//bit 0.   spifc controller reset.  1 = RESET. 0 : normal working mode.
//`endif
//
// Closing file:  ../spifc/rtl/spifc_reg.vh
//
//
// Reading file:  ../psram/rtl/psram_reg.vh
//
//`ifdef PSRAM_REG_DEFINE
//`else
//`define PSRAM_REG_DEFINE
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe007400
// -----------------------------------------------
#define P_PSRAM_AXI_INTF_CTRL                      ((volatile uint32_t *)0xfe007400)
//bit 3   axi interface soft reset.      1 = reset AXI interface.  0 = normal.
//bit 2   disable axi interface clock.    1 = disable; 0 = enable.
//bit 1   axi interface auto clock gating enable. 1 = enable; 0 = disable.
//bit 0   axi interface AXI request enable.  1 = enable. 0 = disable.
#define P_PSRAM_DBUF_CTRL                          ((volatile uint32_t *)0xfe007404)
//bit 16.    MWRITE_EN.  1 : enable mask write(with DM pin). 0 not enable.
//bit 15:0   DBUF AGE to write back to PSRAM if DBUF is dirty. but whole DBUF data is not READ to write.(with MWRITE enabled).
#define P_PSRAM_DBUF_CTRL1                         ((volatile uint32_t *)0xfe007408)
//bit 15:0  DBUF AGE to write back to psram if DBUF is dirty and whole DBUF data is ready to write.
#define P_PSRAM_APB_CTRL                           ((volatile uint32_t *)0xfe00740c)
//7:4      PSRAM PHY register APB secure contrl.
//bit 7,  1: no secure control.  0 : use bit 4 to match PPROT[0] bit.
//bit 4.  when bit 7 == 0, bit 4 must match PPROT[0] to access.
//3:0      PCTL_CTRL APB bus control regsiters secure control.
//bit 3,  1: no secure control.  0 : use bit 0 to match PPROT[0] bit.
//bit 0.  when bit 3 == 0, bit 0 must match PPROT[0] to access.
#define P_PSRAM_SEC_CTRL                           ((volatile uint32_t *)0xfe007410)
//bit 31.  ADDRESS security range enable.  1 = enable; 0: disable.
//if scrutiny range disabled, but AXI data describe enabled key0 is selected.
//bit 23   range 7  des key selection 0 : key0;  1: key1;
//bit 22   range 6  des key selection 0 : key0;  1: key1;
//bit 21   range 5  des key selection 0 : key0;  1: key1;
//bit 20   range 4  des key selection 0 : key0;  1: key1;
//bit 19   range 3  des key selection 0 : key0;  1: key1;
//bit 18   range 2  des key selection 0 : key0;  1: key1;
//bit 17   range 1  des key selection 0 : key0;  1: key1;
//bit 16   range0  des key selection 0 : key0;  1: key1;
//bit 6.   range6 enable:   1: enable; 0 : disable.
//bit 5.   range5 enable:   1: enable; 0 : disable.
//bit 4.   range4 enable:   1: enable; 0 : disable.
//bit 3.   range3 enable:   1: enable; 0 : disable.
//bit 2.   range2 enable:   1: enable; 0 : disable.
//bit 1.   range1 enable:   1: enable; 0 : disable.
//bit 0.   range0 enable:   1: enable; 0 : disable.
#define P_PSRAM_DES_PADDING                        ((volatile uint32_t *)0xfe007414)
//bit 31 :0 Padding, with address together to generate describe 64 bits input.
#define P_PSRAM_RANGE0_STA                         ((volatile uint32_t *)0xfe007440)
//bit 22 :0.
//AXI security range 0  start address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE0_EDA                         ((volatile uint32_t *)0xfe007444)
//bit 22 :0.
//AXI security range 0  end address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE0_CTRL0                       ((volatile uint32_t *)0xfe007448)
// AXI security range 0 control.
#define P_PSRAM_RANGE0_CTRL1                       ((volatile uint32_t *)0xfe00744c)
// AXI security range 0 control.
#define P_PSRAM_RANGE1_STA                         ((volatile uint32_t *)0xfe007450)
//bit 22 :0.
//AXI security range 1  start address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE1_EDA                         ((volatile uint32_t *)0xfe007454)
//bit 22 :0.
//AXI security range 1  end address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE1_CTRL0                       ((volatile uint32_t *)0xfe007458)
// AXI security range 1 control.
#define P_PSRAM_RANGE1_CTRL1                       ((volatile uint32_t *)0xfe00745c)
// AXI security range 1 control.
#define P_PSRAM_RANGE2_STA                         ((volatile uint32_t *)0xfe007460)
//bit 31 : 6.
//AXI security range 2  start address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE2_EDA                         ((volatile uint32_t *)0xfe007464)
//bit 31 :6.
//AXI security range 2  end address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE2_CTRL0                       ((volatile uint32_t *)0xfe007468)
#define P_PSRAM_RANGE2_CTRL1                       ((volatile uint32_t *)0xfe00746c)
#define P_PSRAM_RANGE3_STA                         ((volatile uint32_t *)0xfe007470)
//bit 31 : 6.
//AXI security range 3  start address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE3_EDA                         ((volatile uint32_t *)0xfe007474)
//bit 31 :6.
//AXI security range 3  end address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE3_CTRL0                       ((volatile uint32_t *)0xfe007478)
// AXI security range 3 control.
#define P_PSRAM_RANGE3_CTRL1                       ((volatile uint32_t *)0xfe00747c)
// AXI security range 3 control.
#define P_PSRAM_RANGE4_STA                         ((volatile uint32_t *)0xfe007480)
//bit 31 :6.
//AXI security range 4  start address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE4_EDA                         ((volatile uint32_t *)0xfe007484)
//bit 31 :6.
//AXI security range 4  end address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE4_CTRL0                       ((volatile uint32_t *)0xfe007488)
// AXI security range 4 control.
#define P_PSRAM_RANGE4_CTRL1                       ((volatile uint32_t *)0xfe00748c)
#define P_PSRAM_RANGE5_STA                         ((volatile uint32_t *)0xfe007490)
//bit 22 :0.
//AXI security range 5 start address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE5_EDA                         ((volatile uint32_t *)0xfe007494)
//bit 22 :0.
//AXI security range 5  end address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE5_CTRL0                       ((volatile uint32_t *)0xfe007498)
// AXI security range 5 control.
#define P_PSRAM_RANGE5_CTRL1                       ((volatile uint32_t *)0xfe00749c)
// AXI security range 5 control.
#define P_PSRAM_RANGE6_STA                         ((volatile uint32_t *)0xfe0074a0)
//bit 22 :0.
//AXI security range 6 start address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE6_EDA                         ((volatile uint32_t *)0xfe0074a4)
//bit 22 :0.
//AXI security range 6  end address in unit of 64Byte. related to HADDR bit 28:6.
#define P_PSRAM_RANGE6_CTRL0                       ((volatile uint32_t *)0xfe0074a8)
// AXI security range 6 control.
#define P_PSRAM_RANGE6_CTRL1                       ((volatile uint32_t *)0xfe0074ac)
// AXI security range 6 control.
#define P_PSRAM_RANGE7_CTRL0                       ((volatile uint32_t *)0xfe0074b0)
#define P_PSRAM_RANGE7_CTRL1                       ((volatile uint32_t *)0xfe0074b4)
#define P_PSRAM_VIO_STATUS                         ((volatile uint32_t *)0xfe0074b8)
//bit 31:  security violation.  write 1 to clean security violation status.
//bit 30.   1 : AXI write violation.  0 : axi read violation.
//bit 18:16.  AXI ARPROT/AWPROT>
//bit 11:0.   AXI violation ID.
#define P_PSRAM_VIO_ADDR                           ((volatile uint32_t *)0xfe0074bc)
//read only
//31:0  AXI violation address.
#define P_PSRAM_USER_CTRL0                         ((volatile uint32_t *)0xfe007600)
//bit 31.    user request enable.  write 1 to enable.  read 0, means user command accepted by the SPI_CTRL.
//bit 30.    usr request done.     after user command done. this bit set to 1.
//bit 29.    USR COMMAND enable.
//bit 28.    command type0. 1 READ command.  0 write command.
//bit 27.    command type1. 1 for register.  0 : for memory.
//bit 21~16. USR  command clock cycles
//bit 15~0.  USR COMMAND CODE.
#define P_PSRAM_USER_CTRL1                         ((volatile uint32_t *)0xfe007604)
//bit 31.    USR_ADDR_EN
//bit 30:29. USR_ADDR_DW
//bit 28~20. USR address clock cycle number.
//bit 19:16  USR data output/input switch postion
//bit 15.    USR DUMMY ENABLE
//bit 13~8.  USR DUMMY clock cycles.
//bit 5:0    USR data  DQS read enable postion.
#define P_PSRAM_USER_CTRL2                         ((volatile uint32_t *)0xfe007608)
//bit 31  usr des key selection. 1 : key 1; 0 : key 0.
//bit 30. usr check latency. for Winbond PSRAM data read/write, DQS input identify the latency is doubled or not.
//bit 29. usr data des enable.
//bit 28   usr datain enable.
//bit 27.  usr datain destination. 1 : write to CFG_STS register. 0 : write to DATA REGISTERs.
//bit 26~16  usr data in clock cycle number.
//bit 15.   usr datout enable.
//bit 14.  usr data out source  1 : from CFG_STS register. 0 from DATA register.
//bit 13~11. not used.
//bit 10~0.  usr dataout clock cycles.
#define P_PSRAM_USER_CTRL3                         ((volatile uint32_t *)0xfe00760c)
//bit 17:16. which cycle to ask phy to check read/write latency at PSRAM SEND command stage.
//since PSRAM send command stage only 3 clock cycle.  So this number only can chose 0, 1, 2.
//bit 15:0.  user dummy data when output in dummy cycle.
#define P_PSRAM_USER_ADDR                          ((volatile uint32_t *)0xfe007610)
//bit 31:0.  32bits user address.
#define P_PSRAM_AXI_REQ_CTRL0                      ((volatile uint32_t *)0xfe007614)
//bit 31.    AXI request enable.
//bit 30     AXI cmd_en.  command cycle enable.
//bit 29:24. AXI request command cycle clock numbers.
//bit 22.    AXI request address cycle enable.
//bit 21:16. AXI request address cycle clock number.
//bit 15:14. AXI address cycle data width.
//bit 13:12. at which clock cycle to send signal to phy to check latency flag for WINBOND PSRAM flexible latency.
//since PSRAM send command stage only 3 clock cycle.  So this number only can chose 0, 1, 2.
//bit 6     // AXI Write dummy enable.
//bit 5:0  // axi write dummy clock cycle number.
#define P_PSRAM_AXI_REQ_CTRL1                      ((volatile uint32_t *)0xfe007618)
//bit 31:16. AXI request psram READ command code.
//bit 15:0.  AXI request psram write command code.
#define P_PSRAM_AXI_REQ_CTRL2                      ((volatile uint32_t *)0xfe00761c)
//bit 31.  AXI  read request Dummy enable.
//bit 30.  Check latency enable for WINBOND PSRAM flexible latency.
//bit 27:22.  clock cycles in dummy stage to eanble PSRAM PHY reading logic.
//bit 21:16.  axi_read dummy clock cycles number.
//bit 15:0.   dummy data if output in dummy cycles.
#define P_PSRAM_AXI_REQ_CTRL3                      ((volatile uint32_t *)0xfe007620)
//bit 31      des enable.
//bit 26:16.  DATA input clock cycles.
//bit 10:0.   DATA output clock cycles.
#define P_PSRAM_ACTIMING0                          ((volatile uint32_t *)0xfe007624)
//bit 31:30.  tSLCH
//bit 29:28   tCLSH
//bit 20:16   tSHWL
//bit 15:12   tSHSL2
//bit 11:8    tSHSL1
//bit 7:0     tWHSL
#define P_PSRAM_ACTIMING1                          ((volatile uint32_t *)0xfe007628)
//bit 7:0 tRWR  for winbond PSRAM,  = tRWR -3. Winbond PSRAM include one CS cycle and 2 command cycle.
//for APMEMORY  should be same value of tCPH
#define P_PSRAM_ACTIMING2                          ((volatile uint32_t *)0xfe00762c)
#define P_PSRAM_WDG_CTRL                           ((volatile uint32_t *)0xfe007630)
//bit [31]   1: force to reset PSRAM PCTL and PHY logic. 0: normal.
//bit [30]   1: automatic reset PSRAM PCTL and PHY logic if watch dog triggered.
//bit 12:0.  watch dog timer.   for Winbond PSRAM should be 1us for tRC.  for APMEMORY should be 1us for tCEM.
//SPI DATA BUFFER.  There's a total 512KByte + 64byte(organized as 36x128bits) SRAM .
// since each APB access is 32bits. So we use word address as APB read/write address.
// But for SPI side we have to use 128bit boundary. That's the first program/read SPI with data buffer must start at data buffer 128bit boundary.  that means the PSRAM_USE_DBUF_ADDR last 2 bits must be 0.
#define P_PSRAM_DBUF_0                             ((volatile uint32_t *)0xfe007640)
#define P_PSRAM_DBUF_1                             ((volatile uint32_t *)0xfe007644)
#define P_PSRAM_DBUF_2                             ((volatile uint32_t *)0xfe007648)
#define P_PSRAM_DBUF_3                             ((volatile uint32_t *)0xfe00764c)
#define P_PSRAM_DBUF_4                             ((volatile uint32_t *)0xfe007650)
#define P_PSRAM_DBUF_5                             ((volatile uint32_t *)0xfe007654)
#define P_PSRAM_DBUF_6                             ((volatile uint32_t *)0xfe007658)
#define P_PSRAM_DBUF_7                             ((volatile uint32_t *)0xfe00765c)
#define P_PSRAM_DBUF_8                             ((volatile uint32_t *)0xfe007660)
#define P_PSRAM_DBUF_9                             ((volatile uint32_t *)0xfe007664)
#define P_PSRAM_DBUF_A                             ((volatile uint32_t *)0xfe007668)
#define P_PSRAM_DBUF_B                             ((volatile uint32_t *)0xfe00766c)
#define P_PSRAM_DBUF_C                             ((volatile uint32_t *)0xfe007670)
#define P_PSRAM_DBUF_D                             ((volatile uint32_t *)0xfe007674)
#define P_PSRAM_DBUF_E                             ((volatile uint32_t *)0xfe007678)
#define P_PSRAM_DBUF_F                             ((volatile uint32_t *)0xfe00767c)
#define P_PSRAM_CFG_STS                            ((volatile uint32_t *)0xfe007680)
//32bits register to save the USR read command read back values.
#define P_PSRAM_STATUS                             ((volatile uint32_t *)0xfe007684)
//bit 31.   usr_req_done flag.  1 : user request done . write 1 to clean.
//bit 30~11.
//bit 10:8
//bit 7:3.
//bit 2.  axi_data buffer idle bit.  1 : idle. 0 : working.
//bit 1.  axi interface idle bit.    1 : idle. 0 : working.
//bit 0.  psram ctrl idle bit.       1 : idle. 0 : working.
#define P_PSRAM_CTRL                               ((volatile uint32_t *)0xfe007688)
//bit 31.   PSRAM in DTR mode.
//bit 30.   psram input data latch  clock select. 1: from clock input pin. 0: from DQS
//if use clock input pin as data input latch clock, bit 13:12 should select 01.
//bit 29:27.  PSRAM mode.
//'b000 : STR SPI mode.
//'b001 : STR QPI mode.
//'b010 : DTR QPI mode.
//'b011 : DTR OPI FLASH.
//'b100 : APmemory PSRAM mode.
//'b101 : Winbond PSRAM mode.
//bit 26. enable to generation interruption after usr request done.  1: enable.  0: disable.
//bit 25.  to enable APMEMORY 2 command cycle mode.  1 : enable : 0: 3 command cycles.
//bit 24.  to enable the psram clock output always enable. 1 : enable. 0: disable.
//bit 23.  to enable the psram clock output extend mode to cover APMEMORY       tCPI.
//bit 22.  to disable the CKN output.  1: disable PSRAM CK_N pin output. 0: CK_N working normal.
//bit 21.  PSRAM DATA IN/OUT high 8bit and low 8 bit endian. 1 : rising edge is low byte. falling edge data is high byte.  0 : rising edge is high byte.  falling edge data is low byte.
//bit 20.   PSRAM ADDRESS unit.  1: Byte.  0 : word.
// APMEMORY used byte address.   Winbond use word address.
//bit 19.  DQSEN generation.   if use dqs to latch input data,  this bit  should be set to 1 to ask DMC generate ENABLE singal in dummy stage. 0: not generate DQS_EN.
//bit 18.  PSRAM RESET I/O oe_n value.   if need RESET pin.
//bit 17.  PSRAM RESET I/O output vale.  if need reset pin.
//bit 16.  force CS output low. for APMEMORY exit DPPD mode.  1:  to force cs output low  : 0 output normal working mode.
//bit 13:12.  psram data in clock enable selection. 00: use DQSEN.  01: use clkin cnt.
//bit 11.  DM Disable.  1: disable DM output.   0: with dmoutput.
//bit 9. psram pctl auto clock gating enable.   1: enable. 0 : disable.
//bit 8. psram pctl clock disable.              1: disable. 0 : enable.
//bit 2  PSRAM SPI mode, DQ2 works as  WP(write protection mode).
//bit 1.  DQ2 WP mode value.
//bit 0.  PSRAM SPI mode, DQ3 works as HOLD function.
#define P_PSRAM_PIN_CTRL                           ((volatile uint32_t *)0xfe00768c)
//bit 30:28  DATA 6 DQ pin selection.
//bit 27:24  DATA 6 DQ pin selection.
//bit 22:20  DATA 5 DQ pin selection.
//bit 18:16  DATA 4 DQ pin selection.
//bit 14:12  DATA 3 DQ pin selection.
//bit 10:8   DATA 2 DQ pin selection.
//bit 6:4    DATA 1 DQ pin selection.
//bit 2:0    DATA 0 DQ pin selection.
//0 :  from DQ[0]
//1 :  from DQ[1]
//2 :  from DQ[2]
//3 :  from DQ[3]
//4 :  from DQ[4]
//5 :  from DQ[5]
//6 :  from DQ[6]
//7 :  from DQ[7]
//psram_dly_16 is used for all DQ[0~] in/out/oen delay adjustment.
//psram_dly_16 consists of 15 delay cells, one delay cell is 50ps delay( typical corner).
//psram_dly_16 is control by delay_sel[3:0].  0 = no delay. 1~15 controls how many delay cells used.
//psram_dly_16 can be controlled by each control register either with real delay mode or VT updated with 4xclock period.
//psram_lcdl is used for DQS/DQSN input and CK/CKN output delay 90degree delay generation and/or delay fine tune.
//psram_lcdl consits of 96 delay cells,  same delay cell used in psram_dly_16.
//psram_lcdl should be calibrated with the psram_4xclock. and tracked with psram_4xclock with VT updated.
#define P_PSRAM_DQ0_DIN_DLY                        ((volatile uint32_t *)0xfe007700)
//bit 6:0. DQ0 DIN delay control.  in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ1_DIN_DLY                        ((volatile uint32_t *)0xfe007704)
//bit 6:0. DQ1 input delay control. in DLY16 read mode,  only [3:0] is valid.
#define P_PSRAM_DQ2_DIN_DLY                        ((volatile uint32_t *)0xfe007708)
//bit 6:0. DQ2 input delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ3_DIN_DLY                        ((volatile uint32_t *)0xfe00770c)
//bit 6:0. DQ3 input delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ4_DIN_DLY                        ((volatile uint32_t *)0xfe007710)
//bit 6:0. DQ4 input delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ5_DIN_DLY                        ((volatile uint32_t *)0xfe007714)
//bit 6:0. DQ5 input delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ6_DIN_DLY                        ((volatile uint32_t *)0xfe007718)
//bit 6:0. DQ6 input delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ7_DIN_DLY                        ((volatile uint32_t *)0xfe00771c)
//bit 6:0. DQ7 input delay control.  in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ0_DOUT_DLY                       ((volatile uint32_t *)0xfe007720)
//bit 6:0. DQ0 DOUT delay control.  in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ1_DOUT_DLY                       ((volatile uint32_t *)0xfe007724)
//bit 6:0. DQ1 output delay control. in DLY16 read mode,  only [3:0] is valid.
#define P_PSRAM_DQ2_DOUT_DLY                       ((volatile uint32_t *)0xfe007728)
//bit 6:0. DQ2 output delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ3_DOUT_DLY                       ((volatile uint32_t *)0xfe00772c)
//bit 6:0. DQ3 output delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ4_DOUT_DLY                       ((volatile uint32_t *)0xfe007730)
//bit 6:0. DQ4 output delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ5_DOUT_DLY                       ((volatile uint32_t *)0xfe007734)
//bit 6:0. DQ5 output delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ6_DOUT_DLY                       ((volatile uint32_t *)0xfe007738)
//bit 6:0. DQ6 output delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ7_DOUT_DLY                       ((volatile uint32_t *)0xfe00773c)
//bit 6:0. DQ7 output delay control.  in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ0_OEN_DLY                        ((volatile uint32_t *)0xfe007740)
//bit 6:0. DQ0 OEN delay control.  in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ1_OEN_DLY                        ((volatile uint32_t *)0xfe007744)
//bit 6:0. DQ1 output enable delay control. in DLY16 read mode,  only [3:0] is valid.
#define P_PSRAM_DQ2_OEN_DLY                        ((volatile uint32_t *)0xfe007748)
//bit 6:0. DQ2 output enable delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ3_OEN_DLY                        ((volatile uint32_t *)0xfe00774c)
//bit 6:0. DQ3 output enable delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ4_OEN_DLY                        ((volatile uint32_t *)0xfe007750)
//bit 6:0. DQ4 output enable delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ5_OEN_DLY                        ((volatile uint32_t *)0xfe007754)
//bit 6:0. DQ5 output enable delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ6_OEN_DLY                        ((volatile uint32_t *)0xfe007758)
//bit 6:0. DQ6 output enable delay control. in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQ7_OEN_DLY                        ((volatile uint32_t *)0xfe00775c)
//bit 6:0. DQ7 output enable delay control.  in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DM_DOUT_DLY                        ((volatile uint32_t *)0xfe007760)
//bit 6:0. DM output delay control.  in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DM_OEN_DLY                         ((volatile uint32_t *)0xfe007764)
//bit 6:0. DM output enable delay control.  in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_CS_DOUT_DLY                        ((volatile uint32_t *)0xfe007768)
//bit 6:0. CS output  delay control.  in DLY16 real mode, only [3:0] is valid.
#define P_PSRAM_DQS_DIN_DLY                        ((volatile uint32_t *)0xfe00776c)
//bit 6:0. DQS input delay control.   unit = 1/64 UI with PSRAM_LCDL_CTRL bit 28 = 0;
//  unit = 1 delay cell,    with PSRAM_LCDL_CTRL bit 28 = 1;
#define P_PSRAM_DQSN_DIN_DLY                       ((volatile uint32_t *)0xfe007770)
//bit 6:0. DQSN input delay control.   unit = 1/64 UI with PSRAM_LCDL_CTRL bit 28 = 0;
//  unit = 1 delay cell,    with PSRAM_LCDL_CTRL bit 28 = 1;
#define P_PSRAM_CKN_DOUT_DLY                       ((volatile uint32_t *)0xfe007774)
//bit 6:0. CKN output delay control.   unit = 1/64 UI with PSRAM_LCDL_CTRL bit 28 = 0;
//  unit = 1 delay cell,    with PSRAM_LCDL_CTRL bit 28 = 1;
#define P_PSRAM_CK_DOUT_DLY                        ((volatile uint32_t *)0xfe007778)
//bit 6:0. CK output delay control.   unit = 1/64 UI with PSRAM_LCDL_CTRL bit 28 = 0;
//  unit = 1 delay cell,    with PSRAM_LCDL_CTRL bit 28 = 1;
#define P_PSRAM_RDEN_DLY                           ((volatile uint32_t *)0xfe00777c)
//bit 10:7.  READ enable phase delay. unit = 1UI.  for AP QPI PSRAM.  PSRAM only send one cycle DQS preamble.
//           We have to used this UI delay to compensate the whole clock output delay and the DQS input delay.
//           if run high frequency, this delay need to be trained.  the valid value from 0 ~ 12.  Since one UI is 1/4 of PSRAM clock,
//           the total delay of the clock output delay + input delay should be less than 3 PSRAM clock.
//bit 6:0. READ enable fine tune delay control.   unit = 1/64 UI with PSRAM_LCDL_CTRL bit 28 = 0;
//  unit = 1 delay cell,    with PSRAM_LCDL_CTRL bit 28 = 1;
#define P_PSRAM_LCDL_CTRL                          ((volatile uint32_t *)0xfe0077c0)
//bit 31.   write 1 to update all delay cell delays control.
//bit 30.   write 1 to  calibration LCDL.
//bit 29.   LCDL track enable.  1: enable LCDL auto track VT changes.
//bit 28.   LCDL delay control mode.
//1 : real delay mode. For DQS/DQSN/CKN/CK/RDEN delay control register, one number means one delay cells.
//0 : UI MODE.    For DQS/DQSN/CKN/CK/RDEN delay control register, one number  means 1/64 psram_4xclk period.
//bit 27.   DLY16 ( psram_dly_16 cell) delay control mode.
//1 : real delay mode. For all data delay control register, one number means one delay cells.
//0 : UI MODE.     For all data delay control register, one number means 1/64 psram_4xclk period.
//bit 27:24.  not used.
//bit 23:0.   timer do trigger  LCDL track the VT once.  each track will compensate the UI with  1/16 delay cell delay.
//LCDL calibration ctroller.
#define P_PSRAM_LCDL_CAL_CTRL1                     ((volatile uint32_t *)0xfe0077c4)
//27:24. which LCDL delay line used to run calibration.
//19:16. LCDL calibration loop end position.
//15:12. LCDL calibration loop initial position.
//10:0.  LCDL calibration initial phase counter.
#define P_PSRAM_LCDL_CAL_CTRL2                     ((volatile uint32_t *)0xfe0077c8)
//26:16  LCDL phase lock limit. if the lcdl phase counter  difference between 2 calibration loops is less than this number, The LCDL would be locked and stop the calibration.
//10:0.  deta_init. the first loop phase changing number.
#define P_PSRAM_LCDL_CAL_CTRL3                     ((volatile uint32_t *)0xfe0077cc)
//29:25 to configure LCDL calibration step 5 position.
//24:20 to configure LCDL calibration step 4 position.
//19:15 to configure LCDL calibration step 3 position.
//14:10 to configure LCDL calibration step 2 position.
//9:5   to configure LCDL calibration step 1 position.
//4:0   to configure LCDL calibration step 0 position.
#define P_PSRAM_LCDL_CAL_CTRL4                     ((volatile uint32_t *)0xfe0077d0)
//19:15 to configure LCDL calibration step 9 position.
//14:10 to configure LCDL calibration step 8 position.
//9:5   to configure LCDL calibration step 7 position.
//4:0   to configure LCDL calibration step 6 position.
#define P_PSRAM_CLK_UI                             ((volatile uint32_t *)0xfe0077d4)
//bit 6:0. 4xCLOCK period delay measured from LCDL calibration.
#define P_PSRAM_LCDL_PH                            ((volatile uint32_t *)0xfe0077d8)
//10:0 LCDL phase counter after calibration.
#define P_PSRAM_LCDL_STATUS                        ((volatile uint32_t *)0xfe0077dc)
//bit 31. LCDL lock status.   1: LCDL locked to 4x clock.
//`endif
//
// Closing file:  ../psram/rtl/psram_reg.vh
//
//
// Reading file:  ../mmc_lp4/dmc/rtl/dmc_sec.vh
//
//`ifdef DMC_SEC_REG_DEFINE
//`else
//`define DMC_SEC_REG_DEFINE
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfd021000
// -----------------------------------------------
//`define DMC_SEC_REG_BASE      32'hfd021000
// ID assignment inside DMC..
//  id[12 :0]  total 13 bit ID.
//  id[12] == 1:  for DMC internal TEST module.
//  id[12] == 0: for AXI port input.
//  id[11:0] == AXI port ID.
//  id[2:0] == 0 :   A55 CPU.
//  id[2:0] == 1 : DSPA
//  id[2:0] == 2 : DSPB
//  id[2:0] == 3 :  Not used.
//  id[2:0] == 4 :  for Device.
//Device sublevel ID.
//id[9:7] == 5  : for audio.   id[3] == 0: audio
//id[9:7] == 6  : for DMA.    id[6:4] for DMA thread id.  id[3] for DMC security level.
//id[9:7] == others: for device other masters.
//  id[2:0] == 5 : for USB device.
//  id[2:0] == 6 : for I2C debug input.
//  id[2:0] == 7 : for USB host.
#define P_DMC_SEC_RANGE0_STA                       ((volatile uint32_t *)0xfd021000)
//bit 31:12.  range0 start address.  in 4KB boundary.
#define P_DMC_SEC_RANGE0_EDA                       ((volatile uint32_t *)0xfd021004)
//bit 31:12.  range0 end address.  in 4KB boundary.
#define P_DMC_SEC_RANGE0_RID_CTRL                  ((volatile uint32_t *)0xfd021008)
//range 0 read access enable based on DMC ID assignments.
//bit 28.   for AXI port 3.  Not used in A1.  should be 0.
//bit 27.   for dmc_test.  1 = enable. 0 = disable.
//bit 26.   for usbhost.   1 = enable. 0 = disable.
//bit 25.   for i2c.   1 = enable. 0 = disable.
//bit 24.   for usbdevide.  1 = enable. 0 = disable.
//bit 23.   for AXI  device  sublevel id 7.   1 = enable. 0 = disable.
//bit 22:18.for AXI  device sub level id 5 ~ 0.
//bit 17:10.for AXI device DMA thread 7 ~ thread 0.( in A1, DMA only has thread 3 to thread 0).
//bit 9:6.  for AXI device audio read subid 3~0.  (in A1, audio read only has subid 1 and subid 0).
//bit 5:4.  for AXI DSPB.  bit 5 for security access. bit 4 for non secure access.
//bit 3:2.  for AXI DSPA.  bit 3 for security access. bit 2 for non secure access.
//bit 1:0.  for AXI A55 CPU.  bit 1 for security access. bit 0 for non secure access.
#define P_DMC_SEC_RANGE0_WID_CTRL                  ((volatile uint32_t *)0xfd02100c)
//range 0 write access enable based on DMC ID assignments.
//bit 28.   for AXI port 3.  Not used in A1.  should be 0.
//bit 27.   for dmc_test.  1 = enable. 0 = disable.
//bit 26.   for usbhost.   1 = enable. 0 = disable.
//bit 25.   for i2c.   1 = enable. 0 = disable.
//bit 24.   for usbdevide.  1 = enable. 0 = disable.
//bit 23.   for AXI  device  sublevel id 7.   1 = enable. 0 = disable.
//bit 22:18.for AXI  device sub level id 5 ~ 0.
//bit 17:10.for AXI device DMA thread 7 ~ thread 0.( in A1, DMA only 4 threads from thread 3 to thread 0).
//bit 9:6.  for AXI device audio write subid 7~4.  (in A1, audio write only 2 masters usey  subid 4 and subid 5).
//bit 5:4.  for AXI DSPB.  bit 5 for security access. bit 4 for non secure access.
//bit 3:2.  for AXI DSPA.  bit 3 for security access. bit 2 for non secure access.
//bit 1:0.  for AXI A55 CPU.  bit 1 for security access. bit 0 for non secure access.
#define P_DMC_SEC_RANGE1_STA                       ((volatile uint32_t *)0xfd021010)
//bit 31:12.  range1 start address.  in 4KB boundary.
#define P_DMC_SEC_RANGE1_EDA                       ((volatile uint32_t *)0xfd021014)
//bit 31:12.  range0 end address.  in 4KB boundary.
#define P_DMC_SEC_RANGE1_RID_CTRL                  ((volatile uint32_t *)0xfd021018)
//range 1 read access enable based on DMC ID assignments.
//bit 28.   for AXI port 3.  Not used in A1.  should be 0.
//bit 27.   for dmc_test.  1 = enable. 0 = disable.
//bit 26.   for usbhost.   1 = enable. 0 = disable.
//bit 25.   for i2c.   1 = enable. 0 = disable.
//bit 24.   for usbdevide.  1 = enable. 0 = disable.
//bit 23.   for AXI  device  sublevel id 7.   1 = enable. 0 = disable.
//bit 22:18.for AXI  device sub level id 5 ~ 0.
//bit 17:10.for AXI device DMA thread 7 ~ thread 0.( in A1, DMA only has thread 3 to thread 0).
//bit 9:6.  for AXI device audio read subid 3~0.  (in A1, audio read only has subid 1 and subid 0).
//bit 5:4.  for AXI DSPB.  bit 5 for security access. bit 4 for non secure access.
//bit 3:2.  for AXI DSPA.  bit 3 for security access. bit 2 for non secure access.
//bit 1:0.  for AXI A55 CPU.  bit 1 for security access. bit 0 for non secure access.
#define P_DMC_SEC_RANGE1_WID_CTRL                  ((volatile uint32_t *)0xfd02101c)
//range 1 write access enable based on DMC ID assignments.
//bit 28.   for AXI port 3.  Not used in A1.  should be 0.
//bit 27.   for dmc_test.  1 = enable. 0 = disable.
//bit 26.   for usbhost.   1 = enable. 0 = disable.
//bit 25.   for i2c.   1 = enable. 0 = disable.
//bit 24.   for usbdevide.  1 = enable. 0 = disable.
//bit 23.   for AXI  device  sublevel id 7.   1 = enable. 0 = disable.
//bit 22:18.for AXI  device sub level id 5 ~ 0.
//bit 17:10.for AXI device DMA thread 7 ~ thread 0.( in A1, DMA only 4 threads from thread 3 to thread 0).
//bit 9:6.  for AXI device audio write subid 7~4.  (in A1, audio write only 2 masters usey  subid 4 and subid 5).
//bit 5:4.  for AXI DSPB.  bit 5 for security access. bit 4 for non secure access.
//bit 3:2.  for AXI DSPA.  bit 3 for security access. bit 2 for non secure access.
//bit 1:0.  for AXI A55 CPU.  bit 1 for security access. bit 0 for non secure access.
#define P_DMC_SEC_RANGE2_STA                       ((volatile uint32_t *)0xfd021020)
//bit 31:12.  range2 start address.  in 4KB boundary.
#define P_DMC_SEC_RANGE2_EDA                       ((volatile uint32_t *)0xfd021024)
//bit 31:12.  range2 end address.  in 4KB boundary.
#define P_DMC_SEC_RANGE2_RID_CTRL                  ((volatile uint32_t *)0xfd021028)
//range 2 read access enable based on DMC ID assignments.
//bit 28.   for AXI port 3.  Not used in A1.  should be 0.
//bit 27.   for dmc_test.  1 = enable. 0 = disable.
//bit 26.   for usbhost.   1 = enable. 0 = disable.
//bit 25.   for i2c.   1 = enable. 0 = disable.
//bit 24.   for usbdevide.  1 = enable. 0 = disable.
//bit 23.   for AXI  device  sublevel id 7.   1 = enable. 0 = disable.
//bit 22:18.for AXI  device sub level id 5 ~ 0.
//bit 17:10.for AXI device DMA thread 7 ~ thread 0.( in A1, DMA only has thread 3 to thread 0).
//bit 9:6.  for AXI device audio read subid 3~0.  (in A1, audio read only has subid 1 and subid 0).
//bit 5:4.  for AXI DSPB.  bit 5 for security access. bit 4 for non secure access.
//bit 3:2.  for AXI DSPA.  bit 3 for security access. bit 2 for non secure access.
//bit 1:0.  for AXI A55 CPU.  bit 1 for security access. bit 0 for non secure access.
#define P_DMC_SEC_RANGE2_WID_CTRL                  ((volatile uint32_t *)0xfd02102c)
//range 2 write access enable based on DMC ID assignments.
//bit 28.   for AXI port 3.  Not used in A1.  should be 0.
//bit 27.   for dmc_test.  1 = enable. 0 = disable.
//bit 26.   for usbhost.   1 = enable. 0 = disable.
//bit 25.   for i2c.   1 = enable. 0 = disable.
//bit 24.   for usbdevide.  1 = enable. 0 = disable.
//bit 23.   for AXI  device  sublevel id 7.   1 = enable. 0 = disable.
//bit 22:18.for AXI  device sub level id 5 ~ 0.
//bit 17:10.for AXI device DMA thread 7 ~ thread 0.( in A1, DMA only 4 threads from thread 3 to thread 0).
//bit 9:6.  for AXI device audio write subid 7~4.  (in A1, audio write only 2 masters usey  subid 4 and subid 5).
//bit 5:4.  for AXI DSPB.  bit 5 for security access. bit 4 for non secure access.
//bit 3:2.  for AXI DSPA.  bit 3 for security access. bit 2 for non secure access.
//bit 1:0.  for AXI A55 CPU.  bit 1 for security access. bit 0 for non secure access.
#define P_DMC_SEC_RANGE3_STA                       ((volatile uint32_t *)0xfd021030)
//bit 31:12.  range3 start address.  in 4KB boundary.
#define P_DMC_SEC_RANGE3_EDA                       ((volatile uint32_t *)0xfd021034)
//bit 31:12.  range3 end address.  in 4KB boundary.
#define P_DMC_SEC_RANGE3_RID_CTRL                  ((volatile uint32_t *)0xfd021038)
//range 3 read access enable based on DMC ID assignments.
//check DMC_SEC_RANGE0_RID_CTRL for each bit define.
#define P_DMC_SEC_RANGE3_WID_CTRL                  ((volatile uint32_t *)0xfd02103c)
//range 3 write access enable based on DMC ID assignments.
//check DMC_SEC_RANGE0_WID_CTRL for each bit define.
#define P_DMC_SEC_RANGE4_STA                       ((volatile uint32_t *)0xfd021040)
//bit 31:12.  range4 start address.  in 4KB boundary.
#define P_DMC_SEC_RANGE4_EDA                       ((volatile uint32_t *)0xfd021044)
//bit 31:12.  range4 end address.  in 4KB boundary.
#define P_DMC_SEC_RANGE4_RID_CTRL                  ((volatile uint32_t *)0xfd021048)
//range 4 read access enable based on DMC ID assignments.
//check DMC_SEC_RANGE0_RID_CTRL for each bit define.
#define P_DMC_SEC_RANGE4_WID_CTRL                  ((volatile uint32_t *)0xfd02104c)
//range 4 write access enable based on DMC ID assignments.
//check DMC_SEC_RANGE0_WID_CTRL for each bit define.
#define P_DMC_SEC_RANGE5_STA                       ((volatile uint32_t *)0xfd021050)
//bit 31:12.  range5 start address.  in 4KB boundary.
#define P_DMC_SEC_RANGE5_EDA                       ((volatile uint32_t *)0xfd021054)
//bit 31:12.  range5 end address.  in 4KB boundary.
#define P_DMC_SEC_RANGE5_RID_CTRL                  ((volatile uint32_t *)0xfd021058)
//range 5 read access enable based on DMC ID assignments.
//check DMC_SEC_RANGE0_RID_CTRL for each bit define.
#define P_DMC_SEC_RANGE5_WID_CTRL                  ((volatile uint32_t *)0xfd02105c)
//range 5 write access enable based on DMC ID assignments.
//check DMC_SEC_RANGE0_WID_CTRL for each bit define.
#define P_DMC_SEC_RANGE6_STA                       ((volatile uint32_t *)0xfd021060)
//bit 31:12.  range5 start address.  in 4KB boundary.
#define P_DMC_SEC_RANGE6_EDA                       ((volatile uint32_t *)0xfd021064)
//bit 31:12.  range6 end address.  in 4KB boundary.
#define P_DMC_SEC_RANGE6_RID_CTRL                  ((volatile uint32_t *)0xfd021068)
//range 6 read access enable based on DMC ID assignments.
//check DMC_SEC_RANGE0_RID_CTRL for each bit define.
#define P_DMC_SEC_RANGE6_WID_CTRL                  ((volatile uint32_t *)0xfd02106c)
//range 6 write access enable based on DMC ID assignments.
//check DMC_SEC_RANGE0_WID_CTRL for each bit define.
//range 7 is default range if the access not hit any of the previous 7 range, if would hit range 7.
#define P_DMC_SEC_RANGE7_RID_CTRL                  ((volatile uint32_t *)0xfd021070)
//range 7 read access enable based on DMC ID assignments.
//check DMC_SEC_RANGE0_RID_CTRL for each bit define.
#define P_DMC_SEC_RANGE7_WID_CTRL                  ((volatile uint32_t *)0xfd021074)
//range 7 write access enable based on DMC ID assignments.
//check DMC_SEC_RANGE0_WID_CTRL for each bit define.
#define P_DMC_SEC_RANGE_CTRL                       ((volatile uint32_t *)0xfd021078)
//bit 23:16    8 range security level. to choose with DES KEY to use data describe.
//bit 7:0      8 range enable. each bit for one range to identify the range is enabled or not.
//registers to check the security protection and watch point error information.
#define P_DMC_DES_CTRL                             ((volatile uint32_t *)0xfd021080)
//bit 1 data describe enable.
//bit 0  wirte 1 to block DES CTRL and DES PADDING read/write.
#define P_DMC_DES_PADDING                          ((volatile uint32_t *)0xfd021084)
#define P_DMC_SEC_STATUS                           ((volatile uint32_t *)0xfd0212e0)
//bit 31~2. not used.
//bit 1   write security violation.
//bit 0.  read security violation.
#define P_DMC_VIO_ADDR0                            ((volatile uint32_t *)0xfd0212e4)
//ddr0 write secure violation address.
#define P_DMC_VIO_ADDR1                            ((volatile uint32_t *)0xfd0212e8)
//bit 31:18 . not used. always 0.
//17     ddr0 secure check violation.
//16     ddr0 write address overflow. write out of DDR size.
//15:13. ddr0 write violation AWPROT bits.
//12:0   ddr0_write violation ID.  Please check ID assignment for ID bit defines.
#define P_DMC_VIO_ADDR2                            ((volatile uint32_t *)0xfd0212ec)
//ddr0 write secure violation address.
#define P_DMC_VIO_ADDR3                            ((volatile uint32_t *)0xfd0212f0)
//bit 31:18 . not used. always 0.
//17     ddr0 secure check violation.
//16     ddr0 read address overflow. write out of DDR size.
//15:13. ddr0 read violation AWPROT bits.
//12:0   ddr0_read violation ID. please ID assignment for ID bit defines.
//DDR0_ADDRMAP is used for RANK0 bank, row, col address mapping from AXI 32bits linear address.
#define P_DDR0_ADDRMAP_0                           ((volatile uint32_t *)0xfd021340)
//29:25 ca8.
//24:20 ca7.
//19:15 ca6.
//14:10 ca5.
//9:5   ca4.
//4:0   ca3.
#define P_DDR0_ADDRMAP_1                           ((volatile uint32_t *)0xfd021344)
//29:25 ra2.
//24:20 ra1.
//19:15 ra0.
//14:10 ca11.
//9:5   ca10.
//4:0   ca9.
#define P_DDR0_ADDRMAP_2                           ((volatile uint32_t *)0xfd021348)
//29:25 ra8.
//24:20 ra7.
//19:15 ra6.
//14:10 ra5.
//9:5   ra4.
//4:0   ra3.
#define P_DDR0_ADDRMAP_3                           ((volatile uint32_t *)0xfd02134c)
//29:25 ra14.
//24:20 ra13.
//19:15 ra12.
//14:10 ra11.
//9:5   ra10.
//4:0   ra9.
#define P_DDR0_ADDRMAP_4                           ((volatile uint32_t *)0xfd021350)
//29:25 ra16 for DDR4 SDRAM
//24:20 bg1  for DDR4 SDRAM.
//19:15 ba2.    or bg0 for DDR4.
//14:10 ba1.
//9:5   ba0.
//4:0   ra15.
#define P_DMC_DDR_CTRL                             ((volatile uint32_t *)0xfd021368)
// DDR PHY and DMC only supports 16bits and rank0 only.
//bit 24:22   3'b000 : ddr3 mode.
//3'b001 : ddr4 mode.
//3'b010 : lpddr3 mode.
//3'b011 : lpddr4 mode.
//bit 20    DDR4 BG1 enable bit.
//bit 3:0  :  DDR rank 0 size bit [2:0].
//4'b0000 : DDR rank 0 is 128Mbyte.
//4'b0001 : DDR rank 0 is 256Mbyte.
//4'b0010 : DDR rank 0 is 512Mbyte.
//4'b0011 : DDR rank 0 is 1Gbyte.
//4'b0100 : DDR rank 0 is 2Gbyte.
//4'b0101 : DDR rank 0 is 4Gbyte.
//4'b1000 : DDR rank 0 is 4Gbyte.
//4'b1001 : DDR rank 0 is 4Gbyte.
//others :  reserved.
#define P_DMC_APB_SEC_CTRL                         ((volatile uint32_t *)0xfd02136c)
//bit 19:16.  DMC normal APB register secure control.
//bit 19:  1:  all can write those register.  0:  the APB_PROT[0] must match the bit 16 to access those regsiter.
//Bit15:12.   DMC sticky APB regsiter secure control.
//bit 15.  1: All APB bus can write those registers. 0: The APB_PROT[0] must match the bit 12 to access those register.
//bit 11:8.   not used.
//Bit 7:4.   DMC DDR SDRAM protocal contorl register contorl
//bit 11.  1: All APB bus can write those registers. 0: The APB_PROT[0] must match the bit 4 to access those register.
//bit 3:0.   PHY APB regsiter secure control.
//bit 3.  1: All APB bus can write those registers. 0: The APB_PROT[0] must match the bit 0 to access those register.
#define P_DMC_DDR_PHY_CTRL                         ((volatile uint32_t *)0xfd0212fc)
//bit 4    PwrOkIn
//bit 3.   PHY APB soft reset_n
//bit 2.   PHY soft reset_n
//bit 1.   PHY DfiClk  enable.
//bit 0.   PHY DfiCtlClk enable.
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfd021400
// -----------------------------------------------
//`define DMC_TEST_REG_BASE               32'hfd021400
#define P_DMC_TEST_WRCMD_ADDR                      ((volatile uint32_t *)0xfd021770)
// the current write cmd address.
#define P_DMC_TEST_RDRSP_ADDR                      ((volatile uint32_t *)0xfd021774)
// the failed read response address(for error data )
#define P_DMC_TEST_RDCMD_ADDR                      ((volatile uint32_t *)0xfd021778)
// the current read command address.
#define P_DMC_TEST_WDG                             ((volatile uint32_t *)0xfd02177c)
//31:16.  write response watch dog.
//15:0.   read response  watch dog.
#define P_DMC_TEST_STA                             ((volatile uint32_t *)0xfd021780)
//test start address.  for non-sha mode,  the last 5 bits would be ignored. the test address at 32bytes boundary.
//                     for sha mode,      address must be in 64 bytes boundary. that mean the last 6 bits must be 0.
#define P_DMC_TEST_EDA                             ((volatile uint32_t *)0xfd021784)
//test end address.  for non-sha mode,  the last 5 bits would be ignored. the test address at 32bytes boundary.
//                   for sha mode,       address must be in 64 bytes boundary. that mean the last 6bits must be 1.
#define P_DMC_TEST_CTRL                            ((volatile uint32_t *)0xfd021788)
//bit 31.  enable test.
//bit 30.  when enable test, enable the write to DDR function.
//bit 29.  when enable test, enable the read from DDR function.
//bit 28.  Not used.
//bit 27.  enable to compare data.  when do the read enable to enable the error comparison. suppose the read data should be same as the data in the write buffer.
//bit 26.  Not used.
//bit 25.  address generation type.  0: continuous increase the address in the range of test start address and test end address.
//                                   1: test module would pick the random address from test start address  and test end address.
//bit 24.  done type.      0 : use the DMC_TEST_NUM register as the counter of test numbers.
//                             for write if the write command number == the DMC_TEST_NUM, the write is done.
//                             for read if the read command number == the DMC TEST_num, the read id done. for one read command can be repeated repeat number times.
//                         1 : finshed at end address.
//bit 23.  wdata type.     1 : the first write is {WD3, WD2,WD1,WD0}, then the latter is the previous data plus a pattern.( { + WD7,  + WD6, + WD5, + WD4}).
//                         0 : the WDATA is the data in write register.
//bit 23.  Not used.
//bit 22:20.   read repeat times.  for non-sha function, we can define multi times of the read. the test module would repeat the same adddress repeat times.
//bit 19.     limit write.  0: no outstanding write request limitation.
//                          1: limit the outstanding write commands to the number of bits [15:8]
//bit 18.     limit read.   0. no outstanding read request limitation.
//                          1. limit the read outstanding request to the number of bits[7:0].
//bit 17:16.  Not used.
//bit 15:8.   write outstanding commands limit.
//bit 7:0.    read  outstanding commands limit.
#define P_DMC_TEST_NUM                             ((volatile uint32_t *)0xfd02178c)
// how many test command for the test if the DMC_TEST_CTRL bit 24 is 0.
#define P_DMC_TEST_WD0                             ((volatile uint32_t *)0xfd021790)
// write data 0 for write command. also for read back data comparision.
#define P_DMC_TEST_WD1                             ((volatile uint32_t *)0xfd021794)
// write data 1 for write command. also for read back data comparision.
#define P_DMC_TEST_WD2                             ((volatile uint32_t *)0xfd021798)
// write data 2 for write command. also for read back data comparision.
#define P_DMC_TEST_WD3                             ((volatile uint32_t *)0xfd02179c)
// write data 3 for write command. also for read back data comparision.
#define P_DMC_TEST_WD4                             ((volatile uint32_t *)0xfd0217a0)
// write data 4 for write command. also for read back data comparision.
#define P_DMC_TEST_WD5                             ((volatile uint32_t *)0xfd0217a4)
// write data 5 for write command. also for read back data comparision.
#define P_DMC_TEST_WD6                             ((volatile uint32_t *)0xfd0217a8)
// write data 6 for write command. also for read back data comparision.
#define P_DMC_TEST_WD7                             ((volatile uint32_t *)0xfd0217ac)
// write data 7 for write command. also for read back data comparision.
#define P_DMC_TEST_RD0                             ((volatile uint32_t *)0xfd0217b0)
// the read back data 0.  if error happens, it would capture the first error data.
#define P_DMC_TEST_RD1                             ((volatile uint32_t *)0xfd0217b4)
// the read back data 1.  if error happens, it would capture the first error data.
#define P_DMC_TEST_RD2                             ((volatile uint32_t *)0xfd0217b8)
// the read back data 2.  if error happens, it would capture the first error data.
#define P_DMC_TEST_RD3                             ((volatile uint32_t *)0xfd0217bc)
// the read back data 3.  if error happens, it would capture the first error data.
#define P_DMC_TEST_RD4                             ((volatile uint32_t *)0xfd0217c0)
// the read back data 4.  if error happens, it would capture the first error data.
#define P_DMC_TEST_RD5                             ((volatile uint32_t *)0xfd0217c4)
// the read back data 5.  if error happens, it would capture the first error data.
#define P_DMC_TEST_RD6                             ((volatile uint32_t *)0xfd0217c8)
// the read back data 6.  if error happens, it would capture the first error data.
#define P_DMC_TEST_RD7                             ((volatile uint32_t *)0xfd0217cc)
// the read back data 7.  if error happens, it would capture the first error data.
#define P_DMC_TEST_ERR_ADDR                        ((volatile uint32_t *)0xfd0217d0)
// it capture the first error address.
#define P_DMC_TEST_ERR_CNT                         ((volatile uint32_t *)0xfd0217d4)
// how many data error happens in the whole test period.
#define P_DMC_TEST_STS                             ((volatile uint32_t *)0xfd0217d8)
//bit 31,   test done bit. write 1 to clean.
//bit 30,   indicate address err
//bit 29~7.  not used.
//bit 6.    read data resp error(caused by security or rd latency).
//bit 5.    test MRR/MPR rd latency error. write 1 clear
//bit 4,    sha done.     write 1 to clean.
//bit 3,    write done.   write 1 to clean.
//bit 2,    read done.    write 1 to clean
//bit 1,    write watchdog triggered.   write 1 to clean
//bit 0,    read watchdog triggered.    write 1 to clean.
#define P_DMC_TEST_COMP_MASK                       ((volatile uint32_t *)0xfd0214cc)
//32bits for DMC TEST COMPARE bit enable.
//1 : to MASK this bit.  0: compare this bit.
#define P_DMC_TEST_RDBI0                           ((volatile uint32_t *)0xfd0214d0)
//MPC RD FIFO command DBI read back data
//bit 31:16  the second cycle.
//bit 15:0   the first cycle.
#define P_DMC_TEST_RDBI1                           ((volatile uint32_t *)0xfd0214d4)
//MPC RD FIFO command DBI read back data
//bit 31:16. the Forth cycle.
//bit 15:0.  the third cycle.
#define P_DMC_TEST_WSTRB0                          ((volatile uint32_t *)0xfd0214d8)
//MPC WR FIFO command DM bit write data
//bit 31:16  the second cycle.
//bit 15:0   the first cycle.
#define P_DMC_TEST_DRAM_CMD                        ((volatile uint32_t *)0xfd0214e0)
//bit 31. cmd done.  write 0 to clean.
//bit 30. data done. write 0 to clean.
//bit 4:0.  only one bit can be 1. read data stored in DMC_TEST_RD* write data from DMC_TEST_W
//bit 4. LPDDR4 MPC write data command( MPC WR FIFO).      Not support LPDDR4 in A1.
//bit 3. LPDDR4 MPC read data command (MPC RD Calibration and RD FIFO).Not support LPDDR4 in A1.
//bit 2. LPDDR4 MPC-1 command ( NOP,  Start DQS interval ....)Not support LPDDR4 in A1.
//bit 1. mrr comand.
//bit 0. mrw command.
#define P_DMC_TEST_DRAM_CMD_CODE                   ((volatile uint32_t *)0xfd0214e4)
//bit 27:26. 128bits data cycles . 0: 1 clock cycles;  1: 2  clock cycles; 2: 3 clock cycles; 3:4 clock cycles.
//           LPDDR4 32bits: 4 clock cycles; LPDDR4 16bits: 2 clock cycles;
//           DDR3/4/LPDDR3 32 bits : 2 clock cycles. DDR3/4 16bits:  1 cycles.
//bit 25  MRW/MRR/MPC command rank 1 select.  1: select.  0: not select.  In A1, only rank0 supported.
//bit 24. MRW/MRR/MPC command rank 0 select.  1: select.  0: not select.In A1, only rank0 supported.
//bit 23:16  MR addr.  DDR4 case :  18:16 ba[2:0].    20:19 BG[1:0].
//bit 15:0   opcode.
#define P_DMC_TEST_DRAM_CMD_TIME                   ((volatile uint32_t *)0xfd0214e8)
//bit 31:16  PRE  CMD timer. //delay how many cycle to start the command.
//bit 15:0   POST CMD timer  //delay how many cycle after the command execute.
//`endif
//
// Closing file:  ../mmc_lp4/dmc/rtl/dmc_sec.vh
//
//
// Reading file:  ../mmc_lp4/dmc/rtl/dmc_reg.vh
//
//`ifdef DMC_REG_DEFINE
//`else
//`define DMC_REG_DEFINE
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfd020000
// -----------------------------------------------
//`define DMC_REG_BASE      32'hfd020000
#define P_DMC_SOFT_RST2                            ((volatile uint32_t *)0xfd02000c)
//bit 31~11.  reserved for future.
//bit 10  DMC DFI cmd soft reset_n
//bit 9   DMC DFI MISC soft reset_n
//bit 8   DMC DFI data soft reset_n
//bit 7   DMC DFI dcu soft reset_n
//bit 6   DMC siu soft reset_n
//bit 5.  DMC test soft reset_n.  0 : reset. 1 : normal working mode.
//bit 4.  DMC low power control module soft reset_n.    0 : reset. 1 : normal working mode.
//bit 3.  DMC QOS monitor module soft reset_n.   0 : reset. 1 : normal working mode.
//bit 2.  DMC register module soft reset_n.       0 : reset. 1 : normal working mode.
//bit 1.  DMC canvas transfer module soft reset_n.  0 : reset. 1 : normal working mode.
//bit 0.  DMC command buffers and command generation modules soft reset.  0 = reset. 1:
#define P_DMC_VERSION                              ((volatile uint32_t *)0xfd020014)
//read only 32'h000b0000.  for A1
#define P_DMC_MON_CTRL0                            ((volatile uint32_t *)0xfd020080)
//bit 31.   qos_mon_en.    write 1 to trigger the enable. polling this bit 0, means finished.  or use interrupt to check finish.
//bit 30.   qos_mon interrupt clear.  clear the qos monitor result.  read 1 = qos mon finish interrupt.
//bit 1.    qos monitor 1 enable.
//bit 0.    qos monitor 0 enable.
#define P_DMC_MON_CTRL1                            ((volatile uint32_t *)0xfd020084)
//bit 8:0.  qos monitor 0 channel select.
//bit 8    to select DMC TEST
//bit 7:0  each bit enable one AXI subID.
#define P_DMC_MON_CTRL2                            ((volatile uint32_t *)0xfd020088)
//bit 8.  to enable monitor 0 second level subID selection for example device.
//bit 7:0  each bit to enable one second level subID inside device.
#define P_DMC_MON_CTRL3                            ((volatile uint32_t *)0xfd02008c)
//bit 8:0.  qos monitor 1 channel select.
//bit 8    to select DMC TEST
//bit 7:0  each bit enable one AXI subID.
#define P_DMC_MON_CTRL4                            ((volatile uint32_t *)0xfd020090)
//bit 8.  to enable monitor 1 second level subID selection for example device.
//bit 7:0  each bit to enable one second level subID inside device.
#define P_DMC_MON_ALL_REQ_CNT                      ((volatile uint32_t *)0xfd0200a4)
// at the test period,  the whole MMC request time.
#define P_DMC_MON_ALL_GRANT_CNT                    ((volatile uint32_t *)0xfd0200a8)
// at the test period,  the whole MMC granted data cycles. 64bits unit.
#define P_DMC_MON_ONE_GRANT_CNT                    ((volatile uint32_t *)0xfd0200ac)
// at the test period,  the granted data cycles for the selected channel and ports.
#define P_DMC_MON_SEC_GRANT_CNT                    ((volatile uint32_t *)0xfd0200b0)
// at the test period,  the granted data cycles for the selected channel and ports.
#define P_DMC_MON_TIMER                            ((volatile uint32_t *)0xfd0200bc)
// timer for the monitor period.
#define P_DMC_CLKG_CTRL2                           ((volatile uint32_t *)0xfd0200c8)
//bit 10  enable auto clock gating for dfi command generation
//bit 9   enable auto clock gating for dram controller
//bit 8   enable auto clock gating for dfi data path.
//bit 7.  enalbe auto clock gating for write rsp generation.
//bit 6.  enalbe auto clock gating for read rsp generation.
//bit 5.  enalbe auto clock gating for ddr0 command filter.
//bit 4.  enalbe auto clock gating for ddr0 write reorder buffer.
//bit 3.  enalbe auto clock gating for ddr0 write data buffer.
//bit 2.  enalbe auto clock gating for ddr0 read reorder buffer.
//bit 1.  reserved
//bit 0.  reserved
#define P_DMC_CLKG_CTRL3                           ((volatile uint32_t *)0xfd0200cc)
//bit 10  force to disable the clock of dfi command generation
//bit 9   force to disable the clock of dram controller
//bit 8   force to disable the clock of dfi data path.
//bit 7. force to disalbe the clock of write rsp generation.
//bit 6. force to disalbe the clock of read rsp generation.
//bit 5.  force to disalbe the clock of  command filter.
//bit 4.  force to disalbe the clock of  write reorder buffer.
//bit 3.  force to disalbe the clock of write data buffer.
//bit 2.  force to disalbe the clock of read reorder buffer.
//bit 1.  reserved.
//bit 0.  reserved.
#define P_DMC_CHAN_STS                             ((volatile uint32_t *)0xfd0200d8)
//bit 4      always 1
//bit 3      ddr0 write data buffer idle. 1 : idle 0: busy.
//bit 2      always 1.
//bit 1      ddr0 wbuf idle.              1 : idle 0: busy.
//bit 0.      axi0 channel idle.         1 : idle 0: busy.
#define P_DMC_CMD_FILTER_CTRL3                     ((volatile uint32_t *)0xfd020108)
//bit 31.    force wbuf empty.
//bit 30:26  wbuf high level number
//bit 25:21  wbuf mid  level number
//bit 20:16  wbuf low level number
//bit 14:10  rbuf high level number
//bit 9:5    rbuf middle level number
//bit 4:0    rbuf low level number
#define P_DMC_CMD_FILTER_CTRL4                     ((volatile uint32_t *)0xfd02010c)
//bit 29:25.  tITW.long
//bit 24:20.  tITW. short
//bit 19:12   tAP auto precharge the bank not used if idle that long time.
//bit 11:6    write to read accesses if there write hit request.
//bit 5:0     read to write accesses if there write hit request.
#define P_DMC_CMD_FILTER_CTRL5                     ((volatile uint32_t *)0xfd020110)
//bit 31:24   Once ddr data bus switch to read, the maxmum read command number to give up the bus when there's write request pending for write buffer.
//bit 23:16   Once ddr data bus switch to write, the maxmum write command number to give up the bus when there's read request pending too long.
//bit 15:8.   Once ddr data bus switch to read, the minimum read command number to transfer back to write stage if there's still pending read request.
//bit 7:0.    Once ddr data bus switch to write, the minimum write command number to transfer back to read stage if there's still pending write request.
#define P_DMC_CMD_FILTER_CTRL6                     ((volatile uint32_t *)0xfd020114)
//bit 31:24   write urgent 3 request pending hold num.
//bit 23:16   write urgent 2 request pending hold num.
//bit 15:8.   write urgent 1 request pending hold num.
//bit 7:0.    write urgent 0 request pending hold num.
#define P_DMC_CMD_FILTER_CTRL7                     ((volatile uint32_t *)0xfd020118)
//bit 15:8    write to read waiting cycles if there write hit request.
//bit 7:0     read to write waiting cycles if there write hit request.
#define P_DMC_CMD_BUFFER_CTRL                      ((volatile uint32_t *)0xfd02011c)
//bit 31:26  total write buffer number. default 32.
//bit 25:20  total read buffer number. default 32.
//bit 19:8    reserved.
//bit 7:0    aw_pending_inc_num.  incease write ugent level 1 when write command waiting to in write buffer that long.
#define P_DMC_CMD_BUFFER_CTRL1                     ((volatile uint32_t *)0xfd020120)
//bit 29:24  read buffer number in non-urgent request.
//bit 23:16  read buffer bank miss watch dog threshold.
//bit 15:12  read buffer urgent level 3 counter inc weight.
//bit 11:8   read buffer urgent level 2 counter inc weight.
//bit 7:4    read buffer urgent level 1 counter inc weight.
//bit 3:0    read buffer urgent level 0 counter inc weight.
#define P_DMC_AXI0_CHAN_CTRL                       ((volatile uint32_t *)0xfd020200)
//bit 31       AXI interface request enable.
//bit 19       AXI interface  auto clock gating enable.  1: enable; 0 disable.
//bit 18       AXI interface  clock disable. 1 : disable clock.  0 enable clock.
//bit 17       AXI interface  dmc clock domain soft reset.  0 : reset.  1 : normal working mode.
//bit 16       AXI interface  s0 clock domain soft reset.   0 : reset.  1 : normal working mode.
//bit 15:12    ugt3 QOS level.
//bit 11:8     ugt2 QOS level.
//bit 7:4      ugt1 QOS level.
//bit 3:0      not used.
#define P_DMC_AXI0_HOLD_CTRL                       ((volatile uint32_t *)0xfd020204)
//31:24 write hold num.   max outstanding request number.
//23:16  write hold release num. if the outstanding request == hold num, then hold this request unless the outstanding request number bellow the hold release number, then continue to request.
//15:8 read hold num.   max outstanding request number.
//7:0  read hold release num. if the outstanding request == hold num, then hold this request unless the outstanding request number bellow the hold release number, then continue to request.
#define P_DMC_AXI5_HOLD_CTRL                       ((volatile uint32_t *)0xfd020254)
//31:24 write hold num.   max outstanding request number.
//23:16  write hold release num. if the outstanding request == hold num, then hold this request unless the outstanding request number bellow the hold release number, then continue to request.
//15:8 read hold num.   max outstanding request number.
//7:0  read hold release num. if the outstanding request == hold num, then hold this request unless the outstanding request number bellow the hold release number, then continue to request.
//`endif
//
// Closing file:  ../mmc_lp4/dmc/rtl/dmc_reg.vh
//
//
// Reading file:  ../mmc_lp4/dmc/rtl/dmc_siu_reg.vh
//
//`ifdef DMC_SIU_REG_DEFINE
//`else
//`define DMC_SIU_REG_DEFINE
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfd020400
// -----------------------------------------------
//`define DMC_DRAM_REG_BASE      32'hfd020400
//in A1 project, don't support hardware controlled frequency change.
#define P_DMC_DRAM_TMRD                            ((volatile uint32_t *)0xfd020400)
//bit 3:0.  tMRD.
#define P_DMC_DRAM_TRFC                            ((volatile uint32_t *)0xfd020404)
//bit 9:0 tRFC
#define P_DMC_DRAM_TRP                             ((volatile uint32_t *)0xfd020408)
//bit 21:16.  tRP for precharge all banks.
//bit 5:0.    tRP for precharge one bank.
#define P_DMC_DRAM_TRTW                            ((volatile uint32_t *)0xfd02040c)
//bit 5:0   tRTW
#define P_DMC_DRAM_TCL                             ((volatile uint32_t *)0xfd020410)
//bit 5:0  tCL/tRL. read latency.
#define P_DMC_DRAM_TCWL                            ((volatile uint32_t *)0xfd020414)
//bit 5:0.  CWL:  write latency.
#define P_DMC_DRAM_TRAS                            ((volatile uint32_t *)0xfd020418)
//bit 7:0.  tRAS.  minimum active to precharge time for same bank.
#define P_DMC_DRAM_TRC                             ((volatile uint32_t *)0xfd02041c)
//bit 7:0.  tRC.  minimum active to active time for same bank.
#define P_DMC_DRAM_TRCD                            ((volatile uint32_t *)0xfd020420)
//bit 7:0  tRCD  active to read/write timing for same bank.
#define P_DMC_DRAM_TRRD                            ((volatile uint32_t *)0xfd020424)
//bit 21:16.  tRRD_l  active bank A to active B in same band group for DDR4.
//bit 5:0.    tRRD/tRRD_s   active bank A to active bank b time.
//tRRD_s:   active bank A to active bank b in different bank grousp for DDR4.
#define P_DMC_DRAM_TFAW                            ((volatile uint32_t *)0xfd020428)
//bit 8:0   tFAW.  four active command windows
#define P_DMC_DRAM_TRTP                            ((volatile uint32_t *)0xfd02042c)
//bit 5:0  tRTP.
#define P_DMC_DRAM_TWR                             ((volatile uint32_t *)0xfd020430)
//bit 5:0 tWR.
#define P_DMC_DRAM_TWTR                            ((volatile uint32_t *)0xfd020434)
//bit 5:0 tWTR.
#define P_DMC_DRAM_TCCD                            ((volatile uint32_t *)0xfd020438)
//bit 19:16. tCCD/tCCD_l.
//bit 3:0 tCCD/tCCD_s    read to read command time or write to write command time.
#define P_DMC_DRAM_TEXSR                           ((volatile uint32_t *)0xfd02043c)
//bit 9:0. tEXSR.  EXIT SELF-REFRESH to read/write command.
#define P_DMC_DRAM_TXS                             ((volatile uint32_t *)0xfd020440)
//bit 9:0  tXS.  EXIT SELF_REFRESH to other command time
#define P_DMC_DRAM_TXP                             ((volatile uint32_t *)0xfd020444)
//bit 3:0. tXP.  EXIT power down to other command time
#define P_DMC_DRAM_TXPDLL                          ((volatile uint32_t *)0xfd020448)
//bit 9:0.  tXPDLL,  EXIT power down to read/write command time(need to relock PLL).
#define P_DMC_DRAM_TZQCS                           ((volatile uint32_t *)0xfd02044c)
//bit 11:0.  ZQCS command to other command time.
#define P_DMC_DRAM_TCKSRE                          ((volatile uint32_t *)0xfd020450)
//bit 4:0. enter self refresh to disable clock time.
#define P_DMC_DRAM_TCKSRX                          ((volatile uint32_t *)0xfd020454)
//bit 4:0. enable clock to exit self refresh time.
#define P_DMC_DRAM_TCKE                            ((volatile uint32_t *)0xfd020458)
//bit 4:0.  CKE high or low minimum time.
#define P_DMC_DRAM_TMOD                            ((volatile uint32_t *)0xfd02045c)
//bit 4:0  tMOD.  MRR/MRW to other command time.
#define P_DMC_DRAM_TDQS                            ((volatile uint32_t *)0xfd020460)
//bit 3:0 tDQS. the delay to access different rank.
#define P_DMC_DRAM_TRSTL                           ((volatile uint32_t *)0xfd020464)
//not used.
#define P_DMC_DRAM_TZQLAT                          ((volatile uint32_t *)0xfd020468)
//bit 5:0 ZQ LATCH command to other comand timing in LPDDR4 mode.
#define P_DMC_DRAM_TMRR                            ((volatile uint32_t *)0xfd02046c)
//bit 7:0 tMRR  not used in DMC. not support MR READ.
#define P_DMC_DRAM_TCKESR                          ((volatile uint32_t *)0xfd020470)
//bit 9:0 tCKESR.   CKE low minimum pulse in self refresh mode.
#define P_DMC_DRAM_TDPD                            ((volatile uint32_t *)0xfd020474)
//not support.
#define P_DMC_DRAM_DFITCTRLDELAY                   ((volatile uint32_t *)0xfd020478)
//bit 3:0. DFI_t_ctrldealy
#define P_DMC_DRAM_DFITPHYWRDATA                   ((volatile uint32_t *)0xfd02047c)
//bit 5:0.  dfi_t_phy_wrdata.
#define P_DMC_DRAM_DFITPHYWRLAT                    ((volatile uint32_t *)0xfd020480)
//bit 5:0.  dfi_t_phy_wrlat.  in DDR3/4/LPDDR3 mode:   WL -5.   in LPDDR4 mode: WL -5 + 2.
#define P_DMC_DRAM_DFITRDDATAEN                    ((volatile uint32_t *)0xfd020484)
//bit 5:0.  dfi_t_rddata_en.  in DDR3/4/LPDDR3 mode: RL -5. in LPDDR4 mode : RL -5 + 1.
#define P_DMC_DRAM_DFITPHYRDLAT                    ((volatile uint32_t *)0xfd020488)
//bit 5:0.  dfi_t_rdlat.
#define P_DMC_DRAM_DFITCTRLUPDMIN                  ((volatile uint32_t *)0xfd02048c)
//bit 7:0.  CTRLUPD_MIN  minimux clock cycle to maintain CTRLUPD_REQ.
#define P_DMC_DRAM_DFITCTRLUPDMAX                  ((volatile uint32_t *)0xfd020490)
//bit 7:0   CTRLUPD_MAX.  maxmum clock cycle to maintain CTRLUPD_REQ if no CTRLUPD_ACK response.
#define P_DMC_DRAM_DFITMSTRRESP                    ((volatile uint32_t *)0xfd020494)
//not used.
#define P_DMC_DRAM_DFITREFMSKI                     ((volatile uint32_t *)0xfd020498)
//not used.
#define P_DMC_DRAM_DFITCTRLUPDI                    ((volatile uint32_t *)0xfd02049c)
//not used.
#define P_DMC_DRAM_DFITDRAMCLK                     ((volatile uint32_t *)0xfd0204a0)
//bit 17    dram clk1 enable.
//bit 16    dram clk0 enable.
//bit 15:8  DRAM CLK disable waiting time
//bit 7:0   DRAM CLK enable  enable timer
#define P_DMC_DRAM_DFITLPRESP                      ((volatile uint32_t *)0xfd0204a8)
//bit 3:0.  dfi_lp_ctrl_req response time. after dfi_lp_ctrl_req asserted, and after response time if there's still no dfi_lp_ack respone, then drop the dfi_lp_ctrl_req.
#define P_DMC_DRAM_DFITPHYMSTR                     ((volatile uint32_t *)0xfd0204ac)
//bit 15  1: enable DFIPHYMASTER INTERFACE 0 disable DFIPHYMSTR en response.
// always 0  we would use DFI_RETRAIN function for PHY retrain required from LPDDR4.
#define P_DMC_DRAM_TCKECK                          ((volatile uint32_t *)0xfd0204b0)
//bit 4:0. tCKECK  from CKE low to assert dfi_dram_clk_disable time. this time + dfi_t_ctrl_delay
#define P_DMC_DRAM_TREFI                           ((volatile uint32_t *)0xfd0204b4)
//write this register will update the auto refresh related register to the auto refresh control logic.
//bit 31:24:  tZQCI dmc send zqci period.  unit is how much auto refresh period.
//bit 23:16   pvti  dmc send dfi_ctrlupd_req period.  unit is one auto refresh period.
//bit 15:8.   tREFI.dmc send auto refresh command period. unit is 100ns.
//bit 7:0.    t100ns period. unit is dmc clock cycles
#define P_DMC_DRAM_TSR                             ((volatile uint32_t *)0xfd0204b8)
//bit 5:0 tSR.  self resfresh enter to exit time.
#define P_DMC_DRAM_TCCDMW                          ((volatile uint32_t *)0xfd0204bc)
//bit 5:0.  4*tCCD in LPDDR4 mask write.
#define P_DMC_DRAM_TESCKE                          ((volatile uint32_t *)0xfd0204c0)
//bit 5:0  tESCKE.  enter self refresh to power time for LPDDR4.
#define P_DMC_DRAM_TREFI_DDR3                      ((volatile uint32_t *)0xfd0204c4)
//7:0. 8*DDR3 SDRAM tREFI time . the unit is t100ns.  use this to check in 8*tREFI time, the DMC should sent more than 16 auto REFRESH command.
#define P_DMC_DRAM_TZQCAL                          ((volatile uint32_t *)0xfd0204c8)
//7:0. ZQCAL for LPDDR4.
#define P_DMC_DRAM_T10US                           ((volatile uint32_t *)0xfd0204cc)
//10us clock cycle number used for LP2 mode.
#define P_DMC_DRAM_TMRRI                           ((volatile uint32_t *)0xfd0204d0)
//bit 7:0   tMRRI for MRR
#define P_DMC_DRAM_DFITPHYUPDTYPE0                 ((volatile uint32_t *)0xfd020600)
//dfi_phyupd_ack hold time for dfi_phyupd_req type = 0.
#define P_DMC_DRAM_DFITPHYUPDTYPE1                 ((volatile uint32_t *)0xfd020604)
//dfi_phyupd_ack hold time for dfi_phyupd_req type = 1.
#define P_DMC_DRAM_DFITPHYUPDTYPE2                 ((volatile uint32_t *)0xfd020608)
//dfi_phyupd_ack hold time for dfi_phyupd_req type = 2.
#define P_DMC_DRAM_DFITPHYUPDTYPE3                 ((volatile uint32_t *)0xfd02060c)
//dfi_phyupd_ack hold time for dfi_phyupd_req type = 3.
#define P_DMC_DRAM_DFIODTCFG                       ((volatile uint32_t *)0xfd020610)
//bit 12.  rank1 ODT default. default value for ODT[1] pins if theres no read/write activity.
//bit 11.  rank1 ODT write sel.  enable ODT[1] if there's write occur in rank1.
//bit 10.  rank1 ODT write nsel. enable ODT[1] if theres's write occur in rank0.
//bit 9.   rank1 odt read sel.   enable ODT[1] if there's read occur in rank1.
//bit 8.   rank1 odt read nsel.  enable ODT[1] if there's read occure in rank0.
//bit 4.   rank0 ODT default.    default value for ODT[0] pins if theres no read/write activity.
//bit 3.   rank0 ODT write sel.  enable ODT[0] if there's write occur in rank0.
//bit 2.   rank0 ODT write nsel. enable ODT[0] if theres's write occur in rank1.
//bit 1.   rank0 odt read sel.   enable ODT[0] if there's read occur in rank0.
//bit 0.   rank0 odt read nsel.  enable ODT[0] if there's read occure in rank1.
#define P_DMC_DRAM_DFIODTCFG1                      ((volatile uint32_t *)0xfd020614)
//bit 27:24  ODT length for BL8 read transfer.
//bit 19:16. ODT length for BL8 write transfer.
//bit 12:8.  ODT latency for reads.  suppose to be 0.
//bit 4:0.   ODT latency for writes.  suppose to be 0.
#define P_DMC_DRAM_MCFG                            ((volatile uint32_t *)0xfd020618)
//bit 14.   1: cke init low.  0 : cke init high.
//bit 13    1: dbi write enable only for LPDDR4.
//bit 12.   1: dbi read inversion.   0:  dbi read high inversion.
//bit 11.   1: dbi read enable. 0:  dbi not enabled.
//bit 10    1: enable staggered chip select for 2 ranks DRAM.
//bit 9     1: enable send auto refresh command to DDR SDRAM when PCTL is in CFG/STOP state.
//bit 8     send auto refr cmd before enter register triggered  self refresh
//bit 7     send auto refr cmd after exit regsiter triggered self refresh mode.
//bit 6     disable dram clock after enter register triggered self refresh.
//bit 5     send DFI_LP_REQ to PHY after enter register triggered elf refresh mode.
//bit 4     send DRAM to power down mode after enter self refresh. ONLY for LPDDR4.
//bit 3     send DFI_CTRLUPD_REQ after exit register triggered self refresh.
//bit 2     send ZQCS command after exit register triggered self refresh.
//bit 1     enable PHY triggered DFI_PHYUPD_REQ.
//bit 0     2T mode. always 1 in DDR3/4 mode.
#define P_DMC_DRAM_DFI_CTRL                        ((volatile uint32_t *)0xfd020624)
//bit 31 siu_dfi_lat err generation enable.  1: if dfi read latency violation, generate data error. 0 : disable.
//bit 30:0. not used.
#define P_DMC_DRAM_DFIINITCFG                      ((volatile uint32_t *)0xfd020628)
//bit 31.   dfi_init_complete status. read only.
//bit 15:14.  Frequency set 1 dfi_freq_ratio value.  Not used in A1
//bit 12:8    Frequency set 1 dfi_freq value.  Not used in A1
//bit 7:6     Frequency set 0 dfi_freq_ratio value.
//bit 5:1     Frequency set 0 dfi_freq value.
//bit 0.      dfi_init_start value  can be use manually config dfi_init_start signal.
#define P_DMC_DRAM_ZQ_CTRL                         ((volatile uint32_t *)0xfd02062c)
// only bit 0 can be enable in A1.
//bit 2  send ZQCS command to RANK0 then send comand to RANK1.
//bit 1. send ZQCS command to both RANK0 and RANK1 together.
//bit 0. send ZQCS command to only rank0.
#define P_DMC_DRAM_APD_CTRL                        ((volatile uint32_t *)0xfd020630)
//bit 19:16  DFI_LP_WAKEUP value in APD DFI_LP_REQ mode
//bit 12    1: exit power down slow mode(waiting PLL LOCK).  0 : fast mode.
//bit 11    enable DFI_LP_REQ when enter Auto power down mode.
//bit 10    disable DFI_clk_disable when enter auto power down mode.
//bit 9:0    0  disable auto power down mode.
//non zero value to enable auto power down when DMC is in idle state for this number of clock cycles.
#define P_DMC_DRAM_ASR_CTRL                        ((volatile uint32_t *)0xfd020634)
//bit [25] LP2 mode PHYCLK enable. 1 : keep PHY clock enable after enter LP2 mode. 0 : shutdown PHY clock after enter LP2 mode.
//bit [24] LP2 mode SRX waiting enable. if Self refresh too long, waiting 10us for the PHY stable before DDR SDRAM exit from self refresh state.
//bit [23:20] DFI_LP_WAKEUP value in self refresh DFI_LP_REQ mode.
//bit 17 : send REFRESH command after exit from auto self refersh mode(ASR).
//bit 16 : send REFERSH command before enter to Auto self refresh mode(ASR).
//bit 15 : send ZQCS command after exit from Auto self refresh mode(ASR).
//bit 14 : send dfi_ctrl_upd after exit from ASR mode
//bit 13 : send power down command when enter ASR mode. //for LPDDR4 only.
//bit 12 : set the PHY enter LP2 mode after enter ASR mode.
//bit 11 : send DFI_LP_REQ  after enter ASR mode.
//bit 10 : set DFI_CLK_DISABLE after enter ASR mode.
//bit 9:0.   0 disable auto ASR mode.
// Non-zero value enable ASR mode. when DMC is in idle state for this number of clock cycles, the DMC will enter ASR mode.
#define P_DMC_DRAM_PHYMSTR_CTRL                    ((volatile uint32_t *)0xfd020640)
//not used in A1. should be all 0.
#define P_DMC_DRAM_DFIODTRANKMAP                   ((volatile uint32_t *)0xfd020644)
//not used in A1.
#define P_DMC_DRAM_REFR_CTRL                       ((volatile uint32_t *)0xfd020648)
//bit 17:8 auto refresh request pending cnt if there's page hit request.
//bit 6  Disabled auto refresh command if over 16 auto refresh command sent in 2 TREFI_DDR3 period
//bit 5  enable dmc send ZQCS command .
//bit 4. enable dmc send DFI_CTRUPD_REQ.
//bit 3:1. how many refresh command send for one period. = this number + 1
//bit 0.  enable dmc send auto refresh command.
#define P_DMC_DRAM_FREQ_CTRL                       ((volatile uint32_t *)0xfd02064c)
//bit 31 .  write 1 to change freqency   read 0: finished.
//bit 30:9.  not used.
//bit 9.  1 : FREQ MRW done. let FREQ change machine continue.
//bit 8   FREQ WAIT. 1 when freq change finishes, state machine stop at self refresh state in case there's something need to handle.
//     0 after freq change finishes  the state machine go back to access state.
//bit 7   when change PLL setting, disable dmc clock
//bit 6   when change PLL setting, disable PHY DfiClk and DfiCtlClk.
//bit 5   check vpu_sleep_en ==1 when do FREQ change.  if vpu_sleep_en == 0, just wait.
//bit 4   nxt frequency selection.  1 = freq1. 0 = freq0.
//bit 3:1.  not used.
//bit 0.   current frequency selection.
#define P_DMC_DRAM_SCFG                            ((volatile uint32_t *)0xfd020650)
// bit 2:0 only one bit can be high at same time.
// bit 2  1 : to ask PCTL enter ACCESS STATE.  0 : deassert the request.
// bit 1  1 : to ask PCTL enter SELF REFRESH STATE.  0 : deassert the request.
// bit 0  1 : to ask PCTL enter STOP/CONFIG STATE .  0 : deassert the request.
#define P_DMC_DRAM_STAT                            ((volatile uint32_t *)0xfd020654)
//bit 31     rd latency error. 1: means after dfiphytrdlat cycles, the read data still not back.
//bit 28:24   dram_sr_state
//bit 23:20   stop_st
//bit 19:15   sleep_st
//bit 14:12  ACCESS STATUS 0 :  ACCESS is in normal working mode.
//1 :   ACCESS sending precharge command.
//2 :   ACCESS sending AUTO REFRESH command.
//3 :   ACCESS sending DIF_CTRLUPD_REQ command.
//4 :   ACCESS sending ZQCS command to DDR DRAM(ZQCAL for LPDDR4).
//5 :   ACCESS sending ZQLATCH command to  LPDDR4 only.
//bit 11:8   APD STATUS:   0 :   APD_IDLE
//1 :    APD sending PRECHARGE command
//2 :    APD sending CKE low command
//3 :    APD sending DISABLE DRAM CLOCK command
//4 :    APD sending DFI_LP_CTRL_REQ
//5 :    APD in Auto Power down mode.
//6 :    APD deassert DFI_LP_CTRL_REQ
//7 :    APD sending enable DRAM CLOCK command
//8 :    APD sending out CKE high command.
//bit 7:4: DRAM_STATUS:  0  :    DRAM IDLE
//1  :    DRAM_STOP/DRAM_CFG
//2  :    DRAM_ACCESS
//3  :    DRAM_SLEEP
//4  :    DRAM APD(AUTO POWER DOWN).
//5  :    IDLE -> STOP/CONFIG
//6  :    STOP -> SLEEP
//7  :    STOP -> ACCESS
//8  :    ACCESS -> SLEEP.
//9  :    ACCESS -> STOP
//A  :    ACCESS -> APD
//B  :    SLEEP -> STOP
//C  :    SLEEP -> ACCESS
//D  :    APD -> ACCESS
//bit 3        reserved.
//bit 2        1 : DRAM enter normal working state.
//bit 1        1 : DRAM enter sleep state. self refresh state.
//bit 0        1 : dram enter cfg state.
#define P_DMC_DRAM_STAT1                           ((volatile uint32_t *)0xfd020658)
//bit 11:8  freq_st.
//bit 7:5   train_st
//bit 4:0   dram_phy_st
#define P_DMC_PHY_RETRAINING_CTRL                  ((volatile uint32_t *)0xfd02065c)
// A1 not support LPDDR4 so this register is not used in A1.
//bit 31 :  phy_retraining enable.
//bit 30 :  check  vpu sleep_en.
//bit 25:24 : retraining dfi_freq[4:3], the [2:0] bit still use the dfi_freq bits to keep the frequency.
//bit 23:0: retraining period unit : 100ns.
#define P_DMC_DFI_ERR_STAT                         ((volatile uint32_t *)0xfd020660)
//LPDDR4 PHY DFI error infomation.
//bit 31:20. not used.
//bit 9.    ddr0_dfi_error
//bit 8:5   ddr0_dfi_error_info.
//bit 4.    ddr1_dfi_error.
//bit 3:0.  ddr1_dfi_error_info.
#define P_DMC_LP2_TIMER                            ((volatile uint32_t *)0xfd020668)
//bit 15:0.   timer setting to measure how long the chip is entered LP2 mode.
//this timer is 40bits counter with DMC PCLK.
//we use the high 16bits to compare this register. if the counter is large than this number,  that means  the PHY need addition 10us after wakeup the PHY and before exit self_refresh mode.
#define P_DMC_DRAM_DFI_SWAP_0                      ((volatile uint32_t *)0xfd020680)
//bit 5:0. dfi_act_n function select
// 6'h00 : the function itself.
// 6'h01 : act_n.
// 6'h02 : ras_n.
// 6'h03 : cas_n.
// 6'h04 : we_n.
// 6'h05 : bg[0].
// 6'h06 : bg[1].
// 6'h07 : ba[0].
// 6'h08 : ba[1].
// 6'h09 : ba[2].
// 6'h0a : a[0].
// 6'h0b : a[1].
// 6'h0c : a[2].
// 6'h0d : a[3].
// 6'h0e : a[4].
// 6'h0f : a[5].
// 6'h10 : a[6].
// 6'h11 : a[7].
// 6'h12 : a[8].
// 6'h13 : a[9].
// 6'h14 : a[10].
// 6'h15 : a[11].
// 6'h16 : a[12].
// 6'h17 : a[13].
// 6'h18 : a[14].
// 6'h19 : a[15].
// 6'h1a : a[16].
// 6'h1b : a[17].
#define P_DMC_DRAM_DFI_SWAP_1                      ((volatile uint32_t *)0xfd020684)
//bit 5:0. dfi_ras_n function select
// 6'h00 : the function itself.
// 6'h01 : act_n.
// 6'h02 : ras_n.
// 6'h03 : cas_n.
// 6'h04 : we_n.
// 6'h05 : bg[0].
// 6'h06 : bg[1].
// 6'h07 : ba[0].
// 6'h08 : ba[1].
// 6'h09 : ba[2].
// 6'h0a : a[0].
// 6'h0b : a[1].
// 6'h0c : a[2].
// 6'h0d : a[3].
// 6'h0e : a[4].
// 6'h0f : a[5].
// 6'h10 : a[6].
// 6'h11 : a[7].
// 6'h12 : a[8].
// 6'h13 : a[9].
// 6'h14 : a[10].
// 6'h15 : a[11].
// 6'h16 : a[12].
// 6'h17 : a[13].
// 6'h18 : a[14].
// 6'h19 : a[15].
// 6'h1a : a[16].
// 6'h1b : a[17].
#define P_DMC_DRAM_DFI_SWAP_2                      ((volatile uint32_t *)0xfd020688)
//bit 5:0. dfi_cas_n function select
// 6'h00 : the function itself.
// 6'h01 : act_n.
// 6'h02 : ras_n.
// 6'h03 : cas_n.
// 6'h04 : we_n.
// 6'h05 : bg[0].
// 6'h06 : bg[1].
// 6'h07 : ba[0].
// 6'h08 : ba[1].
// 6'h09 : ba[2].
// 6'h0a : a[0].
// 6'h0b : a[1].
// 6'h0c : a[2].
// 6'h0d : a[3].
// 6'h0e : a[4].
// 6'h0f : a[5].
// 6'h10 : a[6].
// 6'h11 : a[7].
// 6'h12 : a[8].
// 6'h13 : a[9].
// 6'h14 : a[10].
// 6'h15 : a[11].
// 6'h16 : a[12].
// 6'h17 : a[13].
// 6'h18 : a[14].
// 6'h19 : a[15].
// 6'h1a : a[16].
// 6'h1b : a[17].
#define P_DMC_DRAM_DFI_SWAP_3                      ((volatile uint32_t *)0xfd02068c)
//bit 5:0. dfi_we_n function select
// 6'h00 : the function itself.
// 6'h01 : act_n.
// 6'h02 : ras_n.
// 6'h03 : cas_n.
// 6'h04 : we_n.
// 6'h05 : bg[0].
// 6'h06 : bg[1].
// 6'h07 : ba[0].
// 6'h08 : ba[1].
// 6'h09 : ba[2].
// 6'h0a : a[0].
// 6'h0b : a[1].
// 6'h0c : a[2].
// 6'h0d : a[3].
// 6'h0e : a[4].
// 6'h0f : a[5].
// 6'h10 : a[6].
// 6'h11 : a[7].
// 6'h12 : a[8].
// 6'h13 : a[9].
// 6'h14 : a[10].
// 6'h15 : a[11].
// 6'h16 : a[12].
// 6'h17 : a[13].
// 6'h18 : a[14].
// 6'h19 : a[15].
// 6'h1a : a[16].
// 6'h1b : a[17].
#define P_DMC_DRAM_DFI_SWAP_4                      ((volatile uint32_t *)0xfd020690)
//bit 5:0. dfi_bg0 function select
// 6'h00 : the function itself.
// 6'h01 : act_n.
// 6'h02 : ras_n.
// 6'h03 : cas_n.
// 6'h04 : we_n.
// 6'h05 : bg[0].
// 6'h06 : bg[1].
// 6'h07 : ba[0].
// 6'h08 : ba[1].
// 6'h09 : ba[2].
// 6'h0a : a[0].
// 6'h0b : a[1].
// 6'h0c : a[2].
// 6'h0d : a[3].
// 6'h0e : a[4].
// 6'h0f : a[5].
// 6'h10 : a[6].
// 6'h11 : a[7].
// 6'h12 : a[8].
// 6'h13 : a[9].
// 6'h14 : a[10].
// 6'h15 : a[11].
// 6'h16 : a[12].
// 6'h17 : a[13].
// 6'h18 : a[14].
// 6'h19 : a[15].
// 6'h1a : a[16].
// 6'h1b : a[17].
#define P_DMC_DRAM_DFI_SWAP_5                      ((volatile uint32_t *)0xfd020694)
//bit 5:0. dfi_bg[1] function select
// 6'h00 : the function itself.
// 6'h01 : act_n.
// 6'h02 : ras_n.
// 6'h03 : cas_n.
// 6'h04 : we_n.
// 6'h05 : bg[0].
// 6'h06 : bg[1].
// 6'h07 : ba[0].
// 6'h08 : ba[1].
// 6'h09 : ba[2].
// 6'h0a : a[0].
// 6'h0b : a[1].
// 6'h0c : a[2].
// 6'h0d : a[3].
// 6'h0e : a[4].
// 6'h0f : a[5].
// 6'h10 : a[6].
// 6'h11 : a[7].
// 6'h12 : a[8].
// 6'h13 : a[9].
// 6'h14 : a[10].
// 6'h15 : a[11].
// 6'h16 : a[12].
// 6'h17 : a[13].
// 6'h18 : a[14].
// 6'h19 : a[15].
// 6'h1a : a[16].
// 6'h1b : a[17].
#define P_DMC_DRAM_DFI_SWAP_6                      ((volatile uint32_t *)0xfd020698)
//bit 5:0. dfi_ba[0] function select
// 6'h00 : the function itself.
// 6'h01 : act_n.
// 6'h02 : ras_n.
// 6'h03 : cas_n.
// 6'h04 : we_n.
// 6'h05 : bg[0].
// 6'h06 : bg[1].
// 6'h07 : ba[0].
// 6'h08 : ba[1].
// 6'h09 : ba[2].
// 6'h0a : a[0].
// 6'h0b : a[1].
// 6'h0c : a[2].
// 6'h0d : a[3].
// 6'h0e : a[4].
// 6'h0f : a[5].
// 6'h10 : a[6].
// 6'h11 : a[7].
// 6'h12 : a[8].
// 6'h13 : a[9].
// 6'h14 : a[10].
// 6'h15 : a[11].
// 6'h16 : a[12].
// 6'h17 : a[13].
// 6'h18 : a[14].
// 6'h19 : a[15].
// 6'h1a : a[16].
// 6'h1b : a[17].
#define P_DMC_DRAM_DFI_SWAP_7                      ((volatile uint32_t *)0xfd02069c)
//bit 5:0. dfi_ba[1] function select
// 6'h00 : the function itself.
// 6'h01 : act_n.
// 6'h02 : ras_n.
// 6'h03 : cas_n.
// 6'h04 : we_n.
// 6'h05 : bg[0].
// 6'h06 : bg[1].
// 6'h07 : ba[0].
// 6'h08 : ba[1].
// 6'h09 : ba[2].
// 6'h0a : a[0].
// 6'h0b : a[1].
// 6'h0c : a[2].
// 6'h0d : a[3].
// 6'h0e : a[4].
// 6'h0f : a[5].
// 6'h10 : a[6].
// 6'h11 : a[7].
// 6'h12 : a[8].
// 6'h13 : a[9].
// 6'h14 : a[10].
// 6'h15 : a[11].
// 6'h16 : a[12].
// 6'h17 : a[13].
// 6'h18 : a[14].
// 6'h19 : a[15].
// 6'h1a : a[16].
// 6'h1b : a[17].
#define P_DMC_DRAM_DFI_SWAP_8                      ((volatile uint32_t *)0xfd0206a0)
//bit 5:0. dfi_ba[2] function select
// 6'h00 : the function itself.
// 6'h01 : act_n.
// 6'h02 : ras_n.
// 6'h03 : cas_n.
// 6'h04 : we_n.
// 6'h05 : bg[0].
// 6'h06 : bg[1].
// 6'h07 : ba[0].
// 6'h08 : ba[1].
// 6'h09 : ba[2].
// 6'h0a : a[0].
// 6'h0b : a[1].
// 6'h0c : a[2].
// 6'h0d : a[3].
// 6'h0e : a[4].
// 6'h0f : a[5].
// 6'h10 : a[6].
// 6'h11 : a[7].
// 6'h12 : a[8].
// 6'h13 : a[9].
// 6'h14 : a[10].
// 6'h15 : a[11].
// 6'h16 : a[12].
// 6'h17 : a[13].
// 6'h18 : a[14].
// 6'h19 : a[15].
// 6'h1a : a[16].
// 6'h1b : a[17].
#define P_DMC_DRAM_DFI_SWAP_9                      ((volatile uint32_t *)0xfd0206a4)
//bit 5:0. dfi_a[0] function select
// 6'h00 : the function itself.
// 6'h01 : act_n.
// 6'h02 : ras_n.
// 6'h03 : cas_n.
// 6'h04 : we_n.
// 6'h05 : bg[0].
// 6'h06 : bg[1].
// 6'h07 : ba[0].
// 6'h08 : ba[1].
// 6'h09 : ba[2].
// 6'h0a : a[0].
// 6'h0b : a[1].
// 6'h0c : a[2].
// 6'h0d : a[3].
// 6'h0e : a[4].
// 6'h0f : a[5].
// 6'h10 : a[6].
// 6'h11 : a[7].
// 6'h12 : a[8].
// 6'h13 : a[9].
// 6'h14 : a[10].
// 6'h15 : a[11].
// 6'h16 : a[12].
// 6'h17 : a[13].
// 6'h18 : a[14].
// 6'h19 : a[15].
// 6'h1a : a[16].
// 6'h1b : a[17].
#define P_DMC_DRAM_DFI_SWAP_10                     ((volatile uint32_t *)0xfd0206a8)
//bit 5:0. dfi_a[1] function select
#define P_DMC_DRAM_DFI_SWAP_11                     ((volatile uint32_t *)0xfd0206ac)
//bit 5:0. dfi_a[2] function select
#define P_DMC_DRAM_DFI_SWAP_12                     ((volatile uint32_t *)0xfd0206b0)
//bit 5:0. dfi_a[3] function select
#define P_DMC_DRAM_DFI_SWAP_13                     ((volatile uint32_t *)0xfd0206b4)
//bit 5:0. dfi_a[4] function select
#define P_DMC_DRAM_DFI_SWAP_14                     ((volatile uint32_t *)0xfd0206b8)
//bit 5:0. dfi_a[5] function select
#define P_DMC_DRAM_DFI_SWAP_15                     ((volatile uint32_t *)0xfd0206bc)
//bit 5:0. dfi_a[6] function select
#define P_DMC_DRAM_DFI_SWAP_16                     ((volatile uint32_t *)0xfd0206c0)
//bit 5:0. dfi_a[7] function select
#define P_DMC_DRAM_DFI_SWAP_17                     ((volatile uint32_t *)0xfd0206c4)
//bit 5:0. dfi_a[8] function select
#define P_DMC_DRAM_DFI_SWAP_18                     ((volatile uint32_t *)0xfd0206c8)
//bit 5:0. dfi_a[9] function select
#define P_DMC_DRAM_DFI_SWAP_19                     ((volatile uint32_t *)0xfd0206cc)
//bit 5:0. dfi_a[10] function select
#define P_DMC_DRAM_DFI_SWAP_20                     ((volatile uint32_t *)0xfd0206d0)
//bit 5:0. dfi_a[11] function select
#define P_DMC_DRAM_DFI_SWAP_21                     ((volatile uint32_t *)0xfd0206d4)
//bit 5:0. dfi_a[12] function select
#define P_DMC_DRAM_DFI_SWAP_22                     ((volatile uint32_t *)0xfd0206d8)
//bit 5:0. dfi_a[13] function select
#define P_DMC_DRAM_DFI_SWAP_23                     ((volatile uint32_t *)0xfd0206dc)
//bit 5:0. dfi_a[14] function select
#define P_DMC_DRAM_DFI_SWAP_24                     ((volatile uint32_t *)0xfd0206e0)
//bit 5:0. dfi_a[15] function select
#define P_DMC_DRAM_DFI_SWAP_25                     ((volatile uint32_t *)0xfd0206e4)
//bit 5:0. dfi_a[16] function select
#define P_DMC_DRAM_DFI_SWAP_26                     ((volatile uint32_t *)0xfd0206ec)
//bit 5:0. dfi_a[17] function select
//
// Closing file:  ../mmc_lp4/dmc/rtl/dmc_siu_reg.vh
//
//
// Reading file:  ../mmc_lp4/dmc/rtl/dmc_sticky_reg.vh
//
//`ifdef DMC_STICKY_REG_DEFINE
//`else
//`define DMC_STICKY_REG_DEFINE
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfd020800
// -----------------------------------------------
//`define DMC_STICKY_REG_BASE   32'hfd020800
//those register is for software save some temporary value. and because it's in RAM. it won't lose if DMC get reseted.
#define P_DMC_STICKY_0                             ((volatile uint32_t *)0xfd020800)
#define P_DMC_STICKY_1                             ((volatile uint32_t *)0xfd020804)
#define P_DMC_STICKY_2                             ((volatile uint32_t *)0xfd020808)
#define P_DMC_STICKY_3                             ((volatile uint32_t *)0xfd02080c)
#define P_DMC_STICKY_4                             ((volatile uint32_t *)0xfd020810)
#define P_DMC_STICKY_5                             ((volatile uint32_t *)0xfd020814)
#define P_DMC_STICKY_6                             ((volatile uint32_t *)0xfd020818)
#define P_DMC_STICKY_7                             ((volatile uint32_t *)0xfd02081c)
#define P_DMC_STICKY_8                             ((volatile uint32_t *)0xfd020820)
#define P_DMC_STICKY_9                             ((volatile uint32_t *)0xfd020824)
#define P_DMC_STICKY_10                            ((volatile uint32_t *)0xfd020828)
#define P_DMC_STICKY_11                            ((volatile uint32_t *)0xfd02082c)
#define P_DMC_STICKY_12                            ((volatile uint32_t *)0xfd020830)
#define P_DMC_STICKY_13                            ((volatile uint32_t *)0xfd020834)
#define P_DMC_STICKY_14                            ((volatile uint32_t *)0xfd020838)
#define P_DMC_STICKY_15                            ((volatile uint32_t *)0xfd02083c)
#define P_DMC_STICKY_16                            ((volatile uint32_t *)0xfd020840)
#define P_DMC_STICKY_17                            ((volatile uint32_t *)0xfd020844)
#define P_DMC_STICKY_18                            ((volatile uint32_t *)0xfd020848)
#define P_DMC_STICKY_19                            ((volatile uint32_t *)0xfd02084c)
#define P_DMC_STICKY_20                            ((volatile uint32_t *)0xfd020850)
#define P_DMC_STICKY_21                            ((volatile uint32_t *)0xfd020854)
#define P_DMC_STICKY_22                            ((volatile uint32_t *)0xfd020858)
#define P_DMC_STICKY_23                            ((volatile uint32_t *)0xfd02085c)
#define P_DMC_STICKY_24                            ((volatile uint32_t *)0xfd020860)
#define P_DMC_STICKY_25                            ((volatile uint32_t *)0xfd020864)
#define P_DMC_STICKY_26                            ((volatile uint32_t *)0xfd020868)
#define P_DMC_STICKY_27                            ((volatile uint32_t *)0xfd02086c)
#define P_DMC_STICKY_28                            ((volatile uint32_t *)0xfd020870)
#define P_DMC_STICKY_29                            ((volatile uint32_t *)0xfd020874)
#define P_DMC_STICKY_30                            ((volatile uint32_t *)0xfd020878)
#define P_DMC_STICKY_31                            ((volatile uint32_t *)0xfd02087c)
#define P_DMC_STICKY_32                            ((volatile uint32_t *)0xfd020880)
#define P_DMC_STICKY_33                            ((volatile uint32_t *)0xfd020884)
#define P_DMC_STICKY_34                            ((volatile uint32_t *)0xfd020888)
#define P_DMC_STICKY_35                            ((volatile uint32_t *)0xfd02088c)
#define P_DMC_STICKY_36                            ((volatile uint32_t *)0xfd020890)
#define P_DMC_STICKY_37                            ((volatile uint32_t *)0xfd020894)
#define P_DMC_STICKY_38                            ((volatile uint32_t *)0xfd020898)
#define P_DMC_STICKY_39                            ((volatile uint32_t *)0xfd02089c)
#define P_DMC_STICKY_40                            ((volatile uint32_t *)0xfd0208a0)
#define P_DMC_STICKY_41                            ((volatile uint32_t *)0xfd0208a4)
#define P_DMC_STICKY_42                            ((volatile uint32_t *)0xfd0208a8)
#define P_DMC_STICKY_43                            ((volatile uint32_t *)0xfd0208ac)
#define P_DMC_STICKY_44                            ((volatile uint32_t *)0xfd0208b0)
#define P_DMC_STICKY_45                            ((volatile uint32_t *)0xfd0208b4)
#define P_DMC_STICKY_46                            ((volatile uint32_t *)0xfd0208b8)
#define P_DMC_STICKY_47                            ((volatile uint32_t *)0xfd0208bc)
#define P_DMC_STICKY_48                            ((volatile uint32_t *)0xfd0208c0)
#define P_DMC_STICKY_49                            ((volatile uint32_t *)0xfd0208c4)
#define P_DMC_STICKY_50                            ((volatile uint32_t *)0xfd0208c8)
#define P_DMC_STICKY_51                            ((volatile uint32_t *)0xfd0208cc)
#define P_DMC_STICKY_52                            ((volatile uint32_t *)0xfd0208d0)
#define P_DMC_STICKY_53                            ((volatile uint32_t *)0xfd0208d4)
#define P_DMC_STICKY_54                            ((volatile uint32_t *)0xfd0208d8)
#define P_DMC_STICKY_55                            ((volatile uint32_t *)0xfd0208dc)
#define P_DMC_STICKY_56                            ((volatile uint32_t *)0xfd0208e0)
#define P_DMC_STICKY_57                            ((volatile uint32_t *)0xfd0208e4)
#define P_DMC_STICKY_58                            ((volatile uint32_t *)0xfd0208e8)
#define P_DMC_STICKY_59                            ((volatile uint32_t *)0xfd0208ec)
#define P_DMC_STICKY_60                            ((volatile uint32_t *)0xfd0208f0)
#define P_DMC_STICKY_61                            ((volatile uint32_t *)0xfd0208f4)
#define P_DMC_STICKY_62                            ((volatile uint32_t *)0xfd0208f8)
#define P_DMC_STICKY_63                            ((volatile uint32_t *)0xfd0208fc)
//`endif
//
// Closing file:  ../mmc_lp4/dmc/rtl/dmc_sticky_reg.vh
//
//========================================================================
//  APB0_RESET_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe000000
// -----------------------------------------------
#define P_RESETCTRL_RESET0                         ((volatile uint32_t *)0xfe000000)
#define P_RESETCTRL_RESET1                         ((volatile uint32_t *)0xfe000004)
#define P_RESETCTRL_RESET2                         ((volatile uint32_t *)0xfe000008)
#define P_RESETCTRL_RESET0_LEVEL                   ((volatile uint32_t *)0xfe000040)
#define P_RESETCTRL_RESET1_LEVEL                   ((volatile uint32_t *)0xfe000044)
#define P_RESETCTRL_RESET2_LEVEL                   ((volatile uint32_t *)0xfe000048)
#define P_RESETCTRL_RESET0_MASK                    ((volatile uint32_t *)0xfe000080)
#define P_RESETCTRL_RESET1_MASK                    ((volatile uint32_t *)0xfe000084)
#define P_RESETCTRL_RESET2_MASK                    ((volatile uint32_t *)0xfe000088)
#define P_RESETCTRL_RESET_HOLD_CYC                 ((volatile uint32_t *)0xfe0000c0)
#define P_RESETCTRL_WATCHDOG_CTRL0                 ((volatile uint32_t *)0xfe000100)
#define P_RESETCTRL_WATCHDOG_CTRL1                 ((volatile uint32_t *)0xfe000104)
#define P_RESETCTRL_WATCHDOG_CNT                   ((volatile uint32_t *)0xfe000108)
#define P_RESETCTRL_WATCHDOG_CLR                   ((volatile uint32_t *)0xfe00010c)
#define P_RESETCTRL_SEC_RESET0                     ((volatile uint32_t *)0xfe000140)
#define P_RESETCTRL_SEC_RESET0_LEVEL               ((volatile uint32_t *)0xfe000144)
#define P_RESETCTRL_SEC_RESET0_MASK                ((volatile uint32_t *)0xfe000148)
//========================================================================
//  APB0_PAD_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe000400
// -----------------------------------------------
#define P_PADCTRL_PIN_MUX_REG0                     ((volatile uint32_t *)0xfe000400)
#define P_PADCTRL_PIN_MUX_REG1                     ((volatile uint32_t *)0xfe000404)
#define P_PADCTRL_PIN_MUX_REG2                     ((volatile uint32_t *)0xfe000408)
#define P_PADCTRL_PIN_MUX_REG3                     ((volatile uint32_t *)0xfe00040c)
#define P_PADCTRL_PIN_MUX_REG4                     ((volatile uint32_t *)0xfe000410)
#define P_PADCTRL_PIN_MUX_REG5                     ((volatile uint32_t *)0xfe000414)
#define P_PADCTRL_PIN_MUX_REG6                     ((volatile uint32_t *)0xfe000418)
#define P_PADCTRL_PIN_MUX_REG7                     ((volatile uint32_t *)0xfe00041c)
#define P_PADCTRL_PIN_MUX_REG8                     ((volatile uint32_t *)0xfe000420)
#define P_PADCTRL_PIN_MUX_REG9                     ((volatile uint32_t *)0xfe000424)
#define P_PADCTRL_PIN_MUX_REGA                     ((volatile uint32_t *)0xfe000428)
#define P_PADCTRL_PIN_MUX_REGB                     ((volatile uint32_t *)0xfe00042c)
#define P_PADCTRL_PIN_MUX_REGC                     ((volatile uint32_t *)0xfe000430)
#define P_PADCTRL_PIN_MUX_REGD                     ((volatile uint32_t *)0xfe000434)
#define P_PADCTRL_PIN_MUX_REGE                     ((volatile uint32_t *)0xfe000438)
#define P_PADCTRL_PIN_MUX_REGF                     ((volatile uint32_t *)0xfe00043c)
#define P_PADCTRL_GPIO_IRQ_CTRL0                   ((volatile uint32_t *)0xfe000440)
#define P_PADCTRL_GPIO_IRQ_CTRL1                   ((volatile uint32_t *)0xfe000444)
#define P_PADCTRL_GPIO_IRQ_CTRL2                   ((volatile uint32_t *)0xfe000448)
#define P_PADCTRL_GPIO_IRQ_CTRL3                   ((volatile uint32_t *)0xfe00044c)
#define P_PADCTRL_GPIO_IRQ_CTRL4                   ((volatile uint32_t *)0xfe000450)
#define P_PADCTRL_WORLDSYNC_CTRL0                  ((volatile uint32_t *)0xfe000460)
#define P_PADCTRL_GPIO_MSR_CTRL0                   ((volatile uint32_t *)0xfe000464)
#define P_PADCTRL_GPIOP_I                          ((volatile uint32_t *)0xfe000480)
#define P_PADCTRL_GPIOP_O                          ((volatile uint32_t *)0xfe000484)
#define P_PADCTRL_GPIOP_OEN                        ((volatile uint32_t *)0xfe000488)
#define P_PADCTRL_GPIOP_PULL_EN                    ((volatile uint32_t *)0xfe00048c)
#define P_PADCTRL_GPIOP_PULL_UP                    ((volatile uint32_t *)0xfe000490)
#define P_PADCTRL_GPIOP_DS                         ((volatile uint32_t *)0xfe000494)
#define P_PADCTRL_GPIOB_I                          ((volatile uint32_t *)0xfe0004c0)
#define P_PADCTRL_GPIOB_O                          ((volatile uint32_t *)0xfe0004c4)
#define P_PADCTRL_GPIOB_OEN                        ((volatile uint32_t *)0xfe0004c8)
#define P_PADCTRL_GPIOB_PULL_EN                    ((volatile uint32_t *)0xfe0004cc)
#define P_PADCTRL_GPIOB_PULL_UP                    ((volatile uint32_t *)0xfe0004d0)
#define P_PADCTRL_GPIOB_DS                         ((volatile uint32_t *)0xfe0004d4)
#define P_PADCTRL_GPIOX_I                          ((volatile uint32_t *)0xfe000500)
#define P_PADCTRL_GPIOX_O                          ((volatile uint32_t *)0xfe000504)
#define P_PADCTRL_GPIOX_OEN                        ((volatile uint32_t *)0xfe000508)
#define P_PADCTRL_GPIOX_PULL_EN                    ((volatile uint32_t *)0xfe00050c)
#define P_PADCTRL_GPIOX_PULL_UP                    ((volatile uint32_t *)0xfe000510)
#define P_PADCTRL_GPIOX_DS                         ((volatile uint32_t *)0xfe000514)
#define P_PADCTRL_GPIOX_DS_EXT                     ((volatile uint32_t *)0xfe000518)
#define P_PADCTRL_GPIOF_I                          ((volatile uint32_t *)0xfe000540)
#define P_PADCTRL_GPIOF_O                          ((volatile uint32_t *)0xfe000544)
#define P_PADCTRL_GPIOF_OEN                        ((volatile uint32_t *)0xfe000548)
#define P_PADCTRL_GPIOF_PULL_EN                    ((volatile uint32_t *)0xfe00054c)
#define P_PADCTRL_GPIOF_PULL_UP                    ((volatile uint32_t *)0xfe000550)
#define P_PADCTRL_GPIOF_DS                         ((volatile uint32_t *)0xfe000554)
#define P_PADCTRL_GPIOA_I                          ((volatile uint32_t *)0xfe000580)
#define P_PADCTRL_GPIOA_O                          ((volatile uint32_t *)0xfe000584)
#define P_PADCTRL_GPIOA_OEN                        ((volatile uint32_t *)0xfe000588)
#define P_PADCTRL_GPIOA_PULL_EN                    ((volatile uint32_t *)0xfe00058c)
#define P_PADCTRL_GPIOA_PULL_UP                    ((volatile uint32_t *)0xfe000590)
#define P_PADCTRL_GPIOA_DS                         ((volatile uint32_t *)0xfe000594)
#define P_PADCTRL_TESTN_I                          ((volatile uint32_t *)0xfe0005c0)
#define P_PADCTRL_TESTN_O                          ((volatile uint32_t *)0xfe0005c4)
#define P_PADCTRL_TESTN_OEN                        ((volatile uint32_t *)0xfe0005c8)
#define P_PADCTRL_TESTN_PULL_EN                    ((volatile uint32_t *)0xfe0005cc)
#define P_PADCTRL_TESTN_PULL_UP                    ((volatile uint32_t *)0xfe0005d0)
#define P_PADCTRL_TESTN_DS                         ((volatile uint32_t *)0xfe0005d4)
#define P_PADCTRL_MUTE_CTRL                        ((volatile uint32_t *)0xfe000600)
//========================================================================
//  APB0_CLK_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe000800
// -----------------------------------------------
#define P_CLKTREE_SYS_OSCIN_CTRL                   ((volatile uint32_t *)0xfe000800)
#define P_CLKTREE_RTC_BY_OSCIN_CTRL0               ((volatile uint32_t *)0xfe000804)
#define P_CLKTREE_RTC_BY_OSCIN_CTRL1               ((volatile uint32_t *)0xfe000808)
#define P_CLKTREE_RTC_CTRL                         ((volatile uint32_t *)0xfe00080c)
#define P_CLKTREE_SYS_CLK_CTRL0                    ((volatile uint32_t *)0xfe000810)
#define P_CLKTREE_AXI_CLK_CTRL0                    ((volatile uint32_t *)0xfe000814)
#define P_CLKTREE_SYS_CLK_EN0                      ((volatile uint32_t *)0xfe00081c)
#define P_CLKTREE_SYS_CLK_EN1                      ((volatile uint32_t *)0xfe000820)
#define P_CLKTREE_AXI_CLK_EN                       ((volatile uint32_t *)0xfe000824)
#define P_CLKTREE_DSPA_CLK_EN                      ((volatile uint32_t *)0xfe000828)
#define P_CLKTREE_DSPB_CLK_EN                      ((volatile uint32_t *)0xfe00082c)
#define P_CLKTREE_DSPA_CLK_CTRL0                   ((volatile uint32_t *)0xfe000830)
#define P_CLKTREE_DSPB_CLK_CTRL0                   ((volatile uint32_t *)0xfe000834)
#define P_CLKTREE_CLK12_24_CTRL                    ((volatile uint32_t *)0xfe000838)
#define P_CLKTREE_GEN_CLK_CTRL                     ((volatile uint32_t *)0xfe00083c)
#define P_CLKTREE_TIMESTAMP_CTRL0                  ((volatile uint32_t *)0xfe000840)
#define P_CLKTREE_TIMESTAMP_CTRL1                  ((volatile uint32_t *)0xfe000844)
#define P_CLKTREE_TIMESTAMP_CTRL2                  ((volatile uint32_t *)0xfe000848)
#define P_CLKTREE_TIMESTAMP_VAL0                   ((volatile uint32_t *)0xfe00084c)
#define P_CLKTREE_TIMESTAMP_VAL1                   ((volatile uint32_t *)0xfe000850)
#define P_CLKTREE_TIMEBASE_CTRL0                   ((volatile uint32_t *)0xfe000854)
#define P_CLKTREE_TIMEBASE_CTRL1                   ((volatile uint32_t *)0xfe000858)
#define P_CLKTREE_SAR_ADC_CLK_CTRL                 ((volatile uint32_t *)0xfe0008c0)
#define P_CLKTREE_PWM_CLK_AB_CTRL                  ((volatile uint32_t *)0xfe0008c4)
#define P_CLKTREE_PWM_CLK_CD_CTRL                  ((volatile uint32_t *)0xfe0008c8)
#define P_CLKTREE_PWM_CLK_EF_CTRL                  ((volatile uint32_t *)0xfe0008cc)
#define P_CLKTREE_SPICC_CLK_CTRL                   ((volatile uint32_t *)0xfe0008d0)
#define P_CLKTREE_TS_CLK_CTRL                      ((volatile uint32_t *)0xfe0008d4)
#define P_CLKTREE_SPIFC_CLK_CTRL                   ((volatile uint32_t *)0xfe0008d8)
#define P_CLKTREE_USB_BUSCLK_CTRL                  ((volatile uint32_t *)0xfe0008dc)
#define P_CLKTREE_SD_EMMC_CLK_CTRL                 ((volatile uint32_t *)0xfe0008e0)
#define P_CLKTREE_CECA_CLK_CTRL0                   ((volatile uint32_t *)0xfe0008e4)
#define P_CLKTREE_CECA_CLK_CTRL1                   ((volatile uint32_t *)0xfe0008e8)
#define P_CLKTREE_CECB_CLK_CTRL0                   ((volatile uint32_t *)0xfe0008ec)
#define P_CLKTREE_CECB_CLK_CTRL1                   ((volatile uint32_t *)0xfe0008f0)
#define P_CLKTREE_PSRAM_CLK_CTRL                   ((volatile uint32_t *)0xfe0008f4)
#define P_CLKTREE_DMC_CLK_CTRL                     ((volatile uint32_t *)0xfe0008f8)
#define P_CLKTREE_FCLK_DIV1_SEL                    ((volatile uint32_t *)0xfe0008fc)
#define P_CLKTREE_TST_CTRL0                        ((volatile uint32_t *)0xfe000900)
//========================================================================
//  APB0_CEC - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe000c00
// -----------------------------------------------
#define P_CECA_GEN_CNTL                            ((volatile uint32_t *)0xfe000c00)
#define P_CECA_RW_REG                              ((volatile uint32_t *)0xfe000c04)
#define P_CECA_INTR_MASKN                          ((volatile uint32_t *)0xfe000c08)
#define P_CECA_INTR_CLR                            ((volatile uint32_t *)0xfe000c0c)
#define P_CECA_INTR_STAT                           ((volatile uint32_t *)0xfe000c10)
#define P_CECB_GEN_CNTL                            ((volatile uint32_t *)0xfe000c40)
#define P_CECB_RW_REG                              ((volatile uint32_t *)0xfe000c44)
#define P_CECB_INTR_MASKN                          ((volatile uint32_t *)0xfe000c48)
#define P_CECB_INTR_CLR                            ((volatile uint32_t *)0xfe000c4c)
#define P_CECB_INTR_STAT                           ((volatile uint32_t *)0xfe000c50)
//========================================================================
//  APB0_IR_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe001000
// -----------------------------------------------
#define P_IRCTRL_IR_DEC_LDR_ACTIVE                 ((volatile uint32_t *)0xfe001000)
#define P_IRCTRL_IR_DEC_LDR_IDLE                   ((volatile uint32_t *)0xfe001004)
#define P_IRCTRL_IR_DEC_LDR_REPEAT                 ((volatile uint32_t *)0xfe001008)
#define P_IRCTRL_IR_DEC_BIT_0                      ((volatile uint32_t *)0xfe00100c)
#define P_IRCTRL_IR_DEC_REG0                       ((volatile uint32_t *)0xfe001010)
#define P_IRCTRL_IR_DEC_FRAME                      ((volatile uint32_t *)0xfe001014)
#define P_IRCTRL_IR_DEC_STATUS                     ((volatile uint32_t *)0xfe001018)
#define P_IRCTRL_IR_DEC_REG1                       ((volatile uint32_t *)0xfe00101c)
#define P_IRCTRL_MF_IR_DEC_LDR_ACTIVE              ((volatile uint32_t *)0xfe001040)
#define P_IRCTRL_MF_IR_DEC_LDR_IDLE                ((volatile uint32_t *)0xfe001044)
#define P_IRCTRL_MF_IR_DEC_LDR_REPEAT              ((volatile uint32_t *)0xfe001048)
#define P_IRCTRL_MF_IR_DEC_BIT_0                   ((volatile uint32_t *)0xfe00104c)
#define P_IRCTRL_MF_IR_DEC_REG0                    ((volatile uint32_t *)0xfe001050)
#define P_IRCTRL_MF_IR_DEC_FRAME                   ((volatile uint32_t *)0xfe001054)
#define P_IRCTRL_MF_IR_DEC_STATUS                  ((volatile uint32_t *)0xfe001058)
#define P_IRCTRL_MF_IR_DEC_REG1                    ((volatile uint32_t *)0xfe00105c)
#define P_IRCTRL_MF_IR_DEC_REG2                    ((volatile uint32_t *)0xfe001060)
#define P_IRCTRL_MF_IR_DEC_DURATN2                 ((volatile uint32_t *)0xfe001064)
#define P_IRCTRL_MF_IR_DEC_DURATN3                 ((volatile uint32_t *)0xfe001068)
#define P_IRCTRL_MF_IR_DEC_FRAME1                  ((volatile uint32_t *)0xfe00106c)
#define P_IRCTRL_MF_IR_DEC_STATUS1                 ((volatile uint32_t *)0xfe001070)
#define P_IRCTRL_MF_IR_DEC_STATUS2                 ((volatile uint32_t *)0xfe001074)
#define P_IRCTRL_MF_IR_DEC_REG3                    ((volatile uint32_t *)0xfe001078)
#define P_IRCTRL_MF_IR_DEC_FRAME_RSV0              ((volatile uint32_t *)0xfe00107c)
#define P_IRCTRL_MF_IR_DEC_FRAME_RSV1              ((volatile uint32_t *)0xfe001080)
#define P_IRCTRL_MF_IR_DEC_FILTE                   ((volatile uint32_t *)0xfe001084)
#define P_IRCTRL_MF_IR_DEC_IRQ_CTL                 ((volatile uint32_t *)0xfe001088)
#define P_IRCTRL_MF_IR_DEC_FIFO_CTL                ((volatile uint32_t *)0xfe00108c)
#define P_IRCTRL_MF_IR_DEC_WIDTH_NEW               ((volatile uint32_t *)0xfe001090)
#define P_IRCTRL_MF_IR_DEC_REPEAT_DET              ((volatile uint32_t *)0xfe001094)
#define P_IRCTRL_IR_DEC_DEMOD_CNTL0                ((volatile uint32_t *)0xfe0010c0)
#define P_IRCTRL_IR_DEC_DEMOD_CNTL1                ((volatile uint32_t *)0xfe0010c4)
#define P_IRCTRL_IR_DEC_DEMOD_IIR_THD              ((volatile uint32_t *)0xfe0010c8)
#define P_IRCTRL_IR_DEC_DEMOD_THD0                 ((volatile uint32_t *)0xfe0010cc)
#define P_IRCTRL_IR_DEC_DEMOD_THD1                 ((volatile uint32_t *)0xfe0010d0)
#define P_IRCTRL_IR_DEC_DEMOD_SUM_CNT0             ((volatile uint32_t *)0xfe0010d4)
#define P_IRCTRL_IR_DEC_DEMOD_SUM_CNT1             ((volatile uint32_t *)0xfe0010d8)
#define P_IRCTRL_IR_DEC_DEMOD_CNT0                 ((volatile uint32_t *)0xfe0010dc)
#define P_IRCTRL_IR_DEC_DEMOD_CNT1                 ((volatile uint32_t *)0xfe0010e0)
#define P_IRCTRL_IR_BLASTER_ADDR0                  ((volatile uint32_t *)0xfe00110c)
#define P_IRCTRL_IR_BLASTER_ADDR1                  ((volatile uint32_t *)0xfe001110)
#define P_IRCTRL_IR_BLASTER_ADDR2                  ((volatile uint32_t *)0xfe001114)
#define P_IRCTRL_IR_BLASTER_ADDR3                  ((volatile uint32_t *)0xfe001118)
//========================================================================
//  APB0_I2C_M - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe001400
// -----------------------------------------------
#define P_I2C_M_A_CONTROL_REG                      ((volatile uint32_t *)0xfe001400)
#define P_I2C_M_A_SLAVE_ADDR                       ((volatile uint32_t *)0xfe001404)
#define P_I2C_M_A_TOKEN_LIST0                      ((volatile uint32_t *)0xfe001408)
#define P_I2C_M_A_TOKEN_LIST1                      ((volatile uint32_t *)0xfe00140c)
#define P_I2C_M_A_WDATA_REG0                       ((volatile uint32_t *)0xfe001410)
#define P_I2C_M_A_WDATA_REG1                       ((volatile uint32_t *)0xfe001414)
#define P_I2C_M_A_RDATA_REG0                       ((volatile uint32_t *)0xfe001418)
#define P_I2C_M_A_RDATA_REG1                       ((volatile uint32_t *)0xfe00141c)
#define P_I2C_M_A_TIMEOUT_TH                       ((volatile uint32_t *)0xfe001420)
//========================================================================
//  APB0_I2C_S - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe001800
// -----------------------------------------------
#define P_I2C_S_A_CONTROL_REG                      ((volatile uint32_t *)0xfe001800)
#define P_I2C_S_A_SEND_REG                         ((volatile uint32_t *)0xfe001804)
#define P_I2C_S_A_RECV_REG                         ((volatile uint32_t *)0xfe001808)
#define P_I2C_S_A_CNTL1_REG                        ((volatile uint32_t *)0xfe00180c)
//========================================================================
//  APB0_UART_A - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe001c00
// -----------------------------------------------
#define P_UART_A_WFIFO                             ((volatile uint32_t *)0xfe001c00)
#define P_UART_A_RFIFO                             ((volatile uint32_t *)0xfe001c04)
#define P_UART_A_CONTROL                           ((volatile uint32_t *)0xfe001c08)
#define P_UART_A_STATUS                            ((volatile uint32_t *)0xfe001c0c)
#define P_UART_A_MISC                              ((volatile uint32_t *)0xfe001c10)
#define P_UART_A_REG5                              ((volatile uint32_t *)0xfe001c14)
//========================================================================
//  APB0_UART_B - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe002000
// -----------------------------------------------
#define P_UART_B_WFIFO                             ((volatile uint32_t *)0xfe002000)
#define P_UART_B_RFIFO                             ((volatile uint32_t *)0xfe002004)
#define P_UART_B_CONTROL                           ((volatile uint32_t *)0xfe002008)
#define P_UART_B_STATUS                            ((volatile uint32_t *)0xfe00200c)
#define P_UART_B_MISC                              ((volatile uint32_t *)0xfe002010)
#define P_UART_B_REG5                              ((volatile uint32_t *)0xfe002014)
//========================================================================
//  APB0_PWM_AB - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe002400
// -----------------------------------------------
#define P_PWMAB_PWM_A                              ((volatile uint32_t *)0xfe002400)
#define P_PWMAB_PWM_B                              ((volatile uint32_t *)0xfe002404)
#define P_PWMAB_MISC_REG_AB                        ((volatile uint32_t *)0xfe002408)
#define P_PWMAB_DELTA_SIGMA_AB                     ((volatile uint32_t *)0xfe00240c)
#define P_PWMAB_TIME_AB                            ((volatile uint32_t *)0xfe002410)
#define P_PWMAB_A2                                 ((volatile uint32_t *)0xfe002414)
#define P_PWMAB_B2                                 ((volatile uint32_t *)0xfe002418)
#define P_PWMAB_BLINK_AB                           ((volatile uint32_t *)0xfe00241c)
#define P_PWMAB_LOCK_AB                            ((volatile uint32_t *)0xfe002420)
//========================================================================
//  APB0_PWM_CD - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe002800
// -----------------------------------------------
#define P_PWMCD_PWM_C                              ((volatile uint32_t *)0xfe002800)
#define P_PWMCD_PWM_D                              ((volatile uint32_t *)0xfe002804)
#define P_PWMCD_MISC_REG_CD                        ((volatile uint32_t *)0xfe002808)
#define P_PWMCD_DELTA_SIGMA_CD                     ((volatile uint32_t *)0xfe00280c)
#define P_PWMCD_TIME_CD                            ((volatile uint32_t *)0xfe002810)
#define P_PWMCD_C2                                 ((volatile uint32_t *)0xfe002814)
#define P_PWMCD_D2                                 ((volatile uint32_t *)0xfe002818)
#define P_PWMCD_BLINK_CD                           ((volatile uint32_t *)0xfe00281c)
#define P_PWMCD_LOCK_CD                            ((volatile uint32_t *)0xfe002820)
//========================================================================
//  APB0_SARADC - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe002c00
// -----------------------------------------------
#define P_SAR_ADC_REG0                             ((volatile uint32_t *)0xfe002c00)
#define P_SAR_ADC_CHAN_LIST                        ((volatile uint32_t *)0xfe002c04)
#define P_SAR_ADC_AVG_CNTL                         ((volatile uint32_t *)0xfe002c08)
#define P_SAR_ADC_REG3                             ((volatile uint32_t *)0xfe002c0c)
#define P_SAR_ADC_DELAY                            ((volatile uint32_t *)0xfe002c10)
#define P_SAR_ADC_LAST_RD                          ((volatile uint32_t *)0xfe002c14)
#define P_SAR_ADC_FIFO_RD                          ((volatile uint32_t *)0xfe002c18)
#define P_SAR_ADC_AUX_SW                           ((volatile uint32_t *)0xfe002c1c)
#define P_SAR_ADC_CHAN_10_SW                       ((volatile uint32_t *)0xfe002c20)
#define P_SAR_ADC_DETECT_IDLE_SW                   ((volatile uint32_t *)0xfe002c24)
#define P_SAR_ADC_DELTA_10                         ((volatile uint32_t *)0xfe002c28)
#define P_SAR_ADC_REG11                            ((volatile uint32_t *)0xfe002c2c)
#define P_SAR_ADC_REG12                            ((volatile uint32_t *)0xfe002c30)
#define P_SAR_ADC_REG13                            ((volatile uint32_t *)0xfe002c34)
#define P_SAR_ADC_CHNL01                           ((volatile uint32_t *)0xfe002c38)
#define P_SAR_ADC_CHNL23                           ((volatile uint32_t *)0xfe002c3c)
#define P_SAR_ADC_CHNL45                           ((volatile uint32_t *)0xfe002c40)
#define P_SAR_ADC_CHNL67                           ((volatile uint32_t *)0xfe002c44)
//========================================================================
//  JTAGCTRL_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe003000
// -----------------------------------------------
#define P_JTAGCTRL_CTRL0                           ((volatile uint32_t *)0xfe003000)
#define P_JTAGCTRL_PWD_CTRL0                       ((volatile uint32_t *)0xfe003004)
#define P_JTAGCTRL_PWD_ADDR0                       ((volatile uint32_t *)0xfe003008)
#define P_JTAGCTRL_PWD_ADDR1                       ((volatile uint32_t *)0xfe00300c)
#define P_JTAGCTRL_PWD_ADDR2                       ((volatile uint32_t *)0xfe003010)
#define P_JTAGCTRL_PWD_ADDR3                       ((volatile uint32_t *)0xfe003014)
#define P_JTAGCTRL_PWD_VALUE0                      ((volatile uint32_t *)0xfe003018)
#define P_JTAGCTRL_PWD_VALUE1                      ((volatile uint32_t *)0xfe00301c)
#define P_JTAGCTRL_PWD_VALUE2                      ((volatile uint32_t *)0xfe003020)
#define P_JTAGCTRL_PWD_VALUE3                      ((volatile uint32_t *)0xfe003024)
#define P_JTAGCTRL_PWD_STS                         ((volatile uint32_t *)0xfe003028)
//========================================================================
//  APB0_MSR_CLK - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe003400
// -----------------------------------------------
#define P_MSR_CLK_REG0                             ((volatile uint32_t *)0xfe003400)
#define P_MSR_CLK_REG1                             ((volatile uint32_t *)0xfe003404)
#define P_MSR_CLK_REG2                             ((volatile uint32_t *)0xfe003408)
#define P_MSR_CLK_REG3                             ((volatile uint32_t *)0xfe00340c)
#define P_MSR_CLK_REG4                             ((volatile uint32_t *)0xfe003410)
#define P_MSR_CLK_REG5                             ((volatile uint32_t *)0xfe003414)
#define P_MSR_CLK_DUTY                             ((volatile uint32_t *)0xfe003418)
//========================================================================
//  APB0_SPICCA_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe003800
// -----------------------------------------------
#define P_SPICC_A_RXDATA                           ((volatile uint32_t *)0xfe003800)
#define P_SPICC_A_TXDATA                           ((volatile uint32_t *)0xfe003804)
#define P_SPICC_A_CONREG                           ((volatile uint32_t *)0xfe003808)
#define P_SPICC_A_INTREG                           ((volatile uint32_t *)0xfe00380c)
#define P_SPICC_A_DMAREG                           ((volatile uint32_t *)0xfe003810)
#define P_SPICC_A_STATREG                          ((volatile uint32_t *)0xfe003814)
#define P_SPICC_A_PERIODREG                        ((volatile uint32_t *)0xfe003818)
#define P_SPICC_A_TESTREG                          ((volatile uint32_t *)0xfe00381c)
#define P_SPICC_A_DRADDR                           ((volatile uint32_t *)0xfe003820)
#define P_SPICC_A_DWADDR                           ((volatile uint32_t *)0xfe003824)
#define P_SPICC_A_LD_CNTL0                         ((volatile uint32_t *)0xfe003828)
#define P_SPICC_A_LD_CNTL1                         ((volatile uint32_t *)0xfe00382c)
#define P_SPICC_A_LD_RADDR                         ((volatile uint32_t *)0xfe003830)
#define P_SPICC_A_LD_WADDR                         ((volatile uint32_t *)0xfe003834)
#define P_SPICC_A_ENHANCE_CNTL                     ((volatile uint32_t *)0xfe003838)
#define P_SPICC_A_ENHANCE_CNTL1                    ((volatile uint32_t *)0xfe00383c)
#define P_SPICC_A_ENHANCE_CNTL2                    ((volatile uint32_t *)0xfe003840)
//========================================================================
//  APB0_SPICCB_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe003c00
// -----------------------------------------------
#define P_SPICC_B_RXDATA                           ((volatile uint32_t *)0xfe003c00)
#define P_SPICC_B_TXDATA                           ((volatile uint32_t *)0xfe003c04)
#define P_SPICC_B_CONREG                           ((volatile uint32_t *)0xfe003c08)
#define P_SPICC_B_INTREG                           ((volatile uint32_t *)0xfe003c0c)
#define P_SPICC_B_DMAREG                           ((volatile uint32_t *)0xfe003c10)
#define P_SPICC_B_STATREG                          ((volatile uint32_t *)0xfe003c14)
#define P_SPICC_B_PERIODREG                        ((volatile uint32_t *)0xfe003c18)
#define P_SPICC_B_TESTREG                          ((volatile uint32_t *)0xfe003c1c)
#define P_SPICC_B_DRADDR                           ((volatile uint32_t *)0xfe003c20)
#define P_SPICC_B_DWADDR                           ((volatile uint32_t *)0xfe003c24)
#define P_SPICC_B_LD_CNTL0                         ((volatile uint32_t *)0xfe003c28)
#define P_SPICC_B_LD_CNTL1                         ((volatile uint32_t *)0xfe003c2c)
#define P_SPICC_B_LD_RADDR                         ((volatile uint32_t *)0xfe003c30)
#define P_SPICC_B_LD_WADDR                         ((volatile uint32_t *)0xfe003c34)
#define P_SPICC_B_ENHANCE_CNTL                     ((volatile uint32_t *)0xfe003c38)
#define P_SPICC_B_ENHANCE_CNTL1                    ((volatile uint32_t *)0xfe003c3c)
#define P_SPICC_B_ENHANCE_CNTL2                    ((volatile uint32_t *)0xfe003c40)
//========================================================================
//  USBCTRL    - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe004000
// -----------------------------------------------
#define P_USB_CTRL0                                ((volatile uint32_t *)0xfe004000)
#define P_USB_CTRL1                                ((volatile uint32_t *)0xfe004004)
#define P_USB_CTRL2                                ((volatile uint32_t *)0xfe004008)
#define P_USB_CTRL3                                ((volatile uint32_t *)0xfe00400c)
#define P_USB_CTRL4                                ((volatile uint32_t *)0xfe004010)
#define P_USB_CTRL5                                ((volatile uint32_t *)0xfe004014)
#define P_USB_CTRL6                                ((volatile uint32_t *)0xfe004018)
#define P_USB_CTRL7                                ((volatile uint32_t *)0xfe00401c)
#define P_USB_CTRL8                                ((volatile uint32_t *)0xfe004020)
#define P_USB_CTRL9                                ((volatile uint32_t *)0xfe004024)
#define P_USB_CTRL10                               ((volatile uint32_t *)0xfe004028)
#define P_USB_CTRL11                               ((volatile uint32_t *)0xfe00402c)
#define P_USB_CTRL12                               ((volatile uint32_t *)0xfe004030)
#define P_USB_CTRL13                               ((volatile uint32_t *)0xfe004034)
#define P_USB_CTRL14                               ((volatile uint32_t *)0xfe004038)
#define P_USB_CTRL15                               ((volatile uint32_t *)0xfe00403c)
#define P_USB_CTRL16                               ((volatile uint32_t *)0xfe004040)
#define P_USB_CTRL17                               ((volatile uint32_t *)0xfe004044)
#define P_USB_CTRL18                               ((volatile uint32_t *)0xfe004048)
#define P_USB_CTRL19                               ((volatile uint32_t *)0xfe00404c)
#define P_USB_CTRL20                               ((volatile uint32_t *)0xfe004050)
#define P_USB_CTRL21                               ((volatile uint32_t *)0xfe004054)
#define P_USB_CTRL22                               ((volatile uint32_t *)0xfe004058)
#define P_USB_CTRL23                               ((volatile uint32_t *)0xfe00405c)
#define P_USB_CTRL24                               ((volatile uint32_t *)0xfe004060)
#define P_USB_CTRL25                               ((volatile uint32_t *)0xfe004064)
#define P_USB_CTRL26                               ((volatile uint32_t *)0xfe004068)
#define P_USB_CTRL27                               ((volatile uint32_t *)0xfe00406c)
#define P_USB_CTRL28                               ((volatile uint32_t *)0xfe004070)
#define P_USB_CTRL29                               ((volatile uint32_t *)0xfe004074)
#define P_USB_CTRL30                               ((volatile uint32_t *)0xfe004078)
#define P_USB_CTRL31                               ((volatile uint32_t *)0xfe00407c)
//========================================================================
//  APB0_ACODEC_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe004800
// -----------------------------------------------
#define P_ACODEC_0                                 ((volatile uint32_t *)0xfe004800)
#define P_ACODEC_1                                 ((volatile uint32_t *)0xfe004804)
#define P_ACODEC_2                                 ((volatile uint32_t *)0xfe004808)
#define P_ACODEC_3                                 ((volatile uint32_t *)0xfe00480c)
#define P_ACODEC_4                                 ((volatile uint32_t *)0xfe004810)
#define P_ACODEC_5                                 ((volatile uint32_t *)0xfe004814)
#define P_ACODEC_6                                 ((volatile uint32_t *)0xfe004818)
#define P_ACODEC_7                                 ((volatile uint32_t *)0xfe00481c)
#define P_ACODEC_8                                 ((volatile uint32_t *)0xfe004820)
#define P_ACODEC_9                                 ((volatile uint32_t *)0xfe004824)
#define P_ACODEC_STS0                              ((volatile uint32_t *)0xfe004840)
//========================================================================
//  APB0_TEMP_SENSOR - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe004c00
// -----------------------------------------------
#define P_TS_CFG_REG1                              ((volatile uint32_t *)0xfe004c04)
#define P_TS_CFG_REG2                              ((volatile uint32_t *)0xfe004c08)
#define P_TS_CFG_REG3                              ((volatile uint32_t *)0xfe004c0c)
#define P_TS_CFG_REG4                              ((volatile uint32_t *)0xfe004c10)
#define P_TS_CFG_REG5                              ((volatile uint32_t *)0xfe004c14)
#define P_TS_CFG_REG6                              ((volatile uint32_t *)0xfe004c18)
#define P_TS_CFG_REG7                              ((volatile uint32_t *)0xfe004c1c)
#define P_TS_STAT0                                 ((volatile uint32_t *)0xfe004c40)
#define P_TS_STAT1                                 ((volatile uint32_t *)0xfe004c44)
#define P_TS_STAT2                                 ((volatile uint32_t *)0xfe004c48)
#define P_TS_STAT3                                 ((volatile uint32_t *)0xfe004c4c)
#define P_TS_STAT4                                 ((volatile uint32_t *)0xfe004c50)
#define P_TS_STAT5                                 ((volatile uint32_t *)0xfe004c54)
#define P_TS_STAT6                                 ((volatile uint32_t *)0xfe004c58)
#define P_TS_STAT7                                 ((volatile uint32_t *)0xfe004c5c)
#define P_TS_STAT8                                 ((volatile uint32_t *)0xfe004c60)
#define P_TS_STAT9                                 ((volatile uint32_t *)0xfe004c64)
//========================================================================
//  APB0_KL_EFUSE_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe005000
// -----------------------------------------------
//
// Reading file:  ../crypto/crypto_reg.h
//
#define P_OTP_TEE_RDY                              ((volatile uint32_t *)0xfe005000)
#define P_OTP_TEE_CHK                              ((volatile uint32_t *)0xfe005004)
#define P_OTP_TEE_CFG                              ((volatile uint32_t *)0xfe005008)
#define P_OTP_TEE_WR_DAT0                          ((volatile uint32_t *)0xfe005010)
#define P_OTP_TEE_WR_DAT1                          ((volatile uint32_t *)0xfe005014)
#define P_OTP_TEE_WR_DAT2                          ((volatile uint32_t *)0xfe005018)
#define P_OTP_TEE_WR_DAT3                          ((volatile uint32_t *)0xfe00501c)
#define P_OTP_TEE_RD_DAT0                          ((volatile uint32_t *)0xfe005020)
#define P_OTP_TEE_RD_DAT1                          ((volatile uint32_t *)0xfe005024)
#define P_OTP_TEE_RD_DAT2                          ((volatile uint32_t *)0xfe005028)
#define P_OTP_TEE_RD_DAT3                          ((volatile uint32_t *)0xfe00502c)
#define P_OTP_RMA_PW0                              ((volatile uint32_t *)0xfe005030)
#define P_OTP_RMA_PW1                              ((volatile uint32_t *)0xfe005034)
#define P_OTP_RMA_PW2                              ((volatile uint32_t *)0xfe005038)
#define P_OTP_RMA_PW3                              ((volatile uint32_t *)0xfe00503c)
#define P_OTP_REE_RDY                              ((volatile uint32_t *)0xfe005040)
#define P_OTP_REE_CHK                              ((volatile uint32_t *)0xfe005044)
#define P_OTP_REE_CFG                              ((volatile uint32_t *)0xfe005048)
#define P_OTP_REE_RD_DAT0                          ((volatile uint32_t *)0xfe005050)
#define P_OTP_REE_RD_DAT1                          ((volatile uint32_t *)0xfe005054)
#define P_OTP_REE_RD_DAT2                          ((volatile uint32_t *)0xfe005058)
#define P_OTP_REE_RD_DAT3                          ((volatile uint32_t *)0xfe00505c)
#define P_OTP_LIC0                                 ((volatile uint32_t *)0xfe005060)
#define P_OTP_LIC1                                 ((volatile uint32_t *)0xfe005064)
#define P_OTP_LIC2                                 ((volatile uint32_t *)0xfe005068)
#define P_OTP_LIC3                                 ((volatile uint32_t *)0xfe00506c)
#define P_RNG_SEC_CONFIG_REG1                      ((volatile uint32_t *)0xfe005104)
#define P_RNG_SEC_CONFIG_REG2                      ((volatile uint32_t *)0xfe005108)
#define P_RNG_SEC_DATA                             ((volatile uint32_t *)0xfe005110)
#define P_RNG_SEC_STS                              ((volatile uint32_t *)0xfe005114)
#define P_RNG_USR_DATA                             ((volatile uint32_t *)0xfe005118)
#define P_RNG_USR_STS                              ((volatile uint32_t *)0xfe00511c)
#define P_KTE_SLOT0                                ((volatile uint32_t *)0xfe005180)
#define P_KTE_SLOT1                                ((volatile uint32_t *)0xfe005184)
#define P_KTE_SLOT2                                ((volatile uint32_t *)0xfe005188)
#define P_KTE_SLOT3                                ((volatile uint32_t *)0xfe00518c)
#define P_KTE_SLOT4                                ((volatile uint32_t *)0xfe005190)
#define P_KTE_SLOT5                                ((volatile uint32_t *)0xfe005194)
#define P_KTE_SLOT6                                ((volatile uint32_t *)0xfe005198)
#define P_KTE_SLOT7                                ((volatile uint32_t *)0xfe00519c)
#define P_MKL_TEE_RDY                              ((volatile uint32_t *)0xfe005200)
#define P_MKL_TEE_CHK                              ((volatile uint32_t *)0xfe005204)
#define P_MKL_TEE_CFG                              ((volatile uint32_t *)0xfe005208)
#define P_MKL_TEE_EK                               ((volatile uint32_t *)0xfe005210)
#define P_MKL_REE_RDY                              ((volatile uint32_t *)0xfe005280)
#define P_MKL_REE_CHK                              ((volatile uint32_t *)0xfe005284)
#define P_MKL_REE_CFG                              ((volatile uint32_t *)0xfe005288)
#define P_MKL_REE_EK                               ((volatile uint32_t *)0xfe005290)
//
// Closing file:  ../crypto/crypto_reg.h
//
//========================================================================
//  APB0_PWM_EF - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe005400
// -----------------------------------------------
#define P_PWMEF_PWM_E                              ((volatile uint32_t *)0xfe005400)
#define P_PWMEF_PWM_F                              ((volatile uint32_t *)0xfe005404)
#define P_PWMEF_MISC_REG_EF                        ((volatile uint32_t *)0xfe005408)
#define P_PWMEF_DELTA_SIGMA_EF                     ((volatile uint32_t *)0xfe00540c)
#define P_PWMEF_TIME_EF                            ((volatile uint32_t *)0xfe005410)
#define P_PWMEF_E2                                 ((volatile uint32_t *)0xfe005414)
#define P_PWMEF_F2                                 ((volatile uint32_t *)0xfe005418)
#define P_PWMEF_BLINK_EF                           ((volatile uint32_t *)0xfe00541c)
#define P_PWMEF_LOCK_EF                            ((volatile uint32_t *)0xfe005420)
//========================================================================
//  APB0_SYS_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe005800
// -----------------------------------------------
#define P_SYSCTRL_METAL_REV0                       ((volatile uint32_t *)0xfe005800)
#define P_SYSCTRL_METAL_REV1                       ((volatile uint32_t *)0xfe005804)
//`define SYSCTRL_PROD_ENABLE                     8'h0a
#define P_SYSCTRL_CHIP_ID                          ((volatile uint32_t *)0xfe00582c)
#define P_SYSCTRL_WRITEONCE1_REG0                  ((volatile uint32_t *)0xfe005840)
#define P_SYSCTRL_WRITEONCE1_STICKY_REG0           ((volatile uint32_t *)0xfe005844)
#define P_SYSCTRL_WRITEONCE1_SEC_REG0              ((volatile uint32_t *)0xfe005848)
#define P_SYSCTRL_WRITEONCE1_STICKY_SEC_REG0       ((volatile uint32_t *)0xfe00584c)
#define P_SYSCTRL_WRITEONCE_REG0                   ((volatile uint32_t *)0xfe005850)
#define P_SYSCTRL_WRITEONCE_REG1                   ((volatile uint32_t *)0xfe005854)
#define P_SYSCTRL_WRITEONCE_SEC_REG0               ((volatile uint32_t *)0xfe005858)
#define P_SYSCTRL_WRITEONCE_STICKY_REG0            ((volatile uint32_t *)0xfe00585c)
#define P_SYSCTRL_WRITEONCE_STICKY_REG1            ((volatile uint32_t *)0xfe005860)
#define P_SYSCTRL_WRITEONCE_STICKY_SEC_REG0        ((volatile uint32_t *)0xfe005864)
#define P_SYSCTRL_TIMERA_CTRL                      ((volatile uint32_t *)0xfe0058c0)
#define P_SYSCTRL_TIMERA                           ((volatile uint32_t *)0xfe0058c4)
#define P_SYSCTRL_TIMERB_CTRL                      ((volatile uint32_t *)0xfe0058c8)
#define P_SYSCTRL_TIMERB                           ((volatile uint32_t *)0xfe0058cc)
#define P_SYSCTRL_TIMERC_CTRL                      ((volatile uint32_t *)0xfe0058d0)
#define P_SYSCTRL_TIMERC                           ((volatile uint32_t *)0xfe0058d4)
#define P_SYSCTRL_TIMERD_CTRL                      ((volatile uint32_t *)0xfe0058d8)
#define P_SYSCTRL_TIMERD                           ((volatile uint32_t *)0xfe0058dc)
#define P_SYSCTRL_SEC_TIMERA_CTRL                  ((volatile uint32_t *)0xfe0058e0)
#define P_SYSCTRL_SEC_TIMERA                       ((volatile uint32_t *)0xfe0058e4)
#define P_SYSCTRL_TIMERE_CTRL                      ((volatile uint32_t *)0xfe005900)
#define P_SYSCTRL_TIMERE                           ((volatile uint32_t *)0xfe005904)
#define P_SYSCTRL_TIMERE_HI                        ((volatile uint32_t *)0xfe005908)
#define P_SYSCTRL_TIMERF_CTRL                      ((volatile uint32_t *)0xfe00590c)
#define P_SYSCTRL_TIMERF                           ((volatile uint32_t *)0xfe005910)
#define P_SYSCTRL_TIMERF_HI                        ((volatile uint32_t *)0xfe005914)
#define P_SYSCTRL_SEC_TIMERE_CTRL                  ((volatile uint32_t *)0xfe005918)
#define P_SYSCTRL_SEC_TIMERE                       ((volatile uint32_t *)0xfe00591c)
#define P_SYSCTRL_SEC_TIMERE_HI                    ((volatile uint32_t *)0xfe005920)
#define P_SYSCTRL_OSC_RING_CTRL0                   ((volatile uint32_t *)0xfe005940)
#define P_SYSCTRL_SEC_DDR_AXI_SEL                  ((volatile uint32_t *)0xfe005944)
#define P_SYSCTRL_AM2AXI_CTRL0                     ((volatile uint32_t *)0xfe005948)
#define P_SYSCTRL_AM2AXI_STS                       ((volatile uint32_t *)0xfe00594c)
#define P_SYSCTRL_POC                              ((volatile uint32_t *)0xfe005980)
//`define SYSCTRL_PROD_PWD_VALUE0                 8'h80
//`define SYSCTRL_PROD_PWD_VALUE1                 8'h81
//`define SYSCTRL_PROD_PWD_VALUE2                 8'h82
//`define SYSCTRL_PROD_PWD_VALUE3                 8'h83
//`define SYSCTRL_PROD_STATUS                     8'h84
#define P_SYSCTRL_DEBUG_REG0                       ((volatile uint32_t *)0xfe005a40)
#define P_SYSCTRL_DEBUG_REG1                       ((volatile uint32_t *)0xfe005a44)
#define P_SYSCTRL_DEBUG_REG2                       ((volatile uint32_t *)0xfe005a48)
#define P_SYSCTRL_DEBUG_REG3                       ((volatile uint32_t *)0xfe005a4c)
#define P_SYSCTRL_DEBUG_REG4                       ((volatile uint32_t *)0xfe005a50)
#define P_SYSCTRL_DEBUG_REG5                       ((volatile uint32_t *)0xfe005a54)
#define P_SYSCTRL_DEBUG_REG6                       ((volatile uint32_t *)0xfe005a58)
#define P_SYSCTRL_DEBUG_REG7                       ((volatile uint32_t *)0xfe005a5c)
#define P_SYSCTRL_STATUS_REG0                      ((volatile uint32_t *)0xfe005a80)
#define P_SYSCTRL_STATUS_REG1                      ((volatile uint32_t *)0xfe005a84)
#define P_SYSCTRL_STATUS_REG2                      ((volatile uint32_t *)0xfe005a88)
#define P_SYSCTRL_STATUS_REG3                      ((volatile uint32_t *)0xfe005a8c)
#define P_SYSCTRL_STATUS_REG4                      ((volatile uint32_t *)0xfe005a90)
#define P_SYSCTRL_STATUS_REG5                      ((volatile uint32_t *)0xfe005a94)
#define P_SYSCTRL_STATUS_REG6                      ((volatile uint32_t *)0xfe005a98)
#define P_SYSCTRL_STATUS_REG7                      ((volatile uint32_t *)0xfe005a9c)
#define P_SYSCTRL_STICKY_REG0                      ((volatile uint32_t *)0xfe005ac0)
#define P_SYSCTRL_STICKY_REG1                      ((volatile uint32_t *)0xfe005ac4)
#define P_SYSCTRL_STICKY_REG2                      ((volatile uint32_t *)0xfe005ac8)
#define P_SYSCTRL_STICKY_REG3                      ((volatile uint32_t *)0xfe005acc)
#define P_SYSCTRL_STICKY_REG4                      ((volatile uint32_t *)0xfe005ad0)
#define P_SYSCTRL_STICKY_REG5                      ((volatile uint32_t *)0xfe005ad4)
#define P_SYSCTRL_STICKY_REG6                      ((volatile uint32_t *)0xfe005ad8)
#define P_SYSCTRL_STICKY_REG7                      ((volatile uint32_t *)0xfe005adc)
#define P_SYSCTRL_SEC_STATUS_REG0                  ((volatile uint32_t *)0xfe005b00)
#define P_SYSCTRL_SEC_STATUS_REG1                  ((volatile uint32_t *)0xfe005b04)
#define P_SYSCTRL_SEC_STATUS_REG2                  ((volatile uint32_t *)0xfe005b08)
#define P_SYSCTRL_SEC_STATUS_REG3                  ((volatile uint32_t *)0xfe005b0c)
#define P_SYSCTRL_SEC_STATUS_REG4                  ((volatile uint32_t *)0xfe005b10)
#define P_SYSCTRL_SEC_STATUS_REG5                  ((volatile uint32_t *)0xfe005b14)
#define P_SYSCTRL_SEC_STATUS_REG6                  ((volatile uint32_t *)0xfe005b18)
#define P_SYSCTRL_SEC_STATUS_REG7                  ((volatile uint32_t *)0xfe005b1c)
#define P_SYSCTRL_SEC_STATUS_REG8                  ((volatile uint32_t *)0xfe005b20)
#define P_SYSCTRL_SEC_STATUS_REG9                  ((volatile uint32_t *)0xfe005b24)
#define P_SYSCTRL_SEC_STATUS_REG10                 ((volatile uint32_t *)0xfe005b28)
#define P_SYSCTRL_SEC_STATUS_REG11                 ((volatile uint32_t *)0xfe005b2c)
#define P_SYSCTRL_SEC_STATUS_REG12                 ((volatile uint32_t *)0xfe005b30)
#define P_SYSCTRL_SEC_STATUS_REG13                 ((volatile uint32_t *)0xfe005b34)
#define P_SYSCTRL_SEC_STATUS_REG14                 ((volatile uint32_t *)0xfe005b38)
#define P_SYSCTRL_SEC_STATUS_REG15                 ((volatile uint32_t *)0xfe005b3c)
#define P_SYSCTRL_SEC_STATUS_REG16                 ((volatile uint32_t *)0xfe005b40)
#define P_SYSCTRL_SEC_STATUS_REG17                 ((volatile uint32_t *)0xfe005b44)
#define P_SYSCTRL_SEC_STATUS_REG18                 ((volatile uint32_t *)0xfe005b48)
#define P_SYSCTRL_SEC_STATUS_REG19                 ((volatile uint32_t *)0xfe005b4c)
#define P_SYSCTRL_SEC_STATUS_REG20                 ((volatile uint32_t *)0xfe005b50)
#define P_SYSCTRL_SEC_STATUS_REG21                 ((volatile uint32_t *)0xfe005b54)
#define P_SYSCTRL_SEC_STATUS_REG22                 ((volatile uint32_t *)0xfe005b58)
#define P_SYSCTRL_SEC_STATUS_REG23                 ((volatile uint32_t *)0xfe005b5c)
#define P_SYSCTRL_SEC_STICKY_REG0                  ((volatile uint32_t *)0xfe005b80)
#define P_SYSCTRL_SEC_STICKY_REG1                  ((volatile uint32_t *)0xfe005b84)
#define P_SYSCTRL_SEC_STICKY_REG2                  ((volatile uint32_t *)0xfe005b88)
#define P_SYSCTRL_SEC_STICKY_REG3                  ((volatile uint32_t *)0xfe005b8c)
#define P_SYSCTRL_SEC_STICKY_REG4                  ((volatile uint32_t *)0xfe005b90)
#define P_SYSCTRL_SEC_STICKY_REG5                  ((volatile uint32_t *)0xfe005b94)
#define P_SYSCTRL_SEC_STICKY_REG6                  ((volatile uint32_t *)0xfe005b98)
#define P_SYSCTRL_SEC_STICKY_REG7                  ((volatile uint32_t *)0xfe005b9c)
#define P_SYSCTRL_MSG_INDEX0_STICKY                ((volatile uint32_t *)0xfe005bc0)
#define P_SYSCTRL_MSG_INDEX1_STICKY                ((volatile uint32_t *)0xfe005bc4)
#define P_SYSCTRL_MSG_INDEX2_STICKY                ((volatile uint32_t *)0xfe005bc8)
#define P_SYSCTRL_MSG_INDEX3_STICKY                ((volatile uint32_t *)0xfe005bcc)
//========================================================================
//  APB0_I2C_M_B - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe005c00
// -----------------------------------------------
#define P_I2C_M_B_CONTROL_REG                      ((volatile uint32_t *)0xfe005c00)
#define P_I2C_M_B_SLAVE_ADDR                       ((volatile uint32_t *)0xfe005c04)
#define P_I2C_M_B_TOKEN_LIST0                      ((volatile uint32_t *)0xfe005c08)
#define P_I2C_M_B_TOKEN_LIST1                      ((volatile uint32_t *)0xfe005c0c)
#define P_I2C_M_B_WDATA_REG0                       ((volatile uint32_t *)0xfe005c10)
#define P_I2C_M_B_WDATA_REG1                       ((volatile uint32_t *)0xfe005c14)
#define P_I2C_M_B_RDATA_REG0                       ((volatile uint32_t *)0xfe005c18)
#define P_I2C_M_B_RDATA_REG1                       ((volatile uint32_t *)0xfe005c1c)
#define P_I2C_M_B_TIMEOUT_TH                       ((volatile uint32_t *)0xfe005c20)
//========================================================================
//  APB0_DMA - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe006000
// -----------------------------------------------
//TODO
//========================================================================
//  APB0_IRQ_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe006400
// -----------------------------------------------
#define P_IRQCTRL_IRQOUT_MASK0                     ((volatile uint32_t *)0xfe006400)
#define P_IRQCTRL_IRQOUT_MASK1                     ((volatile uint32_t *)0xfe006404)
#define P_IRQCTRL_IRQOUT_MASK2                     ((volatile uint32_t *)0xfe006408)
#define P_IRQCTRL_IRQOUT_MASK3                     ((volatile uint32_t *)0xfe00640c)
#define P_IRQCTRL_IRQOUT_INV0                      ((volatile uint32_t *)0xfe006440)
#define P_IRQCTRL_IRQOUT_INV1                      ((volatile uint32_t *)0xfe006444)
#define P_IRQCTRL_IRQOUT_INV2                      ((volatile uint32_t *)0xfe006448)
#define P_IRQCTRL_IRQOUT_INV3                      ((volatile uint32_t *)0xfe00644c)
//`define APB0_IRQIN_MASK0_CLK_USB       8'h00
//`define APB0_IRQIN_MASK0_CLK_CPU       8'h01
//`define APB0_IRQIN_MASK0_CLK_SYS       8'h02
//`define APB0_IRQIN_MASK1_CLK_SYS       8'h03
//`define APB0_IRQIN_MASK0_CLK_DSPA      8'h04
//`define APB0_IRQIN_MASK0_CLK_DSPB      8'h05
//`define APB0_IRQIN_CLRLVL_CLK_USB      8'h10
//`define APB0_IRQIN_CLRLVL_CLK_DSPA     8'h11
//`define APB0_IRQIN_CLRLVL_CLK_DSPB     8'h12
//========================================================================
//  APB0_I2C_M - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe006800
// -----------------------------------------------
#define P_I2C_M_C_CONTROL_REG                      ((volatile uint32_t *)0xfe006800)
#define P_I2C_M_C_SLAVE_ADDR                       ((volatile uint32_t *)0xfe006804)
#define P_I2C_M_C_TOKEN_LIST0                      ((volatile uint32_t *)0xfe006808)
#define P_I2C_M_C_TOKEN_LIST1                      ((volatile uint32_t *)0xfe00680c)
#define P_I2C_M_C_WDATA_REG0                       ((volatile uint32_t *)0xfe006810)
#define P_I2C_M_C_WDATA_REG1                       ((volatile uint32_t *)0xfe006814)
#define P_I2C_M_C_RDATA_REG0                       ((volatile uint32_t *)0xfe006818)
#define P_I2C_M_C_RDATA_REG1                       ((volatile uint32_t *)0xfe00681c)
#define P_I2C_M_C_TIMEOUT_TH                       ((volatile uint32_t *)0xfe006820)
//========================================================================
//  APB0_I2C_M - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe006c00
// -----------------------------------------------
#define P_I2C_M_D_CONTROL_REG                      ((volatile uint32_t *)0xfe006c00)
#define P_I2C_M_D_SLAVE_ADDR                       ((volatile uint32_t *)0xfe006c04)
#define P_I2C_M_D_TOKEN_LIST0                      ((volatile uint32_t *)0xfe006c08)
#define P_I2C_M_D_TOKEN_LIST1                      ((volatile uint32_t *)0xfe006c0c)
#define P_I2C_M_D_WDATA_REG0                       ((volatile uint32_t *)0xfe006c10)
#define P_I2C_M_D_WDATA_REG1                       ((volatile uint32_t *)0xfe006c14)
#define P_I2C_M_D_RDATA_REG0                       ((volatile uint32_t *)0xfe006c18)
#define P_I2C_M_D_RDATA_REG1                       ((volatile uint32_t *)0xfe006c1c)
#define P_I2C_M_D_TIMEOUT_TH                       ((volatile uint32_t *)0xfe006c20)
//========================================================================
//  APB0_UART_C - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe007000
// -----------------------------------------------
#define P_UART_C_WFIFO                             ((volatile uint32_t *)0xfe007000)
#define P_UART_C_RFIFO                             ((volatile uint32_t *)0xfe007004)
#define P_UART_C_CONTROL                           ((volatile uint32_t *)0xfe007008)
#define P_UART_C_STATUS                            ((volatile uint32_t *)0xfe00700c)
#define P_UART_C_MISC                              ((volatile uint32_t *)0xfe007010)
#define P_UART_C_REG5                              ((volatile uint32_t *)0xfe007014)
//========================================================================
//  APB0_PWR_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe007800
// -----------------------------------------------
#define P_PWRCTRL_PWR_ACK0                         ((volatile uint32_t *)0xfe007800)
#define P_PWRCTRL_ISO_EN0                          ((volatile uint32_t *)0xfe007804)
#define P_PWRCTRL_PWR_OFF0                         ((volatile uint32_t *)0xfe007808)
#define P_PWRCTRL_SAVE_EN0                         ((volatile uint32_t *)0xfe00780c)
#define P_PWRCTRL_RESTORE_EN0                      ((volatile uint32_t *)0xfe007810)
#define P_PWRCTRL_MEM_PD0                          ((volatile uint32_t *)0xfe007840)
#define P_PWRCTRL_MEM_PD1                          ((volatile uint32_t *)0xfe007844)
#define P_PWRCTRL_MEM_PD2                          ((volatile uint32_t *)0xfe007848)
#define P_PWRCTRL_MEM_PD3                          ((volatile uint32_t *)0xfe00784c)
#define P_PWRCTRL_MEM_PD4                          ((volatile uint32_t *)0xfe007850)
#define P_PWRCTRL_R_ISO_EN0                        ((volatile uint32_t *)0xfe007880)
#define P_PWRCTRL_R_PWR_OFF0                       ((volatile uint32_t *)0xfe007884)
#define P_PWRCTRL_R_MEM_PD0                        ((volatile uint32_t *)0xfe007888)
#define P_PWRCTRL_R_MEM_PD1                        ((volatile uint32_t *)0xfe00788c)
#define P_PWRCTRL_R_MEM_PD2                        ((volatile uint32_t *)0xfe007890)
#define P_PWRCTRL_R_MEM_PD3                        ((volatile uint32_t *)0xfe007894)
#define P_PWRCTRL_R_MEM_PD4                        ((volatile uint32_t *)0xfe007898)
//`define PWRCTRL_MEM_PD_DSPA         8'h10
//`define PWRCTRL_MEM_PD_DSPB         8'h11
//`define PWRCTRL_MEM_PD_RAMA         8'h12
//`define PWRCTRL_MEM_PD_RAMB         8'h13
//`define PWRCTRL_MEM_PD_CPU          8'h14
//`define PWRCTRL_MEM_PD_AUDIO        8'h20
//`define PWRCTRL_MEM_PD_USBCTRL      8'h21
//`define PWRCTRL_MEM_PD_SPICC        8'h22
//`define PWRCTRL_MEM_PD_SPIFC        8'h23
//`define PWRCTRL_MEM_PD_IR           8'h24
//`define PWRCTRL_MEM_PD_SDIO         8'h25
//`define PWRCTRL_MEM_PD_KLEFUSE      8'h26
//`define PWRCTRL_MEM_PD_PSRAM        8'h27
//`define PWRCTRL_MEM_PD_DDR          8'h28
//`define PWRCTRL_MEM_PD_DMC          8'h29
#define P_PWRCTRL_AUTO_OFF_CTRL                    ((volatile uint32_t *)0xfe0078c0)
#define P_PWRCTRL_AUTO_OFF                         ((volatile uint32_t *)0xfe0078c4)
#define P_PWRCTRL_AUTO_OFF_STS                     ((volatile uint32_t *)0xfe0078c8)
#define P_PWRCTRL_TIMER_TH_01                      ((volatile uint32_t *)0xfe0078cc)
#define P_PWRCTRL_TIMER_TH_23                      ((volatile uint32_t *)0xfe0078d0)
#define P_PWRCTRL_TIMER_TH_45                      ((volatile uint32_t *)0xfe0078d4)
#define P_PWRCTRL_TIMER_TH_67                      ((volatile uint32_t *)0xfe0078d8)
#define P_PWRCTRL_MASK_PWR_ACK0                    ((volatile uint32_t *)0xfe007900)
#define P_PWRCTRL_MASK_RSTN_OFF0                   ((volatile uint32_t *)0xfe007904)
#define P_PWRCTRL_MASK_RSTN_ON0                    ((volatile uint32_t *)0xfe007908)
#define P_PWRCTRL_MASK_PWR_OFF0                    ((volatile uint32_t *)0xfe00790c)
#define P_PWRCTRL_MASK_PWR_ON0                     ((volatile uint32_t *)0xfe007910)
#define P_PWRCTRL_MASK_ISO_OFF0                    ((volatile uint32_t *)0xfe007914)
#define P_PWRCTRL_MASK_ISO_ON0                     ((volatile uint32_t *)0xfe007918)
#define P_PWRCTRL_MASK_MEM_OFF0                    ((volatile uint32_t *)0xfe007940)
#define P_PWRCTRL_MASK_MEM_OFF1                    ((volatile uint32_t *)0xfe007944)
#define P_PWRCTRL_MASK_MEM_OFF2                    ((volatile uint32_t *)0xfe007948)
#define P_PWRCTRL_MASK_MEM_OFF3                    ((volatile uint32_t *)0xfe00794c)
#define P_PWRCTRL_MASK_MEM_OFF4                    ((volatile uint32_t *)0xfe007950)
#define P_PWRCTRL_MASK_MEM_ON0                     ((volatile uint32_t *)0xfe007960)
#define P_PWRCTRL_MASK_MEM_ON1                     ((volatile uint32_t *)0xfe007964)
#define P_PWRCTRL_MASK_MEM_ON2                     ((volatile uint32_t *)0xfe007968)
#define P_PWRCTRL_MASK_MEM_ON3                     ((volatile uint32_t *)0xfe00796c)
#define P_PWRCTRL_MASK_MEM_ON4                     ((volatile uint32_t *)0xfe007970)
//========================================================================
//  APB0_ANA_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe007c00
// -----------------------------------------------
#define P_ANACTRL_FIXPLL_CTRL0                     ((volatile uint32_t *)0xfe007c80)
#define P_ANACTRL_FIXPLL_CTRL1                     ((volatile uint32_t *)0xfe007c84)
#define P_ANACTRL_FIXPLL_CTRL2                     ((volatile uint32_t *)0xfe007c88)
#define P_ANACTRL_FIXPLL_CTRL3                     ((volatile uint32_t *)0xfe007c8c)
#define P_ANACTRL_FIXPLL_CTRL4                     ((volatile uint32_t *)0xfe007c90)
#define P_ANACTRL_FIXPLL_STS                       ((volatile uint32_t *)0xfe007c94)
#define P_ANACTRL_SYSPLL_CTRL0                     ((volatile uint32_t *)0xfe007d00)
#define P_ANACTRL_SYSPLL_CTRL1                     ((volatile uint32_t *)0xfe007d04)
#define P_ANACTRL_SYSPLL_CTRL2                     ((volatile uint32_t *)0xfe007d08)
#define P_ANACTRL_SYSPLL_CTRL3                     ((volatile uint32_t *)0xfe007d0c)
#define P_ANACTRL_SYSPLL_CTRL4                     ((volatile uint32_t *)0xfe007d10)
#define P_ANACTRL_SYSPLL_STS                       ((volatile uint32_t *)0xfe007d14)
#define P_ANACTRL_HIFIPLL_CTRL0                    ((volatile uint32_t *)0xfe007d40)
#define P_ANACTRL_HIFIPLL_CTRL1                    ((volatile uint32_t *)0xfe007d44)
#define P_ANACTRL_HIFIPLL_CTRL2                    ((volatile uint32_t *)0xfe007d48)
#define P_ANACTRL_HIFIPLL_CTRL3                    ((volatile uint32_t *)0xfe007d4c)
#define P_ANACTRL_HIFIPLL_CTRL4                    ((volatile uint32_t *)0xfe007d50)
#define P_ANACTRL_HIFIPLL_STS                      ((volatile uint32_t *)0xfe007d54)
#define P_ANACTRL_AUDDDS_CTRL0                     ((volatile uint32_t *)0xfe007d80)
#define P_ANACTRL_AUDDDS_CTRL1                     ((volatile uint32_t *)0xfe007d84)
#define P_ANACTRL_AUDDDS_CTRL2                     ((volatile uint32_t *)0xfe007d88)
#define P_ANACTRL_AUDDDS_CTRL3                     ((volatile uint32_t *)0xfe007d8c)
#define P_ANACTRL_AUDDDS_CTRL4                     ((volatile uint32_t *)0xfe007d90)
#define P_ANACTRL_AUDDDS_STS                       ((volatile uint32_t *)0xfe007d94)
#define P_ANACTRL_MISCTOP_CTRL0                    ((volatile uint32_t *)0xfe007dc0)
#define P_ANACTRL_POR_CNTL                         ((volatile uint32_t *)0xfe007e08)
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe020000
// -----------------------------------------------
#define P_RSA_BASE                                 ((volatile uint32_t *)0xfe020000)
//========================================================================
//  CPUCTRL_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfd000000
// -----------------------------------------------
#define P_CPUCTRL_CTRL0                            ((volatile uint32_t *)0xfd000000)
#define P_CPUCTRL_CTRL1                            ((volatile uint32_t *)0xfd000004)
#define P_CPUCTRL_CTRL2                            ((volatile uint32_t *)0xfd000008)
#define P_CPUCTRL_CTRL3                            ((volatile uint32_t *)0xfd00000c)
#define P_CPUCTRL_CTRL4                            ((volatile uint32_t *)0xfd000010)
#define P_CPUCTRL_CTRL5                            ((volatile uint32_t *)0xfd000014)
#define P_CPUCTRL_CTRL6                            ((volatile uint32_t *)0xfd000018)
#define P_CPUCTRL_CTRL7                            ((volatile uint32_t *)0xfd00001c)
#define P_CPUCTRL_CTRL8                            ((volatile uint32_t *)0xfd000020)
#define P_CPUCTRL_CTRL9                            ((volatile uint32_t *)0xfd000024)
#define P_CPUCTRL_CTRL10                           ((volatile uint32_t *)0xfd000028)
#define P_CPUCTRL_CTRL11                           ((volatile uint32_t *)0xfd00002c)
#define P_CPUCTRL_CTRL12                           ((volatile uint32_t *)0xfd000030)
#define P_CPUCTRL_CTRL13                           ((volatile uint32_t *)0xfd000034)
#define P_CPUCTRL_CTRL14                           ((volatile uint32_t *)0xfd000038)
#define P_CPUCTRL_CTRL15                           ((volatile uint32_t *)0xfd00003c)
#define P_CPUCTRL_STS0                             ((volatile uint32_t *)0xfd000040)
#define P_CPUCTRL_STS1                             ((volatile uint32_t *)0xfd000044)
#define P_CPUCTRL_STS2                             ((volatile uint32_t *)0xfd000048)
#define P_CPUCTRL_STS3                             ((volatile uint32_t *)0xfd00004c)
#define P_CPUCTRL_STS4                             ((volatile uint32_t *)0xfd000050)
#define P_CPUCTRL_STS5                             ((volatile uint32_t *)0xfd000054)
#define P_CPUCTRL_STS6                             ((volatile uint32_t *)0xfd000058)
#define P_CPUCTRL_STS7                             ((volatile uint32_t *)0xfd00005c)
#define P_CPUCTRL_STS8                             ((volatile uint32_t *)0xfd000060)
#define P_CPUCTRL_STS9                             ((volatile uint32_t *)0xfd000064)
#define P_CPUCTRL_CLK_CTRL0                        ((volatile uint32_t *)0xfd000080)
#define P_CPUCTRL_CLK_CTRL1                        ((volatile uint32_t *)0xfd000084)
#define P_CPUCTRL_CLK_CTRL2                        ((volatile uint32_t *)0xfd000088)
#define P_CPUCTRL_CLK_CTRL3                        ((volatile uint32_t *)0xfd00008c)
#define P_CPUCTRL_CLK_CTRL4                        ((volatile uint32_t *)0xfd000090)
#define P_CPUCTRL_CLK_CTRL5                        ((volatile uint32_t *)0xfd000094)
#define P_CPUCTRL_CLK_CTRL6                        ((volatile uint32_t *)0xfd000098)
#define P_CPUCTRL_CLK_CTRL7                        ((volatile uint32_t *)0xfd00009c)
#define P_CPUCTRL_RESET_CTRL                       ((volatile uint32_t *)0xfd0000c0)
#define P_CPUCTRL_ROM_DISABLE                      ((volatile uint32_t *)0xfd0000c4)
//========================================================================
//  RAMA_SEC_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfd000800
// -----------------------------------------------
#define P_SRAM_RANGE0_STA                          ((volatile uint32_t *)0xfd000800)
#define P_SRAM_RANGE1_STA                          ((volatile uint32_t *)0xfd000804)
#define P_SRAM_RANGE2_STA                          ((volatile uint32_t *)0xfd000808)
#define P_SRAM_RANGE3_STA                          ((volatile uint32_t *)0xfd00080c)
#define P_SRAM_RANGE4_STA                          ((volatile uint32_t *)0xfd000810)
#define P_SRAM_RANGE5_STA                          ((volatile uint32_t *)0xfd000814)
#define P_SRAM_RANGE6_STA                          ((volatile uint32_t *)0xfd000818)
#define P_SRAM_RANGE0_EDA                          ((volatile uint32_t *)0xfd00081c)
#define P_SRAM_RANGE1_EDA                          ((volatile uint32_t *)0xfd000820)
#define P_SRAM_RANGE2_EDA                          ((volatile uint32_t *)0xfd000824)
#define P_SRAM_RANGE3_EDA                          ((volatile uint32_t *)0xfd000828)
#define P_SRAM_RANGE4_EDA                          ((volatile uint32_t *)0xfd00082c)
#define P_SRAM_RANGE5_EDA                          ((volatile uint32_t *)0xfd000830)
#define P_SRAM_RANGE6_EDA                          ((volatile uint32_t *)0xfd000834)
#define P_SRAM_RANGE0_CTRL0                        ((volatile uint32_t *)0xfd000838)
#define P_SRAM_RANGE1_CTRL0                        ((volatile uint32_t *)0xfd00083c)
#define P_SRAM_RANGE2_CTRL0                        ((volatile uint32_t *)0xfd000840)
#define P_SRAM_RANGE3_CTRL0                        ((volatile uint32_t *)0xfd000844)
#define P_SRAM_RANGE4_CTRL0                        ((volatile uint32_t *)0xfd000848)
#define P_SRAM_RANGE5_CTRL0                        ((volatile uint32_t *)0xfd00084c)
#define P_SRAM_RANGE6_CTRL0                        ((volatile uint32_t *)0xfd000850)
#define P_SRAM_RANGE7_CTRL0                        ((volatile uint32_t *)0xfd000854)
#define P_SRAM_RANGE0_CTRL1                        ((volatile uint32_t *)0xfd000858)
#define P_SRAM_RANGE1_CTRL1                        ((volatile uint32_t *)0xfd00085c)
#define P_SRAM_RANGE2_CTRL1                        ((volatile uint32_t *)0xfd000860)
#define P_SRAM_RANGE3_CTRL1                        ((volatile uint32_t *)0xfd000864)
#define P_SRAM_RANGE4_CTRL1                        ((volatile uint32_t *)0xfd000868)
#define P_SRAM_RANGE5_CTRL1                        ((volatile uint32_t *)0xfd00086c)
#define P_SRAM_RANGE6_CTRL1                        ((volatile uint32_t *)0xfd000870)
#define P_SRAM_RANGE7_CTRL1                        ((volatile uint32_t *)0xfd000874)
#define P_SRAM_SEC_CTRL0                           ((volatile uint32_t *)0xfd000878)
#define P_SRAM_REG_LOCK                            ((volatile uint32_t *)0xfd00087c)
//========================================================================
//  RAMB_SEC_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfd000c00
// -----------------------------------------------
#define P_SRAM_RANGE0_STA_B                        ((volatile uint32_t *)0xfd000c00)
#define P_SRAM_RANGE1_STA_B                        ((volatile uint32_t *)0xfd000c04)
#define P_SRAM_RANGE2_STA_B                        ((volatile uint32_t *)0xfd000c08)
#define P_SRAM_RANGE3_STA_B                        ((volatile uint32_t *)0xfd000c0c)
#define P_SRAM_RANGE4_STA_B                        ((volatile uint32_t *)0xfd000c10)
#define P_SRAM_RANGE5_STA_B                        ((volatile uint32_t *)0xfd000c14)
#define P_SRAM_RANGE6_STA_B                        ((volatile uint32_t *)0xfd000c18)
#define P_SRAM_RANGE0_EDA_B                        ((volatile uint32_t *)0xfd000c1c)
#define P_SRAM_RANGE1_EDA_B                        ((volatile uint32_t *)0xfd000c20)
#define P_SRAM_RANGE2_EDA_B                        ((volatile uint32_t *)0xfd000c24)
#define P_SRAM_RANGE3_EDA_B                        ((volatile uint32_t *)0xfd000c28)
#define P_SRAM_RANGE4_EDA_B                        ((volatile uint32_t *)0xfd000c2c)
#define P_SRAM_RANGE5_EDA_B                        ((volatile uint32_t *)0xfd000c30)
#define P_SRAM_RANGE6_EDA_B                        ((volatile uint32_t *)0xfd000c34)
#define P_SRAM_RANGE0_CTRL0_B                      ((volatile uint32_t *)0xfd000c38)
#define P_SRAM_RANGE1_CTRL0_B                      ((volatile uint32_t *)0xfd000c3c)
#define P_SRAM_RANGE2_CTRL0_B                      ((volatile uint32_t *)0xfd000c40)
#define P_SRAM_RANGE3_CTRL0_B                      ((volatile uint32_t *)0xfd000c44)
#define P_SRAM_RANGE4_CTRL0_B                      ((volatile uint32_t *)0xfd000c48)
#define P_SRAM_RANGE5_CTRL0_B                      ((volatile uint32_t *)0xfd000c4c)
#define P_SRAM_RANGE6_CTRL0_B                      ((volatile uint32_t *)0xfd000c50)
#define P_SRAM_RANGE7_CTRL0_B                      ((volatile uint32_t *)0xfd000c54)
#define P_SRAM_RANGE0_CTRL1_B                      ((volatile uint32_t *)0xfd000c58)
#define P_SRAM_RANGE1_CTRL1_B                      ((volatile uint32_t *)0xfd000c5c)
#define P_SRAM_RANGE2_CTRL1_B                      ((volatile uint32_t *)0xfd000c60)
#define P_SRAM_RANGE3_CTRL1_B                      ((volatile uint32_t *)0xfd000c64)
#define P_SRAM_RANGE4_CTRL1_B                      ((volatile uint32_t *)0xfd000c68)
#define P_SRAM_RANGE5_CTRL1_B                      ((volatile uint32_t *)0xfd000c6c)
#define P_SRAM_RANGE6_CTRL1_B                      ((volatile uint32_t *)0xfd000c70)
#define P_SRAM_RANGE7_CTRL1_B                      ((volatile uint32_t *)0xfd000c74)
#define P_SRAM_SEC_CTRL0_B                         ((volatile uint32_t *)0xfd000c78)
#define P_SRAM_REG_LOCK_B                          ((volatile uint32_t *)0xfd000c7c)
//
// Reading file:  REG_LIST_DSP_RTL.h
//
// synopsys translate_off
// synopsys translate_on
//========================================================================
//  DSPA - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe030000
// -----------------------------------------------
#define P_DSP_CFG0                                 ((volatile uint32_t *)0xfe030000)
#define P_DSP_CFG1                                 ((volatile uint32_t *)0xfe030004)
#define P_DSP_CFG2                                 ((volatile uint32_t *)0xfe030008)
#define P_DSP_IMPWIRE                              ((volatile uint32_t *)0xfe03000c)
#define P_DSP_RESET_VEC                            ((volatile uint32_t *)0xfe030010)
#define P_DSP_SEC_CFG0                             ((volatile uint32_t *)0xfe030018)
#define P_DSP_SEC_CFG1                             ((volatile uint32_t *)0xfe03001c)
#define P_DSP_IRQ_CTRL0                            ((volatile uint32_t *)0xfe030040)
#define P_DSP_IRQ_CTRL1                            ((volatile uint32_t *)0xfe030044)
#define P_DSP_IRQ_CTRL2                            ((volatile uint32_t *)0xfe030048)
#define P_DSP_IRQ_CTRL3                            ((volatile uint32_t *)0xfe03004c)
#define P_DSP_IRQ_CTRL4                            ((volatile uint32_t *)0xfe030050)
#define P_DSP_IRQ_CTRL5                            ((volatile uint32_t *)0xfe030054)
#define P_DSP_IRQ_CTRL6                            ((volatile uint32_t *)0xfe030058)
#define P_DSP_IRQ_CTRL7                            ((volatile uint32_t *)0xfe03005c)
#define P_DSP_IRQ_CTRL8                            ((volatile uint32_t *)0xfe030060)
#define P_DSP_IRQ_STS                              ((volatile uint32_t *)0xfe03007c)
#define P_DSP_REMAP0                               ((volatile uint32_t *)0xfe030080)
#define P_DSP_REMAP1                               ((volatile uint32_t *)0xfe030084)
#define P_DSP_REMAP2                               ((volatile uint32_t *)0xfe030088)
#define P_DSP_STS0                                 ((volatile uint32_t *)0xfe030100)
#define P_DSP_STS1                                 ((volatile uint32_t *)0xfe030104)
#define P_DSP_STS2                                 ((volatile uint32_t *)0xfe030108)
#define P_DSP_STS3                                 ((volatile uint32_t *)0xfe03010c)
#define P_DSP_STS4                                 ((volatile uint32_t *)0xfe030110)
#define P_DSP_STS5                                 ((volatile uint32_t *)0xfe030114)
#define P_DSP_MAILBOX_SET_0                        ((volatile uint32_t *)0xfe030140)
#define P_DSP_MAILBOX_SET_1                        ((volatile uint32_t *)0xfe030144)
#define P_DSP_MAILBOX_SET_2                        ((volatile uint32_t *)0xfe030148)
#define P_DSP_MAILBOX_SET_3                        ((volatile uint32_t *)0xfe03014c)
#define P_DSP_MAILBOX_SET_4                        ((volatile uint32_t *)0xfe030150)
#define P_DSP_MAILBOX_SET_5                        ((volatile uint32_t *)0xfe030154)
#define P_DSP_MAILBOX_SET_6                        ((volatile uint32_t *)0xfe030158)
#define P_DSP_MAILBOX_SET_7                        ((volatile uint32_t *)0xfe03015c)
#define P_DSP_MAILBOX_SET_8                        ((volatile uint32_t *)0xfe030160)
#define P_DSP_MAILBOX_SET_9                        ((volatile uint32_t *)0xfe030164)
#define P_DSP_MAILBOX_SET_10                       ((volatile uint32_t *)0xfe030168)
#define P_DSP_MAILBOX_SET_11                       ((volatile uint32_t *)0xfe03016c)
#define P_DSP_MAILBOX_CLR_0                        ((volatile uint32_t *)0xfe030180)
#define P_DSP_MAILBOX_CLR_1                        ((volatile uint32_t *)0xfe030184)
#define P_DSP_MAILBOX_CLR_2                        ((volatile uint32_t *)0xfe030188)
#define P_DSP_MAILBOX_CLR_3                        ((volatile uint32_t *)0xfe03018c)
#define P_DSP_MAILBOX_CLR_4                        ((volatile uint32_t *)0xfe030190)
#define P_DSP_MAILBOX_CLR_5                        ((volatile uint32_t *)0xfe030194)
#define P_DSP_MAILBOX_CLR_6                        ((volatile uint32_t *)0xfe030198)
#define P_DSP_MAILBOX_CLR_7                        ((volatile uint32_t *)0xfe03019c)
#define P_DSP_MAILBOX_CLR_8                        ((volatile uint32_t *)0xfe0301a0)
#define P_DSP_MAILBOX_CLR_9                        ((volatile uint32_t *)0xfe0301a4)
#define P_DSP_MAILBOX_CLR_10                       ((volatile uint32_t *)0xfe0301a8)
#define P_DSP_MAILBOX_CLR_11                       ((volatile uint32_t *)0xfe0301ac)
#define P_DSP_MAILBOX_STAT_0                       ((volatile uint32_t *)0xfe0301c0)
#define P_DSP_MAILBOX_STAT_1                       ((volatile uint32_t *)0xfe0301c4)
#define P_DSP_MAILBOX_STAT_2                       ((volatile uint32_t *)0xfe0301c8)
#define P_DSP_MAILBOX_STAT_3                       ((volatile uint32_t *)0xfe0301cc)
#define P_DSP_MAILBOX_STAT_4                       ((volatile uint32_t *)0xfe0301d0)
#define P_DSP_MAILBOX_STAT_5                       ((volatile uint32_t *)0xfe0301d4)
#define P_DSP_MAILBOX_STAT_6                       ((volatile uint32_t *)0xfe0301d8)
#define P_DSP_MAILBOX_STAT_7                       ((volatile uint32_t *)0xfe0301dc)
#define P_DSP_MAILBOX_STAT_8                       ((volatile uint32_t *)0xfe0301e0)
#define P_DSP_MAILBOX_STAT_9                       ((volatile uint32_t *)0xfe0301e4)
#define P_DSP_MAILBOX_STAT_10                      ((volatile uint32_t *)0xfe0301e8)
#define P_DSP_MAILBOX_STAT_11                      ((volatile uint32_t *)0xfe0301ec)
#define P_DSP_QIF_CTRL                             ((volatile uint32_t *)0xfe030200)
#define P_DSP_QIF_STS                              ((volatile uint32_t *)0xfe030204)
#define P_DSP_WRFIFO_TOCPUA                        ((volatile uint32_t *)0xfe030208)
#define P_DSP_WRFIFO_TOCPUB                        ((volatile uint32_t *)0xfe03020c)
#define P_DSP_WRFIFO_TODSP                         ((volatile uint32_t *)0xfe030210)
#define P_DSP_RDFIFO_FRCPUA                        ((volatile uint32_t *)0xfe030220)
#define P_DSP_RDFIFO_FRCPUB                        ((volatile uint32_t *)0xfe030224)
#define P_DSP_RDFIFO_FRDSP                         ((volatile uint32_t *)0xfe030228)
//========================================================================
//  DSPB - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe040000
// -----------------------------------------------
#define P_DSPB_CFG0                                ((volatile uint32_t *)0xfe040000)
#define P_DSPB_CFG1                                ((volatile uint32_t *)0xfe040004)
#define P_DSPB_CFG2                                ((volatile uint32_t *)0xfe040008)
#define P_DSPB_IMPWIRE                             ((volatile uint32_t *)0xfe04000c)
#define P_DSPB_RESET_VEC                           ((volatile uint32_t *)0xfe040010)
#define P_DSPB_SEC_CFG0                            ((volatile uint32_t *)0xfe040018)
#define P_DSPB_SEC_CFG1                            ((volatile uint32_t *)0xfe04001c)
#define P_DSPB_IRQ_CTRL0                           ((volatile uint32_t *)0xfe040040)
#define P_DSPB_IRQ_CTRL1                           ((volatile uint32_t *)0xfe040044)
#define P_DSPB_IRQ_CTRL2                           ((volatile uint32_t *)0xfe040048)
#define P_DSPB_IRQ_CTRL3                           ((volatile uint32_t *)0xfe04004c)
#define P_DSPB_IRQ_CTRL4                           ((volatile uint32_t *)0xfe040050)
#define P_DSPB_IRQ_CTRL5                           ((volatile uint32_t *)0xfe040054)
#define P_DSPB_IRQ_CTRL6                           ((volatile uint32_t *)0xfe040058)
#define P_DSPB_IRQ_CTRL7                           ((volatile uint32_t *)0xfe04005c)
#define P_DSPB_IRQ_CTRL8                           ((volatile uint32_t *)0xfe040060)
#define P_DSPB_IRQ_STS                             ((volatile uint32_t *)0xfe04007c)
#define P_DSPB_REMAP0                              ((volatile uint32_t *)0xfe040080)
#define P_DSPB_REMAP1                              ((volatile uint32_t *)0xfe040084)
#define P_DSPB_REMAP2                              ((volatile uint32_t *)0xfe040088)
#define P_DSPB_STS0                                ((volatile uint32_t *)0xfe040100)
#define P_DSPB_STS1                                ((volatile uint32_t *)0xfe040104)
#define P_DSPB_STS2                                ((volatile uint32_t *)0xfe040108)
#define P_DSPB_STS3                                ((volatile uint32_t *)0xfe04010c)
#define P_DSPB_STS4                                ((volatile uint32_t *)0xfe040110)
#define P_DSPB_STS5                                ((volatile uint32_t *)0xfe040114)
#define P_DSPB_MAILBOX_SET_0                       ((volatile uint32_t *)0xfe040140)
#define P_DSPB_MAILBOX_SET_1                       ((volatile uint32_t *)0xfe040144)
#define P_DSPB_MAILBOX_SET_2                       ((volatile uint32_t *)0xfe040148)
#define P_DSPB_MAILBOX_SET_3                       ((volatile uint32_t *)0xfe04014c)
#define P_DSPB_MAILBOX_SET_4                       ((volatile uint32_t *)0xfe040150)
#define P_DSPB_MAILBOX_SET_5                       ((volatile uint32_t *)0xfe040154)
#define P_DSPB_MAILBOX_SET_6                       ((volatile uint32_t *)0xfe040158)
#define P_DSPB_MAILBOX_SET_7                       ((volatile uint32_t *)0xfe04015c)
#define P_DSPB_MAILBOX_SET_8                       ((volatile uint32_t *)0xfe040160)
#define P_DSPB_MAILBOX_SET_9                       ((volatile uint32_t *)0xfe040164)
#define P_DSPB_MAILBOX_SET_10                      ((volatile uint32_t *)0xfe040168)
#define P_DSPB_MAILBOX_SET_11                      ((volatile uint32_t *)0xfe04016c)
#define P_DSPB_MAILBOX_CLR_0                       ((volatile uint32_t *)0xfe040180)
#define P_DSPB_MAILBOX_CLR_1                       ((volatile uint32_t *)0xfe040184)
#define P_DSPB_MAILBOX_CLR_2                       ((volatile uint32_t *)0xfe040188)
#define P_DSPB_MAILBOX_CLR_3                       ((volatile uint32_t *)0xfe04018c)
#define P_DSPB_MAILBOX_CLR_4                       ((volatile uint32_t *)0xfe040190)
#define P_DSPB_MAILBOX_CLR_5                       ((volatile uint32_t *)0xfe040194)
#define P_DSPB_MAILBOX_CLR_6                       ((volatile uint32_t *)0xfe040198)
#define P_DSPB_MAILBOX_CLR_7                       ((volatile uint32_t *)0xfe04019c)
#define P_DSPB_MAILBOX_CLR_8                       ((volatile uint32_t *)0xfe0401a0)
#define P_DSPB_MAILBOX_CLR_9                       ((volatile uint32_t *)0xfe0401a4)
#define P_DSPB_MAILBOX_CLR_10                      ((volatile uint32_t *)0xfe0401a8)
#define P_DSPB_MAILBOX_CLR_11                      ((volatile uint32_t *)0xfe0401ac)
#define P_DSPB_MAILBOX_STAT_0                      ((volatile uint32_t *)0xfe0401c0)
#define P_DSPB_MAILBOX_STAT_1                      ((volatile uint32_t *)0xfe0401c4)
#define P_DSPB_MAILBOX_STAT_2                      ((volatile uint32_t *)0xfe0401c8)
#define P_DSPB_MAILBOX_STAT_3                      ((volatile uint32_t *)0xfe0401cc)
#define P_DSPB_MAILBOX_STAT_4                      ((volatile uint32_t *)0xfe0401d0)
#define P_DSPB_MAILBOX_STAT_5                      ((volatile uint32_t *)0xfe0401d4)
#define P_DSPB_MAILBOX_STAT_6                      ((volatile uint32_t *)0xfe0401d8)
#define P_DSPB_MAILBOX_STAT_7                      ((volatile uint32_t *)0xfe0401dc)
#define P_DSPB_MAILBOX_STAT_8                      ((volatile uint32_t *)0xfe0401e0)
#define P_DSPB_MAILBOX_STAT_9                      ((volatile uint32_t *)0xfe0401e4)
#define P_DSPB_MAILBOX_STAT_10                     ((volatile uint32_t *)0xfe0401e8)
#define P_DSPB_MAILBOX_STAT_11                     ((volatile uint32_t *)0xfe0401ec)
#define P_DSPB_QIF_CTRL                            ((volatile uint32_t *)0xfe040200)
#define P_DSPB_QIF_STS                             ((volatile uint32_t *)0xfe040204)
#define P_DSPB_WRFIFO_TOCPUA                       ((volatile uint32_t *)0xfe040208)
#define P_DSPB_WRFIFO_TOCPUB                       ((volatile uint32_t *)0xfe04020c)
#define P_DSPB_WRFIFO_TODSP                        ((volatile uint32_t *)0xfe040210)
#define P_DSPB_RDFIFO_FRCPUA                       ((volatile uint32_t *)0xfe040220)
#define P_DSPB_RDFIFO_FRCPUB                       ((volatile uint32_t *)0xfe040224)
#define P_DSPB_RDFIFO_FRDSP                        ((volatile uint32_t *)0xfe040228)
// synopsys translate_off
// synopsys translate_on
//
// Closing file:  REG_LIST_DSP_RTL.h
//
// synopsys translate_off
// synopsys translate_on
//
// Closing file:  ./REG_LIST_RTL.h
//

#endif // P_REGISTER_H

