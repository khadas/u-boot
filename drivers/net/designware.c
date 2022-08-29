/*
 * (C) Copyright 2010
 * Vipin Kumar, ST Micoelectronics, vipin.kumar@st.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Designware ethernet IP driver for u-boot
 */

#include <common.h>
#include <miiphy.h>
#include <malloc.h>
#include <linux/compiler.h>
#include <linux/err.h>
#include <asm/io.h>
#include <asm/arch/io.h>
#include "designware.h"
#include <asm/arch/secure_apb.h>
#include <amlogic/keyunify.h>
#include <version.h>

#if !defined(CONFIG_PHYLIB)
# error "DesignWare Ether MAC requires PHYLIB - missing CONFIG_PHYLIB"
#endif

struct eth_device *dev = NULL;
struct dw_eth_dev *priv = NULL;
struct eqos_eth_dev *eqos_priv = NULL;
struct phy_device * p_phydev=NULL;
int do_cali_process = 0;
int do_cali_timeout = 0;

static int dw_mdio_read(struct mii_dev *bus, int addr, int devad, int reg)
{
	struct eth_mac_regs *mac_p = bus->priv;
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

	return -1;
}

static int dw_mdio_write(struct mii_dev *bus, int addr, int devad, int reg,
			u16 val)
{
	struct eth_mac_regs *mac_p = bus->priv;
	ulong start;
	u16 miiaddr;
	int ret = -1, timeout = CONFIG_MDIO_TIMEOUT;

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

static int dw_mdio_init(char *name, struct eth_mac_regs *mac_regs_p)
{
	struct mii_dev *bus = mdio_alloc();

	if (!bus) {
		printf("Failed to allocate MDIO bus\n");
		return -1;
	}

	bus->read = dw_mdio_read;
	bus->write = dw_mdio_write;
	sprintf(bus->name, name);

	bus->priv = (void *)mac_regs_p;

	return mdio_register(bus);
}

static void tx_descs_init(struct eth_device *dev)
{
	struct dw_eth_dev *priv = dev->priv;
	struct eth_dma_regs *dma_p = priv->dma_regs_p;
	struct dmamacdescr *desc_table_p = &priv->tx_mac_descrtable[0];
	char *txbuffs = &priv->txbuffs[0];
	struct dmamacdescr *desc_p;
	u32 idx;

	for (idx = 0; idx < CONFIG_TX_DESCR_NUM; idx++) {
		desc_p = &desc_table_p[idx];
		desc_p->dmamac_addr = (u32)(phys_addr_t)&txbuffs[idx * CONFIG_ETH_BUFSIZE];
		desc_p->dmamac_next = (u32)(phys_addr_t)&desc_table_p[idx + 1];

#if defined(CONFIG_DW_ALTDESCRIPTOR)
		desc_p->txrx_status &= ~(DESC_TXSTS_TXINT | DESC_TXSTS_TXLAST |
				DESC_TXSTS_TXFIRST | DESC_TXSTS_TXCRCDIS | \
				DESC_TXSTS_TXCHECKINSCTRL | \
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
	desc_p->dmamac_next = (u32)(phys_addr_t)&desc_table_p[0];

	/* Flush all Tx buffer descriptors at once */
	flush_dcache_range((phys_addr_t)priv->tx_mac_descrtable,
			   (phys_addr_t)priv->tx_mac_descrtable +
			   sizeof(priv->tx_mac_descrtable));

	writel((ulong)&desc_table_p[0], &dma_p->txdesclistaddr);
	priv->tx_currdescnum = 0;
}

static void rx_descs_init(struct eth_device *dev)
{
	struct dw_eth_dev *priv = dev->priv;
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
	flush_dcache_range((phys_addr_t)rxbuffs, (phys_addr_t)rxbuffs +
			   RX_TOTAL_BUFSIZE);

	for (idx = 0; idx < CONFIG_RX_DESCR_NUM; idx++) {
		desc_p = &desc_table_p[idx];
		desc_p->dmamac_addr = (u32)(phys_addr_t)&rxbuffs[idx * CONFIG_ETH_BUFSIZE];
		desc_p->dmamac_next = (u32)(phys_addr_t)&desc_table_p[idx + 1];

		desc_p->dmamac_cntl =
			(MAC_MAX_FRAME_SZ & DESC_RXCTRL_SIZE1MASK) | \
				      DESC_RXCTRL_RXCHAIN;

		desc_p->txrx_status = DESC_RXSTS_OWNBYDMA;
	}

	/* Correcting the last pointer of the chain */
	desc_p->dmamac_next = (u32)(phys_addr_t)&desc_table_p[0];

	/* Flush all Rx buffer descriptors at once */
	flush_dcache_range((phys_addr_t)priv->rx_mac_descrtable,
			   (phys_addr_t)priv->rx_mac_descrtable +
			   sizeof(priv->rx_mac_descrtable));

	writel((ulong)&desc_table_p[0], &dma_p->rxdesclistaddr);
	priv->rx_currdescnum = 0;
}

static int dw_write_hwaddr(struct eth_device *dev)
{
	struct dw_eth_dev *priv = dev->priv;
	struct eth_mac_regs *mac_p = priv->mac_regs_p;
	u32 macid_lo, macid_hi;
	u8 *mac_id = &dev->enetaddr[0];

	macid_lo = mac_id[0] + (mac_id[1] << 8) + (mac_id[2] << 16) +
		   (mac_id[3] << 24);
	macid_hi = mac_id[4] + (mac_id[5] << 8);

	writel(macid_hi, &mac_p->macaddr0hi);
	writel(macid_lo, &mac_p->macaddr0lo);

	return 0;
}

static void dw_adjust_link(struct eth_mac_regs *mac_p,
			   struct phy_device *phydev)
{
	u32 conf = readl(&mac_p->conf) | FRAMEBURSTENABLE | DISABLERXOWN;

	if (!phydev->link) {
		printf("%s: No link.\n", phydev->dev->name);
		return;
	}

	if (phydev->speed != 1000)
		conf |= MII_PORTSELECT;

	if (phydev->speed == 100)
		conf |= FES_100;

	if (phydev->duplex)
		conf |= FULLDPLXMODE;

	writel(conf, &mac_p->conf);

	if (!do_cali_process) {
		printf("Speed: %d, %s duplex%s\n", phydev->speed,
			(phydev->duplex) ? "full" : "half",
			(phydev->port == PORT_FIBRE) ? ", fiber mode" : "");
	}
}

static void dw_eth_halt(struct eth_device *dev)
{
	struct dw_eth_dev *priv = dev->priv;
	struct eth_mac_regs *mac_p = priv->mac_regs_p;
	struct eth_dma_regs *dma_p = priv->dma_regs_p;

	writel(readl(&mac_p->conf) & ~(RXENABLE | TXENABLE), &mac_p->conf);
	writel(readl(&dma_p->opmode) & ~(RXSTART | TXSTART), &dma_p->opmode);

	phy_shutdown(priv->phydev);
}

static int dw_eth_init(struct eth_device *dev, bd_t *bis)
{
	struct dw_eth_dev *priv = dev->priv;
	struct eth_mac_regs *mac_p = priv->mac_regs_p;
	struct eth_dma_regs *dma_p = priv->dma_regs_p;
	unsigned int start;

	writel(readl(&dma_p->busmode) | DMAMAC_SRST, &dma_p->busmode);

	start = get_timer(0);
	while (readl(&dma_p->busmode) & DMAMAC_SRST) {
		if (get_timer(start) >= CONFIG_MACRESET_TIMEOUT)
			return -1;
#ifdef CONFIG_PXP_EMULATOR
		udelay(100);
#else
		mdelay(100);
#endif

	};

	/* Soft reset above clears HW address registers.
	 * So we have to set it here once again */
	dw_write_hwaddr(dev);

	rx_descs_init(dev);
	tx_descs_init(dev);

	writel(FIXEDBURST | PRIORXTX_41 | DMA_PBL, &dma_p->busmode);

	writel(readl(&dma_p->opmode) | FLUSHTXFIFO | STOREFORWARD,
	       &dma_p->opmode);

	writel(readl(&dma_p->opmode) | RXSTART | TXSTART, &dma_p->opmode);

	/* Start up the PHY */
	if (phy_startup(priv->phydev)) {
		printf("Could not initialize PHY %s\n",
		       priv->phydev->dev->name);
		return -1;
	}
#ifdef CONFIG_PXP_EMULATOR
	priv->phydev->link = 1;
	priv->phydev->speed = 100;
	priv->phydev->duplex = 1;
#endif
	dw_adjust_link(mac_p, priv->phydev);

	if (!priv->phydev->link)
		return -1;

	writel(readl(&mac_p->conf) | RXENABLE | TXENABLE, &mac_p->conf);

	return 0;
}

static int dw_eth_send(struct eth_device *dev, void *packet, int length)
{
	struct dw_eth_dev *priv = dev->priv;
	struct eth_dma_regs *dma_p = priv->dma_regs_p;
	u32 desc_num = priv->tx_currdescnum;
	struct dmamacdescr *desc_p = &priv->tx_mac_descrtable[desc_num];
	phys_addr_t desc_start = (phys_addr_t)desc_p;
	phys_addr_t desc_end = desc_start +
		roundup(sizeof(*desc_p), ARCH_DMA_MINALIGN);
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
		return -1;
	}

	memcpy((void *)(phys_addr_t)desc_p->dmamac_addr, packet, length);
	/* Flush data to be sent */
	flush_dcache_range((phys_addr_t)priv->txbuffs, (phys_addr_t)priv->txbuffs + TX_TOTAL_BUFSIZE);

#if defined(CONFIG_DW_ALTDESCRIPTOR)
	desc_p->txrx_status |= DESC_TXSTS_TXFIRST | DESC_TXSTS_TXLAST;
	desc_p->dmamac_cntl &= (~DESC_TXCTRL_SIZE1MASK);
	desc_p->dmamac_cntl |= (length << DESC_TXCTRL_SIZE1SHFT) & \
			       DESC_TXCTRL_SIZE1MASK;

	desc_p->txrx_status &= ~(DESC_TXSTS_MSK);
	desc_p->txrx_status |= DESC_TXSTS_OWNBYDMA;
#else
	desc_p->dmamac_cntl &= (~DESC_TXCTRL_SIZE1MASK);
	desc_p->dmamac_cntl |= ((length << DESC_TXCTRL_SIZE1SHFT) & \
			       DESC_TXCTRL_SIZE1MASK) | DESC_TXCTRL_TXLAST | \
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

static int dw_eth_recv(struct eth_device *dev)
{
	struct dw_eth_dev *priv = dev->priv;
	u32 status, desc_num = priv->rx_currdescnum;
	struct dmamacdescr *desc_p = &priv->rx_mac_descrtable[desc_num];
	int length = 0;
	phys_addr_t desc_start = (phys_addr_t)desc_p;
	phys_addr_t desc_end = desc_start +
	roundup(sizeof(*desc_p), ARCH_DMA_MINALIGN);
	/* Invalidate entire buffer descriptor */
	invalidate_dcache_range(desc_start, desc_end);

	status = desc_p->txrx_status;

	/* Check  if the owner is the CPU */
	if (!(status & DESC_RXSTS_OWNBYDMA)) {
		length = (status & DESC_RXSTS_FRMLENMSK) >> \
			 DESC_RXSTS_FRMLENSHFT;

		/* Invalidate received data */
		invalidate_dcache_range((phys_addr_t)priv->rxbuffs, (phys_addr_t)priv->rxbuffs + RX_TOTAL_BUFSIZE);

		NetReceive((uchar *)(phys_addr_t)desc_p->dmamac_addr, length);
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
	}

	priv->rx_currdescnum = desc_num;

	return length;
}

static int dw_phy_init(struct eth_device *dev)
{
	struct dw_eth_dev *priv = dev->priv;
	struct phy_device *phydev;
	int mask = 0xffffffff;

#ifdef CONFIG_PHY_ADDR
	mask = 1 << CONFIG_PHY_ADDR;
#endif

	phydev = phy_find_by_mask(priv->bus, mask, priv->interface);
	if (!phydev)
		return -1;

	phy_connect_dev(phydev, dev);

	phydev->supported &= PHY_GBIT_FEATURES;
	phydev->advertising = phydev->supported;

	priv->phydev = phydev;
	p_phydev = phydev;
	phy_config(phydev);

	return 1;
}

static int dw_initialize(ulong base_addr, u32 interface)
{
	int ret;
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
	ret = dw_phy_init(dev);

	return ret;
}

/*for gmac4*/
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

static int eqos_mdio_init(char *name, void *mac_regs_p)
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

static int eqos_set_full_duplex(struct eth_device *dev)
{
	struct eqos_eth_dev *eqos = dev->priv;

	debug("%s(dev=%p):\n", __func__, dev);

	setbits_le32(&eqos->mac_regs_p->configuration, EQOS_MAC_CONFIGURATION_DM);

	return 0;
}

static int eqos_set_half_duplex(struct eth_device *dev)
{
	struct eqos_eth_dev *eqos = dev->priv;

	debug("%s(dev=%p):\n", __func__, dev);

	clrbits_le32(&eqos->mac_regs_p->configuration, EQOS_MAC_CONFIGURATION_DM);

	/* WAR: Flush TX queue when switching to half-duplex */
	setbits_le32(&eqos->mtl_regs_p->txq0_operation_mode,
		     EQOS_MTL_TXQ0_OPERATION_MODE_FTQ);

	return 0;
}

static int eqos_set_gmii_speed(struct eth_device *dev)
{
	struct eqos_eth_dev *eqos = dev->priv;

	debug("%s(dev=%p):\n", __func__, dev);

	clrbits_le32(&eqos->mac_regs_p->configuration,
		     EQOS_MAC_CONFIGURATION_PS | EQOS_MAC_CONFIGURATION_FES);

	return 0;
}

static int eqos_set_mii_speed_100(struct eth_device *dev)
{
	struct eqos_eth_dev *eqos = dev->priv;

	debug("%s(dev=%p):\n", __func__, dev);

	setbits_le32(&eqos->mac_regs_p->configuration,
		     EQOS_MAC_CONFIGURATION_PS | EQOS_MAC_CONFIGURATION_FES);

	return 0;
}

static int eqos_set_mii_speed_10(struct eth_device *dev)
{
	struct eqos_eth_dev *eqos = dev->priv;

	debug("%s(dev=%p):\n", __func__, dev);

	clrsetbits_le32(&eqos->mac_regs_p->configuration,
			EQOS_MAC_CONFIGURATION_FES, EQOS_MAC_CONFIGURATION_PS);

	return 0;
}

static int eqos_adjust_link(struct eth_device *dev)
{
	struct eqos_eth_dev *eqos = dev->priv;
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

static int eqos_write_hwaddr(struct eth_device *dev)
{
	struct eqos_eth_dev *eqos = dev->priv;
	u32 macid_lo, macid_hi;
	u8 *mac_id = &dev->enetaddr[0];

	macid_lo = mac_id[0] + (mac_id[1] << 8) + (mac_id[2] << 16) +
		   (mac_id[3] << 24);
	macid_hi = mac_id[4] + (mac_id[5] << 8);

	writel(macid_hi, &eqos->mac_regs_p->address0_high);
	writel(macid_lo, &eqos->mac_regs_p->address0_low);

	return 0;
}

static void eqos_eth_halt(struct eth_device *dev)
{
	struct eqos_eth_dev *eqos = dev->priv;
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

static int eqos_eth_init(struct eth_device *dev, bd_t *bis)
{
	struct eqos_eth_dev *eqos = dev->priv;
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
	eqos->phydev = NULL;
	printf("eth init FAILED\n");
	return ret;
}

static int eqos_eth_send(struct eth_device *dev, void *packet, int length)
{
	struct eqos_eth_dev *eqos = dev->priv;
	struct eqos_desc *tx_desc;
	int i;

	debug("%s(dev=%p, packet=%p, length=%d):\n", __func__, dev, packet,
	      length);

	memcpy(eqos->tx_dma_buf, packet, length);
	eqos_flush_buffer(eqos->tx_dma_buf, length);

	tx_desc = &(eqos->tx_descs[eqos->tx_desc_idx]);
	invalidate_dcache_all();
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

static int eqos_eth_recv(struct eth_device *dev)
{
	struct eqos_eth_dev *eqos = dev->priv;
	struct eqos_desc *rx_desc;
	uchar *packetp;
	int length;

	debug("%s(dev=%p):\n", __func__, dev);

	rx_desc = &(eqos->rx_descs[eqos->rx_desc_idx]);
	invalidate_dcache_all();
	if (rx_desc->des3 & EQOS_DESC3_OWN) {
		debug("%s: RX packet not available\n", __func__);
		return -EAGAIN;
	}

	packetp = eqos->rx_dma_buf +
		(eqos->rx_desc_idx * EQOS_MAX_PACKET_SIZE);
	length = rx_desc->des3 & 0x7fff;
	debug("%s: *packetp=%p, length=%d\n", __func__, packetp, length);

	eqos_inval_buffer(packetp, length);

	NetReceive(packetp, length);
	rx_desc->des3 |= EQOS_DESC3_OWN | EQOS_DESC3_BUF1V;
	writel((ulong)rx_desc, &eqos->dma_regs_p->ch0_rxdesc_tail_pointer);
	eqos_flush_desc(rx_desc);
	eqos->rx_desc_idx++;
	eqos->rx_desc_idx %= EQOS_DESCRIPTORS_RX;
	return length;
}

static int eqos_probe_resources_core(struct eth_device * dev)
{
	struct eqos_eth_dev *eqos = dev->priv;
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

static int eqos_phy_init(struct eth_device *dev)
{
	struct eqos_eth_dev *priv = dev->priv;
	struct phy_device *phydev;
	unsigned mask = 0xffffffff;

#ifdef CONFIG_PHY_ADDR
	mask = 1 << CONFIG_PHY_ADDR;
#endif

	phydev = phy_find_by_mask(priv->bus, mask, priv->interface);
	if (!phydev)
		return -1;

	phy_connect_dev(phydev, dev);

	phydev->supported &= PHY_GBIT_FEATURES;
	phydev->advertising = phydev->supported;

	priv->phydev = phydev;
	p_phydev = phydev;
	phy_config(phydev);

	return 1;
}

int eqos_initialize(ulong base_addr, u32 interface)
{
	int ret;
	dev = (struct eth_device *) malloc(sizeof(struct eth_device));
	if (!dev)
		return -ENOMEM;

	/*
	 * Since the priv structure contains the descriptors which need a strict
	 * buswidth alignment, memalign is used to allocate memory
	 */
	eqos_priv = (struct eqos_eth_dev *) memalign(ARCH_DMA_MINALIGN,
					      sizeof(struct eqos_eth_dev));
	if (!eqos_priv) {
		free(dev);
		return -ENOMEM;
	}

	memset(dev, 0, sizeof(struct eth_device));
	memset(eqos_priv, 0, sizeof(struct eqos_eth_dev));

	sprintf(dev->name, "dwmac.%lx", base_addr);
	dev->iobase = (int)base_addr;
	dev->priv = eqos_priv;

	eqos_priv->dev = dev;
	eqos_priv->mac_regs_p = (void *)base_addr;
	eqos_priv->dma_regs_p = (void *)(base_addr +
			EQOS_DMA_REGS_BASE);
	eqos_priv->mtl_regs_p = (void *)(base_addr +
			EQOS_MTL_REGS_BASE);

	ret = eqos_probe_resources_core(dev);
	dev->init = eqos_eth_init;
	dev->send = eqos_eth_send;
	dev->recv = eqos_eth_recv;
	dev->halt = eqos_eth_halt;
	dev->write_hwaddr = eqos_write_hwaddr;

	eth_register(dev);

	eqos_priv->interface = interface;

	eqos_mdio_init(dev->name, eqos_priv->mac_regs_p);
	eqos_priv->bus = miiphy_get_dev_by_name(dev->name);
	ret = eqos_phy_init(dev);

	return ret;
}

#ifdef ETHERNET_EXTERNAL_PHY
int check_eth_para(void);
char bestwindow = -1;
char cmd[64];
#endif

int designware_initialize(ulong base_addr, u32 interface)
{
	int ret;
	int mac_version;

	mac_version = (readl(base_addr+0x20) & 0xff);
	if (mac_version == 0x37) {
		printf("sysnopsys gmac3.7\n");
		ret = dw_initialize(base_addr, interface);
	} else {
		mac_version = (readl(base_addr+0x110) & 0xff);
		if (mac_version == 0x51) {
			printf("sysnopsys gmac5.1\n");
			ret = eqos_initialize(base_addr, interface);
		} else {
			printf("no find match gmac\n");
			return -1;
		}
	}

#ifdef ETHERNET_EXTERNAL_PHY
	if (check_eth_para()) {
		run_command("autocali 5 1 1 0", 0);
	}
	if (bestwindow >= 0) {
		sprintf(cmd, "fdt set /ethernet@%08x auto_cali_idx <%d>", (unsigned int)base_addr, bestwindow);
		run_command("fdt addr $dtb_mem_addr", 0);
		run_command(cmd, 0);
	}
#endif
	return ret;
}


/* amlogic debug cmd start */
static int do_phyreg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int reg, value;
	unsigned char *cmd = NULL;
	unsigned int i;

	if (argc < 2) {
		return cmd_usage(cmdtp);
	}

	if (dev->priv == NULL || p_phydev == NULL) {
		return -1;
	}

	cmd = (unsigned char *)argv[1];
	switch (*cmd) {
		case 'd':
			printf("=== ethernet phy register dump:\n");
			for (i = 0; i < 32; i++)
				printf("[reg_%d] 0x%x\n", i, phy_read(p_phydev, MDIO_DEVAD_NONE, i));
			break;
		case 'r':
			if (argc != 3) {
				return cmd_usage(cmdtp);
			}
			printf("=== ethernet phy register read:\n");
			reg = simple_strtoul(argv[2], NULL, 10);
			printf("[reg_%d] 0x%x\n", reg, phy_read(p_phydev, MDIO_DEVAD_NONE, reg));

			break;
		case 'w':
			if (argc != 4) {
				return cmd_usage(cmdtp);
			}
			printf("=== ethernet phy register write:\n");
			reg = simple_strtoul(argv[2], NULL, 10);
			value = simple_strtoul(argv[3], NULL, 16);
			phy_write(p_phydev, MDIO_DEVAD_NONE, reg, value);
			printf("[reg_%d] 0x%x\n", reg, phy_read(p_phydev, MDIO_DEVAD_NONE, reg));
			break;

		default:
			return cmd_usage(cmdtp);
	}

	return 0;
}

static int do_macreg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int reg, value;
	unsigned char *cmd = NULL;
	unsigned int i = 0;

	if (priv == NULL) {
		printf("please use command(md 0xff3f000)\n");
		return 0;
	}

	if (argc  < 2) {
		return cmd_usage(cmdtp);
	}

	cmd = (unsigned char *)argv[1];
	switch (*cmd) {
		case 'd':
			printf("=== ETH_MAC register dump:\n");
			for (i = 0x0000; i <= 0x004C; i += 0x4)
				printf("[0x%04x] 0x%lx\n", i, (unsigned long)readl((unsigned long)priv->mac_regs_p + i));

			printf("=== ETH_DMA register dump:\n");
			for (i = 0x0000; i <= 0x0054; i += 0x4)
				printf("[0x%04x] 0x%x\n", i, (unsigned int)readl((unsigned long)priv->dma_regs_p + i));

			break;
		case 'r':
			if (argc != 3) {
				return cmd_usage(cmdtp);
			}
			printf("=== ethernet mac register read:\n");
			reg = simple_strtoul(argv[2], NULL, 10);
			printf("[0x%04x] 0x%x\n", i, (unsigned int)readl((unsigned long)priv->mac_regs_p + reg));

			break;
		case 'w':
			if (argc != 4) {
				return cmd_usage(cmdtp);
			}
			printf("=== ethernet mac register write:\n");
			reg = simple_strtoul(argv[2], NULL, 10);
			value = simple_strtoul(argv[3], NULL, 16);
			writel(value, (unsigned long)priv->mac_regs_p + reg);
			printf("[0x%04x] 0x%x\n", reg, value);
			break;

		default:
			return cmd_usage(cmdtp);
	}

	return 0;
}

static int do_cbusreg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int reg, value;
	char *cmd = NULL;


	if (argc < 3) {
		return cmd_usage(cmdtp);
	}

	cmd = argv[1];
	switch (*cmd) {
		case 'r':
			if (argc != 3) {
				return cmd_usage(cmdtp);
			}
			printf("=== cbus register read:\n");
			reg = simple_strtoul(argv[2], NULL, 16);
			printf("[0x%04x] 0x%x\n", reg, READ_CBUS_REG(reg));

			break;
		case 'w':
			if (argc != 4) {
				return cmd_usage(cmdtp);
			}
			printf("=== cbus register write:\n");
			reg = simple_strtoul(argv[2], NULL, 16);
			value = simple_strtoul(argv[3], NULL, 16);
			WRITE_CBUS_REG(reg, value);
			printf("[0x%04x] 0x%x\n", reg, READ_CBUS_REG(reg));
			break;

		default:
			return cmd_usage(cmdtp);
	}

	return 0;
}
#ifdef ETHERNET_EXTERNAL_PHY
int print_flag = 0;

#define ETH_MAGIC "exphy:"
struct unify_eth_info {
	char magic[6];
	char index;
	char chk;
	char ver[24];
};

int check_eth_para(void)
{
	struct unify_eth_info unify_eth;
	int ret;

	ret = key_unify_read("eth_exphy_para", &unify_eth, sizeof(struct unify_eth_info));
	if (ret)
		return ret;

	if (strncmp(unify_eth.magic, ETH_MAGIC, 6) != 0)
		return -1;

	if (unify_eth.index + unify_eth.chk !=0xff)
		return -1;

	if (strncmp(unify_eth.ver, PLAIN_VERSION, 24) != 0)
		return -1;

	bestwindow = unify_eth.index;
	return 0;
}

int calc_result(u16* calilist)
{
	int best_window = -1;
	int start_window = -1;
	int end_window = -1;
	int cur_test_start_window = -1;
	int cur_window;
	int i,j;
	u16 cali;

	for (i=0; i<4; i++) {
		cali = calilist[i];
		cur_test_start_window = -1;
		for (j=0; j<16; j++) {
			cur_window = (i * 16) +j;
			if (cali & (1<<j)) {
				if (cur_test_start_window == -1)
					cur_test_start_window = cur_window;

				if (j == 15) {
					if (cur_window - cur_test_start_window >= end_window - start_window) {
						start_window = cur_test_start_window;
						end_window = cur_window;
						if (print_flag)
							printf("cur: %d, start:%d, end:%d\n", cur_window,start_window,end_window);
					}
				}
			} else {
				if (cur_test_start_window != -1) {
					if (cur_window-1 - cur_test_start_window >= end_window - start_window) {
						start_window = cur_test_start_window;
						end_window = cur_window-1;
						if (print_flag)
							printf("cur: %d, start:%d, end:%d\n", cur_window,start_window,end_window);
					}
					cur_test_start_window = -1;
				}
			}
		}
	}

	if (print_flag)
		printf("final start:%d, end:%d\n", start_window,end_window);
	if ((start_window == -1) || (end_window == -1))
		best_window = -1;
	else {
		best_window = (start_window + end_window)/2;
		if (((end_window & 0xf) == 0xf) && ((best_window & 0xf) != 0xf))
			best_window += 1;
	}
	return best_window;
}

static int do_autocali(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[])
{
	unsigned int i,j /*, valid = 0*/;
	unsigned int loop_num;
	unsigned int loop_type;
	u16 reg = 0;
	u16 calilist[4];
	int cali_window;
	struct unify_eth_info unify_eth;

	if (argc < 5) {
		return cmd_usage(cmdtp);
	}
	loop_num = simple_strtoul(argv[1], NULL, 10);
	loop_type = simple_strtoul(argv[2], NULL, 10);
	print_flag = simple_strtoul(argv[4], NULL, 10);
	do_cali_timeout = simple_strtoul(argv[3], NULL, 10);
	do_cali_process = 1;
	for (i=0; i<4; i++) {
		calilist[i] = 0;
	}
	if (loop_type) {
		phy_write(p_phydev, MDIO_DEVAD_NONE, 0, 0x5040);
		mdelay(2);
	} else {
		//MDI 1000M loopback
		phy_write(p_phydev, MDIO_DEVAD_NONE, 31, 0x0a43);
		phy_write(p_phydev, MDIO_DEVAD_NONE, 0, 0x8000);
		mdelay(40);
		phy_write(p_phydev, MDIO_DEVAD_NONE, 0, 0x1140);
		phy_write(p_phydev, MDIO_DEVAD_NONE, 24, 0x2d18);
		mdelay(400);
	}
	if (print_flag)
		printf("----------normal----------\n");

	phy_write(p_phydev, MDIO_DEVAD_NONE,31, 0xd08);
	reg = phy_read(p_phydev, MDIO_DEVAD_NONE,0x11);
	reg = phy_write(p_phydev, MDIO_DEVAD_NONE, 0x11, reg & (~0x100));
	reg = phy_read(p_phydev, MDIO_DEVAD_NONE,0x15);
	reg = phy_write(p_phydev, MDIO_DEVAD_NONE, 0x15, reg & (~0x8));
	phy_write(priv->phydev, MDIO_DEVAD_NONE, 31, 0x0);
	writel(0x1621, 0xff634540);
	for (i = 0; i < 16; i++) {
		writel(i << 16, 0xff634544);
		if (print_flag)
			printf("0x%05x\n", i << 16);
		//mdelay(20);
		for (j=0; j<loop_num; j++) {
			if (NetLoop(ETHLOOP) < 0) {
				if (print_flag)
					printf("-------------failed\n\n");
				calilist[0] &=(~(1<<i));
				break;
			} else {
				if (print_flag)
					printf("-------------success\n\n");
				calilist[0] |=(1<<i);
			}
			mdelay(1);
		}
	}
	if (print_flag)
		printf("----------invert----------\n");
	writel(0x1629, 0xff634540);
	for (i = 0; i < 16; i++) {
		writel(i << 16, 0xff634544);
		if (print_flag)
			printf("0x%05x\n", i << 16);
		//mdelay(20);
		for (j=0; j<loop_num; j++) {
			if (NetLoop(ETHLOOP) < 0) {
				if (print_flag)
					printf("-------------failed\n\n");
				calilist[1] &=(~(1<<i));
				break;
			} else {
				if (print_flag)
					printf("-------------success\n\n");
				calilist[1] |=(1<<i);
			}
			mdelay(1);
		}
	}

	/*add 2ns delay and invert clk*/
	/*reset exphy to add 2ns delay*/
	phy_write(p_phydev, MDIO_DEVAD_NONE,31, 0xd08);
	reg = phy_read(p_phydev, MDIO_DEVAD_NONE,0x15);
	reg = phy_write(p_phydev, MDIO_DEVAD_NONE, 0x15, reg | 0x8);
	phy_write(p_phydev, MDIO_DEVAD_NONE, 31, 0x0);

	/*inverted clk*/
	writel(0x1629, 0xff634540);

	if (print_flag)
		printf("----invert && add 2ns-----\n");
	for (i = 0; i < 16; i++) {
		writel(i << 16, 0xff634544);
		if (print_flag)
			printf("0x%05x\n", i << 16);
		//mdelay(20);
		for (j=0;j<loop_num;j++) {
			if (NetLoop(ETHLOOP) < 0) {
				if (print_flag)
					printf("-------------failed\n\n");
				calilist[2] &=(~(1<<i));
				break;
			} else {
				if (print_flag)
					printf("-------------success\n\n");
				calilist[2] |=(1<<i);
			}
			mdelay(1);
		}
	}

	if (print_flag)
		printf("----normal && add 2ns-----\n");
	writel(0x1621, 0xff634540);
	for (i = 0; i < 16; i++) {
		writel(i << 16, 0xff634544);
		if (print_flag)
			printf("0x%05x\n", i << 16);
		//mdelay(20);
		for (j=0; j<loop_num; j++) {
			if (NetLoop(ETHLOOP) < 0) {
				if (print_flag)
					printf("-------------failed\n\n");
				calilist[3] &=(~(1<<i));
				break;
			} else {
				if (print_flag)
					printf("-------------success\n\n");
				calilist[3] |=(1<<i);
			}
			mdelay(1);
		}
	}

	if (print_flag)
		printf("result:	%04x\t%04x\t%04x\t%04x\n", calilist[0], calilist[1], calilist[2], calilist[3]);

	cali_window = calc_result(calilist);
	printf("The Best Window is index %d\n", cali_window);

	strncpy(unify_eth.magic, ETH_MAGIC, 6);
	unify_eth.index = cali_window;
	unify_eth.chk = 0xff - unify_eth.index;
	strncpy(unify_eth.ver, PLAIN_VERSION, 24);

	key_unify_write("eth_exphy_para", &unify_eth, sizeof(struct unify_eth_info));
	bestwindow = unify_eth.index;
	do_cali_process = 0;
	return 0;
}

#endif

U_BOOT_CMD(
		phyreg, 4, 1, do_phyreg,
		"ethernet phy register read/write/dump",
		"d            - dump phy registers\n"
		"       r reg        - read phy register\n"
		"       w reg val    - write phy register"
		);

U_BOOT_CMD(
		macreg, 4, 1, do_macreg,
		"ethernet mac register read/write/dump",
		"d            - dump mac registers\n"
		"       r reg        - read mac register\n"
		"       w reg val    - write mac register"
		);

U_BOOT_CMD(
		cbusreg, 4, 1, do_cbusreg,
		"cbus register read/write",
		"r reg        - read cbus register\n"
		"        w reg val    - write cbus register"
		);
#ifdef ETHERNET_EXTERNAL_PHY
U_BOOT_CMD(
	autocali,	5,	1,	do_autocali,
	"auto cali\t- auto set cali value for exphy\n",
	"loopcnt type timeout flag\n"
	"		loopcnt	- loop package count\n"
	"		type	- 0: pcs loop 1: phy loop\n"
	"		timeout	- timeout (ms)\n"
	"		flag	- print flag\n"
);
#endif
/* amlogic debug cmd end */
