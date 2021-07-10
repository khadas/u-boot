
	.globl	acs_entry

acs_entry:
	.word	__param_hdr
	.word	__bl2_reg
	.word	__board_clk_setting
	.word	__bl2_ops_reg
	.word	__store_para
	.word	__ddr_setting
	.word	__bl2_ddr_reg_data
	.word	__ddr_parameter_reg_index
	.rept	10
	.word	0x0
	.endr

