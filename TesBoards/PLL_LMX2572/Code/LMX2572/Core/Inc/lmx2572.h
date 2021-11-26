#ifndef lmx2572_H_
#define lmx2572_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"

// **************** PLL parameters ****************
// Registers Bank
static uint32_t R[126];

typedef struct PLL{
	float frequency;		// Target frequency in MHz
	float Fvco;				// Computed frequency of the VCO
	float fpd;				// Computed frequency of the PD

	float Fosc;				// OSCin frequency in MHz
	uint8_t osc_2x;			// OSC doubler
	uint8_t pll_r_pre;
	uint8_t pll_r;
	uint8_t mult;

	uint16_t pll_n;
	uint32_t pll_num;
	uint32_t pll_den;

	uint16_t CPG;
	uint8_t VCO;
	uint8_t VCO_force;
	uint8_t MASH_order;
	uint8_t PFD_DLY_SEL;

	uint8_t out_mux_a;				// CHDIV = 0, VCO = 1, HI_Z = 3
	uint8_t out_mux_b;				// VCO = 1, CHDIV = 2, HI_Z = 3
	uint8_t chdiv;

	uint8_t out_pwr_a;				// RFoutA Power
	uint8_t out_pwr_b;				// RFoutB Power
	uint8_t out_pd_a;				// On = 0, Off = 1
	uint8_t out_pd_b;				// On = 0, Off = 1
} PLL;


/* Exported functions prototypes ---------------------------------------------*/

// Functions related to PLL parameters
PLL 		LMX2572_det_param(struct PLL pll);
void 		LMX2572_LoadDefaultRegBank();

// Functions related to PLL operation
void 		LMX2572_write(SPI_HandleTypeDef *hspi, uint32_t value);
uint32_t	LMX2572_read(SPI_HandleTypeDef *hspi, uint32_t value);
void 		LMX2572_load_regs(SPI_HandleTypeDef *hspi);
void 		LMX2572_reset(SPI_HandleTypeDef *hspi);
void 		LMX2572_set_cpg(struct PLL pll);
void	 	LMX2572_set_fpd(struct PLL pll);
void 		LMX2572_set_frequency(struct PLL pll);
void 		LMX2752_vco_assist(struct PLL pll);
void 		LMX2572_calibrate_VCO(struct PLL pll);
void 		LMX2572_set_CHDIV(struct PLL pll);
void 		LMX2572_switch_RFout(struct PLL pll);
void 		LMX2572_mux_RFoutA(struct PLL pll);
void 		LMX2572_mux_RFoutB(struct PLL pll);
void 		LMX2572_pwr_RFoutA(struct PLL pll);
void 		LMX2572_pwr_RFoutB(struct PLL pll);
void 		LMX2572_set_MASH(struct PLL pll);
void 		LMX2572_on_PLL(SPI_HandleTypeDef *hspi);
void 		LMX2572_off_PLL(SPI_HandleTypeDef *hspi);


void 		LMX2572_init(struct PLL pll, uint32_t lmx2572Regs[]);

#ifdef __cplusplus
}
#endif
#endif /* LMX2572_H_ */
