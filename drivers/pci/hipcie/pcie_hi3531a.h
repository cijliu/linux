/*
 * Copyright (c) 2015 HiSilicon Technologies Co., Ltd.
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

#ifndef __HISI_PCIE_H__
#define __HISI_PCIE_H__

#define MISC_CTRL_BASE      0x12120000
#define PERI_CRG_BASE       0x12040000
#define PCIE_SYS_STAT       IO_ADDRESS(0x1205008C)

#define PCIE0_PARA_REG      IO_ADDRESS(0x12120134)
#define PCIE1_PARA_REG      IO_ADDRESS(0x12120138)

#define PCIE0_MEM_BASE      0x28000000
#define PCIE0_EP_CONF_BASE  0x20000000
#define PCIE0_DBI_BASE      0x11020000
#define PCIE_DBI_BASE       PCIE0_MEM_BASE

#define PCIE1_MEM_BASE      0x38000000
#define PCIE1_EP_CONF_BASE  0x30000000
#define PCIE1_DBI_BASE      0x11030000

#define PERI_CRG73      0x124

#define PCIE0_X2_SRST_REQ   6
#define PCIE0_X2_AUX_CKEN   3
#define PCIE0_X2_PIPE_CKEN  2
#define PCIE0_X2_SYS_CKEN   1
#define PCIE0_X2_BUS_CKEN   0

#define PCIE1_X2_SRST_REQ   14
#define PCIE1_X2_AUX_CKEN   11
#define PCIE1_X2_PIPE_CKEN  10
#define PCIE1_X2_SYS_CKEN   9
#define PCIE1_X2_BUS_CKEN   8

#define PCIE0_SYS_CTRL0     0xA0
#define PCIE1_SYS_CTRL0     0xE4
#define PCIE_DEVICE_TYPE    28
#define PCIE_WM_EP      0x0
#define PCIE_WM_LEGACY      0x1
#define PCIE_WM_RC      0x4

#define PCIE0_SYS_CTRL7     0xBC
#define PCIE1_SYS_CTRL7     0x100
#define PCIE_APP_LTSSM_ENBALE   11

#define PCIE0_SYS_STATE0    0xD0
#define PCIE1_SYS_STATE0    0x114
#define PCIE_XMLH_LINK_UP   15
#define PCIE_RDLH_LINK_UP   5

#define PCIE0_IRQ_INTA      94
#define PCIE0_IRQ_INTB      95
#define PCIE0_IRQ_INTC      96
#define PCIE0_IRQ_INTD      97
#define PCIE0_IRQ_EDMA      98
#define PCIE0_IRQ_MSI       99
#define PCIE0_IRQ_LINK_DOWN 100

#define PCIE1_IRQ_INTA      101
#define PCIE1_IRQ_INTB      102
#define PCIE1_IRQ_INTC      103
#define PCIE1_IRQ_INTD      104
#define PCIE1_IRQ_EDMA      105
#define PCIE1_IRQ_MSI       106
#define PCIE1_IRQ_LINK_DOWN 107

#define PCIE_INTA_PIN       1
#define PCIE_INTB_PIN       2
#define PCIE_INTC_PIN       3
#define PCIE_INTD_PIN       4

#define LINK_CTRL2_STATUS2  0xA0

#define MISC_CTRL77            0x0134
#define MISC_CTRL78            0x0138

#define REG_CRG72              0x0120
#define REG_CRG73              0x0124

#endif
