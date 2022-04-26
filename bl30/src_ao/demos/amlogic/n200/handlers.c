//See LICENSE for license details.
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "riscv_encoding.h"
#include "n200_func.h"

uintptr_t handle_trap(uintptr_t mcause, uintptr_t sp);
__attribute__((weak)) uintptr_t handle_nmi(void);

__attribute__((weak)) uintptr_t handle_nmi(void)
{
  //_exit(1);

	  printf("\nhandle_nmi");


  do {}while(1);
  return 0;
}


__attribute__((weak)) uintptr_t handle_trap(uintptr_t mcause, uintptr_t sp)
{
  sp = sp;
  if ((mcause&0xFFF) == 0xFFF) {
      handle_nmi();
  }
  //write(1, "trap\n", 5);
  printf("In trap handler, the mcause is %d\n", mcause);
  printf("In trap handler, the mepc is 0x%lx\n", read_csr(mepc));
  printf("In trap handler, the mtval is 0x%lx\n", read_csr(mbadaddr));
  //_exit(mcause);
  printf("\nhandle_trap");

  do {}while(1);
  return 0;
}


#ifdef N200_REVA
/*Entry Point for PIC Interrupt Handler*/
__attribute__((weak)) uint32_t handle_irq(uint32_t int_num);

__attribute__((weak)) uint32_t handle_irq(uint32_t int_num){
    // Enable interrupts to allow interrupt preempt based on priority
    //set_csr(mstatus, MSTATUS_MIE);

  pic_interrupt_handlers[int_num]();
  pic_complete_interrupt(int_num);
    // Disable interrupts
    //clear_csr(mstatus, MSTATUS_MIE);
  return int_num;
}
#endif


