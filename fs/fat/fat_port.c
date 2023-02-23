// SPDX-License-Identifier: GPL-2.0+
#include <common.h>
#include <command.h>
#include <errno.h>
#include <rtc.h>
#include <linux/time.h>

static int month_offset[] = {
	0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};

/*
 * This only works for the Gregorian calendar - i.e. after 1752 (in the UK)
 */
int rtc_calc_weekday(struct rtc_time *tm)
{
	int leaps_to_date;
	int last_year;
	int day;

	if (tm->tm_year < 1753)
		return -1;
	last_year = tm->tm_year - 1;

	/* Number of leap corrections to apply up to end of last year */
	leaps_to_date = last_year / 4 - last_year / 100 + last_year / 400;

	/*
	 * This year is a leap year if it is divisible by 4 except when it is
	 * divisible by 100 unless it is divisible by 400
	 *
	 * e.g. 1904 was a leap year, 1900 was not, 1996 is, and 2000 is.
	 */
	if (tm->tm_year % 4 == 0 &&
	    ((tm->tm_year % 100 != 0) || (tm->tm_year % 400 == 0)) &&
	    tm->tm_mon > 2) {
		/* We are past Feb. 29 in a leap year */
		day = 1;
	} else {
		day = 0;
	}

	day += last_year * 365 + leaps_to_date + month_offset[tm->tm_mon - 1] +
			tm->tm_mday;
	tm->tm_wday = day % 7;

	return 0;
}