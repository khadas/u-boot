#ifndef _BTWAKE_H_
#define _BTWAKE_H_

#ifdef __cplusplus
extern "C" {
#endif

void bt_task_start(void);
void bt_task_disable(void);
void Bt_GpioIRQRegister(void);
void Bt_GpioIRQFree(void);

#ifdef __cplusplus
}
#endif

#endif
