/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __HDR2_H__
#define __HDR2_H__

enum hdr_module_sel {
	VD1_HDR = 0x1,
	VD2_HDR = 0x2,
	VD3_HDR = 0x4,
	OSD1_HDR = 0x8,
	OSD2_HDR = 0x10,
	VDIN0_HDR = 0x20,
	VDIN1_HDR = 0x40,
	DI_HDR = 0x80,
	OSD3_HDR = 0x100,
	OSD4_HDR = 0x200,
	RGB_OSD = 0x20000000,
	HDR_MAX
};

enum hdr_matrix_sel {
	HDR_IN_MTX = 0x1,
	HDR_GAMUT_MTX = 0x2,
	HDR_OUT_MTX = 0x4,
	HDR_MTX_MAX
};

enum hdr_lut_sel {
	HDR_EOTF_LUT = 0x1,
	HDR_OOTF_LUT = 0x2,
	HDR_OETF_LUT = 0x4,
	HDR_CGAIN_LUT = 0x8,
	HDR_LUT_MAX
};

enum hdr_process_sel {
	HDR_BYPASS = 0x1,
	HDR_SDR = 0x2,
	SDR_HDR = 0x4,
	HLG_BYPASS = 0x8,
	HLG_SDR = 0x10,
	HLG_HDR = 0x20,
	SDR_HLG = 0X40,
	HDRPLUS_SDR = 0x80,
	HDR_OFF = 0x100,
	RGB_BYPASS = 0x200,
	HDR_p_MAX
};

#define MTX_ON  1
#define MTX_OFF 0

#define MTX_ONLY  1
#define HDR_ONLY  0

#define LUT_ON  1
#define LUT_OFF 0

#define HDR2_EOTF_LUT_SIZE 143
#define HDR2_OOTF_LUT_SIZE 149
#define HDR2_OETF_LUT_SIZE 149
#define HDR2_CGAIN_LUT_SIZE 65

struct hdr_proc_mtx_param_s {
	int mtx_only;
	int mtx_in[15];
	int mtx_gamut[9];
	int mtx_gamut_mode;
	int mtx_cgain[15];
	int mtx_ogain[15];
	int mtx_out[15];
	int mtxi_pre_offset[3];
	int mtxi_pos_offset[3];
	int mtxo_pre_offset[3];
	int mtxo_pos_offset[3];
	unsigned int mtx_on;
	enum hdr_process_sel p_sel;
};

struct hdr_proc_lut_param_s {
	int eotf_lut[143];
	int oetf_lut[149];
	int ogain_lut[149];
	int cgain_lut[65];
	unsigned int lut_on;
	unsigned int bitdepth;
	unsigned int cgain_en;
};

void hdr_func(enum hdr_module_sel module_sel,
	      enum hdr_process_sel hdr_process_select);
/*osd2 hdr*/
#ifndef OSD2_HDR2_CTRL
#define OSD2_HDR2_CTRL                             0x5b00
#endif
#ifndef OSD2_HDR2_CLK_GATE
#define OSD2_HDR2_CLK_GATE                         0x5b01
#endif
#ifndef OSD2_HDR2_MATRIXI_COEF00_01
#define OSD2_HDR2_MATRIXI_COEF00_01                0x5b02
#endif
#ifndef OSD2_HDR2_MATRIXI_COEF02_10
#define OSD2_HDR2_MATRIXI_COEF02_10                0x5b03
#endif
#ifndef OSD2_HDR2_MATRIXI_COEF11_12
#define OSD2_HDR2_MATRIXI_COEF11_12                0x5b04
#endif
#ifndef OSD2_HDR2_MATRIXI_COEF20_21
#define OSD2_HDR2_MATRIXI_COEF20_21                0x5b05
#endif
#ifndef OSD2_HDR2_MATRIXI_COEF22
#define OSD2_HDR2_MATRIXI_COEF22                   0x5b06
#endif
#ifndef OSD2_HDR2_MATRIXI_COEF30_31
#define OSD2_HDR2_MATRIXI_COEF30_31                0x5b07
#endif
#ifndef OSD2_HDR2_MATRIXI_COEF32_40
#define OSD2_HDR2_MATRIXI_COEF32_40                0x5b08
#endif
#ifndef OSD2_HDR2_MATRIXI_COEF41_42
#define OSD2_HDR2_MATRIXI_COEF41_42                0x5b09
#endif
#ifndef OSD2_HDR2_MATRIXI_OFFSET0_1
#define OSD2_HDR2_MATRIXI_OFFSET0_1                0x5b0a
#endif
#ifndef OSD2_HDR2_MATRIXI_OFFSET2
#define OSD2_HDR2_MATRIXI_OFFSET2                  0x5b0b
#endif
#ifndef OSD2_HDR2_MATRIXI_PRE_OFFSET0_1
#define OSD2_HDR2_MATRIXI_PRE_OFFSET0_1            0x5b0c
#endif
#ifndef OSD2_HDR2_MATRIXI_PRE_OFFSET2
#define OSD2_HDR2_MATRIXI_PRE_OFFSET2              0x5b0d
#endif
#ifndef OSD2_HDR2_MATRIXO_COEF00_01
#define OSD2_HDR2_MATRIXO_COEF00_01                0x5b0e
#endif
#ifndef OSD2_HDR2_MATRIXO_COEF02_10
#define OSD2_HDR2_MATRIXO_COEF02_10                0x5b0f
#endif
#ifndef OSD2_HDR2_MATRIXO_COEF11_12
#define OSD2_HDR2_MATRIXO_COEF11_12                0x5b10
#endif
#ifndef OSD2_HDR2_MATRIXO_COEF20_21
#define OSD2_HDR2_MATRIXO_COEF20_21                0x5b11
#endif
#ifndef OSD2_HDR2_MATRIXO_COEF22
#define OSD2_HDR2_MATRIXO_COEF22                   0x5b12
#endif
#ifndef OSD2_HDR2_MATRIXO_COEF30_31
#define OSD2_HDR2_MATRIXO_COEF30_31                0x5b13
#endif
#ifndef OSD2_HDR2_MATRIXO_COEF32_40
#define OSD2_HDR2_MATRIXO_COEF32_40                0x5b14
#endif
#ifndef OSD2_HDR2_MATRIXO_COEF41_42
#define OSD2_HDR2_MATRIXO_COEF41_42                0x5b15
#endif
#ifndef OSD2_HDR2_MATRIXO_OFFSET0_1
#define OSD2_HDR2_MATRIXO_OFFSET0_1                0x5b16
#endif
#ifndef OSD2_HDR2_MATRIXO_OFFSET2
#define OSD2_HDR2_MATRIXO_OFFSET2                  0x5b17
#endif
#ifndef OSD2_HDR2_MATRIXO_PRE_OFFSET0_1
#define OSD2_HDR2_MATRIXO_PRE_OFFSET0_1            0x5b18
#endif
#ifndef OSD2_HDR2_MATRIXO_PRE_OFFSET2
#define OSD2_HDR2_MATRIXO_PRE_OFFSET2              0x5b19
#endif
#ifndef OSD2_HDR2_MATRIXI_CLIP
#define OSD2_HDR2_MATRIXI_CLIP                     0x5b1a
#endif
#ifndef OSD2_HDR2_MATRIXO_CLIP
#define OSD2_HDR2_MATRIXO_CLIP                     0x5b1b
#endif
#ifndef OSD2_HDR2_CGAIN_OFFT
#define OSD2_HDR2_CGAIN_OFFT                       0x5b1c
#endif
#ifndef OSD2_HDR2_HIST_RD
#define OSD2_HDR2_HIST_RD                          0x5b1d
#endif
#ifndef OSD2_EOTF_LUT_ADDR_PORT
#define OSD2_EOTF_LUT_ADDR_PORT                    0x5b1e
#endif
#ifndef OSD2_EOTF_LUT_DATA_PORT
#define OSD2_EOTF_LUT_DATA_PORT                    0x5b1f
#endif
#ifndef OSD2_OETF_LUT_ADDR_PORT
#define OSD2_OETF_LUT_ADDR_PORT                    0x5b20
#endif
#ifndef OSD2_OETF_LUT_DATA_PORT
#define OSD2_OETF_LUT_DATA_PORT                    0x5b21
#endif
#ifndef OSD2_CGAIN_LUT_ADDR_PORT
#define OSD2_CGAIN_LUT_ADDR_PORT                   0x5b22
#endif
#ifndef OSD2_CGAIN_LUT_DATA_PORT
#define OSD2_CGAIN_LUT_DATA_PORT                   0x5b23
#endif
#ifndef OSD2_HDR2_CGAIN_COEF0
#define OSD2_HDR2_CGAIN_COEF0                      0x5b24
#endif
#ifndef OSD2_HDR2_CGAIN_COEF1
#define OSD2_HDR2_CGAIN_COEF1                      0x5b25
#endif
#ifndef OSD2_OGAIN_LUT_ADDR_PORT
#define OSD2_OGAIN_LUT_ADDR_PORT                   0x5b26
#endif
#ifndef OSD2_OGAIN_LUT_DATA_PORT
#define OSD2_OGAIN_LUT_DATA_PORT                   0x5b27
#endif
#ifndef OSD2_HDR2_ADPS_CTRL
#define OSD2_HDR2_ADPS_CTRL                        0x5b28
#endif
#ifndef OSD2_HDR2_ADPS_ALPHA0
#define OSD2_HDR2_ADPS_ALPHA0                      0x5b29
#endif
#ifndef OSD2_HDR2_ADPS_ALPHA1
#define OSD2_HDR2_ADPS_ALPHA1                      0x5b2a
#endif
#ifndef OSD2_HDR2_ADPS_BETA0
#define OSD2_HDR2_ADPS_BETA0                       0x5b2b
#endif
#ifndef OSD2_HDR2_ADPS_BETA1
#define OSD2_HDR2_ADPS_BETA1                       0x5b2c
#endif
#ifndef OSD2_HDR2_ADPS_BETA2
#define OSD2_HDR2_ADPS_BETA2                       0x5b2d
#endif
#ifndef OSD2_HDR2_ADPS_COEF0
#define OSD2_HDR2_ADPS_COEF0                       0x5b2e
#endif
#ifndef OSD2_HDR2_ADPS_COEF1
#define OSD2_HDR2_ADPS_COEF1                       0x5b2f
#endif
#ifndef OSD2_HDR2_GMUT_CTRL
#define OSD2_HDR2_GMUT_CTRL                        0x5b30
#endif
#ifndef OSD2_HDR2_GMUT_COEF0
#define OSD2_HDR2_GMUT_COEF0                       0x5b31
#endif
#ifndef OSD2_HDR2_GMUT_COEF1
#define OSD2_HDR2_GMUT_COEF1                       0x5b32
#endif
#ifndef OSD2_HDR2_GMUT_COEF2
#define OSD2_HDR2_GMUT_COEF2                       0x5b33
#endif
#ifndef OSD2_HDR2_GMUT_COEF3
#define OSD2_HDR2_GMUT_COEF3                       0x5b34
#endif
#ifndef OSD2_HDR2_GMUT_COEF4
#define OSD2_HDR2_GMUT_COEF4                       0x5b35
#endif
#ifndef OSD2_HDR2_PIPE_CTRL1
#define OSD2_HDR2_PIPE_CTRL1                       0x5b36
#endif
#ifndef OSD2_HDR2_PIPE_CTRL2
#define OSD2_HDR2_PIPE_CTRL2                       0x5b37
#endif
#ifndef OSD2_HDR2_PIPE_CTRL3
#define OSD2_HDR2_PIPE_CTRL3                       0x5b38
#endif
#ifndef OSD2_HDR2_PROC_WIN1
#define OSD2_HDR2_PROC_WIN1                        0x5b39
#endif

#ifndef OSD2_HDR2_PROC_WIN2
#define OSD2_HDR2_PROC_WIN2                        0x5b3a
#endif
#ifndef OSD2_HDR2_MATRIXI_EN_CTRL
#define OSD2_HDR2_MATRIXI_EN_CTRL                  0x5b3b
#endif
#ifndef OSD2_HDR2_MATRIXO_EN_CTRL
#define OSD2_HDR2_MATRIXO_EN_CTRL                  0x5b3c
#endif
#ifndef OSD2_HDR2_HIST_CTRL
#define OSD2_HDR2_HIST_CTRL                        0x5b3d
#endif
#ifndef OSD2_HDR2_HIST_H_START_END
#define OSD2_HDR2_HIST_H_START_END                 0x5b3e
#endif
#ifndef OSD2_HDR2_HIST_V_START_END
#define OSD2_HDR2_HIST_V_START_END                 0x5b3f
#endif

/*osd3 hdr*/
#ifndef OSD3_HDR2_CTRL
#define OSD3_HDR2_CTRL                             0x5b50
#endif
#ifndef OSD3_HDR2_CLK_GATE
#define OSD3_HDR2_CLK_GATE                         0x5b51
#endif
#ifndef OSD3_HDR2_MATRIXI_COEF00_01
#define OSD3_HDR2_MATRIXI_COEF00_01                0x5b52
#endif
#ifndef OSD3_HDR2_MATRIXI_COEF02_10
#define OSD3_HDR2_MATRIXI_COEF02_10                0x5b53
#endif
#ifndef OSD3_HDR2_MATRIXI_COEF11_12
#define OSD3_HDR2_MATRIXI_COEF11_12                0x5b54
#endif
#ifndef OSD3_HDR2_MATRIXI_COEF20_21
#define OSD3_HDR2_MATRIXI_COEF20_21                0x5b55
#endif
#ifndef OSD3_HDR2_MATRIXI_COEF22
#define OSD3_HDR2_MATRIXI_COEF22                   0x5b56
#endif
#ifndef OSD3_HDR2_MATRIXI_COEF30_31
#define OSD3_HDR2_MATRIXI_COEF30_31                0x5b57
#endif
#ifndef OSD3_HDR2_MATRIXI_COEF32_40
#define OSD3_HDR2_MATRIXI_COEF32_40                0x5b58
#endif
#ifndef OSD3_HDR2_MATRIXI_COEF41_42
#define OSD3_HDR2_MATRIXI_COEF41_42                0x5b59
#endif
#ifndef OSD3_HDR2_MATRIXI_OFFSET0_1
#define OSD3_HDR2_MATRIXI_OFFSET0_1                0x5b5a
#endif
#ifndef OSD3_HDR2_MATRIXI_OFFSET2
#define OSD3_HDR2_MATRIXI_OFFSET2                  0x5b5b
#endif
#ifndef OSD3_HDR2_MATRIXI_PRE_OFFSET0_1
#define OSD3_HDR2_MATRIXI_PRE_OFFSET0_1            0x5b5c
#endif
#ifndef OSD3_HDR2_MATRIXI_PRE_OFFSET2
#define OSD3_HDR2_MATRIXI_PRE_OFFSET2              0x5b5d
#endif
#ifndef OSD3_HDR2_MATRIXO_COEF00_01
#define OSD3_HDR2_MATRIXO_COEF00_01                0x5b5e
#endif
#ifndef OSD3_HDR2_MATRIXO_COEF02_10
#define OSD3_HDR2_MATRIXO_COEF02_10                0x5b5f
#endif
#ifndef OSD3_HDR2_MATRIXO_COEF11_12
#define OSD3_HDR2_MATRIXO_COEF11_12                0x5b60
#endif
#ifndef OSD3_HDR2_MATRIXO_COEF20_21
#define OSD3_HDR2_MATRIXO_COEF20_21                0x5b61
#endif
#ifndef OSD3_HDR2_MATRIXO_COEF22
#define OSD3_HDR2_MATRIXO_COEF22                   0x5b62
#endif
#ifndef OSD3_HDR2_MATRIXO_COEF30_31
#define OSD3_HDR2_MATRIXO_COEF30_31                0x5b63
#endif
#ifndef OSD3_HDR2_MATRIXO_COEF32_40
#define OSD3_HDR2_MATRIXO_COEF32_40                0x5b64
#endif
#ifndef OSD3_HDR2_MATRIXO_COEF41_42
#define OSD3_HDR2_MATRIXO_COEF41_42                0x5b65
#endif
#ifndef OSD3_HDR2_MATRIXO_OFFSET0_1
#define OSD3_HDR2_MATRIXO_OFFSET0_1                0x5b66
#endif
#ifndef OSD3_HDR2_MATRIXO_OFFSET2
#define OSD3_HDR2_MATRIXO_OFFSET2                  0x5b67
#endif
#ifndef OSD3_HDR2_MATRIXO_PRE_OFFSET0_1
#define OSD3_HDR2_MATRIXO_PRE_OFFSET0_1            0x5b68
#endif
#ifndef OSD3_HDR2_MATRIXO_PRE_OFFSET2
#define OSD3_HDR2_MATRIXO_PRE_OFFSET2              0x5b69
#endif
#ifndef OSD3_HDR2_MATRIXI_CLIP
#define OSD3_HDR2_MATRIXI_CLIP                     0x5b6a
#endif
#ifndef OSD3_HDR2_MATRIXO_CLIP
#define OSD3_HDR2_MATRIXO_CLIP                     0x5b6b
#endif
#ifndef OSD3_HDR2_CGAIN_OFFT
#define OSD3_HDR2_CGAIN_OFFT                       0x5b6c
#endif
#ifndef OSD3_HDR2_HIST_RD
#define OSD3_HDR2_HIST_RD                          0x5b6d
#endif
#ifndef OSD3_EOTF_LUT_ADDR_PORT
#define OSD3_EOTF_LUT_ADDR_PORT                    0x5b6e
#endif
#ifndef OSD3_EOTF_LUT_DATA_PORT
#define OSD3_EOTF_LUT_DATA_PORT                    0x5b6f
#endif
#ifndef OSD3_OETF_LUT_ADDR_PORT
#define OSD3_OETF_LUT_ADDR_PORT                    0x5b70
#endif
#ifndef OSD3_OETF_LUT_DATA_PORT
#define OSD3_OETF_LUT_DATA_PORT                    0x5b71
#endif
#ifndef OSD3_CGAIN_LUT_ADDR_PORT
#define OSD3_CGAIN_LUT_ADDR_PORT                   0x5b72
#endif
#ifndef OSD3_CGAIN_LUT_DATA_PORT
#define OSD3_CGAIN_LUT_DATA_PORT                   0x5b73
#endif
#ifndef OSD3_HDR2_CGAIN_COEF0
#define OSD3_HDR2_CGAIN_COEF0                      0x5b74
#endif
#ifndef OSD3_HDR2_CGAIN_COEF1
#define OSD3_HDR2_CGAIN_COEF1                      0x5b75
#endif
#ifndef OSD3_OGAIN_LUT_ADDR_PORT
#define OSD3_OGAIN_LUT_ADDR_PORT                   0x5b76
#endif
#ifndef OSD3_OGAIN_LUT_DATA_PORT
#define OSD3_OGAIN_LUT_DATA_PORT                   0x5b77
#endif
#ifndef OSD3_HDR2_ADPS_CTRL
#define OSD3_HDR2_ADPS_CTRL                        0x5b78
#endif
#ifndef OSD3_HDR2_ADPS_ALPHA0
#define OSD3_HDR2_ADPS_ALPHA0                      0x5b79
#endif
#ifndef OSD3_HDR2_ADPS_ALPHA1
#define OSD3_HDR2_ADPS_ALPHA1                      0x5b7a
#endif
#ifndef OSD3_HDR2_ADPS_BETA0
#define OSD3_HDR2_ADPS_BETA0                       0x5b7b
#endif
#ifndef OSD3_HDR2_ADPS_BETA1
#define OSD3_HDR2_ADPS_BETA1                       0x5b7c
#endif
#ifndef OSD3_HDR2_ADPS_BETA2
#define OSD3_HDR2_ADPS_BETA2                       0x5b7d
#endif
#ifndef OSD3_HDR2_ADPS_COEF0
#define OSD3_HDR2_ADPS_COEF0                       0x5b7e
#endif
#ifndef OSD3_HDR2_ADPS_COEF1
#define OSD3_HDR2_ADPS_COEF1                       0x5b7f
#endif
#ifndef OSD3_HDR2_GMUT_CTRL
#define OSD3_HDR2_GMUT_CTRL                        0x5b80
#endif
#ifndef OSD3_HDR2_GMUT_COEF0
#define OSD3_HDR2_GMUT_COEF0                       0x5b81
#endif
#ifndef OSD3_HDR2_GMUT_COEF1
#define OSD3_HDR2_GMUT_COEF1                       0x5b82
#endif
#ifndef OSD3_HDR2_GMUT_COEF2
#define OSD3_HDR2_GMUT_COEF2                       0x5b83
#endif
#ifndef OSD3_HDR2_GMUT_COEF3
#define OSD3_HDR2_GMUT_COEF3                       0x5b84
#endif
#ifndef OSD3_HDR2_GMUT_COEF4
#define OSD3_HDR2_GMUT_COEF4                       0x5b85
#endif
#ifndef OSD3_HDR2_PIPE_CTRL1
#define OSD3_HDR2_PIPE_CTRL1                       0x5b86
#endif
#ifndef OSD3_HDR2_PIPE_CTRL2
#define OSD3_HDR2_PIPE_CTRL2                       0x5b87
#endif
#ifndef OSD3_HDR2_PIPE_CTRL3
#define OSD3_HDR2_PIPE_CTRL3                       0x5b88
#endif
#ifndef OSD3_HDR2_PROC_WIN1
#define OSD3_HDR2_PROC_WIN1                        0x5b89
#endif
#ifndef OSD3_HDR2_PROC_WIN2
#define OSD3_HDR2_PROC_WIN2                        0x5b8a
#endif
#ifndef OSD3_HDR2_MATRIXI_EN_CTRL
#define OSD3_HDR2_MATRIXI_EN_CTRL                  0x5b8b
#endif
#ifndef OSD3_HDR2_MATRIXO_EN_CTRL
#define OSD3_HDR2_MATRIXO_EN_CTRL                  0x5b8c
#endif
#ifndef OSD3_HDR2_HIST_CTRL
#define OSD3_HDR2_HIST_CTRL                        0x5b8d
#endif
#ifndef OSD3_HDR2_HIST_H_START_END
#define OSD3_HDR2_HIST_H_START_END                 0x5b8e
#endif
#ifndef OSD3_HDR2_HIST_V_START_END
#define OSD3_HDR2_HIST_V_START_END                 0x5b8f
#endif

/*osd4 hdr*/
#ifndef OSD4_HDR2_CTRL
#define OSD4_HDR2_CTRL                             0x5b50
#endif
#ifndef OSD4_HDR2_CLK_GATE
#define OSD4_HDR2_CLK_GATE                         0x5b51
#endif
#ifndef OSD4_HDR2_MATRIXI_COEF00_01
#define OSD4_HDR2_MATRIXI_COEF00_01                0x5b52
#endif
#ifndef OSD4_HDR2_MATRIXI_COEF02_10
#define OSD4_HDR2_MATRIXI_COEF02_10                0x5b53
#endif
#ifndef OSD4_HDR2_MATRIXI_COEF11_12
#define OSD4_HDR2_MATRIXI_COEF11_12                0x5b54
#endif
#ifndef OSD4_HDR2_MATRIXI_COEF20_21
#define OSD4_HDR2_MATRIXI_COEF20_21                0x5b55
#endif
#ifndef OSD4_HDR2_MATRIXI_COEF22
#define OSD4_HDR2_MATRIXI_COEF22                   0x5b56
#endif
#ifndef OSD4_HDR2_MATRIXI_COEF30_31
#define OSD4_HDR2_MATRIXI_COEF30_31                0x5b57
#endif
#ifndef OSD4_HDR2_MATRIXI_COEF32_40
#define OSD4_HDR2_MATRIXI_COEF32_40                0x5b58
#endif
#ifndef OSD4_HDR2_MATRIXI_COEF41_42
#define OSD4_HDR2_MATRIXI_COEF41_42                0x5b59
#endif
#ifndef OSD4_HDR2_MATRIXI_OFFSET0_1
#define OSD4_HDR2_MATRIXI_OFFSET0_1                0x5b5a
#endif
#ifndef OSD4_HDR2_MATRIXI_OFFSET2
#define OSD4_HDR2_MATRIXI_OFFSET2                  0x5b5b
#endif
#ifndef OSD4_HDR2_MATRIXI_PRE_OFFSET0_1
#define OSD4_HDR2_MATRIXI_PRE_OFFSET0_1            0x5b5c
#endif
#ifndef OSD4_HDR2_MATRIXI_PRE_OFFSET2
#define OSD4_HDR2_MATRIXI_PRE_OFFSET2              0x5b5d
#endif
#ifndef OSD4_HDR2_MATRIXO_COEF00_01
#define OSD4_HDR2_MATRIXO_COEF00_01                0x5b5e
#endif
#ifndef OSD4_HDR2_MATRIXO_COEF02_10
#define OSD4_HDR2_MATRIXO_COEF02_10                0x5b5f
#endif
#ifndef OSD4_HDR2_MATRIXO_COEF11_12
#define OSD4_HDR2_MATRIXO_COEF11_12                0x5b60
#endif
#ifndef OSD4_HDR2_MATRIXO_COEF20_21
#define OSD4_HDR2_MATRIXO_COEF20_21                0x5b61
#endif
#ifndef OSD4_HDR2_MATRIXO_COEF22
#define OSD4_HDR2_MATRIXO_COEF22                   0x5b62
#endif
#ifndef OSD4_HDR2_MATRIXO_COEF30_31
#define OSD4_HDR2_MATRIXO_COEF30_31                0x5b63
#endif
#ifndef OSD4_HDR2_MATRIXO_COEF32_40
#define OSD4_HDR2_MATRIXO_COEF32_40                0x5b64
#endif
#ifndef OSD4_HDR2_MATRIXO_COEF41_42
#define OSD4_HDR2_MATRIXO_COEF41_42                0x5b65
#endif
#ifndef OSD4_HDR2_MATRIXO_OFFSET0_1
#define OSD4_HDR2_MATRIXO_OFFSET0_1                0x5b66
#endif
#ifndef OSD4_HDR2_MATRIXO_OFFSET2
#define OSD4_HDR2_MATRIXO_OFFSET2                  0x5b67
#endif
#ifndef OSD4_HDR2_MATRIXO_PRE_OFFSET0_1
#define OSD4_HDR2_MATRIXO_PRE_OFFSET0_1            0x5b68
#endif
#ifndef OSD4_HDR2_MATRIXO_PRE_OFFSET2
#define OSD4_HDR2_MATRIXO_PRE_OFFSET2              0x5b69
#endif
#ifndef OSD4_HDR2_MATRIXI_CLIP
#define OSD4_HDR2_MATRIXI_CLIP                     0x5b6a
#endif
#ifndef OSD4_HDR2_MATRIXO_CLIP
#define OSD4_HDR2_MATRIXO_CLIP                     0x5b6b
#endif
#ifndef OSD4_HDR2_CGAIN_OFFT
#define OSD4_HDR2_CGAIN_OFFT                       0x5b6c
#endif
#ifndef OSD4_HDR2_HIST_RD
#define OSD4_HDR2_HIST_RD                          0x5b6d
#endif
#ifndef OSD4_EOTF_LUT_ADDR_PORT
#define OSD4_EOTF_LUT_ADDR_PORT                    0x5b6e
#endif
#ifndef OSD4_EOTF_LUT_DATA_PORT
#define OSD4_EOTF_LUT_DATA_PORT                    0x5b6f
#endif
#ifndef OSD4_OETF_LUT_ADDR_PORT
#define OSD4_OETF_LUT_ADDR_PORT                    0x5b70
#endif
#ifndef OSD4_OETF_LUT_DATA_PORT
#define OSD4_OETF_LUT_DATA_PORT                    0x5b71
#endif
#ifndef OSD4_CGAIN_LUT_ADDR_PORT
#define OSD4_CGAIN_LUT_ADDR_PORT                   0x5b72
#endif
#ifndef OSD4_CGAIN_LUT_DATA_PORT
#define OSD4_CGAIN_LUT_DATA_PORT                   0x5b73
#endif
#ifndef OSD4_HDR2_CGAIN_COEF0
#define OSD4_HDR2_CGAIN_COEF0                      0x5b74
#endif
#ifndef OSD4_HDR2_CGAIN_COEF1
#define OSD4_HDR2_CGAIN_COEF1                      0x5b75
#endif
#ifndef OSD4_OGAIN_LUT_ADDR_PORT
#define OSD4_OGAIN_LUT_ADDR_PORT                   0x5b76
#endif
#ifndef OSD4_OGAIN_LUT_DATA_PORT
#define OSD4_OGAIN_LUT_DATA_PORT                   0x5b77
#endif
#ifndef OSD4_HDR2_ADPS_CTRL
#define OSD4_HDR2_ADPS_CTRL                        0x5b78
#endif
#ifndef OSD4_HDR2_ADPS_ALPHA0
#define OSD4_HDR2_ADPS_ALPHA0                      0x5b79
#endif
#ifndef OSD4_HDR2_ADPS_ALPHA1
#define OSD4_HDR2_ADPS_ALPHA1                      0x5b7a
#endif
#ifndef OSD4_HDR2_ADPS_BETA0
#define OSD4_HDR2_ADPS_BETA0                       0x5b7b
#endif
#ifndef OSD4_HDR2_ADPS_BETA1
#define OSD4_HDR2_ADPS_BETA1                       0x5b7c
#endif
#ifndef OSD4_HDR2_ADPS_BETA2
#define OSD4_HDR2_ADPS_BETA2                       0x5b7d
#endif
#ifndef OSD4_HDR2_ADPS_COEF0
#define OSD4_HDR2_ADPS_COEF0                       0x5b7e
#endif
#ifndef OSD4_HDR2_ADPS_COEF1
#define OSD4_HDR2_ADPS_COEF1                       0x5b7f
#endif
#ifndef OSD4_HDR2_GMUT_CTRL
#define OSD4_HDR2_GMUT_CTRL                        0x5b80
#endif
#ifndef OSD4_HDR2_GMUT_COEF0
#define OSD4_HDR2_GMUT_COEF0                       0x5b81
#endif
#ifndef OSD4_HDR2_GMUT_COEF1
#define OSD4_HDR2_GMUT_COEF1                       0x5b82
#endif
#ifndef OSD4_HDR2_GMUT_COEF2
#define OSD4_HDR2_GMUT_COEF2                       0x5b83
#endif
#ifndef OSD4_HDR2_GMUT_COEF3
#define OSD4_HDR2_GMUT_COEF3                       0x5b84
#endif
#ifndef OSD4_HDR2_GMUT_COEF4
#define OSD4_HDR2_GMUT_COEF4                       0x5b85
#endif
#ifndef OSD4_HDR2_PIPE_CTRL1
#define OSD4_HDR2_PIPE_CTRL1                       0x5b86
#endif
#ifndef OSD4_HDR2_PIPE_CTRL2
#define OSD4_HDR2_PIPE_CTRL2                       0x5b87
#endif
#ifndef OSD4_HDR2_PIPE_CTRL3
#define OSD4_HDR2_PIPE_CTRL3                       0x5b88
#endif
#ifndef OSD4_HDR2_PROC_WIN1
#define OSD4_HDR2_PROC_WIN1                        0x5b89
#endif
#ifndef OSD4_HDR2_PROC_WIN2
#define OSD4_HDR2_PROC_WIN2                        0x5b8a
#endif
#ifndef OSD4_HDR2_MATRIXI_EN_CTRL
#define OSD4_HDR2_MATRIXI_EN_CTRL                  0x5b8b
#endif
#ifndef OSD4_HDR2_MATRIXO_EN_CTRL
#define OSD4_HDR2_MATRIXO_EN_CTRL                  0x5b8c
#endif
#ifndef OSD4_HDR2_HIST_CTRL
#define OSD4_HDR2_HIST_CTRL                        0x5b8d
#endif
#ifndef OSD4_HDR2_HIST_H_START_END
#define OSD4_HDR2_HIST_H_START_END                 0x5b8e
#endif
#ifndef OSD4_HDR2_HIST_V_START_END
#define OSD4_HDR2_HIST_V_START_END                 0x5b8f
#endif
#endif
