/*
 * PCI AER infomation display library
 *
 * Author: Shawn Lin <shawn.lin@rock-chips.com>
 *
 * Copyright 2025 Rockchip Co., Ltd.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <pci.h>
#include <errno.h>
#include <dm/device.h>

/**
 * pci_aer_dump - Parse and print AER information in a human-readable format
 * @dev: PCI device
 *
 * Return: 0 on success, negative error code on failure.
 */
int pci_aer_dump(struct udevice *udev, pci_dev_t dev)
{
	int aer_cap_ptr;
	u32 aer_status, aer_mask, aer_severity;
	u32 aer_capabilities;
	struct dm_pci_ops *ops;
	struct udevice *bus;

	/* Find the AER Capability */
	aer_cap_ptr = dm_pci_find_ext_capability(udev, PCI_EXT_CAP_ID_ERR);
	if (!aer_cap_ptr) {
		printf("AER Capability not found for device %04x:%04x\n",
			   PCI_BUS(dev), PCI_DEV(dev));
		return -ENODEV;
	}

	/* Read AER-related registers */
	dm_pci_read_config32(udev, aer_cap_ptr + PCI_AER_STATUS, &aer_status);
	dm_pci_read_config32(udev, aer_cap_ptr + PCI_AER_MASK, &aer_mask);
	dm_pci_read_config32(udev, aer_cap_ptr + PCI_AER_SEVERITY, &aer_severity);
	dm_pci_read_config32(udev, aer_cap_ptr + 0x18, &aer_capabilities); /* AER Capabilities Register */

	/* Print AER Capability information */
	printf("AER Capability found at offset 0x%x\n", aer_cap_ptr);

	/* Print Uncorrectable Error Status (UES) */
	printf("  UESta:  ");
	printf("DLP-%c ",  (aer_status & (1 << 0))  ? '+' : '-');
	printf("SDES-%c ", (aer_status & (1 << 1))  ? '+' : '-');
	printf("TLP-%c ",  (aer_status & (1 << 2))  ? '+' : '-');
	printf("FCP-%c ",  (aer_status & (1 << 3))  ? '+' : '-');
	printf("CmpltTO-%c ", (aer_status & (1 << 4))  ? '+' : '-');
	printf("CmpltAbrt-%c ", (aer_status & (1 << 5))  ? '+' : '-');
	printf("UnxCmplt-%c ", (aer_status & (1 << 6))  ? '+' : '-');
	printf("RxOF-%c ", (aer_status & (1 << 7))  ? '+' : '-');
	printf("MalfTLP-%c ", (aer_status & (1 << 8))  ? '+' : '-');
	printf("ECRC-%c ", (aer_status & (1 << 9))  ? '+' : '-');
	printf("UnsupReq-%c ", (aer_status & (1 << 10)) ? '+' : '-');
	printf("ACSViol-%c\n", (aer_status & (1 << 11)) ? '+' : '-');

	/* Print Uncorrectable Error Mask (UEMsk) */
	printf("  UEMsk:  ");
	printf("DLP-%c ",  (aer_mask & (1 << 0))  ? '+' : '-');
	printf("SDES-%c ", (aer_mask & (1 << 1))  ? '+' : '-');
	printf("TLP-%c ",  (aer_mask & (1 << 2))  ? '+' : '-');
	printf("FCP-%c ",  (aer_mask & (1 << 3))  ? '+' : '-');
	printf("CmpltTO-%c ", (aer_mask & (1 << 4))  ? '+' : '-');
	printf("CmpltAbrt-%c ", (aer_mask & (1 << 5))  ? '+' : '-');
	printf("UnxCmplt-%c ", (aer_mask & (1 << 6))  ? '+' : '-');
	printf("RxOF-%c ", (aer_mask & (1 << 7))  ? '+' : '-');
	printf("MalfTLP-%c ", (aer_mask & (1 << 8))  ? '+' : '-');
	printf("ECRC-%c ", (aer_mask & (1 << 9))  ? '+' : '-');
	printf("UnsupReq-%c ", (aer_mask & (1 << 10)) ? '+' : '-');
	printf("ACSViol-%c\n", (aer_mask & (1 << 11)) ? '+' : '-');

	/* Print Uncorrectable Error Severity (UESvrt) */
	printf("  UESvrt: ");
	printf("DLP%c ",  (aer_severity & (1 << 0))  ? '+' : '-');
	printf("SDES%c ", (aer_severity & (1 << 1))  ? '+' : '-');
	printf("TLP%c ",  (aer_severity & (1 << 2))  ? '+' : '-');
	printf("FCP%c ",  (aer_severity & (1 << 3))  ? '+' : '-');
	printf("CmpltTO%c ", (aer_severity & (1 << 4))  ? '+' : '-');
	printf("CmpltAbrt%c ", (aer_severity & (1 << 5))  ? '+' : '-');
	printf("UnxCmplt%c ", (aer_severity & (1 << 6))  ? '+' : '-');
	printf("RxOF%c ", (aer_severity & (1 << 7))  ? '+' : '-');
	printf("MalfTLP%c ", (aer_severity & (1 << 8))  ? '+' : '-');
	printf("ECRC%c ", (aer_severity & (1 << 9))  ? '+' : '-');
	printf("UnsupReq%c ", (aer_severity & (1 << 10)) ? '+' : '-');
	printf("ACSViol%c\n", (aer_severity & (1 << 11)) ? '+' : '-');

	/* Print Correctable Error Status (CESta) */
	printf("  CESta:  ");
	printf("RxErr-%c ",  (aer_status & (1 << 12)) ? '+' : '-');
	printf("BadTLP-%c ", (aer_status & (1 << 13)) ? '+' : '-');
	printf("BadDLLP-%c ", (aer_status & (1 << 14)) ? '+' : '-');
	printf("Rollover-%c ", (aer_status & (1 << 15)) ? '+' : '-');
	printf("Timeout-%c ", (aer_status & (1 << 16)) ? '+' : '-');
	printf("NonFatalErr-%c\n", (aer_status & (1 << 17)) ? '+' : '-');

	/* Print Correctable Error Mask (CEMsk) */
	printf("  CEMsk:  ");
	printf("RxErr-%c ",  (aer_mask & (1 << 12)) ? '+' : '-');
	printf("BadTLP-%c ", (aer_mask & (1 << 13)) ? '+' : '-');
	printf("BadDLLP-%c ", (aer_mask & (1 << 14)) ? '+' : '-');
	printf("Rollover-%c ", (aer_mask & (1 << 15)) ? '+' : '-');
	printf("Timeout-%c ", (aer_mask & (1 << 16)) ? '+' : '-');
	printf("NonFatalErr-%c\n", (aer_mask & (1 << 17)) ? '+' : '-');

	/* Print AER Capabilities (AERCap) */
	printf("  AERCap: ");
	printf("First Error Pointer: %02x, ", (aer_capabilities >> 0) & 0x1F);
	printf("GenCap%c ", (aer_capabilities & (1 << 5)) ? '+' : '-');
	printf("CGenEn%c ", (aer_capabilities & (1 << 6)) ? '+' : '-');
	printf("ChkCap%c ", (aer_capabilities & (1 << 7)) ? '+' : '-');
	printf("ChkEn%c\n", (aer_capabilities & (1 << 8)) ? '+' : '-');

	for (bus = udev; device_is_on_pci_bus(bus);)
		bus = bus->parent;

	ops = pci_get_ops(bus);
	if (ops->vendor_aer_dump)
		return ops->vendor_aer_dump(bus);

	return 0;
}
