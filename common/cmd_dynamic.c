/*
 * (C) Copyright 2013 Amlogic, Inc
 *
 * This file is used to get metadata from super partition
 * More detail to check the command "run readMetadata" usage
 *
 * xindong.xu@amlogic.com,
 * 2018-08-15 @ ShangHai
 *
 */
#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <config.h>
#include <asm/arch/io.h>
#include <emmc_partitions.h>

#ifdef CONFIG_BOOTLOADER_CONTROL_BLOCK
extern int store_read_ops(
    unsigned char *partition_name,
    unsigned char * buf, uint64_t off, uint64_t size);
extern int store_write_ops(
    unsigned char *partition_name,
    unsigned char * buf, uint64_t off, uint64_t size);

/* Magic signature for LpMetadataGeometry. */
#define LP_METADATA_GEOMETRY_MAGIC 0x616c4467

/* Space reserved for geometry information. */
#define LP_METADATA_GEOMETRY_SIZE 4096

#define LP_METADATA_HEADER_SIZE 124

#define SUPERBUF_SIZE 16384

/* Magic signature for LpMetadataHeader. */
#define LP_METADATA_HEADER_MAGIC 0x414C5030

/* Current metadata version. */
#define LP_METADATA_MAJOR_VERSION 10
#define LP_METADATA_MINOR_VERSION 0

/* Attributes for the LpMetadataPartition::attributes field.
 *
 * READONLY - The partition should not be considered writable. When used with
 * device mapper, the block device will be created as read-only.
 */
#define LP_PARTITION_ATTR_NONE 0x0
#define LP_PARTITION_ATTR_READONLY (1 << 0)

/* This flag is only intended to be used with super_empty.img and super.img on
 * retrofit devices. On these devices there are A and B super partitions, and
 * we don't know ahead of time which slot the image will be applied to.
 *
 * If set, the partition name needs a slot suffix applied. The slot suffix is
 * determined by the metadata slot number (0 = _a, 1 = _b).
 */
#define LP_PARTITION_ATTR_SLOT_SUFFIXED (1 << 1)

/* Mask that defines all valid attributes. */
#define LP_PARTITION_ATTRIBUTE_MASK (LP_PARTITION_ATTR_READONLY | LP_PARTITION_ATTR_SLOT_SUFFIXED)

/* Default name of the physical partition that holds logical partition entries.
 * The layout of this partition will look like:
 *
 *     +--------------------+
 *     | Disk Geometry      |
 *     +--------------------+
 *     | Geometry Backup    |
 *     +--------------------+
 *     | Metadata           |
 *     +--------------------+
 *     | Backup Metadata    |
 *     +--------------------+
 *     | Logical Partitions |
 *     +--------------------+
 */
#define LP_METADATA_DEFAULT_PARTITION_NAME "super"

/* Size of a sector is always 512 bytes for compatibility with the Linux kernel. */
#define LP_SECTOR_SIZE 512

/* Amount of space reserved at the start of every super partition to avoid
 * creating an accidental boot sector.
 */
#define LP_PARTITION_RESERVED_BYTES 4096

/* This structure is stored at block 0 in the first 4096 bytes of the
 * partition, and again in the following block. It is never modified and
 * describes how logical partition information can be located.
 */
typedef struct LpMetadataGeometry {
    /*  0: Magic signature (LP_METADATA_GEOMETRY_MAGIC). */
    uint32_t magic;

    /*  4: Size of the LpMetadataGeometry struct. */
    uint32_t struct_size;

    /*  8: SHA256 checksum of this struct, with this field set to 0. */
    uint8_t checksum[32];

    /* 40: Maximum amount of space a single copy of the metadata can use. This
     * must be a multiple of LP_SECTOR_SIZE.
     */
    uint32_t metadata_max_size;

    /* 44: Number of copies of the metadata to keep. For A/B devices, this
     * will be 2. For an A/B/C device, it would be 3, et cetera. For Non-A/B
     * it will be 1. A backup copy of each slot is kept, so if this is "2",
     * there will be four copies total.
     */
    uint32_t metadata_slot_count;

    /* 48: Logical block size. This is the minimal alignment for partition and
     * extent sizes, and it must be a multiple of LP_SECTOR_SIZE. Note that
     * this must be equal across all LUNs that comprise the super partition,
     * and thus this field is stored in the geometry, not per-device.
     */
    uint32_t logical_block_size;
} __attribute__((packed)) LpMetadataGeometry;

/* The logical partition metadata has a number of tables; they are described
 * in the header via the following structure.
 *
 * The size of the table can be computed by multiplying entry_size by
 * num_entries, and the result must not overflow a 32-bit signed integer.
 */
typedef struct LpMetadataTableDescriptor {
    /*  0: Location of the table, relative to end of the metadata header. */
    uint32_t offset;
    /*  4: Number of entries in the table. */
    uint32_t num_entries;
    /*  8: Size of each entry in the table, in bytes. */
    uint32_t entry_size;
} __attribute__((packed)) LpMetadataTableDescriptor;

/* Binary format for the header of the logical partition metadata format.
 *
 * The format has three sections. The header must occur first, and the
 * proceeding tables may be placed in any order after.
 *
 *  +-----------------------------------------+
 *  | Header data - fixed size                |
 *  +-----------------------------------------+
 *  | Partition table - variable size         |
 *  +-----------------------------------------+
 *  | Partition table extents - variable size |
 *  +-----------------------------------------+
 *
 * The "Header" portion is described by LpMetadataHeader. It will always
 * precede the other three blocks.
 *
 * All fields are stored in little-endian byte order when serialized.
 *
 * This struct is versioned; see the |major_version| and |minor_version|
 * fields.
 */
typedef struct LpMetadataHeader {
    /*  0: Four bytes equal to LP_METADATA_HEADER_MAGIC. */
    uint32_t magic;

    /*  4: Version number required to read this metadata. If the version is not
     * equal to the library version, the metadata should be considered
     * incompatible.
     */
    uint16_t major_version;

    /*  6: Minor version. A library supporting newer features should be able to
     * read metadata with an older minor version. However, an older library
     * should not support reading metadata if its minor version is higher.
     */
    uint16_t minor_version;

    /*  8: The size of this header struct. */
    uint32_t header_size;

    /* 12: SHA256 checksum of the header, up to |header_size| bytes, computed as
     * if this field were set to 0.
     */
    uint8_t header_checksum[32];

    /* 44: The total size of all tables. This size is contiguous; tables may not
     * have gaps in between, and they immediately follow the header.
     */
    uint32_t tables_size;

    /* 48: SHA256 checksum of all table contents. */
    uint8_t tables_checksum[32];

    /* 80: Partition table descriptor. */
    LpMetadataTableDescriptor partitions;
    /* 92: Extent table descriptor. */
    LpMetadataTableDescriptor extents;
    /* 104: Updateable group descriptor. */
    LpMetadataTableDescriptor groups;
    /* 116: Block device table. */
    LpMetadataTableDescriptor block_devices;
} __attribute__((packed)) LpMetadataHeader;

/* This struct defines a logical partition entry, similar to what would be
 * present in a GUID Partition Table.
 */
typedef struct LpMetadataPartition {
    /*  0: Name of this partition in ASCII characters. Any unused characters in
     * the buffer must be set to 0. Characters may only be alphanumeric or _.
     * The name must include at least one ASCII character, and it must be unique
     * across all partition names. The length (36) is the same as the maximum
     * length of a GPT partition name.
     */
    char name[36];

    /* 36: Attributes for the partition (see LP_PARTITION_ATTR_* flags above). */
    uint32_t attributes;

    /* 40: Index of the first extent owned by this partition. The extent will
     * start at logical sector 0. Gaps between extents are not allowed.
     */
    uint32_t first_extent_index;

    /* 44: Number of extents in the partition. Every partition must have at
     * least one extent.
     */
    uint32_t num_extents;

    /* 48: Group this partition belongs to. */
    uint32_t group_index;
} __attribute__((packed)) LpMetadataPartition;

/* This extent is a dm-linear target, and the index is an index into the
 * LinearExtent table.
 */
#define LP_TARGET_TYPE_LINEAR 0

/* This extent is a dm-zero target. The index is ignored and must be 0. */
#define LP_TARGET_TYPE_ZERO 1

/* This struct defines an extent entry in the extent table block. */
typedef struct LpMetadataExtent {
    /*  0: Length of this extent, in 512-byte sectors. */
    uint64_t num_sectors;

    /*  8: Target type for device-mapper (see LP_TARGET_TYPE_* values). */
    uint32_t target_type;

    /* 12: Contents depends on target_type.
     *
     * LINEAR: The sector on the physical partition that this extent maps onto.
     * ZERO: This field must be 0.
     */
    uint64_t target_data;

    /* 20: Contents depends on target_type.
     *
     * LINEAR: Must be an index into the block devices table.
     * ZERO: This field must be 0.
     */
    uint32_t target_source;
} __attribute__((packed)) LpMetadataExtent;

/* This struct defines an entry in the groups table. Each group has a maximum
 * size, and partitions in a group must not exceed that size. There is always
 * a "default" group of unlimited size, which is used when not using update
 * groups or when using overlayfs or fastbootd.
 */
typedef struct LpMetadataPartitionGroup {
    /*  0: Name of this group. Any unused characters must be 0. */
    char name[36];

    /* 36: Flags (see LP_GROUP_*). */
    uint32_t flags;

    /* 40: Maximum size in bytes. If 0, the group has no maximum size. */
    uint64_t maximum_size;
} __attribute__((packed)) LpMetadataPartitionGroup;

/* This flag is only intended to be used with super_empty.img and super.img on
 * retrofit devices. If set, the group needs a slot suffix to be interpreted
 * correctly. The suffix is automatically applied by ReadMetadata().
 */
#define LP_GROUP_SLOT_SUFFIXED (1 << 0)

/* This struct defines an entry in the block_devices table. There must be at
 * least one device, and the first device must represent the partition holding
 * the super metadata.
 */
typedef struct LpMetadataBlockDevice {
    /* 0: First usable sector for allocating logical partitions. this will be
     * the first sector after the initial geometry blocks, followed by the
     * space consumed by metadata_max_size*metadata_slot_count*2.
     */
    uint64_t first_logical_sector;

    /* 8: Alignment for defining partitions or partition extents. For example,
     * an alignment of 1MiB will require that all partitions have a size evenly
     * divisible by 1MiB, and that the smallest unit the partition can grow by
     * is 1MiB.
     *
     * Alignment is normally determined at runtime when growing or adding
     * partitions. If for some reason the alignment cannot be determined, then
     * this predefined alignment in the geometry is used instead. By default
     * it is set to 1MiB.
     */
    uint32_t alignment;

    /* 12: Alignment offset for "stacked" devices. For example, if the "super"
     * partition itself is not aligned within the parent block device's
     * partition table, then we adjust for this in deciding where to place
     * |first_logical_sector|.
     *
     * Similar to |alignment|, this will be derived from the operating system.
     * If it cannot be determined, it is assumed to be 0.
     */
    uint32_t alignment_offset;

    /* 16: Block device size, as specified when the metadata was created. This
     * can be used to verify the geometry against a target device.
     */
    uint64_t size;

    /* 24: Partition name in the GPT. Any unused characters must be 0. */
    char partition_name[36];

    /* 60: Flags (see LP_BLOCK_DEVICE_* flags below). */
    uint32_t flags;
} __attribute__((packed)) LpMetadataBlockDevice;

/* This flag is only intended to be used with super_empty.img and super.img on
 * retrofit devices. On these devices there are A and B super partitions, and
 * we don't know ahead of time which slot the image will be applied to.
 *
 * If set, the block device needs a slot suffix applied before being used with
 * IPartitionOpener. The slot suffix is determined by the metadata slot number
 * (0 = _a, 1 = _b).
 */
#define LP_BLOCK_DEVICE_SLOT_SUFFIXED (1 << 0)


typedef struct PartitionList
{
    char name[128];
    struct PartitionList* next;
}__attribute__((packed)) PartitionList;

PartitionList* part_list = NULL;

void printlist(void)
{
    PartitionList* node = part_list;
    while (NULL != node)
    {
        printf("name: %s\n",node->name);
        node = node->next;
    }
}

int GetPrimaryGeometryOffset(void) {
    int offset = LP_PARTITION_RESERVED_BYTES;
    return offset;
}

int GetBackupGeometryOffset(void) {
    int offset = GetPrimaryGeometryOffset() + LP_METADATA_GEOMETRY_SIZE;
    return offset;
}

int GetPrimaryMetadataOffset(LpMetadataGeometry* geometry, int slot_number) {
    int offset = LP_PARTITION_RESERVED_BYTES + (LP_METADATA_GEOMETRY_SIZE * 2) +
                     geometry->metadata_max_size * slot_number;
    printf("GetPrimaryMetadataOffset : %d\n", offset);
    return offset;
}

int GetBackupMetadataOffset(LpMetadataGeometry* geometry, int slot_number) {
    int start = LP_PARTITION_RESERVED_BYTES + (LP_METADATA_GEOMETRY_SIZE * 2) +
                   geometry->metadata_max_size * geometry->metadata_slot_count;
    printf("GetBackupMetadataOffset : %d\n", start + geometry->metadata_max_size * slot_number);
    return start + geometry->metadata_max_size * slot_number;
}

int GetTotalMetadataSize(int metadata_max_size, int max_slots) {
    return LP_PARTITION_RESERVED_BYTES +
           (LP_METADATA_GEOMETRY_SIZE + metadata_max_size * max_slots) * 2;
}

int ParseGeometry(const void* buffer, LpMetadataGeometry* geometry) {
    memcpy(geometry, buffer, sizeof(LpMetadataGeometry));

    // Check the magic signature.
    if (geometry->magic != LP_METADATA_GEOMETRY_MAGIC) {
        printf("Logical partition metadata has invalid geometry magic signature\n");
        return -1;
    }
    // Reject if the struct size is larger than what we compiled. This is so we
    // can compute a checksum with the |struct_size| field rather than using
    // sizeof.
    if (geometry->struct_size > sizeof(LpMetadataGeometry)) {
        printf("Logical partition metadata has unrecognized fields.\n");
        return -1;
    }

    // Check that the struct size is equal (this will have to change if we ever
    // change the struct size in a release).
    if (geometry->struct_size != sizeof(LpMetadataGeometry)) {
        printf("Logical partition metadata has invalid struct size.\n");
        return -1;
    }
    if (geometry->metadata_slot_count == 0) {
        printf("Logical partition metadata has invalid slot count.\n");
        return -1;
    }
    if (geometry->metadata_max_size % LP_SECTOR_SIZE != 0) {
        printf("Metadata max size is not sector-aligned.\n");
        return -1;
    }
    return 0;
}

int ReadPrimaryGeometry(char *superbuf, LpMetadataGeometry* geometry) {
    char buffer[LP_METADATA_GEOMETRY_SIZE];
    memcpy(buffer, superbuf+LP_PARTITION_RESERVED_BYTES, LP_METADATA_GEOMETRY_SIZE);
    return ParseGeometry(buffer, geometry);
}

int ReadBackupGeometry(char *superbuf, LpMetadataGeometry* geometry) {
    char buffer[LP_METADATA_GEOMETRY_SIZE];
    memcpy(buffer, superbuf+LP_PARTITION_RESERVED_BYTES+LP_METADATA_GEOMETRY_SIZE, LP_METADATA_GEOMETRY_SIZE);
    return ParseGeometry(buffer, geometry);
}

// Read and validate geometry information from a block device that holds
// logical partitions. If the information is corrupted, this will attempt
// to read it from a secondary backup location.
int ReadLogicalPartitionGeometry(char *superbuf, LpMetadataGeometry* geometry) {
    if (ReadPrimaryGeometry(superbuf, geometry) == 0) {
        return 0;
    }
    return ReadBackupGeometry(superbuf, geometry);
}

static int ValidateMetadataHeader(LpMetadataHeader* header) {
    // Do basic validation of key metadata bits.
    if (header->magic != LP_METADATA_HEADER_MAGIC) {
        printf("Logical partition metadata has invalid magic value.\n");
        return -1;
    }
    // Check that the version is compatible.
    if (header->major_version != LP_METADATA_MAJOR_VERSION ||
        header->minor_version > LP_METADATA_MINOR_VERSION) {
        printf("Logical partition metadata has incompatible version.\n");
        return -1;
    }
    /*if (!ValidateTableBounds(header, &header->partitions) ||
        !ValidateTableBounds(header, &header->extents) ||
        !ValidateTableBounds(header, &header->groups) ||
        !ValidateTableBounds(header, &header->block_devices)) {
        printf("Logical partition metadata has invalid table bounds.\n");
        return -1;
    }*/
    // Check that table entry sizes can accomodate their respective structs. If
    // table sizes change, these checks will have to be adjusted.
    if (header->partitions.entry_size != sizeof(LpMetadataPartition)) {
        printf("Logical partition metadata has invalid partition table entry size.\n");
        return -1;
    }
    if (header->extents.entry_size != sizeof(LpMetadataExtent)) {
        printf("Logical partition metadata has invalid extent table entry size.\n");
        return -1;
    }
    if (header->groups.entry_size != sizeof(LpMetadataPartitionGroup)) {
        printf("Logical partition metadata has invalid group table entry size.\n");
        return -1;
    }
    return 0;
}


int ReadMetadataHeader(char *superbuf, LpMetadataHeader* header,
                   LpMetadataGeometry* geometry, int slot_number) {
    char* buffer = NULL;
    int cursor = 0;
    PartitionList* tail = NULL ;
    PartitionList* node = NULL ;
    int ishead = 0;
    int i;

    printf("metaoffset: %d\n", GetPrimaryMetadataOffset(geometry, slot_number));

    memcpy(header, superbuf + GetPrimaryMetadataOffset(geometry, slot_number), sizeof(LpMetadataHeader));
    if (ValidateMetadataHeader(header) != 0) {
        return -1;
    }

    printf("header table size = %d\n", header->tables_size);
    buffer = (char*)malloc(header->tables_size);
    if (buffer == NULL) {
        printf("Out of memory reading logical partition tables.\n");
        return -1;
    }

    memcpy(buffer, superbuf + GetPrimaryMetadataOffset(geometry, slot_number) + sizeof(LpMetadataHeader), header->tables_size);
    cursor = GetPrimaryMetadataOffset(geometry, slot_number) + sizeof(LpMetadataHeader) + header->partitions.offset;

    printf("header->partitions.offset: %d\n", header->partitions.offset);
    printf("cursor: %d\n", cursor);

    // ValidateTableSize ensured that |cursor| is valid for the number of
    // entries in the table.
    for (i = 0; i < header->partitions.num_entries; i++) {
        LpMetadataPartition partition;
        memcpy(&partition, superbuf + cursor, sizeof(partition));
        cursor += header->partitions.entry_size;

        printf("partition name : %s\n", partition.name);

        if (partition.attributes & ~LP_PARTITION_ATTRIBUTE_MASK) {
            printf("Logical partition has invalid attribute set.\n");
            if (buffer)
                free (buffer);
            return -1;
        }
        if (partition.first_extent_index + partition.num_extents < partition.first_extent_index) {
            printf("Logical partition first_extent_index + num_extents overflowed.\n");
            if (buffer)
                free (buffer);
            return -1;
        }
        if (partition.first_extent_index + partition.num_extents > header->extents.num_entries) {
            printf("Logical partition has invalid extent list.\n");
            if (buffer)
                free (buffer);
            return -1;
        }
        if (partition.group_index >= header->groups.num_entries) {
            printf("Logical partition has invalid group index.\n");
            if (buffer)
                free (buffer);
            return -1;
        }

        node = malloc(sizeof(PartitionList));
        strcpy(node->name, partition.name);
        if (ishead == 0)
        {
            part_list = node ;
            part_list->next = NULL ;
            tail = node;
            ishead = -1;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
        //metadata->partitions.push_back(partition);
    }

    if (NULL != tail)
        tail->next = NULL;

    if (buffer)
        free (buffer);

    return 0;
}

/*void dump_mem(char * buffer, int count)
{
    int i;
    printf("***********************************************\n");
    for (i=0; i<count ; i++)
    {
        if (i % 16 == 0)
            printf("\n");
        printf("%02x ", buffer[i]);
    }
    printf("\n");
    printf("***********************************************\n");
}*/


int do_ReadMetadata(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *partition = "super";
    char* superbuf;
    LpMetadataGeometry geometry;
    LpMetadataHeader metadata_header;
    char *slot;
    int slot_number = 0;
    superbuf = (char*)malloc(SUPERBUF_SIZE);
    if (superbuf == NULL) {
        printf("Out of memory reading logical partition tables.\n");
        goto ERR;
    }

    if (dynamic_partition) {
        if (store_read_ops((unsigned char *)partition,
            (unsigned char *)superbuf, 0, SUPERBUF_SIZE) < 0) {
            printf("failed to store read %s.\n", partition);
            goto ERR;
        }

        //dump_mem(superbuf, SUPERBUF_SIZE);

        if (ReadLogicalPartitionGeometry(superbuf, &geometry) != 0) {
            goto ERR;
        }

        if (has_boot_slot == 1) {
            slot = getenv("slot-suffixes");
		    printf("slot-suffixes: %s\n", slot);
		    if (strcmp(slot, "0") == 0) {
			    slot_number = 0;
		    } else if (strcmp(slot, "1") == 0) {
			    slot_number = 1;
		    }
        }

        ReadMetadataHeader(superbuf, &metadata_header, &geometry, slot_number);

        //printlist();
    }

    if (superbuf)
        free (superbuf);

    return 0;

ERR:
    if (superbuf)
        free (superbuf);
    return -1;

}

int is_partition_logical(char* parition_name) {
    run_command("readMetadata", 0);
    PartitionList* node = part_list;
    while (NULL != node)
    {
        printf("name: %s\n",node->name);
        if (strcmp(node->name, parition_name) == 0)
            return 0;
        node = node->next;
    }

    return -1;
}

#else
static int do_ReadMetadata(
    cmd_tbl_t * cmdtp,
    int flag,
    int argc,
    char * const argv[]) {
    // Do-Nothing!
    return 0;
}
#endif /* CONFIG_BOOTLOADER_CONTROL_BLOCK */

U_BOOT_CMD(
    readMetadata, 1, 0, do_ReadMetadata,
    "readMetadata",
    "\nThis command will read metadata in super \n"
    "So you can execute command: readMetadata"
);

