/*
 *  linux/lib/vsprintf.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */

#include <common.h>
#if !defined(CONFIG_PANIC_HANG)
#include <command.h>
#endif

static void panic_finish(void) __attribute__ ((noreturn));

static void panic_finish(void)
{
	putc('\n');
#if defined(CONFIG_PANIC_HANG)
	hang();
#else
	udelay(100000);	/* allow messages to go out */
	do_reset(NULL, 0, 0, NULL);
#endif
	while (1)
		;
}

void panic_str(const char *str)
{
	puts(str);
	panic_finish();
}

void panic(const char *fmt, ...)
{
#if CONFIG_IS_ENABLED(PRINTF)
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
#endif
	panic_finish();
}

void __assert_fail(const char *assertion, const char *file, unsigned int line,
		   const char *function)
{
	/* This will not return */
	panic("%s:%u: %s: Assertion `%s' failed.", file, line, function,
	      assertion);
}

#define STACK_CHECK_GUARD      0xdeadbeefdeadbeefUL
uintptr_t __stack_chk_guard = STACK_CHECK_GUARD;
void __attribute__ ((noreturn)) __stack_chk_fail(void)
{
	unsigned long pc_reg;

	__asm__ volatile("mov %0, x30\n"
				: "=r" (pc_reg)
				:
				: "memory");
	/* This will not return */
	panic("Stack-protector: stack smashing detected at caller 0x%lx !\n",
			(pc_reg - 0x4));
}
