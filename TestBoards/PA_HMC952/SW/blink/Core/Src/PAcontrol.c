/*
 * PAcontrol.c
 *
 * TBD:
 * 		Drain current measurement
 * 		RF enable/disable
 *
 *  Created on: 11 de maig 2022
 *      Author: Guillem Gracia I Sola
 */

// **** INCLUDES **********************************************
#include "PAcontrol.h"

// Sets DAC output voltage
void PA_setGateVoltage(DAC_HandleTypeDef *hdac, float voltage){
	uint32_t DACvalue = voltage*DACbits/VDACref;

	HAL_DAC_SetValue(hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, DACvalue);

	HAL_Delay(50);
}

// Turns on PA in OFF mode (no transmission)
void PA_start(DAC_HandleTypeDef *hdac){
	// enable STM32 DAC
	HAL_DAC_Start (hdac, DAC_CHANNEL_1);

	// turn off the PA channel
	voltage = VPA_min;
	PA_setGateVoltage(hdac, voltage);

	// activate 6 V and 5 V PA supplies
	HAL_GPIO_WritePin(GPIOB, EN_6V_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, EN_5V_Pin, GPIO_PIN_SET);

	/*
	 * ADD PA DRAIN CURRENT MEASUREMENT AND CHECK
	 */
}

// Stop and turn off PA
void PA_stop(DAC_HandleTypeDef *hdac){
	// deactivate 6 V and 5 V PA supplies
	HAL_GPIO_WritePin(GPIOB, EN_6V_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, EN_5V_Pin, GPIO_PIN_RESET);

	// decrease PA gate voltage to turn off PA with 100 mV steps
	while(voltage < VPA_min){
		PA_setGateVoltage(hdac, voltage);
		voltage += PA_step;
	}

	HAL_DAC_Stop(hdac, DAC_CHANNEL_1);

	/*
	 * ADD PA DRAIN CURRENT MEASUREMENT AND CHECK
	 */
}

// Set PA in pinch-off conduction mode
void PA_pinchoff(DAC_HandleTypeDef *hdac){
	// increase PA gate voltage to turn PA into pinch-off conduction mode
	while(voltage >= VPA_pinchoff){
		PA_setGateVoltage(hdac, voltage);
		voltage -= PA_step;
	}

	/*
	 * ADD PA DRAIN CURRENT MEASUREMENT AND CHECK
	 */
}

void PA_transmit(DAC_HandleTypeDef *hdac){
	uint8_t Tx_good = 0;

	// increase PA gate voltage to turn PA into on conduction mode
	while(voltage >= VPA_transmit && Tx_good == 1){
		PA_setGateVoltage(hdac, voltage);
		voltage -= PA_step;

		/*
		 * ADD PA DRAIN CURRENT MEASUREMENT AND CHECK
		 */

		/*
		 * ADD PA OUTPUT MEASUREMENT AND CHECK
		 */
	}
}
