/*
 * Copyright (c) 2009 - 2014 Espressif System.
 * 
 * SIP ctrl packet parse and pack
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

#include <net/mac80211.h>
#include <net/cfg80211.h>
#include <linux/skbuff.h>
#include <linux/bitops.h>
#include <linux/firmware.h>

#include "esp_pub.h"
#include "esp_sip.h"
#include "esp_ctrl.h"
#include "esp_sif.h"
#include "esp_debug.h"
#include "esp_wmac.h"
#include "esp_utils.h"
#include "esp_wl.h"
#include "esp_file.h"
#include "esp_path.h"
#ifdef TEST_MODE
#include "testmode.h"
#endif				/* TEST_MODE */
#include "esp_version.h"

extern struct completion *gl_bootup_cplx;

static void esp_tx_ba_session_op(struct esp_sip *sip,
				 struct esp_node *node,
				 trc_ampdu_state_t state, u8 tid)
{
	struct esp_tx_tid *txtid;

	txtid = &node->tid[tid];
	if (state == TRC_TX_AMPDU_STOPPED) {
		if (txtid->state == ESP_TID_STATE_OPERATIONAL) {
			esp_dbg(ESP_DBG_TXAMPDU,
				"%s tid %d TXAMPDU GOT STOP EVT\n",
				__func__, tid);

			spin_lock_bh(&sip->epub->tx_ampdu_lock);
			txtid->state = ESP_TID_STATE_WAIT_STOP;
			spin_unlock_bh(&sip->epub->tx_ampdu_lock);
			ieee80211_stop_tx_ba_session(node->sta, (u16) tid);
		} else {
			esp_dbg(ESP_DBG_TXAMPDU,
				"%s tid %d TXAMPDU GOT STOP EVT IN WRONG STATE %d\n",
				__func__, tid, txtid->state);
		}
	} else if (state == TRC_TX_AMPDU_OPERATIONAL) {
		if (txtid->state == ESP_TID_STATE_STOP) {
			esp_dbg(ESP_DBG_TXAMPDU,
				"%s tid %d TXAMPDU GOT OPERATIONAL\n",
				__func__, tid);

			spin_lock_bh(&sip->epub->tx_ampdu_lock);
			txtid->state = ESP_TID_STATE_TRIGGER;
			spin_unlock_bh(&sip->epub->tx_ampdu_lock);
			ieee80211_start_tx_ba_session(node->sta, (u16) tid,
						      0);

		} else if (txtid->state == ESP_TID_STATE_OPERATIONAL) {
			sip_send_ampdu_action(sip->epub,
					      SIP_AMPDU_TX_OPERATIONAL,
					      node->sta->addr, tid,
					      node->ifidx, 0);
		} else {
			esp_dbg(ESP_DBG_TXAMPDU,
				"%s tid %d TXAMPDU GOT OPERATIONAL EVT IN WRONG STATE %d\n",
				__func__, tid, txtid->state);
		}
	}
}

int sip_parse_events(struct esp_sip *sip, u8 * buf)
{
	struct sip_hdr *hdr = (struct sip_hdr *) buf;

	switch (hdr->c_evtid) {
	case SIP_EVT_TARGET_ON:{
			/* use rx work queue to send... */
			if (atomic_read(&sip->state) == SIP_PREPARE_BOOT
			    || atomic_read(&sip->state) == SIP_BOOT) {
				atomic_set(&sip->state, SIP_SEND_INIT);
				queue_work(sip->epub->esp_wkq,
					   &sip->rx_process_work);
			} else {
				esp_dbg(ESP_DBG_ERROR,
					"%s boot during wrong state %d\n",
					__func__,
					atomic_read(&sip->state));
			}
			break;
		}

	case SIP_EVT_BOOTUP:{
			struct sip_evt_bootup2 *bootup_evt =
			    (struct sip_evt_bootup2 *) (buf +
							SIP_CTRL_HDR_LEN);
			if (sip->rawbuf)
				kfree(sip->rawbuf);

			sip_post_init(sip, bootup_evt);

			if (gl_bootup_cplx)
				complete(gl_bootup_cplx);

			break;
		}
	case SIP_EVT_RESETTING:{
			sip->epub->wait_reset = 1;
			if (gl_bootup_cplx)
				complete(gl_bootup_cplx);
			break;
		}
	case SIP_EVT_SLEEP:{
			//atomic_set(&sip->epub->ps.state, ESP_PM_ON);
			break;
		}
	case SIP_EVT_TXIDLE:{
			//struct sip_evt_txidle *txidle = (struct sip_evt_txidle *)(buf + SIP_CTRL_HDR_LEN);
			//sip_txdone_clear(sip, txidle->last_seq);
			break;
		}

	case SIP_EVT_SCAN_RESULT:{
			struct sip_evt_scan_report *report =
			    (struct sip_evt_scan_report *) (buf +
							    SIP_CTRL_HDR_LEN);
			if (atomic_read(&sip->epub->wl.off)) {
				esp_dbg(ESP_DBG_ERROR,
					"%s scan result while wlan off\n",
					__func__);
				return 0;
			}
			sip_scandone_process(sip, report);

			break;
		}

	case SIP_EVT_ROC:{
			struct sip_evt_roc *report =
			    (struct sip_evt_roc *) (buf +
						    SIP_CTRL_HDR_LEN);
			esp_rocdone_process(sip->epub->hw, report);
			break;
		}


#ifdef ESP_RX_COPYBACK_TEST

	case SIP_EVT_COPYBACK:{
			u32 len = hdr->len - SIP_CTRL_HDR_LEN;

			esp_dbg(ESP_DBG_TRACE,
				"%s copyback len %d   seq %u\n", __func__,
				len, hdr->seq);

			memcpy(copyback_buf + copyback_offset,
			       pkt->buf + SIP_CTRL_HDR_LEN, len);
			copyback_offset += len;

			//show_buf(pkt->buf, 256);

			//how about totlen % 256 == 0??
			if (hdr->hdr.len < 256) {
				kfree(copyback_buf);
			}
		}
		break;
#endif				/* ESP_RX_COPYBACK_TEST */
	case SIP_EVT_CREDIT_RPT:
		break;

#ifdef TEST_MODE
	case SIP_EVT_WAKEUP:{
			u8 check_str[12];
			struct sip_evt_wakeup *wakeup_evt =
			    (struct sip_evt_wakeup *) (buf +
						       SIP_CTRL_HDR_LEN);
			sprintf((char *) &check_str, "%d",
				wakeup_evt->check_data);
			esp_test_cmd_event(TEST_CMD_WAKEUP,
					   (char *) &check_str);
			break;
		}

	case SIP_EVT_DEBUG:{
			u8 check_str[640];
			sip_parse_event_debug(sip->epub, buf, check_str);
			esp_dbg(ESP_DBG_TRACE, "%s", check_str);
			esp_test_cmd_event(TEST_CMD_DEBUG,
					   (char *) &check_str);
			break;
		}

	case SIP_EVT_LOOPBACK:{
			u8 check_str[12];
			struct sip_evt_loopback *loopback_evt =
			    (struct sip_evt_loopback *) (buf +
							 SIP_CTRL_HDR_LEN);
			esp_dbg(ESP_DBG_LOG, "%s loopback len %d seq %u\n",
				__func__, hdr->len, hdr->seq);

			if (loopback_evt->pack_id != get_loopback_id()) {
				sprintf((char *) &check_str,
					"seq id error %d, expect %d",
					loopback_evt->pack_id,
					get_loopback_id());
				esp_test_cmd_event(TEST_CMD_LOOPBACK,
						   (char *) &check_str);
			}

			if ((loopback_evt->pack_id + 1) <
			    get_loopback_num()) {
				inc_loopback_id();
				sip_send_loopback_mblk(sip,
						       loopback_evt->txlen,
						       loopback_evt->rxlen,
						       get_loopback_id());
			} else {
				sprintf((char *) &check_str, "test over!");
				esp_test_cmd_event(TEST_CMD_LOOPBACK,
						   (char *) &check_str);
			}
			break;
		}
#endif				/*TEST_MODE */

	case SIP_EVT_SNPRINTF_TO_HOST:{
			u8 *p =
			    (buf + sizeof(struct sip_hdr) + sizeof(u16));
			u16 *len = (u16 *) (buf + sizeof(struct sip_hdr));
			char test_res_str[560];
			sprintf(test_res_str,
				"esp_host:%llx\nesp_target: %.*s",
				DRIVER_VER, *len, p);

			esp_dbg(ESP_DBG_TRACE, "%s\n", test_res_str);
			if (*len
			    && sip->epub->sdio_state ==
			    ESP_SDIO_STATE_FIRST_INIT) {
				char filename[256];
				if (mod_eagle_path_get() == NULL)
					sprintf(filename, "%s/%s", FWPATH,
						"test_results");
				else
					sprintf(filename, "%s/%s",
						mod_eagle_path_get(),
						"test_results");
				esp_dbg(ESP_DBG_TRACE,
					"SNPRINTF TO HOST: %s\n",
					test_res_str);
			}
			break;
		}
	case SIP_EVT_TRC_AMPDU:{
			struct sip_evt_trc_ampdu *ep =
			    (struct sip_evt_trc_ampdu *) (buf +
							  SIP_CTRL_HDR_LEN);
			struct esp_node *node = NULL;
			int i = 0;

			if (atomic_read(&sip->epub->wl.off)) {
				esp_dbg(ESP_DBG_ERROR,
					"%s scan result while wlan off\n",
					__func__);
				return 0;
			}

			node = esp_get_node_by_addr(sip->epub, ep->addr);
			if (node == NULL)
				break;
			for (i = 0; i < 8; i++) {
				if (ep->tid & (1 << i)) {
					esp_tx_ba_session_op(sip, node,
							     ep->state, i);
				}
			}
			break;
		}

#ifdef TEST_MODE
	case SIP_EVT_EP:{
			char *ep = (char *) (buf + SIP_CTRL_HDR_LEN);
			static int counter = 0;

			esp_dbg(ESP_ATE, "%s EVT_EP \n\n", __func__);
			if (counter++ < 2) {
				esp_dbg(ESP_ATE, "ATE: %s \n", ep);
			}

			esp_test_ate_done_cb(ep);

			break;
		}
#endif				/*TEST_MODE */

	case SIP_EVT_INIT_EP:{
			char *ep = (char *) (buf + SIP_CTRL_HDR_LEN);
			esp_dbg(ESP_ATE, "Phy Init: %s \n", ep);
			break;
		}

	case SIP_EVT_NOISEFLOOR:{
			struct sip_evt_noisefloor *ep =
			    (struct sip_evt_noisefloor *) (buf +
							   SIP_CTRL_HDR_LEN);
			atomic_set(&sip->noise_floor, ep->noise_floor);
			break;
		}
	default:
		break;
	}

	return 0;
}

#include "esp_init_data.h"

void sip_send_chip_init(struct esp_sip *sip)
{
	size_t size = 0;
	size = sizeof(esp_init_data);

	fix_init_data(esp_init_data, size);

	atomic_sub(1, &sip->tx_credits);

	sip_send_cmd(sip, SIP_CMD_INIT, size, (void *) esp_init_data);

}

int sip_send_config(struct esp_pub *epub, struct ieee80211_conf *conf)
{
	struct sk_buff *skb = NULL;
	struct sip_cmd_config *configcmd;

	skb =
	    sip_alloc_ctrl_skbuf(epub->sip,
				 sizeof(struct sip_cmd_config) +
				 sizeof(struct sip_hdr), SIP_CMD_CONFIG);
	if (!skb)
		return -EINVAL;
	esp_dbg(ESP_DBG_TRACE, "%s config center freq %d\n", __func__,
		conf->chandef.chan->center_freq);
	configcmd =
	    (struct sip_cmd_config *) (skb->data + sizeof(struct sip_hdr));
	configcmd->center_freq = conf->chandef.chan->center_freq;
	configcmd->duration = 0;
	return sip_cmd_enqueue(epub->sip, skb, ENQUEUE_PRIOR_TAIL);
}

int sip_send_bss_info_update(struct esp_pub *epub, struct esp_vif *evif,
			     u8 * bssid, int assoc)
{
	struct sk_buff *skb = NULL;
	struct sip_cmd_bss_info_update *bsscmd;

	skb =
	    sip_alloc_ctrl_skbuf(epub->sip,
				 sizeof(struct sip_cmd_bss_info_update) +
				 sizeof(struct sip_hdr),
				 SIP_CMD_BSS_INFO_UPDATE);
	if (!skb)
		return -EINVAL;

	bsscmd =
	    (struct sip_cmd_bss_info_update *) (skb->data +
						sizeof(struct sip_hdr));
	if (assoc == 2) {	//hack for softAP mode
		bsscmd->beacon_int = evif->beacon_interval;
	} else if (assoc == 1) {
		set_bit(ESP_WL_FLAG_CONNECT, &epub->wl.flags);
	} else {
		clear_bit(ESP_WL_FLAG_CONNECT, &epub->wl.flags);
	}
	bsscmd->bssid_no = evif->index;
	bsscmd->isassoc = assoc;
	bsscmd->beacon_int = evif->beacon_interval;
	memcpy(bsscmd->bssid, bssid, ETH_ALEN);
	return sip_cmd_enqueue(epub->sip, skb, ENQUEUE_PRIOR_TAIL);
}

int sip_send_wmm_params(struct esp_pub *epub, u8 aci,
			const struct ieee80211_tx_queue_params *params)
{
	struct sk_buff *skb = NULL;
	struct sip_cmd_set_wmm_params *bsscmd;
	skb =
	    sip_alloc_ctrl_skbuf(epub->sip,
				 sizeof(struct sip_cmd_set_wmm_params) +
				 sizeof(struct sip_hdr),
				 SIP_CMD_SET_WMM_PARAM);
	if (!skb)
		return -EINVAL;

	bsscmd =
	    (struct sip_cmd_set_wmm_params *) (skb->data +
					       sizeof(struct sip_hdr));
	bsscmd->aci = aci;
	bsscmd->aifs = params->aifs;
	bsscmd->txop_us = params->txop * 32;

	bsscmd->ecw_min = 32 - __builtin_clz(params->cw_min);
	bsscmd->ecw_max = 32 - __builtin_clz(params->cw_max);

	return sip_cmd_enqueue(epub->sip, skb, ENQUEUE_PRIOR_TAIL);
}

int sip_send_ampdu_action(struct esp_pub *epub, u8 action_num,
			  const u8 * addr, u16 tid, u16 ssn, u8 buf_size)
{
	int index = 0;
	struct sk_buff *skb = NULL;
	struct sip_cmd_ampdu_action *action;
	if (action_num == SIP_AMPDU_RX_START) {
		index = esp_get_empty_rxampdu(epub, addr, tid);
	} else if (action_num == SIP_AMPDU_RX_STOP) {
		index = esp_get_exist_rxampdu(epub, addr, tid);
	}
	if (index < 0)
		return -EACCES;
	skb =
	    sip_alloc_ctrl_skbuf(epub->sip,
				 sizeof(struct sip_cmd_ampdu_action) +
				 sizeof(struct sip_hdr),
				 SIP_CMD_AMPDU_ACTION);
	if (!skb)
		return -EINVAL;

	action =
	    (struct sip_cmd_ampdu_action *) (skb->data +
					     sizeof(struct sip_hdr));
	action->action = action_num;
	//for TX, it means interface index
	action->index = ssn;

	switch (action_num) {
	case SIP_AMPDU_RX_START:
		action->ssn = ssn;
	case SIP_AMPDU_RX_STOP:
		action->index = index;
	case SIP_AMPDU_TX_OPERATIONAL:
	case SIP_AMPDU_TX_STOP:
		action->win_size = buf_size;
		action->tid = tid;
		memcpy(action->addr, addr, ETH_ALEN);
		break;
	}

	return sip_cmd_enqueue(epub->sip, skb, ENQUEUE_PRIOR_TAIL);
}

#ifdef HW_SCAN
/*send cmd to target, if aborted is true, inform target stop scan, report scan complete imediately
  return 1: complete over, 0: success, still have next scan, -1: hardware failure
  */
int sip_send_scan(struct esp_pub *epub)
{
	struct cfg80211_scan_request *scan_req = epub->wl.scan_req;
	struct sk_buff *skb = NULL;
	struct sip_cmd_scan *scancmd;
	u8 *ptr = NULL;
	int i;
	u8 append_len, ssid_len;

	ESSERT(scan_req != NULL);
	ssid_len = scan_req->n_ssids == 0 ? 0 :
	    (scan_req->n_ssids ==
	     1 ? scan_req->ssids->ssid_len : scan_req->ssids->ssid_len +
	     (scan_req->ssids + 1)->ssid_len);
	append_len = ssid_len + scan_req->n_channels + scan_req->ie_len;

	skb =
	    sip_alloc_ctrl_skbuf(epub->sip,
				 sizeof(struct sip_cmd_scan) +
				 sizeof(struct sip_hdr) + append_len,
				 SIP_CMD_SCAN);

	if (!skb)
		return -EINVAL;

	ptr = skb->data;
	scancmd = (struct sip_cmd_scan *) (ptr + sizeof(struct sip_hdr));
	ptr += sizeof(struct sip_hdr);

	scancmd->aborted = false;

	if (scancmd->aborted == false) {
		ptr += sizeof(struct sip_cmd_scan);
		if (scan_req->n_ssids <= 0
		    || (scan_req->n_ssids == 1 && ssid_len == 0)) {
			scancmd->ssid_len = 0;
		} else {
			scancmd->ssid_len = ssid_len;
			if (scan_req->ssids->ssid_len == ssid_len)
				memcpy(ptr, scan_req->ssids->ssid,
				       scancmd->ssid_len);
			else
				memcpy(ptr, (scan_req->ssids + 1)->ssid,
				       scancmd->ssid_len);
		}

		ptr += scancmd->ssid_len;
		scancmd->n_channels = scan_req->n_channels;
		for (i = 0; i < scan_req->n_channels; i++)
			ptr[i] = scan_req->channels[i]->hw_value;

		ptr += scancmd->n_channels;
		if (scan_req->ie_len && scan_req->ie != NULL) {
			scancmd->ie_len = scan_req->ie_len;
			memcpy(ptr, scan_req->ie, scan_req->ie_len);
		} else {
			scancmd->ie_len = 0;
		}
		//add a flag that support two ssids,
		if (scan_req->n_ssids > 1)
			scancmd->ssid_len |= 0x80;

	}

	return sip_cmd_enqueue(epub->sip, skb, ENQUEUE_PRIOR_TAIL);
}
#endif

int sip_send_suspend_config(struct esp_pub *epub, u8 suspend)
{
	struct sip_cmd_suspend *cmd = NULL;
	struct sk_buff *skb = NULL;

	skb =
	    sip_alloc_ctrl_skbuf(epub->sip,
				 sizeof(struct sip_cmd_suspend) +
				 sizeof(struct sip_hdr), SIP_CMD_SUSPEND);

	if (!skb)
		return -EINVAL;

	cmd =
	    (struct sip_cmd_suspend *) (skb->data +
					sizeof(struct sip_hdr));
	cmd->suspend = suspend;
	return sip_cmd_enqueue(epub->sip, skb, ENQUEUE_PRIOR_TAIL);
}

int sip_send_ps_config(struct esp_pub *epub, struct esp_ps *ps)
{
	struct sip_cmd_ps *pscmd = NULL;
	struct sk_buff *skb = NULL;
	struct sip_hdr *shdr = NULL;

	skb =
	    sip_alloc_ctrl_skbuf(epub->sip,
				 sizeof(struct sip_cmd_ps) +
				 sizeof(struct sip_hdr), SIP_CMD_PS);

	if (!skb)
		return -EINVAL;


	shdr = (struct sip_hdr *) skb->data;
	pscmd = (struct sip_cmd_ps *) (skb->data + sizeof(struct sip_hdr));

	pscmd->dtim_period = ps->dtim_period;
	pscmd->max_sleep_period = ps->max_sleep_period;

	return sip_cmd_enqueue(epub->sip, skb, ENQUEUE_PRIOR_TAIL);
}

void sip_scandone_process(struct esp_sip *sip,
			  struct sip_evt_scan_report *scan_report)
{
	struct esp_pub *epub = sip->epub;

	esp_dbg(ESP_DBG_TRACE, "eagle hw scan report\n");

	if (epub->wl.scan_req) {
		hw_scan_done(epub, scan_report->aborted);
		epub->wl.scan_req = NULL;
	}
}

int sip_send_setkey(struct esp_pub *epub, u8 bssid_no, u8 * peer_addr,
		    struct ieee80211_key_conf *key, u8 isvalid)
{
	struct sip_cmd_setkey *setkeycmd;
	struct sk_buff *skb = NULL;

	skb =
	    sip_alloc_ctrl_skbuf(epub->sip,
				 sizeof(struct sip_cmd_setkey) +
				 sizeof(struct sip_hdr), SIP_CMD_SETKEY);

	if (!skb)
		return -EINVAL;

	setkeycmd =
	    (struct sip_cmd_setkey *) (skb->data + sizeof(struct sip_hdr));

	if (peer_addr) {
		memcpy(setkeycmd->addr, peer_addr, ETH_ALEN);
	} else {
		memset(setkeycmd->addr, 0, ETH_ALEN);
	}

	setkeycmd->bssid_no = bssid_no;
	setkeycmd->hw_key_idx = key->hw_key_idx;

	if (isvalid) {
		setkeycmd->alg = esp_cipher2alg(key->cipher);
		setkeycmd->keyidx = key->keyidx;
		setkeycmd->keylen = key->keylen;
		if (key->cipher == WLAN_CIPHER_SUITE_TKIP) {
			memcpy(setkeycmd->key, key->key, 16);
			memcpy(setkeycmd->key + 16, key->key + 24, 8);
			memcpy(setkeycmd->key + 24, key->key + 16, 8);
		} else {
			memcpy(setkeycmd->key, key->key, key->keylen);
		}

		setkeycmd->flags = 1;
	} else {
		setkeycmd->flags = 0;
	}
	return sip_cmd_enqueue(epub->sip, skb, ENQUEUE_PRIOR_TAIL);
}

#ifdef FPGA_LOOPBACK
#define LOOPBACK_PKT_LEN 200
int sip_send_loopback_cmd_mblk(struct esp_sip *sip)
{
	int cnt, ret;

	for (cnt = 0; cnt < 4; cnt++) {
		if (0 !=
		    (ret =
		     sip_send_loopback_mblk(sip, LOOPBACK_PKT_LEN,
					    LOOPBACK_PKT_LEN, 0)))
			return ret;
	}
	return 0;
}
#endif				/* FPGA_LOOPBACK */

int sip_send_loopback_mblk(struct esp_sip *sip, int txpacket_len,
			   int rxpacket_len, int packet_id)
{
	struct sk_buff *skb = NULL;
	struct sip_cmd_loopback *cmd;
	u8 *ptr = NULL;
	int i, ret;

	//send 100 loopback pkt
	if (txpacket_len)
		skb =
		    sip_alloc_ctrl_skbuf(sip,
					 sizeof(struct sip_cmd_loopback) +
					 sizeof(struct sip_hdr) +
					 txpacket_len, SIP_CMD_LOOPBACK);
	else
		skb =
		    sip_alloc_ctrl_skbuf(sip,
					 sizeof(struct sip_cmd_loopback) +
					 sizeof(struct sip_hdr),
					 SIP_CMD_LOOPBACK);

	if (!skb)
		return -ENOMEM;

	ptr = skb->data;
	cmd = (struct sip_cmd_loopback *) (ptr + sizeof(struct sip_hdr));
	ptr += sizeof(struct sip_hdr);
	cmd->txlen = txpacket_len;
	cmd->rxlen = rxpacket_len;
	cmd->pack_id = packet_id;

	if (txpacket_len) {
		ptr += sizeof(struct sip_cmd_loopback);
		/* fill up pkt payload */
		for (i = 0; i < txpacket_len; i++) {
			ptr[i] = i;
		}
	}

	ret = sip_cmd_enqueue(sip, skb, ENQUEUE_PRIOR_TAIL);
	if (ret < 0)
		return ret;

	return 0;
}

//remain_on_channel 
int sip_send_roc(struct esp_pub *epub, u16 center_freq, u16 duration)
{
	struct sk_buff *skb = NULL;
	struct sip_cmd_config *configcmd;

	skb =
	    sip_alloc_ctrl_skbuf(epub->sip,
				 sizeof(struct sip_cmd_config) +
				 sizeof(struct sip_hdr), SIP_CMD_CONFIG);
	if (!skb)
		return -EINVAL;

	configcmd =
	    (struct sip_cmd_config *) (skb->data + sizeof(struct sip_hdr));
	configcmd->center_freq = center_freq;
	configcmd->duration = duration;
	return sip_cmd_enqueue(epub->sip, skb, ENQUEUE_PRIOR_TAIL);
}

int sip_send_set_sta(struct esp_pub *epub, u8 ifidx, u8 set,
		     struct ieee80211_sta *sta, struct ieee80211_vif *vif,
		     u8 index)
{
	struct sk_buff *skb = NULL;
	struct sip_cmd_setsta *setstacmd;
	skb =
	    sip_alloc_ctrl_skbuf(epub->sip,
				 sizeof(struct sip_cmd_setsta) +
				 sizeof(struct sip_hdr), SIP_CMD_SETSTA);
	if (!skb)
		return -EINVAL;

	setstacmd =
	    (struct sip_cmd_setsta *) (skb->data + sizeof(struct sip_hdr));
	setstacmd->ifidx = ifidx;
	setstacmd->index = index;
	setstacmd->set = set;
	if (sta->aid == 0)
		setstacmd->aid = vif->bss_conf.aid;
	else
		setstacmd->aid = sta->aid;
	memcpy(setstacmd->mac, sta->addr, ETH_ALEN);
	if (set) {
		if (sta->ht_cap.ht_supported) {
			if (sta->ht_cap.cap & IEEE80211_HT_CAP_SGI_20)
				setstacmd->phymode =
				    ESP_IEEE80211_T_HT20_S;
			else
				setstacmd->phymode =
				    ESP_IEEE80211_T_HT20_L;
			setstacmd->ampdu_factor = sta->ht_cap.ampdu_factor;
			setstacmd->ampdu_density =
			    sta->ht_cap.ampdu_density;
		} else {
			if (sta->
			    supp_rates[NL80211_BAND_2GHZ] & (~(u32)
							       CONF_HW_BIT_RATE_11B_MASK))
			{
				setstacmd->phymode = ESP_IEEE80211_T_OFDM;
			} else {
				setstacmd->phymode = ESP_IEEE80211_T_CCK;
			}
		}
	}
	return sip_cmd_enqueue(epub->sip, skb, ENQUEUE_PRIOR_TAIL);
}

int sip_send_recalc_credit(struct esp_pub *epub)
{
	struct sk_buff *skb = NULL;

	skb =
	    sip_alloc_ctrl_skbuf(epub->sip, 0 + sizeof(struct sip_hdr),
				 SIP_CMD_RECALC_CREDIT);
	if (!skb)
		return -ENOMEM;

	return sip_cmd_enqueue(epub->sip, skb, ENQUEUE_PRIOR_HEAD);
}

int sip_cmd(struct esp_pub *epub, enum sip_cmd_id cmd_id, u8 * cmd_buf,
	    u8 cmd_len)
{
	struct sk_buff *skb = NULL;

	skb =
	    sip_alloc_ctrl_skbuf(epub->sip,
				 cmd_len + sizeof(struct sip_hdr), cmd_id);
	if (!skb)
		return -ENOMEM;

	memcpy(skb->data + sizeof(struct sip_hdr), cmd_buf, cmd_len);

	return sip_cmd_enqueue(epub->sip, skb, ENQUEUE_PRIOR_TAIL);
}
