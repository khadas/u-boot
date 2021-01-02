#include <config.h>
#include <linux/kernel.h>
#ifdef CONFIG_SECURE_POWER_CONTROL
#include <asm/arch/pwr_ctrl.h>
#endif
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "lcd_reg.h"
#include "lcd_common.h"

/*******************************************************************************
 * vpu/display init
 ******************************************************************************/
#include "edp_tx_reg.h"
#define EDPTX0_BASE    0xfe0b0000L

static void dptx_reg_write(unsigned int reg, unsigned int val)
{
	*(volatile unsigned int *)(EDPTX0_BASE + reg) = (val);
}

static unsigned int dptx_reg_read(unsigned int reg)
{
	return *(volatile unsigned int *)(EDPTX0_BASE + reg);
}

#define EDP_AUX_RETRY_CNT    5
#define EDP_AUX_TIMEOUT      1000
#define EDP_AUX_INTERVAL     200
static int dptx_aux_write(unsigned int addr, unsigned int len, unsigned char *buf)
{
	unsigned int data, i, state;
	int retry_cnt = 0, timeout = 0;

aux_write_retry:
	do {
		state = dptx_reg_read(EDP_TX_AUX_STATE);
	} while (state & (1 << 1));

	dptx_reg_write(EDP_TX_AUX_ADDRESS, addr);
	for (i = 0; i < len; i++)
		dptx_reg_write(EDP_TX_AUX_WRITE_FIFO, buf[i]);

	dptx_reg_write(EDP_TX_AUX_COMMAND, (0x800 | ((len - 1) & 0xf)));

	while (timeout++ < EDP_AUX_TIMEOUT) {
		udelay(EDP_AUX_INTERVAL);
		data = dptx_reg_read(EDP_TX_AUX_TRANSFER_STATUS);
		if (data & (1 << 0)) {
			state = dptx_reg_read(EDP_TX_AUX_REPLY_CODE);
			if (state == 0)
				return 0;
			if (state == 1) {
				LCDPR("edp aux write addr 0x%x NACK!\n", addr);
				return -1;
			}
			if (state == 2)
				LCDPR("  edp aux write addr 0x%x Defer!\n", addr);
			break;
		}

		if (data & (1 << 3)) {
			LCDPR("  edp aux write addr 0x%x Error!\n", addr);
			break;
		}
	}

	if (retry_cnt++ < EDP_AUX_RETRY_CNT) {
		udelay(EDP_AUX_INTERVAL);
		LCDPR("edp aux write addr 0x%x timeout, retry %d\n", addr, retry_cnt);
		goto aux_write_retry;
	}

	LCDPR("edp aux write addr 0x%x failed\n", addr);
	return -1;
}

static int dptx_aux_read(unsigned int addr, unsigned int len, unsigned char *buf)
{
	unsigned int data, i, state;
	int retry_cnt = 0, timeout = 0;

aux_read_retry:
	do {
		state = dptx_reg_read(EDP_TX_AUX_STATE);
	} while (state & (1 << 1));

	dptx_reg_write(EDP_TX_AUX_ADDRESS, addr);
	dptx_reg_write(EDP_TX_AUX_COMMAND, (0x900 | ((len - 1) & 0xf)));

	while (timeout++ < EDP_AUX_TIMEOUT) {
		udelay(EDP_AUX_INTERVAL);
		data = dptx_reg_read(EDP_TX_AUX_TRANSFER_STATUS);
		if (data & (1 << 0)) {
			state = dptx_reg_read(EDP_TX_AUX_REPLY_CODE);
			if (state == 0)
				goto aux_read_succeed;
			if (state == 1) {
				LCDPR("edp aux read addr 0x%x NACK!\n", addr);
				return -1;
			}
			if (state == 2)
				LCDPR("  edp aux read addr 0x%x Defer!\n", addr);
			break;
		}

		if (data & (1 << 3)) {
			LCDPR("  edp aux read addr 0x%x Error!\n", addr);
			break;
		}
	}

	if (retry_cnt++ < EDP_AUX_RETRY_CNT) {
		udelay(EDP_AUX_INTERVAL);
		LCDPR("  edp aux read addr 0x%x timeout, retry %d\n", addr, retry_cnt);
		goto aux_read_retry;
	}

	LCDPR("edp aux read addr 0x%x failed\n", addr);
	return -1;

aux_read_succeed:
	for (i = 0; i < len; i++)
		buf[i] = (unsigned char)(dptx_reg_read(EDP_TX_AUX_REPLY_DATA));

	return 0;
}

static void dptx_link_fast_training(void)
{
	unsigned char p_data = 0;
	int ret;

	LCDPR("..... Sending edp training pattern 1 .....\n");
	dptx_reg_write(EDP_TX_SCRAMBLING_DISABLE ,0x1);   // disable scrambling
	dptx_reg_write(EDP_TX_TRAINING_PATTERN_SET, 0x1); // set training pattern 1

	p_data = 0x21;
	ret = dptx_aux_write(0x102, 1, &p_data);    // set pattern 1 in the RX
	if (ret) {
		LCDPR("..... edp training pattern 1 failed.....\n");
		//return;
	}

	udelay(10);

	LCDPR("..... Sending training pattern 2 .....\n");
	dptx_reg_write(EDP_TX_TRAINING_PATTERN_SET, 0x2);
	p_data = 0x22;
	ret = dptx_aux_write(0x102, 1, &p_data);      // set pattern 2 in the RX
	if (ret) {
		LCDPR("..... edp training pattern 2 failed.....\n");
		//return;
	}

	udelay(10);

	LCDPR("..... Sending training pattern 3 .....\n");
	dptx_reg_write(EDP_TX_TRAINING_PATTERN_SET, 0x3);
	p_data = 0x23;
	ret = dptx_aux_write(0x102, 1, &p_data);      // set pattern 3 in the RX
	if (ret) {
		LCDPR("..... edp training pattern 3 failed.....\n");
		//return;
	}

	udelay(10);

	p_data = 0x20;
	ret = dptx_aux_write(0x102, 1, &p_data);         // pattern off
	if (ret) {
		LCDPR("..... edp training pattern off failed.....\n");
		//return;
	}

	// disable the training pattern
	dptx_reg_write(EDP_TX_TRAINING_PATTERN_SET, 0x0);
}

static void dptx_dpcd_dump(void)
{
	unsigned char p_data[12];
	int ret, i;

	memset(p_data, 0, 12);
	LCDPR("edp DPCD link status:\n");
	ret = dptx_aux_read(0x100, 8, p_data);
	if (ret == 0) {
		for (i = 0; i < 8; i++)
			LCDPR("0x%04x: 0x%02x\n", (0x100 + i), p_data[i]);
		LCDPR("\n");
	}

	memset(p_data, 0, 12);
	LCDPR("edp DPCD training status:\n");
	ret = dptx_aux_read(0x200, 12, p_data);
	if (ret == 0) {
		for (i = 0; i < 12; i++)
			LCDPR("0x%04x: 0x%02x\n", (0x200 + i), p_data[i]);
		LCDPR("\n");
	}
}

static void dptx_set_msa(void)
{
	unsigned int hactive = 1920;
	unsigned int vactive = 1080;
	unsigned int htotal = 2200;
	unsigned int vtotal = 1120;
	unsigned int hsw = 44;
	unsigned int hbp = 148;
	unsigned int vsw = 5;
	unsigned int vbp = 30;
	//unsigned int pclk = 147840000;

	//unsigned int lane_count = 2;
	unsigned int data_per_lane;
	unsigned int misc0_data;
	unsigned int m_vid = 147840; //pclk/1000
	unsigned int n_vid = 270000; //162000, 270000, 540000

	unsigned int ppc = 1;// 1 pix per clock pix0 only
	unsigned int cfmt = 0;// RGB
	unsigned int bpc = 8;// bits per color

	data_per_lane = ((hactive * bpc * 3) + 15) / 16 - 1;

	//bit[0] sync mode (1=sync 0=async)
	misc0_data = (cfmt << 1) | (1 << 0);
	misc0_data |= (0x1 << 5); //6bit:0x0, 8bit:0x1, 10bit:0x2, 12bit:0x3

	dptx_reg_write(EDP_TX_MAIN_STREAM_HTOTAL, htotal);
	dptx_reg_write(EDP_TX_MAIN_STREAM_VTOTAL, vtotal);
	dptx_reg_write(EDP_TX_MAIN_STREAM_POLARITY, (0 << 1) | (0 << 0));
	dptx_reg_write(EDP_TX_MAIN_STREAM_HSWIDTH, hsw);
	dptx_reg_write(EDP_TX_MAIN_STREAM_VSWIDTH, vsw);
	dptx_reg_write(EDP_TX_MAIN_STREAM_HRES, hactive);
	dptx_reg_write(EDP_TX_MAIN_STREAM_VRES, vactive);
	dptx_reg_write(EDP_TX_MAIN_STREAM_HSTART, (hsw + hbp));
	dptx_reg_write(EDP_TX_MAIN_STREAM_VSTART, (vsw + vbp));
	dptx_reg_write(EDP_TX_MAIN_STREAM_MISC0, misc0_data);
	dptx_reg_write(EDP_TX_MAIN_STREAM_MISC1, 0x00000000);
	dptx_reg_write(EDP_TX_MAIN_STREAM_M_VID, m_vid); //unit: 1kHz
	dptx_reg_write(EDP_TX_MAIN_STREAM_N_VID, n_vid); //unit: 10kHz
	dptx_reg_write(EDP_TX_MAIN_STREAM_TRANSFER_UNIT_SIZE, 32);
	dptx_reg_write(EDP_TX_MAIN_STREAM_DATA_COUNT_PER_LANE, data_per_lane);
	dptx_reg_write(EDP_TX_MAIN_STREAM_USER_PIXEL_WIDTH, ppc);

	LCDPR(" edp MSA done\n");
}

static void dptx_reset(void)
{
	unsigned int data32;

	// Level reset mail
	data32 = readl(RESETCTRL_RESET1_MASK);
	data32 &= (~(0x1 << 17));
	writel(data32, RESETCTRL_RESET1_MASK);
	data32 = readl(RESETCTRL_RESET1_LEVEL);
	data32 &= (~(0x1 << 17));
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(1);
	data32 |= (0x1 << 17);
	writel(data32, RESETCTRL_RESET1_LEVEL);

	// Set clock-divider for EDP-APB
	dptx_reg_write(EDP_TX_AUX_CLOCK_DIVIDER, 24); // Set Aux channel clk-div: 24MHz
}

static void dptx_phy_reset(void)
{
	unsigned int data32;

	// Level reset mail
	data32 = readl(RESETCTRL_RESET1_MASK);
	data32 &= (~(0x1 << 19));
	writel(data32, RESETCTRL_RESET1_MASK);
	data32 = readl(RESETCTRL_RESET1_LEVEL);
	data32 &= (~(0x1 << 19));
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(1);
	data32 |= (0x1 << 19);
	writel(data32, RESETCTRL_RESET1_LEVEL);
}

static void dptx_init(void)
{
	unsigned int data32;

	// Select Data-in
	data32 = readl(COMBO_DPHY_CNTL0);
	data32 &= (~((0x1 << 0) | (0x1 << 1)));
	data32 |= ((0x0 << 0) | (0x1 << 1)); //reg_phy0_din_sel_edp
	writel(data32, COMBO_DPHY_CNTL0);

	data32 = (0x1 | (0x1 << 4) | (0x0 << 5) | (0x1 << 6) | (0x1 << 7));
	writel(data32, COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1);
	data32 = ((0x4 << 5) | (0x1f << 16));
	writel(data32, COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0); //select clkdiv20 , datachan-sel
	data32 = readl(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0);
	data32 &= ~(0x1 << 2);  //no pn sawp
	writel(data32, COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0);
	// Mux pads in combo-phy -- Lanes 0 through 4
	data32 = readl(COMBO_DPHY_CNTL1);
	data32 &= (~(0x3 | (0x3 << 2) | (0x3 << 4) | (0x3 << 6) | (0x3 << 8)));
	data32 |= (0x1 | (0x1 << 2) | (0x1 << 4) | (0x1 << 6) | (0x1 << 8));
	writel(data32, COMBO_DPHY_CNTL1); // Select EDP in combo-phy

	//Hot-plug GPIO
	data32 = readl(PADCTRL_PIN_MUX_REGK);
	data32 &= (~(0xf << 8));
	data32 |= (0x4 << 8);
	writel(data32, PADCTRL_PIN_MUX_REGK);

	// Set clock-divider for EDP-APB
	dptx_reg_write(EDP_TX_AUX_CLOCK_DIVIDER, 24); // Set Aux channel clk-div: 24MHz
}

static void dptx_set_phy_clk(uint16_t lnkclk_x100)
{
	dptx_phy_reset();
	dptx_reset();
}

static void dptx_set_pixclk_divN(uint32_t N)
{
	unsigned int data32;
	unsigned int sel0; // 1,2,3,4,5,7,8,9,11,13,17,19,23,29,31
	unsigned int sel1; // 1,2,3,4,5,6,7,8,9,13

	switch (N) {
	case 10:
		sel0 = 4;
		sel1 = 1;
		break;
	case 13:
		sel0 = 9;
		sel1 = 0;
		break;
	case 18:
		sel0 = 2;
		sel1 = 5;
		break;
	case 64:
		sel0 = 6;
		sel1 = 7;
		break;
	default:
		sel0 = 2;
		sel1 = 1;
		break;
	}

	// Disable edp_div clock
	//Wr(COMBO_DPHY_EDP_PIXEL_CLK_DIV, 0x0);  // Note this register is shared with edp1 so need to set specific bits
	data32 = readl(COMBO_DPHY_EDP_PIXEL_CLK_DIV);
	data32 &= (~(0x1 << 24));
	writel(data32, COMBO_DPHY_EDP_PIXEL_CLK_DIV);

	// Disable hdmi clk_div clock output + bypass
	writel((0x0 << 19) | (0x1 << 18), COMBO_DPHY_VID_PLL0_DIV);

	// Set EDP clock divider
	data32 = readl(COMBO_DPHY_EDP_PIXEL_CLK_DIV);
	data32 &= (~(0xf | (0xf << 4)));
	data32 |= ((sel0 << 0) | (sel1 << 4));
	writel(data32, COMBO_DPHY_EDP_PIXEL_CLK_DIV);

	// Enable hdmi clk_div clock output + bypass
	writel((0x1 << 19) | (0x1 << 18), COMBO_DPHY_VID_PLL0_DIV);

	// Enable edp_div clock
	data32 = readl(COMBO_DPHY_EDP_PIXEL_CLK_DIV);
	data32 |= (0x1 << 24);
	writel(data32, COMBO_DPHY_EDP_PIXEL_CLK_DIV);

	dptx_reset();  // Reset controller : IP is sync reset, may not have been reset if
			// first time video clock
}

static int dptx_wait_phy_ready(void)
{
	unsigned int data = 0;
	unsigned int done = 100;
	int ret;

	do {
		data = dptx_reg_read(EDP_TX_PHY_STATUS);
		if (done < 20) {
			LCDPR("dptx wait phy ready: reg_val=0x%x, wait_count=%u\n",
				data, (100 - done));
		}
		done--;
		udelay(100);
	}while(((data & 0x7f) != 0x7f) && (done > 0));

	if ((data & 0x7f) == 0x7f)
		return 0;

	LCDPR(" edp tx phy error!\n");
	return -1;
}

static void edp_tx_init(void)
{
	unsigned char edptx_auxdata[2];
	int ret;

	dptx_reset();
	dptx_init();  // initialize aux-channel clk_div

	dptx_set_phy_clk(270);  // 2.7GHz link
	dptx_set_pixclk_divN(18);

	dptx_reset();
	mdelay(10);

	LCDPR(" edp tx reset done, start host\n");

	// Enable the transmitter
	//dptx_reg_write(EDP_TX_TRANSMITTER_OUTPUT_ENABLE, 0x1);
	dptx_reg_write(EDP_TX_PHY_RESET, 0);  // remove the reset on the PHY
	dptx_wait_phy_ready();
	mdelay(10);
	dptx_reg_write(EDP_TX_TRANSMITTER_OUTPUT_ENABLE, 0x1);

	// Program Link-rate and Lane_count
	dptx_reg_write(EDP_TX_LINK_BW_SET, 0x0a); // Link-rate
	dptx_reg_write(EDP_TX_LINK_COUNT_SET, 0x02); // Number of Lanes

	// Program Lane count
	LCDPR("edp set lane bw & count\n");
	edptx_auxdata[0] = 0x0a; // 2.7GHz
	edptx_auxdata[1] = 2;
	ret = dptx_aux_write(0x100, 2, edptx_auxdata);
	if (ret) {
		LCDPR("..... edp set lane bw & count failed.....\n");
		//return;
	}

	// Power up link
	LCDPR("edp power up link\n");
	edptx_auxdata[0] = 0x1;
	ret = dptx_aux_write(0x600, 1, edptx_auxdata);
	if (ret) {
		LCDPR("..... edp power up link failed.....\n");
		//return;
	}

	// Fast Link train
	dptx_link_fast_training();
	//dptx_dpcd_dump();

	dptx_set_msa();

	LCDPR("edp enabling the main stream video\n");
	dptx_reg_write(EDP_TX_MAIN_STREAM_ENABLE, 0x1); // Enable main-link

	LCDPR(" edp tx finish\n");
}

static void edp_power_init(void)
{
//#define PM_EDP0          48
//#define PM_EDP1          49
//#define PM_MIPI_DSI1     50
//#define PM_MIPI_DSI0     41
	pwr_ctrl_psci_smc(PM_EDP0, 1);
	pwr_ctrl_psci_smc(PM_EDP1, 1);

	LCDPR(" edp power domain on\n");
}

static void lcd_init_pre_edp(void)
{
	unsigned int data32;
	int cnt = 0, i;

	//1.config pll
set_pll_retry_edp:
	writel(0x000f04e1, ANACTRL_TCON_PLL0_CNTL0);
	udelay(10);
	writel(0x200f04e1, ANACTRL_TCON_PLL0_CNTL0);
	udelay(10);
	writel(0x300f04e1, ANACTRL_TCON_PLL0_CNTL0);
	udelay(10);
	writel(0x10000000, ANACTRL_TCON_PLL0_CNTL1);
	udelay(10);
	writel(0x0000110c, ANACTRL_TCON_PLL0_CNTL2);
	udelay(10);
	writel(0x10051400, ANACTRL_TCON_PLL0_CNTL3);
	udelay(10);
	writel(0x000100c0, ANACTRL_TCON_PLL0_CNTL4);
	udelay(10);
	writel(0x008300c0, ANACTRL_TCON_PLL0_CNTL4);
	udelay(10);
	writel(0x340f04e1, ANACTRL_TCON_PLL0_CNTL0);
	udelay(10);
	writel(0x140f04e1, ANACTRL_TCON_PLL0_CNTL0);
	udelay(10);
	writel(0x0000300c, ANACTRL_TCON_PLL0_CNTL2);
	udelay(100);
	i = 0;
	while (i++ < 200) {
		udelay(50);
		if (readl(ANACTRL_TCON_PLL0_STS) & 0x80000000)
			break;
	}
	if (!(readl(ANACTRL_TCON_PLL0_STS) & 0x80000000)) {
		if (cnt++ < 20)
			goto set_pll_retry_edp;
		else
			LCDPR(" pll lock failed!!!\n");
	}

	//2.config divider
	data32 = readl(CLKCTRL_VIID_CLK0_CTRL);
	//cntrl_clk_en0 disable
	data32 &= ~(1 << 19);
	writel(data32, CLKCTRL_VIID_CLK0_CTRL);
	/* Disable the div output clock */
	data32 = readl(COMBO_DPHY_VID_PLL0_DIV);
	//clk_final_en disable ?
	data32 &= ~(1 << 19);
	writel(data32, COMBO_DPHY_VID_PLL0_DIV);
	//set_preset disable ?
	data32 &= ~(1 << 15);
	writel(data32, COMBO_DPHY_VID_PLL0_DIV);

	data32 = readl(COMBO_DPHY_VID_PLL0_DIV);  //used COMBO not VID
	data32 |= (1 << 18);
	writel(data32, COMBO_DPHY_VID_PLL0_DIV);
	data32 |= (1 << 19);
	writel(data32, COMBO_DPHY_VID_PLL0_DIV);

	//3.config vclk
	writel(0x00000000, CLKCTRL_VIID_CLK0_DIV);
	udelay(5);
	writel(0x00080000, CLKCTRL_VIID_CLK0_CTRL);
	udelay(5);
	writel(0x00008000, CLKCTRL_VIID_CLK0_DIV);
	writel(0x00018000, CLKCTRL_VIID_CLK0_DIV);
	udelay(5);
	writel(0x00080001, CLKCTRL_VIID_CLK0_CTRL);
	writel(0x00088001, CLKCTRL_VIID_CLK0_CTRL);
	udelay(10);
	writel(0x00080001, CLKCTRL_VIID_CLK0_CTRL);
	udelay(5);
	writel(0x00000008, CLKCTRL_VID_CLK0_CTRL2);

	//4. config phy clk
	//done by edp init

	//5. reset phy
	data32 = readl(RESETCTRL_RESET1_MASK);
	data32 &= ~((0x1 << 20) | (0x1 << 19) | (0x1 << 7));
	writel(data32, RESETCTRL_RESET1_MASK);
	data32 = readl(RESETCTRL_RESET1_LEVEL);
	data32 &= ~((0x1 << 20) | (0x1 << 19) | (0x1 << 7));
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(1);
	data32 |= (0x1 << 20) | (0x1 << 19) | (0x1 << 7);
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(10);

	writel(0x2, COMBO_DPHY_CNTL0);
	writel(0x55555555, COMBO_DPHY_CNTL1);

	//6. config phy
	writel(0x46770038, ANACTRL_DIF_PHY_CNTL1);
	writel(0x0000ffff, ANACTRL_DIF_PHY_CNTL2); //0x0100ffff
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL3);
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL4);
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL5);
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL6);
	writel(0x46770038, ANACTRL_DIF_PHY_CNTL10);
	writel(0x0000ffff, ANACTRL_DIF_PHY_CNTL11);
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL12);
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL13);
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL14);
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL15);
	writel(0x00406253, ANACTRL_DIF_PHY_CNTL19);
	writel(0x0, ANACTRL_DIF_PHY_CNTL20);
	writel(0x0, ANACTRL_DIF_PHY_CNTL21);

	LCDPR(" lcd init pre\n");
}

static void lcd_venc_set_edp(void)
{
	unsigned int hactive = 1920;
	unsigned int vactive = 1080;
	unsigned int htotal = 2200;
	unsigned int vtotal = 1120;
	unsigned int hsw = 44;
	unsigned int hbp = 148;
	unsigned int vsw = 5;
	unsigned int vbp = 30;
	//unsigned int pclk = 147840000;
	unsigned int data32;

	writel(0, ENCL_VIDEO_EN);

	writel(0x8000, ENCL_VIDEO_MODE);/*bit[15] shadown en*/
	writel(0x0418, ENCL_VIDEO_MODE_ADV); /* Sampling rate: 1 */

	writel(0x1000, ENCL_VIDEO_FILT_CTRL); /* bypass filter */
	writel(htotal - 1, ENCL_VIDEO_MAX_PXCNT);
	writel(vtotal - 1, ENCL_VIDEO_MAX_LNCNT);
	writel(hsw + hbp, ENCL_VIDEO_HAVON_BEGIN);
	writel(hactive - 1 + hsw + hbp, ENCL_VIDEO_HAVON_END);
	writel(vsw + vbp, ENCL_VIDEO_VAVON_BLINE);
	writel(vactive - 1 + vsw + vbp, ENCL_VIDEO_VAVON_ELINE);

	writel(0, ENCL_VIDEO_HSO_BEGIN);
	writel(hsw, ENCL_VIDEO_HSO_END);
	writel(0, ENCL_VIDEO_VSO_BEGIN);
	writel(0, ENCL_VIDEO_VSO_END);
	writel(0, ENCL_VIDEO_VSO_BLINE);
	writel(vsw, ENCL_VIDEO_VSO_ELINE);
	writel(3, ENCL_VIDEO_RGBIN_CTRL); //yuv: 1, rgb: 3

	writel((5 << 13) | (hactive - 1), ENCL_INBUF_CNTL1);
	writel(0x200, ENCL_INBUF_CNTL0);

	/* default colorbar pattern */
	writel(1, ENCL_TST_MDSEL);
	writel(0x200, ENCL_TST_Y);
	writel(0x200, ENCL_TST_CB);
	writel(0x200, ENCL_TST_CR);
	writel(hsw + hbp, ENCL_TST_CLRBAR_STRT);
	writel(240, ENCL_TST_CLRBAR_WIDTH);
	writel(1, ENCL_TST_EN);
	//writel(0x0410, ENCL_VIDEO_MODE_ADV);

	writel(1, ENCL_VIDEO_EN);

	//select venc to edp
	data32 = (0 << 31) | (0 << 30) | (0 << 29) | (1 << 28);
	writel(data32, VPU_DISP_VIU0_CTRL);

	//config venc_tcon
	writel(0x0, LCD_RGB_BASE_ADDR);
	writel(0x400, LCD_RGB_COEFF_ADDR);
	writel((1 << 0), LCD_POL_CNTL_ADDR);

	/* DE signal */
	writel(hsw + hbp, DE_HS_ADDR);
	writel(hsw + hbp + hactive, DE_HE_ADDR);
	writel(vsw + vbp, DE_VS_ADDR);
	writel(vsw + vbp + vactive - 1, DE_VE_ADDR);

	/* Hsync signal */
	writel(0, HSYNC_HS_ADDR);
	writel(hsw, HSYNC_HE_ADDR);
	writel(0, HSYNC_VS_ADDR);
	writel(vtotal - 1, HSYNC_VE_ADDR);

	/* Vsync signal */
	writel(0, VSYNC_HS_ADDR);
	writel(0, VSYNC_HE_ADDR);
	writel(0, VSYNC_VS_ADDR);
	writel(vsw, VSYNC_VE_ADDR);

	//select encl
	writel(2, VPU_VENC_CTRL);

	LCDPR(" lcd venc init\n");
}

static void lcd_init_pre_lvds(void)
{
	unsigned int data32;
	int cnt = 0, i;

	//1.config pll
set_pll_retry_lvds:
	writel(0x008e04ad, ANACTRL_TCON_PLL2_CNTL0);
	udelay(10);
	writel(0x208e04ad, ANACTRL_TCON_PLL2_CNTL0);
	udelay(10);
	writel(0x308e04ad, ANACTRL_TCON_PLL2_CNTL0);
	udelay(10);
	writel(0x10108000, ANACTRL_TCON_PLL2_CNTL1);
	udelay(10);
	writel(0x0000110c, ANACTRL_TCON_PLL2_CNTL2);
	udelay(10);
	writel(0x10051400, ANACTRL_TCON_PLL2_CNTL3);
	udelay(10);
	writel(0x000100c0, ANACTRL_TCON_PLL2_CNTL4);
	udelay(10);
	writel(0x008300c0, ANACTRL_TCON_PLL2_CNTL4);
	udelay(10);
	writel(0x348e04ad, ANACTRL_TCON_PLL2_CNTL0);
	udelay(10);
	writel(0x148e04ad, ANACTRL_TCON_PLL2_CNTL0);
	udelay(10);
	writel(0x0000300c, ANACTRL_TCON_PLL2_CNTL2);
	//udelay(100);
	//writel(0x0b8300c0, ANACTRL_TCON_PLL2_CNTL4);
	udelay(10);
	i = 0;
	while (i++ < 200) {
		udelay(50);
		if (readl(ANACTRL_TCON_PLL2_STS) & 0x80000000)
			break;
	}
	if (!(readl(ANACTRL_TCON_PLL2_STS) & 0x80000000)) {
		if (cnt++ < 20)
			goto set_pll_retry_lvds;
		else
			LCDPR(" pll lock failed!!!\n");
	}

	//2.config divider
	data32 = readl(CLKCTRL_VIID_CLK2_CTRL);
	data32 &= ~(1 << 19);
	writel(data32, CLKCTRL_VIID_CLK2_CTRL);
	/* Disable the div output clock */
	data32 = readl(COMBO_DPHY_VID_PLL2_DIV);
	data32 &= ~(1 << 19);
	writel(data32, COMBO_DPHY_VID_PLL2_DIV);
	data32 &= ~(1 << 15);
	writel(data32, COMBO_DPHY_VID_PLL2_DIV);

	lcd_hiu_setb(COMBO_DPHY_VID_PLL2_DIV, 0, 18, 1);
	lcd_hiu_setb(COMBO_DPHY_VID_PLL2_DIV, 0, 16, 2);
	lcd_hiu_setb(COMBO_DPHY_VID_PLL2_DIV, 0, 15, 1);
	lcd_hiu_setb(COMBO_DPHY_VID_PLL2_DIV, 0, 0, 14);

	lcd_hiu_setb(COMBO_DPHY_VID_PLL2_DIV, 1, 16, 2);
	lcd_hiu_setb(COMBO_DPHY_VID_PLL2_DIV, 1, 15, 1);
	lcd_hiu_setb(COMBO_DPHY_VID_PLL2_DIV, 0x3c78, 0, 15);
	lcd_hiu_setb(COMBO_DPHY_VID_PLL2_DIV, 0, 15, 1);
	data32 = readl(COMBO_DPHY_VID_PLL2_DIV);

	data32 |= (1 << 19);
	writel(data32, COMBO_DPHY_VID_PLL2_DIV);

	//3.config vclk
	writel(0x00000000, CLKCTRL_VIID_CLK2_CTRL);
	udelay(5);
	writel(0x00000000, CLKCTRL_VID_PLL_CLK2_DIV);
	writel(0x00010000, CLKCTRL_VID_PLL_CLK2_DIV);
	writel(0x00018000, CLKCTRL_VID_PLL_CLK2_DIV);
	writel(0x0001bc78, CLKCTRL_VID_PLL_CLK2_DIV);
	writel(0x00013c78, CLKCTRL_VID_PLL_CLK2_DIV);
	writel(0x00093c78, CLKCTRL_VID_PLL_CLK2_DIV);

	writel(0x00000000, CLKCTRL_VIID_CLK2_DIV);
	udelay(5);
	writel(0x00080000, CLKCTRL_VIID_CLK2_CTRL);
	udelay(5);
	writel(0x00008000, CLKCTRL_VIID_CLK2_DIV);
	writel(0x00018000, CLKCTRL_VIID_CLK2_DIV);
	udelay(5);
	writel(0x00080001, CLKCTRL_VIID_CLK2_CTRL);
	writel(0x00088001, CLKCTRL_VIID_CLK2_CTRL);
	udelay(10);
	writel(0x00080001, CLKCTRL_VIID_CLK2_CTRL);
	udelay(5);
	writel(0x00000008, CLKCTRL_VID_CLK2_CTRL2);

	//4. config phy clk
	//done by edp init

	//5. reset phy
	data32 = readl(RESETCTRL_RESET1_MASK);
	data32 &= ~((0x1 << 20) | (0x1 << 19) | (0x1 << 7));
	writel(data32, RESETCTRL_RESET1_MASK);
	data32 = readl(RESETCTRL_RESET1_LEVEL);
	data32 &= ~((0x1 << 20) | (0x1 << 19) | (0x1 << 7));
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(1);
	data32 |= (0x1 << 20) | (0x1 << 19) | (0x1 << 7);
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(10);
	writel(0x10, COMBO_DPHY_CNTL0);
	writel(0xaaaaaaaa, COMBO_DPHY_CNTL1);

	//6. config phy
	writel(0xc6770038, ANACTRL_DIF_PHY_CNTL1);
	writel(0x0100ffff, ANACTRL_DIF_PHY_CNTL2);
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL3);
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL4);
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL5);
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL6);
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL7);//ch5
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL8);//ch6
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL9);//ch7
	writel(0x16430028, ANACTRL_DIF_PHY_CNTL10);//ch8
	writel(0x0100ffff, ANACTRL_DIF_PHY_CNTL11);//ch8 AUX
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL12);//ch9
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL13);//ch10
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL14);//ch11
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL15);//ch12
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL16);//ch13
	writel(0x16530028, ANACTRL_DIF_PHY_CNTL17);//ch14
	writel(0x00406253, ANACTRL_DIF_PHY_CNTL19);//COMMON
	writel(0x0, ANACTRL_DIF_PHY_CNTL20);
	writel(0x0, ANACTRL_DIF_PHY_CNTL21);

	LCDPR(" lcd init pre\n");
}

static void lvds_init(void)
{
	/* set fifo_clk_sel: div 7 */
	lcd_combo_write(COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL0, (1 << 5));
	/* set cntl_ser_en:  8-channel to 1 */
	lcd_combo_setb(COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL0, 0x3ff, 16, 10);

	/* decoupling fifo enable, gated clock enable */
	lcd_combo_write(COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1,
			(1 << 6) | (1 << 0));
	/* decoupling fifo write enable after fifo enable */
	lcd_combo_setb(COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1, 1, 7, 1);

	lcd_vcbus_write(LVDS_SER_EN + (0x600 << 2), 0xfff );
	lcd_vcbus_write(LVDS_PACK_CNTL_ADDR + (0x600 << 2),
			(1 << 0) | // repack //[1:0]
			(0 << 3) |		// reserve
			(0 << 4) |		// lsb first
			(0 << 5) |	// pn swap
			(1 << 6) |	// dual port
			(0 << 7) |		// use tcon control
			(1 << 8) |	// 0:10bits, 1:8bits, 2:6bits, 3:4bits.
			(0 << 10) |		//r_select  //0:R, 1:G, 2:B, 3:0
			(1 << 12) |		//g_select  //0:R, 1:G, 2:B, 3:0
			(2 << 14));		//b_select  //0:R, 1:G, 2:B, 3:0;

	lcd_vcbus_write(LVDS_GEN_CNTL + (0x600 << 2),
		(lcd_vcbus_read(LVDS_GEN_CNTL + (0x600 << 2)) | (1 << 4) | (0x3 << 0)));
	lcd_vcbus_setb(LVDS_GEN_CNTL + (0x600 << 2), 1, 3, 1);

	lcd_vcbus_write(P2P_CH_SWAP0+(0x600<<2) , 0x87643210);//ch5 data4 none for LVDS
	lcd_vcbus_write(P2P_CH_SWAP1+(0x600<<2) , 0x000000a9);//chb data4 none for LVDS
	LCDPR("CH swap: 0x%x=0x%x, 0x%x=0x%x\n",
		P2P_CH_SWAP0+(0x600<<2), lcd_vcbus_read(P2P_CH_SWAP0+(0x600<<2)),
		P2P_CH_SWAP1+(0x600<<2), lcd_vcbus_read(P2P_CH_SWAP1+(0x600<<2)));
	lcd_vcbus_write(P2P_BIT_REV+(0x600<<2) , 2);
	LCDPR("bit rev: 0x%x=0x%x\n",
		P2P_BIT_REV+(0x600<<2), lcd_vcbus_read(P2P_BIT_REV+(0x600<<2)));

	LCDPR(" lvds init finish\n");
}

static void lcd_venc_set_lvds(void)
{
	unsigned int hactive = 1920;
	unsigned int vactive = 1080;
	unsigned int htotal = 2200;
	unsigned int vtotal = 1120;
	unsigned int hsw = 44;
	unsigned int hbp = 148;
	unsigned int vsw = 5;
	unsigned int vbp = 30;
	//unsigned int pclk = 147840000;
	unsigned int data32;

	writel(0, ENCL_VIDEO_EN + (0x800 << 2));

	writel(0x8000, ENCL_VIDEO_MODE + (0x800 << 2));/*bit[15] shadown en*/
	writel(0x0418, ENCL_VIDEO_MODE_ADV + (0x800 << 2)); /* Sampling rate: 1 */

	writel(0x1000, ENCL_VIDEO_FILT_CTRL + (0x800 << 2)); /* bypass filter */
	writel(htotal - 1, ENCL_VIDEO_MAX_PXCNT + (0x800 << 2));
	writel(vtotal - 1, ENCL_VIDEO_MAX_LNCNT + (0x800 << 2));
	writel(hsw + hbp, ENCL_VIDEO_HAVON_BEGIN + (0x800 << 2));
	writel(hactive - 1 + hsw + hbp, ENCL_VIDEO_HAVON_END + (0x800 << 2));
	writel(vsw + vbp, ENCL_VIDEO_VAVON_BLINE + (0x800 << 2));
	writel(vactive - 1 + vsw + vbp, ENCL_VIDEO_VAVON_ELINE + (0x800 << 2));

	writel(0, ENCL_VIDEO_HSO_BEGIN + (0x800 << 2));
	writel(hsw, ENCL_VIDEO_HSO_END + (0x800 << 2));
	writel(0, ENCL_VIDEO_VSO_BEGIN + (0x800 << 2));
	writel(0, ENCL_VIDEO_VSO_END + (0x800 << 2));
	writel(0, ENCL_VIDEO_VSO_BLINE + (0x800 << 2));
	writel(vsw, ENCL_VIDEO_VSO_ELINE + (0x800 << 2));
	writel(3, ENCL_VIDEO_RGBIN_CTRL + (0x800 << 2)); //yuv: 1, rgb: 3

	writel((5 << 13) | (hactive - 1), ENCL_INBUF_CNTL1 + (0x800 << 2));
	writel(0x200, ENCL_INBUF_CNTL0 + (0x800 << 2));

	/* default colorbar pattern */
	writel(1, ENCL_TST_MDSEL + (0x800 << 2));
	writel(0x200, ENCL_TST_Y + (0x800 << 2));
	writel(0x200, ENCL_TST_CB + (0x800 << 2));
	writel(0x200, ENCL_TST_CR + (0x800 << 2));
	writel(hsw + hbp, ENCL_TST_CLRBAR_STRT + (0x800 << 2));
	writel(240, ENCL_TST_CLRBAR_WIDTH + (0x800 << 2));
	writel(0, ENCL_TST_EN + (0x800 << 2));

	writel(1, ENCL_VIDEO_EN + (0x800 << 2));

	//select venc to lvds
	data32 = (1 << 31) | (0 << 30) | (0 << 29) | (0 << 28);
	writel(data32, VPU_DISP_VIU2_CTRL);

	//config venc_tcon
	writel(0x0, LCD_RGB_BASE_ADDR + (0x200 << 2));
	writel(0x400, LCD_RGB_COEFF_ADDR + (0x200 << 2));
	writel((1 << 0), LCD_POL_CNTL_ADDR + (0x200 << 2));

	/* DE signal */
	writel(hsw + hbp, DE_HS_ADDR + (0x600 << 2));
	writel(hsw + hbp + hactive, DE_HE_ADDR + (0x600 << 2));
	writel(vsw + vbp, DE_VS_ADDR + (0x600 << 2));
	writel(vsw + vbp + vactive - 1, DE_VE_ADDR + (0x600 << 2));

	/* Hsync signal */
	writel(0, HSYNC_HS_ADDR + (0x600 << 2));
	writel(hsw, HSYNC_HE_ADDR + (0x600 << 2));
	writel(0, HSYNC_VS_ADDR + (0x600 << 2));
	writel(vtotal - 1, HSYNC_VE_ADDR + (0x600 << 2));

	/* Vsync signal */
	writel(0, VSYNC_HS_ADDR + (0x600 << 2));
	writel(0, VSYNC_HE_ADDR + (0x600 << 2));
	writel(0, VSYNC_VS_ADDR + (0x600 << 2));
	writel(vsw, VSYNC_VE_ADDR + (0x600 << 2));

	//select encl
	writel(2, VPU_VENC_CTRL + (0x800 << 2));

	LCDPR(" lcd venc init\n");
}

void lcd_display_init_test(void)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();

	if (lcd_drv->lcd_config->lcd_basic.lcd_type == LCD_LVDS) {
		writel(0x6, VPU_VIU_VENC_MUX_CTRL);
		lcd_init_pre_lvds();
		lcd_venc_set_lvds();
		mdelay(100);
		lvds_init();
		LCDPR("lvds init test done\n");
	} else if (lcd_drv->lcd_config->lcd_basic.lcd_type == LCD_EDP) {
		writel(0x24, VPU_VIU_VENC_MUX_CTRL);
		lcd_init_pre_edp();
		lcd_venc_set_edp();
		mdelay(100);
		edp_power_init();
		edp_tx_init();
		LCDPR("edp init test done\n");
	}
}

void lcd_display_init_reg_dump(void)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();

	if (lcd_drv->lcd_config->lcd_basic.lcd_type == LCD_LVDS) {
		printf("pll regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ANACTRL_TCON_PLL2_CNTL0,
			readl(ANACTRL_TCON_PLL2_CNTL0),
			ANACTRL_TCON_PLL2_CNTL1,
			readl(ANACTRL_TCON_PLL2_CNTL1),
			ANACTRL_TCON_PLL2_CNTL2,
			readl(ANACTRL_TCON_PLL2_CNTL2),
			ANACTRL_TCON_PLL2_CNTL3,
			readl(ANACTRL_TCON_PLL2_CNTL3),
			ANACTRL_TCON_PLL2_CNTL4,
			readl(ANACTRL_TCON_PLL2_CNTL4),
			ANACTRL_TCON_PLL2_STS,
			readl(ANACTRL_TCON_PLL2_STS));

		printf("clk regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			COMBO_DPHY_VID_PLL2_DIV,
			readl(COMBO_DPHY_VID_PLL2_DIV),
			CLKCTRL_VIID_CLK2_DIV,
			readl(CLKCTRL_VIID_CLK2_DIV),
			CLKCTRL_VIID_CLK2_CTRL,
			readl(CLKCTRL_VIID_CLK2_CTRL),
			CLKCTRL_VID_CLK2_CTRL2,
			readl(CLKCTRL_VID_CLK2_CTRL2));

		printf("phy regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ANACTRL_DIF_PHY_CNTL1,
			readl(ANACTRL_DIF_PHY_CNTL1),
			ANACTRL_DIF_PHY_CNTL2,
			readl(ANACTRL_DIF_PHY_CNTL2),
			ANACTRL_DIF_PHY_CNTL3,
			readl(ANACTRL_DIF_PHY_CNTL3),
			ANACTRL_DIF_PHY_CNTL4,
			readl(ANACTRL_DIF_PHY_CNTL4),
			ANACTRL_DIF_PHY_CNTL5,
			readl(ANACTRL_DIF_PHY_CNTL5),
			ANACTRL_DIF_PHY_CNTL6,
			readl(ANACTRL_DIF_PHY_CNTL6),
			ANACTRL_DIF_PHY_CNTL7,
			readl(ANACTRL_DIF_PHY_CNTL7),
			ANACTRL_DIF_PHY_CNTL8,
			readl(ANACTRL_DIF_PHY_CNTL8),
			ANACTRL_DIF_PHY_CNTL9,
			readl(ANACTRL_DIF_PHY_CNTL9),
			ANACTRL_DIF_PHY_CNTL10,
			readl(ANACTRL_DIF_PHY_CNTL10),
			ANACTRL_DIF_PHY_CNTL11,
			readl(ANACTRL_DIF_PHY_CNTL11),
			ANACTRL_DIF_PHY_CNTL12,
			readl(ANACTRL_DIF_PHY_CNTL12),
			ANACTRL_DIF_PHY_CNTL13,
			readl(ANACTRL_DIF_PHY_CNTL13),
			ANACTRL_DIF_PHY_CNTL14,
			readl(ANACTRL_DIF_PHY_CNTL14),
			ANACTRL_DIF_PHY_CNTL15,
			readl(ANACTRL_DIF_PHY_CNTL15),
			ANACTRL_DIF_PHY_CNTL16,
			readl(ANACTRL_DIF_PHY_CNTL16),
			ANACTRL_DIF_PHY_CNTL17,
			readl(ANACTRL_DIF_PHY_CNTL17),
			ANACTRL_DIF_PHY_CNTL18,
			readl(ANACTRL_DIF_PHY_CNTL18),
			ANACTRL_DIF_PHY_CNTL19,
			readl(ANACTRL_DIF_PHY_CNTL19),
			ANACTRL_DIF_PHY_CNTL20,
			readl(ANACTRL_DIF_PHY_CNTL20),
			ANACTRL_DIF_PHY_CNTL21,
			readl(ANACTRL_DIF_PHY_CNTL21));

		printf("lvds regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL0,
			readl(COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL0),
			COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1,
			readl(COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1),
			LVDS_SER_EN + (0x600 << 2),
			readl(LVDS_SER_EN + (0x600 << 2)),
			LVDS_PACK_CNTL_ADDR + (0x600 << 2),
			readl(LVDS_PACK_CNTL_ADDR + (0x600 << 2)),
			LVDS_GEN_CNTL + (0x600 << 2),
			readl(LVDS_GEN_CNTL + (0x600 << 2)),
			P2P_CH_SWAP0 + (0x600 << 2),
			readl(P2P_CH_SWAP0 + (0x600 << 2)),
			P2P_CH_SWAP1 + (0x600 << 2),
			readl(P2P_CH_SWAP1 + (0x600 << 2)),
			P2P_BIT_REV + (0x600 << 2),
			readl(P2P_BIT_REV + (0x600 << 2)));

		printf("venc regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ENCL_VIDEO_EN + (0x800 << 2),
			readl(ENCL_VIDEO_EN + (0x800 << 2)),
			ENCL_VIDEO_MODE + (0x800 << 2),
			readl(ENCL_VIDEO_MODE + (0x800 << 2)),
			ENCL_VIDEO_MODE_ADV + (0x800 << 2),
			readl(ENCL_VIDEO_MODE_ADV + (0x800 << 2)),
			ENCL_VIDEO_FILT_CTRL + (0x800 << 2),
			readl(ENCL_VIDEO_FILT_CTRL + (0x800 << 2)),
			ENCL_VIDEO_MAX_PXCNT + (0x800 << 2),
			readl(ENCL_VIDEO_MAX_PXCNT + (0x800 << 2)),
			ENCL_VIDEO_MAX_LNCNT + (0x800 << 2),
			readl(ENCL_VIDEO_MAX_LNCNT + (0x800 << 2)),
			ENCL_VIDEO_HAVON_BEGIN + (0x800 << 2),
			readl(ENCL_VIDEO_HAVON_BEGIN + (0x800 << 2)),
			ENCL_VIDEO_HAVON_END + (0x800 << 2),
			readl(ENCL_VIDEO_HAVON_END + (0x800 << 2)),
			ENCL_VIDEO_VAVON_BLINE + (0x800 << 2),
			readl(ENCL_VIDEO_VAVON_BLINE + (0x800 << 2)),
			ENCL_VIDEO_VAVON_ELINE + (0x800 << 2),
			readl(ENCL_VIDEO_VAVON_ELINE + (0x800 << 2)),
			ENCL_VIDEO_HSO_BEGIN + (0x800 << 2),
			readl(ENCL_VIDEO_HSO_BEGIN + (0x800 << 2)),
			ENCL_VIDEO_HSO_END + (0x800 << 2),
			readl(ENCL_VIDEO_HSO_END + (0x800 << 2)),
			ENCL_VIDEO_VSO_BEGIN + (0x800 << 2),
			readl(ENCL_VIDEO_VSO_BEGIN + (0x800 << 2)),
			ENCL_VIDEO_VSO_END + (0x800 << 2),
			readl(ENCL_VIDEO_VSO_END + (0x800 << 2)),
			ENCL_VIDEO_VSO_BLINE + (0x800 << 2),
			readl(ENCL_VIDEO_VSO_BLINE + (0x800 << 2)),
			ENCL_VIDEO_VSO_ELINE + (0x800 << 2),
			readl(ENCL_VIDEO_VSO_ELINE + (0x800 << 2)),
			ENCL_VIDEO_RGBIN_CTRL + (0x800 << 2),
			readl(ENCL_VIDEO_RGBIN_CTRL + (0x800 << 2)),
			ENCL_INBUF_CNTL1 + (0x800 << 2),
			readl(ENCL_INBUF_CNTL1 + (0x800 << 2)),
			ENCL_INBUF_CNTL0 + (0x800 << 2),
			readl(ENCL_INBUF_CNTL0 + (0x800 << 2)),
			VPU_DISP_VIU2_CTRL,
			readl(VPU_DISP_VIU2_CTRL),
			VPU_VENC_CTRL + (0x800 << 2),
			readl(VPU_VENC_CTRL + (0x800 << 2)));
	} else if (lcd_drv->lcd_config->lcd_basic.lcd_type == LCD_EDP) {
		printf("pll regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ANACTRL_TCON_PLL0_CNTL0,
			readl(ANACTRL_TCON_PLL0_CNTL0),
			ANACTRL_TCON_PLL0_CNTL1,
			readl(ANACTRL_TCON_PLL0_CNTL1),
			ANACTRL_TCON_PLL0_CNTL2,
			readl(ANACTRL_TCON_PLL0_CNTL2),
			ANACTRL_TCON_PLL0_CNTL3,
			readl(ANACTRL_TCON_PLL0_CNTL3),
			ANACTRL_TCON_PLL0_CNTL4,
			readl(ANACTRL_TCON_PLL0_CNTL4),
			ANACTRL_TCON_PLL0_STS,
			readl(ANACTRL_TCON_PLL0_STS));

		printf("clk regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			COMBO_DPHY_EDP_PIXEL_CLK_DIV,
			readl(COMBO_DPHY_EDP_PIXEL_CLK_DIV),
			COMBO_DPHY_VID_PLL0_DIV,
			readl(COMBO_DPHY_VID_PLL0_DIV),
			CLKCTRL_VIID_CLK0_DIV,
			readl(CLKCTRL_VIID_CLK0_DIV),
			CLKCTRL_VIID_CLK0_CTRL,
			readl(CLKCTRL_VIID_CLK0_CTRL),
			CLKCTRL_VID_CLK0_CTRL2,
			readl(CLKCTRL_VID_CLK0_CTRL2));

		printf("phy regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ANACTRL_DIF_PHY_CNTL1,
			readl(ANACTRL_DIF_PHY_CNTL1),
			ANACTRL_DIF_PHY_CNTL2,
			readl(ANACTRL_DIF_PHY_CNTL2),
			ANACTRL_DIF_PHY_CNTL3,
			readl(ANACTRL_DIF_PHY_CNTL3),
			ANACTRL_DIF_PHY_CNTL4,
			readl(ANACTRL_DIF_PHY_CNTL4),
			ANACTRL_DIF_PHY_CNTL5,
			readl(ANACTRL_DIF_PHY_CNTL5),
			ANACTRL_DIF_PHY_CNTL6,
			readl(ANACTRL_DIF_PHY_CNTL6),
			ANACTRL_DIF_PHY_CNTL7,
			readl(ANACTRL_DIF_PHY_CNTL7),
			ANACTRL_DIF_PHY_CNTL8,
			readl(ANACTRL_DIF_PHY_CNTL8),
			ANACTRL_DIF_PHY_CNTL9,
			readl(ANACTRL_DIF_PHY_CNTL9),
			ANACTRL_DIF_PHY_CNTL10,
			readl(ANACTRL_DIF_PHY_CNTL10),
			ANACTRL_DIF_PHY_CNTL11,
			readl(ANACTRL_DIF_PHY_CNTL11),
			ANACTRL_DIF_PHY_CNTL12,
			readl(ANACTRL_DIF_PHY_CNTL12),
			ANACTRL_DIF_PHY_CNTL13,
			readl(ANACTRL_DIF_PHY_CNTL13),
			ANACTRL_DIF_PHY_CNTL14,
			readl(ANACTRL_DIF_PHY_CNTL14),
			ANACTRL_DIF_PHY_CNTL15,
			readl(ANACTRL_DIF_PHY_CNTL15),
			ANACTRL_DIF_PHY_CNTL16,
			readl(ANACTRL_DIF_PHY_CNTL16),
			ANACTRL_DIF_PHY_CNTL17,
			readl(ANACTRL_DIF_PHY_CNTL17),
			ANACTRL_DIF_PHY_CNTL18,
			readl(ANACTRL_DIF_PHY_CNTL18),
			ANACTRL_DIF_PHY_CNTL19,
			readl(ANACTRL_DIF_PHY_CNTL19),
			ANACTRL_DIF_PHY_CNTL20,
			readl(ANACTRL_DIF_PHY_CNTL20),
			ANACTRL_DIF_PHY_CNTL21,
			readl(ANACTRL_DIF_PHY_CNTL21));

		printf("edp regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n",
			COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0,
			readl(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0),
			COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1,
			readl(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1),
			EDP_TX_LINK_BW_SET,
			dptx_reg_read(EDP_TX_LINK_BW_SET),
			EDP_TX_LINK_COUNT_SET,
			dptx_reg_read(EDP_TX_LINK_COUNT_SET),
			EDP_TX_TRAINING_PATTERN_SET,
			dptx_reg_read(EDP_TX_TRAINING_PATTERN_SET),
			EDP_TX_SCRAMBLING_DISABLE,
			dptx_reg_read(EDP_TX_SCRAMBLING_DISABLE),
			EDP_TX_TRANSMITTER_OUTPUT_ENABLE,
			dptx_reg_read(EDP_TX_TRANSMITTER_OUTPUT_ENABLE),
			EDP_TX_MAIN_STREAM_ENABLE,
			dptx_reg_read(EDP_TX_MAIN_STREAM_ENABLE),
			EDP_TX_PHY_RESET,
			dptx_reg_read(EDP_TX_PHY_RESET),
			EDP_TX_PHY_STATUS,
			dptx_reg_read(EDP_TX_PHY_STATUS),
			EDP_TX_AUX_COMMAND,
			dptx_reg_read(EDP_TX_AUX_COMMAND),
			EDP_TX_AUX_ADDRESS,
			dptx_reg_read(EDP_TX_AUX_ADDRESS),
			EDP_TX_AUX_STATE,
			dptx_reg_read(EDP_TX_AUX_STATE),
			EDP_TX_AUX_REPLY_CODE,
			dptx_reg_read(EDP_TX_AUX_REPLY_CODE),
			EDP_TX_AUX_REPLY_COUNT,
			dptx_reg_read(EDP_TX_AUX_REPLY_COUNT),
			EDP_TX_AUX_REPLY_DATA_COUNT,
			dptx_reg_read(EDP_TX_AUX_REPLY_DATA_COUNT),
			EDP_TX_AUX_TRANSFER_STATUS,
			dptx_reg_read(EDP_TX_AUX_TRANSFER_STATUS));

		printf("venc regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ENCL_VIDEO_EN,
			readl(ENCL_VIDEO_EN),
			ENCL_VIDEO_MODE,
			readl(ENCL_VIDEO_MODE),
			ENCL_VIDEO_MODE_ADV,
			readl(ENCL_VIDEO_MODE_ADV),
			ENCL_VIDEO_FILT_CTRL,
			readl(ENCL_VIDEO_FILT_CTRL),
			ENCL_VIDEO_MAX_PXCNT,
			readl(ENCL_VIDEO_MAX_PXCNT),
			ENCL_VIDEO_MAX_LNCNT,
			readl(ENCL_VIDEO_MAX_LNCNT),
			ENCL_VIDEO_HAVON_BEGIN,
			readl(ENCL_VIDEO_HAVON_BEGIN),
			ENCL_VIDEO_HAVON_END,
			readl(ENCL_VIDEO_HAVON_END),
			ENCL_VIDEO_VAVON_BLINE,
			readl(ENCL_VIDEO_VAVON_BLINE),
			ENCL_VIDEO_VAVON_ELINE,
			readl(ENCL_VIDEO_VAVON_ELINE),
			ENCL_VIDEO_HSO_BEGIN,
			readl(ENCL_VIDEO_HSO_BEGIN),
			ENCL_VIDEO_HSO_END,
			readl(ENCL_VIDEO_HSO_END),
			ENCL_VIDEO_VSO_BEGIN,
			readl(ENCL_VIDEO_VSO_BEGIN),
			ENCL_VIDEO_VSO_END,
			readl(ENCL_VIDEO_VSO_END),
			ENCL_VIDEO_VSO_BLINE,
			readl(ENCL_VIDEO_VSO_BLINE),
			ENCL_VIDEO_VSO_ELINE,
			readl(ENCL_VIDEO_VSO_ELINE),
			ENCL_VIDEO_RGBIN_CTRL,
			readl(ENCL_VIDEO_RGBIN_CTRL),
			ENCL_INBUF_CNTL1,
			readl(ENCL_INBUF_CNTL1),
			ENCL_INBUF_CNTL0,
			readl(ENCL_INBUF_CNTL0),
			VPU_DISP_VIU2_CTRL,
			readl(VPU_DISP_VIU2_CTRL),
			VPU_VENC_CTRL,
			readl(VPU_VENC_CTRL));
	}
	printf("0x%08x: 0x%08x\n",
		VPU_VIU_VENC_MUX_CTRL,
		readl(VPU_VIU_VENC_MUX_CTRL));
}

void lcd_edp_debug(void)
{
	dptx_dpcd_dump();
}

