
	.globl	acs_entry

acs_entry:
	.word	__RO_START__
	.word	__chip_acs_set
	.word	0x0
	.word	0x0

