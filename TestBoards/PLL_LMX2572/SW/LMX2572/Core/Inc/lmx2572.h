#ifndef lmx2572_H_
#define lmx2572_H_

#ifdef __cplusplus
extern "C" {
#endif

// **** INCLUDES **********************************************
#include "stm32l4xx_hal.h"
#include "lmx2572_configs.h"

// **** DEFINES ********************************************
#define PLL_DEFAULT_frequency	5000
#define PLL_DEFAULT_power		-10
#define PLL_DEFAULT_fosc		25
#define PLL_DEFAULT_doubler 	1
#define PLL_DEFAULT_r_pre		1
#define PLL_DEFAULT_r			1
#define PLL_DEFAULT_mult		2
#define PLL_DEFAULT_CPG			2500
#define PLL_DEFAULT_VCO			4
#define PLL_DEFAULT_VCO_force	0
#define PLL_DEFAULT_MASH_order	3
#define PLL_DEFAULT_PFD_DLY_SEL	3
#define PLL_DEFAULT_chdiv		2
#define PLL_DEFAULT_out_mux_a	1
#define PLL_DEFAULT_out_mux_b	1
#define PLL_DEFAULT_out_pd_a	1
#define PLL_DEFAULT_out_pd_b	1
#define PLL_DEFAULT_out_pwr_a	0
#define PLL_DEFAULT_out_pwr_b	0

// **** PARAMETERS ********************************************
static uint32_t R[126];		// Registers Bank

typedef struct PLL{
	float frequency;		// Target frequency in MHz
	int power;				// Output power level in dBm
	float Fvco;				// Computed frequency of the VCO
	float fpd;				// Computed frequency of the PD

	float fosc;				// OSCin frequency in MHz
	uint8_t doubler;			// OSC doubler
	uint8_t r_pre;
	uint8_t r;
	uint8_t mult;
	uint16_t n;
	uint32_t num;
	uint32_t den;

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

// **** FUNCTION PROTOTYPES********************************************
PLL 		LMX2572_det_param(struct PLL pll);
PLL			LMX2572_defaultConfig(struct PLL pll);

void 		LMX2572_write(SPI_HandleTypeDef *hspi, uint32_t value);
uint32_t	LMX2572_read(SPI_HandleTypeDef *hspi, uint32_t value);
void 		LMX2572_load_regs(SPI_HandleTypeDef *hspi);
void 		LMX2572_reset(SPI_HandleTypeDef *hspi);

void 		LMX2572_set_cpg(struct PLL pll);
void	 	LMX2572_set_fpd(struct PLL pll);
void 		LMX2572_set_frequency(struct PLL pll);
void 		LMX2752_vco_assist(struct PLL pll);
void 		LMX2572_calibrate(struct PLL pll);
void 		LMX2572_set_CHDIV(struct PLL pll);
void 		LMX2572_set_MASH(struct PLL pll);

void 		LMX2572_on_PLL(SPI_HandleTypeDef *hspi);
void 		LMX2572_off_PLL(SPI_HandleTypeDef *hspi);
void 		LMX2572_switchOn_RFoutA(struct PLL pll);
void 		LMX2572_switchOff_RFoutA(struct PLL pll);
void 		LMX2572_switchOn_RFoutB(struct PLL pll);
void 		LMX2572_switchOff_RFoutB(struct PLL pll);
void 		LMX2572_mux_RFoutA(struct PLL pll);
void 		LMX2572_mux_RFoutB(struct PLL pll);
void 		LMX2572_pwr_RFoutA(struct PLL pll);
void 		LMX2572_pwr_RFoutB(struct PLL pll);

PLL 		LMX2572_init(struct PLL pll, SPI_HandleTypeDef *hspi);
PLL 		LMX2572_frequency(struct PLL pll, SPI_HandleTypeDef *hspi, float frequency);
PLL			LMX2572_powerA(struct PLL pll, SPI_HandleTypeDef *hspi, int power);

#ifdef __cplusplus
}
#endif
#endif /* LMX2572_H_ */
