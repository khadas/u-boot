/*
 *  Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
 *
 *  All information contained herein is Amlogic confidential.
 *
 *  This software is provided to you pursuant to Software License Agreement
 *  (SLA) with Amlogic Inc ("Amlogic"). This software may be used
 *  only in accordance with the terms of this agreement.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification is strictly prohibited without prior written permission from
 *  Amlogic.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Printf-like functionality for Chrome EC */

#include "myprintf.h"
#include "projdefs.h"
#include "util.h"
//#include <stdarg.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"     /* RTOS task related API prototypes. */

static const char error_str[] = "ERROR";

#define MAX_FORMAT 1024		/* Maximum chars in a single format field */
static char printbuffer[512];

/**
 * Convert the lowest nibble of a number to hex
 *
 * @param c	Number to extract lowest nibble from
 *
 * @return The corresponding ASCII character ('0' - 'f').
 */
static int hexdigit(int c)
{
	/* Strip off just the last nibble */
	c &= 0x0f;

	return c > 9 ? (c + 'a' - 10) : (c + '0');
}

/* Flags for vfnprintf() flags */
#define PF_LEFT		(1 << 0)	/* Left-justify */
#define PF_PADZERO	(1 << 1)	/* Pad with 0's not spaces */
#define PF_NEGATIVE	(1 << 2)	/* Number is negative */
#define PF_64BIT	(1 << 3)	/* Number is 64-bit */
#if 1
int vfnprintf(int (*addchar)(void *context, int c), void *context,
	      const char *format, va_list args)
{
	/*
	 * Longest uint64 in decimal = 20
	 * Longest uint32 in binary  = 32
	 * + sign bit
	 * + terminating null
	 */
	char intbuf[34];
	int flags;
	int pad_width;
	int precision;
	char *vstr;
	int vlen;

	while (*format) {
		int c = *format++;

		/* Copy normal characters */
		if (c != '%') {
			if (addchar(context, c))
				return pdFREERTOS_ERRNO_EINVAL;
			continue;
		}

		/* Zero flags, now that we're in a format */
		flags = 0;

		/* Get first format character */
		c = *format++;

		/* Send "%" for "%%" input */
		if (c == '%' || c == '\0') {
			if (addchar(context, '%'))
				return pdFREERTOS_ERRNO_EINVAL;
			continue;
		}

		/* Handle %c */
		if (c == 'c') {
			c = va_arg(args, int);

			if (addchar(context, c))
				return pdFREERTOS_ERRNO_EINVAL;
			continue;
		}

		/* Handle left-justification ("%-5s") */
		if (c == '-') {
			flags |= PF_LEFT;
			c = *format++;
		}

		/* Handle padding with 0's */
		if (c == '0') {
			flags |= PF_PADZERO;
			c = *format++;
		}

		/* Count padding length */
		pad_width = 0;
		if (c == '*') {
			pad_width = va_arg(args, int);

			c = *format++;
		} else {
			while (c >= '0' && c <= '9') {
				pad_width = (10 * pad_width) + c - '0';
				c = *format++;
			}
		}
		if (pad_width < 0 || pad_width > MAX_FORMAT) {
			/* Sanity check for precision failed */
			format = error_str;
			continue;
		}
		/* Count precision */
		precision = 0;
		if (c == '.') {
			c = *format++;
			if (c == '*') {
				precision = va_arg(args, int);

				c = *format++;
			} else {
				while (c >= '0' && c <= '9') {
					precision = (10 * precision) + c - '0';
					c = *format++;
				}
			}
			if (precision < 0 || precision > MAX_FORMAT) {
				/* Sanity check for precision failed */
				format = error_str;
				continue;
			}
		}

		if (c == 's') {
			vstr = va_arg(args, char *);

			if (vstr == NULL) {	/*Fix me */
				;	//vstr = "(NULL)";
			}
		} else if (c == 'h') {
			/* Hex dump output */
			vstr = va_arg(args, char *);

			if (!precision) {
				/* Hex dump requires precision */
				format = error_str;
				continue;
			}

			for (; precision; precision--, vstr++) {
				if (addchar(context, hexdigit(*vstr >> 4)) ||
				    addchar(context, hexdigit(*vstr)))
					return pdFREERTOS_ERRNO_EINVAL;
			}

			continue;
		} else {
			uint64_t v;
			int base = 10;

			/* Handle length */
			if (c == 'l') {
				if (sizeof(long) == 8)
					flags |= PF_64BIT;
				c = *format++;
				if (c == 'l') {
					flags |= PF_64BIT;
					c = *format++;	// long long is 64bit at LP64
				}
			}

			/* Special-case: %T = current time */
			if (c == 'T') {
				//v = get_time().val;
				flags |= PF_64BIT;
				precision = 6;
			} else if (flags & PF_64BIT) {
				v = va_arg(args, uint64_t);
			} else {
				v = va_arg(args, uint32_t);
			}

			switch (c) {
			case 'd':
				if (flags & PF_64BIT) {
					if ((int64_t) v < 0) {
						flags |= PF_NEGATIVE;
						if (v != (1ULL << 63))
							v = -v;
					}
				} else {
					if ((int)v < 0) {
						flags |= PF_NEGATIVE;
						if (v != (1ULL << 31))
							v = -(int)v;
					}
				}
				break;
			case 'u':
			case 'T':
				break;
			case 'X':
			case 'x':
			case 'p':
				base = 16;
				break;
			case 'b':
				base = 2;
				break;
			default:
				format = error_str;
			}
			if (format == error_str)
				continue;	/* Bad format specifier */

			/*
			 * Convert integer to string, starting at end of
			 * buffer and working backwards.
			 */
			vstr = intbuf + sizeof(intbuf) - 1;
			*(vstr) = '\0';

			/*
			 * Fixed-point precision must fit in our buffer.
			 * Leave space for "0." and the terminating null.
			 */
			if (precision > (int)(sizeof(intbuf) - 3))
				precision = (int)(sizeof(intbuf) - 3);

			/*
			 * Handle digits to right of decimal for fixed point
			 * numbers.
			 */
			for (vlen = 0; vlen < precision; vlen++)
				*(--vstr) = '0' + uint64divmod(&v, 10);
			if (precision)
				*(--vstr) = '.';

			if (!v)
				*(--vstr) = '0';

			while (v) {
				int digit = uint64divmod(&v, base);

				if (digit < 10)
					*(--vstr) = '0' + digit;
				else if (c == 'X')
					*(--vstr) = 'A' + digit - 10;
				else
					*(--vstr) = 'a' + digit - 10;
			}

			if (flags & PF_NEGATIVE)
				*(--vstr) = '-';

			/*
			 * Precision field was interpreted by fixed-point
			 * logic, so clear it.
			 */
			precision = 0;
		}

		/* Copy string (or stringified integer) */
		if (vstr != NULL)
			vlen = strlen(vstr);
		else
			vlen = 0;

		/* No padding strings to wider than the precision */
		if (precision > 0 && pad_width > precision)
			pad_width = precision;

		/* If precision is zero, print everything */
		if (!precision)
			precision = MAX(vlen, pad_width);

		while (vlen < pad_width && !(flags & PF_LEFT)) {
			if (addchar(context, flags & PF_PADZERO ? '0' : ' '))
				return pdFREERTOS_ERRNO_EINVAL;
			vlen++;
		}

		if (vstr != NULL) {
			while (*vstr && --precision >= 0)
				if (addchar(context, *vstr++))
					return pdFREERTOS_ERRNO_EINVAL;
		}
		while (vlen < pad_width && flags & PF_LEFT) {
			if (addchar(context, ' '))
				return pdFREERTOS_ERRNO_EINVAL;
			vlen++;
		}
	}

	/* If we're still here, we consumed all output */
	return pdFREERTOS_ERRNO_NONE;
}
#endif
/* Context for snprintf() */
struct snprintf_context {
	char *str;
	size_t size;
};

/**
 * Add a character to the string context.
 *
 * @param context	Context receiving character
 * @param c		Character to add
 * @return 0 if character added, 1 if character dropped because no space.
 */
static int snprintf_addchar(void *context, int c)
{
	struct snprintf_context *ctx = (struct snprintf_context *)context;

	if (!ctx->size)
		return 1;

	*(ctx->str++) = c;
	ctx->size--;

	return 0;
}

int sPrintf_ext(char *str, size_t size, const char *format, va_list args)
{
	struct snprintf_context ctx;
	int rv;

	if (!str || !size)
		return pdFREERTOS_ERRNO_EINVAL;

	ctx.str = str;
	ctx.size = size - 1;	/* Reserve space for terminating '\0' */

	rv = vfnprintf(snprintf_addchar, &ctx, format, args);

	/* Terminate string */
	*ctx.str = '\0';

	return rv;
}

int sPrintf(char *str, size_t size, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);

	i = sPrintf_ext(str, size, fmt, args);
	va_end(args);

	return i;
}

//int iprintf(const char *fmt, ...)
int iprintf(const char *fmt, ...)
{
	va_list args;
	int i;
	UBaseType_t uxSavedInterruptStatus;

	//char buf[20] = {0};

	uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();

	va_start(args, fmt);

	/* For this to work, printbuffer must be larger than
	 * anything we ever want to print.
	 */
	i = sPrintf_ext(printbuffer, sizeof(printbuffer), fmt, args);
	va_end(args);
	{
		/* Print the string */
		//sprintf(buf, "%lld : ", (long long int)xTaskGetTickCount());
		//vSerialPutString(ConsoleSerial, buf);
		//vSerialPutString(ConsoleSerial, printbuffer);
		vUartPuts(printbuffer);
	}
	portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );
	return i;
}

int iprint_string(char *str)
{
	//vSerialPutString(ConsoleSerial, str);
	vUartPuts(str);

	return 0;
}

extern int puts(const char *str);

int puts(const char *str)
{
	vUartPuts(str);

	return 0;
}

