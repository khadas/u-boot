#ifndef _VERSION_H_
#define _VERSION_H_

#ifdef __cplusplus
extern "C" {
#endif

int version(void);
#ifdef configBL30_VERSION_SAVE
int bl30_plat_save_version(void);

#define PARAM_MESSAGE	0x04
#define VERSION_1	0x01
/***************************************************************************
 * This structure provides version information and the size of the
 * structure, attributes for the structure it represents
 ***************************************************************************/
struct param_header_t {
	uint8_t type;       /* type of the structure */
	uint8_t version;    /* version of this structure */
	uint16_t size;      /* size of this structure in bytes */
	uint32_t attr;      /* attributes: unused bits SBZ */
};

/* build message structure for BL30 to BL2e */
struct build_messages_t {
	struct param_header_t h;
	uint64_t bl2_message_addr;
	uint64_t bl2_message_size;
	uint64_t bl2e_message_addr;
	uint64_t bl2e_message_size;
	uint64_t bl2x_message_addr;
	uint64_t bl2x_message_size;
	uint64_t bl30_message_addr;
	uint64_t bl30_message_size;
	uint64_t bl31_message_addr;
	uint64_t bl31_message_size;
	uint64_t bl32_message_addr;
	uint64_t bl32_message_size;
};
#endif

#ifdef __cplusplus
}
#endif

#endif
