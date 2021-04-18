// ----------------------------------------------------------------------
// vpu related regs header
//
// which is used for vpu module undefined regs in register.h
//
// ----------------------------------------------------------------------
//
#ifndef __INC_AML_VPU_REG_H__
#define __INC_AML_VPU_REG_H__

/*tcon*/
#define REG_TCON_APB_BASE                          (0xfe3b0000L)
#define TCON_CORE_REG_START                        0x0000
#define TCON_CTRL_TIMING_BASE                      0x01b0
#define TCON_TOP_CTRL                              0x2000
#define TCON_RGB_IN_MUX                            0x2001
#define TCON_OUT_CH_SEL0                           0x2002
#define TCON_OUT_CH_SEL1                           0x2003
#define TCON_I2C_DEGLITCH_CNTL                     0x2004
#define TCON_STATUS0                               0x2008 /* read only */
#define TCON_PLLLOCK_CNTL                          0x2009
#define TCON_PLLLCK_RST_CNT                        0x200a
#define TCON_RST_CTRL                              0x200b
#define TCON_AXI_OFST                              0x200c
#define TCON_DDRIF_CTRL0                           0x200d
#define TCON_CLK_CTRL                              0x200e
#define TCON_DDRIF_CTRL1                           0x200f
#define TCON_STATUS1                               0x2010 /* read only */
#define TCON_DDRIF_CTRL2                           0x2011
#define TCON_INTR_MASKN                            0x2022
#define TCON_INTR                                  0x2023 /* read only */

#endif /* __INC_AML_VPU_REG_H__ */
