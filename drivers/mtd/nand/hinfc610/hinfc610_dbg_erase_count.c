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

#include <linux/moduleparam.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#include "hinfc610_os.h"
#include "hinfc610.h"
#include "hinfc610_dbg.h"

struct hinfc610_dbg_erase_count_t {

    unsigned int index;  /* display pos */
    unsigned int offset; /* display offset */
    unsigned int length; /* display length */

    struct dentry *dentry;

    unsigned int blocknum;
    unsigned int page_per_block;

    unsigned int pe[1];
};

static DEFINE_MUTEX(dbg_erase_count_mutex);
static struct hinfc610_dbg_erase_count_t *dbg_erase_count;

/*****************************************************************************/

static int dbgfs_erase_count_read(struct file *filp, char __user *buffer,
                                  size_t count, loff_t *ppos)
{
    int len = 0;
    int value = 0;
    unsigned int *pe;
    unsigned int index;
    char buf[128] = {0};
    char __user *pusrbuf = buffer;

    if (*ppos == 0) {
        dbg_erase_count->index = dbg_erase_count->offset;

        len = snprintf(buf, sizeof(buf),
                       "Print parameter: \"offset=%d length=%d\"\n",
                       dbg_erase_count->offset,
                       dbg_erase_count->length);

        if (copy_to_user(pusrbuf, buf, len)) {
            return -EFAULT;
        }
        pusrbuf += len;

        len = snprintf(buf, sizeof(buf),
                       "Block Index  ---------------- "
                       "Erase count from system startup ----------------\n");

        if (copy_to_user(pusrbuf, buf, len)) {
            return -EFAULT;
        }
        pusrbuf += len;
    }

    for (index = dbg_erase_count->index;
            index < (dbg_erase_count->offset + dbg_erase_count->length) &&
            ((pusrbuf - buffer) < (count - 100));
            index += 8) {

        pe = &dbg_erase_count->pe[index];

        len = snprintf(buf, sizeof(buf),
                       "%4d: %8u %8u %8u %8u  %8u %8u %8u %8u\n",
                       index,
                       pe[0], pe[1], pe[2], pe[3],
                       pe[4], pe[5], pe[6], pe[7]);

        if (copy_to_user(pusrbuf, buf, len)) {
            return -EFAULT;
        }
        pusrbuf += len;
    }

    dbg_erase_count->index = index;

    *ppos += (pusrbuf - buffer);
    value = pusrbuf - buffer;
    return value;
}
/*****************************************************************************/
/*
 * echo "offset=48,length=78" > /sys/kernel/debug/nand/erase_count
 * echo "clear" > /sys/kernel/debug/nand/erase_count
 *

 # cat ./debugfs/nand/erase_count
 Print parameter: "offset=0 length=1024"
 Block Index  ---------------- Erase count from system startup ----------------
  0:        0        0        0        0         0        0        0        0
  8:        0        0        0        0         0        0        0        0
 16:        0        0        0        0         0        0        0        0
 24:        0        0        0        0         0        0        0        0
 32:        0        0        0        0         0        0        0        0
 40:        0        0        0        0         0        0        0        0
 48:        0        0        0        0         0        0        0        0
 56:        0        0        0        0         0        0        0        0
 64:        0        0        0        0         0        0        0        0
 72:        0        0        0        0         0        0        0        0
 80:        0        0        0        0         0        0        0        0

 */
static int dbgfs_erase_count_write(struct file *filp,
                                   const char __user *buffer,
                                   size_t count, loff_t *ppos)
{
    char *str;
    char buf[128] = {0};
    int ret;
    unsigned long value = 0;
    unsigned long pos = 0;

    if (count > sizeof(buf)) {
        count = sizeof(buf);
    }

    if (copy_from_user(buf, buffer, count)) {
        return -EFAULT;
    }

    while (pos < count) {
        while (pos < count &&
                (buf[pos] == ' ' || buf[pos] == ',' || buf[pos] == ';')) {
            pos++;
        }

        if (pos >= count) {
            break;
        }

        switch (buf[pos]) {

            case 'o':

                if (memcmp(&buf[pos], CMD_WORD_OFFSET,
                           sizeof(CMD_WORD_OFFSET) - 1)) {
                    break;
                }

                pos += sizeof(CMD_WORD_OFFSET) - 1;
                str = (char *)(buf + pos);
                ret = kstrtoul(str, 10, &value);
                if (ret < 0) {
                    value = 0;
                }
                if (value >= dbg_erase_count->blocknum) {
                    value = 0;
                }

                dbg_erase_count->offset = (value & ~7);

                break;

            case 'l':
                if (memcmp(&buf[pos], CMD_WORD_LENGTH,
                           sizeof(CMD_WORD_LENGTH) - 1)) {
                    break;
                }

                pos += sizeof(CMD_WORD_LENGTH) - 1;
                str = (char *)(buf + pos);
                ret = kstrtoul(str, 10, &value);
                if (ret < 0) {
                    value = dbg_erase_count->blocknum;
                }

                value = ((value + 7) & ~7);

                if (dbg_erase_count->offset + value
                        > dbg_erase_count->blocknum)
                    value = dbg_erase_count->blocknum
                            - dbg_erase_count->offset;

                dbg_erase_count->length = value;

                break;

            case 'c':
                if (memcmp(&buf[pos], CMD_WORD_CLEAN,
                           sizeof(CMD_WORD_CLEAN) - 1)) {
                    break;
                }

                memset(dbg_erase_count->pe, 0,
                       dbg_erase_count->blocknum *
                       sizeof(struct hinfc610_dbg_erase_count_t));

                return count;
        }

        while (pos < count &&
                (buf[pos] != ' ' && buf[pos] != ',' && buf[pos] != ';')) {
            pos++;
        }
    }

    return count;
}
/*****************************************************************************/

static const struct file_operations dbgfs_erase_count_fops = {
    .owner = THIS_MODULE,
    .read  = dbgfs_erase_count_read,
    .write = dbgfs_erase_count_write,
};
/*****************************************************************************/

static int dbgfs_erase_count_init(struct dentry *root, struct hinfc_host *host)
{
    unsigned int size;
    unsigned int blocknum;
    unsigned int pagesize;
    unsigned int blocksize;
    unsigned int chipsize;
    struct hinfc610_dbg_erase_count_t *erase_count;

    if (dbg_erase_count) {
        return 0;
    }

    pagesize  = (host->pagesize >> 10);
    blocksize = (host->mtd->erasesize >> 10);
    chipsize = (unsigned int)(host->chip->chipsize >> 10);

    blocknum = chipsize / blocksize;
    size = sizeof(int) * blocknum
           + sizeof(struct hinfc610_dbg_erase_count_t);

    erase_count = vmalloc(size);
    if (!erase_count) {
        PR_ERR("Can't allocate memory.\n");
        return -ENOMEM;
    }
    memset(erase_count, 0, size);

    erase_count->blocknum = blocknum;
    erase_count->page_per_block = blocksize / pagesize;
    erase_count->length = blocknum;

    erase_count->dentry = debugfs_create_file("erase_count",
                          S_IFREG | S_IRUSR | S_IWUSR,
                          root, NULL, &dbgfs_erase_count_fops);
    if (!erase_count->dentry) {
        PR_ERR("Can't create 'erase_count' file.\n");
        vfree(erase_count);
        return -ENOENT;
    }

    dbg_erase_count = erase_count;

    return 0;
}
/*****************************************************************************/

static int dbgfs_erase_count_uninit(void)
{
    if (!dbg_erase_count) {
        return 0;
    }

    mutex_lock(&dbg_erase_count_mutex);

    debugfs_remove(dbg_erase_count->dentry);

    vfree(dbg_erase_count);
    dbg_erase_count = NULL;

    mutex_unlock(&dbg_erase_count_mutex);

    return 0;
}
/*****************************************************************************/

static void dbg_erase_count_erase(struct hinfc_host *host)
{
    unsigned int block_index;

    mutex_lock(&dbg_erase_count_mutex);

    if (!dbg_erase_count) {
        goto exit;
    }

    block_index = (host->addr_value[0] / dbg_erase_count->page_per_block);

    if (block_index > dbg_erase_count->blocknum) {
        PR_ERR("Block out of range.\n");
        return;
    }

    dbg_erase_count->pe[block_index]++;

exit:
    mutex_unlock(&dbg_erase_count_mutex);
}
/*****************************************************************************/

struct hinfc610_dbg_inf_t hinfc610_dbg_inf_erase_count = {
    "erase_count", 0,
    dbgfs_erase_count_init,
    dbgfs_erase_count_uninit,
    NULL,
    NULL,
    dbg_erase_count_erase,
    NULL,
};
