/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/arch-gxtvbb/ddr_define.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#define CONFIG_DDR_TYPE_DDR3				0
#define CONFIG_DDR_TYPE_LPDDR2				1
#define CONFIG_DDR_TYPE_LPDDR3				2

/* ddr channel defines */
#define CONFIG_DDR01_TWO_CHANNEL			1
#define CONFIG_DDR0_ONLY_32BIT				2
#define CONFIG_DDR0_ONLY_16BIT				3
#define CONFIG_DDR01_TWO_CHANNEL_16BIT		4 //two channels both working at 16bit mode
#define CONFIG_DDR1_ONLY_32BIT              5

#define CFG_DDR_BASE_ADDR					0X0
#define CFG_DDR_START_OFFSET				0X01000000 //SKIP 16MB

/* ddr type identifier */
#define CONFIG_DDR_TIMMING_LPDDR2			0x02
#define CONFIG_DDR_TIMMING_LPDDR3			0x03
#define CONFIG_DDR_TIMMING_DDR3_7			0x07
#define CONFIG_DDR_TIMMING_DDR3_9			0x09
#define CONFIG_DDR_TIMMING_DDR3_11			0x0B
#define CONFIG_DDR_TIMMING_DDR3_12			0x0C
#define CONFIG_DDR_TIMMING_DDR3_13			0x0D
#define CONFIG_DDR_TIMMING_DDR3_14			0x0E

#define DDR_USE_1_CHANNEL(chl_set)	((CONFIG_DDR0_ONLY_16BIT == chl_set)||\
									 (CONFIG_DDR0_ONLY_32BIT == chl_set)||\
									 (CONFIG_DDR1_ONLY_32BIT == chl_set))

#define DDR_USE_2_CHANNEL(chl_set)	((CONFIG_DDR01_TWO_CHANNEL == chl_set))

/* how to add a new ddr function?
   1. add CONFIG_CMD_DDR_XXX in (config).h file
   2. add define in this file.
      2.1 add
        #ifndef CONFIG_CMD_DDR_XXX
        #define CONFIG_CMD_DDR_XXX 0
        #endif
      2.2 add
        #define DDR_FUNC_XXX (CONFIG_FUNC_XXX<<X)
      2.3 add DDR_FUNC_XXX |\ in DDR_FUNC
   3. add same define and parser in bl2 code
   */
/* 2.1, 2,2, 2,3 example */
/*
#ifndef CONFIG_CMD_DDR_D2PLL
#define CONFIG_CMD_DDR_D2PLL				0
#endif
#define DDR_FUNC_D2PLL						(CONFIG_CMD_DDR_D2PLL<<0)
#define DDR_FUNC							(EXISTING_FUNCTIONS) |\
											(DDR_FUNC_D2PLL)
*/

#ifndef CONFIG_CMD_DDR_D2PLL
#define CONFIG_CMD_DDR_D2PLL				0
#endif
#define DDR_FUNC_D2PLL						(CONFIG_CMD_DDR_D2PLL<<0)

#define DDR_FUNC							(DDR_FUNC_D2PLL)