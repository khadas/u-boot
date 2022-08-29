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
#define AUDIO_RSAMP_RO_ADJ_PHS_ERR_VAR             ((0x0018  << 2) + 0xfe051c00)
//Bit   31:0       ro_det_phase_err_var
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
//`define SPIFC BASE ADDR      32'hfe00ac00
// -----------------------------------------------
// APB_BASE:  APB2_BASE_ADDR = 0xfe00ac00
// -----------------------------------------------
//AHB domain regsiter.
#define SPIFC_AHB_CTRL                             ((0x0000  << 2) + 0xfe00ac00)
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
#define SPIFC_CLK_CTRL                             ((0x0001  << 2) + 0xfe00ac00)
  //bit 14   asynchronous buffer ahb clock disable.  1 = disable. 0 = enable.
  //bit 13   ahb2spi ahb clock disable.  1 = disable. 0 = enable.
  //bit 12   ahb_arb ahb clock disable.  1 = disable. 0 = enable.
  //bit 10   asynchronous buffer ahb clock auto gating enable.  1 = enable. 0 = disable.
  //bit 9    ahb2spi ahb clock auto gating enable.  1 = enable. 0 = disable.
  //bit 8    ahbarb  ahb clock auto gating enable.  1 = enable. 0 = disable.
  //bit 2.    asynchronous AHB clock domain software reset.  1 = reset. 0 = normal working mode.
  //bit 1.    ahb2spi ahb clock domain software reset.  1 = reset. 0 = normal working mode.
  //bit 0.    not used.
#define SPIFC_SEC_CTRL                             ((0x0002  << 2) + 0xfe00ac00)
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
#define SPIFC_RANGE0_STA                           ((0x0010  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 0  start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE0_EDA                           ((0x0011  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 0  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE0_CTRL                          ((0x0012  << 2) + 0xfe00ac00)
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
#define SPIFC_RANGE1_STA                           ((0x0013  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 1  start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE1_EDA                           ((0x0014  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 1  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE1_CTRL                          ((0x0015  << 2) + 0xfe00ac00)
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
#define SPIFC_RANGE2_STA                           ((0x0016  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 2  start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE2_EDA                           ((0x0017  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 2  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE2_CTRL                          ((0x0018  << 2) + 0xfe00ac00)
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
#define SPIFC_RANGE3_STA                           ((0x0019  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 3  start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE3_EDA                           ((0x001a  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 3  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE3_CTRL                          ((0x001b  << 2) + 0xfe00ac00)
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
#define SPIFC_RANGE4_STA                           ((0x001c  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 4  start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE4_EDA                           ((0x001d  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 4  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE4_CTRL                          ((0x001e  << 2) + 0xfe00ac00)
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
#define SPIFC_RANGE5_STA                           ((0x001f  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 5 start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE5_EDA                           ((0x0021  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 5  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE5_CTRL                          ((0x0020  << 2) + 0xfe00ac00)
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
#define SPIFC_RANGE6_STA                           ((0x0022  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 6 start address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE6_EDA                           ((0x0023  << 2) + 0xfe00ac00)
  //bit 28 :6.
  //AHB security range 6  end address in unit of 64Byte. related to HADDR bit 28:6.
  //bit 5:0.  always 0.
#define SPIFC_RANGE6_CTRL                          ((0x0024  << 2) + 0xfe00ac00)
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
#define SPIFC_RANGE7_CTRL                          ((0x0025  << 2) + 0xfe00ac00)
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
#define SPIFC_AHB_WTCH_CTRL                        ((0x0026  << 2) + 0xfe00ac00)
  //bit 15:0.   ahb bus watch dog timer. if AHB bus not response, the timer over this number, the SPIFC will send back  a error response.
#define SPIFC_SEC_VIO0                             ((0x0027  << 2) + 0xfe00ac00)
  //bit 31:0  VIO ADDR
#define SPIFC_SEC_VIO1                             ((0x0028  << 2) + 0xfe00ac00)
  //bit  31.  AHB VIO status.  write 1 to clear the error info.
  //bit 6:5  HMASTER.
  //bit 4:2  HPROT.
  //bit 1    HNONSEC.
  //bit 0    HWRITE.
#define SPIFC_AHB_STS                              ((0x0029  << 2) + 0xfe00ac00)
  //bit 31.  AHB IDLE.  when ahb disabled, to show ahb status. 1,  no more ahb request or ahb frequent is holded.  0, ahb is still busy.
  //bit 30.  AHB data cycle status, when ahb disabled, to show ahb data cycle status. 1, data cycle is holded,  0 : not in data cycle or data cycle is processing.
  //bit 29.  AHB bus spi requst status. 1: still waiting for spi data. 0: idle.
  //bit 28:0.  not used.
//SPI cts_spi_clk domain regsiter.
#define SPIFC_USER_CTRL0                           ((0x0080  << 2) + 0xfe00ac00)
  //bit 31.    user request enable.  write 1 to enable.  read 0, means user command accepted by the SPI_CTRL.
  //bit 30.    user request finish bit.  write 0 when enable user request.  read  1 means SPI_CTRL finished this user command.
  //bit 0.     user data updated.  write 0 when enable user request.   1 means there's datas/status read from SPI flash.
#define SPIFC_USER_CTRL1                           ((0x0081  << 2) + 0xfe00ac00)
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
#define SPIFC_USER_CTRL2                           ((0x0082  << 2) + 0xfe00ac00)
  //bit 31 usr_dummy_en  1: enable dummy cycles.  0 : no dummy cycles.
  //bit 30: 29 usr_dummy_mode  2'b00 = SPI extend mode. 01 dual mode. 10 Quad mode.
  //bit 28:23  user dummy clock cycle number.
  //bit 19:16. user command input/output direction control.  000000: right after the last output.
  //          1: after the first cycle of dummy cycle.
  //          .....
  //          4'hf:  right before the datin cycle.
  //bit 15:8.  data on the dummy cycle after the first byte.
  //bit 7:0   The first bytes data showed on dummy cycle. some flash may need special data for enhance performance mode or XIP mode.
#define SPIFC_USER_CTRL3                           ((0x0083  << 2) + 0xfe00ac00)
  //bit 31 usr_datin_en  1: datain cycle enabled.  0: no datain cycle.
  //bit 30: datin_dest.  1: save the input data to STATUS register.  0 save the input data to data buffer.
  //bit 29: 1: enable datain data AES describe. 0: not use AES.
  //bit 28:27.  datin mode.  2'b00 SPI extend mode. 01: dual mode. 10 Quad mode.
  //bit 25:16.  user data in data bytes.  how many bytes of user datain expected.
#define SPIFC_USER_ADDR                            ((0x0084  << 2) + 0xfe00ac00)
  //bit 31:0.  32bits user address.
#define SPIFC_AHB_REQ_CTRL                         ((0x0085  << 2) + 0xfe00ac00)
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
#define SPIFC_AHB_REQ_CTRL1                        ((0x0086  << 2) + 0xfe00ac00)
  //bit 31.  ahb Dummy enable.
  //bit 30:29.  ahb dummy mode.
  //bit 28:23.  ahb request dummy clock cycles.
  //bit 15:0.   ahb dummy period output data.
#define SPIFC_AHB_REQ_CTRL2                        ((0x0087  << 2) + 0xfe00ac00)
  //bit 1:0  AHB address position. this setting depend on SPIFC_AHB_CTRL register CWF_EN bit and RDBUF_SIZE.
  // if CWF_EN is enabled. these bit should be set to 2'b10 (16BYTE boundary).
  // if CWF_EN is disabled. these bit should be set to same as the RDBUF_SIZE and AHB REQ DATA SIZE.
          //00 :  64byte boundary.
          //01 :  32byte boundary.
          //10 :  16byte boundary.
          //11 :  reserved.
#define SPIFC_ACTIMING0                            ((0x0088  << 2) + 0xfe00ac00)
  //bit 31:30.  tSLCH
  //bit 29:28   tCLSH
  //bit 20:16   tSHWL
  //bit 15:12   tSHSL2
  //bit 11:8    tSHSL1
  //bit 7:0     tWHSL
#define SPIFC_ACTIMING1                            ((0x0089  << 2) + 0xfe00ac00)
 //bit 31.  D2 pin WP_n function enable.
 //bit 30.  D2 Pin value in WP_n fucntion.
 //bit 29.  D3 Pin HOLD_n function enable.
 //bit 8.   DTR mode. not support.
 //bit 6:4. Clock turn around delay. use it to set the system SPI clock read data delay.
 //bit 3:0. not used.
#define SPIFC_ACTIMING2                            ((0x008a  << 2) + 0xfe00ac00)
 //bit 31.  spi clock input pin enable. SPIFC controller used this pin as clock to latch the input data.
 //bit [3:0]  spi clock input pin delay adjustment.
//SPI DATA BUFFER.  There's a total 512Byte + 64byte(organized as 36x128bits) SRAM .
// since each APB access is 32bits. So we use word address as APB read/write address.
// But for SPI side we have to use 128bit boundary. That's the first program/read SPI with data buffer must start at data buffer 128bit boundary.  that means the SPIFC_USE_DBUF_ADDR last 2 bits must be 0.
#define SPIFC_DBUF_CTRL                            ((0x0090  << 2) + 0xfe00ac00)
  //bit 31.    1 = write DBUF.  0 : read DBUF.
  //bit 30.    1. = auto update address. 0 don't change address.
  //bit 7:0   DBUF address unit 32bits.
#define SPIFC_DBUF_DATA                            ((0x0091  << 2) + 0xfe00ac00)
  //bit 31:0.   if SPIFC_DBUF_CTRL bit 31 = 1.  write to this register will trigger one write to DBUF. and the data will be writen to DBUF.
#define SPIFC_USER_DBUF_ADDR                       ((0x0092  << 2) + 0xfe00ac00)
  //bit 7:0.  the DBUF ADDRESS used to send to or receive from SPI FLASH. the last 2 bits must be 0.
#define SPIFC_FLASH_STATUS                         ((0x00a0  << 2) + 0xfe00ac00)
#define SPIFC_STATUS                               ((0x00a1  << 2) + 0xfe00ac00)
  //bit 1.  AES KEY valid. read only 1 : The aes key is ready to use. 0: the aes key is not ready.
  //bit 0.  SPI CTRL STATE IDLE.  read only. 1 = idle; 0 = busy.
#define SPIFC_CTRL                                 ((0x00a2  << 2) + 0xfe00ac00)
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
// Reading file:  ../mmc_lp4/dmc/rtl/dmc_sec.vh
//
//`ifdef DMC_SEC_REG_DEFINE
//`else
//`define DMC_SEC_REG_DEFINE
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfe025000
// -----------------------------------------------
//`define DMC_SEC_REG_BASE      32'hfe025000
// DMC ID assignment.
//  ID[13 :0]  total 14 bit ID.
//  ID[4:0]:  for MASTER IDs.
//           0:    A55 CPU.
//           1:    DSPA
//           2:    DSPB
//           3:    WAVE
//           4:    GDC
//           5:    GE2D
//           6:    ISP
//           7:    NNA
//           8:    JPEG
//           9~15:    not used in C1.
//           16 : Device.  need to look at ID bit [11:9] to see the sub module master.
//           16 : for Device.  need to look at ID bit [11:9] to see the sub module master.
//           17 : for USB AHB device.
//           18 : for USB AXI host.
//           19 : for I2C debug input.
//           20~30:  Not used in C1
//           31:  DMC internal test channel.
//   Device master is for small periphs. it is consists of 8 sub modules and identified with ID bits [11:9]
//     bit[11:9] == 0 : ETH
//     bit[11:9] == 1 : SPICC_A
//     bit[11:9] == 2 : SPICC_B
//     bit[11:9] == 3 : SD_EMMCA
//     bit[11:9] == 4 : SD_EMMCB
//     bit[11:9] == 5 : AUDIO.   For AUDIO need to check ID[8:5] for detail.
//                      ID[8:5] == 0:    TO_DDR A
//                      ID[8:5] == 1:    TO_DDR B
//                      ID[8:5] == 4:    FROM_DDR A
//                      ID[8:5] == 5:    FROM_DDR B
//     bit[11:9] == 6 : DMA.   For DMC need to check ID [8:6] for totoal 8 threads IDs.
//     bit[11:9] == 7 : NAND_EMMCC
// Total 16 address regions can be defined in All DDR address space.
// The first 15 region can be defined by start address and end address in 4Kbyte boundary. The 16th range is all other space not defined by the first 15 regions.
// all the 15 regions can be enabled or disabled  seperately. Each range use security level bit to select one of the two des key to do data scramble.
// The region can be overlapped or crossover.  For one transaction, DMC would check the address from region 0 to 15.
// DMC would use the policy defined in the region that this transaction address hit first.
// the region is define by  >= STA && <= EDA.
#define DMC_SEC_RANGE0_STA                         ((0x0000  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE0_EDA                         ((0x0001  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE1_STA                         ((0x0002  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE1_EDA                         ((0x0003  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE2_STA                         ((0x0004  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE2_EDA                         ((0x0005  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE3_STA                         ((0x0006  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE3_EDA                         ((0x0007  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE4_STA                         ((0x0008  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE4_EDA                         ((0x0009  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE5_STA                         ((0x000a  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE5_EDA                         ((0x000b  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE6_STA                         ((0x000c  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE6_EDA                         ((0x000d  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE7_STA                         ((0x000e  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE7_EDA                         ((0x000f  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE8_STA                         ((0x0010  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE8_EDA                         ((0x0011  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE9_STA                         ((0x0012  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE9_EDA                         ((0x0013  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE10_STA                        ((0x0014  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE10_EDA                        ((0x0015  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE11_STA                        ((0x0016  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE11_EDA                        ((0x0017  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE12_STA                        ((0x0018  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE12_EDA                        ((0x0019  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE13_STA                        ((0x001a  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE13_EDA                        ((0x001b  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE14_STA                        ((0x001c  << 2) + 0xfe025000)
  //bit 31:12   : range 0 start address.
  //bit 11:0    : reserved to be 0.
#define DMC_SEC_RANGE14_EDA                        ((0x001d  << 2) + 0xfe025000)
  //bit 31:12   : range 0 end address.
  //bit 11:0    : reserved to be 0.
//there's total 36 bit sid_en policies for each region read/write seperately.
//*_sid_en[35:0]
//bit 35: dmc_test access enable bit. 1: enable : 0 disable.
//bit 34: i2c  access enable bit. 1: enable : 0 disable.
//bit 33: jpeg  access enable bit. 1: enable : 0 disable.
//bit 32: USB host access enable bit. 1: enable : 0 disable.
//bit 31: USB device access enable bit. 1: enable : 0 disable.
//bit 30: NNA non-secure access enable bit. 1: enable : 0 disable.
//bit 29: NNA secure access enable bit. 1: enable : 0 disable.
//bit 28: ISP access enable bit. 1: enable : 0 disable.
//bit 27: GE2D read source 1 enable bit.     not used for write. 1: enable : 0 disable.
//bit 26: GE2D read source 0 enable bit.    GE2D write access enable bit for write. 1: enable : 0 disable.
//bit 25: GDC access enable bit. 1: enable.  0: disable.
//bit 24: WAVE access enable bit. 1: enable.  0: disable.
//bit 23: NAND_EMMCC access enable bit. 1: enable.  0: disable.
//bit 22: SD_EMMCB access enable bit. 1: enable.  0: disable.
//bit 21: SD_EMMCA access enable bit. 1: enable.  0: disable.
//bit 20: SPICC_B access enable bit. 1: enable.  0: disable.
//bit 19: SPICC_A access enable bit. 1: enable.  0: disable.
//bit 18: ETH access enable bit. 1: enable.  0: disable.
//bit 17:10: DMA thread ID access enable bit.  each thread for one bit.  (only 4/6 threads supports in C1). 1: enable.  0: disable.
//bit 9:6: AUDIO channel sub ID access enable bit.  each ID for one bit. (2 read IDs and 2 write IDs supports in C1). 1: enable.  0: disable.
//bit 5:   DSP_B non-secure access enable bit. 1: enable.  0: disable.
//bit 4:   DSP_B secure access enable bit. 1: enable.  0: disable.
//bit 3:   DSP_A non-secure access enable bit. 1: enable.  0: disable.
//bit 2:   DSP_A secure access enable bit. 1: enable.  0: disable.
//bit 1:   A55 non-secure access enable bit. 1: enable.  0: disable.
//bit 0:   A55 secure access enable bit. 1: enable.  0: disable.
//wire [1:0]  cpu_sid_en    = range_sid_en[1:0];
//wire [1:0]  dspa_sid_en   = range_sid_en[3:2];
//wire [1:0]  dspb_sid_en   = range_sid_en[5:4];
//wire [3:0]  audio_sid_en  = range_sid_en[9:6];
//wire [7:0]  dma_sid_en    = range_sid_en[17:10];
//wire [7:0]  dev_sid_en    = {range_sid_en[23], 2'b0, range_sid_en[22:18]};
//wire        wave_sid_en   = range_sid_en[24];
//wire        gdc_sid_en    = range_sid_en[25];
//wire [1:0]  ge2d_sid_en   = range_sid_en[27:26];
//wire        isp_sid_en    = range_sid_en[28];
//wire [1:0]  nna_sid_en    = range_sid_en[30:29];
//wire        usb_sid_en    = range_sid_en[31];
//wire        usb2_sid_en   = range_sid_en[32];
//wire        jpeg_sid_en   = range_sid_en[33];
//wire        i2c_sid_en    = range_sid_en[34];
//wire        test_sid_en   = range_sid_en[35];
#define DMC_SEC_RANGE0_RID_CTRL0                   ((0x0020  << 2) + 0xfe025000)
  //range0_rd_sid_en[31:0];
#define DMC_SEC_RANGE0_RID_CTRL1                   ((0x0021  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_rd_sid_en[35:32];
#define DMC_SEC_RANGE1_RID_CTRL0                   ((0x0022  << 2) + 0xfe025000)
  //range1_rd_sid_en[31:0];
#define DMC_SEC_RANGE1_RID_CTRL1                   ((0x0023  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range1_rd_sid_en[35:32];
#define DMC_SEC_RANGE2_RID_CTRL0                   ((0x0024  << 2) + 0xfe025000)
  //range2_rd_sid_en[31:0];
#define DMC_SEC_RANGE2_RID_CTRL1                   ((0x0025  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range2_rd_sid_en[35:32];
#define DMC_SEC_RANGE3_RID_CTRL0                   ((0x0026  << 2) + 0xfe025000)
  //range3_rd_sid_en[31:0];
#define DMC_SEC_RANGE3_RID_CTRL1                   ((0x0027  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range3_rd_sid_en[35:32];
#define DMC_SEC_RANGE4_RID_CTRL0                   ((0x0028  << 2) + 0xfe025000)
  //range4_rd_sid_en[31:0];
#define DMC_SEC_RANGE4_RID_CTRL1                   ((0x0029  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range4_rd_sid_en[35:32];
#define DMC_SEC_RANGE5_RID_CTRL0                   ((0x002a  << 2) + 0xfe025000)
  //range5_rd_sid_en[31:0];
#define DMC_SEC_RANGE5_RID_CTRL1                   ((0x002b  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range5_rd_sid_en[35:32];
#define DMC_SEC_RANGE6_RID_CTRL0                   ((0x002c  << 2) + 0xfe025000)
  //range6_rd_sid_en[31:0];
#define DMC_SEC_RANGE6_RID_CTRL1                   ((0x002d  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range6_rd_sid_en[35:32];
#define DMC_SEC_RANGE7_RID_CTRL0                   ((0x002e  << 2) + 0xfe025000)
  //range7_rd_sid_en[31:0];
#define DMC_SEC_RANGE7_RID_CTRL1                   ((0x002f  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range7_rd_sid_en[35:32];
#define DMC_SEC_RANGE8_RID_CTRL0                   ((0x0030  << 2) + 0xfe025000)
  //range8_rd_sid_en[31:0];
#define DMC_SEC_RANGE8_RID_CTRL1                   ((0x0031  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range8_rd_sid_en[35:32];
#define DMC_SEC_RANGE9_RID_CTRL0                   ((0x0032  << 2) + 0xfe025000)
  //range9_rd_sid_en[31:0];
#define DMC_SEC_RANGE9_RID_CTRL1                   ((0x0033  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range9_rd_sid_en[35:32];
#define DMC_SEC_RANGE10_RID_CTRL0                  ((0x0034  << 2) + 0xfe025000)
  //range10_rd_sid_en[31:0];
#define DMC_SEC_RANGE10_RID_CTRL1                  ((0x0035  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range10_rd_sid_en[35:32];
#define DMC_SEC_RANGE11_RID_CTRL0                  ((0x0036  << 2) + 0xfe025000)
  //range11_rd_sid_en[31:0];
#define DMC_SEC_RANGE11_RID_CTRL1                  ((0x0037  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range11_rd_sid_en[35:32];
#define DMC_SEC_RANGE12_RID_CTRL0                  ((0x0038  << 2) + 0xfe025000)
  //range12_rd_sid_en[31:0];
#define DMC_SEC_RANGE12_RID_CTRL1                  ((0x0039  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range12_rd_sid_en[35:32];
#define DMC_SEC_RANGE13_RID_CTRL0                  ((0x003a  << 2) + 0xfe025000)
  //range13_rd_sid_en[31:0];
#define DMC_SEC_RANGE13_RID_CTRL1                  ((0x003b  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range13_rd_sid_en[35:32];
#define DMC_SEC_RANGE14_RID_CTRL0                  ((0x003c  << 2) + 0xfe025000)
  //range14_rd_sid_en[31:0];
#define DMC_SEC_RANGE14_RID_CTRL1                  ((0x003d  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range14_rd_sid_en[35:32];
#define DMC_SEC_RANGE15_RID_CTRL0                  ((0x003e  << 2) + 0xfe025000)
  //range15_rd_sid_en[31:0];
#define DMC_SEC_RANGE15_RID_CTRL1                  ((0x003f  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range15_rd_sid_en[35:32];
#define DMC_SEC_RANGE0_WID_CTRL0                   ((0x0060  << 2) + 0xfe025000)
  //range0_wr_sid_en[31:0];
#define DMC_SEC_RANGE0_WID_CTRL1                   ((0x0061  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE1_WID_CTRL0                   ((0x0062  << 2) + 0xfe025000)
  //range1_wr_sid_en[31:0];
#define DMC_SEC_RANGE1_WID_CTRL1                   ((0x0063  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE2_WID_CTRL0                   ((0x0064  << 2) + 0xfe025000)
  //range2_wr_sid_en[31:0];
#define DMC_SEC_RANGE2_WID_CTRL1                   ((0x0065  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE3_WID_CTRL0                   ((0x0066  << 2) + 0xfe025000)
  //range3_wr_sid_en[31:0];
#define DMC_SEC_RANGE3_WID_CTRL1                   ((0x0067  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE4_WID_CTRL0                   ((0x0068  << 2) + 0xfe025000)
  //range4_wr_sid_en[31:0];
#define DMC_SEC_RANGE4_WID_CTRL1                   ((0x0069  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE5_WID_CTRL0                   ((0x006a  << 2) + 0xfe025000)
  //range5_wr_sid_en[31:0];
#define DMC_SEC_RANGE5_WID_CTRL1                   ((0x006b  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE6_WID_CTRL0                   ((0x006c  << 2) + 0xfe025000)
  //range6_wr_sid_en[31:0];
#define DMC_SEC_RANGE6_WID_CTRL1                   ((0x006d  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE7_WID_CTRL0                   ((0x006e  << 2) + 0xfe025000)
  //range7_wr_sid_en[31:0];
#define DMC_SEC_RANGE7_WID_CTRL1                   ((0x006f  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE8_WID_CTRL0                   ((0x0070  << 2) + 0xfe025000)
  //range8_wr_sid_en[31:0];
#define DMC_SEC_RANGE8_WID_CTRL1                   ((0x0071  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE9_WID_CTRL0                   ((0x0072  << 2) + 0xfe025000)
  //range9_wr_sid_en[31:0];
#define DMC_SEC_RANGE9_WID_CTRL1                   ((0x0073  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE10_WID_CTRL0                  ((0x0074  << 2) + 0xfe025000)
  //range10_wr_sid_en[31:0];
#define DMC_SEC_RANGE10_WID_CTRL1                  ((0x0075  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE11_WID_CTRL0                  ((0x0076  << 2) + 0xfe025000)
  //range11_wr_sid_en[31:0];
#define DMC_SEC_RANGE11_WID_CTRL1                  ((0x0077  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE12_WID_CTRL0                  ((0x0078  << 2) + 0xfe025000)
  //range12_wr_sid_en[31:0];
#define DMC_SEC_RANGE12_WID_CTRL1                  ((0x0079  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE13_WID_CTRL0                  ((0x007a  << 2) + 0xfe025000)
  //range13_wr_sid_en[31:0];
#define DMC_SEC_RANGE13_WID_CTRL1                  ((0x007b  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE14_WID_CTRL0                  ((0x007c  << 2) + 0xfe025000)
  //range14_wr_sid_en[31:0];
#define DMC_SEC_RANGE14_WID_CTRL1                  ((0x007d  << 2) + 0xfe025000)
  //bit 31:4.  Not used.
  //bit 3:0 range0_wr_sid_en[35:32];
#define DMC_SEC_RANGE15_WID_CTRL0                  ((0x007e  << 2) + 0xfe025000)
  //range15_wr_sid_en[31:0];
#define DMC_SEC_RANGE15_WID_CTRL1                  ((0x007f  << 2) + 0xfe025000)
#define DMC_SEC_RANGE_CTRL                         ((0x00a0  << 2) + 0xfe025000)
  //bit 31:16    16 range security level. each
  //bit 15:0     16 range enable. each bit for one range to identify the range is enabled or not.
#define DMC_DES_PADDING                            ((0x00a1  << 2) + 0xfe025000)
  //32bits address padding used for DES data generation.
#define DMC_DES_CTRL                               ((0x00a2  << 2) + 0xfe025000)
 //bit 1   DES enable.  1: DES enable. 0 : DES disable.  default is 1.
 //bit 0.  DES register mask. if write 1 only. after write 1, DES_CTRL and DES_padding regsiter can't be write and read.
// two address protection function for debug purpose.
// protected address[31:12] >= STA && <= EDA,
#define DMC_PROT0_STA                              ((0x00a3  << 2) + 0xfe025000)
  //bit 31:12.   PROT0 start addresss
#define DMC_PROT0_EDA                              ((0x00a4  << 2) + 0xfe025000)
  //bit 31:12.   PROT0 end addresss
#define DMC_PROT0_CTRL                             ((0x00a5  << 2) + 0xfe025000)
  //31:0  PROT0 IDs enable bit for ID[4:0].  1: under protection control.  0 normal access.
#define DMC_PROT0_CTRL1                            ((0x00a6  << 2) + 0xfe025000)
  //bit 26.  protection 0  read access protection enable.
  //bit 25.  protection 0  write access block function. if enabled, the hit access wouldn't write to the DDR SDRAM.  if not enabled only generate a interrupt, but the access still wrote to DDR.
  //bit 24.  protection 0  write access protection enable.
  //other bits not used.
#define DMC_PROT1_STA                              ((0x00a7  << 2) + 0xfe025000)
  //bit 31:12.   PROT1 start addresss
#define DMC_PROT1_EDA                              ((0x00a8  << 2) + 0xfe025000)
  //bit 31:12.   PROT1 end addresss
#define DMC_PROT1_CTRL                             ((0x00a9  << 2) + 0xfe025000)
  //31:0  PROT1 IDs enable bit for ID[4:0].  1: under protection control.  0 normal access.
#define DMC_PROT1_CTRL1                            ((0x00aa  << 2) + 0xfe025000)
  //bit 26.  PROT1  read access protection enable.
  //bit 25.  PROT1  write access block function. if enabled, the hit access wouldn't write to the DDR SDRAM.  if not enabled only generate a interrupt, but the access still wrote to DDR.
  //bit 24.  PROT1  write access protection enable.
  //other bits not used.
//registers to check the security protection and watch point error information.
#define DMC_SEC_STATUS                             ((0x00b8  << 2) + 0xfe025000)
 //bit 31. sec key valid.  des key is from outside, when is valid, DMC will catch the Key and save in local when the key valid signal change to 1 from 0.
 //bit 30~2. not used.
 //bit 1   write security violation.
 //bit 0.  read security violation.
#define DMC_VIO_ADDR0                              ((0x00b9  << 2) + 0xfe025000)
  //ddr0 write secure violation address.
#define DMC_VIO_ADDR1                              ((0x00ba  << 2) + 0xfe025000)
  //bit 31:25 . not used. always 0.
  //24     ddr0 secure check violation.
  //23     ddr0 protection 1 violation.
  //22     ddr0 protection 0 violation.
  //21:20  not used. always 0.
  //19     ddr0 write address overflow. write out of DDR size.
  //18:16. ddr0 write violation AWPROT bits.
  //15:14. reserved for future ID bits.
  //13:0   ddr0_write violation ID.
#define DMC_VIO_ADDR2                              ((0x00bb  << 2) + 0xfe025000)
  //ddr0 read secure violation address
#define DMC_VIO_ADDR3                              ((0x00bc  << 2) + 0xfe025000)
  //24     ddr0 read secure check violation.
  //23     ddr0 read protection 1 violation.
  //22     ddr0 read protection 0 violation.
  //21:20  Not used.  read to be 0.
  //19     ddr 0 read address overflow. write out of DDR size.
  //18:16. ddr 0 read violation ARPROT bits.
  //15:14  reserved for future ID bits.
  //13:0   ddr 0 read violation ID.
//DDR3/4 LPDDR3/4 SDRAM address  to 32bits linear address address map:
//DDR0_ADDRMAP is used to select RANK0 bank, row, col address selection.
//DDR1_ADDRMAP is used to select RANK1 bank, row, col address selection.
#define DDR0_ADDRMAP_0                             ((0x00d0  << 2) + 0xfe025000)
  //29:25 ca8.
  //24:20 ca7.
  //19:15 ca6.
  //14:10 ca5.
  //9:5   ca4.
  //4:0   ca3.
#define DDR0_ADDRMAP_1                             ((0x00d1  << 2) + 0xfe025000)
  //29:25 ra2.
  //24:20 ra1.
  //19:15 ra0.
  //14:10 ca11.
  //9:5   ca10.
  //4:0   ca9.
#define DDR0_ADDRMAP_2                             ((0x00d2  << 2) + 0xfe025000)
  //29:25 ra8.
  //24:20 ra7.
  //19:15 ra6.
  //14:10 ra5.
  //9:5   ra4.
  //4:0   ra3.
#define DDR0_ADDRMAP_3                             ((0x00d3  << 2) + 0xfe025000)
  //29:25 ra14.
  //24:20 ra13.
  //19:15 ra12.
  //14:10 ra11.
  //9:5   ra10.
  //4:0   ra9.
#define DDR0_ADDRMAP_4                             ((0x00d4  << 2) + 0xfe025000)
  //29:25 ra16 for DDR4 SDRAM
  //24:20 bg1  for DDR4 SDRAM.
  //19:15 ba2.    or bg0 for DDR4.
  //14:10 ba1.
  //9:5   ba0.
  //4:0   ra15.
#define DDR1_ADDRMAP_0                             ((0x00d5  << 2) + 0xfe025000)
  //29:25 ca8.
  //24:20 ca7.
  //19:15 ca6.
  //14:10 ca5.
  //9:5   ca4.
  //4:0   ca3.
#define DDR1_ADDRMAP_1                             ((0x00d6  << 2) + 0xfe025000)
  //29:25 ra2.
  //24:20 ra1.
  //19:15 ra0.
  //14:10 ca11.
  //9:5   ca10.
  //4:0   ca9.
#define DDR1_ADDRMAP_2                             ((0x00d7  << 2) + 0xfe025000)
  //29:25 ra8.
  //24:20 ra7.
  //19:15 ra6.
  //14:10 ra5.
  //9:5   ra4.
  //4:0   ra3.
#define DDR1_ADDRMAP_3                             ((0x00d8  << 2) + 0xfe025000)
  //29:25 ra14.
  //24:20 ra13.
  //19:15 ra12.
  //14:10 ra11.
  //9:5   ra10.
  //4:0   ra9.
#define DDR1_ADDRMAP_4                             ((0x00d9  << 2) + 0xfe025000)
  //29:25 ra16 for DDR4 SDRAM
  //24:20 bg1  for DDR4 SDRAM.
  //19:15 ba2  or bg0 for DDR4 SDRAM..
  //14:10 ba1.
  //9:5   ba0.
  //4:0   ra15.
#define DMC_DDR_CTRL                               ((0x00da  << 2) + 0xfe025000)
   //bit 31:28.  16bit selection for DDR3/4 not balanced mode.
        //4'b1000:    3Gbyte mode.  low 2Gbyte is in 32bits mode. 2G~3G is 16bits mode.
        //4'b0100:    1.5Gbyte mode.  low 1Gbyte is in 32bits mode. 1G~1.5G is in 16bits mode.
        //4'b0010:    768Mbyte mode.  low 512Mbyte is in 32bits mode. 512M~768M is in 16bits mode.
        //4'b0001:    384Mbyte mode.  low 25Mbyte is in 32bits mode.  256M~384M is in 16bits m0doe.
       // 4'b000 or others   balance mode. ether 32bits mode or 16bits mode depends on bit
   //bit 27.  0 : in 16bits mode. 16bit address postion at the top.
           // 1 : in 16bits mode. 16bits address postion start at address 0.
  //bit 24:22   3'b000 : ddr3 mode.
              //3'b001 : ddr4 mode.
              //3'b010 : lpddr3 mode.
              //3'b011 : lpddr4 mode.
   //bit 21.   rank1 enable bit. if 1,  rank1 used the address map is as bit 5:3 defined.
   //bit 20    DDR4 BG1 enable bit.
   //bit 18:   always 0.
   //bit 16.   1 only use 16bits data in a 32bits phy data interface. 0 : normal data interface.
  //bit 7:4 :  DDR rank 1 size.
     //4'b0000 : DDR rank 1 is 128Mbyte.
     //4'b0001 : DDR rank 1 is 256Mbyte.
     //4'b0010 : DDR rank 1 is 512Mbyte.
     //4'b0011 : DDR rank 1 is 1Gbyte.
     //4'b0100 : DDR rank 1 is 2Gbyte.
     //4'b1000 : DDR rank 1 is 192Mbyte.
     //4'b1001 : DDR rank 1 is 374Mbyte.
     //4'b1010 : DDR rank 1 is 768Mbyte.
     //4'b1011 : DDR rank 1 is 1.5Gbyte.
     //4'b1100 : DDR rank 1 is 3Gbyte.
     //others :  reserved.
  //bit 3:0  :  DDR rank 0 size.
     //4'b0000 : DDR rank 0 is 128Mbyte.
     //4'b0001 : DDR rank 0 is 256Mbyte.
     //4'b0010 : DDR rank 0 is 512Mbyte.
     //4'b0011 : DDR rank 0 is 1Gbyte.
     //4'b0100 : DDR rank 0 is 2Gbyte.
     //4'b1000 : DDR rank 0 is 192Mbyte.
     //4'b1001 : DDR rank 0 is 374Mbyte.
     //4'b1010 : DDR rank 0 is 768Mbyte.
     //4'b1011 : DDR rank 0 is 1.5Gbyte.
     //4'b1100 : DDR rank 0 is 3Gbyte.
     //others :  reserved.
#define DDR_APB_SEC_CTRL                           ((0x00db  << 2) + 0xfe025000)
  //bit 19:16.  DMC normal APB register secure control.
    //bit 19:  1:  all can write those register.  0:  the APB_PROT[0] must match the bit 0 to write those regsiter.
    //Bit15:12.   DMC sticky APB regsiter secure control.
    //bit 15.  1: All APB bus can write those registers. 0: The APB_PROT[0] must match the bit 12 to write those register.
    //bit 11:8.   DMC DDR PLL clock related  APB regsiter secure control.
    //bit 11.  1: All APB bus can write those registers. 0: The APB_PROT[0] must match the bit 8 to write those register.
    //Bit 7:4.   DMC DDR SDRAM protocal contorl register contorl
    //bit 11.  1: All APB bus can write those registers. 0: The APB_PROT[0] must match the bit 4 to write those register.
    //bit 3:0.   LPDDR4 PHY APB regsiter secure control.
    //bit 3.  1: All APB bus can write those registers. 0: The APB_PROT[0] must match the bit 0 to write those register.
//`endif
//
// Closing file:  ../mmc_lp4/dmc/rtl/dmc_sec.vh
//
//
// Reading file:  ../mmc_lp4/dmc/rtl/dmc_test.vh
//
// -----------------------------------------------
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfe025400
// -----------------------------------------------
// -----------------------------------------------
//`define DMC_TEST_REG_BASE               32'hfe025400
#define DMC_TEST_WRCMD_ADDR                        ((0x0000  << 2) + 0xfe025400)
  // the current write cmd address.
#define DMC_TEST_RDRSP_ADDR                        ((0x0001  << 2) + 0xfe025400)
  // the failed read response address(for error data )
#define DMC_TEST_RDCMD_ADDR                        ((0x0002  << 2) + 0xfe025400)
   // the current read command address.
#define DMC_TEST_WDG                               ((0x0003  << 2) + 0xfe025400)
  //31:16.  write response watch dog.
  //15:0.   read response  watch dog.
#define DMC_TEST_STA                               ((0x0004  << 2) + 0xfe025400)
  //test start address.  for non-sha mode,  the last 5 bits would be ignored. the test address at 32bytes boundary.
  //                     for sha mode,      address must be in 64 bytes boundary. that mean the last 6 bits must be 0.
#define DMC_TEST_EDA                               ((0x0005  << 2) + 0xfe025400)
  //test end address.  for non-sha mode,  the last 5 bits would be ignored. the test address at 32bytes boundary.
  //                   for sha mode,       address must be in 64 bytes boundary. that mean the last 6bits must be 1.
#define DMC_TEST_CTRL                              ((0x0006  << 2) + 0xfe025400)
   //bit 31.  enable test.
   //bit 30.  when enable test, enable the write to DDR function.
   //bit 29.  when enable test, enable the read from DDR function.
   //bit 28.  when enable test,  enable the sha calculation function  must be same as read enable but without write function.
   //bit 27.  enable to compare data.  when do the read enable to enable the error comparison. suppose the read data should be same as the data in the write buffer.
   //bit 26.   reserved.
   //bit 25.  address generation type.  0: continuous increase the address in the range of test start address and test end address.
   //                                   1: test module would pick the random address from test start address  and test end address.
   //bit 24.  done type.      0 : use the DMC_TEST_NUM register as the counter of test numbers.
   //                             for write if the write command number == the DMC_TEST_NUM, the write is done.
   //                             for read if the read command number == the DMC TEST_num, the read id done. for one read command can be repeated repeat number times.
   //                         1 : finshed at end address.
   //bit 23.  wdata type.     1 : the first write is {WD3, WD2,WD1,WD0}, then the latter is the previous data plus a pattern.( { + WD7,  + WD6, + WD5, + WD4}).
   //                         0 : the WDATA is the data in write register.
   //bit 23.  reserved.
   //bit 22:20.   read repeat times.  for non-sha function, we can define multi times of the read. the test module would repeat the same adddress repeat times.
   //bit 19.     limit write.  0: no outstanding write request limitation.
   //                          1: limit the outstanding write commands to the number of bits [15:8]
   //bit 18.     limit read.   0. no outstanding read request limitation.
   //                          1. limit the read outstanding request to the number of bits[7:0].
   //bit 17:16.  sha mode for sha function enabled.  00 : not used.  01 : sha1. 2: sha2-256. 3: sha2_224. not used in GXL fixed to be  Sha 2.
   //bit 15:8.   write outstanding commands limit.
   //bit 7:0.    read  outstanding commands limit.
#define DMC_TEST_NUM                               ((0x0007  << 2) + 0xfe025400)
   // how many test command for the test if the DMC_TEST_CTRL bit 24 is 0.
#define DMC_TEST_WD0                               ((0x0010  << 2) + 0xfe025400)
   // write data 0 for write command. also for read back data comparision.
#define DMC_TEST_WD1                               ((0x0011  << 2) + 0xfe025400)
   // write data 1 for write command. also for read back data comparision.
#define DMC_TEST_WD2                               ((0x0012  << 2) + 0xfe025400)
   // write data 2 for write command. also for read back data comparision.
#define DMC_TEST_WD3                               ((0x0013  << 2) + 0xfe025400)
   // write data 3 for write command. also for read back data comparision.
#define DMC_TEST_WD4                               ((0x0014  << 2) + 0xfe025400)
   // write data 4 for write command. also for read back data comparision.
#define DMC_TEST_WD5                               ((0x0015  << 2) + 0xfe025400)
   // write data 5 for write command. also for read back data comparision.
#define DMC_TEST_WD6                               ((0x0016  << 2) + 0xfe025400)
   // write data 6 for write command. also for read back data comparision.
#define DMC_TEST_WD7                               ((0x0017  << 2) + 0xfe025400)
   // write data 7 for write command. also for read back data comparision.
#define DMC_TEST_WD8                               ((0x0018  << 2) + 0xfe025400)
   // write data 8 for write command. also for read back data comparision.
#define DMC_TEST_WD9                               ((0x0019  << 2) + 0xfe025400)
   // write data 9 for write command. also for read back data comparision.
#define DMC_TEST_WD10                              ((0x001a  << 2) + 0xfe025400)
   // write data 10 for write command. also for read back data comparision.
#define DMC_TEST_WD11                              ((0x001b  << 2) + 0xfe025400)
   // write data 11 for write command. also for read back data comparision.
#define DMC_TEST_WD12                              ((0x001c  << 2) + 0xfe025400)
   // write data 12 for write command. also for read back data comparision.
#define DMC_TEST_WD13                              ((0x001d  << 2) + 0xfe025400)
   // write data 13 for write command. also for read back data comparision.
#define DMC_TEST_WD14                              ((0x001e  << 2) + 0xfe025400)
   // write data 14 for write command. also for read back data comparision.
#define DMC_TEST_WD15                              ((0x001f  << 2) + 0xfe025400)
   // write data 15 for write command. also for read back data comparision.
#define DMC_TEST_RD0                               ((0x0020  << 2) + 0xfe025400)
   // the read back data 0.  if error happens, it would capture the first error data.
#define DMC_TEST_RD1                               ((0x0021  << 2) + 0xfe025400)
   // the read back data 1.  if error happens, it would capture the first error data.
#define DMC_TEST_RD2                               ((0x0022  << 2) + 0xfe025400)
   // the read back data 2.  if error happens, it would capture the first error data.
#define DMC_TEST_RD3                               ((0x0023  << 2) + 0xfe025400)
   // the read back data 3.  if error happens, it would capture the first error data.
#define DMC_TEST_RD4                               ((0x0024  << 2) + 0xfe025400)
   // the read back data 4.  if error happens, it would capture the first error data.
#define DMC_TEST_RD5                               ((0x0025  << 2) + 0xfe025400)
   // the read back data 5.  if error happens, it would capture the first error data.
#define DMC_TEST_RD6                               ((0x0026  << 2) + 0xfe025400)
   // the read back data 6.  if error happens, it would capture the first error data.
#define DMC_TEST_RD7                               ((0x0027  << 2) + 0xfe025400)
   // the read back data 7.  if error happens, it would capture the first error data.
#define DMC_TEST_RD8                               ((0x0028  << 2) + 0xfe025400)
   // the read back data 7.  if error happens, it would capture the first error data.
#define DMC_TEST_RD9                               ((0x0029  << 2) + 0xfe025400)
   // the read back data 7.  if error happens, it would capture the first error data.
#define DMC_TEST_RD10                              ((0x002a  << 2) + 0xfe025400)
   // the read back data 7.  if error happens, it would capture the first error data.
#define DMC_TEST_RD11                              ((0x002b  << 2) + 0xfe025400)
   // the read back data 7.  if error happens, it would capture the first error data.
#define DMC_TEST_RD12                              ((0x002c  << 2) + 0xfe025400)
   // the read back data 7.  if error happens, it would capture the first error data.
#define DMC_TEST_RD13                              ((0x002d  << 2) + 0xfe025400)
   // the read back data 7.  if error happens, it would capture the first error data.
#define DMC_TEST_RD14                              ((0x002e  << 2) + 0xfe025400)
   // the read back data 7.  if error happens, it would capture the first error data.
#define DMC_TEST_RD15                              ((0x002f  << 2) + 0xfe025400)
   // the read back data 7.  if error happens, it would capture the first error data.
#define DMC_TEST_ERR_ADDR                          ((0x0030  << 2) + 0xfe025400)
  // it capture the first error address.
#define DMC_TEST_ERR_CNT                           ((0x0031  << 2) + 0xfe025400)
  // how many data error happens in the whole test period.
#define DMC_TEST_STS                               ((0x0032  << 2) + 0xfe025400)
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
#define DMC_TEST_COMP_MASK                         ((0x0033  << 2) + 0xfe025400)
  //32bits for DMC TEST COMPARE bit enable.
  //1 : to MASK this bit.  0: compare this bit.
#define DMC_TEST_RDBI0                             ((0x0034  << 2) + 0xfe025400)
  //MPC RD FIFO command DBI read back data
  //bit 31:16  the second cycle.
  //bit 15:0   the first cycle.
#define DMC_TEST_RDBI1                             ((0x0035  << 2) + 0xfe025400)
  //MPC RD FIFO command DBI read back data
  //bit 31:16. the Forth cycle.
  //bit 15:0.  the third cycle.
#define DMC_TEST_WSTRB0                            ((0x0036  << 2) + 0xfe025400)
  //MPC WR FIFO command DM bit write data
  //bit 31:16  the second cycle.
  //bit 15:0   the first cycle.
#define DMC_TEST_WSTRB1                            ((0x0037  << 2) + 0xfe025400)
  //MPC WR FIFO command DM bit write data
  //bit 31:16. the Forth cycle.
  //bit 15:0.  the third cycle.
#define DMC_TEST_DRAM_CMD                          ((0x0038  << 2) + 0xfe025400)
 //bit 31. cmd done.  write 0 to clean.
 //bit 30. data done. write 0 to clean.
 //bit 4:0.  only one bit can be 1. read data stored in DMC_TEST_RD* write data from DMC_TEST_W
 //bit 4. LPDDR4 MPC write data command( MPC WR FIFO).
 //bit 3. LPDDR4 MPC read data command (MPC RD Calibration and RD FIFO).
 //bit 2. LPDDR4 MPC-1 command ( NOP,  Start DQS interval ....)
 //bit 1. mrr comand.
 //bit 0. mrw command.
#define DMC_TEST_DRAM_CMD_CODE                     ((0x0039  << 2) + 0xfe025400)
 //bit 27:26. 128bits data cycles . 0: 1 clock cycles;  1: 2  clock cycles; 2: 3 clock cycles; 3:4 clock cycles.
 //           LPDDR4 32bits: 4 clock cycles; LPDDR4 16bits: 2 clock cycles;
 //           DDR3/4/LPDDR3 32 bits : 2 clock cycles. DDR3/4 16bits:  1 cycles.
 //bit 25  MRW/MRR/MPC command rank 1 select.  1: select.  0: not select.
 //bit 24. MRW/MRR/MPC command rank 0 select.  1: select.  0: not select.
 //bit 23:16  MR addr.  DDR4 case :  18:16 ba[2:0].    20:19 BG[1:0].
 //bit 15:0   opcode.
#define DMC_TEST_DRAM_CMD_TIME                     ((0x003a  << 2) + 0xfe025400)
//bit 31:16  PRE  CMD timer. //delay how many cycle to start the command.
//bit 15:0   POST CMD timer  //delay how many cycle after the command execute.
//`endif
//
// Closing file:  ../mmc_lp4/dmc/rtl/dmc_test.vh
//
//
// Reading file:  ../mmc_lp4/dmc/rtl/dmc_reg.vh
//
//`ifdef DMC_REG_DEFINE
//`else
//`define DMC_REG_DEFINE
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfe024000
// -----------------------------------------------
//`define DMC_REG_BASE      32'hfe024000
#define DMC_SOFT_RST                               ((0x0000  << 2) + 0xfe024000)
#define DMC_SOFT_RST1                              ((0x0002  << 2) + 0xfe024000)
#define DMC_SOFT_RST2                              ((0x0003  << 2) + 0xfe024000)
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
#define DMC_VERSION                                ((0x0005  << 2) + 0xfe024000)
   //read only 32'h000b0001.  for A1
#define DMC_MON_CTRL0                              ((0x0020  << 2) + 0xfe024000)
   //bit 31.   qos_mon_en.    write 1 to trigger the enable. polling this bit 0, means finished.  or use interrupt to check finish.
   //bit 30.   qos_mon interrupt clear.  clear the qos monitor result.  read 1 = qos mon finish interrupt.
   //bit 3.    qos monitor 3 enable.
   //bit 2.    qos monitor 2 enable.
   //bit 1.    qos monitor 1 enable.
   //bit 0.    qos monitor 0 enable.
#define DMC_MON_CTRL1                              ((0x0021  << 2) + 0xfe024000)
  //enable to check which modules  bandwidth in QOS monitor 0
  //bit 31:21: not used.
  //bit 20:   enable DMC_TEST bandwidth check. 1 : enable. 0 : disable.
  //bit 19:   enable I2C bandwidth check. 1 : enable. 0 : disable.
  //bit 18:   enable USB host bandwidth check. 1 : enable. 0 : disable.
  //bit 17:   enable USB device bandwidth check. 1 : enable. 0 : disable.
  //bit 16:   enable NAND_EMMCC bandwidth check. 1 : enable. 0 : disable.
  //bit 15:   enable DMA bandwidth check. 1 : enable. 0 : disable.
  //bit 14:   enable AUDIO bandwidth check. 1 : enable. 0 : disable.
  //bit 13:   enable SD_EMMCB bandwidth check. 1 : enable. 0 : disable.
  //bit 12:   enable SD_EMMCA bandwidth check. 1 : enable. 0 : disable.
  //bit 11:   enable SPICC_B bandwidth check. 1 : enable. 0 : disable.
  //bit 10:   enable SPICC_A bandwidth check. 1 : enable. 0 : disable.
  //bit 9:    enable ETH bandwidth check. 1 : enable. 0 : disable.
  //bit 8:    enable JPEG bandwidth check. 1 : enable. 0 : disable.
  //bit 7:    enable NNA bandwidth check. 1 : enable. 0 : disable.
  //bit 6:    enable ISP bandwidth check. 1 : enable. 0 : disable.
  //bit 5:    enable GE2D bandwidth check. 1 : enable. 0 : disable.
  //bit 4:    enable GDC bandwidth check. 1 : enable. 0 : disable.
  //bit 3:    enable WAVE bandwidth check. 1 : enable. 0 : disable.
  //bit 2:    enable DSPB bandwidth check. 1 : enable. 0 : disable.
  //bit 1:    enable DSPA bandwidth check. 1 : enable. 0 : disable.
  //bit 0:    enable CPU bandwidth check. 1 : enable. 0 : disable.
#define DMC_MON_CTRL2                              ((0x0022  << 2) + 0xfe024000)
 //not used in C1
#define DMC_MON_CTRL3                              ((0x0023  << 2) + 0xfe024000)
  //enable to check which modules  bandwidth in QOS monitor 1
  //bit 31:21: not used.
  //bit 20:   enable DMC_TEST bandwidth check. 1 : enable. 0 : disable.
  //bit 19:   enable I2C bandwidth check. 1 : enable. 0 : disable.
  //bit 18:   enable USB host bandwidth check. 1 : enable. 0 : disable.
  //bit 17:   enable USB device bandwidth check. 1 : enable. 0 : disable.
  //bit 16:   enable NAND_EMMCC bandwidth check. 1 : enable. 0 : disable.
  //bit 15:   enable DMA bandwidth check. 1 : enable. 0 : disable.
  //bit 14:   enable AUDIO bandwidth check. 1 : enable. 0 : disable.
  //bit 13:   enable SD_EMMCB bandwidth check. 1 : enable. 0 : disable.
  //bit 12:   enable SD_EMMCA bandwidth check. 1 : enable. 0 : disable.
  //bit 11:   enable SPICC_B bandwidth check. 1 : enable. 0 : disable.
  //bit 10:   enable SPICC_A bandwidth check. 1 : enable. 0 : disable.
  //bit 9:    enable ETH bandwidth check. 1 : enable. 0 : disable.
  //bit 8:    enable JPEG bandwidth check. 1 : enable. 0 : disable.
  //bit 7:    enable NNA bandwidth check. 1 : enable. 0 : disable.
  //bit 6:    enable ISP bandwidth check. 1 : enable. 0 : disable.
  //bit 5:    enable GE2D bandwidth check. 1 : enable. 0 : disable.
  //bit 4:    enable GDC bandwidth check. 1 : enable. 0 : disable.
  //bit 3:    enable WAVE bandwidth check. 1 : enable. 0 : disable.
  //bit 2:    enable DSPB bandwidth check. 1 : enable. 0 : disable.
  //bit 1:    enable DSPA bandwidth check. 1 : enable. 0 : disable.
  //bit 0:    enable CPU bandwidth check. 1 : enable. 0 : disable.
#define DMC_MON_CTRL4                              ((0x0024  << 2) + 0xfe024000)
  //not used in C1
#define DMC_MON_CTRL5                              ((0x0025  << 2) + 0xfe024000)
  //enable to check which modules  bandwidth in QOS monitor 2
  //bit 31:21: not used.
  //bit 20:   enable DMC_TEST bandwidth check. 1 : enable. 0 : disable.
  //bit 19:   enable I2C bandwidth check. 1 : enable. 0 : disable.
  //bit 18:   enable USB host bandwidth check. 1 : enable. 0 : disable.
  //bit 17:   enable USB device bandwidth check. 1 : enable. 0 : disable.
  //bit 16:   enable NAND_EMMCC bandwidth check. 1 : enable. 0 : disable.
  //bit 15:   enable DMA bandwidth check. 1 : enable. 0 : disable.
  //bit 14:   enable AUDIO bandwidth check. 1 : enable. 0 : disable.
  //bit 13:   enable SD_EMMCB bandwidth check. 1 : enable. 0 : disable.
  //bit 12:   enable SD_EMMCA bandwidth check. 1 : enable. 0 : disable.
  //bit 11:   enable SPICC_B bandwidth check. 1 : enable. 0 : disable.
  //bit 10:   enable SPICC_A bandwidth check. 1 : enable. 0 : disable.
  //bit 9:    enable ETH bandwidth check. 1 : enable. 0 : disable.
  //bit 8:    enable JPEG bandwidth check. 1 : enable. 0 : disable.
  //bit 7:    enable NNA bandwidth check. 1 : enable. 0 : disable.
  //bit 6:    enable ISP bandwidth check. 1 : enable. 0 : disable.
  //bit 5:    enable GE2D bandwidth check. 1 : enable. 0 : disable.
  //bit 4:    enable GDC bandwidth check. 1 : enable. 0 : disable.
  //bit 3:    enable WAVE bandwidth check. 1 : enable. 0 : disable.
  //bit 2:    enable DSPB bandwidth check. 1 : enable. 0 : disable.
  //bit 1:    enable DSPA bandwidth check. 1 : enable. 0 : disable.
  //bit 0:    enable CPU bandwidth check. 1 : enable. 0 : disable.
#define DMC_MON_CTRL6                              ((0x0026  << 2) + 0xfe024000)
  //not USED in C1
#define DMC_MON_CTRL7                              ((0x0027  << 2) + 0xfe024000)
  //enable to check which modules  bandwidth in QOS monitor 3
  //bit 31:21: not used.
  //bit 20:   enable DMC_TEST bandwidth check. 1 : enable. 0 : disable.
  //bit 19:   enable I2C bandwidth check. 1 : enable. 0 : disable.
  //bit 18:   enable USB host bandwidth check. 1 : enable. 0 : disable.
  //bit 17:   enable USB device bandwidth check. 1 : enable. 0 : disable.
  //bit 16:   enable NAND_EMMCC bandwidth check. 1 : enable. 0 : disable.
  //bit 15:   enable DMA bandwidth check. 1 : enable. 0 : disable.
  //bit 14:   enable AUDIO bandwidth check. 1 : enable. 0 : disable.
  //bit 13:   enable SD_EMMCB bandwidth check. 1 : enable. 0 : disable.
  //bit 12:   enable SD_EMMCA bandwidth check. 1 : enable. 0 : disable.
  //bit 11:   enable SPICC_B bandwidth check. 1 : enable. 0 : disable.
  //bit 10:   enable SPICC_A bandwidth check. 1 : enable. 0 : disable.
  //bit 9:    enable ETH bandwidth check. 1 : enable. 0 : disable.
  //bit 8:    enable JPEG bandwidth check. 1 : enable. 0 : disable.
  //bit 7:    enable NNA bandwidth check. 1 : enable. 0 : disable.
  //bit 6:    enable ISP bandwidth check. 1 : enable. 0 : disable.
  //bit 5:    enable GE2D bandwidth check. 1 : enable. 0 : disable.
  //bit 4:    enable GDC bandwidth check. 1 : enable. 0 : disable.
  //bit 3:    enable WAVE bandwidth check. 1 : enable. 0 : disable.
  //bit 2:    enable DSPB bandwidth check. 1 : enable. 0 : disable.
  //bit 1:    enable DSPA bandwidth check. 1 : enable. 0 : disable.
  //bit 0:    enable CPU bandwidth check. 1 : enable. 0 : disable.
#define DMC_MON_CTRL8                              ((0x0028  << 2) + 0xfe024000)
  //not used.
#define DMC_MON_ALL_REQ_CNT                        ((0x0029  << 2) + 0xfe024000)
  // at the test period,  the whole MMC request time.
#define DMC_MON_ALL_GRANT_CNT                      ((0x002a  << 2) + 0xfe024000)
  // at the test period,  the whole MMC granted data cycles. 64bits unit.
#define DMC_MON_ONE_GRANT_CNT                      ((0x002b  << 2) + 0xfe024000)
  // at the test period,  the granted data cycles for the selected channel and ports.
#define DMC_MON_SEC_GRANT_CNT                      ((0x002c  << 2) + 0xfe024000)
  // at the test period,  the granted data cycles for the selected channel and ports.
#define DMC_MON_THD_GRANT_CNT                      ((0x002d  << 2) + 0xfe024000)
  // at the test period,  the granted data cycles for the selected channel and ports.
#define DMC_MON_FOR_GRANT_CNT                      ((0x002e  << 2) + 0xfe024000)
  // at the test period,  the granted data cycles for the selected channel and ports.
#define DMC_MON_TIMER                              ((0x002f  << 2) + 0xfe024000)
 // timer for the monitor period.
#define DMC_CLKG_CTRL2                             ((0x0032  << 2) + 0xfe024000)
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
#define DMC_CLKG_CTRL3                             ((0x0033  << 2) + 0xfe024000)
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
#define DMC_CHAN_STS                               ((0x0036  << 2) + 0xfe024000)
  //bit 4      always 1
  //bit 3      ddr0 write data buffer idle. 1 : idle 0: busy.
  //bit 2      always 1.
  //bit 1      ddr0 wbuf idle.              1 : idle 0: busy.
  //bit 0.      axi0 channel idle.         1 : idle 0: busy.
#define DMC_CMD_FILTER_CTRL3                       ((0x0042  << 2) + 0xfe024000)
  //bit 31.    force wbuf empty.
  //bit 30:26  wbuf high level number
  //bit 25:21  wbuf mid  level number
  //bit 20:16  wbuf low level number
  //bit 14:10  rbuf high level number
  //bit 9:5    rbuf middle level number
  //bit 4:0    rbuf low level number
#define DMC_CMD_FILTER_CTRL4                       ((0x0043  << 2) + 0xfe024000)
  //bit 29:25.  tITW.long
  //bit 24:20.  tITW. short
  //bit 19:12   tAP auto precharge the bank not used if idle that long time.
  //bit 11:6    write to read accesses if there write hit request.
  //bit 5:0     read to write accesses if there write hit request.
#define DMC_CMD_FILTER_CTRL5                       ((0x0044  << 2) + 0xfe024000)
  //bit 31:24   Once ddr data bus switch to read, the maxmum read command number to give up the bus when there's write request pending for write buffer.
  //bit 23:16   Once ddr data bus switch to write, the maxmum write command number to give up the bus when there's read request pending too long.
  //bit 15:8.   Once ddr data bus switch to read, the minimum read command number to transfer back to write stage if there's still pending read request.
  //bit 7:0.    Once ddr data bus switch to write, the minimum write command number to transfer back to read stage if there's still pending write request.
#define DMC_CMD_FILTER_CTRL6                       ((0x0045  << 2) + 0xfe024000)
  //bit 31:24   write urgent 3 request pending hold num.
  //bit 23:16   write urgent 2 request pending hold num.
  //bit 15:8.   write urgent 1 request pending hold num.
  //bit 7:0.    write urgent 0 request pending hold num.
#define DMC_CMD_FILTER_CTRL7                       ((0x0046  << 2) + 0xfe024000)
  //bit 15:8    write to read waiting cycles if there write hit request.
  //bit 7:0     read to write waiting cycles if there write hit request.
#define DMC_CMD_BUFFER_CTRL                        ((0x0047  << 2) + 0xfe024000)
  //bit 31:26  total write buffer number. default 32.
  //bit 25:20  total read buffer number. default 32.
  //bit 19:14.  Not used.
  //bit 13:8   write command fifo depth. default 32.
  //bit 7:0    aw_pending_inc_num.  incease write ugent level 1 when write command waiting to in write buffer that long.
#define DMC_CMD_BUFFER_CTRL1                       ((0x0048  << 2) + 0xfe024000)
  //bit 29:24  read buffer number in non-urgent request.
  //bit 23:16  read buffer bank miss watch dog threshold.
  //bit 15:12  read buffer urgent level 3 counter inc weight.
  //bit 11:8   read buffer urgent level 2 counter inc weight.
  //bit 7:4    read buffer urgent level 1 counter inc weight.
  //bit 3:0    read buffer urgent level 0 counter inc weight.
#define DMC_AXI0_CHAN_CTRL                         ((0x0080  << 2) + 0xfe024000)
  //bit 31       AXI interface request enable.
  //bit 19       AXI interface  auto clock gating enable.  1: enable; 0 disable.
  //bit 18       AXI interface  clock disable. 1 : disable clock.  0 enable clock.
  //bit 17       AXI interface  dmc clock domain soft reset.  0 : reset.  1 : normal working mode.
  //bit 16       AXI interface  s0 clock domain soft reset.   0 : reset.  1 : normal working mode.
  //bit 15:12    ugt3 QOS level.
  //bit 11:8     ugt2 QOS level.
  //bit 7:4      ugt1 QOS level.
  //bit 3:0      not used.
#define DMC_AXI0_HOLD_CTRL                         ((0x0081  << 2) + 0xfe024000)
    //31:24 write hold num.   max outstanding request number.
    //23:16  write hold release num. if the outstanding request == hold num, then hold this request unless the outstanding request number bellow the hold release number, then continue to request.
    //15:8 read hold num.   max outstanding request number.
    //7:0  read hold release num. if the outstanding request == hold num, then hold this request unless the outstanding request number bellow the hold release number, then continue to request.
#define DMC_AXI5_HOLD_CTRL                         ((0x0095  << 2) + 0xfe024000)
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
// APB_BASE:  APB1_BASE_ADDR = 0xfe024400
// -----------------------------------------------
//`define DMC_DRAM_REG_BASE      32'hfe024400
//there's 2 set of timing DDR timing parameter for 2 set of DDR freqency parameter.
//when change frequency, the hardware would automatically select one of these two set of timing parameter
//DMC_DRAM_* is for Frequency set 0.
//DMC_NFQ_* is for Freqency set 1.
#define DMC_DRAM_TMRD                              ((0x0000  << 2) + 0xfe024400)
   //bit 3:0.  tMRD.
#define DMC_DRAM_TRFC                              ((0x0001  << 2) + 0xfe024400)
   //bit 9:0 tRFC
#define DMC_DRAM_TRP                               ((0x0002  << 2) + 0xfe024400)
   //bit 21:16.  tRP for precharge all banks.
   //bit 5:0.    tRP for precharge one bank.
#define DMC_DRAM_TRTW                              ((0x0003  << 2) + 0xfe024400)
   //bit 5:0   tRTW
#define DMC_DRAM_TCL                               ((0x0004  << 2) + 0xfe024400)
  //bit 5:0  tCL/tRL. read latency.
#define DMC_DRAM_TCWL                              ((0x0005  << 2) + 0xfe024400)
  //bit 5:0.  CWL:  write latency.
#define DMC_DRAM_TRAS                              ((0x0006  << 2) + 0xfe024400)
  //bit 7:0.  tRAS.  minimum active to precharge time for same bank.
#define DMC_DRAM_TRC                               ((0x0007  << 2) + 0xfe024400)
  //bit 7:0.  tRC.  minimum active to active time for same bank.
#define DMC_DRAM_TRCD                              ((0x0008  << 2) + 0xfe024400)
  //bit 7:0  tRCD  active to read/write timing for same bank.
#define DMC_DRAM_TRRD                              ((0x0009  << 2) + 0xfe024400)
  //bit 21:16.  tRRD_l  active bank A to active B in same band group for DDR4.
  //bit 5:0.    tRRD/tRRD_s   active bank A to active bank b time.
              //tRRD_s:   active bank A to active bank b in different bank grousp for DDR4.
#define DMC_DRAM_TFAW                              ((0x000a  << 2) + 0xfe024400)
  //bit 8:0   tFAW.  four active command windows
#define DMC_DRAM_TRTP                              ((0x000b  << 2) + 0xfe024400)
  //bit 5:0  tRTP.
#define DMC_DRAM_TWR                               ((0x000c  << 2) + 0xfe024400)
  //bit 5:0 tWR.
#define DMC_DRAM_TWTR                              ((0x000d  << 2) + 0xfe024400)
  //bit 5:0 tWTR.
#define DMC_DRAM_TCCD                              ((0x000e  << 2) + 0xfe024400)
  //bit 19:16. tCCD/tCCD_l.
  //bit 3:0 tCCD/tCCD_s    read to read command time or write to write command time.
#define DMC_DRAM_TEXSR                             ((0x000f  << 2) + 0xfe024400)
  //bit 9:0. tEXSR.  EXIT SELF-REFRESH to read/write command.
#define DMC_DRAM_TXS                               ((0x0010  << 2) + 0xfe024400)
  //bit 9:0  tXS.  EXIT SELF_REFRESH to other command time
#define DMC_DRAM_TXP                               ((0x0011  << 2) + 0xfe024400)
  //bit 3:0. tXP.  EXIT power down to other command time
#define DMC_DRAM_TXPDLL                            ((0x0012  << 2) + 0xfe024400)
  //bit 9:0.  tXPDLL,  EXIT power down to read/write command time(need to relock PLL).
#define DMC_DRAM_TZQCS                             ((0x0013  << 2) + 0xfe024400)
  //bit 11:0.  ZQCS command to other command time.
#define DMC_DRAM_TCKSRE                            ((0x0014  << 2) + 0xfe024400)
  //bit 4:0. enter self refresh to disable clock time.
#define DMC_DRAM_TCKSRX                            ((0x0015  << 2) + 0xfe024400)
  //bit 4:0. enable clock to exit self refresh time.
#define DMC_DRAM_TCKE                              ((0x0016  << 2) + 0xfe024400)
  //bit 4:0.  CKE high or low minimum time.
#define DMC_DRAM_TMOD                              ((0x0017  << 2) + 0xfe024400)
  //bit 4:0  tMOD.  MRR/MRW to other command time.
#define DMC_DRAM_TDQS                              ((0x0018  << 2) + 0xfe024400)
  //bit 3:0 tDQS. the delay to access different rank.
#define DMC_DRAM_TRSTL                             ((0x0019  << 2) + 0xfe024400)
  //not used.
#define DMC_DRAM_TZQLAT                            ((0x001a  << 2) + 0xfe024400)
  //bit 5:0 ZQ LATCH command to other comand timing in LPDDR4 mode.
#define DMC_DRAM_TMRR                              ((0x001b  << 2) + 0xfe024400)
  //bit 7:0 tMRR  not used in DMC. not support MR READ.
#define DMC_DRAM_TCKESR                            ((0x001c  << 2) + 0xfe024400)
 //bit 9:0 tCKESR.   CKE low minimum pulse in self refresh mode.
#define DMC_DRAM_TDPD                              ((0x001d  << 2) + 0xfe024400)
 //not support.
#define DMC_DRAM_DFITCTRLDELAY                     ((0x001e  << 2) + 0xfe024400)
  //bit 3:0. DFI_t_ctrldealy
#define DMC_DRAM_DFITPHYWRDATA                     ((0x001f  << 2) + 0xfe024400)
  //bit 5:0.  dfi_t_phy_wrdata.
#define DMC_DRAM_DFITPHYWRLAT                      ((0x0020  << 2) + 0xfe024400)
  //bit 5:0.  dfi_t_phy_wrlat.  in DDR3/4/LPDDR3 mode:   WL -5.   in LPDDR4 mode: WL -5 + 2.
#define DMC_DRAM_DFITRDDATAEN                      ((0x0021  << 2) + 0xfe024400)
  //bit 5:0.  dfi_t_rddata_en.  in DDR3/4/LPDDR3 mode: RL -5. in LPDDR4 mode : RL -5 + 1.
#define DMC_DRAM_DFITPHYRDLAT                      ((0x0022  << 2) + 0xfe024400)
  //bit 5:0.  dfi_t_rdlat.
#define DMC_DRAM_DFITCTRLUPDMIN                    ((0x0023  << 2) + 0xfe024400)
  //bit 7:0.  CTRLUPD_MIN  minimux clock cycle to maintain CTRLUPD_REQ.
#define DMC_DRAM_DFITCTRLUPDMAX                    ((0x0024  << 2) + 0xfe024400)
  //bit 7:0   CTRLUPD_MAX.  maxmum clock cycle to maintain CTRLUPD_REQ if no CTRLUPD_ACK response.
#define DMC_DRAM_DFITMSTRRESP                      ((0x0025  << 2) + 0xfe024400)
  //not used.
#define DMC_DRAM_DFITREFMSKI                       ((0x0026  << 2) + 0xfe024400)
  //not used.
#define DMC_DRAM_DFITCTRLUPDI                      ((0x0027  << 2) + 0xfe024400)
  //not used.
#define DMC_DRAM_DFITDRAMCLK                       ((0x0028  << 2) + 0xfe024400)
//bit 17    dram clk1 enable.
//bit 16    dram clk0 enable.
//bit 15:8  DRAM CLK disable waiting time
//bit 7:0   DRAM CLK enable  enable timer
#define DMC_DRAM_DFITLPRESP                        ((0x002a  << 2) + 0xfe024400)
  //bit 3:0.  dfi_lp_ctrl_req response time. after dfi_lp_ctrl_req asserted, and after response time if there's still no dfi_lp_ack respone, then drop the dfi_lp_ctrl_req.
#define DMC_DRAM_DFITPHYMSTR                       ((0x002b  << 2) + 0xfe024400)
 //bit 15  1: enable DFIPHYMASTER INTERFACE 0 disable DFIPHYMSTR en response.
 // always 0  we would use DFI_RETRAIN function for PHY retrain required from LPDDR4.
#define DMC_DRAM_TCKECK                            ((0x002c  << 2) + 0xfe024400)
 //bit 4:0. tCKECK  from CKE low to assert dfi_dram_clk_disable time. this time + dfi_t_ctrl_delay
#define DMC_DRAM_TREFI                             ((0x002d  << 2) + 0xfe024400)
 //write this register will update the auto refresh related register to the auto refresh control logic.
 //bit 31:24:  tZQCI dmc send zqci period.  unit is how much auto refresh period.
 //bit 23:16   pvti  dmc send dfi_ctrlupd_req period.  unit is one auto refresh period.
 //bit 15:8.   tREFI.dmc send auto refresh command period. unit is 100ns.
 //bit 7:0.    t100ns period. unit is dmc clock cycles
#define DMC_DRAM_TSR                               ((0x002e  << 2) + 0xfe024400)
  //bit 5:0 tSR.  self resfresh enter to exit time.
#define DMC_DRAM_TCCDMW                            ((0x002f  << 2) + 0xfe024400)
  //bit 5:0.  4*tCCD in LPDDR4 mask write.
#define DMC_DRAM_TESCKE                            ((0x0030  << 2) + 0xfe024400)
  //bit 5:0  tESCKE.  enter self refresh to power time for LPDDR4.
#define DMC_DRAM_TREFI_DDR3                        ((0x0031  << 2) + 0xfe024400)
  //7:0. 8*DDR3 SDRAM tREFI time . the unit is t100ns.  use this to check in 8*tREFI time, the DMC should sent more than 16 auto REFRESH command.
#define DMC_DRAM_TZQCAL                            ((0x0032  << 2) + 0xfe024400)
  //7:0. ZQCAL for LPDDR4.
#define DMC_DRAM_T10US                             ((0x0033  << 2) + 0xfe024400)
  //10us clock cycle number used for LP2 mode.
#define DMC_DRAM_TMRRI                             ((0x0034  << 2) + 0xfe024400)
//bit 7:0   tMRRI for MRR
//timing paramter for frequency set 1.
#define DMC_NFQ_TMRD                               ((0x0040  << 2) + 0xfe024400)
#define DMC_NFQ_TRFC                               ((0x0041  << 2) + 0xfe024400)
#define DMC_NFQ_TRP                                ((0x0042  << 2) + 0xfe024400)
#define DMC_NFQ_TRTW                               ((0x0043  << 2) + 0xfe024400)
#define DMC_NFQ_TCL                                ((0x0044  << 2) + 0xfe024400)
#define DMC_NFQ_TCWL                               ((0x0045  << 2) + 0xfe024400)
#define DMC_NFQ_TRAS                               ((0x0046  << 2) + 0xfe024400)
#define DMC_NFQ_TRC                                ((0x0047  << 2) + 0xfe024400)
#define DMC_NFQ_TRCD                               ((0x0048  << 2) + 0xfe024400)
#define DMC_NFQ_TRRD                               ((0x0049  << 2) + 0xfe024400)
#define DMC_NFQ_TFAW                               ((0x004a  << 2) + 0xfe024400)
#define DMC_NFQ_TRTP                               ((0x004b  << 2) + 0xfe024400)
#define DMC_NFQ_TWR                                ((0x004c  << 2) + 0xfe024400)
#define DMC_NFQ_TWTR                               ((0x004d  << 2) + 0xfe024400)
#define DMC_NFQ_TCCD                               ((0x004e  << 2) + 0xfe024400)
#define DMC_NFQ_TEXSR                              ((0x004f  << 2) + 0xfe024400)
#define DMC_NFQ_TXS                                ((0x0050  << 2) + 0xfe024400)
#define DMC_NFQ_TXP                                ((0x0051  << 2) + 0xfe024400)
#define DMC_NFQ_TXPDLL                             ((0x0052  << 2) + 0xfe024400)
#define DMC_NFQ_TZQCS                              ((0x0053  << 2) + 0xfe024400)
#define DMC_NFQ_TCKSRE                             ((0x0054  << 2) + 0xfe024400)
#define DMC_NFQ_TCKSRX                             ((0x0055  << 2) + 0xfe024400)
#define DMC_NFQ_TCKE                               ((0x0056  << 2) + 0xfe024400)
#define DMC_NFQ_TMOD                               ((0x0057  << 2) + 0xfe024400)
#define DMC_NFQ_TDQS                               ((0x0058  << 2) + 0xfe024400)
#define DMC_NFQ_TRSTL                              ((0x0059  << 2) + 0xfe024400)
#define DMC_NFQ_TZQLAT                             ((0x005a  << 2) + 0xfe024400)
#define DMC_NFQ_TMRR                               ((0x005b  << 2) + 0xfe024400)
#define DMC_NFQ_TCKESR                             ((0x005c  << 2) + 0xfe024400)
#define DMC_NFQ_TDPD                               ((0x005d  << 2) + 0xfe024400)
#define DMC_NFQ_DFITCTRLDELAY                      ((0x005e  << 2) + 0xfe024400)
#define DMC_NFQ_DFITPHYWRDATA                      ((0x005f  << 2) + 0xfe024400)
#define DMC_NFQ_DFITPHYWRLAT                       ((0x0060  << 2) + 0xfe024400)
#define DMC_NFQ_DFITRDDATAEN                       ((0x0061  << 2) + 0xfe024400)
#define DMC_NFQ_DFITPHYRDLAT                       ((0x0062  << 2) + 0xfe024400)
#define DMC_NFQ_DFITCTRLUPDMIN                     ((0x0063  << 2) + 0xfe024400)
#define DMC_NFQ_DFITCTRLUPDMAX                     ((0x0064  << 2) + 0xfe024400)
#define DMC_NFQ_DFITMSTRRESP                       ((0x0065  << 2) + 0xfe024400)
#define DMC_NFQ_DFITREFMSKI                        ((0x0066  << 2) + 0xfe024400)
#define DMC_NFQ_DFITCTRLUPDI                       ((0x0067  << 2) + 0xfe024400)
#define DMC_NFQ_DFITDRAMCLK                        ((0x0068  << 2) + 0xfe024400)
#define DMC_NFQ_DFITLPRESP                         ((0x006a  << 2) + 0xfe024400)
#define DMC_NFQ_DFITPHYMSTR                        ((0x006b  << 2) + 0xfe024400)
#define DMC_NFQ_TCKECK                             ((0x006c  << 2) + 0xfe024400)
#define DMC_NFQ_TREFI                              ((0x006d  << 2) + 0xfe024400)
#define DMC_NFQ_TSR                                ((0x006e  << 2) + 0xfe024400)
#define DMC_NFQ_TCCDMW                             ((0x006f  << 2) + 0xfe024400)
#define DMC_NFQ_TESCKE                             ((0x0070  << 2) + 0xfe024400)
#define DMC_NFQ_TREFI_DDR3                         ((0x0071  << 2) + 0xfe024400)
#define DMC_NFQ_TZQCAL                             ((0x0072  << 2) + 0xfe024400)
#define DMC_NFQ_T10US                              ((0x0073  << 2) + 0xfe024400)
#define DMC_NFQ_TMRRI                              ((0x0074  << 2) + 0xfe024400)
#define DMC_DRAM_DFITPHYUPDTYPE0                   ((0x0080  << 2) + 0xfe024400)
 //dfi_phyupd_ack hold time for dfi_phyupd_req type = 0.
#define DMC_DRAM_DFITPHYUPDTYPE1                   ((0x0081  << 2) + 0xfe024400)
 //dfi_phyupd_ack hold time for dfi_phyupd_req type = 1.
#define DMC_DRAM_DFITPHYUPDTYPE2                   ((0x0082  << 2) + 0xfe024400)
 //dfi_phyupd_ack hold time for dfi_phyupd_req type = 2.
#define DMC_DRAM_DFITPHYUPDTYPE3                   ((0x0083  << 2) + 0xfe024400)
 //dfi_phyupd_ack hold time for dfi_phyupd_req type = 3.
#define DMC_DRAM_DFIODTCFG                         ((0x0084  << 2) + 0xfe024400)
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
#define DMC_DRAM_DFIODTCFG1                        ((0x0085  << 2) + 0xfe024400)
  //bit 27:24  ODT length for BL8 read transfer.
  //bit 19:16. ODT length for BL8 write transfer.
  //bit 12:8.  ODT latency for reads.  suppose to be 0.
  //bit 4:0.   ODT latency for writes.  suppose to be 0.
#define DMC_DRAM_MCFG                              ((0x0086  << 2) + 0xfe024400)
 //bit 16.   send refresh command after finish frequency change. 1 : enable. 0 : disable.
 //bit 15.   send refresh command after finish LPDDR4 retraining. 1 : enable. 0 : disable.
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
#define DMC_DRAM_DFI_CTRL                          ((0x0089  << 2) + 0xfe024400)
  //bit 20 siu_dfi1_phymstr_ack_en
  //bit 19 siu_dfi_phymstr_req_and
  //bit 18 siu_dfi_phymstr_req_or
  //bit 17 siu_dfi_phymstr_type_sel
  //bit 16 siu_dfi_phymstr_cs_sel
  //bit 15 siu_dfi1_lp_en
  //bit 14 siu_dfi_lp_ack_and
  //bit 13 siu_dfi_lp_ack_or
  //bit 12 siu_dfi1_init_start_en
  //bit 11 siu_dfi_init_com_and
  //bit 10 siu_dfi_init_com_or
  //bit  9 siu_dfi1_freq_en
  //bit  8 siu_dfi1_dram_clk_dis_en
  //bit  7 siu_dfi_phyupd_type_sel
  //bit  6 siu_dfi1_phyupd_ack_en
  //bit  5 siu_dfi_phyupd_req_and
  //bit  4 siu_dfi_phyupd_req_or
  //bit  3 siu_dfi_ctrlupd_ack_and
  //bit  2 siu_dfi_ctrlupd_ack_or
  //bit  1 siu_dfi1_ctrlupd_req_en
  //bit  0 siu_dfi1_cmd_en
#define DMC_DRAM_DFIINITCFG                        ((0x008a  << 2) + 0xfe024400)
  //bit 31.   dfi_init_complete status. read only.
  //bit 15:14.  Frequency set 1 dfi_freq_ratio value.
  //bit 12:8    Frequency set 1 dfi_freq value.
  //bit 7:6     Frequency set 0 dfi_freq_ratio value.
  //bit 5:1     Frequency set 0 dfi_freq value.
  //bit 0.      dfi_init_start value  can be use manually config dfi_init_start signal.
#define DMC_DRAM_ZQ_CTRL                           ((0x008b  << 2) + 0xfe024400)
  // only 1 bit can be enabled at same time.
  //bit 2  send ZQCS command to RANK0 then send comand to RANK1.
  //bit 1. send ZQCS command to both RANK0 and RANK1 together.
  //bit 0. send ZQCS command to only rank0.
#define DMC_DRAM_APD_CTRL                          ((0x008c  << 2) + 0xfe024400)
 //bit 19:16  DFI_LP_WAKEUP value in APD DFI_LP_REQ mode
 //bit 12    1: exit power down slow mode(waiting PLL LOCK).  0 : fast mode.
 //bit 11    enable DFI_LP_REQ when enter Auto power down mode.
 //bit 10    disable DFI_clk_disable when enter auto power down mode.
 //bit 9:0    0  disable auto power down mode.
            //non zero value to enable auto power down when DMC is in idle state for this number of clock cycles.
#define DMC_DRAM_ASR_CTRL                          ((0x008d  << 2) + 0xfe024400)
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
#define DMC_DRAM_PHYMSTR_CTRL                      ((0x0090  << 2) + 0xfe024400)
  //not used in G12A. should be all 0.
#define DMC_DRAM_DFIODTRANKMAP                     ((0x0091  << 2) + 0xfe024400)
 //not used in G12A.
#define DMC_DRAM_REFR_CTRL                         ((0x0092  << 2) + 0xfe024400)
  //bit 17:8 auto refresh request pending cnt if there's page hit request.
  //bit 6  Disabled auto refresh command if over 16 auto refresh command sent in 2 TREFI_DDR3 period
  //bit 5  enable dmc send ZQCS command .
  //bit 4. enable dmc send DFI_CTRUPD_REQ.
  //bit 3:1. how many refresh command send for one period. = this number + 1
  //bit 0.  enable dmc send auto refresh command.
#define DMC_DRAM_FREQ_CTRL                         ((0x0093  << 2) + 0xfe024400)
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
#define DMC_DRAM_SCFG                              ((0x0094  << 2) + 0xfe024400)
  // bit 2:0 only one bit can be high at same time.
  // bit 2  1 : to ask PCTL enter ACCESS STATE.  0 : deassert the request.
  // bit 1  1 : to ask PCTL enter SELF REFRESH STATE.  0 : deassert the request.
  // bit 0  1 : to ask PCTL enter STOP/CONFIG STATE .  0 : deassert the request.
#define DMC_DRAM_STAT                              ((0x0095  << 2) + 0xfe024400)
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
#define DMC_DRAM_STAT1                             ((0x0096  << 2) + 0xfe024400)
  //bit 11:8  freq_st.
  //bit 7:5   train_st
  //bit 4:0   dram_phy_st
#define DMC_PHY_RETRAINING_CTRL                    ((0x0097  << 2) + 0xfe024400)
  //bit 31 :  phy_retraining enable.
  //bit 30 :  check  vpu sleep_en.
  //bit 25:24 : retraining dfi_freq[4:3], the [2:0] bit still use the dfi_freq bits to keep the frequency.
  //bit 23:0: retraining period unit : 100ns.
#define DMC_DFI_ERR_STAT                           ((0x0098  << 2) + 0xfe024400)
 //LPDDR4 PHY DFI error infomation.
 //bit 31:20. not used.
 //bit 9.    ddr0_dfi_error
 //bit 8:5   ddr0_dfi_error_info.
 //bit 4.    ddr1_dfi_error.
 //bit 3:0.  ddr1_dfi_error_info.
#define DMC_LP2_TIMER                              ((0x009a  << 2) + 0xfe024400)
//bit 15:0.   timer setting to measure how long the chip is entered LP2 mode.
//this timer is 40bits counter with DMC PCLK.
//we use the high 16bits to compare this register. if the counter is large than this number,  that means  the PHY need addition 10us after wakeup the PHY and before exit self_refresh mode.
#define DMC_DRAM_DFI_SWAP_0                        ((0x00a0  << 2) + 0xfe024400)
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
#define DMC_DRAM_DFI_SWAP_1                        ((0x00a1  << 2) + 0xfe024400)
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
#define DMC_DRAM_DFI_SWAP_2                        ((0x00a2  << 2) + 0xfe024400)
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
#define DMC_DRAM_DFI_SWAP_3                        ((0x00a3  << 2) + 0xfe024400)
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
#define DMC_DRAM_DFI_SWAP_4                        ((0x00a4  << 2) + 0xfe024400)
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
#define DMC_DRAM_DFI_SWAP_5                        ((0x00a5  << 2) + 0xfe024400)
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
#define DMC_DRAM_DFI_SWAP_6                        ((0x00a6  << 2) + 0xfe024400)
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
#define DMC_DRAM_DFI_SWAP_7                        ((0x00a7  << 2) + 0xfe024400)
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
#define DMC_DRAM_DFI_SWAP_8                        ((0x00a8  << 2) + 0xfe024400)
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
#define DMC_DRAM_DFI_SWAP_9                        ((0x00a9  << 2) + 0xfe024400)
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
#define DMC_DRAM_DFI_SWAP_10                       ((0x00aa  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[1] function select
#define DMC_DRAM_DFI_SWAP_11                       ((0x00ab  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[2] function select
#define DMC_DRAM_DFI_SWAP_12                       ((0x00ac  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[3] function select
#define DMC_DRAM_DFI_SWAP_13                       ((0x00ad  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[4] function select
#define DMC_DRAM_DFI_SWAP_14                       ((0x00ae  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[5] function select
#define DMC_DRAM_DFI_SWAP_15                       ((0x00af  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[6] function select
#define DMC_DRAM_DFI_SWAP_16                       ((0x00b0  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[7] function select
#define DMC_DRAM_DFI_SWAP_17                       ((0x00b1  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[8] function select
#define DMC_DRAM_DFI_SWAP_18                       ((0x00b2  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[9] function select
#define DMC_DRAM_DFI_SWAP_19                       ((0x00b3  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[10] function select
#define DMC_DRAM_DFI_SWAP_20                       ((0x00b4  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[11] function select
#define DMC_DRAM_DFI_SWAP_21                       ((0x00b5  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[12] function select
#define DMC_DRAM_DFI_SWAP_22                       ((0x00b6  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[13] function select
#define DMC_DRAM_DFI_SWAP_23                       ((0x00b7  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[14] function select
#define DMC_DRAM_DFI_SWAP_24                       ((0x00b8  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[15] function select
#define DMC_DRAM_DFI_SWAP_25                       ((0x00b9  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[16] function select
#define DMC_DRAM_DFI_SWAP_26                       ((0x00bb  << 2) + 0xfe024400)
  //bit 5:0. dfi_a[17] function select
//`endif
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
// APB_BASE:  APB1_BASE_ADDR = 0xfe024800
// -----------------------------------------------
//`define DMC_STICKY_REG_BASE   32'hfe024800
//those register is for software save some temporary value. and because it's in RAM. it won't lose if DMC get reseted.
#define DMC_STICKY_0                               ((0x0000  << 2) + 0xfe024800)
#define DMC_STICKY_1                               ((0x0001  << 2) + 0xfe024800)
#define DMC_STICKY_2                               ((0x0002  << 2) + 0xfe024800)
#define DMC_STICKY_3                               ((0x0003  << 2) + 0xfe024800)
#define DMC_STICKY_4                               ((0x0004  << 2) + 0xfe024800)
#define DMC_STICKY_5                               ((0x0005  << 2) + 0xfe024800)
#define DMC_STICKY_6                               ((0x0006  << 2) + 0xfe024800)
#define DMC_STICKY_7                               ((0x0007  << 2) + 0xfe024800)
#define DMC_STICKY_8                               ((0x0008  << 2) + 0xfe024800)
#define DMC_STICKY_9                               ((0x0009  << 2) + 0xfe024800)
#define DMC_STICKY_10                              ((0x000a  << 2) + 0xfe024800)
#define DMC_STICKY_11                              ((0x000b  << 2) + 0xfe024800)
#define DMC_STICKY_12                              ((0x000c  << 2) + 0xfe024800)
#define DMC_STICKY_13                              ((0x000d  << 2) + 0xfe024800)
#define DMC_STICKY_14                              ((0x000e  << 2) + 0xfe024800)
#define DMC_STICKY_15                              ((0x000f  << 2) + 0xfe024800)
#define DMC_STICKY_16                              ((0x0010  << 2) + 0xfe024800)
#define DMC_STICKY_17                              ((0x0011  << 2) + 0xfe024800)
#define DMC_STICKY_18                              ((0x0012  << 2) + 0xfe024800)
#define DMC_STICKY_19                              ((0x0013  << 2) + 0xfe024800)
#define DMC_STICKY_20                              ((0x0014  << 2) + 0xfe024800)
#define DMC_STICKY_21                              ((0x0015  << 2) + 0xfe024800)
#define DMC_STICKY_22                              ((0x0016  << 2) + 0xfe024800)
#define DMC_STICKY_23                              ((0x0017  << 2) + 0xfe024800)
#define DMC_STICKY_24                              ((0x0018  << 2) + 0xfe024800)
#define DMC_STICKY_25                              ((0x0019  << 2) + 0xfe024800)
#define DMC_STICKY_26                              ((0x001a  << 2) + 0xfe024800)
#define DMC_STICKY_27                              ((0x001b  << 2) + 0xfe024800)
#define DMC_STICKY_28                              ((0x001c  << 2) + 0xfe024800)
#define DMC_STICKY_29                              ((0x001d  << 2) + 0xfe024800)
#define DMC_STICKY_30                              ((0x001e  << 2) + 0xfe024800)
#define DMC_STICKY_31                              ((0x001f  << 2) + 0xfe024800)
#define DMC_STICKY_32                              ((0x0020  << 2) + 0xfe024800)
#define DMC_STICKY_33                              ((0x0021  << 2) + 0xfe024800)
#define DMC_STICKY_34                              ((0x0022  << 2) + 0xfe024800)
#define DMC_STICKY_35                              ((0x0023  << 2) + 0xfe024800)
#define DMC_STICKY_36                              ((0x0024  << 2) + 0xfe024800)
#define DMC_STICKY_37                              ((0x0025  << 2) + 0xfe024800)
#define DMC_STICKY_38                              ((0x0026  << 2) + 0xfe024800)
#define DMC_STICKY_39                              ((0x0027  << 2) + 0xfe024800)
#define DMC_STICKY_40                              ((0x0028  << 2) + 0xfe024800)
#define DMC_STICKY_41                              ((0x0029  << 2) + 0xfe024800)
#define DMC_STICKY_42                              ((0x002a  << 2) + 0xfe024800)
#define DMC_STICKY_43                              ((0x002b  << 2) + 0xfe024800)
#define DMC_STICKY_44                              ((0x002c  << 2) + 0xfe024800)
#define DMC_STICKY_45                              ((0x002d  << 2) + 0xfe024800)
#define DMC_STICKY_46                              ((0x002e  << 2) + 0xfe024800)
#define DMC_STICKY_47                              ((0x002f  << 2) + 0xfe024800)
#define DMC_STICKY_48                              ((0x0030  << 2) + 0xfe024800)
#define DMC_STICKY_49                              ((0x0031  << 2) + 0xfe024800)
#define DMC_STICKY_50                              ((0x0032  << 2) + 0xfe024800)
#define DMC_STICKY_51                              ((0x0033  << 2) + 0xfe024800)
#define DMC_STICKY_52                              ((0x0034  << 2) + 0xfe024800)
#define DMC_STICKY_53                              ((0x0035  << 2) + 0xfe024800)
#define DMC_STICKY_54                              ((0x0036  << 2) + 0xfe024800)
#define DMC_STICKY_55                              ((0x0037  << 2) + 0xfe024800)
#define DMC_STICKY_56                              ((0x0038  << 2) + 0xfe024800)
#define DMC_STICKY_57                              ((0x0039  << 2) + 0xfe024800)
#define DMC_STICKY_58                              ((0x003a  << 2) + 0xfe024800)
#define DMC_STICKY_59                              ((0x003b  << 2) + 0xfe024800)
#define DMC_STICKY_60                              ((0x003c  << 2) + 0xfe024800)
#define DMC_STICKY_61                              ((0x003d  << 2) + 0xfe024800)
#define DMC_STICKY_62                              ((0x003e  << 2) + 0xfe024800)
#define DMC_STICKY_63                              ((0x003f  << 2) + 0xfe024800)
//`endif
//
// Closing file:  ../mmc_lp4/dmc/rtl/dmc_sticky_reg.vh
//
//
// Reading file:  ../mmc_lp4/dmc_clk_freq/rtl/dmc_clk_freq.vh
//
//`ifdef DMC_FREQ_REG_DEFINE
//`else
//`define DMC_FREQ_REG_DEFINE
// -----------------------------------------------
// APB_BASE:  APB1_BASE_ADDR = 0xfe024c00
// -----------------------------------------------
//`define DMC_FREQ_REG_BASE      32'hfe024c00
#define AM_DDR_PLL_CNTL0                           ((0x0000  << 2) + 0xfe024c00)
  //bit 29    : dpll_reset.
  //bit 28    : dpll_en.
  //bit 27:26 : dpll_clk_en
  //bit
  //bit 20:19 : od1
  //bit 18:16 : od
  //bit 14:10 : dpll_ref_div_n
  //bit 8:0   : dpll_int_num
#define AM_DDR_PLL_CNTL1                           ((0x0001  << 2) + 0xfe024c00)
  //bit 18:0    : ddr_dpll_frac
#define AM_DDR_PLL_CNTL2                           ((0x0002  << 2) + 0xfe024c00)
  //bit 22:20  : fref_sel
  //bit 17:16  : os_ssc
  //bit 15:12  : ssc_str_m
  //bit 8      : ssc_en
  //bit 7:4    : ssc_dep_sel
  //bit 1:0    : dpll ss_mode.
#define AM_DDR_PLL_CNTL3                           ((0x0003  << 2) + 0xfe024c00)
  //bit 31     : afc bypass
  //bit 30     : afc clk sel
  //bit 29     : code new
  //bit 28     : dco_m_en
  //bit 27     : dco_sdm_en
  //bit 26     : div2
  //bit 25     : div mode
  //bit 24     : fast_lock mode
  //bit 23     : fb_pre_div
  //bit 22     : filter_mode
  //bit 21     : fix_en
  //bit 20     : freq_shift_en
  //bit 19     : load
  //bit 18     : load_en
  //bit 17     : lock_f
  //bit 16     : pulse_width_en
  //bit 15     : sdmnc_en
  //bit 14     : sdmnc_mode
  //bit 13     : sdmnc_range
  //bit 12     : tdc_en
  //bit 11     : tdc_mode_sel
  //bit 10     :  wait_en
#define AM_DDR_PLL_CNTL4                           ((0x0004  << 2) + 0xfe024c00)
  //bit 1:0    : pfd_gain
  //bit 7:4    : filter_pvt1
  //bit 11:8   : filter pvt2
  //bit 13:12  : acq_gain
  //bit 18:16  : lambda0
  //bit 22:20  : lambda1
  //bit 26:24  : rou
  //bit 30:28  : alpha
#define AM_DDR_PLL_CNTL5                           ((0x0005  << 2) + 0xfe024c00)
  //bit 15:0   : reve
  //bit 21:16  : lm_s
  //bit 27:24  : lm_w
  //bit 30:28  : adj_vco_ldo
#define AM_DDR_PLL_CNTL6                           ((0x0006  << 2) + 0xfe024c00)
  //bit 15:0   : reve
  //bit 21:16  : lm_s
  //bit 27:24  : lm_w
  //bit 30:28  : adj_vco_ldo
#define AM_DDR_PLL_STS                             ((0x0007  << 2) + 0xfe024c00)
  //bit 31     : DDR_PLL_LOCK
  //bit 30:19  : not used.
  //bit 18     : DDR_AFC_DONE
  //bit 17     : DDR_PLL_LOCK
  //bit 16:7   : DDR_DPLL_OUT_RSV
  //bit 6:0    : DDR_SDMNC_MONITOR
#define DDR_CLK_CNTL                               ((0x0008  << 2) + 0xfe024c00)
  //bit 31     ddr_pll_clk enable. enable the clock from DDR_PLL to clock generateion.
  // whenever change the DDR_PLL frequency, disable the clock, after the DDR_PLL locked, then enable it again.
  //bit 30.    ddr_pll_prod_test_en.  enable the clock to clock/32 which to clock frequency measurement and production test pin.
  //bit 29.    not used.
  //bit 28.    clock generation logic soft reset. 0 = reset.
  //bit 27.    phy_4xclk phase inverter..
  //bit 25.    DDRPHY DfiClk/DfiCtlClk/DMC clock selection.  1:  AM_PLL clk output /2.  0: directly output from AM_PLL .
  //bit 24.    enable AM_PLL CLK output /2 function.   1: enable.  0: disable.   if try to use this clkoutput/2 function.
  //bit 26.    pll_freq divide/2. 1:  use pll div/2 clock as the n_clk. 0: use pll clock as n_clk.  this setting is used for the synopsys DDR PHY PLL fast lock mode.
  //bit 2. enable dmc_clk.
  //bit 1. enable LPDDR4-PHY DfiClk.
  //bit 0. enable LPDDR4-PHY DfiCtlClk.
#define DDR_PHY_CTRL                               ((0x0009  << 2) + 0xfe024c00)
  // LPDDR4 power on reset need to special combination of PwrOkIn and phy_reset_n.
  // please check the PHY PUB data book for detail.
  //bit 4.   DDR PHY PwrOkIn pin.
  //bit 1.   DDR PHY APB soft reset_n.
  //bit 0.   phy_reset_n.
#define AM_DDR_PLL_FREQ1_OD                        ((0x000c  << 2) + 0xfe024c00)
 //freqency set 1 PLL OD and OD1 setting. when change to freqency 1 will use this setting.
//frequency set 0 use original DDR_PLL_CNTL0 setting.
 //bit 8     currunt FREQ selection.  it can forced to change to select which freqency to select, or it can auto changed by FREQ change hardware.
 //bit 5:4   OD1.
 //bit 2:0.  OD.
//`endif
//
// Closing file:  ../mmc_lp4/dmc_clk_freq/rtl/dmc_clk_freq.vh
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
#define PADCTRL_MISC_CTRL0                         ((0x001a  << 2) + 0xfe000400)
#define PADCTRL_GPIOD_I                            ((0x0020  << 2) + 0xfe000400)
#define PADCTRL_GPIOD_O                            ((0x0021  << 2) + 0xfe000400)
#define PADCTRL_GPIOD_OEN                          ((0x0022  << 2) + 0xfe000400)
#define PADCTRL_GPIOD_PULL_EN                      ((0x0023  << 2) + 0xfe000400)
#define PADCTRL_GPIOD_PULL_UP                      ((0x0024  << 2) + 0xfe000400)
#define PADCTRL_GPIOD_DS                           ((0x0025  << 2) + 0xfe000400)
#define PADCTRL_GPIOE_I                            ((0x0030  << 2) + 0xfe000400)
#define PADCTRL_GPIOE_O                            ((0x0031  << 2) + 0xfe000400)
#define PADCTRL_GPIOE_OEN                          ((0x0032  << 2) + 0xfe000400)
#define PADCTRL_GPIOE_PULL_EN                      ((0x0033  << 2) + 0xfe000400)
#define PADCTRL_GPIOE_PULL_UP                      ((0x0034  << 2) + 0xfe000400)
#define PADCTRL_GPIOE_DS                           ((0x0035  << 2) + 0xfe000400)
#define PADCTRL_GPIOB_I                            ((0x0040  << 2) + 0xfe000400)
#define PADCTRL_GPIOB_O                            ((0x0041  << 2) + 0xfe000400)
#define PADCTRL_GPIOB_OEN                          ((0x0042  << 2) + 0xfe000400)
#define PADCTRL_GPIOB_PULL_EN                      ((0x0043  << 2) + 0xfe000400)
#define PADCTRL_GPIOB_PULL_UP                      ((0x0044  << 2) + 0xfe000400)
#define PADCTRL_GPIOB_DS                           ((0x0045  << 2) + 0xfe000400)
#define PADCTRL_GPIOC_I                            ((0x0050  << 2) + 0xfe000400)
#define PADCTRL_GPIOC_O                            ((0x0051  << 2) + 0xfe000400)
#define PADCTRL_GPIOC_OEN                          ((0x0052  << 2) + 0xfe000400)
#define PADCTRL_GPIOC_PULL_EN                      ((0x0053  << 2) + 0xfe000400)
#define PADCTRL_GPIOC_PULL_UP                      ((0x0054  << 2) + 0xfe000400)
#define PADCTRL_GPIOC_DS                           ((0x0055  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_I                            ((0x0060  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_O                            ((0x0061  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_OEN                          ((0x0062  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_PULL_EN                      ((0x0063  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_PULL_UP                      ((0x0064  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_DS                           ((0x0065  << 2) + 0xfe000400)
#define PADCTRL_GPIOX_DS_EXT                       ((0x0066  << 2) + 0xfe000400)
#define PADCTRL_GPIOM_I                            ((0x0070  << 2) + 0xfe000400)
#define PADCTRL_GPIOM_O                            ((0x0071  << 2) + 0xfe000400)
#define PADCTRL_GPIOM_OEN                          ((0x0072  << 2) + 0xfe000400)
#define PADCTRL_GPIOM_PULL_EN                      ((0x0073  << 2) + 0xfe000400)
#define PADCTRL_GPIOM_PULL_UP                      ((0x0074  << 2) + 0xfe000400)
#define PADCTRL_GPIOM_DS                           ((0x0075  << 2) + 0xfe000400)
#define PADCTRL_GPIOA_I                            ((0x0080  << 2) + 0xfe000400)
#define PADCTRL_GPIOA_O                            ((0x0081  << 2) + 0xfe000400)
#define PADCTRL_GPIOA_OEN                          ((0x0082  << 2) + 0xfe000400)
#define PADCTRL_GPIOA_PULL_EN                      ((0x0083  << 2) + 0xfe000400)
#define PADCTRL_GPIOA_PULL_UP                      ((0x0084  << 2) + 0xfe000400)
#define PADCTRL_GPIOA_DS                           ((0x0085  << 2) + 0xfe000400)
#define PADCTRL_TESTN_I                            ((0x0090  << 2) + 0xfe000400)
#define PADCTRL_TESTN_O                            ((0x0091  << 2) + 0xfe000400)
#define PADCTRL_TESTN_OEN                          ((0x0092  << 2) + 0xfe000400)
#define PADCTRL_TESTN_PULL_EN                      ((0x0093  << 2) + 0xfe000400)
#define PADCTRL_TESTN_PULL_UP                      ((0x0094  << 2) + 0xfe000400)
#define PADCTRL_TESTN_DS                           ((0x0095  << 2) + 0xfe000400)
#define PADCTRL_MUTE_CTRL                          ((0x00a0  << 2) + 0xfe000400)
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
#define CLKTREE_SYS_CLK_EN0                        ((0x0006  << 2) + 0xfe000800)
#define CLKTREE_SYS_CLK_EN1                        ((0x0007  << 2) + 0xfe000800)
#define CLKTREE_SYS_CLK_EN2                        ((0x0008  << 2) + 0xfe000800)
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
#define CLKTREE_WAVE_CLK_CTRL0                     ((0x0041  << 2) + 0xfe000800)
#define CLKTREE_WAVE_CLK_CTRL1                     ((0x0042  << 2) + 0xfe000800)
#define CLKTREE_JPEG_CLK_CTRL                      ((0x0043  << 2) + 0xfe000800)
#define CLKTREE_MIPI_ISP_CLK_CTRL                  ((0x0044  << 2) + 0xfe000800)
#define CLKTREE_NNA_CLK_CTRL                       ((0x0045  << 2) + 0xfe000800)
#define CLKTREE_GDC_CLK_CTRL                       ((0x0046  << 2) + 0xfe000800)
#define CLKTREE_GE2D_CLK_CTRL                      ((0x0047  << 2) + 0xfe000800)
#define CLKTREE_SD_EMMC_CLK_CTRL1                  ((0x0048  << 2) + 0xfe000800)
#define CLKTREE_ETH_CLK_CTRL                       ((0x0049  << 2) + 0xfe000800)
#define CLKTREE_PWM_CLK_GH_CTRL                    ((0x004a  << 2) + 0xfe000800)
#define CLKTREE_PWM_CLK_IJ_CTRL                    ((0x004b  << 2) + 0xfe000800)
#define CLKTREE_MBIST_ATSPEED_CTRL                 ((0x004c  << 2) + 0xfe000800)
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
#define I2C_M_A_CNTL_DELY1                         ((0x0009  << 2) + 0xfe001400)
#define I2C_M_A_CNTL_DELY2                         ((0x000a  << 2) + 0xfe001400)
#define I2C_M_A_LOW_DELY                           ((0x000b  << 2) + 0xfe001400)
#define I2C_M_A_HIGH_DELY                          ((0x000c  << 2) + 0xfe001400)
#define I2C_M_A_FIFO_CTRL_REG                      ((0x000d  << 2) + 0xfe001400)
#define I2C_M_A_STATE_REG                          ((0x000e  << 2) + 0xfe001400)
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
#define SYSCTRL_OSC_RING_CTRL1                     ((0x0054  << 2) + 0xfe005800)
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
#define I2C_M_B_CNTL_DELY1                         ((0x0009  << 2) + 0xfe005c00)
#define I2C_M_B_CNTL_DELY2                         ((0x000a  << 2) + 0xfe005c00)
#define I2C_M_B_LOW_DELY                           ((0x000b  << 2) + 0xfe005c00)
#define I2C_M_B_HIGH_DELY                          ((0x000c  << 2) + 0xfe005c00)
#define I2C_M_B_FIFO_CTRL_REG                      ((0x000d  << 2) + 0xfe005c00)
#define I2C_M_B_STATE_REG                          ((0x000e  << 2) + 0xfe005c00)
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
#define IRQCTRL_IRQOUT_MASK4                       ((0x0004  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_MASK5                       ((0x0005  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_INV0                        ((0x0010  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_INV1                        ((0x0011  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_INV2                        ((0x0012  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_INV3                        ((0x0013  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_INV4                        ((0x0014  << 2) + 0xfe006400)
#define IRQCTRL_IRQOUT_INV5                        ((0x0015  << 2) + 0xfe006400)
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
#define I2C_M_C_CNTL_DELY1                         ((0x0009  << 2) + 0xfe006800)
#define I2C_M_C_CNTL_DELY2                         ((0x000a  << 2) + 0xfe006800)
#define I2C_M_C_LOW_DELY                           ((0x000b  << 2) + 0xfe006800)
#define I2C_M_C_HIGH_DELY                          ((0x000c  << 2) + 0xfe006800)
#define I2C_M_C_FIFO_CTRL_REG                      ((0x000d  << 2) + 0xfe006800)
#define I2C_M_C_STATE_REG                          ((0x000e  << 2) + 0xfe006800)
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
#define I2C_M_D_CNTL_DELY1                         ((0x0009  << 2) + 0xfe006c00)
#define I2C_M_D_CNTL_DELY2                         ((0x000a  << 2) + 0xfe006c00)
#define I2C_M_D_LOW_DELY                           ((0x000b  << 2) + 0xfe006c00)
#define I2C_M_D_HIGH_DELY                          ((0x000c  << 2) + 0xfe006c00)
#define I2C_M_D_FIFO_CTRL_REG                      ((0x000d  << 2) + 0xfe006c00)
#define I2C_M_D_STATE_REG                          ((0x000e  << 2) + 0xfe006c00)
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
//  CPUCTRL_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe007400
// -----------------------------------------------
#define CPUCTRL_CTRL0                              ((0x0000  << 2) + 0xfe007400)
#define CPUCTRL_CTRL1                              ((0x0001  << 2) + 0xfe007400)
#define CPUCTRL_CTRL2                              ((0x0002  << 2) + 0xfe007400)
#define CPUCTRL_CTRL3                              ((0x0003  << 2) + 0xfe007400)
#define CPUCTRL_CTRL4                              ((0x0004  << 2) + 0xfe007400)
#define CPUCTRL_CTRL5                              ((0x0005  << 2) + 0xfe007400)
#define CPUCTRL_CTRL6                              ((0x0006  << 2) + 0xfe007400)
#define CPUCTRL_CTRL7                              ((0x0007  << 2) + 0xfe007400)
#define CPUCTRL_CTRL8                              ((0x0008  << 2) + 0xfe007400)
#define CPUCTRL_CTRL9                              ((0x0009  << 2) + 0xfe007400)
#define CPUCTRL_CTRL10                             ((0x000a  << 2) + 0xfe007400)
#define CPUCTRL_CTRL11                             ((0x000b  << 2) + 0xfe007400)
#define CPUCTRL_CTRL12                             ((0x000c  << 2) + 0xfe007400)
#define CPUCTRL_CTRL13                             ((0x000d  << 2) + 0xfe007400)
#define CPUCTRL_CTRL14                             ((0x000e  << 2) + 0xfe007400)
#define CPUCTRL_CTRL15                             ((0x000f  << 2) + 0xfe007400)
#define CPUCTRL_STS0                               ((0x0010  << 2) + 0xfe007400)
#define CPUCTRL_STS1                               ((0x0011  << 2) + 0xfe007400)
#define CPUCTRL_STS2                               ((0x0012  << 2) + 0xfe007400)
#define CPUCTRL_STS3                               ((0x0013  << 2) + 0xfe007400)
#define CPUCTRL_STS4                               ((0x0014  << 2) + 0xfe007400)
#define CPUCTRL_STS5                               ((0x0015  << 2) + 0xfe007400)
#define CPUCTRL_STS6                               ((0x0016  << 2) + 0xfe007400)
#define CPUCTRL_STS7                               ((0x0017  << 2) + 0xfe007400)
#define CPUCTRL_STS8                               ((0x0018  << 2) + 0xfe007400)
#define CPUCTRL_STS9                               ((0x0019  << 2) + 0xfe007400)
#define CPUCTRL_CLK_CTRL0                          ((0x0020  << 2) + 0xfe007400)
#define CPUCTRL_CLK_CTRL1                          ((0x0021  << 2) + 0xfe007400)
#define CPUCTRL_CLK_CTRL2                          ((0x0022  << 2) + 0xfe007400)
#define CPUCTRL_CLK_CTRL3                          ((0x0023  << 2) + 0xfe007400)
#define CPUCTRL_CLK_CTRL4                          ((0x0024  << 2) + 0xfe007400)
#define CPUCTRL_CLK_CTRL5                          ((0x0025  << 2) + 0xfe007400)
#define CPUCTRL_CLK_CTRL6                          ((0x0026  << 2) + 0xfe007400)
#define CPUCTRL_CLK_CTRL7                          ((0x0027  << 2) + 0xfe007400)
#define CPUCTRL_RESET_CTRL                         ((0x0030  << 2) + 0xfe007400)
#define CPUCTRL_ROM_DISABLE                        ((0x0031  << 2) + 0xfe007400)
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
#define PWRCTRL_PWR_ACK1                           ((0x0005  << 2) + 0xfe007800)
#define PWRCTRL_ISO_EN1                            ((0x0006  << 2) + 0xfe007800)
#define PWRCTRL_PWR_OFF1                           ((0x0007  << 2) + 0xfe007800)
#define PWRCTRL_FOCRSTN0                           ((0x0008  << 2) + 0xfe007800)
#define PWRCTRL_FOCRSTN1                           ((0x0009  << 2) + 0xfe007800)
#define PWRCTRL_R_ISO_EN0                          ((0x0010  << 2) + 0xfe007800)
#define PWRCTRL_R_PWR_OFF0                         ((0x0011  << 2) + 0xfe007800)
#define PWRCTRL_R_ISO_EN1                          ((0x0012  << 2) + 0xfe007800)
#define PWRCTRL_R_PWR_OFF1                         ((0x0013  << 2) + 0xfe007800)
#define PWRCTRL_R_FOCRSTN0                         ((0x0014  << 2) + 0xfe007800)
#define PWRCTRL_R_FOCRSTN1                         ((0x0015  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD0                            ((0x0020  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD1                            ((0x0021  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD2                            ((0x0022  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD3                            ((0x0023  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD4                            ((0x0024  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD5                            ((0x0025  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD6                            ((0x0026  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD7                            ((0x0027  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD8                            ((0x0028  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD9                            ((0x0029  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD10                           ((0x002a  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD11                           ((0x002b  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD12                           ((0x002c  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD13                           ((0x002d  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD14                           ((0x002e  << 2) + 0xfe007800)
#define PWRCTRL_MEM_PD15                           ((0x002f  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD0                          ((0x0030  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD1                          ((0x0031  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD2                          ((0x0032  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD3                          ((0x0033  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD4                          ((0x0034  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD5                          ((0x0035  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD6                          ((0x0036  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD7                          ((0x0037  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD8                          ((0x0038  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD9                          ((0x0039  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD10                         ((0x003a  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD11                         ((0x003b  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD12                         ((0x003c  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD13                         ((0x003d  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD14                         ((0x003e  << 2) + 0xfe007800)
#define PWRCTRL_R_MEM_PD15                         ((0x003f  << 2) + 0xfe007800)
#define PWRCTRL_AUTO_OFF_CTRL                      ((0x0040  << 2) + 0xfe007800)
#define PWRCTRL_AUTO_OFF                           ((0x0041  << 2) + 0xfe007800)
#define PWRCTRL_AUTO_OFF_STS                       ((0x0042  << 2) + 0xfe007800)
#define PWRCTRL_TIMER_TH_01                        ((0x0043  << 2) + 0xfe007800)
#define PWRCTRL_TIMER_TH_23                        ((0x0044  << 2) + 0xfe007800)
#define PWRCTRL_TIMER_TH_45                        ((0x0045  << 2) + 0xfe007800)
#define PWRCTRL_TIMER_TH_67                        ((0x0046  << 2) + 0xfe007800)
#define PWRCTRL_AUTO_OFF_CTRL1                     ((0x0048  << 2) + 0xfe007800)
#define PWRCTRL_MASK_PWR_ACK0                      ((0x0050  << 2) + 0xfe007800)
#define PWRCTRL_MASK_RSTN_OFF0                     ((0x0051  << 2) + 0xfe007800)
#define PWRCTRL_MASK_RSTN_ON0                      ((0x0052  << 2) + 0xfe007800)
#define PWRCTRL_MASK_PWR_OFF0                      ((0x0053  << 2) + 0xfe007800)
#define PWRCTRL_MASK_PWR_ON0                       ((0x0054  << 2) + 0xfe007800)
#define PWRCTRL_MASK_ISO_OFF0                      ((0x0055  << 2) + 0xfe007800)
#define PWRCTRL_MASK_ISO_ON0                       ((0x0056  << 2) + 0xfe007800)
#define PWRCTRL_MASK_PWR_ACK1                      ((0x0058  << 2) + 0xfe007800)
#define PWRCTRL_MASK_RSTN_OFF1                     ((0x0059  << 2) + 0xfe007800)
#define PWRCTRL_MASK_RSTN_ON1                      ((0x005a  << 2) + 0xfe007800)
#define PWRCTRL_MASK_PWR_OFF1                      ((0x005b  << 2) + 0xfe007800)
#define PWRCTRL_MASK_PWR_ON1                       ((0x005c  << 2) + 0xfe007800)
#define PWRCTRL_MASK_ISO_OFF1                      ((0x005d  << 2) + 0xfe007800)
#define PWRCTRL_MASK_ISO_ON1                       ((0x005e  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF0                      ((0x0060  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF1                      ((0x0061  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF2                      ((0x0062  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF3                      ((0x0063  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF4                      ((0x0064  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF5                      ((0x0065  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF6                      ((0x0066  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF7                      ((0x0067  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF8                      ((0x0068  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF9                      ((0x0069  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF10                     ((0x006a  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF11                     ((0x006b  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF12                     ((0x006c  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF13                     ((0x006d  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF14                     ((0x006e  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_OFF15                     ((0x006f  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON0                       ((0x0070  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON1                       ((0x0071  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON2                       ((0x0072  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON3                       ((0x0073  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON4                       ((0x0074  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON5                       ((0x0075  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON6                       ((0x0076  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON7                       ((0x0077  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON8                       ((0x0078  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON9                       ((0x0079  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON10                      ((0x007a  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON11                      ((0x007b  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON12                      ((0x007c  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON13                      ((0x007d  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON14                      ((0x007e  << 2) + 0xfe007800)
#define PWRCTRL_MASK_MEM_ON15                      ((0x007f  << 2) + 0xfe007800)
#define PWRCTRL_NNA_AXI_PWR_CNTL                   ((0x0080  << 2) + 0xfe007800)
//========================================================================
//  APB0_ANA_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe007c00
// -----------------------------------------------
#define ANACTRL_PLL_GATE_DIS                       ((0x0010  << 2) + 0xfe007c00)
#define ANACTRL_FIXPLL_CTRL0                       ((0x0020  << 2) + 0xfe007c00)
#define ANACTRL_FIXPLL_CTRL1                       ((0x0021  << 2) + 0xfe007c00)
#define ANACTRL_FIXPLL_CTRL2                       ((0x0022  << 2) + 0xfe007c00)
#define ANACTRL_FIXPLL_CTRL3                       ((0x0023  << 2) + 0xfe007c00)
#define ANACTRL_FIXPLL_CTRL4                       ((0x0024  << 2) + 0xfe007c00)
#define ANACTRL_FIXPLL_CTRL5                       ((0x0025  << 2) + 0xfe007c00)
#define ANACTRL_FIXPLL_CTRL6                       ((0x0026  << 2) + 0xfe007c00)
#define ANACTRL_FIXPLL_STS                         ((0x0027  << 2) + 0xfe007c00)
#define ANACTRL_GPPLL_CTRL0                        ((0x0030  << 2) + 0xfe007c00)
#define ANACTRL_GPPLL_CTRL1                        ((0x0031  << 2) + 0xfe007c00)
#define ANACTRL_GPPLL_CTRL2                        ((0x0032  << 2) + 0xfe007c00)
#define ANACTRL_GPPLL_CTRL3                        ((0x0033  << 2) + 0xfe007c00)
#define ANACTRL_GPPLL_CTRL4                        ((0x0034  << 2) + 0xfe007c00)
#define ANACTRL_GPPLL_CTRL5                        ((0x0035  << 2) + 0xfe007c00)
#define ANACTRL_GPPLL_STS                          ((0x0036  << 2) + 0xfe007c00)
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
#define ANACTRL_CSI_PHY_CNTL0                      ((0x0090  << 2) + 0xfe007c00)
#define ANACTRL_CSI_PHY_CNTL1                      ((0x0091  << 2) + 0xfe007c00)
#define ANACTRL_CSI_PHY_CNTL2                      ((0x0092  << 2) + 0xfe007c00)
#define ANACTRL_CSI_PHY_CNTL3                      ((0x0093  << 2) + 0xfe007c00)
#define ANACTRL_CSI_PHY_STS                        ((0x0094  << 2) + 0xfe007c00)
#define ANACTRL_ISP_MAX_ADDR_CNTL                  ((0x0095  << 2) + 0xfe007c00)
//========================================================================
//  APB0_ETHTOP - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe009000
// -----------------------------------------------
#define ETHTOP_CNTL0                               ((0x0000  << 2) + 0xfe009000)
#define ETHTOP_CNTL1                               ((0x0001  << 2) + 0xfe009000)
//========================================================================
//  APB0_ETHPHY - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe009400
// -----------------------------------------------
#define ETH_PHY_DBG_CTL0                           ((0x0000  << 2) + 0xfe009400)
#define ETH_PHY_DBG_CTL1                           ((0x0001  << 2) + 0xfe009400)
#define ETH_PHY_DBG_CFG0                           ((0x0002  << 2) + 0xfe009400)
#define ETH_PHY_DBG_CFG1                           ((0x0003  << 2) + 0xfe009400)
#define ETH_PHY_DBG_CFG2                           ((0x0004  << 2) + 0xfe009400)
#define ETH_PHY_DBG_CFG3                           ((0x0005  << 2) + 0xfe009400)
#define ETH_PHY_DBG_CFG4                           ((0x0006  << 2) + 0xfe009400)
#define ETH_PLL_STS                                ((0x0010  << 2) + 0xfe009400)
#define ETH_PLL_CTL0                               ((0x0011  << 2) + 0xfe009400)
#define ETH_PLL_CTL1                               ((0x0012  << 2) + 0xfe009400)
#define ETH_PLL_CTL2                               ((0x0013  << 2) + 0xfe009400)
#define ETH_PLL_CTL3                               ((0x0014  << 2) + 0xfe009400)
#define ETH_PLL_CTL4                               ((0x0015  << 2) + 0xfe009400)
#define ETH_PLL_CTL5                               ((0x0016  << 2) + 0xfe009400)
#define ETH_PLL_CTL6                               ((0x0017  << 2) + 0xfe009400)
#define ETH_PLL_CTL7                               ((0x0018  << 2) + 0xfe009400)
#define ETH_PHY_CNTL0                              ((0x0020  << 2) + 0xfe009400)
#define ETH_PHY_CNTL1                              ((0x0021  << 2) + 0xfe009400)
#define ETH_PHY_CNTL2                              ((0x0022  << 2) + 0xfe009400)
#define ETH_PHY_CNTL3                              ((0x0023  << 2) + 0xfe009400)
#define ETH_PHY_STS0                               ((0x0025  << 2) + 0xfe009400)
#define ETH_PHY_STS1                               ((0x0026  << 2) + 0xfe009400)
#define ETH_PHY_STS2                               ((0x0027  << 2) + 0xfe009400)
#define ETH_PHY_DBG_REG                            ((0x0028  << 2) + 0xfe009400)
//========================================================================
//  APB0_UART_D - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe009c00
// -----------------------------------------------
#define UART_D_WFIFO                               ((0x0000  << 2) + 0xfe009c00)
#define UART_D_RFIFO                               ((0x0001  << 2) + 0xfe009c00)
#define UART_D_CONTROL                             ((0x0002  << 2) + 0xfe009c00)
#define UART_D_STATUS                              ((0x0003  << 2) + 0xfe009c00)
#define UART_D_MISC                                ((0x0004  << 2) + 0xfe009c00)
#define UART_D_REG5                                ((0x0005  << 2) + 0xfe009c00)
//========================================================================
//  APB0_UART_E - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe00a000
// -----------------------------------------------
#define UART_E_WFIFO                               ((0x0000  << 2) + 0xfe00a000)
#define UART_E_RFIFO                               ((0x0001  << 2) + 0xfe00a000)
#define UART_E_CONTROL                             ((0x0002  << 2) + 0xfe00a000)
#define UART_E_STATUS                              ((0x0003  << 2) + 0xfe00a000)
#define UART_E_MISC                                ((0x0004  << 2) + 0xfe00a000)
#define UART_E_REG5                                ((0x0005  << 2) + 0xfe00a000)
//========================================================================
//  APB0_PWM_GH - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe00a400
// -----------------------------------------------
#define PWMGH_PWM_G                                ((0x0000  << 2) + 0xfe00a400)
#define PWMGH_PWM_H                                ((0x0001  << 2) + 0xfe00a400)
#define PWMGH_MISC_REG_GH                          ((0x0002  << 2) + 0xfe00a400)
#define PWMGH_DELTA_SIGMA_GH                       ((0x0003  << 2) + 0xfe00a400)
#define PWMGH_TIME_GH                              ((0x0004  << 2) + 0xfe00a400)
#define PWMGH_G2                                   ((0x0005  << 2) + 0xfe00a400)
#define PWMGH_H2                                   ((0x0006  << 2) + 0xfe00a400)
#define PWMGH_BLINK_GH                             ((0x0007  << 2) + 0xfe00a400)
#define PWMGH_LOCK_GH                              ((0x0008  << 2) + 0xfe00a400)
//========================================================================
//  APB0_PWM_IJ - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe00a800
// -----------------------------------------------
#define PWMIJ_PWM_I                                ((0x0000  << 2) + 0xfe00a800)
#define PWMIJ_PWM_J                                ((0x0001  << 2) + 0xfe00a800)
#define PWMIJ_MISC_REG_IJ                          ((0x0002  << 2) + 0xfe00a800)
#define PWMIJ_DELTA_SIGMA_IJ                       ((0x0003  << 2) + 0xfe00a800)
#define PWMIJ_TIME_IJ                              ((0x0004  << 2) + 0xfe00a800)
#define PWMIJ_I2                                   ((0x0005  << 2) + 0xfe00a800)
#define PWMIJ_J2                                   ((0x0006  << 2) + 0xfe00a800)
#define PWMIJ_BLINK_IJ                             ((0x0007  << 2) + 0xfe00a800)
#define PWMIJ_LOCK_IJ                              ((0x0008  << 2) + 0xfe00a800)
//========================================================================
//  APB0_I2C_M - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe00b000
// -----------------------------------------------
#define I2C_M_E_CONTROL_REG                        ((0x0000  << 2) + 0xfe00b000)
#define I2C_M_E_SLAVE_ADDR                         ((0x0001  << 2) + 0xfe00b000)
#define I2C_M_E_TOKEN_LIST0                        ((0x0002  << 2) + 0xfe00b000)
#define I2C_M_E_TOKEN_LIST1                        ((0x0003  << 2) + 0xfe00b000)
#define I2C_M_E_WDATA_REG0                         ((0x0004  << 2) + 0xfe00b000)
#define I2C_M_E_WDATA_REG1                         ((0x0005  << 2) + 0xfe00b000)
#define I2C_M_E_RDATA_REG0                         ((0x0006  << 2) + 0xfe00b000)
#define I2C_M_E_RDATA_REG1                         ((0x0007  << 2) + 0xfe00b000)
#define I2C_M_E_TIMEOUT_TH                         ((0x0008  << 2) + 0xfe00b000)
#define I2C_M_E_CNTL_DELY1                         ((0x0009  << 2) + 0xfe00b000)
#define I2C_M_E_CNTL_DELY2                         ((0x000a  << 2) + 0xfe00b000)
#define I2C_M_E_LOW_DELY                           ((0x000b  << 2) + 0xfe00b000)
#define I2C_M_E_HIGH_DELY                          ((0x000c  << 2) + 0xfe00b000)
#define I2C_M_E_FIFO_CTRL_REG                      ((0x000d  << 2) + 0xfe00b000)
#define I2C_M_E_STATE_REG                          ((0x000e  << 2) + 0xfe00b000)
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe020000
// -----------------------------------------------
#define RSA_BASE                                   ((0x0000  << 2) + 0xfe020000)
//========================================================================
//  RAMA_SEC_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe00b400
// -----------------------------------------------
#define SRAM_RANGE0_STA                            ((0x0000  << 2) + 0xfe00b400)
#define SRAM_RANGE1_STA                            ((0x0001  << 2) + 0xfe00b400)
#define SRAM_RANGE2_STA                            ((0x0002  << 2) + 0xfe00b400)
#define SRAM_RANGE3_STA                            ((0x0003  << 2) + 0xfe00b400)
#define SRAM_RANGE4_STA                            ((0x0004  << 2) + 0xfe00b400)
#define SRAM_RANGE5_STA                            ((0x0005  << 2) + 0xfe00b400)
#define SRAM_RANGE6_STA                            ((0x0006  << 2) + 0xfe00b400)
#define SRAM_RANGE0_EDA                            ((0x0007  << 2) + 0xfe00b400)
#define SRAM_RANGE1_EDA                            ((0x0008  << 2) + 0xfe00b400)
#define SRAM_RANGE2_EDA                            ((0x0009  << 2) + 0xfe00b400)
#define SRAM_RANGE3_EDA                            ((0x000a  << 2) + 0xfe00b400)
#define SRAM_RANGE4_EDA                            ((0x000b  << 2) + 0xfe00b400)
#define SRAM_RANGE5_EDA                            ((0x000c  << 2) + 0xfe00b400)
#define SRAM_RANGE6_EDA                            ((0x000d  << 2) + 0xfe00b400)
#define SRAM_RANGE0_CTRL0                          ((0x000e  << 2) + 0xfe00b400)
#define SRAM_RANGE0_CTRL01                         ((0x000f  << 2) + 0xfe00b400)
#define SRAM_RANGE1_CTRL0                          ((0x0010  << 2) + 0xfe00b400)
#define SRAM_RANGE1_CTRL01                         ((0x0011  << 2) + 0xfe00b400)
#define SRAM_RANGE2_CTRL0                          ((0x0012  << 2) + 0xfe00b400)
#define SRAM_RANGE2_CTRL01                         ((0x0013  << 2) + 0xfe00b400)
#define SRAM_RANGE3_CTRL0                          ((0x0014  << 2) + 0xfe00b400)
#define SRAM_RANGE3_CTRL01                         ((0x0015  << 2) + 0xfe00b400)
#define SRAM_RANGE4_CTRL0                          ((0x0016  << 2) + 0xfe00b400)
#define SRAM_RANGE4_CTRL01                         ((0x0017  << 2) + 0xfe00b400)
#define SRAM_RANGE5_CTRL0                          ((0x0018  << 2) + 0xfe00b400)
#define SRAM_RANGE5_CTRL01                         ((0x0019  << 2) + 0xfe00b400)
#define SRAM_RANGE6_CTRL0                          ((0x001a  << 2) + 0xfe00b400)
#define SRAM_RANGE6_CTRL01                         ((0x001b  << 2) + 0xfe00b400)
#define SRAM_RANGE7_CTRL0                          ((0x001c  << 2) + 0xfe00b400)
#define SRAM_RANGE7_CTRL01                         ((0x001d  << 2) + 0xfe00b400)
#define SRAM_RANGE0_CTRL1                          ((0x001e  << 2) + 0xfe00b400)
#define SRAM_RANGE0_CTRL11                         ((0x001f  << 2) + 0xfe00b400)
#define SRAM_RANGE1_CTRL1                          ((0x0020  << 2) + 0xfe00b400)
#define SRAM_RANGE1_CTRL11                         ((0x0021  << 2) + 0xfe00b400)
#define SRAM_RANGE2_CTRL1                          ((0x0022  << 2) + 0xfe00b400)
#define SRAM_RANGE2_CTRL11                         ((0x0023  << 2) + 0xfe00b400)
#define SRAM_RANGE3_CTRL1                          ((0x0024  << 2) + 0xfe00b400)
#define SRAM_RANGE3_CTRL11                         ((0x0025  << 2) + 0xfe00b400)
#define SRAM_RANGE4_CTRL1                          ((0x0026  << 2) + 0xfe00b400)
#define SRAM_RANGE4_CTRL11                         ((0x0027  << 2) + 0xfe00b400)
#define SRAM_RANGE5_CTRL1                          ((0x0028  << 2) + 0xfe00b400)
#define SRAM_RANGE5_CTRL11                         ((0x0029  << 2) + 0xfe00b400)
#define SRAM_RANGE6_CTRL1                          ((0x002a  << 2) + 0xfe00b400)
#define SRAM_RANGE6_CTRL11                         ((0x002b  << 2) + 0xfe00b400)
#define SRAM_RANGE7_CTRL1                          ((0x002c  << 2) + 0xfe00b400)
#define SRAM_RANGE7_CTRL11                         ((0x002d  << 2) + 0xfe00b400)
#define SRAM_SEC_CTRL0                             ((0x002e  << 2) + 0xfe00b400)
#define SRAM_REG_LOCK0                             ((0x002f  << 2) + 0xfe00b400)
#define SRAM_REG_LOCK1                             ((0x0030  << 2) + 0xfe00b400)
//========================================================================
//  RAMB_SEC_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe00b800
// -----------------------------------------------
#define SRAM_RANGE0_STA_B                          ((0x0000  << 2) + 0xfe00b800)
#define SRAM_RANGE1_STA_B                          ((0x0001  << 2) + 0xfe00b800)
#define SRAM_RANGE2_STA_B                          ((0x0002  << 2) + 0xfe00b800)
#define SRAM_RANGE3_STA_B                          ((0x0003  << 2) + 0xfe00b800)
#define SRAM_RANGE4_STA_B                          ((0x0004  << 2) + 0xfe00b800)
#define SRAM_RANGE5_STA_B                          ((0x0005  << 2) + 0xfe00b800)
#define SRAM_RANGE6_STA_B                          ((0x0006  << 2) + 0xfe00b800)
#define SRAM_RANGE0_EDA_B                          ((0x0007  << 2) + 0xfe00b800)
#define SRAM_RANGE1_EDA_B                          ((0x0008  << 2) + 0xfe00b800)
#define SRAM_RANGE2_EDA_B                          ((0x0009  << 2) + 0xfe00b800)
#define SRAM_RANGE3_EDA_B                          ((0x000a  << 2) + 0xfe00b800)
#define SRAM_RANGE4_EDA_B                          ((0x000b  << 2) + 0xfe00b800)
#define SRAM_RANGE5_EDA_B                          ((0x000c  << 2) + 0xfe00b800)
#define SRAM_RANGE6_EDA_B                          ((0x000d  << 2) + 0xfe00b800)
#define SRAM_RANGE0_CTRL0_B                        ((0x000e  << 2) + 0xfe00b800)
#define SRAM_RANGE0_CTRL01_B                       ((0x000f  << 2) + 0xfe00b800)
#define SRAM_RANGE1_CTRL0_B                        ((0x0010  << 2) + 0xfe00b800)
#define SRAM_RANGE1_CTRL01_B                       ((0x0011  << 2) + 0xfe00b800)
#define SRAM_RANGE2_CTRL0_B                        ((0x0012  << 2) + 0xfe00b800)
#define SRAM_RANGE2_CTRL01_B                       ((0x0013  << 2) + 0xfe00b800)
#define SRAM_RANGE3_CTRL0_B                        ((0x0014  << 2) + 0xfe00b800)
#define SRAM_RANGE3_CTRL01_B                       ((0x0015  << 2) + 0xfe00b800)
#define SRAM_RANGE4_CTRL0_B                        ((0x0016  << 2) + 0xfe00b800)
#define SRAM_RANGE4_CTRL01_B                       ((0x0017  << 2) + 0xfe00b800)
#define SRAM_RANGE5_CTRL0_B                        ((0x0018  << 2) + 0xfe00b800)
#define SRAM_RANGE5_CTRL01_B                       ((0x0019  << 2) + 0xfe00b800)
#define SRAM_RANGE6_CTRL0_B                        ((0x001a  << 2) + 0xfe00b800)
#define SRAM_RANGE6_CTRL01_B                       ((0x001b  << 2) + 0xfe00b800)
#define SRAM_RANGE7_CTRL0_B                        ((0x001c  << 2) + 0xfe00b800)
#define SRAM_RANGE7_CTRL01_B                       ((0x001d  << 2) + 0xfe00b800)
#define SRAM_RANGE0_CTRL1_B                        ((0x001e  << 2) + 0xfe00b800)
#define SRAM_RANGE0_CTRL11_B                       ((0x001f  << 2) + 0xfe00b800)
#define SRAM_RANGE1_CTRL1_B                        ((0x0020  << 2) + 0xfe00b800)
#define SRAM_RANGE1_CTRL11_B                       ((0x0021  << 2) + 0xfe00b800)
#define SRAM_RANGE2_CTRL1_B                        ((0x0022  << 2) + 0xfe00b800)
#define SRAM_RANGE2_CTRL11_B                       ((0x0023  << 2) + 0xfe00b800)
#define SRAM_RANGE3_CTRL1_B                        ((0x0024  << 2) + 0xfe00b800)
#define SRAM_RANGE3_CTRL11_B                       ((0x0025  << 2) + 0xfe00b800)
#define SRAM_RANGE4_CTRL1_B                        ((0x0026  << 2) + 0xfe00b800)
#define SRAM_RANGE4_CTRL11_B                       ((0x0027  << 2) + 0xfe00b800)
#define SRAM_RANGE5_CTRL1_B                        ((0x0028  << 2) + 0xfe00b800)
#define SRAM_RANGE5_CTRL11_B                       ((0x0029  << 2) + 0xfe00b800)
#define SRAM_RANGE6_CTRL1_B                        ((0x002a  << 2) + 0xfe00b800)
#define SRAM_RANGE6_CTRL11_B                       ((0x002b  << 2) + 0xfe00b800)
#define SRAM_RANGE7_CTRL1_B                        ((0x002c  << 2) + 0xfe00b800)
#define SRAM_RANGE7_CTRL11_B                       ((0x002d  << 2) + 0xfe00b800)
#define SRAM_SEC_CTRL0_B                           ((0x002e  << 2) + 0xfe00b800)
#define SRAM_REG_LOCK0_B                           ((0x002f  << 2) + 0xfe00b800)
#define SRAM_REG_LOCK1_B                           ((0x0030  << 2) + 0xfe00b800)
//========================================================================
//  RAMC_SEC_CTRL - Registers
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe00bc00
// -----------------------------------------------
#define SRAM_RANGE0_STA_C                          ((0x0000  << 2) + 0xfe00bc00)
#define SRAM_RANGE1_STA_C                          ((0x0001  << 2) + 0xfe00bc00)
#define SRAM_RANGE2_STA_C                          ((0x0002  << 2) + 0xfe00bc00)
#define SRAM_RANGE3_STA_C                          ((0x0003  << 2) + 0xfe00bc00)
#define SRAM_RANGE4_STA_C                          ((0x0004  << 2) + 0xfe00bc00)
#define SRAM_RANGE5_STA_C                          ((0x0005  << 2) + 0xfe00bc00)
#define SRAM_RANGE6_STA_C                          ((0x0006  << 2) + 0xfe00bc00)
#define SRAM_RANGE0_EDA_C                          ((0x0007  << 2) + 0xfe00bc00)
#define SRAM_RANGE1_EDA_C                          ((0x0008  << 2) + 0xfe00bc00)
#define SRAM_RANGE2_EDA_C                          ((0x0009  << 2) + 0xfe00bc00)
#define SRAM_RANGE3_EDA_C                          ((0x000a  << 2) + 0xfe00bc00)
#define SRAM_RANGE4_EDA_C                          ((0x000b  << 2) + 0xfe00bc00)
#define SRAM_RANGE5_EDA_C                          ((0x000c  << 2) + 0xfe00bc00)
#define SRAM_RANGE6_EDA_C                          ((0x000d  << 2) + 0xfe00bc00)
#define SRAM_RANGE0_CTRL0_C                        ((0x000e  << 2) + 0xfe00bc00)
#define SRAM_RANGE0_CTRL01_C                       ((0x000f  << 2) + 0xfe00bc00)
#define SRAM_RANGE1_CTRL0_C                        ((0x0010  << 2) + 0xfe00bc00)
#define SRAM_RANGE1_CTRL01_C                       ((0x0011  << 2) + 0xfe00bc00)
#define SRAM_RANGE2_CTRL0_C                        ((0x0012  << 2) + 0xfe00bc00)
#define SRAM_RANGE2_CTRL01_C                       ((0x0013  << 2) + 0xfe00bc00)
#define SRAM_RANGE3_CTRL0_C                        ((0x0014  << 2) + 0xfe00bc00)
#define SRAM_RANGE3_CTRL01_C                       ((0x0015  << 2) + 0xfe00bc00)
#define SRAM_RANGE4_CTRL0_C                        ((0x0016  << 2) + 0xfe00bc00)
#define SRAM_RANGE4_CTRL01_C                       ((0x0017  << 2) + 0xfe00bc00)
#define SRAM_RANGE5_CTRL0_C                        ((0x0018  << 2) + 0xfe00bc00)
#define SRAM_RANGE5_CTRL01_C                       ((0x0019  << 2) + 0xfe00bc00)
#define SRAM_RANGE6_CTRL0_C                        ((0x001a  << 2) + 0xfe00bc00)
#define SRAM_RANGE6_CTRL01_C                       ((0x001b  << 2) + 0xfe00bc00)
#define SRAM_RANGE7_CTRL0_C                        ((0x001c  << 2) + 0xfe00bc00)
#define SRAM_RANGE7_CTRL01_C                       ((0x001d  << 2) + 0xfe00bc00)
#define SRAM_RANGE0_CTRL1_C                        ((0x001e  << 2) + 0xfe00bc00)
#define SRAM_RANGE0_CTRL11_C                       ((0x001f  << 2) + 0xfe00bc00)
#define SRAM_RANGE1_CTRL1_C                        ((0x0020  << 2) + 0xfe00bc00)
#define SRAM_RANGE1_CTRL11_C                       ((0x0021  << 2) + 0xfe00bc00)
#define SRAM_RANGE2_CTRL1_C                        ((0x0022  << 2) + 0xfe00bc00)
#define SRAM_RANGE2_CTRL11_C                       ((0x0023  << 2) + 0xfe00bc00)
#define SRAM_RANGE3_CTRL1_C                        ((0x0024  << 2) + 0xfe00bc00)
#define SRAM_RANGE3_CTRL11_C                       ((0x0025  << 2) + 0xfe00bc00)
#define SRAM_RANGE4_CTRL1_C                        ((0x0026  << 2) + 0xfe00bc00)
#define SRAM_RANGE4_CTRL11_C                       ((0x0027  << 2) + 0xfe00bc00)
#define SRAM_RANGE5_CTRL1_C                        ((0x0028  << 2) + 0xfe00bc00)
#define SRAM_RANGE5_CTRL11_C                       ((0x0029  << 2) + 0xfe00bc00)
#define SRAM_RANGE6_CTRL1_C                        ((0x002a  << 2) + 0xfe00bc00)
#define SRAM_RANGE6_CTRL11_C                       ((0x002b  << 2) + 0xfe00bc00)
#define SRAM_RANGE7_CTRL1_C                        ((0x002c  << 2) + 0xfe00bc00)
#define SRAM_RANGE7_CTRL11_C                       ((0x002d  << 2) + 0xfe00bc00)
#define SRAM_SEC_CTRL0_C                           ((0x002e  << 2) + 0xfe00bc00)
#define SRAM_REG_LOCK0_C                           ((0x002f  << 2) + 0xfe00bc00)
#define SRAM_REG_LOCK1_C                           ((0x0030  << 2) + 0xfe00bc00)
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
//
// Reading file:  REG_LIST_DOS_RTL.h
//
// synopsys translate_off
// synopsys translate_on
// -----------------------------------------------
// `define APB_DOS_VDEC_ADDR_OFFSET        6'h00 // VDEC 1
// -----------------------------------------------
// -----------------------------------------------
// `define APB_DOS_HCODEC_ADDR_OFFSET      6'h10 // HCODEC 1
// -----------------------------------------------
// -----------------------------------------------
// `define APB_DOS_VDEC2_ADDR_OFFSET       6'h20 // VDEC 2   // Share with HEVC2
// -----------------------------------------------
// -----------------------------------------------
// `define APB_DOS_HEVC2_ADDR_OFFSET       6'h20 // HEVC 2   // Share with VDEC2
// -----------------------------------------------
// -----------------------------------------------
// `define APB_DOS_HEVC_ADDR_OFFSET        6'h30 // HEVC
// -----------------------------------------------
//========================================================================
//  DOS - Registers
//========================================================================
// VDEC
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe060000
// -----------------------------------------------
// HCODEC
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe064000
// -----------------------------------------------
// -----------------------------------------------
// `define DOS_HCODEC_ASSIST_CBUS_BASE       8'h00
// -----------------------------------------------
#define HCODEC_ASSIST_MMC_CTRL0                    ((0x0001  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MMC_CTRL1                    ((0x0002  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MMC_CTRL2                    ((0x0003  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MMC_CTRL3                    ((0x0004  << 2) + 0xfe064000)
// bit[11]  canvas_blk32_wr
// bit[10]  canvas_blk32
// bit[9]   Reserved
// bit[8]   canvas_index_wr
// bit[7:0] canvas_index
#define HCODEC_ASSIST_CANVAS_BLK32                 ((0x0005  << 2) + 0xfe064000)
//cfg_soft_rst = mfdin_reg0_crst[0]; // Soft Reset
//cfg_cg_cfe   = mfdin_reg0_crst[1]; // Clock Gating Forcing Enable
//cfg_dbuf_rst = mfdin_reg0_crst[2]; // Reset DBUF information
#define HCODEC_MFDIN_REG0_CRST                     ((0x0008  << 2) + 0xfe064000)
//cfg_iformat       = mfdin_reg1_ctrl[3:0]; // Data In Format: 0 :CANVAS 422 YCbCr<-one canvas,
                                                     //        1 :CANVAS 444 YCbCr(or RGB)<-one canvas,
                                                     //        2 :CANVAS 420 Y<-one CrCb<-one (NV21)
                                                     //        3 :CANVAS 420 Y<-one CbCr<-one (NV12)
                                                     //        4 :CANVAS 420 Y<-one Cb<-one Cr<-one
                                                     //        5 :CANVAS 444 Y<-one Cb<-one Cr<-one
                                                     //        6 : Reserved for other canvas mode
                                                     //        7 : should combine with cfg_ifmt_extra, for 10-12bits
                                                     //        8 :LINEAR RGB888 (24bit)
                                                     //        9 :LINEAR RGB565 (16bit)
                                                     //        10:LINEAR YUV422
                                                     //        11:LINEAR YUV420 (Note:Legacy mode not supported)
                                                     //        12~15: Reserved for other linear mode
//cfg_oformat       = mfdin_reg1_ctrl[5:4];  // Data Out Format: 0:420 1:422 2:444 3:reserved
//cfg_dsample_enable= mfdin_reg1_ctrl[6];    // Downsample Enable
//cfg_dsample_trunc = mfdin_reg1_ctrl[7];    // Downsample Addition Option: 0:Round 1:Truncated
//cfg_block_ysize   = mfdin_reg1_ctrl[8];    // 0:block-y-size=16, 1:block-y-size=8(for JPEG's efficiency)
//cfg_interp_enable = mfdin_reg1_ctrl[9];    // Chroma Interpolation Enable
//cfg_interp_mode   = mfdin_reg1_ctrl[11:10];// 0:average round 1:average truncated 2:repeat left 3:repeat right
//cfg_r2y_enable    = mfdin_reg1_ctrl[12];   // RGb->YUV Enable
//cfg_r2y_mode      = mfdin_reg1_ctrl[14:13];// RGb->YUV Mode
//cfg_rgb565_mode   = mfdin_reg1_ctrl[15];   // RGB565 mode: 0:fill zero in tail 1:fill zero in head
//cfg_ifmt_extra    = mfdin_reg1_ctrl[17:16];// when cfg_iformat==7,
                                             //        0: CANVAS 12bit 422, YCbCr<-one canvas{...Y1[11:0],C1[11:0],Y0[11:0],C0[11:0]}
                                             //        1: CANVAS 10bit 444, YCbCr<-one canvas{...2'b0,Y0[9:0],U0[9:0],V0[9:0]}
                                             //        2: CANVAS 10bit 422, YCbCr<-one canvas{...Y1[9:0],C1[9:0],Y0[9:0],C0[9:0]}
//cfg_reserved1     = mfdin_reg1_ctrl[18];   // Reserved
//cfg_nr_enable     = mfdin_reg1_ctrl[19];   // Noise Reduction Enable
//cfg_outofpic      = mfdin_reg1_ctrl[28:20];// Out Of Picture Control: [8]mode:0:extend 1:fixed, [7:0]fixed value
//cfg_rd_blktype    = mfdin_reg1_ctrl[30:29];// Block Type: 0:H264_I_PIC_ALL_4x4, 1:H264_P_PIC_Y_16x16_C_8x8, 2:JPEG_ALL_8x8, 3:Reserved
//cfg_rd_nonycintl  = mfdin_reg1_ctrl[31];   // 0:YC interleaved 1:YC non-interleaved(for JPEG)
#define HCODEC_MFDIN_REG1_CTRL                     ((0x0009  << 2) + 0xfe064000)
//mfdin_reg2: DEBUG Only Status
#define HCODEC_MFDIN_REG2_STAT                     ((0x000a  << 2) + 0xfe064000)
//cfg_canvas_index0  = mfdin_reg3_canv[7:0];  //canvas index 0
//cfg_canvas_index1  = mfdin_reg3_canv[15:8]; //canvas index 1
//cfg_canvas_index2  = mfdin_reg3_canv[23:16];//canvas index 2
//cfg_canv_idx0_bppx = mfdin_reg3_canv[25:24];//canvas bytes per pixel in x direction for index0, 0:half 1:1 2:2 3:3
//cfg_canv_idx1_bppx = mfdin_reg3_canv[27:26];//canvas bytes per pixel in x direction for index1, 0:half 1:1 2:2 3:3
//cfg_canv_idx0_bppy = mfdin_reg3_canv[29:28];//canvas bytes per pixel in y direction for index0, 0:half 1:1 2:2 3:3
//cfg_canv_idx1_bppy = mfdin_reg3_canv[31:30];//canvas bytes per pixel in y direction for index1, 0:half 1:1 2:2 3:3
#define HCODEC_MFDIN_REG3_CANV                     ((0x000b  << 2) + 0xfe064000)
//cfg_bytes_per_line  = mfdin_reg4_lnr0[15:0];  //linear Bytes per line
//cfg_linear_bytes4p  = mfdin_reg4_lnr0[17:16]; //linear (Bytes per pixel) - 1
//cfg_linear_dbl2line = mfdin_reg4_lnr0[18];    //linear double size for odd line
#define HCODEC_MFDIN_REG4_LNR0                     ((0x000c  << 2) + 0xfe064000)
//cfg_base_address    = mfdin_reg5_lnr1[31:0];  //linear base address
#define HCODEC_MFDIN_REG5_LNR1                     ((0x000d  << 2) + 0xfe064000)
//assign cfg_r2y_trunc     = mfdin_reg6_dcfg[9];   //RGb->YUV Addition Option: 0:Round 1:Truncated
//assign cfg_dma_ugt       = mfdin_reg6_dcfg[8];   //DMA Urgent
//assign cfg_dma_thread_id = mfdin_reg6_dcfg[7:6]; //DMA Thread ID
//assign cfg_dma_burst_num = mfdin_reg6_dcfg[5:0]; //DMA Burst Number
#define HCODEC_MFDIN_REG6_DCFG                     ((0x000e  << 2) + 0xfe064000)
//cfg_soft_cmd = mfdin_reg7_scmd; // Soft Command [28]selfcleared start,[27:14]dmb_x,[13:0]dmb_y
#define HCODEC_MFDIN_REG7_SCMD                     ((0x000f  << 2) + 0xfe064000)
//cfg_pic_xsize = mfdin_reg8_dmbl[23:12]; //pixel (x,y) at the begining of last dmb in the picture, picture x size
//cfg_pic_ysize = mfdin_reg8_dmbl[11:0];  //picture y size
#define HCODEC_MFDIN_REG8_DMBL                     ((0x0010  << 2) + 0xfe064000)
//cfg_endian = mfdin_reg9_endn; //Endian Control
#define HCODEC_MFDIN_REG9_ENDN                     ((0x0011  << 2) + 0xfe064000)
//cfg_canv_biasx = mfdin_rega_cav1[23:12]; //canvas bias address x
//cfg_canv_biasy = mfdin_rega_cav1[11:0];  //canvas bias address y
#define HCODEC_MFDIN_REGA_CAV1                     ((0x0012  << 2) + 0xfe064000)
//cfg_amp_en = mfdin_regb_ampc[24];      //Amplitude Enable
//cfg_amp_cy = mfdin_regb_ampc[23:16];   //Amplitude Coeff Y
//cfg_amp_cu = mfdin_regb_ampc[15:8];    //Amplitude Coeff U
//cfg_amp_cv = mfdin_regb_ampc[7:0];     //Amplitude Coeff V
#define HCODEC_MFDIN_REGB_AMPC                     ((0x0013  << 2) + 0xfe064000)
//cfg_mb_end = mfdin_regc_mblp; // Soft Command [28]mb end enable,[27:14]dmb_x,[13:0]dmb_y
#define HCODEC_MFDIN_REGC_MBLP                     ((0x0014  << 2) + 0xfe064000)
//       cfg_y_snr_en               = mfdin_reg0d[0];
//       cfg_y_snr_err_norm         = mfdin_reg0d[1];
// [3:0] cfg_y_snr_gau_bld_core     = mfdin_reg0d[5:2];
// [7:0] cfg_y_snr_gau_bld_ofst     = mfdin_reg0d[13:6];
// [5:0] cfg_y_snr_gau_bld_rate     = mfdin_reg0d[19:14];
// [5:0] cfg_y_snr_gau_alp0_min     = mfdin_reg0d[25:20];
// [5:0] cfg_y_snr_gau_alp0_max     = mfdin_reg0d[31:26];
#define HCODEC_MFDIN_REG0D                         ((0x0015  << 2) + 0xfe064000)
//       cfg_y_tnr_en               = mfdin_reg0e[0];
//       cfg_y_tnr_mc_en            = mfdin_reg0e[1];
//       cfg_y_tnr_txt_mode         = mfdin_reg0e[2];
// [3:0] cfg_y_tnr_mot_sad_margin   = mfdin_reg0e[6:3];
// [5:0] cfg_y_tnr_alpha_min        = mfdin_reg0e[12:7];
// [5:0] cfg_y_tnr_alpha_max        = mfdin_reg0e[18:13];
// [5:0] cfg_y_tnr_deghost_os       = mfdin_reg0e[24:19];
#define HCODEC_MFDIN_REG0E                         ((0x0016  << 2) + 0xfe064000)
// [3:0] cfg_y_tnr_mot_cortxt_rate  = mfdin_reg0f[3:0];
// [7:0] cfg_y_tnr_mot_distxt_ofst  = mfdin_reg0f[15:8];
// [3:0] cfg_y_tnr_mot_distxt_rate  = mfdin_reg0f[7:4];
// [7:0] cfg_y_tnr_mot_dismot_ofst  = mfdin_reg0f[23:16];
// [7:0] cfg_y_tnr_mot_frcsad_lock  = mfdin_reg0f[31:24];
#define HCODEC_MFDIN_REG0F                         ((0x0017  << 2) + 0xfe064000)
// [7:0] cfg_y_tnr_mot2alp_frc_gain = mfdin_reg10[7:0];
// [7:0] cfg_y_tnr_mot2alp_nrm_gain = mfdin_reg10[15:8];
// [7:0] cfg_y_tnr_mot2alp_dis_gain = mfdin_reg10[23:16];
// [5:0] cfg_y_tnr_mot2alp_dis_ofst = mfdin_reg10[29:24];
#define HCODEC_MFDIN_REG10                         ((0x0018  << 2) + 0xfe064000)
// [7:0] cfg_y_bld_beta2alp_rate    = mfdin_reg11[7:0];
// [5:0] cfg_y_bld_beta_min         = mfdin_reg11[13:8];
// [5:0] cfg_y_bld_beta_max         = mfdin_reg11[19:14];
#define HCODEC_MFDIN_REG11                         ((0x0019  << 2) + 0xfe064000)
// REG12~16 for Chroma, same as Luma
#define HCODEC_MFDIN_REG12                         ((0x001a  << 2) + 0xfe064000)
#define HCODEC_MFDIN_REG13                         ((0x001b  << 2) + 0xfe064000)
#define HCODEC_MFDIN_REG14                         ((0x001c  << 2) + 0xfe064000)
#define HCODEC_MFDIN_REG15                         ((0x001d  << 2) + 0xfe064000)
#define HCODEC_MFDIN_REG16                         ((0x001e  << 2) + 0xfe064000)
#define HCODEC_MFDIN_REG17                         ((0x001f  << 2) + 0xfe064000)
#define HCODEC_MFDIN_REG18                         ((0x0020  << 2) + 0xfe064000)
#define HCODEC_MFDIN_REG19                         ((0x0021  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INT0                    ((0x0025  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INT1                    ((0x0026  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INT2                    ((0x0027  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INT3                    ((0x0028  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INT4                    ((0x0029  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INT5                    ((0x002a  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INT6                    ((0x002b  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INT7                    ((0x002c  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INT8                    ((0x002d  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INT9                    ((0x002e  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INTA                    ((0x002f  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INTB                    ((0x0030  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INTC                    ((0x0031  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INTD                    ((0x0032  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INTE                    ((0x0033  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR1_INTF                    ((0x0034  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INT0                    ((0x0035  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INT1                    ((0x0036  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INT2                    ((0x0037  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INT3                    ((0x0038  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INT4                    ((0x0039  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INT5                    ((0x003a  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INT6                    ((0x003b  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INT7                    ((0x003c  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INT8                    ((0x003d  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INT9                    ((0x003e  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INTA                    ((0x003f  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INTB                    ((0x0040  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INTC                    ((0x0041  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INTD                    ((0x0042  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INTE                    ((0x0043  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AMR2_INTF                    ((0x0044  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MBX_SSEL                     ((0x0045  << 2) + 0xfe064000)
#define HCODEC_ASSIST_TIMER0_LO                    ((0x0060  << 2) + 0xfe064000)
#define HCODEC_ASSIST_TIMER0_HI                    ((0x0061  << 2) + 0xfe064000)
#define HCODEC_ASSIST_TIMER1_LO                    ((0x0062  << 2) + 0xfe064000)
#define HCODEC_ASSIST_TIMER1_HI                    ((0x0063  << 2) + 0xfe064000)
#define HCODEC_ASSIST_DMA_INT                      ((0x0064  << 2) + 0xfe064000)
#define HCODEC_ASSIST_DMA_INT_MSK                  ((0x0065  << 2) + 0xfe064000)
#define HCODEC_ASSIST_DMA_INT2                     ((0x0066  << 2) + 0xfe064000)
#define HCODEC_ASSIST_DMA_INT_MSK2                 ((0x0067  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MBOX0_IRQ_REG                ((0x0070  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MBOX0_CLR_REG                ((0x0071  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MBOX0_MASK                   ((0x0072  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MBOX0_FIQ_SEL                ((0x0073  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MBOX1_IRQ_REG                ((0x0074  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MBOX1_CLR_REG                ((0x0075  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MBOX1_MASK                   ((0x0076  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MBOX1_FIQ_SEL                ((0x0077  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MBOX2_IRQ_REG                ((0x0078  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MBOX2_CLR_REG                ((0x0079  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MBOX2_MASK                   ((0x007a  << 2) + 0xfe064000)
#define HCODEC_ASSIST_MBOX2_FIQ_SEL                ((0x007b  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AXI_CTRL                     ((0x007c  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AXI_STATUS                   ((0x007d  << 2) + 0xfe064000)
#define HCODEC_ASSIST_AXI_SETTING_VLC              ((0x007e  << 2) + 0xfe064000)
//------------------------------------------------------------------------------
// HCODEC IQIDCT module level register offset
//------------------------------------------------------------------------------
// `define DOS_HCODEC_VCPU_CBUS_BASE     8'h03
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe064c00
// -----------------------------------------------
//
// Reading file:  hcodec_vcpu_regs.h
//
//========================================================================
//  VCPU module level register offset
//========================================================================
// -----------------------------------------------
// `define DOS_HCODEC_VCPU_CBUS_BASE     8'h03
// -----------------------------------------------
#define HCODEC_MSP                                 ((0x0000  << 2) + 0xfe064c00)
#define HCODEC_MPSR                                ((0x0001  << 2) + 0xfe064c00)
#define HCODEC_MINT_VEC_BASE                       ((0x0002  << 2) + 0xfe064c00)
#define HCODEC_MCPU_INTR_GRP                       ((0x0003  << 2) + 0xfe064c00)
#define HCODEC_MCPU_INTR_MSK                       ((0x0004  << 2) + 0xfe064c00)
#define HCODEC_MCPU_INTR_REQ                       ((0x0005  << 2) + 0xfe064c00)
//`define HCODEC_MPC_P               8'h06
//`define HCODEC_MPC_D               8'h07
//`define HCODEC_MPC_E			    8'h08
//`define HCODEC_MPC_W			    8'h09
//`define HCODEC_MINDEX0_REG			8'h0a
//`define HCODEC_MINDEX1_REG			8'h0b
//`define HCODEC_MINDEX2_REG			8'h0c
//`define HCODEC_MINDEX3_REG			8'h0d
//`define HCODEC_MINDEX4_REG			8'h0e
//`define HCODEC_MINDEX5_REG			8'h0f
//`define HCODEC_MINDEX6_REG			8'h10
//`define HCODEC_MINDEX7_REG			8'h11
//`define HCODEC_MMIN_REG			8'h12
//`define HCODEC_MMAX_REG			8'h13
//`define HCODEC_MBREAK0_REG			8'h14
//`define HCODEC_MBREAK1_REG			8'h15
//`define HCODEC_MBREAK2_REG			8'h16
//`define HCODEC_MBREAK3_REG			8'h17
//`define HCODEC_MBREAK_TYPE			8'h18
//`define HCODEC_MBREAK_CTRL			8'h19
//`define HCODEC_MBREAK_STAUTS		8'h1a
//`define HCODEC_MDB_ADDR_REG		8'h1b
//`define HCODEC_MDB_DATA_REG		8'h1c
//`define HCODEC_MDB_CTRL			8'h1d
//`define HCODEC_MSFTINT0            8'h1e
//`define HCODEC_MSFTINT1            8'h1f
//
#define HCODEC_CSP                                 ((0x0020  << 2) + 0xfe064c00)
#define HCODEC_CPSR                                ((0x0021  << 2) + 0xfe064c00)
//`define HCODEC_CINT_VEC_BASE       8'h22
//`define HCODEC_CCPU_INTR_GRP       8'h23
//`define HCODEC_CCPU_INTR_MSK       8'h24
//`define HCODEC_CCPU_INTR_REQ       8'h25
//`define HCODEC_CPC_P               8'h26
//`define HCODEC_CPC_D               8'h27
//`define HCODEC_CPC_E			    8'h28
//`define HCODEC_CPC_W			    8'h29
//`define HCODEC_CINDEX0_REG			8'h2a
//`define HCODEC_CINDEX1_REG			8'h2b
//`define HCODEC_CINDEX2_REG			8'h2c
//`define HCODEC_CINDEX3_REG			8'h2d
//`define HCODEC_CINDEX4_REG			8'h2e
//`define HCODEC_CINDEX5_REG			8'h2f
//`define HCODEC_CINDEX6_REG			8'h30
//`define HCODEC_CINDEX7_REG			8'h31
//`define HCODEC_CMIN_REG			8'h32
//`define HCODEC_CMAX_REG			8'h33
//`define HCODEC_CBREAK0_REG			8'h34
//`define HCODEC_CBREAK1_REG			8'h35
//`define HCODEC_CBREAK2_REG			8'h36
//`define HCODEC_CBREAK3_REG			8'h37
//`define HCODEC_CBREAK_TYPE			8'h38
//`define HCODEC_CBREAK_CTRL			8'h39
//`define HCODEC_CBREAK_STAUTS		8'h3a
//`define HCODEC_CDB_ADDR_REG		8'h3b
//`define HCODEC_CDB_DATA_REG		8'h3c
//`define HCODEC_CDB_CTRL			8'h3d
//`define HCODEC_CSFTINT0            8'h3e
//`define HCODEC_CSFTINT1            8'h3f
//
#define HCODEC_IMEM_DMA_CTRL                       ((0x0040  << 2) + 0xfe064c00)
#define HCODEC_IMEM_DMA_ADR                        ((0x0041  << 2) + 0xfe064c00)
#define HCODEC_IMEM_DMA_COUNT                      ((0x0042  << 2) + 0xfe064c00)
//// bit[29:24] A_brst_num_imem
//// bit[21:16] A_id_imem
//// bit[11:0] wrrsp_count_imem (reserved)
#define HCODEC_WRRSP_IMEM                          ((0x0043  << 2) + 0xfe064c00)
//
//`define HCODEC_LMEM_DMA_CTRL       8'h50
//`define HCODEC_LMEM_DMA_ADR        8'h51
//`define HCODEC_LMEM_DMA_COUNT      8'h52
//// bit[29:24] A_brst_num_lmem
//// bit[21:16] A_id_lmem
//// bit[11:0] wrrsp_count_lmem
//`define HCODEC_WRRSP_LMEM          8'h53
//
//`define HCODEC_MAC_CTRL1			8'h60
//`define HCODEC_ACC0REG1			8'h61
//`define HCODEC_ACC1REG1			8'h62
//
//`define HCODEC_MAC_CTRL2			8'h70
//`define HCODEC_ACC0REG2			8'h71
//`define HCODEC_ACC1REG2			8'h72
//`define HCODEC_CPU_TRACE			8'h80
//
//
// Closing file:  hcodec_vcpu_regs.h
//
//------------------------------------------------------------------------------
// -----------------------------------------------
// `define DOS_HCODEC_HDEC_CBUS_BASE     8'h09
// -----------------------------------------------
//------------------------------------------------------------------------------
// HENC module level register offset
//------------------------------------------------------------------------------
// `define DOS_HCODEC_HENC_CBUS_BASE   8'h0a
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe066800
// -----------------------------------------------
//
// Reading file:  henc_regs.h
//
//========================================================================
//  MDEC module level register offset
//========================================================================
// -----------------------------------------------
// `define DOS_HCODEC_HENC_CBUS_BASE   8'h0a
// -----------------------------------------------
#define HENC_SCRATCH_0                             ((0x00c0  << 2) + 0xfe066800)
#define HENC_SCRATCH_1                             ((0x00c1  << 2) + 0xfe066800)
#define HENC_SCRATCH_2                             ((0x00c2  << 2) + 0xfe066800)
#define HENC_SCRATCH_3                             ((0x00c3  << 2) + 0xfe066800)
#define HENC_SCRATCH_4                             ((0x00c4  << 2) + 0xfe066800)
#define HENC_SCRATCH_5                             ((0x00c5  << 2) + 0xfe066800)
#define HENC_SCRATCH_6                             ((0x00c6  << 2) + 0xfe066800)
#define HENC_SCRATCH_7                             ((0x00c7  << 2) + 0xfe066800)
#define HENC_SCRATCH_8                             ((0x00c8  << 2) + 0xfe066800)
#define HENC_SCRATCH_9                             ((0x00c9  << 2) + 0xfe066800)
#define HENC_SCRATCH_A                             ((0x00ca  << 2) + 0xfe066800)
#define HENC_SCRATCH_B                             ((0x00cb  << 2) + 0xfe066800)
#define HENC_SCRATCH_C                             ((0x00cc  << 2) + 0xfe066800)
#define HENC_SCRATCH_D                             ((0x00cd  << 2) + 0xfe066800)
#define HENC_SCRATCH_E                             ((0x00ce  << 2) + 0xfe066800)
#define HENC_SCRATCH_F                             ((0x00cf  << 2) + 0xfe066800)
#define HENC_SCRATCH_G                             ((0x00d0  << 2) + 0xfe066800)
#define HENC_SCRATCH_H                             ((0x00d1  << 2) + 0xfe066800)
#define HENC_SCRATCH_I                             ((0x00d2  << 2) + 0xfe066800)
#define HENC_SCRATCH_J                             ((0x00d3  << 2) + 0xfe066800)
#define HENC_SCRATCH_K                             ((0x00d4  << 2) + 0xfe066800)
#define HENC_SCRATCH_L                             ((0x00d5  << 2) + 0xfe066800)
#define HENC_SCRATCH_M                             ((0x00d6  << 2) + 0xfe066800)
#define HENC_SCRATCH_N                             ((0x00d7  << 2) + 0xfe066800)
// bit [7:0] data_feed_buff_id
#define IE_DATA_FEED_BUFF_INFO                     ((0x00d8  << 2) + 0xfe066800)
//
// Closing file:  henc_regs.h
//
//------------------------------------------------------------------------------
// -----------------------------------------------
// `define DOS_HCODEC_VLD_CBUS_BASE      8'h0c
// -----------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// VLC module level register offset
//------------------------------------------------------------------------------
// `define DOS_HCODEC_VLC_CBUS_BASE    8'h0d
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe067400
// -----------------------------------------------
//
// Reading file:  vlc_regs.h
//
//========================================================================
//  VLC module level register offset
//========================================================================
// -----------------------------------------------
// `define DOS_HCODEC_VLC_CBUS_BASE    8'h0d
// -----------------------------------------------
// Bit[31:28] - henc_status
// Bit[27:9] - reserved
// Bit[8]    disable_UVLC_SKIP_fix
// Bit[7]    mc_hcmd_buff_enable
// Bit[6]    mc_hcmd_use_mc_hcmd_buff
// Bit[5]    mc_hcmd_use_mc_hcmd_hw_start
// Bit[4]    no_mc_out_null_non_skipped_mb
// Bit[3]    mc_out_even_skipped_mb
// Bit[2] - hcmd_enable
// Bit[1] - vlc_control_enable (0 will treat as soft_reset)
// Bit[0] - vlc_input_enable (enable input interface)
#define VLC_STATUS_CTRL                            ((0x0000  << 2) + 0xfe067400)
// Bit [31]  - small_luma_ignore_chroma
// Bit[30:16] - Reserved
// Bit [15]  - enable_free_clk_mc_hcmd_buff
// Bit [14]  - enable_free_clk_reg
// Bit [13]  - enable_free_clk_stream
// Bit [12]  - enable_free_clk_pre_buff
// Bit [11]  - enable_free_clk_vb_buff
// Bit [10]  - enable_free_clk_dc_input
// Bit [9]   - enable_free_clk_input
// Bit [8]   - enable_free_clk_mv_cal
// Bit [7]   - enable_free_clk_status
// Bit [6]   - enable_free_clk_mc_dblk_cmd_if
// Bit [5]   - disable_mvd_enc_finished
// Bit [4]   - I16MB_share_ipred_mode_with_I4MB
// Bit [3]   - fixed_picture_qp
// Bit [2]   - use_set_b8_mode ( not used now)
// Bit [1]   - use_set_mb_skip_run
// Bit [0]   - pop_coeff_even_all_zero
#define VLC_CONFIG                                 ((0x0001  << 2) + 0xfe067400)
// --------------------------------------------
// Bitstream DDR Interface
// --------------------------------------------
#define VLC_VB_START_PTR                           ((0x0010  << 2) + 0xfe067400)
#define VLC_VB_END_PTR                             ((0x0011  << 2) + 0xfe067400)
#define VLC_VB_WR_PTR                              ((0x0012  << 2) + 0xfe067400)
// Read Only, VLC_VB_SW_RD_PTR or VLC_VB_HW_RD_PTR
#define VLC_VB_RD_PTR                              ((0x0013  << 2) + 0xfe067400)
#define VLC_VB_SW_RD_PTR                           ((0x0014  << 2) + 0xfe067400)
// Read Only
#define VLC_VB_LEFT                                ((0x0015  << 2) + 0xfe067400)
// VB FIFO Control
// bit  [31:24] vb_full_int_enable_cpu[7:0]
// bit  [23:16] vb_full_int_enable_amrisc[7:0]
//   -bit 23 Video BUFFER LEFT < 0x400 Bytes
//   -bit 22 Video BUFFER LEFT < 0x200 Bytes
//   -bit 21 Video BUFFER LEFT < 0x100 Bytes
//   -bit 20 Video BUFFER LEFT < 0x80 Bytes
//   -bit 19 Video BUFFER LEFT < 0x40 Bytes
//   -bit 18 Video BUFFER LEFT < 0x20 Bytes
//   -bit 17 vbfifo left  < 16 double words (8x64bits)
//   -bit 16 vbfifo left  < 8 double words (4x64bits)
// bit  [15]    wr_ptr_int_enable_cpu
// bit  [14]    wr_ptr_int_enable_amrisc
// bit  [13]    wr_ptr_int (Read Only, write 1 will clear)
// bit  [12]    vb_full_int_enable_cpu(Read Only)
// bit  [11]    vb_full_int_enable_amrisc(Read Only)
// bit  [10]    reserved
// bit  [9]     use_vb_hw_rd_ptr
// bit  [8]     vb_always_ready Set to 1 to ignore vb_level blocking DDR write
// bit  [7]     vb_ready_for_transfer (Read Only)
// bit  [6]     data_ready_for_transfer (Read Only)
// bits [5:3]   vb_endian       Set VB Endian inside DDR memory
// bit  [2]     vb_write_busy   Read Only - indicate DDR write busy
// bit  [1]     vb_write_en     Set to 1 to enable writing data to DDR memory
// bit  [0]     vb_soft_reset   Soft reset
#define VLC_VB_CONTROL                             ((0x0016  << 2) + 0xfe067400)
// bit 31    transfer_length 0 - 32x64 Bits per request, 1 - 16x64 Bits per request  // default : 0
// bit 30    A_urgent  // default : 0
// bit 29:24 A_brst_num // default : 0x3f
// bit 23:22 reserved
// bit 21:16 A_id  // default : 0x20
// bit 15:0  vb_left_hold // default : 0
#define VLC_VB_MEM_CTL                             ((0x0017  << 2) + 0xfe067400)
// vb_wr_ptr address to generate interrupt
#define VLC_VB_INT_PTR                             ((0x0018  << 2) + 0xfe067400)
// bit[31:12] reserved
// bit[11:0] vlc_wrrsp_count
#define VLC_WRRSP                                  ((0x0019  << 2) + 0xfe067400)
#define VLC_TOTAL_BYTES                            ((0x001a  << 2) + 0xfe067400)
// bit  [31:26] reserved
// bit  [25]    vb_flush
// bit  [24]    vb_reg_ready
// bit  [23:16] vb_buff_left     -- Read only
// bit  [15:8]  vb_buff_wr_point -- RW
// bit  [7:0]   vb_buff_rd_point -- RW
#define VLC_VB_BUFF                                ((0x001b  << 2) + 0xfe067400)
// bit[31]    flush_vlc_pre_buff
// bit[30:27] reserved
// bit[26:24] vb_pre_buff_level
// bit[23:0]  vb_pre_buff_data[55:32]
#define VLC_VB_PRE_BUFF_HI                         ((0x001c  << 2) + 0xfe067400)
// bit[31:0] vb_pre_buff_data[31:0]
#define VLC_VB_PRE_BUFF_LOW                        ((0x001d  << 2) + 0xfe067400)
// bit[31]    stream_encap_enable  // 0x0000(00-03) will be changed to 0x000003(00-03) if enabled
// bit[30:29] stream_encap_delay   // delay bytes for encap used
// bit[28:24] stream_buff_point
// bit[23:0]  stream_buff_data
#define VLC_STREAM_BUFF                            ((0x001e  << 2) + 0xfe067400)
// bit[31]    push_stream_block - Read Only (Will set if vb_buff full)
// bit[30]    push_stream_busy  - Read Only (busy when output one byte from stream_buf (max 2 clocks for 8 bits push)
// bit[29:28] stream_encap_status - Read Only
// bit[27:21] reserved
// Long Mode : (Amrisc need movilo and mvihi high to set stream_info)
//   bit[20:16] push_stream_long_length (0 or 31 : Mid or Short mode)
//   bit[15:0]  push_stream_data
// Mid Mode : (Amrisc need movi to set stream_info)
//   bit[15:12]] push_stream_mid_length (1-12 : mid mode active, 0-short mode, 15 - byte align)
//   bit[11:0]  push_stream_data
// Short Mode : (Amrisc can use mtspi to push stream directly)
//   bit[7:5] push_stream_short_length (0-5 : short length, else - invalid)
//   bit[4:0]  push_stream_data
#define VLC_PUSH_STREAM                            ((0x001f  << 2) + 0xfe067400)
// bit[15:8] ELEMENT_LENGTH
// bit[7:1]  reserved
// bit[0]  Write - push start, Read - Push Busy
#define VLC_PUSH_ELEMENT                           ((0x0020  << 2) + 0xfe067400)
#define VLC_ELEMENT_DATA                           ((0x0021  << 2) + 0xfe067400)
// bit[9] reset_last_dquant_ne_zero
// bit[8] set_last_dquant_ne_zero
// bit[7] hcmd_intra_use_sw_info
// bit[6] hcmd_use_sw_info
// bit[5] always_use_info_0_cbp_blk
// bit[4] always_update_info_0_cbp_blk
// bit[3] reset_left_mb_wr
// bit[2] reset_top_mb_wr
// bit[1] set_left_mb_wr
// bit[0] set_top_mb_wr
#define VLC_SPECIAL_CTL                            ((0x0022  << 2) + 0xfe067400)
// bit[28]    left_intra
// bit[27:22] left_quant
// bit[21:16] left_quant_c
// bit[12]    top_intra
// bit[11:6]  top_quant
// bit[5:0]   top_quant_c
#define VLC_HCMD_T_L_INFO                          ((0x0023  << 2) + 0xfe067400)
// bit[12]    cur_intra
// bit[11:6]  cur_quant
// bit[5:0]   cur_quant_c
#define VLC_HCMD_CUR_INFO                          ((0x0024  << 2) + 0xfe067400)
// bit[13]    top_left_info_in_data_mv_cal
// bit[12]    only_use_info_0_top_left_in_I
// bit[11]    top_left_info_in_data
// bit[10]    early_mix_mc_hcmd
// bit[9]     update_top_left_mix
// bit[8]     p_top_left_mix
// bit[7]     mv_cal_mixed_type
// bit[6]     mc_hcmd_mixed_type
// bit[5]     use_seperate_int_control
// bit[4]     hcmd_intra_use_q_info
// bit[3]     hcmd_left_use_prev_info
// bit[2]     hcmd_use_q_info
// bit[1]     use_q_delta_quant
// bit[0]     detect_I16_from_I4
#define VLC_ADV_CONFIG                             ((0x0025  << 2) + 0xfe067400)
//   31 - use_hcmd_mb_xy_auto
//23:16 - hcmd_max_mb_x
//15:8  - hcmd_mb_y_auto
// 7:0  - hcmd_mb_x_auto
#define VLC_HCMD_MBXY_AUTO                         ((0x0026  << 2) + 0xfe067400)
// bit[31:0] vlc_int_control_inter -- will be used when use_seperate_int_control is set
#define VLC_INT_CONTROL_INTER                      ((0x002f  << 2) + 0xfe067400)
// --------------------------------------------
// Picture VLC
// --------------------------------------------
// (&(int|(~AND_enable))|(|(int&OR_enable)))
// bit[31]    OR interrupt Enable Reserved
// bit[30]    OR interrupt Enable for mv_busy
// bit[29]    OR interrupt Enable for b8_mode_busy
// bit[28]    OR interrupt Enable for mb_skip_run_busy
// bit[27]    OR interrupt Enable for cbp_busy
// bit[26]    OR interrupt Enable for Reserved
// bit[25]    OR interrupt Enable for Reserved
// bit[24]    OR interrupt Enable for mb_info_busy
// bit[23]    AND interrupt Enable Reserved
// bit[22]    AND interrupt Enable for mv_busy
// bit[21]    AND interrupt Enable for b8_mode_busy
// bit[20]    AND interrupt Enable for mb_skip_run_busy
// bit[19]    AND interrupt Enable for cbp_busy
// bit[18]    AND interrupt Enable for Reserved
// bit[17]    AND interrupt Enable for Reserved
// bit[16]    AND interrupt Enable for mb_info_busy
// bit[15]    OR interrupt Enable Reserved
// bit[14]    OR interrupt Enable for mvd
// bit[13]    OR interrupt Enable for b8_mode
// bit[12]    OR interrupt Enable for mb_skip_run
// bit[11]    OR interrupt Enable for cbp_ready
// bit[10]    OR interrupt Enable for c_ipred_mode_ready
// bit[9]     OR interrupt Enable for ipred_mode_ready
// bit[8]     OR interrupt Enable for mb_info_ready
// bit[7]     AND interrupt Enable Reserved
// bit[6]     AND interrupt Enable for mvd
// bit[5]     AND interrupt Enable for b8_mode
// bit[4]     AND interrupt Enable for mb_skip_run
// bit[3]     AND interrupt Enable for cbp_ready
// bit[2]     AND interrupt Enable for c_ipred_mode_ready
// bit[1]     AND interrupt Enable for ipred_mode_ready
// bit[0]     AND interrupt Enable for mb_info_ready
#define VLC_INT_CONTROL                            ((0x0030  << 2) + 0xfe067400)
// Bit[31:16] picture_height
// Bit[15:0]  picture_width
#define VLC_PIC_SIZE                               ((0x0031  << 2) + 0xfe067400)
// [31:16] Reserved
// [15:3] pic_width_in_mbs_minus1
// [2:0]  picture_type
#define VLC_PIC_INFO                               ((0x0032  << 2) + 0xfe067400)
// Bit[31:16] MB_NR
// Bit[15:8] MBY
// Bit[7:0] MBX
#define VLC_PIC_POSITION                           ((0x0033  << 2) + 0xfe067400)
// Bit[31] MV_busy
// Bit[30:6] Reserved
// Bit[15] Reserved
// Bit[14] vlc_input_MV_busy
// Bit[13] vlc_input_b8_mode_busy
// Bit[12] vlc_input_mb_skip_run_busy
// Bit[11] vlc_input_cbp_busy
// Bit[10] Reserved
// Bit[9]  Reserved
// Bit[8] vlc_input_mb_info_busy
// Bit[7] Reserved
// Bit[6] mvd
// Bit[5] b8_mode
// Bit[4] mb_skip_run
// Bit[3] cbp
// Bit[2] c_ipred_mode
// Bit[1] ipred_mode
// Bit[0] mb_info ( mb_type, mb_x, mb_y)
#define VLC_INPUT_STATUS                           ((0x0034  << 2) + 0xfe067400)
// Bit[31:28] detected_mb_type
// Bit[27:16] mb_y
// Bit[15:4]  mb_x
// Bit[3:0]   mb_type
#define VLC_MB_INFO                                ((0x0035  << 2) + 0xfe067400)
// command 0 -- no pending
// normal command + 1 is pending command
//
// bit 31:28 -- pending command 7
// bit 27:24 -- pending command 6
// bit 23:20 -- pending command 5
// bit 19:16 -- pending command 4
// bit 15:12 -- pending command 3
// bit 11:8  -- pending command 2
// bit  7:4  -- pending command 1
// bit  3:0  -- pending command 0
#define VLC_ENC_PEND_CMD                           ((0x0036  << 2) + 0xfe067400)
// For I Slice
// Bit[31:16] Reserved
// Bit[15:0] IntraType
#define HENC_TOP_INFO_0                            ((0x0037  << 2) + 0xfe067400)
#define HENC_LEFT_INFO_0                           ((0x0038  << 2) + 0xfe067400)
// For I Slice
// Bit[31:24] V_nnz
// Bit[23:16] U_nnz
// Bit[15:0]  Y_nnz
#define HENC_TOP_INFO_1                            ((0x0039  << 2) + 0xfe067400)
#define HENC_LEFT_INFO_1                           ((0x003a  << 2) + 0xfe067400)
// bit[63:0] ipred_mode
#define VLC_IPRED_MODE_HI                          ((0x003b  << 2) + 0xfe067400)
#define VLC_IPRED_MODE_LO                          ((0x003c  << 2) + 0xfe067400)
// bit[15:8] SE delta quant
// bit[7:0]  UE delta quant
#define VLC_DELTA_QP                               ((0x003d  << 2) + 0xfe067400)
// bit[31:16] Reserved
// bit[15:12] Reserved
// bit[11:8] c_ipred_mode
// bit[7:0] cbp
#define VLC_MB_HEADER_INFO                         ((0x003e  << 2) + 0xfe067400)
// bit[31:16] mb_skip_run
// bit[15:0]  b8_mode
#define VLC_P_MB_HEADER_INFO                       ((0x003f  << 2) + 0xfe067400)
// bit[31]    write_ptr_update
// bit[30]    read_ptr_update
// bit[29:20] coeff_buf_level
// bit[19:10] coeff_buf_write_ptr
// bit[9:0]   coeff_buf_read_ptr
#define VLC_COEFF_BUF_STATUS                       ((0x0040  << 2) + 0xfe067400)
// bit[31:10] Reserved
// bit[9:0] read_req_addr
#define VLC_COEFF_RD_REQ                           ((0x0041  << 2) + 0xfe067400)
// bit[20]   - coeff_buf_read_finished
// bit[19]   - coeff_first
// bit[18:4] - coeff_level
// bit[3:0]  - coeff_run
#define VLC_COEFF                                  ((0x0042  << 2) + 0xfe067400)
// bit[31]    - coeff_info_ready
// bit[30:25] - Reserved
// bit[24:15] - block_begin_addr
// bit[14:11] - trailing_one_sign
// bit[10:6]  - total_coeff
// bit[5:4]   - trailing_ones
// bit[3:0]   - total_zeros
#define VLC_COEFF_INFO                             ((0x0043  << 2) + 0xfe067400)
// bit[31]    write_ptr_update
// bit[30]    read_ptr_update
// bit[29:25] Reserved
// bit[24:20] dc_buf_level -- 0-16
// bit[19:15] Reserved
// bit[14:10] dc_buf_write_ptr
// bit[9:5]   Reserved
// bit[4:0]   dc_buf_read_ptr
#define VLC_DC_BUF_STATUS                          ((0x0044  << 2) + 0xfe067400)
// bit[31:10] Reserved
// bit[9:4]   Reserved
// bit[3:0]   read_req_addr
#define VLC_DC_RD_REQ                              ((0x0045  << 2) + 0xfe067400)
// bit[31:17] - Reserved
// bit[16]   - dc_first
// bit[15:1] - dc_level
// bit[0]    - dc_type  0 - CDC, 1 - LDC
#define VLC_DC                                     ((0x0046  << 2) + 0xfe067400)
// bit[31]    - dc_info_ready
// bit[30:25] - Reserved
// bit[24:20] - Reserved
// bit[19:15] - dc_idx_begin_addr
// bit[14:11] - dc_trailing_one_sign
// bit[10:6]  - dc_total_coeff
// bit[5:4]   - dc_trailing_ones
// bit[3:0]   - dc_total_zeros
#define VLC_DC_INFO                                ((0x0047  << 2) + 0xfe067400)
// bit[31:4]  - Reserved
// bit[3:0]   - mv_rw_idx  (auto increment when read/write VLC_MV)
#define VLC_MV_INDEX                               ((0x0048  << 2) + 0xfe067400)
// bit[31:16] - MVY
// bit[15:0]  - MVX
#define VLC_MV                                     ((0x0049  << 2) + 0xfe067400)
// bit[31:16] - MVY
// bitp15:0]  - MVX
// Write to HENC_TOP_MV_0 will cause previous U->UL, UR->U, and UR write
// Read will read U MV only
#define HENC_TOP_MV_0                              ((0x004a  << 2) + 0xfe067400)
#define HENC_TOP_MV_1                              ((0x004b  << 2) + 0xfe067400)
#define HENC_TOP_MV_2                              ((0x004c  << 2) + 0xfe067400)
#define HENC_TOP_MV_3                              ((0x004d  << 2) + 0xfe067400)
#define HENC_LEFT_MV_0                             ((0x004e  << 2) + 0xfe067400)
#define HENC_LEFT_MV_1                             ((0x004f  << 2) + 0xfe067400)
#define HENC_LEFT_MV_2                             ((0x0050  << 2) + 0xfe067400)
#define HENC_LEFT_MV_3                             ((0x0051  << 2) + 0xfe067400)
// Bit[31:1] Reserved
// Bit[0] top_left_mv_ready
#define TOP_LEFT_READY                             ((0x0052  << 2) + 0xfe067400)
// Bit[31:16] Reserved
// Bit[15:0] mb_skip_run
#define MB_SKIP_RUN                                ((0x0053  << 2) + 0xfe067400)
// Bit[31:16] Reserved
// Bit[15]    Reserved
// Bit[14:13] rd_struct // 00 - frame, 10 - top field, 11 - bottom field
// Bit[12:11] wr_struct // 00 - frame, 10 - top field, 11 - bottom field
// Bit[10:5]  mv_buff_id
// Bit[4:0]   mv_ref
#define VLC_HCMD_CONFIG                            ((0x0054  << 2) + 0xfe067400)
// Bit[9:0] h264_dblk_info
#define VLC_HCMD_DBLK_INFO                         ((0x0055  << 2) + 0xfe067400)
// Bit[3:0] vlc_dbg_idx
#define VLC_DBG_IDX                                ((0x0056  << 2) + 0xfe067400)
// Read Only
#define VLC_DBG_READ                               ((0x0057  << 2) + 0xfe067400)
// [   29] RW jpeg_comp2_ac_table_sel.                                                      Default 0.
// [   28] RW jpeg_comp2_dc_table_sel.                                                      Default 0.
// [26:25] RW jpeg_comp2_cnt_max.                                                           Default 0.
// [   24] RW jpeg_comp2_en. Set 1 if the scan will include component 2 (Cr).               Default 0.
// [   21] RW jpeg_comp1_ac_table_sel.                                                      Default 0.
// [   20] RW jpeg_comp1_dc_table_sel.                                                      Default 0.
// [18:17] RW jpeg_comp1_cnt_max.                                                           Default 0.
// [   16] RW jpeg_comp1_en. Set 1 if the scan will include component 1 (Cb).               Default 0.
// [   13] RW jpeg_comp0_ac_table_sel.                                                      Default 0.
// [   12] RW jpeg_comp0_dc_table_sel.                                                      Default 0.
// [10: 9] RW jpeg_comp0_cnt_max.                                                           Default 0.
// [    8] RW jpeg_comp0_en. Set 1 if the scan will include component 0 (Y).                Default 0.
// [    1] W  jpeg_coeff_init. Write 1 to this bit to initialize JPEG run length encoder.
// [    0] RW jpeg_en. 0=Disable JPEG encoder; 1=Enable JPEG encoder.                       Default 0.
#define VLC_JPEG_CTRL                              ((0x0058  << 2) + 0xfe067400)
// [   13] RW jpeg_coeff_buf_overfl. Write this bit to 1 to clear overfl.
// [   12] RW jpeg_coeff_buf_undrfl. Write this bit to 1 to clear undrfl.
// [   11] R  jpeg_coeff_buf_full.
// [   10] R  jpeg_coeff_buf_empty.
// [ 8: 0] R  jpeg_coeff_buf_level.
#define VLC_JPEG_COEFF_BUF_STAT                    ((0x0059  << 2) + 0xfe067400)
// [   16] RW JPEG HUFFMAN table access mode. 0=Write; 1=Read.  Default 1.
// [ 8: 0] RW JPEG HUFFMAN table addr.                          Default 0.
#define VLC_HUFFMAN_ADDR                           ((0x005a  << 2) + 0xfe067400)
// [19: 0] RW JPEG HUFFMAN table data.
#define VLC_HUFFMAN_DATA                           ((0x005b  << 2) + 0xfe067400)
// bit[31:0] enc_mv_bits - including b8_mode and mvd
#define VLC_ENC_MV_BITS                            ((0x005c  << 2) + 0xfe067400)
// bit[31:0] enc_coeff_bits - including cbp, token, trailing_sign, coeff_enc, h264_totzeros, h264_run
#define VLC_ENC_COEFF_BITS                         ((0x005d  << 2) + 0xfe067400)
#define VLC_RESERVED_5E                            ((0x005e  << 2) + 0xfe067400)
#define VLC_RESERVED_5F                            ((0x005f  << 2) + 0xfe067400)
// ----------------------------------------------------------
// CABAC
// ----------------------------------------------------------
// [15]     h264_biari_init
// [14]     h264_cabac_init
// [13:12]  reserved
// [11:10]  picture_ty
// [9:8]    cabac_init_idc  // should be 0 for  I_SLICE
// [7]      reserved
// [6]      cabac_en
// [5:0]    slice_quant
#define VLC_SLICE_QP                               ((0x0060  << 2) + 0xfe067400)
// For save mvd[12:0] {mvd_y[6:0], mvd_x[5:0]}
#define HENC_TOP_INFO_2                            ((0x0061  << 2) + 0xfe067400)
#define HENC_LEFT_INFO_2                           ((0x0062  << 2) + 0xfe067400)
#define HENC_TOP_INFO_3                            ((0x0063  << 2) + 0xfe067400)
#define HENC_LEFT_INFO_3                           ((0x0064  << 2) + 0xfe067400)
//
// Closing file:  vlc_regs.h
//
//------------------------------------------------------------------------------
// -----------------------------------------------
// `define DOS_HCODEC_IQIDCT_CBUS_BASE       8'h0e
// -----------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// QDCT module level register offset
//------------------------------------------------------------------------------
// `define DOS_HCODEC_QDCT_CBUS_BASE   8'h0f
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe067c00
// -----------------------------------------------
//
// Reading file:  qdct_regs.h
//
//========================================================================
//  QDCT module level register offset
//========================================================================
// -----------------------------------------------
// `define DOS_HCODEC_QDCT_CBUS_BASE   8'h0f
// -----------------------------------------------
// bit[31:28] - mb_info_state
#define QDCT_STATUS_CTRL                           ((0x0000  << 2) + 0xfe067c00)
// bit[18]    - enable_free_clk_sad_top
// bit[17]    - enable_free_clk_ie_top
// bit[16]    - enable_free_clk_ie_sub
// bit[15]    - enable_free_clk_mc_out
// bit[14]    - enable_free_clk_i_pred_top
// bit[13]    - enable_free_clk_idct_top
// bit[12]    - enable_free_clk_iq_top
// bit[11]    - enable_free_clk_q_top
// bit[10]    - enable_free_clk_dc_buff_8
// bit[9]     - enable_free_clk_dct_top
// bit[8]     - enable_free_clk_mb_info
// bit[7]     - enable_free_clk_mb_buff
// bit[6]     - enable_free_clk_reg
// bit[5]     - ignore_1_include_2nd_half_c
// bit[4]     - ignore_1_include_2nd_half
// bit[3]     - ignore_t_p8x8
// bit[2]     - not_ignore_dc
// bit[1]     - ignore_small_lac_intra
// bit[0]     - disable_fast_zero_block
#define QDCT_CONFIG                                ((0x0001  << 2) + 0xfe067c00)
// bit[31]    - ignore_lac_coeff_en
// bit[30:26] - ignore_lac_coeff_else
// bit[25:21] - ignore_lac_coeff_2
// bit[20:16] - ignore_lac_coeff_1
// bit[15]    - ignore_cac_coeff_en
// bit[14:10] - ignore_cac_coeff_else
// bit[9:5]   - ignore_cac_coeff_2
// bit[4:0]   - ignore_cac_coeff_1
#define IGNORE_CONFIG                              ((0x0002  << 2) + 0xfe067c00)
// bit[31]    - ignore_t_lac_coeff_en
// bit[30:26] - ignore_t_lac_coeff_else
// bit[25:21] - ignore_t_lac_coeff_2
// bit[20:16] - ignore_t_lac_coeff_1
// bit[15]    - ignore_cdc_coeff_en
// bit[14]    - ignore_t_lac_coeff_else_le_3
// bit[13]    - ignore_t_lac_coeff_else_le_4
// bit[12]    - ignore_cdc_only_when_empty_cac_inter
// bit[11]    - ignore_cdc_only_when_one_empty_inter
// bit[10:9]  - ignore_cdc_range_max_inter // 0-0, 1-1, 2,-2, 3-3
// bit[8:7]   - ignore_cdc_abs_max_inter // 0-1, 1-2, 2,-3, 3-4
// bit[6]     - reserved
// bit[5]     - ignore_cdc_only_when_empty_cac_intra
// bit[4]     - ignore_cdc_only_when_one_empty_intra
// bit[3:2]   - ignore_cdc_range_max_intra // 0-0, 1-1, 2,-2, 3-3
// bit[1:0]   - ignore_cdc_abs_max_intra // 0-1, 1-2, 2,-3, 3-4
#define IGNORE_CONFIG_2                            ((0x0003  << 2) + 0xfe067c00)
// --------------------------------------------
// MB DATA DDR Interface
// --------------------------------------------
#define QDCT_MB_START_PTR                          ((0x0010  << 2) + 0xfe067c00)
#define QDCT_MB_END_PTR                            ((0x0011  << 2) + 0xfe067c00)
#define QDCT_MB_WR_PTR                             ((0x0012  << 2) + 0xfe067c00)
#define QDCT_MB_RD_PTR                             ((0x0013  << 2) + 0xfe067c00)
#define QDCT_MB_LEVEL                              ((0x0014  << 2) + 0xfe067c00)
// bit  [31]    disable_mbxy_ie_save
// bit  [30]    no_wait_after_q_cdc
// bit  [29]    ie_start_int_enable
// bit  [28]    ignore_t_p8x8
// bit  [27]    zero_mc_out_null_non_skipped_mb
// bit  [26]    no_mc_out_null_non_skipped_mb
// bit  [25]    mc_out_even_skipped_mb
// bit  [24]    mc_out_wait_cbp_ready
// bit  [23]    mc_out_wait_mb_type_ready
// bit  [22]    i_pred_int_enable
// bit  [21]    intra_sat8_enable
// bit  [20]    ie_sub_enable
// bit  [19]    i_pred_enable
// bit  [18]    iq_enable
// bit  [17]    idct_enable
// bit  [16]    no_zero_coeff_mb
// bit  [15]    add_zero_coeff_block
// bit  [14]    mb_pause_enable
// bit  [13]    q_enable
// bit  [12]    dct_enable
// bit  [11]    ignore_magic_word
// bit  [10]    mb_info_en
// bit  [9]     mb_info_soft_reset
// bit  [8]     mb_always_ready Set to 1 to ignore mb_level blocking DDR read
// bit  [7]     mb_ready_for_transfer (Read Only)
// bit  [6]     data_ready_for_transfer (Read Only)
// bits [5:3]   mb_endian       Set MB Endian inside DDR memory
// bit  [2]     mb_read_busy   Read Only - indicate DDR read busy
// bit  [1]     mb_read_en     Set to 1 to enable reading data from DDR memory
// bit  [0]     mb_soft_reset   Soft reset
#define QDCT_MB_CONTROL                            ((0x0015  << 2) + 0xfe067c00)
// bit 31    transfer_length 0 - 32x64 Bits per request, 1 - 16x64 Bits per request  // default : 0
// bit 30    A_urgent  // default : 0
// bit 29:24 A_brst_num // default : 0x3f
// bit 23    reserved
// bit 22    read_till_zero
// bit 21:16 A_id  // default : 0x21
// bit 15:0  mb_level_hold // default : 0
#define QDCT_MB_MEM_CTL                            ((0x0016  << 2) + 0xfe067c00)
// bit  [31:24] reserved
// bit  [23:16] vb_buff_level     -- Read only
// bit  [15:8]  vb_buff_wr_point -- RW
// bit  [7:0]   vb_buff_rd_point -- RW
#define QDCT_MB_BUFF                               ((0x0017  << 2) + 0xfe067c00)
// bit [31:0] mb infor data begin magic word - default : 0xa55aaa55
#define QDCT_MB_MAGIC_WORD                         ((0x0018  << 2) + 0xfe067c00)
// Read Only Register
// bit[19]   dct_ready_1
// bit[18]   dct_ready_0
// bit[17]   dct_rd_idx
// bit[16]   dct_wr_idx
// bit[15]   dct_srdy
// bit[14]   dct_rrdy
// bit[13]   Reserved
// bit[12:8] dct_block_number
// bit[7:4]  dct_rw_addr
// bit[3:0]  dct_status
#define QDCT_DCT_STATUS                            ((0x0019  << 2) + 0xfe067c00)
// bit[24:23]  vlc_quant_count
// bit[22:16]  vlc_delta_quant_data
// bit[15:10]  vlc_quant_data
// bit[9:4]    vlc_prev_quant_data
// bit[3:0]    q_status
#define QDCT_Q_STATUS                              ((0x001a  << 2) + 0xfe067c00)
// bit[31:3] Reserved
// bit[2:0]  picture_type
#define QDCT_PIC_INFO                              ((0x001b  << 2) + 0xfe067c00)
// bit[31:28] Reserved
// bit[27:22] I_pic_quant_c
// bit[21:16] I_pic_quant
// bit[15]    reserved
// bit[14:12] quant_rem_c
// bit[11:8]  quant_per_c
// bit[7]     reserved
// bit[6:4]   quant_rem
// bit[3:0]   quant_per
#define QDCT_Q_QUANT_I                             ((0x001c  << 2) + 0xfe067c00)
// bit[31:28] Reserved
// bit[27:22] P_pic_quant_c
// bit[21:16] P_pic_quant
// bit[15]    reserved
// bit[14:12] quant_rem_c
// bit[11:8]  quant_per_c
// bit[7]     reserved
// bit[6:4]   quant_rem
// bit[3:0]   quant_per
#define QDCT_Q_QUANT_P                             ((0x001d  << 2) + 0xfe067c00)
// bit[31]    mb_info_pause
// bit[30:16] pause_mby
// bit[15:0]  pause_mbx
#define QDCT_MB_PAUSE_CTL                          ((0x001e  << 2) + 0xfe067c00)
// bit[31]   top_control_ready_i_pred
// bit[30:28]top_dma_status
// bit[27]   top_control_ready_ie
// bit[26:24]Reserved
// bit[23]   ul_mb
// bit[22]   right_mb
// bit[21]   top_mb
// bit[20]   left_mb
// bit[19:8] mb_x
// bit[7:6]  i_pred_ref_idx_L
// bit[5:4]  i_pred_ref_idx_UL
// bit[3:2]  i_pred_ref_idx_U
// bit[1:0]  i_pred_ref_idx_UR
#define QDCT_TOP_CONTROL                           ((0x001f  << 2) + 0xfe067c00)
// bit[31:0] top_base_addr - DDR Memory start address for save top Pixel
#define QDCT_TOP_BASE_MEM                          ((0x0020  << 2) + 0xfe067c00)
// bit 15:13 Reserved
// bit 12    A_urgent  // default : 0
// bit 11:6  A_brst_num // default : 0x3f
// bit 5:0   A_id  // default : 0x22
#define QDCT_TOP_MEM_CTL                           ((0x0021  << 2) + 0xfe067c00)
// bit [11:0] top_wrrsp_count
#define QDCT_TOP_WRRSP                             ((0x0022  << 2) + 0xfe067c00)
// Bit[3:0] qdct_dbg_idx
#define QDCT_DBG_IDX                               ((0x0023  << 2) + 0xfe067c00)
// Read Only
#define QDCT_DBG_READ                              ((0x0024  << 2) + 0xfe067c00)
// [19:18] RW jdct_inflow_ctrl. JPEG DCT input flow control.                                Default 0.
//            0=No halt;
//            1=DCT halts request at end of each 8x8 block;
//            2=DCT halts request at end of each MCU.
// [17:16] RW jpeg_coeff_last_sel.                                                          Default 0.
//            Select at which point to finish processing, give update to AmRISC so it can control the next step.
//            0=Mark last coeff at the end of an 8x8 block;
//            1=Mark at the end of an MCU;
//            2=Mark at the end of a scan.
// [   15] RW jpeg_quant_sel_comp2. 0=Comp2 select Quantization table 0; 1=Select table 1.  Default 0.
// [   14] RW jpeg_v_factor_comp2. Component 2's vertical sampling factor.                  Default 0.
// [   13] RW jpeg_h_factor_comp2. Component 2's horizontal sampling factor.                Default 0.
// [   12] RW jpeg_comp2_en. Set 1 if the scan will include component 2 (Cr).               Default 0.
// [   11] RW jpeg_quant_sel_comp1. 0=Comp1 select Quantization table 0; 1=Select table 1.  Default 0.
// [   10] RW jpeg_v_factor_comp1. Component 1's vertical sampling factor.                  Default 0.
// [    9] RW jpeg_h_factor_comp1. Component 1's horizontal sampling factor.                Default 0.
// [    8] RW jpeg_comp1_en. Set 1 if the scan will include component 1 (Cb).               Default 0.
// [    7] RW jpeg_quant_sel_comp0. 0=Comp0 select Quantization table 0; 1=Select table 1.  Default 0.
// [    6] RW jpeg_v_factor_comp0. Component 0's vertical sampling factor.                  Default 0.
// [    5] RW jpeg_h_factor_comp0. Component 0's horizontal sampling factor.                Default 0.
// [    4] RW jpeg_comp0_en. Set 1 if the scan will include component 0 (Y).                Default 0.
// [ 3: 1] RW jdct_intr_sel. JPEG DCT interrupt select.                                     Default 0.
//            0=Disable intr;
//            1=Intr at end of each 8x8 block of DCT input;
//            2=Intr at end of each MCU of DCT input;
//            3=Intr at end of a scan of DCT input;
//            4=Intr at end of each 8x8 block of DCT output;
//            5=Intr at end of each MCU of DCT output;
//            6=Intr at end of a scan of DCT output.
// [    0] RW jpeg_en. 0=Disable JPEG QDCT; 1=Enable JPEG QDCT.                             Default 0.
#define QDCT_JPEG_CTRL                             ((0x0025  << 2) + 0xfe067c00)
// [29:16] RW jpeg_x_end.   Picture X end.      Default 0.
// [13: 0] RW jpeg_x_start. Picture X start.    Default 0.
#define QDCT_JPEG_X_START_END                      ((0x0026  << 2) + 0xfe067c00)
// [29:16] RW jpeg_y_end.   Picture Y end.      Default 0.
// [13: 0] RW jpeg_y_start. Picture Y start.    Default 0.
#define QDCT_JPEG_Y_START_END                      ((0x0027  << 2) + 0xfe067c00)
// [    8] RW JPEG QUANT table access mode. 0=Write; 1=Read.    Default 1.
// [ 5: 0] RW JPEG QUANT table addr.                            Default 0.
#define QDCT_JPEG_QUANT_ADDR                       ((0x0028  << 2) + 0xfe067c00)
// [31: 0] RW JPEG QUANT table data.
#define QDCT_JPEG_QUANT_DATA                       ((0x0029  << 2) + 0xfe067c00)
// [    1] W  resume_jdct.
//            Write 1 to this bit will resume JPEG input engine when previously
//            it is halted by QDCT_JPEG_CTRL.jdct_inflow_ctrl.
// [    0] W  jpeg_sof.
//            Write 1 to this bit will start JPEG input engine.
#define QDCT_JPEG_SOF_RESUME                       ((0x002a  << 2) + 0xfe067c00)
// [31:30] R  dct_st.       jdct_ctrl state. 0=IDLE; 1=WAIT; 2=DCT_REQ.
// [29:28] R  mfdin_comp.   Current component at pixel request to MFDIN.
// [27:14] R  mfdin_y.      Current Y at pixel request to MFDIN.
// [13: 0] R  mfdin_x.      Current X at pixel request to MFDIN.
#define QDCT_JPEG_DCT_STATUS0                      ((0x002b  << 2) + 0xfe067c00)
// [15:12] R  JPEG Quant read MEM select.
// [11: 8] R  JPEG DCT2 write MEM select.
// [ 7: 4] R  JPEG DCT2 read MEM select.
// [ 3: 0] R  JPEG DCT1 write MEM select.
#define QDCT_JPEG_DCT_STATUS1                      ((0x002c  << 2) + 0xfe067c00)
// [28:16] RW jdct_coeff1.  Default 13'h1D90; // 7568 = 4096 * sqrt(2) * (cos(Pi/8) + sin(Pi/8)) // 7569
// [12: 0] RW jdct_coeff0.  Default 13'h16A1; // 5793 = 4096 * sqrt(2)
#define QDCT_JPEG_DCT_COEFF01                      ((0x002d  << 2) + 0xfe067c00)
// [28:16] RW jdct_coeff3.  Default 13'h0C3F; // 3135 = 4096 * sqrt(2) * (cos(Pi/8) - sin(Pi/8))
// [12: 0] RW jdct_coeff2.  Default 13'h08A9; // 2217 = 4096 * sqrt(2) * sin(Pi/8) // 2215
#define QDCT_JPEG_DCT_COEFF23                      ((0x002e  << 2) + 0xfe067c00)
// [28:16] RW jdct_coeff5.  Default 13'h046A; // 1130 = 4096 * (cos(3Pi/16) - sin(3Pi/16))
// [12: 0] RW jdct_coeff4.  Default 13'h1631; // 5681 = 4096 * (cos(3Pi/16) + sin(3Pi/16)) // 5682
#define QDCT_JPEG_DCT_COEFF45                      ((0x002f  << 2) + 0xfe067c00)
// [28:16] RW jdct_coeff7.  Default 13'h0C92; // 3218 = 4096 * (cos(Pi/16) - sin(Pi/16))
// [12: 0] RW jdct_coeff6.  Default 13'h12D0; // 4816 = 4096 * (cos(Pi/16) + sin(Pi/16))
#define QDCT_JPEG_DCT_COEFF67                      ((0x0030  << 2) + 0xfe067c00)
// [28:16] RW jdct_coeff9.  Default 13'h0FB1; // 4017 = 4096 * cos(Pi/16)
// [12: 0] RW jdct_coeff8.  Default 13'h0D4E; // 3406 = 4096 * cos(3Pi/16)
#define QDCT_JPEG_DCT_COEFF89                      ((0x0031  << 2) + 0xfe067c00)
#define QDCT_I_PRED_REF_WR_IDX                     ((0x0032  << 2) + 0xfe067c00)
#define QDCT_I_PRED_REF_WR_DATA                    ((0x0033  << 2) + 0xfe067c00)
// bit[29]  mb_info_latch_no_I16_pred_mode
// bit[28]  ie_dma_mbxy_use_i_pred
// bit[27]  ie_dma_read_write_use_ip_idx
// bit[26]  ie_start_use_top_dma_count
// bit[25]  i_pred_top_dma_rd_mbbot
// bit[24]  i_pred_top_dma_wr_disable
// bit[23]  i_pred_mix
// bit[22]  me_ab_rd_when_intra_in_p
// bit[21]  force_mb_skip_run_when_intra
// bit[20]  mc_out_mixed_type
// bit[19]  ie_start_when_quant_not_full
// bit[18]  mb_info_state_mix
// bit[17]  mb_type_use_mix_result
// bit[16]  me_cb_ie_read_enable
// bit[15]  ie_cur_data_from_me
// bit[14]  rem_per_use_table
// bit[13]  q_latch_int_enable
// bit[12]  q_use_table
// bit[11]  q_start_wait
// bit[10]  LUMA_16_LEFT_use_cur
// bit[9]   DC_16_LEFT_SUM_use_cur
// bit[8]   c_ref_ie_sel_cur
// bit[7]   c_ipred_perfect_mode
// bit[6]   ref_ie_ul_sel 0-old, 1-use next_set
// bit[5]   mb_type_use_ie_result
// bit[4]   detect_I16_from_I4
// bit[3]   ie_not_wait_ref_busy
// bit[2]   ie_I16_enable
// bit[1:0] ie_done_sel (0, 1, 2, -1)
#define QDCT_ADV_CONFIG                            ((0x0034  << 2) + 0xfe067c00)
// bit [31:16] I16x16_weight
// bit [15:0]  I4x4_weight
#define IE_WEIGHT                                  ((0x0035  << 2) + 0xfe067c00)
// bit [31]  reserved
// bit [30] q_I16MB
// bit [29]  reserved
// bit [28]  reserved
// bit [27:23] quant_table_addr I4MB:[0,7], I16MB:[8:15], INTER:[16:23], Reserved:[24:31]
// bit [22]    quant_table_addr_update(when notupdate_rem_per) (read is arb_result : q_intra)
// bit [21:19] quant_rem_c
// bit [18:16] quant_rem_y
// bit [15:12] quant_per_c
// bit [11:8]  quant_per_y
// bit [7:2] quant
// bit [1] update_rem_per
// bit [0] clear_wait_status
#define Q_QUANT_CONTROL                            ((0x0036  << 2) + 0xfe067c00)
#define MBBOT_EVEN_ADDR                            ((0x0037  << 2) + 0xfe067c00)
#define MBBOT_ODD_ADDR                             ((0x0038  << 2) + 0xfe067c00)
// bit [29:24] quant_table_data_3
// bit [21:16] quant_table_data_2
// bit [13:8]  quant_table_data_1
// bit [5:0]   quant_table_data_0
#define QUANT_TABLE_DATA                           ((0x0039  << 2) + 0xfe067c00)
// bit [31:16] ie_sad_offset_I16
// bit [15:0] ie_sad_offset_I4
#define SAD_CONTROL_0                              ((0x003a  << 2) + 0xfe067c00)
// bit [31]     sad_soft_reset
// bit [30:27]  reserved
// bit [26:24]  ie_sad_shift_I16
// bit [22:20]  ie_sad_shift_I4
// bit [18:16]  me_sad_shift_INTER
// bit [15:0]   me_sad_offset_INTER
#define SAD_CONTROL_1                              ((0x003b  << 2) + 0xfe067c00)
// bit [31]     vlc_quant_rd_ptr
// bit [30]     vlc_quant_rd_ptr_update
// bit [29]     vlc_quant_wr_ptr
// bit [28]     vlc_quant_wr_ptr_update
// bit [27:26]  reserved
// bit [25:19]  vlc_delta_quant_1
// bit [18:13]  vlc_quant_1
// bit [12:6]   vlc_delta_quant_0
// bit [5:0]    vlc_quant_0
#define QDCT_VLC_QUANT_CTL_0                       ((0x003c  << 2) + 0xfe067c00)
// bit [31:30]  me_sad_rd_idx
// bit [29]     me_sad_rd_idx_update
// bit [28:27]  me_sad_wr_idx
// bit [26]     me_sad_wr_idx_update
// bit [25:24]  ie_sad_rd_idx
// bit [23]     ie_sad_rd_idx_update
// bit [22:21]  ie_sad_wr_idx
// bit [20]     ie_sad_wr_idx_update
// bit [19:12]  Reserved
// bit [11:6]   vlc_max_delta_q_neg
// bit [5:0]    vlc_max_delta_q_pos
#define QDCT_VLC_QUANT_CTL_1                       ((0x003d  << 2) + 0xfe067c00)
// bit[4] jdct_int
// bit[3] quant_latch
// bit[2] me_done
// bit[1] ie_start
// bit[0] i_pred_start
#define QDCT_INT_STATUS                            ((0x003e  << 2) + 0xfe067c00)
// bit[3]   mix_i_pred_wait_left_en
// bit[2:0] mc_pending_mb_count
#define QDCT_MIX_I_PRED_STATUS                     ((0x003f  << 2) + 0xfe067c00)
// IE/ME share same cbus_base as QDCT
//
// bit[31]  active_ur_block
// bit[30]  active_ul_block
// bit[21]  disable_HENC_V_PRED_16
// bit[20]  disable_HENC_H_PRED_16
// bit[19]  disable_HENC_DC_PRED_16
// bit[18]  disable_HENC_HENC_HOR_UP_PRED
// bit[17]  disable_HENC_VERT_LEFT_PRED
// bit[16]  disable_HENC_HENC_HOR_DOWN_PRED
// bit[15]  disable_HENC_VERT_RIGHT_PRED
// bit[14]  disable_HENC_DIAG_DOWN_RIGHT_PRED
// bit[13]  disable_HENC_DIAG_DOWN_LEFT_PRED
// bit[12]  disable_HENC_DC_PRED
// bit[11]  disable_HENC_HOR_PRED
// bit[10]  disable_HENC_VERT_PRED
// bit[9]   ie_c_wait_ie_pause
// bit[8:4] max_ie_result_buff_count
// bit[3]   ie_wait_for_ie_result  // use for (max_ie_result_buff_count == 0) to wait every block
// bit[2]   reserved
// bit[1]   ie_enable
// bit[0]   ie_soft_reset
#define IE_CONTROL                                 ((0x0040  << 2) + 0xfe067c00)
// bit[31:12] Reserved
// bit[11:0]  mb_y
#define IE_MB_POSITION                             ((0x0041  << 2) + 0xfe067c00)
// bit[31:16] me_skip_run
// bit[15:8]  reserved
// bit [7:4]  me_b8_mode
// bit [3:0]  ie_me_mb_type
#define IE_ME_MB_INFO                              ((0x0042  << 2) + 0xfe067c00)
// bit[3]   ie_result_buff_enable
// bit[2]   ie_result_buff_soft_reset
// bit[1]   sad_enable
// bit[0]   ie_sad_soft_reset
#define SAD_CONTROL                                ((0x0043  << 2) + 0xfe067c00)
// bit[31:16] - ie_result_buff_count - read only
// bit[14:12] -current_ie_result (pred_mode) - read only
// bit[11]  top_control_data_buff_wr_ptr - rd/wr
// bit[10]  top_control_data_buff_rd_ptr - rd/wr
// bit[9:5] ie_result_buff_wr_ptr        - rd/wr
// bit[4:0] ie_result_buff_rd_ptr        - rd/wr
#define IE_RESULT_BUFFER                           ((0x0044  << 2) + 0xfe067c00)
// bit[63:0] ie_i4_pred_mode
#define IE_I4_PRED_MODE_HI                         ((0x0045  << 2) + 0xfe067c00)
#define IE_I4_PRED_MODE_LO                         ((0x0046  << 2) + 0xfe067c00)
// bit [31:9] reserved
// bit [8:4] i_pred_block_number
// bit [3:0] ie_c_pred_mode
#define IE_C_PRED_MODE                             ((0x0047  << 2) + 0xfe067c00)
// 31:16 - Left, 0-select ie_pixel, 1-select mc_data_out
// 15:0  - Top,  0-select ie_pixel, 1-select mc_data_out
#define IE_CUR_REF_SEL                             ((0x0048  << 2) + 0xfe067c00)
// bit[31]  enable_f_skip_1_step_3
// bit[30]  enable_f_skip_1_step_2
// bit[29]  enable_f_skip_1_step_1
// bit[28]  step_3_enable
// bit[27]  step_2_sub_pixel
// bit[26]  always_f_skip_step_3
// bit[25]  always_f_skip_step_2
// bit[24]  always_f_skip_step_1
// bit[23]  half_pixel_only
// bit[22:13] reserved
// bit[12]  adv_mv_enable
// bit[11]  sad_short_valid
// bit[10:9] mv_flexable_ctl 00:0x40, 01:0x80, 10:0xc0, 11 : 0x100
// bit[8]   no_md_ab_rd
// bit[7]   use_dct_state
// bit[6]   enable_free_clk_me_ctl
// bit[5]   enable_free_clk_me_ab
// bit[4]   enable_free_clk_me_cb
// bit[3]   reserved
// bit[2]   me_done_int_enable
// bit[1]   me_enable
// bit[0]   me_soft_reset
#define ME_CONTROL                                 ((0x0049  << 2) + 0xfe067c00)
// bit[31:28] Reserved
// bit[27:16] me_start_mby
// bit[15:12] Reserved
// bit[11:0]  me_start_mbx
#define ME_START_POSITION                          ((0x004a  << 2) + 0xfe067c00)
// bit [31:28] me_status - Read only
// bit [27:24] me_cb_status  // current buffer
// bit [23:20] me_ab_status  // anc buffer
// bit [19]    Reserved
// bit [18:16] me_ab_wr_ptr  // read only
// bit [15:4]  me_last_mbx
// bit [3:1]   me_ab_rd_ptr  // read only
// bit [0]     me_action
#define ME_STATUS                                  ((0x004b  << 2) + 0xfe067c00)
// all Read-only
// bit [31:28] Reserved
// bit [27:16] me_ab_mbx
// bit [15:14] me_cb_wr_ptr
// bit [13:12] me_cb_rd_ptr
// bit [11:0]  me_cb_mbx
#define ME_DEBUG                                   ((0x004c  << 2) + 0xfe067c00)
// bit[29:24] step_3_skip_line
// bit[23:18] step_2_skip_line
// bit[17:12] step_1_skip_line
// bit[11:6]  step_0_skip_line
// bit[5:0]   read_skip_line
#define ME_SKIP_LINE                               ((0x004d  << 2) + 0xfe067c00)
// bit 15:13 Reserved
// bit 12    A_urgent  // default : 0
// bit 11:6  A_brst_num // default : 0x3f
// bit 5:0   A_id  // default : 0x23
#define ME_AB_MEM_CTL                              ((0x004e  << 2) + 0xfe067c00)
// bit[31:24] anc_pic_canvas
// bit[23:12] max_me_mby_num
// bit[11:0]  max_me_mbx_num
#define ME_PIC_INFO                                ((0x004f  << 2) + 0xfe067c00)
// bit [23:12] me_sad_enough_1
// bit [11:0]  me_sad_enough_0
#define ME_SAD_ENOUGH_01                           ((0x0050  << 2) + 0xfe067c00)
// bit [25:12] adv_mv_8x8_enough
// bit [11:0]  me_sad_enough_2
#define ME_SAD_ENOUGH_23                           ((0x0051  << 2) + 0xfe067c00)
// bit [21:10] me_step0_big_sad
// bit [9:5] me_step0_close_mv_y
// bit [4:0] me_step0_close_mv_x
#define ME_STEP0_CLOSE_MV                          ((0x0052  << 2) + 0xfe067c00)
// bit[31:24] force_skip_sad_3
// bit[23:16] force_skip_sad_2
// bit[15:08] force_skip_sad_1
// bit[07:00] force_skip_sad_0
#define ME_F_SKIP_SAD                              ((0x0053  << 2) + 0xfe067c00)
// bit[31:24] force_skip_weight_3
// bit[23:16] force_skip_weight_2
// bit[15:08] force_skip_weight_1
// bit[07:00] force_skip_weight_0
#define ME_F_SKIP_WEIGHT                           ((0x0054  << 2) + 0xfe067c00)
// Merge when (
//              (flex & me_merge_flex_en) |
//              (big_sad & me_merge_sad_en)|
//              ((~me_merge_flex_en)&(~me_merge_sad_en))
//            ) &
//            ( small_mv_diff |(~me_merge_small_mv_en))
//            & (me_merge_flex_en | me_merge_sad_en | me_merge_small_mv_en)
// bit[31]   me_merge_mv_en_16
// bit[30]   me_merge_small_mv_en_16
// bit[29]   me_merge_flex_en_16
// bit[28]   me_merge_sad_en_16
// bit[27]   me_merge_mv_en_8
// bit[26]   me_merge_small_mv_en_8
// bit[25]   me_merge_flex_en_8
// bit[24]   me_merge_sad_en_8
// bit[23:18] me_merge_mv_diff_16
// bit[17:12] me_merge_mv_diff_8
// bit[11:0] me_merge_min_sad
#define ME_MV_MERGE_CTL                            ((0x0055  << 2) + 0xfe067c00)
// bit[31:24] mv_step_weight_1
// bit[23:16] mv_pre_weight_1
// bit [15:0] mv_step_weight_0
// bit[7:0]   mv_pre_weight_0
#define ME_MV_WEIGHT_01                            ((0x0056  << 2) + 0xfe067c00)
// bit[31:24] mv_step_weight_3 (mv_step_weight_1_2nd)
// bit[23:16] mv_pre_weight_3 (mv_pre_weight_1_2nd)
// bit[15:8]  mv_step_weight_2
// bit[7:0]   mv_pre_weight_2
#define ME_MV_WEIGHT_23                            ((0x0057  << 2) + 0xfe067c00)
// bit[31:24] me_sad_range_inc_3
// bit[23:16] me_sad_range_inc_2
// bit[15:8] me_sad_range_inc_1
// bit[7:0] me_sad_range_inc_0
#define ME_SAD_RANGE_INC                           ((0x0058  << 2) + 0xfe067c00)
// bit[31:19]  reserved
// bit[18]     sub_relative_any_1_en
// bit[17]     sub_relative_any_2_en
// bit[16]     sub_relative_any_3_en
// bit[15]     sub_relative_any_4_en
// bit[14]     sub_relative_any_h_en
// bit[13]     sub_make_non_flex_fix
// bit[12]     sub_make_big_non_flex_fix
// bit[11:8]   sub_normal_mv_diff_16
// bit[7:4]    sub_normal_mv_diff_8
// bit[3:2]    Reserved
// bit[1]      sub_merge_enable_16
// bit[0]      sub_merge_enable_8
#define ME_SUB_MERGE_CTL                           ((0x0059  << 2) + 0xfe067c00)
// bit[31:28]  sub_ref_mv_diff_near
// bit[27:24]  sub_ref_mv_diff_near_flex
// bit[23:20]  sub_ref_mv_diff
// bit[19:16]  sub_ref_mv_diff_flex
// bit[15:8]   sub_ref_mv_near_weight
// bit[7:0]    sub_ref_mv_weight
#define ME_SUB_REF_MV_CTL                          ((0x005a  << 2) + 0xfe067c00)
// bit[31:24]  sub_weight_max_sad
// bit[23:16]  sub_any_range_sad
// bit[15:8]   sub_any_min_sad
// bit[7:0]    sub_any_max_sad
#define ME_SUB_ANY_WEIGHT_SAD                      ((0x005b  << 2) + 0xfe067c00)
// bit[31:24]  sub_fix_sad
// bit[23:16]  sub_fix_diff_sad
// bit[15:8]   sub_sad_flex_adj
// bit[7:0]    sub_big_non_flex_fix_sad
#define ME_SUB_FIX_SAD                             ((0x005c  << 2) + 0xfe067c00)
// bit[31:24]  sub_fix_min_sad
// bit[23:16]  sub_fix_min_diff_sad
// bit[15:8]   sub_fix_min_sad_2
// bit[7:0]    sub_fix_min_diff_sad_2
#define ME_SUB_FIX_MIN_SAD                         ((0x005d  << 2) + 0xfe067c00)
// bit[31:24]  sub_snap_to_ref_max_sad
// bit[23]     sub_snap_to_ref_non_flex
// bit[22:16]  sub_snap_to_ref_mv_diff
// bit[15:4]   sub_glitch_mv_sad
// bit[3:0]    sub_glitch_mv_diff
#define ME_SUB_SNAP_GLITCH                         ((0x005e  << 2) + 0xfe067c00)
// bit[31:20]  sad_act_limit
// bit[19:8]   s_a_any_sad
// bit[7]      Reserved
// bit[6:5]    sub_merge_method
// bit[4]      use_sub_act_result
// bit[3]      s_a_rnd
// bit[2]      Reserved
// bit[1]      sub_act_en_16
// bit[0]      sub_act_en
#define ME_SUB_ACT_CTL                             ((0x005f  << 2) + 0xfe067c00)
// bit[31:16]  reserved
// bit[15:0]   me_weight
#define ME_WEIGHT                                  ((0x0060  << 2) + 0xfe067c00)
// bit[31]  ready_update
// bit[30]  data_update
// bit[29:28] reserved
// bit[27:22] me_quant_0
// bit[21]  ie_sad_ready_0
// bit[20:18]  Reserved
// bit[17:16]  me_sad_mv_type_0
// bit[15:0] me_sad_0
#define ME_SAD_0                                   ((0x0061  << 2) + 0xfe067c00)
// bit[31]  ready_update
// bit[30]  data_update
// bit[29:28] reserved
// bit[27:22] me_quant_1
// bit[21]  me_sad_ready_1
// bit[20:18]  Reserved
// bit[17:16]  me_sad_mv_type_1
// bit[15:0] me_sad_1
#define ME_SAD_1                                   ((0x0062  << 2) + 0xfe067c00)
// bit[31]  ready_update
// bit[30]  data_update
// bit[29:28] reserved
// bit[27:22] me_quant_2
// bit[21]  me_sad_ready_2
// bit[20:18]  Reserved
// bit[17:16]  me_sad_mv_type_2
// bit[15:0] me_sad_2
#define ME_SAD_2                                   ((0x0063  << 2) + 0xfe067c00)
// bit[31]  ready_update
// bit[30]  data_update
// bit[29:28] reserved
// bit[27:22] me_quant_3
// bit[21]  me_sad_ready_3
// bit[20:18]  Reserved
// bit[17:16]  me_sad_mv_type_3
// bit[15:0] me_sad_3
#define ME_SAD_3                                   ((0x0064  << 2) + 0xfe067c00)
// bit[31]  ready_update
// bit[30]  data_update
// bit[29:28] reserved
// bit[27:22] ie_quant_0
// bit[21]  ie_sad_ready_0
// bit[20]   I4/I16 (0-I4MB, 1-I16MB)
// bit[19:0] ie_sad_0
#define IE_SAD_0                                   ((0x0065  << 2) + 0xfe067c00)
// bit[31]  ready_update
// bit[30]  data_update
// bit[29:28] reserved
// bit[27:22] ie_quant_1
// bit[21]  ie_sad_ready_1
// bit[20]   I4/I16 (0-I4MB, 1-I16MB)
// bit[19:0] ie_sad_1
#define IE_SAD_1                                   ((0x0066  << 2) + 0xfe067c00)
// bit[31]  ready_update
// bit[30]  data_update
// bit[29:28] reserved
// bit[27:22] ie_quant_2
// bit[21]  ie_sad_ready_2
// bit[20]   I4/I16 (0-I4MB, 1-I16MB)
// bit[19:0] ie_sad_2
#define IE_SAD_2                                   ((0x0067  << 2) + 0xfe067c00)
// bit[31]  ready_update
// bit[30]  data_update
// bit[29:28] reserved
// bit[27:22] ie_quant_3
// bit[21]  ie_sad_ready_3
// bit[20]   I4/I16 (0-I4MB, 1-I16MB)
// bit[19:0] ie_sad_3
#define IE_SAD_3                                   ((0x0068  << 2) + 0xfe067c00)
// bit[31]   enable_large_diff_16x8
// bit[30]   enable_large_diff_8x16
// bit[29:16] adv_mv_8x8_weight
// bit[15:14] reserved
// bit[13:0] adv_mv_4x4x4_weight
#define ADV_MV_CTL0                                ((0x0069  << 2) + 0xfe067c00)
// bit[31:16] adv_mv_16x16_weight
// bit[15]   enable_large_diff_16x16
// bit[14:0] adv_mv_16_8_weight
#define ADV_MV_CTL1                                ((0x006a  << 2) + 0xfe067c00)
#define ADV_MV_CTL2                                ((0x006b  << 2) + 0xfe067c00)
// bit[31]    v3_skip_enable
// bit[30]    v3_step_1_weight_enable // for step1, will only use pre_weight for block0, only step_weight for block 1,2,3
// bit[29]    v3_l2_skip_only_use_l2_sad // set to 1 may allow l1 maybe not skip
// bit[28]    v3_mv_sad_weight_enable
// bit[27]    v3_ipred_type_enable
// bit[25:12] force_skip_sad_1
// bit[11:0]  force_skip_sad_0
#define V3_SKIP_CONTROL                            ((0x006c  << 2) + 0xfe067c00)
// bit[18:16] top_ipred_type_count
// bit[15:14] Reserved
// bit[13:12] top_ipred_type_rd_idx
// bit[11:10] top_ipred_type_wr_idx
// bit[9]   left_ipred_type_ready
// bit[8]   top_ipred_type_ready
// bit[7:6] left_mv_idx
// bit[5:4] top_mv_idx
// bit[3] - left_mv_ready
// bit[2:0] top_mv_ready
#define V3_TOP_LEFT_CTL                            ((0x006d  << 2) + 0xfe067c00)
#define V3_TOP_MV                                  ((0x006e  << 2) + 0xfe067c00)
#define V3_LEFT_MV                                 ((0x006f  << 2) + 0xfe067c00)
// bit[31:30] reserved
// bit[29:16] v3_skip_weight_1
// bit[15:12] reserved
// bit[11:0]  v3_skip_weight_0
#define V3_SKIP_WEIGHT                             ((0x0070  << 2) + 0xfe067c00)
// bit[31:28] reserved
// bit[27:16] v3_l1_f_skip_max_sad
// bit[15:12] reserved
// bit[11:0]  v3_l1_skip_max_sad
#define V3_L1_SKIP_MAX_SAD                         ((0x0071  << 2) + 0xfe067c00)
// bit[31:16] force_skip_sad_2
// bit[15:0]  v3_skip_weight_2
#define V3_L2_SKIP_WEIGHT                          ((0x0072  << 2) + 0xfe067c00)
// bit[21:16] v3_mv_sad_table_addr 0-15 table_step_0
// bit[11:0]  v3_mv_sad
#define V3_MV_SAD_TABLE                            ((0x0073  << 2) + 0xfe067c00)
// [31:16] v3_ie_f_zero_sad_i16
// [15:0]  v3_ie_f_zero_sad_i4
#define V3_F_ZERO_CTL_0                            ((0x0074  << 2) + 0xfe067c00)
// [25]    v3_no_ver_when_top_zero_en
// [24]    v3_no_hor_when_left_zero_en
// [23:16] v3_max_I16_H_num    // 0 means no break
// [15:0]  v3_me_f_zero_sad
#define V3_F_ZERO_CTL_1                            ((0x0075  << 2) + 0xfe067c00)
#define V3_TOP_INTRA_INFO                          ((0x0076  << 2) + 0xfe067c00)
#define V3_LEFT_INTRA_INFO                         ((0x0077  << 2) + 0xfe067c00)
// [31:24]  C_ipred_weight_H
// [23:16]  C_ipred_weight_V
// [15:8]   I4_ipred_weight_else
// [7:0]    I4_ipred_weight_most
#define V3_IPRED_TYPE_WEIGHT_0                     ((0x0078  << 2) + 0xfe067c00)
// [31:24]  I16_ipred_weight_DC
// [23:16]  I16_ipred_weight_H
// [15:8]   I16_ipred_weight_V
// [7:0]    C_ipred_weight_DC
#define V3_IPRED_TYPE_WEIGHT_1                     ((0x0079  << 2) + 0xfe067c00)
// [31:16]  v3_left_small_max_me_sad
// [15:0]   v3_left_small_max_ie_sad
#define V3_LEFT_SMALL_MAX_SAD                      ((0x007a  << 2) + 0xfe067c00)
// (qr==7) & (qp==15) means force zero block
// [31:26]  v4_force_q_r_intra
// [25:20]  v4_force_q_r_inter
// [19]     v4_force_q_y_enable
// [18:16]  v4_force_qr_y
// [15:12]  v4_force_qp_y
// [11:0]   v4_force_skip_sad
#define V4_FORCE_SKIP_CFG                          ((0x007b  << 2) + 0xfe067c00)
// v5 Complexity // Y(256 pixel per MB) and C(128 pixel per MB)
// For every 4x4 block, calculate average and Add abs(pixel-average) together
// bit[31:16] v5_diff_sum_C
// bit[15:0]  v5_diff_sum_Y
#define V5_MB_DIFF_SUM                             ((0x007c  << 2) + 0xfe067c00)
// bit[31:24] v5_small_diff_cnt_C
// bit[23:16] v5_small_diff_C
// bit[15:8]  v5_small_diff_cnt_Y
// bit[7:0]   v5_small_diff_Y
#define V5_SMALL_DIFF_CNT                          ((0x007d  << 2) + 0xfe067c00)
// bit[31:29] Reserved
// bit[28:24] v5_simple_block_cnt // 0 -15 Y 16-23 C
// bit[23:20] v5_simple_pixel_cnt
// bit[19:18] v5_simple_dq_wr_ptr
// bit[17:16] v5_simple_dq_rd_ptr
// bit[15:12] v5_simple_dq (Read Only)
// bit[11:8]  v5_simple_me_weight(Read Only)
// bit[7] v5_use_small_diff_cnt
// bit[6] v5_simple_mb_inter_all_en
// bit[5] v5_simple_mb_inter_8x8_en
// bit[4] v5_simple_mb_inter_16_8_en
// bit[3] v5_simple_mb_inter_16x16_en
// bit[2] v5_simple_mb_intra_en
// bit[1] v5_simple_mb_C_en
// bit[0] v5_simple_mb_Y_en
#define V5_SIMPLE_MB_CTL                           ((0x007e  << 2) + 0xfe067c00)
// bit[31:28] v5_simple_dq_shift
// bit[27:24] v5_simple_dq_6(-7 to +7)
// bit[23:20] v5_simple_dq_5(-7 to +7)
// bit[19:16] v5_simple_dq_4(-7 to +7)
// bit[15:12] v5_simple_dq_3(-7 to +7)
// bit[11:08] v5_simple_dq_2(-7 to +7)
// bit[07:04] v5_simple_dq_1(-7 to +7)
// bit[03:00] v5_simple_dq_0(-7 to +7)
#define V5_SIMPLE_MB_DQUANT                        ((0x007f  << 2) + 0xfe067c00)
// bit[31:28] v5_simple_me_weight_shift
// bit[27:24] v5_simple_me_weight_6(-7 to +7)
// bit[23:20] v5_simple_me_weight_5(-7 to +7)
// bit[19:16] v5_simple_me_weight_4(-7 to +7)
// bit[15:12] v5_simple_me_weight_3(-7 to +7)
// bit[11:08] v5_simple_me_weight_2(-7 to +7)
// bit[07:04] v5_simple_me_weight_1(-7 to +7)
// bit[03:00] v5_simple_me_weight_0(-7 to +7)
#define V5_SIMPLE_MB_ME_WEIGHT                     ((0x0080  << 2) + 0xfe067c00)
//
// Closing file:  qdct_regs.h
//
// HEVC
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe068000
// -----------------------------------------------
// HEVC2
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe06c000
// -----------------------------------------------
// DOC_TOP
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe06fc00
// -----------------------------------------------
//------------------------------------------------------------------------------
//  DOS top-level register offset
//------------------------------------------------------------------------------
// -----------------------------------------------
// `define DOS_TOP_CBUS_BASE           8'h3f
// -----------------------------------------------
// [0]  reserved
// [1]  Reset Dos top level registers, except sw_reset0/1/2 which is only reset by external reset_n.
// [2]  VDEC1 Reset assist, mdec's CBUS
// [3]  VDEC1 Reset mdec's VLD
// [4]  VDEC1 Reset mdec's VLD
// [5]  VDEC1 Reset mdec's VLD
// [6]  VDEC1 Reset mdec's IQIDCT
// [7]  VDEC1 Reset mdec's MC
// [8]  VDEC1 Reset mdec's DBLK
// [9]  VDEC1 Reset mdec's PIC_DC
// [10] VDEC1 Reset mdec's Pscale
// [11] VDEC1 Reset vcpu's MCPU
// [12] VDEC1 Reset vcpu's CCPU
// [13] VDEC1 Reset mmc_pre_arb
#define DOS_SW_RESET0                              ((0x0000  << 2) + 0xfe06fc00)
// [31:27] reserved
// [26] hcodec clk_en for henc assist
// [25:22] reserved
// [21] hcodec clk_en for henc qdct
// [20] hcodec clk_en for henc vlc
// [19] hcodec clk_en for assist and cbus
// [18] hcodec clk_en for ddr
// [17] hcodec clk_en for vcpu
// [16] hcodec clk_en for hdec assist
// [15] hcodec clk_en for hdec dblk
// [14] reserved
// [13] hcodec clk_en for hdec mc
// [12] hcodec clk_en for hdec pic_dc
// [11:10] reserved
// [9] vdec clk_en for vdec assist and cbus
// [8] vdec clk_en for ddr
// [7] vdec clk_en for vcpu
// [6] vdec clk_en for mdec vld
// [5] vdec clk_en for mdec psc
// [4] vdec clk_en for mdec pic_dc
// [3] vdec clk_en for mdec mc
// [2] vdec clk_en for mdec iqidct
// [1] vdec clk_en for mdec dblk
// [0] vdec clk_en for mdec assist
#define DOS_GCLK_EN0                               ((0x0001  << 2) + 0xfe06fc00)
#define DOS_GEN_CTRL0                              ((0x0002  << 2) + 0xfe06fc00)
#define DOS_APB_ERR_CTRL                           ((0x0003  << 2) + 0xfe06fc00)
#define DOS_APB_ERR_STAT                           ((0x0004  << 2) + 0xfe06fc00)
#define DOS_VDEC_INT_EN                            ((0x0005  << 2) + 0xfe06fc00)
#define DOS_HCODEC_INT_EN                          ((0x0006  << 2) + 0xfe06fc00)
// [0]  Reserved
// [1]  Reserved
// [2]  HCODEC Reset assist, hdec's CBUS
// [3]  HCODEC Reset hdec's VLD
// [4]  HCODEC Reset hdec's VLD
// [5]  HCODEC Reset hdec's VLD
// [6]  HCODEC Reset hdec's IQIDCT
// [7]  HCODEC Reset hdec's MC
// [8]  HCODEC Reset hdec's DBLK
// [9]  HCODEC Reset hdec's PIC_DC
// [10] HCODEC Reset hdec's Pscale
// [11] HCODEC Reset vcpu's MCPU
// [12] HCODEC Reset vcpu's CCPU
// [13] HCODEC Reset mmc_pre_arb
// [14] HCODEC Reserved
// [15] HCODEC Reserved
// [16] HCODEC Reset henc's VLC
#define DOS_SW_RESET1                              ((0x0007  << 2) + 0xfe06fc00)
// [0]  Reserved
// [1]  Reserved
// [2]  VDEC2 Reset assist, mdec's CBUS
// [3]  VDEC2 Reset mdec's VLD
// [4]  VDEC2 Reset mdec's VLD
// [5]  VDEC2 Reset mdec's VLD
// [6]  VDEC2 Reset mdec's IQIDCT
// [7]  VDEC2 Reset mdec's MC
// [8]  VDEC2 Reset mdec's DBLK
// [9]  VDEC2 Reset mdec's PIC_DC
// [10] VDEC2 Reset mdec's Pscale
// [11] VDEC2 Reset vcpu's MCPU
// [12] VDEC2 Reset vcpu's CCPU
// [13] VDEC2 Reset mmc_pre_arb
#define DOS_SW_RESET2                              ((0x0008  << 2) + 0xfe06fc00)
// [31:10] Reserved
// [9] vdec2 clk_en for assist and cbus.
// [8] vdec2 clk_en for ddr;
// [7] vdec2 clk_en for vcpu;
// [6] vdec2 clk_en for mdec vld
// [5] vdec2 clk_en for mdec psc
// [4] vdec2 clk_en for mdec pic_dc
// [3] vdec2 clk_en for mdec mc
// [2] vdec2 clk_en for mdec iqidct
// [1] vdec2 clk_en for mdec dblk
// [0] vdec2 clk_en for mdec assist
#define DOS_GCLK_EN1                               ((0x0009  << 2) + 0xfe06fc00)
#define DOS_VDEC2_INT_EN                           ((0x000a  << 2) + 0xfe06fc00)
#define DOS_VDIN_LCNT                              ((0x000b  << 2) + 0xfe06fc00)
#define DOS_VDIN_FCNT                              ((0x000c  << 2) + 0xfe06fc00)
#define DOS_VDIN_CCTL                              ((0x000d  << 2) + 0xfe06fc00)
#define DOS_SCRATCH0                               ((0x0010  << 2) + 0xfe06fc00)
#define DOS_SCRATCH1                               ((0x0011  << 2) + 0xfe06fc00)
#define DOS_SCRATCH2                               ((0x0012  << 2) + 0xfe06fc00)
#define DOS_SCRATCH3                               ((0x0013  << 2) + 0xfe06fc00)
#define DOS_SCRATCH4                               ((0x0014  << 2) + 0xfe06fc00)
#define DOS_SCRATCH5                               ((0x0015  << 2) + 0xfe06fc00)
#define DOS_SCRATCH6                               ((0x0016  << 2) + 0xfe06fc00)
#define DOS_SCRATCH7                               ((0x0017  << 2) + 0xfe06fc00)
#define DOS_SCRATCH8                               ((0x0018  << 2) + 0xfe06fc00)
#define DOS_SCRATCH9                               ((0x0019  << 2) + 0xfe06fc00)
#define DOS_SCRATCH10                              ((0x001a  << 2) + 0xfe06fc00)
#define DOS_SCRATCH11                              ((0x001b  << 2) + 0xfe06fc00)
#define DOS_SCRATCH12                              ((0x001c  << 2) + 0xfe06fc00)
#define DOS_SCRATCH13                              ((0x001d  << 2) + 0xfe06fc00)
#define DOS_SCRATCH14                              ((0x001e  << 2) + 0xfe06fc00)
#define DOS_SCRATCH15                              ((0x001f  << 2) + 0xfe06fc00)
#define DOS_SCRATCH16                              ((0x0020  << 2) + 0xfe06fc00)
#define DOS_SCRATCH17                              ((0x0021  << 2) + 0xfe06fc00)
#define DOS_SCRATCH18                              ((0x0022  << 2) + 0xfe06fc00)
#define DOS_SCRATCH19                              ((0x0023  << 2) + 0xfe06fc00)
#define DOS_SCRATCH20                              ((0x0024  << 2) + 0xfe06fc00)
#define DOS_SCRATCH21                              ((0x0025  << 2) + 0xfe06fc00)
#define DOS_SCRATCH22                              ((0x0026  << 2) + 0xfe06fc00)
#define DOS_SCRATCH23                              ((0x0027  << 2) + 0xfe06fc00)
#define DOS_SCRATCH24                              ((0x0028  << 2) + 0xfe06fc00)
#define DOS_SCRATCH25                              ((0x0029  << 2) + 0xfe06fc00)
#define DOS_SCRATCH26                              ((0x002a  << 2) + 0xfe06fc00)
#define DOS_SCRATCH27                              ((0x002b  << 2) + 0xfe06fc00)
#define DOS_SCRATCH28                              ((0x002c  << 2) + 0xfe06fc00)
#define DOS_SCRATCH29                              ((0x002d  << 2) + 0xfe06fc00)
#define DOS_SCRATCH30                              ((0x002e  << 2) + 0xfe06fc00)
#define DOS_SCRATCH31                              ((0x002f  << 2) + 0xfe06fc00)
#define DOS_MEM_PD_VDEC                            ((0x0030  << 2) + 0xfe06fc00)
#define DOS_MEM_PD_VDEC2                           ((0x0031  << 2) + 0xfe06fc00)
#define DOS_MEM_PD_HCODEC                          ((0x0032  << 2) + 0xfe06fc00)
#define DOS_MEM_PD_HEVC                            ((0x0033  << 2) + 0xfe06fc00)
#define DOS_SW_RESET3                              ((0x0034  << 2) + 0xfe06fc00)
#define DOS_GCLK_EN3                               ((0x0035  << 2) + 0xfe06fc00)
#define DOS_HEVC_INT_EN                            ((0x0036  << 2) + 0xfe06fc00)
#define DOS_SW_RESET4                              ((0x0037  << 2) + 0xfe06fc00)
#define DOS_GCLK_EN4                               ((0x0038  << 2) + 0xfe06fc00)
#define DOS_MEM_PD_WAVE_0                          ((0x0039  << 2) + 0xfe06fc00)
#define DOS_WAVE_CNTL_STAT                         ((0x003a  << 2) + 0xfe06fc00)
#define DOS_MEM_PD_WAVE_1                          ((0x003b  << 2) + 0xfe06fc00)
#define DOS_MEM_PD_WAVE_2                          ((0x003c  << 2) + 0xfe06fc00)
// bit[31]    mcrcc_stall_en
// bit[30:28] Reserved
// bit[27:20] target_canvas
// bit[19:10] target_mby
// bit[ 9: 0] target_mbx
#define DOS_VDEC_MCRCC_STALL_CTRL                  ((0x0040  << 2) + 0xfe06fc00)
#define DOS_VDEC_MCRCC_STALL2_CTRL                 ((0x0042  << 2) + 0xfe06fc00)
#define DOS_VDEC2_MCRCC_STALL_CTRL                 ((0x0041  << 2) + 0xfe06fc00)
#define DOS_VDEC2_MCRCC_STALL2_CTRL                ((0x0043  << 2) + 0xfe06fc00)
#define DOS_MEM_PD_HEVC2                           ((0x0044  << 2) + 0xfe06fc00)
#define DOS_HEVC2_INT_EN                           ((0x0045  << 2) + 0xfe06fc00)
// bit[1] s_clk off
// bit[0] m_clk off
#define DOS_CFG_CMPIF_BRIDGE                       ((0x0048  << 2) + 0xfe06fc00)
// bit[31]    cfg_stall_en
// bit[30]    co_mv_stall_en
// bit[29]    cfg_stall_force
// bit[28:24] reserved
// bit[23:12] cfg_stall_tilestrt_ctb_y
// bit[11: 0] cfg_stall_tilestrt_ctb_x
#define DOS_HEVC_STALL_START                       ((0x0050  << 2) + 0xfe06fc00)
// bit[31:24] cfg_stall_canvas
// bit[23:12] cfg_stall_tileend_ctb_y
// bit[11: 0] cfg_stall_tileend_ctb_x
#define DOS_HEVC_STALL_END                         ((0x0051  << 2) + 0xfe06fc00)
// bit[31:24] reserved
// bit[23:12] cfg_stall_ctb_y
// bit[11: 0] cfg_stall_ctb_x
#define DOS_HEVC_STALL_POSITION                    ((0x0052  << 2) + 0xfe06fc00)
// bit[31]    cfg_stall_en
// bit[30]    co_mv_stall_en
// bit[29]    cfg_stall_force
// bit[28:24] reserved
// bit[23:12] cfg_stall_tilestrt_ctb_y
// bit[11: 0] cfg_stall_tilestrt_ctb_x
#define DOS_HEVC2_STALL_START                      ((0x0053  << 2) + 0xfe06fc00)
// bit[31:24] cfg_stall_canvas
// bit[23:12] cfg_stall_tileend_ctb_y
// bit[11: 0] cfg_stall_tileend_ctb_x
#define DOS_HEVC2_STALL_END                        ((0x0054  << 2) + 0xfe06fc00)
// bit[31:24] reserved
// bit[23:12] cfg_stall_ctb_y
// bit[11: 0] cfg_stall_ctb_x
#define DOS_HEVC2_STALL_POSITION                   ((0x0055  << 2) + 0xfe06fc00)
#define DOS_SECURE_CTL_INDEX                       ((0x0080  << 2) + 0xfe06fc00)
#define DOS_SECURE_CTL_DATA                        ((0x0081  << 2) + 0xfe06fc00)
#define DOS_SECURE_REG_NULL                        ((0x0082  << 2) + 0xfe06fc00)
// synopsys translate_off
// synopsys translate_on
//
// Closing file:  REG_LIST_DOS_RTL.h
//
//
// Reading file:  gdc_regs.h
//
//===========================================================================
////// GDC Registers
//===========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe008800
// -----------------------------------------------
#define GDC_REG_START                              ((0x0000  << 2) + 0xfe008800)
//
// Closing file:  gdc_regs.h
//
//
// Reading file:  ge2d_regs.h
//
// synopsys translate_off
// synopsys translate_on
//===========================================================================
//GE2D Registers
//===========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe008c00
// -----------------------------------------------
//Bit 31, destination bytemask only if destination bitmask is enable
//Bit 30, destination bitmask enable
//Bit 29, source2 key  enable
//Bit 28, source2 key  mode, 0: mask data when match, 1: mask data when unmatch
//Bit 27, source1 key  enable
//Bit 26, source1 key  mode, 0: mask data when match, 1: mask data when unmatch
//Bit 25:24, dst1 8bit mode component selection,
//            00: select Y(R), 01: Cb(G), 10: Cr(B), 11: Alpha
//Bit 23  dst clip mode, 0: write inside clip window, 1: write outside clip window
//Bit 22:17,  reserved
//Bit 16:15, src2 8bit mode component selection,
//            00: select Y(R), 01: Cb(G), 10: Cr(B), 11: Alpha
//Bit 14     src2 fill mode, 0: repeat data, 1: fill default color
//Bit 13:12  src2 picture struct, 00: frame, 10: even, 11: odd
//Bit 11     src1 x direction yc ration, 0: 1:1, 1: 2:1
//Bit 10     src1 y direction yc ration, 0: 1:1, 1: 2:1
//Bit 9:7    reserved
//Bit 6:5,   src1  8bit mode component selection,
//            00: select Y(R), 01: Cb(G), 10: Cr(B), 11: Alpha
//Bit 4      src1 fill mode, 0: repeat data, 1: fill default color
//Bit 3      src1 lookup table enable
//Bit 2:1    src1 picture struct, 00: frame, 10: even, 11: odd
//Bit 0      src1 separate buffer enable
#define GE2D_GEN_CTRL0                             ((0x00a0  << 2) + 0xfe008c00)
//Bit 31, soft rst
//Bit 30, dst write response counter reset
//Bit 29, disable adding dst write response count to busy bit
//Bit 28:26, reserved
//bit 25:24, interrupt control, if bit[0] true, generate interrupt when one command done,
//                              if bit[1] true, generate interrupt when ge2d change from busy to not busy
//Bit 23:22 src2 burst size control
//Bit 21:16 src1 burst size control, 5:4, yfifo, 3:2, cbfifo, 1:0, crfifo
//          each 2bit, 00: 24 64bitword, 01: 32 64bitword, 10: 48 64bitwords, 11: 64 64bitwords
//Bit 15:14, dst1 picture struct, 00: frame, 10:top, 11: bottom
//Bit 13:12, bit 13 if true, force read src1, bit 12 if true, force read src2
//Bit 11, dst2 request urgent enable
//Bit 10, src1 request urgent enable
//Bit 9,  src2 request urgent enable
//Bit 8,  dst1 request urgent enable
//Bit 7:0 src1 global alpha
#define GE2D_GEN_CTRL1                             ((0x00a1  << 2) + 0xfe008c00)
//Bit31      alpha conversion mode in alu, 0: alpha_out = (alpha !=0) ? alpha +1 : 0;
//                 otherwise, alpha_out = (alpha < 128) ? alpha: alpha + 1;
//Bit30      color conversion mode in alu, 0: color_out = (color != 0) ? color +1: 0;
//                 otherwise, color_out = (color < 128) ? color: color + 1;
//Bit29      src1_gb_alpha_en, As = src1_gb_alpha_en ? Asr * Ag: Asr
//Bit28      dst1_color_round_mode, 0: truncate, 1: + 0.5 rounding
//Bit27      src2_color_expand_mode, 0: add 0, 1: add MSBs
//Bit26      src2_alpha_expand_mode, 0: add 0, 1: add MSBs
//Bit25      src1_color_expand_mode, 0: add 0, 1: add MSBs
//Bit24      src1_alpha_expand_mode, 0: add 0, 1: add MSBs
//Bit 23     if true, dst little endian, otherwise big endian
//Bit 22:19 dst1 color_map
//        dst1_format=0                  : output 8-bit;
//        dst1_format=1, dst1_color_map=1: output 16-bit YCbCr  655;
//        dst1_format=1, dst1_color_map=2: output 16-bit YCbCr  844;
//        dst1_format=1, dst1_color_map=3: output 16-bit YCbCrA 6442;
//        dst1_format=1, dst1_color_map=4: output 16-bit YCbCrA 4444;
//        dst1_format=1, dst1_color_map=5: output 16-bit YCbCr  565;
//        dst1_format=1, dst1_color_map=6: output 16-bit AYCbCr 4444;
//        dst1_format=1, dst1_color_map=7: output 16-bit AYCbCr 1555;
//        dst1_format=1, dst1_color_map=8: output 16-bit YCbCrA 4642;
//        dst1_format=1, dst1_color_map=9: output 16-bit CbCr   88;
//        dst1_format=1, dst1_color_map=10:output 16-bit CrCb   88;
//        dst1_format=2, dst1_color_map=0: output 24-bit YCbCr  888;
//        dst1_format=2, dst1_color_map=1: output 24-bit YCbCrA 5658;
//        dst1_format=2, dst1_color_map=2: output 24-bit AYCbCr 8565;
//        dst1_format=2, dst1_color_map=3: output 24-bit YCbCrA 6666;
//        dst1_format=2, dst1_color_map=4: output 24-bit AYCbCr 6666;
//        dst1_format=2, dst1_color_map=5: output 24-bit CrCbY  888;
//        dst1_format=3, dst1_color_map=0: output 32-bit YCbCrA 8888;
//        dst1_format=3, dst1_color_map=1: output 32-bit AYCbCr 8888;
//        dst1_format=3, dst1_color_map=2: output 32-bit ACrCbY 8888;
//        dst1_format=3, dst1_color_map=3: output 32-bit CrCbYA 8888.
//Bit 17:16 dst1_format,  00: 8bit, 01:16bit, 10:24bit, 11: 32bit
//Bit 15    if true, src2 little endian, otherwise big endian
//Bit 14:11  src2 color_map
//        src2_format=0                 : output 8-bit;
//        src2_format=1, src2_color_map=1: output 16-bit YCbCr  655;
//        src2_format=1, src2_color_map=2: output 16-bit YCbCr  844;
//        src2_format=1, src2_color_map=3: output 16-bit YCbCrA 6442;
//        src2_format=1, src2_color_map=4: output 16-bit YCbCrA 4444;
//        src2_format=1, src2_color_map=5: output 16-bit YCbCr  565;
//        src2_format=1, src2_color_map=6: output 16-bit AYCbCr 4444;
//        src2_format=1, src2_color_map=7: output 16-bit AYCbCr 1555;
//        src2_format=1, src2_color_map=8: output 16-bit YCbCrA 4642;
//        src2_format=2, src2_color_map=0: output 24-bit YCbCr  888;
//        src2_format=2, src2_color_map=1: output 24-bit YCbCrA 5658;
//        src2_format=2, src2_color_map=2: output 24-bit AYCbCr 8565;
//        src2_format=2, src2_color_map=3: output 24-bit YCbCrA 6666;
//        src2_format=2, src2_color_map=4: output 24-bit AYCbCr 6666;
//        src2_format=2, src2_color_map=5: output 24-bit CrCbY  888;
//        src2_format=3, src2_color_map=0: output 32-bit YCbCrA 8888;
//        src2_format=3, src2_color_map=1: output 32-bit AYCbCr 8888;
//        src2_format=3, src2_color_map=2: output 32-bit ACrCbY 8888;
//        src2_format=3, src2_color_map=3: output 32-bit CrCbYA 8888.
//Bit 9:8 src2 format, 00: 8bit, 01:16bit, 10:24bit 11: 32bit
//Bit 7     if true, src1 little endian, otherwise big endian
//Bit 6:3   src1 color_map
//        src1_format=0                 : output 8-bit;
//        src1_format=1, src1_color_map=0: output 4:2:2  (Y0Cb0Y1Cr0);
//        src1_format=1, src1_color_map=1: output 16-bit YCbCr  655;
//        src1_format=1, src1_color_map=2: output 16-bit YCbCr  844;
//        src1_format=1, src1_color_map=3: output 16-bit YCbCrA 6442;
//        src1_format=1, src1_color_map=4: output 16-bit YCbCrA 4444;
//        src1_format=1, src1_color_map=5: output 16-bit YCbCr  565;
//        src1_format=1, src1_color_map=6: output 16-bit AYCbCr 4444;
//        src1_format=1, src1_color_map=7: output 16-bit AYCbCr 1555;
//        src1_format=1, src2_color_map=8: output 16-bit YCbCrA 4642;
//        src1_format=2, src1_color_map=0: output 24-bit YCbCr  888;
//        src1_format=2, src1_color_map=1: output 24-bit YCbCrA 5658;
//        src1_format=2, src1_color_map=2: output 24-bit AYCbCr 8565;
//        src1_format=2, src1_color_map=3: output 24-bit YCbCrA 6666;
//        src1_format=2, src1_color_map=4: output 24-bit AYCbCr 6666;
//        src1_format=2, src1_color_map=5: output 24-bit CrCbY  888;
//        src1_format=2, src1_color_map=14:output 8-bit Y and 16-bit CbCr;
//        src1_format=2, src1_color_map=15:output 8-bit Y and 16-bit CrCb;
//        src1_format=3, src1_color_map=0: output 32-bit YCbCrA 8888;
//        src1_format=3, src1_color_map=1: output 32-bit AYCbCr 8888;
//        src1_format=3, src1_color_map=2: output 32-bit ACrCbY 8888;
//        src1_format=3, src1_color_map=3: output 32-bit CrCbYA 8888.
//Bit 1:0 src1 format, 00: 8bit, 01:16bit/4:2:2, 10:24bit 11: 32bit
#define GE2D_GEN_CTRL2                             ((0x00a2  << 2) + 0xfe008c00)
//Bit 9     if true, all src2 data use default color
//Bit 8     if true, all src1 data use default color
//Bit 7     if true, dst x/y swap
//Bit 6     if true, dst x direction reversely read
//Bit 5     if true, dst y direction reversely read
//Bit 4     if true, src2 x direction reversely read
//Bit 3     if true, src2 y direction reversely read
//Bit 2     if true, src1 x direction reversely read
//Bit 1     if true, src1 y direction reversely read
//Bit 0     cmd write
#define GE2D_CMD_CTRL                              ((0x00a3  << 2) + 0xfe008c00)
//Read only
//Bit 28:17 dst write response counter, for debug only
//Bit 16:7  ge2d_dp status, for debug only
//Bit 6     read src1 cmd ready
//Bit 5     read src2 cmd ready
//Bit 4     pre dpcmd ready
//Bit 3     ge2d dpcmd ready
//Bit 2     ge2d buffer command valid
//Bit 1     ge2d current command valid
//Bit 0     ge2d busy
#define GE2D_STATUS0                               ((0x00a4  << 2) + 0xfe008c00)
//
//Read only
// Bit 29:16 ge2d_dst1_status, for debug only
// Bit    15 ge2d_rd_src2 core.fifo_empty
// Bit    14 ge2d_rd_src2 core.fifo_overflow
// Bit 13:12 ge2d_rd_src2 core.req_st
// Bit    11 ge2d_rd_src2 cmd_if.cmd_err, true if cmd_format=1
// Bit    10 ge2d_rd_src2 cmd_if.cmd_st, 0=IDLE state, 1=BUSY state
// Bit     9 ge2d_rd_src1 luma_core(chroma_core).fifo_empty
// Bit     8 ge2d_rd_src1 luma_core(chroma_core).fifo_overflow
// Bit  7: 6 ge2d_rd_src1 chroma_core.req_st_cr
// Bit  5: 4 ge2d_rd_src1 chroma_core.req_st_cb
// Bit  3: 2 ge2d_rd_src1 luma_core.req_st_y
// Bit     1 ge2d_rd_src1 cmd_if.stat_read_window_err, 1=reading/clipping window setting exceed limit
// Bit     0 ge2d_rd_src1 cmd_if.cmd_st, 0=IDLE state, 1=BUSY state
#define GE2D_STATUS1                               ((0x00a5  << 2) + 0xfe008c00)
//SRC1 default color
//{Y,Cb,Cr,A}/{R,G,B,A}
#define GE2D_SRC1_DEF_COLOR                        ((0x00a6  << 2) + 0xfe008c00)
//Bit 31, SRC1 clip x start extra, if true, one more data is read for chroma
//Bit 28:16, SRC1 clip x start
//Bit 15, SRC1 clip x end extra, if true, one more data is read for chroma
//Bit 12:0, SRC1 clip x end
#define GE2D_SRC1_CLIPX_START_END                  ((0x00a7  << 2) + 0xfe008c00)
//Bit 31, SRC1 clip y start extra, if true, one more data is read for chroma
//Bit 28:16, SRC1 clip y start
//Bit 15, SRC1 clip y end extra, if true, one more data is read for chroma
//Bit 12:0, SRC1 clip y end
#define GE2D_SRC1_CLIPY_START_END                  ((0x00a8  << 2) + 0xfe008c00)
//Bit 31:24, SRC1 canvas address0
//Bit 23:16, SRC1 canvas address1
//Bit 15:8, SRC1 canvas address2
#define GE2D_SRC1_CANVAS                           ((0x00a9  << 2) + 0xfe008c00)
//Bit 31, SRC1 x start extra bit1, if true, one more chroma data is read for x even start chroma data when y/c ratio = 2
//             or x even/odd start chroma extra data when y/c ratio = 1
//Bit 30, SRC1 x start extra bit0, if true, one more chroma data is read for x odd start chroma data when y/c ratio = 2
//Bit 29:16, SRC1 x start, signed data
//Bit 15, SRC1 x end extra bit1, if true, one more chroma data is read for x odd end chroma data when y/c ratio = 2
//             or x even/odd end chroma extra data when y/c ratio = 1
//Bit 14, SRC1 x end extra bit0, if true, one more chroma data is read for x even end chroma data when y/c ratio = 2
//Bit 13:0, SRC1 x end, signed data
#define GE2D_SRC1_X_START_END                      ((0x00aa  << 2) + 0xfe008c00)
//Bit 31, SRC1 y start extra, if true, one more chroma data is read for y even start chroma data when y/c ratio = 2
//             or y even/odd start chroma extra data when y/c ratio = 1
//Bit 30, SRC1 y start extra, if true, one more chroma data is read for x odd start chroma data when y/c ratio = 2
//Bit 28:16, SRC1 y start
//Bit 15, SRC1 y end extra bit1, if true, one more chroma data is read for y odd end chroma data when y/c ratio = 2
//             or y even/odd end chroma extra data when y/c ratio = 1
//Bit 14, SRC1 y end extra bit0, if true, one more chroma data is read for y even end chroma data when y/c ratio = 2
//Bit 12:0, SRC1 y end
#define GE2D_SRC1_Y_START_END                      ((0x00ab  << 2) + 0xfe008c00)
// Bit 31: 9 Reserved
// Bit     8 RW, 0 = Write LUT, 1 = Read LUT
// Bit  7: 0 RW, lut_addr
#define GE2D_SRC1_LUT_ADDR                         ((0x00ac  << 2) + 0xfe008c00)
// Bit 31:24 RW, Y or R
// Bit 23:16 RW, Cb or G
// Bit 15: 8 RW, Cr or B
// Bit  7: 0 RW, Alpha
#define GE2D_SRC1_LUT_DAT                          ((0x00ad  << 2) + 0xfe008c00)
//Bit 19, if true, horizontal formatter using repeat to get the pixel, otherwise using interpolation
//Bit 18, horizontal formatter en
//Bit 17, if true, vertical formatter using repeat to get the pixel, otherwise using interpolation
//Bit 16, vertical formatter en
//Bit 15:8 X direction chroma phase,
//          [7:4] for x direction even start/end chroma phase when y/c ratio = 2
//                or start/end even/odd chroma phase  when y/c ratio = 1
//          [3:0] for x direction odd start/end chroma phase only when y/c ration = 2
//Bit 7:0  Y direction chroma phase,
//          [7:4] for y direction even start/end chroma phase when y/c ratio = 2
//          or start/end even/odd chroma phase  when y/c ratio = 1
//          [3:0] for y direction odd start/end chroma phase only when y/c ration = 2
#define GE2D_SRC1_FMT_CTRL                         ((0x00ae  << 2) + 0xfe008c00)
//SRC2 default color
//{Y,Cb,Cr,A}/{R,G,B,A}
#define GE2D_SRC2_DEF_COLOR                        ((0x00af  << 2) + 0xfe008c00)
//Bit 28:16, SRC2 clip x start
//Bit 12:0, SRC2 clip x end
#define GE2D_SRC2_CLIPX_START_END                  ((0x00b0  << 2) + 0xfe008c00)
//Bit 28:16, SRC2 clip y start
//Bit 12:0, SRC2 clip y end
#define GE2D_SRC2_CLIPY_START_END                  ((0x00b1  << 2) + 0xfe008c00)
//Bit 28:16, SRC2 x start
//Bit 12:0, SRC2 x end
#define GE2D_SRC2_X_START_END                      ((0x00b2  << 2) + 0xfe008c00)
//Bit 28:16, SRC2 y start
//Bit 12:0, SRC2 y end
#define GE2D_SRC2_Y_START_END                      ((0x00b3  << 2) + 0xfe008c00)
//Bit 28:16, DST clip x start
//Bit 12:0, DST clip x end
#define GE2D_DST_CLIPX_START_END                   ((0x00b4  << 2) + 0xfe008c00)
//
//Bit 28:16, DST clip y start
//Bit 12:0, DST clip y end
#define GE2D_DST_CLIPY_START_END                   ((0x00b5  << 2) + 0xfe008c00)
//Bit 28:16, DST x start
//Bit 12:0, DST x end
#define GE2D_DST_X_START_END                       ((0x00b6  << 2) + 0xfe008c00)
//
//Bit 28:16, DST x start
//Bit 12:0, DST x end
#define GE2D_DST_Y_START_END                       ((0x00b7  << 2) + 0xfe008c00)
//Bit 23:16 DST2 canvas address
//Bit 15:8 SRC2 canvas address
//Bit 7:0 DST1 canvas address
#define GE2D_SRC2_DST_CANVAS                       ((0x00b8  << 2) + 0xfe008c00)
//vertical scaler phase step
//Bit 28:0,  5.24 format
#define GE2D_VSC_START_PHASE_STEP                  ((0x00b9  << 2) + 0xfe008c00)
//phase slope
//Bit 24:0, bit 24 signed bit
#define GE2D_VSC_PHASE_SLOPE                       ((0x00ba  << 2) + 0xfe008c00)
//Bit 30:29, vertical repeat line0 number
//Bit 23:0, vertical scaler initial phase
#define GE2D_VSC_INI_CTRL                          ((0x00bb  << 2) + 0xfe008c00)
//horizontal scaler phase step
//Bit 28:0,  5.24 format
#define GE2D_HSC_START_PHASE_STEP                  ((0x00bc  << 2) + 0xfe008c00)
//phase slope
//Bit 24:0, bit 24 signed bit
#define GE2D_HSC_PHASE_SLOPE                       ((0x00bd  << 2) + 0xfe008c00)
//Bit 30:29, horizontal repeat line0 number
//Bit 23:0, horizontal scaler initial phase
#define GE2D_HSC_INI_CTRL                          ((0x00be  << 2) + 0xfe008c00)
//Bit 31:24, advance number in this round, if horizontal scaler is working on dividing mode
//Bit 23:0, horizontal scaler advance phase in this round, if horizontal scaler is working on dividing mode
#define GE2D_HSC_ADV_CTRL                          ((0x00bf  << 2) + 0xfe008c00)
//Bit 30, vertical nearest mode enable, must set vt_bank_length = 4
//Bit 29, horizontal nearest mode enable, must set hz_bank_length = 4
//Bit 28, horizontal scaler dividing mode enable
//Bit 27:15, horizontal dividing length, if bit 28 is enable
//Bit 14, pre horizontal scaler enable
//Bit 13, pre vertical scale enable
//Bit 12, vertical scale enable
//Bit 11, horizontal scaler enable
//Bit 9, if true, treat horizontal repeat line number(GE2D_HSC_INI_CTRL bit 30:29) as repeating line,
//        otherwise using treat horizontal repeat line number as minus line number.
//Bit 8, if true, treat vertical repeat line number(GE2D_VSC_INI_CTRL bit 30:29) as repeating line,
//        otherwise using treat vertical repeat line number as minus line number.
//Bit 7, if true, always use phase0 in vertical scaler
//Bit 6:4, vertical scaler bank length
//Bit 3, if true, always use phase0 in horizontal scaler
//Bit 2:0, horizontal scaler bank length
#define GE2D_SC_MISC_CTRL                          ((0x00c0  << 2) + 0xfe008c00)
//Read only
//vertical scaler next round integer pixel pointer, signed data
//Bit 13:0
#define GE2D_VSC_NRND_POINT                        ((0x00c1  << 2) + 0xfe008c00)
//Read only
//vertical scaler next round phase
//bit 23:0
#define GE2D_VSC_NRND_PHASE                        ((0x00c2  << 2) + 0xfe008c00)
//Read only
//horizontal scaler next round integer pixel pointer, signed data
//Bit 13:0
#define GE2D_HSC_NRND_POINT                        ((0x00c3  << 2) + 0xfe008c00)
//Read only
//horizontal scaler next round phase
//bit 23:0
#define GE2D_HSC_NRND_PHASE                        ((0x00c4  << 2) + 0xfe008c00)
//
//Bit 28:20, pre_offset0
//Bit 18:10, pre_offset1
//Bit 8:0,   pre_offset2
#define GE2D_MATRIX_PRE_OFFSET                     ((0x00c5  << 2) + 0xfe008c00)
//Bit 28:16 coef00
//Bit 12:0  coef01
#define GE2D_MATRIX_COEF00_01                      ((0x00c6  << 2) + 0xfe008c00)
//Bit 28:16 coef02
//Bit 12:0  coef10
#define GE2D_MATRIX_COEF02_10                      ((0x00c7  << 2) + 0xfe008c00)
//Bit 28:16 coef11
//Bit 12:0  coef12
#define GE2D_MATRIX_COEF11_12                      ((0x00c8  << 2) + 0xfe008c00)
//Bit 28:16 coef20
//Bit 12:0  coef21
#define GE2D_MATRIX_COEF20_21                      ((0x00c9  << 2) + 0xfe008c00)
//Bit 28:16 coef22
//Bit 7    input y/cb/cr saturation enable
//Bit 0    conversion matrix enable
#define GE2D_MATRIX_COEF22_CTRL                    ((0x00ca  << 2) + 0xfe008c00)
//Bit 28:20, offset0
//Bit 18:10, offset1
//Bit 8:0,   offset2
#define GE2D_MATRIX_OFFSET                         ((0x00cb  << 2) + 0xfe008c00)
//Bit 26:25, SRC1 color multiplier alpha selection
//           if 00, Cs = Csr
//           if 01, Cs = Csr * Asr * Ag (if source is not premultiplied)
//           if 10, Cs = Csr * Ag (if source is premultiplied)
//Bit 24    SRC2 color multiplier alpha selection
//          if 0, no multiplier, Cd = Cdr,  otherwise, Cd = Cdr * Ad.
//Bit 22:12 ALU color operation
//          bit10:8 Blending Mode Parameter
//            3'b000: ADD               Cs*Fs + Cd*Fd
//            3'b001: SUBTRACT          Cs*Fs - Cd*Fd
//            3'b010: REVERSE SUBTRACT  Cd*Fd - Cs*Fs
//            3'b011: MIN               min(Cs*Fs, Cd*Fd)
//            3'b100: MAX               max(Cs*Fs, Cd*Fd)
//            3'b101: LOGIC OP          Cs op Cd
//          bit7:4 Source Color Blending Factor CFs
//            4'b0000: ZERO                        0
//            4'b0001: ONE                         1
//            4'b0010: SRC_COLOR                   Cs(RGBs)
//            4'b0011: ONE_MINUS_SRC_COLOR         1 - Cs(RGBs)
//            4'b0100: DST_COLOR                   Cd(RGBd)
//            4'b0101: ONE_MINUS_DST_COLOR         1 - Cd(RGBd)
//            4'b0110: SRC_ALPHA                   As
//            4'b0111: ONE_MINUS_SRC_ALPHA         1 - As
//            4'b1000: DST_ALPHA                   Ad
//            4'b1001: ONE_MINUS_DST_ALPHA         1 - Ad
//            4'b1010: CONST_COLOR                 Cc(RGBc)
//            4'b1011: ONE_MINUS_CONST_COLOR       1 - Cc(RGBc)
//            4'b1100: CONST_ALPHA                 Ac
//            4'b1101: ONE_MINUS_CONST_ALPHA       1 - Ac
//            4'b1110: SRC_ALPHA_SATURATE          min(As,1-Ad)
//          bit3:0 dest Color Blending Factor CFd, when bit10:8 != LOGIC OP
//            4'b0000: ZERO                        0
//            4'b0001: ONE                         1
//            4'b0010: SRC_COLOR                   Cs(RGBs)
//            4'b0011: ONE_MINUS_SRC_COLOR         1 - Cs(RGBs)
//            4'b0100: DST_COLOR                   Cd(RGBd)
//            4'b0101: ONE_MINUS_DST_COLOR         1 - Cd(RGBd)
//            4'b0110: SRC_ALPHA                   As
//            4'b0111: ONE_MINUS_SRC_ALPHA         1 - As
//            4'b1000: DST_ALPHA                   Ad
//            4'b1001: ONE_MINUS_DST_ALPHA         1 - Ad
//            4'b1010: CONST_COLOR                 Cc(RGBc)
//            4'b1011: ONE_MINUS_CONST_COLOR       1 - Cc(RGBc)
//            4'b1100: CONST_ALPHA                 Ac
//            4'b1101: ONE_MINUS_CONST_ALPHA       1 - Ac
//            4'b1110: SRC_ALPHA_SATURATE          min(As,1-Ad)
//          bit3:0 logic operations, when bit10:8 == LOGIC OP
//            4'b0000: CLEAR                       0
//            4'b0001: COPY                        s
//            4'b0010: NOOP                        d
//            4'b0011: SET                         1
//            4'b0100: COPY_INVERT                 ~s
//            4'b0101: INVERT                      ~d
//            4'b0110: AND_REVERSE                 s & ~d
//            4'b0111: OR_REVERSE                  s | ~d
//            4'b1000: AND                         s & d
//            4'b1001: OR                          s | d
//            4'b1010: NAND                        ~(s & d)
//            4'b1011: NOR                         ~(s | d)
//            4'b1100: XOR                         s ^ d
//            4'b1101: EQUIV                       ~(s ^ d)
//            4'b1110: AND_INVERTED                ~s & d
//            4'b1111: OR_INVERTED                 ~s | d
//Bit 10:0  ALU alpha operation
//            bit10:8 Blending Equation Math Operation
//              3'b000: ADD               As*Fs + Ad*Fd
//              3'b001: SUBTRACT          As*Fs - Ad*Fd
//              3'b010: REVERSE SUBTRACT  Ad*Fd - As*Fs
//              3'b011: MIN               min(As*Fs, Ad*Fd)
//              3'b100: MAX               max(As*Fs, Ad*Fd)
//              3'b101: LOGIC OP          As op Ad
//            bit7:4 Source alpha Blending Factor AFs
//              4'b0000                       0
//              4'b0001                       1
//              4'b0010                       As
//              4'b0011                       1 - As
//              4'b0100                       Ad
//              4'b0101                       1 - Ad
//              4'b0110                       Ac
//              4'b0111                       1 - Ac
//               ....                         reserved
//            bit3:0 Destination alpha Blending Factor AFd, when bit10:8 != LOGIC OP
//              4'b0000                       0
//              4'b0001                       1
//              4'b0010                       As
//              4'b0011                       1 - As
//              4'b0100                       Ad
//              4'b0101                       1 - Ad
//              4'b0110                       Ac
//              4'b0111                       1 - Ac
//               ....                         reserved
//            bit3:0 logic operations, when bit10:8 == LOGIC OP
//              4'b0000: CLEAR                       0
//              4'b0001: COPY                        s
//              4'b0010: NOOP                        d
//              4'b0011: SET                         1
//              4'b0100: COPY_INVERT                 ~s
//              4'b0101: INVERT                      ~d
//              4'b0110: AND_REVERSE                 s & ~d
//              4'b0111: OR_REVERSE                  s | ~d
//              4'b1000: AND                         s & d
//              4'b1001: OR                          s | d
//              4'b1010: NAND                        ~(s & d)
//              4'b1011: NOR                         ~(s | d)
//              4'b1100: XOR                         s ^ d
//              4'b1101: EQUIV                       ~(s ^ d)
//              4'b1110: AND_INVERTED                ~s & d
//              4'b1111: OR_INVERTED                 ~s | d
#define GE2D_ALU_OP_CTRL                           ((0x00cc  << 2) + 0xfe008c00)
//bit 31:0 (RGBA,YCBCRA)
#define GE2D_ALU_CONST_COLOR                       ((0x00cd  << 2) + 0xfe008c00)
//SRC1 Key
//31:0
#define GE2D_SRC1_KEY                              ((0x00ce  << 2) + 0xfe008c00)
//SRC1 Key Mask
//31:0
#define GE2D_SRC1_KEY_MASK                         ((0x00cf  << 2) + 0xfe008c00)
//SRC2 Key
//31:0
#define GE2D_SRC2_KEY                              ((0x00d0  << 2) + 0xfe008c00)
//SRC2 Key Mask
//31:0
#define GE2D_SRC2_KEY_MASK                         ((0x00d1  << 2) + 0xfe008c00)
//Destination Bit Mask
//31:0
#define GE2D_DST_BITMASK                           ((0x00d2  << 2) + 0xfe008c00)
//Bit 31    DP onoff mode, 0: on_counter means how many pixels will output before ge2d turns off
//                         1: on_counter means how many clocks will ge2d turn on before ge2d turns off
//Bit 30:16     DP on counter
//Bit 15        0: vd_format doesnt have onoff mode, 1: vd format has onoff mode
//Bit 14:0      DP off counter
#define GE2D_DP_ONOFF_CTRL                         ((0x00d3  << 2) + 0xfe008c00)
//Because there are many coefficients used in the vertical filter and horizontal filters,
//indirect access the coefficients of vertical filter and horizontal filter is used.
//For vertical filter, there are 33x4 coefficients
//For horizontal filter, there are 33x4 coefficients
//Bit 15	index increment, if bit9 == 1  then (0: index increase 1, 1: index increase 2) else (index increase 2)
//Bit 14	1: read coef through cbus enable, just for debug purpose in case when we wanna check the coef in ram in correct or not
//Bit 9     if true, use 9bit resolution coef, other use 8bit resolution coef
//Bit 8	    type of index, 0: vertical coef
//						   1: horizontal coef
//Bit 6:0 	coef index
#define GE2D_SCALE_COEF_IDX                        ((0x00d4  << 2) + 0xfe008c00)
//coefficients for vertical filter and horizontal filter
#define GE2D_SCALE_COEF                            ((0x00d5  << 2) + 0xfe008c00)
//Bit 24    src2 alpha fill mode: together with GE2D_GEN_CTRL0[4](fill_mode), define what alpha values are used
//                                for the area outside the clipping window. As below:
//                                fill_mode=0, alpha_fill_mode=0 : use inner alpha, (or default_alpha if src data have no alpha values);
//                                fill_mode=0, alpha_fill_mode=1 : use outside_alpha;
//                                fill_mode=1, alpha_fill_mode=0 : use default_alpha;
//                                fill_mode=1, alpha_fill_mode=1 : use outside_alpha.
//Bit 23:16 src2 outside alpha
//Bit 8     src1 alpha fill mode, refer to src2 alpha fill mode above.
//Bit 7:0   src1 outside alpha
#define GE2D_SRC_OUTSIDE_ALPHA                     ((0x00d6  << 2) + 0xfe008c00)
//Bit 31       antiflick enable
//Bit 24       1: alpha value for the first line use repeated alpha, 0: use bit 23:16 as the first line alpha
//Bit 23:16     register value for the first line alpha when bit 24 is 1
//Bit 8        1: alpha value for the last line use repeated alpha, 0: use bit 7:0 as the last line alpha
//Bit 7:0      register value for the last line alpha when bit 8 is 1
#define GE2D_ANTIFLICK_CTRL0                       ((0x00d8  << 2) + 0xfe008c00)
//Bit 25,    rgb_sel, 1: antiflick RGBA, 0: antiflick YCbCrA
//Bit 24,    cbcr_en, 1: also filter cbcr in case of antiflicking YCbCrA, 0: no filter on cbcr in case of antiflicking YCbCrA
//Bit 23:16, R mult coef for converting RGB to Y
//Bit 15:8,  G mult coef for converting RGB to Y
//Bit 7:0,   B mult coef for converting RGB to Y
//Y = (R * y_r + G * y_g + B * y_b) / 256
#define GE2D_ANTIFLICK_CTRL1                       ((0x00d9  << 2) + 0xfe008c00)
//Bit 31:24, Y threshold1, when   0<Y<=th1, use filter0;
//Bit 23:16, color antiflick filter0 n3
//Bit 15:8,  color antiflick filter0 n2
//Bit 7:0,   color antiflick filter0 n1
//Y = (line_up * n1 + line_center * n2 + line_dn * n3) / 128
#define GE2D_ANTIFLICK_COLOR_FILT0                 ((0x00da  << 2) + 0xfe008c00)
//Bit 31:24, Y threshold2, when th1<Y<=th2, use filter1;
//Bit 23:16, color antiflick filter1 n3
//Bit 15:8,  color antiflick filter1 n2
//Bit 7:0,   color antiflick filter1 n1
#define GE2D_ANTIFLICK_COLOR_FILT1                 ((0x00db  << 2) + 0xfe008c00)
//Bit 31:24, Y threshold3, when th2<Y<=th3, use filter2; Y>th3, use filter3
//Bit 23:16, color antiflick filter2 n3
//Bit 15:8,  color antiflick filter2 n2
//Bit 7:0,   color antiflick filter2 n1
#define GE2D_ANTIFLICK_COLOR_FILT2                 ((0x00dc  << 2) + 0xfe008c00)
//Bit 23:16, color antiflick filter3 n3
//Bit 15:8,  color antiflick filter3 n2
//Bit 7:0,   color antiflick filter3 n1
#define GE2D_ANTIFLICK_COLOR_FILT3                 ((0x00dd  << 2) + 0xfe008c00)
//Bit 31:24, Alpha threshold1, when   0<Alpha<=th1, use filter0;
//Bit 23:16, Alpha antiflick filter0 n3
//Bit 15:8,  Alpha antiflick filter0 n2
//Bit 7:0,   Alpha antiflick filter0 n1
//Alpha = (line_up * n1 + line_center * n2 + line_dn * n3) / 128
#define GE2D_ANTIFLICK_ALPHA_FILT0                 ((0x00de  << 2) + 0xfe008c00)
//Bit 31:24, Alpha threshold2, when th1<Alpha<=th2, use filter1;
//Bit 23:16, Alpha antiflick filter1 n3
//Bit 15:8,  Alpha antiflick filter1 n2
//Bit 7:0,   Alpha antiflick filter1 n1
#define GE2D_ANTIFLICK_ALPHA_FILT1                 ((0x00df  << 2) + 0xfe008c00)
//Bit 31:24, Alpha threshold3, when th2<Alpha<=th3, use filter2; Alpha>th3, use filter3
//Bit 23:16, Alpha antiflick filter2 n3
//Bit 15:8,  Alpha antiflick filter2 n2
//Bit 7:0,   Alpha antiflick filter2 n1
#define GE2D_ANTIFLICK_ALPHA_FILT2                 ((0x00e0  << 2) + 0xfe008c00)
//Bit 23:16, Alpha antiflick filter3 n3
//Bit 15:8,  Alpha antiflick filter3 n2
//Bit 7:0,   Alpha antiflick filter3 n1
#define GE2D_ANTIFLICK_ALPHA_FILT3                 ((0x00e1  << 2) + 0xfe008c00)
//dout = clipto_0_255(((din + din_offset) * map_coef + ((1 << (map_sr - 1))) >> map_sr + dout_offset)
//Bit 30:22 din_offset (signed data)
//Bit 21:14 map_coef (unsigned data)
//Bit 13:10 map_sr (unsigned data)
//Bit 9:1   dout_offset (signed data)
//Bit 0     enable
#define GE2D_SRC1_RANGE_MAP_Y_CTRL                 ((0x00e3  << 2) + 0xfe008c00)
//dout = clipto_0_255(((din + din_offset) * map_coef + ((1 << (map_sr - 1))) >> map_sr + dout_offset)
//Bit 30:22 din_offset (signed data)
//Bit 21:14 map_coef (unsigned data)
//Bit 13:10 map_sr (unsigned data)
//Bit 9:1   dout_offset (signed data)
//Bit 0     enable
#define GE2D_SRC1_RANGE_MAP_CB_CTRL                ((0x00e4  << 2) + 0xfe008c00)
//dout = clipto_0_255(((din + din_offset) * map_coef + ((1 << (map_sr - 1))) >> map_sr + dout_offset)
//Bit 30:22 din_offset (signed data)
//Bit 21:14 map_coef (unsigned data)
//Bit 13:10 map_sr (unsigned data)
//Bit 9:1   dout_offset (signed data)
//Bit 0     enable
#define GE2D_SRC1_RANGE_MAP_CR_CTRL                ((0x00e5  << 2) + 0xfe008c00)
//Bit 21:16     src1 pre arbiter burst number
//Bit 13:8      src2 pre arbiter burst number
//Bit 5:0       dst pre arbiter burst number
#define GE2D_ARB_BURST_NUM                         ((0x00e6  << 2) + 0xfe008c00)
//each 6bit ID, high 4bit are thread ID, low 2bits are the token
//Bit 21:16 src1 ID
//Bit 13:8 src2 ID
//Bit 5:0  dst ID
#define GE2D_TID_TOKEN                             ((0x00e7  << 2) + 0xfe008c00)
//Bit 31:28 dst2_bytemask_val. 1-bit mask for each byte (8-bit). Applicable only if both dst_bitmask_en=1 and dst_bytemask_only=1.
//Bit 27:26, dst2 picture struct, 00: frame, 10:top, 11: bottom
//Bit 25:24, dst2 8bit mode component selection,
//            00: select Y(R), 01: Cb(G), 10: Cr(B), 11: Alpha
//Bit 22:19 dst2 color_map
//        dst2_format=0                  : output 8-bit;
//        dst2_format=1, dst2_color_map=1: output 16-bit YCbCr  655;
//        dst2_format=1, dst2_color_map=2: output 16-bit YCbCr  844;
//        dst2_format=1, dst2_color_map=3: output 16-bit YCbCrA 6442;
//        dst2_format=1, dst2_color_map=4: output 16-bit YCbCrA 4444;
//        dst2_format=1, dst2_color_map=5: output 16-bit YCbCr  565;
//        dst2_format=1, dst2_color_map=6: output 16-bit AYCbCr 4444;
//        dst2_format=1, dst2_color_map=7: output 16-bit AYCbCr 1555;
//        dst2_format=1, dst2_color_map=8: output 16-bit YCbCrA 4642;
//        dst2_format=1, dst2_color_map=9: output 16-bit CbCr   88;
//        dst2_format=1, dst2_color_map=10:output 16-bit CrCb   88;
//        dst2_format=2, dst2_color_map=0: output 24-bit YCbCr  888;
//        dst2_format=2, dst2_color_map=1: output 24-bit YCbCrA 5658;
//        dst2_format=2, dst2_color_map=2: output 24-bit AYCbCr 8565;
//        dst2_format=2, dst2_color_map=3: output 24-bit YCbCrA 6666;
//        dst2_format=2, dst2_color_map=4: output 24-bit AYCbCr 6666;
//        dst2_format=2, dst2_color_map=5: output 24-bit CrCbY  888;
//        dst2_format=3, dst2_color_map=0: output 32-bit YCbCrA 8888;
//        dst2_format=3, dst2_color_map=1: output 32-bit AYCbCr 8888;
//        dst2_format=3, dst2_color_map=2: output 32-bit ACrCbY 8888;
//        dst2_format=3, dst2_color_map=3: output 32-bit CrCbYA 8888.
//Bit 17:16 dst2_format,  00: 8bit, 01:16bit, 10:24bit, 11: 32bit
//Bit 15     reserved
//Bit 14     dst2_color_round_mode, 0: truncate, 1: + 0.5 rounding
//Bit 13:12, dst2_x_discard_mode. 00: no discard; 10=discard even x; 11=discard odd x. Note: x is post reverse/rotation.
//Bit 11:10, dst2_y_discard_mode. 00: no discard; 10=discard even y; 11=discard odd y. Note: y is post reverse/rotation.
//Bit     9 reserved
//Bit     8, dst2_enable. 0: disable dst2 (default); 1=enable dst2.
//Bit  7: 6 reserved
//Bit  5: 4, dst1_x_discard_mode. 00: no discard; 10=discard even x; 11=discard odd x. Note: x is post reverse/rotation.
//Bit  3: 2, dst1_y_discard_mode. 00: no discard; 10=discard even y; 11=discard odd y. Note: y is post reverse/rotation.
//Bit     1 reserved
//Bit     0, dst1_enable. 0: disable dst1; 1=enable dst1 (default).
#define GE2D_GEN_CTRL3                             ((0x00e8  << 2) + 0xfe008c00)
//Read only
// Bit 13:0 ge2d_dst2_status, for debug only
#define GE2D_STATUS2                               ((0x00e9  << 2) + 0xfe008c00)
//Bit 27:26  src1 Y fifo size control, 00: 512, 01: 256, 10: 128 11: 96
//Bit 25:24  src2 fifo size control, 00: 512, 01: 256, 10: 128 11: 96
//Bit 23:22  dst1 fifo size control, 00: 512, 01: 256, 10: 128 11: 64
//Bit 21:20  dst2 fifo size control, 00: 512, 01: 256, 10: 128 11: 64
//Bit 19:18, dst1 fifo burst control, 00: 24x64, 01: 32x64, 10: 48x64, 11:64x64
//Bit 17:16, dst2 fifo burst control, 00: 24x64, 01: 32x64, 10: 48x64, 11:64x64
//Bit 15:1, top_wrap_ctrl
//bit 0, if true, disable bug fix about the dp_out_done/scale_out_done(test1823) hang issue when scaling down ratio is high.
#define GE2D_GEN_CTRL4                             ((0x00ea  << 2) + 0xfe008c00)
#define GE2D_GCLK_CTRL0                            ((0x00ef  << 2) + 0xfe008c00)
#define GE2D_GCLK_CTRL1                            ((0x00f0  << 2) + 0xfe008c00)
#define GE2D_GEN_CTRL5                             ((0x00f1  << 2) + 0xfe008c00)
#define GE2D_DST1_BADDR_CTRL                       ((0x00f2  << 2) + 0xfe008c00)
//Bit 31:0, dst1 base address in 64bits
#define GE2D_DST1_STRIDE_CTRL                      ((0x00f3  << 2) + 0xfe008c00)
//Bit 19:0, dst1 stride size in 64bits
#define GE2D_DST2_BADDR_CTRL                       ((0x00f4  << 2) + 0xfe008c00)
//Bit 31:0, dst1 base address in 64bits
#define GE2D_DST2_STRIDE_CTRL                      ((0x00f5  << 2) + 0xfe008c00)
//Bit 19:0, dst1 stride size in 64bits
#define GE2D_SRC1_BADDR_CTRL_Y                     ((0x00f6  << 2) + 0xfe008c00)
//Bit 31:0, src1 base address in 64bits
#define GE2D_SRC1_STRIDE_CTRL_Y                    ((0x00f7  << 2) + 0xfe008c00)
//Bit 19:0, src1 stride size in 64bits
#define GE2D_SRC1_BADDR_CTRL_CB                    ((0x00f8  << 2) + 0xfe008c00)
//Bit 31:0, src1 base address in 64bits
#define GE2D_SRC1_STRIDE_CTRL_CB                   ((0x00f9  << 2) + 0xfe008c00)
//Bit 19:0, src1 stride size in 64bits
#define GE2D_SRC1_BADDR_CTRL_CR                    ((0x00fa  << 2) + 0xfe008c00)
//Bit 31:0, src1 base address in 64bits
#define GE2D_SRC1_STRIDE_CTRL_CR                   ((0x00fb  << 2) + 0xfe008c00)
//Bit 19:0, src1 stride size in 64bits
#define GE2D_SRC2_BADDR_CTRL                       ((0x00fc  << 2) + 0xfe008c00)
//Bit 31:0, src2 base address in 64bits
#define GE2D_SRC2_STRIDE_CTRL                      ((0x00fd  << 2) + 0xfe008c00)
//Bit 19:0, src2 stride size in 64bits
// synopsys translate_off
// synopsys translate_on
//
// Closing file:  ge2d_regs.h
//
//
// Reading file:  mipi_isp.h
//
//========================================================================
// MIPI_ISP
//========================================================================
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe023000
// -----------------------------------------------
#define MIPI_ADAPT_DDR_RD0_CNTL0                   ((0x0000  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD0_CNTL1                   ((0x0001  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD0_CNTL2                   ((0x0002  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD0_CNTL3                   ((0x0003  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD0_CNTL4                   ((0x0004  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD0_ST0                     ((0x0005  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD0_ST1                     ((0x0006  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD0_ST2                     ((0x0007  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD0_CNTL5                   ((0x0008  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD0_CNTL6                   ((0x0009  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD1_CNTL0                   ((0x0010  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD1_CNTL1                   ((0x0011  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD1_CNTL2                   ((0x0012  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD1_CNTL3                   ((0x0013  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD1_CNTL4                   ((0x0014  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD1_ST0                     ((0x0015  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD1_ST1                     ((0x0016  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD1_ST2                     ((0x0017  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD1_CNTL5                   ((0x0018  << 2) + 0xfe023000)
#define MIPI_ADAPT_DDR_RD1_CNTL6                   ((0x0019  << 2) + 0xfe023000)
#define MIPI_ADAPT_PIXEL0_CNTL0                    ((0x0020  << 2) + 0xfe023000)
#define MIPI_ADAPT_PIXEL0_CNTL1                    ((0x0021  << 2) + 0xfe023000)
#define MIPI_ADAPT_PIXEL1_CNTL0                    ((0x0022  << 2) + 0xfe023000)
#define MIPI_ADAPT_PIXEL1_CNTL1                    ((0x0023  << 2) + 0xfe023000)
#define MIPI_ADAPT_PIXEL0_ST0                      ((0x002a  << 2) + 0xfe023000)
#define MIPI_ADAPT_PIXEL0_ST1                      ((0x002b  << 2) + 0xfe023000)
#define MIPI_ADAPT_PIXEL1_ST0                      ((0x002c  << 2) + 0xfe023000)
#define MIPI_ADAPT_PIXEL1_ST1                      ((0x002d  << 2) + 0xfe023000)
#define MIPI_ADAPT_ALIG_CNTL0                      ((0x0030  << 2) + 0xfe023000)
#define MIPI_ADAPT_ALIG_CNTL1                      ((0x0031  << 2) + 0xfe023000)
#define MIPI_ADAPT_ALIG_CNTL2                      ((0x0032  << 2) + 0xfe023000)
#define MIPI_ADAPT_ALIG_CNTL3                      ((0x0033  << 2) + 0xfe023000)
#define MIPI_ADAPT_ALIG_CNTL4                      ((0x0034  << 2) + 0xfe023000)
#define MIPI_ADAPT_ALIG_CNTL5                      ((0x0035  << 2) + 0xfe023000)
#define MIPI_ADAPT_ALIG_CNTL6                      ((0x0036  << 2) + 0xfe023000)
#define MIPI_ADAPT_ALIG_CNTL7                      ((0x0037  << 2) + 0xfe023000)
#define MIPI_ADAPT_ALIG_CNTL8                      ((0x0038  << 2) + 0xfe023000)
#define MIPI_ADAPT_ALIG_CNTL9                      ((0x0039  << 2) + 0xfe023000)
#define MIPI_ADAPT_ALIG_ST0                        ((0x003a  << 2) + 0xfe023000)
#define MIPI_ADAPT_ALIG_ST1                        ((0x003b  << 2) + 0xfe023000)
#define MIPI_ADAPT_ALIG_CNTL10                     ((0x003c  << 2) + 0xfe023000)
#define MIPI_OTHER_CNTL0                           ((0x0040  << 2) + 0xfe023000)
#define MIPI_OTHER_CNTL1                           ((0x0041  << 2) + 0xfe023000)
#define MIPI_OTHER_CNTL2                           ((0x0042  << 2) + 0xfe023000)
#define MIPI_OTHER_CNTL3                           ((0x0043  << 2) + 0xfe023000)
#define MIPI_OTHER_CNTL4                           ((0x0044  << 2) + 0xfe023000)
#define MIPI_OTHER_ST0                             ((0x004a  << 2) + 0xfe023000)
#define MIPI_OTHER_ST1                             ((0x004b  << 2) + 0xfe023000)
#define DDR_RD0_LBUF_STATUS                        ((0x0050  << 2) + 0xfe023000)
#define DDR_RD1_LBUF_STATUS                        ((0x0051  << 2) + 0xfe023000)
#define MIPI_ADAPT_IRQ_MASK0                       ((0x0060  << 2) + 0xfe023000)
#define MIPI_ADAPT_IRQ_PENDING0                    ((0x0061  << 2) + 0xfe023000)
#define MIPI_ADAPT_IRQ_MASK1                       ((0x0062  << 2) + 0xfe023000)
#define MIPI_ADAPT_IRQ_PENDING1                    ((0x0063  << 2) + 0xfe023000)
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe023800
// -----------------------------------------------
#define MIPI_ISP_RDARB_MODE                        ((0x0000  << 2) + 0xfe023800)
#define MIPI_ISP_RDARB_REQEN_SLV                   ((0x0001  << 2) + 0xfe023800)
#define MIPI_ISP_RDARB_WEIGH0_SLV                  ((0x0002  << 2) + 0xfe023800)
#define MIPI_ISP_RDARB_WEIGH1_SLV                  ((0x0003  << 2) + 0xfe023800)
#define MIPI_ISP_RDARB_UGT                         ((0x0004  << 2) + 0xfe023800)
#define MIPI_ISP_RDARB_LIMT0                       ((0x0005  << 2) + 0xfe023800)
#define MIPI_ISP_WRARB_MODE                        ((0x0006  << 2) + 0xfe023800)
#define MIPI_ISP_WRARB_REQEN_SLV                   ((0x0007  << 2) + 0xfe023800)
#define MIPI_ISP_WRARB_WEIGH0_SLV                  ((0x0008  << 2) + 0xfe023800)
#define MIPI_ISP_WRARB_WEIGH1_SLV                  ((0x0009  << 2) + 0xfe023800)
#define MIPI_ISP_WRARB_UGT                         ((0x000a  << 2) + 0xfe023800)
#define MIPI_ISP_RDWR_ARB_STATUS                   ((0x000b  << 2) + 0xfe023800)
#define MIPI_ISP_ARB_DBG_CTRL                      ((0x000c  << 2) + 0xfe023800)
#define MIPI_ISP_ARB_DBG_STAT                      ((0x000d  << 2) + 0xfe023800)
// -----------------------------------------------
// APB_BASE:  APB0_BASE_ADDR = 0xfe023c00
// -----------------------------------------------
#define MIPI_BL_FR_PING_ADDR0_ST                   ((0x0000  << 2) + 0xfe023c00)
#define MIPI_BL_FR_PING_ADDR0_ED                   ((0x0001  << 2) + 0xfe023c00)
#define MIPI_BL_FR_PING_ADDR1_ST                   ((0x0002  << 2) + 0xfe023c00)
#define MIPI_BL_FR_PING_ADDR1_ED                   ((0x0003  << 2) + 0xfe023c00)
#define MIPI_BL_FR_PONG_ADDR0_ST                   ((0x0004  << 2) + 0xfe023c00)
#define MIPI_BL_FR_PONG_ADDR0_ED                   ((0x0005  << 2) + 0xfe023c00)
#define MIPI_BL_FR_PONG_ADDR1_ST                   ((0x0006  << 2) + 0xfe023c00)
#define MIPI_BL_FR_PONG_ADDR1_ED                   ((0x0007  << 2) + 0xfe023c00)
#define MIPI_BL_FR_FRAME_TH                        ((0x0008  << 2) + 0xfe023c00)
#define MIPI_BL_FR_FRAME_BUFFER_START_ADDR0        ((0x0009  << 2) + 0xfe023c00)
#define MIPI_BL_FR_FRAME_SIZE0                     ((0x000a  << 2) + 0xfe023c00)
#define MIPI_BL_FR_FRAME_BUF_SIZE0                 ((0x000b  << 2) + 0xfe023c00)
#define MIPI_BL_FR_FRAME_BUFFER_START_ADDR1        ((0x000c  << 2) + 0xfe023c00)
#define MIPI_BL_FR_FRAME_SIZE1                     ((0x000d  << 2) + 0xfe023c00)
#define MIPI_BL_FR_FRAME_BUF_SIZE1                 ((0x000e  << 2) + 0xfe023c00)
#define MIPI_BL_FR_CTRL0                           ((0x000f  << 2) + 0xfe023c00)
#define MIPI_BL_FR_CTRL1                           ((0x0010  << 2) + 0xfe023c00)
#define MIPI_BL_FR_FRAME_COUNT                     ((0x0011  << 2) + 0xfe023c00)
#define MIPI_BL_FR_AWADDR_O                        ((0x0012  << 2) + 0xfe023c00)
#define MIPI_BL_FR_AWADDR_I                        ((0x0013  << 2) + 0xfe023c00)
#define MIPI_BL_FR_STAT0                           ((0x0014  << 2) + 0xfe023c00)
#define MIPI_BL_FR_STAT1                           ((0x0015  << 2) + 0xfe023c00)
#define MIPI_BL_FR_STAT2                           ((0x0016  << 2) + 0xfe023c00)
#define MIPI_BL_FR_STAT3                           ((0x0017  << 2) + 0xfe023c00)
#define MIPI_BL_FR_FRAME_BUFFER_START_ADDR0P       ((0x0018  << 2) + 0xfe023c00)
#define MIPI_BL_DS_PING_ADDR0_ST                   ((0x0020  << 2) + 0xfe023c00)
#define MIPI_BL_DS_PING_ADDR0_ED                   ((0x0021  << 2) + 0xfe023c00)
#define MIPI_BL_DS_PING_ADDR1_ST                   ((0x0022  << 2) + 0xfe023c00)
#define MIPI_BL_DS_PING_ADDR1_ED                   ((0x0023  << 2) + 0xfe023c00)
#define MIPI_BL_DS_PONG_ADDR0_ST                   ((0x0024  << 2) + 0xfe023c00)
#define MIPI_BL_DS_PONG_ADDR0_ED                   ((0x0025  << 2) + 0xfe023c00)
#define MIPI_BL_DS_PONG_ADDR1_ST                   ((0x0026  << 2) + 0xfe023c00)
#define MIPI_BL_DS_PONG_ADDR1_ED                   ((0x0027  << 2) + 0xfe023c00)
#define MIPI_BL_DS_FRAME_TH                        ((0x0028  << 2) + 0xfe023c00)
#define MIPI_BL_DS_FRAME_BUFFER_START_ADDR0        ((0x0029  << 2) + 0xfe023c00)
#define MIPI_BL_DS_FRAME_SIZE0                     ((0x002a  << 2) + 0xfe023c00)
#define MIPI_BL_DS_FRAME_BUF_SIZE0                 ((0x002b  << 2) + 0xfe023c00)
#define MIPI_BL_DS_FRAME_BUFFER_START_ADDR1        ((0x002c  << 2) + 0xfe023c00)
#define MIPI_BL_DS_FRAME_SIZE1                     ((0x002d  << 2) + 0xfe023c00)
#define MIPI_BL_DS_FRAME_BUF_SIZE1                 ((0x002e  << 2) + 0xfe023c00)
#define MIPI_BL_DS_CTRL0                           ((0x002f  << 2) + 0xfe023c00)
#define MIPI_BL_DS_CTRL1                           ((0x0030  << 2) + 0xfe023c00)
#define MIPI_BL_DS_FRAME_COUNT                     ((0x0031  << 2) + 0xfe023c00)
#define MIPI_BL_DS_AWADDR_O                        ((0x0032  << 2) + 0xfe023c00)
#define MIPI_BL_DS_AWADDR_I                        ((0x0033  << 2) + 0xfe023c00)
#define MIPI_BL_DS_STAT0                           ((0x0034  << 2) + 0xfe023c00)
#define MIPI_BL_DS_STAT1                           ((0x0035  << 2) + 0xfe023c00)
#define MIPI_BL_DS_STAT2                           ((0x0036  << 2) + 0xfe023c00)
#define MIPI_BL_DS_STAT3                           ((0x0037  << 2) + 0xfe023c00)
#define MIPI_BL_DS_FRAME_BUFFER_START_ADDR0P       ((0x0038  << 2) + 0xfe023c00)
#define MIPI_BL_WF_PING_ADDR0_ST                   ((0x0040  << 2) + 0xfe023c00)
#define MIPI_BL_WF_PING_ADDR0_ED                   ((0x0041  << 2) + 0xfe023c00)
#define MIPI_BL_WF_PING_ADDR1_ST                   ((0x0042  << 2) + 0xfe023c00)
#define MIPI_BL_WF_PING_ADDR1_ED                   ((0x0043  << 2) + 0xfe023c00)
#define MIPI_BL_WF_PONG_ADDR0_ST                   ((0x0044  << 2) + 0xfe023c00)
#define MIPI_BL_WF_PONG_ADDR0_ED                   ((0x0045  << 2) + 0xfe023c00)
#define MIPI_BL_WF_PONG_ADDR1_ST                   ((0x0046  << 2) + 0xfe023c00)
#define MIPI_BL_WF_PONG_ADDR1_ED                   ((0x0047  << 2) + 0xfe023c00)
#define MIPI_BL_WF_FRAME_TH                        ((0x0048  << 2) + 0xfe023c00)
#define MIPI_BL_WF_FRAME_BUFFER_START_ADDR0        ((0x0049  << 2) + 0xfe023c00)
#define MIPI_BL_WF_FRAME_SIZE0                     ((0x004a  << 2) + 0xfe023c00)
#define MIPI_BL_WF_FRAME_BUF_SIZE0                 ((0x004b  << 2) + 0xfe023c00)
#define MIPI_BL_WF_FRAME_BUFFER_START_ADDR1        ((0x004c  << 2) + 0xfe023c00)
#define MIPI_BL_WF_FRAME_SIZE1                     ((0x004d  << 2) + 0xfe023c00)
#define MIPI_BL_WF_FRAME_BUF_SIZE1                 ((0x004e  << 2) + 0xfe023c00)
#define MIPI_BL_WF_CTRL0                           ((0x004f  << 2) + 0xfe023c00)
#define MIPI_BL_WF_CTRL1                           ((0x0050  << 2) + 0xfe023c00)
#define MIPI_BL_WF_FRAME_COUNT                     ((0x0051  << 2) + 0xfe023c00)
#define MIPI_BL_WF_AWADDR_O                        ((0x0052  << 2) + 0xfe023c00)
#define MIPI_BL_WF_AWADDR_I                        ((0x0053  << 2) + 0xfe023c00)
#define MIPI_BL_WF_STAT0                           ((0x0054  << 2) + 0xfe023c00)
#define MIPI_BL_WF_STAT1                           ((0x0055  << 2) + 0xfe023c00)
#define MIPI_BL_WF_STAT2                           ((0x0056  << 2) + 0xfe023c00)
#define MIPI_BL_WF_STAT3                           ((0x0057  << 2) + 0xfe023c00)
#define MIPI_BL_WF_FRAME_BUFFER_START_ADDR0P       ((0x0058  << 2) + 0xfe023c00)
//
// Closing file:  mipi_isp.h
//
// synopsys translate_off
// synopsys translate_on
//
// Closing file:  ./REG_LIST_RTL.h
//
#include "soc_def.h"
#endif // REGISTER_H

