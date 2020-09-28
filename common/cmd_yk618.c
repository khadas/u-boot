/*
 * Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Define _STDBOOL_H here to avoid macro expansion of true and false.
 * If the future code requires macro true or false, remove this define
 * and undef true and false before U_BOOT_CMD. This define and comment
 * shall be removed if change to U_BOOT_CMD is made to take string
 * instead of stringifying it.
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <linux/string.h>

#define I2C_YK618_CONTROL_REG      ((volatile unsigned int *)0xff805000)
#define I2C_YK618_SLAVE_ADDR       ((volatile unsigned int *)0xff805004)
#define I2C_YK618_TOKEN_LIST_REG0  ((volatile unsigned int *)0xff805008)
#define I2C_YK618_TOKEN_LIST_REG1  ((volatile unsigned int *)0xff80500c)
#define I2C_YK618_TOKEN_WDATA_REG0 ((volatile unsigned int *)0xff805010)
#define I2C_YK618_TOKEN_WDATA_REG1 ((volatile unsigned int *)0xff805014)
#define I2C_YK618_TOKEN_RDATA_REG0 ((volatile unsigned int *)0xff805018)
#define I2C_YK618_TOKEN_RDATA_REG1 ((volatile unsigned int *)0xff80501c)

#define I2C_YK618_END               0x0
#define I2C_YK618_START             0x1
#define I2C_YK618_SLAVE_ADDR_WRITE  0x2
#define I2C_YK618_SLAVE_ADDR_READ   0x3
#define I2C_YK618_DATA              0x4
#define I2C_YK618_DATA_LAST         0x5
#define I2C_YK618_STOP              0x6
#define I2C_YK618_WAIT_CNT        (24 * 8 * 1000)

#define I2C_YK618_DEVICE_ADDR (0x34<<1)//device ADDR

#define P_AO_TIMER_E1		(*((volatile unsigned *)(0xff800000 + (0xf9 << 2))))
static unsigned int get_time(void)
{
	return P_AO_TIMER_E1;
}

static void _udelay(unsigned int us)
{
	unsigned int t0 = get_time();

	while (get_time() - t0 <= us)
		;
}

int hard_I2C_YK618_check_error(void)
{
	if (*I2C_YK618_CONTROL_REG & 0x08) {
		printf("I2C_YK618 err, CTRL:\n");
		return -1;
	}
	return 0;
}


int hard_I2C_YK618_wait_complete(void)
{
	int delay = 0;

	while (delay < I2C_YK618_WAIT_CNT) {
		if (!((*I2C_YK618_CONTROL_REG) & (1 << 2))) {     // idle
			break;
		}
		delay++;
	}
	if (delay >= I2C_YK618_WAIT_CNT) {
		printf("I2C_YK618 timeout\n");
	}
	return hard_I2C_YK618_check_error();
}

//uncomment this function if you need read back

unsigned char hard_I2C_YK618_read8(unsigned char SlaveAddr, unsigned char RegAddr)
{
	// Set the I2C_YK618 Address
	(*I2C_YK618_SLAVE_ADDR) = ((*I2C_YK618_SLAVE_ADDR) & ~0xff) | SlaveAddr;
	// Fill the token registers

	(*I2C_YK618_TOKEN_LIST_REG0) = (I2C_YK618_END  << 24) |
				 (I2C_YK618_DATA_LAST << 20)        |  // Read Data
				 (I2C_YK618_SLAVE_ADDR_READ << 16)  |
				 (I2C_YK618_START << 12)            |
				 (I2C_YK618_DATA << 8)              | // Read RegAddr
				 (I2C_YK618_SLAVE_ADDR_WRITE << 4)  |
				 (I2C_YK618_START << 0);
	// Fill the write data registers

	(*I2C_YK618_TOKEN_WDATA_REG0) = (RegAddr << 0);
	// Start and Wait
	(*I2C_YK618_CONTROL_REG) &= ~(1 << 0);   // Clear the start bit
	(*I2C_YK618_CONTROL_REG) |= (1 << 0);   // Set the start bit

	hard_I2C_YK618_wait_complete();

	return( (unsigned char)((*I2C_YK618_TOKEN_RDATA_REG0) & 0xFF) );
}

void hard_I2C_YK618_write8(unsigned char SlaveAddr, unsigned char RegAddr, unsigned char Data)
{
	// Set the I2C_YK618 Address
	(*I2C_YK618_SLAVE_ADDR) = ((*I2C_YK618_SLAVE_ADDR) & ~0xff) | SlaveAddr;

	// Fill the token registers
	(*I2C_YK618_TOKEN_LIST_REG0) = (I2C_YK618_END  << 16)|(I2C_YK618_DATA << 12)|(I2C_YK618_DATA << 8)| \
				 (I2C_YK618_SLAVE_ADDR_WRITE << 4)| \
				 (I2C_YK618_START << 0);

	// Fill the write data registers
	(*I2C_YK618_TOKEN_WDATA_REG0) = (Data << 8) | (RegAddr << 0);
	// Start and Wait
	(*I2C_YK618_CONTROL_REG) &= ~(1 << 0);   // Clear the start bit
	(*I2C_YK618_CONTROL_REG) |= (1 << 0);   // Set the start bit

	hard_I2C_YK618_wait_complete();
}

void yk618_set_volt(char addr,int volt)
{
	int val;

	//volt -= 600;
	//val = volt / 20 + ((volt % 20) ? 1 : 0);
	val = (volt - 0x80);

	hard_I2C_YK618_write8(I2C_YK618_DEVICE_ADDR, addr, val);
	val = 0;
	_udelay(100);
	while (!(hard_I2C_YK618_read8(I2C_YK618_DEVICE_ADDR, addr) & 0x80) && val < 1000) {
		_udelay(2);
		val++;
	}
	_udelay(200);
}

static int do_YK618_set_volt(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (argc < 3) {
		printf(" Please enter the register value you want to set \n");
		return 1;
    }

    int voltage;
    int addr;
    addr = (int)simple_strtol(argv[1],NULL,16);
    voltage = (int)simple_strtol(argv[2],NULL,16);

    printf("reg:0x%x\t",addr);
    printf("valu:0x%x\n",voltage);

    switch (addr) {
	case 0x22:
	    if (voltage < 0x80 || voltage > 0xb0) {
			printf("error : this value is out of range ,the range is 0x80 to 0xaf \n");
			return 1;
		}
		break;
	case 0x23:
	    if (voltage < 0x80 || voltage > 0xbf) {
			printf("error : this value is out of range ,the range is 0x80 to 0xbf \n");
			return 1;
		}
		break;
	#if 0
	case 0x21:
	    break;
	case 0x24:
	    break;
	case 0x25:
	    break;
	#endif

	default:
		printf("eror : only support dcdc2/dcdc3");
		return 0;
		break;
    }

	yk618_set_volt( addr, voltage);

	return 0;
}

U_BOOT_CMD(
        write_ykreg,    3,      0,      do_YK618_set_volt,
        " (YK618) set DCDC volatage",
	"  :write_ykreg reg valu "
);
