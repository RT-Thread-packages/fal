/*
 * File      : fal_flash_sfud_port.c
 * COPYRIGHT (C) 2012-2018, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-26     armink       the first version
 */

#include <fal.h>

#include <sfud.h>

extern sfud_flash sfud_norflash0;

static int read(long offset, uint8_t *buf, size_t size)
{
    sfud_read(&sfud_norflash0, nor_flash0.addr + offset, size, buf);

    return size;
}

static int write(long offset, const uint8_t *buf, size_t size)
{
    if (sfud_write(&sfud_norflash0, nor_flash0.addr + offset, size, buf) != SFUD_SUCCESS)
    {
        return -1;
    }

    return size;
}

static int erase(long offset, size_t size)
{
    if (sfud_erase(&sfud_norflash0, nor_flash0.addr + offset, size) != SFUD_SUCCESS)
    {
        return -1;
    }

    return size;
}
const struct fal_flash_dev nor_flash0 = { "norflash0", 0, 8*1024*1024, 4096, {NULL, read, write, erase} };
