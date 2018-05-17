/*
 * File      : fal_partition.c
 * This file is part of FAL (Flash Abstraction Layer) package
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#include <fal.h>
#include <string.h>
#include <stdlib.h>

/* partition magic word */
#define FAL_PART_MAGIC_WROD         0x45503130

/**
 * FAL partition table config has defined on 'fal_cfg.h'.
 * When this option is disable, it will auto find the partition table on a specified location in flash partition.
 */
#ifdef FAL_PART_HAS_TABLE_CFG

/* check partition table definition */
#if !defined(FAL_PART_TABLE)
#error "You must defined FAL_PART_TABLE on 'fal_cfg.h'"
#endif

#ifdef __CC_ARM                        /* ARM Compiler */
    #define SECTION(x)                 __attribute__((section(x)))
    #define USED                       __attribute__((used))
#elif defined (__IAR_SYSTEMS_ICC__)    /* for IAR Compiler */
    #define SECTION(x)                 @ x
    #define USED                       __root
#elif defined (__GNUC__)               /* GNU GCC Compiler */
    #define SECTION(x)                 __attribute__((section(x)))
    #define USED                       __attribute__((used))
#else
    #error not supported tool chain
#endif /* __CC_ARM */
USED static const struct fal_partition partition_table_def[] SECTION("FalPart") = FAL_PART_TABLE;
static const struct fal_partition *partition_table = NULL;

#else /* FAL_PART_HAS_TABLE_CFG */

#if !defined(FAL_PART_TABLE_FLASH_DEV_NAME)
#error "You must defined FAL_PART_TABLE_FLASH_DEV_NAME on 'fal_cfg.h'"
#endif

/* check partition table end offset address definition */
#if !defined(FAL_PART_TABLE_END_OFFSET)
#error "You must defined FAL_PART_TABLE_END_OFFSET on 'fal_cfg.h'"
#endif

static struct fal_partition *partition_table = NULL;
#endif /* FAL_PART_HAS_TABLE_CFG */

static uint8_t init_ok = 0;
static size_t partition_table_len = 0;

/**
 * Initialize all flash partition on FAL partition table
 *
 * @return partitions total number
 */
int fal_partition_init(void)
{
    size_t i;

    if (init_ok)
    {
        return partition_table_len;
    }

#ifdef FAL_PART_HAS_TABLE_CFG
    partition_table = &partition_table_def[0];
    partition_table_len = sizeof(partition_table_def) / sizeof(partition_table_def[0]);
#else
    /* load partition table from the end address FAL_PART_TABLE_END_OFFSET, error return 0 */
    size_t table_num = 0, table_item_size = 0;
    fal_partition_t new_part = NULL;
    const struct fal_flash_dev *flash_dev = NULL;

    flash_dev = fal_flash_device_find(FAL_PART_TABLE_FLASH_DEV_NAME);
    if (flash_dev == NULL)
    {
        log_e("Initialize failed! Don't found flash device(%s).", FAL_PART_TABLE_FLASH_DEV_NAME);
        goto _exit;
    }

    table_item_size = sizeof(struct fal_partition);
    new_part = (fal_partition_t)FAL_MALLOC(table_item_size);
    if (new_part == NULL)
    {
        log_e("Initialize failed! No memory for table buffer.");
        goto _exit;
    }

    do
    {
        memset(new_part, 0x00, table_num);
        if (flash_dev->ops.read(FAL_PART_TABLE_END_OFFSET - table_item_size * (table_num + 1), (uint8_t *) new_part,
                table_item_size) < 0)
        {
            log_e("Initialize failed! Bootloader flash device read error!");
            table_num = 0;
            break;
        }

        if (new_part->magic_word != FAL_PART_MAGIC_WROD)
        {
            break;
        }

        partition_table = (fal_partition_t) FAL_REALLOC(partition_table, table_item_size * (table_num + 1));
        if (partition_table == NULL)
        {
            log_e("Initialize failed! No memory for partition table");
            table_num = 0;
            break;
        }

        memcpy(partition_table + table_num * table_item_size, new_part, table_item_size);

        table_num++;
    } while (1);

    if (table_num == 0)
    {
        goto _exit;
    }
    else
    {
        partition_table_len = table_num;
    }
#endif /* FAL_PART_HAS_TABLE_CFG */

    /* check the partition table device exists */

    for (i = 0; i < partition_table_len; i++)
    {
        flash_dev = fal_flash_device_find(partition_table[i].flash_name);
        if (flash_dev == NULL)
        {
            log_e("Initialize failed! Don't found the flash device(%s).", partition_table[i].flash_name);
            partition_table_len = 0;
            goto _exit;
        }

        if (partition_table[i].offset >= flash_dev->len)
        {
            log_e("Initialize failed! Partition(%s) offset address(%ld) out of flash bound(%ld).",
                    partition_table[i].name, partition_table[i].offset, flash_dev->len);
            partition_table_len = 0;
            goto _exit;
        }
    }

    init_ok = 1;

_exit:

#ifndef FAL_PART_HAS_TABLE_CFG
    if (new_part)
    {
        FAL_FREE(new_part);
    }
#endif /* !FAL_PART_HAS_TABLE_CFG */

    return partition_table_len;
}

const struct fal_partition *fal_partition_find(const char *name)
{
    assert(init_ok);

    size_t i;

    for (i = 0; i < partition_table_len; i++)
    {
        if (!strcmp(name, partition_table[i].name))
        {
            return &partition_table[i];
        }
    }

    return NULL;
}

const struct fal_partition *fal_get_partition_table(size_t *len)
{
    assert(init_ok);
    assert(len);

    *len = partition_table_len;

    return partition_table;
}

/**
 * read data from partition
 *
 * @param part parttion
 * @param addr relative address for partition
 * @param buf read buffer
 * @param size read size
 *
 * @return -1: error
 */
int fal_partition_read(const struct fal_partition *part, uint32_t addr, uint8_t *buf, size_t size)
{
    int ret = 0;
    const struct fal_flash_dev *flash_dev = NULL;

    assert(part);
    assert(buf);

    if (addr + size > part->len)
    {
        log_e("Partition read error! Partition address out of bound.");
        return -1;
    }

    flash_dev = fal_flash_device_find(part->flash_name);
    if (flash_dev == NULL)
    {
        log_e("Partition read error! Don't found flash device(%s) of the partition(%s).", part->flash_name, part->name);
        return -1;
    }

    ret = flash_dev->ops.read(part->offset + addr, buf, size);
    if (ret < 0)
    {
        log_e("Partition read error! Flash device(%s) read error!", part->flash_name);
    }

    return ret;
}

/**
 * write data to partition
 *
 * @param part parttion
 * @param addr relative address for partition
 * @param buf write buffer
 * @param size write size
 *
 * @return -1: error
 */
int fal_partition_write(const struct fal_partition *part, uint32_t addr, const uint8_t *buf, size_t size)
{
    int ret = 0;
    const struct fal_flash_dev *flash_dev = NULL;

    assert(part);
    assert(buf);

    if (addr + size > part->len)
    {
        log_e("Partition write error! Partition address out of bound.");
        return -1;
    }

    flash_dev = fal_flash_device_find(part->flash_name);
    if (flash_dev == NULL)
    {
        log_e("Partition write error!  Don't found flash device(%s) of the partition(%s).", part->flash_name, part->name);
        return -1;
    }

    ret = flash_dev->ops.write(part->offset + addr, buf, size);
    if (ret < 0)
    {
        log_e("Partition write error! Flash device(%s) write error!", part->flash_name);
    }

    return ret;
}

/**
 * erase partition data
 *
 * @param part parttion
 * @param addr relative address for partition
 * @param size erase size
 *
 * @return -1: error
 */
int fal_partition_erase(const struct fal_partition *part, uint32_t addr, size_t size)
{
    int ret = 0;
    const struct fal_flash_dev *flash_dev = NULL;

    assert(part);

    if (addr + size > part->len)
    {
        log_e("Partition erase error! Partition address out of bound.");
        return -1;
    }

    flash_dev = fal_flash_device_find(part->flash_name);
    if (flash_dev == NULL)
    {
        log_e("Partition erase error! Don't found flash device(%s) of the partition(%s).", part->flash_name, part->name);
        return -1;
    }

    ret = flash_dev->ops.erase(part->offset + addr, size);
    if (ret < 0)
    {
        log_e("Partition erase error! Flash device(%s) erase error!", part->flash_name);
    }

    return ret;
}

/**
 * erase partition all data
 *
 * @param part parttion
 *
 * @return -1: error
 */
int fal_partition_erase_all(const struct fal_partition *part)
{
    return fal_partition_erase(part, 0, part->len);
}
