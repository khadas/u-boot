/*
 * (C) Copyright 2025 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <i2c.h>
#include <irq-generic.h>
#include <power/fuel_gauge.h>
#include <linux/usb/phy-rockchip-usb2.h>
#include <power/power_delivery/power_delivery.h>

DECLARE_GLOBAL_DATA_PTR;

static int dbg_enable;

#define CPS_DBG(args...) \
	do { \
		if (dbg_enable) { \
			printf(args); \
		} \
	} while (0)

#define UPDATE(x, h, l)			(((x) << (l)) & GENMASK((h), (l)))

/* Register 00h */
#define CPS5601X_REG_00			0x00
#define CPS5601X_PRODUCT_ID_MASK	GENMASK(7, 0)
/* default 0xA9=CPS5601 */

/* Register 01h */
#define CPS5601X_REG_01			0x01

/* Register 02h */
#define CPS5601X_REG_02			0x02

/* Register 03h */
#define CPS5601X_REG_03			0x03
#define VREG_MASK			GENMASK(6, 0)
#define VREG_BASE			3600000
#define VREG_LSB			10000
#define VREG_MAXVAL			0x6e

/* Register 04h */
#define CPS5601X_REG_04			0x04
#define ICHG_MASK			GENMASK(6, 0)
#define ICHG_BASE			0
#define ICHG_LSB			25000
#define ICHG_MINVAL			0x4
#define ICHG_MAXVAL			0x78

/* Register 05h */
#define CPS5601X_REG_05			0x05
#define EN_TERM_MASK			BIT(6)
#define EN_TERM_ENABLE(x)		UPDATE(x, 6, 6)
#define IPRECHG_MASK			GENMASK(5, 0)
#define IPRECHG_BASE			0
#define IPRECHG_LSB			12500
#define IPRECHG_MINVAL			0x1
#define IPRECHG_MAXVAL			0x3c

/* Register 06h */
#define CPS5601X_REG_06			0x06
#define ITERM_MASK			GENMASK(5, 0)
#define ITERM_BASE			0
#define ITERM_LSB			10000
#define ITERM_MINVAL			0x5
#define ITERM_MAXVAL			0x3c

/* Register 07h */
#define CPS5601X_REG_07			0x07
#define VINDPM_MASK			GENMASK(5, 0)
#define VINDPM_BASE			3400000
#define VINDPM_LSB			100000
#define VINDPM_MINVAL			0x4
#define VINDPM_MAXVAL			0x3e

/* Register 08h */
#define CPS5601X_REG_08			0x08
#define IINDPM_MASK			GENMASK(5, 0)
#define IINDPM_BASE			50000
#define IINDPM_LSB			50000
#define IINDPM_MINVAL			0x1

/* Register 09h */
#define CPS5601X_REG_09			0x09
#define VOTG_MASK			GENMASK(5, 0)
#define VOTG_BASE			3400000
#define VOTG_LSB			100000
#define VOTG_MAXVAL			0x3e

/* Register 0Ah */
#define CPS5601X_REG_0A			0x0A
#define IOTG_MASK			GENMASK(5, 0)
#define IOTG_BASE			50000
#define IOTG_LSB			50000
#define IOTG_MINVAL			0x1

/* Register 0Bh */
#define CPS5601X_REG_0B			0x0B
#define WATCHDOG_MASK			GENMASK(7, 6)
#define WATCHDOG_TIME(x)		UPDATE(x, 7, 6)
#define WATCHDOG_BASE			0
#define WATCHDOG_LSB			40
#define WD_RST_MASK			BIT(5)
#define WD_RST(x)			UPDATE(x, 5, 5)
#define EN_CHG_MASK			BIT(3)
#define EN_CHG(x)			UPDATE(x, 3, 3)

/* Register 0Ch */
#define CPS5601X_REG_0C			0x0C
#define EN_OTG_MASK			BIT(3)
#define EN_OTG(x)			UPDATE(x, 3, 3)

/* Register 0Dh */
#define CPS5601X_REG_0D			0x0D

/* Register 0Eh */
#define CPS5601X_REG_0E			0x0E
#define TS_IGNORE_MASK			BIT(0)
#define EN_TS_IGNORE(x)			UPDATE(x, 0, 0)

/* Register 0Fh */
#define CPS5601X_REG_0F			0x0F
#define PG_STAT_MASK			BIT(3)

/* Register 10h */
#define CPS5601X_REG_10			0x10
#define CHG_STAT_MASK			GENMASK(7, 5)
#define CHG_STAT_SHIFT			5
#define CHG_STAT_NOTCHG			0
#define CHG_STAT_TRICKLECHG		1
#define CHG_STAT_PRECHG			2
#define CHG_STAT_FASTCHG		3
#define CHG_STAT_TAPERCHG		4
#define CHG_STAT_RESERVED		5
#define CHG_STAT_TOTACHG		6
#define CHG_STAT_CHGTERM		7
#define VBUS_STAT_MASK			GENMASK(4, 1)
#define VBUS_STAT_SHIFT			1
#define VBUS_STAT_NOT			0
#define VBUS_STAT_USBSDP		1
#define VBUS_STAT_USBCDP		2
#define VBUS_STAT_USBDCP		3
#define VBUS_STAT_HVDCP			4
#define VBUS_STAT_UNKNOWN		5
#define VBUS_STAT_NONSTANDARD		6
#define VBUS_STAT_OTGMODE		7
#define VBUS_STAT_NOTQUALIFIED		8

/* Register 11h */
#define CPS5601X_REG_11			0x11

/* Register 12h */
#define CPS5601X_REG_12			0x12

/* Register 13h */
#define CPS5601X_REG_13			0x13

/* Register 14h */
#define CPS5601X_REG_14			0x14

/* Register 15h */
#define CPS5601X_REG_15			0x15

/* Register 16h */
#define CPS5601X_REG_16			0x16

/* Register 17h */
#define CPS5601X_REG_17			0x17

/* Register 18h */
#define CPS5601X_REG_18			0x18

/* Register 19h */
#define CPS5601X_REG_19			0x19
#define TREG_MK_MASK			BIT(7)

/* Register 1Ah */
#define CPS5601X_REG_1A			0x1A

/* Register 1Bh */
#define CPS5601X_REG_1B			0x1B

#define CPS5601X_ICHRG_I_DEF_uA		2040000
#define CPS5601X_VREG_V_DEF_uV		4208000
#define CPS5601X_PRECHRG_I_DEF_uA	180000
#define CPS5601X_TERMCHRG_I_DEF_uA	180000
#define CPS5601X_ICHRG_I_MIN_uA		100000
#define CPS5601X_ICHRG_I_MAX_uA		3000000
#define CPS5601X_VINDPM_DEF_uV		4500000
#define CPS5601X_VINDPM_V_MIN_uV	3800000
#define CPS5601X_VINDPM_V_MAX_uV	9600000
#define CPS5601X_IINDPM_DEF_uA		2400000
#define CPS5601X_IINDPM_I_MIN_uA	100000
#define CPS5601X_IINDPM_I_MAX_uA	3200000
#define DEFAULT_INPUT_CURRENT		(500 * 1000)

struct cps5601x {
	struct udevice *dev;
	struct udevice *pd;
	bool pd_online;
	u32 init_count;
	u32 ichg;
	u32 vchg;
	int irq;
};

enum power_supply_type {
	POWER_SUPPLY_TYPE_UNKNOWN = 0,
	POWER_SUPPLY_TYPE_USB,          /* Standard Downstream Port */
	POWER_SUPPLY_TYPE_USB_DCP,      /* Dedicated Charging Port */
	POWER_SUPPLY_TYPE_USB_CDP,      /* Charging Downstream Port */
	POWER_SUPPLY_TYPE_USB_FLOATING, /* DCP without shorting D+/D- */
};

static int cps5601x_read(struct cps5601x *charger, uint reg, u8 *buffer)
{
	u8 val;
	int ret;

	ret = dm_i2c_read(charger->dev, reg, &val, 1);
	if (ret) {
		printf("cps5601x: read %#x error, ret=%d", reg, ret);
		return ret;
	}

	*buffer = val;

	return 0;
}

static int cps5601x_write(struct cps5601x *charger, uint reg, u8 val)
{
	int ret;

	ret = dm_i2c_write(charger->dev, reg, &val, 1);
	if (ret)
		printf("cps5601x: write %#x error, ret=%d", reg, ret);

	return ret;
}

static int cps5601x_update_bits(struct cps5601x *charger,
				u8 offset,
				u8 mask,
				u8 val)
{
	u8 reg;
	int ret;

	ret = cps5601x_read(charger, offset, &reg);
	if (ret)
		return ret;

	reg &= ~mask;

	return cps5601x_write(charger, offset, reg | val);
}

static int cps5601x_set_input_current_limit(struct cps5601x *cps, int curr)
{
	u8 val;

	if (curr < IINDPM_BASE + (IINDPM_MINVAL * IINDPM_LSB))
		curr = IINDPM_BASE + (IINDPM_MINVAL * IINDPM_LSB);

	val = (curr - IINDPM_BASE) / IINDPM_LSB;

	return cps5601x_update_bits(cps, CPS5601X_REG_08, IINDPM_MASK, val);
}

static int cps5601x_get_usb_type(void)
{
#ifdef CONFIG_PHY_ROCKCHIP_INNO_USB2
	return rockchip_chg_get_type();
#else
	return 0;
#endif
}

static int cps5601x_charger_capability(struct udevice *dev)
{
	return FG_CAP_CHARGER;
}

static int cps5601x_set_chargecurrent(struct cps5601x *cps, int curr)
{
	u8 ichg;

	if (curr < (ICHG_BASE + (ICHG_MINVAL * ICHG_LSB)))
		curr = ICHG_BASE + (ICHG_MINVAL * ICHG_LSB);
	else if (curr > (ICHG_BASE + (ICHG_MAXVAL * ICHG_LSB)))
		curr = ICHG_BASE + (ICHG_MAXVAL * ICHG_LSB);

	ichg = (curr - ICHG_BASE) / ICHG_LSB;

	return cps5601x_update_bits(cps, CPS5601X_REG_04, ICHG_MASK, ichg);
}

static int cps5601x_set_iprechg(struct cps5601x *cps, int curr)
{
	u8 iprechg;

	if (curr < (IPRECHG_BASE + (IPRECHG_MINVAL * IPRECHG_LSB)))
		curr = IPRECHG_BASE + (IPRECHG_MINVAL * IPRECHG_LSB);
	else if (curr > (IPRECHG_BASE + (IPRECHG_MAXVAL * IPRECHG_LSB)))
		curr = IPRECHG_BASE + (IPRECHG_MAXVAL * IPRECHG_LSB);

	iprechg = (curr - IPRECHG_BASE) / IPRECHG_LSB;

	return cps5601x_update_bits(cps, CPS5601X_REG_05, IPRECHG_MASK,
				    iprechg);
}

static int cps5601x_set_chargevolt(struct cps5601x *cps, int volt)
{
	u8 val;

	if (volt < VREG_BASE)
		volt = VREG_BASE;
	else if (volt > (VREG_BASE + (VREG_MAXVAL * VREG_LSB)))
		volt = VREG_BASE + (VREG_MAXVAL * VREG_LSB);

	val = (volt - VREG_BASE) / VREG_LSB;

	return cps5601x_update_bits(cps, CPS5601X_REG_03, VREG_MASK, val);
}

static int cps5601x_set_charger_voltage(struct udevice *dev, int uV)
{
	struct cps5601x *charger = dev_get_priv(dev);

	CPS_DBG("CPS5601X: charger voltage %d\n", uV);
	return cps5601x_set_chargevolt(charger, uV);
}

static int cps5601x_charger_enable(struct udevice *dev)
{
	struct cps5601x *charger = dev_get_priv(dev);

	CPS_DBG("CPS5601X: charger enable\n");
	return cps5601x_update_bits(charger, CPS5601X_REG_0B, EN_CHG_MASK,
				    EN_CHG(1));
}

static int cps5601x_charger_disable(struct udevice *dev)
{
	struct cps5601x *charger = dev_get_priv(dev);

	CPS_DBG("CPS5601X: charger disable\n");
	return cps5601x_update_bits(charger, CPS5601X_REG_0B, EN_CHG_MASK,
				    EN_CHG(0));
}

static int cps5601x_iprechg_current(struct udevice *dev, int iprechrg_uA)
{
	struct cps5601x *charger = dev_get_priv(dev);

	CPS_DBG("CPS5601x: charger current:iprechrg_uA: %d\n",
		iprechrg_uA);

	return cps5601x_set_iprechg(charger, iprechrg_uA);
}

static int cps5601x_charger_current(struct udevice *dev, int ichrg_uA)
{
	struct cps5601x *charger = dev_get_priv(dev);

	CPS_DBG("CPS5601X: charger current:ichrg_uA%d\n",
		ichrg_uA);

	return cps5601x_set_chargecurrent(charger, ichrg_uA);
}

static int cps5601x_get_pd_output_val(struct cps5601x *charger,
				      int *vol,
				      int *cur)
{
	struct power_delivery_data pd_data;
	int ret;

	if (!charger->pd)
		return -EINVAL;

	memset(&pd_data, 0, sizeof(pd_data));
	ret = power_delivery_get_data(charger->pd, &pd_data);
	if (ret)
		return ret;
	if (!pd_data.online || !pd_data.voltage || !pd_data.current)
		return -EINVAL;

	*vol = pd_data.voltage;
	*cur = pd_data.current;
	charger->pd_online = pd_data.online;

	return 0;
}

static void cps5601x_charger_input_current_init(struct cps5601x *charger)
{
	int sdp_inputcurrent = 500 * 1000;
	int dcp_inputcurrent = 2000 * 1000;
	int pd_inputvol, pd_inputcurrent;
	int ret;

	if (!charger->pd) {
		ret = uclass_get_device(UCLASS_PD, 0, &charger->pd);
		if (ret) {
			if (ret == -ENODEV)
				printf("cps5601x: Can't find PD\n");
			else
				printf("cps5601x: Get UCLASS PD failed: %d\n", ret);
			charger->pd = NULL;
		}
	}

	if (!cps5601x_get_pd_output_val(charger, &pd_inputvol, &pd_inputcurrent)) {
		CPS_DBG("pd adapter\n");
		cps5601x_set_input_current_limit(charger, pd_inputcurrent);
	} else {
		CPS_DBG("normal adapter: %d\n", cps5601x_get_usb_type());
		if (cps5601x_get_usb_type() == POWER_SUPPLY_TYPE_USB_DCP)
			cps5601x_set_input_current_limit(charger, dcp_inputcurrent);
		else if (cps5601x_get_usb_type() == POWER_SUPPLY_TYPE_USB_CDP)
			cps5601x_set_input_current_limit(charger, dcp_inputcurrent);
		else if (cps5601x_get_usb_type() == POWER_SUPPLY_TYPE_USB_FLOATING)
			cps5601x_set_input_current_limit(charger, dcp_inputcurrent);
		else
			cps5601x_set_input_current_limit(charger, sdp_inputcurrent);
	}
}

static bool cps5601x_charger_status(struct udevice *dev)
{
	struct cps5601x *charger = dev_get_priv(dev);
	int state_of_charger;
	u8 value;
	int i = 0;

__retry:
	cps5601x_read(charger, CPS5601X_REG_0F, &value);
	state_of_charger = !!(value & PG_STAT_MASK);
	if (!state_of_charger && charger->pd_online) {
		if (i < 3) {
			i++;
			mdelay(20);
			goto __retry;
		}
	}

	if ((state_of_charger) && (charger->init_count < 5)) {
		cps5601x_charger_input_current_init(charger);
		cps5601x_charger_enable(dev);
		charger->init_count++;
	}

	if (!state_of_charger)
		cps5601x_set_iprechg(charger, CPS5601X_PRECHRG_I_DEF_uA);

	CPS_DBG("dump register:\n");
	for (i = CPS5601X_REG_00; i < CPS5601X_REG_1B; i++) {
		cps5601x_read(charger, i, &value);
		CPS_DBG("[%d]: 0x%x\n", i, value);
	}
	return state_of_charger;
}

static void cps5601x_irq_handler(int irq, void *data)
{
}

static int cps5601x_ofdata_to_platdata(struct udevice *dev)
{
	struct cps5601x *charger = dev_get_priv(dev);
	u32 interrupt, phandle;
	int ret;

	charger->dev = dev;
	charger->ichg = dev_read_u32_default(dev,
					     "vbat-current-limit-microamp",
					     0);
	if (charger->ichg == 0)
		charger->ichg = 3000 * 1000;
	charger->vchg = dev_read_u32_default(dev,
					     "vbat-voltage-limit-microamp",
					     0);
	if (charger->vchg == 0)
		charger->vchg = 4400 * 1000;
	CPS_DBG("charger->ichg: %d\n", charger->ichg);
	CPS_DBG("charger->vchg: %d\n", charger->vchg);

	phandle = dev_read_u32_default(dev, "interrupt-parent", -ENODATA);
	if (phandle == -ENODATA) {
		printf("cps5601x: read 'interrupt-parent' failed, ret=%d\n",
		       phandle);
		return phandle;
	}

	ret = dev_read_u32_array(dev, "interrupts", &interrupt, 1);
	if (ret) {
		printf("cps5601x: read 'interrupts' failed, ret=%d\n", ret);
		return ret;
	}

	charger->irq = phandle_gpio_to_irq(phandle, interrupt);
	if (charger->irq < 0)
		printf("cps5601x: failed to request irq: %d\n", charger->irq);

	return 0;
}

static int cps5601x_probe(struct udevice *dev)
{
	struct cps5601x *charger = dev_get_priv(dev);
	u8 value;
	int i;

	CPS_DBG("cps5601x: driver version-202502024\n");
	CPS_DBG("cps5601x: dump register:\n");
	for (i = CPS5601X_REG_00; i < CPS5601X_REG_1B; i++) {
		cps5601x_read(charger, i, &value);
		CPS_DBG("cps5601x: [%d]: 0x%x\n", i, value);
	}

	charger->dev = dev;
	/* disable watchdog */
	cps5601x_update_bits(charger, CPS5601X_REG_0B, WATCHDOG_MASK, WATCHDOG_TIME(0));

	cps5601x_update_bits(charger, CPS5601X_REG_0E, TS_IGNORE_MASK, EN_TS_IGNORE(1));

	cps5601x_set_chargecurrent(charger, charger->ichg);
	cps5601x_set_chargevolt(charger, charger->vchg);

	if (0 && charger->irq) {
		CPS_DBG("cps5601x: enable cps5601x irq\n");
		irq_install_handler(charger->irq, cps5601x_irq_handler, dev);
		irq_handler_enable(charger->irq);
	}

	return 0;
}

static const struct udevice_id charger_ids[] = {
	{ .compatible = "cps,cps5601x" },
	{ },
};

static struct dm_fuel_gauge_ops charger_ops = {
	.get_chrg_online = cps5601x_charger_status,
	.capability = cps5601x_charger_capability,
	.set_charger_voltage = cps5601x_set_charger_voltage,
	.set_charger_enable = cps5601x_charger_enable,
	.set_charger_disable = cps5601x_charger_disable,
	.set_charger_current = cps5601x_charger_current,
	.set_iprechg_current = cps5601x_iprechg_current,

};

U_BOOT_DRIVER(cps5601x_charger) = {
	.name = "cps5601x_charger",
	.id = UCLASS_FG,
	.probe = cps5601x_probe,
	.of_match = charger_ids,
	.ops = &charger_ops,
	.ofdata_to_platdata = cps5601x_ofdata_to_platdata,
	.priv_auto_alloc_size = sizeof(struct cps5601x),
};
