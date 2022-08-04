#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/clock.h>
#include <asm/arch/oscring.h>

void ring_powerinit(void)
{
	writel(0x18000a, RING_PWM_VCCK);/*set vcck 0.8v*/
	writel(0x90009, RING_PWM_EE);/*set ee 0.8v*/
}

unsigned long clk_util_ring_msr(unsigned long clk_mux)
{
	unsigned int regval = 0;

	writel(0, MSR_CLK_REG0);
	/* Set the measurement gate to 64uS */
	/* 64uS is enough for measure the frequence? */
	setbits_le32(MSR_CLK_REG0, (10000 - 1));
	/* Disable continuous measurement */
	/* Disable interrupts */
	clrbits_le32(MSR_CLK_REG0, (1 << 17) | (1 << 18));
	clrbits_le32(MSR_CLK_REG0, 0x7f << 20);
	setbits_le32(MSR_CLK_REG0, (clk_mux) << 20 | (1 << 19) | (1 << 16));
	mdelay(10);
	/* Wait for the measurement to be done */
	regval = readl(MSR_CLK_REG0);
	do {
		regval = readl(MSR_CLK_REG0);
	} while (regval & (1 << 31));

	/* Disable measuring */
	clrbits_le32(MSR_CLK_REG0, 1 << 16);
	regval = (readl(MSR_CLK_REG2) + 31) & 0x000fffff;

	return (regval / 10);
}

int ring_msr(int index)
{
	const char* clk_table[] = {
			[17] = "dspa_osc_ring(SVT16) " ,
			[16] = "axi_sram_osc_ring(SVT16) " ,
			[15] = "sys_cpu_osc_ring3(LVT16) " ,
			[14] = "sys_cpu_osc_ring2(ULVT16) " ,
			[13] = "sys_cpu_osc_ring1(ULVT20) " ,
			[12] = "sys_cpu_osc_ring0(ULVT16) " ,
			[11] = "ddr_osc_ring(LVT16) " ,
			[10] = "dos_osc_ring3(ULVT20) " ,
			[9] = "dos_osc_ring2(LVT16) " ,
			[8] = "dos_osc_ring1(SVT16) " ,
			[7] = "dos_osc_ring0(SVT24) " ,
			[6] = "vpu_osc_ring2(LVT16) " ,
			[5] = "vpu_osc_ring1(LVT20) " ,
			[4] = "vpu_osc_ring0(SVT24) " ,
			[3] = "gpu_osc_ring2(SVT16) " ,
			[2] = "gpu_osc_ring1(ULVT16) " ,
			[1] = "gpu_osc_ring0(LVT16) " ,
			[0] = "dmc_osc_ring(LVT16) " ,
		};
	const int tb[] = {180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
			  190, 191, 192, 193, 194, 195, 196, 197};
	unsigned long i;
	uint8_t efuseinfo[4] = {0, 0, 0, 0};
	int multiple = 400;

	if ((index != 0xff) && (index != 0)) {
		if (bl31_get_cornerinfo(efuseinfo, sizeof(efuseinfo) / sizeof(uint8_t)) != 0) {
			printf("fail get efuse info\n");
			return 0;
		}
		if ((index >= 1) && (index <= 3))
			printf("%d\n", efuseinfo[index]);
		else
			printf("input data not support!\n");

		return 0;
	}
	if (0xff != index) {
		ring_powerinit();
		printf("set vcck vddee to 800mv\n");
	}
	/*RING_OSCILLATOR       0x7f: set slow ring*/
	writel(OSCRING_CTL_DATA0, OSCRING_CTL_REG0);
	writel(OSCRING_CTL_DATA1, OSCRING_CTL_REG1);

	for (i = 0; i < 18; i++) {
		printf("%s      :",clk_table[i]);
		printf("%ld     KHz",clk_util_ring_msr(tb[i]));
		printf("\n");
	}

	if (bl31_get_cornerinfo(efuseinfo, sizeof(efuseinfo) / sizeof(uint8_t)) != 0) {
		printf("fail get efuse info\n");
		return 0;
	}

	printf("osc efuse info:\n");
	for (i = 0; i < 4; i++)
		printf("0x%x, ", efuseinfo[i]);
	printf("\n");

	/*efuse to test value*/
	printf("osc_ring_core0(20lvt), idd_ee, idd_cpu\n");

	printf("%d KHz ", (efuseinfo[1] * 20));

	switch (efuseinfo[0]) {
	case 1:
		multiple = 400;
		break;
	case 2:
		multiple = 800;
		break;
	default:
		multiple = 400;
	}

	for (i = 2; i <= 3; i++) {
		printf("%d uA ",  (efuseinfo[i] * multiple));
	}

	printf("\n");

	return 0;
}
