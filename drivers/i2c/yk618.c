#include <common.h>
#include <div64.h>
#include <asm/setup.h>
#include <asm/arch/i2c.h>
#include <asm/arch/gpio.h>
#include <asm-generic/errno.h>
#include <aml_i2c.h>
#include <yk618.h>


#define DBG_PSY_MSG(format,args...)   if(yk618_debug) printf("[YK618]"format,##args)

#define ABS(x)				((x) >0 ? (x) : -(x) )

struct yk618_adc_res {//struct change
	uint16_t vbat_res;
	uint16_t ocvbat_res;
	uint16_t ibat_res;
	uint16_t ichar_res;
	uint16_t idischar_res;
	uint16_t vac_res;
	uint16_t iac_res;
	uint16_t vusb_res;
	uint16_t iusb_res;
};


#define YK618_I2C_ADDR 	0x34
#define MAX_I2C_BUFF 64
#define ALDO  1
#define ELDO  2

int yk618_write(uint8_t reg, uint8_t val)
{
	int ret;
	uint8_t buff[2];
    buff[0] = reg;
    buff[1] = val;
    struct i2c_msg msg[] = {
        {
        .addr = YK618_I2C_ADDR,
        .flags = 0,
        .len = 2,
        .buf = buff,
        }
    };

	ret = aml_i2c_xfer(msg, 1);
    if (ret < 0) {
        printf("%s: i2c transfer failed\n", __FUNCTION__);
		return ret;
    }
	return 0;
}


int yk618_writes(uint8_t reg, uint8_t *val, int len)
{
	int ret;
	uint8_t buff[MAX_I2C_BUFF + 1];
	if(len > MAX_I2C_BUFF)
	{
		printf("%s: i2c len must <= %d\n", __FUNCTION__, MAX_I2C_BUFF);
		return -1;
	}
    buff[0] = reg;
	memcpy((buff+1), val, len);
    struct i2c_msg msg[] = {
        {
        .addr = YK618_I2C_ADDR,
        .flags = 0,
        .len = len + 1,
        .buf = buff,
        }
    };

	ret = aml_i2c_xfer(msg, 1);
    if (ret < 0) {
        printf("%s: i2c transfer failed\n", __FUNCTION__);
		return ret;
    }
	return 0;
}


int yk618_read(uint8_t reg, uint8_t *val)
{
	int ret;
	struct i2c_msg msgs[] = {
        {
            .addr = YK618_I2C_ADDR,
            .flags = 0,
            .len = 1,
            .buf = &reg,
        },
        {
            .addr = YK618_I2C_ADDR,
            .flags = I2C_M_RD,
            .len = 1,
            .buf = val,
        }
    };
	ret = aml_i2c_xfer(msgs, 2);
    if (ret < 0) {
        printf("%s: i2c transfer failed\n", __FUNCTION__);
		return ret;
    }

    return 0;
}


int yk618_reads(uint8_t reg,  uint8_t *val, int len)
{
	int ret;
	struct i2c_msg msgs[] = {
        {
            .addr = YK618_I2C_ADDR,
            .flags = 0,
            .len = 1,
            .buf = &reg,
        },
        {
            .addr = YK618_I2C_ADDR,
            .flags = I2C_M_RD,
            .len = len,
            .buf = val,
        }
    };
	ret = aml_i2c_xfer(msgs, 2);
    if (ret < 0) {
        printf("%s: i2c transfer failed\n", __FUNCTION__);
		return ret;
    }
    return 0;
}

int yk618_set_bits(int reg, uint8_t bit_mask)
{
	uint8_t reg_val;
	int ret = 0;

	ret = yk618_read(reg, &reg_val);
	if (ret)
		goto out;

	if ((reg_val & bit_mask) != bit_mask) {
		reg_val |= bit_mask;
		ret = yk618_write(reg, reg_val);
	}
out:
	return ret;
}

int yk618_clr_bits(int reg, uint8_t bit_mask)
{
	uint8_t reg_val;
	int ret = 0;

	ret = yk618_read(reg, &reg_val);
	if (ret)
		goto out;

	if (reg_val & bit_mask) {
		reg_val &= ~bit_mask;
		ret = yk618_write(reg, reg_val);
	}
out:
	return ret;
}

int yk618_update(uint8_t reg, uint8_t val, uint8_t mask)
{
	uint8_t reg_val;
	int ret = 0;

	ret = yk618_read(reg, &reg_val);
	if (ret)
		goto out;

	if ((reg_val & mask) != val) {
		reg_val = (reg_val & ~mask) | (val & mask);
		ret = yk618_write(reg, reg_val);
	}
out:
	return ret;
}

static int find_idx(int start, int target, uint8_t step, uint8_t size)
{
    int i = 0; 
    do { 
        if (start >= target) {
            break;    
        }    
        start += step;
        i++; 
    } while (i < size);
    return i;
}

static void yk618_set_dcdc_voltage(uint8_t dcdc, int voltage)
{
    uint8_t addr, size, val;
    uint8_t idx_to, idx_cur, mask;

    addr = 0x20+dcdc;
    if((dcdc==1)||(dcdc==5))
    {
    	size = 0x20;
    	mask = 0x1f;
    }	
    else
    { 
    	size = 0x40;
    	mask = 0x3f;
    }	
    if (dcdc == 1)
    	idx_to = find_idx(1600, voltage, 100, size);                  // step is 100mV
    else if(dcdc == 5)
    	idx_to = find_idx(1000, voltage, 50, size);   
    else
    	idx_to = find_idx(600, voltage, 20, size);                  // step is 20mV
    
    yk618_read(addr, &idx_cur);
    val = idx_cur;
    idx_cur &= mask;
   // printf("cur=%x to =%x\n", idx_cur, idx_to);
    while (idx_cur != idx_to) {
        if (idx_cur < idx_to) {                                 // adjust to target voltage step by step
            idx_cur++;    
        } else {
            idx_cur--;
        }
        val &= ~mask;
        val |= idx_cur; 
        yk618_write(addr, val);
        udelay(10);
    }

}

static void yk618_ldo_voltage(uint8_t mode, uint8_t ldo, int voltage)
{ 
	int start;
    uint8_t onoff_addr,bits;
    uint8_t addr, size,  step;
    uint8_t idx_to, idx_cur, mask;

    switch (mode) {
    case 1:
        addr  = 0x27+ldo; 
        size  = 0x20;
        step  = 100;
        mask  = 0x1f;
        start = 700;
        if (ldo == 3)
        {
        	onoff_addr = 0x12;
	        bits = (1<<5);
        }
        else if ((ldo < 3) && (ldo > 0))
        {
        	onoff_addr = 0x10;
       		bits = (1<<(ldo+5));
        }
        else
        {
        	return ;
        }
        break;
    case 2:
        addr  = 0x18+ldo;
        size  = 0x20;
        step  = 100;
        mask  = 0x1f;
        start = 700;
        if ((ldo < 3) && (ldo > 0))
        {
        	onoff_addr = 0x12;
       		bits = (1<<(ldo-1));
        }
        else
        {
        	return ;
        }
        break;
    default:
    	return;
    }

    idx_to = find_idx(start, voltage, step, size);

    yk618_read(addr, &idx_cur);
    idx_cur &= ~mask;
    idx_cur |= idx_to;
    yk618_set_bits(onoff_addr, bits);
    yk618_write(addr, idx_cur);
}

struct virtual_gpio_data {
	int gpio;				//gpio number : 0/1/2/...
	int io;         //0: input      1: output
	int value;		  //0: low        1: high
};

int yk618_gpio_get_io(int *io_mode)
{
	uint8_t val;
	yk618_read(YK618_GPIO1_CFG,&val);
	val &= 0x07;
	if(val < 0x02)
		*io_mode = 1;
	else if (val == 0x02)
		*io_mode = 0;
	else
		return -EIO;

	return 0;
}


int yk618_gpio_set_value(int gpio, int value)
{
	int io_mode,ret;
    
    gpio = gpio;

	ret = yk618_gpio_get_io(&io_mode);
	if(ret)
		return ret;
	printf("yk618_gpio_set_value1\n");
	if(1 == io_mode){//output
		if(value){//high
			yk618_clr_bits(YK618_GPIO1_CFG,0x06);
			ret = yk618_set_bits(YK618_GPIO1_CFG,0x01);
			return ret;
		}
		else{//low
			yk618_clr_bits(YK618_GPIO1_CFG,0x06);
			return yk618_clr_bits(YK618_GPIO1_CFG,0x03);
		}
	}
	return -ENXIO;
}


int yk618_gpio_get_value(int gpio, int *value)
{
	int io_mode;
	int ret;
	uint8_t val;

	gpio = gpio;

	ret = yk618_gpio_get_io(&io_mode);
	printf("%s: line %d,%d,\n", __func__, __LINE__,gpio);
	if(ret)
		return ret;
	printf("yk618_gpio_get_value1\n");	
	if(1 == io_mode){//output
		ret =yk618_read(YK618_GPIO1_CFG,&val);
		if(ret)
			{
			printf("Read YK618_GPIO1 value fail! ");
			*value = 0;
				return -ENXIO;
		  }
		*value = val & 0x01;
	}
	else{//input
	ret = yk618_read(YK618_GPIO01_STATE,&val);val &= 0x10;*value = val>>4;	
	}
	return ret;
}
#if 0
static int yk618_get_freq(void)
{
	int  ret = 100;
	uint8_t  temp;
	yk618_read(YK618_ADC_SPEED, &temp);
	temp &= 0xc0;
	switch(temp >> 6){
		case 0:	ret = 100; break;
		case 1:	ret = 200; break;
		case 2:	ret = 400;break;
		case 3:	ret = 800;break;
		default:break;
	}
	return ret;
}
#endif
static inline void yk618_read_adc(struct yk618_adc_res *adc)
{
	uint8_t tmp[8];

	yk618_reads(YK618_VBATH_RES, tmp, 6);
	adc->vac_res = 0;
  	adc->iac_res = 0;
  	adc->vusb_res = 0;
  	adc->iusb_res = 0;
  	yk618_reads(YK618_VBATH_RES,tmp,6);
  	adc->vbat_res = ((uint16_t) tmp[0] << 8 )| tmp[1];
  	adc->ichar_res = ((uint16_t) tmp[2] << 8 )| tmp[3];
  	adc->idischar_res = ((uint16_t) tmp[4] << 8 )| tmp[5];
  	yk618_reads(YK618_OCVBATH_RES,tmp,2);
  	adc->ocvbat_res = ((uint16_t) tmp[0] << 8 )| tmp[1];
}

static inline int YK618_vbat_to_mV(uint16_t reg)
{
  return ((int)((( reg >> 8) << 4 ) | (reg & 0x000F))) * 1100 / 1000;
}

static inline int YK618_ocvbat_to_mV(uint16_t reg)
{
  return ((int)((( reg >> 8) << 4 ) | (reg & 0x000F))) * 1100 / 1000;
}


static inline int YK618_vdc_to_mV(uint16_t reg)
{
  return ((int)(((reg >> 8) << 4 ) | (reg & 0x000F))) * 1700 / 1000;
}


static inline int YK618_ibat_to_mA(uint16_t reg)
{
    return ((int)(((reg >> 8) << 4 ) | (reg & 0x000F))) ;
}

static inline int YK618_icharge_to_mA(uint16_t reg)
{
    return ((int)(((reg >> 8) << 4 ) | (reg & 0x000F)));
}

static inline int YK618_iac_to_mA(uint16_t reg)
{
    return ((int)(((reg >> 8) << 4 ) | (reg & 0x000F))) * 625 / 1000;
}

static inline int YK618_iusb_to_mA(uint16_t reg)
{
    return ((int)(((reg >> 8) << 4 ) | (reg & 0x000F))) * 375 / 1000;
}

int yk618_charger_online(void)
{
	uint8_t val[2];
	yk618_reads(YK618_STATUS, val, 2);
	if ((val[0] & ((1<<6) | (1<<4))) && (val[1] & (1<<5))) {
		return 1;
	} else {
		return 0;
	}
}


int yk618_charger_get_charging_status(void)
{
	uint8_t status_reg[2];
	yk618_reads(0x00, status_reg, 2);
	if (status_reg[0] & (1 << 2)) {
		return 1;
	} else {
		return 0;
	}
}

int yk618_get_charging_percent(void)
{ 
    uint8_t val;
    int rest_vol;

    yk618_read(YK618_CAP,&val);
    rest_vol	= (int)	(val & 0x7F);   

    if (rest_vol > 100)
       rest_vol = 0;

    return rest_vol;
}


int yk618_set_charging_current(int current)
{
    uint8_t val;
    
    printf("charge current =%d\n", current);
    current *= 1000;
    yk618_read(YK618_CHARGE1, &val);
   
    if (current == 0)
	val &= 0x7f;
    else
       val |= 0x80;

    if(current< 300000)
      current = 300000;
    else if(current > 2550000)
      current = 2550000;

    val &= 0xf0;
    val |= (current - 300000) / 150000;
    printf("cur val =%d\n", val);
    yk618_write(YK618_CHARGE1, val);
    return 0;
}

int yk618_charger_set_usbcur_limit(int usbcur_limit)
{
    uint8_t val;

    printf("yk618_charger_set_usbcur_limit\n");
    if ((usbcur_limit < 0 || usbcur_limit > 900) && (usbcur_limit != -1)) {
        printf("wrong current limit:%d\n", usbcur_limit); 
    }
	yk618_read(YK618_IPS_SET, &val);
    val &= ~0x03;
	switch (usbcur_limit) {
    case -1:
	case  0:
		val |= 0x3;             // Max
		break;
	case 500:
		val |= 0x1;
		break;
	case 900:
		val |= 0x0;
		break;
	default:
		printf("usbcur_limit=%d, not in 0,500,900. please check!\n", usbcur_limit);
		return -1;
		break;
	}
	yk618_write(YK618_IPS_SET, val);
    yk618_read(YK618_IPS_SET, &val);
	printf("[yk618_PMU]%s,YK618_CHARGE_VBUS:0x%x\n", __func__, val);
	
    return 0;
}


int yk618_init(void)
{

#ifdef CONFIG_DCDC2_VOLTAGE
    yk618_set_dcdc_voltage(2, CONFIG_DCDC2_VOLTAGE);      
#endif

#ifdef CONFIG_DCDC3_INIT_VOLTAGE
    yk618_set_dcdc_voltage(3, CONFIG_DCDC3_INIT_VOLTAGE);     
#endif

#ifdef CONFIG_DCDC5_VOLTAGE
    yk618_set_dcdc_voltage(5, CONFIG_DCDC5_VOLTAGE);      
#endif    
  
#ifdef CONFIG_ELDO2_VOLTAGE
    yk618_ldo_voltage(ELDO, 2, CONFIG_ELDO2_VOLTAGE);              
#endif

#ifdef CONFIG_ALDO2_VOLTAGE
    yk618_ldo_voltage(ALDO, 2, CONFIG_ALDO2_VOLTAGE);
#endif

#ifdef CONFIG_ALDO3_VOLTAGE
    yk618_ldo_voltage(ALDO, 3, CONFIG_ALDO3_VOLTAGE);
#endif


    return 0;    
}
/*
int yk618_set_full_charge_voltage(int voltage)
{
    int val;

    switch (voltage) {
    case 4100000: val = 0x00; break;
    case 4150000: val = 0x20; break;
    case 4200000: val = 0x40; break;
    case 4360000: val = 0x60; break;
    default :
        printf("%s, wrong charge target voltage:%d\n", __func__, voltage);    
        return -1;
    }
    return yk618_update(0x33, val, 0x60);
}

int yk618_set_charge_end_current(int rate)
{
    int val;
    switch(rate) {
    case 10:    val = 0x00; break;
    case 15:    val = 0x10; break;
    default:
        printf("%s, wrong charge end rate:%d\n", __func__, rate);    
        return -1;
    } 
    return yk618_update(0x33, val, 0x10);
}

int yk618_set_trickle_time(int time)
{
    int val;
    switch (time) {
    case 40: val = 0x00; break;
    case 50: val = 0x40; break;
    case 60: val = 0x80; break;
    case 70: val = 0xc0; break;
    default:
        printf("%s, wrong trickle charge timeout:%d\n", __func__, time);    
        return -1;
    }
    return yk618_update(0x34, val, 0xc0);
}

int yk618_set_rapid_time(int time)
{
    int val;    
    switch (time) {
    case 360: val = 0x00; break;
    case 480: val = 0x01; break;
    case 600: val = 0x02; break;
    case 720: val = 0x03; break;
    default:
        printf("%s, wrong rapid charge timeout:%d\n", __func__, time);    
        return -1;
    }
    return yk618_update(0x34, val, 0x03);
}

int yk618_set_charge_enable(int en)
{
    int val;    
    switch (en) {
    case 0: val = 0x00; break;
    case 1: val = 0x80; break;
    default:
        printf("%s, wrong charge enable value:%d\n", __func__, en);    
        return -1;
    }
    return yk618_update(0x33, val, 0x80);
}

int yk618_set_long_press_time(int time)
{
    int val;    
    switch (time) {
    case  4000: val = 0x00; break;
    case  6000: val = 0x01; break;
    case  8000: val = 0x02; break;
    case 10000: val = 0x03; break;
    default:
        printf("%s, wrong rapid charge timeout:%d\n", __func__, time);    
        return -1;
    }
    return yk618_update(0x36, val, 0x03);
}

int yk618_set_adc_freq(int freq)
{
    int val;    
    switch (freq) {
    case  100: val = 0x00; break;
    case  200: val = 0x40; break;
    case 400: val = 0x80; break;
    case 800: val = 0xc0; break;
    default:
        printf("%s, wrong rapid charge timeout:%d\n", __func__, freq);    
        return -1;
    }
    return yk618_update(0x84, val, 0xc0);
}
*/
void yk618_power_off(void)
{
	printf("[yk618] send power-off command!\n");
	mdelay(20);
	yk618_set_bits(YK618_OFF_CTL, 0x80);
	mdelay(20);
	printf("[yk618] warning!!! yk618 can't power-off, maybe some error happend!\n");
}
#if 0
int yk618_inti_para(void)
{
	printf("yk618_inti_para\n");

    if (battery) {
        yk618_set_full_charge_voltage(battery->pmu_init_chgvol);
        yk618_set_charge_end_current (battery->pmu_init_chgend_rate);
        yk618_set_charging_current      (battery->pmu_init_chgcur);
        yk618_set_trickle_time       (battery->pmu_init_chg_pretime);
        yk618_set_rapid_time         (battery->pmu_init_chg_csttime);
        yk618_set_charge_enable      (battery->pmu_init_chg_enabled);
        yk618_set_long_press_time    (battery->pmu_pekoff_time);
        yk618_set_adc_freq           (battery->pmu_init_adc_freqc);
        return 0;
    } else {
        return -1;    
    } 
   
}
#endif 
struct aml_pmu_driver g_aml_pmu_driver = {
    .pmu_init                    = yk618_init, 
    .pmu_get_battery_capacity    = yk618_get_charging_percent,
    .pmu_get_extern_power_status = yk618_charger_online,
    .pmu_set_gpio                = yk618_gpio_set_value,
    .pmu_get_gpio                = yk618_gpio_get_value,
    .pmu_reg_read                = yk618_read,
    .pmu_reg_write               = yk618_write,
    .pmu_reg_reads               = yk618_reads,
    .pmu_reg_writes              = yk618_writes,
    .pmu_set_bits                = yk618_update,
    .pmu_set_usb_current_limit   = yk618_charger_set_usbcur_limit,
    .pmu_set_charge_current      = yk618_set_charging_current,
    .pmu_power_off               = yk618_power_off,
};

struct aml_pmu_driver* aml_pmu_get_driver(void)
{
    return &g_aml_pmu_driver;
}
