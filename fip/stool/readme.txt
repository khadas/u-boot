Introduction:
This document is deciated for usage help of Amlogic Script Signing Tool Set which is a tool set for Amlogic secure boot solution.
It includes two topic as following:
a. support sign uboot/kernel/recovery/dtb with the tool
b. support usb password feature


SIGNING PROCESS FOR UBOOT/KERNEL
|--UBOOT prepare
|  |-1. get the latest version which support script sign uboot feature
|	 |-2. then open the switch #define CONFIG_AML_SIGNED_UBOOT   1 //gxl_skt_v1.h
|	 |-3. build uboot successfully with command ./mk gxl_skt_v1
|  1-4. get package file for v3 secript gxl_skt_v1-u-boot.aml.zip
|
|--KERNEL/RECOVERY/DTB prepare
|  |-1. kernel/recovery/dtb image should be ready and place them to folder ./input
|
|--KEY prepare
|  |-1. run ./key.create.bash keypath to generate keys to folder keypath
|  |    |-1. for uboot signing you must afford RSA/AES key for bl2/bl3x and place them to folder   ./key
|  |    |-2. for kernel/recovery/dtb signing you must afford RSA/AES key and place them to folfder ./key
|  |-2. for those project which use aml_encrypt_gxl/g12a/txlx etc for secure boot, please use following mapping table for key transfer
|       |------former key        <---->  script tool key(14 key files)-------
|       |-1.                            /aml-key/bl2aesiv         //dd if=/dev/zero of=bl2aesiv conv=notrunc bs=1 count=16 >& /dev/null
|       |-2.  /userkey/key.aes          /aml-key/bl2aeskey
|       |-3.  /usekey/key.kxa.pem       /aml-key/bl2.pem
|       |-4.  /userkey/key.aes          /aml-key/bl3xaeskey
|       |-5.                            /aml-key/bl3xaesiv        //dd if=/dev/zero of=bl3xaesiv conv=notrunc bs=1 count=16 >& /dev/null
|       |-6.  /usekey/key.kxa.pem       /aml-key/bl3xkey.pem
|       |-7.  /userkey/key.aes          /aml-key/kernelaeskey
|       |-8.                            /aml-key/kernelaesiv      //dd if=/dev/zero of=kernelaesiv conv=notrunc bs=1 count=16 >& /dev/null
|       |-9.  /usekey/key.kxa.pem       /aml-key/kernelkey.pem
|       |-10. /rootkey_1/key.kxa.pem    /aml-key/root.pem
|       |-11. /rootkey_1/key.kxa.pem    /aml-key/root0.pem
|       |-12. /rootkey_2/key.kxa.pem    /aml-key/root1.pem
|       |-13. /rootkey_3/key.kxa.pem    /aml-key/root2.pem
|       |-14. /rootkey_4/key.kxa.pem    /aml-key/root3.pem
|       |-
|       |-NOTE: for those keys which not exist in the former key set please just use one 16bytes random binary file is OK
|       |-IMPORTANT: After make out the new key set you must take following process before use the *.pem RSA key file
|       |-           remove all extra info which after "-----END RSA PRIVATE KEY-----" because the script signing tool does support this format
|
|--Command to signing
|  |-1 ./sign.sh -s soc -p input -r rsakey -a aeskey -o output
|  |             -s soc              //soc type: gxl,txlx,g12a,g12b
|  |             -p input            //input folder
|  |             -r frsakey          //rsa key folder
|  |             -a faeskey          //aes key folder
|  |             -o output           //output folder
|  |-2 ./sign.sh -s soc -z package -r rsakey -a aeskey -o output
|  |             -s soc              //soc type: gxl,txlx,g12a,g12b
|  |             -z zip package file //uboot package image, e.g. gxl_skt_v1-u-boot.aml.zip
|  |             -r frsakey          //rsa key folder
|  |             -a faeskey          //aes key folder
|  |             -o output           //output folder
|
|--NOTE: uboot building support auto sign with CONFIG_AML_SIGNED_UBOOT enabled. Please check build.sh of each SoC. (e.g fip/gxl/build.sh)


USB password process
|
|--password prepare
|   |-1. password.bin  //passowrd for usb boot, min is 4bytes and max to 16bytes, binary format,random value
|   |-2. salt.bin      //solution usage, must be 4byts, binary format, random value
|
|--password process
|   |-1. tool amlpwdefs dedicated to generate the corresponding pattern with password & salt
|        |- ./amlpwdefs --password password.bin --salt salt.bin //get output is password.bin_PMMMMMMMM_SNNNNNNNN.bin
|   |-2. tool efuse.sh is a tool wraper for efuse pattern process, it will canl efuse-gen.sh from signing-tool-gxl-dev or
|   |    |  signing-tool-g12a-dev, current version only support gxl/txlx/g12a/g12b
|   |    |- ./efuse.sh --generate-efuse-pattern --password-hash password.bin_PMMMMMMMM_SNNNNNNNN.bin -o pattern.usb.efuse  --enable-usb-passwor
|
|--use tool step by step
|   |-1. password.bin prepare
|   |    |- set password binary image file password.bin with 16bytes: 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f
|   |
|   |-2. salt.bin prepare
|   |    |- set salt binary image file salt.bin with four bytes: 0x05,0x06,0x07,0x08
|   |
|   |-3. process with amlpwdefs to generate password pattern
|   |    |- ./amlpwdefs --password password.bin --salt salt.bin
|   |    |   |  get 32bytes file with name password.bin_P00010203_S05060708.bin
|   |    |   |  P00010203 : 00 01 02 03 are the first 4bytes of password
|   |    |   |  S05060708 : 05 06 07 08 are the salt content
|   |    |- NOTE: rename the output to usb.password.hash.bin and place it to the script key folder for auto generate pattern.usb.efuse for usb password protect usage.
|   |    |        Yes, you can also process it with following step to generate pattern and burn it to SoC.
|   |
|   |-4. use efuse.sh to generate EFUSE pattern for USB password
|   |    |- ./efuse.sh --generate-efuse-pattern --password-hash password.bin_P00010203_S05060708.bin -o pattern.usb.efuse  --enable-usb-password true --soc gxl
|   |    |    | get output image with fixed size 1024bytes pattern.usb.efuse which can be used to program the pattern to SoC and enable the license bit
|   |
|   |-5. burn pattern to SoC with uboot command
|   |    |-1. fatload mmc 0 1080000 pattern.efuse  //load passowrd pattern file to DDR 0x1080000
|   |    |-2. efuse amlogic_set 1080000            //burn EFUSE
|   |    |-3. reset                                //reset to make the usb password protection work
|   |    |-4.                                      //from now on the SoC usb boot is protected with password
|
|--NOTE: uboot building support auto generate usb password pattern when usb.password.hash.bin does exis in each key folder of SoC. Then three EFUSE pattern file will generate
|        pattern.efuse            //for secure boot & usb password use
|        pattern.secureboot.efuse //for secure boot key use only
|        pattern.usb.efuse        //for usb password use only


EFUSE pattern process
|
|--function usage
|   ./efuse.sh --generate-efuse-pattern \                    //
|               --soc [gxl | txlx | g12a | g12b ] \          //soc type, must afford
|               [--aml-key-path path-of-key]      \          //key path, will get RSA & AES key from it
|               [--rsa-key-path path-of-rsa-key]  \          //key path, will get RSA key from it
|               [--enable-sb false]               \          //secure boot enable flag, default is false
|               [--aes-key aes-key]               \          //aes key
|               [--enable-aes false]              \          //AES scramble enable flag, default is false
|               [--password-hash password.hash]   \          //password hash input, password hash generated with amlpwdefs,32 bytes binary file
|               [--enable-jtag-password false]    \          //JTAG password enable flag, default is false
|               [--enable-usb-password false]     \          //USB password enable flag, default is false
|               [--enable-anti-rollback false]    \          //ANTI-ROLLBACK password enable flag, default is false
|               -o pattern.efuse                             //output pattern file name
|
|--key prepare
|   |-1. RSA key  //root RSA key which will be stored in EFUSE and one of them will be used to sign the user RSA key
|   |             //PEM format only
|   |-2. AES key  //AES key which will be stored in EFUSE and it will be used for BL2 scramble
|   |             //32 bytes binary file
|
|--password process
|   |-1. tool amlpwdefs dedicated to generate the corresponding pattern with password & salt
|        |- ./amlpwdefs --password password.bin --salt salt.bin //get output is password.bin_PMMMMMMMM_SNNNNNNNN.bin
|
|--NOTE: above EFUSE pattern generate process can be used separately, this is no dependence for all of them
|


FOLDER ARCHITECTURE:
------------Script Signing Tool Set folder------------
|-1.  signing-tool-gxl      //D: tool set for signing -- DO NOT MODIFY
|-2.  signing-tool-gxl-dev  //D: tool set for signing -- DO NOT MODIFY
|-3.  amlogic-sign-gxl.sh   //F: tool for signing GXL/TXLX -- DO NOT MODIFY
|-4.  signing-tool-g12a     //D: tool set for signing -- DO NOT MODIFY
|-5.  signing-tool-g12a-dev //D: tool set for signing -- DO NOT MODIFY
|-6.  amlogic-sign-g12a.sh  //F: tool for signing G12A/B -- DO NOT MODIFY
|-7.  sign.sh               //F: tool for signing -- DO NOT MODIFY
|-8.  key.create.bash       //F: tool for key generate -- DO NOT MODIFY
|-9.  amlpwdefs             //F: tool for generate pattern with password binnary image(4byes -- 16bytes) & salt binary image(4bytes) -- DO NOT MODIFY
|-10. efuse.sh              //F: tool for EFUSE pattern process -- DO NOT MODIFY
|-11. readme.txt            //F: it is me

------------User defined file folder------------
|-input  //input for script signing tool, support set with -p inputfolder
|  |
|  |--bl2_new.bin  //bl2  -- must for uboot
|  |--bl30_new.bin //bl30 -- must for uboot
|  |--bl31.img     //bl31 -- must for uboot
|  |--bl33.bin     //bl33 -- must for uboot
|  |--bl32.img     //bl32 -- optional for uboot
|  |--boot.img     //boot     -- must for kernel
|  |--recovery.img //recovery -- must for recovery
|  |--dt.img       //dt       -- must for dtb
|
|-key  //input for script signing tool, support set with -r rsakeyfolder and -a aeskeyfolder
|  |
|  |--root.pem    //root RSA key  -- must for uboot,must come from root0/1/2/3
|  |--root0.pem   //root RSA key0 -- must for uboot
|  |--root1.pem   //root RSA key1 -- must for uboot
|  |--root2.pem   //root RSA key2 -- must for uboot
|  |--root3.pem   //root RSA key3 -- must for uboot
|  |--bl2.pem     //bl2 RSA key  -- must for uboot
|  |--bl2aesiv    //bl2 aes key IV -- must for uboot
|  |--bl2aeskey   //bl2 aes key  -- must for uboot
|	 |--bl3xkey.pem //bl3x RSA key  -- must for uboot
|  |--bl3xaesiv   //bl3x aes key IV -- must for uboot
|  |--bl3xaekey   //bl3x aes key  -- must for uboot
|  |--kernelkey.pem //RSA key    -- must for kernel/recovery/dtb
|  |--kernelaesiv   //aes key IV -- must for kernel/recovery/dtb
|  |--kernelaeskey  //aes key    -- must for kernel/recovery/dtb
|  |
|-output  //input for script signing tool, support set with -o outputfolder
|  |
|  |--pattern.efuse                         //EFUSE pattern for secure boot & usb boot (optional, inlcude the usb password if file usb.password.hash.bin exit in the key folder)
|  |--pattern.secureboot.efuse              //EFUSE pattern for secure boot only
|  |--pattern.usb.efuse                     //EFUSE pattern for usb boot only (auto generate if usb.password.hash.bin does exist in the key folder)
|  |--u-boot.bin.signed.encrypted           //signed uboot for NAND/SPI/eMMC
|  |--u-boot.bin.usb.bl2.signed.encrypted   //signed BL2 for usb boot only
|  |--u-boot.bin.usb.tpl.signed.encrypted   //signed TPL for usb boot only
|  |--u-boot.bin.signed.encrypted.sd.bin    //signed uboot for SD card boot only
|  |--boot.img.encrypt                      //signed kernel image
|  |--recovery.img.encrypt                  //signed recovery image
|  |--dtb.img.encrypt                       //signed dtb image
