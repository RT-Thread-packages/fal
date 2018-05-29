/*
 * File      : fal.h
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

#ifndef _FAL_H_
#define _FAL_H_

#include <rtconfig.h>
#include <fal_cfg.h>
#include "fal_def.h"

/**
 * FAL (Flash Abstraction Layer) initialization.
 * It will initialize all flash device and all flash partition.
 *
 * @return result
 *         >= 0: partitions total number
 */
int fal_init(void);

/* flash device operator API */
const struct fal_flash_dev *fal_flash_device_find(const char *name);

/* partition operator API */
const struct fal_partition *fal_partition_find(const char *name);
const struct fal_partition *fal_get_partition_table(size_t *len);
void fal_set_partition_table_temp(struct fal_partition *table, size_t len);
int fal_partition_read(const struct fal_partition *part, uint32_t addr, uint8_t *buf, size_t size);
int fal_partition_write(const struct fal_partition *part, uint32_t addr, const uint8_t *buf, size_t size);
int fal_partition_erase(const struct fal_partition *part, uint32_t addr, size_t size);
int fal_partition_erase_all(const struct fal_partition *part);
void fal_show_part_table(void);

#endif /* _FAL_H_ */
