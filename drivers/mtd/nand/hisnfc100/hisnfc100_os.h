/*
 * Copyright (c) 2016 HiSilicon Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef HISNFC100_OSH
#define HISNFC100_OSH

/*****************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <asm/errno.h>
#include <asm/setup.h>
#include <linux/io.h>
#include <linux/dma-mapping.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/mtd/nand.h>

#include <mach/platform.h>

#include <linux/clk.h>
#include <linux/clkdev.h>

#if (KERNEL_VERSION(3, 4, 5) <= LINUX_VERSION_CODE)
#include "../../mtdcore.h"
#endif

#define HISNFC100_BUFFER_BASE_ADDRESS_LEN       (2048 + 256)

/*****************************************************************************/
#ifndef CONFIG_HISNFC100_MAX_CHIP
#  define CONFIG_HISNFC100_MAX_CHIP                    (1)
#  warning NOT config CONFIG_HISNFC100_MAX_CHIP, \
used default value, maybe invalid.
#endif /* CONFIG_HISNFC100_MAX_CHIP */

/*****************************************************************************/
#define PR_BUG(fmt, args...) do { \
    pr_info("%s(%d): bug " fmt, __FILE__, __LINE__, ##args); \
    asm("b ."); \
} while (0)

#if 1
#  define DBG_MSG(_fmt, arg...)
#else
#  define DBG_MSG(_fmt, arg...) \
    printk(KERN_INFO "%s(%d): " _fmt, __FILE__, __LINE__, ##arg);
#endif

#endif /* HISNFC100_OSH */

