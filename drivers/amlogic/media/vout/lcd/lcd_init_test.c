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
//#define PHY_PRBS
//#define EDP_INIT_TEST
#define LVDS_INIT_TEST

#ifdef LVDS_INIT_TEST
static void lcd_init_pre_lvds(void)
{
	unsigned int data32;
	int cnt = 0, i;

	//1.config pll
set_pll_retry:
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
			goto set_pll_retry;
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
#endif

#if (defined EDP_INIT_TEST || defined LVDS_INIT_TEST)
static void lcd_venc_set(void)
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
	//writel(0x0410, ENCL_VIDEO_MODE_ADV + (0x800 << 2));

	writel(1, ENCL_VIDEO_EN + (0x800 << 2));
#ifdef LVDS_INIT_TEST
	//select venc to lvds
	data32 = (1 << 31) | (0 << 30) | (0 << 29) | (0 << 28);
	writel(data32, VPU_DISP_VIU2_CTRL);

#endif

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
#endif

void lcd_display_init_test(void)
{

#ifdef LVDS_INIT_TEST
	lcd_init_pre_lvds();
#endif
#if (defined EDP_INIT_TEST || defined LVDS_INIT_TEST)
	lcd_venc_set();
#endif
	run_command("gpio set GPIOH_1", 0);
	run_command("gpio set GPIOY_14", 0);
	mdelay(100);

#ifdef EDP_INIT_TEST
	edp_power_init();
	edp_tx_init();
#endif
#ifdef LVDS_INIT_TEST
	lvds_init();
#endif
	LCDPR("display init test done\n");
}
