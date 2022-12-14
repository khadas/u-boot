#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/clock.h>
#include <asm/arch/oscring.h>

void ring_powerinit(void)
{
	writel(0x18000a, RING_PWM_VCCK_A);/*set vcck 0.8v*/
	writel(0x18000a, RING_PWM_VCCK_B);/*set vcck 0.8v*/
	writel(0x8000a, RING_PWM_EE);/*set ee 0.8v*/
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
			[32] = "am_ring_osc_clk_out[32] " ,
			[31] = "am_ring_osc_clk_out[31] " ,
			[30] = "am_ring_osc_clk_out[30] " ,
			[29] = "am_ring_osc_clk_out[29] " ,
			[28] = "am_ring_osc_clk_out[28] " ,
			[27] = "am_ring_osc_clk_out[27] " ,
			[26] = "am_ring_osc_clk_out[26] " ,
			[25] = "am_ring_osc_clk_out[25] " ,
			[24] = "am_ring_osc_clk_out[24] " ,
			[23] = "am_ring_osc_clk_out[23] " ,
			[22] = "am_ring_osc_clk_out[22] " ,
			[21] = "am_ring_osc_clk_out[21] " ,
			[20] = "am_ring_osc_clk_out[20] " ,
			[19] = "am_ring_osc_clk_out[19] " ,
			[18] = "am_ring_osc_clk_out[18] " ,
			[17] = "am_ring_osc_clk_out[17] " ,
			[16] = "am_ring_osc_clk_out[16] " ,
			[15] = "am_ring_osc_clk_out[15] " ,
			[14] = "am_ring_osc_clk_out[14] " ,
			[13] = "am_ring_osc_clk_out[13] " ,
			[12] = "am_ring_osc_clk_out[12] " ,
			[11] = "am_ring_osc_clk_out[11] " ,
			[10] = "am_ring_osc_clk_out[10] " ,
			[9] = "am_ring_osc_clk_out[9] " ,
			[8] = "am_ring_osc_clk_out[8] " ,
			[7] = "am_ring_osc_clk_out[7] " ,
			[6] = "am_ring_osc_clk_out[6] " ,
			[5] = "am_ring_osc_clk_out[5] " ,
			[4] = "am_ring_osc_clk_out[4] " ,
			[3] = "am_ring_osc_clk_out[3] " ,
			[2] = "am_ring_osc_clk_out[2] " ,
			[1] = "am_ring_osc_clk_out[1] " ,
			[0] = "am_ring_osc_clk_out[0] " ,
		};
	const int tb[] = {180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
			  190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
			  206, 207, 208, 209, 210, 211, 212, 213, 214, 215,
			  216, 217, 218};
	unsigned long i;
	uint8_t efuseinfo[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	if ((index != 0xff) && (index != 0)) {
		if (bl31_get_cornerinfo(efuseinfo, sizeof(efuseinfo) / sizeof(uint8_t)) != 0) {
			printf("fail get efuse info\n");
			return 0;
		}
		if ((index >= 1) && (index <= 11))
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
	writel(OSCRING_CTL_DATA2, OSCRING_CTL_REG2);

	for (i = 0; i < 33; i++) {
		printf("%s      :",clk_table[i]);
		printf("%ld     KHz",clk_util_ring_msr(tb[i]));
		printf("\n");
	}

	if (bl31_get_cornerinfo(efuseinfo, sizeof(efuseinfo) / sizeof(uint8_t)) != 0) {
		printf("fail get efuse info\n");
		return 0;
	}

	printf("osc efuse info:\n");
	for (i = 0; i < 10; i++)
		printf("0x%x, ", efuseinfo[i]);
	printf("0x%x, ", efuseinfo[11]);
	printf("\n");

	/*efuse to test value*/
	printf("idd_ee, idd_cpu_a53, a53_osc_ring, idd_cpu_a73, a73_osc_ring, idd_nn, nna_osc_ring, idd_gpu, gpu_osc_ring, vpu_osc_ring\n");

	printf("%d uA ", (efuseinfo[1] * 400));
	printf("%d uA ", (efuseinfo[2] * 400));
	printf("%d KHz ", (efuseinfo[3] * 20));
	printf("%d uA ", (efuseinfo[4] * 400));
	printf("%d KHz ", (efuseinfo[5] * 20));
	printf("%d uA ", (efuseinfo[6] * 400));
	printf("%d KHz ", (efuseinfo[7] * 20));
	printf("%d uA ", (efuseinfo[8] * 400));
	printf("%d KHz ", (efuseinfo[9] * 20));
	printf("%d KHz ", (efuseinfo[11] * 20));

	printf("\n");

	return 0;
}
