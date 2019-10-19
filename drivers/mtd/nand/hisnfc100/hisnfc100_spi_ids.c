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

#include "hisnfc100_os.h"
#include "hisnfc100_spi_ids.h"
#include "hisnfc100.h"

#include "hisnfc100_spi_general.c"

/*****************************************************************************/
#define SET_READ_STD(_dummy_, _size_, _clk_) \
    static struct spi_op_info read_std_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_STD, SPI_CMD_READ_STD, _dummy_, _size_, _clk_ }

#define READ_STD(_dummy_, _size_, _clk_) read_std_##_dummy_##_size_##_clk_

/*****************************************************************************/
#define SET_READ_FAST(_dummy_, _size_, _clk_) \
    static struct spi_op_info read_fast_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_FAST, SPI_CMD_READ_FAST, _dummy_, _size_, _clk_ }

#define READ_FAST(_dummy_, _size_, _clk_) read_fast_##_dummy_##_size_##_clk_

/*****************************************************************************/
#define SET_READ_DUAL(_dummy_, _size_, _clk_) \
    static struct spi_op_info read_dual_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_DUAL, SPI_CMD_READ_DUAL, _dummy_, _size_, _clk_ }

#define READ_DUAL(_dummy_, _size_, _clk_) read_dual_##_dummy_##_size_##_clk_

/*****************************************************************************/
#define SET_READ_DUAL_ADDR(_dummy_, _size_, _clk_) \
    static struct spi_op_info read_dual_addr_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_DUAL_ADDR, SPI_CMD_READ_DUAL_ADDR, _dummy_, _size_, _clk_ }

#define READ_DUAL_ADDR(_dummy_, _size_, _clk_) \
    read_dual_addr_##_dummy_##_size_##_clk_

/*****************************************************************************/
#define SET_READ_QUAD(_dummy_, _size_, _clk_) \
    static struct spi_op_info read_quad_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_QUAD, SPI_CMD_READ_QUAD, _dummy_, _size_, _clk_ }

#define READ_QUAD(_dummy_, _size_, _clk_) read_quad_##_dummy_##_size_##_clk_

/*****************************************************************************/
#define SET_READ_QUAD_ADDR(_dummy_, _size_, _clk_) \
    static struct spi_op_info read_quad_addr_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_QUAD_ADDR, SPI_CMD_READ_QUAD_ADDR, _dummy_, _size_, _clk_ }

#define READ_QUAD_ADDR(_dummy_, _size_, _clk_) \
    read_quad_addr_##_dummy_##_size_##_clk_

/*****************************************************************************/
#define SET_WRITE_STD(_dummy_, _size_, _clk_) \
    static struct spi_op_info write_std_##_dummy_##_size_##_clk_ = { \
    SPI_IF_WRITE_STD, SPI_CMD_WRITE_STD, _dummy_, _size_, _clk_ }

#define WRITE_STD(_dummy_, _size_, _clk_) write_std_##_dummy_##_size_##_clk_

/*****************************************************************************/
#define SET_WRITE_QUAD(_dummy_, _size_, _clk_) \
    static struct spi_op_info write_quad_##_dummy_##_size_##_clk_ = { \
    SPI_IF_WRITE_QUAD, SPI_CMD_WRITE_QUAD, _dummy_, _size_, _clk_ }

#define WRITE_QUAD(_dummy_, _size_, _clk_) \
    write_quad_##_dummy_##_size_##_clk_

/*****************************************************************************/
#define SET_ERASE_SECTOR_128K(_dummy_, _size_, _clk_) \
    static struct spi_op_info erase_sector_128k_##_dummy_##_size_##_clk_ \
    = { SPI_IF_ERASE_SECTOR_128K, SPI_CMD_SE_128K, _dummy_, _size_, _clk_ }

#define ERASE_SECTOR_128K(_dummy_, _size_, _clk_) \
    erase_sector_128k_##_dummy_##_size_##_clk_

#define SET_ERASE_SECTOR_256K(_dummy_, _size_, _clk_) \
    static struct spi_op_info erase_sector_256k_##_dummy_##_size_##_clk_ \
    = { SPI_IF_ERASE_SECTOR_256K, SPI_CMD_SE_256K, _dummy_, _size_, _clk_ }

#define ERASE_SECTOR_256K(_dummy_, _size_, _clk_) \
    erase_sector_256k_##_dummy_##_size_##_clk_

/*****************************************************************************/
SET_READ_STD(1, INFINITE, 24);

SET_READ_FAST(1, INFINITE, 50);
SET_READ_FAST(1, INFINITE, 80);
SET_READ_FAST(1, INFINITE, 104);
SET_READ_FAST(1, INFINITE, 108);
SET_READ_FAST(1, INFINITE, 120);

SET_READ_DUAL(1, INFINITE, 50);
SET_READ_DUAL(1, INFINITE, 80);
SET_READ_DUAL(1, INFINITE, 104);
SET_READ_DUAL(1, INFINITE, 108);
SET_READ_DUAL(1, INFINITE, 120);

SET_READ_DUAL_ADDR(1, INFINITE, 60);
SET_READ_DUAL_ADDR(1, INFINITE, 80);
SET_READ_DUAL_ADDR(1, INFINITE, 104);
SET_READ_DUAL_ADDR(1, INFINITE, 108);
SET_READ_DUAL_ADDR(1, INFINITE, 120);

SET_READ_QUAD(1, INFINITE, 50);
SET_READ_QUAD(1, INFINITE, 80);
SET_READ_QUAD(1, INFINITE, 104);
SET_READ_QUAD(1, INFINITE, 108);
SET_READ_QUAD(1, INFINITE, 120);

SET_READ_QUAD_ADDR(1, INFINITE, 60);
SET_READ_QUAD_ADDR(1, INFINITE, 104);
SET_READ_QUAD_ADDR(2, INFINITE, 80);
SET_READ_QUAD_ADDR(2, INFINITE, 104);
SET_READ_QUAD_ADDR(1, INFINITE, 108);
SET_READ_QUAD_ADDR(1, INFINITE, 120);

/*****************************************************************************/
SET_WRITE_STD(0, 256, 24);
SET_WRITE_STD(0, 256, 80);
SET_WRITE_STD(0, 256, 104);

SET_WRITE_QUAD(0, 256, 80);
SET_WRITE_QUAD(0, 256, 104);
SET_WRITE_QUAD(0, 256, 108);
SET_WRITE_QUAD(0, 256, 120);

/*****************************************************************************/
SET_ERASE_SECTOR_128K(0, SZ_128K, 24);
SET_ERASE_SECTOR_128K(0, SZ_128K, 80);
SET_ERASE_SECTOR_128K(0, SZ_128K, 104);

SET_ERASE_SECTOR_256K(0, SZ_256K, 24);
SET_ERASE_SECTOR_256K(0, SZ_256K, 80);
SET_ERASE_SECTOR_256K(0, SZ_256K, 104);

/*****************************************************************************/
static struct spi_nand_driver spi_nand_driver_general = {
    .wait_ready   = spi_general_wait_ready,
    .write_enable = spi_general_write_enable,
    .qe_enable = spi_general_qe_enable,
};

/*
 *   Some spi nand don't need set QE bit enable.
 */
static struct spi_nand_driver spi_nand_driver_no_qe = {
    .wait_ready = spi_general_wait_ready,
    .write_enable = spi_general_write_enable,
};

/*****************************************************************************/
#define SPI_NAND_ID_TAB_VER     "2.2"

/******* SPI Nand ID Table ***************************************************
* Version   Manufacturer    Chip Name   Size        Operation
* 1.0       ESMT        F50L512M41A 64MB        Add 5 chip
*       GD      5F1GQ4UAYIG 128MB
*       GD      5F2GQ4UAYIG 256MB
*       GD      5F4GQ4UAYIG 512MB
*       GD      5F4GQ4UBYIG 512MB
* 1.1       ESMT        F50L1G41A   128MB       Add 2 chip
*       Winbond     W25N01GV    128MB
* 1.2       GD      5F1GQ4UBYIG 128MB       Add 2 chip
*       GD      5F2GQ4UBYIG 256MB
* 1.3       ATO     ATO25D1GA   128MB       Add 1 chip
*       Micron      MT29F1G01   128MB       Add 3 chip
*       Micron      MT29F2G01   256MB
*       Micron      MT29F4G01   512MB
* 1.4       MXIC        MX35LF1GE4AB    128MB       Add 2 chip
* 1.5       Paragon     PN26G01A    128MB       Add 1 chip
* 1.6       All-flash   AFS1GQ4UAC  128MB       Add 1 chip
* 1.7       TOSHIBA     TC58CVG0S3H 128MB       Add 2 chip
*       TOSHIBA     TC58CVG2S0H 512MB
* 1.8       ALL-flash   AFS2GQ4UAD  256MB       Add 2 chip
*       Paragon     PN26G02A    256MB
* 1.9       TOSHIBA     TC58CVG1S3H 256MB       Add 1 chip
* 2.0       HeYangTek   HYF1GQ4UAACAE   128MB       Add 3 chip
*       HeYangTek   HYF2GQ4UAACAE   256MB
*       HeYangTek   HYF4GQ4UAACBE   512MB
* 2.1       Micron      MT29F1G01ABA    128MB       Add 1 chip
* 2.2       Micron      MT29F2G01ABA    256MB       Add 1 chip
******************************************************************************/
struct hisnfc_chip_info hisnfc_spi_nand_flash_table[] = {
    /* Micron MT29F1G01ABA 1GBit */
    {
        .name      = "MT29F1G01ABA",
        .id        = {0x2C, 0x14},
        .id_len    = 2,
        .chipsize  = SZ_128M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 128,
        .badblock_pos = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 80),
            &READ_DUAL(1, INFINITE, 80),
            &READ_DUAL_ADDR(1, INFINITE, 80),
            &READ_QUAD(1, INFINITE, 80),
            &READ_QUAD_ADDR(2, INFINITE, 80),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 80),
            &WRITE_QUAD(0, 256, 80),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 80),
            0
        },
        .driver    = &spi_nand_driver_no_qe,
    },

    /* Micron MT29F2G01ABA 2GBit */
    {
        .name      = "MT29F2G01ABA",
        .id        = {0x2C, 0x24},
        .id_len    = 2,
        .chipsize  = SZ_256M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 128,
        .badblock_pos = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 108),
            &READ_DUAL(1, INFINITE, 108),
            &READ_DUAL_ADDR(1, INFINITE, 108),
            &READ_QUAD(1, INFINITE, 108),
            &READ_QUAD_ADDR(2, INFINITE, 104),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 80),
            &WRITE_QUAD(0, 256, 108),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 80),
            0
        },
        .driver    = &spi_nand_driver_no_qe,
    },

    /* ESMT F50L512M41A 512Mbit */
    {
        .name      = "F50L512M41A",
        .id        = {0xC8, 0x20},
        .id_len    = 2,
        .chipsize  = SZ_64M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 64,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 104),
            &READ_DUAL(1, INFINITE, 104),
            &READ_QUAD(1, INFINITE, 104),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 104),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_no_qe,
    },

    /* ESMT F50L1G41A 1Gbit */
    {
        .name      = "F50L1G41A",
        .id        = {0xC8, 0x21},
        .id_len    = 2,
        .chipsize  = SZ_128M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 64,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 104),
            &READ_DUAL(1, INFINITE, 104),
            &READ_QUAD(1, INFINITE, 104),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 104),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_no_qe,
    },

    /* GD 5F1GQ4UAYIG 1Gbit */
    {
        .name      = "5F1GQ4UAYIG",
        .id        = {0xc8, 0xf1},
        .id_len    = 2,
        .chipsize  = SZ_128M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 64,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 120),
            &READ_DUAL(1, INFINITE, 120),
            &READ_DUAL_ADDR(1, INFINITE, 120),
            &READ_QUAD(1, INFINITE, 120),
            &READ_QUAD_ADDR(1, INFINITE, 120),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 120),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* GD 5F1GQ4UBYIG 1Gbit */
    {
        .name      = "5F1GQ4UBYIG",
        .id        = {0xc8, 0xd1},
        .id_len    = 2,
        .chipsize  = SZ_128M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 128,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 120),
            &READ_DUAL(1, INFINITE, 120),
            &READ_DUAL_ADDR(1, INFINITE, 120),
            &READ_QUAD(1, INFINITE, 120),
            &READ_QUAD_ADDR(1, INFINITE, 120),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 120),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* GD 5F2GQ4UAYIG 2Gbit */
    {
        .name      = "5F2GQ4UAYIG",
        .id        = {0xc8, 0xf2},
        .id_len    = 2,
        .chipsize  = SZ_256M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 64,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 120),
            &READ_DUAL(1, INFINITE, 120),
            &READ_DUAL_ADDR(1, INFINITE, 120),
            &READ_QUAD(1, INFINITE, 120),
            &READ_QUAD_ADDR(1, INFINITE, 120),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 120),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* GD 5F2GQ4UBYIG 2Gbit */
    {
        .name      = "5F2GQ4UBYIG",
        .id        = {0xc8, 0xd2},
        .id_len    = 2,
        .chipsize  = SZ_256M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 128,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 120),
            &READ_DUAL(1, INFINITE, 120),
            &READ_DUAL_ADDR(1, INFINITE, 120),
            &READ_QUAD(1, INFINITE, 120),
            &READ_QUAD_ADDR(1, INFINITE, 120),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 120),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* GD 5F4GQ4UAYIG 4Gbit */
    {
        .name      = "5F4GQ4UAYIG",
        .id        = {0xc8, 0xf4},
        .id_len    = 2,
        .chipsize  = SZ_512M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 64,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 120),
            &READ_DUAL(1, INFINITE, 120),
            &READ_DUAL_ADDR(1, INFINITE, 120),
            &READ_QUAD(1, INFINITE, 120),
            &READ_QUAD_ADDR(1, INFINITE, 120),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 120),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* GD 5F4GQ4UBYIG 4Gbit */
    {
        .name      = "5F4GQ4UBYIG",
        .id        = {0xc8, 0xd4},
        .id_len    = 2,
        .chipsize  = SZ_512M,
        .erasesize = SZ_256K,
        .pagesize  = SZ_4K,
        .oobsize   = 256,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 120),
            &READ_DUAL(1, INFINITE, 120),
            &READ_DUAL_ADDR(1, INFINITE, 120),
            &READ_QUAD(1, INFINITE, 120),
            &READ_QUAD_ADDR(1, INFINITE, 120),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 120),
            0
        },
        .erase     = {
            &ERASE_SECTOR_256K(0, SZ_256K, 24),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* Winbond W25N01GV 1Gbit */
    {
        .name      = "W25N01GV",
        .id        = {0xef, 0xaa, 0x21},
        .id_len    = 3,
        .chipsize  = SZ_128M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 64,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 104),
            &READ_DUAL(1, INFINITE, 104),
            &READ_DUAL_ADDR(1, INFINITE, 104),
            &READ_QUAD(1, INFINITE, 104),
            &READ_QUAD_ADDR(2, INFINITE, 104),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 104),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_no_qe,
    },

    /* ATO ATO25D1GA 1Gbit */
    {
        .name      = "ATO25D1GA",
        .id        = {0x9b, 0x12},
        .id_len    = 2,
        .chipsize  = SZ_128M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 64,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 104),
            &READ_QUAD(1, INFINITE, 104),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 104),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* Micron MT29F1G01 */
    {
        .name      = "MT29F1G01",
        .id        = {0x2c, 0x12},
        .id_len    = 2,
        .chipsize  = SZ_128M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 64,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 50),
            &READ_DUAL(1, INFINITE, 50),
            &READ_QUAD(1, INFINITE, 50),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_no_qe,
    },

    /* Micron MT29F2G01 */
    {
        .name      = "MT29F2G01",
        .id        = {0x2c, 0x22},
        .id_len    = 2,
        .chipsize  = SZ_256M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 64,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 50),
            &READ_DUAL(1, INFINITE, 50),
            &READ_QUAD(1, INFINITE, 50),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_no_qe,
    },

    /* Micron MT29F4G01 */
    {
        .name      = "MT29F4G01",
        .id        = {0x2c, 0x32},
        .id_len    = 2,
        .chipsize  = SZ_512M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 64,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 50),
            &READ_DUAL(1, INFINITE, 50),
            &READ_QUAD(1, INFINITE, 50),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_no_qe,
    },

    /* MXIC MX35LF1GE4AB 1Gbit */
    {
        .name      = "MX35LF1GE4AB",
        .id        = {0xc2, 0x12},
        .id_len    = 2,
        .chipsize  = SZ_128M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 64,
        .badblock_pos    = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 104),
            &READ_QUAD(1, INFINITE, 104),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 104),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* Paragon PN26G01A 1Gbit */
    {
        .name      = "PN26G01A",
        .id        = {0xa1, 0xe1},
        .id_len    = 2,
        .chipsize  = SZ_128M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 128,
        .badblock_pos = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 108),
            &READ_DUAL(1, INFINITE, 108),
            &READ_DUAL_ADDR(1, INFINITE, 108),
            &READ_QUAD(1, INFINITE, 108),
            &READ_QUAD_ADDR(1, INFINITE, 108),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 108),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* Paragon PN26G02A 2Gbit */
    {
        .name      = "PN26G02A",
        .id        = {0xa1, 0xe2},
        .id_len    = 2,
        .chipsize  = SZ_256M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 128,
        .badblock_pos = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 108),
            &READ_DUAL(1, INFINITE, 108),
            &READ_DUAL_ADDR(1, INFINITE, 108),
            &READ_QUAD(1, INFINITE, 108),
            &READ_QUAD_ADDR(1, INFINITE, 108),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 108),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* All-flash AFS1GQ4UAC 1Gbit */
    {
        .name      = "AFS1GQ4UAC",
        .id        = {0xc1, 0x51},
        .id_len    = 2,
        .chipsize  = SZ_128M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 128,
        .badblock_pos = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 104),
            &READ_DUAL(1, INFINITE, 104),
            &READ_DUAL_ADDR(1, INFINITE, 104),
            &READ_QUAD(1, INFINITE, 104),
            &READ_QUAD_ADDR(1, INFINITE, 104),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 104),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* All-flash AFS2GQ4UAD 2Gbit */
    {
        .name      = "AFS2GQ4UAD",
        .id        = {0xc1, 0x52},
        .id_len    = 2,
        .chipsize  = SZ_256M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 128,
        .badblock_pos = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 104),
            &READ_DUAL(1, INFINITE, 104),
            &READ_DUAL_ADDR(1, INFINITE, 104),
            &READ_QUAD(1, INFINITE, 104),
            &READ_QUAD_ADDR(1, INFINITE, 104),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 24),
            &WRITE_QUAD(0, 256, 104),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 24),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* TOSHIBA TC58CVG0S3H 1Gbit */
    {
        .name      = "TC58CVG0S3H",
        .id        = {0x98, 0xc2},
        .id_len    = 2,
        .chipsize  = SZ_128M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 64,
        .badblock_pos = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 104),
            &READ_DUAL(1, INFINITE, 104),
            &READ_QUAD(1, INFINITE, 104),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 104),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 104),
            0
        },
        .driver    = &spi_nand_driver_no_qe,
    },

    /* TOSHIBA TC58CVG2S0H 4Gbit */
    {
        .name      = "TC58CVG2S0H",
        .id        = {0x98, 0xcd},
        .id_len    = 2,
        .chipsize  = SZ_512M,
        .erasesize = SZ_256K,
        .pagesize  = SZ_4K,
        .oobsize   = 128,
        .badblock_pos = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 104),
            &READ_DUAL(1, INFINITE, 104),
            &READ_QUAD(1, INFINITE, 104),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 104),
            0
        },
        .erase     = {
            &ERASE_SECTOR_256K(0, SZ_256K, 104),
            0
        },
        .driver    = &spi_nand_driver_no_qe,
    },

    /* HeYangTek HYF1GQ4UAACAE 1Gbit */
    {
        .name      = "HYF1GQ4UAACAE",
        .id        = {0xc9, 0x51},
        .id_len    = 2,
        .chipsize  = SZ_128M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 128,
        .badblock_pos = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 50),
            &READ_DUAL(1, INFINITE, 50),
            &READ_DUAL_ADDR(1, INFINITE, 60),
            &READ_QUAD(1, INFINITE, 50),
            &READ_QUAD_ADDR(1, INFINITE, 60),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 80),
            &WRITE_QUAD(0, 256, 80),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 80),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* HeYangTek HYF2GQ4UAACAE 2Gbit */
    {
        .name      = "HYF2GQ4UAACAE",
        .id        = {0xc9, 0x52},
        .id_len    = 2,
        .chipsize  = SZ_256M,
        .erasesize = SZ_128K,
        .pagesize  = SZ_2K,
        .oobsize   = 128,
        .badblock_pos = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 50),
            &READ_DUAL(1, INFINITE, 50),
            &READ_DUAL_ADDR(1, INFINITE, 60),
            &READ_QUAD(1, INFINITE, 50),
            &READ_QUAD_ADDR(1, INFINITE, 60),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 80),
            &WRITE_QUAD(0, 256, 80),
            0
        },
        .erase     = {
            &ERASE_SECTOR_128K(0, SZ_128K, 80),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    /* HeYangTek HYF4GQ4UAACBE 4Gbit */
    {
        .name      = "HYF4GQ4UAACBE",
        .id        = {0xc9, 0xd4},
        .id_len    = 2,
        .chipsize  = SZ_512M,
        .erasesize = SZ_256K,
        .pagesize  = SZ_4K,
        .oobsize   = 256,
        .badblock_pos = BBP_FIRST_PAGE,
        .read      = {
            &READ_STD(1, INFINITE, 24),
            &READ_FAST(1, INFINITE, 50),
            &READ_DUAL(1, INFINITE, 50),
            &READ_DUAL_ADDR(1, INFINITE, 60),
            &READ_QUAD(1, INFINITE, 50),
            &READ_QUAD_ADDR(1, INFINITE, 60),
            0
        },
        .write     = {
            &WRITE_STD(0, 256, 80),
            &WRITE_QUAD(0, 256, 80),
            0
        },
        .erase     = {
            &ERASE_SECTOR_256K(0, SZ_256K, 80),
            0
        },
        .driver    = &spi_nand_driver_general,
    },

    {   .id_len    = 0, },
};

/*****************************************************************************/
static void hisnfc100_spi_search_rw(struct hisnfc_chip_info *spiinfo,
                                    struct spi_op_info *spiop_rw, unsigned iftype,
                                    unsigned max_dummy, int rw_type)
{
    int ix = 0;
    struct spi_op_info **spiop, **fitspiop;

    for (fitspiop = spiop = (rw_type ? spiinfo->write : spiinfo->read);
            (*spiop) && ix < MAX_SPI_NAND_OP; spiop++, ix++)
        if (((*spiop)->iftype & iftype)
                && ((*spiop)->dummy <= max_dummy)
                && (*fitspiop)->iftype < (*spiop)->iftype) {
            fitspiop = spiop;
        }

    memcpy(spiop_rw, (*fitspiop), sizeof(struct spi_op_info));
}

/*****************************************************************************/
static void hisnfc100_spi_get_erase(struct hisnfc_chip_info *spiinfo,
                                    struct spi_op_info *spiop_erase)
{
    int ix;

    spiop_erase->size = 0;
    for (ix = 0; ix < MAX_SPI_NAND_OP; ix++) {
        if (spiinfo->erase[ix] == NULL) {
            break;
        }
        if (spiinfo->erasesize == spiinfo->erase[ix]->size) {
            memcpy(&spiop_erase[ix], spiinfo->erase[ix],
                   sizeof(struct spi_op_info));
            break;
        }
    }
}

/*****************************************************************************/
static void hisnfc100_map_iftype_and_clock(struct hisnfc_op *spi)
{
    int ix;
    const int iftype_read[] = {
        SPI_IF_READ_STD,       HISNFC100_IFCYCLE_STD,
        SPI_IF_READ_FAST,      HISNFC100_IFCYCLE_STD,
        SPI_IF_READ_DUAL,      HISNFC100_IFCYCLE_DUAL,
        SPI_IF_READ_DUAL_ADDR, HISNFC100_IFCYCLE_DUAL_ADDR,
        SPI_IF_READ_QUAD,      HISNFC100_IFCYCLE_QUAD,
        SPI_IF_READ_QUAD_ADDR, HISNFC100_IFCYCLE_QUAD_ADDR,
        0, 0,
    };
    const int iftype_write[] = {
        SPI_IF_WRITE_STD,       HISNFC100_IFCYCLE_STD,
        SPI_IF_WRITE_QUAD,      HISNFC100_IFCYCLE_QUAD,
        0, 0,
    };

    for (ix = 0; iftype_write[ix]; ix += 2) {
        if (spi->write->iftype == iftype_write[ix]) {
            spi->write->iftype = iftype_write[ix + 1];
            break;
        }
    }
    hisnfc100_get_best_clock(&spi->write->clock);

    for (ix = 0; iftype_read[ix]; ix += 2) {
        if (spi->read->iftype == iftype_read[ix]) {
            spi->read->iftype = iftype_read[ix + 1];
            break;
        }
    }
    hisnfc100_get_best_clock(&spi->read->clock);

    hisnfc100_get_best_clock(&spi->erase->clock);
    spi->erase->iftype = HISNFC100_IFCYCLE_STD;
}

/*****************************************************************************/
static void hisnfc100_spi_probe(struct hisnfc_host *host,
                                struct hisnfc_chip_info *spi_dev)
{
    unsigned regval;
    struct hisnfc_op *spi = host->spi;

    spi->host = host;
    spi->driver = spi_dev->driver;

    hisnfc100_spi_search_rw(spi_dev, spi->read, HISNFC100_SUPPORT_READ,
                            HISNFC100_SUPPORT_MAX_DUMMY, SPI_NAND_READ);

    hisnfc100_spi_search_rw(spi_dev, spi->write, HISNFC100_SUPPORT_WRITE,
                            HISNFC100_SUPPORT_MAX_DUMMY, SPI_NAND_WRITE);

    hisnfc100_spi_get_erase(spi_dev, spi->erase);
    hisnfc100_map_iftype_and_clock(spi);

    if (spi->driver->qe_enable) {
        if (spi->driver->qe_enable(spi)) {
            pr_err("%s set feature QE failed!\n", __func__);
        }
    }

    spi_feature_op(host, GET_OP, PROTECTION_ADDR, &regval);
    if (ANY_BP_ENABLE(regval)) {
        regval &= ~ALL_BP_MASK;
        spi_feature_op(host, SET_OP, PROTECTION_ADDR, &regval);

        spi->driver->wait_ready(spi);

        spi_feature_op(host, GET_OP, PROTECTION_ADDR, &regval);
        if (ANY_BP_ENABLE(regval)) {
            pr_err("%s write protection disable fail! val[%#x]\n",
                   __func__, regval);
        }
    }

    spi_feature_op(host, GET_OP, FEATURE_ADDR, &regval);
    if (regval & FEATURE_ECC_ENABLE) {
        regval &= ~FEATURE_ECC_ENABLE;
        spi_feature_op(host, SET_OP, FEATURE_ADDR, &regval);

        spi->driver->wait_ready(spi);

        spi_feature_op(host, GET_OP, FEATURE_ADDR, &regval);
        if (regval & FEATURE_ECC_ENABLE) {
            pr_err("%s Internal ECC disable fail! val[%#x]\n",
                   __func__, regval);
        }
    }
}

static struct nand_flash_dev spi_nand_dev;
/*****************************************************************************/
static struct nand_flash_dev *spi_nand_get_flash_info(struct mtd_info *mtd,
        unsigned char *id)
{
    struct nand_chip *chip = mtd_to_nand(mtd);
    struct hisnfc_host *host = chip->priv;
    struct hisnfc_chip_info *spi_dev = hisnfc_spi_nand_flash_table;
    unsigned char ix = 0, len = 0, buffer[100];
    struct nand_flash_dev *flash_type = &spi_nand_dev;

    len = sprintf(buffer, "SPI Nand(cs %d) ID: %#x %#x",
                  host->cmd_option.chipselect, id[0], id[1]);

    for (; spi_dev->id_len; spi_dev++) {
        if (memcmp(id, spi_dev->id, spi_dev->id_len)) {
            continue;
        }

        for (ix = 2; ix < spi_dev->id_len; ix++) {
            len += sprintf(buffer + len, " %#x", id[ix]);
        }
        pr_info("%s\n", buffer);

        flash_type->name = spi_dev->name;
        memcpy(flash_type->id, spi_dev->id, spi_dev->id_len);
        flash_type->pagesize  = spi_dev->pagesize;
        flash_type->chipsize = spi_dev->chipsize >> 20;
        flash_type->erasesize = spi_dev->erasesize;
        flash_type->oobsize = spi_dev->oobsize;

        mtd->size = spi_dev->chipsize;
        mtd->oobsize = spi_dev->oobsize;
        mtd->erasesize = spi_dev->erasesize;
        mtd->writesize = spi_dev->pagesize;
        chip->chipsize = spi_dev->chipsize;

        if (host->mtd != mtd) {
            host->mtd = mtd;
        }
        hisnfc100_spi_probe(host, spi_dev);

        return flash_type;
    }

    return NULL;
}

/*****************************************************************************/
void spi_nand_ids_register(void)
{
    pr_info("SPI Nand ID Table Version %s\n", SPI_NAND_ID_TAB_VER);
    get_spi_nand_flash_type_hook = spi_nand_get_flash_info;
}

