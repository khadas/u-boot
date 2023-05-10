#ifndef __FAT_PORT_H_
#define __FAT_PORT_H_

int rtc_calc_weekday(struct rtc_time *tm);

struct nameext {
	char name[8];
	char ext[3];
};

#endif /* __FAT_PORT_H_ */
