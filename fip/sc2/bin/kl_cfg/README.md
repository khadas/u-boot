OTP Pattern Generator for Key ladder
-----------------------------
#Version: 1.0

2022 Amlogic, Inc. All rights reserved.

## Command line
```
1) ETSI KL
There are 5 root key profiles, and each key profile can work with TDES decryption and AES decryption. So, there are total 10 key ladder cases need to be test.
For SC2, only root key profiles can be config in efuse, key ladder configuration can be control by register.
One SoC can be configured with 4 root key profiles. To test all 10 key ladder cases, 2 SoCs are needed to be burnt with correct Efuse pattern and 2 round tests are needed to be done.

<Round 1>
$ ./efuse-gen.sh --kl -etsi kl_cfg/etsi_kl_round1.csv

<Round 2>
$ ./efuse-gen.sh --kl -etsi kl_cfg/etsi_kl_round2.csv

2) AML KL
<DGPK1>
$ ./efuse-gen.sh --kl -dgpk1 kl_cfg/dgpk1.csv

<DGPK2>
$ ./efuse-gen.sh --kl -dgpk2 kl_cfg/dgpk2.csv
```

## CSV format in Depth
* First line of CSV file must be `Feature,Size,Value`
* Name the required feature, a size (in bits) and a value in the following lines
* The value must be in hexadecimali.
* Byte ordering must be little endian.
* 0x44422A00 => 002A4244
* Execute the command line to generate the OTP efuse pattern.

Examples (ETSI Key ladder)

```
Feature,Size,Value
ETSI_SCK_0,128,77656C636F6D65746F6D797061727479
ETSI_SCK_1,128,77656C636F6D65746F6D797061727479
ETSI_SCK_2,128,77656C636F6D65746F6D797061727479
ETSI_SCK_3,128,77656C636F6D65746F6D797061727479
ETSI_SCK_CONFIG_0,32,002A4280
ETSI_SCK_CONFIG_1,32,002A4284
ETSI_SCK_CONFIG_2,32,002A42C0
ETSI_SCK_CONFIG_3,32,002A42C4
ETSI_SCK_0_LOCK,1,1
ETSI_SCK_1_LOCK,1,1
ETSI_SCK_2_LOCK,1,1
ETSI_SCK_3_LOCK,1,1
```

Examples (AML Key ladder-DGPK1)

```
Feature,Size,Value
DGPK_1,128,00112233445566778899aabbccddeeff
DGPK_1_LOCK,1,1
```

Examples (AML Key ladder-DGPK2)

```
Feature,Size,Value
DGPK_2,128,00112233445566778899aabbccddeeff
DGPK_2_LOCK,1,1
```

## Output
1) ETSI KL
<Round 1>
kl_cfg/etsi_kl_round1.csv.bin (generated.output.efuse)
kl_cfg/etsi_kl_round1.csv.bin.dec

<Round 2>
kl_cfg/etsi_kl_round2.csv.bin (generated.output.efuse)
kl_cfg/etsi_kl_round2.csv.bin.dec

2) AML KL
<DGPK1>
kl_cfg/dgpk1.csv.bin (generated.output.efuse)
kl_cfg/dgpk1.csv.bin.dec

<DGPK2>
kl_cfg/dgpk2.csv.bin (generated.output.efuse)
kl_cfg/dgpk2.csv.bin.dec

## Fusing OTP
1. Copy generated files to SD card or usb storage device
2. Insert storage into the slot and boot.

On u-boot prompt:

```
#sdcard
# fatload mmc 0 1080000 <generated.output.efuse>
# efuse amlogic_set 1080000

#usb storage device
# usb start
# usb tree  # to check if usb device has been recognized
# fatload usb 0 1080000 <generated.output.efuse>
# efuse amlogic_set 1080000
```
