/*
 * Copyright (c) 2019 HiSilicon Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#define REG_EMMC_DRV_DLL_CTRL		0x1fc    /*emmc&sd share emmc0 controller*/
#define REG_SDIO0_DRV_DLL_CTRL		0x1fc
#define REG_SDIO1_DRV_DLL_CTRL		0x220
#define REG_SDIO2_DRV_DLL_CTRL		/*no sdio2*/
#define SDIO_DRV_PHASE_SEL_MASK		(0x1f << 24)
#define SDIO_DRV_SEL(phase)		    ((phase) << 24)

#define REG_EMMC_DRV_DLL_STATUS		0x210
#define REG_SDIO0_DRV_DLL_STATUS	0x210
#define REG_SDIO1_DRV_DLL_STATUS	0x228
#define REG_SDIO2_DRV_DLL_STATUS	/*no sdio2*/
#define SDIO_DRV_DLL_LOCK		BIT(15)
#define SDIO_DRV_DLL_READY		BIT(14)

#define REG_EMMC_SAMPL_DLL_STATUS	0x208
#define REG_SDIO0_SAMPL_DLL_STATUS	0x208
#define REG_SDIO1_SAMPL_DLL_STATUS	0x224
#define REG_SDIO2_SAMPL_DLL_STATUS	/*no sdio2*/
#define SDIO_SAMPL_DLL_SLAVE_READY	BIT(0)

#define REG_EMMC_SAMPL_DLL_CTRL		0x1f4
#define REG_SDIO0_SAMPL_DLL_CTRL	0x1f4
#define REG_SDIO1_SAMPL_DLL_CTRL	0x22c
#define REG_SDIO2_SAMPL_DLL_CTRL	/*no sdio2*/
#define SDIO_SAMPL_DLL_SLAVE_EN		BIT(16)

#define REG_EMMC_SAMPLB_DLL_CTRL	0x1f8
#define REG_SDIO0_SAMPLB_DLL_CTRL	0x1f8
#define REG_SDIO1_SAMPLB_DLL_CTRL	0x21c
#define REG_SDIO2_SAMPLB_DLL_CTRL	/*no sdio2*/
#define SDIO_SAMPLB_DLL_CLK_MASK	(0x1f << 0)
#define SDIO_SAMPLB_SEL(phase)		((phase) << 0)

#define REG_EMMC_DS_DLL_CTRL		0x200
#define EMMC_DS_DLL_MODE_SSEL		BIT(13)
#define EMMC_DS_DLL_SSEL_MASK		(0x7f)

#define REG_EMMC_DS180_DLL_CTRL		0x204
#define EMMC_DS180_DLL_BYPASS		BIT(15)
#define REG_EMMC_DS180_DLL_STATUS	0x218
#define EMMC_DS180_DLL_READY		BIT(0)

#define REG_EMMC_DS_DLL_STATUS		0x214
#define EMMC_DS_DLL_READY		BIT(0)

#define REG_EMMC_CLK_CTRL		0x1f4
#define REG_SDIO0_CLK_CTRL		0x1f4
#define REG_SDIO1_CLK_CTRL		0x22c
#define REG_SDIO2_CLK_CTRL		/*no sdio2*/
#define SDIO_CLK_DRV_DLL_RST		BIT(29)
#define SDIO_CLK_CRG_RST		BIT(27)

#define IO_CFG_SR			BIT(10)
#define IO_CFG_PULL_DOWN	    	BIT(9)
#define IO_CFG_PULL_UP			BIT(8)
#define IO_CFG_DRV_STR_MASK		(0xf << 4)
#define IO_CFG_DRV_STR_SEL(str)		((str) << 4)
#define IO_CFG_PIN_MUX_MASK		(0xf << 0)
#define IO_CFG_PIN_MUX_SEL(type)	((type) << 0)
#define IO_CFG_PIN_MUX_TYPE_CLK_EMMC	0x0
#define IO_CFG_PIN_MUX_TYPE_CLK_SD	0x1

#define IO_CFG_EMMC_DATA_LINE_COUNT	8
#define REG_CTRL_EMMC_CLK		0x0014
#define REG_CTRL_EMMC_CMD		0x0018
#define REG_CTRL_EMMC_DATA0		0x001c
#define REG_CTRL_EMMC_DATA1		0x0028
#define REG_CTRL_EMMC_DATA2		0x0024
#define REG_CTRL_EMMC_DATA3		0x0020
#define REG_CTRL_EMMC_DATA4		0x0030
#define REG_CTRL_EMMC_DATA5		0x0034
#define REG_CTRL_EMMC_DATA6		0x0038
#define REG_CTRL_EMMC_DATA7		0x003c
#define REG_CTRL_EMMC_DS		0x0058
#define REG_CTRL_EMMC_RST		0x005c
static unsigned int io_emmc_data_reg[IO_CFG_EMMC_DATA_LINE_COUNT] = {REG_CTRL_EMMC_DATA0, REG_CTRL_EMMC_DATA1,
			REG_CTRL_EMMC_DATA2, REG_CTRL_EMMC_DATA3,
			REG_CTRL_EMMC_DATA4, REG_CTRL_EMMC_DATA5,
			REG_CTRL_EMMC_DATA6, REG_CTRL_EMMC_DATA7};

#define IO_CFG_SDIO0_DATA_LINE_COUNT	4
#define REG_CTRL_SDIO0_CLK		0x0040
#define REG_CTRL_SDIO0_CMD		0x0044
#define REG_CTRL_SDIO0_DATA0		0x0048
#define REG_CTRL_SDIO0_DATA1		0x004C
#define REG_CTRL_SDIO0_DATA2		0x0050
#define REG_CTRL_SDIO0_DATA3		0x0054
static unsigned int io_sdio0_data_reg[IO_CFG_SDIO0_DATA_LINE_COUNT] = {REG_CTRL_SDIO0_DATA0, REG_CTRL_SDIO0_DATA1,
			REG_CTRL_SDIO0_DATA2, REG_CTRL_SDIO0_DATA3};

#define IO_CFG_SDIO1_DATA_LINE_COUNT	4
#define REG_CTRL_SDIO1_CLK		0x0060
#define REG_CTRL_SDIO1_CMD		0x0064
#define REG_CTRL_SDIO1_DATA0		0x0068
#define REG_CTRL_SDIO1_DATA1		0x006C
#define REG_CTRL_SDIO1_DATA2		0x0070
#define REG_CTRL_SDIO1_DATA3		0x0074
static unsigned int io_sdio1_data_reg[IO_CFG_SDIO1_DATA_LINE_COUNT] = {REG_CTRL_SDIO1_DATA0, REG_CTRL_SDIO1_DATA1,
			REG_CTRL_SDIO1_DATA2, REG_CTRL_SDIO1_DATA3};

struct sdhci_hisi_priv {
	struct reset_control *crg_rst;
	struct reset_control *dll_rst;
	struct reset_control *sampl_rst; /* Not used */
	struct regmap *crg_regmap;
	struct regmap *iocfg_regmap;
	unsigned int f_max;
	unsigned int devid;
	unsigned int drv_phase;
	unsigned int sampl_phase;
	unsigned int tuning_phase;
};

static void sdhci_hisi_hs400_enhanced_strobe(struct mmc_host *mmc,
		struct mmc_ios *ios)
{
	u32 ctrl;
	struct sdhci_host *host = mmc_priv(mmc);

	ctrl = sdhci_readl(host, SDHCI_EMMC_CTRL);
	if (ios->enhanced_strobe)
		ctrl |= SDHCI_ENH_STROBE_EN;
	else
		ctrl &= ~SDHCI_ENH_STROBE_EN;

	sdhci_writel(host, ctrl, SDHCI_EMMC_CTRL);

	ctrl = sdhci_readl(host, SDHCI_MULTI_CYCLE);
	if (ios->enhanced_strobe)
		ctrl |= SDHCI_CMD_DLY_EN;
	else
		ctrl &= ~SDHCI_CMD_DLY_EN;

	sdhci_writel(host, ctrl, SDHCI_MULTI_CYCLE);
}

static int sdhci_hisi_pltfm_init(struct platform_device *pdev,
		struct sdhci_host *host)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	struct sdhci_hisi_priv *hisi_priv = sdhci_pltfm_priv(pltfm_host);
	struct device_node *np = pdev->dev.of_node;
	struct clk *clk;
	int ret;

	hisi_priv->crg_rst = devm_reset_control_get(&pdev->dev, "crg_reset");
	if (IS_ERR_OR_NULL(hisi_priv->crg_rst)) {
		dev_err(&pdev->dev, "get crg_rst failed.\n");
		return PTR_ERR(hisi_priv->crg_rst);;
	}

	hisi_priv->dll_rst = devm_reset_control_get(&pdev->dev, "dll_reset");
	if (IS_ERR_OR_NULL(hisi_priv->dll_rst)) {
		dev_err(&pdev->dev, "get dll_rst failed.\n");
		return PTR_ERR(hisi_priv->dll_rst);;
	}

	hisi_priv->sampl_rst = NULL;

	hisi_priv->crg_regmap = syscon_regmap_lookup_by_phandle(np, "crg_regmap");
	if (IS_ERR(hisi_priv->crg_regmap))
	{
		dev_err(&pdev->dev, "get crg regmap failed.\n");
		return PTR_ERR(hisi_priv->crg_regmap);
	}

	hisi_priv->iocfg_regmap = syscon_regmap_lookup_by_phandle(np, "iocfg_regmap");
	if (IS_ERR(hisi_priv->iocfg_regmap))
	{
		dev_err(&pdev->dev, "get iocfg regmap failed.\n");
		return PTR_ERR(hisi_priv->iocfg_regmap);
	}

	if (of_property_read_u32(np, "devid", &hisi_priv->devid))
		return -EINVAL;

	clk = devm_clk_get(mmc_dev(host->mmc), "mmc_clk");
	if (IS_ERR_OR_NULL(clk)) {
		dev_err(mmc_dev(host->mmc), "get clk err\n");
		return -EINVAL;
	}

	pltfm_host->clk = clk;

	hisi_mmc_crg_init(host);
	ret = sdhci_hisi_parse_dt(host);
	if (ret)
		return ret;

	/* only eMMC has a hw reset, and now eMMC signaling
	 * is fixed to 180*/
	if (host->mmc->caps & MMC_CAP_HW_RESET) {
		host->flags &= ~SDHCI_SIGNALING_330;
		host->flags |= SDHCI_SIGNALING_180;
	}

	/* we parse the support timings from dts, so we read the
	 * host capabilities early and clear the timing capabilities,
	 * SDHCI_QUIRK_MISSING_CAPS is set so that sdhci driver would
	 * not read it again */
	host->caps = sdhci_readl(host, SDHCI_CAPABILITIES);
	host->caps &= ~(SDHCI_CAN_DO_HISPD);
	host->caps1 = sdhci_readl(host, SDHCI_CAPABILITIES_1);
	host->caps1 &= ~(SDHCI_SUPPORT_SDR50 | SDHCI_SUPPORT_SDR104 |
				SDHCI_SUPPORT_DDR50 | SDHCI_CAN_DO_ADMA3);
	host->quirks |= SDHCI_QUIRK_MISSING_CAPS |
			SDHCI_QUIRK_NO_ENDATTR_IN_NOPDESC |
			SDHCI_QUIRK_SINGLE_POWER_WRITE;

	host->mmc_host_ops.hs400_enhanced_strobe = sdhci_hisi_hs400_enhanced_strobe;

	mci_host[slot_index++] = host->mmc;

	return 0;
}

static void hisi_wait_ds_dll_lock(struct sdhci_host *host)
{
	//Do nothing
	return;
}

static void hisi_wait_ds_180_dll_ready(struct sdhci_host *host)
{
	struct sdhci_hisi_priv *hisi_priv = sdhci_get_pltfm_priv(host);
	unsigned int reg, timeout = 20;

	do {
		reg = 0;
		regmap_read(hisi_priv->crg_regmap, REG_EMMC_DS180_DLL_STATUS, &reg);
		if (reg & EMMC_DS180_DLL_READY)
			return;

		mdelay(1);
		timeout--;
	} while (timeout > 0);

	pr_err("%s: DS 180 DLL master not ready.\n", mmc_hostname(host->mmc));
}

static void hisi_set_ds_dll_delay(struct sdhci_host *host)
{
	//Do nothing
	return;
}

static void hisi_host_extra_init(struct sdhci_host *host)
{
	unsigned short ctrl;
	unsigned int mbiiu_ctrl, val;

	ctrl = sdhci_readw(host, SDHCI_MSHC_CTRL);
	ctrl &= ~SDHCI_CMD_CONFLIT_CHECK;
	sdhci_writew(host, ctrl, SDHCI_MSHC_CTRL);

	mbiiu_ctrl = sdhci_readl(host, SDHCI_AXI_MBIIU_CTRL);
	mbiiu_ctrl &= ~(SDHCI_GM_WR_OSRC_LMT_MASK | SDHCI_GM_RD_OSRC_LMT_MASK |
			SDHCI_UNDEFL_INCR_EN);
	mbiiu_ctrl |= (SDHCI_GM_WR_OSRC_LMT_SEL(7)
			| SDHCI_GM_RD_OSRC_LMT_SEL(7));
	sdhci_writel(host, mbiiu_ctrl, SDHCI_AXI_MBIIU_CTRL);

	val = sdhci_readl(host, SDHCI_MULTI_CYCLE);
	val &= ~SDHCI_CMD_DLY_EN;
	val |= SDHCI_EDGE_DETECT_EN | SDHCI_DATA_DLY_EN;

	sdhci_writel(host, val, SDHCI_MULTI_CYCLE);
	host->error_count = 0;
}

static void hisi_set_drv_str(struct regmap *iocfg_regmap, unsigned int offset,
	unsigned int pull_up, unsigned int pull_down, unsigned int sr, unsigned int drv_str)
{
	unsigned int reg = 0;
	regmap_read(iocfg_regmap, offset, &reg);

	reg &= ~(IO_CFG_PULL_UP | IO_CFG_PULL_DOWN | IO_CFG_DRV_STR_MASK | IO_CFG_SR);
	reg |= (pull_up ? IO_CFG_PULL_UP: 0);
	reg |= (pull_down ? IO_CFG_PULL_DOWN: 0);
	reg |= (sr ? IO_CFG_SR: 0);
	reg |= IO_CFG_DRV_STR_SEL(drv_str);

	regmap_write(iocfg_regmap, offset, reg);
}

static void hisi_set_emmc_ctrl(struct sdhci_host *host)
{
	unsigned int reg;

	reg = sdhci_readl(host, SDHCI_EMMC_CTRL);
	reg |= SDHCI_CARD_IS_EMMC;
	sdhci_writel(host, reg, SDHCI_EMMC_CTRL);
}


static void hisi_set_mmc_drv(struct sdhci_host *host)
{
	struct sdhci_hisi_priv *hisi_priv = sdhci_get_pltfm_priv(host);
	void* iocfg_regmap = hisi_priv->iocfg_regmap;
	int i;

	switch (host->timing) {
	case MMC_TIMING_MMC_HS400:
		hisi_set_emmc_ctrl(host);
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_EMMC_CLK, 0, 1, 0, 0x3);
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_EMMC_CMD, 1, 0, 0, 0x4);
		for (i = 0; i < IO_CFG_EMMC_DATA_LINE_COUNT; i++)
		{
			hisi_set_drv_str(iocfg_regmap, io_emmc_data_reg[i], 1, 0, 0, 0x4);
		}
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_EMMC_DS, 0, 1, 1, 0x3);
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_EMMC_RST, 1, 0, 1, 0x3);
		break;
	case MMC_TIMING_MMC_HS200:
		hisi_set_emmc_ctrl(host);
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_EMMC_CLK, 0, 1, 0, 0x2);
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_EMMC_CMD, 1, 0, 1, 0x4);
		for (i = 0; i < IO_CFG_EMMC_DATA_LINE_COUNT; i++)
		{
			hisi_set_drv_str(iocfg_regmap, io_emmc_data_reg[i], 1, 0, 1, 0x4);
		}
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_EMMC_RST, 1, 0, 1, 0x3);
		break;
	case MMC_TIMING_MMC_HS:
		hisi_set_emmc_ctrl(host);
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_EMMC_CLK, 0, 1, 1, 0x4);
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_EMMC_CMD, 1, 0, 1, 0x6);
		for (i = 0; i < IO_CFG_EMMC_DATA_LINE_COUNT; i++)
		{
			hisi_set_drv_str(iocfg_regmap, io_emmc_data_reg[i], 1, 0, 1, 0x6);
		}
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_EMMC_RST, 1, 0, 1, 0x3);
		break;
	case MMC_TIMING_LEGACY:
	default:
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_EMMC_CLK, 0, 1, 1, 0x5);
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_EMMC_CMD, 1, 0, 1, 0x6);
		for (i = 0; i < IO_CFG_EMMC_DATA_LINE_COUNT; i++)
		{
			hisi_set_drv_str(iocfg_regmap, io_emmc_data_reg[i], 1, 0, 1, 0x6);
		}
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_EMMC_RST, 1, 0, 1, 0x3);
		break;
	}
}

static void hisi_set_sd_drv(struct sdhci_host *host)
{
	struct sdhci_hisi_priv *hisi_priv = sdhci_get_pltfm_priv(host);
	void* iocfg_regmap = hisi_priv->iocfg_regmap;
	int i;

	switch (host->timing) {
	case MMC_TIMING_SD_HS:
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_SDIO0_CLK, 0, 1, 1, 0x5);
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_SDIO0_CMD, 1, 0, 1, 0x7);
		for (i = 0; i < IO_CFG_SDIO0_DATA_LINE_COUNT; i++)
		{
			hisi_set_drv_str(iocfg_regmap, io_sdio0_data_reg[i], 1, 0, 1, 0x7);
		}
		break;
	case MMC_TIMING_LEGACY:
	default:
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_SDIO0_CLK, 0, 1, 1, 0x7);
		hisi_set_drv_str(iocfg_regmap, REG_CTRL_SDIO0_CMD, 1, 0, 1, 0x7);
		for (i = 0; i < IO_CFG_SDIO0_DATA_LINE_COUNT; i++)
		{
			hisi_set_drv_str(iocfg_regmap, io_sdio0_data_reg[i], 1, 0, 1, 0x7);
		}
		break;
	}
}

static void hisi_set_sdio_drv(struct sdhci_host *host)
{
	struct sdhci_hisi_priv *hisi_priv = sdhci_get_pltfm_priv(host);
	void* iocfg_regmap = hisi_priv->iocfg_regmap;
	int i;

	hisi_set_drv_str(iocfg_regmap, REG_CTRL_SDIO1_CLK, 0, 1, 1, 0x5);
	hisi_set_drv_str(iocfg_regmap, REG_CTRL_SDIO1_CMD, 1, 0, 0, 0x7);
	for (i = 0; i < IO_CFG_SDIO1_DATA_LINE_COUNT; i++)
	{
		hisi_set_drv_str(iocfg_regmap, io_sdio1_data_reg[i], 1, 0, 0, 0x7);
	}
}

static void hisi_set_io_config(struct sdhci_host *host)
{
	struct sdhci_hisi_priv *hisi_priv = sdhci_get_pltfm_priv(host);
	unsigned int devid = hisi_priv->devid;
	void* iocfg_regmap = hisi_priv->iocfg_regmap;
	unsigned int reg = 0;

	if (devid == 0) {
		/* For mmc0: eMMC and SD card */
		regmap_read(iocfg_regmap, REG_CTRL_EMMC_CLK, &reg);
		if ((reg & IO_CFG_PIN_MUX_MASK) == IO_CFG_PIN_MUX_SEL(IO_CFG_PIN_MUX_TYPE_CLK_EMMC))
		{
			hisi_set_mmc_drv(host);
		}

		regmap_read(iocfg_regmap, REG_CTRL_SDIO0_CLK, &reg);
		if ((reg & IO_CFG_PIN_MUX_MASK) == IO_CFG_PIN_MUX_SEL(IO_CFG_PIN_MUX_TYPE_CLK_SD))
		{
			hisi_set_sd_drv(host);
		}
	}
	else {
		/* For mmc1: sdio wifi */
		hisi_set_sdio_drv(host);
	}
}

static void hisi_get_phase(struct sdhci_host *host)
{
	struct sdhci_hisi_priv *hisi_priv = sdhci_get_pltfm_priv(host);
	unsigned int devid = hisi_priv->devid;

	if (devid == 0) {
		/* For eMMC and SD card */
		if (host->mmc->ios.timing == MMC_TIMING_MMC_HS400) {
			hisi_priv->drv_phase = 10;	/* 112.5 degree */
			hisi_priv->sampl_phase = hisi_priv->tuning_phase;
		} else if (host->mmc->ios.timing == MMC_TIMING_MMC_HS200) {
			hisi_priv->drv_phase = 23;	/* 258.75 degree */
			hisi_priv->sampl_phase = hisi_priv->tuning_phase;
		} else if (host->mmc->ios.timing == MMC_TIMING_MMC_HS) {
			hisi_priv->drv_phase = 16;	/* 180 degree */
			hisi_priv->sampl_phase = 4;
		} else if (host->mmc->ios.timing == MMC_TIMING_SD_HS) {
			hisi_priv->drv_phase = 20;	/* 225 degree */
			hisi_priv->sampl_phase = 4;
		} else if (host->mmc->ios.timing == MMC_TIMING_LEGACY) {
			hisi_priv->drv_phase = 16;	/* 180 degree */
			hisi_priv->sampl_phase = 0;
		} else {
			hisi_priv->drv_phase = 20;	/* 225 degree */
			hisi_priv->sampl_phase = 4;
		}
	} else {
		/* For SDIO device */
		if ((host->mmc->ios.timing == MMC_TIMING_SD_HS) ||
			(host->mmc->ios.timing == MMC_TIMING_UHS_SDR25)) {
			hisi_priv->drv_phase = 16;	/* 180 degree */
			hisi_priv->sampl_phase = 4;
		} else {
			/* UHS_SDR12 */
			hisi_priv->drv_phase = 16;	/* 180 degree */
			hisi_priv->sampl_phase = 0;
		}
	}
}

static int hisi_support_runtime_pm(struct sdhci_host *host)
{
	struct sdhci_hisi_priv *hisi_priv = sdhci_get_pltfm_priv(host);
	unsigned int devid = hisi_priv->devid;

	/* Only enable for mmc0 eMMC and SD card */
	if (devid == 0)
		return 1;
	else
		return 0;
}
