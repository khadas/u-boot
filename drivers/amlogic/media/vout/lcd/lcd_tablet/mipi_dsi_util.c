// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/gpio.h>
#include <fdtdec.h>
#ifdef CONFIG_SECURE_POWER_CONTROL
#include <asm/arch/pwr_ctrl.h>
#endif
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "../lcd_reg.h"
#include "../lcd_common.h"
#include "lcd_tablet.h"
#include "mipi_dsi_util.h"

/* *************************************************************
 * Define MIPI DSI Default config
 */
/* Range [0,3] */
#define MIPI_DSI_VIRTUAL_CHAN_ID        0
/* Define DSI command transfer type: high speed or low power */
#define MIPI_DSI_CMD_TRANS_TYPE         DCS_TRANS_LP
/* Define if DSI command need ack: req_ack or no_ack */
#define MIPI_DSI_DCS_ACK_TYPE           MIPI_DSI_DCS_NO_ACK
/* Applicable only to video mode. Define data transfer method:
 *    non-burst sync pulse; non-burst sync event; or burst.
 */

#define MIPI_DSI_COLOR_18BIT            COLOR_18BIT_CFG_2//COLOR_18BIT_CFG_1
#define MIPI_DSI_COLOR_24BIT            COLOR_24BIT
#define MIPI_DSI_TEAR_SWITCH            MIPI_DCS_DISABLE_TEAR
#define CMD_TIMEOUT_CNT                 5000
/* ************************************************************* */

static char *operation_mode_table[] = {
	"VIDEO",
	"COMMAND",
};

static char *video_mode_type_table[] = {
	"SYNC_PULSE",
	"SYNC_EVENT",
	"BURST_MODE",
};

static char *video_data_type_table[] = {
	"COLOR_16BIT_CFG_1",
	"COLOR_16BIT_CFG_2",
	"COLOR_16BIT_CFG_3",
	"COLOR_18BIT_CFG_1",
	"COLOR_18BIT_CFG_2(loosely)",
	"COLOR_24BIT",
	"COLOR_20BIT_LOOSE",
	"COLOR_24_BIT_YCBCR",
	"COLOR_16BIT_YCBCR",
	"COLOR_30BIT",
	"COLOR_36BIT",
	"COLOR_12BIT",
	"COLOR_RGB_111",
	"COLOR_RGB_332",
	"COLOR_RGB_444",
	"un-support type",
};

static char *phy_switch_table[] = {
	"AUTO",
	"STANDARD",
	"SLOW",
};

static struct dsi_phy_s dsi_phy_config;
static struct dsi_vid_s dsi_vconf;
static unsigned short dsi_rx_n;

static void mipi_dsi_init_table_print(struct dsi_config_s *dconf, int on_off)
{
	int i, j, n;
	int n_max;
	unsigned char *dsi_table;

	if (on_off) {
		if (dconf->dsi_init_on == NULL)
			return;
		dsi_table = dconf->dsi_init_on;
		n_max = DSI_INIT_ON_MAX;
		printf("DSI INIT ON:\n");
	} else {
		if (dconf->dsi_init_off == NULL)
			return;
		dsi_table = dconf->dsi_init_off;
		n_max = DSI_INIT_OFF_MAX;
		printf("DSI INIT OFF:\n");
	}
	i = 0;
	n = 0;
	while ((i + DSI_CMD_SIZE_INDEX) < n_max) {
		n = dsi_table[i+DSI_CMD_SIZE_INDEX];
		if (dsi_table[i] == LCD_EXT_CMD_TYPE_END) {
			if (n == 0xff) {
				printf("  0x%02x,0x%02x,\n",
					dsi_table[i], dsi_table[i+1]);
				break;
			}
			if (n == 0) {
				printf("  0x%02x,%d,\n",
					dsi_table[i], dsi_table[i+1]);
				break;
			}
			n = 0;
			printf("  0x%02x,%d,\n", dsi_table[i], dsi_table[i+1]);
		} else if ((dsi_table[i] == LCD_EXT_CMD_TYPE_GPIO) ||
			(dsi_table[i] == LCD_EXT_CMD_TYPE_DELAY)) {
			printf("  0x%02x,%d,", dsi_table[i], n);
			for (j = 0; j < n; j++)
				printf("%d,", dsi_table[i+2+j]);
			printf("\n");
		} else if ((dsi_table[i] & 0xf) == 0x0) {
			printf("dsi_init_%s wrong data_type: 0x%02x\n",
				on_off ? "on" : "off", dsi_table[i]);
			break;
		} else {
			printf("  0x%02x,%d,", dsi_table[i], n);
			for (j = 0; j < n; j++)
				printf("0x%02x,", dsi_table[i+2+j]);
			printf("\n");
		}
		i += (n + 2);
	}
}

static void mipi_dsi_dphy_print_info(struct lcd_config_s *pconf)
{
	unsigned int temp;

	temp = ((1000000 * 100) / (pconf->timing.bit_rate / 1000)) * 8;
	printf("MIPI DSI DPHY timing (unit: ns)\n"
		"  UI:                %d.%02d\n"
		"  LP TESC:           %d\n"
		"  LP LPX:            %d\n"
		"  LP TA_SURE:        %d\n"
		"  LP TA_GO:          %d\n"
		"  LP TA_GET:         %d\n"
		"  HS EXIT:           %d\n"
		"  HS TRAIL:          %d\n"
		"  HS ZERO:           %d\n"
		"  HS PREPARE:        %d\n"
		"  CLK TRAIL:         %d\n"
		"  CLK POST:          %d\n"
		"  CLK ZERO:          %d\n"
		"  CLK PREPARE:       %d\n"
		"  CLK PRE:           %d\n"
		"  INIT:              %d\n"
		"  WAKEUP:            %d\n"
		"  state_change:      %d\n\n",
		(temp / 8 / 100), ((temp / 8) % 100),
		(temp * dsi_phy_config.lp_tesc / 100),
		(temp * dsi_phy_config.lp_lpx / 100),
		(temp * dsi_phy_config.lp_ta_sure / 100),
		(temp * dsi_phy_config.lp_ta_go / 100),
		(temp * dsi_phy_config.lp_ta_get / 100),
		(temp * dsi_phy_config.hs_exit / 100),
		(temp * dsi_phy_config.hs_trail / 100),
		(temp * dsi_phy_config.hs_zero / 100),
		(temp * dsi_phy_config.hs_prepare / 100),
		(temp * dsi_phy_config.clk_trail / 100),
		(temp * dsi_phy_config.clk_post / 100),
		(temp * dsi_phy_config.clk_zero / 100),
		(temp * dsi_phy_config.clk_prepare / 100),
		(temp * dsi_phy_config.clk_pre / 100),
		(temp * dsi_phy_config.init / 100),
		(temp * dsi_phy_config.wakeup / 100),
		dsi_phy_config.state_change);
}

static void mipi_dsi_video_print_info(struct dsi_config_s *dconf)
{
	if (dconf->video_mode_type != BURST_MODE) {
		printf("MIPI DSI NON-BURST setting:\n"
			"  multi_pkt_en:          %d\n"
			"  vid_num_chunks:        %d\n"
			"  pixel_per_chunk:       %d\n"
			"  byte_per_chunk:        %d\n"
			"  vid_null_size:         %d\n\n",
			dsi_vconf.multi_pkt_en, dsi_vconf.vid_num_chunks,
			dsi_vconf.pixel_per_chunk, dsi_vconf.byte_per_chunk,
			dsi_vconf.vid_null_size);
	}
}

static void mipi_dsi_host_print_info(struct lcd_config_s *pconf)
{
	unsigned int esc_clk, factor;
	struct dsi_config_s *dconf;

	dconf = &pconf->control.mipi_cfg;
	esc_clk = pconf->timing.bit_rate / 8 / dsi_phy_config.lp_tesc;
	factor = dconf->factor_numerator;
	factor = ((factor * 1000 / dconf->factor_denominator) + 5) / 10;

	if (dconf->check_en) {
		printf("MIPI DSI check state:\n"
			"  check_reg:             0x%02x\n"
			"  check_cnt:             %d\n"
			"  check_state            %d\n\n",
			dconf->check_reg, dconf->check_cnt, dconf->check_state);
	}

	printf("MIPI DSI Config:\n"
		"  lane num:              %d\n"
		"  bit rate max:          %dMHz\n"
		"  bit rate range:        (%d~%d)kHz\n"
		"  bit rate:              %d.%03dMHz\n"
		"  pclk lanebyte factor:  %d(/100)\n"
		"  operation mode:\n"
		"      init:              %s(%d)\n"
		"      display:           %s(%d)\n"
		"  video mode type:       %s(%d)\n"
		"  clk always hs:         %d\n"
		"  phy switch:            %s(%d)\n"
		"  data format:           %s\n"
		"  lp escape clock:       %d.%03dMHz\n",
		dconf->lane_num, dconf->bit_rate_max,
		dconf->local_bit_rate_max, dconf->local_bit_rate_min,
		(pconf->timing.bit_rate / 1000000),
		(pconf->timing.bit_rate % 1000000) / 1000,
		factor,
		operation_mode_table[dconf->operation_mode_init],
		dconf->operation_mode_init,
		operation_mode_table[dconf->operation_mode_display],
		dconf->operation_mode_display,
		video_mode_type_table[dconf->video_mode_type],
		dconf->video_mode_type,
		dconf->clk_always_hs,
		phy_switch_table[dconf->phy_switch],
		dconf->phy_switch,
		video_data_type_table[dconf->dpi_data_format],
		(esc_clk / 1000000), (esc_clk % 1000000) / 1000);

	mipi_dsi_init_table_print(dconf, 1); /* dsi_init_on table */
	mipi_dsi_init_table_print(dconf, 0); /* dsi_init_off table */

	printf("extern init:             %d\n\n", dconf->extern_init);
}

void mipi_dsi_print_info(struct lcd_config_s *pconf)
{
	mipi_dsi_host_print_info(pconf);

	mipi_dsi_dphy_print_info(pconf);
	mipi_dsi_video_print_info(&pconf->control.mipi_cfg);
}

int lcd_mipi_dsi_init_table_detect(char *dtaddr, int nodeoffset,
				   struct dsi_config_s *dconf, int flag)
{
	unsigned char cmd_size, type;
	int i, j, max_len;
	unsigned char *table = NULL;
	char propname[20];
	char *propdata;

	if (flag) {
		if (dconf->dsi_init_on) {
			free(dconf->dsi_init_on);
			dconf->dsi_init_on = NULL;
		}
		max_len = DSI_INIT_ON_MAX;
		sprintf(propname, "dsi_init_on");
	} else {
		if (dconf->dsi_init_off) {
			free(dconf->dsi_init_off);
			dconf->dsi_init_off = NULL;
		}
		max_len = DSI_INIT_OFF_MAX;
		sprintf(propname, "dsi_init_off");
	}
	table = (unsigned char *)malloc(sizeof(unsigned char) * max_len);
	if (!table) {
		LCDERR("%s: Not enough memory\n", __func__);
		return -1;
	}
	table[0] = LCD_EXT_CMD_TYPE_END;
	table[1] = 0;

	if (!table) {
		table = (unsigned char *)malloc(sizeof(unsigned char) * max_len);
		if (!table) {
			LCDERR("%s: Not enough memory\n", __func__);
			return -1;
		}
	}

	i = 0;
	propdata = (char *)fdt_getprop(dtaddr, nodeoffset, propname, NULL);
	if (propdata == NULL) {
		LCDERR("get %s failed\n", propname);
		table[0] = 0xff;
		table[1] = 0;
		goto lcd_mipi_dsi_init_table_detect_done;
	}

	while ((i + DSI_CMD_SIZE_INDEX) < max_len) {
		table[i] = (unsigned char)(be32_to_cpup((((u32 *)propdata) + i)));
		table[i + DSI_CMD_SIZE_INDEX] =
			(unsigned char)(be32_to_cpup((((u32 *)propdata) + i + 1)));
		type = table[i];
		cmd_size = table[i + DSI_CMD_SIZE_INDEX];

		if (type == LCD_EXT_CMD_TYPE_END) {
			if ((cmd_size == 0xff) || (cmd_size == 0))
				break;
			cmd_size = 0;
		}
		if (cmd_size == 0) {
			i += 2;
			continue;
		}
		if ((i + 2 + cmd_size) > max_len) {
			LCDERR("%s cmd_size out of support\n", propname);
			table[i] = LCD_EXT_CMD_TYPE_END;
			table[i + 1] = 0;
			break;
		}

		for (j = 0; j < cmd_size; j++) {
			table[i + 2 + j] =
				(unsigned char)(be32_to_cpup((((u32 *)propdata) + i + 2 + j)));
		}
		if (type == LCD_EXT_CMD_TYPE_CHECK) { /* check state */
			if (cmd_size >= 2) {
				dconf->check_reg = table[i + 2];
				dconf->check_cnt = table[i + 3];
				if (dconf->check_cnt > 0)
					dconf->check_en = 1;
			} else {
				LCDERR("%s: invalid cmd_size %d for check state\n",
					propname, cmd_size);
			}
		}
		i += (cmd_size + 2);
	}
lcd_mipi_dsi_init_table_detect_done:
	if (flag)
		dconf->dsi_init_on = table;
	else
		dconf->dsi_init_off = table;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		mipi_dsi_init_table_print(dconf, flag);

	return 0;
}

int lcd_mipi_dsi_init_table_check_bsp(struct dsi_config_s *dconf, int flag)
{
	unsigned char cmd_size, type;
	int i, max_len;
	unsigned char *init_table;
	char propname[20];

	if (flag) {
		init_table = dconf->dsi_init_on;
		max_len = DSI_INIT_ON_MAX;
		sprintf(propname, "dsi_init_on");
	} else {
		init_table = dconf->dsi_init_off;
		max_len = DSI_INIT_OFF_MAX;
		sprintf(propname, "dsi_init_off");
	}

	if (!init_table) {
		LCDERR("%s: %s is not exist\n", __func__, propname);
		return -1;
	}

	i = 0;
	while ((i + 1) < max_len) {
		type = init_table[i];
		cmd_size = init_table[i+DSI_CMD_SIZE_INDEX];
		if (type == LCD_EXT_CMD_TYPE_END) {
			if ((cmd_size == 0xff) || (cmd_size == 0))
				break;
			cmd_size = 0;
		}
		if (cmd_size == 0) {
			i += 2;
			continue;
		}
		if ((i + 2 + cmd_size) > max_len) {
			LCDERR("%s cmd_size out of support\n", propname);
			init_table[i] = LCD_EXT_CMD_TYPE_END;
			init_table[i+1] = 0;
			break;
		}

		if (type == LCD_EXT_CMD_TYPE_CHECK) { /* check state */
			if (cmd_size >= 2) {
				dconf->check_reg = init_table[i+2];
				dconf->check_cnt = init_table[i+3];
				if (dconf->check_cnt > 0)
					dconf->check_en = 1;
			} else {
				LCDERR("%s: invalid cmd_size %d for check state\n",
					propname, cmd_size);
			}
		}
		i += (cmd_size + 2);
	}

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		mipi_dsi_init_table_print(dconf, flag);

	return 0;
}

/* *************************************************************
 * Function: mipi_dcs_set
 * Configure relative registers in command mode
 * Parameters:   int trans_type, // 0: high speed, 1: low power
 *               int req_ack,    // 1: request ack, 0: do not need ack
 *               int tear_en     // 1: enable tear ack, 0: disable tear ack
 */
static void mipi_dcs_set(int index, int trans_type, int req_ack, int tear_en)
{
	dsi_host_write(index, MIPI_DSI_DWC_CMD_MODE_CFG_OS,
		(trans_type << BIT_MAX_RD_PKT_SIZE) |
		(trans_type << BIT_DCS_LW_TX)    |
		(trans_type << BIT_DCS_SR_0P_TX) |
		(trans_type << BIT_DCS_SW_1P_TX) |
		(trans_type << BIT_DCS_SW_0P_TX) |
		(trans_type << BIT_GEN_LW_TX)    |
		(trans_type << BIT_GEN_SR_2P_TX) |
		(trans_type << BIT_GEN_SR_1P_TX) |
		(trans_type << BIT_GEN_SR_0P_TX) |
		(trans_type << BIT_GEN_SW_2P_TX) |
		(trans_type << BIT_GEN_SW_1P_TX) |
		(trans_type << BIT_GEN_SW_0P_TX) |
		(req_ack << BIT_ACK_RQST_EN)     |
		(tear_en << BIT_TEAR_FX_EN));

	if (tear_en == MIPI_DCS_ENABLE_TEAR) {
		/* Enable Tear Interrupt if tear_en is valid */
		dsi_host_set_mask(index, MIPI_DSI_TOP_INTR_CNTL_STAT,
			(0x1 << BIT_EDPITE_INT_EN));
		/* Enable Measure Vsync */
		dsi_host_set_mask(index, MIPI_DSI_TOP_MEAS_CNTL,
			(0x1 << BIT_VSYNC_MEAS_EN) | (0x1 << BIT_TE_MEAS_EN));
	}

	/* Packet header settings */
	dsi_host_write(index, MIPI_DSI_DWC_PCKHDL_CFG_OS,
		(1 << BIT_CRC_RX_EN)  |
		(1 << BIT_ECC_RX_EN)  |
		(req_ack << BIT_BTA_EN)     |
		(0 << BIT_EOTP_RX_EN) |
		(0 << BIT_EOTP_TX_EN));
}

#if 0
/* *************************************************************
 * Function: set_mipi_int
 * Configure relative registers for mipi interrupt
 */
static void set_mipi_int(int index)
{
	dsi_host_write(index, MIPI_DSI_DWC_INT_MSK0_OS, 0);
	dsi_host_write(index, MIPI_DSI_DWC_INT_MSK1_OS, 0);
}
#endif

/* *************************************************************
 * Function: check_phy_st
 * Check the status of the dphy: phylock and stopstateclklane,
 *  to decide if the DPHY is ready
 */
#define DPHY_TIMEOUT    200000
static void check_phy_status(int index)
{
	int i = 0;

	while (dsi_host_getb(index, MIPI_DSI_DWC_PHY_STATUS_OS,
		BIT_PHY_LOCK, 1) == 0) {
		if (i++ >= DPHY_TIMEOUT) {
			LCDERR("[%d]: %s: phy_lock timeout\n", index, __func__);
			break;
		}
		udelay(6);
	}

	i = 0;
	udelay(10);
	while (dsi_host_getb(index, MIPI_DSI_DWC_PHY_STATUS_OS,
		BIT_PHY_STOPSTATECLKLANE, 1) == 0) {
		if (i == 0)
			LCDPR("[%d]: Waiting STOP STATE LANE\n", index);
		if (i++ >= DPHY_TIMEOUT) {
			LCDERR("[%d]: %s: lane_state timeout\n", index, __func__);
			break;
		}
		udelay(6);
	}
}

static void dsi_phy_init(struct aml_lcd_drv_s *pdrv, struct dsi_phy_s *dphy,
			 unsigned char lane_num)
{
	int index = pdrv->index;

	/* enable phy clock. */
	dsi_phy_write(index, MIPI_DSI_PHY_CTRL,  0x1); /* enable DSI top clock. */
	dsi_phy_write(index, MIPI_DSI_PHY_CTRL,
		(1 << 0)  | /* enable the DSI PLL clock . */
		(1 << 7)  |
			/* enable pll clock which connected to
			 * DDR clock path
			 */
		(1 << 8)  | /* enable the clock divider counter */
		(0 << 9)  | /* enable the divider clock out */
		(0 << 10) | /* clock divider. 1: freq/4, 0: freq/2 */
		(0 << 11) |
			/* 1: select the mipi DDRCLKHS from clock divider,
			 * 0: from PLL clock
			 */
		(0 << 12)); /* enable the byte clock generateion. */
	/* enable the divider clock out */
	dsi_phy_setb(index, MIPI_DSI_PHY_CTRL,  1, 9, 1);
	/* enable the byte clock generateion. */
	dsi_phy_setb(index, MIPI_DSI_PHY_CTRL,  1, 12, 1);
	dsi_phy_setb(index, MIPI_DSI_PHY_CTRL,  1, 31, 1);
	dsi_phy_setb(index, MIPI_DSI_PHY_CTRL,  0, 31, 1);

	/* 0x05210f08);//0x03211c08 */
	dsi_phy_write(index, MIPI_DSI_CLK_TIM,
		      (dphy->clk_trail |
		       ((dphy->clk_post + dphy->hs_trail) << 8) |
		       (dphy->clk_zero << 16) |
		       (dphy->clk_prepare << 24)));
	dsi_phy_write(index, MIPI_DSI_CLK_TIM1, dphy->clk_pre); /* ?? */
	/* 0x050f090d */
	if ((pdrv->config.timing.bit_rate / 1000000) > 500) { /*MAX than 500MHZ*/
		dsi_phy_write(index, MIPI_DSI_HS_TIM,
			      (dphy->hs_exit |
			       (dphy->hs_trail << 8) |
			       (dphy->hs_zero << 16) |
			       (dphy->hs_prepare << 24)));
	} else {
		LCDPR("[%d]: %s: bit_rate: %dMhz\n",
		      pdrv->index, __func__,
		      (pdrv->config.timing.bit_rate / 1000000));
		dsi_phy_write(index, MIPI_DSI_HS_TIM,
			      (dphy->hs_exit |
			       ((dphy->hs_trail / 2) << 8) |
			       (dphy->hs_zero << 16) |
			       (dphy->hs_prepare << 24)));
	}
	/* 0x4a370e0e */
	dsi_phy_write(index, MIPI_DSI_LP_TIM,
		(dphy->lp_lpx | (dphy->lp_ta_sure << 8) |
		(dphy->lp_ta_go << 16) | (dphy->lp_ta_get << 24)));
	/* ?? //some number to reduce sim time. */
	dsi_phy_write(index, MIPI_DSI_ANA_UP_TIM, 0x0100);
	/* 0xe20   //30d4 -> d4 to reduce sim time. */
	dsi_phy_write(index, MIPI_DSI_INIT_TIM, dphy->init);
	/* 0x8d40  //1E848-> 48 to reduct sim time. */
	dsi_phy_write(index, MIPI_DSI_WAKEUP_TIM, dphy->wakeup);
	/* wait for the LP analog ready. */
	dsi_phy_write(index, MIPI_DSI_LPOK_TIM,  0x7C);
	/* 1/3 of the tWAKEUP. */
	dsi_phy_write(index, MIPI_DSI_ULPS_CHECK,  0x927C);
	/* phy TURN watch dog. */
	dsi_phy_write(index, MIPI_DSI_LP_WCHDOG,  0x1000);
	/* phy ESC command watch dog. */
	dsi_phy_write(index, MIPI_DSI_TURN_WCHDOG,  0x1000);

	/* Powerup the analog circuit. */
	switch (lane_num) {
	case 1:
		dsi_phy_write(index, MIPI_DSI_CHAN_CTRL, 0x0e);
		break;
	case 2:
		dsi_phy_write(index, MIPI_DSI_CHAN_CTRL, 0x0c);
		break;
	case 3:
		dsi_phy_write(index, MIPI_DSI_CHAN_CTRL, 0x08);
		break;
	case 4:
	default:
		dsi_phy_write(index, MIPI_DSI_CHAN_CTRL, 0);
		break;
	}
}

static void set_dsi_phy_config(struct aml_lcd_drv_s *pdrv)
{
	struct dsi_config_s *dconf = &pdrv->config.control.mipi_cfg;
	int index = pdrv->index;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	/* Digital */
	/* Power up DSI */
	dsi_host_write(index, MIPI_DSI_DWC_PWR_UP_OS, 1);

	/* Setup Parameters of DPHY */
	dsi_host_write(index, MIPI_DSI_DWC_PHY_TST_CTRL1_OS, 0x00010044);/*testcode*/
	dsi_host_write(index, MIPI_DSI_DWC_PHY_TST_CTRL0_OS, 0x2);
	dsi_host_write(index, MIPI_DSI_DWC_PHY_TST_CTRL0_OS, 0x0);
	dsi_host_write(index, MIPI_DSI_DWC_PHY_TST_CTRL1_OS, 0x00000074);/*testwrite*/
	dsi_host_write(index, MIPI_DSI_DWC_PHY_TST_CTRL0_OS, 0x2);
	dsi_host_write(index, MIPI_DSI_DWC_PHY_TST_CTRL0_OS, 0x0);

	/* Power up D-PHY */
	dsi_host_write(index, MIPI_DSI_DWC_PHY_RSTZ_OS, 0xf);

	/* Analog */
	dsi_phy_init(pdrv, &dsi_phy_config, dconf->lane_num);

	/* Check the phylock/stopstateclklane to decide if the DPHY is ready */
	check_phy_status(index);

	/* Trigger a sync active for esc_clk */
	dsi_phy_set_mask(index, MIPI_DSI_PHY_CTRL, (1 << 1));
}

static void startup_mipi_dsi_host(int index)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", index, __func__);

	/* Enable dwc mipi_dsi_host's clock */
	dsi_host_set_mask(index, MIPI_DSI_TOP_CNTL, ((1 << 4) | (1 << 5) | (0 << 6)));
	/* mipi_dsi_host's reset */
	dsi_host_set_mask(index, MIPI_DSI_TOP_SW_RESET, 0xf);
	/* Release mipi_dsi_host's reset */
	dsi_host_clr_mask(index, MIPI_DSI_TOP_SW_RESET, 0xf);
	/* Enable dwc mipi_dsi_host's clock */
	dsi_host_set_mask(index, MIPI_DSI_TOP_CLK_CNTL, 0x3);

	dsi_host_write(index, MIPI_DSI_TOP_MEM_PD, 0);

	mdelay(10);
}

static void mipi_dsi_lpclk_ctrl(int index, struct dsi_config_s *dconf)
{
	unsigned int lpclk;

	/* when lpclk = 1, enable clk lp state */
	lpclk = (dconf->clk_always_hs) ? 0 : 1;

	dsi_host_write(index, MIPI_DSI_DWC_LPCLK_CTRL_OS,
		(lpclk << BIT_AUTOCLKLANE_CTRL) |
		(0x1 << BIT_TXREQUESTCLKHS));
}

/* *************************************************************
 * Function: set_mipi_dsi_host
 * Parameters: vcid, // virtual id
 *		chroma_subsample, // chroma_subsample for YUV422 or YUV420 only
 *		operation_mode,   // video mode/command mode
 *		p,                //lcd config
 */
static void set_mipi_dsi_host(unsigned int vcid, unsigned int chroma_subsample,
		unsigned int operation_mode, struct aml_lcd_drv_s *pdrv)
{
	unsigned int dpi_data_format, venc_data_width;
	unsigned int lane_num, vid_mode_type;
	unsigned int temp;
	struct dsi_config_s *dconf;
	int index = pdrv->index;

	dconf = &pdrv->config.control.mipi_cfg;
	venc_data_width = dconf->venc_data_width;
	dpi_data_format = dconf->dpi_data_format;
	lane_num        = (unsigned int)(dconf->lane_num);
	vid_mode_type   = (unsigned int)(dconf->video_mode_type);

	/* ----------------------------------------------------- */
	/* Standard Configuration for Video Mode Operation */
	/* ----------------------------------------------------- */
	/* 1,    Configure Lane number and phy stop wait time */
	if (dsi_phy_config.state_change == 2) {
		dsi_host_write(index, MIPI_DSI_DWC_PHY_IF_CFG_OS,
			(0x28 << BIT_PHY_STOP_WAIT_TIME) |
			((lane_num-1) << BIT_N_LANES));
	} else {
		dsi_host_write(index, MIPI_DSI_DWC_PHY_IF_CFG_OS,
			(1 << BIT_PHY_STOP_WAIT_TIME) |
			((lane_num-1) << BIT_N_LANES));
	}

	/* 2.1,  Configure Virtual channel settings */
	dsi_host_write(index, MIPI_DSI_DWC_DPI_VCID_OS, vcid);
	/* 2.2,  Configure Color format */
	dsi_host_write(index, MIPI_DSI_DWC_DPI_COLOR_CODING_OS,
		(((dpi_data_format == COLOR_18BIT_CFG_2) ?
			1 : 0) << BIT_LOOSELY18_EN) |
		(dpi_data_format << BIT_DPI_COLOR_CODING));
	/* 2.2.1 Configure Set color format for DPI register */
	temp = (dsi_host_read(index, MIPI_DSI_TOP_CNTL) &
		~(0xf<<BIT_DPI_COLOR_MODE) &
		~(0x7<<BIT_IN_COLOR_MODE) &
		~(0x3<<BIT_CHROMA_SUBSAMPLE));
	dsi_host_write(index, MIPI_DSI_TOP_CNTL,
		(temp |
		(dpi_data_format  << BIT_DPI_COLOR_MODE)  |
		(venc_data_width  << BIT_IN_COLOR_MODE)   |
		(chroma_subsample << BIT_CHROMA_SUBSAMPLE)));
	/* 2.3   Configure Signal polarity */
	dsi_host_write(index, MIPI_DSI_DWC_DPI_CFG_POL_OS,
		(0x0 << BIT_COLORM_ACTIVE_LOW) |
		(0x0 << BIT_SHUTD_ACTIVE_LOW)  |
		(0 << BIT_HSYNC_ACTIVE_LOW)    |
		(0 << BIT_VSYNC_ACTIVE_LOW)    |
		(0x0 << BIT_DATAEN_ACTIVE_LOW));

	if (operation_mode == OPERATION_VIDEO_MODE) {
		/* 3.1   Configure Low power and video mode type settings */
		dsi_host_write(index, MIPI_DSI_DWC_VID_MODE_CFG_OS,
			(1 << BIT_LP_HFP_EN)  |       /* enable lp */
			(1 << BIT_LP_HBP_EN)  |       /* enable lp */
			(1 << BIT_LP_VACT_EN) |       /* enable lp */
			(1 << BIT_LP_VFP_EN)  |       /* enable lp */
			(1 << BIT_LP_VBP_EN)  |       /* enable lp */
			(1 << BIT_LP_VSA_EN)  |       /* enable lp */
			(0 << BIT_FRAME_BTA_ACK_EN) |
			   /* enable BTA after one frame, TODO, need check */
			/* (1 << BIT_LP_CMD_EN) |  */
			   /* enable the command transmission only in lp mode */
			(vid_mode_type << BIT_VID_MODE_TYPE));
					/* burst non burst mode */
		/* [23:16]outvact, [7:0]invact */
		dsi_host_write(index, MIPI_DSI_DWC_DPI_LP_CMD_TIM_OS,
			(4 << 16) | (4 << 0));
#if 0
		/* 3.2   Configure video packet size settings */
		if (vid_mode_type == BURST_MODE) {
			/* should be one line in pixels, such as 480/240... */
			dsi_host_write(index, MIPI_DSI_DWC_VID_PKT_SIZE_OS,
				p->lcd_basic.h_active);
		} else {  /* non-burst mode */
			/* in unit of pixels,
			 *   (pclk period/byte clk period)*num_of_lane
			 *    should be integer
			 */
			dsi_host_write(index, MIPI_DSI_DWC_VID_PKT_SIZE_OS,
				dsi_vconf.pixel_per_chunk);
		}

		/* 3.3  Configure number of chunks and null packet size
		 *  for one line
		 */
		if (vid_mode_type == BURST_MODE) {
			dsi_host_write(index, MIPI_DSI_DWC_VID_NUM_CHUNKS_OS, 0);
			dsi_host_write(index, MIPI_DSI_DWC_VID_NULL_SIZE_OS, 0);
		} else {  /* non burst mode */
			/* HACT/VID_PKT_SIZE */
			dsi_host_write(index, MIPI_DSI_DWC_VID_NUM_CHUNKS_OS,
				dsi_vconf.vid_num_chunks);
			/* video null size */
			dsi_host_write(index, MIPI_DSI_DWC_VID_NULL_SIZE_OS,
				dsi_vconf.vid_null_size);
		}
#else
		/* 3.2   Configure video packet size settings */
		dsi_host_write(index, MIPI_DSI_DWC_VID_PKT_SIZE_OS,
			dsi_vconf.pixel_per_chunk);
		dsi_host_write(index, MIPI_DSI_DWC_VID_NUM_CHUNKS_OS,
			dsi_vconf.vid_num_chunks);
		dsi_host_write(index, MIPI_DSI_DWC_VID_NULL_SIZE_OS,
			dsi_vconf.vid_null_size);
#endif
		/* 4 Configure the video relative parameters according to
		 *	   the output type
		 */
		/* include horizontal timing and vertical line */
		dsi_host_write(index, MIPI_DSI_DWC_VID_HLINE_TIME_OS, dsi_vconf.hline);
		dsi_host_write(index, MIPI_DSI_DWC_VID_HSA_TIME_OS, dsi_vconf.hsa);
		dsi_host_write(index, MIPI_DSI_DWC_VID_HBP_TIME_OS, dsi_vconf.hbp);
		dsi_host_write(index, MIPI_DSI_DWC_VID_VSA_LINES_OS, dsi_vconf.vsa);
		dsi_host_write(index, MIPI_DSI_DWC_VID_VBP_LINES_OS, dsi_vconf.vbp);
		dsi_host_write(index, MIPI_DSI_DWC_VID_VFP_LINES_OS, dsi_vconf.vfp);
		dsi_host_write(index, MIPI_DSI_DWC_VID_VACTIVE_LINES_OS,
			dsi_vconf.vact);
	}  /* operation_mode == OPERATION_VIDEO_MODE */

	/* ----------------------------------------------------- */
	/* Finish Configuration */
	/* ----------------------------------------------------- */

	/* Inner clock divider settings */
	dsi_host_write(index, MIPI_DSI_DWC_CLKMGR_CFG_OS,
		(0x1 << BIT_TO_CLK_DIV) |
		(dsi_phy_config.lp_tesc << BIT_TX_ESC_CLK_DIV));
	/* Packet header settings  //move to mipi_dcs_set */
	/* dsi_host_write(index, MIPI_DSI_DWC_PCKHDL_CFG_OS,
	 *	(1 << BIT_CRC_RX_EN) |
	 *	(1 << BIT_ECC_RX_EN) |
	 *	(0 << BIT_BTA_EN) |
	 *	(0 << BIT_EOTP_RX_EN) |
	 *	(0 << BIT_EOTP_TX_EN) );
	 */
	/* operation mode setting: video/command mode */
	dsi_host_write(index, MIPI_DSI_DWC_MODE_CFG_OS, operation_mode);

	/* Phy Timer */
	if (dsi_phy_config.state_change == 2)
		dsi_host_write(index, MIPI_DSI_DWC_PHY_TMR_CFG_OS, 0x03320000);
	else
		dsi_host_write(index, MIPI_DSI_DWC_PHY_TMR_CFG_OS, 0x090f0000);

	if (dsi_phy_config.state_change == 2)
		dsi_host_write(index, MIPI_DSI_DWC_PHY_TMR_LPCLK_CFG_OS, 0x870025);
	else
		dsi_host_write(index, MIPI_DSI_DWC_PHY_TMR_LPCLK_CFG_OS, 0x260017);
}

/* *************************************************************
 * mipi dsi command support
 */

static inline void print_mipi_cmd_status(int index, int cnt, unsigned int status)
{
	if (cnt == 0) {
		LCDPR("[%d]: cmd error: status=0x%04x, int0=0x%06x, int1=0x%06x\n",
			index, status,
			dsi_host_read(index, MIPI_DSI_DWC_INT_ST0_OS),
			dsi_host_read(index, MIPI_DSI_DWC_INT_ST1_OS));
	}
}

#ifdef DSI_CMD_READ_VALID
static void dsi_bta_control(int index, int en)
{
	if (en) {
		dsi_host_setb(index, MIPI_DSI_DWC_CMD_MODE_CFG_OS,
			MIPI_DSI_DCS_REQ_ACK, BIT_ACK_RQST_EN, 1);
		dsi_host_setb(index, MIPI_DSI_DWC_PCKHDL_CFG_OS,
			MIPI_DSI_DCS_REQ_ACK, BIT_BTA_EN, 1);
	} else {
		dsi_host_setb(index, MIPI_DSI_DWC_PCKHDL_CFG_OS,
			MIPI_DSI_DCS_NO_ACK, BIT_BTA_EN, 1);
		dsi_host_setb(index, MIPI_DSI_DWC_CMD_MODE_CFG_OS,
			MIPI_DSI_DCS_NO_ACK, BIT_ACK_RQST_EN, 1);
	}
}

/* *************************************************************
 * Function: generic_if_rd
 * Generic interface read, address has to be MIPI_DSI_DWC_GEN_PLD_DATA_OS
 */
static unsigned int generic_if_rd(int index, unsigned int address)
{
	unsigned int data_out;

	if (address != MIPI_DSI_DWC_GEN_PLD_DATA_OS)
		LCDERR("[%d]:  Error Address : %x\n", index, address);

	data_out = dsi_host_read(index, address);
	return data_out;
}
#endif

/* *************************************************************
 * Function: generic_if_wr
 * Generic interface write, address has to be
 *			MIPI_DSI_DWC_GEN_PLD_DATA_OS,
 *			MIPI_DSI_DWC_GEN_HDR_OS,
 *			MIPI_DSI_DWC_GEN_VCID_OS
 */
static unsigned int generic_if_wr(int index, unsigned int address, unsigned int data_in)
{
	if ((address != MIPI_DSI_DWC_GEN_HDR_OS) &&
		(address != MIPI_DSI_DWC_GEN_PLD_DATA_OS)) {
		LCDERR("[%d]: Error Address : 0x%x\n", index, address);
	}

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: address 0x%x = 0x%08x\n", index, address, data_in);

	dsi_host_write(index, address, data_in);

	return 0;
}

/* *************************************************************
 * Function: wait_bta_ack
 * Poll to check if the BTA ack is finished
 */
static int wait_bta_ack(int index)
{
	unsigned int phy_status;
	int i;

	/* Check if phydirection is RX */
	i = CMD_TIMEOUT_CNT;
	do {
		udelay(10);
		i--;
		phy_status = dsi_host_read(index, MIPI_DSI_DWC_PHY_STATUS_OS);
	} while ((((phy_status & 0x2) >> BIT_PHY_DIRECTION) == 0x0) && (i > 0));
	if (i == 0) {
		LCDERR("[%d]: phy direction error: RX\n", index);
		return -1;
	}

	/* Check if phydirection is return to TX */
	i = CMD_TIMEOUT_CNT;
	do {
		udelay(10);
		i--;
		phy_status = dsi_host_read(index, MIPI_DSI_DWC_PHY_STATUS_OS);
	} while ((((phy_status & 0x2) >> BIT_PHY_DIRECTION) == 0x1) && (i > 0));
	if (i == 0) {
		LCDERR("[%d]: phy direction error: TX\n", index);
		return -1;
	}

	return 0;
}

/* *************************************************************
 * Function: wait_cmd_fifo_empty
 * Poll to check if the generic command fifo is empty
 */
static int wait_cmd_fifo_empty(int index)
{
	unsigned int cmd_status;
	int i = CMD_TIMEOUT_CNT;

	do {
		udelay(10);
		i--;
		cmd_status = dsi_host_getb(index, MIPI_DSI_DWC_CMD_PKT_STATUS_OS,
				BIT_GEN_CMD_EMPTY, 1);
	} while ((cmd_status != 0x1) && (i > 0));

	if (cmd_status == 0) {
		cmd_status = dsi_host_read(index, MIPI_DSI_DWC_CMD_PKT_STATUS_OS);
		print_mipi_cmd_status(index, i, cmd_status);
		return -1;
	}

	return 0;
}

#if 0
/* *************************************************************
 * Function: wait_for_generic_read_response
 * Wait for generic read response
 */
static unsigned int wait_for_generic_read_response(int index)
{
	unsigned int timeout, phy_status, data_out;

	phy_status = dsi_host_read(index, MIPI_DSI_DWC_PHY_STATUS_OS);
	for (timeout = 0; timeout < 50; timeout++) {
		if (((phy_status & 0x40) >> BIT_PHY_RXULPSESC0LANE) == 0x0)
			break;
		phy_status = dsi_host_read(index, MIPI_DSI_DWC_PHY_STATUS_OS);
		udelay(1);
	}
	phy_status = dsi_host_read(index, MIPI_DSI_DWC_PHY_STATUS_OS);
	for (timeout = 0; timeout < 50; timeout++) {
		if (((phy_status & 0x40) >> BIT_PHY_RXULPSESC0LANE) == 0x1)
			break;
		phy_status = dsi_host_read(index, MIPI_DSI_DWC_PHY_STATUS_OS);
		udelay(1);
	}

	data_out = dsi_host_read(index, MIPI_DSI_DWC_GEN_PLD_DATA_OS);
	return data_out;
}

/* *************************************************************
 * Function: generic_read_packet_0_para
 * Generic Read Packet 0 Parameter with Generic Interface
 * Supported DCS Command: DCS_SET_ADDRESS_MODE,
 *			DCS_SET_GAMMA_CURVE,
 *			DCS_SET_PIXEL_FORMAT,
 *			DCS_SET_TEAR_ON
 */
static unsigned int generic_read_packet_0_para(unsigned char data_type,
		unsigned char vc_id, unsigned char dcs_command)
{
	unsigned int read_data;

	/* lcd_print(" para is %x, dcs_command is %x\n", para, dcs_command); */
	/* lcd_print(" vc_id %x, data_type is %x\n", vc_id, data_type); */
	generic_if_wr(index, MIPI_DSI_DWC_GEN_HDR_OS,
		((0 << BIT_GEN_WC_MSBYTE)                           |
		(((unsigned int)dcs_command) << BIT_GEN_WC_LSBYTE)  |
		(((unsigned int)vc_id) << BIT_GEN_VC)               |
		(((unsigned int)data_type) << BIT_GEN_DT)));

	read_data = wait_for_generic_read_response();

	return read_data;
}
#endif

static void dsi_set_max_return_pkt_size(int index, struct dsi_cmd_request_s *req)
{
	unsigned int d_para[2];

	d_para[0] = (unsigned int)(req->payload[2] & 0xff);
	d_para[1] = (unsigned int)(req->payload[3] & 0xff);
	dsi_rx_n = (unsigned short)((d_para[1] << 8) | d_para[0]);
	generic_if_wr(index, MIPI_DSI_DWC_GEN_HDR_OS,
		((d_para[1] << BIT_GEN_WC_MSBYTE)          |
		(d_para[0] << BIT_GEN_WC_LSBYTE)           |
		(((unsigned int)req->vc_id) << BIT_GEN_VC) |
		(DT_SET_MAX_RET_PKT_SIZE << BIT_GEN_DT)));
	if (req->req_ack == MIPI_DSI_DCS_REQ_ACK)
		wait_bta_ack(index);
	else if (req->req_ack == MIPI_DSI_DCS_NO_ACK)
		wait_cmd_fifo_empty(index);
}

#ifdef DSI_CMD_READ_VALID
static int dsi_generic_read_packet(int index, struct dsi_cmd_request_s *req,
		unsigned char *r_data)
{
	unsigned int d_para[2], read_data;
	unsigned int i, j, done;
	int ret = 0;

	switch (req->data_type) {
	case DT_GEN_RD_1:
		d_para[0] = (req->pld_count == 0) ?
			0 : (((unsigned int)req->payload[2]) & 0xff);
		d_para[1] = 0;
		break;
	case DT_GEN_RD_2:
		d_para[0] = (req->pld_count == 0) ?
			0 : (((unsigned int)req->payload[2]) & 0xff);
		d_para[1] = (req->pld_count < 2) ?
			0 : (((unsigned int)req->payload[3]) & 0xff);
		break;
	case DT_GEN_RD_0:
	default:
		d_para[0] = 0;
		d_para[1] = 0;
		break;
	}

	if (MIPI_DSI_DCS_ACK_TYPE == MIPI_DSI_DCS_NO_ACK)
		dsi_bta_control(index, 1);
	generic_if_wr(index, MIPI_DSI_DWC_GEN_HDR_OS,
		((d_para[1] << BIT_GEN_WC_MSBYTE)          |
		(d_para[0] << BIT_GEN_WC_LSBYTE)           |
		(((unsigned int)req->vc_id) << BIT_GEN_VC) |
		(((unsigned int)req->data_type) << BIT_GEN_DT)));
	ret = wait_bta_ack(index);
	if (ret)
		return -1;

	i = 0;
	done = 0;
	while (done == 0) {
		read_data = generic_if_rd(index, MIPI_DSI_DWC_GEN_PLD_DATA_OS);
		for (j = 0; j < 4; j++) {
			if (i < dsi_rx_n) {
				r_data[i] = (unsigned char)
					((read_data >> (j*8)) & 0xff);
				i++;
			} else {
				done = 1;
				break;
			}
		}
	}
	if (MIPI_DSI_DCS_ACK_TYPE == MIPI_DSI_DCS_NO_ACK)
		dsi_bta_control(index, 0);

	return dsi_rx_n;
}

static int dsi_dcs_read_packet(int index, struct dsi_cmd_request_s *req,
		unsigned char *r_data)
{
	unsigned int d_command, read_data;
	unsigned int i, j, done;
	int ret = 0;

	d_command = ((unsigned int)req->payload[2]) & 0xff;

	if (MIPI_DSI_DCS_ACK_TYPE == MIPI_DSI_DCS_NO_ACK)
		dsi_bta_control(index, 1);
	generic_if_wr(index, MIPI_DSI_DWC_GEN_HDR_OS,
		((0 << BIT_GEN_WC_MSBYTE)                  |
		(d_command << BIT_GEN_WC_LSBYTE)           |
		(((unsigned int)req->vc_id) << BIT_GEN_VC) |
		(((unsigned int)req->data_type) << BIT_GEN_DT)));
	ret = wait_bta_ack(index);
	if (ret)
		return -1;

	i = 0;
	done = 0;
	while (done == 0) {
		read_data = generic_if_rd(index, MIPI_DSI_DWC_GEN_PLD_DATA_OS);
		for (j = 0; j < 4; j++) {
			if (i < dsi_rx_n) {
				r_data[i] = (unsigned char)
					((read_data >> (j*8)) & 0xff);
				i++;
			} else {
				done = 1;
				break;
			}
		}
	}

	if (MIPI_DSI_DCS_ACK_TYPE == MIPI_DSI_DCS_NO_ACK)
		dsi_bta_control(index, 0);

	return dsi_rx_n;
}
#endif

/* *************************************************************
 * Function: generic_write_short_packet
 * Generic Write Short Packet with Generic Interface
 * Supported Data Type: DT_GEN_SHORT_WR_0,
			DT_GEN_SHORT_WR_1,
			DT_GEN_SHORT_WR_2,
 */
static int dsi_generic_write_short_packet(int index, struct dsi_cmd_request_s *req)
{
	unsigned int d_para[2];
	int ret = 0;

	switch (req->data_type) {
	case DT_GEN_SHORT_WR_1:
		d_para[0] = (req->pld_count == 0) ?
			0 : (((unsigned int)req->payload[2]) & 0xff);
		d_para[1] = 0;
		break;
	case DT_GEN_SHORT_WR_2:
		d_para[0] = (req->pld_count == 0) ?
			0 : (((unsigned int)req->payload[2]) & 0xff);
		d_para[1] = (req->pld_count < 2) ?
			0 : (((unsigned int)req->payload[3]) & 0xff);
		break;
	case DT_GEN_SHORT_WR_0:
	default:
		d_para[0] = 0;
		d_para[1] = 0;
		break;
	}

	generic_if_wr(index, MIPI_DSI_DWC_GEN_HDR_OS,
		((d_para[1] << BIT_GEN_WC_MSBYTE)          |
		(d_para[0] << BIT_GEN_WC_LSBYTE)           |
		(((unsigned int)req->vc_id) << BIT_GEN_VC) |
		(((unsigned int)req->data_type) << BIT_GEN_DT)));
	if (req->req_ack == MIPI_DSI_DCS_REQ_ACK)
		ret = wait_bta_ack(index);
	else if (req->req_ack == MIPI_DSI_DCS_NO_ACK)
		ret = wait_cmd_fifo_empty(index);

	return ret;
}

/* *************************************************************
 * Function: dcs_write_short_packet
 * DCS Write Short Packet with Generic Interface
 * Supported Data Type: DT_DCS_SHORT_WR_0, DT_DCS_SHORT_WR_1,
 */
static int dsi_dcs_write_short_packet(int index, struct dsi_cmd_request_s *req)
{
	unsigned int d_command, d_para;
	int ret = 0;

	d_command = ((unsigned int)req->payload[2]) & 0xff;
	d_para = (req->pld_count < 2) ?
		0 : (((unsigned int)req->payload[3]) & 0xff);

	generic_if_wr(index, MIPI_DSI_DWC_GEN_HDR_OS,
		((d_para << BIT_GEN_WC_MSBYTE)             |
		(d_command << BIT_GEN_WC_LSBYTE)           |
		(((unsigned int)req->vc_id) << BIT_GEN_VC) |
		(((unsigned int)req->data_type) << BIT_GEN_DT)));
	if (req->req_ack == MIPI_DSI_DCS_REQ_ACK)
		ret = wait_bta_ack(index);
	else if (req->req_ack == MIPI_DSI_DCS_NO_ACK)
		ret = wait_cmd_fifo_empty(index);

	return ret;
}

/* *************************************************************
 * Function: dsi_write_long_packet
 * Write Long Packet with Generic Interface
 * Supported Data Type: DT_GEN_LONG_WR, DT_DCS_LONG_WR
 */
static int dsi_write_long_packet(int index, struct dsi_cmd_request_s *req)
{
	unsigned int d_command, payload_data, header_data;
	unsigned int cmd_status;
	unsigned int i, j, data_index, n, temp;
	int ret = 0;

	/* payload[2] start (payload[0]: data_type, payload[1]: data_cnt) */
	data_index = DSI_CMD_SIZE_INDEX + 1;
	d_command = ((unsigned int)req->payload[data_index]) & 0xff;

	/* Write Payload Register First */
	n = (req->pld_count+3)/4;
	for (i = 0; i < n; i++) {
		payload_data = 0;
		if (i < (req->pld_count/4))
			temp = 4;
		else
			temp = req->pld_count % 4;
		for (j = 0; j < temp; j++) {
			payload_data |= (((unsigned int)
				req->payload[data_index+(i*4)+j]) << (j*8));
		}

		/* Check the pld fifo status before write to it,
		 * do not need check every word
		 */
		if ((i == (n/3)) || (i == (n/2))) {
			j = CMD_TIMEOUT_CNT;
			do {
				udelay(10);
				j--;
				cmd_status = dsi_host_read(index,
						MIPI_DSI_DWC_CMD_PKT_STATUS_OS);
			} while ((((cmd_status >> BIT_GEN_PLD_W_FULL) & 0x1) ==
				0x1) && (j > 0));
			print_mipi_cmd_status(index, j, cmd_status);
		}
		/* Use direct memory write to save time when in
		 * WRITE_MEMORY_CONTINUE
		 */
		if (d_command == DCS_WRITE_MEMORY_CONTINUE) {
			dsi_host_write(index, MIPI_DSI_DWC_GEN_PLD_DATA_OS,
				payload_data);
		} else {
			generic_if_wr(index, MIPI_DSI_DWC_GEN_PLD_DATA_OS,
				payload_data);
		}
	}

	/* Check cmd fifo status before write to it */
	j = CMD_TIMEOUT_CNT;
	do {
		udelay(10);
		j--;
		cmd_status = dsi_host_read(index, MIPI_DSI_DWC_CMD_PKT_STATUS_OS);
	} while ((((cmd_status >> BIT_GEN_CMD_FULL) & 0x1) == 0x1) && (j > 0));
	print_mipi_cmd_status(index, j, cmd_status);
	/* Write Header Register */
	/* include command */
	header_data = ((((unsigned int)req->pld_count) << BIT_GEN_WC_LSBYTE) |
			(((unsigned int)req->vc_id) << BIT_GEN_VC)           |
			(((unsigned int)req->data_type) << BIT_GEN_DT));
	generic_if_wr(index, MIPI_DSI_DWC_GEN_HDR_OS, header_data);
	if (req->req_ack == MIPI_DSI_DCS_REQ_ACK)
		ret = wait_bta_ack(index);
	else if (req->req_ack == MIPI_DSI_DCS_NO_ACK)
		ret = wait_cmd_fifo_empty(index);

	return ret;
}

#ifdef DSI_CMD_READ_VALID
/* *************************************************************
 * Function: dsi_read_single
 * Supported Data Type: DT_GEN_RD_0, DT_GEN_RD_1, DT_GEN_RD_2,
 *		DT_DCS_RD_0
 * Return:              data count, -1 for error
 */
int dsi_read_single(int index, unsigned char *payload, unsigned char *rd_data,
		unsigned int rd_byte_len)
{
	int num = 0;
	unsigned char temp[4];
	unsigned char vc_id = MIPI_DSI_VIRTUAL_CHAN_ID;
	unsigned int req_ack;
	struct dsi_cmd_request_s dsi_cmd_req;

	req_ack = MIPI_DSI_DCS_ACK_TYPE;
	dsi_cmd_req.data_type = DT_SET_MAX_RET_PKT_SIZE;
	dsi_cmd_req.vc_id = (vc_id & 0x3);
	temp[0] = dsi_cmd_req.data_type;
	temp[1] = 2;
	temp[2] = (unsigned char)((rd_byte_len >> 0) & 0xff);
	temp[3] = (unsigned char)((rd_byte_len >> 8) & 0xff);
	dsi_cmd_req.payload = &temp[0];
	dsi_cmd_req.pld_count = 2;
	dsi_cmd_req.req_ack = req_ack;
	dsi_set_max_return_pkt_size(index, &dsi_cmd_req);

	/* payload struct: */
	/* data_type, data_cnt, command, parameters... */
	req_ack = MIPI_DSI_DCS_REQ_ACK; /* need BTA ack */
	dsi_cmd_req.data_type = payload[0];
	dsi_cmd_req.vc_id = (vc_id & 0x3);
	dsi_cmd_req.payload = &payload[0];
	dsi_cmd_req.pld_count = payload[DSI_CMD_SIZE_INDEX];
	dsi_cmd_req.req_ack = req_ack;
	switch (dsi_cmd_req.data_type) {/* analysis data_type */
	case DT_GEN_RD_0:
	case DT_GEN_RD_1:
	case DT_GEN_RD_2:
		num = dsi_generic_read_packet(index, &dsi_cmd_req, rd_data);
		break;
	case DT_DCS_RD_0:
		num = dsi_dcs_read_packet(index, &dsi_cmd_req, rd_data);
		break;
	default:
		LCDPR("[%d]: read un-support data_type: 0x%02x\n",
		      index, dsi_cmd_req.data_type);
		break;
	}

	if (num < 0)
		LCDERR("[%d]: mipi-dsi read error\n", index);

	return num;
}
#else
int dsi_read_single(int index, unsigned char *payload, unsigned char *rd_data,
		unsigned int rd_byte_len)
{
	LCDPR("[%d]: Don't support mipi-dsi read command\n", index);
	return -1;
}
#endif

static int mipi_dsi_check_state(struct aml_lcd_drv_s *pdrv,
				unsigned char reg, unsigned char cnt)
{
	int ret = 0, i;
	unsigned char *rd_data;
	unsigned char payload[3] = {DT_GEN_RD_1, 1, 0x04};
	struct dsi_config_s *dconf;
	unsigned int offset;
	int index = pdrv->index;

	dconf = &pdrv->config.control.mipi_cfg;
	if (dconf->check_en == 0)
		return 0;
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	rd_data = (unsigned char *)malloc(sizeof(unsigned char) * cnt);
	if (!rd_data) {
		LCDERR("[%d]: %s: rd_data malloc error\n", pdrv->index, __func__);
		return 0;
	}

	offset = pdrv->data->offset_venc_if[pdrv->index];

	payload[2] = reg;
	ret = dsi_read_single(index, payload, rd_data, cnt);
	if (ret < 0)
		goto mipi_dsi_check_state_err;
	if (ret > cnt) {
		LCDERR("[%d]: %s: read back cnt is wrong\n", pdrv->index, __func__);
		goto mipi_dsi_check_state_err;
	}

	printf("read reg 0x%02x: ", reg);
	for (i = 0; i < ret; i++) {
		if (i == 0)
			printf("0x%02x", rd_data[i]);
		else
			printf(",0x%02x", rd_data[i]);
	}
	printf("\n");

	dconf->check_state = 1;
	lcd_vcbus_setb(L_VCOM_VS_ADDR + offset, 1, 12, 1);
	pdrv->config.retry_enable_flag = 0;
	LCDPR("[%d]: %s: %d\n", pdrv->index, __func__, dconf->check_state);
	free(rd_data);
	return 0;

mipi_dsi_check_state_err:
	dconf->check_state = 0;
	lcd_vcbus_setb(L_VCOM_VS_ADDR + offset, 0, 12, 1);
	LCDPR("[%d]: %s: %d\n", pdrv->index, __func__, dconf->check_state);
	pdrv->config.retry_enable_flag = 1;
	free(rd_data);
	return -1;
}

/* *************************************************************
 * Function: dsi_write_cmd
 * Supported Data Type: DT_GEN_SHORT_WR_0, DT_GEN_SHORT_WR_1, DT_GEN_SHORT_WR_2,
 *			DT_DCS_SHORT_WR_0, DT_DCS_SHORT_WR_1,
 *			DT_GEN_LONG_WR, DT_DCS_LONG_WR,
 *			DT_SET_MAX_RET_PKT_SIZE
 *			DT_GEN_RD_0, DT_GEN_RD_1, DT_GEN_RD_2,
 *			DT_DCS_RD_0
 * Return:              command number
 */
int dsi_write_cmd(struct aml_lcd_drv_s *pdrv, unsigned char *payload)
{
	int i = 0, j = 0, step = 0;
	unsigned char cmd_size;
#ifdef DSI_CMD_READ_VALID
	int k = 0, n = 0;
	unsigned char rd_data[100];
#endif
	struct dsi_cmd_request_s dsi_cmd_req;
	unsigned char vc_id = MIPI_DSI_VIRTUAL_CHAN_ID;
	unsigned int req_ack = MIPI_DSI_DCS_ACK_TYPE;
	char *str;
	int gpio, delay_ms, ret = 0;
	int index;

	if (!pdrv) {
		LCDERR("%s: pdrv is null\n", __func__);
		return 0;
	}
	index = pdrv->index;

	/* mipi command(payload) */
	/* format:  data_type, cmd_size, data.... */
	/* 	data_type=0xff,
	 *		cmd_size<0xff means delay ms,
	 *		cmd_size=0xff or 0 means ending.
	 *	data_type=0xf0, for gpio control
	 *		data0=gpio_index, data1=gpio_value.
	 *		data0=gpio_index, data1=gpio_value, data2=delay.
	 *	data_type=0xfd, for delay ms
	 *		data0=delay, data_1=delay, ..., data_n=delay.
	 */
	while ((i + DSI_CMD_SIZE_INDEX) < DSI_CMD_SIZE_MAX) {
		if (ret) {
			LCDERR("[%d]: %s: error, exit\n", index, __func__);
			break;
		}
		cmd_size = payload[i+DSI_CMD_SIZE_INDEX];
		if (payload[i] == LCD_EXT_CMD_TYPE_END) {
			if ((cmd_size == 0xff) || (cmd_size == 0))
				break;
			cmd_size = 0;
			mdelay(payload[i + 1]);
		}

		if (cmd_size == 0) {
			i += (cmd_size + 2);
			continue;
		}
		if (i + 2 + cmd_size > DSI_CMD_SIZE_MAX) {
			LCDERR("[%d]: step %d: cmd_size out of support\n",
			       index, step);
			break;
		}

		if (payload[i] == LCD_EXT_CMD_TYPE_DELAY) { /* delay */
			delay_ms = 0;
			for (j = 0; j < cmd_size; j++)
				delay_ms += payload[i + 2 + j];
			if (delay_ms > 0)
				mdelay(delay_ms);
		} else if (payload[i] == LCD_EXT_CMD_TYPE_GPIO) { /* gpio */
			if (cmd_size < 2) {
				LCDERR("[%d]: step %d: invalid cmd_size %d for gpio\n",
				       index, step, cmd_size);
				break;
			}
			str = pdrv->config.power.cpu_gpio[payload[i + 2]];
			gpio = lcd_gpio_name_map_num(str);
			lcd_gpio_set(gpio, payload[i + 3]);
			if (cmd_size > 2) {
				if (payload[i + 4])
					mdelay(payload[i + 4]);
			}
		} else if (payload[i] == LCD_EXT_CMD_TYPE_CHECK) { /* check state */
			if (cmd_size < 2) {
				LCDERR("[%d]: step %d: invalid cmd_size %d for check state\n",
				       index, step, cmd_size);
				break;
			}
			if (!pdrv) {
				LCDERR("[%d]: step %d: invalid pdrv for check state\n",
				       index, step);
				break;
			}
			if (payload[i + 3] > 0)
				ret = mipi_dsi_check_state(pdrv,
							   payload[i + 2],
							   payload[i + 3]);
		} else {
			/* payload[i+DSI_CMD_SIZE_INDEX] is data count */
			dsi_cmd_req.data_type = payload[i];
			dsi_cmd_req.vc_id = (vc_id & 0x3);
			dsi_cmd_req.payload = &payload[i];
			dsi_cmd_req.pld_count = payload[i + DSI_CMD_SIZE_INDEX];
			dsi_cmd_req.req_ack = req_ack;
			switch (dsi_cmd_req.data_type) {/* analysis data_type */
			case DT_GEN_SHORT_WR_0:
			case DT_GEN_SHORT_WR_1:
			case DT_GEN_SHORT_WR_2:
				ret = dsi_generic_write_short_packet(index, &dsi_cmd_req);
				break;
			case DT_DCS_SHORT_WR_0:
			case DT_DCS_SHORT_WR_1:
				ret = dsi_dcs_write_short_packet(index, &dsi_cmd_req);
				break;
			case DT_DCS_LONG_WR:
			case DT_GEN_LONG_WR:
				ret = dsi_write_long_packet(index, &dsi_cmd_req);
				break;
			case DT_TURN_ON:
				dsi_host_setb(index, MIPI_DSI_TOP_CNTL, 1, 2, 1);
				mdelay(20); /* wait for vsync trigger */
				dsi_host_setb(index, MIPI_DSI_TOP_CNTL, 0, 2, 1);
				mdelay(20); /* wait for vsync trigger */
				break;
			case DT_SHUT_DOWN:
				dsi_host_setb(index, MIPI_DSI_TOP_CNTL, 1, 2, 1);
				mdelay(20); /* wait for vsync trigger */
				break;
			case DT_SET_MAX_RET_PKT_SIZE:
				dsi_set_max_return_pkt_size(index, &dsi_cmd_req);
				break;
#ifdef DSI_CMD_READ_VALID
			case DT_GEN_RD_0:
			case DT_GEN_RD_1:
			case DT_GEN_RD_2:
				/* need BTA ack */
				dsi_cmd_req.req_ack = MIPI_DSI_DCS_REQ_ACK;
				dsi_cmd_req.pld_count =
					(dsi_cmd_req.pld_count > 2) ?
					2 : dsi_cmd_req.pld_count;
				n = dsi_generic_read_packet(index, &dsi_cmd_req,
						&rd_data[0]);
				LCDPR("[%d]: generic read data", index);
				for (k = 0; k < dsi_cmd_req.pld_count; k++) {
					printf(" 0x%02x",
						dsi_cmd_req.payload[k+2]);
				}
				for (k = 0; k < n; k++)
					printf("0x%02x ", rd_data[k]);
				printf("\n");
				break;
			case DT_DCS_RD_0:
				/* need BTA ack */
				dsi_cmd_req.req_ack = MIPI_DSI_DCS_REQ_ACK;
				n = dsi_dcs_read_packet(index, &dsi_cmd_req,
					&rd_data[0]);
				printf("dcs read data 0x%02x:\n",
					dsi_cmd_req.payload[2]);
				for (k = 0; k < n; k++)
					printf("0x%02x ", rd_data[k]);
				printf("\n");
				break;
#endif
			default:
				LCDPR("[%d]: step %d: un-support data_type: 0x%02x\n",
				      index, step, dsi_cmd_req.data_type);

				break;
			}
		}
		i += (cmd_size + 2);
		step++;
	}

	return step;
}

static void mipi_dsi_phy_config(struct dsi_phy_s *dphy, unsigned int dsi_ui)
{
	unsigned int temp, t_ui, t_req_min, t_req_max;
	unsigned int val;

	if (dsi_ui == 0) {
		LCDERR("%s: dsi_ui is 0\n", __func__);
		return;
	}

	t_ui = (1000000 * 100) / (dsi_ui / 1000); /*100*ns */
	temp = t_ui * 8; /* lane_byte cycle time */

	dphy->lp_tesc = ((DPHY_TIME_LP_TESC(t_ui) + temp - 1) / temp) & 0xff;
	dphy->lp_lpx = ((DPHY_TIME_LP_LPX(t_ui) + temp - 1) / temp) & 0xff;
	dphy->lp_ta_sure =
		((DPHY_TIME_LP_TA_SURE(t_ui) + temp - 1) / temp) & 0xff;
	dphy->lp_ta_go = ((DPHY_TIME_LP_TA_GO(t_ui) + temp - 1) / temp) & 0xff;
	dphy->lp_ta_get =
		((DPHY_TIME_LP_TA_GETX(t_ui) + temp - 1) / temp) & 0xff;
	dphy->init = (DPHY_TIME_INIT(t_ui) + temp - 1) / temp;
	dphy->wakeup = (DPHY_TIME_WAKEUP(t_ui) + temp - 1) / temp;

	dphy->clk_pre = ((DPHY_TIME_CLK_PRE(t_ui) + temp - 1) / temp) & 0xff;

	t_req_max = 95 * 100;
	t_req_min = 38 * 100;
	val = (t_req_max + t_req_min) / 2;
	dphy->clk_prepare = val / temp;
	if ((dphy->clk_prepare * temp) < t_req_min)
		dphy->clk_prepare += 1;

	t_req_min = 300 * 100 - dphy->clk_prepare * temp + 10 * 100;
	dphy->clk_zero = t_req_min / temp;
	if ((dphy->clk_zero * temp) < t_req_min)
		dphy->clk_zero += 1;

	//t_req_max = 105 + (12 * t_ui) / 100;
	t_req_min = 60 * 100;
	//val = (t_req_max + t_req_min) / 2;
	dphy->clk_trail = t_req_min / temp;
	if ((dphy->clk_trail * temp) < t_req_min)
		dphy->clk_trail += 1;

	t_req_min = 60 * 100 + 52 * t_ui + 30 * 100;
	dphy->clk_post = t_req_min / temp;
	if ((dphy->clk_post * temp) < t_req_min)
		dphy->clk_post += 1;

	t_req_min = 100 * 100;
	dphy->hs_exit = t_req_min / temp;
	if ((dphy->hs_exit * temp) < t_req_min)
		dphy->hs_exit += 1;

	//t_req_max = 105 + (12 * t_ui) / 100;
	t_req_min = ((8 * t_ui) > (60 * 100 + 4 * t_ui)) ?
		(8 * t_ui) : (60 * 100 + 4 * t_ui);
	//val = (t_req_max + t_req_min) / 2;
	dphy->hs_trail = t_req_min / temp;
	if ((dphy->hs_trail * temp) < t_req_min)
		dphy->hs_trail += 1;

	t_req_min = 40 * 100 + 4 * t_ui;
	t_req_max = 85 * 100 + 6 * t_ui;
	val = (t_req_max + t_req_min) / 2;
	dphy->hs_prepare = val / temp;
	if ((dphy->hs_prepare * temp) < t_req_min)
		dphy->hs_prepare += 1;

	t_req_min = 145 * 100 + 10 * t_ui - dphy->hs_prepare * temp;
	dphy->hs_zero = t_req_min / temp;
	if ((dphy->hs_zero * temp) < t_req_min)
		dphy->hs_zero += 1;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("%s:\n"
			"lp_tesc     = 0x%02x\n"
			"lp_lpx      = 0x%02x\n"
			"lp_ta_sure  = 0x%02x\n"
			"lp_ta_go    = 0x%02x\n"
			"lp_ta_get   = 0x%02x\n"
			"hs_exit     = 0x%02x\n"
			"hs_trail    = 0x%02x\n"
			"hs_zero     = 0x%02x\n"
			"hs_prepare  = 0x%02x\n"
			"clk_trail   = 0x%02x\n"
			"clk_post    = 0x%02x\n"
			"clk_zero    = 0x%02x\n"
			"clk_prepare = 0x%02x\n"
			"clk_pre     = 0x%02x\n"
			"init        = 0x%02x\n"
			"wakeup      = 0x%02x\n\n",
			__func__,
			dphy->lp_tesc, dphy->lp_lpx, dphy->lp_ta_sure,
			dphy->lp_ta_go, dphy->lp_ta_get, dphy->hs_exit,
			dphy->hs_trail, dphy->hs_zero, dphy->hs_prepare,
			dphy->clk_trail, dphy->clk_post,
			dphy->clk_zero, dphy->clk_prepare, dphy->clk_pre,
			dphy->init, dphy->wakeup);
	}
}

static void mipi_dsi_video_config(struct lcd_config_s *pconf)
{
	unsigned short h_period, hs_width, hs_bp;
	unsigned int den, num;
	unsigned short v_period, v_active, vs_width, vs_bp;

	h_period = pconf->basic.h_period;
	hs_width = pconf->timing.hsync_width;
	hs_bp = pconf->timing.hsync_bp;
	den = pconf->control.mipi_cfg.factor_denominator;
	num = pconf->control.mipi_cfg.factor_numerator;

	dsi_vconf.hline = (h_period * den + num - 1) / num;
	dsi_vconf.hsa = (hs_width * den + num - 1) / num;
	dsi_vconf.hbp = (hs_bp * den + num - 1) / num;

	v_period = pconf->basic.v_period;
	v_active = pconf->basic.v_active;
	vs_width = pconf->timing.vsync_width;
	vs_bp = pconf->timing.vsync_bp;
	dsi_vconf.vsa = vs_width;
	dsi_vconf.vbp = vs_bp;
	dsi_vconf.vfp = v_period - v_active - vs_bp - vs_width;
	dsi_vconf.vact = v_active;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("MIPI DSI video timing:\n"
			"  HLINE     = %d\n"
			"  HSA       = %d\n"
			"  HBP       = %d\n"
			"  VSA       = %d\n"
			"  VBP       = %d\n"
			"  VFP       = %d\n"
			"  VACT      = %d\n\n",
			dsi_vconf.hline, dsi_vconf.hsa, dsi_vconf.hbp,
			dsi_vconf.vsa, dsi_vconf.vbp, dsi_vconf.vfp, dsi_vconf.vact);
	}
}

static void mipi_dsi_non_burst_packet_config(struct lcd_config_s *pconf)
{
	struct dsi_config_s *dconf = &pconf->control.mipi_cfg;
	unsigned int lane_num, clk_factor, hactive, multi_pkt_en;
	unsigned int bit_rate_required;
	unsigned int pixel_per_chunk = 0, vid_num_chunks = 0;
	unsigned int byte_per_chunk = 0, vid_pkt_byte_per_chunk = 0;
	unsigned int total_bytes_per_chunk = 0;
	unsigned int chunk_overhead = 0, vid_null_size = 0;
	int i = 1, done = 0;

	lane_num = (int)(dconf->lane_num);
	clk_factor = dconf->clk_factor;
	hactive = pconf->basic.h_active;
	bit_rate_required = pconf->timing.lcd_clk * 3 * dsi_vconf.data_bits;
	bit_rate_required = bit_rate_required / lane_num;
	if (pconf->timing.bit_rate > bit_rate_required)
		multi_pkt_en = 1;
	else
		multi_pkt_en = 0;
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("non-burst: bit_rate_required=%d, bit_rate=%d, multi_pkt_en=%d\n",
		      bit_rate_required, pconf->timing.bit_rate, multi_pkt_en);
	}

	if (multi_pkt_en == 0) {
		pixel_per_chunk = hactive;
		if (dconf->dpi_data_format == COLOR_18BIT_CFG_1) {
			/* 18bit (4*18/8=9byte) */
			byte_per_chunk = pixel_per_chunk * 9/4;
		} else {
			/* 24bit or 18bit-loosely */
			byte_per_chunk = pixel_per_chunk * 3;
		}
		vid_pkt_byte_per_chunk = 4 + byte_per_chunk + 2;
		total_bytes_per_chunk = (lane_num * pixel_per_chunk * clk_factor) / 8;

		vid_num_chunks = 0;
		vid_null_size = 0;
	} else {
#if 0
		i = hactive/8;
		while ((i >= 1) && (done == 0)) {
			pixel_per_chunk = i * 4;
			if (dconf->dpi_data_format == COLOR_18BIT_CFG_1) {
				/* 18bit (4*18/8=9byte) */
				byte_per_chunk = pixel_per_chunk * 9/4;
			} else {
				/* 24bit or 18bit-loosely */
				byte_per_chunk = pixel_per_chunk * 3;
			}
			vid_pkt_byte_per_chunk = 4 + byte_per_chunk + 2;
			total_bytes_per_chunk =
				(lane_num * pixel_per_chunk * clk_factor) / 8;
			vid_num_chunks = hactive / pixel_per_chunk;

			chunk_overhead = total_bytes_per_chunk - vid_pkt_byte_per_chunk;
			if (chunk_overhead >= 12) {
				vid_null_size = chunk_overhead - 12;
				done = 1;
			} else if (chunk_overhead >= 6) {
				vid_null_size = 0;
				done = 1;
			}
			i--;
		}
#else
		i = 2;
		while ((i < hactive) && (done == 0)) {
			vid_num_chunks = i;
			pixel_per_chunk = hactive / vid_num_chunks;

			if (dconf->dpi_data_format == COLOR_18BIT_CFG_1) {
				if ((pixel_per_chunk % 4) > 0)
					continue;
				/* 18bit (4*18/8=9byte) */
				byte_per_chunk = pixel_per_chunk * 9/4;
			} else {
				/* 24bit or 18bit-loosely */
				byte_per_chunk = pixel_per_chunk * 3;
			}
			vid_pkt_byte_per_chunk = 4 + byte_per_chunk + 2;
			total_bytes_per_chunk =
				(lane_num * pixel_per_chunk * clk_factor) / 8;

			chunk_overhead = total_bytes_per_chunk - vid_pkt_byte_per_chunk;
			if (chunk_overhead >= 12) {
				vid_null_size = chunk_overhead - 12;
				done = 1;
			} else if (chunk_overhead >= 6) {
				vid_null_size = 0;
				done = 1;
			}
			i += 2;
		}
#endif
		if (done == 0) {
			LCDERR("Packet no room for chunk_overhead\n");
			//pixel_per_chunk = hactive;
			//vid_num_chunks = 0;
			//vid_null_size = 0;
		}
	}

	dsi_vconf.pixel_per_chunk = pixel_per_chunk;
	dsi_vconf.vid_num_chunks = vid_num_chunks;
	dsi_vconf.vid_null_size = vid_null_size;
	dsi_vconf.byte_per_chunk = byte_per_chunk;
	dsi_vconf.multi_pkt_en = multi_pkt_en;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("MIPI DSI NON-BURST setting:\n"
			"  multi_pkt_en             = %d\n"
			"  vid_num_chunks           = %d\n"
			"  pixel_per_chunk          = %d\n"
			"  byte_per_chunk           = %d\n"
			"  vid_pkt_byte_per_chunk   = %d\n"
			"  total_bytes_per_chunk    = %d\n"
			"  chunk_overhead           = %d\n"
			"  vid_null_size            = %d\n\n",
			multi_pkt_en, vid_num_chunks,
			pixel_per_chunk, byte_per_chunk,
			vid_pkt_byte_per_chunk, total_bytes_per_chunk,
			chunk_overhead, vid_null_size);
	}
}

static void mipi_dsi_vid_mode_config(struct lcd_config_s *pconf)
{
	if (pconf->control.mipi_cfg.video_mode_type == BURST_MODE) {
		dsi_vconf.pixel_per_chunk = pconf->basic.h_active;
		dsi_vconf.vid_num_chunks = 0;
		dsi_vconf.vid_null_size = 0;
	} else {
		mipi_dsi_non_burst_packet_config(pconf);
	}

	mipi_dsi_video_config(pconf);
}

static void mipi_dsi_link_on(struct aml_lcd_drv_s *pdrv)
{
	unsigned int op_mode_init, op_mode_disp;
	struct dsi_config_s *dconf;
#ifdef CONFIG_AML_LCD_EXTERN
	struct lcd_extern_driver_s *edrv;
	struct lcd_extern_dev_s *edev;
#endif

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	dconf = &pdrv->config.control.mipi_cfg;
	op_mode_init = dconf->operation_mode_init;
	op_mode_disp = dconf->operation_mode_display;

	if (dconf->dsi_init_on) {
		dsi_write_cmd(pdrv, dconf->dsi_init_on);
		LCDPR("[%d]: dsi init on\n", pdrv->index);
	}

#ifdef CONFIG_AML_LCD_EXTERN
	if (dconf->extern_init < LCD_EXTERN_INDEX_INVALID) {
		edrv = lcd_extern_get_driver(pdrv->index);
		edev = lcd_extern_get_dev(edrv, dconf->extern_init);
		if (!edrv || !edev) {
			LCDPR("[%d]: no lcd_extern dev\n", pdrv->index);
		} else {
			if (edev->config.table_init_on) {
				dsi_write_cmd(pdrv, edev->config.table_init_on);
				LCDPR("[%d]: [extern]%s dsi init on\n",
				      pdrv->index, edev->config.name);
			}
		}
	}
#endif

	if (op_mode_disp != op_mode_init) {
		set_mipi_dsi_host(MIPI_DSI_VIRTUAL_CHAN_ID,
			0, /* Chroma sub sample, only for
			    * YUV 422 or 420, even or odd
			    */
			op_mode_disp, /* DSI operation mode, video or command */
			pdrv);
	}
}

void mipi_dsi_link_off(struct aml_lcd_drv_s *pdrv)
{
	unsigned int op_mode_init, op_mode_disp;
	struct dsi_config_s *dconf;
#ifdef CONFIG_AML_LCD_EXTERN
	struct lcd_extern_driver_s *edrv;
	struct lcd_extern_dev_s *edev;
#endif

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	dconf = &pdrv->config.control.mipi_cfg;
	op_mode_init = dconf->operation_mode_init;
	op_mode_disp = dconf->operation_mode_display;

	if (op_mode_disp != op_mode_init) {
		set_mipi_dsi_host(MIPI_DSI_VIRTUAL_CHAN_ID,
			0, /* Chroma sub sample, only for
			    * YUV 422 or 420, even or odd
			    */
			op_mode_init, /* DSI operation mode, video or command */
			pdrv);
	}

#ifdef CONFIG_AML_LCD_EXTERN
	if (dconf->extern_init < LCD_EXTERN_INDEX_INVALID) {
		edrv = lcd_extern_get_driver(pdrv->index);
		edev = lcd_extern_get_dev(edrv, dconf->extern_init);
		if (!edrv || !edev) {
			LCDPR("[%d]: no lcd_extern dev\n", pdrv->index);
		} else {
			if (edev->config.table_init_off) {
				dsi_write_cmd(pdrv, edev->config.table_init_off);
				LCDPR("[%d]: [extern]%s dsi init off\n",
				      pdrv->index, edev->config.name);
			}
		}
	}
#endif

	if (dconf->dsi_init_off) {
		dsi_write_cmd(pdrv, dconf->dsi_init_off);
		LCDPR("[%d]: dsi init off\n", pdrv->index);
	}
}

void mipi_dsi_config_init(struct lcd_config_s *pconf)
{
	struct dsi_config_s *dconf = &pconf->control.mipi_cfg;
	unsigned int status;

	/* data format */
	if (pconf->basic.lcd_bits == 6) {
		dconf->venc_data_width = MIPI_DSI_VENC_COLOR_18B;
		dconf->dpi_data_format = MIPI_DSI_COLOR_18BIT;
		if (dconf->dpi_data_format == COLOR_18BIT_CFG_2)
			dconf->data_bits = 8;
		else
			dconf->data_bits = 6;
	} else {
		dconf->venc_data_width = MIPI_DSI_VENC_COLOR_24B;
		dconf->dpi_data_format  = MIPI_DSI_COLOR_24BIT;
		dconf->data_bits = 8;
	}
	dsi_vconf.data_bits = dconf->data_bits;

	/* check_state */
	if (dconf->check_en) {
		status = env_get_ulong("lcd_mipi_check", 10, 0xffff);
		if (status == 0) {
			dconf->check_en = 0;
			LCDPR("lcd_mipi_check disable check_state\n");
		}
	}
	dconf->check_state = 0;

	/* Venc resolution format */
	switch (dconf->phy_switch) {
	case 1: /* standard */
		dsi_phy_config.state_change = 1;
		break;
	case 2: /* slow */
		dsi_phy_config.state_change = 2;
		break;
	case 0: /* auto */
	default:
		if ((pconf->basic.h_active != 240) &&
		    (pconf->basic.h_active != 768) &&
		    (pconf->basic.h_active != 1920) &&
		    (pconf->basic.h_active != 2560)) {
			dsi_phy_config.state_change = 2;
		} else {
			dsi_phy_config.state_change = 1;
		}
		break;
	}
}

/* bit_rate is confirm by clk_genrate, so internal clk config must after that */
static void mipi_dsi_config_post(struct lcd_config_s *pconf)
{
	unsigned int pclk, lanebyteclk;
	unsigned int den, num;
	struct dsi_config_s *dconf = &pconf->control.mipi_cfg;

	pclk = pconf->timing.lcd_clk / 1000;

	/* pclk lanebyteclk factor */
	if (dconf->factor_numerator == 0) {
		lanebyteclk = pconf->timing.bit_rate / 8 / 1000;
		LCDPR("pixel_clk = %d.%03dMHz, bit_rate = %d.%03dMHz, lanebyteclk = %d.%03dMHz\n",
			(pclk / 1000), (pclk % 1000),
			(pconf->timing.bit_rate / 1000000),
			((pconf->timing.bit_rate / 1000) % 1000),
			(lanebyteclk / 1000), (lanebyteclk % 1000));
#if 0
		dconf->factor_numerator = pclk;
		dconf->factor_denominator = lanebyteclk;
#else
		dconf->factor_numerator = 8;
		dconf->factor_denominator = dconf->clk_factor;
#endif
	}
	num = dconf->factor_numerator;
	den = dconf->factor_denominator;
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("num=%d, den=%d, factor=%d.%02d\n",
			num, den, (den / num), ((den % num) * 100 / num));
	}

	if (dconf->operation_mode_display == OPERATION_VIDEO_MODE)
		mipi_dsi_vid_mode_config(pconf);

	/* phy config */
	mipi_dsi_phy_config(&dsi_phy_config, pconf->timing.bit_rate);
}

static void mipi_dsi_host_on(struct aml_lcd_drv_s *pdrv)
{
	unsigned int op_mode_init, op_mode_disp;
	unsigned int venc_en = 0;
	int index = pdrv->index;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	lcd_venc_enable(pdrv, 0);
	venc_en = 0;
	udelay(100);

	mipi_dsi_config_post(&pdrv->config);

	startup_mipi_dsi_host(index);

	set_dsi_phy_config(pdrv);

	op_mode_init = pdrv->config.control.mipi_cfg.operation_mode_init;
	op_mode_disp = pdrv->config.control.mipi_cfg.operation_mode_display;
	mipi_dcs_set(index, MIPI_DSI_CMD_TRANS_TYPE, /* 0: high speed, 1: low power */
		MIPI_DSI_DCS_ACK_TYPE,        /* if need bta ack check */
		MIPI_DSI_TEAR_SWITCH);        /* enable tear ack */

	set_mipi_dsi_host(MIPI_DSI_VIRTUAL_CHAN_ID,   /* Virtual channel id */
		0, /* Chroma sub sample, only for YUV 422 or 420, even or odd */
		op_mode_init, /* DSI operation mode, video or command */
		pdrv);

	/* Startup transfer */
	mipi_dsi_lpclk_ctrl(index, &pdrv->config.control.mipi_cfg);
	if (op_mode_init == MIPI_DSI_OPERATION_MODE_VIDEO) {
		lcd_venc_enable(pdrv, 1);
		venc_en = 1;
	}

	mipi_dsi_link_on(pdrv);
	if (op_mode_disp == MIPI_DSI_OPERATION_MODE_VIDEO) {
		if (venc_en == 0) {
			lcd_venc_enable(pdrv, 1);
			venc_en = 1;
		}
	}

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		mipi_dsi_host_print_info(&pdrv->config);
}

static void mipi_dsi_host_off(struct aml_lcd_drv_s *pdrv)
{
	int index = pdrv->index;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	/* Power down DSI */
	dsi_host_write(index, MIPI_DSI_DWC_PWR_UP_OS, 0);

	/* Power down D-PHY, do not have to close dphy */
	/* dsi_host_write(index, MIPI_DSI_DWC_PHY_RSTZ_OS,
	 *	(dsi_host_read(index, MIPI_DSI_DWC_PHY_RSTZ_OS ) & 0xc));
	 */
	/* dsi_host_write(index, MIPI_DSI_DWC_PHY_RSTZ_OS, 0xc); */

	dsi_phy_write(index, MIPI_DSI_CHAN_CTRL, 0x1f);
	//LCDPR("MIPI_DSI_PHY_CTRL=0x%x\n", dsi_phy_read(index, MIPI_DSI_PHY_CTRL));
	dsi_phy_setb(index, MIPI_DSI_PHY_CTRL, 0, 7, 1);
}

static void mipi_dsi_host_reset(int index)
{
	unsigned int bit;

	if (index)
		bit = 31;
	else
		bit = 30;

	lcd_reset_setb(RESETCTRL_RESET1_MASK, 0, bit, 1);
	lcd_reset_setb(RESETCTRL_RESET1_LEVEL, 0, bit, 1);
	udelay(1);
	lcd_reset_setb(RESETCTRL_RESET1_LEVEL, 1, bit, 1);
	udelay(1);
}

static void mipi_dsi_phy_reset(int index)
{
	unsigned int bit;

	if (index)
		bit = 29;
	else
		bit = 28;

	lcd_reset_setb(RESETCTRL_RESET1_MASK, 0, bit, 1);
	lcd_reset_setb(RESETCTRL_RESET1_LEVEL, 0, bit, 1);
	udelay(1);
	lcd_reset_setb(RESETCTRL_RESET1_LEVEL, 1, bit, 1);
	udelay(1);
}

static void mipi_dsi_power_init(int index)
{
#ifdef CONFIG_SECURE_POWER_CONTROL
//#define PM_MIPI_DSI1     50
//#define PM_MIPI_DSI0     41
	if (index)
		pwr_ctrl_psci_smc(PM_MIPI_DSI1, 1);
	else
		pwr_ctrl_psci_smc(PM_MIPI_DSI0, 1);
	LCDPR("[%d]: dsi power domain on\n", index);
#endif
}

void mipi_dsi_tx_ctrl(struct aml_lcd_drv_s *pdrv, int flag)
{
	if (flag) {
		if (pdrv->data->chip_type == LCD_CHIP_T7) {
			mipi_dsi_power_init(pdrv->index);
			mipi_dsi_phy_reset(pdrv->index);
			mipi_dsi_host_reset(pdrv->index);
		}
		mipi_dsi_host_on(pdrv);
	} else {
		mipi_dsi_host_off(pdrv);
	}
}
