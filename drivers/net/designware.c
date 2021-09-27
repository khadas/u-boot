// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2010
 * Vipin Kumar, ST Micoelectronics, vipin.kumar@st.com.
 */

/*
 * Designware ethernet IP driver for U-Boot
 */

#include <common.h>
#include <clk.h>
#include <dm.h>
#include <errno.h>
#include <miiphy.h>
#include <malloc.h>
#include <pci.h>
#include <reset.h>
#include <linux/compiler.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <asm/arch/secure_apb.h>
#include <asm/io.h>
#include <power/regulator.h>
#include "designware.h"
#include <linux/ioport.h>

#ifdef CONFIG_DM_ETH
#include <asm/arch/pwr_ctrl.h>
#include <asm/arch/register.h>
#include <dm/pinctrl.h>
#ifdef CONFIG_DM_GPIO
#include <asm/gpio.h>
#endif
struct dw_eth_dev *priv_tool = NULL;
struct phy_device *p_phydev = NULL;

#ifndef ANACTRL_PLL_GATE_DIS
#define ANACTRL_PLL_GATE_DIS 0xffffffff
#endif

#endif

#define AML_ETH_PLL_CTL0 0x44
#define AML_ETH_PLL_CTL1 0x48
#define AML_ETH_PLL_CTL2 0x4C
#define AML_ETH_PLL_CTL3 0x50
#define AML_ETH_PLL_CTL4 0x54
#define AML_ETH_PLL_CTL5 0x58
#define AML_ETH_PLL_CTL6 0x5C
#define AML_ETH_PLL_CTL7 0x60

#define AML_ETH_PHY_CNTL0 0x80
#define AML_ETH_PHY_CNTL1 0x84
#define AML_ETH_PHY_CNTL2 0x88
enum {
	/* chip num */
	ETH_PHY		= 0x0,
	ETH_PHY_C1	= 0x1,
	ETH_PHY_C2	= 0x2,
	ETH_PHY_SC2	= 0x3,
};

static int dw_mdio_read(struct mii_dev *bus, int addr, int devad, int reg)
{
#ifdef CONFIG_DM_ETH
	struct dw_eth_dev *priv = dev_get_priv((struct udevice *)bus->priv);
	struct eth_mac_regs *mac_p = priv->mac_regs_p;
#else
	struct eth_mac_regs *mac_p = bus->priv;
#endif
	ulong start;
	u16 miiaddr;
	int timeout = CONFIG_MDIO_TIMEOUT;

	miiaddr = ((addr << MIIADDRSHIFT) & MII_ADDRMSK) |
		  ((reg << MIIREGSHIFT) & MII_REGMSK);

	writel(miiaddr | MII_CLKRANGE_150_250M | MII_BUSY, &mac_p->miiaddr);

	start = get_timer(0);
	while (get_timer(start) < timeout) {
		if (!(readl(&mac_p->miiaddr) & MII_BUSY))
			return readl(&mac_p->miidata);
		udelay(10);
	};

	return -ETIMEDOUT;
}

static int dw_mdio_write(struct mii_dev *bus, int addr, int devad, int reg,
			u16 val)
{
#ifdef CONFIG_DM_ETH
	struct dw_eth_dev *priv = dev_get_priv((struct udevice *)bus->priv);
	struct eth_mac_regs *mac_p = priv->mac_regs_p;
#else
	struct eth_mac_regs *mac_p = bus->priv;
#endif
	ulong start;
	u16 miiaddr;
	int ret = -ETIMEDOUT, timeout = CONFIG_MDIO_TIMEOUT;

	writel(val, &mac_p->miidata);
	miiaddr = ((addr << MIIADDRSHIFT) & MII_ADDRMSK) |
		  ((reg << MIIREGSHIFT) & MII_REGMSK) | MII_WRITE;

	writel(miiaddr | MII_CLKRANGE_150_250M | MII_BUSY, &mac_p->miiaddr);

	start = get_timer(0);
	while (get_timer(start) < timeout) {
		if (!(readl(&mac_p->miiaddr) & MII_BUSY)) {
			ret = 0;
			break;
		}
		udelay(10);
	};

	return ret;
}

#if defined(CONFIG_DM_ETH) && defined(CONFIG_DM_GPIO)
static int dw_mdio_reset(struct mii_dev *bus)
{
	struct udevice *dev = bus->priv;
	struct dw_eth_dev *priv = dev_get_priv(dev);
	struct dw_eth_pdata *pdata = dev_get_platdata(dev);
	int ret;

	if (!dm_gpio_is_valid(&priv->reset_gpio))
		return 0;

	/* reset the phy */
	ret = dm_gpio_set_value(&priv->reset_gpio, 0);
	if (ret)
		return ret;

	udelay(pdata->reset_delays[0]);

	ret = dm_gpio_set_value(&priv->reset_gpio, 1);
	if (ret)
		return ret;

	udelay(pdata->reset_delays[1]);

	ret = dm_gpio_set_value(&priv->reset_gpio, 0);
	if (ret)
		return ret;

	udelay(pdata->reset_delays[2]);

	return 0;
}
#endif

static int dw_mdio_init(const char *name, void *priv)
{
	struct mii_dev *bus = mdio_alloc();

	if (!bus) {
		printf("Failed to allocate MDIO bus\n");
		return -ENOMEM;
	}

	bus->read = dw_mdio_read;
	bus->write = dw_mdio_write;
	snprintf(bus->name, sizeof(bus->name), "%s", name);
#if defined(CONFIG_DM_ETH) && defined(CONFIG_DM_GPIO)
	bus->reset = dw_mdio_reset;
#endif

	bus->priv = priv;

	return mdio_register(bus);
}

static void tx_descs_init(struct dw_eth_dev *priv)
{
	struct eth_dma_regs *dma_p = priv->dma_regs_p;
	struct dmamacdescr *desc_table_p = &priv->tx_mac_descrtable[0];
	char *txbuffs = &priv->txbuffs[0];
	struct dmamacdescr *desc_p;
	u32 idx;

	for (idx = 0; idx < CONFIG_TX_DESCR_NUM; idx++) {
		desc_p = &desc_table_p[idx];
		desc_p->dmamac_addr = (ulong)&txbuffs[idx * CONFIG_ETH_BUFSIZE];
		desc_p->dmamac_next = (ulong)&desc_table_p[idx + 1];

#if defined(CONFIG_DW_ALTDESCRIPTOR)
		desc_p->txrx_status &= ~(DESC_TXSTS_TXINT | DESC_TXSTS_TXLAST |
				DESC_TXSTS_TXFIRST | DESC_TXSTS_TXCRCDIS |
				DESC_TXSTS_TXCHECKINSCTRL |
				DESC_TXSTS_TXRINGEND | DESC_TXSTS_TXPADDIS);

		desc_p->txrx_status |= DESC_TXSTS_TXCHAIN;
		desc_p->dmamac_cntl = 0;
		desc_p->txrx_status &= ~(DESC_TXSTS_MSK | DESC_TXSTS_OWNBYDMA);
#else
		desc_p->dmamac_cntl = DESC_TXCTRL_TXCHAIN;
		desc_p->txrx_status = 0;
#endif
	}

	/* Correcting the last pointer of the chain */
	desc_p->dmamac_next = (ulong)&desc_table_p[0];

	/* Flush all Tx buffer descriptors at once */
	flush_dcache_range((ulong)priv->tx_mac_descrtable,
			   (ulong)priv->tx_mac_descrtable +
			   sizeof(priv->tx_mac_descrtable));

	writel((ulong)&desc_table_p[0], &dma_p->txdesclistaddr);
	priv->tx_currdescnum = 0;
}

static void rx_descs_init(struct dw_eth_dev *priv)
{
	struct eth_dma_regs *dma_p = priv->dma_regs_p;
	struct dmamacdescr *desc_table_p = &priv->rx_mac_descrtable[0];
	char *rxbuffs = &priv->rxbuffs[0];
	struct dmamacdescr *desc_p;
	u32 idx;

	/* Before passing buffers to GMAC we need to make sure zeros
	 * written there right after "priv" structure allocation were
	 * flushed into RAM.
	 * Otherwise there's a chance to get some of them flushed in RAM when
	 * GMAC is already pushing data to RAM via DMA. This way incoming from
	 * GMAC data will be corrupted. */
	flush_dcache_range((ulong)rxbuffs, (ulong)rxbuffs + RX_TOTAL_BUFSIZE);

	for (idx = 0; idx < CONFIG_RX_DESCR_NUM; idx++) {
		desc_p = &desc_table_p[idx];
		desc_p->dmamac_addr = (ulong)&rxbuffs[idx * CONFIG_ETH_BUFSIZE];
		desc_p->dmamac_next = (ulong)&desc_table_p[idx + 1];

		desc_p->dmamac_cntl =
			(MAC_MAX_FRAME_SZ & DESC_RXCTRL_SIZE1MASK) |
				      DESC_RXCTRL_RXCHAIN;

		desc_p->txrx_status = DESC_RXSTS_OWNBYDMA;
	}

	/* Correcting the last pointer of the chain */
	desc_p->dmamac_next = (ulong)&desc_table_p[0];

	/* Flush all Rx buffer descriptors at once */
	flush_dcache_range((ulong)priv->rx_mac_descrtable,
			   (ulong)priv->rx_mac_descrtable +
			   sizeof(priv->rx_mac_descrtable));

	writel((ulong)&desc_table_p[0], &dma_p->rxdesclistaddr);
	priv->rx_currdescnum = 0;
}

static int _dw_write_hwaddr(struct dw_eth_dev *priv, u8 *mac_id)
{
	struct eth_mac_regs *mac_p = priv->mac_regs_p;
	u32 macid_lo, macid_hi;

	macid_lo = mac_id[0] + (mac_id[1] << 8) + (mac_id[2] << 16) +
		   (mac_id[3] << 24);
	macid_hi = mac_id[4] + (mac_id[5] << 8);

	writel(macid_hi, &mac_p->macaddr0hi);
	writel(macid_lo, &mac_p->macaddr0lo);

	return 0;
}

static int dw_adjust_link(struct dw_eth_dev *priv, struct eth_mac_regs *mac_p,
			  struct phy_device *phydev)
{
	u32 conf = readl(&mac_p->conf) | FRAMEBURSTENABLE | DISABLERXOWN;

	if (!phydev->link) {
		printf("%s: No link.\n", phydev->dev->name);
		return 0;
	}

	if (phydev->speed != 1000)
		conf |= MII_PORTSELECT;
	else
		conf &= ~MII_PORTSELECT;

	if (phydev->speed == 100)
		conf |= FES_100;

	if (phydev->duplex)
		conf |= FULLDPLXMODE;

	writel(conf, &mac_p->conf);

	printf("Speed: %d, %s duplex%s\n", phydev->speed,
	       (phydev->duplex) ? "full" : "half",
	       (phydev->port == PORT_FIBRE) ? ", fiber mode" : "");

	return 0;
}

static void _dw_eth_halt(struct dw_eth_dev *priv)
{
	struct eth_mac_regs *mac_p = priv->mac_regs_p;
	struct eth_dma_regs *dma_p = priv->dma_regs_p;

	writel(readl(&mac_p->conf) & ~(RXENABLE | TXENABLE), &mac_p->conf);
	writel(readl(&dma_p->opmode) & ~(RXSTART | TXSTART), &dma_p->opmode);

	phy_shutdown(priv->phydev);
}

int designware_eth_init(struct dw_eth_dev *priv, u8 *enetaddr)
{
	struct eth_mac_regs *mac_p = priv->mac_regs_p;
	struct eth_dma_regs *dma_p = priv->dma_regs_p;
	unsigned int start;
	int ret;

	writel(readl(&dma_p->busmode) | DMAMAC_SRST, &dma_p->busmode);

	/*
	 * When a MII PHY is used, we must set the PS bit for the DMA
	 * reset to succeed.
	 */
	if (priv->phydev->interface == PHY_INTERFACE_MODE_MII)
		writel(readl(&mac_p->conf) | MII_PORTSELECT, &mac_p->conf);
	else
		writel(readl(&mac_p->conf) & ~MII_PORTSELECT, &mac_p->conf);

	start = get_timer(0);
	while (readl(&dma_p->busmode) & DMAMAC_SRST) {
		if (get_timer(start) >= CONFIG_MACRESET_TIMEOUT) {
			printf("DMA reset timeout\n");
			return -ETIMEDOUT;
		}

		mdelay(100);
	};

	/*
	 * Soft reset above clears HW address registers.
	 * So we have to set it here once again.
	 */
	_dw_write_hwaddr(priv, enetaddr);

	rx_descs_init(priv);
	tx_descs_init(priv);

	writel(FIXEDBURST | PRIORXTX_41 | DMA_PBL, &dma_p->busmode);

#ifndef CONFIG_DW_MAC_FORCE_THRESHOLD_MODE
	writel(readl(&dma_p->opmode) | FLUSHTXFIFO | STOREFORWARD,
	       &dma_p->opmode);
#else
	writel(readl(&dma_p->opmode) | FLUSHTXFIFO,
	       &dma_p->opmode);
#endif

	writel(readl(&dma_p->opmode) | RXSTART | TXSTART, &dma_p->opmode);

#ifdef CONFIG_DW_AXI_BURST_LEN
	writel((CONFIG_DW_AXI_BURST_LEN & 0x1FF >> 1), &dma_p->axibus);
#endif

	/* Start up the PHY */
	ret = phy_startup(priv->phydev);
	if (ret) {
		printf("Could not initialize PHY %s\n",
		       priv->phydev->dev->name);
		return ret;
	}

	ret = dw_adjust_link(priv, mac_p, priv->phydev);
	if (ret)
		return ret;

	return 0;
}

int designware_eth_enable(struct dw_eth_dev *priv)
{
	struct eth_mac_regs *mac_p = priv->mac_regs_p;

	if (!priv->phydev->link)
		return -EIO;

	writel(readl(&mac_p->conf) | RXENABLE | TXENABLE, &mac_p->conf);

	return 0;
}

#define ETH_ZLEN	60

static int _dw_eth_send(struct dw_eth_dev *priv, void *packet, int length)
{
	struct eth_dma_regs *dma_p = priv->dma_regs_p;
	u32 desc_num = priv->tx_currdescnum;
	struct dmamacdescr *desc_p = &priv->tx_mac_descrtable[desc_num];
	ulong desc_start = (ulong)desc_p;
	ulong desc_end = desc_start +
		roundup(sizeof(*desc_p), ARCH_DMA_MINALIGN);
	ulong data_start = desc_p->dmamac_addr;
	ulong data_end = data_start + roundup(length, ARCH_DMA_MINALIGN);
	/*
	 * Strictly we only need to invalidate the "txrx_status" field
	 * for the following check, but on some platforms we cannot
	 * invalidate only 4 bytes, so we flush the entire descriptor,
	 * which is 16 bytes in total. This is safe because the
	 * individual descriptors in the array are each aligned to
	 * ARCH_DMA_MINALIGN and padded appropriately.
	 */
	invalidate_dcache_range(desc_start, desc_end);

	/* Check if the descriptor is owned by CPU */
	if (desc_p->txrx_status & DESC_TXSTS_OWNBYDMA) {
		printf("CPU not owner of tx frame\n");
		return -EPERM;
	}

	length = max(length, ETH_ZLEN);

	memcpy((void *)data_start, packet, length);

	/* Flush data to be sent */
	flush_dcache_range(data_start, data_end);

#if defined(CONFIG_DW_ALTDESCRIPTOR)
	desc_p->txrx_status |= DESC_TXSTS_TXFIRST | DESC_TXSTS_TXLAST;
	desc_p->dmamac_cntl |= (length << DESC_TXCTRL_SIZE1SHFT) &
			       DESC_TXCTRL_SIZE1MASK;

	desc_p->txrx_status &= ~(DESC_TXSTS_MSK);
	desc_p->txrx_status |= DESC_TXSTS_OWNBYDMA;
#else
	desc_p->dmamac_cntl |= ((length << DESC_TXCTRL_SIZE1SHFT) &
			       DESC_TXCTRL_SIZE1MASK) | DESC_TXCTRL_TXLAST |
			       DESC_TXCTRL_TXFIRST;

	desc_p->txrx_status = DESC_TXSTS_OWNBYDMA;
#endif

	/* Flush modified buffer descriptor */
	flush_dcache_range(desc_start, desc_end);

	/* Test the wrap-around condition. */
	if (++desc_num >= CONFIG_TX_DESCR_NUM)
		desc_num = 0;

	priv->tx_currdescnum = desc_num;

	/* Start the transmission */
	writel(POLL_DATA, &dma_p->txpolldemand);

	return 0;
}

static int _dw_eth_recv(struct dw_eth_dev *priv, uchar **packetp)
{
	u32 status, desc_num = priv->rx_currdescnum;
	struct dmamacdescr *desc_p = &priv->rx_mac_descrtable[desc_num];
	int length = -EAGAIN;
	ulong desc_start = (ulong)desc_p;
	ulong desc_end = desc_start +
		roundup(sizeof(*desc_p), ARCH_DMA_MINALIGN);
	ulong data_start = desc_p->dmamac_addr;
	ulong data_end;

	/* Invalidate entire buffer descriptor */
	invalidate_dcache_range(desc_start, desc_end);

	status = desc_p->txrx_status;

	/* Check  if the owner is the CPU */
	if (!(status & DESC_RXSTS_OWNBYDMA)) {

		length = (status & DESC_RXSTS_FRMLENMSK) >>
			 DESC_RXSTS_FRMLENSHFT;

		/* Invalidate received data */
		data_end = data_start + roundup(length, ARCH_DMA_MINALIGN);
		invalidate_dcache_range(data_start, data_end);
		*packetp = (uchar *)(ulong)desc_p->dmamac_addr;
	}

	return length;
}

static int _dw_free_pkt(struct dw_eth_dev *priv)
{
	u32 desc_num = priv->rx_currdescnum;
	struct dmamacdescr *desc_p = &priv->rx_mac_descrtable[desc_num];
	ulong desc_start = (ulong)desc_p;
	ulong desc_end = desc_start +
		roundup(sizeof(*desc_p), ARCH_DMA_MINALIGN);

	/*
	 * Make the current descriptor valid again and go to
	 * the next one
	 */
	desc_p->txrx_status |= DESC_RXSTS_OWNBYDMA;

	/* Flush only status field - others weren't changed */
	flush_dcache_range(desc_start, desc_end);

	/* Test the wrap-around condition. */
	if (++desc_num >= CONFIG_RX_DESCR_NUM)
		desc_num = 0;
	priv->rx_currdescnum = desc_num;

	return 0;
}

static int dw_phy_init(struct dw_eth_dev *priv, void *dev)
{
	struct phy_device *phydev;
	int mask = 0xffffffff, ret;

#ifdef CONFIG_PHY_ADDR
	mask = 1 << CONFIG_PHY_ADDR;
#endif

	phydev = phy_find_by_mask(priv->bus, mask, priv->interface);
	if (!phydev)
		return -ENODEV;

	phy_connect_dev(phydev, dev);

	phydev->supported &= PHY_GBIT_FEATURES;
	if (priv->max_speed) {
		ret = phy_set_supported(phydev, priv->max_speed);
		if (ret)
			return ret;
	}
	phydev->advertising = phydev->supported;

	priv->phydev = phydev;
	phy_config(phydev);

	return 0;
}

#ifndef CONFIG_DM_ETH
static int dw_eth_init(struct eth_device *dev, bd_t *bis)
{
	int ret;

	ret = designware_eth_init(dev->priv, dev->enetaddr);
	if (!ret)
		ret = designware_eth_enable(dev->priv);

	return ret;
}

static int dw_eth_send(struct eth_device *dev, void *packet, int length)
{
	return _dw_eth_send(dev->priv, packet, length);
}

static int dw_eth_recv(struct eth_device *dev)
{
	uchar *packet;
	int length;

	length = _dw_eth_recv(dev->priv, &packet);
	if (length == -EAGAIN)
		return 0;
	net_process_received_packet(packet, length);

	_dw_free_pkt(dev->priv);

	return 0;
}

static void dw_eth_halt(struct eth_device *dev)
{
	return _dw_eth_halt(dev->priv);
}

static int dw_write_hwaddr(struct eth_device *dev)
{
	return _dw_write_hwaddr(dev->priv, dev->enetaddr);
}

int designware_initialize(ulong base_addr, u32 interface)
{
	struct eth_device *dev;
	struct dw_eth_dev *priv;

	dev = (struct eth_device *) malloc(sizeof(struct eth_device));
	if (!dev)
		return -ENOMEM;

	/*
	 * Since the priv structure contains the descriptors which need a strict
	 * buswidth alignment, memalign is used to allocate memory
	 */
	priv = (struct dw_eth_dev *) memalign(ARCH_DMA_MINALIGN,
					      sizeof(struct dw_eth_dev));
	if (!priv) {
		free(dev);
		return -ENOMEM;
	}

	if ((phys_addr_t)priv + sizeof(*priv) > (1ULL << 32)) {
		printf("designware: buffers are outside DMA memory\n");
		return -EINVAL;
	}

	memset(dev, 0, sizeof(struct eth_device));
	memset(priv, 0, sizeof(struct dw_eth_dev));

	sprintf(dev->name, "dwmac.%lx", base_addr);
	dev->iobase = (int)base_addr;
	dev->priv = priv;

	priv->dev = dev;
	priv->mac_regs_p = (struct eth_mac_regs *)base_addr;
	priv->dma_regs_p = (struct eth_dma_regs *)(base_addr +
			DW_DMA_BASE_OFFSET);

	dev->init = dw_eth_init;
	dev->send = dw_eth_send;
	dev->recv = dw_eth_recv;
	dev->halt = dw_eth_halt;
	dev->write_hwaddr = dw_write_hwaddr;

	eth_register(dev);

	priv->interface = interface;

	dw_mdio_init(dev->name, priv->mac_regs_p);
	priv->bus = miiphy_get_dev_by_name(dev->name);

	return dw_phy_init(priv, dev);
}
#endif

#ifdef CONFIG_DM_ETH
static int designware_eth_start(struct udevice *dev)
{
	struct eth_pdata *pdata = dev_get_platdata(dev);
	struct dw_eth_dev *priv = dev_get_priv(dev);
	int ret;

	ret = designware_eth_init(priv, pdata->enetaddr);
	if (ret)
		return ret;
	ret = designware_eth_enable(priv);
	if (ret)
		return ret;

	return 0;
}

int designware_eth_send(struct udevice *dev, void *packet, int length)
{
	struct dw_eth_dev *priv = dev_get_priv(dev);

	return _dw_eth_send(priv, packet, length);
}

int designware_eth_recv(struct udevice *dev, int flags, uchar **packetp)
{
	struct dw_eth_dev *priv = dev_get_priv(dev);

	return _dw_eth_recv(priv, packetp);
}

int designware_eth_free_pkt(struct udevice *dev, uchar *packet, int length)
{
	struct dw_eth_dev *priv = dev_get_priv(dev);

	return _dw_free_pkt(priv);
}

void designware_eth_stop(struct udevice *dev)
{
	struct dw_eth_dev *priv = dev_get_priv(dev);

	return _dw_eth_halt(priv);
}

int designware_eth_write_hwaddr(struct udevice *dev)
{
	struct eth_pdata *pdata = dev_get_platdata(dev);
	struct dw_eth_dev *priv = dev_get_priv(dev);

	return _dw_write_hwaddr(priv, pdata->enetaddr);
}

static int designware_eth_bind(struct udevice *dev)
{
#ifdef CONFIG_DM_PCI
	static int num_cards;
	char name[20];

	/* Create a unique device name for PCI type devices */
	if (device_is_on_pci_bus(dev)) {
		sprintf(name, "eth_designware#%u", num_cards++);
		device_set_name(dev, name);
	}
#endif

	return 0;
}

#ifdef CONFIG_DM_ETH
unsigned int setup_amp;
void setup_tx_amp(struct udevice *dev)
{
	unsigned int tx_amp_src = 0;
	tx_amp_src = dev_read_u32_default(dev, "tx_amp_src", 0);
	if (0 == tx_amp_src) {
		printf("not set tx_amp_src\n");
	} else {
		setup_amp = readl((uintptr_t)tx_amp_src);
		printf("addr 0x%x  =  0x%x\n", tx_amp_src, readl((uintptr_t)tx_amp_src));
	}
}
static void setup_internal_phy(struct udevice *dev)
{
	int phy_cntl1 = 0;
	int mc_val = 0;
	int chip_num = 0;
	unsigned int pll_val[3] = {0};
	unsigned int analog_val[3] = {0};
	int rtn = 0;
	struct resource eth_top, eth_cfg;

	phy_cntl1 = dev_read_u32_default(dev, "phy_cntl1", 4);
	if (phy_cntl1 < 0) {
		printf("miss phy_cntl1\n");
	}

	mc_val = dev_read_u32_default(dev, "mc_val", 4);
	if (mc_val < 0) {
		printf("miss mc_val\n");
	}

	chip_num = dev_read_u32_default(dev, "chip_num", 4);
	if (chip_num < 0) {
		chip_num = 0;
		printf("use 0 as default chip num\n");
	}
	printf("chip num %d\n", chip_num);

	rtn = dev_read_u32_array(dev, "pll_val", pll_val, ARRAY_SIZE(pll_val));
	if (rtn < 0) {
		printf("miss pll_val\n");
	}

	dev_read_u32_array(dev, "analog_val", analog_val, ARRAY_SIZE(analog_val));
	if (rtn < 0) {
		printf("miss analog_val\n");
	}

	for (int i = 0;i <3; i++) {
		debug("pll_val 0x%08x\n", pll_val[i]);
	}

	for (int i = 0;i <3; i++) {
		debug("analog_val 0x%08x\n", analog_val[i]);
	}

	rtn = dev_read_resource_byname(dev, "eth_top", &eth_top);
	if (rtn) {
		printf("can't get eth_top resource(ret = %d)\n", rtn);
	}

	rtn = dev_read_resource_byname(dev, "eth_cfg", &eth_cfg);
	if (rtn) {
		printf("can't get eth_cfg resource(ret = %d)\n", rtn);
	}
//	printf("wzh eth_top 0x%x eth_cfg 0x%x \n", eth_top.start, eth_cfg.start);

	setup_tx_amp(dev);
	/*top*/
//	setbits_le32(ETHTOP_CNTL0, mc_val);
	setbits_le32(eth_top.start, mc_val);
	/*pll*/
	writel(pll_val[0] | 0x30000000, eth_cfg.start + AML_ETH_PLL_CTL0);
	writel(pll_val[1], eth_cfg.start + AML_ETH_PLL_CTL1);
	writel(pll_val[2], eth_cfg.start + AML_ETH_PLL_CTL2);
	writel(0x00000000, eth_cfg.start + AML_ETH_PLL_CTL3);
	udelay(200);
	writel(pll_val[0] | 0x10000000, eth_cfg.start + AML_ETH_PLL_CTL0);

	/*analog*/
	writel(analog_val[0], eth_cfg.start + AML_ETH_PLL_CTL5);
	writel(analog_val[1], eth_cfg.start + AML_ETH_PLL_CTL6);
	writel(analog_val[2], eth_cfg.start + AML_ETH_PLL_CTL7);

	/*ctrl*/
	/*config phyid should between  a 0~0xffffffff*/
	/*please don't use 44000181, this has been used by internal phy*/
	writel(0x33000180, eth_cfg.start + AML_ETH_PHY_CNTL0);

	/*use_phy_smi | use_phy_ip | co_clkin from eth_phy_top*/
	writel(0x260, eth_cfg.start + AML_ETH_PHY_CNTL2);
	writel(phy_cntl1, eth_cfg.start + AML_ETH_PHY_CNTL1);
	writel(phy_cntl1 & (~0x40000), eth_cfg.start + AML_ETH_PHY_CNTL1);
	writel(phy_cntl1, eth_cfg.start + AML_ETH_PHY_CNTL1);
	udelay(200);

	if (chip_num != ETH_PHY_SC2) {
		clrbits_le32(ANACTRL_PLL_GATE_DIS, (0x1 << 6));
		clrbits_le32(ANACTRL_PLL_GATE_DIS, (0x1 << 7));
		clrbits_le32(ANACTRL_PLL_GATE_DIS, (0x1 << 19));
	}
}

static void setup_external_phy(struct udevice *dev)
{
	int mc_val = 0;
	int cali_val = 0;
	int analog_ver = 0;
	int chip_num = 0;
	int rtn = 0;
	struct resource eth_top, eth_cfg;
	/*reset phy*/
	struct gpio_desc desc;
	int ret;

	chip_num = dev_read_u32_default(dev, "chip_num", 4);
	if (chip_num < 0) {
		chip_num = 0;
		printf("use 0 as default chip num\n");
	}
	printf("chip num %d\n", chip_num);

	if (chip_num != ETH_PHY_SC2) {
		ret = gpio_request_by_name(dev, "reset-gpios", 0, &desc, GPIOD_IS_OUT);
		if (ret) {
			printf("request gpio failed!\n");
		//	return ret;
		}
		if (dm_gpio_is_valid(&desc)) {
			dm_gpio_set_value(&desc, 1);
			mdelay(100);
		}
		dm_gpio_free(dev, &desc);
	}

	mc_val = dev_read_u32_default(dev, "mc_val", 4);
	if (mc_val < 0) {
		printf("miss mc_val\n");
	}

	cali_val = dev_read_u32_default(dev, "cali_val", 4);
	if (mc_val < 0) {
		printf("miss cali_val\n");
	}

	/*set rmii pinmux*/
	if (mc_val & 0x4) {
		pinctrl_select_state(dev, "external_eth_rmii_pins");
		printf("set rmii\n");
	}
	/*set rgmii pinmux*/
	if (mc_val & 0x1) {
		pinctrl_select_state(dev, "external_eth_rgmii_pins");
		printf("set rgmii\n");
	}
	rtn = dev_read_resource_byname(dev, "eth_top", &eth_top);
	if (rtn) {
		printf("can't get eth_top resource(ret = %d)\n", rtn);
	}

	rtn = dev_read_resource_byname(dev, "eth_cfg", &eth_cfg);
	if (rtn) {
		printf("can't get eth_cfg resource(ret = %d)\n", rtn);
	}
//	printf("eth_top 0x%x eth_cfg 0x%x \n", eth_top.start, eth_cfg.start);

	setbits_le32(eth_top.start, mc_val);
	setbits_le32(eth_top.start + 4, cali_val);

	analog_ver = dev_read_u32_default(dev, "analog_ver", 4);
	if (mc_val < 0) {
		printf("miss analog_ver\n");
	}
	if (analog_ver != 2)
		writel(0x0, eth_cfg.start + AML_ETH_PHY_CNTL2);

	if (chip_num != ETH_PHY_SC2) {
		clrbits_le32(ANACTRL_PLL_GATE_DIS, (0x1 << 6));
		clrbits_le32(ANACTRL_PLL_GATE_DIS, (0x1 << 7));
		clrbits_le32(ANACTRL_PLL_GATE_DIS, (0x1 << 19));
	}
}

#endif
#ifdef CONFIG_DM_ETH
static void __iomem *DM_network_interface_setup(struct udevice *dev)
{
	int internal_phy = 0;

	internal_phy = dev_read_u32_default(dev, "internal_phy", 1);
	if (internal_phy < 0) {
		debug("miss internal_phy item\n");
	}
	debug("internal_phy = 0x%x\n", internal_phy);
	if (internal_phy) {
		printf("in-phy\n");
		setup_internal_phy(dev);
	} else {
		printf("ex-phy\n");
		setup_external_phy(dev);
	}
	udelay(1000);
	return 0;
}
#endif
/*parse dts end*/

int designware_eth_probe(struct udevice *dev)
{
	struct eth_pdata *pdata = dev_get_platdata(dev);
	struct dw_eth_dev *priv = dev_get_priv(dev);
	u32 iobase = pdata->iobase;
	ulong ioaddr;
	int ret;
	struct reset_ctl_bulk reset_bulk;

#ifdef CONFIG_DM_ETH
	printf("pwr up eth\n");
	pwr_ctrl_psci_smc(PM_ETH, 1);
	DM_network_interface_setup(dev);
#endif
#ifdef CONFIG_CLK
	int i, err, clock_nb;

	priv->clock_count = 0;
	clock_nb = dev_count_phandle_with_args(dev, "clocks", "#clock-cells");
	if (clock_nb > 0) {
		priv->clocks = devm_kcalloc(dev, clock_nb, sizeof(struct clk),
					    GFP_KERNEL);
		if (!priv->clocks)
			return -ENOMEM;

		for (i = 0; i < clock_nb; i++) {
			err = clk_get_by_index(dev, i, &priv->clocks[i]);
			if (err < 0)
				break;

			err = clk_enable(&priv->clocks[i]);
			if (err && err != -ENOSYS && err != -ENOTSUPP) {
				pr_err("failed to enable clock %d\n", i);
				clk_free(&priv->clocks[i]);
				goto clk_err;
			}
			priv->clock_count++;
		}
	} else if (clock_nb != -ENOENT) {
		pr_err("failed to get clock phandle(%d)\n", clock_nb);
		return clock_nb;
	}
#endif

#if defined(CONFIG_DM_REGULATOR)
	struct udevice *phy_supply;

	ret = device_get_supply_regulator(dev, "phy-supply",
					  &phy_supply);
	if (ret) {
		debug("%s: No phy supply\n", dev->name);
	} else {
		ret = regulator_set_enable(phy_supply, true);
		if (ret) {
			puts("Error enabling phy supply\n");
			return ret;
		}
	}
#endif

	ret = reset_get_bulk(dev, &reset_bulk);
	if (ret)
		dev_warn(dev, "Can't get reset: %d\n", ret);
	else
		reset_deassert_bulk(&reset_bulk);

#ifdef CONFIG_DM_PCI
	/*
	 * If we are on PCI bus, either directly attached to a PCI root port,
	 * or via a PCI bridge, fill in platdata before we probe the hardware.
	 */
	if (device_is_on_pci_bus(dev)) {
		dm_pci_read_config32(dev, PCI_BASE_ADDRESS_0, &iobase);
		iobase &= PCI_BASE_ADDRESS_MEM_MASK;
		iobase = dm_pci_mem_to_phys(dev, iobase);

		pdata->iobase = iobase;
		pdata->phy_interface = PHY_INTERFACE_MODE_RMII;
	}
#endif

	debug("%s, iobase=%x, priv=%p\n", __func__, iobase, priv);
	ioaddr = iobase;
	priv->mac_regs_p = (struct eth_mac_regs *)ioaddr;
	priv->dma_regs_p = (struct eth_dma_regs *)(ioaddr + DW_DMA_BASE_OFFSET);
	priv->interface = pdata->phy_interface;
	priv->max_speed = pdata->max_speed;

	dw_mdio_init(dev->name, dev);
	priv->bus = miiphy_get_dev_by_name(dev->name);

	ret = dw_phy_init(priv, dev);
	debug("%s, ret=%d\n", __func__, ret);

#ifdef CONFIG_DM_ETH
	priv_tool = priv;
#endif
	return ret;

#ifdef CONFIG_CLK
clk_err:
	ret = clk_release_all(priv->clocks, priv->clock_count);
	if (ret)
		pr_err("failed to disable all clocks\n");

	return err;
#endif
}

static int designware_eth_remove(struct udevice *dev)
{
	struct dw_eth_dev *priv = dev_get_priv(dev);

	free(priv->phydev);
	mdio_unregister(priv->bus);
	mdio_free(priv->bus);

#ifdef CONFIG_CLK
	return clk_release_all(priv->clocks, priv->clock_count);
#else
	return 0;
#endif
}

static void *eqos_alloc_descs(unsigned int num)
{
#ifdef CONFIG_SYS_NONCACHED_MEMORY
	return (void *)noncached_alloc(EQOS_DESCRIPTORS_SIZE,
				      EQOS_DESCRIPTOR_ALIGN);
#else
	return memalign(EQOS_DESCRIPTOR_ALIGN, EQOS_DESCRIPTORS_SIZE);
#endif
}

static void eqos_free_descs(void *descs)
{
#ifdef CONFIG_SYS_NONCACHED_MEMORY
	/* FIXME: noncached_alloc() has no opposite */
#else
	free(descs);
#endif
}

static int eqos_mdio_read(struct mii_dev *bus, int addr, int devad, int reg)
{
	struct eqos_mac_regs *mac_p = bus->priv;
	ulong start;
	u32 miiaddr;
	int timeout = CONFIG_MDIO_TIMEOUT;
	miiaddr = (((addr & 0x1f ) << EQOS_MAC_MDIO_ADDRESS_PA_SHIFT) |
		  ((reg & 0x1f) << EQOS_MAC_MDIO_ADDRESS_RDA_SHIFT) |
		  (EQOS_MAC_MDIO_ADDRESS_CR_150_250 << EQOS_MAC_MDIO_ADDRESS_CR_SHIFT)|
		  (EQOS_MAC_MDIO_ADDRESS_GOC_READ << EQOS_MAC_MDIO_ADDRESS_GOC_SHIFT));

	writel(miiaddr | EQOS_MAC_MDIO_ADDRESS_GB, &mac_p->mdio_address);

	start = get_timer(0);
	while (get_timer(start) < timeout) {
		if (!(readl(&mac_p->mdio_address) & EQOS_MAC_MDIO_ADDRESS_GB))
			return readl(&mac_p->mdio_data);
		udelay(10);
	};
	return -1;
}

static int eqos_mdio_write(struct mii_dev *bus, int addr, int devad, int reg,
			u16 val)
{
	struct eqos_mac_regs *mac_p = bus->priv;
	ulong start;
	u32 miiaddr;
	int ret = -1, timeout = CONFIG_MDIO_TIMEOUT;
	writel(val, &mac_p->mdio_data);
	miiaddr = (((addr & 0x1f ) << EQOS_MAC_MDIO_ADDRESS_PA_SHIFT) |
		  ((reg & 0x1f) << EQOS_MAC_MDIO_ADDRESS_RDA_SHIFT) |
		  (EQOS_MAC_MDIO_ADDRESS_CR_150_250 << EQOS_MAC_MDIO_ADDRESS_CR_SHIFT)|
		  (EQOS_MAC_MDIO_ADDRESS_GOC_WRITE<< EQOS_MAC_MDIO_ADDRESS_GOC_SHIFT));

	writel(miiaddr | EQOS_MAC_MDIO_ADDRESS_GB, &mac_p->mdio_address);

	start = get_timer(0);
	while (get_timer(start) < timeout) {
		if (!(readl(&mac_p->mdio_address) & EQOS_MAC_MDIO_ADDRESS_GB)) {
			ret = 0;
			break;
		}
		udelay(10);
	};

	return ret;
}

static int eqos_mdio_init(const char *name, void *mac_regs_p)
{
	struct mii_dev *bus = mdio_alloc();

	if (!bus) {
		printf("Failed to allocate MDIO bus\n");
		return -1;
	}

	bus->read = eqos_mdio_read;
	bus->write = eqos_mdio_write;
	sprintf(bus->name, name);

	bus->priv = (void *)mac_regs_p;

	return mdio_register(bus);
}

static void eqos_inval_buffer(void *buf, size_t size)
{
	unsigned long start = (unsigned long)buf & ~(ARCH_DMA_MINALIGN - 1);
	unsigned long end = ALIGN(start + size, ARCH_DMA_MINALIGN);

	invalidate_dcache_range(start, end);
}

static void eqos_flush_buffer(void *buf, size_t size)
{
	flush_dcache_range((phys_addr_t)buf,
			   (phys_addr_t)((unsigned char *)buf+size));
}

static void eqos_inval_desc(void *desc)
{
#ifndef CONFIG_SYS_NONCACHED_MEMORY
	unsigned long start = (unsigned long)desc & ~(ARCH_DMA_MINALIGN - 1);
	unsigned long end = ALIGN(start + EQOS_DESCRIPTOR_SIZE,
				  ARCH_DMA_MINALIGN);

	invalidate_dcache_range(start, end);
#endif
}

static void eqos_flush_desc(void *desc)
{
#ifndef CONFIG_SYS_NONCACHED_MEMORY
	flush_dcache_range((phys_addr_t)desc, (phys_addr_t)((unsigned char *)desc+EQOS_DESCRIPTOR_SIZE));
#endif
}

static int eqos_set_full_duplex(struct udevice *dev)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);

	setbits_le32(&eqos->mac_regs_p->configuration, EQOS_MAC_CONFIGURATION_DM);

	return 0;
}

static int eqos_set_half_duplex(struct udevice *dev)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);

	clrbits_le32(&eqos->mac_regs_p->configuration, EQOS_MAC_CONFIGURATION_DM);

	/* WAR: Flush TX queue when switching to half-duplex */
	setbits_le32(&eqos->mtl_regs_p->txq0_operation_mode,
		     EQOS_MTL_TXQ0_OPERATION_MODE_FTQ);

	return 0;
}

static int eqos_set_gmii_speed(struct udevice *dev)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);

	clrbits_le32(&eqos->mac_regs_p->configuration,
		     EQOS_MAC_CONFIGURATION_PS | EQOS_MAC_CONFIGURATION_FES);

	return 0;
}

static int eqos_set_mii_speed_100(struct udevice *dev)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);

	setbits_le32(&eqos->mac_regs_p->configuration,
		     EQOS_MAC_CONFIGURATION_PS | EQOS_MAC_CONFIGURATION_FES);

	return 0;
}

static int eqos_set_mii_speed_10(struct udevice *dev)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);

	clrsetbits_le32(&eqos->mac_regs_p->configuration,
			EQOS_MAC_CONFIGURATION_FES, EQOS_MAC_CONFIGURATION_PS);

	return 0;
}

static int eqos_adjust_link(struct udevice *dev)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);
	int ret;

	debug("%s(dev=%p):\n", __func__, dev);

	if (eqos->phydev->duplex)
		ret = eqos_set_full_duplex(dev);
	else
		ret = eqos_set_half_duplex(dev);
	if (ret < 0) {
		printf("eqos_set_*_duplex() failed: %d", ret);
		return ret;
	}

	switch (eqos->phydev->speed) {
	case SPEED_1000:
		ret = eqos_set_gmii_speed(dev);
		break;
	case SPEED_100:
		ret = eqos_set_mii_speed_100(dev);
		break;
	case SPEED_10:
		ret = eqos_set_mii_speed_10(dev);
		break;
	default:
		printf("invalid speed %d", eqos->phydev->speed);
		return -EINVAL;
	}
	if (ret < 0) {
		printf("eqos_set_*mii_speed*() failed: %d", ret);
		return ret;
	}

	return 0;
}

static int eqos_write_hwaddr(struct udevice *dev)
{
	struct eth_pdata *plat = dev_get_platdata(dev);
	struct eqos_eth_dev *eqos = dev_get_priv(dev);
	u32 macid_lo, macid_hi;
	u8 *mac_id = &plat->enetaddr[0];

	macid_lo = mac_id[0] + (mac_id[1] << 8) + (mac_id[2] << 16) +
		   (mac_id[3] << 24);
	macid_hi = mac_id[4] + (mac_id[5] << 8);

	writel(macid_hi, &eqos->mac_regs_p->address0_high);
	writel(macid_lo, &eqos->mac_regs_p->address0_low);

	return 0;
}

static void eqos_stop(struct udevice *dev)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);
	int i;

	debug("%s(dev=%p):\n", __func__, dev);

	/* Disable TX DMA */
	clrbits_le32(&eqos->dma_regs_p->ch0_tx_control,
		     EQOS_DMA_CH0_TX_CONTROL_ST);

	/* Wait for TX all packets to drain out of MTL */
	for (i = 0; i < 1000000; i++) {
		u32 val = readl(&eqos->mtl_regs_p->txq0_debug);
		u32 trcsts = (val >> EQOS_MTL_TXQ0_DEBUG_TRCSTS_SHIFT) &
			EQOS_MTL_TXQ0_DEBUG_TRCSTS_MASK;
		u32 txqsts = val & EQOS_MTL_TXQ0_DEBUG_TXQSTS;
		if ((trcsts != 1) && (!txqsts))
			break;
	}

	/* Turn off MAC TX and RX */
	clrbits_le32(&eqos->mac_regs_p->configuration,
		     EQOS_MAC_CONFIGURATION_TE | EQOS_MAC_CONFIGURATION_RE);

	/* Wait for all RX packets to drain out of MTL */
	for (i = 0; i < 1000000; i++) {
		u32 val = readl(&eqos->mtl_regs_p->rxq0_debug);
		u32 prxq = (val >> EQOS_MTL_RXQ0_DEBUG_PRXQ_SHIFT) &
			EQOS_MTL_RXQ0_DEBUG_PRXQ_MASK;
		u32 rxqsts = (val >> EQOS_MTL_RXQ0_DEBUG_RXQSTS_SHIFT) &
			EQOS_MTL_RXQ0_DEBUG_RXQSTS_MASK;
		if ((!prxq) && (!rxqsts))
			break;
	}

	/* Turn off RX DMA */
	clrbits_le32(&eqos->dma_regs_p->ch0_rx_control,
		     EQOS_DMA_CH0_RX_CONTROL_SR);

	if (eqos->phydev) {
		phy_shutdown(eqos->phydev);
	}

	debug("%s: OK\n", __func__);
}


static int eqos_start(struct udevice *dev)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);
	int ret, i;
	u32 val, tx_fifo_sz, rx_fifo_sz, tqs, rqs, pbl;
	ulong last_rx_desc;

	debug("%s(dev=%p):\n", __func__, dev);

	eqos->tx_desc_idx = 0;
	eqos->rx_desc_idx = 0;

	setbits_le32(&eqos->dma_regs_p->mode,
		     EQOS_DMA_MODE_SWR);

	ret = -1;

	ret = phy_startup(eqos->phydev);
	if (ret < 0) {
		printf("phy_startup() failed: %d", ret);
		goto err_shutdown_phy;
	}
	if (!eqos->phydev->link) {
		printf("No link");
		goto err_shutdown_phy;
	}

	ret = eqos_adjust_link(dev);
	if (ret < 0) {
		printf("eqos_adjust_link() failed: %d", ret);
		goto err_shutdown_phy;
	}

	/* Configure MTL */

	/* Enable Store and Forward mode for TX */
	/* Program Tx operating mode */
	setbits_le32(&eqos->mtl_regs_p->txq0_operation_mode,
		     EQOS_MTL_TXQ0_OPERATION_MODE_TSF |
		     (EQOS_MTL_TXQ0_OPERATION_MODE_TXQEN_ENABLED <<
		      EQOS_MTL_TXQ0_OPERATION_MODE_TXQEN_SHIFT));

	/* Transmit Queue weight */
	writel(0x10, &eqos->mtl_regs_p->txq0_quantum_weight);

	/* Enable Store and Forward mode for RX, since no jumbo frame */
	setbits_le32(&eqos->mtl_regs_p->rxq0_operation_mode,
		     EQOS_MTL_RXQ0_OPERATION_MODE_RSF);

	/* Transmit/Receive queue fifo size; use all RAM for 1 queue */
	val = readl(&eqos->mac_regs_p->hw_feature1);
	tx_fifo_sz = (val >> EQOS_MAC_HW_FEATURE1_TXFIFOSIZE_SHIFT) &
		EQOS_MAC_HW_FEATURE1_TXFIFOSIZE_MASK;
	rx_fifo_sz = (val >> EQOS_MAC_HW_FEATURE1_RXFIFOSIZE_SHIFT) &
		EQOS_MAC_HW_FEATURE1_RXFIFOSIZE_MASK;

	/*
	 * r/tx_fifo_sz is encoded as log2(n / 128). Undo that by shifting.
	 * r/tqs is encoded as (n / 256) - 1.
	 */
	tqs = (128 << tx_fifo_sz) / 256 - 1;
	rqs = (128 << rx_fifo_sz) / 256 - 1;

	clrsetbits_le32(&eqos->mtl_regs_p->txq0_operation_mode,
			EQOS_MTL_TXQ0_OPERATION_MODE_TQS_MASK <<
			EQOS_MTL_TXQ0_OPERATION_MODE_TQS_SHIFT,
			tqs << EQOS_MTL_TXQ0_OPERATION_MODE_TQS_SHIFT);
	clrsetbits_le32(&eqos->mtl_regs_p->rxq0_operation_mode,
			EQOS_MTL_RXQ0_OPERATION_MODE_RQS_MASK <<
			EQOS_MTL_RXQ0_OPERATION_MODE_RQS_SHIFT,
			rqs << EQOS_MTL_RXQ0_OPERATION_MODE_RQS_SHIFT);

	/* Flow control used only if each channel gets 4KB or more FIFO */
	if (rqs >= ((4096 / 256) - 1)) {
		u32 rfd, rfa;

		setbits_le32(&eqos->mtl_regs_p->rxq0_operation_mode,
			     EQOS_MTL_RXQ0_OPERATION_MODE_EHFC);

		/*
		 * Set Threshold for Activating Flow Contol space for min 2
		 * frames ie, (1500 * 1) = 1500 bytes.
		 *
		 * Set Threshold for Deactivating Flow Contol for space of
		 * min 1 frame (frame size 1500bytes) in receive fifo
		 */
		if (rqs == ((4096 / 256) - 1)) {
			/*
			 * This violates the above formula because of FIFO size
			 * limit therefore overflow may occur inspite of this.
			 */
			rfd = 0x3;	/* Full-3K */
			rfa = 0x1;	/* Full-1.5K */
		} else if (rqs == ((8192 / 256) - 1)) {
			rfd = 0x6;	/* Full-4K */
			rfa = 0xa;	/* Full-6K */
		} else if (rqs == ((16384 / 256) - 1)) {
			rfd = 0x6;	/* Full-4K */
			rfa = 0x12;	/* Full-10K */
		} else {
			rfd = 0x6;	/* Full-4K */
			rfa = 0x1E;	/* Full-16K */
		}

		clrsetbits_le32(&eqos->mtl_regs_p->rxq0_operation_mode,
				(EQOS_MTL_RXQ0_OPERATION_MODE_RFD_MASK <<
				 EQOS_MTL_RXQ0_OPERATION_MODE_RFD_SHIFT) |
				(EQOS_MTL_RXQ0_OPERATION_MODE_RFA_MASK <<
				 EQOS_MTL_RXQ0_OPERATION_MODE_RFA_SHIFT),
				(rfd <<
				 EQOS_MTL_RXQ0_OPERATION_MODE_RFD_SHIFT) |
				(rfa <<
				 EQOS_MTL_RXQ0_OPERATION_MODE_RFA_SHIFT));
	}

	/* Configure MAC */

	clrsetbits_le32(&eqos->mac_regs_p->rxq_ctrl0,
			EQOS_MAC_RXQ_CTRL0_RXQ0EN_MASK <<
			EQOS_MAC_RXQ_CTRL0_RXQ0EN_SHIFT,
			EQOS_MAC_RXQ_CTRL0_RXQ0EN_ENABLED_DCB <<
			EQOS_MAC_RXQ_CTRL0_RXQ0EN_SHIFT);

	/* Set TX flow control parameters */
	/* Set Pause Time */
	setbits_le32(&eqos->mac_regs_p->q0_tx_flow_ctrl,
		     0xffff << EQOS_MAC_Q0_TX_FLOW_CTRL_PT_SHIFT);
	/* Assign priority for TX flow control */
	clrbits_le32(&eqos->mac_regs_p->txq_prty_map0,
		     EQOS_MAC_TXQ_PRTY_MAP0_PSTQ0_MASK <<
		     EQOS_MAC_TXQ_PRTY_MAP0_PSTQ0_SHIFT);
	/* Assign priority for RX flow control */
	clrbits_le32(&eqos->mac_regs_p->rxq_ctrl2,
		     EQOS_MAC_RXQ_CTRL2_PSRQ0_MASK <<
		     EQOS_MAC_RXQ_CTRL2_PSRQ0_SHIFT);
	/* Enable flow control */
	setbits_le32(&eqos->mac_regs_p->q0_tx_flow_ctrl,
		     EQOS_MAC_Q0_TX_FLOW_CTRL_TFE);
	setbits_le32(&eqos->mac_regs_p->rx_flow_ctrl,
		     EQOS_MAC_RX_FLOW_CTRL_RFE);

	clrsetbits_le32(&eqos->mac_regs_p->configuration,
			EQOS_MAC_CONFIGURATION_GPSLCE |
			EQOS_MAC_CONFIGURATION_WD |
			EQOS_MAC_CONFIGURATION_JD |
			EQOS_MAC_CONFIGURATION_JE,
			EQOS_MAC_CONFIGURATION_CST |
			EQOS_MAC_CONFIGURATION_ACS);

	eqos_write_hwaddr(dev);

	/* Configure DMA */

	/* Enable OSP mode */
	setbits_le32(&eqos->dma_regs_p->ch0_tx_control,
		     EQOS_DMA_CH0_TX_CONTROL_OSP);

	/* RX buffer size. Must be a multiple of bus width */
	clrsetbits_le32(&eqos->dma_regs_p->ch0_rx_control,
			EQOS_DMA_CH0_RX_CONTROL_RBSZ_MASK <<
			EQOS_DMA_CH0_RX_CONTROL_RBSZ_SHIFT,
			EQOS_MAX_PACKET_SIZE <<
			EQOS_DMA_CH0_RX_CONTROL_RBSZ_SHIFT);

	setbits_le32(&eqos->dma_regs_p->ch0_control,
		     EQOS_DMA_CH0_CONTROL_PBLX8);

	/*
	 * Burst length must be < 1/2 FIFO size.
	 * FIFO size in tqs is encoded as (n / 256) - 1.
	 * Each burst is n * 8 (PBLX8) * 16 (AXI width) == 128 bytes.
	 * Half of n * 256 is n * 128, so pbl == tqs, modulo the -1.
	 */
	pbl = tqs + 1;
	if (pbl > 32)
		pbl = 32;
	clrsetbits_le32(&eqos->dma_regs_p->ch0_tx_control,
			EQOS_DMA_CH0_TX_CONTROL_TXPBL_MASK <<
			EQOS_DMA_CH0_TX_CONTROL_TXPBL_SHIFT,
			pbl << EQOS_DMA_CH0_TX_CONTROL_TXPBL_SHIFT);

	clrsetbits_le32(&eqos->dma_regs_p->ch0_rx_control,
			EQOS_DMA_CH0_RX_CONTROL_RXPBL_MASK <<
			EQOS_DMA_CH0_RX_CONTROL_RXPBL_SHIFT,
			8 << EQOS_DMA_CH0_RX_CONTROL_RXPBL_SHIFT);

	/* DMA performance configuration */
	val = (2 << EQOS_DMA_SYSBUS_MODE_RD_OSR_LMT_SHIFT) |
		EQOS_DMA_SYSBUS_MODE_EAME | EQOS_DMA_SYSBUS_MODE_BLEN16 |
		EQOS_DMA_SYSBUS_MODE_BLEN8 | EQOS_DMA_SYSBUS_MODE_BLEN4;
	writel(val, &eqos->dma_regs_p->sysbus_mode);

	/* Set up descriptors */

	memset(eqos->descs, 0, EQOS_DESCRIPTORS_SIZE);

	for (i = 0; i < EQOS_DESCRIPTORS_RX; i++) {
		struct eqos_desc *rx_desc = &(eqos->rx_descs[i]);
		rx_desc->des0 = (u32)(ulong)(eqos->rx_dma_buf +
					     (i * EQOS_MAX_PACKET_SIZE));
		rx_desc->des3 |= EQOS_DESC3_OWN | EQOS_DESC3_BUF1V;
	}

	flush_dcache_range((phys_addr_t)eqos->descs, (phys_addr_t)((unsigned char *)eqos->descs + EQOS_DESCRIPTORS_SIZE));
	flush_dcache_range((phys_addr_t)eqos->rx_dma_buf, (phys_addr_t)((unsigned char *)eqos->rx_dma_buf + EQOS_DESCRIPTORS_RX*EQOS_MAX_PACKET_SIZE));

	writel(0, &eqos->dma_regs_p->ch0_txdesc_list_haddress);
	writel((ulong)eqos->tx_descs, &eqos->dma_regs_p->ch0_txdesc_list_address);
	writel(EQOS_DESCRIPTORS_TX - 1,
	       &eqos->dma_regs_p->ch0_txdesc_ring_length);

	writel(0, &eqos->dma_regs_p->ch0_rxdesc_list_haddress);
	writel((ulong)eqos->rx_descs, &eqos->dma_regs_p->ch0_rxdesc_list_address);
	writel(EQOS_DESCRIPTORS_RX - 1,
	       &eqos->dma_regs_p->ch0_rxdesc_ring_length);

	/* Enable everything */

	setbits_le32(&eqos->mac_regs_p->configuration,
		     EQOS_MAC_CONFIGURATION_TE | EQOS_MAC_CONFIGURATION_RE);

	setbits_le32(&eqos->dma_regs_p->ch0_tx_control,
		     EQOS_DMA_CH0_TX_CONTROL_ST);
	setbits_le32(&eqos->dma_regs_p->ch0_rx_control,
		     EQOS_DMA_CH0_RX_CONTROL_SR);

	/* TX tail pointer not written until we need to TX a packet */
	/*
	 * Point RX tail pointer at last descriptor. Ideally, we'd point at the
	 * first descriptor, implying all descriptors were available. However,
	 * that's not distinguishable from none of the descriptors being
	 * available.
	 */
	last_rx_desc = (ulong)&(eqos->rx_descs[(EQOS_DESCRIPTORS_RX - 1)]);
	writel(last_rx_desc, &eqos->dma_regs_p->ch0_rxdesc_tail_pointer);

	return 0;

err_shutdown_phy:
	phy_shutdown(eqos->phydev);
//	eqos->phydev = NULL;
	printf("eth init FAILED\n");
	return ret;
}

static int eqos_send(struct udevice *dev, void *packet, int length)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);
	struct eqos_desc *tx_desc;
	int i;

	debug("%s(dev=%p, packet=%p, length=%d):\n", __func__, dev, packet,
	      length);

	memcpy(eqos->tx_dma_buf, packet, length);
	eqos_flush_buffer(eqos->tx_dma_buf, length);

	tx_desc = &(eqos->tx_descs[eqos->tx_desc_idx]);
//	eqos_inval_desc(tx_desc);
	eqos->tx_desc_idx++;
	eqos->tx_desc_idx %= EQOS_DESCRIPTORS_TX;

	tx_desc->des0 = (ulong)eqos->tx_dma_buf;
	tx_desc->des1 = 0;
	tx_desc->des2 = length;
	/*
	 * Make sure that if HW sees the _OWN write below, it will see all the
	 * writes to the rest of the descriptor too.
	 */
	mb();
	tx_desc->des3 = EQOS_DESC3_OWN | EQOS_DESC3_FD | EQOS_DESC3_LD | length;
	eqos_flush_desc(tx_desc);

	writel((ulong)(tx_desc + 1), &eqos->dma_regs_p->ch0_txdesc_tail_pointer);

	for (i = 0; i < 1000000; i++) {
		eqos_inval_desc(tx_desc);
		if (!(readl(&tx_desc->des3) & EQOS_DESC3_OWN)) {
			return 0;
		}
		udelay(1);
	}

	printf("%s: TX timeout\n", __func__);

	return -ETIMEDOUT;
}

static int eqos_recv(struct udevice *dev, int flags, uchar **packetp)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);
	struct eqos_desc *rx_desc;
	int length;

	debug("%s(dev=%p):\n", __func__, dev);

	rx_desc = &(eqos->rx_descs[eqos->rx_desc_idx]);
//	eqos_inval_desc(rx_desc);
	if (rx_desc->des3 & EQOS_DESC3_OWN) {
		debug("%s: RX packet not available\n", __func__);
		return -EAGAIN;
	}

	*packetp = eqos->rx_dma_buf +
		(eqos->rx_desc_idx * EQOS_MAX_PACKET_SIZE);
	length = rx_desc->des3 & 0x7fff;
	debug("%s: *packetp=%p, length=%d\n", __func__, *packetp, length);

	eqos_inval_buffer(*packetp, length);

	return length;
}

int eqos_free_pkt(struct udevice *dev, uchar *packet, int length)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);
	uchar *packet_expected;
	struct eqos_desc *rx_desc;

	debug("%s(packet=%p, length=%d)\n", __func__, packet, length);

	packet_expected = eqos->rx_dma_buf +
		(eqos->rx_desc_idx * EQOS_MAX_PACKET_SIZE);
	if (packet != packet_expected) {
		debug("%s: Unexpected packet (expected %p)\n", __func__,
		      packet_expected);
		return -EINVAL;
	}

	rx_desc = &(eqos->rx_descs[eqos->rx_desc_idx]);
	rx_desc->des0 = (u32)(ulong)packet;
	rx_desc->des1 = 0;
	rx_desc->des2 = 0;
	/*
	 * Make sure that if HW sees the _OWN write below, it will see all the
	 * writes to the rest of the descriptor too.
	 */
	mb();
	rx_desc->des3 |= EQOS_DESC3_OWN | EQOS_DESC3_BUF1V;
	eqos_flush_desc(rx_desc);

	writel((ulong)rx_desc, &eqos->dma_regs_p->ch0_rxdesc_tail_pointer);

	eqos->rx_desc_idx++;
	eqos->rx_desc_idx %= EQOS_DESCRIPTORS_RX;

	return 0;
}

static int eqos_probe_resources_core(struct udevice * dev)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);
	int ret;

	debug("%s(dev=%p):\n", __func__, dev);

	eqos->descs = eqos_alloc_descs(EQOS_DESCRIPTORS_TX +
				       EQOS_DESCRIPTORS_RX);
	if (!eqos->descs) {
		debug("%s: eqos_alloc_descs() failed\n", __func__);
		ret = -ENOMEM;
		goto err;
	}
	eqos->tx_descs = (struct eqos_desc *)eqos->descs;
	eqos->rx_descs = (eqos->tx_descs + EQOS_DESCRIPTORS_TX);
	debug("%s: tx_descs=%p, rx_descs=%p\n", __func__, eqos->tx_descs,
	      eqos->rx_descs);

	eqos->tx_dma_buf = memalign(EQOS_BUFFER_ALIGN, EQOS_MAX_PACKET_SIZE);
	if (!eqos->tx_dma_buf) {
		debug("%s: memalign(tx_dma_buf) failed\n", __func__);
		ret = -ENOMEM;
		goto err_free_descs;
	}
	debug("%s: rx_dma_buf=%p\n", __func__, eqos->rx_dma_buf);

	eqos->rx_dma_buf = memalign(EQOS_BUFFER_ALIGN, EQOS_RX_BUFFER_SIZE);
	if (!eqos->rx_dma_buf) {
		debug("%s: memalign(rx_dma_buf) failed\n", __func__);
		ret = -ENOMEM;
		goto err_free_tx_dma_buf;
	}
	debug("%s: tx_dma_buf=%p\n", __func__, eqos->tx_dma_buf);

	eqos->rx_pkt = malloc(EQOS_MAX_PACKET_SIZE);
	if (!eqos->rx_pkt) {
		debug("%s: malloc(rx_pkt) failed\n", __func__);
		ret = -ENOMEM;
		goto err_free_rx_dma_buf;
	}
	debug("%s: rx_pkt=%p\n", __func__, eqos->rx_pkt);

	debug("%s: OK\n", __func__);
	return 0;

err_free_rx_dma_buf:
	free(eqos->rx_dma_buf);
err_free_tx_dma_buf:
	free(eqos->tx_dma_buf);
err_free_descs:
	eqos_free_descs(eqos->descs);
err:

	debug("%s: returns %d\n", __func__, ret);
	return ret;
}

static int eqos_remove_resources_core(struct udevice *dev)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);

	free(eqos->rx_pkt);
	free(eqos->rx_dma_buf);
	free(eqos->tx_dma_buf);
	eqos_free_descs(eqos->descs);

	debug("%s: OK\n", __func__);
	return 0;
}

static int eqos_phy_init(struct udevice *dev)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);
	struct phy_device *phydev;
	int mask = 0xffffffff;

#ifdef CONFIG_PHY_ADDR
	mask = 1 << CONFIG_PHY_ADDR;
#endif

	phydev = phy_find_by_mask(eqos->bus, mask, eqos->interface);
	if (!phydev)
		return -1;

	phy_connect_dev(phydev, dev);

	phydev->supported &= PHY_GBIT_FEATURES;
	phydev->advertising = phydev->supported;

	eqos->phydev = phydev;
	p_phydev = phydev;
	phy_config(phydev);

	return 0;
}

// from eqos_initialize(ulong base_addr, u32 interface)
int eqos_probe(struct udevice *dev)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);
	ulong base_addr;
	int ret;

#ifdef CONFIG_DM_ETH
	printf("pwr up eth\n");
	pwr_ctrl_psci_smc(PM_ETH, 1);
	DM_network_interface_setup(dev);
#endif

	eqos->dev = dev;

	base_addr = devfdt_get_addr(dev);
	if (base_addr == FDT_ADDR_T_NONE) {
		pr_err("devfdt_get_addr() failed");
		return -ENODEV;
	}

	eqos->mac_regs_p = (void *)base_addr;
	eqos->mtl_regs_p = (void *)(base_addr + EQOS_MTL_REGS_BASE);
	eqos->dma_regs_p = (void *)(base_addr + EQOS_DMA_REGS_BASE);

	ret = eqos_probe_resources_core(dev);

	eqos->interface = 0;

	eqos_mdio_init((char *)dev->name, eqos->mac_regs_p);
	eqos->bus = miiphy_get_dev_by_name(dev->name);
	ret = eqos_phy_init(dev);
	if (ret < 0) {
		pr_err("eqos_phy_init() failed");
		return -ENODEV;
	}

	return 0;
}

static int eqos_remove(struct udevice *dev)
{
	struct eqos_eth_dev *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);

	free(eqos->phydev);
	mdio_unregister(eqos->bus);
	mdio_free(eqos->bus);
	eqos_remove_resources_core(dev);

	debug("%s: OK\n", __func__);
	return 0;
}

/* amlogic debug cmd start */
/*********************ethernet debug function****************************/
static int do_phyreg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int reg, value;
	unsigned char *cmd = NULL;
	unsigned int i;

	if (argc < 2) {
		return cmd_usage(cmdtp);
	}

	if (p_phydev != NULL)
		priv_tool->phydev = p_phydev;
//	if (priv_tool == NULL || priv_tool->phydev == NULL) {
//		return -1;
//	}

	cmd = (unsigned char *)argv[1];
	switch (*cmd) {
		case 'd':
			printf("=== ethernet phy register dump:\n");
			for (i = 0; i < 32; i++)
				printf("[reg_%d] 0x%x\n", i, phy_read(priv_tool->phydev, MDIO_DEVAD_NONE, i));
			break;
		case 'r':
			if (argc != 3) {
				return cmd_usage(cmdtp);
			}
			printf("=== ethernet phy register read:\n");
			reg = simple_strtoul(argv[2], NULL, 10);
			printf("[reg_%d] 0x%x\n", reg, phy_read(priv_tool->phydev, MDIO_DEVAD_NONE, reg));

			break;
		case 'w':
			if (argc != 4) {
				return cmd_usage(cmdtp);
			}
			printf("=== ethernet phy register write:\n");
			reg = simple_strtoul(argv[2], NULL, 10);
			value = simple_strtoul(argv[3], NULL, 16);
			phy_write(priv_tool->phydev, MDIO_DEVAD_NONE, reg, value);
			printf("[reg_%d] 0x%x\n", reg, phy_read(priv_tool->phydev, MDIO_DEVAD_NONE, reg));
			break;

		default:
			return cmd_usage(cmdtp);
	}

	return 0;
}

U_BOOT_CMD(
		phyreg, 4, 1, do_phyreg,
		"ethernet phy register read/write/dump",
		"d            - dump phy registers\n"
		"       r reg        - read phy register\n"
		"       w reg val    - write phy register"
);

const struct eth_ops designware_eth_ops = {
	.start			= designware_eth_start,
	.send			= designware_eth_send,
	.recv			= designware_eth_recv,
	.free_pkt		= designware_eth_free_pkt,
	.stop			= designware_eth_stop,
	.write_hwaddr		= designware_eth_write_hwaddr,
};

const struct eth_ops eqos_ops = {
	.start			= eqos_start,
	.send			= eqos_send,
	.recv			= eqos_recv,
	.free_pkt		= eqos_free_pkt,
	.stop			= eqos_stop,
	.write_hwaddr		= eqos_write_hwaddr,
};

int designware_eth_ofdata_to_platdata(struct udevice *dev)
{
	struct dw_eth_pdata *dw_pdata = dev_get_platdata(dev);
#ifdef CONFIG_DM_GPIO
	struct dw_eth_dev *priv = dev_get_priv(dev);
#endif
	struct eth_pdata *pdata = &dw_pdata->eth_pdata;
	const char *phy_mode;
#ifdef CONFIG_DM_GPIO
	int reset_flags = GPIOD_IS_OUT;
#endif
	int ret = 0;

	pdata->iobase = dev_read_addr(dev);
	pdata->phy_interface = -1;
	phy_mode = dev_read_string(dev, "phy-mode");
	if (phy_mode)
		pdata->phy_interface = phy_get_interface_by_name(phy_mode);
	if (pdata->phy_interface == -1) {
		debug("%s: Invalid PHY interface '%s'\n", __func__, phy_mode);
		return -EINVAL;
	}

	pdata->max_speed = dev_read_u32_default(dev, "max-speed", 0);

#ifdef CONFIG_DM_GPIO
	if (dev_read_bool(dev, "snps,reset-active-low"))
		reset_flags |= GPIOD_ACTIVE_LOW;

	ret = gpio_request_by_name(dev, "snps,reset-gpio", 0,
		&priv->reset_gpio, reset_flags);
	if (ret == 0) {
		ret = dev_read_u32_array(dev, "snps,reset-delays-us",
					 dw_pdata->reset_delays, 3);
	} else if (ret == -ENOENT) {
		ret = 0;
	}
#endif

	return ret;
}

static const struct udevice_id designware_eth_ids[] = {
	{ .compatible = "allwinner,sun7i-a20-gmac" },
	{ .compatible = "altr,socfpga-stmmac" },
	{ .compatible = "amlogic,meson6-dwmac" },
	{ .compatible = "amlogic,g12a-eth-dwmac" },
	{ .compatible = "amlogic,meson-gx-dwmac" },
	{ .compatible = "amlogic,meson-gxbb-dwmac" },
	{ .compatible = "amlogic,meson-axg-dwmac" },
	{ .compatible = "st,stm32-dwmac" },
	{ }
};

U_BOOT_DRIVER(eth_designware) = {
	.name	= "eth_designware",
	.id	= UCLASS_ETH,
	.of_match = designware_eth_ids,
	.ofdata_to_platdata = designware_eth_ofdata_to_platdata,
	.bind	= designware_eth_bind,
	.probe	= designware_eth_probe,
	.remove	= designware_eth_remove,
	.ops	= &designware_eth_ops,
	.priv_auto_alloc_size = sizeof(struct dw_eth_dev),
	.platdata_auto_alloc_size = sizeof(struct dw_eth_pdata),
	.flags = DM_FLAG_ALLOC_PRIV_DMA,
};

static const struct udevice_id eth_qos_ids[] = {
	{ .compatible = "amlogic,t7-eth-dwmac" },
	{ }
};

U_BOOT_DRIVER(eth_qos) = {
	.name = "eth_qos",
	.id = UCLASS_ETH,
	.of_match = eth_qos_ids,
	.probe = eqos_probe,
	.remove = eqos_remove,
	.ops = &eqos_ops,
	.priv_auto_alloc_size = sizeof(struct eqos_eth_dev),
	.platdata_auto_alloc_size = sizeof(struct eth_pdata),
};

static struct pci_device_id supported[] = {
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_QRK_EMAC) },
	{ }
};

U_BOOT_PCI_DEVICE(eth_designware, supported);
#endif
