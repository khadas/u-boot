// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#define pr_fmt(fmt) "uasan test: %s " fmt, __func__

#include <linux/types.h>
#include <common.h>
#include <config.h>
#include <command.h>
#include <asm/io.h>
#include <malloc.h>
#include <amlogic/uasan.h>

/*
 * Note: test functions are marked noinline so that their names appear in
 * reports.
 */

static noinline void malloc_oob_right(void)
{
	char *ptr;
	size_t size = 123;

	printf("out-of-bounds to right\n");
	ptr = malloc(size);
	if (!ptr) {
		printf("Allocation failed\n");
		return;
	}

	ptr[size] = 'x';
	free(ptr);
}

static noinline void malloc_oob_left(void)
{
	char *ptr;
	size_t size = 15;

	printf("out-of-bounds to left\n");
	ptr = malloc(size);
	if (!ptr) {
		printf("Allocation failed\n");
		return;
	}

	*ptr = *(ptr - 1);
	free(ptr);
}

static noinline void malloc_oob_realloc_more(void)
{
	char *ptr1, *ptr2;
	size_t size1 = 17;
	size_t size2 = 19;

	printf("out-of-bounds after realloc more\n");
	ptr1 = malloc(size1);
	ptr2 = realloc(ptr1, size2);
	if (!ptr1 || !ptr2) {
		printf("Allocation failed\n");
		free(ptr1);
		return;
	}

	ptr2[size2] = 'x';
	free(ptr2);
}

static noinline void malloc_oob_realloc_less(void)
{
	char *ptr1, *ptr2;
	size_t size1 = 17;
	size_t size2 = 15;

	printf("out-of-bounds after realloc less\n");
	ptr1 = malloc(size1);
	ptr2 = realloc(ptr1, size2);
	if (!ptr1 || !ptr2) {
		printf("Allocation failed\n");
		free(ptr1);
		return;
	}
	ptr2[size2] = 'x';
	free(ptr2);
}

static noinline void malloc_oob_16(void)
{
	struct {
		u64 words[2];
	} *ptr1, *ptr2;

	printf("malloc out-of-bounds for 16-bytes access\n");
	ptr1 = malloc(sizeof(*ptr1) - 3);
	ptr2 = malloc(sizeof(*ptr2));
	if (!ptr1 || !ptr2) {
		printf("Allocation failed\n");
		free(ptr1);
		free(ptr2);
		return;
	}
	*ptr1 = *ptr2;
	free(ptr1);
	free(ptr2);
}

static noinline void malloc_oob_memset_2(void)
{
	char *ptr;
	size_t size = 8;

	printf("out-of-bounds in memset2\n");
	ptr = malloc(size);
	if (!ptr) {
		printf("Allocation failed\n");
		return;
	}

	memset(ptr + 7, 0, 2);
	free(ptr);
}

static noinline void malloc_oob_memset_4(void)
{
	char *ptr;
	size_t size = 8;

	printf("out-of-bounds in memset4\n");
	ptr = malloc(size);
	if (!ptr) {
		printf("Allocation failed\n");
		return;
	}

	memset(ptr + 5, 0, 4);
	free(ptr);
}

static noinline void malloc_oob_memset_8(void)
{
	char *ptr;
	size_t size = 8;

	printf("out-of-bounds in memset8\n");
	ptr = malloc(size);
	if (!ptr) {
		printf("Allocation failed\n");
		return;
	}

	memset(ptr + 1, 0, 8);
	free(ptr);
}

static noinline void malloc_oob_memset_16(void)
{
	char *ptr;
	size_t size = 16;

	printf("out-of-bounds in memset16\n");
	ptr = malloc(size);
	if (!ptr) {
		printf("Allocation failed\n");
		return;
	}

	memset(ptr + 1, 0, 16);
	free(ptr);
}

static noinline void malloc_oob_in_memset(void)
{
	char *ptr;
	size_t size = 666;

	printf("out-of-bounds in memset\n");
	ptr = malloc(size);
	if (!ptr) {
		printf("Allocation failed\n");
		return;
	}

	memset(ptr, 0, size + 5);
	free(ptr);
}

static noinline void malloc_uaf(void)
{
	char *ptr;
	size_t size = 10;

	printf("use-after-free\n");
	ptr = malloc(size);
	if (!ptr) {
		printf("Allocation failed\n");
		return;
	}

	free(ptr);
	*(ptr + 8) = 'x';
}

static noinline void malloc_uaf_memset(void)
{
	char *ptr;
	size_t size = 33;

	printf("use-after-free in memset\n");
	ptr = malloc(size);
	if (!ptr) {
		printf("Allocation failed\n");
		return;
	}

	free(ptr);
	memset(ptr, 0, size);
}

static noinline void malloc_uaf2(void)
{
	char *ptr1, *ptr2;
	size_t size = 43;

	printf("use-after-free after another malloc\n");
	ptr1 = malloc(size);
	if (!ptr1) {
		printf("Allocation failed\n");
		return;
	}

	free(ptr1);
	ptr2 = malloc(size);
	if (!ptr2) {
		printf("Allocation failed\n");
		return;
	}

	ptr1[40] = 'x';
	if (ptr1 == ptr2)
		printf("Could not detect use-after-free: ptr1 == ptr2\n");
	free(ptr2);
}

static char global_array[10];

static noinline void uasan_global_oob(void)
{
	volatile int i = 3;
	char *p = &global_array[ARRAY_SIZE(global_array) + i];

	printf("out-of-bounds global variable\n");
	*(volatile char *)p;
}

static noinline void uasan_stack_oob(void)
{
	char stack_array[10];
	volatile int i = 0;
	char *p = &stack_array[ARRAY_SIZE(stack_array) + i];

	printf("out-of-bounds on stack\n");
	*(volatile char *)p;
}

static noinline void malloc_size_unpoisons_memory(void)
{
	char *ptr;
	size_t size = 123, real_size = size;

	printf("malloc_usable_size() unpoisons the whole allocated chunk\n");
	ptr = malloc(size);
	if (!ptr) {
		printf("Allocation failed\n");
		return;
	}
	real_size = malloc_usable_size(ptr);
	/* This access doesn't trigger an error. */
	ptr[size] = 'x';
	/* This one does. */
	ptr[real_size] = 'y';
	free(ptr);
}

static noinline void use_after_scope_test(void)
{
	volatile char *volatile p;

	printf("use-after-scope on int\n");
	{
		int local = 0;

		p = (char *)&local;
	}
	p[0] = 1;
	p[3] = 1;

	printf("use-after-scope on array\n");
	{
		char local[1024] = {0};

		p = local;
	}
	p[0] = 1;
	p[1023] = 1;
}

static noinline void uasan_alloca_oob_left(void)
{
	volatile int i = 10;
	char alloca_array[i];
	char *p = alloca_array - 1;

	printf("out-of-bounds to left on alloca\n");
	*(volatile char *)p;
}

static noinline void uasan_alloca_oob_right(void)
{
	volatile int i = 10;
	char alloca_array[i];
	char *p = alloca_array + i;

	printf("out-of-bounds to right on alloca\n");
	*(volatile char *)p;
}

static int do_uasan_test(cmd_tbl_t *cmdtp,
			 int flag, int argc, char * const argv[])
{
	/*
	 * Temporarily enable multi-shot mode. Otherwise, we'd only get a
	 * report for the first case.
	 */
	malloc_oob_right();
	malloc_oob_left();
	malloc_oob_16();
	malloc_oob_in_memset();
	malloc_oob_memset_2();
	malloc_oob_memset_4();
	malloc_oob_memset_8();
	malloc_oob_memset_16();
	malloc_oob_realloc_less();
	malloc_oob_realloc_more();
	malloc_uaf();
	malloc_uaf_memset();
	malloc_uaf2();
	uasan_stack_oob();
	uasan_global_oob();
	uasan_alloca_oob_left();
	uasan_alloca_oob_right();
	use_after_scope_test();
	malloc_size_unpoisons_memory();

	return 0;
}

U_BOOT_CMD(
	uasan_test, 12, 1, do_uasan_test,
	"uasan test command",
	"This command will run uasan test for each wrong memory access cases\n"
	"It's helpful to check the ability that uasan can support\n"
);
