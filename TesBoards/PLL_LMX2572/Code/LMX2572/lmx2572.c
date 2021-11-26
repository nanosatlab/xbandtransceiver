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

//***********************
//***** This Works! *****
//***********************

// Writes a register to the PLL
void LMX2572_write(SPI_HandleTypeDef *hspi, uint32_t value) {
	uint8_t spi_buf[3] = {0,0,0};

	spi_buf[2] = value;
	spi_buf[1] = value >> 8;
	spi_buf[0] = value >> 16;

	HAL_GPIO_WritePin(&port, &pin, GPIO_PIN_RESET);			// CSB LOW
	HAL_SPI_Transmit(hspi, spi_buf, 3, 100);
	HAL_GPIO_WritePin(pll.port, pll.pin, GPIO_PIN_SET);		// CSB HIGH
}

// Loads the registers to the PLL
void LMX2572_load_regs(struct PLL pll, SPI_HandleTypeDef *hspi){
	int i = 0;
	while ((pll.R[125-i] & 0x00FF0000) != 0) {
		LMX2572_write(pll, hspi, pll.R[125-i]);
		i++;
	}
	LMX2572_write(pll, hspi, pll.R[125-i]);
}

// Resets the PLL
void LMX2572_reset(struct PLL pll, SPI_HandleTypeDef *hspi){
	uint8_t spi_buf[3] = {0,0,0};
	spi_buf[2] = 0x1E;
	spi_buf[1] = 0x21;
	spi_buf[0] = 0x00;

	HAL_GPIO_WritePin(&pll.port, &pll.pin, GPIO_PIN_RESET);	// CSB LOW
	HAL_SPI_Transmit(hspi, spi_buf, 3, 100);
	HAL_GPIO_WritePin(&pll.port, &pll.pin, GPIO_PIN_SET);	// CSB LOW

	HAL_Delay(500);
}

// Loads PLL default registers to the PLL register bank
void LMX2572_default(struct PLL pll, uint32_t lmx2572Regs[]){
	for(int i=0; i<126; i++){
		pll.R[i] = lmx2572Regs[125-i];
	}
}

// Determine N, NUM and DEN values from target frequency and fpd
void LMX2572_det_param(struct PLL pll){
	pll.fpd = pll.Fosc * pll.osc_2x * pll.pll_r_pre * pll.pll_r * pll.mult;
	pll.pll_n = pll.frequency/pll.fpd;
	pll.pll_den = 1000;
	pll.pll_num = (pll.frequency/pll.fpd-pll.pll_n)*pll.pll_den;
	pll.Fvco = pll.fpd * pll.pll_n + (pll.pll_num * pll.fpd / pll.pll_den);
}

// Sets PLL N, DEN and NUM registers
void LMX2572_set_PLL(struct PLL pll) {
	// Sets N 
	pll.R[34] &= ~(0x07 << 0);
	pll.R[34] |= (((pll.pll_n >> 16) & 0x07) << 0);
	pll.R[36] &= ~(0xFFFF << 0);
	pll.R[36] |= ((pll.pll_n & 0xFFFF) << 0);
	
	// Sets DEN 
	pll.R[38] &= ~(0xFFFF << 0);
	pll.R[38] |= ((pll.pll_den >> 16) << 0);
	pll.R[39] &= ~(0xFFFF << 0);
	pll.R[39] |= ((pll.pll_den & 0x0000FFFF) << 0);

	// Sets NUM 
	pll.R[42] &= ~(0xFFFF << 0);
	pll.R[42] |= ((pll.pll_num >> 16) << 0);
	pll.R[43] &= ~(0xFFFF << 0);
	pll.R[43] |= ((pll.pll_num & 0x0000FFFF) << 0);
}

// Turns on/off RFout A and RFoutB
void LMX2572_set_RFout(struct PLL pll){
	if (pll.out_pd_a == 1) 	pll.R[44] |= (0x01 << 6);
	else 					pll.R[44] &= ~(0x01 << 6);
	if (pll.out_pd_b == 1)	pll.R[44] |= (0x01 << 7);
	else 			   		pll.R[44] &= ~(0x01 << 7);
}

// Sets RFoutA output power
void LMX2572_pwr_RFoutA(struct PLL pll){
	pll.R[44] &= ~(0x3F << 8);
	pll.R[44] |= (pll.out_pwr_a << 8);
}

// Sets RFoutB output power
void LMX2572_pwr_RFoutB(struct PLL pll){
	pll.R[45] &= ~(0x3F << 0);
	pll.R[45] |= (pll.out_pwr_b << 0);
}

//************************
//***** TO BE TESTED *****
//************************

// Reads a register value from the PLL
uint32_t LMX2572_read(struct PLL pll, SPI_HandleTypeDef *hspi, uint32_t value) {
	uint32_t read = 0;
	uint8_t read_value[2] = {0x00, 0x00};
	uint8_t spi_buf[3] = {0,0,0};
	spi_buf[2] = value | (1<<7);
	spi_buf[1] = value >> 8;
	spi_buf[0] = value >> 16;

	HAL_GPIO_WritePin(pll.port, pll.pin, GPIO_PIN_RESET);	// CSB LOW
	HAL_SPI_Transmit(hspi, spi_buf, 3, 100);
	HAL_SPI_Receive(hspi, read_value, 2, 10);
	HAL_GPIO_WritePin(pll.port, pll.pin, GPIO_PIN_SET);		// CSB HIGH

	read += read_value[0];
	read += read_value[1] >> 8;
	return read;
}

// Powers Down the PLL
void LMX2572_off_PLL(struct PLL pll, SPI_HandleTypeDef *hspi){
	pll.R[0] |= (0x01 << 0);

	uint8_t spi_buf[3] = {0,0,0};
	spi_buf[2] = pll.R[0];
	spi_buf[1] = pll.R[0] >> 8;
	spi_buf[0] = pll.R[0] >> 16;

	HAL_GPIO_WritePin(pll.port, pll.pin, GPIO_PIN_RESET);	// CSB LOW
	HAL_SPI_Transmit(hspi, spi_buf, 3, 100);
	HAL_GPIO_WritePin(pll.port, pll.pin, GPIO_PIN_SET);		// CSB HIGH
}

// Powers Up the PLL
void LMX2572_on_PLL(struct PLL pll, SPI_HandleTypeDef *hspi){
	pll.R[0] &= ~(0x01 << 0);

	uint8_t spi_buf[3] = {0,0,0};
	spi_buf[2] = pll.R[0];
	spi_buf[1] = pll.R[0] >> 8;
	spi_buf[0] = pll.R[0] >> 16;

	HAL_GPIO_WritePin(pll.port, pll.pin, GPIO_PIN_RESET);	// CSB LOW
	HAL_SPI_Transmit(hspi, spi_buf, 3, 100);
	HAL_GPIO_WritePin(pll.port, pll.pin, GPIO_PIN_SET);		// CSB HIGH
}

// Inits LMX2572
void LMX2572_init(struct PLL pll, uint32_t lmx2572Regs[]) {
	HAL_GPIO_WritePin(pll.port, pll.pin, GPIO_PIN_SET);		// CSB HIGH
	LMX2572_default(pll, lmx2572Regs);
	LMX2572_det_param(pll);

	pll.R[44] |= (0x01 << 6);
	pll.R[44] |= (0x01 << 7);
}

// Sets Charge Pump Gain in uA
void LMX2572_set_cpg(struct PLL pll) {
	pll.R[14] &= 0xFF1800;
	if (pll.CPG == 0) 			pll.R[14] |= 0x0000;		// CPG = 0
	else if (pll.CPG == 625) 	pll.R[14] |= 0x0008;		// CPG = 1
	else if (pll.CPG == 1250) 	pll.R[14] |= 0x0010;		// CPG = 2
	else if (pll.CPG == 1875) 	pll.R[14] |= 0x0018;		// CPG = 3
	else if (pll.CPG == 2500) 	pll.R[14] |= 0x0020;		// CPG = 4
	else if (pll.CPG == 3125) 	pll.R[14] |= 0x0028;		// CPG = 5
	else if (pll.CPG == 3750) 	pll.R[14] |= 0x0030;		// CPG = 6
	else if (pll.CPG == 4375) 	pll.R[14] |= 0x0038;		// CPG = 7
	else if (pll.CPG == 5000) 	pll.R[14] |= 0x0060;		// CPG = 12
	else if (pll.CPG == 5625) 	pll.R[14] |= 0x0068;		// CPG = 13
	else if (pll.CPG == 6250) 	pll.R[14] |= 0x0070;		// CPG = 14
	else if (pll.CPG == 6875) 	pll.R[14] |= 0x0078;		// CPG = 15
}

// Sets pfd
void LMX2572_set_fpd(struct PLL pll) {
	// Set Doubler
	if (pll.osc_2x == 2) pll.R[9] |= (0x01 << 12);
	else 			 	 pll.R[9] &= ~(0x01 << 12);

	// Set Pre R
	pll.R[12] &= ~(0xFFF << 0);
	pll.R[12] |= ((pll.pll_r_pre & 0xFFF) << 0);

	// Set Multiplier
	pll.R[10] &= ~(0x1F << 7);
	pll.R[10] |= ((pll.mult & 0x1F) << 7);

	// Set R
	pll.R[11] &= ~(0xFF << 4);
	pll.R[11] |= ((pll.pll_r & 0xFF) << 4);

	if (pll.Fosc * pll.osc_2x * pll.pll_r_pre * pll.mult > 100) pll.R[9] |= (0x01 << 14);
	else pll.R[9] &= ~(0x01 << 14);
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

	pll.R[20] &= ~(0xF << 10);
	pll.R[20] |= (pll.VCO << 11);
	// Forces the VCO to use the core specified by VCO_SEL
	if (pll.VCO_force == 1)	pll.R[20] |= (1 << 10);

	// VCO_CAPCTRL_STRT = Round[CMin – (fVCO – fMin) × (CMin – CMax) / (fMax – fMin)]
	VCO_CAPCTRL_STRT = CMin-(pll.Fvco-fMin)*(CMin-CMax)/(fMax-fMin);
	pll.R[78] &= ~(0xFF << 1);
	pll.R[78] |= (VCO_CAPCTRL_STRT << 1);

	// VCO_DACISET_STRT = Round[AMin – (fVCO – fMin) × (AMin – AMax) / (fMax – fMin)]
	VCO_DACISET_STRT = AMin-(pll.Fvco-fMin)*(AMin-AMax)/(fMax-fMin);
	pll.R[17] &= ~(0xFF << 0);
	pll.R[17] |= (VCO_DACISET_STRT << 0);
}

// Calibrates VCO
void LMX2572_calibrate_VCO(struct PLL pll) {
	pll.R[0] |= (0x01 << 3);
}

// Sets output A Mux: CHDIV = 0, VCO = 1, HI_Z = 3
void LMX2572_mux_RFoutA(struct PLL pll){
	pll.R[45] &= ~(0x03 <<11);
	pll.R[45] |= (pll.out_mux_a << 11);
}

// Sets output B Mux: CHDIV = 0, VCO = 1, HI_Z = 3
void LMX2572_mux_RFoutB(struct PLL pll){
	pll.R[46] &= ~(0x03 << 0);
	pll.R[46] |= (pll.out_mux_b << 0);
}

// Sets CHDIV
void LMX2572_set_CHDIV(struct PLL pll){
	pll.R[75] &= ~(0x1F << 6);
	if		(pll.chdiv == 2)	pll.R[75] |= (0 << 6);
	else if (pll.chdiv == 4)	pll.R[75] |= (1 << 6);
	else if (pll.chdiv == 8)	pll.R[75] |= (3 << 6);
	else if (pll.chdiv == 16)	pll.R[75] |= (5 << 6);
	else if (pll.chdiv == 32)	pll.R[75] |= (7 << 6);
	else if (pll.chdiv == 64)	pll.R[75] |= (9 << 6);
	else if (pll.chdiv == 128)	pll.R[75] |= (12 << 6);
	else if (pll.chdiv == 256)	pll.R[75] |= (14 << 6);
}

void LMX2572_set_MASH(struct PLL pll){
	pll.R[44] &= ~(0x27 << 0);
	pll.R[44] = (pll.MASH_order << 0);
	if (pll.pll_num != 0) pll.R[44] |= (0x01 << 5);

	pll.R[37] &= ~(0x3F << 8);
	pll.R[37] |= (pll.PFD_DLY_SEL << 8);
}
