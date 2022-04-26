/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __SPIFC_H__
#define __SPIFC_H__

/*
 * @reg: controller registers base address.
 * @mem_map: memory_mapped for read operations.
 * @core: clk source, usually clk_81, 166M max.
 * @speed: spi bus frequency, and you should know
 *			we use clk_81 as the clk source now, max
 *	  		speed is 166M.
 * @io_num: max io number, SIO0 SIO1 SIO2 SIO3.
 * @max_cs: the max slave device number, we support
 * 			spi-nor, spi-nand now.
 * @cs_gpios: gpio array, we use the cs pin as
 *            gpio, cause the spifc controller
 *            can not hold enough time sometimes.
 */

struct spifc_platdata {
	ulong reg;
	ulong mem_map;
	u32 speed;
	u32 mode;
	u32 io_num;
	u32 max_cs;
};

/* this is cs pin number, not slave device cs number */
/* #define SPIFC_MAX_CS	1 */
#define SPIFC_BUS_NUM	0
/* slave device cs number */
#define SPIFC_SNOR_CS	0
#define SPIFC_SNAND_CS	1

#endif /* __SPIFC_H__ */