#ifndef __AML_VOUT_H_
#define __AML_VOUT_H_

#include <amlogic/media/vout/aml_vinfo.h>

void vout_init(void);
void vout_vinfo_dump(void);
int vout_get_current_vmode(void);
int vout_get_current_axis(int *axis);
void vout_set_current_vmode(int mode);
struct vinfo_s *vout_get_current_vinfo(void);
extern unsigned long get_fb_addr(void);
#endif

