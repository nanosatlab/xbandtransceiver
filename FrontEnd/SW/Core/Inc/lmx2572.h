#ifndef lmx2572_H_
#define lmx2572_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"

static uint32_t R_default[] = {
		0x7D2288	,
		0x7C0000	,
		0x7B0000	,
		0x7A0000	,
		0x790000	,
		0x780000	,
		0x770000	,
		0x760000	,
		0x750000	,
		0x740000	,
		0x730000	,
		0x727802	,
		0x710000	,
		0x700000	,
		0x6F0000	,
		0x6E0000	,
		0x6D0000	,
		0x6C0000	,
		0x6B0000	,
		0x6A0007	,
		0x694440	,
		0x680000	,
		0x670000	,
		0x660000	,
		0x650000	,
		0x640000	,
		0x630000	,
		0x620000	,
		0x610000	,
		0x600000	,
		0x5F0000	,
		0x5E0000	,
		0x5D0000	,
		0x5C0000	,
		0x5B0000	,
		0x5A0000	,
		0x590000	,
		0x580000	,
		0x570000	,
		0x560000	,
		0x550000	,
		0x540000	,
		0x530000	,
		0x520000	,
		0x510000	,
		0x500000	,
		0x4F0000	,
		0x4E00CF	,
		0x4D0000	,
		0x4C000C	,
		0x4B0800	,
		0x4A0000	,
		0x49003F	,
		0x480001	,
		0x470081	,
		0x46C350	,
		0x450000	,
		0x4403E8	,
		0x430000	,
		0x4201F4	,
		0x410000	,
		0x401388	,
		0x3F0000	,
		0x3E00AF	,
		0x3D00A8	,
		0x3C03E8	,
		0x3B0001	,
		0x3A9001	,
		0x390020	,
		0x380000	,
		0x370000	,
		0x360000	,
		0x350000	,
		0x340421	,
		0x330080	,
		0x320080	,
		0x314180	,
		0x3003E0	,
		0x2F0300	,
		0x2E07F0	,
		0x2DCE1F	,
		0x2C1FA3	,
		0x2B0334	,
		0x2A0000	,
		0x290000	,
		0x280000	,
		0x2703E8	,
		0x260000	,
		0x250305	,
		0x24003B	,
		0x230004	,
		0x220010	,
		0x211E01	,
		0x2005BF	,
		0x1FC3E6	,
		0x1E18A6	,
		0x1D0000	,
		0x1C0488	,
		0x1B0002	,
		0x1A0808	,
		0x190624	,
		0x18071A	,
		0x17007C	,
		0x160001	,
		0x150409	,
		0x147048	,
		0x1327B7	,
		0x120064	,
		0x1100EC	,
		0x100080	,
		0x0F060E	,
		0x0E1820	,
		0x0D4000	,
		0x0C5001	,
		0x0BB018	,
		0x0A10F8	,
		0x090004	,
		0x082000	,
		0x0700B2	,
		0x06C802	,
		0x0530C8	,
		0x040A43	,
		0x030782	,
		0x020500	,
		0x010808	,
		0x00219C
};

// **************** PLL parameters ****************
// Registers Bank
struct PLL{
	uint32_t R[126];

	float frequency;			// Target frequency in MHz
	float Fvco;
	float fpd;
	uint16_t pin;
	GPIO_TypeDef * port;

	float Fosc;				// OSCin frequency in MHz
	uint8_t osc_2x;
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

};

/* Exported functions prototypes ---------------------------------------------*/
void 		LMX2572_det_param(struct PLL pll);
void 		LMX2572_write(struct PLL pll, SPI_HandleTypeDef *hspi, uint32_t value);
uint32_t	LMX2572_read(struct PLL pll, SPI_HandleTypeDef *hspi, uint32_t value);
void 		LMX2572_load_regs(struct PLL pll, SPI_HandleTypeDef *hspi);
void 		LMX2572_reset(struct PLL pll, SPI_HandleTypeDef *hspi);
void 		LMX2572_default(struct PLL pll, uint32_t lmx2572Regs[]);
void 		LMX2572_init(struct PLL pll, uint32_t lmx2572Regs[]);
void 		LMX2572_set_cpg(struct PLL pll);
void	 	LMX2572_set_fpd(struct PLL pll);
void 		LMX2572_set_PLL(struct PLL pll);
void 		LMX2752_vco_assist(struct PLL pll);
void 		LMX2572_calibrate_VCO(struct PLL pll);
void 		LMX2572_set_CHDIV(struct PLL pll);
void 		LMX2572_set_RFout(struct PLL pll);
void 		LMX2572_mux_RFoutA(struct PLL pll);
void 		LMX2572_mux_RFoutB(struct PLL pll);
void 		LMX2572_pwr_RFoutA(struct PLL pll);
void 		LMX2572_pwr_RFoutB(struct PLL pll);
void 		LMX2572_set_MASH(struct PLL pll);

#ifdef __cplusplus
}
#endif
#endif /* LMX2572_H_ */
