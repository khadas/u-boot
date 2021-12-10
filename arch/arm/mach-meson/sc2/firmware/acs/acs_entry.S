
	.globl	acs_entry

acs_entry:
	.word	__param_hdr
	.word	__bl2_reg
	.word	__board_clk_setting
	.word	__bl2_ops_reg
	.word	__store_para
	.word	__ddr_setting
#ifdef CONFIG_MDUMP_COMPRESS
	.word	__ramdump_data
#endif
	.rept	10
	.word	0x0
	.endr
