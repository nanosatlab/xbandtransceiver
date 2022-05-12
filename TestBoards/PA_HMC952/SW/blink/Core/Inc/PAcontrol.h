/*
 * PAcontrol.h
 *
 *  Created on: 11 de maig 2022
 *      Author: GGS
 */

#ifndef INC_PACONTROL_H_
#define INC_PACONTROL_H_

// **** INCLUDES **********************************************
#include "stm32l4xx_hal.h"
#include "PA_config.h"

// **** DEFINES ********************************************
#define VDACref			3.3		// Reference voltage of the STM32 DAC
#define	DACbits			4096	// 2^bits of the STM32 DAC

// **** PARAMETERS ********************************************

// **** FUNCTION PROTOTYPES********************************************
void hmc952_init(DAC_HandleTypeDef *hdac);
void hmc952_setVoltage(DAC_HandleTypeDef *hdac, float voltage);

#endif /* INC_PACONTROL_H_ */
