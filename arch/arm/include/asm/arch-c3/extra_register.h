#ifndef __FIXME_H__
#define __FIXME_H__

//sec_ao
#define SEC_AO_SEC_GP_CFG0    SYSCTRL_SEC_STATUS_REG4
#define SEC_AO_SEC_GP_CFG2  SYSCTRL_SEC_STATUS_REG6

//OTP
#define OTP_LIC			(OTP_LIC_A)
#define OTP_LIC00		(OTP_LIC + 0x00)

#define OTP_LIC0		(OTP_LIC00)

//dsp
#define DSP_CFG0                                   ((0x0000  << 2) + 0xfe340000)
#define DSPB_CFG0                                  ((0x0000  << 2) + 0xfe350000)

#endif
