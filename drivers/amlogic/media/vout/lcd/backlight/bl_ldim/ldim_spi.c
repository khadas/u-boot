// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <asm/arch/gpio.h>
#include <fdtdec.h>
#include <amlogic/media/vout/lcd/bl_ldim.h>
#include "../../lcd_common.h"
#include "ldim_drv.h"
#include "ldim_dev_drv.h"

//#define LDIM_SPI_CUSTOM_DELAY

#ifdef CONFIG_DM_SPI

static unsigned int cs_hold_delay;
#ifdef LDIM_SPI_CUSTOM_DELAY
static unsigned int cs_clk_delay;
#endif

int ldim_spi_write(struct spi_slave *spi, unsigned char *tbuf, int tlen)
{
	int ret, bitlen;

	ret = spi_claim_bus(spi);
	if (ret) {
		LDIMERR("%s: request spi bus failed\n", __func__);
		goto wreg_end;
	}

	if (cs_hold_delay)
		udelay(cs_hold_delay);
#ifdef LDIM_SPI_CUSTOM_DELAY
	spi_cs_activate(spi);
	if (cs_clk_delay)
		udelay(cs_clk_delay);
#endif
	bitlen = tlen * 8;
	ret = spi_xfer(spi, bitlen, tbuf, NULL, SPI_XFER_BEGIN | SPI_XFER_END);
#ifdef LDIM_SPI_CUSTOM_DELAY
	if (cs_clk_delay)
		udelay(cs_clk_delay);
	spi_cs_deactivate(spi);
#endif
	if (ret)
		LDIMERR("%s\n", __func__);

wreg_end:
	spi_release_bus(spi);
	return ret;
}

int ldim_spi_read(struct spi_slave *spi, unsigned char *tbuf, int tlen,
		unsigned char *rbuf, int rlen)
{
	int ret, bitlen;

	ret = spi_claim_bus(spi);
	if (ret) {
		LDIMERR("%s: request spi bus failed\n", __func__);
		goto rreg_end;
	}

	if (cs_hold_delay)
		udelay(cs_hold_delay);
#ifdef LDIM_SPI_CUSTOM_DELAY
	spi_cs_activate(spi);
	if (cs_clk_delay)
		udelay(cs_clk_delay);
#endif
	bitlen = (tlen + rlen) * 8;
	ret = spi_xfer(spi, bitlen, tbuf, rbuf, SPI_XFER_BEGIN | SPI_XFER_END);
	if (ret)
		goto rreg_end;
#ifdef LDIM_SPI_CUSTOM_DELAY
	if (cs_clk_delay)
		udelay(cs_clk_delay);
	spi_cs_deactivate(spi);
#endif
	if (ret)
		LDIMERR("%s\n", __func__);

rreg_end:
	spi_release_bus(spi);
	return ret;
}

int ldim_spi_read_sync(struct spi_slave *spi, unsigned char *tbuf,
		       unsigned char *rbuf, int len)
{
	int ret, bitlen;

	ret = spi_claim_bus(spi);
	if (ret) {
		LDIMERR("%s: request spi bus failed\n", __func__);
		goto rreg_end;
	}

	if (cs_hold_delay)
		udelay(cs_hold_delay);
#ifdef LDIM_SPI_CUSTOM_DELAY
	spi_cs_activate(spi);
	if (cs_clk_delay)
		udelay(cs_clk_delay);
#endif
	bitlen = len * 8;
	ret = spi_xfer(spi, bitlen, tbuf, rbuf, SPI_XFER_BEGIN | SPI_XFER_END);
	if (ret)
		goto rreg_end;
#ifdef LDIM_SPI_CUSTOM_DELAY
	if (cs_clk_delay)
		udelay(cs_clk_delay);
	spi_cs_deactivate(spi);
#endif
	if (ret)
		LDIMERR("%s\n", __func__);

rreg_end:
	spi_release_bus(spi);
	return ret;
}

int ldim_spi_driver_add(struct ldim_dev_driver_s *dev_drv)
{
	struct udevice *dev;
	int ret;

	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}

	/* register spi */
	snprintf(dev_drv->spi_info.spi_name, LDIM_SPI_NAME_MAX,
		"generic_%d:%d",
		dev_drv->spi_info.bus_num,
		dev_drv->spi_info.chip_select);
	ret = spi_get_bus_and_cs(dev_drv->spi_info.bus_num,
				dev_drv->spi_info.chip_select,
				dev_drv->spi_info.max_speed_hz,
				dev_drv->spi_info.mode,
				"spi_generic_drv",
				dev_drv->spi_info.spi_name,
				&dev, &dev_drv->spi_info.spi);
	if (ret) {
		LDIMERR("%s: register spi driver failed\n", __func__);
		return -1;
	}
	dev_drv->spi_info.spi->wordlen = dev_drv->spi_info.wordlen;
	cs_hold_delay = dev_drv->cs_hold_delay;
#ifdef LDIM_SPI_CUSTOM_DELAY
	cs_clk_delay = dev_drv->cs_clk_delay;

	spi_cs_deactivate(dev_drv->spi_info.spi);
#endif
	spi_release_bus(dev_drv->spi_info.spi);

	return 0;
}

int ldim_spi_driver_remove(struct ldim_dev_driver_s *dev_drv)
{
	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}

	dev_drv->spi_info.spi = NULL;

	return 0;
}

#else
int ldim_spi_write(struct spi_slave *spi, unsigned char *tbuf, int tlen)
{
	LDIMERR("%s: no CONFIG_DM_SPI support\n", __func__);

	return -1;
}

int ldim_spi_read(struct spi_slave *spi, unsigned char *tbuf, int tlen,
		  unsigned char *rbuf, int rlen)
{
	LDIMERR("%s: no CONFIG_DM_SPI support\n", __func__);

	return -1;
}

int ldim_spi_read_sync(struct spi_slave *spi, unsigned char *tbuf,
		       unsigned char *rbuf, int len)
{
	LDIMERR("%s: no CONFIG_DM_SPI support\n", __func__);

	return -1;
}

int ldim_spi_driver_add(struct ldim_dev_driver_s *dev_drv)
{
	LDIMERR("%s: no CONFIG_DM_SPI support\n", __func__);

	return -1;
}

int ldim_spi_driver_remove(struct ldim_dev_driver_s *dev_drv)
{
	return 0;
}
#endif

