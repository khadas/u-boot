#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/clock.h>
#include <asm/arch/mailbox.h>
#include <asm/arch/oscring.h>
#include <amlogic/cpu_id.h>

void ring_powerinit(void)
{
	writel(0x150007, RING_PWM_VCCK);/*set vcck 0.8v*/
	writel(0x10000c, RING_PWM_EE);/*set ee 0.8v*/
}

unsigned long clk_util_ring_msr(unsigned long clk_mux)
{
	unsigned int regval = 0;

	WRITE_CBUS_REG(MSR_CLK_REG0, 0);
	/* Set the measurement gate to 64uS */
	CLEAR_CBUS_REG_MASK(MSR_CLK_REG0, 0xffff);
	/* 64uS is enough for measure the frequence*/
	SET_CBUS_REG_MASK(MSR_CLK_REG0, (10000 - 1));
	/* Disable continuous measurement */
	/* Disable interrupts */
	CLEAR_CBUS_REG_MASK(MSR_CLK_REG0, ((1 << 18) | (1 << 17)));
	CLEAR_CBUS_REG_MASK(MSR_CLK_REG0, (0x7f << 20));
	SET_CBUS_REG_MASK(MSR_CLK_REG0, (clk_mux << 20) | /* Select MUX */
			(1 << 19) |       /* enable the clock */
			(1 << 16));       /* enable measuring */
	/* Wait for the measurement to be done */
	regval = READ_CBUS_REG(MSR_CLK_REG0);
	do {
		regval = READ_CBUS_REG(MSR_CLK_REG0);
	} while (regval & (1 << 31));

	/* Disable measuring */
	CLEAR_CBUS_REG_MASK(MSR_CLK_REG0, (1 << 16));
	regval = (READ_CBUS_REG(MSR_CLK_REG2) + 31) & 0x000FFFFF;

	return (regval / 10);
}

int ring_msr_g12a(int index)
{
	const char* clk_table[] = {
			[11] = "sys_cpu_ring_osc_clk[1] " ,
			[10] = "sys_cpu_ring_osc_clk[0] " ,
			[9] = "am_ring_osc_clk_out_ee[9] " ,
			[8] = "am_ring_osc_clk_out_ee[8] " ,
			[7] = "am_ring_osc_clk_out_ee[7] " ,
			[6] = "am_ring_osc_clk_out_ee[6] " ,
			[5] = "am_ring_osc_clk_out_ee[5] " ,
			[4] = "am_ring_osc_clk_out_ee[4] " ,
			[3] = "am_ring_osc_clk_out_ee[3] " ,
			[2] = "am_ring_osc_clk_out_ee[2] " ,
			[1] = "am_ring_osc_clk_out_ee[1] " ,
			[0] = "am_ring_osc_clk_out_ee[0] " ,
		};
	const int tb[] = {0, 1, 2, 99, 100, 101, 102, 103, 104, 105, 3, 33};
	unsigned long i;
	unsigned char efuseinfo[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	if ((index != 0xff) && (index != 0)) {
		if (oscring_get_value(efuseinfo) != 0) {
			printf("fail get efuse info\n");
			return 0;
		}
		if ((index >= 1) && (index <= 5))
			printf("%d\n", (efuseinfo[index] * 20));
		if ((index >= 6) && (index <= 7))
			printf("%d\n", (efuseinfo[index] * 200));
		if ((index >= 8) && (index <= 11))
			printf("%d\n", efuseinfo[index]);
		if ((index < 0) || (index >= 12))
			printf("input data not support!\n");
		return 0;
	}
	if (0xff != index) {
		ring_powerinit();
		printf("set vcck vddee to 800mv\n");
	}
	/*RING_OSCILLATOR       0x7f: set slow ring*/
	writel(OSCRING_CTL_DATA, OSCRING_CTL_REG);
	for (i = 0; i < 12; i++) {
		printf("%s      :",clk_table[i]);
		printf("%ld     KHz",clk_util_ring_msr(tb[i]));
		printf("\n");
	}

	if (oscring_get_value(efuseinfo) != 0) {
		printf("fail get efuse info\n");
		return 0;
	}

	printf("osc efuse info:\n");
	for (i = 0; i < sizeof(efuseinfo) / sizeof(uint8_t); i++)
		printf("0x%x, ", efuseinfo[i]);
	printf("\n");

	/*efuse to test value*/
	printf("ee[9], ee[1], ee[0], cpu[1], cpu[0], iddee, iddcpu, sltver, fta53, fta73, slt\n");
	for (i = 1; i <= 5; i++)
		printf("%d KHz ", (efuseinfo[i] * 20));

	for (i = 6; i <= 7; i++)
		printf("%d uA ", (efuseinfo[i] * 200));

	for (i = 8; i <= 11; i++)
		printf("%d  ", efuseinfo[i]);

	printf("\n");

	return 0;
}

int ring_msr_sm1(int index)
{
	const char* clk_table[] = {
			[15] = "cpu[3]_A55_LVT16 " ,
			[14] = "cpu[2]_A55_ULVT16 " ,
			[13] = "cpu[1]_A55_ULVT20 " ,
			[12] = "cpu[0]_A55_ULVT16 " ,
			[11] = "ee[11]_ddrtop_LVT16 " ,
			[10] = "ee[10]_dos_ULVT20 " ,
			[9] = "ee[9]_dos_LVT16 " ,
			[8] = "ee[8]_dos_SVT16 " ,
			[7] = "ee[7]_dos_SVT24 " ,
			[6] = "ee[6]_vpu_LVT16 " ,
			[5] = "ee[5]_vpu_ULVT20 " ,
			[4] = "ee[4]_vpu_SVT24 " ,
			[3] = "ee[3]_mali_SVT16 " ,
			[2] = "ee[2]_mali_ULVT16 " ,
			[1] = "ee[1]_mali_LVT16 " ,
			[0] = "ee[0]_dmctop_LVT16 " ,
		};
	const int tb[] = {0, 1, 2, 3, 40, 60, 74, 76, 85, 86, 99, 100, 101, 102, 103, 104};
	unsigned long i;
	unsigned char efuseinfo[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	if ((0xff != index) && (0 != index) && (0xfe != index)) {
		if (efuse_get_value(efuseinfo) != 0) {
			printf("fail get efuse info\n");
			return 0;
		}
		if ((index >= 1) && (index <= 4))
			printf("%d\n", (efuseinfo[index] * 20));
		if ((index >=5) && (index <=7))
			printf("%d\n", (efuseinfo[index] * 400));
		if ((index >= 8) && (index <= 11))
			printf("%d\n", efuseinfo[index]);
		if ((index < 0) || (index >= 12))
			printf("input data not support!\n");
		return 0;
	}
	if (0xfe != index) {
		if (0x0 == index)
			ring_powerinit(); /*index 0 set vcck and vddee, index ff no set valtage*/
		/*RING_OSCILLATOR       0x7f: set slow ring*/
		writel(0x51555555, 0xff6345fc);
		for (i = 0; i < 16; i++) {
			printf("%s      :",clk_table[i]);
			printf("%ld     KHz",clk_util_ring_msr(tb[i]));
			printf("\n");
		}
	}

	if (efuse_get_value(efuseinfo) != 0) {
		printf("fail get osc ring efuse info\n");
		return 0;
	}

	printf("osc ring efuse info:\n");
	for (i = 0; i <= 11; i++)
		printf("0x%x, ", efuseinfo[i]);
	printf("\n");

	/*efuse to test value*/

	printf("cpu[0]_A55_ring0, ee[2]_mail_ring1, ee[0]_dmc_ring, cpu[0]_A55_ring2, iddee, reserve, iddcpu_A55, sltver, fta53, fta73, slt\n");
	for (i = 1; i <= 4; i++)
		printf("%d KHz, ", (efuseinfo[i] * 20));

	printf("%d uA, ", (efuseinfo[5] * 400));
	printf("%d, ", efuseinfo[6]);
	printf("%d uA, ", (efuseinfo[7] * 400));

	for (i = 8; i <= 11; i++)
		printf("%d,  ", efuseinfo[i]);

	printf("\n");

	return 0;
}

int ring_msr(int index)
{
	if (get_cpu_id().family_id == MESON_CPU_MAJOR_ID_G12A)
		return ring_msr_g12a(index);
	if (get_cpu_id().family_id == MESON_CPU_MAJOR_ID_SM1)
		return ring_msr_sm1(index);
	return 0;

}
