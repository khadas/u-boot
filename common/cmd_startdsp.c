/*
 * common/cmd_reboot.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <common.h>
#include <command.h>
#include <asm/arch/cpu.h>
#include <asm/arch-tm2/secure_apb.h>
#include <asm/arch-tm2/timer.h>
#include <asm/arch-tm2/mailbox.h>

#include <serial.h>


#define writel32(val, reg)		(*((volatile uint32_t *)((uintptr_t)(reg)))) =  (val)
#define readl32(reg)			(*((volatile uint32_t *)((uintptr_t)(reg))))


/*
 * clk_util_set_dsp_clk
 * freq_sel: 0:286MHz  fclk_7
 *           1:400MHz  fclk_5
 *           2:500MHz  fclk_2/2
 *           4:333MHz  fclk_3/2
 *           5:250Mhz  fclk_2/4
 *           6:200Mhz  fclk_5/2
 *           7:100Mhz  fclk_5/4
 *           8:24Mhz   oscin
 *           others:286MHz fclk/7
 */

// --------------------------------------------------
//              clocks_set_cpu_core_clk
// --------------------------------------------------
void clk_util_set_dsp_clk(uint32_t id, uint32_t freq_sel)
{
	uint32_t    control;
	uint32_t    clk_sel;
	uint32_t    clk_div;
	uint32_t    addr;
	addr = HHI_DSP_CLK_CNTL;

	// Make sure not busy from last setting and we currently match the last setting

	control = readl32(addr);

	switch (freq_sel)
	{
		case 1  : clk_sel = 2; clk_div =0; printf("CLK_UTIL:dsp:fclk/5:400MHz\n" ); break;
		case 2  : clk_sel = 0; clk_div =1; printf("CLK_UTIL:dsp:fclk/4:500MHz\n"); break;
		//case 3  : clk_sel = 1; clk_div =0; printf("CLK_UTIL:dsp:fclk/3:667MHz\n"); break;
		case 4  : clk_sel = 1; clk_div =1; printf("CLK_UTIL:dsp:fclk/3/2:333MHz\n" ); break;
		case 5  : clk_sel = 0; clk_div =3; printf("CLK_UTIL:dsp:fclk/2:250MHz\n"); break;
		case 6  : clk_sel = 2; clk_div =1; printf("CLK_UTIL:dsp:fclk/4/2:200MHz\n"); break;
		case 7  : clk_sel = 2; clk_div =3; printf("CLK_UTIL:dsp:fclk/4/4:100MHz\n"); break;
		case 8  : clk_sel = 4; clk_div =0; printf("CLK_UTIL:dsp:oscin:24MHz\n"); break;
		//case 10 : clk_sel = 0; clk_div =0; printf ("CLK_UTIL:dsp:fclk/2:1000MHz\n"); break;
		default : clk_sel = 3; clk_div =0; printf("CLK_UTIL:dsp:fclk/7:286MHz\n"); break;
	}

	switch ( id ) {
		case 0:
			if (control & (1 << 15)) {  //if sync_mux ==1, sel mux 0
				control = (control & ~( ( 1<<15) | (0xf<<0) | (0x7 <<4) ) ) | (1<<7) | (clk_div<<0) | (clk_sel<<4);
			} else {
				control = (control & ~( ( 1<<15) | (0xf<<8) | (0x7 <<12) ) ) | (1<<7) | (clk_div<<8) | (clk_sel<<12) | (1<<15);
			}
			break;
		default :
			if (control & (1 << 31)) {  //if sync_mux ==1, sel mux 0
				control = (control & ~( ( 1<<31) | (0xf<<16) | (0x7 <<20) ) ) | (1<<23) | (clk_div<<16) | (clk_sel<<20);
			} else {
				control = (control & ~( ( 1<<31) | (0xf<<24) | (0x7 <<28) ) ) | (1<<23) | (clk_div<<24) | (clk_sel<<28) | (1<<31);
			}
		break;
	}

	writel32(control,addr);
}

void start_dsp(uint32_t id,uint32_t reset_addr)
{
	uint32_t StatVectorSel;
	uint32_t strobe = 1;
	//address remap
	printf("\n start_dsp \n");
	//*P_DSP_REMAP2 = 0x3000fff0; //replace the highest 12bits 0xfffxxxxx with 0x300xxxxx

	StatVectorSel = (reset_addr!= 0xfffa0000);

	//open the Dreset, Breset, irq_clken
	if (id == 0) { //dspa
		init_dsp(id,reset_addr, (0x1 | StatVectorSel<<1 | strobe<<2),(6 << 12)|(1<<6),(1<<14)|(1<<15)|(1<<1)|(1<<3)|(1<<2)|(1<<0)|(1<<31)|(1<<7));
		//init_dsp(id,reset_addr, (0x1 | StatVectorSel<<1 | strobe<<2),(6 << 12)|(1<<6),(1<<14)|(1<<15)|(1<<1)|(1<<3)|(1<<2)|(1<<0)|(1<<31)|(1<<6));
		//init_dsp(id,reset_addr, (0x1 | StatVectorSel<<1 | strobe<<2),0,0);
		printf("dspa jtag enalble \n");
		_udelay(50);
		//init_dsp(id,reset_addr, (0x1 |  StatVectorSel<<1 | strobe<<2),(6 << 12),);
		printf("\n *P_DSP_CFG0 : ADDR_0X%p, value_0x%8x \n",P_DSP_CFG0,*P_DSP_CFG0);
		// *P_DSP_CFG0 = (*P_DSP_CFG0 & ~(0xffff <<0)) | (1<<29) | (0<<0) ;      //irq_clken
		*P_DSP_CFG0 = (*P_DSP_CFG0 & ~(0xffff <<0)) | (0x2018 << 0) | (1<<29) | (0<<0) ;      //irq_clken
		_udelay(10);
		*P_DSP_CFG0 = *P_DSP_CFG0 & ~(1<<31);     //Dreset assert
		_udelay(10);
		// *P_DSP_CFG0 = *P_DSP_CFG0 | (1<<31);     //Dreset deassert
		// _udelay(10);
		*P_DSP_CFG0 = *P_DSP_CFG0 & ~(1<<30);    //Breset
		_udelay(10);
		// *P_DSP_CFG0 = *P_DSP_CFG0 | (1<<30);    //Breset deassert
		// _udelay(10);
		printf("\n *P_DSP_CFG0 : ADDR_0X%p, value_0x%8x \n",P_DSP_CFG0,*P_DSP_CFG0);
	} else {
		init_dsp(id,reset_addr, (0x1 | StatVectorSel<<1 | strobe<<2),(7 << 12)|(1<<7),(1<<14)|(1<<15)|(1<<1)|(1<<3)|(1<<2)|(1<<0)|(1<<7)|(1<<31));
		printf("dspb jtag enalble \n");
		_udelay(500);
		printf("\n *P_DSPB_CFG0 : ADDR_0X%p, value_0x%8x \n",P_DSPB_CFG0,*P_DSPB_CFG0);
		// *P_DSPB_CFG0 = (*P_DSPB_CFG0 & ~((1<<31) | (1<<30) | (0xffff <<0)) ) | (1<<29) | (1<<0); //15:0 prid
		// *P_DSPB_CFG0 = (*P_DSPB_CFG0 & ~(0xffff <<0)) | (1<<29) | (0<<0) ;      //irq_clken
		*P_DSPB_CFG0 = (*P_DSPB_CFG0 & ~(0xffff <<0)) | (0x2019 << 0) | (1<<29) | (0<<0) ;      //irq_clken
		_udelay(10);
		*P_DSPB_CFG0 = *P_DSPB_CFG0 & ~(1<<31);     //Dreset
		_udelay(10);
		*P_DSPB_CFG0 = *P_DSPB_CFG0 & ~(1<<30);    //Breset
		_udelay(10);
		printf("\n *P_DSPB_CFG0 : ADDR_0X%p, value_0x%8x \n",P_DSPB_CFG0,*P_DSPB_CFG0);
	}
}

//#define P_RESET0_LEVEL                             (volatile uint32_t *)0xffd01080
//#define P_RESET1_LEVEL                             (volatile uint32_t *)0xffd01084
//#define P_RESET2_LEVEL                             (volatile uint32_t *)0xffd01088
//#define P_RESET3_LEVEL                             (volatile uint32_t *)0xffd0108c
//#define P_RESET4_LEVEL                             (volatile uint32_t *)0xffd01090
//#define P_RESET5_LEVEL                             (volatile uint32_t *)0xffd01094
//#define P_RESET6_LEVEL                             (volatile uint32_t *)0xffd01098
//#define P_RESET7_LEVEL                             (volatile uint32_t *)0xffd0109c

void power_switch_to_dsp_a(int64_t pwr_cntl)
{
	if (pwr_cntl == 1) {
		printf("[PWR]: Power on DSP A\n");
		// Powerup dsp a
		writel32(readl32(P_AO_RTI_GEN_PWR_SLEEP0) & (~(0x1<<21)),P_AO_RTI_GEN_PWR_SLEEP0); // power on
		_udelay(5);
		// Power up memory
		*P_HHI_DSP_MEM_PD_REG0 &= ~(0xffff<<0);
		_udelay(5);
		// reset
		*P_RESET4_LEVEL &= ~(0x1<<0);
		*P_RESET1_LEVEL &= ~(0x1<<20);
		_udelay(5);
		// remove isolation
		writel32(readl32(P_AO_RTI_GEN_PWR_ISO0) & (~(0x1<<21)),P_AO_RTI_GEN_PWR_ISO0);
		// pull up reset
		*P_RESET4_LEVEL |= (0x1<<0);
		*P_RESET1_LEVEL |= (0x1<<20);
		_udelay(5);

	} else {
		// reset
		*P_RESET4_LEVEL &= ~(0x1<<0);
		*P_RESET1_LEVEL &= ~(0x1<<20);
		// add isolation
		writel32(readl32(P_AO_RTI_GEN_PWR_ISO0) | (0x1<<21),P_AO_RTI_GEN_PWR_ISO0 );
		_udelay(5);
		// power down memory
		*P_HHI_DSP_MEM_PD_REG0 |= (0xffff<<0);
		_udelay(5);
		// power down dsp a
		writel32(readl32(P_AO_RTI_GEN_PWR_SLEEP0) | (0x1<<21),P_AO_RTI_GEN_PWR_SLEEP0);
	}
}

void power_switch_to_dsp_b(int64_t pwr_cntl)
{
	if (pwr_cntl == 1) {
		printf("[PWR]: Power on DSP B\n");
		// Powerup dsp b
		writel32(readl32(P_AO_RTI_GEN_PWR_SLEEP0) & (~(0x1<<22)),P_AO_RTI_GEN_PWR_SLEEP0); // power on
		_udelay(5);
		// Power up memory

		*P_HHI_DSP_MEM_PD_REG0 &= ~(0xffff<<16);
		_udelay(5);
		// reset
		*P_RESET4_LEVEL &= ~(0x1<<1);
		*P_RESET1_LEVEL &= ~(0x1<<21);
		_udelay(5);

		// remove isolation
		writel32(readl32(P_AO_RTI_GEN_PWR_ISO0) & (~(0x1<<22)),P_AO_RTI_GEN_PWR_ISO0);
		// pull up reset
		*P_RESET4_LEVEL |= (0x1<<1);
		*P_RESET1_LEVEL |= (0x1<<21);
		_udelay(5);

	} else {
		// reset
		*P_RESET4_LEVEL &= ~(0x1<<1);
		*P_RESET1_LEVEL &= ~(0x1<<21);

		// add isolation
		writel32(readl32(P_AO_RTI_GEN_PWR_ISO0) | (0x1<<22),P_AO_RTI_GEN_PWR_ISO0 );
		_udelay(5);
		// power down memory
		*P_HHI_DSP_MEM_PD_REG0 |= (0xffff<<16);
		_udelay(5);

		// power down dsp a
		writel32(readl32(P_AO_RTI_GEN_PWR_SLEEP0) | (0x1<<22),P_AO_RTI_GEN_PWR_SLEEP0);
	}
}

void dsp_init(unsigned int dspid, uint32_t reset_addr, uint32_t freq_sel) {
	printf("the rest_addr passing : 0x%8x \n",reset_addr);
	clk_util_set_dsp_clk(dspid,freq_sel);
	_udelay(10);
	if (dspid == 0 )
		power_switch_to_dsp_a(1);	//start dsp0, at the reset address 0x30000000
	else
		power_switch_to_dsp_b(1);	//start dsp1, at the reset address 0x30800000
	start_dsp(dspid,reset_addr);	//set the frep to 500Mhz

	*P_AO_RTI_PULL_UP_EN_REG |= (0xf << 6 );
	*P_AO_RTI_PULL_UP_REG |= (0xf << 6 );
}

static int do_startdsp(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned long addr;
	unsigned int dspid;
	uint32_t freq_sel;
	int ret=0;
	if (argc <= 1) {
		printf("plese input dsp boot args:id, addrss, clk!\n");
		return CMD_RET_USAGE;
	}
	dspid = simple_strtoul(argv[1], NULL, 16);
	addr = simple_strtoul(argv[2], NULL, 16);
	freq_sel = simple_strtoul(argv[3], NULL, 16);
	printf("dsp%d boot \n",dspid);
	printf("dspboot start address:0x%lx\n",addr);
	printf("dsp clk num:%d\n",freq_sel);
	dsp_init(dspid, addr, freq_sel);
	printf("dsp init over! \n");
	return ret;
}


U_BOOT_CMD(
	startdsp,	4,	1,	do_startdsp,
	"load dspboot.bin from address",
	"\n arg[0]: cmd\n"
	"arg[1]: dspid \n"
	"arg[2]: dspboot.bin load address!\n"
	"arg[3]: dsp clk set 1:400M 2:500M 4:333M 5:250M 6:200M 7:100M 8 :24M"
);




