/*
 * The Flash Memory Controller v100 Device Driver for hisilicon
 *
 * Copyright (c) 2016 HiSilicon Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
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

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/of_platform.h>
#include <linux/mfd/hisi_fmc.h>

#include <asm/setup.h>

#include "../../mtdcore.h"
#include "hifmc100.h"

/*****************************************************************************/
static int hifmc100_spi_nand_pre_probe(struct nand_chip *chip)
{
    uint8_t nand_maf_id;
    struct hifmc_host *host = chip->priv;

    /* Reset the chip first */
    host->send_cmd_reset(host);
    udelay(1000);

    /* Check the ID */
    host->offset = 0;
    memset((unsigned char *)(chip->IO_ADDR_R), 0, 0x10);
    host->send_cmd_readid(host);
    nand_maf_id = hifmc_readb(chip->IO_ADDR_R);

    if (nand_maf_id == 0x00 || nand_maf_id == 0xff) {
        printk("Cannot found a valid SPI Nand Device\n");
        return 1;
    }

    return 0;
}
/*****************************************************************************/
static int hifmc_nand_scan(struct mtd_info *mtd)
{
    int result = 0;
    unsigned char cs, chip_num = CONFIG_SPI_NAND_MAX_CHIP_NUM;
    struct nand_chip *chip = mtd_to_nand(mtd);
    struct hifmc_host *host = chip->priv;

    for (cs = 0; chip_num && (cs < HIFMC_MAX_CHIP_NUM); cs++) {
        if (hifmc_cs_user[cs]) {
            FMC_PR(BT_DBG, "\t\t*-Current CS(%d) is occupied.\n",
                   cs);
            continue;
        }

        host->cmd_op.cs = cs;

        if (hifmc100_spi_nand_pre_probe(chip)) {
            return -ENODEV;
        }

        FMC_PR(BT_DBG, "\t\t*-Scan SPI nand flash on CS: %d\n", cs);
        if (nand_scan(mtd, chip_num)) {
            continue;
        }
        chip_num--;
    }

    if (chip_num == CONFIG_SPI_NAND_MAX_CHIP_NUM) {
        result = -ENXIO;
    } else {
        result = 0;
    }

    return result;
}

/*****************************************************************************/
static int hisi_spi_nand_probe(struct platform_device *pltdev)
{
    int len, result = 0;
    struct hifmc_host *host;
    struct nand_chip *chip;
    struct mtd_info *mtd;
    struct device *dev = &pltdev->dev;
    struct device_node *np = NULL;
    struct hisi_fmc *fmc = dev_get_drvdata(dev->parent);

    FMC_PR(BT_DBG, "\t*-Start SPI Nand flash driver probe\n");

    if (!fmc) {
        dev_err(dev, "get mfd fmc devices failed\n");
        return -ENXIO;
    }

    len = sizeof(struct hifmc_host) + sizeof(struct nand_chip)
          + sizeof(struct mtd_info);
    host = devm_kzalloc(dev, len, GFP_KERNEL);
    if (!host) {
        return -ENOMEM;
    }
    memset((char *)host, 0, len);

    platform_set_drvdata(pltdev, host);
    host->dev = &pltdev->dev;

    host->chip = chip = (struct nand_chip *)&host[1];
    host->mtd  = mtd  = nand_to_mtd(chip);

    host->regbase = fmc->regbase;
    host->iobase = fmc->iobase;
    host->clk = fmc->clk;
    host->lock = &fmc->lock;
    host->buffer = fmc->buffer;
    host->dma_buffer = fmc->dma_buffer;

    memset((char *)host->iobase, 0xff, fmc->dma_len);
    chip->IO_ADDR_R = chip->IO_ADDR_W = host->iobase;

    chip->priv = host;
    result = hifmc100_spi_nand_init(chip);
    if (result) {
        FMC_PR(BT_DBG, "\t|-SPI Nand init failed, ret: %d\n", result);
        result = -ENODEV;
        goto fail;
    }

    np = of_get_next_available_child(dev->of_node, NULL);
    mtd->name = np->name;
    mtd->type = MTD_NANDFLASH;
    mtd->priv = chip;
    mtd->owner = THIS_MODULE;

    result = of_property_read_u32(np, "spi-max-frequency", &host->clkrate);
    if (result) {
        goto fail;
    }

    result = hifmc_nand_scan(mtd);
    if (result) {
        FMC_PR(BT_DBG, "\t|-Scan SPI Nand failed.\n");
        goto fail;
    }

    result = mtd_device_register(mtd, NULL, 0);
    if (!result) {
        FMC_PR(BT_DBG, "\t*-End driver probe !!\n");
        return 0;
    }

    result = -ENODEV;
fail:
    clk_disable_unprepare(host->clk);
    nand_release(mtd);

    DB_MSG("Error: driver probe, result: %d\n", result);
    return result;
}

/*****************************************************************************/
static int hisi_spi_nand_remove(struct platform_device *pltdev)
{
    struct hifmc_host *host = platform_get_drvdata(pltdev);

    clk_disable_unprepare(host->clk);
    nand_release(host->mtd);

    return 0;
}

#ifdef CONFIG_PM
/*****************************************************************************/
static int hifmc100_os_suspend(struct platform_device *pltdev,
                               pm_message_t state)
{
    struct hifmc_host *host = platform_get_drvdata(pltdev);

    if (host && host->suspend) {
        return (host->suspend)(pltdev, state);
    }

    return 0;
}

/*****************************************************************************/
static int hifmc100_os_resume(struct platform_device *pltdev)
{
    struct hifmc_host *host = platform_get_drvdata(pltdev);

    if (host && host->resume) {
        return (host->resume)(pltdev);
    }

    return 0;
}
#endif /* End of CONFIG_PM */
/*****************************************************************************/
static const struct of_device_id hisi_spi_nand_dt_ids[] = {
    { .compatible = "hisilicon,hisi-spi-nand"},
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, hisi_spi_nand_dt_ids);

static struct platform_driver hisi_spi_nand_driver = {
    .driver = {
        .name   = "hisi_spi_nand",
        .of_match_table = hisi_spi_nand_dt_ids,
    },
    .probe  = hisi_spi_nand_probe,
    .remove = hisi_spi_nand_remove,
#ifdef CONFIG_PM
    .suspend    = hifmc100_os_suspend,
    .resume     = hifmc100_os_resume,
#endif
};
module_platform_driver(hisi_spi_nand_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BVT_BSP");
MODULE_DESCRIPTION("Hisilicon Flash Memory Controller V100 SPI Nand Driver");
