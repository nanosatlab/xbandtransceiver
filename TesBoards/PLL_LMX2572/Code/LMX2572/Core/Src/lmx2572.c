/* 	*************** README *************
 * 	This driver includes the needed register programming for using the PLL of the LMX2572,
 * 	some configurations have not been implemented since from default are deactivated.
 *
 * 	Implemented: User control registers, PLL, VCO Partial Assist, outputs, CPG
 *
 * 	To implement: FSK, Ramping, Burst Mode, SYSREF Divider, MASH and Phase Synchronization
 *
 *  Last Update: 18/11/2021
 *	Author: Guillem Gracia i Sola
 * 	************************************
 */

#include "lmx2572.h"
#include "lmx2572_configs.h"


//***********************
//***** This Works! *****
//***********************

// Writes a register to the PLL
void LMX2572_write(SPI_HandleTypeDef *hspi, uint32_t value) {
	uint8_t spi_buf[3] = {0,0,0};

	spi_buf[2] = value;
	spi_buf[1] = value >> 8;
	spi_buf[0] = value >> 16;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);	// CSB LOW
	HAL_SPI_Transmit(hspi, spi_buf, 3, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);		// CSB HIGH
}

// Loads the registers to the PLL
void LMX2572_load_regs(SPI_HandleTypeDef *hspi){
	int i = 0;
	while ((R[125-i] & 0x00FF0000) != 0) {
		LMX2572_write(hspi, R[125-i]);
		i++;
	}
	LMX2572_write(hspi, R[125-i]);
}

// Resets the PLL to silicon default values
void LMX2572_reset(SPI_HandleTypeDef *hspi){
	uint8_t spi_buf[3] = {0,0,0};
	spi_buf[2] = 0x1E;
	spi_buf[1] = 0x21;
	spi_buf[0] = 0x00;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);	// CSB LOW
	HAL_SPI_Transmit(hspi, spi_buf, 3, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);		// CSB HIGH

	HAL_Delay(500);
}

// Sets pfd frequency
void LMX2572_set_fpd(struct PLL pll) {
	// Set Doubler
	if (pll.doubler == 2) R[9] |= (0x01 << 12);
	else 			 	  R[9] &= ~(0x01 << 12);

	// Set Pre R
	R[12] &= ~(0xFFF << 0);
	R[12] |= ((pll.r_pre & 0xFFF) << 0);

	// Set Multiplier
	R[10] &= ~(0x1F << 7);
	R[10] |= ((pll.mult & 0x1F) << 7);

	// Set R
	R[11] &= ~(0xFF << 4);
	R[11] |= ((pll.r & 0xFF) << 4);

	if (pll.fosc * pll.doubler / pll.r_pre * pll.mult > 100) R[9] |= (0x01 << 14);
	else R[9] &= ~(0x01 << 14);
}

// Determine N, NUM and DEN values from target frequency and fpd
PLL LMX2572_det_param(struct PLL pll){
	pll.fpd = pll.fosc*pll.doubler/pll.r_pre* pll.mult/pll.r;
	pll.n = pll.frequency/pll.fpd;
	pll.den = 1000;
	pll.num = (pll.frequency/pll.fpd-pll.n)*pll.den;
	pll.Fvco = pll.fpd * pll.n + (pll.num * pll.fpd / pll.den);

	return pll;
}

// Loads PLL default registers to the PLL register bank
PLL LMX2572_defaultConfig(struct PLL pll){
	for(int i=0; i<126; i++) R[i] = R_default[125-i];

	pll.frequency = PLL_DEFAULT_frequency;
	pll.fosc = PLL_DEFAULT_fosc;
	pll.doubler = PLL_DEFAULT_doubler;
	pll.r_pre = PLL_DEFAULT_r_pre;
	pll.r = PLL_DEFAULT_r;
	pll.mult = PLL_DEFAULT_mult;
	pll.CPG = PLL_DEFAULT_CPG;
	pll.VCO = PLL_DEFAULT_VCO;
	pll.VCO_force = PLL_DEFAULT_VCO_force;
	pll.MASH_order = PLL_DEFAULT_MASH_order;
	pll.PFD_DLY_SEL = PLL_DEFAULT_PFD_DLY_SEL;
	pll.chdiv = PLL_DEFAULT_chdiv;
	pll.out_mux_a = PLL_DEFAULT_out_mux_a;
	pll.out_mux_b = PLL_DEFAULT_out_mux_b;
	pll.out_pd_a = PLL_DEFAULT_out_pd_a;
	pll.out_pd_b = PLL_DEFAULT_out_pd_b;
	pll.out_pwr_a = PLL_DEFAULT_out_pwr_a;
	pll.out_pwr_b = PLL_DEFAULT_out_pwr_b;

	return pll;
}

// Powers Down the PLL
void LMX2572_off_PLL(SPI_HandleTypeDef *hspi){
	R[0] |= (0x01 << 0);

	uint8_t spi_buf[3] = {0,0,0};
	spi_buf[2] = R[0];
	spi_buf[1] = R[0] >> 8;
	spi_buf[0] = R[0] >> 16;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);	// CSB LOW
	HAL_SPI_Transmit(hspi, spi_buf, 3, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);		// CSB HIGH
}

// Powers Up the PLL
void LMX2572_on_PLL(SPI_HandleTypeDef *hspi){
	R[0] &= ~(0x01 << 0);

	uint8_t spi_buf[3] = {0,0,0};
	spi_buf[2] = R[0];
	spi_buf[1] = R[0] >> 8;
	spi_buf[0] = R[0] >> 16;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);	// CSB LOW
	HAL_SPI_Transmit(hspi, spi_buf, 3, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);		// CSB HIGH
}

// Sets PLL N, DEN and NUM registers
void LMX2572_set_frequency(struct PLL pll) {
	// Sets N
	R[34] &= ~(0x07 << 0);
	R[34] |= (((pll.n >> 16) & 0x07) << 0);
	R[36] &= ~(0xFFFF << 0);
	R[36] |= ((pll.n & 0xFFFF) << 0);

	// Sets DEN
	R[38] &= ~(0xFFFF << 0);
	R[38] |= ((pll.den >> 16) << 0);
	R[39] &= ~(0xFFFF << 0);
	R[39] |= ((pll.den & 0x0000FFFF) << 0);

	// Sets NUM
	R[42] &= ~(0xFFFF << 0);
	R[42] |= ((pll.num >> 16) << 0);
	R[43] &= ~(0xFFFF << 0);
	R[43] |= ((pll.num & 0x0000FFFF) << 0);
}

// Turns on RFout A output
void LMX2572_switchOn_RFoutA(struct PLL pll){
	R[44] &= ~(0x01 << 6);
	pll.out_pd_a = 0;
}

// Turns off RFout A output
void LMX2572_switchOff_RFoutA(struct PLL pll){
	R[44] |= (0x01 << 6);
	pll.out_pd_a = 1;
}

// Turns on RFout B output
void LMX2572_switchOn_RFoutB(struct PLL pll){
	R[44] &= ~(0x01 << 7);
	pll.out_pd_b = 0;
}

// Turns off RFout B output
void LMX2572_switchOff_RFoutB(struct PLL pll){
	R[44] |= (0x01 << 7);
	pll.out_pd_b = 1;
}

// Sets RFoutA output power
void LMX2572_pwr_RFoutA(struct PLL pll){
	R[44] &= ~(0x3F << 8);
	R[44] |= (pll.out_pwr_a << 8);
}

// Sets RFoutB output power
void LMX2572_pwr_RFoutB(struct PLL pll){
	R[45] &= ~(0x3F << 0);
	R[45] |= (pll.out_pwr_b << 0);
}


// Sets output A Mux: CHDIV = 0, VCO = 1, HI_Z = 3
void LMX2572_mux_RFoutA(struct PLL pll){
	R[45] &= ~(0x03 <<11);
	R[45] |= (pll.out_mux_a << 11);
}

// Sets output B Mux: CHDIV = 0, VCO = 1, HI_Z = 3
void LMX2572_mux_RFoutB(struct PLL pll){
	R[46] &= ~(0x03 << 0);
	R[46] |= (pll.out_mux_b << 0);
}

// Sets CHDIV
void LMX2572_set_CHDIV(struct PLL pll){
	R[75] &= ~(0x1F << 6);
	if		(pll.chdiv == 2)	R[75] |= (0 << 6);
	else if (pll.chdiv == 4)	R[75] |= (1 << 6);
	else if (pll.chdiv == 8)	R[75] |= (3 << 6);
	else if (pll.chdiv == 16)	R[75] |= (5 << 6);
	else if (pll.chdiv == 32)	R[75] |= (7 << 6);
	else if (pll.chdiv == 64)	R[75] |= (9 << 6);
	else if (pll.chdiv == 128)	R[75] |= (12 << 6);
	else if (pll.chdiv == 256)	R[75] |= (14 << 6);
}


//************************
//***** TO BE TESTED *****
//************************

// Reads a register value from the PLL
uint32_t LMX2572_read(SPI_HandleTypeDef *hspi, uint32_t value) {
	uint32_t read = 0;
	uint8_t read_value[2] = {0x00, 0x00};
	uint8_t spi_buf[3] = {0,0,0};
	spi_buf[2] = value | (1<<7);
	spi_buf[1] = value >> 8;
	spi_buf[0] = value >> 16;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);	// CSB LOW
	HAL_SPI_Transmit(hspi, spi_buf, 3, 100);
	HAL_SPI_Receive(hspi, read_value, 2, 10);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);		// CSB HIGH

	read += read_value[0];
	read += read_value[1] >> 8;
	return read;
}

// Sets Charge Pump Gain in uA
void LMX2572_set_cpg(struct PLL pll) {
	R[14] &= 0xFF1800;
	if (pll.CPG == 0) 			R[14] |= 0x0000;		// CPG = 0
	else if (pll.CPG == 625) 	R[14] |= 0x0008;		// CPG = 1
	else if (pll.CPG == 1250) 	R[14] |= 0x0010;		// CPG = 2
	else if (pll.CPG == 1875) 	R[14] |= 0x0018;		// CPG = 3
	else if (pll.CPG == 2500) 	R[14] |= 0x0020;		// CPG = 4
	else if (pll.CPG == 3125) 	R[14] |= 0x0028;		// CPG = 5
	else if (pll.CPG == 3750) 	R[14] |= 0x0030;		// CPG = 6
	else if (pll.CPG == 4375) 	R[14] |= 0x0038;		// CPG = 7
	else if (pll.CPG == 5000) 	R[14] |= 0x0060;		// CPG = 12
	else if (pll.CPG == 5625) 	R[14] |= 0x0068;		// CPG = 13
	else if (pll.CPG == 6250) 	R[14] |= 0x0070;		// CPG = 14
	else if (pll.CPG == 6875) 	R[14] |= 0x0078;		// CPG = 15
}

// Sets VCO Partial Assist
void LMX2752_vco_assist(struct PLL pll){
	uint8_t VCO_CAPCTRL_STRT;
	uint8_t VCO_DACISET_STRT;
	uint16_t fMin;
	uint16_t fMax;
	uint16_t CMin;
	uint16_t CMax;
	uint16_t AMin;
	uint16_t AMax;

	// Select VCO
	if	((pll.Fvco > 3200) || (pll.Fvco < 3650)) {
		pll.VCO  = 1;				// VCO1
		fMin = 3200;
		fMax = 3650;
		CMin = 131;
		CMax = 19;
		AMin = 138;
		AMax = 137;
	} else if ((pll.Fvco >= 3650) || (pll.Fvco < 4200)) {
		pll.VCO  = 2;	// VCO2
		fMin = 3650;
		fMax = 4200;
		CMin = 143;
		CMax = 25;
		AMin = 162;
		AMax = 142;
	} else if ((pll.Fvco >= 4200) || (pll.Fvco < 4650)) {
		pll.VCO  = 3;	// VCO3
		fMin = 4200;
		fMax = 4650;
		CMin = 135;
		CMax = 34;
		AMin = 126;
		AMax = 114;
	} else if ((pll.Fvco >= 4650) || (pll.Fvco < 5200)) {
		pll.VCO  = 4;	// VCO4
		fMin = 4650;
		fMax = 5200;
		CMin = 136;
		CMax = 25;
		AMin = 195;
		AMax = 172;
	} else if ((pll.Fvco >= 5200) || (pll.Fvco < 5750)) {
		pll.VCO  = 5;	// VCO5
		fMin = 5200;
		fMax = 5750;
		CMin = 133;
		CMax = 20;
		AMin = 190;
		AMax = 163;
	} else if ((pll.Fvco >= 5750) || (pll.Fvco <= 6400)) {
		pll.VCO  = 6;	// VCO6
		fMin = 5750;
		fMax = 6400;
		CMin = 151;
		CMax = 27;
		AMin = 256;
		AMax = 204;
	}

	R[20] &= ~(0xF << 10);
	R[20] |= (pll.VCO << 11);
	// Forces the VCO to use the core specified by VCO_SEL
	if (pll.VCO_force == 1)	R[20] |= (1 << 10);

	// VCO_CAPCTRL_STRT = Round[CMin – (fVCO – fMin) × (CMin – CMax) / (fMax – fMin)]
	VCO_CAPCTRL_STRT = CMin-(pll.Fvco-fMin)*(CMin-CMax)/(fMax-fMin);
	R[78] &= ~(0xFF << 1);
	R[78] |= (VCO_CAPCTRL_STRT << 1);

	// VCO_DACISET_STRT = Round[AMin – (fVCO – fMin) × (AMin – AMax) / (fMax – fMin)]
	VCO_DACISET_STRT = AMin-(pll.Fvco-fMin)*(AMin-AMax)/(fMax-fMin);
	R[17] &= ~(0xFF << 0);
	R[17] |= (VCO_DACISET_STRT << 0);
}

// Calibrates VCO
void LMX2572_calibrate_VCO(struct PLL pll) {
	R[0] |= (0x01 << 3);
}

void LMX2572_set_MASH(struct PLL pll){
	R[44] &= ~(0x27 << 0);
	R[44] = (pll.MASH_order << 0);
	if (pll.num != 0) R[44] |= (0x01 << 5);

	R[37] &= ~(0x3F << 8);
	R[37] |= (pll.PFD_DLY_SEL << 8);
}

/*// Inits LMX2572
void LMX2572_init(struct PLL pll, uint32_t lmx2572Regs[]) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);		// CSB HIGH
	LMX2572_default();
	LMX2572_det_param(pll);

	R[44] |= (0x01 << 6);
	R[44] |= (0x01 << 7);
}
*/
