// See LICENSE for license details.

#include "interrupt_control.h"
#include "riscv_encoding.h"
#include "register.h"

    // Configure PMP to make all the address space accesable and executable
void eclic_init ( uint32_t num_irq )
{

  typedef volatile uint32_t vuint32_t;

  //clear cfg register
  *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_CFG_OFFSET) = 0;

  //clear minthresh register
  *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_MTH_OFFSET) = 0;

  //clear all IP/IE/ATTR/CTRL bits for all interrupt sources
  vuint32_t * ptr;

  vuint32_t * base = (vuint32_t*)(ECLIC_ADDR_BASE + ECLIC_INT_IP_OFFSET);
  vuint32_t * upper = (vuint32_t*)(base + num_irq*4);

  for (ptr = base; ptr < upper; ptr=ptr+4) {
    *ptr = 0;
  }

  clean_int_src();
}

void print_eclic(void)
{
	typedef volatile uint32_t vuint32_t;

	vuint32_t * ptr = (vuint32_t*)(ECLIC_ADDR_BASE + ECLIC_INT_IP_OFFSET + 7*4);

	printf("\nTIME=0x%lx\n",*ptr);
}


void eclic_enable_interrupt (uint32_t source) {
    *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_INT_IE_OFFSET+source*4) = 1;
}

void eclic_disable_interrupt (uint32_t source){
    *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_INT_IE_OFFSET+source*4) = 0;
}

void eclic_set_pending(uint32_t source){
    *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_INT_IP_OFFSET+source*4) = 1;
}

void eclic_clear_pending(uint32_t source){
    *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_INT_IP_OFFSET+source*4) = 0;
}

void eclic_set_intctrl (uint32_t source, uint8_t intctrl){
  *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_INT_CTRL_OFFSET+source*4) = intctrl;
}

uint8_t eclic_get_intctrl  (uint32_t source){
  return *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_INT_CTRL_OFFSET+source*4);
}

void eclic_set_intattr (uint32_t source, uint8_t intattr){
  *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_INT_ATTR_OFFSET+source*4) = intattr;
}

uint8_t eclic_get_intattr  (uint32_t source){
  return *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_INT_ATTR_OFFSET+source*4);
}

void eclic_set_cliccfg (uint8_t cliccfg){
  *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_CFG_OFFSET) = cliccfg;
}

uint8_t eclic_get_cliccfg (void){
  return *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_CFG_OFFSET);
}

void eclic_set_mth (uint8_t mth){
  *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_MTH_OFFSET) = mth;
}

uint8_t eclic_get_mth  (void){
  return *(volatile uint8_t*)(ECLIC_ADDR_BASE+ECLIC_MTH_OFFSET);
}

//sets nlbits
void eclic_set_nlbits(uint8_t nlbits) {
  //shift nlbits to correct position
  uint8_t nlbits_shifted = nlbits << ECLIC_CFG_NLBITS_LSB;

  //read the current cliccfg
  uint8_t old_cliccfg = eclic_get_cliccfg();
  uint8_t new_cliccfg = (old_cliccfg & (~ECLIC_CFG_NLBITS_MASK)) | (ECLIC_CFG_NLBITS_MASK & nlbits_shifted);

  eclic_set_cliccfg(new_cliccfg);
}

//get nlbits
uint8_t eclic_get_nlbits(void) {
  //extract nlbits
  uint8_t nlbits = eclic_get_cliccfg();
  nlbits = (nlbits & ECLIC_CFG_NLBITS_MASK) >> ECLIC_CFG_NLBITS_LSB;
  return nlbits;
}

//sets an interrupt level based encoding of nlbits and CLICINTCTLBITS
void eclic_set_irq_lvl(uint32_t source, uint8_t lvl) {
  //extract nlbits
  uint8_t nlbits = eclic_get_nlbits();
  if (nlbits > CLICINTCTLBITS) {
    nlbits = CLICINTCTLBITS;
  }

  //shift lvl right to mask off unused bits
  lvl = lvl >> (8-nlbits);
  //shift lvl into correct bit position
  lvl = lvl << (8-nlbits);

  //write to clicintctrl
  uint8_t current_intctrl = eclic_get_intctrl(source);
  //shift intctrl left to mask off unused bits
  current_intctrl = current_intctrl << nlbits;
  //shift intctrl into correct bit position
  current_intctrl = current_intctrl >> nlbits;

  eclic_set_intctrl(source, (current_intctrl | lvl));
}

//gets an interrupt level based encoding of nlbits
uint8_t eclic_get_irq_lvl(uint32_t source) {
  //extract nlbits
  uint8_t nlbits = eclic_get_nlbits();
  if (nlbits > CLICINTCTLBITS) {
    nlbits = CLICINTCTLBITS;
  }

  uint8_t intctrl = eclic_get_intctrl(source);

  //shift intctrl
  intctrl = intctrl >> (8-nlbits);
  //shift intctrl
  uint8_t lvl = intctrl << (8-nlbits);

  return lvl;
}

void eclic_set_irq_lvl_abs(uint32_t source, uint8_t lvl_abs) {
  //extract nlbits
  uint8_t nlbits = eclic_get_nlbits();
  if (nlbits > CLICINTCTLBITS) {
    nlbits = CLICINTCTLBITS;
  }

  //shift lvl_abs into correct bit position
  uint8_t lvl = lvl_abs << (8-nlbits);

  //write to clicintctrl
  uint8_t current_intctrl = eclic_get_intctrl(source);
  //shift intctrl left to mask off unused bits
  current_intctrl = current_intctrl << nlbits;
  //shift intctrl into correct bit position
  current_intctrl = current_intctrl >> nlbits;

  eclic_set_intctrl(source, (current_intctrl | lvl));
}

uint8_t eclic_get_irq_lvl_abs(uint32_t source) {
  //extract nlbits
  uint8_t nlbits = eclic_get_nlbits();
  if (nlbits > CLICINTCTLBITS) {
    nlbits = CLICINTCTLBITS;
  }

  uint8_t intctrl = eclic_get_intctrl(source);

  //shift intctrl
  intctrl = intctrl >> (8-nlbits);
  //shift intctrl
  uint8_t lvl_abs = intctrl;

  return lvl_abs;
}

void eclic_set_irq_pri(uint32_t source, uint8_t pri) {
  //extract nlbits
  uint8_t nlbits = eclic_get_nlbits();
  if (nlbits > CLICINTCTLBITS) {
    nlbits = CLICINTCTLBITS;
  }

  //write to clicintctrl
  uint8_t current_intctrl = eclic_get_intctrl(source);
  //shift intctrl left to mask off unused bits
  current_intctrl = current_intctrl >> (8 - nlbits);
  //shift intctrl into correct bit position
  current_intctrl = current_intctrl << (8 - nlbits);

  eclic_set_intctrl(source, (current_intctrl | pri));
}

void eclic_mode_enable(void) {
  uint32_t mtvec_value = read_csr(mtvec);
  mtvec_value = mtvec_value & 0xFFFFFFC0;
  mtvec_value = mtvec_value | 0x00000003;
  write_csr(mtvec,mtvec_value);
}

//sets vector-mode or non-vector mode
void eclic_set_vmode(uint32_t source) {
  //read the current attr
  uint8_t old_intattr = eclic_get_intattr(source);
      // Keep other bits unchanged and only set the LSB bit
  uint8_t new_intattr = (old_intattr | 0x1);

  eclic_set_intattr(source,new_intattr);
}

void eclic_set_nonvmode(uint32_t source) {
  //read the current attr
  uint8_t old_intattr = eclic_get_intattr(source);
      // Keep other bits unchanged and only clear the LSB bit
  uint8_t new_intattr = (old_intattr & (~0x1));

  eclic_set_intattr(source,new_intattr);
}

//sets interrupt as level sensitive
//Bit 1, trig[0], is defined as "edge-triggered" (0: level-triggered, 1: edge-triggered);
//Bit 2, trig[1], is defined as "negative-edge" (0: positive-edge, 1: negative-edge).

void eclic_set_level_trig(uint32_t source) {
  //read the current attr
  uint8_t old_intattr = eclic_get_intattr(source);
      // Keep other bits unchanged and only clear the bit 1
  uint8_t new_intattr = (old_intattr & (~0x2));

  eclic_set_intattr(source,new_intattr);
}

void eclic_set_posedge_trig(uint32_t source) {
  //read the current attr
  uint8_t old_intattr = eclic_get_intattr(source);
      // Keep other bits unchanged and only set the bit 1
  uint8_t new_intattr = (old_intattr | 0x2);
      // Keep other bits unchanged and only clear the bit 2
  new_intattr = (new_intattr & (~0x4));

  eclic_set_intattr(source,new_intattr);
}

void eclic_set_negedge_trig(uint32_t source) {
  //read the current attr
  uint8_t old_intattr = eclic_get_intattr(source);
      // Keep other bits unchanged and only set the bit 1
  uint8_t new_intattr = (old_intattr | 0x2);
      // Keep other bits unchanged and only set the bit 2
  new_intattr = (new_intattr | 0x4);

  eclic_set_intattr(source,new_intattr);
}

extern void core_wfe(void);
void wfe(void) {
  core_wfe();
}

void clean_int_src(void)
{
	for (uint32_t i=0; i<16; i++)
		REG32(AOCPU_IRQ_SEL0 + i*4) = 0;
}

int int_src_sel(uint32_t ulIrq, uint32_t src)
{
	uint32_t index;

	if (ulIrq < ECLIC_INTERNAL_NUM_INTERRUPTS ||
		ulIrq > ECLIC_NUM_INTERRUPTS) {
		printf("Error ulIrq!\n");
		return -1;
	}

	if (src > 0xff) {
		printf("Error src!\n");
		return -2;
	}

	ulIrq -= ECLIC_INTERNAL_NUM_INTERRUPTS;

	index = ulIrq/2;
	REG32(AOCPU_IRQ_SEL0 + index*4) &= ~(0x1ff << (ulIrq%2)*16);
	REG32(AOCPU_IRQ_SEL0 + index*4) |= src << (ulIrq%2)*16;
	return 0;
}

int int_src_clean(uint32_t ulIrq)
{
	uint32_t index;

	if (ulIrq < ECLIC_INTERNAL_NUM_INTERRUPTS ||
		ulIrq > ECLIC_NUM_INTERRUPTS) {
		printf("Error ulIrq!\n");
		return -1;
	}

	ulIrq -= ECLIC_INTERNAL_NUM_INTERRUPTS;

	index = ulIrq/2;
	REG32(AOCPU_IRQ_SEL0 + index*4) &= ~(0x1ff << (ulIrq%2)*16);
	return 0;
}

/*Just for external interrupt source.
 *Because int_src_sel() just support external select
 */
int eclic_map_interrupt(uint32_t ulIrq, uint32_t src)
{
	uint8_t val;

	if (int_src_sel(ulIrq, src)) {
		printf("Enable %ld irq, %ld src fail!\n", ulIrq, src);
		return -1;
	}

	val = eclic_get_intattr (ulIrq);
	val |= ECLIC_INT_ATTR_MACH_MODE;
	/*Use edge trig interrupt default*/
	val |= ECLIC_INT_ATTR_TRIG_EDGE;
	eclic_set_intattr(ulIrq, val);
	//eclic_enable_interrupt(ulIrq);
	return 0;
}

uint32_t eclic_interrupt_inner[SOC_ECLIC_NUM_INTERRUPTS] = {0};
extern uint32_t vector_base;

int RegisterIrq(uint32_t int_num, uint32_t int_priority, function_ptr_t handler) {
	int irq = 0;

	for (irq = ECLIC_INTERNAL_NUM_INTERRUPTS; irq <= ECLIC_NUM_INTERRUPTS; irq ++) {
		if (eclic_interrupt_inner[irq - ECLIC_INTERNAL_NUM_INTERRUPTS] == 0)
			break;
	}
	if (eclic_map_interrupt(irq, int_num) < 0) {
		printf("eclic map error.\n");
		return -1;
	}
	eclic_interrupt_inner[irq - ECLIC_INTERNAL_NUM_INTERRUPTS] = int_num;

	*(&vector_base + irq) = (uint32_t)handler;
	eclic_set_irq_pri(irq, int_priority);

	return 0;
}

int UnRegisterIrq(uint32_t ulIrq)
{
	int irq = 0;
	for (irq = ECLIC_INTERNAL_NUM_INTERRUPTS; irq <= ECLIC_NUM_INTERRUPTS; irq ++) {
		if (eclic_interrupt_inner[irq - ECLIC_INTERNAL_NUM_INTERRUPTS] == ulIrq)
			break;
	}
	if (irq > ECLIC_NUM_INTERRUPTS) {
		printf("Error ulIrq!\n");
		return -1;
	} else {
		if (int_src_clean(irq)) {
			printf("unregister %ld irq, %ld src fail!\n", ulIrq, irq);
			return -1;
		}
		eclic_interrupt_inner[irq - ECLIC_INTERNAL_NUM_INTERRUPTS] = 0;
		*(&vector_base + irq) = 0;
	}
	return 0;
}

int EnableIrq(uint32_t ulIrq)
{
	int irq = 0;
	for (irq = ECLIC_INTERNAL_NUM_INTERRUPTS; irq <= ECLIC_NUM_INTERRUPTS; irq ++) {
		if (eclic_interrupt_inner[irq - ECLIC_INTERNAL_NUM_INTERRUPTS] == ulIrq)
			break;
	}
	if (irq > ECLIC_NUM_INTERRUPTS) {
		printf("Error ulIrq!\n");
		return -1;
	} else {
		eclic_enable_interrupt(irq);
	}
	return 0;
}

int DisableIrq(uint32_t ulIrq)
{
	int irq = 0;
	for (irq = ECLIC_INTERNAL_NUM_INTERRUPTS; irq <= ECLIC_NUM_INTERRUPTS; irq ++) {
		if (eclic_interrupt_inner[irq - ECLIC_INTERNAL_NUM_INTERRUPTS] == ulIrq)
			break;
	}
	if (irq > ECLIC_NUM_INTERRUPTS) {
		printf("Error ulIrq!\n");
		return -1;
	} else {
		eclic_disable_interrupt(irq);
	}
	return 0;
}

int SetIrqPriority(uint32_t ulIrq, uint32_t ulProi)
{
	int irq = 0;
	for (irq = ECLIC_INTERNAL_NUM_INTERRUPTS; irq <= ECLIC_NUM_INTERRUPTS; irq ++) {
		if (eclic_interrupt_inner[irq - ECLIC_INTERNAL_NUM_INTERRUPTS] == ulIrq)
			break;
	}
	if (irq > ECLIC_NUM_INTERRUPTS) {
		printf("Error ulIrq!\n");
		return -1;
	} else {
		eclic_set_irq_pri(irq, ulProi);
	}
	return 0;
}

int ClearPendingIrq(uint32_t ulIrq)
{
	int irq = 0;
	for (irq = ECLIC_INTERNAL_NUM_INTERRUPTS; irq <= ECLIC_NUM_INTERRUPTS; irq ++) {
		if (eclic_interrupt_inner[irq - ECLIC_INTERNAL_NUM_INTERRUPTS] == ulIrq)
			break;
	}
	if (irq > ECLIC_NUM_INTERRUPTS) {
		printf("Error ulIrq!\n");
		return -1;
	} else {
		eclic_clear_pending(irq);
	}
	return 0;
}
