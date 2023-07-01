#include <stdio.h>
#include<stdlib.h>
#include<string.h>
//#include "acs_entry.S"
//#include "timing.c"
//#include <asm/arch/acs.h>
//#include <asm/arch/timing.h>
//#include "timing.c"
typedef struct training_delay_set_ps{
	unsigned	char	ac_trace_delay[10];
	unsigned	char	ac_trace_delay_rev[2];
	unsigned	char	read_dqs_delay[16];
	unsigned	char	read_dq_bit_delay[72];
	unsigned	short	write_dqs_delay[16];

	unsigned	short	write_dq_bit_delay[72];
	unsigned	short	read_dqs_gate_delay[16];
	unsigned	char	soc_bit_vref[36];
	unsigned	char	dram_bit_vref[32];

	unsigned	char	rever1;//read_dqs  read_dq,write_dqs, write_dq
	unsigned	char	dfi_mrl;
	unsigned	char	dfi_hwtmrl;
	unsigned	char	ARdPtrInitVal;
	unsigned	short csr_vrefinglobal;
	unsigned short    csr_dqsrcvcntrl[4];
	unsigned short     csr_pptdqscntinvtrntg0[4];
	unsigned short     csr_pptdqscntinvtrntg1[4];
	unsigned short     csr_seq0bgpr[9];
	unsigned short     csr_dllgainctl;
	unsigned short     csr_dlllockpara;
//	unsigned short     rever2;
}__attribute__ ((packed)) training_delay_set_ps_t;

typedef struct ddr_phy_common_extra_set{
	unsigned short    csr_pllctrl3;
	unsigned short    csr_pptctlstatic[4];
	unsigned short    csr_trainingincdecdtsmen[4];
	unsigned short      csr_tsmbyte0[4];
	unsigned short     csr_hwtcamode;
	unsigned short     csr_hwtlpcsena;
	unsigned short     csr_hwtlpcsenb;
	unsigned short     csr_acsmctrl13;
	unsigned short     csr_acsmctrl23;
	unsigned char     csr_soc_vref_dac1_dfe[36];
}__attribute__ ((packed)) ddr_phy_common_extra_set_t;
#define DWC_AC_PINMUX_TOTAL						28
#define DWC_DFI_PINMUX_TOTAL					26
typedef struct ddr_set{
	unsigned	int		magic;
	unsigned	char	fast_boot[4];// 0   fastboot enable  1 window test margin  2 auto offset after window test 3 auto window test
	//unsigned	int		rsv_int0;
	unsigned	int		ddr_func;
	unsigned	char	board_id;
	//board id reserve,,do not modify
	unsigned	char	version;
	// firmware reserve version,,do not modify
	unsigned	char	DramType;
	//support DramType should confirm with amlogic
	//#define CONFIG_DDR_TYPE_DDR3				0
	//#define CONFIG_DDR_TYPE_DDR4				1
	//#define CONFIG_DDR_TYPE_LPDDR4				2
	//#define CONFIG_DDR_TYPE_LPDDR3				3
	//#define CONFIG_DDR_TYPE_LPDDR2				4
	unsigned	char	DisabledDbyte;
	//use for dram bus 16bit or 32bit,if use 16bit mode ,should disable bit 2,3
	//bit 0 ---use byte 0 ,1 disable byte 0,
	//bit 1 ---use byte 1 ,1 disable byte 1,
	//bit 2 ---use byte 2 ,1 disable byte 2,
	//bit 3 ---use byte 3 ,1 disable byte 3,
	unsigned	char	Is2Ttiming;
	//ddr3/ddr3 use 2t timing,now only support 2t timing
	unsigned	char	HdtCtrl;
	//training information control,do not modify
	unsigned	char	dram_rank_config;
	//support Dram connection type should confirm with amlogic
	//#define CONFIG_DDR0_16BIT_CH0				0x1  //dram total bus width 16bit only use cs0
	//#define CONFIG_DDR0_16BIT_RANK01_CH0		0x4  //dram total bus width 16bit  use cs0 cs1
	//#define CONFIG_DDR0_32BIT_RANK0_CH0			0x2  //dram total bus width 32bit  use cs0
	//#define CONFIG_DDR0_32BIT_RANK01_CH01		0x3    //only for lpddr4,dram total bus width 32bit  use channel a cs0 cs1 channel b cs0 cs1
	//#define CONFIG_DDR0_32BIT_16BIT_RANK0_CH0		0x5    //dram total bus width 32bit only use cs0,but high address use 16bit mode
	//#define CONFIG_DDR0_32BIT_16BIT_RANK01_CH0	0x6   //dram total bus width 32bit  use cs0 cs1,but cs1 use 16bit mode ,current phy not support reserve
	//#define CONFIG_DDR0_32BIT_RANK01_CH0		0x7       //dram total bus width 32bit  use cs0 cs1
	//#define CONFIG_DDR0_32BIT_RANK0_CH01		0x8     //only for lpddr4,dram total bus width 32bit  use channel a cs0  channel b cs0

	/* rsv_char0. update for diagnose type define */
	unsigned	char	diagnose;

	unsigned	short	soc_data_drv_ohm_ps1;
	unsigned	short	dram_data_drv_ohm_ps1;
	unsigned	short	soc_data_odt_ohm_ps1;
	unsigned	short	dram_data_odt_ohm_ps1;
	unsigned	short	dram_data_wr_odt_ohm_ps1;
	#if 0
	/* imem/dmem define */
	unsigned	int		imem_load_addr;
	//system reserve,do not modify
	unsigned	int		dmem_load_addr;
	//system reserve,do not modify
	unsigned	short	imem_load_size;
	#endif
	//system reserve,do not modify
	unsigned	short	dmem_load_size;
	//system reserve,do not modify
	unsigned	int		ddr_base_addr;
	//system reserve,do not modify
	unsigned	int		ddr_start_offset;
	//system reserve,do not modify

	unsigned	short	dram_cs0_size_MB;
	//config cs0 dram size ,like 1G DRAM ,setting 1024
	unsigned	short	dram_cs1_size_MB;
	//config cs1 dram size,like 512M DRAM ,setting 512
	/* align8 */

	unsigned	short	training_SequenceCtrl[2];
	//system reserve,do not modify
	unsigned	char	phy_odt_config_rank[2];
	//unsigned	char	 rever1;
	//unsigned	char	 rever2;
	unsigned	short	rank1_ca_vref_permil;
	//training odt config ,only use for training
	// [0]Odt pattern for accesses targeting rank 0. [3:0] is used for write ODT [7:4] is used for read ODT
	// [1]Odt pattern for accesses targeting rank 1. [3:0] is used for write ODT [7:4] is used for read ODT
	unsigned	int		dfi_odt_config;
	//normal go status od config,use for normal status
	//bit 12.  rank1 ODT default. default value for ODT[1] pins if theres no read/write activity.
	//bit 11.  rank1 ODT write sel.  enable ODT[1] if there's write occur in rank1.
	//bit 10.  rank1 ODT write nsel. enable ODT[1] if theres's write occur in rank0.
	//bit 9.   rank1 odt read sel.   enable ODT[1] if there's read occur in rank1.
	//bit 8.   rank1 odt read nsel.  enable ODT[1] if there's read occure in rank0.
	//bit 4.   rank0 ODT default.    default value for ODT[0] pins if theres no read/write activity.
	//bit 3.   rank0 ODT write sel.  enable ODT[0] if there's write occur in rank0.
	//bit 2.   rank0 ODT write nsel. enable ODT[0] if theres's write occur in rank1.
	//bit 1.   rank0 odt read sel.   enable ODT[0] if there's read occur in rank0.
	//bit 0.   rank0 odt read nsel.  enable ODT[0] if there's read occure in rank1.
	unsigned	short	DRAMFreq[4];
	//config dram frequency,use DRAMFreq[0],other reserve
	unsigned	char	PllBypassEn;
	//system reserve,do not modify
	unsigned	char	ddr_rdbi_wr_enable;
	//system reserve,do not modify
	unsigned	char	ddr_rfc_type;
	//config dram rfc type,according dram type,also can use same dram type max config
	//#define DDR_RFC_TYPE_DDR3_512Mbx1				0
	//#define DDR_RFC_TYPE_DDR3_512Mbx2				1
	//#define DDR_RFC_TYPE_DDR3_512Mbx4				2
	//#define DDR_RFC_TYPE_DDR3_512Mbx8				3
	//#define DDR_RFC_TYPE_DDR3_512Mbx16				4
	//#define DDR_RFC_TYPE_DDR4_2Gbx1					5
	//#define DDR_RFC_TYPE_DDR4_2Gbx2					6
	//#define DDR_RFC_TYPE_DDR4_2Gbx4					7
	//#define DDR_RFC_TYPE_DDR4_2Gbx8					8
	//#define DDR_RFC_TYPE_LPDDR4_2Gbx1				9
	//#define DDR_RFC_TYPE_LPDDR4_3Gbx1				10
	//#define DDR_RFC_TYPE_LPDDR4_4Gbx1				11
	unsigned	char	enable_lpddr4x_mode;
	//system reserve,do not modify
	/* align8 */

	unsigned	int		pll_ssc_mode;
	//
	/* pll ssc config:
	 *
	 *   pll_ssc_mode = (1<<20) | (1<<8) | ([strength] << 4) | [mode],
	 *      ppm = strength * 500
	 *      mode: 0=center, 1=up, 2=down
	 *
	 *   eg:
	 *     1. config 1000ppm center ss. then mode=0, strength=2
	 *        .pll_ssc_mode = (1<<20) | (1<<8) | (2 << 4) | 0,
	 *     2. config 3000ppm down ss. then mode=2, strength=6
	 *        .pll_ssc_mode = (1<<20) | (1<<8) | (6 << 4) | 2,
	 */
	unsigned	short	clk_drv_ohm;
	//config soc clk pin signal driver strength ,select 20,30,40,60ohm
	unsigned	short	cs_drv_ohm;
	//config soc cs0 cs1 pin signal driver strength ,select 20,30,40,60ohm
	unsigned	short	ac_drv_ohm;
	//config soc  normal address command pin driver strength ,select 20,30,40,60ohm
	unsigned	short	soc_data_drv_ohm_p;
	//config soc data pin pull up driver strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned	short	soc_data_drv_ohm_n;
	//config soc data pin pull down driver strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned	short	soc_data_odt_ohm_p;
	//config soc data pin odt pull up strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned	short	soc_data_odt_ohm_n;
	//config soc data pin odt pull down strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned	short	dram_data_drv_ohm;
	//config dram data pin pull up pull down driver strength,ddr3 select 34,40ohm,ddr4 select 34,48ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned	short	dram_data_odt_ohm;
	//config dram data pin odt pull up down strength,ddr3 select 40,60,120ohm,ddr4 select 34,40,48,60,120,240ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned	short	dram_ac_odt_ohm;
	//config dram ac pin odt pull up down strength,use for lpddr4, select 40,48,60,80,120,240ohm
	unsigned	short	soc_clk_slew_rate;
	//system reserve,do not modify
	unsigned	short	soc_cs_slew_rate;
	//system reserve,do not modify
	unsigned	short	soc_ac_slew_rate;
	//system reserve,do not modify
	unsigned	short	soc_data_slew_rate;
	//system reserve,do not modify
	unsigned	short	vref_output_permil; //phy
	//setting same with vref_dram_permil
	unsigned	short	vref_receiver_permil; //soc
	//soc init SOC receiver vref ,config like 500 means 0.5VDDQ,take care ,please follow SI
	unsigned	short	vref_dram_permil;
	//soc init DRAM receiver vref ,config like 500 means 0.5VDDQ,take care ,please follow SI
	unsigned	short	max_core_timing_frequency;
	//use for limited ddr speed core timing parameter,for some old dram maybe have no over speed register
	/* align8 */

	unsigned	char	ac_trace_delay[10];
	unsigned	char	lpddr4_dram_vout_voltage_1_3_2_5_setting;
	unsigned	char	lpddr4_x8_mode;
	unsigned	char	slt_test_function[2];  //[0] slt test function enable,bit 0 enable 4 frequency scan,bit 1 enable force delay line offset ,bit 7 enable skip training function
	//[1],slt test parameter ,use for force delay line offset
	//system reserve,do not modify
	unsigned	short	tdqs2dq;
	unsigned	char	dram_data_wr_odt_ohm;
	unsigned	char	bitTimeControl_2d;
	//system reserve,do not modify
	/* align8 */
    unsigned	char    char_rev1;
	unsigned	char    char_rev2;
	unsigned	int		ddr_dmc_remap[5];
	unsigned	int		dram_rtt_nom_wr_park[2];
	//system reserve,do not modify
	/* align8 */
	unsigned char		ddr_lpddr34_ca_remap[4];
	////use for lpddr3 /lpddr4 ca training data byte lane remap
	unsigned	char	ddr_lpddr34_dq_remap[32];
	////use for lpddr3 /lpddr4 ca pinmux remap
	unsigned	char	ac_pinmux[DWC_AC_PINMUX_TOTAL];
	//use for lpddr3 /lpddr4 ca pinmux remap
	unsigned	char	dfi_pinmux[DWC_DFI_PINMUX_TOTAL];
	unsigned	char    char_rev3;
	unsigned	char    char_rev4;
	ddr_phy_common_extra_set_t cfg_ddr_phy_common_extra_set_t;
	training_delay_set_ps_t	cfg_ddr_training_delay_ps[2];

} __attribute__ ((packed)) ddr_set_t;
typedef struct acs_setting{
		char				acs_magic[5];	//acs setting magic word, make sure this piece of data was right.
		unsigned char		chip_type;		//chip type
		unsigned short		version;		//version of acs_setting struct, for PC tool use.
		unsigned long		acs_set_length;	//length of current struct.

				//ddr setting part, 16 bytes
		char				ddr_magic[5];		//magic word to indicate that following 12 bytes was ddr setting.
		unsigned char		ddr_set_version;	//struct version, for PC tool use.
		unsigned short		ddr_set_length;		//length of ddr struct.
		unsigned long		ddr_set_addr;		//address of ddr setting.

		char				ddr_reg_magic[5];
		unsigned char		ddr_reg_version;
		unsigned short		ddr_reg_length;
		unsigned long		ddr_reg_addr;

		char				pll_magic[5];
		unsigned char		pll_set_version;
		unsigned short		pll_set_length;
		unsigned long		pll_set_addr;

		char				sto_magic[5];
		unsigned char		sto_set_version;
		unsigned short		sto_set_length;
		unsigned long		sto_set_addr;

		char				bl2_regs_magic[5];
		unsigned char		bl2_regs_version;
		unsigned short		bl2_regs_length;
		unsigned long		bl2_regs_addr;

		char				rsv_magic[5];
		unsigned char		rsv_set_version;
		unsigned short		rsv_set_length;
		unsigned long		rsv_set_addr;

		char				board_id[12];
		unsigned short				ddr_struct_size[12];
		unsigned long	ddr_struct_org_size;


}__attribute__ ((packed)) acs_set_t;
typedef struct pll_set{
	unsigned short    cpu_clk;
	unsigned short    pxp;
	unsigned int      spi_ctrl;
	unsigned short    vddee;
	unsigned short    vcck;
	unsigned char     szPad[4];

	unsigned long     lCustomerID;
	unsigned short    debug_mode;
	unsigned char    rsv1;
	unsigned char    ddr_timing_save_mode;

	/* align 8Byte */

	unsigned int      sys_pll_cntl[8];
	unsigned int      ddr_pll_cntl[8];
	unsigned int      fix_pll_cntl[8];

}__attribute__ ((packed)) pll_set_t;

//main acs struct
acs_set_t __acs_set={0};
acs_set_t  *acs_set_p;
acs_set_t  *acs_set_p_f;
pll_set_t __pll_set_t={0};
pll_set_t  *pll_set_p;
pll_set_t  *pll_set_p_f;

ddr_set_t __ddr_set_t={0};
ddr_set_t *ddr_set_p;
ddr_set_t *ddr_set_p_f;

int main(int argc, char **argv)
{
	ddr_set_p_f=&__ddr_set_t;
	pll_set_p_f=&__pll_set_t;
	acs_set_p_f=&__acs_set;
	int i=0;
	int temp1=0;
	int length_f=0;
	for (i = 0; i < argc; i++)
	{
		fprintf(stderr, "\narc[%d]=%s", i,argv[i]);
	}
	FILE *file;
	FILE *file_f;
	char *buffer = NULL;
	char *buffer_f = NULL;
	int finial_size=4096;

	buffer_f=(char *)malloc(finial_size+1);
	if (!buffer_f)
	{
		fprintf(stderr, "buffer_f Memory error!");
		exit(1);
	}
	memset(buffer_f, 0, (finial_size+1));
	unsigned long fileLen;
	char *file_path;
	char file_path_f[256];
	FILE *file_o;
	char file_path_o[256];
	char temp_value=0;

	if (argc>1)
		file_path=	argv[1];
	else
		file_path="acs.bin";

	file = fopen(file_path, "rb");
	if (!file)
	{
		fprintf(stderr, "can't open file %s", "acs.bin");
		free(buffer_f);
		exit(1);
	}

	//printf("\nfile_path==%s",file_path);
	sprintf(file_path_f, "%s_new", file_path);
	sprintf(file_path_o, "%s_old", file_path);
	//printf("\nfile_path_f==%s",file_path_f);
	file_f = fopen(file_path_f, "wb+");
	if (!file_f)
	{
		fprintf(stderr, "can't open file %s", "acs_new.bin");
		fclose(file);
		free(buffer_f);
		exit(1);
	}
	file_o= fopen(file_path_o, "wb+");
	if (!file_f)
	{
		fprintf(stderr, "can't open file %s", "acs_new.bin");
		fclose(file);
		fclose(file_f);
		free(buffer_f);
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	fileLen=ftell(file);
	fseek(file, 0, SEEK_SET);
	printf("\nfileLen==%ld\n", fileLen);

	buffer=(char *)malloc(fileLen+1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!");
		fclose(file);
		fclose(file_f);
		fclose(file_o);
		free(buffer_f);
		exit(1);
	}

	fread(buffer, 1, fileLen+1, file);
	#if 0
	for (i = 0; i < fileLen; i++)
	{
		temp_value=(char)(buffer[i]);
		printf("%02x ",temp_value&0xff);
		if (i%16 == 15)
			printf("\n");
		// if(i%4==3)printf("  ");
	}
	#endif

	fwrite(buffer , sizeof(char), fileLen, file_o);
	fclose(file_o);

	unsigned int value_p = *((unsigned int *)(buffer + 4));
	unsigned int ddr_setting_addr = *(unsigned int *)(buffer + 12);
	unsigned int link_add = (*((unsigned int *)(buffer + 12)) & 0xfffff000);

	acs_set_p=(acs_set_t  *)((buffer)+(((unsigned long)(value_p))-link_add));
	pll_set_p=(pll_set_t  *)((buffer)+(((unsigned long)(acs_set_p->pll_set_addr))-link_add));
	ddr_set_p=(ddr_set_t  *)((buffer)+(((unsigned long)(acs_set_p->ddr_set_addr))-link_add));
	unsigned int ddr_timing_array=(acs_set_p->ddr_set_length/acs_set_p->ddr_struct_org_size);

	if (ddr_setting_addr != (unsigned int)(acs_set_p->ddr_set_addr))
	{
		fprintf(stderr, "link_add not right or exceed 4K! __ddr_setting: 0x%08x, __acs_set: 0x%08x",
			ddr_setting_addr, value_p);
		fclose(file);
		fclose(file_f);
		free(buffer);
		free(buffer_f);
		exit(1);
	}

	#if 1
	printf("\nvalue_p=0x%08x ", value_p);
	printf("\nacs_set_p->ddr_struct_org_size==0x%08x ",(unsigned int )acs_set_p->ddr_struct_org_size);
	printf("\nacs_set_p->ddr_set_addr==0x%08x ",(unsigned int )acs_set_p->ddr_set_addr);
	printf("\nacs_set_p->ddr_set_length==0x%08x ",(unsigned int )acs_set_p->ddr_set_length);
	printf("\nacs_set_p->ddr_array==0x%08x ", ddr_timing_array);

	printf("\nacs_set_p->pll_set_addr==0x%08x ",(unsigned int )acs_set_p->pll_set_addr);
	printf("\n");
	#endif
	if (0 == pll_set_p->ddr_timing_save_mode)
	{
		printf("\nstart reduce timing for mode %d", pll_set_p->ddr_timing_save_mode);

		for (i = 0; i < ((acs_set_p->ddr_set_addr) - link_add); i++)
		{
			buffer_f[i]=((buffer[i])&0xff);
			//printf("\n 0x%0x  ,buffer[i] %02x, buffer_f[i] %02x",i,((buffer[i])&0xff),(buffer_f[i]&0xff));
		}
		#if 0
		for (i = 0; i < finial_size; i++)
		{
			temp_value=(char)(buffer_f[i]);
			printf("%02x ",temp_value&0xff);
			if (i%16 == 15) printf("\n");
			// if(i%4==3)printf("  ");
		}
		#endif

		unsigned int ddr_set_addr_offset=0;
		ddr_set_addr_offset = ((acs_set_p->ddr_set_addr)-link_add);
		printf("\nlink_add=0x%08x", (unsigned int)(unsigned long)(link_add));
	#ifndef NO_DDR_SETTINGS_LAYOUT_OPTIMIZATION
		if (ddr_setting_addr > value_p)
		{
			acs_set_p = (acs_set_t *)((unsigned long)(buffer_f) + \
				(*(unsigned int *)(buffer_f + 4) - link_add));
			printf("\nupdate acs_set_p for layout optimized!");
		}
	#endif

		for (i = 0; i < ddr_timing_array; i++)
		{
			printf("\nconfig %d  (ddr_set_p->board_id)=0x%08x",i,(unsigned int )(unsigned long )((ddr_set_p->board_id)));
			if (ddr_set_p->board_id)
			{
				for (temp1 = (acs_set_p->ddr_struct_org_size); temp1 > 0; temp1--)
				{
					if ((buffer[i*(acs_set_p->ddr_struct_org_size)+ddr_set_addr_offset+temp1-1]) & 0xff)
					{
						acs_set_p->board_id[i]=(ddr_set_p->board_id);
						acs_set_p->ddr_struct_size[i]=((temp1+3)/4)*4;
						printf("\nacs_set_p->ddr_struct_size[i]=0x%08x",
								(unsigned int )(unsigned long )(acs_set_p->ddr_struct_size[i]));
						break;
					}
				}
			}
			else
			{
				acs_set_p->board_id[i]=0;
				acs_set_p->ddr_struct_size[i]=0;

			}
			ddr_set_p=ddr_set_p+1;
		}

		printf("\ncopy ddr timing ddr_set_p=0x%08x\n",(unsigned int )(unsigned long )(ddr_set_p));
		unsigned int ddr_set_addr_offset_f=0;
		ddr_set_addr_offset_f=((acs_set_p->ddr_set_addr)-link_add);
		for (i = 0; i < ddr_timing_array; i++)
		{
			if ((acs_set_p->board_id[i]) & 0xff)
			{
				if (i)
					ddr_set_addr_offset_f=ddr_set_addr_offset_f+acs_set_p->ddr_struct_size[i-1];
				for (temp1=0; temp1 < acs_set_p->ddr_struct_size[i]; temp1++)
				{
					(buffer_f[ddr_set_addr_offset_f+temp1])=(buffer[i*(acs_set_p->ddr_struct_org_size)+ddr_set_addr_offset+temp1])&0xff;
				}
			}
		}
		ddr_set_addr_offset_f=ddr_set_addr_offset_f+acs_set_p->ddr_struct_size[ddr_timing_array-1];
		unsigned int ddr_set_reduce_offset=ddr_timing_array*(acs_set_p->ddr_struct_org_size)+ddr_set_addr_offset-ddr_set_addr_offset_f;

	#ifdef NO_DDR_SETTINGS_LAYOUT_OPTIMIZATION
		if ((ddr_set_reduce_offset/4)%2)  //align byte 8  for other data
		{
			ddr_set_reduce_offset=ddr_set_reduce_offset-4;
			buffer_f[ddr_set_addr_offset_f+0]=0;
			buffer_f[ddr_set_addr_offset_f+1]=0;
			buffer_f[ddr_set_addr_offset_f+2]=0;
			buffer_f[ddr_set_addr_offset_f+3]=0;

			acs_set_p->ddr_struct_size[ddr_timing_array-1] += 4;
			ddr_set_addr_offset_f=ddr_set_addr_offset_f+4;
		}
	#endif

		if (fileLen - ddr_timing_array*(acs_set_p->ddr_struct_org_size) - ddr_set_addr_offset)
		{
			printf("\nError: __ddr_setting is not at end of acs firmware,no layout refine!\n");

	#ifdef NO_DDR_SETTINGS_LAYOUT_OPTIMIZATION
			for (i = 0;i < (fileLen-ddr_timing_array*(acs_set_p->ddr_struct_org_size)-ddr_set_addr_offset); i++)
			{
				buffer_f[ddr_set_addr_offset_f+i]=(buffer[i+ddr_timing_array*(acs_set_p->ddr_struct_org_size)+ddr_set_addr_offset])&0xff;
			}

			*((unsigned int *)((buffer_f)+4)) = *((unsigned int *)((buffer_f)+4)) - ddr_set_reduce_offset;
	#endif
		}

		printf("\nddr_set_reduce_offset=0x%08x",(unsigned int )(unsigned long )(ddr_set_reduce_offset));
		acs_set_p_f = (acs_set_t *)((unsigned long)(buffer_f) + \
				(((unsigned int)(*((unsigned int *)((unsigned long)(buffer_f)+4))))-link_add));
		printf("\n(buffer_f)=0x%0x,acs_set_p_f=0x%0x,acs_set_p_f_offset=0x%0x \n",
				(unsigned int)(unsigned long)(buffer_f), (unsigned int)(unsigned long)acs_set_p_f,
				(unsigned int)(*((unsigned int *)((unsigned long)(buffer_f)+4))) - link_add);

	#ifdef NO_DDR_SETTINGS_LAYOUT_OPTIMIZATION
		printf("\nacs_set_p->ddr_reg_addr=0x%08x\n",(unsigned int )(unsigned long )(acs_set_p->ddr_reg_addr));
		printf("\nacs_set_p_f->ddr_reg_addr=0x%08x\n",(unsigned int )(unsigned long )(acs_set_p_f->ddr_reg_addr));
		acs_set_p_f->ddr_reg_addr=acs_set_p->ddr_reg_addr-ddr_set_reduce_offset;
		//printf("\nwrite back 002  \n");
		acs_set_p_f->pll_set_addr=acs_set_p->pll_set_addr-ddr_set_reduce_offset;
		//printf("\nwrite back 003  \n");
		acs_set_p_f->bl2_regs_addr=acs_set_p->bl2_regs_addr-ddr_set_reduce_offset;
		//printf("\nwrite back 004  \n");
		acs_set_p_f->sto_set_addr=acs_set_p->sto_set_addr-ddr_set_reduce_offset;
		acs_set_p_f->rsv_set_addr=acs_set_p->rsv_set_addr-ddr_set_reduce_offset;
		acs_set_p_f->ddr_set_addr=acs_set_p->ddr_set_addr;//-ddr_set_reduce_offset;
		//unsigned long		sto_set_addr;
	#endif

		acs_set_p_f->ddr_set_length=acs_set_p->ddr_set_length-ddr_set_reduce_offset;
		length_f = fileLen - ddr_set_reduce_offset;

	#if 1
		printf("\nwrite back  \n");
		for (i = 0; i < length_f; i++)
		{
			temp_value=(char)(buffer_f[i]);
			printf("%02x ",temp_value&0xff);
			if (i%16 == 15)	printf("\n");
			// if(i%4==3)printf("  ");
		}
	#endif

	}
	else
	{
		length_f=fileLen;
		for (i = 0; i < (fileLen); i++)
		{
			buffer_f[i] = ((buffer[i]) & 0xff);
			//printf("\n 0x%0x  ,buffer[i] %02x, buffer_f[i] %02x",i,((buffer[i])&0xff),(buffer_f[i]&0xff));
		}
	}

	printf("\nfinal timing size:%dbytes, while original size:%dbytes\n", length_f, (unsigned int)fileLen);
	if (length_f > 4096)
		printf("\nwarning timing size over limit");

	fwrite(buffer_f , sizeof(char), length_f, file_f);
	fclose(file_f);
	fclose(file);
	free(buffer);

	file = fopen(file_path, "wb+");
	if (!file)
	{
		fprintf(stderr, "can't open file %s", file_path);
		free(buffer_f);
		exit(1);
	}
	fwrite(buffer_f , sizeof(char), length_f, file);
	fclose(file);
	free(buffer_f);
}
