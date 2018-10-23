/*
 * \file        optimus_fat.c
 * \brief       fopen/fread/fclose for fat32
 *
 * \version     1.0.0
 * \date        2018/11/08
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2018 Amlogic. All Rights Reserved.
 *
 */
// SPDX-License-Identifier: GPL-2.0+
/*
 * fat.c
 *
 * R/O (V)FAT 12/16/32 filesystem implementation by Marcus Sundberg
 *
 * 2002-07-28 - rjones@nexus-tech.net - ported to ppcboot v1.1.6
 * 2003-03-10 - kharris@nexus-tech.net - ported to uboot
 * 2018-11-12 - yihui.wu@amlogic.com - ported for speed up fragment reading
 */
#include "../v2_burning_i.h"
#include "../v2_sdc_burn/optimus_sdc_burn_i.h"
#include <partition_table.h>
#include <common.h>
#include <blk.h>
#include <config.h>
#include <exports.h>
#include <fat.h>
#include <fs.h>
#include <asm/byteorder.h>
#include <part.h>
#include <malloc.h>
#include <memalign.h>
#include <linux/compiler.h>
#include <linux/ctype.h>
#undef debug
#define debug(fmt...) //printf("%s[%3d]", __func__, __LINE__),printf(fmt)

/*
 * Directory iterator, to simplify filesystem traversal
 *
 * Implements an iterator pattern to traverse directory tables,
 * transparently handling directory tables split across multiple
 * clusters, and the difference between FAT12/FAT16 root directory
 * (contiguous) and subdirectories + FAT32 root (chained).
 *
 * Rough usage:
 *
 *   for (fat_itr_root(&itr, fsdata); fat_itr_next(&itr); ) {
 *      // to traverse down to a subdirectory pointed to by
 *      // current iterator position:
 *      fat_itr_child(&itr, &itr);
 *   }
 *
 * For more complete example, see fat_itr_resolve()
 */

typedef struct {
	fsdata    *fsdata;        /* filesystem parameters */
	unsigned   clust;         /* current cluster */
	int        last_cluster;  /* set once we've read last cluster */
	int        is_root;       /* is iterator at root directory */
	int        remaining;     /* remaining dent's in current cluster */

	/* current iterator position values: */
	dir_entry *dent;          /* current directory entry */
	char       l_name[VFAT_MAXLEN_BYTES];    /* long (vfat) name */
	char       s_name[14];    /* short 8.3 name */
	char      *name;          /* l_name if there is one, else s_name */

	/* storage for current cluster in memory: */
	u8         block[MAX_CLUSTSIZE] __aligned(ARCH_DMA_MINALIGN);
} fat_itr;

static int fat_itr_isdir(fat_itr *itr);

struct fopen_para {
    fat_itr     itr;
    fsdata      fsdata;
    __u32       curclust;
    __u32       startclust;
    int         bytesperclust;
    int64_t     fileOffset;
    int64_t     filesize;
};
#define MAX_FILE_NUM    1
static struct fopen_para* _hFile[MAX_FILE_NUM];

/*
 * Convert a string to lowercase.  Converts at most 'len' characters,
 * 'len' may be larger than the length of 'str' if 'str' is NULL
 * terminated.
 */
static void downcase(char *str, size_t len)
{
    while (*str != '\0' && len--) {
        *str = tolower(*str);
        str++;
    }
}

static struct blk_desc *cur_dev;
static disk_partition_t cur_part_info;

#define DOS_BOOT_MAGIC_OFFSET	0x1fe
#define DOS_FS_TYPE_OFFSET	0x36
#define DOS_FS32_TYPE_OFFSET	0x52

static int disk_read(__u32 block, __u32 nr_blocks, void *buf)
{
    ulong ret;

    if (!cur_dev) {
        DWN_ERR("not cur_dev in disk_read\n");
        return -1;
    }

    ret = blk_dread(cur_dev, cur_part_info.start + block, nr_blocks, buf);

    if (ret != nr_blocks) {
        DWN_ERR("Want read %u blocks, but only %lu\n", nr_blocks, ret);
        return -1;
    }

    return ret;
}

/*
 * Extract zero terminated short name from a directory entry.
 */
static void get_name(dir_entry *dirent, char *s_name)
{
    char *ptr;

    memcpy(s_name, dirent->name, 8);
    s_name[8] = '\0';
    ptr = s_name;
    while (*ptr && *ptr != ' ')
        ptr++;
    if (dirent->lcase & CASE_LOWER_BASE)
        downcase(s_name, (unsigned)(ptr - s_name));
    if (dirent->ext[0] && dirent->ext[0] != ' ') {
        *ptr++ = '.';
        memcpy(ptr, dirent->ext, 3);
        if (dirent->lcase & CASE_LOWER_EXT)
            downcase(ptr, 3);
        ptr[3] = '\0';
        while (*ptr && *ptr != ' ')
            ptr++;
    }
    *ptr = '\0';
    if (*s_name == DELETED_FLAG)
        *s_name = '\0';
    else if (*s_name == aRING)
        *s_name = DELETED_FLAG;
}

static int flush_dirty_fat_buffer(fsdata *mydata);
#if !defined(CONFIG_FAT_WRITE)
/* Stub for read only operation */
static int flush_dirty_fat_buffer(fsdata *mydata)
{
    (void)(mydata);
    return 0;
}
#endif

/*
 * Get the entry at index 'entry' in a FAT (12/16/32) table.
 * On failure 0x00 is returned.
 */
static __u32 get_fatent(fsdata *mydata, __u32 entry)
{
    __u32 bufnum;
    __u32 offset, off8;
    __u32 ret = 0x00;

    if (CHECK_CLUST(entry, mydata->fatsize)) {
        printf("Error: Invalid FAT entry: 0x%08x\n", entry);
        return ret;
    }

    switch (mydata->fatsize) {
        case 32:
            bufnum = entry / FAT32BUFSIZE;
            offset = entry - bufnum * FAT32BUFSIZE;
            break;
        case 16:
            bufnum = entry / FAT16BUFSIZE;
            offset = entry - bufnum * FAT16BUFSIZE;
            break;
        case 12:
            bufnum = entry / FAT12BUFSIZE;
            offset = entry - bufnum * FAT12BUFSIZE;
            break;

        default:
            /* Unsupported FAT size */
            return ret;
    }

    debug("FAT%d: entry: 0x%08x = %d, offset: 0x%04x = %d\n",
            mydata->fatsize, entry, entry, offset, offset);

    /* Read a new block of FAT entries into the cache. */
    if (bufnum != mydata->fatbufnum) {
        __u32 getsize = FATBUFBLOCKS;
        __u8 *bufptr = mydata->fatbuf;
        __u32 fatlength = mydata->fatlength;
        __u32 startblock = bufnum * FATBUFBLOCKS;

        /* Cap length if fatlength is not a multiple of FATBUFBLOCKS */
        if (startblock + getsize > fatlength)
            getsize = fatlength - startblock;

        startblock += mydata->fat_sect;	/* Offset from start of disk */

        /* Write back the fatbuf to the disk */
        if (flush_dirty_fat_buffer(mydata) < 0)
            return -1;

        if (disk_read(startblock, getsize, bufptr) < 0) {
            debug("Error reading FAT blocks\n");
            return ret;
        }
        mydata->fatbufnum = bufnum;
    }

    /* Get the actual entry from the table */
    switch (mydata->fatsize) {
        case 32:
            ret = FAT2CPU32(((__u32 *) mydata->fatbuf)[offset]);
            break;
        case 16:
            ret = FAT2CPU16(((__u16 *) mydata->fatbuf)[offset]);
            break;
        case 12:
            off8 = (offset * 3) / 2;
            /* fatbut + off8 may be unaligned, read in byte granularity */
            ret = mydata->fatbuf[off8] + (mydata->fatbuf[off8 + 1] << 8);

            if (offset & 0x1)
                ret >>= 4;
            ret &= 0xfff;
    }
    debug("FAT%d: ret: 0x%08x, entry: 0x%08x, offset: 0x%04x\n",
            mydata->fatsize, ret, entry, offset);

    return ret;
}

/*
 * Read at most 'size' bytes from the specified cluster into 'buffer'.
 * Return 0 on success, -1 otherwise.
 */
static int
get_cluster(fsdata *mydata, __u32 clustnum, __u8 *buffer, unsigned long size)
{
    __u32 idx = 0;
    __u32 startsect;
    int ret;

    if (clustnum > 0) {
        startsect = clust_to_sect(mydata, clustnum);
    } else {
        startsect = mydata->rootdir_sect;
    }

    debug("gc - clustnum: %d, startsect: %d\n", clustnum, startsect);

    if ((unsigned long)buffer & (ARCH_DMA_MINALIGN - 1)) {
        ALLOC_CACHE_ALIGN_BUFFER(__u8, tmpbuf, mydata->sect_size);

        printf("FAT: Misaligned buffer address (%p)\n", buffer);

        while (size >= mydata->sect_size) {
            ret = disk_read(startsect++, 1, tmpbuf);
            if (ret != 1) {
                DWN_ERR("Error reading data (got %d)\n", ret);
                return -1;
            }

            memcpy(buffer, tmpbuf, mydata->sect_size);
            buffer += mydata->sect_size;
            size -= mydata->sect_size;
        }
    } else {
        idx = size / mydata->sect_size;
        ret = disk_read(startsect, idx, buffer);
        if (ret != idx) {
            DWN_ERR("Error reading data (got %d)\n", ret);
            return -1;
        }
        startsect += idx;
        idx *= mydata->sect_size;
        buffer += idx;
        size -= idx;
    }
    if (size) {
        ALLOC_CACHE_ALIGN_BUFFER(__u8, tmpbuf, mydata->sect_size);

        ret = disk_read(startsect, 1, tmpbuf);
        if (ret != 1) {
            DWN_ERR("Error reading data (got %d)\n", ret);
            return -1;
        }

        memcpy(buffer, tmpbuf, size);
    }

    return 0;
}

/*
 * Read at most 'maxsize' bytes from 'pos' in the file associated with 'dentptr'
 * into 'buffer'.
 * Update the number of bytes read in *gotsize or return -1 on fatal errors.
 */
extern __u8 get_contents_vfatname_block[MAX_CLUSTSIZE];//extern as large, 64k wo

static int _get_contents(int fd, loff_t pos,
        __u8 *buffer, loff_t maxsize, loff_t *gotsize)
{
    struct fopen_para* hFile = _hFile[fd];
    fsdata *mydata = &(hFile->fsdata);
    //dir_entry *dentptr = (_hFile[fd]->itr).dent;
    loff_t filesize = hFile->filesize;
    __u32 endclust, newclust;
    const unsigned int bytesperclust = hFile->bytesperclust;
    const unsigned int clusterMask = bytesperclust - 1;
    loff_t actsize = 0;//already seeked to pos in burning mode

    *gotsize = 0;
    debug("Filesize: %llu bytes\n", filesize);

    if (pos + maxsize > filesize) {
        DWN_MSG("Read position %llx + sz %llx past EOF: %llu\n", pos, maxsize, filesize);
        return 0;
    }
    if (do_fat_fseek(fd, pos, 0)) {
        DWN_ERR("Fail seek to pos %llu\n", pos);
        return -__LINE__;
    }
    __u32 curclust = hFile->curclust;
    filesize -= pos;

    /* align to beginning of next cluster if any */
    const int firstClusterNotAlign = pos & clusterMask;
    if (firstClusterNotAlign) {
        const int rightPart = bytesperclust - firstClusterNotAlign;
        actsize = min(rightPart, (int)maxsize);
        DWN_DBG("actsize 0x%x, firstClusterNotAlign 0x%x, curclust 0x%x\n",
                actsize, firstClusterNotAlign, curclust);
        if (get_cluster(mydata, curclust, get_contents_vfatname_block, bytesperclust)) {
            printf("Error reading cluster\n");
            return -1;
        }
        maxsize -= actsize;
        memcpy(buffer, get_contents_vfatname_block + firstClusterNotAlign, actsize);
        *gotsize = actsize;

        if (maxsize || rightPart == actsize) {
            curclust = get_fatent(mydata, curclust);
            if (CHECK_CLUST(curclust, mydata->fatsize)) {
                DWN_MSG("curclust: 0x%x\n", curclust);
                DWN_MSG("Invalid FAT entry\n");
                return -__LINE__;
            }
        }

        if (!maxsize) {
            if (rightPart == actsize) hFile->curclust = curclust;
            return 0;
        }
    }

    buffer += actsize;
    actsize = (int)bytesperclust;
    endclust = curclust;
    do {
        /* search for consecutive clusters */ while (actsize < maxsize) {
            newclust = get_fatent(mydata, endclust);
            if ((newclust - 1) != endclust)
                goto getit;
            if (CHECK_CLUST(newclust, mydata->fatsize)) {
                DWN_MSG("curclust: 0x%x\n", newclust);
                DWN_MSG("Invalid FAT entry\n");
                return -__LINE__;
            }
            endclust = newclust;
            actsize += bytesperclust;
        }

        /* get remaining bytes */
        actsize = maxsize;//<= bytesperclust
        if (get_cluster(mydata, curclust, buffer, (int)actsize) != 0) {
            printf("Error reading cluster\n");
            return -1;
        }
        *gotsize += actsize;
        if (!(actsize & clusterMask) && filesize > *gotsize) {
            endclust = get_fatent(mydata, endclust);
            if (CHECK_CLUST(endclust, mydata->fatsize)) {
                DWN_MSG("curclust: 0x%x\n", endclust);
                DWN_MSG("Invalid FAT entry\n");
                return -__LINE__;
            }
        }

        hFile->curclust = endclust;//for next reading
        return 0;//read ok
getit: //dispose consecutive clusters
        if (get_cluster(mydata, curclust, buffer, (int)actsize) != 0) {
            DWN_ERR("Error reading cluster %u\n", curclust);
            return -1;
        }
        *gotsize += (int)actsize;
        maxsize -= actsize;
        buffer += actsize;

        curclust = get_fatent(mydata, endclust);
        if (CHECK_CLUST(curclust, mydata->fatsize)) {
            DWN_MSG("curclust: 0x%x\n", curclust);
            printf("Invalid FAT entry\n");
            return -__LINE__;
        }
        actsize = bytesperclust;
        endclust = curclust;
    } while (1);

    return -__LINE__;//should not reach here
}

/*
 * Extract the file name information from 'slotptr' into 'l_name',
 * starting at l_name[*idx].
 * Return 1 if terminator (zero byte) is found, 0 otherwise.
 */
static int slot2str(dir_slot *slotptr, char *l_name, int *idx)
{
    int j;

    for (j = 0; j <= 8; j += 2) {
        l_name[*idx] = slotptr->name0_4[j];
        if (l_name[*idx] == 0x00)
            return 1;
        (*idx)++;
    }
    for (j = 0; j <= 10; j += 2) {
        l_name[*idx] = slotptr->name5_10[j];
        if (l_name[*idx] == 0x00)
            return 1;
        (*idx)++;
    }
    for (j = 0; j <= 2; j += 2) {
        l_name[*idx] = slotptr->name11_12[j];
        if (l_name[*idx] == 0x00)
            return 1;
        (*idx)++;
    }

    return 0;
}

/* Calculate short name checksum */
static __u8 mkcksum(const char name[8], const char ext[3])
{
    int i;

    __u8 ret = 0;

    for (i = 0; i < 8; i++)
        ret = (((ret & 1) << 7) | ((ret & 0xfe) >> 1)) + name[i];
    for (i = 0; i < 3; i++)
        ret = (((ret & 1) << 7) | ((ret & 0xfe) >> 1)) + ext[i];

    return ret;
}

/*
 * Read boot sector and volume info from a FAT filesystem
 */
static int
read_bootsectandvi(boot_sector *bs, volume_info *volinfo, int *fatsize)
{
    __u8 *block;
    volume_info *vistart;
    int ret = 0;

    if (cur_dev == NULL) {
        DWN_ERR("Error: no device selected\n");
        return -1;
    }

    block = malloc_cache_aligned(cur_dev->blksz);
    if (block == NULL) {
        DWN_ERR("Error: allocating block\n");
        return -1;
    }

    if (disk_read(0, 1, block) < 0) {
        DWN_ERR("Error: reading block\n");
        goto fail;
    }

    memcpy(bs, block, sizeof(boot_sector));
    bs->reserved = FAT2CPU16(bs->reserved);
    bs->fat_length = FAT2CPU16(bs->fat_length);
    bs->secs_track = FAT2CPU16(bs->secs_track);
    bs->heads = FAT2CPU16(bs->heads);
    bs->total_sect = FAT2CPU32(bs->total_sect);

    /* FAT32 entries */
    if (bs->fat_length == 0) {
        /* Assume FAT32 */
        bs->fat32_length = FAT2CPU32(bs->fat32_length);
        bs->flags = FAT2CPU16(bs->flags);
        bs->root_cluster = FAT2CPU32(bs->root_cluster);
        bs->info_sector = FAT2CPU16(bs->info_sector);
        bs->backup_boot = FAT2CPU16(bs->backup_boot);
        vistart = (volume_info *)(block + sizeof(boot_sector));
        *fatsize = 32;
    } else {
        vistart = (volume_info *)&(bs->fat32_length);
        *fatsize = 0;
    }
    memcpy(volinfo, vistart, sizeof(volume_info));

    if (*fatsize == 32) {
        if (strncmp(FAT32_SIGN, vistart->fs_type, SIGNLEN) == 0)
            goto exit;
    } else {
        if (strncmp(FAT12_SIGN, vistart->fs_type, SIGNLEN) == 0) {
            *fatsize = 12;
            goto exit;
        }
        if (strncmp(FAT16_SIGN, vistart->fs_type, SIGNLEN) == 0) {
            *fatsize = 16;
            goto exit;
        }
    }

    debug("Error: broken fs_type sign\n");
fail:
    ret = -1;
exit:
    free(block);
    return ret;
}

static int get_fs_info(fsdata *mydata)
{
    boot_sector bs;
    volume_info volinfo;
    int ret;

    ret = read_bootsectandvi(&bs, &volinfo, &mydata->fatsize);
    if (ret) {
        debug("Error: reading boot sector\n");
        return ret;
    }

    if (mydata->fatsize == 32) {
        mydata->fatlength = bs.fat32_length;
    } else {
        mydata->fatlength = bs.fat_length;
    }

    mydata->fat_sect = bs.reserved;

    mydata->rootdir_sect = mydata->fat_sect + mydata->fatlength * bs.fats;

    mydata->sect_size = (bs.sector_size[1] << 8) + bs.sector_size[0];
    mydata->clust_size = bs.cluster_size;
    if (mydata->sect_size != cur_part_info.blksz) {
        printf("Error: FAT sector size mismatch (fs=%hu, dev=%lu)\n",
                mydata->sect_size, cur_part_info.blksz);
        return -1;
    }

    if (mydata->fatsize == 32) {
        mydata->data_begin = mydata->rootdir_sect -
            (mydata->clust_size * 2);
        mydata->root_cluster = bs.root_cluster;
    } else {
        mydata->rootdir_size = ((bs.dir_entries[1]  * (int)256 +
                    bs.dir_entries[0]) *
                sizeof(dir_entry)) /
            mydata->sect_size;
        mydata->data_begin = mydata->rootdir_sect +
            mydata->rootdir_size -
            (mydata->clust_size * 2);
        mydata->root_cluster =
            sect_to_clust(mydata, mydata->rootdir_sect);
    }

    mydata->fatbufnum = -1;
    mydata->fat_dirty = 0;
    mydata->fatbuf = malloc_cache_aligned(FATBUFSIZE);
    if (mydata->fatbuf == NULL) {
        DWN_ERR("Error: allocating memory\n");
        return -1;
    }

    debug("FAT%d, fat_sect: %d, fatlength: %d\n",
            mydata->fatsize, mydata->fat_sect, mydata->fatlength);
    debug("Rootdir begins at cluster: %d, sector: %d, offset: %x\n"
            "Data begins at: %d\n",
            mydata->root_cluster,
            mydata->rootdir_sect,
            mydata->rootdir_sect * mydata->sect_size, mydata->data_begin);
    debug("Sector size: %d, cluster size: %d\n", mydata->sect_size,
            mydata->clust_size);

    return 0;
}


/**
 * fat_itr_root() - initialize an iterator to start at the root
 * directory
 *
 * @itr: iterator to initialize
 * @fsdata: filesystem data for the partition
 * @return 0 on success, else -errno
 */
static int fat_itr_root(fat_itr *itr, fsdata *fsdata)
{
    if (get_fs_info(fsdata))
        return -ENXIO;

    itr->fsdata = fsdata;
    itr->clust = fsdata->root_cluster;
    itr->dent = NULL;
    itr->remaining = 0;
    itr->last_cluster = 0;
    itr->is_root = 1;

    return 0;
}

/**
 * fat_itr_child() - initialize an iterator to descend into a sub-
 * directory
 *
 * Initializes 'itr' to iterate the contents of the directory at
 * the current cursor position of 'parent'.  It is an error to
 * call this if the current cursor of 'parent' is pointing at a
 * regular file.
 *
 * Note that 'itr' and 'parent' can be the same pointer if you do
 * not need to preserve 'parent' after this call, which is useful
 * for traversing directory structure to resolve a file/directory.
 *
 * @itr: iterator to initialize
 * @parent: the iterator pointing at a directory entry in the
 *    parent directory of the directory to iterate
 */
static void fat_itr_child(fat_itr *itr, fat_itr *parent)
{
    fsdata *mydata = parent->fsdata;  /* for silly macros */
    unsigned clustnum = START(parent->dent);

    assert(fat_itr_isdir(parent));

    itr->fsdata = parent->fsdata;
    if (clustnum > 0) {
        itr->clust = clustnum;
        itr->is_root = 0;
    } else {
        itr->clust = parent->fsdata->root_cluster;
        itr->is_root = 1;
    }
    itr->dent = NULL;
    itr->remaining = 0;
    itr->last_cluster = 0;
}

static void *next_cluster(fat_itr *itr)
{
    //fsdata *mydata = itr->fsdata;  /* for silly macros */
    int ret;
    u32 sect;

    /* have we reached the end? */
    if (itr->last_cluster)
        return NULL;

    sect = clust_to_sect(itr->fsdata, itr->clust);

    debug("FAT read(sect=%d), clust_size=%d, DIRENTSPERBLOCK=%zd\n",
            sect, itr->fsdata->clust_size, DIRENTSPERBLOCK);

    /*
     * NOTE: do_fat_read_at() had complicated logic to deal w/
     * vfat names that span multiple clusters in the fat16 case,
     * which get_dentfromdir() probably also needed (and was
     * missing).  And not entirely sure what fat32 didn't have
     * the same issue..  We solve that by only caring about one
     * dent at a time and iteratively constructing the vfat long
     * name.
     */
    ret = disk_read(sect, itr->fsdata->clust_size,
            itr->block);
    if (ret < 0) {
        debug("Error: reading block\n");
        return NULL;
    }

    if (itr->is_root && itr->fsdata->fatsize != 32) {
        itr->clust++;
        sect = clust_to_sect(itr->fsdata, itr->clust);
        if (sect - itr->fsdata->rootdir_sect >=
                itr->fsdata->rootdir_size) {
            debug("cursect: 0x%x\n", itr->clust);
            itr->last_cluster = 1;
        }
    } else {
        itr->clust = get_fatent(itr->fsdata, itr->clust);
        if (CHECK_CLUST(itr->clust, itr->fsdata->fatsize)) {
            debug("cursect: 0x%x\n", itr->clust);
            itr->last_cluster = 1;
        }
    }

    return itr->block;
}

static dir_entry *next_dent(fat_itr *itr)
{
    if (itr->remaining == 0) {
        struct dir_entry *dent = next_cluster(itr);
        unsigned nbytes = itr->fsdata->sect_size *
            itr->fsdata->clust_size;

        /* have we reached the last cluster? */
        if (!dent)
            return NULL;

        itr->remaining = nbytes / sizeof(dir_entry) - 1;
        itr->dent = dent;
    } else {
        itr->remaining--;
        itr->dent++;
    }

    /* have we reached the last valid entry? */
    if (itr->dent->name[0] == 0)
        return NULL;

    return itr->dent;
}

static dir_entry *extract_vfat_name(fat_itr *itr)
{
    struct dir_entry *dent = itr->dent;
    int seqn = itr->dent->name[0] & ~LAST_LONG_ENTRY_MASK;
    u8 chksum, alias_checksum = ((dir_slot *)dent)->alias_checksum;
    int n = 0;

    while (seqn--) {
        char buf[13];
        int idx = 0;

        slot2str((dir_slot *)dent, buf, &idx);

        /* shift accumulated long-name up and copy new part in: */
        memmove(itr->l_name + idx, itr->l_name, n);
        memcpy(itr->l_name, buf, idx);
        n += idx;

        dent = next_dent(itr);
        if (!dent)
            return NULL;
    }

    itr->l_name[n] = '\0';

    chksum = mkcksum(dent->name, dent->ext);

    /* checksum mismatch could mean deleted file, etc.. skip it: */
    if (chksum != alias_checksum) {
        debug("** chksum=%x, alias_checksum=%x, l_name=%s, s_name=%8s.%3s\n",
                chksum, alias_checksum, itr->l_name, dent->name, dent->ext);
        return NULL;
    }

    return dent;
}

/**
 * fat_itr_next() - step to the next entry in a directory
 *
 * Must be called once on a new iterator before the cursor is valid.
 *
 * @itr: the iterator to iterate
 * @return boolean, 1 if success or 0 if no more entries in the
 *    current directory
 */
static int fat_itr_next(fat_itr *itr)
{
    dir_entry *dent;

    itr->name = NULL;

    while (1) {
        dent = next_dent(itr);
        if (!dent)
            return 0;

        if (dent->name[0] == DELETED_FLAG ||
                dent->name[0] == aRING)
            continue;

        if (dent->attr & ATTR_VOLUME) {
            if ((dent->attr & ATTR_VFAT) == ATTR_VFAT &&
                    (dent->name[0] & LAST_LONG_ENTRY_MASK)) {
                dent = extract_vfat_name(itr);
                if (!dent)
                    continue;
                itr->name = itr->l_name;
                break;
            } else {
                /* Volume label or VFAT entry, skip */
                continue;
            }
        }

        break;
    }

    get_name(dent, itr->s_name);
    if (!itr->name)
        itr->name = itr->s_name;

    return 1;
}

/**
 * fat_itr_isdir() - is current cursor position pointing to a directory
 *
 * @itr: the iterator
 * @return true if cursor is at a directory
 */
static int fat_itr_isdir(fat_itr *itr)
{
    return !!(itr->dent->attr & ATTR_DIR);
}

/*
 * Helpers:
 */

#define TYPE_FILE 0x1
#define TYPE_DIR  0x2
#define TYPE_ANY  (TYPE_FILE | TYPE_DIR)

/**
 * fat_itr_resolve() - traverse directory structure to resolve the
 * requested path.
 *
 * Traverse directory structure to the requested path.  If the specified
 * path is to a directory, this will descend into the directory and
 * leave it iterator at the start of the directory.  If the path is to a
 * file, it will leave the iterator in the parent directory with current
 * cursor at file's entry in the directory.
 *
 * @itr: iterator initialized to root
 * @path: the requested path
 * @type: bitmask of allowable file types
 * @return 0 on success or -errno
 */
static int fat_itr_resolve(fat_itr *itr, const char *path, unsigned type)
{
    const char *next;

    /* chomp any extra leading slashes: */
    while (path[0] && ISDIRDELIM(path[0]))
        path++;

    /* are we at the end? */
    if (strlen(path) == 0) {
        if (!(type & TYPE_DIR))
            return -ENOENT;
        return 0;
    }

    /* find length of next path entry: */
    next = path;
    while (next[0] && !ISDIRDELIM(next[0]))
        next++;

    while (fat_itr_next(itr)) {
        int match = 0;
        unsigned n = max(strlen(itr->name), (size_t)(next - path));

        /* check both long and short name: */
        if (!strncasecmp(path, itr->name, n))
            match = 1;
        else if (itr->name != itr->s_name &&
                !strncasecmp(path, itr->s_name, n))
            match = 1;

        if (!match)
            continue;

        if (fat_itr_isdir(itr)) {
            /* recurse into directory: */
            fat_itr_child(itr, itr);
            return fat_itr_resolve(itr, next, type);
        } else if (next[0]) {
            /*
             * If next is not empty then we have a case
             * like: /path/to/realfile/nonsense
             */
            debug("bad trailing path: %s\n", next);
            return -ENOENT;
        } else if (!(type & TYPE_FILE)) {
            return -ENOTDIR;
        } else {
            return 0;
        }
    }

    return -ENOENT;
}

static int _fat_set_blk_dev(struct blk_desc *dev_desc, disk_partition_t *info)
{
    ALLOC_CACHE_ALIGN_BUFFER(unsigned char, buffer, dev_desc->blksz);

    cur_dev = dev_desc;
    cur_part_info = *info;

    /* Make sure it has a valid FAT header */
    if (disk_read(0, 1, buffer) != 1) {
        cur_dev = NULL;
        return -1;
    }

    /* Check if it's actually a DOS volume */
    if (memcmp(buffer + DOS_BOOT_MAGIC_OFFSET, "\x55\xAA", 2)) {
        cur_dev = NULL;
        return -1;
    }

    /* Check for FAT12/FAT16/FAT32 filesystem */
    if (!memcmp(buffer + DOS_FS_TYPE_OFFSET, "FAT", 3))
        return 0;
    if (!memcmp(buffer + DOS_FS32_TYPE_OFFSET, "FAT32", 5))
        return 0;

    cur_dev = NULL;
    return -1;
}

int optimus_fat_register_device(const char *ifname, const char *dev_part_str)
{
    int dev, part;
    struct blk_desc *dev_desc;
    disk_partition_t info;

    part = blk_get_device_part_str(ifname, dev_part_str, &dev_desc, &info, 1);
    if (part < 0) {
        DWN_ERR("fail in blk_get_device_part_str\n");
        return -__LINE__;
    }

    dev = dev_desc->devnum;
    if (_fat_set_blk_dev(dev_desc, &info) != 0) {
        printf("\n** Unable to use %s %d:%d for fatinfo **\n",
                ifname, dev, part);
        return 1;
    }

    return 0;
}

int optimus_sdc_burn_switch_to_extmmc(void)
{
    return 0;
}

unsigned do_fat_get_bytesperclust(int fd)
{
    struct fopen_para* hFile = _hFile[fd];;
    fsdata* mydata = &(hFile->fsdata);

    unsigned int bytesperclust = mydata->clust_size * mydata->sect_size;
    DWN_DBG("bytesperclust %x, clust_size %x, sec %x\n", bytesperclust,
            mydata->clust_size, mydata->sect_size);
    return bytesperclust;
}

long do_fat_fopen(const char *filename)
{
    fsdata* mydata;
    fat_itr *itr;
    int ret = -__LINE__;
    int index = -__LINE__;

    struct fopen_para* hFile = malloc_cache_aligned(sizeof(struct fopen_para));
    if (!hFile) {
        DWN_ERR("Fail alloc mem for open\n");
        return -__LINE__;
    }
    itr = &(hFile->itr);
    mydata = &(hFile->fsdata);
    ret = fat_itr_root(itr, mydata);
    if (ret) {
        DWN_ERR("Fail read fat root\n");
        goto _out_free;
    }

    ret = fat_itr_resolve(itr, filename, TYPE_FILE);
    if (ret) {
        DWN_ERR("Fail find file[%s]\n", filename);
        goto _out_free;
    }

    index = 0;
    hFile->fileOffset = 0;
    hFile->curclust = START(itr->dent);
    hFile->startclust = hFile->curclust;
    hFile->filesize = FAT2CPU32(itr->dent->size);
    //if (_hFile[index]) free(_hFile[index]); //close last opened, why crashed!!
    _hFile[index] = hFile;
    _hFile[index]->bytesperclust = do_fat_get_bytesperclust(index);
    DWN_MSG("filesize 0x%llx\n", hFile->filesize);
    DWN_MSG("startclust 0x%x\n", hFile->startclust);
    return index;

_out_free:
    free(hFile);
    return -__LINE__;
}

void do_fat_fclose(int fd)
{
    struct fopen_para* hFile = _hFile[fd];
    if (hFile) free(hFile);
    return;
}

/* wherehence: 0 to seek from start of file; 1 to seek from current position from file */
int do_fat_fseek(int fd, const int64_t offset, int wherehence)
{
    struct fopen_para* hFile = _hFile[fd];
    int64_t* filePos = &(hFile->fileOffset);
    const int64_t oldFileOffset = *filePos;
    int64_t pos = oldFileOffset;
    const unsigned clusterSz = do_fat_get_bytesperclust(fd);
    fsdata* mydata = &(hFile->fsdata);
    unsigned curclust = hFile->curclust;

    switch (wherehence)
    {
        case 0://seek from begin
            pos = offset;
            break;

        case 1://seek from current
            pos += offset;
            break;
        default: DWN_ERR("wherehence %d err\n", wherehence);
                 return -__LINE__;
    }
    if ( pos >= hFile->filesize ) {
        DWN_ERR("seek %lld err from wherehence %d\n", offset, wherehence);
        return -__LINE__;
    }
    const int64_t newFileOffset = pos; ////////
    DWN_DBG("Seek 0x%llx from 0x%llx\n", pos, oldFileOffset);
    if (newFileOffset == oldFileOffset) return 0;

    //Above already update field @.fileOffset
    //following update @.curClusterIndex
    unsigned newClusterIndex = newFileOffset / clusterSz;
    unsigned curClusterIndex = oldFileOffset / clusterSz;
    int      nCluster        = 0;
    if (newClusterIndex > curClusterIndex) {
        curclust = hFile->curclust;
        nCluster = newClusterIndex - curClusterIndex;
        DWN_DBG("pos 0x%llx, nCluster %u, cur %u\n", pos, nCluster, curClusterIndex);
        while (nCluster--) {
            curclust = get_fatent(mydata, curclust);
            if (CHECK_CLUST(curclust, mydata->fatsize)) {
                DWN_ERR("curclust: 0x%x\n", curclust);
                DWN_ERR("Invalid FAT entry\n");
                return -__LINE__;
            }
        }
    } else if (newClusterIndex < curClusterIndex) {
        curclust = hFile->startclust;
        nCluster = newClusterIndex;
        DWN_DBG("pos %llx, nCluster %u, start %u\n", pos, nCluster, curClusterIndex);
        while (nCluster--) {
            curclust = get_fatent(mydata, curclust);
            if (CHECK_CLUST(curclust, mydata->fatsize)) {
                DWN_ERR("curclust: 0x%x\n", curclust);
                DWN_ERR("Invalid FAT entry\n");
                return -__LINE__;
            }
        }
    }

    DWN_DBG("Seek to 0x%x from 0x%x cluster\n", curclust, hFile->curclust);
    hFile->curclust = curclust;
    hFile->fileOffset = newFileOffset;
    return 0;
}

long do_fat_fread(int fd, __u8 *buffer, unsigned long maxsize)
{
    loff_t actRead    = 0;
    int ret = -__LINE__;
    struct fopen_para* hFile = _hFile[fd];;
    loff_t pos = hFile->fileOffset;

    ret = _get_contents(fd, pos, buffer, maxsize, &actRead);
    if (ret) {
        DWN_ERR("fail read 0x%lx from pos 0x%llx.err(%d)\n", maxsize, pos, ret);
        return -__LINE__;
    }
    hFile->fileOffset += actRead;
    DWN_DBG("update fpos 0x%llx, curclust 0x%x\n", hFile->fileOffset, hFile->curclust);

    return actRead;
}

s64 do_fat_get_fileSz(const char* imgItemPath)
{
    char cmdBuf[256] = "";
    int rcode = 0;
    const char* envFileSz = NULL;
    const char* usb_update = getenv("usb_update");

    if (!strcmp(usb_update,"1"))
    {
        //fatexist usb host 0 imgItemPath
        sprintf(cmdBuf, "fatsize usb 0 %s", imgItemPath);
    }
    else
    {
        rcode = optimus_sdc_burn_switch_to_extmmc();
        sprintf(cmdBuf, "fatsize mmc 0 %s", imgItemPath);
    }
    /*SDC_DBG("to run cmd [%s]\n", cmdBuf);*/
    rcode = run_command(cmdBuf, 0);
    if (rcode) {
        printf("fail in cmd [%s], rcode %d\n", cmdBuf, rcode);
        return 0;//item size is 0
    }
    envFileSz = getenv("filesize");
    /*SDC_DBG("size of item %s is 0x%s\n", imgItemPath, envFileSz);*/

    return simple_strtoull(envFileSz, NULL, 16);
}

//<0 if failed, 0 is normal, 1 is sparse, others reserved
int do_fat_get_file_format(const char* imgFilePath, unsigned char* pbuf, const unsigned bufSz)
{
    int readSz = 0;

    int hFile = do_fat_fopen(imgFilePath);
    if (hFile < 0) {
        printf("Fail to open file (%s)\n", imgFilePath);
        return -1;
    }

    readSz = do_fat_fread(hFile, pbuf, bufSz);
    if (readSz <= 0) {
        printf("Fail to read file(%s), readSz=%d\n", imgFilePath, readSz);
        do_fat_fclose(hFile);
        return -1;
    }

    readSz = optimus_simg_probe(pbuf, readSz);

    do_fat_fclose(hFile);

    return readSz;
}

