#ifndef __LINUX_YK618_H_
#define __LINUX_YK618_H_
/* LDO0 For RTCLDO ,LDO1-3 for ALDO,LDO*/
enum {
	YK618_ID_LDO1,   //RTCLDO
	YK618_ID_LDO2,   //ALDO1
	YK618_ID_LDO3,   //ALDO2
	YK618_ID_LDO4,   //ALDO3
	YK618_ID_LDO5,   //DLDO1
	YK618_ID_LDO6,   //DLDO2
	YK618_ID_LDO7,   //DLDO3
	YK618_ID_LDO8,   //DLDO4
	YK618_ID_LDO9,   //ELDO1
	YK618_ID_LDO10,  //ELDO2
	YK618_ID_LDO11,  //ELDO3
	YK618_ID_LDO12,  //DC5LDO
	YK618_ID_DCDC1,
	YK618_ID_DCDC2,
	YK618_ID_DCDC3,
	YK618_ID_DCDC4,
	YK618_ID_DCDC5,
	YK618_ID_LDOIO0,
	YK618_ID_LDOIO1,
	YK618_ID_SUPPLY,
	YK618_ID_GPIO,	
};


#define YK618                     (22)
#define YK618_STATUS              (0x00)
#define YK618_MODE_CHGSTATUS      (0x01)
#define YK618_IC_TYPE			  (0x03)
#define YK618_BUFFER1             (0x04)
#define YK618_BUFFER2             (0x05)
#define YK618_BUFFER3             (0x06)
#define YK618_BUFFER4             (0x07)
#define YK618_BUFFER5             (0x08)
#define YK618_BUFFER6             (0x09)
#define YK618_BUFFER7             (0x0A)
#define YK618_BUFFER8             (0x0B)
#define YK618_BUFFER9             (0x0C)
#define YK618_BUFFERA             (0x0D)
#define YK618_BUFFERB             (0x0E)
#define YK618_BUFFERC             (0x0F)
#define YK618_IPS_SET             (0x30)
#define YK618_VOFF_SET            (0x31)
#define YK618_OFF_CTL             (0x32)
#define YK618_CHARGE1             (0x33)
#define YK618_CHARGE2             (0x34)
#define YK618_POK_SET             (0x36)
#define YK618_INTEN1              (0x40)
#define YK618_INTEN2              (0x41)
#define YK618_INTEN3              (0x42)
#define YK618_INTEN4              (0x43)
#define YK618_INTEN5              (0x44)
#define YK618_INTSTS1             (0x48)
#define YK618_INTSTS2             (0x49)
#define YK618_INTSTS3             (0x4A)
#define YK618_INTSTS4             (0x4B)
#define YK618_INTSTS5             (0x4C)

#define YK618_LDO_DC_EN1          (0X10)
#define YK618_LDO_DC_EN2          (0X12)
#define YK618_LDO_DC_EN3          (0X13)
#define YK618_DLDO1OUT_VOL        (0x15)
#define YK618_DLDO2OUT_VOL        (0x16)
#define YK618_DLDO3OUT_VOL        (0x17)
#define YK618_DLDO4OUT_VOL        (0x18)
#define YK618_ELDO1OUT_VOL        (0x19)
#define YK618_ELDO2OUT_VOL        (0x1A)
#define YK618_ELDO3OUT_VOL        (0x1B)
#define YK618_DC5LDOOUT_VOL       (0x1C)
#define YK618_DC1OUT_VOL          (0x21)
#define YK618_DC2OUT_VOL          (0x22)
#define YK618_DC3OUT_VOL          (0x23)
#define YK618_DC4OUT_VOL          (0x24)
#define YK618_DC5OUT_VOL          (0x25)
#define YK618_GPIO0LDOOUT_VOL     (0x91)
#define YK618_GPIO1LDOOUT_VOL     (0x93)
#define YK618_ALDO1OUT_VOL        (0x28)
#define YK618_ALDO2OUT_VOL        (0x29)
#define YK618_ALDO3OUT_VOL        (0x2A)

#define YK618_DCDC_MODESET        (0x80)
#define YK618_DCDC_FREQSET        (0x37) 
#define YK618_ADC_EN              (0x82)
#define YK618_ADC_SPEED           (0x84)
#define YK618_HOTOVER_CTL         (0x8F)

#define YK618_VBATH_RES	 	  (0x78)

#define YK618_GPIO0_CTL           (0x90)
#define YK618_GPIO1_CTL           (0x92)
#define YK618_GPIO01_SIGNAL       (0x94)
#define YK618_BAT_CHGCOULOMB3     (0xB0)
#define YK618_BAT_CHGCOULOMB2     (0xB1)
#define YK618_BAT_CHGCOULOMB1     (0xB2)
#define YK618_BAT_CHGCOULOMB0     (0xB3)
#define YK618_BAT_DISCHGCOULOMB3  (0xB4)
#define YK618_BAT_DISCHGCOULOMB2  (0xB5)
#define YK618_BAT_DISCHGCOULOMB1  (0xB6)
#define YK618_BAT_DISCHGCOULOMB0  (0xB7)
#define YK618_COULOMB_CTL         (0xB8)

#define YK618_OCV_BUFFER0		  (0xBC)

/* bit definitions for YK618 events ,irq event */
/*  YK618  */
#define	YK618_IRQ_USBLO			( 1 <<  1)
#define	YK618_IRQ_USBRE			( 1 <<  2)
#define	YK618_IRQ_USBIN			( 1 <<  3)
#define	YK618_IRQ_USBOV     	( 1 <<  4)
#define	YK618_IRQ_ACRE     		( 1 <<  5)
#define	YK618_IRQ_ACIN     		( 1 <<  6)
#define	YK618_IRQ_ACOV     		( 1 <<  7)
#define	YK618_IRQ_TEMLO      	( 1 <<  8)
#define	YK618_IRQ_TEMOV      	( 1 <<  9)
#define	YK618_IRQ_CHAOV			( 1 << 10)
#define	YK618_IRQ_CHAST 	    ( 1 << 11)
#define	YK618_IRQ_BATATOU    	( 1 << 12)
#define	YK618_IRQ_BATATIN  		( 1 << 13)
#define YK618_IRQ_BATRE			( 1 << 14)
#define YK618_IRQ_BATIN			( 1 << 15)
#define	YK618_IRQ_POKLO			( 1 << 16)
#define	YK618_IRQ_POKSH	    	( 1 << 17)
#define YK618_IRQ_CHACURLO    	( 1 << 22)
#define YK618_IRQ_ICTEMOV    	( 1 << 23)
#define YK618_IRQ_EXTLOWARN2  	( 1 << 24)
#define YK618_IRQ_EXTLOWARN1  	( 1 << 25)
#define YK618_IRQ_GPIO0TG     	((uint64_t)1 << 32)
#define YK618_IRQ_GPIO1TG     	((uint64_t)1 << 33)
#define YK618_IRQ_GPIO2TG     	((uint64_t)1 << 34)
#define YK618_IRQ_GPIO3TG     	((uint64_t)1 << 35)

#define YK618_IRQ_PEKFE     	((uint64_t)1 << 37)
#define YK618_IRQ_PEKRE     	((uint64_t)1 << 38)
#define YK618_IRQ_TIMER     	((uint64_t)1 << 39)


/* Status Query Interface */

#define YK618_STATUS_SOURCE    	( 1 <<  0)
#define YK618_STATUS_ACUSBSH 	( 1 <<  1)
#define YK618_STATUS_BATCURDIR 	( 1 <<  2)
#define YK618_STATUS_USBLAVHO 	( 1 <<  3)
#define YK618_STATUS_USBVA    	( 1 <<  4)
#define YK618_STATUS_USBEN    	( 1 <<  5)
#define YK618_STATUS_ACVA	    ( 1 <<  6)
#define YK618_STATUS_ACEN	    ( 1 <<  7)

#define YK618_STATUS_BATINACT  	( 1 << 11)
                               	
#define YK618_STATUS_BATEN     	( 1 << 13)
#define YK618_STATUS_INCHAR    	( 1 << 14)
#define YK618_STATUS_ICTEMOV   	( 1 << 15)


#define YK618_ADC_CONTROL3                 (0x84)

#define YK618_VBATH_RES						    (0x78)
#define YK618_VBATL_RES						    (0x79)

#define YK618_OCVBATH_RES						 (0xBC)
#define YK618_OCVBATL_RES						 (0xBD)

#define YK618_CAP                    (0xB9)
#define YK618_BATCAP0                    (0xe0)
#define YK618_BATCAP1                    (0xe1)
#define YK618_RDC0                    (0xba)
#define YK618_RDC1                    (0xbb)
#define YK618_WARNING_LEVEL           (0xe6)
#define YK618_ADJUST_PARA             (0xe8)


#define YK618_INTTEMP                      (0x56)
#define YK618_DATA_BUFFER0					YK618_BUFFER1
#define YK618_DATA_BUFFER1					YK618_BUFFER2
#define YK618_DATA_BUFFER2					YK618_BUFFER3
#define YK618_DATA_BUFFER3					YK618_BUFFER4
#define YK618_DATA_BUFFER4					YK618_BUFFER5
#define YK618_DATA_BUFFER5					YK618_BUFFER6
#define YK618_DATA_BUFFER6					YK618_BUFFER7
#define YK618_DATA_BUFFER7					YK618_BUFFER8
#define YK618_DATA_BUFFER8					YK618_BUFFER9
#define YK618_DATA_BUFFER9					YK618_BUFFERA
#define YK618_DATA_BUFFERA					YK618_BUFFERB
#define YK618_DATA_BUFFERB					YK618_BUFFERC



#define YK618_GPIO0_CFG                   (YK618_GPIO0_CTL)//0x90
#define YK618_GPIO1_CFG                   (YK618_GPIO1_CTL)//0x92
#define YK618_GPIO2_CFG                   (YK618_LDO_DC_EN2)//0x12
#define YK618_GPIO3_CFG                   (YK618_OFF_CTL)//0x32
#define YK618_GPIO4_CFG                   (YK618_HOTOVER_CTL)//0x8f
#define YK618_GPIO4_STA                   (YK618_IPS_SET)//0x30
#define YK618_GPIO01_STATE               (YK618_GPIO01_SIGNAL)

#define YK618_LDO1_MIN		3000000
#define YK618_LDO1_MAX		3000000
#define YK618_LDO2_MIN		700000
#define YK618_LDO2_MAX		3300000
#define YK618_LDO3_MIN		700000
#define YK618_LDO3_MAX		3300000
#define YK618_LDO4_MIN		700000
#define YK618_LDO4_MAX		3300000
#define YK618_LDO5_MIN		700000
#define YK618_LDO5_MAX		3300000
#define YK618_LDO6_MIN		700000
#define YK618_LDO6_MAX		3300000
#define YK618_LDO7_MIN		700000
#define YK618_LDO7_MAX		3300000
#define YK618_LDO8_MIN		700000
#define YK618_LDO8_MAX		3300000
#define YK618_LDO9_MIN		700000
#define YK618_LDO9_MAX		3300000
#define YK618_LDO10_MIN         700000
#define YK618_LDO10_MAX         3300000
#define YK618_LDO11_MIN         700000
#define YK618_LDO11_MAX         3300000
#define YK618_LDO12_MIN         700000
#define YK618_LDO12_MAX         1400000

#define YK618_DCDC1_MIN		1600000
#define YK618_DCDC1_MAX		3400000
#define YK618_DCDC2_MIN		600000
#define YK618_DCDC2_MAX		1540000
#define YK618_DCDC3_MIN		600000
#define YK618_DCDC3_MAX		1860000
#define YK618_DCDC4_MIN		600000
#define YK618_DCDC4_MAX		1540000
#define YK618_DCDC5_MIN		1000000
#define YK618_DCDC5_MAX		2550000

#define YK618_LDOIO0_MIN	700000
#define YK618_LDOIO0_MAX	3300000
#define YK618_LDOIO1_MIN	700000
#define YK618_LDOIO1_MAX	3300000

struct aml_pmu_driver {
    int  (*pmu_init)(void);                                                     // initialize PMU board
    int  (*pmu_get_battery_capacity)(void);                                     // return battery percent
    int  (*pmu_get_extern_power_status)(void);                                  // return extern power is online
    int  (*pmu_set_gpio)(int gpio, int value);                                  // export for other driver
    int  (*pmu_get_gpio)(int gpio, int *value);                                 // export for other driver
    int  (*pmu_reg_read)  (unsigned char addr, unsigned char *buf);                       // single register read
    int  (*pmu_reg_write) (unsigned char addr, unsigned char value);                      // single register write
    int  (*pmu_reg_reads) (unsigned char addr, unsigned char *buf, int count);            // large amount registers reads
    int  (*pmu_reg_writes)(unsigned char addr, unsigned char *buf, int count);            // large amount registers writes
    int  (*pmu_set_bits)  (unsigned char addr, unsigned char bits, unsigned char mask);   // set bits in mask
    int  (*pmu_set_usb_current_limit)(int curr);                                // set usb current limit
    int  (*pmu_set_charge_current)(int curr);                                   // set charge current
    void (*pmu_power_off)(void);   
};

extern struct aml_pmu_driver g_aml_pmu_driver;
//extern struct aml_pmu_driver* aml_pmu_get_driver(void);

#endif /* __LINUX_YK618_H_ */
