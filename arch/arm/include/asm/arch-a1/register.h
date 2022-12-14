/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifdef REGISTER_H
#else
#define REGISTER_H

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
#define EE_AUDIO_CLK_GATE_EN0                      ((0x0000  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_GATE_EN1                      ((0x0001  << 2) + 0xfe050000)
#define EE_AUDIO_MCLK_A_CTRL                       ((0x0002  << 2) + 0xfe050000)
#define EE_AUDIO_MCLK_B_CTRL                       ((0x0003  << 2) + 0xfe050000)
#define EE_AUDIO_MCLK_C_CTRL                       ((0x0004  << 2) + 0xfe050000)
#define EE_AUDIO_MCLK_D_CTRL                       ((0x0005  << 2) + 0xfe050000)
#define EE_AUDIO_MCLK_E_CTRL                       ((0x0006  << 2) + 0xfe050000)
#define EE_AUDIO_MCLK_F_CTRL                       ((0x0007  << 2) + 0xfe050000)
#define EE_AUDIO_PAD_CTRL0                         ((0x0008  << 2) + 0xfe050000)
#define EE_AUDIO_PAD_CTRL1                         ((0x0009  << 2) + 0xfe050000)
#define EE_AUDIO_SW_RESET0                         ((0x000a  << 2) + 0xfe050000)
#define EE_AUDIO_SW_RESET1                         ((0x000b  << 2) + 0xfe050000)
#define EE_AUDIO_CLK81_CTRL                        ((0x000c  << 2) + 0xfe050000)
#define EE_AUDIO_CLK81_EN                          ((0x000d  << 2) + 0xfe050000)
#define EE_AUDIO_MST_A_SCLK_CTRL0                  ((0x0010  << 2) + 0xfe050000)
#define EE_AUDIO_MST_A_SCLK_CTRL1                  ((0x0011  << 2) + 0xfe050000)
#define EE_AUDIO_MST_B_SCLK_CTRL0                  ((0x0012  << 2) + 0xfe050000)
#define EE_AUDIO_MST_B_SCLK_CTRL1                  ((0x0013  << 2) + 0xfe050000)
#define EE_AUDIO_MST_C_SCLK_CTRL0                  ((0x0014  << 2) + 0xfe050000)
#define EE_AUDIO_MST_C_SCLK_CTRL1                  ((0x0015  << 2) + 0xfe050000)
#define EE_AUDIO_MST_D_SCLK_CTRL0                  ((0x0016  << 2) + 0xfe050000)
#define EE_AUDIO_MST_D_SCLK_CTRL1                  ((0x0017  << 2) + 0xfe050000)
#define EE_AUDIO_MST_E_SCLK_CTRL0                  ((0x0018  << 2) + 0xfe050000)
#define EE_AUDIO_MST_E_SCLK_CTRL1                  ((0x0019  << 2) + 0xfe050000)
#define EE_AUDIO_MST_F_SCLK_CTRL0                  ((0x001a  << 2) + 0xfe050000)
#define EE_AUDIO_MST_F_SCLK_CTRL1                  ((0x001b  << 2) + 0xfe050000)
#define EE_AUDIO_MST_DLY_CTRL0                     ((0x001c  << 2) + 0xfe050000)
#define EE_AUDIO_MST_DLY_CTRL1                     ((0x001d  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_TDMIN_A_CTRL                  ((0x0020  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_TDMIN_B_CTRL                  ((0x0021  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_TDMIN_C_CTRL                  ((0x0022  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_TDMIN_LB_CTRL                 ((0x0023  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_TDMOUT_A_CTRL                 ((0x0024  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_TDMOUT_B_CTRL                 ((0x0025  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_TDMOUT_C_CTRL                 ((0x0026  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_SPDIFIN_CTRL                  ((0x0027  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_SPDIFOUT_CTRL                 ((0x0028  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_RESAMPLEA_CTRL                ((0x0029  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_LOCKER_CTRL                   ((0x002a  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_PDMIN_CTRL0                   ((0x002b  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_PDMIN_CTRL1                   ((0x002c  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_SPDIFOUT_B_CTRL               ((0x002d  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_RESAMPLEB_CTRL                ((0x002e  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_SPDIFIN_LB_CTRL               ((0x002f  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_EQDRC_CTRL0                   ((0x0030  << 2) + 0xfe050000)
#define EE_AUDIO_VAD_CLK_CTRL                      ((0x0031  << 2) + 0xfe050000)
#define EE_AUDIO_EARCTX_CMDC_CLK_CTRL              ((0x0032  << 2) + 0xfe050000)
#define EE_AUDIO_EARCTX_DMAC_CLK_CTRL              ((0x0033  << 2) + 0xfe050000)
#define EE_AUDIO_EARCRX_CMDC_CLK_CTRL              ((0x0034  << 2) + 0xfe050000)
#define EE_AUDIO_EARCRX_DMAC_CLK_CTRL              ((0x0035  << 2) + 0xfe050000)
#define EE_AUDIO_CLK_LOCKERB_CTRL                  ((0x0036  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_A_CTRL0                     ((0x0040  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_A_CTRL1                     ((0x0041  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_A_START_ADDR                ((0x0042  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_A_FINISH_ADDR               ((0x0043  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_A_INT_ADDR                  ((0x0044  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_A_STATUS1                   ((0x0045  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_A_STATUS2                   ((0x0046  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_A_START_ADDRB               ((0x0047  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_A_FINISH_ADDRB              ((0x0048  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_A_INIT_ADDR                 ((0x0049  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_A_CTRL2                     ((0x004a  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_B_CTRL0                     ((0x0050  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_B_CTRL1                     ((0x0051  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_B_START_ADDR                ((0x0052  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_B_FINISH_ADDR               ((0x0053  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_B_INT_ADDR                  ((0x0054  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_B_STATUS1                   ((0x0055  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_B_STATUS2                   ((0x0056  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_B_START_ADDRB               ((0x0057  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_B_FINISH_ADDRB              ((0x0058  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_B_INIT_ADDR                 ((0x0059  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_B_CTRL2                     ((0x005a  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_C_CTRL0                     ((0x0060  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_C_CTRL1                     ((0x0061  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_C_START_ADDR                ((0x0062  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_C_FINISH_ADDR               ((0x0063  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_C_INT_ADDR                  ((0x0064  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_C_STATUS1                   ((0x0065  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_C_STATUS2                   ((0x0066  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_C_START_ADDRB               ((0x0067  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_C_FINISH_ADDRB              ((0x0068  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_C_INIT_ADDR                 ((0x0069  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_C_CTRL2                     ((0x006a  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_A_CTRL0                     ((0x0070  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_A_CTRL1                     ((0x0071  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_A_START_ADDR                ((0x0072  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_A_FINISH_ADDR               ((0x0073  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_A_INT_ADDR                  ((0x0074  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_A_STATUS1                   ((0x0075  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_A_STATUS2                   ((0x0076  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_A_START_ADDRB               ((0x0077  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_A_FINISH_ADDRB              ((0x0078  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_A_INIT_ADDR                 ((0x0079  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_A_CTRL2                     ((0x007a  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_B_CTRL0                     ((0x0080  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_B_CTRL1                     ((0x0081  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_B_START_ADDR                ((0x0082  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_B_FINISH_ADDR               ((0x0083  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_B_INT_ADDR                  ((0x0084  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_B_STATUS1                   ((0x0085  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_B_STATUS2                   ((0x0086  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_B_START_ADDRB               ((0x0087  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_B_FINISH_ADDRB              ((0x0088  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_B_INIT_ADDR                 ((0x0089  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_B_CTRL2                     ((0x008a  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_C_CTRL0                     ((0x0090  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_C_CTRL1                     ((0x0091  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_C_START_ADDR                ((0x0092  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_C_FINISH_ADDR               ((0x0093  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_C_INT_ADDR                  ((0x0094  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_C_STATUS1                   ((0x0095  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_C_STATUS2                   ((0x0096  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_C_START_ADDRB               ((0x0097  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_C_FINISH_ADDRB              ((0x0098  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_C_INIT_ADDR                 ((0x0099  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_C_CTRL2                     ((0x009a  << 2) + 0xfe050000)
#define EE_AUDIO_ARB_CTRL                          ((0x00a0  << 2) + 0xfe050000)
#define EE_AUDIO_LB_A_CTRL0                        ((0x00b0  << 2) + 0xfe050000)
#define EE_AUDIO_LB_A_CTRL1                        ((0x00b1  << 2) + 0xfe050000)
#define EE_AUDIO_LB_A_CTRL2                        ((0x00b2  << 2) + 0xfe050000)
#define EE_AUDIO_LB_A_CTRL3                        ((0x00b3  << 2) + 0xfe050000)
#define EE_AUDIO_LB_A_DAT_CH_ID0                   ((0x00b4  << 2) + 0xfe050000)
#define EE_AUDIO_LB_A_DAT_CH_ID1                   ((0x00b5  << 2) + 0xfe050000)
#define EE_AUDIO_LB_A_DAT_CH_ID2                   ((0x00b6  << 2) + 0xfe050000)
#define EE_AUDIO_LB_A_DAT_CH_ID3                   ((0x00b7  << 2) + 0xfe050000)
#define EE_AUDIO_LB_A_LB_CH_ID0                    ((0x00b8  << 2) + 0xfe050000)
#define EE_AUDIO_LB_A_LB_CH_ID1                    ((0x00b9  << 2) + 0xfe050000)
#define EE_AUDIO_LB_A_LB_CH_ID2                    ((0x00ba  << 2) + 0xfe050000)
#define EE_AUDIO_LB_A_LB_CH_ID3                    ((0x00bb  << 2) + 0xfe050000)
#define EE_AUDIO_LB_A_STS                          ((0x00bc  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_CTRL                      ((0x00c0  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_SWAP0                     ((0x00c1  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MASK0                     ((0x00c2  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MASK1                     ((0x00c3  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MASK2                     ((0x00c4  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MASK3                     ((0x00c5  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_STAT                      ((0x00c6  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MUTE_VAL                  ((0x00c7  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MUTE0                     ((0x00c8  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MUTE1                     ((0x00c9  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MUTE2                     ((0x00ca  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MUTE3                     ((0x00cb  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_CTRL                      ((0x00d0  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_SWAP0                     ((0x00d1  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MASK0                     ((0x00d2  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MASK1                     ((0x00d3  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MASK2                     ((0x00d4  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MASK3                     ((0x00d5  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_STAT                      ((0x00d6  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MUTE_VAL                  ((0x00d7  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MUTE0                     ((0x00d8  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MUTE1                     ((0x00d9  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MUTE2                     ((0x00da  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MUTE3                     ((0x00db  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_CTRL                      ((0x00e0  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_SWAP0                     ((0x00e1  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MASK0                     ((0x00e2  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MASK1                     ((0x00e3  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MASK2                     ((0x00e4  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MASK3                     ((0x00e5  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_STAT                      ((0x00e6  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MUTE_VAL                  ((0x00e7  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MUTE0                     ((0x00e8  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MUTE1                     ((0x00e9  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MUTE2                     ((0x00ea  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MUTE3                     ((0x00eb  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_CTRL                     ((0x00f0  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_SWAP0                    ((0x00f1  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MASK0                    ((0x00f2  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MASK1                    ((0x00f3  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MASK2                    ((0x00f4  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MASK3                    ((0x00f5  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_STAT                     ((0x00f6  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MUTE_VAL                 ((0x00f7  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MUTE0                    ((0x00f8  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MUTE1                    ((0x00f9  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MUTE2                    ((0x00fa  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MUTE3                    ((0x00fb  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_CTRL0                     ((0x0100  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_CTRL1                     ((0x0101  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_CTRL2                     ((0x0102  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_CTRL3                     ((0x0103  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_CTRL4                     ((0x0104  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_CTRL5                     ((0x0105  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_CTRL6                     ((0x0106  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_STAT0                     ((0x0107  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_STAT1                     ((0x0108  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_STAT2                     ((0x0109  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_MUTE_VAL                  ((0x010a  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEA_CTRL0                   ((0x0110  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEA_CTRL1                   ((0x0111  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEA_CTRL2                   ((0x0112  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEA_CTRL3                   ((0x0113  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEA_COEF0                   ((0x0114  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEA_COEF1                   ((0x0115  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEA_COEF2                   ((0x0116  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEA_COEF3                   ((0x0117  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEA_COEF4                   ((0x0118  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEA_STATUS1                 ((0x0119  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_STAT                     ((0x0120  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_GAIN0                    ((0x0121  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_GAIN1                    ((0x0122  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CTRL0                    ((0x0123  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CTRL1                    ((0x0124  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_PREAMB                   ((0x0125  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_SWAP                     ((0x0126  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CHSTS0                   ((0x0127  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CHSTS1                   ((0x0128  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CHSTS2                   ((0x0129  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CHSTS3                   ((0x012a  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CHSTS4                   ((0x012b  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CHSTS5                   ((0x012c  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CHSTS6                   ((0x012d  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CHSTS7                   ((0x012e  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CHSTS8                   ((0x012f  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CHSTS9                   ((0x0130  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CHSTSA                   ((0x0131  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_CHSTSB                   ((0x0132  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_MUTE_VAL                 ((0x0133  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_CTRL0                    ((0x0140  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_CTRL1                    ((0x0141  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_SWAP0                    ((0x0142  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MASK0                    ((0x0143  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MASK1                    ((0x0144  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MASK2                    ((0x0145  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MASK3                    ((0x0146  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_STAT                     ((0x0147  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_GAIN0                    ((0x0148  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_GAIN1                    ((0x0149  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MUTE_VAL                 ((0x014a  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MUTE0                    ((0x014b  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MUTE1                    ((0x014c  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MUTE2                    ((0x014d  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MUTE3                    ((0x014e  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MASK_VAL                 ((0x014f  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_CTRL0                    ((0x0150  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_CTRL1                    ((0x0151  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_SWAP0                    ((0x0152  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MASK0                    ((0x0153  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MASK1                    ((0x0154  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MASK2                    ((0x0155  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MASK3                    ((0x0156  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_STAT                     ((0x0157  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_GAIN0                    ((0x0158  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_GAIN1                    ((0x0159  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MUTE_VAL                 ((0x015a  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MUTE0                    ((0x015b  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MUTE1                    ((0x015c  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MUTE2                    ((0x015d  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MUTE3                    ((0x015e  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MASK_VAL                 ((0x015f  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_CTRL0                    ((0x0160  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_CTRL1                    ((0x0161  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_SWAP0                    ((0x0162  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MASK0                    ((0x0163  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MASK1                    ((0x0164  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MASK2                    ((0x0165  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MASK3                    ((0x0166  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_STAT                     ((0x0167  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_GAIN0                    ((0x0168  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_GAIN1                    ((0x0169  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MUTE_VAL                 ((0x016a  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MUTE0                    ((0x016b  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MUTE1                    ((0x016c  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MUTE2                    ((0x016d  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MUTE3                    ((0x016e  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MASK_VAL                 ((0x016f  << 2) + 0xfe050000)
//`define EE_AUDIO_POW_DET_CTRL0          10'h180
//`define EE_AUDIO_POW_DET_CTRL1          10'h181
//`define EE_AUDIO_POW_DET_TH_HI          10'h182
//`define EE_AUDIO_POW_DET_TH_LO          10'h183
//`define EE_AUDIO_POW_DET_VALUE          10'h184
#define EE_AUDIO_SECURITY_CTRL0                    ((0x0190  << 2) + 0xfe050000)
#define EE_AUDIO_SECURITY_CTRL1                    ((0x0191  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_STAT                   ((0x01a0  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_GAIN0                  ((0x01a1  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_GAIN1                  ((0x01a2  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CTRL0                  ((0x01a3  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CTRL1                  ((0x01a4  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_PREAMB                 ((0x01a5  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_SWAP                   ((0x01a6  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CHSTS0                 ((0x01a7  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CHSTS1                 ((0x01a8  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CHSTS2                 ((0x01a9  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CHSTS3                 ((0x01aa  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CHSTS4                 ((0x01ab  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CHSTS5                 ((0x01ac  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CHSTS6                 ((0x01ad  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CHSTS7                 ((0x01ae  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CHSTS8                 ((0x01af  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CHSTS9                 ((0x01b0  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CHSTSA                 ((0x01b1  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_CHSTSB                 ((0x01b2  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFOUT_B_MUTE_VAL               ((0x01b3  << 2) + 0xfe050000)
#define EE_AUDIO_TORAM_CTRL0                       ((0x01c0  << 2) + 0xfe050000)
#define EE_AUDIO_TORAM_CTRL1                       ((0x01c1  << 2) + 0xfe050000)
#define EE_AUDIO_TORAM_START_ADDR                  ((0x01c2  << 2) + 0xfe050000)
#define EE_AUDIO_TORAM_FINISH_ADDR                 ((0x01c3  << 2) + 0xfe050000)
#define EE_AUDIO_TORAM_INT_ADDR                    ((0x01c4  << 2) + 0xfe050000)
#define EE_AUDIO_TORAM_STATUS1                     ((0x01c5  << 2) + 0xfe050000)
#define EE_AUDIO_TORAM_STATUS2                     ((0x01c6  << 2) + 0xfe050000)
#define EE_AUDIO_TORAM_INIT_ADDR                   ((0x01c7  << 2) + 0xfe050000)
#define EE_AUDIO_TOACODEC_CTRL0                    ((0x01d0  << 2) + 0xfe050000)
#define EE_AUDIO_TOHDMITX_CTRL0                    ((0x01d1  << 2) + 0xfe050000)
#define EE_AUDIO_TOVAD_CTRL0                       ((0x01d2  << 2) + 0xfe050000)
#define EE_AUDIO_FRATV_CTRL0                       ((0x01d3  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEB_CTRL0                   ((0x01e0  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEB_CTRL1                   ((0x01e1  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEB_CTRL2                   ((0x01e2  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEB_CTRL3                   ((0x01e3  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEB_COEF0                   ((0x01e4  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEB_COEF1                   ((0x01e5  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEB_COEF2                   ((0x01e6  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEB_COEF3                   ((0x01e7  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEB_COEF4                   ((0x01e8  << 2) + 0xfe050000)
#define EE_AUDIO_RESAMPLEB_STATUS1                 ((0x01e9  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_LB_CTRL0                  ((0x01f0  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_LB_CTRL1                  ((0x01f1  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_LB_CTRL6                  ((0x01f6  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_LB_STAT0                  ((0x01f7  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_LB_STAT1                  ((0x01f8  << 2) + 0xfe050000)
#define EE_AUDIO_SPDIFIN_LB_MUTE_VAL               ((0x01fa  << 2) + 0xfe050000)
#define EE_AUDIO_FRHDMIRX_CTRL0                    ((0x0200  << 2) + 0xfe050000)
#define EE_AUDIO_FRHDMIRX_CTRL1                    ((0x0201  << 2) + 0xfe050000)
#define EE_AUDIO_FRHDMIRX_CTRL2                    ((0x0202  << 2) + 0xfe050000)
#define EE_AUDIO_FRHDMIRX_CTRL3                    ((0x0203  << 2) + 0xfe050000)
#define EE_AUDIO_FRHDMIRX_CTRL4                    ((0x0204  << 2) + 0xfe050000)
#define EE_AUDIO_FRHDMIRX_CTRL5                    ((0x0205  << 2) + 0xfe050000)
#define EE_AUDIO_FRHDMIRX_STAT0                    ((0x020a  << 2) + 0xfe050000)
#define EE_AUDIO_FRHDMIRX_STAT1                    ((0x020b  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_D_CTRL0                     ((0x0210  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_D_CTRL1                     ((0x0211  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_D_START_ADDR                ((0x0212  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_D_FINISH_ADDR               ((0x0213  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_D_INT_ADDR                  ((0x0214  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_D_STATUS1                   ((0x0215  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_D_STATUS2                   ((0x0216  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_D_START_ADDRB               ((0x0217  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_D_FINISH_ADDRB              ((0x0218  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_D_INIT_ADDR                 ((0x0219  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_D_CTRL2                     ((0x021a  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_D_CTRL0                     ((0x0220  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_D_CTRL1                     ((0x0221  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_D_START_ADDR                ((0x0222  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_D_FINISH_ADDR               ((0x0223  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_D_INT_ADDR                  ((0x0224  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_D_STATUS1                   ((0x0225  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_D_STATUS2                   ((0x0226  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_D_START_ADDRB               ((0x0227  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_D_FINISH_ADDRB              ((0x0228  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_D_INIT_ADDR                 ((0x0229  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_D_CTRL2                     ((0x022a  << 2) + 0xfe050000)
#define EE_AUDIO_LB_B_CTRL0                        ((0x0230  << 2) + 0xfe050000)
#define EE_AUDIO_LB_B_CTRL1                        ((0x0231  << 2) + 0xfe050000)
#define EE_AUDIO_LB_B_CTRL2                        ((0x0232  << 2) + 0xfe050000)
#define EE_AUDIO_LB_B_CTRL3                        ((0x0233  << 2) + 0xfe050000)
#define EE_AUDIO_LB_B_DAT_CH_ID0                   ((0x0234  << 2) + 0xfe050000)
#define EE_AUDIO_LB_B_DAT_CH_ID1                   ((0x0235  << 2) + 0xfe050000)
#define EE_AUDIO_LB_B_DAT_CH_ID2                   ((0x0236  << 2) + 0xfe050000)
#define EE_AUDIO_LB_B_DAT_CH_ID3                   ((0x0237  << 2) + 0xfe050000)
#define EE_AUDIO_LB_B_LB_CH_ID0                    ((0x0238  << 2) + 0xfe050000)
#define EE_AUDIO_LB_B_LB_CH_ID1                    ((0x0239  << 2) + 0xfe050000)
#define EE_AUDIO_LB_B_LB_CH_ID2                    ((0x023a  << 2) + 0xfe050000)
#define EE_AUDIO_LB_B_LB_CH_ID3                    ((0x023b  << 2) + 0xfe050000)
#define EE_AUDIO_LB_B_STS                          ((0x023c  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_E_CTRL0                     ((0x0240  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_E_CTRL1                     ((0x0241  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_E_START_ADDR                ((0x0242  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_E_FINISH_ADDR               ((0x0243  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_E_INT_ADDR                  ((0x0244  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_E_STATUS1                   ((0x0245  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_E_STATUS2                   ((0x0246  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_E_START_ADDRB               ((0x0247  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_E_FINISH_ADDRB              ((0x0248  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_E_INIT_ADDR                 ((0x0249  << 2) + 0xfe050000)
#define EE_AUDIO_TODDR_E_CTRL2                     ((0x024a  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_E_CTRL0                     ((0x0250  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_E_CTRL1                     ((0x0251  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_E_START_ADDR                ((0x0252  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_E_FINISH_ADDR               ((0x0253  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_E_INT_ADDR                  ((0x0254  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_E_STATUS1                   ((0x0255  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_E_STATUS2                   ((0x0256  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_E_START_ADDRB               ((0x0257  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_E_FINISH_ADDRB              ((0x0258  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_E_INIT_ADDR                 ((0x0259  << 2) + 0xfe050000)
#define EE_AUDIO_FRDDR_E_CTRL2                     ((0x025a  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_SWAP1                     ((0x0260  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MASK4                     ((0x0261  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MASK5                     ((0x0262  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MASK6                     ((0x0263  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MASK7                     ((0x0264  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MUTE4                     ((0x0265  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MUTE5                     ((0x0266  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MUTE6                     ((0x0267  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_A_MUTE7                     ((0x0268  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_SWAP1                     ((0x0270  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MASK4                     ((0x0271  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MASK5                     ((0x0272  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MASK6                     ((0x0273  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MASK7                     ((0x0274  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MUTE4                     ((0x0275  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MUTE5                     ((0x0276  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MUTE6                     ((0x0277  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_B_MUTE7                     ((0x0278  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_SWAP1                     ((0x0280  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MASK4                     ((0x0281  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MASK5                     ((0x0282  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MASK6                     ((0x0283  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MASK7                     ((0x0284  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MUTE4                     ((0x0285  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MUTE5                     ((0x0286  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MUTE6                     ((0x0287  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_C_MUTE7                     ((0x0288  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_SWAP1                    ((0x0290  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MASK4                    ((0x0291  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MASK5                    ((0x0292  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MASK6                    ((0x0293  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MASK7                    ((0x0294  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MUTE4                    ((0x0295  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MUTE5                    ((0x0296  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MUTE6                    ((0x0297  << 2) + 0xfe050000)
#define EE_AUDIO_TDMIN_LB_MUTE7                    ((0x0298  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_CTRL2                    ((0x02a0  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_SWAP1                    ((0x02a1  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_GAIN2                    ((0x02a2  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_GAIN3                    ((0x02a3  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MASK4                    ((0x02a4  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MASK5                    ((0x02a5  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MASK6                    ((0x02a6  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MASK7                    ((0x02a7  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MUTE4                    ((0x02a8  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MUTE5                    ((0x02a9  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MUTE6                    ((0x02aa  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_A_MUTE7                    ((0x02ab  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_CTRL2                    ((0x02b0  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_SWAP1                    ((0x02b1  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_GAIN2                    ((0x02b2  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_GAIN3                    ((0x02b3  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MASK4                    ((0x02b4  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MASK5                    ((0x02b5  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MASK6                    ((0x02b6  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MASK7                    ((0x02b7  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MUTE4                    ((0x02b8  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MUTE5                    ((0x02b9  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MUTE6                    ((0x02ba  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_B_MUTE7                    ((0x02bb  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_CTRL2                    ((0x02c0  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_SWAP1                    ((0x02c1  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_GAIN2                    ((0x02c2  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_GAIN3                    ((0x02c3  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MASK4                    ((0x02c4  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MASK5                    ((0x02c5  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MASK6                    ((0x02c6  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MASK7                    ((0x02c7  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MUTE4                    ((0x02c8  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MUTE5                    ((0x02c9  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MUTE6                    ((0x02ca  << 2) + 0xfe050000)
#define EE_AUDIO_TDMOUT_C_MUTE7                    ((0x02cb  << 2) + 0xfe050000)
//========================================================================
//  PDM - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe051000
// -----------------------------------------------
#define PDM_CTRL                                   ((0x0000  << 2) + 0xfe051000)
  //bit 31:   PDM enable.
  //bit 30:   invert the PDM_DCLK.
  //bit 29:   output mode:  1: 24bits. 0: 32 bits.
  //bit 28:   bypass mode. 1: bypass all filter. directly output the PDM input to DDR. 0: normal mode.
  //bit 27~9: not used.
  //bit 16:.   PDM Asynchronous FIFO soft reset.  write 1 to soft reset AFIFO.
  //bit 15:8   PDM channel reset.  0: to reset each PDM channel.  1: normal mode.
  //bit 7:0.  PDM channel enable. each bit for one channel.
#define PDM_HCIC_CTRL1                             ((0x0001  << 2) + 0xfe051000)
  //bit 31      hcic filter enable.  1 use sinc filter. 0 bypass input to output.
  //bit 29:24.  hcic final gain shift parameter.
  //bit 23:16   hcic final gain multiplier.
  //bit 8:4     hcic  down sample rate.
  //bit 3:0     hcic  stage number. must be between 3 to 9.
#define PDM_HCIC_CTRL2                             ((0x0002  << 2) + 0xfe051000)
  //Not used.
#define PDM_F1_CTRL                                ((0x0003  << 2) + 0xfe051000)
  //bit 31 .   filter 1 enable.
  //bit 16:15. f1 round mode.  2'b00 : sign bit at bit 49.  28bits output [49:22] round at bit 21. 32bits output [49:18]. 24bits output [49:26]
         //                    2'b01 : sign bit at bit 50.  28bits output [50:23] round at bit 22. 32bits output [49:18]. 24bits output [49:26]
         //                    2'b10 : sign bit at bit 51.  28bits output [51:24] round at bit 23 32bits output [49:18]. 24bits output [49:26].
  //bit 15:12. filter 1 down sample rate.
  //bit 8:0.   filter 1 stage number.
#define PDM_F2_CTRL                                ((0x0004  << 2) + 0xfe051000)
  //bit 31 .   filter 2 enable.
  //bit 16:15. f2 round mode.  2'b00 : round at bit 21. 2'b01 : round at bit 22.  2'b10 : round at bit 23.
  //bit 15:12. filter 2 down sample rate.
  //bit 8:0.   filter 2 stage number.
#define PDM_F3_CTRL                                ((0x0005  << 2) + 0xfe051000)
  //bit 31 .   filter 3 enable.
  //bit 16:15. f3 round mode.  2'b00 : round at bit 21. 2'b01 : round at bit 22.  2'b10 : round at bit 23.
  //bit 15:12. filter 3 down sample rate.
  //bit 8:0.   filter 3 stage number.
#define PDM_HPF_CTRL                               ((0x0006  << 2) + 0xfe051000)
  //bit 31  High pass filter enable.
  //bit 20:16 high pass filter shift steps. 6~19 steps.
  //bit 15:0 high pass filter output factor.
#define PDM_CHAN_CTRL                              ((0x0007  << 2) + 0xfe051000)
  //bit 31:24.  chan3 data sample pointer vs edge of the PDM_DCLK.
  //bit 23:16   chan2 data sample pointer vs edge of the PDM_DCLK.
  //bit 15:8.   chan1 data sample pointer vs edge of the PDM_DCLK.
  //bit 7:0     chan0 data sample pointer vs edge of the PDM_DCLK.
#define PDM_CHAN_CTRL1                             ((0x0008  << 2) + 0xfe051000)
  //bit 31:24.  chan7 data sample pointer vs edge of the PDM_DCLK.
  //bit 23:16   chan6 data sample pointer vs edge of the PDM_DCLK.
  //bit 15:8.   chan5 data sample pointer vs edge of the PDM_DCLK.
  //bit 7:0     chan4 data sample pointer vs edge of the PDM_DCLK.
#define PDM_COEFF_ADDR                             ((0x0009  << 2) + 0xfe051000)
  // address of the write/read of coeff data.
#define PDM_COEFF_DATA                             ((0x000a  << 2) + 0xfe051000)
  //write/read data to coeff memory.
#define PDM_CLKG_CTRL                              ((0x000b  << 2) + 0xfe051000)
  // auto clock gating control.  1: disable the clock gating function. the clock will awlays enabled. 0 : use RTL auto clock gating.
  //31:7 not used.
  //bit 6  filt_ctrl module auto clock gating control.
  //bit 5  sinc fifo module auto clock gating control.
  //bit 4  filter module auto clock gating control.
  //bit 3  apb module auto clock gating control.
  //bit 2  coeff memory module auto clock gating control.
  //bit 1  each channel module auto clock gating control.
  //bit 0 cts_pdm_clk   auto clock gating control.
#define PDM_STS                                    ((0x000c  << 2) + 0xfe051000)
//bit 1  HPF filter output overflow.  means the PCLK is too slow.
//bit 0  HCIC filter output overflow. means the CTS_PDM_CLK is too slow. can't finished the filter function.
#define PDM_MUTE_VALUE                             ((0x000d  << 2) + 0xfe051000)
#define PDM_MASK_NUM                               ((0x000e  << 2) + 0xfe051000)
//========================================================================
//  EQ DRC - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe052000
// -----------------------------------------------
#define AED_COEF_RAM_CNTL                          ((0x0000  << 2) + 0xfe052000)
#define AED_COEF_RAM_DATA                          ((0x0001  << 2) + 0xfe052000)
#define AED_EQ_EN                                  ((0x0002  << 2) + 0xfe052000)
#define AED_EQ_TAP_CNTL                            ((0x0003  << 2) + 0xfe052000)
#define AED_EQ_VOLUME                              ((0x0004  << 2) + 0xfe052000)
#define AED_EQ_VOLUME_SLEW_CNT                     ((0x0005  << 2) + 0xfe052000)
#define AED_MUTE                                   ((0x0006  << 2) + 0xfe052000)
#define AED_DRC_CNTL                               ((0x0007  << 2) + 0xfe052000)
#define AED_DRC_RMS_COEF0                          ((0x0008  << 2) + 0xfe052000)
#define AED_DRC_RMS_COEF1                          ((0x0009  << 2) + 0xfe052000)
#define AED_DRC_THD0                               ((0x000a  << 2) + 0xfe052000)
#define AED_DRC_THD1                               ((0x000b  << 2) + 0xfe052000)
#define AED_DRC_THD2                               ((0x000c  << 2) + 0xfe052000)
#define AED_DRC_THD3                               ((0x000d  << 2) + 0xfe052000)
#define AED_DRC_THD4                               ((0x000e  << 2) + 0xfe052000)
#define AED_DRC_K0                                 ((0x000f  << 2) + 0xfe052000)
#define AED_DRC_K1                                 ((0x0010  << 2) + 0xfe052000)
#define AED_DRC_K2                                 ((0x0011  << 2) + 0xfe052000)
#define AED_DRC_K3                                 ((0x0012  << 2) + 0xfe052000)
#define AED_DRC_K4                                 ((0x0013  << 2) + 0xfe052000)
#define AED_DRC_K5                                 ((0x0014  << 2) + 0xfe052000)
#define AED_DRC_THD_OUT0                           ((0x0015  << 2) + 0xfe052000)
#define AED_DRC_THD_OUT1                           ((0x0016  << 2) + 0xfe052000)
#define AED_DRC_THD_OUT2                           ((0x0017  << 2) + 0xfe052000)
#define AED_DRC_THD_OUT3                           ((0x0018  << 2) + 0xfe052000)
#define AED_DRC_OFFSET                             ((0x0019  << 2) + 0xfe052000)
#define AED_DRC_RELEASE_COEF00                     ((0x001a  << 2) + 0xfe052000)
#define AED_DRC_RELEASE_COEF01                     ((0x001b  << 2) + 0xfe052000)
#define AED_DRC_RELEASE_COEF10                     ((0x001c  << 2) + 0xfe052000)
#define AED_DRC_RELEASE_COEF11                     ((0x001d  << 2) + 0xfe052000)
#define AED_DRC_RELEASE_COEF20                     ((0x001e  << 2) + 0xfe052000)
#define AED_DRC_RELEASE_COEF21                     ((0x001f  << 2) + 0xfe052000)
#define AED_DRC_RELEASE_COEF30                     ((0x0020  << 2) + 0xfe052000)
#define AED_DRC_RELEASE_COEF31                     ((0x0021  << 2) + 0xfe052000)
#define AED_DRC_RELEASE_COEF40                     ((0x0022  << 2) + 0xfe052000)
#define AED_DRC_RELEASE_COEF41                     ((0x0023  << 2) + 0xfe052000)
#define AED_DRC_RELEASE_COEF50                     ((0x0024  << 2) + 0xfe052000)
#define AED_DRC_RELEASE_COEF51                     ((0x0025  << 2) + 0xfe052000)
#define AED_DRC_ATTACK_COEF00                      ((0x0026  << 2) + 0xfe052000)
#define AED_DRC_ATTACK_COEF01                      ((0x0027  << 2) + 0xfe052000)
#define AED_DRC_ATTACK_COEF10                      ((0x0028  << 2) + 0xfe052000)
#define AED_DRC_ATTACK_COEF11                      ((0x0029  << 2) + 0xfe052000)
#define AED_DRC_ATTACK_COEF20                      ((0x002a  << 2) + 0xfe052000)
#define AED_DRC_ATTACK_COEF21                      ((0x002b  << 2) + 0xfe052000)
#define AED_DRC_ATTACK_COEF30                      ((0x002c  << 2) + 0xfe052000)
#define AED_DRC_ATTACK_COEF31                      ((0x002d  << 2) + 0xfe052000)
#define AED_DRC_ATTACK_COEF40                      ((0x002e  << 2) + 0xfe052000)
#define AED_DRC_ATTACK_COEF41                      ((0x002f  << 2) + 0xfe052000)
#define AED_DRC_ATTACK_COEF50                      ((0x0030  << 2) + 0xfe052000)
#define AED_DRC_ATTACK_COEF51                      ((0x0031  << 2) + 0xfe052000)
#define AED_DRC_LOOPBACK_CNTL                      ((0x0032  << 2) + 0xfe052000)
#define AED_MDRC_CNTL                              ((0x0033  << 2) + 0xfe052000)
#define AED_MDRC_RMS_COEF00                        ((0x0034  << 2) + 0xfe052000)
#define AED_MDRC_RMS_COEF01                        ((0x0035  << 2) + 0xfe052000)
#define AED_MDRC_RELEASE_COEF00                    ((0x0036  << 2) + 0xfe052000)
#define AED_MDRC_RELEASE_COEF01                    ((0x0037  << 2) + 0xfe052000)
#define AED_MDRC_ATTACK_COEF00                     ((0x0038  << 2) + 0xfe052000)
#define AED_MDRC_ATTACK_COEF01                     ((0x0039  << 2) + 0xfe052000)
#define AED_MDRC_THD0                              ((0x003a  << 2) + 0xfe052000)
#define AED_MDRC_K0                                ((0x003b  << 2) + 0xfe052000)
#define AED_MDRC_LOW_GAIN                          ((0x003c  << 2) + 0xfe052000)
#define AED_MDRC_OFFSET0                           ((0x003d  << 2) + 0xfe052000)
#define AED_MDRC_RMS_COEF10                        ((0x003e  << 2) + 0xfe052000)
#define AED_MDRC_RMS_COEF11                        ((0x003f  << 2) + 0xfe052000)
#define AED_MDRC_RELEASE_COEF10                    ((0x0040  << 2) + 0xfe052000)
#define AED_MDRC_RELEASE_COEF11                    ((0x0041  << 2) + 0xfe052000)
#define AED_MDRC_ATTACK_COEF10                     ((0x0042  << 2) + 0xfe052000)
#define AED_MDRC_ATTACK_COEF11                     ((0x0043  << 2) + 0xfe052000)
#define AED_MDRC_THD1                              ((0x0044  << 2) + 0xfe052000)
#define AED_MDRC_K1                                ((0x0045  << 2) + 0xfe052000)
#define AED_MDRC_OFFSET1                           ((0x0046  << 2) + 0xfe052000)
#define AED_MDRC_MID_GAIN                          ((0x0047  << 2) + 0xfe052000)
#define AED_MDRC_RMS_COEF20                        ((0x0048  << 2) + 0xfe052000)
#define AED_MDRC_RMS_COEF21                        ((0x0049  << 2) + 0xfe052000)
#define AED_MDRC_RELEASE_COEF20                    ((0x004a  << 2) + 0xfe052000)
#define AED_MDRC_RELEASE_COEF21                    ((0x004b  << 2) + 0xfe052000)
#define AED_MDRC_ATTACK_COEF20                     ((0x004c  << 2) + 0xfe052000)
#define AED_MDRC_ATTACK_COEF21                     ((0x004d  << 2) + 0xfe052000)
#define AED_MDRC_THD2                              ((0x004e  << 2) + 0xfe052000)
#define AED_MDRC_K2                                ((0x004f  << 2) + 0xfe052000)
#define AED_MDRC_OFFSET2                           ((0x0050  << 2) + 0xfe052000)
#define AED_MDRC_HIGH_GAIN                         ((0x0051  << 2) + 0xfe052000)
#define AED_ED_CNTL                                ((0x0052  << 2) + 0xfe052000)
#define AED_DC_EN                                  ((0x0053  << 2) + 0xfe052000)
#define AED_ND_LOW_THD                             ((0x0054  << 2) + 0xfe052000)
#define AED_ND_HIGH_THD                            ((0x0055  << 2) + 0xfe052000)
#define AED_ND_CNT_THD                             ((0x0056  << 2) + 0xfe052000)
#define AED_ND_SUM_NUM                             ((0x0057  << 2) + 0xfe052000)
#define AED_ND_CZ_NUM                              ((0x0058  << 2) + 0xfe052000)
#define AED_ND_SUM_THD0                            ((0x0059  << 2) + 0xfe052000)
#define AED_ND_SUM_THD1                            ((0x005a  << 2) + 0xfe052000)
#define AED_ND_CZ_THD0                             ((0x005b  << 2) + 0xfe052000)
#define AED_ND_CZ_THD1                             ((0x005c  << 2) + 0xfe052000)
#define AED_ND_COND_CNTL                           ((0x005d  << 2) + 0xfe052000)
#define AED_ND_RELEASE_COEF0                       ((0x005e  << 2) + 0xfe052000)
#define AED_ND_RELEASE_COEF1                       ((0x005f  << 2) + 0xfe052000)
#define AED_ND_ATTACK_COEF0                        ((0x0060  << 2) + 0xfe052000)
#define AED_ND_ATTACK_COEF1                        ((0x0061  << 2) + 0xfe052000)
#define AED_ND_CNTL                                ((0x0062  << 2) + 0xfe052000)
#define AED_MIX0_LL                                ((0x0063  << 2) + 0xfe052000)
#define AED_MIX0_RL                                ((0x0064  << 2) + 0xfe052000)
#define AED_MIX0_LR                                ((0x0065  << 2) + 0xfe052000)
#define AED_MIX0_RR                                ((0x0066  << 2) + 0xfe052000)
#define AED_CLIP_THD                               ((0x0067  << 2) + 0xfe052000)
#define AED_CH1_ND_SUM_OUT                         ((0x0068  << 2) + 0xfe052000)
#define AED_CH2_ND_SUM_OUT                         ((0x0069  << 2) + 0xfe052000)
#define AED_CH1_ND_CZ_OUT                          ((0x006a  << 2) + 0xfe052000)
#define AED_CH2_ND_CZ_OUT                          ((0x006b  << 2) + 0xfe052000)
#define AED_NOISE_STATUS                           ((0x006c  << 2) + 0xfe052000)
#define AED_POW_CURRENT_S0                         ((0x006d  << 2) + 0xfe052000)
#define AED_POW_CURRENT_S1                         ((0x006e  << 2) + 0xfe052000)
#define AED_POW_CURRENT_S2                         ((0x006f  << 2) + 0xfe052000)
#define AED_POW_OUT0                               ((0x0070  << 2) + 0xfe052000)
#define AED_POW_OUT1                               ((0x0071  << 2) + 0xfe052000)
#define AED_POW_OUT2                               ((0x0072  << 2) + 0xfe052000)
#define AED_POW_ADJ_INDEX0                         ((0x0073  << 2) + 0xfe052000)
#define AED_POW_ADJ_INDEX1                         ((0x0074  << 2) + 0xfe052000)
#define AED_POW_ADJ_INDEX2                         ((0x0075  << 2) + 0xfe052000)
#define AED_DRC_GAIN_INDEX0                        ((0x0076  << 2) + 0xfe052000)
#define AED_DRC_GAIN_INDEX1                        ((0x0077  << 2) + 0xfe052000)
#define AED_DRC_GAIN_INDEX2                        ((0x0078  << 2) + 0xfe052000)
#define AED_CH1_VOLUME_STATE                       ((0x0079  << 2) + 0xfe052000)
#define AED_CH2_VOLUME_STATE                       ((0x007a  << 2) + 0xfe052000)
#define AED_CH1_VOLUME_GAIN                        ((0x007b  << 2) + 0xfe052000)
#define AED_CH2_VOLUME_GAIN                        ((0x007c  << 2) + 0xfe052000)
#define AED_FULL_POW_CURRENT                       ((0x007d  << 2) + 0xfe052000)
#define AED_FULL_POW_OUT                           ((0x007e  << 2) + 0xfe052000)
#define AED_FULL_POW_ADJ                           ((0x007f  << 2) + 0xfe052000)
#define AED_FULL_DRC_GAIN                          ((0x0080  << 2) + 0xfe052000)
#define AED_MASTER_VOLUME_STATE                    ((0x0081  << 2) + 0xfe052000)
#define AED_MASTER_VOLUME_GAIN                     ((0x0082  << 2) + 0xfe052000)
#define AED_TOP_CTL0                               ((0x0083  << 2) + 0xfe052000)
#define AED_TOP_CTL1                               ((0x0084  << 2) + 0xfe052000)
#define AED_TOP_CTL2                               ((0x0085  << 2) + 0xfe052000)
//========================================================================
//  AUDIO locker - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe051400
// -----------------------------------------------
#define AUD_LOCK_EN                                ((0x0000  << 2) + 0xfe051400)
#define AUD_LOCK_SW_RESET                          ((0x0001  << 2) + 0xfe051400)
#define AUD_LOCK_SW_LATCH                          ((0x0002  << 2) + 0xfe051400)
#define AUD_LOCK_HW_LATCH                          ((0x0003  << 2) + 0xfe051400)
#define AUD_LOCK_REFCLK_SRC                        ((0x0004  << 2) + 0xfe051400)
#define AUD_LOCK_REFCLK_LAT_INT                    ((0x0005  << 2) + 0xfe051400)
#define AUD_LOCK_IMCLK_LAT_INT                     ((0x0006  << 2) + 0xfe051400)
#define AUD_LOCK_OMCLK_LAT_INT                     ((0x0007  << 2) + 0xfe051400)
#define AUD_LOCK_REFCLK_DS_INT                     ((0x0008  << 2) + 0xfe051400)
#define AUD_LOCK_IMCLK_DS_INT                      ((0x0009  << 2) + 0xfe051400)
#define AUD_LOCK_OMCLK_DS_INT                      ((0x000a  << 2) + 0xfe051400)
#define AUD_LOCK_INT_CLR                           ((0x000b  << 2) + 0xfe051400)
#define AUD_LOCK_GCLK_CTRL                         ((0x000c  << 2) + 0xfe051400)
#define AUD_LOCK_INT_CTRL                          ((0x000d  << 2) + 0xfe051400)
#define RO_REF2IMCLK_CNT_L                         ((0x0010  << 2) + 0xfe051400)
#define RO_REF2IMCLK_CNT_H                         ((0x0011  << 2) + 0xfe051400)
#define RO_REF2OMCLK_CNT_L                         ((0x0012  << 2) + 0xfe051400)
#define RO_REF2OMCLK_CNT_H                         ((0x0013  << 2) + 0xfe051400)
#define RO_IMCLK2REF_CNT_L                         ((0x0014  << 2) + 0xfe051400)
#define RO_IMCLK2REF_CNT_H                         ((0x0015  << 2) + 0xfe051400)
#define RO_OMCLK2REF_CNT_L                         ((0x0016  << 2) + 0xfe051400)
#define RO_OMCLK2REF_CNT_H                         ((0x0017  << 2) + 0xfe051400)
#define RO_REFCLK_PKG_CNT                          ((0x0018  << 2) + 0xfe051400)
#define RO_IMCLK_PKG_CNT                           ((0x0019  << 2) + 0xfe051400)
#define RO_OMCLK_PKG_CNT                           ((0x001a  << 2) + 0xfe051400)
#define RO_AUD_LOCK_INT_STATUS                     ((0x001b  << 2) + 0xfe051400)
//========================================================================
//  AUDIO VAD - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe051800
// -----------------------------------------------
//
// Reading file:  VAD_REG.h
//
#define VAD_TOP_CTRL0                              ((0x0000  << 2) + 0xfe051800)
#define VAD_TOP_CTRL1                              ((0x0001  << 2) + 0xfe051800)
#define VAD_TOP_CTRL2                              ((0x0002  << 2) + 0xfe051800)
#define VAD_FIR_CTRL                               ((0x0003  << 2) + 0xfe051800)
#define VAD_FIR_EMP                                ((0x0004  << 2) + 0xfe051800)
#define VAD_FIR_COEF0                              ((0x0005  << 2) + 0xfe051800)
#define VAD_FIR_COEF1                              ((0x0006  << 2) + 0xfe051800)
#define VAD_FIR_COEF2                              ((0x0007  << 2) + 0xfe051800)
#define VAD_FIR_COEF3                              ((0x0008  << 2) + 0xfe051800)
#define VAD_FIR_COEF4                              ((0x0009  << 2) + 0xfe051800)
#define VAD_FIR_COEF5                              ((0x000a  << 2) + 0xfe051800)
#define VAD_FIR_COEF6                              ((0x000b  << 2) + 0xfe051800)
#define VAD_FIR_COEF7                              ((0x000c  << 2) + 0xfe051800)
#define VAD_FIR_COEF8                              ((0x000d  << 2) + 0xfe051800)
#define VAD_FIR_COEF9                              ((0x000e  << 2) + 0xfe051800)
#define VAD_FIR_COEF10                             ((0x000f  << 2) + 0xfe051800)
#define VAD_FIR_COEF11                             ((0x0010  << 2) + 0xfe051800)
#define VAD_FIR_COEF12                             ((0x0011  << 2) + 0xfe051800)
#define VAD_FRAME_CTRL0                            ((0x0012  << 2) + 0xfe051800)
#define VAD_FRAME_CTRL1                            ((0x0013  << 2) + 0xfe051800)
#define VAD_FRAME_CTRL2                            ((0x0014  << 2) + 0xfe051800)
#define VAD_CEP_CTRL0                              ((0x0015  << 2) + 0xfe051800)
#define VAD_CEP_CTRL1                              ((0x0016  << 2) + 0xfe051800)
#define VAD_CEP_CTRL2                              ((0x0017  << 2) + 0xfe051800)
#define VAD_CEP_CTRL3                              ((0x0018  << 2) + 0xfe051800)
#define VAD_CEP_CTRL4                              ((0x0019  << 2) + 0xfe051800)
#define VAD_CEP_CTRL5                              ((0x001a  << 2) + 0xfe051800)
#define VAD_DEC_CTRL                               ((0x001b  << 2) + 0xfe051800)
#define VAD_TOP_STS0                               ((0x001c  << 2) + 0xfe051800)
#define VAD_TOP_STS1                               ((0x001d  << 2) + 0xfe051800)
#define VAD_TOP_STS2                               ((0x001e  << 2) + 0xfe051800)
#define VAD_FIR_STS0                               ((0x001f  << 2) + 0xfe051800)
#define VAD_FIR_STS1                               ((0x0020  << 2) + 0xfe051800)
#define VAD_POW_STS0                               ((0x0021  << 2) + 0xfe051800)
#define VAD_POW_STS1                               ((0x0022  << 2) + 0xfe051800)
#define VAD_POW_STS2                               ((0x0023  << 2) + 0xfe051800)
#define VAD_FFT_STS0                               ((0x0024  << 2) + 0xfe051800)
#define VAD_FFT_STS1                               ((0x0025  << 2) + 0xfe051800)
#define VAD_SPE_STS0                               ((0x0026  << 2) + 0xfe051800)
#define VAD_SPE_STS1                               ((0x0027  << 2) + 0xfe051800)
#define VAD_SPE_STS2                               ((0x0028  << 2) + 0xfe051800)
#define VAD_SPE_STS3                               ((0x0029  << 2) + 0xfe051800)
#define VAD_DEC_STS0                               ((0x002a  << 2) + 0xfe051800)
#define VAD_DEC_STS1                               ((0x002b  << 2) + 0xfe051800)
#define VAD_LUT_CTRL                               ((0x002c  << 2) + 0xfe051800)
#define VAD_LUT_WR                                 ((0x002d  << 2) + 0xfe051800)
#define VAD_LUT_RD                                 ((0x002e  << 2) + 0xfe051800)
#define VAD_IN_SEL0                                ((0x002f  << 2) + 0xfe051800)
#define VAD_IN_SEL1                                ((0x0030  << 2) + 0xfe051800)
#define VAD_TO_DDR                                 ((0x0031  << 2) + 0xfe051800)
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
#define AUDIO_RSAMP_CTRL0                          ((0x0000  << 2) + 0xfe051c00)
//Bit   31:3      reserved
//Bit   2         reg_lock_rst      //unsigned  , default =0;
//Bit   1         reg_rsamp_rst     //unsigned  , default =0;
//Bit   0         reg_sw_rst        //unsigned  , default =0;
#define AUDIO_RSAMP_CTRL1                          ((0x0001  << 2) + 0xfe051c00)
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
#define AUDIO_RSAMP_CTRL2                          ((0x0002  << 2) + 0xfe051c00)
//Bit 31:30    reserved              //unsigned  , default =0;
//Bit 29:24    reg_chx_size          //unsigned  , default =2;
//Bit 23:18    reserved              //unsigned  , default =0;
//Bit 17:16    reg_scl_step          //unsigned  , default =0; 0: 1/1  1: 1/2  2: 1/4
//Bit 15:8     reg_filt_tap          //unsigned  , default =63;
//Bit 7:0      reg_intp_tap          //unsigned  , default =63;
#define AUDIO_RSAMP_PHSINIT                        ((0x0003  << 2) + 0xfe051c00)
//Bit   31:28      reserved          //unsigned  , default = 0;
//Bit   27:0       reg_init_phs      //unsigned  , default = 0;
#define AUDIO_RSAMP_PHSSTEP                        ((0x0004  << 2) + 0xfe051c00)
//Bit   31         reserved          //unsigned  , default = 0;
//Bit   30:0       reg_rsamp_step    //unsigned  , default = 134217728;//'h800_0000
#define AUDIO_RSAMP_SHIFT                          ((0x0005  << 2) + 0xfe051c00)
//Bit   31:24       reg_rsft_iir    //unsigned  , default = 23;
//Bit   23:16       reg_rsft_blnd   //unsigned  , default = 21;
//Bit   15:8        reg_rsft_sinc   //unsigned  , default = 31;
//Bit   7:0         reg_rsft_aa     //unsigned  , default = 31;
#define AUDIO_RSAMP_ADJ_CTRL0                      ((0x0006  << 2) + 0xfe051c00)
//Bit   31:3        reserved                //unsigned
//Bit   2           reg_rsamp_adj_out_inv   //unsigned , default = 0;
//Bit   1           reg_rsamp_adj_force_en  //unsigned , default = 0;
//Bit   0           reg_rsamp_adj_en        //unsigned , default = 0;
#define AUDIO_RSAMP_ADJ_CTRL1                      ((0x0007  << 2) + 0xfe051c00)
//Bit   31:16       reg_rsamp_adj_odet_step     //unsigned , default = 8;
//Bit   15:0        reg_rsamp_adj_kmax          //unsigned , default = 32768;
#define AUDIO_RSAMP_ADJ_SFT                        ((0x0008  << 2) + 0xfe051c00)
//Bit   31:30       reserved                //unsigned , default = 0;
//Bit   29          reg_rsamp_adj_dif_sel   //unsigned , default = 0;
//Bit   28:24       reg_rsamp_adj_ki        //unsigned , default = 9;
//Bit   23:21       reserved                //unsigned , default = 0;
//Bit   20:16       reg_rsamp_adj_kp        //unsigned , default = 1;
//Bit   15:13       reserved                //unsigned , default = 0;
//Bit   12:8        reg_rsamp_adj_ki_sft    //unsigned , default = 6;
//Bit   7:6         reserved                //unsigned , default = 0;
//Bit   5:0         reg_rsamp_adj_out_sft   //unsigned , default = 12;
#define AUDIO_RSAMP_ADJ_IDET_LEN                   ((0x0009  << 2) + 0xfe051c00)
//Bit   31:0       reg_rsamp_adj_idet_len       //unsigned , default = 10000;
#define AUDIO_RSAMP_ADJ_FORCE                      ((0x000a  << 2) + 0xfe051c00)
//Bit   31:0       reg_rsamp_adj_force_err      //signed , default = 8;
#define AUDIO_RSAMP_ADJ_KI_FORCE                   ((0x000b  << 2) + 0xfe051c00)
//Bit   31:0       reg_rsamp_adj_ki_force //signed , default = 0;
#define AUDIO_RSAMP_RO_STATUS                      ((0x0010  << 2) + 0xfe051c00)
//Bit   31:0       ro_rsamp_stat  //{din_chx_chk_err,is_idle_st,rsamp_fifo_over_cnt[7:0]}
#define AUDIO_RSAMP_RO_ADJ_FREQ                    ((0x0011  << 2) + 0xfe051c00)
//Bit   31:0       ro_rsamp_adj_freq
#define AUDIO_RSAMP_RO_ADJ_DIFF_BAK                ((0x0012  << 2) + 0xfe051c00)
//Bit   31:0       ro_det_diff_bak
#define AUDIO_RSAMP_RO_ADJ_DIFF_DLT                ((0x0013  << 2) + 0xfe051c00)
//Bit   31:0       ro_det_diff_dlt
#define AUDIO_RSAMP_RO_ADJ_PHS_ERR                 ((0x0014  << 2) + 0xfe051c00)
//Bit   31:0       ro_det_phase_err
#define AUDIO_RSAMP_RO_ADJ_KI_OUT                  ((0x0015  << 2) + 0xfe051c00)
//Bit   31:0       ro_rsamp_ki_out
#define AUDIO_RSAMP_RO_IN_CNT                      ((0x0016  << 2) + 0xfe051c00)
//Bit   31:0       ro_rsamp_in_cnt
#define AUDIO_RSAMP_RO_OUT_CNT                     ((0x0017  << 2) + 0xfe051c00)
//Bit   31:0       ro_rsamp_out_cnt
#define AUDIO_RSAMP_POST_COEF0                     ((0x0020  << 2) + 0xfe051c00)
//Bit   31:0       reg_post_coef0 //signed  , default = 0;
#define AUDIO_RSAMP_POST_COEF1                     ((0x0021  << 2) + 0xfe051c00)
//Bit   31:0       reg_post_coef1 //signed  , default = 0;
#define AUDIO_RSAMP_POST_COEF2                     ((0x0022  << 2) + 0xfe051c00)
//Bit   31:0       reg_post_coef2 //signed  , default = 0;
#define AUDIO_RSAMP_POST_COEF3                     ((0x0023  << 2) + 0xfe051c00)
//Bit   31:0       reg_post_coef3 //signed  , default = 0;
#define AUDIO_RSAMP_POST_COEF4                     ((0x0024  << 2) + 0xfe051c00)
//Bit   31:0       reg_post_coef4 //signed  , default = 0;
#define AUDIO_RSAMP_AA_COEF_ADDR                   ((0x0030  << 2) + 0xfe051c00)
//Bit   31:0       reg_aa_coef_addr     //unsigned, default = 0;
#define AUDIO_RSAMP_AA_COEF_DATA                   ((0x0031  << 2) + 0xfe051c00)
//Bit   31:0       reg_aa_coef_data     //signed  , default = 0;
#define AUDIO_RSAMP_SINC_COEF_ADDR                 ((0x0040  << 2) + 0xfe051c00)
//Bit   31:0       reg_sinc_coef_addr   //unsigned, default = 0;
#define AUDIO_RSAMP_SINC_COEF_DATA                 ((0x0041  << 2) + 0xfe051c00)
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
#define EE_AUDIO2_CLK81_CTRL                       ((0x0000  << 2) + 0xfe054800)
#define EE_AUDIO2_CLK81_EN                         ((0x0001  << 2) + 0xfe054800)
#define EE_AUDIO2_SW_RESET0                        ((0x0002  << 2) + 0xfe054800)
#define EE_AUDIO2_CLK_GATE_EN0                     ((0x0003  << 2) + 0xfe054800)
#define EE_AUDIO2_SECURITY_CTRL0                   ((0x0004  << 2) + 0xfe054800)
#define EE_AUDIO2_MCLK_VAD_CTRL                    ((0x0010  << 2) + 0xfe054800)
#define EE_AUDIO2_VAD_CLK_CTRL                     ((0x0011  << 2) + 0xfe054800)
#define EE_AUDIO2_MST_DLY_CTRL0                    ((0x0012  << 2) + 0xfe054800)
#define EE_AUDIO2_MST_VAD_SCLK_CTRL0               ((0x0013  << 2) + 0xfe054800)
#define EE_AUDIO2_MST_VAD_SCLK_CTRL1               ((0x0014  << 2) + 0xfe054800)
#define EE_AUDIO2_CLK_TDMIN_VAD_CTRL               ((0x0015  << 2) + 0xfe054800)
#define EE_AUDIO2_CLK_PDMIN_CTRL0                  ((0x0016  << 2) + 0xfe054800)
#define EE_AUDIO2_CLK_PDMIN_CTRL1                  ((0x0017  << 2) + 0xfe054800)
#define EE_AUDIO2_TOVAD_CTRL0                      ((0x0020  << 2) + 0xfe054800)
#define EE_AUDIO2_TODDR_VAD_CTRL0                  ((0x0030  << 2) + 0xfe054800)
#define EE_AUDIO2_TODDR_VAD_CTRL1                  ((0x0031  << 2) + 0xfe054800)
#define EE_AUDIO2_TODDR_VAD_CTRL2                  ((0x0032  << 2) + 0xfe054800)
#define EE_AUDIO2_TODDR_VAD_START_ADDR             ((0x0033  << 2) + 0xfe054800)
#define EE_AUDIO2_TODDR_VAD_INIT_ADDR              ((0x0034  << 2) + 0xfe054800)
#define EE_AUDIO2_TODDR_VAD_FINISH_ADDR            ((0x0035  << 2) + 0xfe054800)
#define EE_AUDIO2_TODDR_VAD_START_ADDRB            ((0x0036  << 2) + 0xfe054800)
#define EE_AUDIO2_TODDR_VAD_FINISH_ADDRB           ((0x0037  << 2) + 0xfe054800)
#define EE_AUDIO2_TODDR_VAD_INT_ADDR               ((0x0038  << 2) + 0xfe054800)
#define EE_AUDIO2_TODDR_VAD_STATUS1                ((0x0039  << 2) + 0xfe054800)
#define EE_AUDIO2_TODDR_VAD_STATUS2                ((0x003a  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_CTRL                   ((0x0040  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_SWAP0                  ((0x0041  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_SWAP1                  ((0x0042  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MUTE_VAL               ((0x0043  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_STAT                   ((0x0044  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MUTE0                  ((0x0050  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MUTE1                  ((0x0051  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MUTE2                  ((0x0052  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MUTE3                  ((0x0053  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MUTE4                  ((0x0054  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MUTE5                  ((0x0055  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MUTE6                  ((0x0056  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MUTE7                  ((0x0057  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MASK0                  ((0x0058  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MASK1                  ((0x0059  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MASK2                  ((0x005a  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MASK3                  ((0x005b  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MASK4                  ((0x005c  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MASK5                  ((0x005d  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MASK6                  ((0x005e  << 2) + 0xfe054800)
#define EE_AUDIO2_TDMIN_VAD_MASK7                  ((0x005f  << 2) + 0xfe054800)
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
#define SPIFC_AHB_CTRL                             ((0x0000  << 2) + 0xfd000400)
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
#define SPIFC_CLK_CTRL                             ((0x0001  << 2) + 0xfd000400)
  //bit 14   asynchronous buffer ahb clock disable.  1 = disable. 0 = enable.
  //bit 13   ahb2spi ahb clock disable.  1 = disable. 0 = enable.
  //bit 12   ahb_arb ahb clock disable.  1 = disable. 0 = enable.
  //bit 10   asynchronous buffer ahb clock auto gating enable.  1 = enable. 0 = disable.
  //bit 9    ahb2spi ahb clock auto gating enable.  1 = enable. 0 = disable.
  //bit 8    ahbarb  ahb clock auto gating enable.  1 = enable. 0 = disable.
  //bit 2.    asynchronous AHB clock domain software reset.  1 = reset. 0 = normal working mode.
  //bit 1.    ahb2spi ahb clock domain software reset.  1 = reset. 0 = normal working mode.
  //bit 0.    not used.
#define SPIFC_SEC_CTRL                             ((0x0002  << 2) + 0xfd000400)
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
#define SPIFC_RANGE0_STA                           ((0x0010  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 0  start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE0_EDA                           ((0x0011  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 0  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE0_CTRL                          ((0x0012  << 2) + 0xfd000400)
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
#define SPIFC_RANGE1_STA                           ((0x0013  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 1  start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE1_EDA                           ((0x0014  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 1  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE1_CTRL                          ((0x0015  << 2) + 0xfd000400)
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
#define SPIFC_RANGE2_STA                           ((0x0016  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 2  start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE2_EDA                           ((0x0017  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 2  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE2_CTRL                          ((0x0018  << 2) + 0xfd000400)
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
#define SPIFC_RANGE3_STA                           ((0x0019  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 3  start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE3_EDA                           ((0x001a  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 3  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE3_CTRL                          ((0x001b  << 2) + 0xfd000400)
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
#define SPIFC_RANGE4_STA                           ((0x001c  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 4  start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE4_EDA                           ((0x001d  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 4  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE4_CTRL                          ((0x001e  << 2) + 0xfd000400)
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
#define SPIFC_RANGE5_STA                           ((0x001f  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 5 start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE5_EDA                           ((0x0021  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 5  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE5_CTRL                          ((0x0020  << 2) + 0xfd000400)
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
#define SPIFC_RANGE6_STA                           ((0x0022  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 6 start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE6_EDA                           ((0x0023  << 2) + 0xfd000400)
  //bit 28 :6.
  //AHB security range 6  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE6_CTRL                          ((0x0024  << 2) + 0xfd000400)
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
#define SPIFC_RANGE7_CTRL                          ((0x0025  << 2) + 0xfd000400)
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
#define SPIFC_AHB_WTCH_CTRL                        ((0x0026  << 2) + 0xfd000400)
  //bit 15:0.   ahb bus watch dog timer. if AHB bus not response, the timer over this number, the SPIFC will send back  a error response.
#define SPIFC_SEC_VIO0                             ((0x0027  << 2) + 0xfd000400)
  //bit 31:0  VIO ADDR
#define SPIFC_SEC_VIO1                             ((0x0028  << 2) + 0xfd000400)
  //bit  31.  AHB VIO status.  write 1 to clear the error info.
  //bit 6:5  HMASTER.
  //bit 4:2  HPROT.
  //bit 1    HNONSEC.
  //bit 0    HWRITE.
#define SPIFC_AHB_STS                              ((0x0029  << 2) + 0xfd000400)
  //bit 31.  AHB IDLE.  when ahb disabled, to show ahb status. 1,  no more ahb request or ahb frequent is holded.  0, ahb is still busy.
  //bit 30.  AHB data cycle status, when ahb disabled, to show ahb data cycle status. 1, data cycle is holded,  0 : not in data cycle or data cycle is processing.
  //bit 29.  AHB bus spi requst status. 1: still waiting for spi data. 0: idle.
  //bit 28:0.  not used.
//SPI cts_spi_clk domain regsiter.
#define SPIFC_USER_CTRL0                           ((0x0080  << 2) + 0xfd000400)
  //bit 31.    user request enable.  write 1 to enable.  read 0, means user command accepted by the SPI_CTRL.
  //bit 30.    user request finish bit.  write 0 when enable user request.  read  1 means SPI_CTRL finished this user command.
  //bit 0.     user data updated.  write 0 when enable user request.   1 means there's datas/status read from SPI flash.
#define SPIFC_USER_CTRL1                           ((0x0081  << 2) + 0xfd000400)
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
#define SPIFC_USER_CTRL2                           ((0x0082  << 2) + 0xfd000400)
  //bit 31 usr_dummy_en  1: enable dummy cycles.  0 : no dummy cycles.
  //bit 30: 29 usr_dummy_mode  2'b00 = SPI extend mode. 01 dual mode. 10 Quad mode.
  //bit 28:23  user dummy clock cycle number.
  //bit 19:16. user command input/output direction control.  000000: right after the last output.
  //          1: after the first cycle of dummy cycle.
  //          .....
  //          4'hf:  right before the datin cycle.
  //bit 15:8.  data on the dummy cycle after the first byte.
  //bit 7:0   The first bytes data showed on dummy cycle. some flash may need special data for enhance performance mode or XIP mode.
#define SPIFC_USER_CTRL3                           ((0x0083  << 2) + 0xfd000400)
  //bit 31 usr_datin_en  1: datain cycle enabled.  0: no datain cycle.
  //bit 30: datin_dest.  1: save the input data to STATUS register.  0 save the input data to data buffer.
  //bit 29: 1: enable datain data AES describe. 0: not use AES.
  //bit 28:27.  datin mode.  2'b00 SPI extend mode. 01: dual mode. 10 Quad mode.
  //bit 25:16.  user data in data bytes.  how many bytes of user datain expected.
#define SPIFC_USER_ADDR                            ((0x0084  << 2) + 0xfd000400)
  //bit 31:0.  32bits user address.
#define SPIFC_AHB_REQ_CTRL                         ((0x0085  << 2) + 0xfd000400)
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
#define SPIFC_AHB_REQ_CTRL1                        ((0x0086  << 2) + 0xfd000400)
  //bit 31.  ahb Dummy enable.
  //bit 30:29.  ahb dummy mode.
  //bit 28:23.  ahb request dummy clock cycles.
  //bit 15:0.   ahb dummy period output data.
#define SPIFC_AHB_REQ_CTRL2                        ((0x0087  << 2) + 0xfd000400)
  //bit 1:0  AHB address position. this setting depend on SPIFC_AHB_CTRL register CWF_EN bit and RDBUF_SIZE.
  // if CWF_EN is enabled. these bit should be set to 2'b10 (16BYTE boundary).
  // if CWF_EN is disabled. these bit should be set to same as the RDBUF_SIZE and AHB REQ DATA SIZE.
          //00 :  64byte boundary.
          //01 :  32byte boundary.
          //10 :  16byte boundary.
          //11 :  reserved.
#define SPIFC_ACTIMING0                            ((0x0088  << 2) + 0xfd000400)
  //bit 31:30.  tSLCH
  //bit 29:28   tCLSH
  //bit 20:16   tSHWL
  //bit 15:12   tSHSL2
  //bit 11:8    tSHSL1
  //bit 7:0     tWHSL
#define SPIFC_ACTIMING1                            ((0x0089  << 2) + 0xfd000400)
 //bit 31.  D2 pin WP_n function enable.
 //bit 30.  D2 Pin value in WP_n fucntion.
 //bit 29.  D3 Pin HOLD_n function enable.
 //bit 8.   DTR mode. not support.
 //bit 6:4. Clock turn around delay. use it to set the system SPI clock read data delay.
 //bit 3:0. not used.
#define SPIFC_ACTIMING2                            ((0x008a  << 2) + 0xfd000400)
 //bit 31.  spi clock input pin enable. SPIFC controller used this pin as clock to latch the input data.
 //bit [3:0]  spi clock input pin delay adjustment.
//SPI DATA BUFFER.  There's a total 512Byte + 64byte(organized as 36x128bits) SRAM .
// since each APB access is 32bits. So we use word address as APB read/write address.
// But for SPI side we have to use 128bit boundary. That's the first program/read SPI with data buffer must start at data buffer 128bit boundary.  that means the SPIFC_USE_DBUF_ADDR last 2 bits must be 0.
#define SPIFC_DBUF_CTRL                            ((0x0090  << 2) + 0xfd000400)
  //bit 31.    1 = write DBUF.  0 : read DBUF.
  //bit 30.    1. = auto update address. 0 don't change address.
  //bit 7:0   DBUF address unit 32bits.
#define SPIFC_DBUF_DATA                            ((0x0091  << 2) + 0xfd000400)
  //bit 31:0.   if SPIFC_DBUF_CTRL bit 31 = 1.  write to this register will trigger one write to DBUF. and the data will be writen to DBUF.
#define SPIFC_USER_DBUF_ADDR                       ((0x0092  << 2) + 0xfd000400)
  //bit 7:0.  the DBUF ADDRESS used to send to or receive from SPI FLASH. the last 2 bits must be 0.
#define SPIFC_FLASH_STATUS                         ((0x00a0  << 2) + 0xfd000400)
#define SPIFC_STATUS                               ((0x00a1  << 2) + 0xfd000400)
  //bit 1.  AES KEY valid. read only 1 : The aes key is ready to use. 0: the aes key is not ready.
  //bit 0.  SPI CTRL STATE IDLE.  read only. 1 = idle; 0 = busy.
#define SPIFC_CTRL                                 ((0x00a2  << 2) + 0xfd000400)
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
#define PSRAM_AXI_INTF_CTRL                        ((0x0000  << 2) + 0xfe007400)
 //bit 3   axi interface soft reset.      1 = reset AXI interface.  0 = normal.
 //bit 2   disable axi interface clock.    1 = disable; 0 = enable.
 //bit 1   axi interface auto clock gating enable. 1 = enable; 0 = disable.
 //bit 0   axi interface AXI request enable.  1 = enable. 0 = disable.
#define PSRAM_DBUF_CTRL                            ((0x0001  << 2) + 0xfe007400)
 //bit 16.    MWRITE_EN.  1 : enable mask write(with DM pin). 0 not enable.
 //bit 15:0   DBUF AGE to write back to PSRAM if DBUF is dirty. but whole DBUF data is not READ to write.(with MWRITE enabled).
#define PSRAM_DBUF_CTRL1                           ((0x0002  << 2) + 0xfe007400)
  //bit 15:0  DBUF AGE to write back to psram if DBUF is dirty and whole DBUF data is ready to write.
#define PSRAM_APB_CTRL                             ((0x0003  << 2) + 0xfe007400)
  //7:4      PSRAM PHY register APB secure contrl.
             //bit 7,  1: no secure control.  0 : use bit 4 to match PPROT[0] bit.
             //bit 4.  when bit 7 == 0, bit 4 must match PPROT[0] to access.
  //3:0      PCTL_CTRL APB bus control regsiters secure control.
             //bit 3,  1: no secure control.  0 : use bit 0 to match PPROT[0] bit.
             //bit 0.  when bit 3 == 0, bit 0 must match PPROT[0] to access.
#define PSRAM_SEC_CTRL                             ((0x0004  << 2) + 0xfe007400)
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
#define PSRAM_DES_PADDING                          ((0x0005  << 2) + 0xfe007400)
   //bit 31 :0 Padding, with address together to generate describe 64 bits input.
#define PSRAM_RANGE0_STA                           ((0x0010  << 2) + 0xfe007400)
  //bit 22 :0.
  //AXI security range 0  start address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE0_EDA                           ((0x0011  << 2) + 0xfe007400)
  //bit 22 :0.
  //AXI security range 0  end address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE0_CTRL0                         ((0x0012  << 2) + 0xfe007400)
  // AXI security range 0 control.
#define PSRAM_RANGE0_CTRL1                         ((0x0013  << 2) + 0xfe007400)
  // AXI security range 0 control.
#define PSRAM_RANGE1_STA                           ((0x0014  << 2) + 0xfe007400)
  //bit 22 :0.
  //AXI security range 1  start address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE1_EDA                           ((0x0015  << 2) + 0xfe007400)
  //bit 22 :0.
  //AXI security range 1  end address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE1_CTRL0                         ((0x0016  << 2) + 0xfe007400)
  // AXI security range 1 control.
#define PSRAM_RANGE1_CTRL1                         ((0x0017  << 2) + 0xfe007400)
  // AXI security range 1 control.
#define PSRAM_RANGE2_STA                           ((0x0018  << 2) + 0xfe007400)
  //bit 31 : 6.
  //AXI security range 2  start address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE2_EDA                           ((0x0019  << 2) + 0xfe007400)
  //bit 31 :6.
  //AXI security range 2  end address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE2_CTRL0                         ((0x001a  << 2) + 0xfe007400)
#define PSRAM_RANGE2_CTRL1                         ((0x001b  << 2) + 0xfe007400)
#define PSRAM_RANGE3_STA                           ((0x001c  << 2) + 0xfe007400)
  //bit 31 : 6.
  //AXI security range 3  start address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE3_EDA                           ((0x001d  << 2) + 0xfe007400)
  //bit 31 :6.
  //AXI security range 3  end address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE3_CTRL0                         ((0x001e  << 2) + 0xfe007400)
  // AXI security range 3 control.
#define PSRAM_RANGE3_CTRL1                         ((0x001f  << 2) + 0xfe007400)
  // AXI security range 3 control.
#define PSRAM_RANGE4_STA                           ((0x0020  << 2) + 0xfe007400)
  //bit 31 :6.
  //AXI security range 4  start address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE4_EDA                           ((0x0021  << 2) + 0xfe007400)
  //bit 31 :6.
  //AXI security range 4  end address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE4_CTRL0                         ((0x0022  << 2) + 0xfe007400)
  // AXI security range 4 control.
#define PSRAM_RANGE4_CTRL1                         ((0x0023  << 2) + 0xfe007400)
#define PSRAM_RANGE5_STA                           ((0x0024  << 2) + 0xfe007400)
  //bit 22 :0.
  //AXI security range 5 start address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE5_EDA                           ((0x0025  << 2) + 0xfe007400)
  //bit 22 :0.
  //AXI security range 5  end address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE5_CTRL0                         ((0x0026  << 2) + 0xfe007400)
  // AXI security range 5 control.
#define PSRAM_RANGE5_CTRL1                         ((0x0027  << 2) + 0xfe007400)
  // AXI security range 5 control.
#define PSRAM_RANGE6_STA                           ((0x0028  << 2) + 0xfe007400)
  //bit 22 :0.
  //AXI security range 6 start address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE6_EDA                           ((0x0029  << 2) + 0xfe007400)
  //bit 22 :0.
  //AXI security range 6  end address in unit of 64Byte. related to HADDR bit 28:6.
#define PSRAM_RANGE6_CTRL0                         ((0x002a  << 2) + 0xfe007400)
  // AXI security range 6 control.
#define PSRAM_RANGE6_CTRL1                         ((0x002b  << 2) + 0xfe007400)
  // AXI security range 6 control.
#define PSRAM_RANGE7_CTRL0                         ((0x002c  << 2) + 0xfe007400)
#define PSRAM_RANGE7_CTRL1                         ((0x002d  << 2) + 0xfe007400)
#define PSRAM_VIO_STATUS                           ((0x002e  << 2) + 0xfe007400)
  //bit 31:  security violation.  write 1 to clean security violation status.
  //bit 30.   1 : AXI write violation.  0 : axi read violation.
  //bit 18:16.  AXI ARPROT/AWPROT>
  //bit 11:0.   AXI violation ID.
#define PSRAM_VIO_ADDR                             ((0x002f  << 2) + 0xfe007400)
  //read only
  //31:0  AXI violation address.
#define PSRAM_USER_CTRL0                           ((0x0080  << 2) + 0xfe007400)
  //bit 31.    user request enable.  write 1 to enable.  read 0, means user command accepted by the SPI_CTRL.
  //bit 30.    usr request done.     after user command done. this bit set to 1.
  //bit 29.    USR COMMAND enable.
  //bit 28.    command type0. 1 READ command.  0 write command.
  //bit 27.    command type1. 1 for register.  0 : for memory.
  //bit 21~16. USR  command clock cycles
  //bit 15~0.  USR COMMAND CODE.
#define PSRAM_USER_CTRL1                           ((0x0081  << 2) + 0xfe007400)
  //bit 31.    USR_ADDR_EN
  //bit 30:29. USR_ADDR_DW
  //bit 28~20. USR address clock cycle number.
  //bit 19:16  USR data output/input switch postion
  //bit 15.    USR DUMMY ENABLE
  //bit 13~8.  USR DUMMY clock cycles.
  //bit 5:0    USR data  DQS read enable postion.
#define PSRAM_USER_CTRL2                           ((0x0082  << 2) + 0xfe007400)
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
#define PSRAM_USER_CTRL3                           ((0x0083  << 2) + 0xfe007400)
  //bit 17:16. which cycle to ask phy to check read/write latency at PSRAM SEND command stage.
             //since PSRAM send command stage only 3 clock cycle.  So this number only can chose 0, 1, 2.
  //bit 15:0.  user dummy data when output in dummy cycle.
#define PSRAM_USER_ADDR                            ((0x0084  << 2) + 0xfe007400)
  //bit 31:0.  32bits user address.
#define PSRAM_AXI_REQ_CTRL0                        ((0x0085  << 2) + 0xfe007400)
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
#define PSRAM_AXI_REQ_CTRL1                        ((0x0086  << 2) + 0xfe007400)
  //bit 31:16. AXI request psram READ command code.
  //bit 15:0.  AXI request psram write command code.
#define PSRAM_AXI_REQ_CTRL2                        ((0x0087  << 2) + 0xfe007400)
  //bit 31.  AXI  read request Dummy enable.
  //bit 30.  Check latency enable for WINBOND PSRAM flexible latency.
  //bit 27:22.  clock cycles in dummy stage to eanble PSRAM PHY reading logic.
  //bit 21:16.  axi_read dummy clock cycles number.
  //bit 15:0.   dummy data if output in dummy cycles.
#define PSRAM_AXI_REQ_CTRL3                        ((0x0088  << 2) + 0xfe007400)
  //bit 31      des enable.
  //bit 26:16.  DATA input clock cycles.
  //bit 10:0.   DATA output clock cycles.
#define PSRAM_ACTIMING0                            ((0x0089  << 2) + 0xfe007400)
  //bit 31:30.  tSLCH
  //bit 29:28   tCLSH
  //bit 20:16   tSHWL
  //bit 15:12   tSHSL2
  //bit 11:8    tSHSL1
  //bit 7:0     tWHSL
#define PSRAM_ACTIMING1                            ((0x008a  << 2) + 0xfe007400)
  //bit 7:0 tRWR  for winbond PSRAM,  = tRWR -3. Winbond PSRAM include one CS cycle and 2 command cycle.
                  //for APMEMORY  should be same value of tCPH
#define PSRAM_ACTIMING2                            ((0x008b  << 2) + 0xfe007400)
#define PSRAM_WDG_CTRL                             ((0x008c  << 2) + 0xfe007400)
 //bit [31]   1: force to reset PSRAM PCTL and PHY logic. 0: normal.
 //bit [30]   1: automatic reset PSRAM PCTL and PHY logic if watch dog triggered.
 //bit 12:0.  watch dog timer.   for Winbond PSRAM should be 1us for tRC.  for APMEMORY should be 1us for tCEM.
//SPI DATA BUFFER.  There's a total 512KByte + 64byte(organized as 36x128bits) SRAM .
// since each APB access is 32bits. So we use word address as APB read/write address.
// But for SPI side we have to use 128bit boundary. That's the first program/read SPI with data buffer must start at data buffer 128bit boundary.  that means the PSRAM_USE_DBUF_ADDR last 2 bits must be 0.
#define PSRAM_DBUF_0                               ((0x0090  << 2) + 0xfe007400)
#define PSRAM_DBUF_1                               ((0x0091  << 2) + 0xfe007400)
#define PSRAM_DBUF_2                               ((0x0092  << 2) + 0xfe007400)
#define PSRAM_DBUF_3                               ((0x0093  << 2) + 0xfe007400)
#define PSRAM_DBUF_4                               ((0x0094  << 2) + 0xfe007400)
#define PSRAM_DBUF_5                               ((0x0095  << 2) + 0xfe007400)
#define PSRAM_DBUF_6                               ((0x0096  << 2) + 0xfe007400)
#define PSRAM_DBUF_7                               ((0x0097  << 2) + 0xfe007400)
#define PSRAM_DBUF_8                               ((0x0098  << 2) + 0xfe007400)
#define PSRAM_DBUF_9                               ((0x0099  << 2) + 0xfe007400)
#define PSRAM_DBUF_A                               ((0x009a  << 2) + 0xfe007400)
#define PSRAM_DBUF_B                               ((0x009b  << 2) + 0xfe007400)
#define PSRAM_DBUF_C                               ((0x009c  << 2) + 0xfe007400)
#define PSRAM_DBUF_D                               ((0x009d  << 2) + 0xfe007400)
#define PSRAM_DBUF_E                               ((0x009e  << 2) + 0xfe007400)
#define PSRAM_DBUF_F                               ((0x009f  << 2) + 0xfe007400)
#define PSRAM_CFG_STS                              ((0x00a0  << 2) + 0xfe007400)
 //32bits register to save the USR read command read back values.
#define PSRAM_STATUS                               ((0x00a1  << 2) + 0xfe007400)
  //bit 31.   usr_req_done flag.  1 : user request done . write 1 to clean.
  //bit 30~11.
  //bit 10:8
  //bit 7:3.
  //bit 2.  axi_data buffer idle bit.  1 : idle. 0 : working.
  //bit 1.  axi interface idle bit.    1 : idle. 0 : working.
  //bit 0.  psram ctrl idle bit.       1 : idle. 0 : working.
#define PSRAM_CTRL                                 ((0x00a2  << 2) + 0xfe007400)
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
#define PSRAM_PIN_CTRL                             ((0x00a3  << 2) + 0xfe007400)
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
#define PSRAM_DQ0_DIN_DLY                          ((0x00c0  << 2) + 0xfe007400)
  //bit 6:0. DQ0 DIN delay control.  in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ1_DIN_DLY                          ((0x00c1  << 2) + 0xfe007400)
  //bit 6:0. DQ1 input delay control. in DLY16 read mode,  only [3:0] is valid.
#define PSRAM_DQ2_DIN_DLY                          ((0x00c2  << 2) + 0xfe007400)
  //bit 6:0. DQ2 input delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ3_DIN_DLY                          ((0x00c3  << 2) + 0xfe007400)
  //bit 6:0. DQ3 input delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ4_DIN_DLY                          ((0x00c4  << 2) + 0xfe007400)
  //bit 6:0. DQ4 input delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ5_DIN_DLY                          ((0x00c5  << 2) + 0xfe007400)
  //bit 6:0. DQ5 input delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ6_DIN_DLY                          ((0x00c6  << 2) + 0xfe007400)
  //bit 6:0. DQ6 input delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ7_DIN_DLY                          ((0x00c7  << 2) + 0xfe007400)
  //bit 6:0. DQ7 input delay control.  in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ0_DOUT_DLY                         ((0x00c8  << 2) + 0xfe007400)
  //bit 6:0. DQ0 DOUT delay control.  in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ1_DOUT_DLY                         ((0x00c9  << 2) + 0xfe007400)
  //bit 6:0. DQ1 output delay control. in DLY16 read mode,  only [3:0] is valid.
#define PSRAM_DQ2_DOUT_DLY                         ((0x00ca  << 2) + 0xfe007400)
  //bit 6:0. DQ2 output delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ3_DOUT_DLY                         ((0x00cb  << 2) + 0xfe007400)
  //bit 6:0. DQ3 output delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ4_DOUT_DLY                         ((0x00cc  << 2) + 0xfe007400)
  //bit 6:0. DQ4 output delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ5_DOUT_DLY                         ((0x00cd  << 2) + 0xfe007400)
  //bit 6:0. DQ5 output delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ6_DOUT_DLY                         ((0x00ce  << 2) + 0xfe007400)
  //bit 6:0. DQ6 output delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ7_DOUT_DLY                         ((0x00cf  << 2) + 0xfe007400)
  //bit 6:0. DQ7 output delay control.  in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ0_OEN_DLY                          ((0x00d0  << 2) + 0xfe007400)
  //bit 6:0. DQ0 OEN delay control.  in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ1_OEN_DLY                          ((0x00d1  << 2) + 0xfe007400)
  //bit 6:0. DQ1 output enable delay control. in DLY16 read mode,  only [3:0] is valid.
#define PSRAM_DQ2_OEN_DLY                          ((0x00d2  << 2) + 0xfe007400)
  //bit 6:0. DQ2 output enable delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ3_OEN_DLY                          ((0x00d3  << 2) + 0xfe007400)
  //bit 6:0. DQ3 output enable delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ4_OEN_DLY                          ((0x00d4  << 2) + 0xfe007400)
  //bit 6:0. DQ4 output enable delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ5_OEN_DLY                          ((0x00d5  << 2) + 0xfe007400)
  //bit 6:0. DQ5 output enable delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ6_OEN_DLY                          ((0x00d6  << 2) + 0xfe007400)
  //bit 6:0. DQ6 output enable delay control. in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQ7_OEN_DLY                          ((0x00d7  << 2) + 0xfe007400)
  //bit 6:0. DQ7 output enable delay control.  in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DM_DOUT_DLY                          ((0x00d8  << 2) + 0xfe007400)
  //bit 6:0. DM output delay control.  in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DM_OEN_DLY                           ((0x00d9  << 2) + 0xfe007400)
  //bit 6:0. DM output enable delay control.  in DLY16 real mode, only [3:0] is valid.
#define PSRAM_CS_DOUT_DLY                          ((0x00da  << 2) + 0xfe007400)
  //bit 6:0. CS output  delay control.  in DLY16 real mode, only [3:0] is valid.
#define PSRAM_DQS_DIN_DLY                          ((0x00db  << 2) + 0xfe007400)
  //bit 6:0. DQS input delay control.   unit = 1/64 UI with PSRAM_LCDL_CTRL bit 28 = 0;
  //  unit = 1 delay cell,    with PSRAM_LCDL_CTRL bit 28 = 1;
#define PSRAM_DQSN_DIN_DLY                         ((0x00dc  << 2) + 0xfe007400)
  //bit 6:0. DQSN input delay control.   unit = 1/64 UI with PSRAM_LCDL_CTRL bit 28 = 0;
  //  unit = 1 delay cell,    with PSRAM_LCDL_CTRL bit 28 = 1;
#define PSRAM_CKN_DOUT_DLY                         ((0x00dd  << 2) + 0xfe007400)
  //bit 6:0. CKN output delay control.   unit = 1/64 UI with PSRAM_LCDL_CTRL bit 28 = 0;
  //  unit = 1 delay cell,    with PSRAM_LCDL_CTRL bit 28 = 1;
#define PSRAM_CK_DOUT_DLY                          ((0x00de  << 2) + 0xfe007400)
  //bit 6:0. CK output delay control.   unit = 1/64 UI with PSRAM_LCDL_CTRL bit 28 = 0;
  //  unit = 1 delay cell,    with PSRAM_LCDL_CTRL bit 28 = 1;
#define PSRAM_RDEN_DLY                             ((0x00df  << 2) + 0xfe007400)
  //bit 10:7.  READ enable phase delay. unit = 1UI.  for AP QPI PSRAM.  PSRAM only send one cycle DQS preamble.
  //           We have to used this UI delay to compensate the whole clock output delay and the DQS input delay.
  //           if run high frequency, this delay need to be trained.  the valid value from 0 ~ 12.  Since one UI is 1/4 of PSRAM clock,
  //           the total delay of the clock output delay + input delay should be less than 3 PSRAM clock.
  //bit 6:0. READ enable fine tune delay control.   unit = 1/64 UI with PSRAM_LCDL_CTRL bit 28 = 0;
  //  unit = 1 delay cell,    with PSRAM_LCDL_CTRL bit 28 = 1;
#define PSRAM_LCDL_CTRL                            ((0x00f0  << 2) + 0xfe007400)
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
#define PSRAM_LCDL_CAL_CTRL1                       ((0x00f1  << 2) + 0xfe007400)
  //27:24. which LCDL delay line used to run calibration.
  //19:16. LCDL calibration loop end position.
  //15:12. LCDL calibration loop initial position.
  //10:0.  LCDL calibration initial phase counter.
#define PSRAM_LCDL_CAL_CTRL2                       ((0x00f2  << 2) + 0xfe007400)
 //26:16  LCDL phase lock limit. if the lcdl phase counter  difference between 2 calibration loops is less than this number, The LCDL would be locked and stop the calibration.
 //10:0.  deta_init. the first loop phase changing number.
#define PSRAM_LCDL_CAL_CTRL3                       ((0x00f3  << 2) + 0xfe007400)
  //29:25 to configure LCDL calibration step 5 position.
  //24:20 to configure LCDL calibration step 4 position.
  //19:15 to configure LCDL calibration step 3 position.
  //14:10 to configure LCDL calibration step 2 position.
  //9:5   to configure LCDL calibration step 1 position.
  //4:0   to configure LCDL calibration step 0 position.
#define PSRAM_LCDL_CAL_CTRL4                       ((0x00f4  << 2) + 0xfe007400)
  //19:15 to configure LCDL calibration step 9 position.
  //14:10 to configure LCDL calibration step 8 position.
  //9:5   to configure LCDL calibration step 7 position.
  //4:0   to configure LCDL calibration step 6 position.
#define PSRAM_CLK_UI                               ((0x00f5  << 2) + 0xfe007400)
  //bit 6:0. 4xCLOCK period delay measured from LCDL calibration.
#define PSRAM_LCDL_PH                              ((0x00f6  << 2) + 0xfe007400)
  //10:0 LCDL phase counter after calibration.
#define PSRAM_LCDL_STATUS                          ((0x00f7  << 2) + 0xfe007400)
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
#define DMC_SEC_RANGE0_STA                         ((0x0000  << 2) + 0xfd021000)
 //bit 31:12.  range0 start address.  in 4KB boundary.
#define DMC_SEC_RANGE0_EDA                         ((0x0001  << 2) + 0xfd021000)
 //bit 31:12.  range0 end address.  in 4KB boundary.
#define DMC_SEC_RANGE0_RID_CTRL                    ((0x0002  << 2) + 0xfd021000)
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
#define DMC_SEC_RANGE0_WID_CTRL                    ((0x0003  << 2) + 0xfd021000)
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
#define DMC_SEC_RANGE1_STA                         ((0x0004  << 2) + 0xfd021000)
 //bit 31:12.  range1 start address.  in 4KB boundary.
#define DMC_SEC_RANGE1_EDA                         ((0x0005  << 2) + 0xfd021000)
 //bit 31:12.  range0 end address.  in 4KB boundary.
#define DMC_SEC_RANGE1_RID_CTRL                    ((0x0006  << 2) + 0xfd021000)
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
#define DMC_SEC_RANGE1_WID_CTRL                    ((0x0007  << 2) + 0xfd021000)
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
#define DMC_SEC_RANGE2_STA                         ((0x0008  << 2) + 0xfd021000)
 //bit 31:12.  range2 start address.  in 4KB boundary.
#define DMC_SEC_RANGE2_EDA                         ((0x0009  << 2) + 0xfd021000)
 //bit 31:12.  range2 end address.  in 4KB boundary.
#define DMC_SEC_RANGE2_RID_CTRL                    ((0x000a  << 2) + 0xfd021000)
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
#define DMC_SEC_RANGE2_WID_CTRL                    ((0x000b  << 2) + 0xfd021000)
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
#define DMC_SEC_RANGE3_STA                         ((0x000c  << 2) + 0xfd021000)
 //bit 31:12.  range3 start address.  in 4KB boundary.
#define DMC_SEC_RANGE3_EDA                         ((0x000d  << 2) + 0xfd021000)
 //bit 31:12.  range3 end address.  in 4KB boundary.
#define DMC_SEC_RANGE3_RID_CTRL                    ((0x000e  << 2) + 0xfd021000)
 //range 3 read access enable based on DMC ID assignments.
  //check DMC_SEC_RANGE0_RID_CTRL for each bit define.
#define DMC_SEC_RANGE3_WID_CTRL                    ((0x000f  << 2) + 0xfd021000)
 //range 3 write access enable based on DMC ID assignments.
  //check DMC_SEC_RANGE0_WID_CTRL for each bit define.
#define DMC_SEC_RANGE4_STA                         ((0x0010  << 2) + 0xfd021000)
 //bit 31:12.  range4 start address.  in 4KB boundary.
#define DMC_SEC_RANGE4_EDA                         ((0x0011  << 2) + 0xfd021000)
 //bit 31:12.  range4 end address.  in 4KB boundary.
#define DMC_SEC_RANGE4_RID_CTRL                    ((0x0012  << 2) + 0xfd021000)
 //range 4 read access enable based on DMC ID assignments.
 //check DMC_SEC_RANGE0_RID_CTRL for each bit define.
#define DMC_SEC_RANGE4_WID_CTRL                    ((0x0013  << 2) + 0xfd021000)
 //range 4 write access enable based on DMC ID assignments.
 //check DMC_SEC_RANGE0_WID_CTRL for each bit define.
#define DMC_SEC_RANGE5_STA                         ((0x0014  << 2) + 0xfd021000)
 //bit 31:12.  range5 start address.  in 4KB boundary.
#define DMC_SEC_RANGE5_EDA                         ((0x0015  << 2) + 0xfd021000)
 //bit 31:12.  range5 end address.  in 4KB boundary.
#define DMC_SEC_RANGE5_RID_CTRL                    ((0x0016  << 2) + 0xfd021000)
 //range 5 read access enable based on DMC ID assignments.
 //check DMC_SEC_RANGE0_RID_CTRL for each bit define.
#define DMC_SEC_RANGE5_WID_CTRL                    ((0x0017  << 2) + 0xfd021000)
 //range 5 write access enable based on DMC ID assignments.
 //check DMC_SEC_RANGE0_WID_CTRL for each bit define.
#define DMC_SEC_RANGE6_STA                         ((0x0018  << 2) + 0xfd021000)
 //bit 31:12.  range5 start address.  in 4KB boundary.
#define DMC_SEC_RANGE6_EDA                         ((0x0019  << 2) + 0xfd021000)
 //bit 31:12.  range6 end address.  in 4KB boundary.
#define DMC_SEC_RANGE6_RID_CTRL                    ((0x001a  << 2) + 0xfd021000)
 //range 6 read access enable based on DMC ID assignments.
 //check DMC_SEC_RANGE0_RID_CTRL for each bit define.
#define DMC_SEC_RANGE6_WID_CTRL                    ((0x001b  << 2) + 0xfd021000)
 //range 6 write access enable based on DMC ID assignments.
 //check DMC_SEC_RANGE0_WID_CTRL for each bit define.
//range 7 is default range if the access not hit any of the previous 7 range, if would hit range 7.
#define DMC_SEC_RANGE7_RID_CTRL                    ((0x001c  << 2) + 0xfd021000)
 //range 7 read access enable based on DMC ID assignments.
 //check DMC_SEC_RANGE0_RID_CTRL for each bit define.
#define DMC_SEC_RANGE7_WID_CTRL                    ((0x001d  << 2) + 0xfd021000)
 //range 7 write access enable based on DMC ID assignments.
 //check DMC_SEC_RANGE0_WID_CTRL for each bit define.
#define DMC_SEC_RANGE_CTRL                         ((0x001e  << 2) + 0xfd021000)
  //bit 23:16    8 range security level. to choose with DES KEY to use data describe.
  //bit 7:0      8 range enable. each bit for one range to identify the range is enabled or not.
//registers to check the security protection and watch point error information.
#define DMC_DES_CTRL                               ((0x0020  << 2) + 0xfd021000)
 //bit 1 data describe enable.
 //bit 0  wirte 1 to block DES CTRL and DES PADDING read/write.
#define DMC_DES_PADDING                            ((0x0021  << 2) + 0xfd021000)
#define DMC_SEC_STATUS                             ((0x00b8  << 2) + 0xfd021000)
 //bit 31~2. not used.
 //bit 1   write security violation.
 //bit 0.  read security violation.
#define DMC_VIO_ADDR0                              ((0x00b9  << 2) + 0xfd021000)
  //ddr0 write secure violation address.
#define DMC_VIO_ADDR1                              ((0x00ba  << 2) + 0xfd021000)
  //bit 31:18 . not used. always 0.
  //17     ddr0 secure check violation.
  //16     ddr0 write address overflow. write out of DDR size.
  //15:13. ddr0 write violation AWPROT bits.
  //12:0   ddr0_write violation ID.  Please check ID assignment for ID bit defines.
#define DMC_VIO_ADDR2                              ((0x00bb  << 2) + 0xfd021000)
  //ddr0 write secure violation address.
#define DMC_VIO_ADDR3                              ((0x00bc  << 2) + 0xfd021000)
  //bit 31:18 . not used. always 0.
  //17     ddr0 secure check violation.
  //16     ddr0 read address overflow. write out of DDR size.
  //15:13. ddr0 read violation AWPROT bits.
  //12:0   ddr0_read violation ID. please ID assignment for ID bit defines.
//DDR0_ADDRMAP is used for RANK0 bank, row, col address mapping from AXI 32bits linear address.
#define DDR0_ADDRMAP_0                             ((0x00d0  << 2) + 0xfd021000)
  //29:25 ca8.
  //24:20 ca7.
  //19:15 ca6.
  //14:10 ca5.
  //9:5   ca4.
  //4:0   ca3.
#define DDR0_ADDRMAP_1                             ((0x00d1  << 2) + 0xfd021000)
  //29:25 ra2.
  //24:20 ra1.
  //19:15 ra0.
  //14:10 ca11.
  //9:5   ca10.
  //4:0   ca9.
#define DDR0_ADDRMAP_2                             ((0x00d2  << 2) + 0xfd021000)
  //29:25 ra8.
  //24:20 ra7.
  //19:15 ra6.
  //14:10 ra5.
  //9:5   ra4.
  //4:0   ra3.
#define DDR0_ADDRMAP_3                             ((0x00d3  << 2) + 0xfd021000)
  //29:25 ra14.
  //24:20 ra13.
  //19:15 ra12.
  //14:10 ra11.
  //9:5   ra10.
  //4:0   ra9.
#define DDR0_ADDRMAP_4                             ((0x00d4  << 2) + 0xfd021000)
  //29:25 ra16 for DDR4 SDRAM
  //24:20 bg1  for DDR4 SDRAM.
  //19:15 ba2.    or bg0 for DDR4.
  //14:10 ba1.
  //9:5   ba0.
  //4:0   ra15.
#define DMC_DDR_CTRL                               ((0x00da  << 2) + 0xfd021000)
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
#define DMC_APB_SEC_CTRL                           ((0x00db  << 2) + 0xfd021000)
  //bit 19:16.  DMC normal APB register secure control.
    //bit 19:  1:  all can write those register.  0:  the APB_PROT[0] must match the bit 16 to access those regsiter.
    //Bit15:12.   DMC sticky APB regsiter secure control.
    //bit 15.  1: All APB bus can write those registers. 0: The APB_PROT[0] must match the bit 12 to access those register.
    //bit 11:8.   not used.
    //Bit 7:4.   DMC DDR SDRAM protocal contorl register contorl
    //bit 11.  1: All APB bus can write those registers. 0: The APB_PROT[0] must match the bit 4 to access those register.
    //bit 3:0.   PHY APB regsiter secure control.
    //bit 3.  1: All APB bus can write those registers. 0: The APB_PROT[0] must match the bit 0 to access those register.
#define DMC_DDR_PHY_CTRL                           ((0x00bf  << 2) + 0xfd021000)
  //bit 4    PwrOkIn
  //bit 3.   PHY APB soft reset_n
  //bit 2.   PHY soft reset_n
  //bit 1.   PHY DfiClk  enable.
  //bit 0.   PHY DfiCtlClk enable.
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfd021400
// -----------------------------------------------
//`define DMC_TEST_REG_BASE               32'hfd021400
#define DMC_TEST_WRCMD_ADDR                        ((0x00dc  << 2) + 0xfd021400)
  // the current write cmd address.
#define DMC_TEST_RDRSP_ADDR                        ((0x00dd  << 2) + 0xfd021400)
  // the failed read response address(for error data )
#define DMC_TEST_RDCMD_ADDR                        ((0x00de  << 2) + 0xfd021400)
   // the current read command address.
#define DMC_TEST_WDG                               ((0x00df  << 2) + 0xfd021400)
  //31:16.  write response watch dog.
  //15:0.   read response  watch dog.
#define DMC_TEST_STA                               ((0x00e0  << 2) + 0xfd021400)
  //test start address.  for non-sha mode,  the last 5 bits would be ignored. the test address at 32bytes boundary.
  //                     for sha mode,      address must be in 64 bytes boundary. that mean the last 6 bits must be 0.
#define DMC_TEST_EDA                               ((0x00e1  << 2) + 0xfd021400)
  //test end address.  for non-sha mode,  the last 5 bits would be ignored. the test address at 32bytes boundary.
  //                   for sha mode,       address must be in 64 bytes boundary. that mean the last 6bits must be 1.
#define DMC_TEST_CTRL                              ((0x00e2  << 2) + 0xfd021400)
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
#define DMC_TEST_NUM                               ((0x00e3  << 2) + 0xfd021400)
   // how many test command for the test if the DMC_TEST_CTRL bit 24 is 0.
#define DMC_TEST_WD0                               ((0x00e4  << 2) + 0xfd021400)
   // write data 0 for write command. also for read back data comparision.
#define DMC_TEST_WD1                               ((0x00e5  << 2) + 0xfd021400)
   // write data 1 for write command. also for read back data comparision.
#define DMC_TEST_WD2                               ((0x00e6  << 2) + 0xfd021400)
   // write data 2 for write command. also for read back data comparision.
#define DMC_TEST_WD3                               ((0x00e7  << 2) + 0xfd021400)
   // write data 3 for write command. also for read back data comparision.
#define DMC_TEST_WD4                               ((0x00e8  << 2) + 0xfd021400)
   // write data 4 for write command. also for read back data comparision.
#define DMC_TEST_WD5                               ((0x00e9  << 2) + 0xfd021400)
   // write data 5 for write command. also for read back data comparision.
#define DMC_TEST_WD6                               ((0x00ea  << 2) + 0xfd021400)
   // write data 6 for write command. also for read back data comparision.
#define DMC_TEST_WD7                               ((0x00eb  << 2) + 0xfd021400)
   // write data 7 for write command. also for read back data comparision.
#define DMC_TEST_RD0                               ((0x00ec  << 2) + 0xfd021400)
   // the read back data 0.  if error happens, it would capture the first error data.
#define DMC_TEST_RD1                               ((0x00ed  << 2) + 0xfd021400)
   // the read back data 1.  if error happens, it would capture the first error data.
#define DMC_TEST_RD2                               ((0x00ee  << 2) + 0xfd021400)
   // the read back data 2.  if error happens, it would capture the first error data.
#define DMC_TEST_RD3                               ((0x00ef  << 2) + 0xfd021400)
   // the read back data 3.  if error happens, it would capture the first error data.
#define DMC_TEST_RD4                               ((0x00f0  << 2) + 0xfd021400)
   // the read back data 4.  if error happens, it would capture the first error data.
#define DMC_TEST_RD5                               ((0x00f1  << 2) + 0xfd021400)
   // the read back data 5.  if error happens, it would capture the first error data.
#define DMC_TEST_RD6                               ((0x00f2  << 2) + 0xfd021400)
   // the read back data 6.  if error happens, it would capture the first error data.
#define DMC_TEST_RD7                               ((0x00f3  << 2) + 0xfd021400)
   // the read back data 7.  if error happens, it would capture the first error data.
#define DMC_TEST_ERR_ADDR                          ((0x00f4  << 2) + 0xfd021400)
  // it capture the first error address.
#define DMC_TEST_ERR_CNT                           ((0x00f5  << 2) + 0xfd021400)
  // how many data error happens in the whole test period.
#define DMC_TEST_STS                               ((0x00f6  << 2) + 0xfd021400)
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
#define DMC_TEST_COMP_MASK                         ((0x0033  << 2) + 0xfd021400)
  //32bits for DMC TEST COMPARE bit enable.
  //1 : to MASK this bit.  0: compare this bit.
#define DMC_TEST_RDBI0                             ((0x0034  << 2) + 0xfd021400)
  //MPC RD FIFO command DBI read back data
  //bit 31:16  the second cycle.
  //bit 15:0   the first cycle.
#define DMC_TEST_RDBI1                             ((0x0035  << 2) + 0xfd021400)
  //MPC RD FIFO command DBI read back data
  //bit 31:16. the Forth cycle.
  //bit 15:0.  the third cycle.
#define DMC_TEST_WSTRB0                            ((0x0036  << 2) + 0xfd021400)
  //MPC WR FIFO command DM bit write data
  //bit 31:16  the second cycle.
  //bit 15:0   the first cycle.
#define DMC_TEST_DRAM_CMD                          ((0x0038  << 2) + 0xfd021400)
 //bit 31. cmd done.  write 0 to clean.
 //bit 30. data done. write 0 to clean.
 //bit 4:0.  only one bit can be 1. read data stored in DMC_TEST_RD* write data from DMC_TEST_W
 //bit 4. LPDDR4 MPC write data command( MPC WR FIFO).      Not support LPDDR4 in A1.
 //bit 3. LPDDR4 MPC read data command (MPC RD Calibration and RD FIFO).Not support LPDDR4 in A1.
 //bit 2. LPDDR4 MPC-1 command ( NOP,  Start DQS interval ....)Not support LPDDR4 in A1.
 //bit 1. mrr comand.
 //bit 0. mrw command.
#define DMC_TEST_DRAM_CMD_CODE                     ((0x0039  << 2) + 0xfd021400)
 //bit 27:26. 128bits data cycles . 0: 1 clock cycles;  1: 2  clock cycles; 2: 3 clock cycles; 3:4 clock cycles.
 //           LPDDR4 32bits: 4 clock cycles; LPDDR4 16bits: 2 clock cycles;
 //           DDR3/4/LPDDR3 32 bits : 2 clock cycles. DDR3/4 16bits:  1 cycles.
 //bit 25  MRW/MRR/MPC command rank 1 select.  1: select.  0: not select.  In A1, only rank0 supported.
 //bit 24. MRW/MRR/MPC command rank 0 select.  1: select.  0: not select.In A1, only rank0 supported.
 //bit 23:16  MR addr.  DDR4 case :  18:16 ba[2:0].    20:19 BG[1:0].
 //bit 15:0   opcode.
#define DMC_TEST_DRAM_CMD_TIME                     ((0x003a  << 2) + 0xfd021400)
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
#define DMC_SOFT_RST2                              ((0x0003  << 2) + 0xfd020000)
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
#define DMC_VERSION                                ((0x0005  << 2) + 0xfd020000)
   //read only 32'h000b0000.  for A1
#define DMC_MON_CTRL0                              ((0x0020  << 2) + 0xfd020000)
   //bit 31.   qos_mon_en.    write 1 to trigger the enable. polling this bit 0, means finished.  or use interrupt to check finish.
   //bit 30.   qos_mon interrupt clear.  clear the qos monitor result.  read 1 = qos mon finish interrupt.
   //bit 1.    qos monitor 1 enable.
   //bit 0.    qos monitor 0 enable.
#define DMC_MON_CTRL1                              ((0x0021  << 2) + 0xfd020000)
   //bit 8:0.  qos monitor 0 channel select.
   //bit 8    to select DMC TEST
   //bit 7:0  each bit enable one AXI subID.
#define DMC_MON_CTRL2                              ((0x0022  << 2) + 0xfd020000)
   //bit 8.  to enable monitor 0 second level subID selection for example device.
   //bit 7:0  each bit to enable one second level subID inside device.
#define DMC_MON_CTRL3                              ((0x0023  << 2) + 0xfd020000)
   //bit 8:0.  qos monitor 1 channel select.
   //bit 8    to select DMC TEST
   //bit 7:0  each bit enable one AXI subID.
#define DMC_MON_CTRL4                              ((0x0024  << 2) + 0xfd020000)
   //bit 8.  to enable monitor 1 second level subID selection for example device.
   //bit 7:0  each bit to enable one second level subID inside device.
#define DMC_MON_ALL_REQ_CNT                        ((0x0029  << 2) + 0xfd020000)
  // at the test period,  the whole MMC request time.
#define DMC_MON_ALL_GRANT_CNT                      ((0x002a  << 2) + 0xfd020000)
  // at the test period,  the whole MMC granted data cycles. 64bits unit.
#define DMC_MON_ONE_GRANT_CNT                      ((0x002b  << 2) + 0xfd020000)
  // at the test period,  the granted data cycles for the selected channel and ports.
#define DMC_MON_SEC_GRANT_CNT                      ((0x002c  << 2) + 0xfd020000)
  // at the test period,  the granted data cycles for the selected channel and ports.
#define DMC_MON_TIMER                              ((0x002f  << 2) + 0xfd020000)
 // timer for the monitor period.
#define DMC_CLKG_CTRL2                             ((0x0032  << 2) + 0xfd020000)
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
#define DMC_CLKG_CTRL3                             ((0x0033  << 2) + 0xfd020000)
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
#define DMC_CHAN_STS                               ((0x0036  << 2) + 0xfd020000)
  //bit 4      always 1
  //bit 3      ddr0 write data buffer idle. 1 : idle 0: busy.
  //bit 2      always 1.
  //bit 1      ddr0 wbuf idle.              1 : idle 0: busy.
  //bit 0.      axi0 channel idle.         1 : idle 0: busy.
#define DMC_CMD_FILTER_CTRL3                       ((0x0042  << 2) + 0xfd020000)
  //bit 31.    force wbuf empty.
  //bit 30:26  wbuf high level number
  //bit 25:21  wbuf mid  level number
  //bit 20:16  wbuf low level number
  //bit 14:10  rbuf high level number
  //bit 9:5    rbuf middle level number
  //bit 4:0    rbuf low level number
#define DMC_CMD_FILTER_CTRL4                       ((0x0043  << 2) + 0xfd020000)
  //bit 29:25.  tITW.long
  //bit 24:20.  tITW. short
  //bit 19:12   tAP auto precharge the bank not used if idle that long time.
  //bit 11:6    write to read accesses if there write hit request.
  //bit 5:0     read to write accesses if there write hit request.
#define DMC_CMD_FILTER_CTRL5                       ((0x0044  << 2) + 0xfd020000)
  //bit 31:24   Once ddr data bus switch to read, the maxmum read command number to give up the bus when there's write request pending for write buffer.
  //bit 23:16   Once ddr data bus switch to write, the maxmum write command number to give up the bus when there's read request pending too long.
  //bit 15:8.   Once ddr data bus switch to read, the minimum read command number to transfer back to write stage if there's still pending read request.
  //bit 7:0.    Once ddr data bus switch to write, the minimum write command number to transfer back to read stage if there's still pending write request.
#define DMC_CMD_FILTER_CTRL6                       ((0x0045  << 2) + 0xfd020000)
  //bit 31:24   write urgent 3 request pending hold num.
  //bit 23:16   write urgent 2 request pending hold num.
  //bit 15:8.   write urgent 1 request pending hold num.
  //bit 7:0.    write urgent 0 request pending hold num.
#define DMC_CMD_FILTER_CTRL7                       ((0x0046  << 2) + 0xfd020000)
  //bit 15:8    write to read waiting cycles if there write hit request.
  //bit 7:0     read to write waiting cycles if there write hit request.
#define DMC_CMD_BUFFER_CTRL                        ((0x0047  << 2) + 0xfd020000)
  //bit 31:26  total write buffer number. default 32.
  //bit 25:20  total read buffer number. default 32.
  //bit 19:8    reserved.
  //bit 7:0    aw_pending_inc_num.  incease write ugent level 1 when write command waiting to in write buffer that long.
#define DMC_CMD_BUFFER_CTRL1                       ((0x0048  << 2) + 0xfd020000)
  //bit 29:24  read buffer number in non-urgent request.
  //bit 23:16  read buffer bank miss watch dog threshold.
  //bit 15:12  read buffer urgent level 3 counter inc weight.
  //bit 11:8   read buffer urgent level 2 counter inc weight.
  //bit 7:4    read buffer urgent level 1 counter inc weight.
  //bit 3:0    read buffer urgent level 0 counter inc weight.
#define DMC_AXI0_CHAN_CTRL                         ((0x0080  << 2) + 0xfd020000)
  //bit 31       AXI interface request enable.
  //bit 19       AXI interface  auto clock gating enable.  1: enable; 0 disable.
  //bit 18       AXI interface  clock disable. 1 : disable clock.  0 enable clock.
  //bit 17       AXI interface  dmc clock domain soft reset.  0 : reset.  1 : normal working mode.
  //bit 16       AXI interface  s0 clock domain soft reset.   0 : reset.  1 : normal working mode.
  //bit 15:12    ugt3 QOS level.
  //bit 11:8     ugt2 QOS level.
  //bit 7:4      ugt1 QOS level.
  //bit 3:0      not used.
#define DMC_AXI0_HOLD_CTRL                         ((0x0081  << 2) + 0xfd020000)
    //31:24 write hold num.   max outstanding request number.
    //23:16  write hold release num. if the outstanding request == hold num, then hold this request unless the outstanding request number bellow the hold release number, then continue to request.
    //15:8 read hold num.   max outstanding request number.
    //7:0  read hold release num. if the outstanding request == hold num, then hold this request unless the outstanding request number bellow the hold release number, then continue to request.
#define DMC_AXI5_HOLD_CTRL                         ((0x0095  << 2) + 0xfd020000)
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
#define DMC_DRAM_TMRD                              ((0x0000  << 2) + 0xfd020400)
   //bit 3:0.  tMRD.
#define DMC_DRAM_TRFC                              ((0x0001  << 2) + 0xfd020400)
   //bit 9:0 tRFC
#define DMC_DRAM_TRP                               ((0x0002  << 2) + 0xfd020400)
   //bit 21:16.  tRP for precharge all banks.
   //bit 5:0.    tRP for precharge one bank.
#define DMC_DRAM_TRTW                              ((0x0003  << 2) + 0xfd020400)
   //bit 5:0   tRTW
#define DMC_DRAM_TCL                               ((0x0004  << 2) + 0xfd020400)
  //bit 5:0  tCL/tRL. read latency.
#define DMC_DRAM_TCWL                              ((0x0005  << 2) + 0xfd020400)
  //bit 5:0.  CWL:  write latency.
#define DMC_DRAM_TRAS                              ((0x0006  << 2) + 0xfd020400)
  //bit 7:0.  tRAS.  minimum active to precharge time for same bank.
#define DMC_DRAM_TRC                               ((0x0007  << 2) + 0xfd020400)
  //bit 7:0.  tRC.  minimum active to active time for same bank.
#define DMC_DRAM_TRCD                              ((0x0008  << 2) + 0xfd020400)
  //bit 7:0  tRCD  active to read/write timing for same bank.
#define DMC_DRAM_TRRD                              ((0x0009  << 2) + 0xfd020400)
  //bit 21:16.  tRRD_l  active bank A to active B in same band group for DDR4.
  //bit 5:0.    tRRD/tRRD_s   active bank A to active bank b time.
              //tRRD_s:   active bank A to active bank b in different bank grousp for DDR4.
#define DMC_DRAM_TFAW                              ((0x000a  << 2) + 0xfd020400)
  //bit 8:0   tFAW.  four active command windows
#define DMC_DRAM_TRTP                              ((0x000b  << 2) + 0xfd020400)
  //bit 5:0  tRTP.
#define DMC_DRAM_TWR                               ((0x000c  << 2) + 0xfd020400)
  //bit 5:0 tWR.
#define DMC_DRAM_TWTR                              ((0x000d  << 2) + 0xfd020400)
  //bit 5:0 tWTR.
#define DMC_DRAM_TCCD                              ((0x000e  << 2) + 0xfd020400)
  //bit 19:16. tCCD/tCCD_l.
  //bit 3:0 tCCD/tCCD_s    read to read command time or write to write command time.
#define DMC_DRAM_TEXSR                             ((0x000f  << 2) + 0xfd020400)
  //bit 9:0. tEXSR.  EXIT SELF-REFRESH to read/write command.
#define DMC_DRAM_TXS                               ((0x0010  << 2) + 0xfd020400)
  //bit 9:0  tXS.  EXIT SELF_REFRESH to other command time
#define DMC_DRAM_TXP                               ((0x0011  << 2) + 0xfd020400)
  //bit 3:0. tXP.  EXIT power down to other command time
#define DMC_DRAM_TXPDLL                            ((0x0012  << 2) + 0xfd020400)
  //bit 9:0.  tXPDLL,  EXIT power down to read/write command time(need to relock PLL).
#define DMC_DRAM_TZQCS                             ((0x0013  << 2) + 0xfd020400)
  //bit 11:0.  ZQCS command to other command time.
#define DMC_DRAM_TCKSRE                            ((0x0014  << 2) + 0xfd020400)
  //bit 4:0. enter self refresh to disable clock time.
#define DMC_DRAM_TCKSRX                            ((0x0015  << 2) + 0xfd020400)
  //bit 4:0. enable clock to exit self refresh time.
#define DMC_DRAM_TCKE                              ((0x0016  << 2) + 0xfd020400)
  //bit 4:0.  CKE high or low minimum time.
#define DMC_DRAM_TMOD                              ((0x0017  << 2) + 0xfd020400)
  //bit 4:0  tMOD.  MRR/MRW to other command time.
#define DMC_DRAM_TDQS                              ((0x0018  << 2) + 0xfd020400)
  //bit 3:0 tDQS. the delay to access different rank.
#define DMC_DRAM_TRSTL                             ((0x0019  << 2) + 0xfd020400)
  //not used.
#define DMC_DRAM_TZQLAT                            ((0x001a  << 2) + 0xfd020400)
  //bit 5:0 ZQ LATCH command to other comand timing in LPDDR4 mode.
#define DMC_DRAM_TMRR                              ((0x001b  << 2) + 0xfd020400)
  //bit 7:0 tMRR  not used in DMC. not support MR READ.
#define DMC_DRAM_TCKESR                            ((0x001c  << 2) + 0xfd020400)
 //bit 9:0 tCKESR.   CKE low minimum pulse in self refresh mode.
#define DMC_DRAM_TDPD                              ((0x001d  << 2) + 0xfd020400)
 //not support.
#define DMC_DRAM_DFITCTRLDELAY                     ((0x001e  << 2) + 0xfd020400)
  //bit 3:0. DFI_t_ctrldealy
#define DMC_DRAM_DFITPHYWRDATA                     ((0x001f  << 2) + 0xfd020400)
  //bit 5:0.  dfi_t_phy_wrdata.
#define DMC_DRAM_DFITPHYWRLAT                      ((0x0020  << 2) + 0xfd020400)
  //bit 5:0.  dfi_t_phy_wrlat.  in DDR3/4/LPDDR3 mode:   WL -5.   in LPDDR4 mode: WL -5 + 2.
#define DMC_DRAM_DFITRDDATAEN                      ((0x0021  << 2) + 0xfd020400)
  //bit 5:0.  dfi_t_rddata_en.  in DDR3/4/LPDDR3 mode: RL -5. in LPDDR4 mode : RL -5 + 1.
#define DMC_DRAM_DFITPHYRDLAT                      ((0x0022  << 2) + 0xfd020400)
  //bit 5:0.  dfi_t_rdlat.
#define DMC_DRAM_DFITCTRLUPDMIN                    ((0x0023  << 2) + 0xfd020400)
  //bit 7:0.  CTRLUPD_MIN  minimux clock cycle to maintain CTRLUPD_REQ.
#define DMC_DRAM_DFITCTRLUPDMAX                    ((0x0024  << 2) + 0xfd020400)
  //bit 7:0   CTRLUPD_MAX.  maxmum clock cycle to maintain CTRLUPD_REQ if no CTRLUPD_ACK response.
#define DMC_DRAM_DFITMSTRRESP                      ((0x0025  << 2) + 0xfd020400)
  //not used.
#define DMC_DRAM_DFITREFMSKI                       ((0x0026  << 2) + 0xfd020400)
  //not used.
#define DMC_DRAM_DFITCTRLUPDI                      ((0x0027  << 2) + 0xfd020400)
  //not used.
#define DMC_DRAM_DFITDRAMCLK                       ((0x0028  << 2) + 0xfd020400)
//bit 17    dram clk1 enable.
//bit 16    dram clk0 enable.
//bit 15:8  DRAM CLK disable waiting time
//bit 7:0   DRAM CLK enable  enable timer
#define DMC_DRAM_DFITLPRESP                        ((0x002a  << 2) + 0xfd020400)
  //bit 3:0.  dfi_lp_ctrl_req response time. after dfi_lp_ctrl_req asserted, and after response time if there's still no dfi_lp_ack respone, then drop the dfi_lp_ctrl_req.
#define DMC_DRAM_DFITPHYMSTR                       ((0x002b  << 2) + 0xfd020400)
 //bit 15  1: enable DFIPHYMASTER INTERFACE 0 disable DFIPHYMSTR en response.
 // always 0  we would use DFI_RETRAIN function for PHY retrain required from LPDDR4.
#define DMC_DRAM_TCKECK                            ((0x002c  << 2) + 0xfd020400)
 //bit 4:0. tCKECK  from CKE low to assert dfi_dram_clk_disable time. this time + dfi_t_ctrl_delay
#define DMC_DRAM_TREFI                             ((0x002d  << 2) + 0xfd020400)
 //write this register will update the auto refresh related register to the auto refresh control logic.
 //bit 31:24:  tZQCI dmc send zqci period.  unit is how much auto refresh period.
 //bit 23:16   pvti  dmc send dfi_ctrlupd_req period.  unit is one auto refresh period.
 //bit 15:8.   tREFI.dmc send auto refresh command period. unit is 100ns.
 //bit 7:0.    t100ns period. unit is dmc clock cycles
#define DMC_DRAM_TSR                               ((0x002e  << 2) + 0xfd020400)
  //bit 5:0 tSR.  self resfresh enter to exit time.
#define DMC_DRAM_TCCDMW                            ((0x002f  << 2) + 0xfd020400)
  //bit 5:0.  4*tCCD in LPDDR4 mask write.
#define DMC_DRAM_TESCKE                            ((0x0030  << 2) + 0xfd020400)
  //bit 5:0  tESCKE.  enter self refresh to power time for LPDDR4.
#define DMC_DRAM_TREFI_DDR3                        ((0x0031  << 2) + 0xfd020400)
  //7:0. 8*DDR3 SDRAM tREFI time . the unit is t100ns.  use this to check in 8*tREFI time, the DMC should sent more than 16 auto REFRESH command.
#define DMC_DRAM_TZQCAL                            ((0x0032  << 2) + 0xfd020400)
  //7:0. ZQCAL for LPDDR4.
#define DMC_DRAM_T10US                             ((0x0033  << 2) + 0xfd020400)
  //10us clock cycle number used for LP2 mode.
#define DMC_DRAM_TMRRI                             ((0x0034  << 2) + 0xfd020400)
//bit 7:0   tMRRI for MRR
#define DMC_DRAM_DFITPHYUPDTYPE0                   ((0x0080  << 2) + 0xfd020400)
 //dfi_phyupd_ack hold time for dfi_phyupd_req type = 0.
#define DMC_DRAM_DFITPHYUPDTYPE1                   ((0x0081  << 2) + 0xfd020400)
 //dfi_phyupd_ack hold time for dfi_phyupd_req type = 1.
#define DMC_DRAM_DFITPHYUPDTYPE2                   ((0x0082  << 2) + 0xfd020400)
 //dfi_phyupd_ack hold time for dfi_phyupd_req type = 2.
#define DMC_DRAM_DFITPHYUPDTYPE3                   ((0x0083  << 2) + 0xfd020400)
 //dfi_phyupd_ack hold time for dfi_phyupd_req type = 3.
#define DMC_DRAM_DFIODTCFG                         ((0x0084  << 2) + 0xfd020400)
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
#define DMC_DRAM_DFIODTCFG1                        ((0x0085  << 2) + 0xfd020400)
  //bit 27:24  ODT length for BL8 read transfer.
  //bit 19:16. ODT length for BL8 write transfer.
  //bit 12:8.  ODT latency for reads.  suppose to be 0.
  //bit 4:0.   ODT latency for writes.  suppose to be 0.
#define DMC_DRAM_MCFG                              ((0x0086  << 2) + 0xfd020400)
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
#define DMC_DRAM_DFI_CTRL                          ((0x0089  << 2) + 0xfd020400)
  //bit 31 siu_dfi_lat err generation enable.  1: if dfi read latency violation, generate data error. 0 : disable.
  //bit 30:0. not used.
#define DMC_DRAM_DFIINITCFG                        ((0x008a  << 2) + 0xfd020400)
  //bit 31.   dfi_init_complete status. read only.
  //bit 15:14.  Frequency set 1 dfi_freq_ratio value.  Not used in A1
  //bit 12:8    Frequency set 1 dfi_freq value.  Not used in A1
  //bit 7:6     Frequency set 0 dfi_freq_ratio value.
  //bit 5:1     Frequency set 0 dfi_freq value.
  //bit 0.      dfi_init_start value  can be use manually config dfi_init_start signal.
#define DMC_DRAM_ZQ_CTRL                           ((0x008b  << 2) + 0xfd020400)
  // only bit 0 can be enable in A1.
  //bit 2  send ZQCS command to RANK0 then send comand to RANK1.
  //bit 1. send ZQCS command to both RANK0 and RANK1 together.
  //bit 0. send ZQCS command to only rank0.
#define DMC_DRAM_APD_CTRL                          ((0x008c  << 2) + 0xfd020400)
 //bit 19:16  DFI_LP_WAKEUP value in APD DFI_LP_REQ mode
 //bit 12    1: exit power down slow mode(waiting PLL LOCK).  0 : fast mode.
 //bit 11    enable DFI_LP_REQ when enter Auto power down mode.
 //bit 10    disable DFI_clk_disable when enter auto power down mode.
 //bit 9:0    0  disable auto power down mode.
            //non zero value to enable auto power down when DMC is in idle state for this number of clock cycles.
#define DMC_DRAM_ASR_CTRL                          ((0x008d  << 2) + 0xfd020400)
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
#define DMC_DRAM_PHYMSTR_CTRL                      ((0x0090  << 2) + 0xfd020400)
  //not used in A1. should be all 0.
#define DMC_DRAM_DFIODTRANKMAP                     ((0x0091  << 2) + 0xfd020400)
 //not used in A1.
#define DMC_DRAM_REFR_CTRL                         ((0x0092  << 2) + 0xfd020400)
  //bit 17:8 auto refresh request pending cnt if there's page hit request.
  //bit 6  Disabled auto refresh command if over 16 auto refresh command sent in 2 TREFI_DDR3 period
  //bit 5  enable dmc send ZQCS command .
  //bit 4. enable dmc send DFI_CTRUPD_REQ.
  //bit 3:1. how many refresh command send for one period. = this number + 1
  //bit 0.  enable dmc send auto refresh command.
#define DMC_DRAM_FREQ_CTRL                         ((0x0093  << 2) + 0xfd020400)
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
#define DMC_DRAM_SCFG                              ((0x0094  << 2) + 0xfd020400)
  // bit 2:0 only one bit can be high at same time.
  // bit 2  1 : to ask PCTL enter ACCESS STATE.  0 : deassert the request.
  // bit 1  1 : to ask PCTL enter SELF REFRESH STATE.  0 : deassert the request.
  // bit 0  1 : to ask PCTL enter STOP/CONFIG STATE .  0 : deassert the request.
#define DMC_DRAM_STAT                              ((0x0095  << 2) + 0xfd020400)
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
#define DMC_DRAM_STAT1                             ((0x0096  << 2) + 0xfd020400)
  //bit 11:8  freq_st.
  //bit 7:5   train_st
  //bit 4:0   dram_phy_st
#define DMC_PHY_RETRAINING_CTRL                    ((0x0097  << 2) + 0xfd020400)
  // A1 not support LPDDR4 so this register is not used in A1.
  //bit 31 :  phy_retraining enable.
  //bit 30 :  check  vpu sleep_en.
  //bit 25:24 : retraining dfi_freq[4:3], the [2:0] bit still use the dfi_freq bits to keep the frequency.
  //bit 23:0: retraining period unit : 100ns.
#define DMC_DFI_ERR_STAT                           ((0x0098  << 2) + 0xfd020400)
 //LPDDR4 PHY DFI error infomation.
 //bit 31:20. not used.
 //bit 9.    ddr0_dfi_error
 //bit 8:5   ddr0_dfi_error_info.
 //bit 4.    ddr1_dfi_error.
 //bit 3:0.  ddr1_dfi_error_info.
#define DMC_LP2_TIMER                              ((0x009a  << 2) + 0xfd020400)
//bit 15:0.   timer setting to measure how long the chip is entered LP2 mode.
//this timer is 40bits counter with DMC PCLK.
//we use the high 16bits to compare this register. if the counter is large than this number,  that means  the PHY need addition 10us after wakeup the PHY and before exit self_refresh mode.
#define DMC_DRAM_DFI_SWAP_0                        ((0x00a0  << 2) + 0xfd020400)
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
#define DMC_DRAM_DFI_SWAP_1                        ((0x00a1  << 2) + 0xfd020400)
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
#define DMC_DRAM_DFI_SWAP_2                        ((0x00a2  << 2) + 0xfd020400)
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
#define DMC_DRAM_DFI_SWAP_3                        ((0x00a3  << 2) + 0xfd020400)
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
#define DMC_DRAM_DFI_SWAP_4                        ((0x00a4  << 2) + 0xfd020400)
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
#define DMC_DRAM_DFI_SWAP_5                        ((0x00a5  << 2) + 0xfd020400)
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
#define DMC_DRAM_DFI_SWAP_6                        ((0x00a6  << 2) + 0xfd020400)
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
#define DMC_DRAM_DFI_SWAP_7                        ((0x00a7  << 2) + 0xfd020400)
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
#define DMC_DRAM_DFI_SWAP_8                        ((0x00a8  << 2) + 0xfd020400)
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
#define DMC_DRAM_DFI_SWAP_9                        ((0x00a9  << 2) + 0xfd020400)
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
#define DMC_DRAM_DFI_SWAP_10                       ((0x00aa  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[1] function select
#define DMC_DRAM_DFI_SWAP_11                       ((0x00ab  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[2] function select
#define DMC_DRAM_DFI_SWAP_12                       ((0x00ac  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[3] function select
#define DMC_DRAM_DFI_SWAP_13                       ((0x00ad  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[4] function select
#define DMC_DRAM_DFI_SWAP_14                       ((0x00ae  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[5] function select
#define DMC_DRAM_DFI_SWAP_15                       ((0x00af  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[6] function select
#define DMC_DRAM_DFI_SWAP_16                       ((0x00b0  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[7] function select
#define DMC_DRAM_DFI_SWAP_17                       ((0x00b1  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[8] function select
#define DMC_DRAM_DFI_SWAP_18                       ((0x00b2  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[9] function select
#define DMC_DRAM_DFI_SWAP_19                       ((0x00b3  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[10] function select
#define DMC_DRAM_DFI_SWAP_20                       ((0x00b4  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[11] function select
#define DMC_DRAM_DFI_SWAP_21                       ((0x00b5  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[12] function select
#define DMC_DRAM_DFI_SWAP_22                       ((0x00b6  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[13] function select
#define DMC_DRAM_DFI_SWAP_23                       ((0x00b7  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[14] function select
#define DMC_DRAM_DFI_SWAP_24                       ((0x00b8  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[15] function select
#define DMC_DRAM_DFI_SWAP_25                       ((0x00b9  << 2) + 0xfd020400)
  //bit 5:0. dfi_a[16] function select
#define DMC_DRAM_DFI_SWAP_26                       ((0x00bb  << 2) + 0xfd020400)
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
#define DMC_STICKY_0                               ((0x0000  << 2) + 0xfd020800)
#define DMC_STICKY_1                               ((0x0001  << 2) + 0xfd020800)
#define DMC_STICKY_2                               ((0x0002  << 2) + 0xfd020800)
#define DMC_STICKY_3                               ((0x0003  << 2) + 0xfd020800)
#define DMC_STICKY_4                               ((0x0004  << 2) + 0xfd020800)
#define DMC_STICKY_5                               ((0x0005  << 2) + 0xfd020800)
#define DMC_STICKY_6                               ((0x0006  << 2) + 0xfd020800)
#define DMC_STICKY_7                               ((0x0007  << 2) + 0xfd020800)
#define DMC_STICKY_8                               ((0x0008  << 2) + 0xfd020800)
#define DMC_STICKY_9                               ((0x0009  << 2) + 0xfd020800)
#define DMC_STICKY_10                              ((0x000a  << 2) + 0xfd020800)
#define DMC_STICKY_11                              ((0x000b  << 2) + 0xfd020800)
#define DMC_STICKY_12                              ((0x000c  << 2) + 0xfd020800)
#define DMC_STICKY_13                              ((0x000d  << 2) + 0xfd020800)
#define DMC_STICKY_14                              ((0x000e  << 2) + 0xfd020800)
#define DMC_STICKY_15                              ((0x000f  << 2) + 0xfd020800)
#define DMC_STICKY_16                              ((0x0010  << 2) + 0xfd020800)
#define DMC_STICKY_17                              ((0x0011  << 2) + 0xfd020800)
#define DMC_STICKY_18                              ((0x0012  << 2) + 0xfd020800)
#define DMC_STICKY_19                              ((0x0013  << 2) + 0xfd020800)
#define DMC_STICKY_20                              ((0x0014  << 2) + 0xfd020800)
#define DMC_STICKY_21                              ((0x0015  << 2) + 0xfd020800)
#define DMC_STICKY_22                              ((0x0016  << 2) + 0xfd020800)
#define DMC_STICKY_23                              ((0x0017  << 2) + 0xfd020800)
#define DMC_STICKY_24                              ((0x0018  << 2) + 0xfd020800)
#define DMC_STICKY_25                              ((0x0019  << 2) + 0xfd020800)
#define DMC_STICKY_26                              ((0x001a  << 2) + 0xfd020800)
#define DMC_STICKY_27                              ((0x001b  << 2) + 0xfd020800)
#define DMC_STICKY_28                              ((0x001c  << 2) + 0xfd020800)
#define DMC_STICKY_29                              ((0x001d  << 2) + 0xfd020800)
#define DMC_STICKY_30                              ((0x001e  << 2) + 0xfd020800)
#define DMC_STICKY_31                              ((0x001f  << 2) + 0xfd020800)
#define DMC_STICKY_32                              ((0x0020  << 2) + 0xfd020800)
#define DMC_STICKY_33                              ((0x0021  << 2) + 0xfd020800)
#define DMC_STICKY_34                              ((0x0022  << 2) + 0xfd020800)
#define DMC_STICKY_35                              ((0x0023  << 2) + 0xfd020800)
#define DMC_STICKY_36                              ((0x0024  << 2) + 0xfd020800)
#define DMC_STICKY_37                              ((0x0025  << 2) + 0xfd020800)
#define DMC_STICKY_38                              ((0x0026  << 2) + 0xfd020800)
#define DMC_STICKY_39                              ((0x0027  << 2) + 0xfd020800)
#define DMC_STICKY_40                              ((0x0028  << 2) + 0xfd020800)
#define DMC_STICKY_41                              ((0x0029  << 2) + 0xfd020800)
#define DMC_STICKY_42                              ((0x002a  << 2) + 0xfd020800)
#define DMC_STICKY_43                              ((0x002b  << 2) + 0xfd020800)
#define DMC_STICKY_44                              ((0x002c  << 2) + 0xfd020800)
#define DMC_STICKY_45                              ((0x002d  << 2) + 0xfd020800)
#define DMC_STICKY_46                              ((0x002e  << 2) + 0xfd020800)
#define DMC_STICKY_47                              ((0x002f  << 2) + 0xfd020800)
#define DMC_STICKY_48                              ((0x0030  << 2) + 0xfd020800)
#define DMC_STICKY_49                              ((0x0031  << 2) + 0xfd020800)
#define DMC_STICKY_50                              ((0x0032  << 2) + 0xfd020800)
#define DMC_STICKY_51                              ((0x0033  << 2) + 0xfd020800)
#define DMC_STICKY_52                              ((0x0034  << 2) + 0xfd020800)
#define DMC_STICKY_53                              ((0x0035  << 2) + 0xfd020800)
#define DMC_STICKY_54                              ((0x0036  << 2) + 0xfd020800)
#define DMC_STICKY_55                              ((0x0037  << 2) + 0xfd020800)
#define DMC_STICKY_56                              ((0x0038  << 2) + 0xfd020800)
#define DMC_STICKY_57                              ((0x0039  << 2) + 0xfd020800)
#define DMC_STICKY_58                              ((0x003a  << 2) + 0xfd020800)
#define DMC_STICKY_59                              ((0x003b  << 2) + 0xfd020800)
#define DMC_STICKY_60                              ((0x003c  << 2) + 0xfd020800)
#define DMC_STICKY_61                              ((0x003d  << 2) + 0xfd020800)
#define DMC_STICKY_62                              ((0x003e  << 2) + 0xfd020800)
#define DMC_STICKY_63                              ((0x003f  << 2) + 0xfd020800)
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
#define RESETCTRL_RESET0                           ((0x0000  << 2) + 0xfe000000)
#define RESETCTRL_RESET1                           ((0x0001  << 2) + 0xfe000000)
#define RESETCTRL_RESET2                           ((0x0002  << 2) + 0xfe000000)
#define RESETCTRL_RESET0_LEVEL                     ((0x0010  << 2) + 0xfe000000)
#define RESETCTRL_RESET1_LEVEL                     ((0x0011  << 2) + 0xfe000000)
#define RESETCTRL_RESET2_LEVEL                     ((0x0012  << 2) + 0xfe000000)
#define RESETCTRL_RESET0_MASK                      ((0x0020  << 2) + 0xfe000000)
#define RESETCTRL_RESET1_MASK                      ((0x0021  << 2) + 0xfe000000)
#define RESETCTRL_RESET2_MASK                      ((0x0022  << 2) + 0xfe000000)
#define RESETCTRL_RESET_HOLD_CYC                   ((0x0030  << 2) + 0xfe000000)
#define RESETCTRL_WATCHDOG_CTRL0                   ((0x0040  << 2) + 0xfe000000)
#define RESETCTRL_WATCHDOG_CTRL1                   ((0x0041  << 2) + 0xfe000000)
#define RESETCTRL_WATCHDOG_CNT                     ((0x0042  << 2) + 0xfe000000)
#define RESETCTRL_WATCHDOG_CLR                     ((0x0043  << 2) + 0xfe000000)
#define RESETCTRL_SEC_RESET0                       ((0x0050  << 2) + 0xfe000000)
#define RESETCTRL_SEC_RESET0_LEVEL                 ((0x0051  << 2) + 0xfe000000)
#define RESETCTRL_SEC_RESET0_MASK                  ((0x0052  << 2) + 0xfe000000)
//========================================================================
//  APB0_PAD_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe000400
// -----------------------------------------------
#define PADCTRL_PIN_MUX_REG0                       ((0x0000  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REG1                       ((0x0001  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REG2                       ((0x0002  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REG3                       ((0x0003  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REG4                       ((0x0004  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REG5                       ((0x0005  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REG6                       ((0x0006  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REG7                       ((0x0007  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REG8                       ((0x0008  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REG9                       ((0x0009  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REGA                       ((0x000a  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REGB                       ((0x000b  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REGC                       ((0x000c  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REGD                       ((0x000d  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REGE                       ((0x000e  << 2) + 0xfe000400)
#define PADCTRL_PIN_MUX_REGF                       ((0x000f  << 2) + 0xfe000400)
#define PADCTRL_GPIO_IRQ_CTRL0                     ((0x0010  << 2) + 0xfe000400)
#define PADCTRL_GPIO_IRQ_CTRL1                     ((0x0011  << 2) + 0xfe000400)
#define PADCTRL_GPIO_IRQ_CTRL2                     ((0x0012  << 2) + 0xfe000400)
#define PADCTRL_GPIO_IRQ_CTRL3                     ((0x0013  << 2) + 0xfe000400)
#define PADCTRL_GPIO_IRQ_CTRL4                     ((0x0014  << 2) + 0xfe000400)
#define PADCTRL_WORLDSYNC_CTRL0                    ((0x0018  << 2) + 0xfe000400)
#define PADCTRL_GPIO_MSR_CTRL0                     ((0x0019  << 2) + 0xfe000400)
#define PADCTRL_GPIOP_I                            ((0x0020  << 2) + 0xfe000400)
#define PADCTRL_GPIOP_O                            ((0x0021  << 2) + 0xfe000400)
#define PADCTRL_GPIOP_OEN                          ((0x0022  << 2) + 0xfe000400)
#define PADCTRL_GPIOP_PULL_EN                      ((0x0023  << 2) + 0xfe000400)
#define PADCTRL_GPIOP_PULL_UP                      ((0x0024  << 2) + 0xfe000400)
#define PADCTRL_GPIOP_DS                           ((0x0025  << 2) + 0xfe000400)
#define PADCTRL_GPIOB_I                            ((0x0030  << 2) + 0xfe000400)
#define PADCTRL_GPIOB_O                            ((0x0031  << 2) + 0xfe000400)
#define PADCTRL_GPIOB_OEN                          ((0x0032  << 2) + 0xfe000400)
#define PADCTRL_GPIOB_PULL_EN                      ((0x0033  << 2) + 0xfe000400)
#define PADCTRL_GPIOB_PULL_UP                      ((0x0034  << 2) + 0xfe000400)
#define PADCTRL_GPIOB_DS                           ((0x0035  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_I                            ((0x0040  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_O                            ((0x0041  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_OEN                          ((0x0042  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_PULL_EN                      ((0x0043  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_PULL_UP                      ((0x0044  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_DS                           ((0x0045  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_DS_EXT                       ((0x0046  << 2) + 0xfe000400)
#define PADCTRL_GPIOF_I                            ((0x0050  << 2) + 0xfe000400)
#define PADCTRL_GPIOF_O                            ((0x0051  << 2) + 0xfe000400)
#define PADCTRL_GPIOF_OEN                          ((0x0052  << 2) + 0xfe000400)
#define PADCTRL_GPIOF_PULL_EN                      ((0x0053  << 2) + 0xfe000400)
#define PADCTRL_GPIOF_PULL_UP                      ((0x0054  << 2) + 0xfe000400)
#define PADCTRL_GPIOF_DS                           ((0x0055  << 2) + 0xfe000400)
#define PADCTRL_GPIOA_I                            ((0x0060  << 2) + 0xfe000400)
#define PADCTRL_GPIOA_O                            ((0x0061  << 2) + 0xfe000400)
#define PADCTRL_GPIOA_OEN                          ((0x0062  << 2) + 0xfe000400)
#define PADCTRL_GPIOA_PULL_EN                      ((0x0063  << 2) + 0xfe000400)
#define PADCTRL_GPIOA_PULL_UP                      ((0x0064  << 2) + 0xfe000400)
#define PADCTRL_GPIOA_DS                           ((0x0065  << 2) + 0xfe000400)
#define PADCTRL_TESTN_I                            ((0x0070  << 2) + 0xfe000400)
#define PADCTRL_TESTN_O                            ((0x0071  << 2) + 0xfe000400)
#define PADCTRL_TESTN_OEN                          ((0x0072  << 2) + 0xfe000400)
#define PADCTRL_TESTN_PULL_EN                      ((0x0073  << 2) + 0xfe000400)
#define PADCTRL_TESTN_PULL_UP                      ((0x0074  << 2) + 0xfe000400)
#define PADCTRL_TESTN_DS                           ((0x0075  << 2) + 0xfe000400)
#define PADCTRL_MUTE_CTRL                          ((0x0080  << 2) + 0xfe000400)
//========================================================================
//  APB0_CLK_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe000800
// -----------------------------------------------
#define CLKTREE_SYS_OSCIN_CTRL                     ((0x0000  << 2) + 0xfe000800)
#define CLKTREE_RTC_BY_OSCIN_CTRL0                 ((0x0001  << 2) + 0xfe000800)
#define CLKTREE_RTC_BY_OSCIN_CTRL1                 ((0x0002  << 2) + 0xfe000800)
#define CLKTREE_RTC_CTRL                           ((0x0003  << 2) + 0xfe000800)
#define CLKTREE_SYS_CLK_CTRL0                      ((0x0004  << 2) + 0xfe000800)
#define CLKTREE_AXI_CLK_CTRL0                      ((0x0005  << 2) + 0xfe000800)
#define CLKTREE_SYS_CLK_EN0                        ((0x0007  << 2) + 0xfe000800)
#define CLKTREE_SYS_CLK_EN1                        ((0x0008  << 2) + 0xfe000800)
#define CLKTREE_AXI_CLK_EN                         ((0x0009  << 2) + 0xfe000800)
#define CLKTREE_DSPA_CLK_EN                        ((0x000a  << 2) + 0xfe000800)
#define CLKTREE_DSPB_CLK_EN                        ((0x000b  << 2) + 0xfe000800)
#define CLKTREE_DSPA_CLK_CTRL0                     ((0x000c  << 2) + 0xfe000800)
#define CLKTREE_DSPB_CLK_CTRL0                     ((0x000d  << 2) + 0xfe000800)
#define CLKTREE_CLK12_24_CTRL                      ((0x000e  << 2) + 0xfe000800)
#define CLKTREE_GEN_CLK_CTRL                       ((0x000f  << 2) + 0xfe000800)
#define CLKTREE_TIMESTAMP_CTRL0                    ((0x0010  << 2) + 0xfe000800)
#define CLKTREE_TIMESTAMP_CTRL1                    ((0x0011  << 2) + 0xfe000800)
#define CLKTREE_TIMESTAMP_CTRL2                    ((0x0012  << 2) + 0xfe000800)
#define CLKTREE_TIMESTAMP_VAL0                     ((0x0013  << 2) + 0xfe000800)
#define CLKTREE_TIMESTAMP_VAL1                     ((0x0014  << 2) + 0xfe000800)
#define CLKTREE_TIMEBASE_CTRL0                     ((0x0015  << 2) + 0xfe000800)
#define CLKTREE_TIMEBASE_CTRL1                     ((0x0016  << 2) + 0xfe000800)
#define CLKTREE_SAR_ADC_CLK_CTRL                   ((0x0030  << 2) + 0xfe000800)
#define CLKTREE_PWM_CLK_AB_CTRL                    ((0x0031  << 2) + 0xfe000800)
#define CLKTREE_PWM_CLK_CD_CTRL                    ((0x0032  << 2) + 0xfe000800)
#define CLKTREE_PWM_CLK_EF_CTRL                    ((0x0033  << 2) + 0xfe000800)
#define CLKTREE_SPICC_CLK_CTRL                     ((0x0034  << 2) + 0xfe000800)
#define CLKTREE_TS_CLK_CTRL                        ((0x0035  << 2) + 0xfe000800)
#define CLKTREE_SPIFC_CLK_CTRL                     ((0x0036  << 2) + 0xfe000800)
#define CLKTREE_USB_BUSCLK_CTRL                    ((0x0037  << 2) + 0xfe000800)
#define CLKTREE_SD_EMMC_CLK_CTRL                   ((0x0038  << 2) + 0xfe000800)
#define CLKTREE_CECA_CLK_CTRL0                     ((0x0039  << 2) + 0xfe000800)
#define CLKTREE_CECA_CLK_CTRL1                     ((0x003a  << 2) + 0xfe000800)
#define CLKTREE_CECB_CLK_CTRL0                     ((0x003b  << 2) + 0xfe000800)
#define CLKTREE_CECB_CLK_CTRL1                     ((0x003c  << 2) + 0xfe000800)
#define CLKTREE_PSRAM_CLK_CTRL                     ((0x003d  << 2) + 0xfe000800)
#define CLKTREE_DMC_CLK_CTRL                       ((0x003e  << 2) + 0xfe000800)
#define CLKTREE_FCLK_DIV1_SEL                      ((0x003f  << 2) + 0xfe000800)
#define CLKTREE_TST_CTRL0                          ((0x0040  << 2) + 0xfe000800)
//========================================================================
//  APB0_CEC - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe000c00
// -----------------------------------------------
#define CECA_GEN_CNTL                              ((0x0000  << 2) + 0xfe000c00)
#define CECA_RW_REG                                ((0x0001  << 2) + 0xfe000c00)
#define CECA_INTR_MASKN                            ((0x0002  << 2) + 0xfe000c00)
#define CECA_INTR_CLR                              ((0x0003  << 2) + 0xfe000c00)
#define CECA_INTR_STAT                             ((0x0004  << 2) + 0xfe000c00)
#define CECB_GEN_CNTL                              ((0x0010  << 2) + 0xfe000c00)
#define CECB_RW_REG                                ((0x0011  << 2) + 0xfe000c00)
#define CECB_INTR_MASKN                            ((0x0012  << 2) + 0xfe000c00)
#define CECB_INTR_CLR                              ((0x0013  << 2) + 0xfe000c00)
#define CECB_INTR_STAT                             ((0x0014  << 2) + 0xfe000c00)
//========================================================================
//  APB0_IR_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe001000
// -----------------------------------------------
#define IRCTRL_IR_DEC_LDR_ACTIVE                   ((0x0000  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_LDR_IDLE                     ((0x0001  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_LDR_REPEAT                   ((0x0002  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_BIT_0                        ((0x0003  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_REG0                         ((0x0004  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_FRAME                        ((0x0005  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_STATUS                       ((0x0006  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_REG1                         ((0x0007  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_LDR_ACTIVE                ((0x0010  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_LDR_IDLE                  ((0x0011  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_LDR_REPEAT                ((0x0012  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_BIT_0                     ((0x0013  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_REG0                      ((0x0014  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_FRAME                     ((0x0015  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_STATUS                    ((0x0016  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_REG1                      ((0x0017  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_REG2                      ((0x0018  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_DURATN2                   ((0x0019  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_DURATN3                   ((0x001a  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_FRAME1                    ((0x001b  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_STATUS1                   ((0x001c  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_STATUS2                   ((0x001d  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_REG3                      ((0x001e  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_FRAME_RSV0                ((0x001f  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_FRAME_RSV1                ((0x0020  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_FILTE                     ((0x0021  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_IRQ_CTL                   ((0x0022  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_FIFO_CTL                  ((0x0023  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_WIDTH_NEW                 ((0x0024  << 2) + 0xfe001000)
#define IRCTRL_MF_IR_DEC_REPEAT_DET                ((0x0025  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_DEMOD_CNTL0                  ((0x0030  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_DEMOD_CNTL1                  ((0x0031  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_DEMOD_IIR_THD                ((0x0032  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_DEMOD_THD0                   ((0x0033  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_DEMOD_THD1                   ((0x0034  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_DEMOD_SUM_CNT0               ((0x0035  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_DEMOD_SUM_CNT1               ((0x0036  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_DEMOD_CNT0                   ((0x0037  << 2) + 0xfe001000)
#define IRCTRL_IR_DEC_DEMOD_CNT1                   ((0x0038  << 2) + 0xfe001000)
#define IRCTRL_IR_BLASTER_ADDR0                    ((0x0043  << 2) + 0xfe001000)
#define IRCTRL_IR_BLASTER_ADDR1                    ((0x0044  << 2) + 0xfe001000)
#define IRCTRL_IR_BLASTER_ADDR2                    ((0x0045  << 2) + 0xfe001000)
#define IRCTRL_IR_BLASTER_ADDR3                    ((0x0046  << 2) + 0xfe001000)
//========================================================================
//  APB0_I2C_M - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe001400
// -----------------------------------------------
#define I2C_M_A_CONTROL_REG                        ((0x0000  << 2) + 0xfe001400)
#define I2C_M_A_SLAVE_ADDR                         ((0x0001  << 2) + 0xfe001400)
#define I2C_M_A_TOKEN_LIST0                        ((0x0002  << 2) + 0xfe001400)
#define I2C_M_A_TOKEN_LIST1                        ((0x0003  << 2) + 0xfe001400)
#define I2C_M_A_WDATA_REG0                         ((0x0004  << 2) + 0xfe001400)
#define I2C_M_A_WDATA_REG1                         ((0x0005  << 2) + 0xfe001400)
#define I2C_M_A_RDATA_REG0                         ((0x0006  << 2) + 0xfe001400)
#define I2C_M_A_RDATA_REG1                         ((0x0007  << 2) + 0xfe001400)
#define I2C_M_A_TIMEOUT_TH                         ((0x0008  << 2) + 0xfe001400)
//========================================================================
//  APB0_I2C_S - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe001800
// -----------------------------------------------
#define I2C_S_A_CONTROL_REG                        ((0x0000  << 2) + 0xfe001800)
#define I2C_S_A_SEND_REG                           ((0x0001  << 2) + 0xfe001800)
#define I2C_S_A_RECV_REG                           ((0x0002  << 2) + 0xfe001800)
#define I2C_S_A_CNTL1_REG                          ((0x0003  << 2) + 0xfe001800)
//========================================================================
//  APB0_UART_A - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe001c00
// -----------------------------------------------
#define UART_A_WFIFO                               ((0x0000  << 2) + 0xfe001c00)
#define UART_A_RFIFO                               ((0x0001  << 2) + 0xfe001c00)
#define UART_A_CONTROL                             ((0x0002  << 2) + 0xfe001c00)
#define UART_A_STATUS                              ((0x0003  << 2) + 0xfe001c00)
#define UART_A_MISC                                ((0x0004  << 2) + 0xfe001c00)
#define UART_A_REG5                                ((0x0005  << 2) + 0xfe001c00)
//========================================================================
//  APB0_UART_B - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe002000
// -----------------------------------------------
#define UART_B_WFIFO                               ((0x0000  << 2) + 0xfe002000)
#define UART_B_RFIFO                               ((0x0001  << 2) + 0xfe002000)
#define UART_B_CONTROL                             ((0x0002  << 2) + 0xfe002000)
#define UART_B_STATUS                              ((0x0003  << 2) + 0xfe002000)
#define UART_B_MISC                                ((0x0004  << 2) + 0xfe002000)
#define UART_B_REG5                                ((0x0005  << 2) + 0xfe002000)
//========================================================================
//  APB0_PWM_AB - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe002400
// -----------------------------------------------
#define PWMAB_PWM_A                                ((0x0000  << 2) + 0xfe002400)
#define PWMAB_PWM_B                                ((0x0001  << 2) + 0xfe002400)
#define PWMAB_MISC_REG_AB                          ((0x0002  << 2) + 0xfe002400)
#define PWMAB_DELTA_SIGMA_AB                       ((0x0003  << 2) + 0xfe002400)
#define PWMAB_TIME_AB                              ((0x0004  << 2) + 0xfe002400)
#define PWMAB_A2                                   ((0x0005  << 2) + 0xfe002400)
#define PWMAB_B2                                   ((0x0006  << 2) + 0xfe002400)
#define PWMAB_BLINK_AB                             ((0x0007  << 2) + 0xfe002400)
#define PWMAB_LOCK_AB                              ((0x0008  << 2) + 0xfe002400)
//========================================================================
//  APB0_PWM_CD - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe002800
// -----------------------------------------------
#define PWMCD_PWM_C                                ((0x0000  << 2) + 0xfe002800)
#define PWMCD_PWM_D                                ((0x0001  << 2) + 0xfe002800)
#define PWMCD_MISC_REG_CD                          ((0x0002  << 2) + 0xfe002800)
#define PWMCD_DELTA_SIGMA_CD                       ((0x0003  << 2) + 0xfe002800)
#define PWMCD_TIME_CD                              ((0x0004  << 2) + 0xfe002800)
#define PWMCD_C2                                   ((0x0005  << 2) + 0xfe002800)
#define PWMCD_D2                                   ((0x0006  << 2) + 0xfe002800)
#define PWMCD_BLINK_CD                             ((0x0007  << 2) + 0xfe002800)
#define PWMCD_LOCK_CD                              ((0x0008  << 2) + 0xfe002800)
//========================================================================
//  APB0_SARADC - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe002c00
// -----------------------------------------------
#define SAR_ADC_REG0                               ((0x0000  << 2) + 0xfe002c00)
#define SAR_ADC_CHAN_LIST                          ((0x0001  << 2) + 0xfe002c00)
#define SAR_ADC_AVG_CNTL                           ((0x0002  << 2) + 0xfe002c00)
#define SAR_ADC_REG3                               ((0x0003  << 2) + 0xfe002c00)
#define SAR_ADC_DELAY                              ((0x0004  << 2) + 0xfe002c00)
#define SAR_ADC_LAST_RD                            ((0x0005  << 2) + 0xfe002c00)
#define SAR_ADC_FIFO_RD                            ((0x0006  << 2) + 0xfe002c00)
#define SAR_ADC_AUX_SW                             ((0x0007  << 2) + 0xfe002c00)
#define SAR_ADC_CHAN_10_SW                         ((0x0008  << 2) + 0xfe002c00)
#define SAR_ADC_DETECT_IDLE_SW                     ((0x0009  << 2) + 0xfe002c00)
#define SAR_ADC_DELTA_10                           ((0x000a  << 2) + 0xfe002c00)
#define SAR_ADC_REG11                              ((0x000b  << 2) + 0xfe002c00)
#define SAR_ADC_REG12                              ((0x000c  << 2) + 0xfe002c00)
#define SAR_ADC_REG13                              ((0x000d  << 2) + 0xfe002c00)
#define SAR_ADC_CHNL01                             ((0x000e  << 2) + 0xfe002c00)
#define SAR_ADC_CHNL23                             ((0x000f  << 2) + 0xfe002c00)
#define SAR_ADC_CHNL45                             ((0x0010  << 2) + 0xfe002c00)
#define SAR_ADC_CHNL67                             ((0x0011  << 2) + 0xfe002c00)
//========================================================================
//  JTAGCTRL_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe003000
// -----------------------------------------------
#define JTAGCTRL_CTRL0                             ((0x0000  << 2) + 0xfe003000)
#define JTAGCTRL_PWD_CTRL0                         ((0x0001  << 2) + 0xfe003000)
#define JTAGCTRL_PWD_ADDR0                         ((0x0002  << 2) + 0xfe003000)
#define JTAGCTRL_PWD_ADDR1                         ((0x0003  << 2) + 0xfe003000)
#define JTAGCTRL_PWD_ADDR2                         ((0x0004  << 2) + 0xfe003000)
#define JTAGCTRL_PWD_ADDR3                         ((0x0005  << 2) + 0xfe003000)
#define JTAGCTRL_PWD_VALUE0                        ((0x0006  << 2) + 0xfe003000)
#define JTAGCTRL_PWD_VALUE1                        ((0x0007  << 2) + 0xfe003000)
#define JTAGCTRL_PWD_VALUE2                        ((0x0008  << 2) + 0xfe003000)
#define JTAGCTRL_PWD_VALUE3                        ((0x0009  << 2) + 0xfe003000)
#define JTAGCTRL_PWD_STS                           ((0x000a  << 2) + 0xfe003000)
//========================================================================
//  APB0_MSR_CLK - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe003400
// -----------------------------------------------
#define MSR_CLK_REG0                               ((0x0000  << 2) + 0xfe003400)
#define MSR_CLK_REG1                               ((0x0001  << 2) + 0xfe003400)
#define MSR_CLK_REG2                               ((0x0002  << 2) + 0xfe003400)
#define MSR_CLK_REG3                               ((0x0003  << 2) + 0xfe003400)
#define MSR_CLK_REG4                               ((0x0004  << 2) + 0xfe003400)
#define MSR_CLK_REG5                               ((0x0005  << 2) + 0xfe003400)
#define MSR_CLK_DUTY                               ((0x0006  << 2) + 0xfe003400)
//========================================================================
//  APB0_SPICCA_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe003800
// -----------------------------------------------
#define SPICC_A_RXDATA                             ((0x0000  << 2) + 0xfe003800)
#define SPICC_A_TXDATA                             ((0x0001  << 2) + 0xfe003800)
#define SPICC_A_CONREG                             ((0x0002  << 2) + 0xfe003800)
#define SPICC_A_INTREG                             ((0x0003  << 2) + 0xfe003800)
#define SPICC_A_DMAREG                             ((0x0004  << 2) + 0xfe003800)
#define SPICC_A_STATREG                            ((0x0005  << 2) + 0xfe003800)
#define SPICC_A_PERIODREG                          ((0x0006  << 2) + 0xfe003800)
#define SPICC_A_TESTREG                            ((0x0007  << 2) + 0xfe003800)
#define SPICC_A_DRADDR                             ((0x0008  << 2) + 0xfe003800)
#define SPICC_A_DWADDR                             ((0x0009  << 2) + 0xfe003800)
#define SPICC_A_LD_CNTL0                           ((0x000a  << 2) + 0xfe003800)
#define SPICC_A_LD_CNTL1                           ((0x000b  << 2) + 0xfe003800)
#define SPICC_A_LD_RADDR                           ((0x000c  << 2) + 0xfe003800)
#define SPICC_A_LD_WADDR                           ((0x000d  << 2) + 0xfe003800)
#define SPICC_A_ENHANCE_CNTL                       ((0x000e  << 2) + 0xfe003800)
#define SPICC_A_ENHANCE_CNTL1                      ((0x000f  << 2) + 0xfe003800)
#define SPICC_A_ENHANCE_CNTL2                      ((0x0010  << 2) + 0xfe003800)
//========================================================================
//  APB0_SPICCB_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe003c00
// -----------------------------------------------
#define SPICC_B_RXDATA                             ((0x0000  << 2) + 0xfe003c00)
#define SPICC_B_TXDATA                             ((0x0001  << 2) + 0xfe003c00)
#define SPICC_B_CONREG                             ((0x0002  << 2) + 0xfe003c00)
#define SPICC_B_INTREG                             ((0x0003  << 2) + 0xfe003c00)
#define SPICC_B_DMAREG                             ((0x0004  << 2) + 0xfe003c00)
#define SPICC_B_STATREG                            ((0x0005  << 2) + 0xfe003c00)
#define SPICC_B_PERIODREG                          ((0x0006  << 2) + 0xfe003c00)
#define SPICC_B_TESTREG                            ((0x0007  << 2) + 0xfe003c00)
#define SPICC_B_DRADDR                             ((0x0008  << 2) + 0xfe003c00)
#define SPICC_B_DWADDR                             ((0x0009  << 2) + 0xfe003c00)
#define SPICC_B_LD_CNTL0                           ((0x000a  << 2) + 0xfe003c00)
#define SPICC_B_LD_CNTL1                           ((0x000b  << 2) + 0xfe003c00)
#define SPICC_B_LD_RADDR                           ((0x000c  << 2) + 0xfe003c00)
#define SPICC_B_LD_WADDR                           ((0x000d  << 2) + 0xfe003c00)
#define SPICC_B_ENHANCE_CNTL                       ((0x000e  << 2) + 0xfe003c00)
#define SPICC_B_ENHANCE_CNTL1                      ((0x000f  << 2) + 0xfe003c00)
#define SPICC_B_ENHANCE_CNTL2                      ((0x0010  << 2) + 0xfe003c00)
//========================================================================
//  USBCTRL    - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe004000
// -----------------------------------------------
#define USB_CTRL0                                  ((0x0000  << 2) + 0xfe004000)
#define USB_CTRL1                                  ((0x0001  << 2) + 0xfe004000)
#define USB_CTRL2                                  ((0x0002  << 2) + 0xfe004000)
#define USB_CTRL3                                  ((0x0003  << 2) + 0xfe004000)
#define USB_CTRL4                                  ((0x0004  << 2) + 0xfe004000)
#define USB_CTRL5                                  ((0x0005  << 2) + 0xfe004000)
#define USB_CTRL6                                  ((0x0006  << 2) + 0xfe004000)
#define USB_CTRL7                                  ((0x0007  << 2) + 0xfe004000)
#define USB_CTRL8                                  ((0x0008  << 2) + 0xfe004000)
#define USB_CTRL9                                  ((0x0009  << 2) + 0xfe004000)
#define USB_CTRL10                                 ((0x000a  << 2) + 0xfe004000)
#define USB_CTRL11                                 ((0x000b  << 2) + 0xfe004000)
#define USB_CTRL12                                 ((0x000c  << 2) + 0xfe004000)
#define USB_CTRL13                                 ((0x000d  << 2) + 0xfe004000)
#define USB_CTRL14                                 ((0x000e  << 2) + 0xfe004000)
#define USB_CTRL15                                 ((0x000f  << 2) + 0xfe004000)
#define USB_CTRL16                                 ((0x0010  << 2) + 0xfe004000)
#define USB_CTRL17                                 ((0x0011  << 2) + 0xfe004000)
#define USB_CTRL18                                 ((0x0012  << 2) + 0xfe004000)
#define USB_CTRL19                                 ((0x0013  << 2) + 0xfe004000)
#define USB_CTRL20                                 ((0x0014  << 2) + 0xfe004000)
#define USB_CTRL21                                 ((0x0015  << 2) + 0xfe004000)
#define USB_CTRL22                                 ((0x0016  << 2) + 0xfe004000)
#define USB_CTRL23                                 ((0x0017  << 2) + 0xfe004000)
#define USB_CTRL24                                 ((0x0018  << 2) + 0xfe004000)
#define USB_CTRL25                                 ((0x0019  << 2) + 0xfe004000)
#define USB_CTRL26                                 ((0x001a  << 2) + 0xfe004000)
#define USB_CTRL27                                 ((0x001b  << 2) + 0xfe004000)
#define USB_CTRL28                                 ((0x001c  << 2) + 0xfe004000)
#define USB_CTRL29                                 ((0x001d  << 2) + 0xfe004000)
#define USB_CTRL30                                 ((0x001e  << 2) + 0xfe004000)
#define USB_CTRL31                                 ((0x001f  << 2) + 0xfe004000)
//========================================================================
//  APB0_ACODEC_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe004800
// -----------------------------------------------
#define ACODEC_0                                   ((0x0000  << 2) + 0xfe004800)
#define ACODEC_1                                   ((0x0001  << 2) + 0xfe004800)
#define ACODEC_2                                   ((0x0002  << 2) + 0xfe004800)
#define ACODEC_3                                   ((0x0003  << 2) + 0xfe004800)
#define ACODEC_4                                   ((0x0004  << 2) + 0xfe004800)
#define ACODEC_5                                   ((0x0005  << 2) + 0xfe004800)
#define ACODEC_6                                   ((0x0006  << 2) + 0xfe004800)
#define ACODEC_7                                   ((0x0007  << 2) + 0xfe004800)
#define ACODEC_8                                   ((0x0008  << 2) + 0xfe004800)
#define ACODEC_9                                   ((0x0009  << 2) + 0xfe004800)
#define ACODEC_STS0                                ((0x0010  << 2) + 0xfe004800)
//========================================================================
//  APB0_TEMP_SENSOR - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe004c00
// -----------------------------------------------
#define TS_CFG_REG1                                ((0x0001  << 2) + 0xfe004c00)
#define TS_CFG_REG2                                ((0x0002  << 2) + 0xfe004c00)
#define TS_CFG_REG3                                ((0x0003  << 2) + 0xfe004c00)
#define TS_CFG_REG4                                ((0x0004  << 2) + 0xfe004c00)
#define TS_CFG_REG5                                ((0x0005  << 2) + 0xfe004c00)
#define TS_CFG_REG6                                ((0x0006  << 2) + 0xfe004c00)
#define TS_CFG_REG7                                ((0x0007  << 2) + 0xfe004c00)
#define TS_STAT0                                   ((0x0010  << 2) + 0xfe004c00)
#define TS_STAT1                                   ((0x0011  << 2) + 0xfe004c00)
#define TS_STAT2                                   ((0x0012  << 2) + 0xfe004c00)
#define TS_STAT3                                   ((0x0013  << 2) + 0xfe004c00)
#define TS_STAT4                                   ((0x0014  << 2) + 0xfe004c00)
#define TS_STAT5                                   ((0x0015  << 2) + 0xfe004c00)
#define TS_STAT6                                   ((0x0016  << 2) + 0xfe004c00)
#define TS_STAT7                                   ((0x0017  << 2) + 0xfe004c00)
#define TS_STAT8                                   ((0x0018  << 2) + 0xfe004c00)
#define TS_STAT9                                   ((0x0019  << 2) + 0xfe004c00)
//========================================================================
//  APB0_KL_EFUSE_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe005000
// -----------------------------------------------
//
// Reading file:  ../crypto/crypto_reg.h
//
#define OTP_TEE_RDY                                ((0x0000  << 2) + 0xfe005000)
#define OTP_TEE_CHK                                ((0x0001  << 2) + 0xfe005000)
#define OTP_TEE_CFG                                ((0x0002  << 2) + 0xfe005000)
#define OTP_TEE_WR_DAT0                            ((0x0004  << 2) + 0xfe005000)
#define OTP_TEE_WR_DAT1                            ((0x0005  << 2) + 0xfe005000)
#define OTP_TEE_WR_DAT2                            ((0x0006  << 2) + 0xfe005000)
#define OTP_TEE_WR_DAT3                            ((0x0007  << 2) + 0xfe005000)
#define OTP_TEE_RD_DAT0                            ((0x0008  << 2) + 0xfe005000)
#define OTP_TEE_RD_DAT1                            ((0x0009  << 2) + 0xfe005000)
#define OTP_TEE_RD_DAT2                            ((0x000a  << 2) + 0xfe005000)
#define OTP_TEE_RD_DAT3                            ((0x000b  << 2) + 0xfe005000)
#define OTP_RMA_PW0                                ((0x000c  << 2) + 0xfe005000)
#define OTP_RMA_PW1                                ((0x000d  << 2) + 0xfe005000)
#define OTP_RMA_PW2                                ((0x000e  << 2) + 0xfe005000)
#define OTP_RMA_PW3                                ((0x000f  << 2) + 0xfe005000)
#define OTP_REE_RDY                                ((0x0010  << 2) + 0xfe005000)
#define OTP_REE_CHK                                ((0x0011  << 2) + 0xfe005000)
#define OTP_REE_CFG                                ((0x0012  << 2) + 0xfe005000)
#define OTP_REE_RD_DAT0                            ((0x0014  << 2) + 0xfe005000)
#define OTP_REE_RD_DAT1                            ((0x0015  << 2) + 0xfe005000)
#define OTP_REE_RD_DAT2                            ((0x0016  << 2) + 0xfe005000)
#define OTP_REE_RD_DAT3                            ((0x0017  << 2) + 0xfe005000)
#define OTP_LIC0                                   ((0x0018  << 2) + 0xfe005000)
#define OTP_LIC1                                   ((0x0019  << 2) + 0xfe005000)
#define OTP_LIC2                                   ((0x001a  << 2) + 0xfe005000)
#define OTP_LIC3                                   ((0x001b  << 2) + 0xfe005000)
#define RNG_SEC_CONFIG_REG1                        ((0x0041  << 2) + 0xfe005000)
#define RNG_SEC_CONFIG_REG2                        ((0x0042  << 2) + 0xfe005000)
#define RNG_SEC_DATA                               ((0x0044  << 2) + 0xfe005000)
#define RNG_SEC_STS                                ((0x0045  << 2) + 0xfe005000)
#define RNG_USR_DATA                               ((0x0046  << 2) + 0xfe005000)
#define RNG_USR_STS                                ((0x0047  << 2) + 0xfe005000)
#define KTE_SLOT0                                  ((0x0060  << 2) + 0xfe005000)
#define KTE_SLOT1                                  ((0x0061  << 2) + 0xfe005000)
#define KTE_SLOT2                                  ((0x0062  << 2) + 0xfe005000)
#define KTE_SLOT3                                  ((0x0063  << 2) + 0xfe005000)
#define KTE_SLOT4                                  ((0x0064  << 2) + 0xfe005000)
#define KTE_SLOT5                                  ((0x0065  << 2) + 0xfe005000)
#define KTE_SLOT6                                  ((0x0066  << 2) + 0xfe005000)
#define KTE_SLOT7                                  ((0x0067  << 2) + 0xfe005000)
#define MKL_TEE_RDY                                ((0x0080  << 2) + 0xfe005000)
#define MKL_TEE_CHK                                ((0x0081  << 2) + 0xfe005000)
#define MKL_TEE_CFG                                ((0x0082  << 2) + 0xfe005000)
#define MKL_TEE_EK                                 ((0x0084  << 2) + 0xfe005000)
#define MKL_REE_RDY                                ((0x00a0  << 2) + 0xfe005000)
#define MKL_REE_CHK                                ((0x00a1  << 2) + 0xfe005000)
#define MKL_REE_CFG                                ((0x00a2  << 2) + 0xfe005000)
#define MKL_REE_EK                                 ((0x00a4  << 2) + 0xfe005000)
//
// Closing file:  ../crypto/crypto_reg.h
//
//========================================================================
//  APB0_PWM_EF - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe005400
// -----------------------------------------------
#define PWMEF_PWM_E                                ((0x0000  << 2) + 0xfe005400)
#define PWMEF_PWM_F                                ((0x0001  << 2) + 0xfe005400)
#define PWMEF_MISC_REG_EF                          ((0x0002  << 2) + 0xfe005400)
#define PWMEF_DELTA_SIGMA_EF                       ((0x0003  << 2) + 0xfe005400)
#define PWMEF_TIME_EF                              ((0x0004  << 2) + 0xfe005400)
#define PWMEF_E2                                   ((0x0005  << 2) + 0xfe005400)
#define PWMEF_F2                                   ((0x0006  << 2) + 0xfe005400)
#define PWMEF_BLINK_EF                             ((0x0007  << 2) + 0xfe005400)
#define PWMEF_LOCK_EF                              ((0x0008  << 2) + 0xfe005400)
//========================================================================
//  APB0_SYS_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe005800
// -----------------------------------------------
#define SYSCTRL_METAL_REV0                         ((0x0000  << 2) + 0xfe005800)
#define SYSCTRL_METAL_REV1                         ((0x0001  << 2) + 0xfe005800)
//`define SYSCTRL_PROD_ENABLE                     8'h0a
#define SYSCTRL_CHIP_ID                            ((0x000b  << 2) + 0xfe005800)
#define SYSCTRL_WRITEONCE1_REG0                    ((0x0010  << 2) + 0xfe005800)
#define SYSCTRL_WRITEONCE1_STICKY_REG0             ((0x0011  << 2) + 0xfe005800)
#define SYSCTRL_WRITEONCE1_SEC_REG0                ((0x0012  << 2) + 0xfe005800)
#define SYSCTRL_WRITEONCE1_STICKY_SEC_REG0         ((0x0013  << 2) + 0xfe005800)
#define SYSCTRL_WRITEONCE_REG0                     ((0x0014  << 2) + 0xfe005800)
#define SYSCTRL_WRITEONCE_REG1                     ((0x0015  << 2) + 0xfe005800)
#define SYSCTRL_WRITEONCE_SEC_REG0                 ((0x0016  << 2) + 0xfe005800)
#define SYSCTRL_WRITEONCE_STICKY_REG0              ((0x0017  << 2) + 0xfe005800)
#define SYSCTRL_WRITEONCE_STICKY_REG1              ((0x0018  << 2) + 0xfe005800)
#define SYSCTRL_WRITEONCE_STICKY_SEC_REG0          ((0x0019  << 2) + 0xfe005800)
#define SYSCTRL_TIMERA_CTRL                        ((0x0030  << 2) + 0xfe005800)
#define SYSCTRL_TIMERA                             ((0x0031  << 2) + 0xfe005800)
#define SYSCTRL_TIMERB_CTRL                        ((0x0032  << 2) + 0xfe005800)
#define SYSCTRL_TIMERB                             ((0x0033  << 2) + 0xfe005800)
#define SYSCTRL_TIMERC_CTRL                        ((0x0034  << 2) + 0xfe005800)
#define SYSCTRL_TIMERC                             ((0x0035  << 2) + 0xfe005800)
#define SYSCTRL_TIMERD_CTRL                        ((0x0036  << 2) + 0xfe005800)
#define SYSCTRL_TIMERD                             ((0x0037  << 2) + 0xfe005800)
#define SYSCTRL_SEC_TIMERA_CTRL                    ((0x0038  << 2) + 0xfe005800)
#define SYSCTRL_SEC_TIMERA                         ((0x0039  << 2) + 0xfe005800)
#define SYSCTRL_TIMERE_CTRL                        ((0x0040  << 2) + 0xfe005800)
#define SYSCTRL_TIMERE                             ((0x0041  << 2) + 0xfe005800)
#define SYSCTRL_TIMERE_HI                          ((0x0042  << 2) + 0xfe005800)
#define SYSCTRL_TIMERF_CTRL                        ((0x0043  << 2) + 0xfe005800)
#define SYSCTRL_TIMERF                             ((0x0044  << 2) + 0xfe005800)
#define SYSCTRL_TIMERF_HI                          ((0x0045  << 2) + 0xfe005800)
#define SYSCTRL_SEC_TIMERE_CTRL                    ((0x0046  << 2) + 0xfe005800)
#define SYSCTRL_SEC_TIMERE                         ((0x0047  << 2) + 0xfe005800)
#define SYSCTRL_SEC_TIMERE_HI                      ((0x0048  << 2) + 0xfe005800)
#define SYSCTRL_OSC_RING_CTRL0                     ((0x0050  << 2) + 0xfe005800)
#define SYSCTRL_SEC_DDR_AXI_SEL                    ((0x0051  << 2) + 0xfe005800)
#define SYSCTRL_AM2AXI_CTRL0                       ((0x0052  << 2) + 0xfe005800)
#define SYSCTRL_AM2AXI_STS                         ((0x0053  << 2) + 0xfe005800)
#define SYSCTRL_POC                                ((0x0060  << 2) + 0xfe005800)
//`define SYSCTRL_PROD_PWD_VALUE0                 8'h80
//`define SYSCTRL_PROD_PWD_VALUE1                 8'h81
//`define SYSCTRL_PROD_PWD_VALUE2                 8'h82
//`define SYSCTRL_PROD_PWD_VALUE3                 8'h83
//`define SYSCTRL_PROD_STATUS                     8'h84
#define SYSCTRL_DEBUG_REG0                         ((0x0090  << 2) + 0xfe005800)
#define SYSCTRL_DEBUG_REG1                         ((0x0091  << 2) + 0xfe005800)
#define SYSCTRL_DEBUG_REG2                         ((0x0092  << 2) + 0xfe005800)
#define SYSCTRL_DEBUG_REG3                         ((0x0093  << 2) + 0xfe005800)
#define SYSCTRL_DEBUG_REG4                         ((0x0094  << 2) + 0xfe005800)
#define SYSCTRL_DEBUG_REG5                         ((0x0095  << 2) + 0xfe005800)
#define SYSCTRL_DEBUG_REG6                         ((0x0096  << 2) + 0xfe005800)
#define SYSCTRL_DEBUG_REG7                         ((0x0097  << 2) + 0xfe005800)
#define SYSCTRL_STATUS_REG0                        ((0x00a0  << 2) + 0xfe005800)
#define SYSCTRL_STATUS_REG1                        ((0x00a1  << 2) + 0xfe005800)
#define SYSCTRL_STATUS_REG2                        ((0x00a2  << 2) + 0xfe005800)
#define SYSCTRL_STATUS_REG3                        ((0x00a3  << 2) + 0xfe005800)
#define SYSCTRL_STATUS_REG4                        ((0x00a4  << 2) + 0xfe005800)
#define SYSCTRL_STATUS_REG5                        ((0x00a5  << 2) + 0xfe005800)
#define SYSCTRL_STATUS_REG6                        ((0x00a6  << 2) + 0xfe005800)
#define SYSCTRL_STATUS_REG7                        ((0x00a7  << 2) + 0xfe005800)
#define SYSCTRL_STICKY_REG0                        ((0x00b0  << 2) + 0xfe005800)
#define SYSCTRL_STICKY_REG1                        ((0x00b1  << 2) + 0xfe005800)
#define SYSCTRL_STICKY_REG2                        ((0x00b2  << 2) + 0xfe005800)
#define SYSCTRL_STICKY_REG3                        ((0x00b3  << 2) + 0xfe005800)
#define SYSCTRL_STICKY_REG4                        ((0x00b4  << 2) + 0xfe005800)
#define SYSCTRL_STICKY_REG5                        ((0x00b5  << 2) + 0xfe005800)
#define SYSCTRL_STICKY_REG6                        ((0x00b6  << 2) + 0xfe005800)
#define SYSCTRL_STICKY_REG7                        ((0x00b7  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG0                    ((0x00c0  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG1                    ((0x00c1  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG2                    ((0x00c2  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG3                    ((0x00c3  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG4                    ((0x00c4  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG5                    ((0x00c5  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG6                    ((0x00c6  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG7                    ((0x00c7  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG8                    ((0x00c8  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG9                    ((0x00c9  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG10                   ((0x00ca  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG11                   ((0x00cb  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG12                   ((0x00cc  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG13                   ((0x00cd  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG14                   ((0x00ce  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG15                   ((0x00cf  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG16                   ((0x00d0  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG17                   ((0x00d1  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG18                   ((0x00d2  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG19                   ((0x00d3  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG20                   ((0x00d4  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG21                   ((0x00d5  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG22                   ((0x00d6  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STATUS_REG23                   ((0x00d7  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STICKY_REG0                    ((0x00e0  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STICKY_REG1                    ((0x00e1  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STICKY_REG2                    ((0x00e2  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STICKY_REG3                    ((0x00e3  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STICKY_REG4                    ((0x00e4  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STICKY_REG5                    ((0x00e5  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STICKY_REG6                    ((0x00e6  << 2) + 0xfe005800)
#define SYSCTRL_SEC_STICKY_REG7                    ((0x00e7  << 2) + 0xfe005800)
#define SYSCTRL_MSG_INDEX0_STICKY                  ((0x00f0  << 2) + 0xfe005800)
#define SYSCTRL_MSG_INDEX1_STICKY                  ((0x00f1  << 2) + 0xfe005800)
#define SYSCTRL_MSG_INDEX2_STICKY                  ((0x00f2  << 2) + 0xfe005800)
#define SYSCTRL_MSG_INDEX3_STICKY                  ((0x00f3  << 2) + 0xfe005800)
//========================================================================
//  APB0_I2C_M_B - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe005c00
// -----------------------------------------------
#define I2C_M_B_CONTROL_REG                        ((0x0000  << 2) + 0xfe005c00)
#define I2C_M_B_SLAVE_ADDR                         ((0x0001  << 2) + 0xfe005c00)
#define I2C_M_B_TOKEN_LIST0                        ((0x0002  << 2) + 0xfe005c00)
#define I2C_M_B_TOKEN_LIST1                        ((0x0003  << 2) + 0xfe005c00)
#define I2C_M_B_WDATA_REG0                         ((0x0004  << 2) + 0xfe005c00)
#define I2C_M_B_WDATA_REG1                         ((0x0005  << 2) + 0xfe005c00)
#define I2C_M_B_RDATA_REG0                         ((0x0006  << 2) + 0xfe005c00)
#define I2C_M_B_RDATA_REG1                         ((0x0007  << 2) + 0xfe005c00)
#define I2C_M_B_TIMEOUT_TH                         ((0x0008  << 2) + 0xfe005c00)
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
#define IRQCTRL_IRQOUT_MASK0                       ((0x0000  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_MASK1                       ((0x0001  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_MASK2                       ((0x0002  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_MASK3                       ((0x0003  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_INV0                        ((0x0010  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_INV1                        ((0x0011  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_INV2                        ((0x0012  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_INV3                        ((0x0013  << 2) + 0xfe006400)
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
#define I2C_M_C_CONTROL_REG                        ((0x0000  << 2) + 0xfe006800)
#define I2C_M_C_SLAVE_ADDR                         ((0x0001  << 2) + 0xfe006800)
#define I2C_M_C_TOKEN_LIST0                        ((0x0002  << 2) + 0xfe006800)
#define I2C_M_C_TOKEN_LIST1                        ((0x0003  << 2) + 0xfe006800)
#define I2C_M_C_WDATA_REG0                         ((0x0004  << 2) + 0xfe006800)
#define I2C_M_C_WDATA_REG1                         ((0x0005  << 2) + 0xfe006800)
#define I2C_M_C_RDATA_REG0                         ((0x0006  << 2) + 0xfe006800)
#define I2C_M_C_RDATA_REG1                         ((0x0007  << 2) + 0xfe006800)
#define I2C_M_C_TIMEOUT_TH                         ((0x0008  << 2) + 0xfe006800)
//========================================================================
//  APB0_I2C_M - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe006c00
// -----------------------------------------------
#define I2C_M_D_CONTROL_REG                        ((0x0000  << 2) + 0xfe006c00)
#define I2C_M_D_SLAVE_ADDR                         ((0x0001  << 2) + 0xfe006c00)
#define I2C_M_D_TOKEN_LIST0                        ((0x0002  << 2) + 0xfe006c00)
#define I2C_M_D_TOKEN_LIST1                        ((0x0003  << 2) + 0xfe006c00)
#define I2C_M_D_WDATA_REG0                         ((0x0004  << 2) + 0xfe006c00)
#define I2C_M_D_WDATA_REG1                         ((0x0005  << 2) + 0xfe006c00)
#define I2C_M_D_RDATA_REG0                         ((0x0006  << 2) + 0xfe006c00)
#define I2C_M_D_RDATA_REG1                         ((0x0007  << 2) + 0xfe006c00)
#define I2C_M_D_TIMEOUT_TH                         ((0x0008  << 2) + 0xfe006c00)
//========================================================================
//  APB0_UART_C - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe007000
// -----------------------------------------------
#define UART_C_WFIFO                               ((0x0000  << 2) + 0xfe007000)
#define UART_C_RFIFO                               ((0x0001  << 2) + 0xfe007000)
#define UART_C_CONTROL                             ((0x0002  << 2) + 0xfe007000)
#define UART_C_STATUS                              ((0x0003  << 2) + 0xfe007000)
#define UART_C_MISC                                ((0x0004  << 2) + 0xfe007000)
#define UART_C_REG5                                ((0x0005  << 2) + 0xfe007000)
//========================================================================
//  APB0_PWR_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe007800
// -----------------------------------------------
#define PWRCTRL_PWR_ACK0                           ((0x0000  << 2) + 0xfe007800)
#define PWRCTRL_ISO_EN0                            ((0x0001  << 2) + 0xfe007800)
#define PWRCTRL_PWR_OFF0                           ((0x0002  << 2) + 0xfe007800)
#define PWRCTRL_SAVE_EN0                           ((0x0003  << 2) + 0xfe007800)
#define PWRCTRL_RESTORE_EN0                        ((0x0004  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD0                            ((0x0010  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD1                            ((0x0011  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD2                            ((0x0012  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD3                            ((0x0013  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD4                            ((0x0014  << 2) + 0xfe007800)
#define PWRCTRL_R_ISO_EN0                          ((0x0020  << 2) + 0xfe007800)
#define PWRCTRL_R_PWR_OFF0                         ((0x0021  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD0                          ((0x0022  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD1                          ((0x0023  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD2                          ((0x0024  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD3                          ((0x0025  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD4                          ((0x0026  << 2) + 0xfe007800)
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
#define PWRCTRL_AUTO_OFF_CTRL                      ((0x0030  << 2) + 0xfe007800)
#define PWRCTRL_AUTO_OFF                           ((0x0031  << 2) + 0xfe007800)
#define PWRCTRL_AUTO_OFF_STS                       ((0x0032  << 2) + 0xfe007800)
#define PWRCTRL_TIMER_TH_01                        ((0x0033  << 2) + 0xfe007800)
#define PWRCTRL_TIMER_TH_23                        ((0x0034  << 2) + 0xfe007800)
#define PWRCTRL_TIMER_TH_45                        ((0x0035  << 2) + 0xfe007800)
#define PWRCTRL_TIMER_TH_67                        ((0x0036  << 2) + 0xfe007800)
#define PWRCTRL_MASK_PWR_ACK0                      ((0x0040  << 2) + 0xfe007800)
#define PWRCTRL_MASK_RSTN_OFF0                     ((0x0041  << 2) + 0xfe007800)
#define PWRCTRL_MASK_RSTN_ON0                      ((0x0042  << 2) + 0xfe007800)
#define PWRCTRL_MASK_PWR_OFF0                      ((0x0043  << 2) + 0xfe007800)
#define PWRCTRL_MASK_PWR_ON0                       ((0x0044  << 2) + 0xfe007800)
#define PWRCTRL_MASK_ISO_OFF0                      ((0x0045  << 2) + 0xfe007800)
#define PWRCTRL_MASK_ISO_ON0                       ((0x0046  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF0                      ((0x0050  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF1                      ((0x0051  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF2                      ((0x0052  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF3                      ((0x0053  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF4                      ((0x0054  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON0                       ((0x0058  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON1                       ((0x0059  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON2                       ((0x005a  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON3                       ((0x005b  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON4                       ((0x005c  << 2) + 0xfe007800)
//========================================================================
//  APB0_ANA_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe007c00
// -----------------------------------------------
#define ANACTRL_FIXPLL_CTRL0                       ((0x0020  << 2) + 0xfe007c00)
#define ANACTRL_FIXPLL_CTRL1                       ((0x0021  << 2) + 0xfe007c00)
#define ANACTRL_FIXPLL_CTRL2                       ((0x0022  << 2) + 0xfe007c00)
#define ANACTRL_FIXPLL_CTRL3                       ((0x0023  << 2) + 0xfe007c00)
#define ANACTRL_FIXPLL_CTRL4                       ((0x0024  << 2) + 0xfe007c00)
#define ANACTRL_FIXPLL_STS                         ((0x0025  << 2) + 0xfe007c00)
#define ANACTRL_SYSPLL_CTRL0                       ((0x0040  << 2) + 0xfe007c00)
#define ANACTRL_SYSPLL_CTRL1                       ((0x0041  << 2) + 0xfe007c00)
#define ANACTRL_SYSPLL_CTRL2                       ((0x0042  << 2) + 0xfe007c00)
#define ANACTRL_SYSPLL_CTRL3                       ((0x0043  << 2) + 0xfe007c00)
#define ANACTRL_SYSPLL_CTRL4                       ((0x0044  << 2) + 0xfe007c00)
#define ANACTRL_SYSPLL_STS                         ((0x0045  << 2) + 0xfe007c00)
#define ANACTRL_HIFIPLL_CTRL0                      ((0x0050  << 2) + 0xfe007c00)
#define ANACTRL_HIFIPLL_CTRL1                      ((0x0051  << 2) + 0xfe007c00)
#define ANACTRL_HIFIPLL_CTRL2                      ((0x0052  << 2) + 0xfe007c00)
#define ANACTRL_HIFIPLL_CTRL3                      ((0x0053  << 2) + 0xfe007c00)
#define ANACTRL_HIFIPLL_CTRL4                      ((0x0054  << 2) + 0xfe007c00)
#define ANACTRL_HIFIPLL_STS                        ((0x0055  << 2) + 0xfe007c00)
#define ANACTRL_AUDDDS_CTRL0                       ((0x0060  << 2) + 0xfe007c00)
#define ANACTRL_AUDDDS_CTRL1                       ((0x0061  << 2) + 0xfe007c00)
#define ANACTRL_AUDDDS_CTRL2                       ((0x0062  << 2) + 0xfe007c00)
#define ANACTRL_AUDDDS_CTRL3                       ((0x0063  << 2) + 0xfe007c00)
#define ANACTRL_AUDDDS_CTRL4                       ((0x0064  << 2) + 0xfe007c00)
#define ANACTRL_AUDDDS_STS                         ((0x0065  << 2) + 0xfe007c00)
#define ANACTRL_MISCTOP_CTRL0                      ((0x0070  << 2) + 0xfe007c00)
#define ANACTRL_POR_CNTL                           ((0x0082  << 2) + 0xfe007c00)
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe020000
// -----------------------------------------------
#define RSA_BASE                                   ((0x0000  << 2) + 0xfe020000)
//========================================================================
//  CPUCTRL_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfd000000
// -----------------------------------------------
#define CPUCTRL_CTRL0                              ((0x0000  << 2) + 0xfd000000)
#define CPUCTRL_CTRL1                              ((0x0001  << 2) + 0xfd000000)
#define CPUCTRL_CTRL2                              ((0x0002  << 2) + 0xfd000000)
#define CPUCTRL_CTRL3                              ((0x0003  << 2) + 0xfd000000)
#define CPUCTRL_CTRL4                              ((0x0004  << 2) + 0xfd000000)
#define CPUCTRL_CTRL5                              ((0x0005  << 2) + 0xfd000000)
#define CPUCTRL_CTRL6                              ((0x0006  << 2) + 0xfd000000)
#define CPUCTRL_CTRL7                              ((0x0007  << 2) + 0xfd000000)
#define CPUCTRL_CTRL8                              ((0x0008  << 2) + 0xfd000000)
#define CPUCTRL_CTRL9                              ((0x0009  << 2) + 0xfd000000)
#define CPUCTRL_CTRL10                             ((0x000a  << 2) + 0xfd000000)
#define CPUCTRL_CTRL11                             ((0x000b  << 2) + 0xfd000000)
#define CPUCTRL_CTRL12                             ((0x000c  << 2) + 0xfd000000)
#define CPUCTRL_CTRL13                             ((0x000d  << 2) + 0xfd000000)
#define CPUCTRL_CTRL14                             ((0x000e  << 2) + 0xfd000000)
#define CPUCTRL_CTRL15                             ((0x000f  << 2) + 0xfd000000)
#define CPUCTRL_STS0                               ((0x0010  << 2) + 0xfd000000)
#define CPUCTRL_STS1                               ((0x0011  << 2) + 0xfd000000)
#define CPUCTRL_STS2                               ((0x0012  << 2) + 0xfd000000)
#define CPUCTRL_STS3                               ((0x0013  << 2) + 0xfd000000)
#define CPUCTRL_STS4                               ((0x0014  << 2) + 0xfd000000)
#define CPUCTRL_STS5                               ((0x0015  << 2) + 0xfd000000)
#define CPUCTRL_STS6                               ((0x0016  << 2) + 0xfd000000)
#define CPUCTRL_STS7                               ((0x0017  << 2) + 0xfd000000)
#define CPUCTRL_STS8                               ((0x0018  << 2) + 0xfd000000)
#define CPUCTRL_STS9                               ((0x0019  << 2) + 0xfd000000)
#define CPUCTRL_CLK_CTRL0                          ((0x0020  << 2) + 0xfd000000)
#define CPUCTRL_CLK_CTRL1                          ((0x0021  << 2) + 0xfd000000)
#define CPUCTRL_CLK_CTRL2                          ((0x0022  << 2) + 0xfd000000)
#define CPUCTRL_CLK_CTRL3                          ((0x0023  << 2) + 0xfd000000)
#define CPUCTRL_CLK_CTRL4                          ((0x0024  << 2) + 0xfd000000)
#define CPUCTRL_CLK_CTRL5                          ((0x0025  << 2) + 0xfd000000)
#define CPUCTRL_CLK_CTRL6                          ((0x0026  << 2) + 0xfd000000)
#define CPUCTRL_CLK_CTRL7                          ((0x0027  << 2) + 0xfd000000)
#define CPUCTRL_RESET_CTRL                         ((0x0030  << 2) + 0xfd000000)
#define CPUCTRL_ROM_DISABLE                        ((0x0031  << 2) + 0xfd000000)
//========================================================================
//  RAMA_SEC_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfd000800
// -----------------------------------------------
#define SRAM_RANGE0_STA                            ((0x0000  << 2) + 0xfd000800)
#define SRAM_RANGE1_STA                            ((0x0001  << 2) + 0xfd000800)
#define SRAM_RANGE2_STA                            ((0x0002  << 2) + 0xfd000800)
#define SRAM_RANGE3_STA                            ((0x0003  << 2) + 0xfd000800)
#define SRAM_RANGE4_STA                            ((0x0004  << 2) + 0xfd000800)
#define SRAM_RANGE5_STA                            ((0x0005  << 2) + 0xfd000800)
#define SRAM_RANGE6_STA                            ((0x0006  << 2) + 0xfd000800)
#define SRAM_RANGE0_EDA                            ((0x0007  << 2) + 0xfd000800)
#define SRAM_RANGE1_EDA                            ((0x0008  << 2) + 0xfd000800)
#define SRAM_RANGE2_EDA                            ((0x0009  << 2) + 0xfd000800)
#define SRAM_RANGE3_EDA                            ((0x000a  << 2) + 0xfd000800)
#define SRAM_RANGE4_EDA                            ((0x000b  << 2) + 0xfd000800)
#define SRAM_RANGE5_EDA                            ((0x000c  << 2) + 0xfd000800)
#define SRAM_RANGE6_EDA                            ((0x000d  << 2) + 0xfd000800)
#define SRAM_RANGE0_CTRL0                          ((0x000e  << 2) + 0xfd000800)
#define SRAM_RANGE1_CTRL0                          ((0x000f  << 2) + 0xfd000800)
#define SRAM_RANGE2_CTRL0                          ((0x0010  << 2) + 0xfd000800)
#define SRAM_RANGE3_CTRL0                          ((0x0011  << 2) + 0xfd000800)
#define SRAM_RANGE4_CTRL0                          ((0x0012  << 2) + 0xfd000800)
#define SRAM_RANGE5_CTRL0                          ((0x0013  << 2) + 0xfd000800)
#define SRAM_RANGE6_CTRL0                          ((0x0014  << 2) + 0xfd000800)
#define SRAM_RANGE7_CTRL0                          ((0x0015  << 2) + 0xfd000800)
#define SRAM_RANGE0_CTRL1                          ((0x0016  << 2) + 0xfd000800)
#define SRAM_RANGE1_CTRL1                          ((0x0017  << 2) + 0xfd000800)
#define SRAM_RANGE2_CTRL1                          ((0x0018  << 2) + 0xfd000800)
#define SRAM_RANGE3_CTRL1                          ((0x0019  << 2) + 0xfd000800)
#define SRAM_RANGE4_CTRL1                          ((0x001a  << 2) + 0xfd000800)
#define SRAM_RANGE5_CTRL1                          ((0x001b  << 2) + 0xfd000800)
#define SRAM_RANGE6_CTRL1                          ((0x001c  << 2) + 0xfd000800)
#define SRAM_RANGE7_CTRL1                          ((0x001d  << 2) + 0xfd000800)
#define SRAM_SEC_CTRL0                             ((0x001e  << 2) + 0xfd000800)
#define SRAM_REG_LOCK                              ((0x001f  << 2) + 0xfd000800)
//========================================================================
//  RAMB_SEC_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfd000c00
// -----------------------------------------------
#define SRAM_RANGE0_STA_B                          ((0x0000  << 2) + 0xfd000c00)
#define SRAM_RANGE1_STA_B                          ((0x0001  << 2) + 0xfd000c00)
#define SRAM_RANGE2_STA_B                          ((0x0002  << 2) + 0xfd000c00)
#define SRAM_RANGE3_STA_B                          ((0x0003  << 2) + 0xfd000c00)
#define SRAM_RANGE4_STA_B                          ((0x0004  << 2) + 0xfd000c00)
#define SRAM_RANGE5_STA_B                          ((0x0005  << 2) + 0xfd000c00)
#define SRAM_RANGE6_STA_B                          ((0x0006  << 2) + 0xfd000c00)
#define SRAM_RANGE0_EDA_B                          ((0x0007  << 2) + 0xfd000c00)
#define SRAM_RANGE1_EDA_B                          ((0x0008  << 2) + 0xfd000c00)
#define SRAM_RANGE2_EDA_B                          ((0x0009  << 2) + 0xfd000c00)
#define SRAM_RANGE3_EDA_B                          ((0x000a  << 2) + 0xfd000c00)
#define SRAM_RANGE4_EDA_B                          ((0x000b  << 2) + 0xfd000c00)
#define SRAM_RANGE5_EDA_B                          ((0x000c  << 2) + 0xfd000c00)
#define SRAM_RANGE6_EDA_B                          ((0x000d  << 2) + 0xfd000c00)
#define SRAM_RANGE0_CTRL0_B                        ((0x000e  << 2) + 0xfd000c00)
#define SRAM_RANGE1_CTRL0_B                        ((0x000f  << 2) + 0xfd000c00)
#define SRAM_RANGE2_CTRL0_B                        ((0x0010  << 2) + 0xfd000c00)
#define SRAM_RANGE3_CTRL0_B                        ((0x0011  << 2) + 0xfd000c00)
#define SRAM_RANGE4_CTRL0_B                        ((0x0012  << 2) + 0xfd000c00)
#define SRAM_RANGE5_CTRL0_B                        ((0x0013  << 2) + 0xfd000c00)
#define SRAM_RANGE6_CTRL0_B                        ((0x0014  << 2) + 0xfd000c00)
#define SRAM_RANGE7_CTRL0_B                        ((0x0015  << 2) + 0xfd000c00)
#define SRAM_RANGE0_CTRL1_B                        ((0x0016  << 2) + 0xfd000c00)
#define SRAM_RANGE1_CTRL1_B                        ((0x0017  << 2) + 0xfd000c00)
#define SRAM_RANGE2_CTRL1_B                        ((0x0018  << 2) + 0xfd000c00)
#define SRAM_RANGE3_CTRL1_B                        ((0x0019  << 2) + 0xfd000c00)
#define SRAM_RANGE4_CTRL1_B                        ((0x001a  << 2) + 0xfd000c00)
#define SRAM_RANGE5_CTRL1_B                        ((0x001b  << 2) + 0xfd000c00)
#define SRAM_RANGE6_CTRL1_B                        ((0x001c  << 2) + 0xfd000c00)
#define SRAM_RANGE7_CTRL1_B                        ((0x001d  << 2) + 0xfd000c00)
#define SRAM_SEC_CTRL0_B                           ((0x001e  << 2) + 0xfd000c00)
#define SRAM_REG_LOCK_B                            ((0x001f  << 2) + 0xfd000c00)
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
#define DSP_CFG0                                   ((0x0000  << 2) + 0xfe030000)
#define DSP_CFG1                                   ((0x0001  << 2) + 0xfe030000)
#define DSP_CFG2                                   ((0x0002  << 2) + 0xfe030000)
#define DSP_IMPWIRE                                ((0x0003  << 2) + 0xfe030000)
#define DSP_RESET_VEC                              ((0x0004  << 2) + 0xfe030000)
#define DSP_SEC_CFG0                               ((0x0006  << 2) + 0xfe030000)
#define DSP_SEC_CFG1                               ((0x0007  << 2) + 0xfe030000)
#define DSP_IRQ_CTRL0                              ((0x0010  << 2) + 0xfe030000)
#define DSP_IRQ_CTRL1                              ((0x0011  << 2) + 0xfe030000)
#define DSP_IRQ_CTRL2                              ((0x0012  << 2) + 0xfe030000)
#define DSP_IRQ_CTRL3                              ((0x0013  << 2) + 0xfe030000)
#define DSP_IRQ_CTRL4                              ((0x0014  << 2) + 0xfe030000)
#define DSP_IRQ_CTRL5                              ((0x0015  << 2) + 0xfe030000)
#define DSP_IRQ_CTRL6                              ((0x0016  << 2) + 0xfe030000)
#define DSP_IRQ_CTRL7                              ((0x0017  << 2) + 0xfe030000)
#define DSP_IRQ_CTRL8                              ((0x0018  << 2) + 0xfe030000)
#define DSP_IRQ_STS                                ((0x001f  << 2) + 0xfe030000)
#define DSP_REMAP0                                 ((0x0020  << 2) + 0xfe030000)
#define DSP_REMAP1                                 ((0x0021  << 2) + 0xfe030000)
#define DSP_REMAP2                                 ((0x0022  << 2) + 0xfe030000)
#define DSP_STS0                                   ((0x0040  << 2) + 0xfe030000)
#define DSP_STS1                                   ((0x0041  << 2) + 0xfe030000)
#define DSP_STS2                                   ((0x0042  << 2) + 0xfe030000)
#define DSP_STS3                                   ((0x0043  << 2) + 0xfe030000)
#define DSP_STS4                                   ((0x0044  << 2) + 0xfe030000)
#define DSP_STS5                                   ((0x0045  << 2) + 0xfe030000)
#define DSP_MAILBOX_SET_0                          ((0x0050  << 2) + 0xfe030000)
#define DSP_MAILBOX_SET_1                          ((0x0051  << 2) + 0xfe030000)
#define DSP_MAILBOX_SET_2                          ((0x0052  << 2) + 0xfe030000)
#define DSP_MAILBOX_SET_3                          ((0x0053  << 2) + 0xfe030000)
#define DSP_MAILBOX_SET_4                          ((0x0054  << 2) + 0xfe030000)
#define DSP_MAILBOX_SET_5                          ((0x0055  << 2) + 0xfe030000)
#define DSP_MAILBOX_SET_6                          ((0x0056  << 2) + 0xfe030000)
#define DSP_MAILBOX_SET_7                          ((0x0057  << 2) + 0xfe030000)
#define DSP_MAILBOX_SET_8                          ((0x0058  << 2) + 0xfe030000)
#define DSP_MAILBOX_SET_9                          ((0x0059  << 2) + 0xfe030000)
#define DSP_MAILBOX_SET_10                         ((0x005a  << 2) + 0xfe030000)
#define DSP_MAILBOX_SET_11                         ((0x005b  << 2) + 0xfe030000)
#define DSP_MAILBOX_CLR_0                          ((0x0060  << 2) + 0xfe030000)
#define DSP_MAILBOX_CLR_1                          ((0x0061  << 2) + 0xfe030000)
#define DSP_MAILBOX_CLR_2                          ((0x0062  << 2) + 0xfe030000)
#define DSP_MAILBOX_CLR_3                          ((0x0063  << 2) + 0xfe030000)
#define DSP_MAILBOX_CLR_4                          ((0x0064  << 2) + 0xfe030000)
#define DSP_MAILBOX_CLR_5                          ((0x0065  << 2) + 0xfe030000)
#define DSP_MAILBOX_CLR_6                          ((0x0066  << 2) + 0xfe030000)
#define DSP_MAILBOX_CLR_7                          ((0x0067  << 2) + 0xfe030000)
#define DSP_MAILBOX_CLR_8                          ((0x0068  << 2) + 0xfe030000)
#define DSP_MAILBOX_CLR_9                          ((0x0069  << 2) + 0xfe030000)
#define DSP_MAILBOX_CLR_10                         ((0x006a  << 2) + 0xfe030000)
#define DSP_MAILBOX_CLR_11                         ((0x006b  << 2) + 0xfe030000)
#define DSP_MAILBOX_STAT_0                         ((0x0070  << 2) + 0xfe030000)
#define DSP_MAILBOX_STAT_1                         ((0x0071  << 2) + 0xfe030000)
#define DSP_MAILBOX_STAT_2                         ((0x0072  << 2) + 0xfe030000)
#define DSP_MAILBOX_STAT_3                         ((0x0073  << 2) + 0xfe030000)
#define DSP_MAILBOX_STAT_4                         ((0x0074  << 2) + 0xfe030000)
#define DSP_MAILBOX_STAT_5                         ((0x0075  << 2) + 0xfe030000)
#define DSP_MAILBOX_STAT_6                         ((0x0076  << 2) + 0xfe030000)
#define DSP_MAILBOX_STAT_7                         ((0x0077  << 2) + 0xfe030000)
#define DSP_MAILBOX_STAT_8                         ((0x0078  << 2) + 0xfe030000)
#define DSP_MAILBOX_STAT_9                         ((0x0079  << 2) + 0xfe030000)
#define DSP_MAILBOX_STAT_10                        ((0x007a  << 2) + 0xfe030000)
#define DSP_MAILBOX_STAT_11                        ((0x007b  << 2) + 0xfe030000)
#define DSP_QIF_CTRL                               ((0x0080  << 2) + 0xfe030000)
#define DSP_QIF_STS                                ((0x0081  << 2) + 0xfe030000)
#define DSP_WRFIFO_TOCPUA                          ((0x0082  << 2) + 0xfe030000)
#define DSP_WRFIFO_TOCPUB                          ((0x0083  << 2) + 0xfe030000)
#define DSP_WRFIFO_TODSP                           ((0x0084  << 2) + 0xfe030000)
#define DSP_RDFIFO_FRCPUA                          ((0x0088  << 2) + 0xfe030000)
#define DSP_RDFIFO_FRCPUB                          ((0x0089  << 2) + 0xfe030000)
#define DSP_RDFIFO_FRDSP                           ((0x008a  << 2) + 0xfe030000)
//========================================================================
//  DSPB - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe040000
// -----------------------------------------------
#define DSPB_CFG0                                  ((0x0000  << 2) + 0xfe040000)
#define DSPB_CFG1                                  ((0x0001  << 2) + 0xfe040000)
#define DSPB_CFG2                                  ((0x0002  << 2) + 0xfe040000)
#define DSPB_IMPWIRE                               ((0x0003  << 2) + 0xfe040000)
#define DSPB_RESET_VEC                             ((0x0004  << 2) + 0xfe040000)
#define DSPB_SEC_CFG0                              ((0x0006  << 2) + 0xfe040000)
#define DSPB_SEC_CFG1                              ((0x0007  << 2) + 0xfe040000)
#define DSPB_IRQ_CTRL0                             ((0x0010  << 2) + 0xfe040000)
#define DSPB_IRQ_CTRL1                             ((0x0011  << 2) + 0xfe040000)
#define DSPB_IRQ_CTRL2                             ((0x0012  << 2) + 0xfe040000)
#define DSPB_IRQ_CTRL3                             ((0x0013  << 2) + 0xfe040000)
#define DSPB_IRQ_CTRL4                             ((0x0014  << 2) + 0xfe040000)
#define DSPB_IRQ_CTRL5                             ((0x0015  << 2) + 0xfe040000)
#define DSPB_IRQ_CTRL6                             ((0x0016  << 2) + 0xfe040000)
#define DSPB_IRQ_CTRL7                             ((0x0017  << 2) + 0xfe040000)
#define DSPB_IRQ_CTRL8                             ((0x0018  << 2) + 0xfe040000)
#define DSPB_IRQ_STS                               ((0x001f  << 2) + 0xfe040000)
#define DSPB_REMAP0                                ((0x0020  << 2) + 0xfe040000)
#define DSPB_REMAP1                                ((0x0021  << 2) + 0xfe040000)
#define DSPB_REMAP2                                ((0x0022  << 2) + 0xfe040000)
#define DSPB_STS0                                  ((0x0040  << 2) + 0xfe040000)
#define DSPB_STS1                                  ((0x0041  << 2) + 0xfe040000)
#define DSPB_STS2                                  ((0x0042  << 2) + 0xfe040000)
#define DSPB_STS3                                  ((0x0043  << 2) + 0xfe040000)
#define DSPB_STS4                                  ((0x0044  << 2) + 0xfe040000)
#define DSPB_STS5                                  ((0x0045  << 2) + 0xfe040000)
#define DSPB_MAILBOX_SET_0                         ((0x0050  << 2) + 0xfe040000)
#define DSPB_MAILBOX_SET_1                         ((0x0051  << 2) + 0xfe040000)
#define DSPB_MAILBOX_SET_2                         ((0x0052  << 2) + 0xfe040000)
#define DSPB_MAILBOX_SET_3                         ((0x0053  << 2) + 0xfe040000)
#define DSPB_MAILBOX_SET_4                         ((0x0054  << 2) + 0xfe040000)
#define DSPB_MAILBOX_SET_5                         ((0x0055  << 2) + 0xfe040000)
#define DSPB_MAILBOX_SET_6                         ((0x0056  << 2) + 0xfe040000)
#define DSPB_MAILBOX_SET_7                         ((0x0057  << 2) + 0xfe040000)
#define DSPB_MAILBOX_SET_8                         ((0x0058  << 2) + 0xfe040000)
#define DSPB_MAILBOX_SET_9                         ((0x0059  << 2) + 0xfe040000)
#define DSPB_MAILBOX_SET_10                        ((0x005a  << 2) + 0xfe040000)
#define DSPB_MAILBOX_SET_11                        ((0x005b  << 2) + 0xfe040000)
#define DSPB_MAILBOX_CLR_0                         ((0x0060  << 2) + 0xfe040000)
#define DSPB_MAILBOX_CLR_1                         ((0x0061  << 2) + 0xfe040000)
#define DSPB_MAILBOX_CLR_2                         ((0x0062  << 2) + 0xfe040000)
#define DSPB_MAILBOX_CLR_3                         ((0x0063  << 2) + 0xfe040000)
#define DSPB_MAILBOX_CLR_4                         ((0x0064  << 2) + 0xfe040000)
#define DSPB_MAILBOX_CLR_5                         ((0x0065  << 2) + 0xfe040000)
#define DSPB_MAILBOX_CLR_6                         ((0x0066  << 2) + 0xfe040000)
#define DSPB_MAILBOX_CLR_7                         ((0x0067  << 2) + 0xfe040000)
#define DSPB_MAILBOX_CLR_8                         ((0x0068  << 2) + 0xfe040000)
#define DSPB_MAILBOX_CLR_9                         ((0x0069  << 2) + 0xfe040000)
#define DSPB_MAILBOX_CLR_10                        ((0x006a  << 2) + 0xfe040000)
#define DSPB_MAILBOX_CLR_11                        ((0x006b  << 2) + 0xfe040000)
#define DSPB_MAILBOX_STAT_0                        ((0x0070  << 2) + 0xfe040000)
#define DSPB_MAILBOX_STAT_1                        ((0x0071  << 2) + 0xfe040000)
#define DSPB_MAILBOX_STAT_2                        ((0x0072  << 2) + 0xfe040000)
#define DSPB_MAILBOX_STAT_3                        ((0x0073  << 2) + 0xfe040000)
#define DSPB_MAILBOX_STAT_4                        ((0x0074  << 2) + 0xfe040000)
#define DSPB_MAILBOX_STAT_5                        ((0x0075  << 2) + 0xfe040000)
#define DSPB_MAILBOX_STAT_6                        ((0x0076  << 2) + 0xfe040000)
#define DSPB_MAILBOX_STAT_7                        ((0x0077  << 2) + 0xfe040000)
#define DSPB_MAILBOX_STAT_8                        ((0x0078  << 2) + 0xfe040000)
#define DSPB_MAILBOX_STAT_9                        ((0x0079  << 2) + 0xfe040000)
#define DSPB_MAILBOX_STAT_10                       ((0x007a  << 2) + 0xfe040000)
#define DSPB_MAILBOX_STAT_11                       ((0x007b  << 2) + 0xfe040000)
#define DSPB_QIF_CTRL                              ((0x0080  << 2) + 0xfe040000)
#define DSPB_QIF_STS                               ((0x0081  << 2) + 0xfe040000)
#define DSPB_WRFIFO_TOCPUA                         ((0x0082  << 2) + 0xfe040000)
#define DSPB_WRFIFO_TOCPUB                         ((0x0083  << 2) + 0xfe040000)
#define DSPB_WRFIFO_TODSP                          ((0x0084  << 2) + 0xfe040000)
#define DSPB_RDFIFO_FRCPUA                         ((0x0088  << 2) + 0xfe040000)
#define DSPB_RDFIFO_FRCPUB                         ((0x0089  << 2) + 0xfe040000)
#define DSPB_RDFIFO_FRDSP                          ((0x008a  << 2) + 0xfe040000)
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
#include "soc_def.h"
#endif // REGISTER_H

