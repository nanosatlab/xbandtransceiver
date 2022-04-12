///*  XCOMSS.c
// *
// *  Created on: 11 de nov. 2020
// *      Author: guillem
// */
//#include "XCOMMS.h"
//
//void MCU_configuration(void){
//
//}
//
//void TxPLL_INIT(SPI_HandleTypeDef *hspi){
//	PLL_Tx.frequency= 6000;			// Target frequency in MHz
//	PLL_Tx.Fosc 	= 25;				// OSCin frequency in MHz
//	PLL_Tx.pin 		= PLL_Tx_pin;
//	PLL_Tx.port		= &Tx_CE_GPIO_Port;
//	PLL_Tx.osc_2x 	= 1;
//	PLL_Tx.pll_r_pre= 1;
//	PLL_Tx.pll_r 	= 1;
//	PLL_Tx.mult 	= 1;
//	PLL_Tx.out_pwr_a= 30;				// RFoutA Power
//	PLL_Tx.out_pwr_b= 0;				// RFoutB Power
//	PLL_Tx.out_pd_a	= 0;				// On = 0, Off = 1
//	PLL_Tx.out_pd_b	= 1;				// On = 0, Off = 1
//
//	HAL_GPIO_WritePin(&PLL_Tx.port, PLL_Tx.pin, GPIO_PIN_SET);	// Tx PLL Enable
//
//	LMX2572_init(PLL_Tx, R_default);
//	LMX2572_reset(PLL_Tx, hspi);
//	LMX2572_set_fpd(PLL_Tx);
//	LMX2572_set_cpg(PLL_Tx);
//	LMX2572_set_PLL(PLL_Tx);
//	LMX2572_pwr_RFoutA(PLL_Tx);
//	LMX2572_set_RFout(PLL_Tx);
//	LMX2752_vco_assist(PLL_Tx);
//	LMX2572_load_regs(PLL_Tx, hspi);
//}
//
//void RxPLL_INIT(SPI_HandleTypeDef *hspi){
//	PLL_Rx.frequency = 6000;			// Target frequency in MHz
//	PLL_Rx.Fosc 	= 25;				// OSCin frequency in MHz
//	PLL_Rx.osc_2x 	= 1;
//	PLL_Rx.pll_r_pre= 1;
//	PLL_Rx.pll_r 	= 1;
//	PLL_Rx.mult 	= 1;
//	PLL_Rx.out_pwr_a= 30;				// RFoutA Power
//	PLL_Rx.out_pwr_b= 0;				// RFoutB Power
//	PLL_Rx.out_pd_a	= 0;				// On = 0, Off = 1
//	PLL_Rx.out_pd_b	= 1;				// On = 0, Off = 1
//
//	HAL_GPIO_WritePin(PLL_Rx_port, PLL_Rx_pin, GPIO_PIN_SET);	// Rx PLL Enable
//
//	LMX2572_init(PLL_Rx, R_default);
//	LMX2572_reset(PLL_Rx, hspi);
//	LMX2572_set_fpd(PLL_Rx);
//	LMX2572_set_cpg(PLL_Rx);
//	LMX2572_set_PLL(PLL_Rx);
//	LMX2572_pwr_RFoutA(PLL_Rx);
//	LMX2572_set_RFout(PLL_Rx);
//	LMX2752_vco_assist(PLL_Rx);
//	LMX2572_load_regs(PLL_Rx, hspi);
//}
//
//void HAL_LPTIM_TriggerCallback(LPTIM_HandleTypeDef *hlptim){
//
//}
//
//void pckt_creator(uint8_t *msg, uint8_t *data, uint8_t size, uint8_t comand){
//	//	uint8_t msg_length = msg_pckt+size;
//}
//
//void HOUSEKEEPING(UART_HandleTypeDef *huart, uint16_t *pData, uint8_t size){
//	uint8_t data[2*size];
//	uint8_t msg[msg_pckt+(2*size)];
//
//	for(uint8_t i = 0; i < size; i++){
//		data[2*i] = *(pData+i) & 0xff;
//		data[2*i+1] = *(pData+i) >> 8;
//	}
//
//	pckt_creator(msg, data, 2*size, answ_HK);
//
//	HAL_UART_Transmit(huart, msg, sizeof(msg), TIMEOUT);	//send housekeeping data
//}
