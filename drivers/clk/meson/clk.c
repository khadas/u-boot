// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/io.h>
#include <clk-uclass.h>
#include <dm.h>
#include "clk_meson.h"

/*
 * set meson gates bit
 */
int meson_set_gate_by_id(struct clk *clk, struct meson_gate *gate_arr,
			unsigned int arr_size, bool on)
{
	struct meson_clk *priv = dev_get_priv(clk->dev);
	struct meson_gate *gate;
	unsigned i, index = 0;
	unsigned long id;

	debug("%s: %sabling %ld\n", __func__, on ? "en" : "dis", id);

	/* Find the array by id */
	id = clk->id;
	for (i = 0; i < arr_size; i++) {
		if (id == gate_arr[i].index)
			index = i;
	}
	gate = &gate_arr[index];
	if (gate->reg == 0)
		return 0;

	debug("%s: really %sabling %ld\n", __func__, on ? "en" : "dis", id);
	clrsetbits_le32(priv->addr + gate->reg,
			BIT(gate->bit), on ? BIT(gate->bit) : 0);

	return 0;
}

/*
 *set the mux clock's parent clock
 */
int meson_mux_set_parent_by_id(struct clk *clk, struct meson_mux *mux_arr,
				unsigned int arr_size, struct clk*parent_clk)

{
	 struct meson_clk *priv = dev_get_priv(clk->dev);
	 struct meson_mux *mux;
	 unsigned int i, val;
	 unsigned int parent_index = 0;
	 unsigned int index = 0;
	 unsigned int *parents;

	/*1.find the index(the array)
	 *2.find the parent clock index in table
	*/
	for (i = 0; i < arr_size; i++) {
		if (clk->id == mux_arr[i].index)
			index = i;
	}
	mux = &mux_arr[index];
	parents = mux->table;
	for (i = 0; i < mux->table_size; i++) {
		if (parent_clk->id == parents[i])
			parent_index = i;
	}

	val = readl(priv->addr + mux->reg);
	val &= ~(mux->mask << mux->shift);
	val |= parent_index << mux->shift;
	writel(val, priv->addr + mux->reg);

	return 0;
}

/*
 * get the parent index in parent table
 */
int meson_clk_get_mux_parent(struct clk *clk, struct meson_mux *mux_arr,
				unsigned int arr_size, ulong parent_id)
{
	struct meson_clk *priv = dev_get_priv(clk->dev);
	struct meson_mux *mux = NULL;
	unsigned int *p;
	unsigned int i, val, mux_parent_id;

	for (i = 0; i < arr_size; i++) {
		if (parent_id == mux_arr[i].index)
			mux = &mux_arr[i];
	}
	val = (readl(priv->addr + mux->reg) >> mux->shift) & mux->mask;
	p = mux->table;
	mux_parent_id = p[val];

	return mux_parent_id;
}

int meson_clk_get_div_parent(struct clk *clk, struct meson_div *div_arr,
				unsigned int arr_size)
{
	unsigned int i;
	struct meson_div *div;
	ulong parent_index = 0;

	for (i = 0; i < arr_size; i++) {
		if (clk->id == div_arr[i].index) {
			div = &div_arr[i];
			parent_index = div->parent_index;
		}
	}

	return parent_index;
}

int meson_clk_set_div(struct meson_clk *priv, struct meson_div *div,
			unsigned int div_val)
{
	unsigned int val;
	unsigned int div_max = (1 << div->width) - 1;

	val = readl(priv->addr + div->reg);
	val &= ~(div_max << div->shift);
	val |= min_t(unsigned int, div_val, div_max) << div->shift;
	writel(val, priv->addr + div->reg);

	return 0;
}
