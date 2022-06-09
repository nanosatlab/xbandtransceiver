/* 	*********************************
 * 	Config file for the HMC952 PA
 *
 *  Last Update: 02/12/2021
 *	Author: Guillem Gracia i Sola
 * 	*********************************
 */

#ifndef INC_PA_CONFIG_H_
#define INC_PA_CONFIG_H_

// **** DEFINES ********************************************
#define VPA_min 		2.5		// Minimum negative PA Vgate
#define VPA_max			0.8		// Maximum negative PA Vgate
#define VPA_off			1.6		// Pinch-off threshold
#define VPA_pinchoff	1.4		// Conduction threshold
#define VPA_transmit		1		// Optimal transmission

#define PA_step			0.05		// gate voltage steps

#endif /* INC_PA_CONFIG_H_ */
