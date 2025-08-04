/*
 * (C) Copyright 2025 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef _ASM_ARCH_GRF_RV1126B_H
#define _ASM_ARCH_GRF_RV1126B_H

#include <common.h>

/* cpu_grf register structure define */
struct rv1126b_cpu_grf_reg {
	uint32_t con0;                               /* address offset: 0x0000 */
	uint32_t con1;                               /* address offset: 0x0004 */
	uint32_t mem_cfg_uhdspra;                    /* address offset: 0x0008 */
	uint32_t status0;                            /* address offset: 0x000c */
	uint32_t status1;                            /* address offset: 0x0010 */
};

check_member(rv1126b_cpu_grf_reg, status1, 0x0010);

/* ddr_grf register structure define */
struct rv1126b_ddr_grf_reg {
	uint32_t con0;                               /* address offset: 0x0000 */
	uint32_t con1;                               /* address offset: 0x0004 */
	uint32_t reserved0008[2];                    /* address offset: 0x0008 */
	uint32_t con4;                               /* address offset: 0x0010 */
	uint32_t reserved0014[7];                    /* address offset: 0x0014 */
	uint32_t con12;                              /* address offset: 0x0030 */
	uint32_t con13;                              /* address offset: 0x0034 */
	uint32_t con14;                              /* address offset: 0x0038 */
	uint32_t con15;                              /* address offset: 0x003c */
	uint32_t con16;                              /* address offset: 0x0040 */
	uint32_t con17;                              /* address offset: 0x0044 */
	uint32_t con18;                              /* address offset: 0x0048 */
	uint32_t reserved004c;                       /* address offset: 0x004c */
	uint32_t con20;                              /* address offset: 0x0050 */
	uint32_t con21;                              /* address offset: 0x0054 */
	uint32_t con22;                              /* address offset: 0x0058 */
	uint32_t con23;                              /* address offset: 0x005c */
	uint32_t reserved0060[8];                    /* address offset: 0x0060 */
	uint32_t probe_ctrl;                         /* address offset: 0x0080 */
	uint32_t reserved0084[31];                   /* address offset: 0x0084 */
	uint32_t status0;                            /* address offset: 0x0100 */
	uint32_t status1;                            /* address offset: 0x0104 */
	uint32_t status2;                            /* address offset: 0x0108 */
	uint32_t status3;                            /* address offset: 0x010c */
	uint32_t status4;                            /* address offset: 0x0110 */
	uint32_t status5;                            /* address offset: 0x0114 */
	uint32_t status6;                            /* address offset: 0x0118 */
	uint32_t status7;                            /* address offset: 0x011c */
	uint32_t status8;                            /* address offset: 0x0120 */
	uint32_t status9;                            /* address offset: 0x0124 */
	uint32_t status10;                           /* address offset: 0x0128 */
	uint32_t status11;                           /* address offset: 0x012c */
	uint32_t status12;                           /* address offset: 0x0130 */
	uint32_t status13;                           /* address offset: 0x0134 */
	uint32_t status14;                           /* address offset: 0x0138 */
	uint32_t status15;                           /* address offset: 0x013c */
	uint32_t status16;                           /* address offset: 0x0140 */
	uint32_t status17;                           /* address offset: 0x0144 */
	uint32_t reserved0148;                       /* address offset: 0x0148 */
	uint32_t status19;                           /* address offset: 0x014c */
	uint32_t reserved0150[10];                   /* address offset: 0x0150 */
	uint32_t status30;                           /* address offset: 0x0178 */
};

check_member(rv1126b_ddr_grf_reg, status30, 0x0178);

/* pmu_grf register structure define */
struct rv1126b_pmu_grf_reg {
	uint32_t soc_con0;                           /* address offset: 0x0000 */
	uint32_t soc_con1;                           /* address offset: 0x0004 */
	uint32_t soc_con2;                           /* address offset: 0x0008 */
	uint32_t soc_con3;                           /* address offset: 0x000c */
	uint32_t soc_con4;                           /* address offset: 0x0010 */
	uint32_t soc_con5;                           /* address offset: 0x0014 */
	uint32_t soc_con6;                           /* address offset: 0x0018 */
	uint32_t soc_con7;                           /* address offset: 0x001c */
	uint32_t soc_con8;                           /* address offset: 0x0020 */
	uint32_t soc_con9;                           /* address offset: 0x0024 */
	uint32_t soc_con10;                          /* address offset: 0x0028 */
	uint32_t soc_con11;                          /* address offset: 0x002c */
	uint32_t soc_con12;                          /* address offset: 0x0030 */
	uint32_t soc_con13;                          /* address offset: 0x0034 */
	uint32_t soc_con14;                          /* address offset: 0x0038 */
	uint32_t soc_con15;                          /* address offset: 0x003c */
	uint32_t reserved0040[16];                   /* address offset: 0x0040 */
	uint32_t aad_con0;                           /* address offset: 0x0080 */
	uint32_t reserved0084[47];                   /* address offset: 0x0084 */
	uint32_t men_con0;                           /* address offset: 0x0140 */
	uint32_t men_con1;                           /* address offset: 0x0144 */
	uint32_t men_con2;                           /* address offset: 0x0148 */
	uint32_t reserved014c;                       /* address offset: 0x014c */
	uint32_t soc_special0;                       /* address offset: 0x0150 */
	uint32_t reserved0154[3];                    /* address offset: 0x0154 */
	uint32_t soc_aov_int_con;                    /* address offset: 0x0160 */
	uint32_t reserved0164[3];                    /* address offset: 0x0164 */
	uint32_t soc_status0;                        /* address offset: 0x0170 */
	uint32_t soc_status1;                        /* address offset: 0x0174 */
	uint32_t soc_status2;                        /* address offset: 0x0178 */
	uint32_t reserved017c[33];                   /* address offset: 0x017c */
	uint32_t os_reg0;                            /* address offset: 0x0200 */
	uint32_t os_reg1;                            /* address offset: 0x0204 */
	uint32_t os_reg2;                            /* address offset: 0x0208 */
	uint32_t os_reg3;                            /* address offset: 0x020c */
	uint32_t os_reg4;                            /* address offset: 0x0210 */
	uint32_t os_reg5;                            /* address offset: 0x0214 */
	uint32_t os_reg6;                            /* address offset: 0x0218 */
	uint32_t os_reg7;                            /* address offset: 0x021c */
	uint32_t os_reg8;                            /* address offset: 0x0220 */
	uint32_t os_reg9;                            /* address offset: 0x0224 */
	uint32_t os_reg10;                           /* address offset: 0x0228 */
	uint32_t os_reg11;                           /* address offset: 0x022c */
	uint32_t reset_function_status;              /* address offset: 0x0230 */
	uint32_t reset_function_clr;                 /* address offset: 0x0234 */
	uint32_t reserved0238[82];                   /* address offset: 0x0238 */
	uint32_t sig_detect_con;                     /* address offset: 0x0380 */
	uint32_t reserved0384[3];                    /* address offset: 0x0384 */
	uint32_t sig_detect_status;                  /* address offset: 0x0390 */
	uint32_t reserved0394[3];                    /* address offset: 0x0394 */
	uint32_t sig_detect_status_clear;            /* address offset: 0x03a0 */
	uint32_t reserved03a4[3];                    /* address offset: 0x03a4 */
	uint32_t sdmmc_det_counter;                  /* address offset: 0x03b0 */
};

check_member(rv1126b_pmu_grf_reg, sdmmc_det_counter, 0x03b0);

/* npu_grf register structure define */
struct rv1126b_npu_grf_reg {
	uint32_t mem_grf_spra;                       /* address offset: 0x0000 */
	uint32_t mem_grf_dpra;                       /* address offset: 0x0004 */
	uint32_t npu_grf_cbuf_mem_soft_gate;         /* address offset: 0x0008 */
	uint32_t npu_grf_cfg_nsp_slv_addr;           /* address offset: 0x000c */
	uint32_t npu_grf_nsp_mem_soft_gate;          /* address offset: 0x0010 */
	uint32_t npu_grf_cfg_use_nsp;                /* address offset: 0x0014 */
	uint32_t npu_grf_shape;                      /* address offset: 0x0018 */
};

check_member(rv1126b_npu_grf_reg, npu_grf_shape, 0x0018);

/* peri_grf register structure define */
struct rv1126b_peri_grf_reg {
	uint32_t mem_grf_spra;                       /* address offset: 0x0000 */
	uint32_t mem_grf_dpra;                       /* address offset: 0x0004 */
	uint32_t reserved0008;                       /* address offset: 0x0008 */
	uint32_t usb3_grf_con_pending;               /* address offset: 0x000c */
	uint32_t reserved0010;                       /* address offset: 0x0010 */
	uint32_t mem_gate_grf_con;                   /* address offset: 0x0014 */
	uint32_t hprot_grf_con;                      /* address offset: 0x0018 */
	uint32_t usbhostphy_con0;                    /* address offset: 0x001c */
	uint32_t usbotgphy_con0;                     /* address offset: 0x0020 */
	uint32_t usbotgphy_con1;                     /* address offset: 0x0024 */
	uint32_t usbotgphy_con2;                     /* address offset: 0x0028 */
	uint32_t usbotgphy_con3;                     /* address offset: 0x002c */
	uint32_t host0_con0;                         /* address offset: 0x0030 */
	uint32_t host0_con1;                         /* address offset: 0x0034 */
	uint32_t usb3otg0_con0;                      /* address offset: 0x0038 */
	uint32_t usb3otg0_con1;                      /* address offset: 0x003c */
	uint32_t reserved0040[13];                   /* address offset: 0x0040 */
	uint32_t otgphy_int_en;                      /* address offset: 0x0074 */
	uint32_t otgphy_int_st;                      /* address offset: 0x0078 */
	uint32_t otgphy_int_st_clr;                  /* address offset: 0x007c */
	uint32_t otgphy_ls_con;                      /* address offset: 0x0080 */
	uint32_t otgphy_dis_con;                     /* address offset: 0x0084 */
	uint32_t otgphy_bvalid_con;                  /* address offset: 0x0088 */
	uint32_t otgphy_id_con;                      /* address offset: 0x008c */
	uint32_t hostphy_int_en;                     /* address offset: 0x0090 */
	uint32_t hostphy_int_st;                     /* address offset: 0x0094 */
	uint32_t hostphy_int_st_clr;                 /* address offset: 0x0098 */
	uint32_t hostphy_ls_con;                     /* address offset: 0x009c */
	uint32_t hostphy_dis_con;                    /* address offset: 0x00a0 */
	uint32_t hostphy_bvalid_con;                 /* address offset: 0x00a4 */
	uint32_t hostphy_id_con;                     /* address offset: 0x00a8 */
	uint32_t reserved00ac[21];                   /* address offset: 0x00ac */
	uint32_t usb3otg0_status;                    /* address offset: 0x0100 */
	uint32_t usb3otg0_status_cb;                 /* address offset: 0x0104 */
	uint32_t usb3otg0_status_lat0;               /* address offset: 0x0108 */
	uint32_t usb3otg0_status_lat1;               /* address offset: 0x010c */
	uint32_t usbphy_st;                          /* address offset: 0x0110 */
	uint32_t host0_st;                           /* address offset: 0x0114 */
	uint32_t usb3_host_utmi_st;                  /* address offset: 0x0118 */
	uint32_t rtc_grf_st;                         /* address offset: 0x011c */
};

check_member(rv1126b_peri_grf_reg, rtc_grf_st, 0x011c);

/* usb3_phy_grf register structure define */
struct rv1126b_usb3_phy_grf_reg {
	uint32_t pipe_con0;                          /* address offset: 0x0000 */
	uint32_t pipe_con1;                          /* address offset: 0x0004 */
	uint32_t pipe_con2;                          /* address offset: 0x0008 */
	uint32_t pipe_con3;                          /* address offset: 0x000c */
	uint32_t pipe_con4;                          /* address offset: 0x0010 */
	uint32_t reserved0014[8];                    /* address offset: 0x0014 */
	uint32_t pipe_status1;                       /* address offset: 0x0034 */
	uint32_t reserved0038[18];                   /* address offset: 0x0038 */
	uint32_t lfps_det_con;                       /* address offset: 0x0080 */
	uint32_t reserved0084[7];                    /* address offset: 0x0084 */
	uint32_t phy_int_en;                         /* address offset: 0x00a0 */
	uint32_t phy_int_status;                     /* address offset: 0x00a4 */
};

check_member(rv1126b_usb3_phy_grf_reg, phy_int_status, 0x00a4);

/* sys_grf register structure define */
struct rv1126b_sys_grf_reg {
	uint32_t mem_grf_spra;                       /* address offset: 0x0000 */
	uint32_t mem_grf_dpra;                       /* address offset: 0x0004 */
	uint32_t mem_grf_rom;                        /* address offset: 0x0008 */
	uint32_t bus_grf_misc;                       /* address offset: 0x000c */
	uint32_t mem_con_gate;                       /* address offset: 0x0010 */
	uint32_t bus_grf_hprot_stall;                /* address offset: 0x0014 */
	uint32_t hpmcu_cache_misc;                   /* address offset: 0x0018 */
	uint32_t hpmcu_cache_addr_start;             /* address offset: 0x001c */
	uint32_t hpmcu_cache_addr_end;               /* address offset: 0x0020 */
	uint32_t hpmcu_code_addr_start;              /* address offset: 0x0024 */
	uint32_t hpmcu_sram_addr_start;              /* address offset: 0x0028 */
	uint32_t hpmcu_exsram_addr_start;            /* address offset: 0x002c */
	uint32_t biu_con0;                           /* address offset: 0x0030 */
	uint32_t biu_con1;                           /* address offset: 0x0034 */
	uint32_t uart_grf_rts_cts;                   /* address offset: 0x0038 */
	uint32_t uart_grf_dma_bypass;                /* address offset: 0x003c */
	uint32_t audio_con0;                         /* address offset: 0x0040 */
	uint32_t reserved0044;                       /* address offset: 0x0044 */
	uint32_t audio_con2;                         /* address offset: 0x0048 */
	uint32_t otp_con;                            /* address offset: 0x004c */
	uint32_t tsadc_grf_con0;                     /* address offset: 0x0050 */
	uint32_t tsadc_grf_con1;                     /* address offset: 0x0054 */
	uint32_t tsadc_grf_con2;                     /* address offset: 0x0058 */
	uint32_t tsadc_grf_con3;                     /* address offset: 0x005c */
	uint32_t tsadc_grf_con4;                     /* address offset: 0x0060 */
	uint32_t tsadc_grf_con5;                     /* address offset: 0x0064 */
	uint32_t tsadc_grf_con6;                     /* address offset: 0x0068 */
	uint32_t reserved006c[37];                   /* address offset: 0x006c */
	uint32_t biu_status0;                        /* address offset: 0x0100 */
	uint32_t biu_status1;                        /* address offset: 0x0104 */
	uint32_t biu_status2;                        /* address offset: 0x0108 */
	uint32_t hpmcu_cache_status;                 /* address offset: 0x010c */
	uint32_t tsadc_grf_status0;                  /* address offset: 0x0110 */
	uint32_t tsadc_grf_status1;                  /* address offset: 0x0114 */
	uint32_t sys_status;                         /* address offset: 0x0118 */
	uint32_t reserved011c[441];                  /* address offset: 0x011c */
	uint32_t chip_id;                            /* address offset: 0x0800 */
	uint32_t chip_version;                       /* address offset: 0x0804 */
};

check_member(rv1126b_sys_grf_reg, chip_version, 0x0804);

/* vcp_grf register structure define */
struct rv1126b_vcp_grf_reg {
	uint32_t mem_grf_spra;                       /* address offset: 0x0000 */
	uint32_t mem_grf_dpra;                       /* address offset: 0x0004 */
	uint32_t vcp_grf_aisp_mem_con;               /* address offset: 0x0008 */
};

check_member(rv1126b_vcp_grf_reg, vcp_grf_aisp_mem_con, 0x0008);

/* vdo_grf register structure define */
struct rv1126b_vdo_grf_reg {
	uint32_t mem_grf_spra;                       /* address offset: 0x0000 */
	uint32_t mem_grf_dpra;                       /* address offset: 0x0004 */
	uint32_t mem_gate_grf_con;                   /* address offset: 0x0008 */
	uint32_t dsi_grf_con;                        /* address offset: 0x000c */
	uint32_t dsiphy_grf_con;                     /* address offset: 0x0010 */
	uint32_t rkmmu_grf_con;                      /* address offset: 0x0014 */
	uint32_t reserved0018[14];                   /* address offset: 0x0018 */
	uint32_t vdo_grf_status0;                    /* address offset: 0x0050 */
	uint32_t vdo_grf_status1;                    /* address offset: 0x0054 */
};

check_member(rv1126b_vdo_grf_reg, vdo_grf_status1, 0x0054);

/* vepu_grf register structure define */
struct rv1126b_vepu_grf_reg {
	uint32_t mem_grf_spra;                       /* address offset: 0x0000 */
	uint32_t mem_grf_dpra;                       /* address offset: 0x0004 */
	uint32_t vepu_grf_con0;                      /* address offset: 0x0008 */
	uint32_t saradc0_grf_con0;                   /* address offset: 0x000c */
	uint32_t saradc0_grf_con1;                   /* address offset: 0x0010 */
	uint32_t saradc0_grf_con2;                   /* address offset: 0x0014 */
	uint32_t reserved0018[3];                    /* address offset: 0x0018 */
	uint32_t sdmmc1_det_cnt;                     /* address offset: 0x0024 */
	uint32_t sdmmc1_sig_detect_con;              /* address offset: 0x0028 */
	uint32_t sdmmc1_sig_detect_status;           /* address offset: 0x002c */
	uint32_t sdmmc1_status_clr;                  /* address offset: 0x0030 */
};

check_member(rv1126b_vepu_grf_reg, sdmmc1_status_clr, 0x0030);

/* vi_grf register structure define */
struct rv1126b_vi_grf_reg {
	uint32_t mem_con_spra;                       /* address offset: 0x0000 */
	uint32_t mem_con_dpra;                       /* address offset: 0x0004 */
	uint32_t vi_grf_status;                      /* address offset: 0x0008 */
	uint32_t reserved000c;                       /* address offset: 0x000c */
	uint32_t csiphy0_grf_con;                    /* address offset: 0x0010 */
	uint32_t csiphy1_grf_con;                    /* address offset: 0x0014 */
	uint32_t csiphy0_grf_status;                 /* address offset: 0x0018 */
	uint32_t csiphy1_grf_status;                 /* address offset: 0x001c */
	uint32_t misc_grf_con;                       /* address offset: 0x0020 */
	uint32_t reserved0024[11];                   /* address offset: 0x0024 */
	uint32_t gmac_grf_con0;                      /* address offset: 0x0050 */
	uint32_t gmac_dma_ack;                       /* address offset: 0x0054 */
	uint32_t reserved0058[2];                    /* address offset: 0x0058 */
	uint32_t gmac_grf_status0;                   /* address offset: 0x0060 */
	uint32_t gmac_grf_status1;                   /* address offset: 0x0064 */
	uint32_t gmac_grf_status2;                   /* address offset: 0x0068 */
	uint32_t reserved006c[5];                    /* address offset: 0x006c */
	uint32_t saradc1_grf_con0;                   /* address offset: 0x0080 */
	uint32_t saradc1_grf_con1;                   /* address offset: 0x0084 */
	uint32_t saradc1_grf_con2;                   /* address offset: 0x0088 */
	uint32_t reserved008c;                       /* address offset: 0x008c */
	uint32_t saradc2_grf_con0;                   /* address offset: 0x0090 */
	uint32_t saradc2_grf_con1;                   /* address offset: 0x0094 */
	uint32_t saradc2_grf_con2;                   /* address offset: 0x0098 */
	uint32_t reserved009c[6];                    /* address offset: 0x009c */
	uint32_t rkmacphy_grf_con0;                  /* address offset: 0x00b4 */
	uint32_t rkmacphy_grf_con1;                  /* address offset: 0x00b8 */
	uint32_t rkmacphy_grf_con2;                  /* address offset: 0x00bc */
	uint32_t rkmacphy_grf_status;                /* address offset: 0x00c0 */
	uint32_t rkmacphy_calib_con;                 /* address offset: 0x00c4 */
};

check_member(rv1126b_vi_grf_reg, rkmacphy_calib_con, 0x00c4);

#endif /*  _ASM_ARCH_GRF_RV1126B_H  */
