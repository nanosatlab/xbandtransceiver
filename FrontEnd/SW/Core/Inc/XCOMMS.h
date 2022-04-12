/*
 * XCOMMS.h
 *
 *  Created on: 11 de nov. 2020
 *      Author: guill
 */

#ifndef INC_XCOMMS_H_
#define INC_XCOMMS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"
#include "main.h"
#include "lmx2572.h"

//state list
#define XCOMMS_INIT_state	0x01
#define IDLE_state			0x02
#define MEASUREMENT_state	0x03
#define TUNE_PLL_state		0x04
#define POWER_RF_state		0x05
#define HOUSEKEEPING_state	0x06
#define SHUTDOWN_state		0x07

//OBC command list
#define order_mcuoff	0b11111111
#define order_reset		0b11111100
#define order_report	0b11001111
#define order_switchTx	0b11001100
#define	order_switxhRx	0b11000011
#define order_tune		0b11000000

//mcu command list
#define answ_confrim	0b00000000
#define answ_error		0b00001111
#define answ_HK			0b00000011

//UART constants
#define TIMEOUT 		10
#define HK_length 		4					// total number of samples of temperature and Tx power (HK_length/2 of each)
#define msg_pckt		11					// bytes of a UART packet without data
#define PERIOD			65535				//
#define PULSE			32767
#define MEASURE_PERIOD 	5					// measurement every MEASURE_PERIOD sec

void HOUSEKEEPING(UART_HandleTypeDef *huart, uint16_t *pData, uint8_t size);
void TxPLL_INIT(SPI_HandleTypeDef *hspi);
void RxPLL_INIT(SPI_HandleTypeDef *hspi);
void MCU_configuration(void);
void state_machine(void);
uint8_t UART_decode();

#ifdef __cplusplus
}
#endif

#endif /* INC_XCOMMS_H_ */
