/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "XCOMMS.h"

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
LPTIM_HandleTypeDef hlptim1;
SPI_HandleTypeDef hspi1;
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI1_Init(void);
static void MX_LPTIM1_Init(void);

static struct PLL PLL_Rx;
static struct PLL PLL_Tx;
uint8_t UART1_rxBuffer[12] = {0};
uint8_t state;
uint16_t measurements[HK_length];

int main(void){
	state = XCOMMS_INIT_state;
	uint8_t timerCount = 0;
	uint8_t measurements_pointer = 0;

	while (1){
		switch (state){
		case XCOMMS_INIT_state:
			/* MCU Configuration--------------------------------------------------------*/
			/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
			HAL_Init();
			/* Configure the system clock */
			SystemClock_Config();
			/* Initialize all configured peripherals */
			MX_GPIO_Init();
			MX_DMA_Init();
			MX_USART1_UART_Init();
			MX_ADC1_Init();
			MX_SPI1_Init();
			MX_LPTIM1_Init();

			TxPLL_INIT(&hspi1);								// Initialize Tx PLL
			RxPLL_INIT(&hspi1);								// Initialize Rx PLL
			break;
		case IDLE_state:
			HAL_LPTIM_Counter_Start_IT(&hlptim1, PERIOD);	// Start 1 sec timer
			HAL_UART_Receive_IT (&huart1, UART1_rxBuffer, 12);
			HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);	// Enter Stop 1 powermode
			break;
		case MEASUREMENT_state:
			HAL_GPIO_TogglePin(mcu_watchdog_GPIO_Port, mcu_watchdog_Pin);
			timerCount ++;
			if(timerCount == MEASURE_PERIOD){
				measure();
				timerCount = 0;
			}
			break;
		case TUNE_PLL_state:
			state = IDLE_state;
			break;
		case POWER_RF_state:
			state = IDLE_state;
			break;
		case HOUSEKEEPING_state:
			HOUSEKEEPING(&huart1, measurements, HK_length);
			state = IDLE_state;
			break;
		case SHUTDOWN_state:
			break;
		default:
			state = XCOMMS_INIT_state;
			break;
		}
	}
}

void TxPLL_INIT(SPI_HandleTypeDef *hspi){
	PLL_Tx.frequency= 6000;			// Target frequency in MHz
	PLL_Tx.Fosc 	= 25;				// OSCin frequency in MHz
	PLL_Tx.pin 		= Tx_CE_Pin;
	PLL_Tx.port		= Tx_CE_GPIO_Port;
	PLL_Tx.osc_2x 	= 1;
	PLL_Tx.pll_r_pre= 1;
	PLL_Tx.pll_r 	= 1;
	PLL_Tx.mult 	= 1;
	PLL_Tx.out_pwr_a= 30;				// RFoutA Power
	PLL_Tx.out_pwr_b= 0;				// RFoutB Power
	PLL_Tx.out_pd_a	= 0;				// On = 0, Off = 1
	PLL_Tx.out_pd_b	= 1;				// On = 0, Off = 1

	HAL_GPIO_WritePin(PLL_Tx.port, PLL_Tx.pin, GPIO_PIN_SET);	// Tx PLL Enable

	LMX2572_init(PLL_Tx, R_default);
	LMX2572_reset(PLL_Tx, hspi);
	LMX2572_set_fpd(PLL_Tx);
	LMX2572_set_cpg(PLL_Tx);
	LMX2572_set_PLL(PLL_Tx);
	LMX2572_pwr_RFoutA(PLL_Tx);
	LMX2572_set_RFout(PLL_Tx);
	LMX2752_vco_assist(PLL_Tx);
	LMX2572_load_regs(PLL_Tx, hspi);
}

void RxPLL_INIT(SPI_HandleTypeDef *hspi){
	PLL_Rx.frequency = 6000;			// Target frequency in MHz
	PLL_Rx.Fosc 	= 25;				// OSCin frequency in MHz
	PLL_Tx.pin 		= Rx_CE_Pin;
	PLL_Tx.port		= Rx_CE_GPIO_Port;
	PLL_Rx.osc_2x 	= 1;
	PLL_Rx.pll_r_pre= 1;
	PLL_Rx.pll_r 	= 1;
	PLL_Rx.mult 	= 1;
	PLL_Rx.out_pwr_a= 30;				// RFoutA Power
	PLL_Rx.out_pwr_b= 0;				// RFoutB Power
	PLL_Rx.out_pd_a	= 0;				// On = 0, Off = 1
	PLL_Rx.out_pd_b	= 1;				// On = 0, Off = 1

	HAL_GPIO_WritePin(PLL_Rx.port, PLL_Rx.pin, GPIO_PIN_SET);	// Rx PLL Enable

	LMX2572_init(PLL_Rx, R_default);
	LMX2572_reset(PLL_Rx, hspi);
	LMX2572_set_fpd(PLL_Rx);
	LMX2572_set_cpg(PLL_Rx);
	LMX2572_set_PLL(PLL_Rx);
	LMX2572_pwr_RFoutA(PLL_Rx);
	LMX2572_set_RFout(PLL_Rx);
	LMX2752_vco_assist(PLL_Rx);
	LMX2572_load_regs(PLL_Rx, hspi);
}

void HAL_LPTIM_TriggerCallback(LPTIM_HandleTypeDef *hlptim){
	state = HOUSEKEEPING_state;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    HAL_UART_Receive_IT(&huart1, UART1_rxBuffer, 12);
    state = UART_decode();
}

uint8_t UART_decode(){
	uint8_t code = IDLE_state;
	// TBD
	return code;
}

void measure(){
	uint16_t temp;
	uint16_t tx_pwr;


}




void pckt_creator(uint8_t *msg, uint8_t *data, uint8_t size, uint8_t comand){
	// TBD
}

void HOUSEKEEPING(UART_HandleTypeDef *huart, uint16_t *pData, uint8_t size){
	uint8_t data[2*size];
	uint8_t msg[msg_pckt+(2*size)];

	for(uint8_t i = 0; i < size; i++){
		data[2*i] = *(pData+i) & 0xff;
		data[2*i+1] = *(pData+i) >> 8;
	}

	pckt_creator(msg, data, 2*size, answ_HK);

	HAL_UART_Transmit(huart, msg, sizeof(msg), TIMEOUT);	//send housekeeping data
}







/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void){
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_LPTIM1
			|RCC_PERIPHCLK_ADC;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_PCLK;
	PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
	PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
	PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
	PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
	PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
	PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void){
	ADC_ChannelConfTypeDef sConfig = {0};
	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_7;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief LPTIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_LPTIM1_Init(void){
	/* USER CODE END LPTIM1_Init 1 */
	hlptim1.Instance = LPTIM1;
	hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
	hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
	hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
	hlptim1.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
	hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
	hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
	hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
	hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
	if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void){
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void){
	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void){
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel4_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(mcu_watchdog_GPIO_Port, mcu_watchdog_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, Rx_CSB_Pin|TCXO_NE_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, Tx_CSB_Pin|Rx_CE_Pin|Tx_CE_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : mcu_watchdog_Pin */
	GPIO_InitStruct.Pin = mcu_watchdog_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(mcu_watchdog_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PC15 */
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : Rx_CSB_Pin TCXO_NE_Pin */
	GPIO_InitStruct.Pin = Rx_CSB_Pin|TCXO_NE_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PA6 PA8 PA11 PA12 */
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : Tx_CSB_Pin Rx_CE_Pin Tx_CE_Pin */
	GPIO_InitStruct.Pin = Tx_CSB_Pin|Rx_CE_Pin|Tx_CE_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PB1 */
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : Tx_MUXout_Pin Rx_MUXout_Pin */
	GPIO_InitStruct.Pin = Tx_MUXout_Pin|Rx_MUXout_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void){
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
