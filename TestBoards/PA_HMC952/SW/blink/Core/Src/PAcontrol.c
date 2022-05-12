/*
 * PAcontrol.c
 *
 *  Created on: 11 de maig 2022
 *      Author: GGS
 */

// **** INCLUDES **********************************************
#include "PAcontrol.h"

// Initializes the DAC for controlling the PA
void hmc952_init(DAC_HandleTypeDef *hdac){
	HAL_DAC_Start (hdac, DAC_CHANNEL_1);

}

// Sets DAC output voltage
void hmc952_setVoltage(DAC_HandleTypeDef *hdac, float voltage){
	uint32_t DACvalue = voltage*DACbits/VDACref;

	HAL_DAC_SetValue(hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, DACvalue);

}
