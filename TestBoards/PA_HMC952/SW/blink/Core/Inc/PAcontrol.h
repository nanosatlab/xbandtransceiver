/*
 * PAcontrol.h
 *
 *  Created on: 11 de maig 2022
 *      Author: Guillem Gracia I Sola
 */

#ifndef INC_PACONTROL_H_
#define INC_PACONTROL_H_

// **** INCLUDES **********************************************
#include "stm32l4xx_hal.h"
#include "PA_config.h"
#include "main.h"

// **** DEFINES ********************************************
#define VDACref			3.3		// Reference voltage of the STM32 DAC
#define	DACbits			4096	// 2^bits of the STM32 DAC

#define PA_ERROR		0
#define PA_STOP			1
#define PA_START		2
#define PA_OFF			3
#define PA_ON			4
#define PA_PINCH		5
#define PA_TX			6

// **** PARAMETERS ********************************************
float voltage;

// **** FUNCTION PROTOTYPES********************************************
void PA_setGateVoltage(DAC_HandleTypeDef *hdac, float voltage);

void PA_start(DAC_HandleTypeDef *hdac);
void PA_stop(DAC_HandleTypeDef *hdac);
void PA_pinchoff(DAC_HandleTypeDef *hdac);
void PA_transmit(DAC_HandleTypeDef *hdac);

#endif /* INC_PACONTROL_H_ */
