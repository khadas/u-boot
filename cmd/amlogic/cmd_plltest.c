/*
* Copyright (C) 2017 Amlogic, Inc. All rights reserved.
* *
This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* *
This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
* *
You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
* *
Description:
*/



/* README */
/*

Part1: S905X usage

1. sys pll

	test pass print: sys pll test pass!
	test fail print: sys pll test failed!

	uboot command(select one as needed):
	 960MHz:	plltest sys 0x60000228 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1056MHz:	plltest sys 0x6000022c 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1152MHz:	plltest sys 0x60000230 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1248MHz:	plltest sys 0x60000234 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1344MHz:	plltest sys 0x60000238 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1440MHz:	plltest sys 0x6000023c 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1536MHz:	plltest sys 0x60000240 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1632MHz:	plltest sys 0x60000244 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	all:		plltest sys

2. hdmi pll

	test pass print: hdmi pll test pass!
	test fail print: hdmi pll test failed!

	uboot command:
	5940MHz:	plltest hdmi 0x4000027b 0x800cb300 0xc65f30e0 0x0c8e0000 0x001fa729 0x01a31500
	5405MHz:	plltest hdmi 0x400002e1 0x800cb0e6 0x865f30c4 0x0c8e0000 0x001fa729 0x01a31500
	4455MHz:	plltest hdmi 0x400002b9 0x800cb280 0x865f30c4 0x0c8e0000 0x001fa729 0x01a31500
	4324MHz:	plltest hdmi 0x400002b4 0x800cb0b8 0x865f30c4 0x0c8e0000 0x001fa729 0x01a31500
	3712MHz:	plltest hdmi 0x4000029a 0x800cb2c0 0x865f30c4 0x0c8e0000 0x001fa729 0x01a31500
	3450MHz:	plltest hdmi 0x4000028f 0x800cb300 0x865f30c4 0x0c8e0000 0x001fa729 0x01a31500
	3243MHz:	plltest hdmi 0x40000287 0x800cb08a 0x865f30c4 0x0c8e0000 0x001fa729 0x01a31500
	2970MHz:	plltest hdmi 0x4000027b 0x800cb300 0x865f30c4 0x0c8e0000 0x001fa729 0x01a31500
	all:		plltest hdmi

3. gp0 pll

	test pass print: gp0 pll test pass!
	test fail print: gp0 pll test failed!

	uboot command:
	504MHz:		plltest gp0 0xc001022a 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	516MHz:		plltest gp0 0xc001022b 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	528MHz:		plltest gp0 0xc001022c 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	540MHz:		plltest gp0 0xc001022d 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	552MHz:		plltest gp0 0xc001022e 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	564MHz:		plltest gp0 0xc001022f 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	576MHz:		plltest gp0 0xc0010230 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	588MHz:		plltest gp0 0xc0010231 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	600MHz:		plltest gp0 0xc0010232 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	612MHz:		plltest gp0 0xc0010233 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	624MHz:		plltest gp0 0xc0010234 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	636MHz:		plltest gp0 0xc0010235 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	648MHz:		plltest gp0 0xc0010236 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	660MHz:		plltest gp0 0xc0010237 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	672MHz:		plltest gp0 0xc0010238 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	684MHz:		plltest gp0 0xc0010239 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	696MHz:		plltest gp0 0xc001023a 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	708MHz:		plltest gp0 0xc001023b 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	720MHz:		plltest gp0 0xc001023c 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	732MHz:		plltest gp0 0xc001023d 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	744MHz:		plltest gp0 0xc001023e 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	756MHz:		plltest gp0 0xc001023f 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	768MHz:		plltest gp0 0xc0010240 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	780MHz:		plltest gp0 0xc0010241 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	792MHz:		plltest gp0 0xc0010242 0xc084a000 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	all:		plltest gp0



Part2: T968 usage

1. sys pll

	test pass print: sys pll test pass!
	test fail print: sys pll test failed!

	uboot command:
	1152MHz:	plltest sys 0x60000230 0x5ac80000 0x8e452015 0x0401d40c 0x00000870
	1248MHz:	plltest sys 0x60000234 0x5ac80000 0x8e452015 0x0401d40c 0x00000870
	1344MHz:	plltest sys 0x60000238 0x5ac80000 0x8e452015 0x0401d40c 0x00000870
	1440MHz:	plltest sys 0x6000023c 0x5ac80000 0x8e452015 0x0401d40c 0x00000870
	1536MHz:	plltest sys 0x60000240 0x5ac80000 0x8e452015 0x0401d40c 0x00000870
	1632MHz:	plltest sys 0x60000244 0x5ac80000 0x8e452015 0x0401d40c 0x00000870
	all:		plltest sys

2. hdmi pll

	test pass print: hdmi pll test pass!
	test fail print: hdmi pll test failed!

	uboot command:
	5940MHz:	plltest hdmi 0x5800027b 0x000E4300 0x12dc5081 0x801da72c 0x71486980 0x00002e55
	4320MHz:	plltest hdmi 0x5800025a 0x000E0000 0x0d5c5091 0x801da72c 0x71486980 0x00002e55
	3712MHz:	plltest hdmi 0x5800024d 0x000E4160 0x0d5c5091 0x801da72c 0x71486980 0x00002e55
	all:		plltest hdmi

3. gp0 pll

	test pass print: gp0 pll test pass!
	test fail print: gp0 pll test failed!

	uboot command:
	504MHz:		plltest gp0 0xc001022a 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	516MHz:		plltest gp0 0xc001022b 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	528MHz:		plltest gp0 0xc001022c 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	540MHz:		plltest gp0 0xc001022d 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	552MHz:		plltest gp0 0xc001022e 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	564MHz:		plltest gp0 0xc001022f 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	576MHz:		plltest gp0 0xc0010230 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	588MHz:		plltest gp0 0xc0010231 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	600MHz:		plltest gp0 0xc0010232 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	612MHz:		plltest gp0 0xc0010233 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	624MHz:		plltest gp0 0xc0010234 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	636MHz:		plltest gp0 0xc0010235 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	648MHz:		plltest gp0 0xc0010236 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	660MHz:		plltest gp0 0xc0010237 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	672MHz:		plltest gp0 0xc0010238 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	684MHz:		plltest gp0 0xc0010239 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	696MHz:		plltest gp0 0xc001023a 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	708MHz:		plltest gp0 0xc001023b 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	720MHz:		plltest gp0 0xc001023c 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	732MHz:		plltest gp0 0xc001023d 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	all:		plltest gp0


Part3: T962 usage

1. sys pll

	test pass print: sys pll test pass!
	test fail print: sys pll test failed!

	uboot command(select one as needed):
	 960MHz:	plltest sys 0x60000228 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1056MHz:	plltest sys 0x6000022c 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1152MHz:	plltest sys 0x60000230 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1248MHz:	plltest sys 0x60000234 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1344MHz:	plltest sys 0x60000238 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1440MHz:	plltest sys 0x6000023c 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1536MHz:	plltest sys 0x60000240 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	1632MHz:	plltest sys 0x60000244 0xc4258100 0xb7400000 0x0a59a288 0x0040002d 0x7c700007
	all:		plltest sys

2. hdmi pll

	test pass print: hdmi pll test pass!
	test fail print: hdmi pll test failed!

	uboot command:
	5940MHz:	plltest hdmi 0x400002f7 0x800cb200 0x865f30c4 0x0c8e0000 0x001fa729 0x01a31500
	4320MHz:	plltest hdmi 0x400002b4 0x800cb000 0x865f30c4 0x0c8e0000 0x001fa729 0x01a31500
	3712MHz:	plltest hdmi 0x4000029a 0x800cb2c0 0x865f30c4 0x0c8e0000 0x001fa729 0x01a31500
	3712MHz:	plltest hdmi 0x4000027b 0x800cb300 0x865f30c4 0x0c8e0000 0x001fa729 0x01a31500
	all:		plltest hdmi

3. gp0 pll

	test pass print: gp0 pll test pass!
	test fail print: gp0 pll test failed!

	uboot command:
	504MHz:		plltest gp0 0xc001022a 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	516MHz:		plltest gp0 0xc001022b 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	528MHz:		plltest gp0 0xc001022c 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	540MHz:		plltest gp0 0xc001022d 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	552MHz:		plltest gp0 0xc001022e 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	564MHz:		plltest gp0 0xc001022f 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	576MHz:		plltest gp0 0xc0010230 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	588MHz:		plltest gp0 0xc0010231 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	600MHz:		plltest gp0 0xc0010232 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	612MHz:		plltest gp0 0xc0010233 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	624MHz:		plltest gp0 0xc0010234 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	636MHz:		plltest gp0 0xc0010235 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	648MHz:		plltest gp0 0xc0010236 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	660MHz:		plltest gp0 0xc0010237 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	672MHz:		plltest gp0 0xc0010238 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	684MHz:		plltest gp0 0xc0010239 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	696MHz:		plltest gp0 0xc001023a 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	708MHz:		plltest gp0 0xc001023b 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	720MHz:		plltest gp0 0xc001023c 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	732MHz:		plltest gp0 0xc001023d 0xb75020be 0x0a59a288 0xc000004d 0x00078000
	all:		plltest gp0


T962: please check Part3

Part4: G12A usage

1. sys pll

	test pass print: sys pll test pass!
	test fail print: sys pll test failed!

	uboot command(select one as needed):
	 960MHz:	plltest sys 0x380204a0 0x0 0x0 0x48681c00 0x88770290 0x39272000 0x56540000
	1056MHz:	plltest sys 0x380204b0 0x0 0x0 0x48681c00 0x88770290 0x39272000 0x56540000
	1152MHz:	plltest sys 0x380204c0 0x0 0x0 0x48681c00 0x88770290 0x39272000 0x56540000
	1248MHz:	plltest sys 0x380204d0 0x0 0x0 0x48681c00 0x88770290 0x39272000 0x56540000
	1344MHz:	plltest sys 0x380204e0 0x0 0x0 0x48681c00 0x88770290 0x39272000 0x56540000
	1440MHz:	plltest sys 0x380204f0 0x0 0x0 0x48681c00 0x88770290 0x39272000 0x56540000
	1536MHz:	plltest sys 0x38010480 0x0 0x0 0x48681c00 0x88770290 0x39272000 0x56540000
	1632MHz:	plltest sys 0x38010488 0x0 0x0 0x48681c00 0x88770290 0x39272000 0x56540000
	all:		plltest sys

2. hdmi pll

	test pass print: hdmi pll test pass!
	test fail print: hdmi pll test failed!

	uboot command:
	5405MHz:	plltest hdmi 0x3b0004e1 0x00007333 0x00000000 0x0a691c00 0x33771290 0x39270000 0x50540000
	4455MHz:	plltest hdmi 0x3b0004b9 0x00014000 0x00000000 0x0a691c00 0x33771290 0x39270000 0x50540000
	3450MHz:	plltest hdmi 0x3b00048f 0x00018000 0x00000000 0x0a691c00 0x33771290 0x39270000 0x50540000
	2970MHz:	plltest hdmi 0x3b00047b 0x00018000 0x00000000 0x0a691c00 0x33771290 0x39270000 0x50540000
	all:		plltest hdmi

3. gp0 pll

	test pass print: gp0 pll test pass!
	test fail print: gp0 pll test failed!

	uboot command:
	408MHz:		plltest gp0 0x38070488 0 0 0x48681c00 0x33771290 0x39272000 0x56540000
	600MHz:		plltest gp0 0x380704c8 0 0 0x48681c00 0x33771290 0x39272000 0x56540000
	696MHz:		plltest gp0 0x380704e8 0 0 0x48681c00 0x33771290 0x39272000 0x56540000
	792MHz:		plltest gp0 0x38060484 0 0 0x48681c00 0x33771290 0x39272000 0x56540000
	846MHz:		plltest gp0 0x3806048d 0 0 0x48681c00 0x33771290 0x39272000 0x56540000
	912MHz:		plltest gp0 0x38060498 0 0 0x48681c00 0x33771290 0x39272000 0x56540000
	all:		plltest gp0

4. hifi pll

	test pass print: hifi pll test pass!
	test fail print: hifi pll test failed!

	uboot command:
	1008MHz:	plltest hifi 0x380604A8 0 0 0x48681c00 0x33771290 0x39272000 0x56540000
	1104MHz:	plltest hifi 0x380604B8 0 0 0x48681c00 0x33771290 0x39272000 0x56540000
	1200MHz:	plltest hifi 0x380604C8 0 0 0x48681c00 0x33771290 0x39272000 0x56540000
	1296MHz:	plltest hifi 0x380604D8 0 0 0x48681c00 0x33771290 0x39272000 0x56540000
	1398MHz:	plltest hifi 0x380604E9 0 0 0x48681c00 0x33771290 0x39272000 0x56540000
	1494MHz:	plltest hifi 0x380604F9 0 0 0x48681c00 0x33771290 0x39272000 0x56540000
	all:		plltest hifi

5. pcie pll

	test pass print: pcie pll test pass!
	test fail print: pcie pll test failed!

	uboot command:
	100MHz:	    plltest pcie 0x200c04c8 0 0x00001100 0x10058e00 0x000100c0 0x68000048
	all:		plltest pcie

6. ethphy pll

	test pass print: ethphy pll test pass!
	test fail print: ethphy pll test failed!

	uboot command:
	500MHz:	    plltest ethphy 0x9c0040a 0x927e0000 0xac5f49e5 0x00000000
	all:		plltest ethphy

7. usbphy pll

	test pass print: usbphy pll test pass!
	test fail print: usbphy pll test failed!

	uboot command:
	480MHz:	    plltest usbphy 0x8000fff 0x34 0x78000 0 0x09400414 0x927E0000 0xac5f69e5 0xfe18
	all:		plltest usbphy

	NOTICE:usbphy pll does not connect clk_msr£¬so here only judge whether is locked



Part5: A1 usage

1. sys pll

	test pass print: sys pll test pass!
	test fail print: sys pll test failed!

	uboot command(select one as needed):
	768MHz:	    plltest sys 0x01f18420 0x01800000 0x00001100 0x10022300 0x00300000
	888MHz:	    plltest sys 0x01f18425 0x01800000 0x00001100 0x10022300 0x00300000
	960MHz:	    plltest sys 0x01f18428 0x01800000 0x00001100 0x10022300 0x00300000
	1008MHz:	plltest sys 0x01f1842A 0x01800000 0x00001100 0x10022300 0x00300000
	1152MHz:	plltest sys 0x01f18430 0x01800000 0x00001100 0x10022300 0x00300000
	1248MHz:	plltest sys 0x01f18434 0x01800000 0x00001100 0x10022300 0x00300000
	1392MHz:	plltest sys 0x01f1843A 0x01800000 0x00001100 0x10022300 0x00300000
	1536MHz:	plltest sys 0x01f18440 0x01800000 0x00001100 0x10022300 0x00300000
	all:		plltest sys

2. hifi pll

	test pass print: hifi pll test pass!
	test fail print: hifi pll test failed!

	uboot command:
	864MHz:	    plltest hifi 0x01f18424 0x01800000 0x00001100 0x10022300 0x00300000
	960MHz:	    plltest hifi 0x01f18428 0x01800000 0x00001100 0x10022300 0x00300000
	1056MHz:    plltest hifi 0x01f1842C 0x01800000 0x00001100 0x10022300 0x00300000
	1200MHz:    plltest hifi 0x01f18432 0x01800000 0x00001100 0x10022300 0x00300000
	1320MHz:    plltest hifi 0x01f18437 0x01800000 0x00001100 0x10022300 0x00300000
	1536MHz:    plltest hifi 0x01f18440 0x01800000 0x00001100 0x10022300 0x00300000
	all:	    plltest hifi

3. usbphy pll

	test pass print: usbphy pll test pass!
	test fail print: usbphy pll test failed!

	uboot command:
	480MHz:	    plltest usbphy 0x09400414 0x927e0000 0xac5f69e5
	all:		plltest usbphy

	NOTICE:usbphy pll does not connect clk_msr£¬so here only judge whether is locked

Part6: C1 usage

1. sys pll

	test pass print: sys pll test pass!
	test fail print: sys pll test failed!

	uboot command(select one as needed):
	768MHz:	    plltest sys 0x01f18420 0x01800000 0x00001100 0x10022300 0x00300000
	888MHz:	    plltest sys 0x01f18425 0x01800000 0x00001100 0x10022300 0x00300000
	960MHz:	    plltest sys 0x01f18428 0x01800000 0x00001100 0x10022300 0x00300000
	1008MHz:	plltest sys 0x01f1842A 0x01800000 0x00001100 0x10022300 0x00300000
	1152MHz:	plltest sys 0x01f18430 0x01800000 0x00001100 0x10022300 0x00300000
	1248MHz:	plltest sys 0x01f18434 0x01800000 0x00001100 0x10022300 0x00300000
	1392MHz:	plltest sys 0x01f1843A 0x01800000 0x00001100 0x10022300 0x00300000
	1536MHz:	plltest sys 0x01f18440 0x01800000 0x00001100 0x10022300 0x00300000
	all:		plltest sys

2. hifi pll

	test pass print: hifi pll test pass!
	test fail print: hifi pll test failed!

	uboot command:
	864MHz:	    plltest hifi 0x01f18424 0x01800000 0x00001100 0x10022300 0x00300000
	960MHz:	    plltest hifi 0x01f18428 0x01800000 0x00001100 0x10022300 0x00300000
	1056MHz:    plltest hifi 0x01f1842C 0x01800000 0x00001100 0x10022300 0x00300000
	1200MHz:    plltest hifi 0x01f18432 0x01800000 0x00001100 0x10022300 0x00300000
	1320MHz:    plltest hifi 0x01f18437 0x01800000 0x00001100 0x10022300 0x00300000
	1536MHz:    plltest hifi 0x01f18440 0x01800000 0x00001100 0x10022300 0x00300000
	all:	    plltest hifi

3. gp pll

	test pass print: gp pll test pass!
	test fail print: gp pll test failed!

	uboot command:
	864MHz:	    plltest gp 0x01f18424 0x01800000 0x00001100 0x10022300 0x00300000 0x00088888
	960MHz:	    plltest gp 0x01f18428 0x01800000 0x00001100 0x10022300 0x00300000 0x00088888
	1056MHz:    plltest gp 0x01f1842C 0x01800000 0x00001100 0x10022300 0x00300000 0x00088888
	1200MHz:    plltest gp 0x01f18432 0x01800000 0x00001100 0x10022300 0x00300000 0x00088888
	1320MHz:    plltest gp 0x01f18437 0x01800000 0x00001100 0x10022300 0x00300000 0x00088888
	1536MHz:    plltest gp 0x01f18440 0x01800000 0x00001100 0x10022300 0x00300000 0x00088888
	all:	    plltest gp

4. dds pll

	test pass print: dds pll test pass!
	test fail print: dds pll test failed!

	uboot command:
	32MHz:	    plltest dds 0x4 0x50041340 0x0 0x0 0x000003e8 0x00000040
	50MHz:	    plltest dds 0x4 0x50041340 0x0 0x0 0x000003e8 0x00000064
	60MHz:      plltest dds 0x4 0x50041340 0x0 0x0 0x0000052  0x000000A
	80MHz:      plltest dds 0x4 0x50041340 0x0 0x0 0x00000271 0x00000064
	100MHz:     plltest dds 0x4 0x50041340 0x0 0x0 0x000005   0x0000001
	all:	    plltest dds

5. usbphy pll

	test pass print: usbphy pll test pass!
	test fail print: usbphy pll test failed!

	uboot command:
	480MHz:	    plltest usbphy 0x09400414 0x927e0000 0xac5f69e5
	all:		plltest usbphy

	NOTICE:usbphy pll does not connect clk_msr£¬so here only judge whether is locked

6. ethphy pll

	test pass print: ethphy pll test pass!
	test fail print: ethphy pll test failed!

	uboot command:
	500MHz:	       plltest ethphy 0x9c0040a 0x927e0000 0xac5f49e5
	all:	       plltest ethphy


*/

#include <common.h>
#include <command.h>
#include <asm/arch/pll.h>

static int do_plltest(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]) {
	if (argc < 2) {
		return -1;
	}

	pll_test(argc, argv);

	return 0;
}

U_BOOT_CMD(
	plltest,	CONFIG_SYS_MAXARGS,	1,	do_plltest,
	"test pll",
	"\n"
	"	- test pll and report result\n\n"
	"plltest [all/sys/hdmi/gp0/hifi/pcie/ethphy/usbphy] [pll_cntl pll_cntl2 ...]\n\n"
	"examples:\n"
	"plltest all                                 - test all plls\n"
	"plltest sys                                 - test sys pll with all preset freq\n"
	"plltest sys cntl cntl1 cntl2 ...            - test sys pll with params\n"
	"plltest hdmi                                - test hdmi pll with all preset freq\n"
	"plltest hdmi cntl cntl1 cntl2 ...           - test hdmi pll with params\n"
	"plltest gp0                                 - test gp0 pll with all preset freq\n"
	"plltest gp0 cntl cntl1 cntl2 ...            - test gp0 pll with params\n"
	"plltest hifi                                - test hifi pll with all preset freq\n"
	"plltest hifi cntl cntl1 cntl2 ...           - test hifi pll with params\n"
	"plltest pcie                                - test pcie pll with all preset freq\n"
	"plltest pcie cntl cntl1 cntl2 ...           - test pcie pll with params\n"
	"plltest ethphy                              - test ethphy pll with all preset freq\n"
	"plltest ethphy cntl cntl1 cntl2 ...         - test ethphy pll with params\n"
	"plltest usbphy                              - test usbphy pll with all preset freq\n"
	"plltest usbphy cntl cntl1 cntl2 ...         - test usbphy pll with params\n"
	"plltest dds                                 - test dds pll with all preset freq\n"
	"plltest dds cntl cntl1 cntl2 ...            - test dds pll with params\n"
);
