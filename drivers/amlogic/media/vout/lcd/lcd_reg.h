/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __AML_LCD_REG_H__
#define __AML_LCD_REG_H__
#include <asm/arch/cpu.h>
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/vpu_regs.h>
#include "edp_tx_reg.h"
#include "lcd_dummy_reg.h"

/* ********************************
 * register define
 * ********************************* */
/* base & offset */
#define REG_OFFSET_CBUS(reg)            ((reg) << 2)
#define REG_OFFSET_VCBUS(reg)           ((reg) << 2)
#define REG_OFFSET_DSI_HOST(reg)        ((reg > 0x10000) ? (reg & 0xfff) : ((reg & 0xff) << 2))
#define REG_OFFSET_DSI_PHY(reg)         (reg & 0xfff)
#define REG_OFFSET_TCON_APB(reg)        ((reg) << 2)
#define REG_OFFSET_TCON_APB_BYTE(reg)   (reg)

/* memory mapping */
#define REG_ADDR_AOBUS(reg)             (reg + 0L)
#define REG_ADDR_PERIPHS(reg)           (reg + 0L)
#define REG_ADDR_CBUS(reg)              ((reg > 0x10000) ? (reg + 0L) : \
					(REG_BASE_CBUS + REG_OFFSET_CBUS(reg)))
#define REG_ADDR_RESET(reg)             (reg + 0L)
#define REG_ADDR_HIU(reg)               (reg + 0L)
#define REG_ADDR_COMBO(reg)		(reg + 0L)
#define REG_ADDR_VCBUS(reg)             ((reg > 0x10000) ? (reg + 0L) : \
				(REG_BASE_VCBUS + REG_OFFSET_VCBUS(reg)))
#define REG_ADDR_DSI_HOST(reg)          (MIPI_DSI_BASE + REG_OFFSET_DSI_HOST(reg))
#define REG_ADDR_DSI_PHY(reg)           (MIPI_DSI_PHY_BASE + REG_OFFSET_DSI_PHY(reg))
#define REG_ADDR_DSI_B_HOST(reg)        (MIPI_DSI_B_BASE + REG_OFFSET_DSI_HOST(reg))
#define REG_ADDR_DSI_B_PHY(reg)         (MIPI_DSI_B_PHY_BASE + REG_OFFSET_DSI_PHY(reg))
#define REG_ADDR_TCON_APB(reg)          (REG_TCON_APB_BASE + REG_OFFSET_TCON_APB(reg))
#define REG_ADDR_TCON_APB_BYTE(reg)     (REG_TCON_APB_BASE + REG_OFFSET_TCON_APB_BYTE(reg))

/*#define HHI_VIID_CLK_DIV     	0x4a*/
#define DAC0_CLK_SEL           28
#define DAC1_CLK_SEL           24
#define DAC2_CLK_SEL           20
#define VCLK2_XD_RST           17
#define VCLK2_XD_EN            16
#define ENCL_CLK_SEL           12
#define VCLK2_XD                0

/*#define HHI_VIID_CLK_CNTL    	0x4b*/
#define VCLK2_EN               19
#define VCLK2_CLK_IN_SEL       16
#define VCLK2_SOFT_RST         15
#define VCLK2_DIV12_EN          4
#define VCLK2_DIV6_EN           3
#define VCLK2_DIV4_EN           2
#define VCLK2_DIV2_EN           1
#define VCLK2_DIV1_EN           0

/*#define HHI_VIID_DIVIDER_CNTL  0x4c*/
#define DIV_CLK_IN_EN          16
#define DIV_CLK_SEL            15
#define DIV_POST_TCNT          12
#define DIV_LVDS_CLK_EN        11
#define DIV_LVDS_DIV2          10
#define DIV_POST_SEL            8
#define DIV_POST_SOFT_RST       7
#define DIV_PRE_SEL             4
#define DIV_PRE_SOFT_RST        3
#define DIV_POST_RST            1
#define DIV_PRE_RST             0

/*#define HHI_VID_CLK_DIV        0x59*/
#define ENCI_CLK_SEL           28
#define ENCP_CLK_SEL           24
#define ENCT_CLK_SEL           20
#define VCLK_XD_RST            17
#define VCLK_XD_EN             16
#define ENCL_CLK_SEL           12
#define VCLK_XD1                8
#define VCLK_XD0                0

/*#define HHI_VID_CLK_CNTL2        0x65*/
#define HDMI_TX_PIXEL_GATE_VCLK  5
#define VDAC_GATE_VCLK           4
#define ENCL_GATE_VCLK           3
#define ENCP_GATE_VCLK           2
#define ENCT_GATE_VCLK           1
#define ENCI_GATE_VCLK           0

//#define LCD_REG_DEBUG		1

/* ********************************
 * register access api
 * ********************************* */
/* use offset address */
static inline unsigned int lcd_combo_dphy_read(unsigned int reg)
{
	unsigned int val;

	val = *(volatile unsigned int *)(REG_ADDR_COMBO(reg));
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);

	return val;
};

static inline void lcd_combo_dphy_write(unsigned int reg, unsigned int val)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);
	*(volatile unsigned int *)REG_ADDR_COMBO(reg) = (val);
};

static inline void lcd_combo_dphy_setb(unsigned int reg, unsigned int val,
				   unsigned int start, unsigned int len)
{
	lcd_combo_dphy_write(reg, ((lcd_combo_dphy_read(reg) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

static inline unsigned int lcd_combo_dphy_getb(unsigned int reg,
					   unsigned int start,
					   unsigned int len)
{
	return (lcd_combo_dphy_read(reg) >> (start)) & ((1L << (len)) - 1);
}

static inline void lcd_combo_dphy_set_mask(unsigned int reg, unsigned int _mask)
{
	lcd_combo_dphy_write(reg, (lcd_combo_dphy_read(reg) | (_mask)));
}

static inline void lcd_combo_dphy_clr_mask(unsigned int reg, unsigned int _mask)
{
	lcd_combo_dphy_write(reg, (lcd_combo_dphy_read(reg) & (~(_mask))));
}

static inline unsigned int lcd_clk_read(unsigned int reg)
{
	unsigned int val;

	val = *(volatile unsigned int *)(REG_ADDR_HIU(reg));
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);

	return val;
};

static inline void lcd_clk_write(unsigned int reg, unsigned int val)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);
	*(volatile unsigned int *)REG_ADDR_HIU(reg) = (val);
};

static inline void lcd_clk_setb(unsigned int reg, unsigned int val,
		unsigned int start, unsigned int len)
{
	lcd_clk_write(reg, ((lcd_clk_read(reg) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

static inline unsigned int lcd_clk_getb(unsigned int reg,
		unsigned int start, unsigned int len)
{
	return (lcd_clk_read(reg) >> (start)) & ((1L << (len)) - 1);
}

static inline void lcd_clk_set_mask(unsigned int reg, unsigned int _mask)
{
	lcd_clk_write(reg, (lcd_clk_read(reg) | (_mask)));
}

static inline void lcd_clk_clr_mask(unsigned int reg, unsigned int _mask)
{
	lcd_clk_write(reg, (lcd_clk_read(reg) & (~(_mask))));
}

static inline unsigned int lcd_ana_read(unsigned int reg)
{
	unsigned int val;

	val = *(volatile unsigned int *)(REG_ADDR_HIU(reg));
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);

	return val;
};

static inline void lcd_ana_write(unsigned int reg, unsigned int val)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);
	*(volatile unsigned int *)REG_ADDR_HIU(reg) = (val);
};

static inline void lcd_ana_setb(unsigned int reg, unsigned int val,
		unsigned int start, unsigned int len)
{
	lcd_ana_write(reg, ((lcd_ana_read(reg) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

static inline unsigned int lcd_ana_getb(unsigned int reg,
		unsigned int start, unsigned int len)
{
	return (lcd_ana_read(reg) >> (start)) & ((1L << (len)) - 1);
}

static inline void lcd_ana_set_mask(unsigned int reg, unsigned int _mask)
{
	lcd_ana_write(reg, (lcd_ana_read(reg) | (_mask)));
}

static inline void lcd_ana_clr_mask(unsigned int reg, unsigned int _mask)
{
	lcd_ana_write(reg, (lcd_ana_read(reg) & (~(_mask))));
}

static inline unsigned int lcd_cbus_read(unsigned int reg)
{
	unsigned int val;

	val = (*(volatile unsigned int *)REG_ADDR_CBUS(reg));
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);

	return val;
};

static inline void lcd_cbus_write(unsigned int reg, unsigned int val)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);
	*(volatile unsigned int *)REG_ADDR_CBUS(reg) = (val);
};

static inline void lcd_cbus_setb(unsigned int reg, unsigned int val,
		unsigned int start, unsigned int len)
{
	lcd_cbus_write(reg, ((lcd_cbus_read(reg) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

static inline unsigned int lcd_cbus_getb(unsigned int reg,
		unsigned int start, unsigned int len)
{
	return (lcd_cbus_read(reg) >> (start)) & ((1L << (len)) - 1);
}

static inline void lcd_cbus_set_mask(unsigned int reg, unsigned int _mask)
{
	lcd_cbus_write(reg, (lcd_cbus_read(reg) | (_mask)));
}

static inline void lcd_cbus_clr_mask(unsigned int reg, unsigned int _mask)
{
	lcd_cbus_write(reg, (lcd_cbus_read(reg) & (~(_mask))));
}

static inline unsigned int lcd_reset_read(unsigned int reg)
{
	unsigned int val;

	val = (*(volatile unsigned int *)REG_ADDR_RESET(reg));
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);

	return val;
};

static inline void lcd_reset_write(unsigned int reg, unsigned int val)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);
	*(volatile unsigned int *)REG_ADDR_RESET(reg) = (val);
};

static inline void lcd_reset_setb(unsigned int reg, unsigned int val,
		unsigned int start, unsigned int len)
{
	lcd_reset_write(reg, ((lcd_reset_read(reg) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

static inline unsigned int lcd_vcbus_read(unsigned int reg)
{
	unsigned int val;

	val = (*(volatile unsigned int *)REG_ADDR_VCBUS(reg));
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);

	return val;
}

static inline void lcd_vcbus_write(unsigned int reg, unsigned int val)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);
	(*(volatile unsigned int *)REG_ADDR_VCBUS(reg)) = (val);
}

static inline void lcd_vcbus_setb(unsigned int reg, unsigned int val,
		unsigned int start, unsigned int len)
{
	lcd_vcbus_write(reg, ((lcd_vcbus_read(reg) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

static inline unsigned int lcd_vcbus_getb(unsigned int reg,
		unsigned int start, unsigned int len)
{
	return (lcd_vcbus_read(reg) >> (start)) & ((1L << (len)) - 1);
}

static inline void lcd_vcbus_set_mask(unsigned int reg, unsigned int _mask)
{
	lcd_vcbus_write(reg, (lcd_vcbus_read(reg) | (_mask)));
}

static inline void lcd_vcbus_clr_mask(unsigned int reg, unsigned int _mask)
{
	lcd_vcbus_write(reg, (lcd_vcbus_read(reg) & (~(_mask))));
}

static inline unsigned int lcd_aobus_read(unsigned int reg)
{
	unsigned int val;

	val = (*(volatile unsigned int *)REG_ADDR_AOBUS(reg));
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);

	return val;
};

static inline void lcd_aobus_write(unsigned int reg, unsigned int val)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);
	*(volatile unsigned int *)REG_ADDR_AOBUS(reg) = (val);
};

static inline void lcd_aobus_setb(unsigned int reg, unsigned int val,
		unsigned int start, unsigned int len)
{
	lcd_aobus_write(reg, ((lcd_aobus_read(reg) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

static inline unsigned int lcd_aobus_getb(unsigned int reg,
		unsigned int start, unsigned int len)
{
	return (lcd_aobus_read(reg) & (((1L << (len)) - 1) << (start)));
}

static inline unsigned int lcd_periphs_read(unsigned int reg)
{
	unsigned int val;

	val = (*(volatile unsigned int *)REG_ADDR_PERIPHS(reg));
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);

	return val;
};

static inline void lcd_periphs_write(unsigned int reg, unsigned int val)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);
	*(volatile unsigned int *)REG_ADDR_PERIPHS(reg) = (val);
};

static inline void lcd_periphs_setb(unsigned int reg, unsigned int val,
		unsigned int start, unsigned int len)
{
	lcd_periphs_write(reg, ((lcd_periphs_read(reg) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

static inline unsigned int lcd_periphs_getb(unsigned int reg,
		unsigned int start, unsigned int len)
{
	return (lcd_periphs_read(reg) & (((1L << (len)) - 1) << (start)));
}

static inline void lcd_pinmux_set_mask(unsigned int n, unsigned int _mask)
{
	unsigned int reg = PERIPHS_PIN_MUX_0;

	reg += (n << 2);
	lcd_periphs_write(reg, (lcd_periphs_read(reg) | (_mask)));
}

static inline void lcd_pinmux_clr_mask(unsigned int n, unsigned int _mask)
{
	unsigned int reg = PERIPHS_PIN_MUX_0;

	reg += (n << 2);
	lcd_periphs_write(reg, (lcd_periphs_read(reg) & (~(_mask))));
}

static inline unsigned int dsi_host_read(int index, unsigned int reg)
{
	unsigned long paddr;
	unsigned int val;

	if (index)
		paddr = (REG_ADDR_DSI_B_HOST(reg));
	else
		paddr = (REG_ADDR_DSI_HOST(reg));
	val = *(volatile unsigned int *)paddr;
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08lx]=0x%08x\n", __func__, paddr, val);

	return val;
}

static inline void dsi_host_write(int index, unsigned int reg, unsigned int val)
{
	unsigned long paddr;

	if (index)
		paddr = REG_ADDR_DSI_B_HOST(reg);
	else
		paddr = REG_ADDR_DSI_HOST(reg);
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08lx]=0x%08x\n", __func__, paddr, val);
	*(volatile unsigned int *)paddr = (val);
}

static inline void dsi_host_setb(int index, unsigned int reg, unsigned int val,
		unsigned int start, unsigned int len)
{
	dsi_host_write(index, reg, ((dsi_host_read(index, reg) &
			~(((1L << (len)) - 1) << (start))) |
			(((val) & ((1L << (len)) - 1)) << (start))));
}

static inline unsigned int dsi_host_getb(int index, unsigned int reg,
		unsigned int start, unsigned int len)
{
	return (dsi_host_read(index, reg) >> (start)) & ((1L << (len)) - 1);
}

static inline void dsi_host_set_mask(int index, unsigned int reg, unsigned int _mask)
{
	dsi_host_write(index, reg, (dsi_host_read(index, reg) | (_mask)));
}

static inline void dsi_host_clr_mask(int index, unsigned int reg, unsigned int _mask)
{
	dsi_host_write(index, reg, (dsi_host_read(index, reg) & (~(_mask))));
}

static inline unsigned int dsi_phy_read(int index, unsigned int reg)
{
	unsigned long paddr;
	unsigned int val;

	if (index)
		paddr = (REG_ADDR_DSI_B_PHY(reg));
	else
		paddr = (REG_ADDR_DSI_PHY(reg));
	val = *(volatile unsigned int *)paddr;
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08lx]=0x%08x\n", __func__, paddr, val);

	return val;
};

static inline void dsi_phy_write(int index, unsigned int reg, unsigned int val)
{
	unsigned long paddr;

	if (index)
		paddr = REG_ADDR_DSI_B_PHY(reg);
	else
		paddr = REG_ADDR_DSI_PHY(reg);
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08lx]=0x%08x\n", __func__, paddr, val);
	*(volatile unsigned int *)paddr = (val);
};

static inline void dsi_phy_setb(int index, unsigned int reg, unsigned int val,
		unsigned int start, unsigned int len)
{
	dsi_phy_write(index, reg, ((dsi_phy_read(index, reg) &
			~(((1L << (len)) - 1) << (start))) |
			(((val) & ((1L << (len)) - 1)) << (start))));
}

static inline unsigned int dsi_phy_getb(int index, unsigned int reg,
		unsigned int start, unsigned int len)
{
	return (dsi_phy_read(index, reg) >> (start)) & ((1L << (len)) - 1);
}

static inline void dsi_phy_set_mask(int index, unsigned int reg, unsigned int _mask)
{
	dsi_phy_write(index, reg, (dsi_phy_read(index, reg) | (_mask)));
}

static inline void dsi_phy_clr_mask(int index, unsigned int reg, unsigned int _mask)
{
	dsi_phy_write(index, reg, (dsi_phy_read(index, reg) & (~(_mask))));
}

static inline unsigned int lcd_tcon_read(unsigned int reg)
{
	unsigned int val;

	val = *(volatile unsigned int *)(REG_ADDR_TCON_APB(reg));
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);

	return val;
};

static inline void lcd_tcon_write(unsigned int reg, unsigned int val)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%08x\n", __func__, reg, val);
	*(volatile unsigned int *)REG_ADDR_TCON_APB(reg) = (val);
};

static inline void lcd_tcon_setb(unsigned int reg, unsigned int val,
		unsigned int start, unsigned int len)
{
	lcd_tcon_write(reg, ((lcd_tcon_read(reg) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

static inline unsigned int lcd_tcon_getb(unsigned int reg,
		unsigned int start, unsigned int len)
{
	return (lcd_tcon_read(reg) >> (start)) & ((1L << (len)) - 1);
}

static inline void lcd_tcon_set_mask(unsigned int reg, unsigned int _mask)
{
	lcd_tcon_write(reg, (lcd_tcon_read(reg) | (_mask)));
}

static inline void lcd_tcon_clr_mask(unsigned int reg, unsigned int _mask)
{
	lcd_tcon_write(reg, (lcd_tcon_read(reg) & (~(_mask))));
}

static inline void lcd_tcon_update_bits(unsigned int reg,
					unsigned int mask,
					unsigned int value)
{
	if (mask == 0xffffffff) {
		lcd_tcon_write(reg, value);
	} else {
		lcd_tcon_write(reg, (lcd_tcon_read(reg) & (~(mask))) |
			       (value & mask));
	}
}

static inline int lcd_tcon_check_bits(unsigned int reg,
				      unsigned int mask,
				      unsigned int value)
{
	unsigned int temp;

	temp = lcd_tcon_read(reg) & mask;
	if (value != temp)
		return -1;

	return 0;
}

static inline unsigned char lcd_tcon_read_byte(unsigned int reg)
{
	unsigned int val;

	val = *(volatile unsigned char *)(REG_ADDR_TCON_APB_BYTE(reg));
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%02x\n", __func__, reg, val);

	return (unsigned char)val;
};

static inline void lcd_tcon_write_byte(unsigned int reg, unsigned char val)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [0x%08x]=0x%02x\n", __func__, reg, val);
	*(volatile unsigned char *)REG_ADDR_TCON_APB_BYTE(reg) = (val);
};

static inline void lcd_tcon_setb_byte(unsigned int reg, unsigned char val,
				      unsigned int start, unsigned int len)
{
	lcd_tcon_write_byte(reg, ((lcd_tcon_read_byte(reg) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

static inline unsigned char lcd_tcon_getb_byte(unsigned int reg,
					       unsigned int start,
					       unsigned int len)
{
	return (lcd_tcon_read_byte(reg) >> (start)) & ((1L << (len)) - 1);
}

static inline void lcd_tcon_update_bits_byte(unsigned int reg,
					     unsigned char mask,
					     unsigned char value)
{
	if (mask == 0xff) {
		lcd_tcon_write_byte(reg, value);
	} else {
		lcd_tcon_write_byte(reg, (lcd_tcon_read_byte(reg) & (~(mask))) |
				    (value & mask));
	}
}

static inline int lcd_tcon_check_bits_byte(unsigned int reg,
					   unsigned char mask,
					   unsigned char value)
{
	unsigned char temp;

	temp = lcd_tcon_read_byte(reg) & mask;
	if ((value & mask) != temp)
		return -1;

	return 0;
}

static inline unsigned int dptx_reg_read(int index, unsigned int reg)
{
	unsigned long paddr;
	unsigned int val;

	if (index)
		paddr = EDPTX1_BASE + reg;
	else
		paddr = EDPTX0_BASE + reg;
	val = *(volatile unsigned int *)paddr;
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [%d][0x%04x]=0x%08x\n", __func__, index, reg, val);

	return val;
}

static inline void dptx_reg_write(int index, unsigned int reg, unsigned int val)
{
	unsigned long paddr;

	if (index)
		paddr = EDPTX1_BASE + reg;
	else
		paddr = EDPTX0_BASE + reg;
	*(volatile unsigned int *)paddr = (val);
	if (lcd_debug_print_flag & LCD_DBG_PR_REG)
		printf("%s: [%d][0x%04x]=0x%08x\n", __func__, index, reg, val);
}

static inline void dptx_reg_setb(int index, unsigned int reg,
		unsigned int val, unsigned int start, unsigned int len)
{
	dptx_reg_write(index, reg,
		((dptx_reg_read(index, reg) & ~(((1L << (len)) - 1) << (start))) |
		(((val) & ((1L << (len)) - 1)) << (start))));
}

static inline unsigned int dptx_reg_getb(int index, unsigned int reg,
		unsigned int start, unsigned int len)
{
	return (dptx_reg_read(index, reg) >> (start)) & ((1L << (len)) - 1);
}

#endif
