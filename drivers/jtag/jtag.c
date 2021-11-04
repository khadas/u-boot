#include <common.h>
#include <command.h>
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>

#define JTAG_M3_AO	0
#define JTAG_M3_EE	1
#define JTAG_AP_AO	2
#define JTAG_AP_EE	3
#define JTAG_M4_AO	8
#define JTAG_M4_EE	9
#define SWD_AP_AO	10

struct jtag_pinctrl_reg_desc {
	unsigned char mode;
	volatile uint32_t *reg;
	uint32_t val;
	uint32_t mask;
	uint32_t valbak;
};

/* axg */
#if defined(CONFIG_TARGET_MESON_AXG)
	struct jtag_pinctrl_reg_desc regs_data[] = {
		{JTAG_AP_AO, P_AO_RTI_PINMUX_REG0,
			(0x4 << 12) | (0x4 << 16) | (0x4 << 20) | (0x4 << 28),
			(0xf << 12) | (0xf << 16) | (0xf << 20) | (0xf << 28)},
		{JTAG_AP_EE, P_PERIPHS_PIN_MUX_4,
			(0x2 << 0) | (0x2 << 4) | (0x2 << 16) | (0x2 << 20),
			(0xf << 0) | (0xf << 4) | (0xf << 16) | (0xf << 20)},
	};
/* g12a/g12b/tl1 */
#elif defined(CONFIG_TARGET_MESON_G12A) || \
	defined(CONFIG_TARGET_MESON_G12B) || \
	defined(CONFIG_TARGET_MESON_TL1) || \
	defined(CONFIG_TARGET_MESON_SM1) || \
	defined(CONFIG_TARGET_MESON_TM2) || \
	defined(CONFIG_TARGET_MESON_T5D) || \
	defined(CONFIG_TARGET_MESON_T5W) || \
	defined(CONFIG_TARGET_MESON_T5)
	struct jtag_pinctrl_reg_desc regs_data[] = {
		{JTAG_AP_AO, P_AO_RTI_PINMUX_REG0,
			(0x1 << 24) | (0x1 << 28),
			(0xf << 24) | (0xf << 28)},
		{JTAG_AP_AO, P_AO_RTI_PINMUX_REG1,
			(0x1 << 4) | (0x1 << 0),
			(0xf << 4) | (0xf << 0)},
		{JTAG_AP_EE, P_PERIPHS_PIN_MUX_9,
			(0x2 << 0) | (0x2 << 4) | (0x2 << 16) | (0x2 << 20),
			(0xf << 0) | (0xf << 4) | (0xf << 16) | (0xf << 20)},
		{SWD_AP_AO, P_AO_RTI_PINMUX_REG0,
			(0x4 << 24) | (0x4 << 28),
			(0xf << 24) | (0xf << 28)},
	};
/* txhd */
#elif defined(CONFIG_TARGET_MESON_TXHD)
	struct jtag_pinctrl_reg_desc regs_data[] = {
		{JTAG_AP_AO, P_AO_RTI_PIN_MUX_REG,
			(0x3 << 12) | (0x4 << 16) | (0x4 << 20) | (0x3 << 28),
			(0xf << 12) | (0xf << 16) | (0xf << 20) | (0xf << 28)},
		{JTAG_AP_EE, P_PERIPHS_PIN_MUX_9,
			(0x2 << 0) | (0x2 << 4) | (0x2 << 16) | (0x2 << 20),
			(0xf << 0) | (0xf << 4) | (0xf << 16) | (0xf << 20)},
	};
/* axg before */
#else
	struct jtag_pinctrl_reg_desc regs_data[] = {
		{0xff, (uint32_t *)0xffffffff, 0xffffffff},
	};

#endif

static int enabled_flag = 0;
void jtag_set_pinmux(unsigned int sel, int enable)
{
	int i;
	uint32_t value;

	if (regs_data[0].mode == 0xff)
		return;
	if (enable) {
		for (i = 0; i < sizeof(regs_data)/sizeof(struct jtag_pinctrl_reg_desc); i++) {
			if (sel == regs_data[i].mode) {
				enabled_flag = 1;
				value = readl(regs_data[i].reg);
				regs_data[i].valbak = value;

				value = (value & ~regs_data[i].mask) |
							regs_data[i].val;
				writel(value, regs_data[i].reg);
			}
		}
	} else {
		if (enabled_flag == 0)
			return;
		for (i = 0; i < sizeof(regs_data)/sizeof(struct jtag_pinctrl_reg_desc); i++) {
			if (sel == regs_data[i].mode) {
				enabled_flag = 0;
				writel(regs_data[i].valbak, regs_data[i].reg);
			}
		}
	}
}

