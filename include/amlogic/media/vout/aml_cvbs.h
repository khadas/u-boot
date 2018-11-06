#ifndef __AML_CVBS_H__
#define __AML_CVBS_H__

#define AML_CVBS_PERFORMANCE_COMPATIBILITY_SUPPORT	1

#define AML_CVBS_CHINASARFT		0x0
#define AML_CVBS_CHINATELECOM	0x1
#define AML_CVBS_CHINAMOBILE		0x2
#define AML_CVBS_PERFORMANCE_ACTIVED	AML_CVBS_CHINASARFT

void cvbs_init(void);
void cvbs_show_valid_vmode(void);
int cvbs_set_vmode(char* vmode_name);
int cvbs_set_bist(char* bist_mode);
int cvbs_set_vdac(int status);
int cvbs_reg_debug(int argc, char* const argv[]);

extern int cvbs_outputmode_check(char *vmode_name);

#endif

