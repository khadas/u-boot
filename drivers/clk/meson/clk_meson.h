/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef CLK_MESON_H
#define CLK_MESON_H

/* Gate Structure */
struct meson_gate {
	unsigned int index;
	unsigned int reg;
	unsigned int bit;
};

/* Mux Structure */
struct meson_mux {
	unsigned int index;
	unsigned int reg;
	unsigned int shift;
	unsigned int mask;
	unsigned int *table;
	unsigned int table_size;
};

/* Div Structure */
struct meson_div {
	unsigned int index;
	unsigned int reg;
	unsigned int shift;
	unsigned int width;
	unsigned int parent_index;
};

/* PLL Parameters */
struct parm {
	u16 reg_off;
	u8 shift;
	u8 width;
};

#define PMASK(width)                    GENMASK(width - 1, 0)
#define SETPMASK(width, shift)          GENMASK(shift + width - 1, shift)
#define CLRPMASK(width, shift)          (~SETPMASK(width, shift))

#define PARM_GET(width, shift, reg)                                     \
	(((reg) & SETPMASK(width, shift)) >> (shift))
#define PARM_SET(width, shift, reg, val)                                \
	(((reg) & CLRPMASK(width, shift)) | ((val) << (shift)))

#define CLK81_RATE	166666667

struct meson_clk {
	void __iomem *addr;
	struct clk clkin;
	u32 actual_rate;
};

/* MPLL Parameters */

#define SDM_DEN		16384
#define N2_MIN  	4
#define N2_MAX  	511

int meson_set_gate_by_id(struct clk *clk, struct meson_gate *gate_arr,
				unsigned int arr_size, bool on);

int meson_mux_set_parent_by_id(struct clk *clk, struct meson_mux *mux_arr,
				unsigned int arr_size, struct clk *parent_clk);

int meson_clk_get_mux_parent(struct clk *clk, struct meson_mux *mux_arr,
				unsigned int arr_size, ulong parent_id);

int meson_clk_set_div(struct meson_clk *priv, struct meson_div *div,
				unsigned int div_val);

#endif
