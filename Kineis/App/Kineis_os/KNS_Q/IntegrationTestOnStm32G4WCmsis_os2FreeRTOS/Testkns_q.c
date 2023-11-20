// SPDX-License-Identifier: no SPDX license
/**
 * @file    Testkns_q.c
 * @brief   Test KND queues utilities compiled with FreeRTOS
 * @author  William Begoc
 * @date    creation 2022/08/01
 * @version 1.0
 * @note
 */

/**
 * @addtogroup INTEGRATION_TEST_KNS_Q_CMSIS_OS2
 * @{
 * For static test vectors, following configuration is used to fix size of the 2 queues:
 * * SERVICE-to-PROFILE low_priority queues: length 3, element size 30 bytes
 * * DRIVER-to-SERVICE higher-priority queue:  length 3, element size 16bytes
 *
 * Cf in kns_q_conf.h:
 * * KNS_Q_DL_APP2MAC_LEN           3
 * * KNS_Q_DL_APP2MAC_ITEM_BYTESIZE 30
 * * KNS_Q_UL_SRVC2MAC_LEN            3
 * * KNS_Q_UL_SRVC2MAC_ITEM_BYTESIZE  16
 */
#include "unity.h"
#include "string.h"
#include "stm_hw_conf.h"

#include "kineis_sw_conf.h"
#include "kns_q_conf.h"
#include "kns_q.h"
#include "mgr_log.h"

/* Externs ------------------------------------------------------------------------------------- */

/* Structures ---------------------------------------------------------------------------------- */
enum RF_event_t {
	TCXO_READY,   /**< TCXO warmup complete */
	TX_DONE,      /**< TX completed */
	RX_RECEIVED,  /**< RX frame received */
	SAT_DETECTED, /**< SATellite detection complete */
	RF_ABORTED    /**< RF action fully aborted */
} __attribute__((packed));

struct RF_ctxt_tx_done_t {
	uint8_t tx_data_size;
	uint8_t tx_buf_elt[20];
} __attribute__((packed));

struct RF_ctxt_rx_received_t {
	uint8_t rx_data_size;
	uint8_t rx_buf_elt[10];
} __attribute__((packed));

struct RF_evt_t {
	enum RF_event_t  evt;
	union {
		struct RF_ctxt_tx_done_t tx_ctxt;
		struct RF_ctxt_rx_received_t rx_ctxt;
		uint32_t sat_id; // fake definition for detected SAT on 4 bytes
	};
} __attribute__((packed));

/* Variables ----------------------------------------------------------------------------------- */
/**
 * @attention Push and Pop service will copy data which is longer than RF events below, depending
 * on the size of each element of the queues. Here, RF_evt are mapped as global variables,
 * expecting then to be mapped one just after the other, in a way to control the memory data
 * between variables.
 *
 * Actually, this is compiler/linker dependent. Depending on it, tests below may fail when checking
 * popped item full content. Refer to TEST_ASSERT_EQUAL_UINT8_ARRAY below.
 */
struct RF_evt_t RF_evt_1 = {
		.evt = TX_DONE,
		.tx_ctxt = {
				.tx_data_size = 20
		}
};

struct RF_evt_t RF_evt_2 = {
		.evt = TX_DONE,
		.tx_ctxt = {
				.tx_data_size = 10
		}
};

struct RF_evt_t RF_evt_3 = {
		.evt = RX_RECEIVED,
		.rx_ctxt = {
				.rx_data_size = 6
		}
};

struct RF_evt_t RF_evt_4 = {
		.evt = SAT_DETECTED,
		.sat_id = 0xBEA27189
};

uint8_t txPayload1[20] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
		0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0X12, 0x13 };
uint8_t txPayload2[10] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00};
uint8_t rxPayload1[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xB1, 0x65};

/* Override functions -------------------------------------------------------------------------- */

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 *
 * Such function is present in generated code from STM32Cube
 *
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	MGR_LOG_DEBUG("ASSERT FAIL: %u %s\r\n", line, file);
	TEST_FAIL();
	/* USER CODE END 6 */
}

/* main test functions ------------------------------------------------------------------------- */

void setUp(void)
{
	/* ran before each unit test */
	stm_hw_conf();

	/* @attention: no re-init of the queues between tests below. They are written to be
	 * executed in the coding order
	 */
}

void tearDown(void)
{
	/* ran after each unit test */
}

/** @brief Test pushing/popping in high-priority queue.
 *
 * This test is also checking one event (RF_evt_1) being pushed in a queue where element's size is
 * too short. Indeed, KNS queues must be well dimensioned, to handle longest-event's size.
 *
 * @note RF_evt 1 will be longer than SRVC2MAC's queue elt size (16).
 * Only first 16 bytes will be copied
 */
void test_KNS_Q_FREERTOS_push_pop_in_higher_prio_queue(void)
{
	uint8_t popItem[KNS_Q_UL_SRVC2MAC_ITEM_BYTESIZE];

	/** Init content of RF events
	 *
	 * @note RF_evt 1 will be longer than SRVC2MAC's queue elt size (16)
	 * Only first 16 bytes will be copied
	 */
	memcpy(RF_evt_1.tx_ctxt.tx_buf_elt, txPayload1, sizeof(txPayload1));
	memcpy(RF_evt_2.tx_ctxt.tx_buf_elt, txPayload2, sizeof(txPayload2));
	memcpy(RF_evt_3.rx_ctxt.rx_buf_elt, rxPayload1, sizeof(rxPayload1));

	TEST_ASSERT_TRUE(KNS_Q_create(KNS_Q_UL_SRVC2MAC, KNS_Q_UL_SRVC2MAC_LEN,
		KNS_Q_UL_SRVC2MAC_ITEM_BYTESIZE) == KNS_STATUS_OK);

	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_UL_SRVC2MAC, (void *)&RF_evt_1) == KNS_STATUS_OK);
	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_UL_SRVC2MAC, (void *)&RF_evt_2) == KNS_STATUS_OK);
	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_UL_SRVC2MAC, (void *)&RF_evt_3) == KNS_STATUS_OK);
	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_UL_SRVC2MAC, (void *)&RF_evt_4) == KNS_STATUS_ERROR);

	/** Pop first element and check integrity
	 */
	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_UL_SRVC2MAC, (void *)popItem) == KNS_STATUS_OK);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&RF_evt_1, popItem, KNS_Q_UL_SRVC2MAC_ITEM_BYTESIZE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->evt == TX_DONE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->tx_ctxt.tx_data_size == 20);
	/** @note first RF event's length was longer than queue's element size. Only 14 first bytes
	 * of data buf should have been handled by KNS_Q_push and KNS_Q_pop.
	 *
	 * @note As this is sensitive to compiler packing/alignment strategy, you may need to
	 * align thanks to your compiler behavior
	 */
	TEST_ASSERT_EQUAL_UINT8_ARRAY(((struct RF_evt_t *)popItem)->tx_ctxt.tx_buf_elt,
		txPayload1, KNS_Q_UL_SRVC2MAC_ITEM_BYTESIZE - 2);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->tx_ctxt.tx_buf_elt[13] == txPayload1[13]);
	TEST_ASSERT_FALSE(((struct RF_evt_t *)popItem)->tx_ctxt.tx_buf_elt[14] == txPayload1[14]);

	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_UL_SRVC2MAC, (void *)&RF_evt_4) == KNS_STATUS_OK);

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_UL_SRVC2MAC, (void *)popItem) == KNS_STATUS_OK);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&RF_evt_2, popItem, KNS_Q_UL_SRVC2MAC_ITEM_BYTESIZE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->evt == TX_DONE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->tx_ctxt.tx_data_size == 10);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(((struct RF_evt_t *)popItem)->tx_ctxt.tx_buf_elt,
		txPayload2, sizeof(txPayload2));

	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_UL_SRVC2MAC, (void *)&RF_evt_3) == KNS_STATUS_OK);

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_UL_SRVC2MAC, (void *)popItem) == KNS_STATUS_OK);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&RF_evt_3, popItem, KNS_Q_UL_SRVC2MAC_ITEM_BYTESIZE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->evt == RX_RECEIVED);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->tx_ctxt.tx_data_size == 6);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(((struct RF_evt_t *)popItem)->tx_ctxt.tx_buf_elt,
		rxPayload1, sizeof(rxPayload1));

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_UL_SRVC2MAC, (void *)popItem) == KNS_STATUS_OK);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&RF_evt_4, popItem, KNS_Q_UL_SRVC2MAC_ITEM_BYTESIZE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->evt == SAT_DETECTED);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->sat_id == 0xBEA27189);

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_UL_SRVC2MAC, (void *)popItem) == KNS_STATUS_OK);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&RF_evt_3, popItem, KNS_Q_UL_SRVC2MAC_ITEM_BYTESIZE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->evt == RX_RECEIVED);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->tx_ctxt.tx_data_size == 6);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(((struct RF_evt_t *)popItem)->tx_ctxt.tx_buf_elt,
		rxPayload1, sizeof(rxPayload1));

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_UL_SRVC2MAC, (void *)popItem) == KNS_STATUS_ERROR);

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_UL_SRVC2MAC, (void *)popItem) == KNS_STATUS_ERROR);

	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_UL_SRVC2MAC, (void *)&RF_evt_4) == KNS_STATUS_OK);

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_UL_SRVC2MAC, (void *)popItem) == KNS_STATUS_OK);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&RF_evt_4, popItem, KNS_Q_UL_SRVC2MAC_ITEM_BYTESIZE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->evt == SAT_DETECTED);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->sat_id == 0xBEA27189);
}

/** Test pushing/popping in lower priority queue.
 *
 * @note: RF_evt above are also posted in this queue for convenience, avoiding to code extra events
 */
void test_KNS_Q_FREERTOS_push_pop_in_lower_prio_queue(void)
{
	uint8_t popItem[KNS_Q_DL_APP2MAC_ITEM_BYTESIZE];

	/** Init content of RF events
	 */
	memcpy(RF_evt_1.tx_ctxt.tx_buf_elt, txPayload1, sizeof(txPayload1));
	memcpy(RF_evt_2.tx_ctxt.tx_buf_elt, txPayload2, sizeof(txPayload2));
	memcpy(RF_evt_3.rx_ctxt.rx_buf_elt, rxPayload1, sizeof(rxPayload1));

	TEST_ASSERT_TRUE(KNS_Q_create(KNS_Q_DL_APP2MAC, KNS_Q_DL_APP2MAC_LEN,
		KNS_Q_DL_APP2MAC_ITEM_BYTESIZE) == KNS_STATUS_OK);

	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&RF_evt_1) == KNS_STATUS_OK);
	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&RF_evt_2) == KNS_STATUS_OK);
	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&RF_evt_3) == KNS_STATUS_OK);
	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&RF_evt_4) == KNS_STATUS_ERROR);

	/** Pop first element and check integrity
	 */
	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_DL_APP2MAC, (void *)popItem) == KNS_STATUS_OK);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&RF_evt_1, popItem, KNS_Q_DL_APP2MAC_ITEM_BYTESIZE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->evt == TX_DONE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->tx_ctxt.tx_data_size == 20);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(((struct RF_evt_t *)popItem)->tx_ctxt.tx_buf_elt,
		txPayload1, sizeof(txPayload1));

	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&RF_evt_4) == KNS_STATUS_OK);

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_DL_APP2MAC, (void *)popItem) == KNS_STATUS_OK);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&RF_evt_2, popItem, KNS_Q_DL_APP2MAC_ITEM_BYTESIZE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->evt == TX_DONE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->tx_ctxt.tx_data_size == 10);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(((struct RF_evt_t *)popItem)->tx_ctxt.tx_buf_elt,
		txPayload2, sizeof(txPayload2));

	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&RF_evt_3) == KNS_STATUS_OK);

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_DL_APP2MAC, (void *)popItem) == KNS_STATUS_OK);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&RF_evt_3, popItem, KNS_Q_DL_APP2MAC_ITEM_BYTESIZE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->evt == RX_RECEIVED);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->tx_ctxt.tx_data_size == 6);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(((struct RF_evt_t *)popItem)->tx_ctxt.tx_buf_elt,
		rxPayload1, sizeof(rxPayload1));

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_DL_APP2MAC, (void *)popItem) == KNS_STATUS_OK);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&RF_evt_4, popItem, KNS_Q_DL_APP2MAC_ITEM_BYTESIZE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->evt == SAT_DETECTED);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->sat_id == 0xBEA27189);

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_DL_APP2MAC, (void *)popItem) == KNS_STATUS_OK);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&RF_evt_3, popItem, KNS_Q_DL_APP2MAC_ITEM_BYTESIZE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->evt == RX_RECEIVED);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->tx_ctxt.tx_data_size == 6);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(((struct RF_evt_t *)popItem)->tx_ctxt.tx_buf_elt,
		rxPayload1, sizeof(rxPayload1));

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_DL_APP2MAC, (void *)popItem) == KNS_STATUS_ERROR);

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_DL_APP2MAC, (void *)popItem) == KNS_STATUS_ERROR);

	TEST_ASSERT_TRUE(KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&RF_evt_4) == KNS_STATUS_OK);

	TEST_ASSERT_TRUE(KNS_Q_pop(KNS_Q_DL_APP2MAC, (void *)popItem) == KNS_STATUS_OK);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&RF_evt_4, popItem, KNS_Q_DL_APP2MAC_ITEM_BYTESIZE);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->evt == SAT_DETECTED);
	TEST_ASSERT_TRUE(((struct RF_evt_t *)popItem)->sat_id == 0xBEA27189);
}

/**
 * @}
 */
