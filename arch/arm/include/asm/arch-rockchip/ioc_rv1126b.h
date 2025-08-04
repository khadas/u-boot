/*
 * (C) Copyright 2025 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef _ASM_ARCH_IOC_RV1126B_H
#define _ASM_ARCH_IOC_RV1126B_H

#include <common.h>

/* pmuio0_ioc register structure define */
struct rv1126b_pmuio0_ioc_reg {
	uint32_t gpio0a_iomux_sel_0;                 /* address offset: 0x0000 */
	uint32_t gpio0a_iomux_sel_1;                 /* address offset: 0x0004 */
	uint32_t gpio0b_iomux_sel_0;                 /* address offset: 0x0008 */
	uint32_t gpio0b_iomux_sel_1;                 /* address offset: 0x000c */
	uint32_t reserved0010[60];                   /* address offset: 0x0010 */
	uint32_t gpio0a_ds_0;                        /* address offset: 0x0100 */
	uint32_t gpio0a_ds_1;                        /* address offset: 0x0104 */
	uint32_t gpio0a_ds_2;                        /* address offset: 0x0108 */
	uint32_t gpio0a_ds_3;                        /* address offset: 0x010c */
	uint32_t gpio0b_ds_0;                        /* address offset: 0x0110 */
	uint32_t gpio0b_ds_1;                        /* address offset: 0x0114 */
	uint32_t gpio0b_ds_2;                        /* address offset: 0x0118 */
	uint32_t reserved011c[121];                  /* address offset: 0x011c */
	uint32_t gpio0a_pull;                        /* address offset: 0x0300 */
	uint32_t gpio0b_pull;                        /* address offset: 0x0304 */
	uint32_t reserved0308[62];                   /* address offset: 0x0308 */
	uint32_t gpio0a_ie;                          /* address offset: 0x0400 */
	uint32_t gpio0b_ie;                          /* address offset: 0x0404 */
	uint32_t reserved0408[62];                   /* address offset: 0x0408 */
	uint32_t gpio0a_smt;                         /* address offset: 0x0500 */
	uint32_t gpio0b_smt;                         /* address offset: 0x0504 */
	uint32_t reserved0508[62];                   /* address offset: 0x0508 */
	uint32_t gpio0a_sus;                         /* address offset: 0x0600 */
	uint32_t gpio0b_sus;                         /* address offset: 0x0604 */
	uint32_t reserved0608[62];                   /* address offset: 0x0608 */
	uint32_t gpio0a_sl;                          /* address offset: 0x0700 */
	uint32_t gpio0b_sl;                          /* address offset: 0x0704 */
	uint32_t reserved0708[62];                   /* address offset: 0x0708 */
	uint32_t gpio0a_od;                          /* address offset: 0x0800 */
	uint32_t gpio0b_od;                          /* address offset: 0x0804 */
	uint32_t reserved0808[62];                   /* address offset: 0x0808 */
	uint32_t io_vsel;                            /* address offset: 0x0900 */
	uint32_t grf_jtag_con0;                      /* address offset: 0x0904 */
	uint32_t grf_jtag_con1;                      /* address offset: 0x0908 */
	uint32_t reserved090c;                       /* address offset: 0x090c */
	uint32_t xin_con;                            /* address offset: 0x0910 */
	uint32_t reserved0914[187];                  /* address offset: 0x0914 */
	uint32_t grf_gpio0_filter_con0;              /* address offset: 0x0c00 */
	uint32_t grf_gpio0_filter_con1;              /* address offset: 0x0c04 */
	uint32_t grf_gpio0_filter_con2;              /* address offset: 0x0c08 */
	uint32_t grf_gpio0_filter_con3;              /* address offset: 0x0c0c */
	uint32_t grf_gpio0_filter_con4;              /* address offset: 0x0c10 */
	uint32_t grf_gpio0_filter_con5;              /* address offset: 0x0c14 */
};

check_member(rv1126b_pmuio0_ioc_reg, grf_gpio0_filter_con5, 0x0c14);

/* pmuio1_ioc register structure define */
struct rv1126b_pmuio1_ioc_reg {
	uint32_t reserved0000[4];                    /* address offset: 0x0000 */
	uint32_t gpio0c_iomux_sel_0;                 /* address offset: 0x0010 */
	uint32_t gpio0c_iomux_sel_1;                 /* address offset: 0x0014 */
	uint32_t gpio0d_iomux_sel_0;                 /* address offset: 0x0018 */
	uint32_t reserved001c[65];                   /* address offset: 0x001c */
	uint32_t gpio0c_ds_0;                        /* address offset: 0x0120 */
	uint32_t gpio0c_ds_1;                        /* address offset: 0x0124 */
	uint32_t gpio0c_ds_2;                        /* address offset: 0x0128 */
	uint32_t gpio0c_ds_3;                        /* address offset: 0x012c */
	uint32_t gpio0d_ds_0;                        /* address offset: 0x0130 */
	uint32_t reserved0134[117];                  /* address offset: 0x0134 */
	uint32_t gpio0c_pull;                        /* address offset: 0x0308 */
	uint32_t gpio0d_pull;                        /* address offset: 0x030c */
	uint32_t reserved0310[62];                   /* address offset: 0x0310 */
	uint32_t gpio0c_ie;                          /* address offset: 0x0408 */
	uint32_t gpio0d_ie;                          /* address offset: 0x040c */
	uint32_t reserved0410[62];                   /* address offset: 0x0410 */
	uint32_t gpio0c_smt;                         /* address offset: 0x0508 */
	uint32_t gpio0d_smt;                         /* address offset: 0x050c */
	uint32_t reserved0510[62];                   /* address offset: 0x0510 */
	uint32_t gpio0c_sus;                         /* address offset: 0x0608 */
	uint32_t gpio0d_sus;                         /* address offset: 0x060c */
	uint32_t reserved0610[62];                   /* address offset: 0x0610 */
	uint32_t gpio0c_sl;                          /* address offset: 0x0708 */
	uint32_t gpio0d_sl;                          /* address offset: 0x070c */
	uint32_t reserved0710[62];                   /* address offset: 0x0710 */
	uint32_t gpio0c_od;                          /* address offset: 0x0808 */
	uint32_t gpio0d_od;                          /* address offset: 0x080c */
	uint32_t reserved0810[60];                   /* address offset: 0x0810 */
	uint32_t io_vsel;                            /* address offset: 0x0900 */
};

check_member(rv1126b_pmuio1_ioc_reg, io_vsel, 0x0900);

/* vccio1_ioc register structure define */
struct rv1126b_vccio1_ioc_reg {
	uint32_t reserved0000[8];                    /* address offset: 0x0000 */
	uint32_t gpio1a_iomux_sel_0;                 /* address offset: 0x0020 */
	uint32_t gpio1a_iomux_sel_1;                 /* address offset: 0x0024 */
	uint32_t gpio1b_iomux_sel_0;                 /* address offset: 0x0028 */
	uint32_t gpio1b_iomux_sel_1;                 /* address offset: 0x002c */
	uint32_t reserved0030[68];                   /* address offset: 0x0030 */
	uint32_t gpio1a_ds_0;                        /* address offset: 0x0140 */
	uint32_t gpio1a_ds_1;                        /* address offset: 0x0144 */
	uint32_t gpio1a_ds_2;                        /* address offset: 0x0148 */
	uint32_t gpio1a_ds_3;                        /* address offset: 0x014c */
	uint32_t gpio1b_ds_0;                        /* address offset: 0x0150 */
	uint32_t gpio1b_ds_1;                        /* address offset: 0x0154 */
	uint32_t gpio1b_ds_2;                        /* address offset: 0x0158 */
	uint32_t gpio1b_ds_3;                        /* address offset: 0x015c */
	uint32_t reserved0160[108];                  /* address offset: 0x0160 */
	uint32_t gpio1a_pull;                        /* address offset: 0x0310 */
	uint32_t gpio1b_pull;                        /* address offset: 0x0314 */
	uint32_t reserved0318[62];                   /* address offset: 0x0318 */
	uint32_t gpio1a_ie;                          /* address offset: 0x0410 */
	uint32_t gpio1b_ie;                          /* address offset: 0x0414 */
	uint32_t reserved0418[62];                   /* address offset: 0x0418 */
	uint32_t gpio1a_smt;                         /* address offset: 0x0510 */
	uint32_t gpio1b_smt;                         /* address offset: 0x0514 */
	uint32_t reserved0518[62];                   /* address offset: 0x0518 */
	uint32_t gpio1a_sus;                         /* address offset: 0x0610 */
	uint32_t gpio1b_sus;                         /* address offset: 0x0614 */
	uint32_t reserved0618[62];                   /* address offset: 0x0618 */
	uint32_t gpio1a_sl;                          /* address offset: 0x0710 */
	uint32_t gpio1b_sl;                          /* address offset: 0x0714 */
	uint32_t reserved0718[62];                   /* address offset: 0x0718 */
	uint32_t gpio1a_od;                          /* address offset: 0x0810 */
	uint32_t gpio1b_od;                          /* address offset: 0x0814 */
	uint32_t reserved0818[60];                   /* address offset: 0x0818 */
	uint32_t io1_vsel;                           /* address offset: 0x0908 */
	uint32_t reserved090c[61];                   /* address offset: 0x090c */
	uint32_t ioc_misc_con0;                      /* address offset: 0x0a00 */
	uint32_t ioc_misc_con1;                      /* address offset: 0x0a04 */
	uint32_t ioc_misc_con2;                      /* address offset: 0x0a08 */
	uint32_t ioc_misc_con3;                      /* address offset: 0x0a0c */
	uint32_t ioc_misc_con4;                      /* address offset: 0x0a10 */
	uint32_t ioc_misc_con5;                      /* address offset: 0x0a14 */
	uint32_t ioc_misc_con6;                      /* address offset: 0x0a18 */
};

check_member(rv1126b_vccio1_ioc_reg, ioc_misc_con6, 0x0a18);

/* vccio2_ioc register structure define */
struct rv1126b_vccio2_ioc_reg {
	uint32_t reserved0000[16];                   /* address offset: 0x0000 */
	uint32_t gpio2a_iomux_sel_0;                 /* address offset: 0x0040 */
	uint32_t gpio2a_iomux_sel_1;                 /* address offset: 0x0044 */
	uint32_t reserved0048[78];                   /* address offset: 0x0048 */
	uint32_t gpio2a_ds_0;                        /* address offset: 0x0180 */
	uint32_t gpio2a_ds_1;                        /* address offset: 0x0184 */
	uint32_t gpio2a_ds_2;                        /* address offset: 0x0188 */
	uint32_t reserved018c[101];                  /* address offset: 0x018c */
	uint32_t gpio2a_pull;                        /* address offset: 0x0320 */
	uint32_t reserved0324[63];                   /* address offset: 0x0324 */
	uint32_t gpio2a_ie;                          /* address offset: 0x0420 */
	uint32_t reserved0424[63];                   /* address offset: 0x0424 */
	uint32_t gpio2a_smt;                         /* address offset: 0x0520 */
	uint32_t reserved0524[63];                   /* address offset: 0x0524 */
	uint32_t gpio2a_sus;                         /* address offset: 0x0620 */
	uint32_t reserved0624[63];                   /* address offset: 0x0624 */
	uint32_t gpio2a_sl;                          /* address offset: 0x0720 */
	uint32_t reserved0724[63];                   /* address offset: 0x0724 */
	uint32_t gpio2a_od;                          /* address offset: 0x0820 */
	uint32_t reserved0824[58];                   /* address offset: 0x0824 */
	uint32_t io_vsel;                            /* address offset: 0x090c */
	uint32_t reserved0910[159];                  /* address offset: 0x0910 */
	uint32_t grf_sddet_dly_con;                  /* address offset: 0x0b8c */
	uint32_t grf_jtag_con;                       /* address offset: 0x0b90 */
	uint32_t reserved0b94[27];                   /* address offset: 0x0b94 */
	uint32_t grf_gpio2_filter_con0;              /* address offset: 0x0c00 */
	uint32_t grf_gpio2_filter_con1;              /* address offset: 0x0c04 */
	uint32_t grf_gpio2_filter_con2;              /* address offset: 0x0c08 */
	uint32_t grf_gpio2_filter_con3;              /* address offset: 0x0c0c */
	uint32_t grf_gpio2_filter_con4;              /* address offset: 0x0c10 */
	uint32_t grf_gpio2_filter_con5;              /* address offset: 0x0c14 */
};

check_member(rv1126b_vccio2_ioc_reg, grf_gpio2_filter_con5, 0x0c14);

/* vccio3_ioc register structure define */
struct rv1126b_vccio3_ioc_reg {
	uint32_t reserved0000[24];                   /* address offset: 0x0000 */
	uint32_t gpio3a_iomux_sel_0;                 /* address offset: 0x0060 */
	uint32_t gpio3a_iomux_sel_1;                 /* address offset: 0x0064 */
	uint32_t gpio3b_iomux_sel_0;                 /* address offset: 0x0068 */
	uint32_t gpio3b_iomux_sel_1;                 /* address offset: 0x006c */
	uint32_t reserved0070[84];                   /* address offset: 0x0070 */
	uint32_t gpio3a_ds_0;                        /* address offset: 0x01c0 */
	uint32_t gpio3a_ds_1;                        /* address offset: 0x01c4 */
	uint32_t gpio3a_ds_2;                        /* address offset: 0x01c8 */
	uint32_t gpio3a_ds_3;                        /* address offset: 0x01cc */
	uint32_t gpio3b_ds_0;                        /* address offset: 0x01d0 */
	uint32_t gpio3b_ds_1;                        /* address offset: 0x01d4 */
	uint32_t gpio3b_ds_2;                        /* address offset: 0x01d8 */
	uint32_t gpio3b_ds_3;                        /* address offset: 0x01dc */
	uint32_t reserved01e0[84];                   /* address offset: 0x01e0 */
	uint32_t gpio3a_pull;                        /* address offset: 0x0330 */
	uint32_t gpio3b_pull;                        /* address offset: 0x0334 */
	uint32_t reserved0338[62];                   /* address offset: 0x0338 */
	uint32_t gpio3a_ie;                          /* address offset: 0x0430 */
	uint32_t gpio3b_ie;                          /* address offset: 0x0434 */
	uint32_t reserved0438[62];                   /* address offset: 0x0438 */
	uint32_t gpio3a_smt;                         /* address offset: 0x0530 */
	uint32_t gpio3b_smt;                         /* address offset: 0x0534 */
	uint32_t reserved0538[62];                   /* address offset: 0x0538 */
	uint32_t gpio3a_sus;                         /* address offset: 0x0630 */
	uint32_t gpio3b_sus;                         /* address offset: 0x0634 */
	uint32_t reserved0638[62];                   /* address offset: 0x0638 */
	uint32_t gpio3a_sl;                          /* address offset: 0x0730 */
	uint32_t gpio3b_sl;                          /* address offset: 0x0734 */
	uint32_t reserved0738[62];                   /* address offset: 0x0738 */
	uint32_t gpio3a_od;                          /* address offset: 0x0830 */
	uint32_t gpio3b_od;                          /* address offset: 0x0834 */
	uint32_t reserved0838[54];                   /* address offset: 0x0838 */
	uint32_t io3_vsel;                           /* address offset: 0x0910 */
	uint32_t reserved0914[59];                   /* address offset: 0x0914 */
	uint32_t ioc_misc_con0;                      /* address offset: 0x0a00 */
	uint32_t ioc_misc_con1;                      /* address offset: 0x0a04 */
	uint32_t ioc_misc_con2;                      /* address offset: 0x0a08 */
	uint32_t ioc_misc_con3;                      /* address offset: 0x0a0c */
	uint32_t ioc_misc_con4;                      /* address offset: 0x0a10 */
	uint32_t ioc_misc_con5;                      /* address offset: 0x0a14 */
	uint32_t ioc_misc_con6;                      /* address offset: 0x0a18 */
};

check_member(rv1126b_vccio3_ioc_reg, ioc_misc_con6, 0x0a18);

/* vccio4_ioc register structure define */
struct rv1126b_vccio4_ioc_reg {
	uint32_t reserved0000[32];                   /* address offset: 0x0000 */
	uint32_t gpio4a_iomux_sel_0;                 /* address offset: 0x0080 */
	uint32_t gpio4a_iomux_sel_1;                 /* address offset: 0x0084 */
	uint32_t gpio4b_iomux_sel_0;                 /* address offset: 0x0088 */
	uint32_t reserved008c[93];                   /* address offset: 0x008c */
	uint32_t gpio4a_ds_0;                        /* address offset: 0x0200 */
	uint32_t gpio4a_ds_1;                        /* address offset: 0x0204 */
	uint32_t gpio4a_ds_2;                        /* address offset: 0x0208 */
	uint32_t gpio4a_ds_3;                        /* address offset: 0x020c */
	uint32_t gpio4b_ds_0;                        /* address offset: 0x0210 */
	uint32_t reserved0214[75];                   /* address offset: 0x0214 */
	uint32_t gpio4a_pull;                        /* address offset: 0x0340 */
	uint32_t gpio4b_pull;                        /* address offset: 0x0344 */
	uint32_t reserved0348[62];                   /* address offset: 0x0348 */
	uint32_t gpio4a_ie;                          /* address offset: 0x0440 */
	uint32_t gpio4b_ie;                          /* address offset: 0x0444 */
	uint32_t reserved0448[62];                   /* address offset: 0x0448 */
	uint32_t gpio4a_smt;                         /* address offset: 0x0540 */
	uint32_t gpio4b_smt;                         /* address offset: 0x0544 */
	uint32_t reserved0548[62];                   /* address offset: 0x0548 */
	uint32_t gpio4a_sus;                         /* address offset: 0x0640 */
	uint32_t gpio4b_sus;                         /* address offset: 0x0644 */
	uint32_t reserved0648[62];                   /* address offset: 0x0648 */
	uint32_t gpio4a_sl;                          /* address offset: 0x0740 */
	uint32_t gpio4b_sl;                          /* address offset: 0x0744 */
	uint32_t reserved0748[62];                   /* address offset: 0x0748 */
	uint32_t gpio4a_od;                          /* address offset: 0x0840 */
	uint32_t gpio4b_od;                          /* address offset: 0x0844 */
	uint32_t reserved0848[51];                   /* address offset: 0x0848 */
	uint32_t io_vsel;                            /* address offset: 0x0914 */
	uint32_t reserved0918[194];                  /* address offset: 0x0918 */
	uint32_t grf_gpio4_filter_con0;              /* address offset: 0x0c20 */
	uint32_t grf_gpio4_filter_con1;              /* address offset: 0x0c24 */
	uint32_t grf_gpio4_filter_con2;              /* address offset: 0x0c28 */
	uint32_t grf_gpio4_filter_con3;              /* address offset: 0x0c2c */
	uint32_t grf_gpio4_filter_con4;              /* address offset: 0x0c30 */
	uint32_t grf_gpio4_filter_con5;              /* address offset: 0x0c34 */
};

check_member(rv1126b_vccio4_ioc_reg, grf_gpio4_filter_con5, 0x0c34);

/* vccio5_ioc register structure define */
struct rv1126b_vccio5_ioc_reg {
	uint32_t reserved0000[40];                   /* address offset: 0x0000 */
	uint32_t gpio5a_iomux_sel_0;                 /* address offset: 0x00a0 */
	uint32_t gpio5a_iomux_sel_1;                 /* address offset: 0x00a4 */
	uint32_t gpio5b_iomux_sel_0;                 /* address offset: 0x00a8 */
	uint32_t gpio5b_iomux_sel_1;                 /* address offset: 0x00ac */
	uint32_t gpio5c_iomux_sel_0;                 /* address offset: 0x00b0 */
	uint32_t gpio5c_iomux_sel_1;                 /* address offset: 0x00b4 */
	uint32_t gpio5d_iomux_sel_0;                 /* address offset: 0x00b8 */
	uint32_t gpio5d_iomux_sel_1;                 /* address offset: 0x00bc */
	uint32_t reserved00c0[96];                   /* address offset: 0x00c0 */
	uint32_t gpio5a_ds_0;                        /* address offset: 0x0240 */
	uint32_t gpio5a_ds_1;                        /* address offset: 0x0244 */
	uint32_t gpio5a_ds_2;                        /* address offset: 0x0248 */
	uint32_t gpio5a_ds_3;                        /* address offset: 0x024c */
	uint32_t gpio5b_ds_0;                        /* address offset: 0x0250 */
	uint32_t gpio5b_ds_1;                        /* address offset: 0x0254 */
	uint32_t gpio5b_ds_2;                        /* address offset: 0x0258 */
	uint32_t gpio5b_ds_3;                        /* address offset: 0x025c */
	uint32_t gpio5c_ds_0;                        /* address offset: 0x0260 */
	uint32_t gpio5c_ds_1;                        /* address offset: 0x0264 */
	uint32_t gpio5c_ds_2;                        /* address offset: 0x0268 */
	uint32_t gpio5c_ds_3;                        /* address offset: 0x026c */
	uint32_t gpio5d_ds_0;                        /* address offset: 0x0270 */
	uint32_t gpio5d_ds_1;                        /* address offset: 0x0274 */
	uint32_t gpio5d_ds_2;                        /* address offset: 0x0278 */
	uint32_t gpio5d_ds_3;                        /* address offset: 0x027c */
	uint32_t reserved0280[52];                   /* address offset: 0x0280 */
	uint32_t gpio5a_pull;                        /* address offset: 0x0350 */
	uint32_t gpio5b_pull;                        /* address offset: 0x0354 */
	uint32_t gpio5c_pull;                        /* address offset: 0x0358 */
	uint32_t gpio5d_pull;                        /* address offset: 0x035c */
	uint32_t reserved0360[60];                   /* address offset: 0x0360 */
	uint32_t gpio5a_ie;                          /* address offset: 0x0450 */
	uint32_t gpio5b_ie;                          /* address offset: 0x0454 */
	uint32_t gpio5c_ie;                          /* address offset: 0x0458 */
	uint32_t gpio5d_ie;                          /* address offset: 0x045c */
	uint32_t reserved0460[60];                   /* address offset: 0x0460 */
	uint32_t gpio5a_smt;                         /* address offset: 0x0550 */
	uint32_t gpio5b_smt;                         /* address offset: 0x0554 */
	uint32_t gpio5c_smt;                         /* address offset: 0x0558 */
	uint32_t gpio5d_smt;                         /* address offset: 0x055c */
	uint32_t reserved0560[60];                   /* address offset: 0x0560 */
	uint32_t gpio5a_sus;                         /* address offset: 0x0650 */
	uint32_t gpio5b_sus;                         /* address offset: 0x0654 */
	uint32_t gpio5c_sus;                         /* address offset: 0x0658 */
	uint32_t gpio5d_sus;                         /* address offset: 0x065c */
	uint32_t reserved0660[60];                   /* address offset: 0x0660 */
	uint32_t gpio5a_sl;                          /* address offset: 0x0750 */
	uint32_t gpio5b_sl;                          /* address offset: 0x0754 */
	uint32_t gpio5c_sl;                          /* address offset: 0x0758 */
	uint32_t gpio5d_sl;                          /* address offset: 0x075c */
	uint32_t reserved0760[60];                   /* address offset: 0x0760 */
	uint32_t gpio5a_od;                          /* address offset: 0x0850 */
	uint32_t gpio5b_od;                          /* address offset: 0x0854 */
	uint32_t gpio5c_od;                          /* address offset: 0x0858 */
	uint32_t gpio5d_od;                          /* address offset: 0x085c */
	uint32_t reserved0860[46];                   /* address offset: 0x0860 */
	uint32_t io_vsel;                            /* address offset: 0x0918 */
	uint32_t reserved091c[159];                  /* address offset: 0x091c */
	uint32_t grf_vicif_m1_con;                   /* address offset: 0x0b98 */
	uint32_t grf_vop_lcdc_con;                   /* address offset: 0x0b9c */
	uint32_t reserved0ba0[2];                    /* address offset: 0x0ba0 */
	uint32_t grf_gmacio_m1_con0;                 /* address offset: 0x0ba8 */
	uint32_t grf_gmacio_m1_con1;                 /* address offset: 0x0bac */
	uint32_t grf_uart0_dly_con;                  /* address offset: 0x0bb0 */
	uint32_t grf_uart_jtag_con;                  /* address offset: 0x0bb4 */
	uint32_t reserved0bb8[34];                   /* address offset: 0x0bb8 */
	uint32_t grf_gpio5_filter_con0;              /* address offset: 0x0c40 */
	uint32_t grf_gpio5_filter_con1;              /* address offset: 0x0c44 */
	uint32_t grf_gpio5_filter_con2;              /* address offset: 0x0c48 */
	uint32_t grf_gpio5_filter_con3;              /* address offset: 0x0c4c */
	uint32_t grf_gpio5_filter_con4;              /* address offset: 0x0c50 */
	uint32_t grf_gpio5_filter_con5;              /* address offset: 0x0c54 */
};

check_member(rv1126b_vccio5_ioc_reg, grf_gpio5_filter_con5, 0x0c54);

/* vccio6_ioc register structure define */
struct rv1126b_vccio6_ioc_reg {
	uint32_t reserved0000[48];                   /* address offset: 0x0000 */
	uint32_t gpio6a_iomux_sel_0;                 /* address offset: 0x00c0 */
	uint32_t gpio6a_iomux_sel_1;                 /* address offset: 0x00c4 */
	uint32_t gpio6b_iomux_sel_0;                 /* address offset: 0x00c8 */
	uint32_t gpio6b_iomux_sel_1;                 /* address offset: 0x00cc */
	uint32_t gpio6c_iomux_sel_0;                 /* address offset: 0x00d0 */
	uint32_t reserved00d4[107];                  /* address offset: 0x00d4 */
	uint32_t gpio6a_ds_0;                        /* address offset: 0x0280 */
	uint32_t gpio6a_ds_1;                        /* address offset: 0x0284 */
	uint32_t gpio6a_ds_2;                        /* address offset: 0x0288 */
	uint32_t gpio6a_ds_3;                        /* address offset: 0x028c */
	uint32_t gpio6b_ds_0;                        /* address offset: 0x0290 */
	uint32_t gpio6b_ds_1;                        /* address offset: 0x0294 */
	uint32_t gpio6b_ds_2;                        /* address offset: 0x0298 */
	uint32_t gpio6b_ds_3;                        /* address offset: 0x029c */
	uint32_t gpio6c_ds_0;                        /* address offset: 0x02a0 */
	uint32_t gpio6c_ds_1;                        /* address offset: 0x02a4 */
	uint32_t reserved02a8[46];                   /* address offset: 0x02a8 */
	uint32_t gpio6a_pull;                        /* address offset: 0x0360 */
	uint32_t gpio6b_pull;                        /* address offset: 0x0364 */
	uint32_t gpio6c_pull;                        /* address offset: 0x0368 */
	uint32_t reserved036c[61];                   /* address offset: 0x036c */
	uint32_t gpio6a_ie;                          /* address offset: 0x0460 */
	uint32_t gpio6b_ie;                          /* address offset: 0x0464 */
	uint32_t gpio6c_ie;                          /* address offset: 0x0468 */
	uint32_t reserved046c[61];                   /* address offset: 0x046c */
	uint32_t gpio6a_smt;                         /* address offset: 0x0560 */
	uint32_t gpio6b_smt;                         /* address offset: 0x0564 */
	uint32_t gpio6c_smt;                         /* address offset: 0x0568 */
	uint32_t reserved056c[61];                   /* address offset: 0x056c */
	uint32_t gpio6a_sus;                         /* address offset: 0x0660 */
	uint32_t gpio6b_sus;                         /* address offset: 0x0664 */
	uint32_t gpio6c_sus;                         /* address offset: 0x0668 */
	uint32_t reserved066c[61];                   /* address offset: 0x066c */
	uint32_t gpio6a_sl;                          /* address offset: 0x0760 */
	uint32_t gpio6b_sl;                          /* address offset: 0x0764 */
	uint32_t gpio6c_sl;                          /* address offset: 0x0768 */
	uint32_t reserved076c[61];                   /* address offset: 0x076c */
	uint32_t gpio6a_od;                          /* address offset: 0x0860 */
	uint32_t gpio6b_od;                          /* address offset: 0x0864 */
	uint32_t gpio6c_od;                          /* address offset: 0x0868 */
	uint32_t reserved086c[44];                   /* address offset: 0x086c */
	uint32_t io_vsel;                            /* address offset: 0x091c */
	uint32_t reserved0920[157];                  /* address offset: 0x0920 */
	uint32_t grf_vicif_m0_con;                   /* address offset: 0x0b94 */
	uint32_t reserved0b98[2];                    /* address offset: 0x0b98 */
	uint32_t grf_gmacio_m0_con0;                 /* address offset: 0x0ba0 */
	uint32_t grf_gmacio_m0_con1;                 /* address offset: 0x0ba4 */
	uint32_t reserved0ba8[46];                   /* address offset: 0x0ba8 */
	uint32_t grf_gpio6_filter_con0;              /* address offset: 0x0c60 */
	uint32_t grf_gpio6_filter_con1;              /* address offset: 0x0c64 */
	uint32_t grf_gpio6_filter_con2;              /* address offset: 0x0c68 */
	uint32_t grf_gpio6_filter_con3;              /* address offset: 0x0c6c */
	uint32_t grf_gpio6_filter_con4;              /* address offset: 0x0c70 */
	uint32_t grf_gpio6_filter_con5;              /* address offset: 0x0c74 */
};

check_member(rv1126b_vccio6_ioc_reg, grf_gpio6_filter_con5, 0x0c74);

/* vccio7_ioc register structure define */
struct rv1126b_vccio7_ioc_reg {
	uint32_t reserved0000[56];                   /* address offset: 0x0000 */
	uint32_t gpio7a_iomux_sel_0;                 /* address offset: 0x00e0 */
	uint32_t gpio7a_iomux_sel_1;                 /* address offset: 0x00e4 */
	uint32_t gpio7b_iomux_sel_0;                 /* address offset: 0x00e8 */
	uint32_t reserved00ec[117];                  /* address offset: 0x00ec */
	uint32_t gpio7a_ds_0;                        /* address offset: 0x02c0 */
	uint32_t gpio7a_ds_1;                        /* address offset: 0x02c4 */
	uint32_t gpio7a_ds_2;                        /* address offset: 0x02c8 */
	uint32_t gpio7a_ds_3;                        /* address offset: 0x02cc */
	uint32_t gpio7b_ds_0;                        /* address offset: 0x02d0 */
	uint32_t reserved02d4[39];                   /* address offset: 0x02d4 */
	uint32_t gpio7a_pull;                        /* address offset: 0x0370 */
	uint32_t gpio7b_pull;                        /* address offset: 0x0374 */
	uint32_t reserved0378[62];                   /* address offset: 0x0378 */
	uint32_t gpio7a_ie;                          /* address offset: 0x0470 */
	uint32_t gpio7b_ie;                          /* address offset: 0x0474 */
	uint32_t reserved0478[62];                   /* address offset: 0x0478 */
	uint32_t gpio7a_smt;                         /* address offset: 0x0570 */
	uint32_t gpio7b_smt;                         /* address offset: 0x0574 */
	uint32_t reserved0578[62];                   /* address offset: 0x0578 */
	uint32_t gpio7a_sus;                         /* address offset: 0x0670 */
	uint32_t gpio7b_sus;                         /* address offset: 0x0674 */
	uint32_t reserved0678[62];                   /* address offset: 0x0678 */
	uint32_t gpio7a_sl;                          /* address offset: 0x0770 */
	uint32_t gpio7b_sl;                          /* address offset: 0x0774 */
	uint32_t reserved0778[62];                   /* address offset: 0x0778 */
	uint32_t gpio7a_od;                          /* address offset: 0x0870 */
	uint32_t gpio7b_od;                          /* address offset: 0x0874 */
	uint32_t reserved0878[42];                   /* address offset: 0x0878 */
	uint32_t io_vsel;                            /* address offset: 0x0920 */
	uint32_t reserved0924[215];                  /* address offset: 0x0924 */
	uint32_t grf_gpio7_filter_con0;              /* address offset: 0x0c80 */
	uint32_t grf_gpio7_filter_con1;              /* address offset: 0x0c84 */
	uint32_t grf_gpio7_filter_con2;              /* address offset: 0x0c88 */
	uint32_t grf_gpio7_filter_con3;              /* address offset: 0x0c8c */
	uint32_t grf_gpio7_filter_con4;              /* address offset: 0x0c90 */
	uint32_t grf_gpio7_filter_con5;              /* address offset: 0x0c94 */
	uint32_t reserved0c98[2];                    /* address offset: 0x0c98 */
	uint32_t grf_dsm_ioc_con;                    /* address offset: 0x0ca0 */
};

check_member(rv1126b_vccio7_ioc_reg, grf_dsm_ioc_con, 0x0ca0);

#endif /*  _ASM_ARCH_GRF_RV1126B_H  */
