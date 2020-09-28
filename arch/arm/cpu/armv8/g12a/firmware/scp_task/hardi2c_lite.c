#include <config.h>
#include "config.h"
#include "registers.h"
#include "task_apis.h"
#if 0
#define I2C_BASE_ADDR		(0xc11087c0)         // 
#define I2C_CONTROL_REG      ((volatile unsigned int *)0xc11087c0)
#define I2C_SLAVE_ADDR       ((volatile unsigned int *)0xc11087c4)
#define I2C_TOKEN_LIST_REG0  ((volatile unsigned int *)0xc11087c8)
#define I2C_TOKEN_LIST_REG1  ((volatile unsigned int *)0xc11087cc)
#define I2C_TOKEN_WDATA_REG0 ((volatile unsigned int *)0xc11087d0)
#define I2C_TOKEN_WDATA_REG1 ((volatile unsigned int *)0xc11087d4)
#define I2C_TOKEN_RDATA_REG0 ((volatile unsigned int *)0xc11087d8)
#define I2C_TOKEN_RDATA_REG1 ((volatile unsigned int *)0xc11087dc)
#define P_PIN_MUX_REG1       ((volatile unsigned int *)0xc88344b4)
#define P_PIN_MUX_REG2       ((volatile unsigned int *)0xc88344b8)
#endif
#define I2C_BASE_ADDR		(0xff805000)         // i2c b FF805000+7800*4=1e00 +
#define I2C_CONTROL_REG      ((volatile unsigned int *)0xff805000)
#define I2C_SLAVE_ADDR       ((volatile unsigned int *)0xff805004)
#define I2C_TOKEN_LIST_REG0  ((volatile unsigned int *)0xff805008)
#define I2C_TOKEN_LIST_REG1  ((volatile unsigned int *)0xff80500c)
#define I2C_TOKEN_WDATA_REG0 ((volatile unsigned int *)0xff805010)
#define I2C_TOKEN_WDATA_REG1 ((volatile unsigned int *)0xff805014)
#define I2C_TOKEN_RDATA_REG0 ((volatile unsigned int *)0xff805018)
#define I2C_TOKEN_RDATA_REG1 ((volatile unsigned int *)0xff80501c)
#define P_PIN_MUX_REG3       ((volatile unsigned int *)(0xff800000 + (0x06 << 2))) //i2cb gpioz8-9 ff634400 +23*4




#define I2C_END               0x0
#define I2C_START             0x1
#define I2C_SLAVE_ADDR_WRITE  0x2
#define I2C_SLAVE_ADDR_READ   0x3
#define I2C_DATA              0x4
#define I2C_DATA_LAST         0x5
#define I2C_STOP              0x6

#define I2C_WAIT_CNT        (24 * 8 * 1000)

void hard_i2c_init(void)
{
	unsigned int nCLK81;
	unsigned int reg;

	nCLK81 = 500; //24 000 000/400 000=60
	(*I2C_CONTROL_REG) = ((*I2C_CONTROL_REG) & ~(0x3FF << 12)) |
			     (nCLK81 << 12);
	reg = *P_PIN_MUX_REG3;
	reg |= ((1<<18) | (1<<22));
	*P_PIN_MUX_REG3 = reg;
}

int hard_i2c_check_error(void)
{
	if (*I2C_CONTROL_REG & 0x08) {
		uart_puts("i2c err, CTRL:\n");
		uart_put_hex(*I2C_CONTROL_REG, 32);
		uart_puts("\n");
		return -1;
	}
	return 0;
}

int hard_i2c_wait_complete(void)
{
	int delay = 0;

	while (delay < I2C_WAIT_CNT) {
		if (!((*I2C_CONTROL_REG) & (1 << 2))) {     // idle
			break;
		}
		delay++;
	}
	if (delay >= I2C_WAIT_CNT) {
		uart_puts("i2c timeout\n");
	}
	return hard_i2c_check_error();
}

//uncomment this function if you need read back

unsigned char hard_i2c_read8(unsigned char SlaveAddr, unsigned char RegAddr)
{
	// Set the I2C Address
	(*I2C_SLAVE_ADDR) = ((*I2C_SLAVE_ADDR) & ~0xff) | SlaveAddr;
	// Fill the token registers

	(*I2C_TOKEN_LIST_REG0) = (I2C_END  << 24) |
				 (I2C_DATA_LAST << 20)        |  // Read Data
				 (I2C_SLAVE_ADDR_READ << 16)  |
				 (I2C_START << 12)            |
				 (I2C_DATA << 8)              | // Read RegAddr
				 (I2C_SLAVE_ADDR_WRITE << 4)  |
				 (I2C_START << 0);
	// Fill the write data registers

	(*I2C_TOKEN_WDATA_REG0) = (RegAddr << 0);
	// Start and Wait
	(*I2C_CONTROL_REG) &= ~(1 << 0);   // Clear the start bit
	(*I2C_CONTROL_REG) |= (1 << 0);   // Set the start bit

	hard_i2c_wait_complete();

	return( (unsigned char)((*I2C_TOKEN_RDATA_REG0) & 0xFF) );
}

void hard_i2c_write8(unsigned char SlaveAddr, unsigned char RegAddr, unsigned char Data)
{
	// Set the I2C Address
	(*I2C_SLAVE_ADDR) = ((*I2C_SLAVE_ADDR) & ~0xff) | SlaveAddr;

	// Fill the token registers
	(*I2C_TOKEN_LIST_REG0) = (I2C_END  << 16)|(I2C_DATA << 12)|(I2C_DATA << 8)| \
				 (I2C_SLAVE_ADDR_WRITE << 4)| \
				 (I2C_START << 0);

	// Fill the write data registers
	(*I2C_TOKEN_WDATA_REG0) = (Data << 8) | (RegAddr << 0);
	// Start and Wait
	(*I2C_CONTROL_REG) &= ~(1 << 0);   // Clear the start bit
	(*I2C_CONTROL_REG) |= (1 << 0);   // Set the start bit

	hard_i2c_wait_complete();
}
void hard_i2c_set_bits(unsigned char SlaveAddr, unsigned char RegAddr,unsigned char bits, unsigned char mask)
{
	unsigned char val;

	val = hard_i2c_read8(SlaveAddr, RegAddr);
	val &= ~(mask);
	val |=  (bits & mask);
	hard_i2c_write8(SlaveAddr, RegAddr, val);
}

// axp216 api
#define I2C_DEVICE_ADDR (0x34<<1)//device ADDR

#define DCDC2_OUT_CTL 0x22 // vddee
#define DCDC3_OUT_CTL 0x23 // vddcpu

#define DCDCX_EN_CTL 0x10 // DCDC1/2/3/4/5 ... out_put_enable/disable
#define YK618_STATUS							(0x00)
#define YK618_STATUS1							(0x01)
#define YK618_REBOOT             (0x31)
#define YK618_OFF_CTL             (0x32)
#define YK618_CHARGER_REBOOT 0

void yk618_set_dcdc2_volt(int volt){
	int val;
	
	volt -= 600;
	val = volt / 20 + ((volt % 20) ? 1 : 0);

	hard_i2c_write8(I2C_DEVICE_ADDR, DCDC2_OUT_CTL, val);
	val = 0;
	_udelay(100);
	while (!(hard_i2c_read8(I2C_DEVICE_ADDR, DCDC2_OUT_CTL) & 0x80) && val < 1000) {
		_udelay(2);
		val++;
	}
	_udelay(200);

}

void yk618_set_dcdc3_volt(int volt){
	int val;
	
	volt -= 600;
	val = volt / 20 + ((volt % 20) ? 1 : 0);

	hard_i2c_write8(I2C_DEVICE_ADDR, DCDC3_OUT_CTL, val);
	val = 0;
	_udelay(100);
	while (!(hard_i2c_read8(I2C_DEVICE_ADDR, DCDC3_OUT_CTL) & 0x80) && val < 1000) {
		_udelay(2);
		val++;
	}
	_udelay(200);

}
/*
*dcdcx:dcdcx=1 -->dcdc1 .... dcdcx=5--> dcdc5
*enable: 0--> disable 1--> enable
*/
void yk618_enable_dcdc_x(int dcdcx,int enable){
	hard_i2c_set_bits(I2C_DEVICE_ADDR, DCDCX_EN_CTL, enable << dcdcx, 1<<dcdcx);
}

void yk618_power_off(void){
	unsigned char status, status1;

	status = hard_i2c_read8(I2C_DEVICE_ADDR, YK618_STATUS);
	status1 = hard_i2c_read8(I2C_DEVICE_ADDR, YK618_STATUS1);
	_udelay(10000);
	if (YK618_CHARGER_REBOOT == 1 && (status & ((1<<6) | (1<<4))) && (status1 & (1<<5)))
		hard_i2c_set_bits(I2C_DEVICE_ADDR, YK618_REBOOT, 1 << 6, 1<<6);
	else
		hard_i2c_set_bits(I2C_DEVICE_ADDR, YK618_OFF_CTL, 1 << 7, 1<<7);
	_udelay(20000);
}

/*
	rtc 
*/
#define RTC_DEVICE_ADDR (0x51 << 1)
#define RTC_REG_ST2		0x01
#define TCS9563_BIT_AIE		(1 << 1)
#define TCS9563_BIT_AF		(1 << 3)
int rtc_read_irq(void)
{
	unsigned char val;
	val = hard_i2c_read8(RTC_DEVICE_ADDR, RTC_REG_ST2);
	if (val & TCS9563_BIT_AF)
	{
		val &= ~(TCS9563_BIT_AIE | TCS9563_BIT_AF);
		hard_i2c_write8(RTC_DEVICE_ADDR, RTC_REG_ST2, val);
		return 0;
	}
	return -1;
} 


#if 0
unsigned char hard_i2c_read168(unsigned char SlaveAddr, unsigned short RegAddr)
{
	unsigned char *pr = (unsigned char*)&RegAddr;
	unsigned char data;

	// Set the I2C Address
	(*I2C_SLAVE_ADDR) = ((*I2C_SLAVE_ADDR) & ~0xff) | SlaveAddr;
	// Fill the token registers
	(*I2C_TOKEN_LIST_REG0) = (I2C_END << 28)              |
				 (I2C_DATA_LAST << 24)        |  // Read Data
				 (I2C_SLAVE_ADDR_READ << 20)  |
				 (I2C_START << 16)            |
				 (I2C_DATA << 12)             |
				 (I2C_DATA << 8)              |  // Read RegAddr
				 (I2C_SLAVE_ADDR_WRITE << 4)  |
				 (I2C_START << 0);

	// Fill the write data registers
	(*I2C_TOKEN_WDATA_REG0) = (*(pr + 1) << 8) | *pr;
	// Start and Wait
	(*I2C_CONTROL_REG) &= ~(1 << 0);   // Clear the start bit
	(*I2C_CONTROL_REG) |= (1 << 0);   // Set the start bit

	hard_i2c_wait_complete();

	data = *I2C_TOKEN_RDATA_REG0 & 0xff;
	return data;
}

void hard_i2c_write168(unsigned char SlaveAddr,
	unsigned short RegAddr, unsigned char Data)
{
	unsigned char *pr = (unsigned char*)&RegAddr;

	// Set the I2C Address
	(*I2C_SLAVE_ADDR) = ((*I2C_SLAVE_ADDR) & ~0xff) | SlaveAddr;
	// Fill the token registers
	(*I2C_TOKEN_LIST_REG0) = (I2C_END << 20)              |
				 (I2C_DATA << 16)             | // Write Data
				 (I2C_DATA << 12)             |
				 (I2C_DATA << 8)              | // Write RegAddr
				 (I2C_SLAVE_ADDR_WRITE << 4)  |
				 (I2C_START << 0);

	// Fill the write data registers
	(*I2C_TOKEN_WDATA_REG0) = (Data<<16) | (*(pr + 1) << 8) | *pr;
	// Start and Wait
	(*I2C_CONTROL_REG) &= ~(1 << 0);   // Clear the start bit
	(*I2C_CONTROL_REG) |= (1 << 0);   // Set the start bit

	hard_i2c_wait_complete();
}
#endif
