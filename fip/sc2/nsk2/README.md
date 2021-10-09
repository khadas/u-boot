OTP Pattern Generator for OEM
-----------------------------

2022 Amlogic, Inc. All rights reserved.

## Command line
```
$ ./gen-oem-otp.sh -i <csv_file_input> -o <output_filename>
```

## CSV format in Depth
* First line of CSV file must be `Feature,Size,Value`
* Name the required feature, a size (in bits) and a value in the following lines
* The value must be in hexadecimal.
* Execute the command line to generate the OTP efuse pattern.
* What features to be included with what values must be discussed with Synamedia.

Examples

```
Feature,Size,Value
FEAT_ENABLE_DEVICE_SCS_SIG_0,1,1
FEAT_ENABLE_DEVICE_SCS_SIG_1,1,1
FEAT_ENABLE_DEVICE_VENDOR_SIG_0,1,1
FEAT_ENABLE_DEVICE_VENDOR_SIG_1,1,1
FEAT_ENABLE_DEVICE_PROT_0,1,1
FEAT_ENABLE_DEVICE_PROT_1,1,1
FEAT_DISABLE_BOOT_NORMAL_SPI_NOR,1,1
FEAT_DISABLE_BOOT_NORMAL_NAND,1,1
FEAT_DISABLE_BOOT_NORMAL_EMMC,1,1
FEAT_DISABLE_BOOT_NORMAL_SDCARD,1,1
FEAT_DISABLE_BOOT_NORMAL_SPI_NAND,1,1
FEAT_DISABLE_BOOT_DFU_USB,1,1
FEAT_DISABLE_BOOT_ALTSRC,1,1
FEAT_DISABLE_BOOT_NORMAL_FALLBACK2USB,1,1
FEAT_DISABLE_SW_BOOT_OVERRIDE,1,1
FEAT_DISABLE_EXT_BOOT_OVERRIDE,1,1
FEAT_DISABLE_DDR_FIP_CONTAINER,1,1
FEAT_ENABLE_USB_AUTH,1,1
FEAT_ENABLE_DIF_MASTER_PROT,1,1
FEAT_ENABLE_ACPU_JTAG_PROT,1,1
FEAT_ENABLE_AOCPU_JTAG_PROT,1,1
FEAT_ENABLE_DSP_JTAG_PROT,1,1
FEAT_DISABLE_NSK_CONCURRENCY,1,1
NSK_PUB_OTP_OID,16,1122
NSK_PUB_OTP_VID,16,3344
DEVICE_SCS_SEGID,32,55667788
DEVICE_VENDOR_SEGID,32,aabbccdd
DGPK1,128,00112233445566778899aabbccddEEFF
LICENSE_LOCK_1_B228_LOCK,1,1
NSK_OTP_WRITE_PASSWORD_PROTECT,1,1
```
## Fusing OTP
1. Copy generated files to SD card or usb storage device
2. Insert storage into the slot and boot.

On u-boot prompt:

```
#sdcard
sc2_ah219# fatload mmc 0 1080000 <generated.output.efuse>
sc2_ah219# efuse amlogic_set 1080000

#usb storage device
sc2_ah219# usb start
sc2_ah219# usb tree  # to check if usb device has been recognized
sc2_ah219# fatload usb 0 1080000 <generated.output.efuse>
sc2_ah219# efuse amlogic_set 1080000
```