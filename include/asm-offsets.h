#ifndef	DO_DEPS_ONLY

#include <generated/generic-asm-offsets.h>
/* #include <generated/asm-offsets.h> */
#define TSK_ACTIVE_MM 1120 /* offsetof(struct task_struct, active_mm) */
#define TCR_T0SZ_OFFSET		0
#define TCR_TxSZ_WIDTH		6
#define TSK_TI_FLAGS 0 /* offsetof(struct task_struct, thread_info.flags) */
#define TSK_TI_PREEMPT 24 /* offsetof(struct task_struct, thread_info.preempt_count) */
#define TSK_TI_ADDR_LIMIT 8 /* offsetof(struct task_struct, thread_info.addr_limit) */
#define TSK_TI_TTBR0 16 /* offsetof(struct task_struct, thread_info.ttbr0) */
#define TSK_STACK 40 /* offsetof(struct task_struct, stack) */
#define TSK_STACK_CANARY 1296 /* offsetof(struct task_struct, stack_canary) */

#define THREAD_CPU_CONTEXT 2576 /* offsetof(struct task_struct, thread.cpu_context) */

#define S_X0 0 /* offsetof(struct pt_regs, regs[0]) */
#define S_X2 16 /* offsetof(struct pt_regs, regs[2]) */
#define S_X4 32 /* offsetof(struct pt_regs, regs[4]) */
#define S_X6 48 /* offsetof(struct pt_regs, regs[6]) */
#define S_X8 64 /* offsetof(struct pt_regs, regs[8]) */
#define S_X10 80 /* offsetof(struct pt_regs, regs[10]) */
#define S_X12 96 /* offsetof(struct pt_regs, regs[12]) */
#define S_X14 112 /* offsetof(struct pt_regs, regs[14]) */
#define S_X16 128 /* offsetof(struct pt_regs, regs[16]) */
#define S_X18 144 /* offsetof(struct pt_regs, regs[18]) */
#define S_X20 160 /* offsetof(struct pt_regs, regs[20]) */
#define S_X22 176 /* offsetof(struct pt_regs, regs[22]) */
#define S_X24 192 /* offsetof(struct pt_regs, regs[24]) */
#define S_X26 208 /* offsetof(struct pt_regs, regs[26]) */
#define S_X28 224 /* offsetof(struct pt_regs, regs[28]) */
#define S_LR 240 /* offsetof(struct pt_regs, regs[30]) */
#define S_SP 248 /* offsetof(struct pt_regs, sp) */
#define S_PSTATE 264 /* offsetof(struct pt_regs, pstate) */
#define S_PC 256 /* offsetof(struct pt_regs, pc) */
#define S_SYSCALLNO 280 /* offsetof(struct pt_regs, syscallno) */
#define S_ORIG_ADDR_LIMIT 288 /* offsetof(struct pt_regs, orig_addr_limit) */
#define S_PMR_SAVE 296 /* offsetof(struct pt_regs, pmr_save) */
#define S_STACKFRAME 304 /* offsetof(struct pt_regs, stackframe) */
#define S_FRAME_SIZE 320 /* sizeof(struct pt_regs) */

/* offsetof(struct compat_sigframe, uc.uc_mcontext.arm_r0) */
#define COMPAT_SIGFRAME_REGS_OFFSET 32
/* offsetof(struct compat_rt_sigframe, sig.uc.uc_mcontext.arm_r0) */
#define COMPAT_RT_SIGFRAME_REGS_OFFSET 160

#define MM_CONTEXT_ID 736 /* offsetof(struct mm_struct, context.id.counter) */

#define VMA_VM_MM 64 /* offsetof(struct vm_area_struct, vm_mm) */
#define VMA_VM_FLAGS 80 /* offsetof(struct vm_area_struct, vm_flags) */

#define VM_EXEC 4 /* VM_EXEC */

#define PAGE_SZ 4096 /* PAGE_SIZE */

//#define DMA_TO_DEVICE 1 /* DMA_TO_DEVICE */
//#define DMA_FROM_DEVICE 2 /* DMA_FROM_DEVICE */

#define PREEMPT_DISABLE_OFFSET 1 /* PREEMPT_DISABLE_OFFSET */

#define CPU_BOOT_STACK 0 /* offsetof(struct secondary_data, stack) */
#define CPU_BOOT_TASK 8 /* offsetof(struct secondary_data, task) */

#define CPU_CTX_SP 104 /* offsetof(struct cpu_suspend_ctx, sp) */
#define MPIDR_HASH_MASK 0 /* offsetof(struct mpidr_hash, mask) */
#define MPIDR_HASH_SHIFTS 8 /* offsetof(struct mpidr_hash, shift_aff) */
#define SLEEP_STACK_DATA_SYSTEM_REGS 0 /* offsetof(struct sleep_stack_data, system_regs) */
#define SLEEP_STACK_DATA_CALLEE_REGS 112 /* offsetof(struct sleep_stack_data, callee_saved_regs) */
#define ARM_SMCCC_RES_X0_OFFS 0 /* offsetof(struct arm_smccc_res, a0) */
#define ARM_SMCCC_RES_X2_OFFS 16 /* offsetof(struct arm_smccc_res, a2) */
#define ARM_SMCCC_QUIRK_ID_OFFS 0 /* offsetof(struct arm_smccc_quirk, id) */
#define ARM_SMCCC_QUIRK_STATE_OFFS 8 /* offsetof(struct arm_smccc_quirk, state) */

#define HIBERN_PBE_ORIG 8 /* offsetof(struct pbe, orig_address) */
#define HIBERN_PBE_ADDR 0 /* offsetof(struct pbe, address) */
#define HIBERN_PBE_NEXT 16 /* offsetof(struct pbe, next) */
#define ARM64_FTR_SYSVAL 24 /* offsetof(struct arm64_ftr_reg, sys_val) */

#define TRAMP_VALIAS -4322250752 /* TRAMP_VALIAS */
#endif
