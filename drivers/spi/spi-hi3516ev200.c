/*
 * Copyright (c) 2006 Ben Dooks
 * Copyright 2006-2009 Simtec Electronics
 *	cijliu <cijliu@qq.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/slab.h>

#include <linux/spi/spi.h>
#include <linux/spi/spi_bitbang.h>
#include <linux/module.h>

#include <mach/io.h>/* for IO_ADDRESS */
#define hisilicon_debug(fmt, ...)

static struct hi3516ev200_spi hisilicon_spi;
#if 0
#define SSP_BASE    hisilicon_spi.regs
#define SSP_SIZE    0x1000          // 4KB
		

#define DEFAULT_MD_LEN (128)



#define IO_ADDRESS_VERIFY(x) (x)




/* SSP register definition .*/
#define SSP_CR0              IO_ADDRESS_VERIFY(SSP_BASE + 0x00)
#define SSP_CR1              IO_ADDRESS_VERIFY(SSP_BASE + 0x04)
#define SSP_DR               IO_ADDRESS_VERIFY(SSP_BASE + 0x08)
#define SSP_SR               IO_ADDRESS_VERIFY(SSP_BASE + 0x0C)
#define SSP_CPSR             IO_ADDRESS_VERIFY(SSP_BASE + 0x10)
#define SSP_IMSC             IO_ADDRESS_VERIFY(SSP_BASE + 0x14)
#define SSP_RIS              IO_ADDRESS_VERIFY(SSP_BASE + 0x18)
#define SSP_MIS              IO_ADDRESS_VERIFY(SSP_BASE + 0x1C)
#define SSP_ICR              IO_ADDRESS_VERIFY(SSP_BASE + 0x20)
#define SSP_DMACR            IO_ADDRESS_VERIFY(SSP_BASE + 0x24)

#define SPI_SR_BSY        (0x1 << 4)/* spi busy flag */
#define SPI_SR_TFE        (0x1 << 0)/* Whether to send fifo is empty */
#define SPI_DATA_WIDTH    (8)
#define SPI_SPO           (0)
#define SPI_SPH           (0)
#define SPI_SCR           (0)
#define SPI_CPSDVSR       (2)
#define SPI_FRAMEMODE     (0)

#define MAX_WAIT 1000

#define  ssp_readw(addr,ret)            (ret =(readl(addr)))
#define  ssp_writew(addr,value)            (writel(value, addr))
#else

#define SSP_BASE    0x12070000
#define SSP_SIZE    0x1000          // 4KB


#define DEFAULT_MD_LEN (128)

void __iomem *reg_ssp_base_va;
#define IO_ADDRESS_VERIFY(x) (reg_ssp_base_va + ((x)-(SSP_BASE)))

/* SSP register definition .*/
#define SSP_CR0              IO_ADDRESS_VERIFY(SSP_BASE + 0x00)
#define SSP_CR1              IO_ADDRESS_VERIFY(SSP_BASE + 0x04)
#define SSP_DR               IO_ADDRESS_VERIFY(SSP_BASE + 0x08)
#define SSP_SR               IO_ADDRESS_VERIFY(SSP_BASE + 0x0C)
#define SSP_CPSR             IO_ADDRESS_VERIFY(SSP_BASE + 0x10)
#define SSP_IMSC             IO_ADDRESS_VERIFY(SSP_BASE + 0x14)
#define SSP_RIS              IO_ADDRESS_VERIFY(SSP_BASE + 0x18)
#define SSP_MIS              IO_ADDRESS_VERIFY(SSP_BASE + 0x1C)
#define SSP_ICR              IO_ADDRESS_VERIFY(SSP_BASE + 0x20)
#define SSP_DMACR            IO_ADDRESS_VERIFY(SSP_BASE + 0x24)

#define SPI_SR_BSY        (0x1 << 4)/* spi busy flag */
#define SPI_SR_TFE        (0x1 << 0)/* Whether to send fifo is empty */
#define SPI_DATA_WIDTH    (8)
#define SPI_SPO           (0)
#define SPI_SPH           (0)
#define SPI_SCR           (0)
#define SPI_CPSDVSR       (2)
#define SPI_FRAMEMODE     (0)

#define MAX_WAIT 10

#define  ssp_readw(addr,ret)            (ret =(*(volatile unsigned int *)(addr)))
#define  ssp_writew(addr,value)            ((*(volatile unsigned int *)(addr)) = (value))

#endif
#include <asm/gpio.h>
struct hi3516ev200_spi {
	void __iomem *regs;
	struct clk *clk;
	int irq;
	const u8 *tx_buf;
	u8 *rx_buf;
	int tx_len;
	int rx_len;
	bool dma_pending;
};
static int ssp_set_reg(unsigned int Addr, unsigned int Value)
{
#ifdef __HuaweiLite__
    (*(volatile unsigned int *)(Addr)) = Value;
#else
    void* pmem = ioremap_nocache(Addr, DEFAULT_MD_LEN);
    if (pmem == NULL)
    {
		hisilicon_debug("mem fail\n");
        return -1;
    }

    *(unsigned int*)pmem = Value;
    iounmap(pmem);
#endif
    return 0;
}

static void hi_ssp_enable(void)
{
    unsigned int ret = 0;
    ssp_readw(SSP_CR1,ret);
    ret = (ret & 0xFFFD) | 0x2;

    ret = ret | (0x1 << 4); /* big/little end, 1: little, 0: big */

    ret = ret | (0x1 << 15); /* wait en */

    ssp_writew(SSP_CR1,ret);

    //hi_ssp_writeOnly(0);
}


static void hi_ssp_disable(void)
{
    unsigned int ret = 0;
    ssp_readw(SSP_CR1,ret);
    ret = ret & (~(0x1 << 1));
    ssp_writew(SSP_CR1,ret);
}

static int hi_ssp_set_frameform(unsigned char framemode,unsigned char spo,unsigned char sph,unsigned char datawidth)
{
    unsigned int ret = 0;
    ssp_readw(SSP_CR0,ret);
    if(framemode > 3)
    {
        hisilicon_debug("set frame parameter err.\n");
        return -1;
    }
    ret = (ret & 0xFFCF) | (framemode << 4);
    if((ret & 0x30) == 0)
    {
        if(spo > 1)
        {
            hisilicon_debug("set spo parameter err.\n");
            return -1;
        }
        if(sph > 1)
        {
            hisilicon_debug("set sph parameter err.\n");
            return -1;
        }
        ret = (ret & 0xFF3F) | (sph << 7) | (spo << 6);
    }
    if((datawidth > 16) || (datawidth < 4))
    {
        hisilicon_debug("set datawidth parameter err.\n");
        return -1;
    }
    ret = (ret & 0xFFF0) | (datawidth -1);
    ssp_writew(SSP_CR0,ret);
    return 0;
}


static int hi_ssp_set_serialclock(unsigned char scr,unsigned char cpsdvsr)
{
    unsigned int ret = 0;
    ssp_readw(SSP_CR0,ret);
    ret = (ret & 0xFF) | (scr << 8);
    ssp_writew(SSP_CR0,ret);
    if((cpsdvsr & 0x1))
    {
        hisilicon_debug("set cpsdvsr parameter err.\n");
        return -1;
    }
    ssp_writew(SSP_CPSR,cpsdvsr);
    return 0;
}
static void ssp_write(unsigned char dat)
{
    ssp_writew(SSP_DR,dat);
}
static void spi_enable(void)
{
    ssp_writew(SSP_CR1, 0x02);
}

static void spi_disable(void)
{
    ssp_writew(SSP_CR1, 0x00);
}

static void ssp_set(void)
{
	reg_ssp_base_va = ioremap_nocache((unsigned long)SSP_BASE, (unsigned long)SSP_SIZE);
    spi_disable();
    hi_ssp_set_frameform(SPI_FRAMEMODE, SPI_SPO, SPI_SPH, SPI_DATA_WIDTH);
    hi_ssp_set_serialclock(SPI_SCR, SPI_CPSDVSR);
    //hi_ssp_alt_mode_set(1);
    hi_ssp_enable();
	spi_enable();
	
}

static int hi3516ev200_spi_setup(struct spi_device *spi)
{
	hisilicon_debug("%s\n",__func__);
	ssp_set();
	return 0;
}
static void hi3516ev200_spi_set_cs(struct spi_device *spi, bool gpio_level)
{
	hisilicon_debug("%s\n",__func__);

}
static int hi3516ev200_spi_transfer_one(struct spi_master *master,
				    struct spi_device *spi,
				    struct spi_transfer *tfr)
{
	struct hi3516ev200_spi *hs = spi_master_get_devdata(master);
	int i;
	hs->tx_buf = tfr->tx_buf;
	//hs->rx_buf = tfr->rx_buf;
	//hs->tx_len = tfr->len;
	//hs->rx_len = tfr->len;
	hisilicon_debug("%s txlen:%d\n",__func__, hs->tx_len);
	hisilicon_debug("buf:");
	for(i=0;i < tfr->len; i++){
		//hisilicon_debug(" 0x%02x", hs->tx_buf[i]);
		//ssp_write(hs->tx_buf[i]);
		ssp_writew(SSP_DR,hs->tx_buf[i]);
	}
	hisilicon_debug("\n");
	return 0;
}
static void hi3516ev200_spi_handle_err(struct spi_master *master,
				   struct spi_message *msg)
{
	hisilicon_debug("%s\n",__func__);
}
static int hi3516ev200_spi_prepare_message(struct spi_master *master,
				       struct spi_message *msg)
{
	hisilicon_debug("%s\n",__func__);
	return 0;
}
static irqreturn_t hi3516ev200_spi_interrupt(int irq, void *dev_id)
{
	hisilicon_debug("%s\n",__func__);
	return IRQ_HANDLED;
}

static int hi3516ev200_spi_probe(struct platform_device *pdev)
{
	
	struct spi_master *master;
	struct hi3516ev200_spi *hs;
	struct resource *res;
	int err;
	hisilicon_debug("%s\n",__func__);
	master = spi_alloc_master(&pdev->dev, sizeof(*hs));
	if (!master) {
		dev_err(&pdev->dev, "spi_alloc_master() failed\n");
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, master);

	master->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH | SPI_LOOP;
	master->bits_per_word_mask = SPI_BPW_MASK(8);
	master->num_chipselect = 1;
	master->setup = hi3516ev200_spi_setup;
	//master->set_cs = hi3516ev200_spi_set_cs;
	master->transfer_one = hi3516ev200_spi_transfer_one;
	master->handle_err = hi3516ev200_spi_handle_err;
	//master->prepare_message = hi3516ev200_spi_prepare_message;
	master->dev.of_node = pdev->dev.of_node;

	hs = spi_master_get_devdata(master);
	hisilicon_spi.regs = hs->regs;
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	hs->regs = devm_ioremap_resource(&pdev->dev, res);
	hisilicon_spi.regs = hs->regs;
	if (IS_ERR(hs->regs)) {
		err = PTR_ERR(hs->regs);
		goto out_master_put;
	}

	hs->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(hs->clk)) {
		err = PTR_ERR(hs->clk);
		dev_err(&pdev->dev, "could not get clk: %d\n", err);
		goto out_master_put;
	}

	hs->irq = platform_get_irq(pdev, 0);
	if (hs->irq <= 0) {
		dev_err(&pdev->dev, "could not get IRQ: %d\n", hs->irq);
		err = hs->irq ? hs->irq : -ENODEV;
		goto out_master_put;
	}

	clk_prepare_enable(hs->clk);

	err = devm_request_irq(&pdev->dev, hs->irq, hi3516ev200_spi_interrupt, 0,
			       dev_name(&pdev->dev), master);
	if (err) {
		dev_err(&pdev->dev, "could not request IRQ: %d\n", err);
		goto out_clk_disable;
	}

	err = devm_spi_register_master(&pdev->dev, master);
	if (err) {
		dev_err(&pdev->dev, "could not register SPI master: %d\n", err);
		goto out_clk_disable;
	}

	return 0;

out_clk_disable:
	clk_disable_unprepare(hs->clk);
out_master_put:
	spi_master_put(master);
	return err;
}

static int hi3516ev200_spi_remove(struct platform_device *pdev)
{
	
	struct spi_master *master = platform_get_drvdata(pdev);
	struct hi3516ev200_spi *hs = spi_master_get_devdata(master);
	hisilicon_debug("%s\n",__func__);
	clk_disable_unprepare(hs->clk);

	return 0;
}

static const struct of_device_id hi3516ev200_spi_match[] = {
	{ .compatible = "hi3516ev200-spi", },
	{}
};

MODULE_DEVICE_TABLE(of, hi3516ev200_spi_match);

static struct platform_driver  hi3516ev200_spi_driver = {
	.probe		= hi3516ev200_spi_probe,
	.remove		= hi3516ev200_spi_remove,
	.driver		= {
		.name	= "hi3516ev200-spi",
		.of_match_table	= of_match_ptr(hi3516ev200_spi_match),
		//.owner  = THIS_MODULE,
	},
};
#if 0

static int __init hi3516ev200_spi_init(void)
{
	hisilicon_debug("%s\n",__func__);
	return spi_register_driver(&hi3516ev200_spi_driver);
}
static void __exit hi3516ev200_spi_exit(void)
{
	hisilicon_debug("%s\n",__func__);
	spi_unregister_driver(&hi3516ev200_spi_driver);
}
module_init(hi3516ev200_spi_init);
module_exit(hi3516ev200_spi_exit);

#endif
module_platform_driver(hi3516ev200_spi_driver);

//module_spi_driver(hi3516ev200_spi_driver);



MODULE_DESCRIPTION("hi3516ev200 SPI Driver");
MODULE_AUTHOR("cijliu, <cijliu@qq.com>");
MODULE_LICENSE("GPL");
