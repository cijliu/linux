/*
 * Copyright (c) 2011 - 2014 Espressif System.
 *
 *   Serial I/F wrapper layer for eagle WLAN device,
 *    abstraction of buses like SDIO/SIP, and provides
 *    flow control for tx/rx layer
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
 */

#ifndef _ESP_SIF_H_
#define _ESP_SIF_H_

#include "esp_pub.h"
#include <linux/mmc/host.h>
#include <linux/spi/spi.h>

/*
 *  H/W SLC module definitions
 */

#define SIF_SLC_BLOCK_SIZE                512


/* S/W struct mapping to slc registers */
typedef struct slc_host_regs {
	/* do NOT read token_rdata
	 *
	 u32 pf_data;
	 u32 token_rdata;
	 */
	u32 intr_raw;
	u32 state_w0;
	u32 state_w1;
	u32 config_w0;
	u32 config_w1;
	u32 intr_status;
	u32 config_w2;
	u32 config_w3;
	u32 config_w4;
	u32 token_wdata;
	u32 intr_clear;
	u32 intr_enable;
} sif_slc_reg_t;


enum io_sync_type {
	ESP_SIF_NOSYNC = 0,
	ESP_SIF_SYNC,
};

typedef struct esp_sdio_ctrl {
	struct sdio_func *func;
	struct esp_pub *epub;


	struct list_head free_req;

	u8 *dma_buffer;

	spinlock_t scat_lock;
	struct list_head scat_req;

	bool off;
	atomic_t irq_handling;
	const struct sdio_device_id *id;
	u32 slc_blk_sz;
	u32 target_id;
	u32 slc_window_end_addr;

	struct slc_host_regs slc_regs;
	atomic_t irq_installed;

} esp_sdio_ctrl_t;

#define SIF_TO_DEVICE                    0x1
#define SIF_FROM_DEVICE                    0x2

#define SIF_SYNC             0x00000010
#define SIF_ASYNC           0x00000020

#define SIF_BYTE_BASIS              0x00000040
#define SIF_BLOCK_BASIS             0x00000080

#define SIF_FIXED_ADDR           0x00000100
#define SIF_INC_ADDR     0x00000200

#define EPUB_CTRL_CHECK(_epub, _go_err) do{\
	if (_epub == NULL) {\
		ESSERT(0);\
		goto _go_err;\
	}\
	if ((_epub)->sif == NULL) {\
		ESSERT(0);\
		goto _go_err;\
	}\
}while(0)

#define EPUB_FUNC_CHECK(_epub, _go_err) do{\
	if (_epub == NULL) {\
		ESSERT(0);\
		goto _go_err;\
	}\
	if ((_epub)->sif == NULL) {\
		ESSERT(0);\
		goto _go_err;\
	}\
	if (((struct esp_sdio_ctrl *)(_epub)->sif)->func == NULL) {\
		ESSERT(0);\
		goto _go_err;\
	}\
}while(0)

#define EPUB_TO_CTRL(_epub) (((struct esp_sdio_ctrl *)(_epub)->sif))

#define EPUB_TO_FUNC(_epub) (((struct esp_sdio_ctrl *)(_epub)->sif)->func)

void sdio_io_writeb(struct esp_pub *epub, u8 value, int addr, int *res);
u8 sdio_io_readb(struct esp_pub *epub, int addr, int *res);


void sif_enable_irq(struct esp_pub *epub);
void sif_disable_irq(struct esp_pub *epub);
void sif_disable_target_interrupt(struct esp_pub *epub);

u32 sif_get_blksz(struct esp_pub *epub);
u32 sif_get_target_id(struct esp_pub *epub);

void sif_dsr(struct sdio_func *func);
int sif_io_raw(struct esp_pub *epub, u32 addr, u8 * buf, u32 len,
	       u32 flag);
int sif_io_sync(struct esp_pub *epub, u32 addr, u8 * buf, u32 len,
		u32 flag);
int sif_io_async(struct esp_pub *epub, u32 addr, u8 * buf, u32 len,
		 u32 flag, void *context);
int sif_lldesc_read_sync(struct esp_pub *epub, u8 * buf, u32 len);
int sif_lldesc_write_sync(struct esp_pub *epub, u8 * buf, u32 len);
int sif_lldesc_read_raw(struct esp_pub *epub, u8 * buf, u32 len,
			bool noround);
int sif_lldesc_write_raw(struct esp_pub *epub, u8 * buf, u32 len);

int sif_platform_get_irq_no(void);
int sif_platform_is_irq_occur(void);
void sif_platform_irq_clear(void);
void sif_platform_irq_mask(int enable_mask);
int sif_platform_irq_init(void);
void sif_platform_irq_deinit(void);

int esp_common_read(struct esp_pub *epub, u8 * buf, u32 len, int sync,
		    bool noround);
int esp_common_write(struct esp_pub *epub, u8 * buf, u32 len, int sync);
int esp_common_read_with_addr(struct esp_pub *epub, u32 addr, u8 * buf,
			      u32 len, int sync);
int esp_common_write_with_addr(struct esp_pub *epub, u32 addr, u8 * buf,
			       u32 len, int sync);

int esp_common_readbyte_with_addr(struct esp_pub *epub, u32 addr, u8 * buf,
				  int sync);
int esp_common_writebyte_with_addr(struct esp_pub *epub, u32 addr, u8 buf,
				   int sync);

int sif_read_reg_window(struct esp_pub *epub, unsigned int reg_addr,
			unsigned char *value);
int sif_write_reg_window(struct esp_pub *epub, unsigned int reg_addr,
			 unsigned char *value);
int sif_ack_target_read_err(struct esp_pub *epub);
int sif_had_io_enable(struct esp_pub *epub);

struct slc_host_regs *sif_get_regs(struct esp_pub *epub);

void sif_lock_bus(struct esp_pub *epub);
void sif_unlock_bus(struct esp_pub *epub);

int sif_interrupt_target(struct esp_pub *epub, u8 index);
#ifdef USE_EXT_GPIO
int sif_config_gpio_mode(struct esp_pub *epub, u8 gpio_num, u8 gpio_mode);
int sif_set_gpio_output(struct esp_pub *epub, u16 mask, u16 value);
int sif_get_gpio_intr(struct esp_pub *epub, u16 intr_mask, u16 * value);
int sif_get_gpio_input(struct esp_pub *epub, u16 * mask, u16 * value);
#endif

void check_target_id(struct esp_pub *epub);

void sif_record_bt_config(int value);
int sif_get_bt_config(void);
void sif_record_rst_config(int value);
int sif_get_rst_config(void);
void sif_record_ate_config(int value);
int sif_get_ate_config(void);
void sif_record_retry_config(void);
int sif_get_retry_config(void);
void sif_record_wakeup_gpio_config(int value);
int sif_get_wakeup_gpio_config(void);

#define sif_reg_read_sync(epub, addr, buf, len) sif_io_sync((epub), (addr), (buf), (len), SIF_FROM_DEVICE | SIF_BYTE_BASIS | SIF_INC_ADDR)

#define sif_reg_write_sync(epub, addr, buf, len) sif_io_sync((epub), (addr), (buf), (len), SIF_TO_DEVICE | SIF_BYTE_BASIS | SIF_INC_ADDR)

#endif				/* _ESP_SIF_H_ */
