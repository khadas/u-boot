#include <common.h>
#include <linux/compiler.h>

DECLARE_GLOBAL_DATA_PTR;

void print_symbol(unsigned long addr, unsigned long offset)
{
	const char *sym;
	unsigned long base;
	unsigned long end;
	unsigned long size;
	unsigned long faddr = addr - offset;

	sym = symbol_lookup(faddr, &base, &end);
	if (sym) {
		size = end - base;
		printf("[<%016lx>]%s+0x%lx/0x%lx\n", addr, sym, addr-(base+offset), size);
	} else {
		printf("[<%016lx>]%s\n", addr, "N/A");
	}
}

static int unwind_frame(struct stackframe *frame)
{
	unsigned long fp = frame->fp;
	unsigned long stack_high;
	unsigned long stack_low;

	stack_low = frame->sp;
	stack_high = gd->start_addr_sp;
	frame->fp = *(unsigned long *)(fp);

	if (frame->fp < stack_low || fp > stack_high - 0x10)
		return -1;

	frame->sp = fp + 0x10;
	/*
	 * -4 here because we care about the PC not
	 * where the return will go
	 */
	frame->pc = *(unsigned long *)(fp+8) - 4;

	return 0;
}

int dump_backtrace(struct pt_regs *regs)
{
	struct stackframe frame;
	unsigned long offset;
	const register unsigned long current_sp asm ("sp");
	int ret;

	offset = gd->reloc_off;
	memset(&frame, 0x00, sizeof(frame));
	if (regs) {
		frame.fp = regs->regs[29];
		frame.sp = regs->regs[31];
		frame.pc = regs->elr;
	} else {
		frame.fp = (unsigned long)__builtin_frame_address(0);
		/*
		 * +0x10 to reach the real bottom of
		 * the curent frame
		 */
		frame.sp = current_sp + 0x10;
		frame.pc = (unsigned long)dump_backtrace;
	}

	printf("\nCall trace: \n");
	while (1) {
		unsigned long where = frame.pc;
		print_symbol(where, offset);

		ret = unwind_frame(&frame);
		if (ret < 0)
			break;
	}

	return 0;
}

void stack_dump(void)
{
	dump_backtrace(NULL);
}
