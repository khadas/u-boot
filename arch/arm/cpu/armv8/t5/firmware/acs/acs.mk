SOURCES		+=	acs_entry.S

ifdef CONFIG_MDUMP_COMPRESS
SOURCES		+=	ramdump.c
endif

SOURCES		+=	acs.c

LINKERFILE_T		:=	acs.ld.S
