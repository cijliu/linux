/*
 * Copyright (c) 2017 HiSilicon Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

extern void hisi_usb_phy_on(struct phy *phy);
extern void hisi_usb_phy_off(struct phy *phy);
extern void hisi_usb3_phy_on(struct phy *phy);
extern void hisi_usb3_phy_off(struct phy *phy);

#if defined(CONFIG_ARCH_HI3559AV100)
extern int hisi_usb3_init_para(struct phy *phy, struct device_node *np);
extern void multi_cfg_f_pcie(struct phy *phy);
#endif

struct hisi_priv {
    void __iomem *sys_ctrl;
    void __iomem *peri_ctrl;
    void __iomem *combphy_base;
    void __iomem *misc_ctrl;
    unsigned int phyid;
    void __iomem *ctrl_base;
    void __iomem *switch_base;
};
