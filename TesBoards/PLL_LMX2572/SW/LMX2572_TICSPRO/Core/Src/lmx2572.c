#include "lmx2572.h"

#define DEBUG_SERIAL  SD2
#define DEBUG_CHP     ((BaseSequentialStream *) &DEBUG_SERIAL)

void lmx2572_write_reg(SPI_HandleTypeDef *spip, uint32_t value){
  uint8_t spi_buf[3] = {0,0,0};

  spi_buf[2] = value;
  spi_buf[1] = value >> 8;
  spi_buf[0] = value >> 16;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);		// CSB LOW
  HAL_SPI_Transmit(spip, spi_buf, 3, 100);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);		// CSB HIGH

}

uint32_t lmx2572_read(SPI_HandleTypeDef *hspi, uint32_t value) {
	uint32_t read = 0;
	uint8_t read_value[2] = {0x00, 0x00};
	uint8_t spi_buf[3] = {0,0,0};
	spi_buf[2] = value | (1<<7);
	spi_buf[1] = value >> 8;
	spi_buf[0] = value >> 16;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);		// CSB LOW
	HAL_SPI_Transmit(hspi, spi_buf, 3, 100);
	HAL_SPI_Receive(hspi, read_value, 2, 10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);		// CSB HIGH

	read += read_value[0];
	read += read_value[1] >> 8;
	return read;
}

void lmx2572_load_regs(SPI_HandleTypeDef *spip, uint32_t lmx2572Regs[]){
  int i = 0;
  while ((lmx2572Regs[125-i] & 0x00FF0000) != 0) {
    lmx2572_write_reg(spip,lmx2572Regs[125-i]);
    i++;
  }
  lmx2572_write_reg(spip,lmx2572Regs[125-i]);

}

void lmx2572_reset(SPI_HandleTypeDef *spip){
  uint8_t spi_buf[3] = {0,0,0};
  
  spi_buf[2] = 0x1E;
  spi_buf[1] = 0x21;
  spi_buf[0] = 0x00;
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);		// CSB LOW
  HAL_SPI_Transmit(spip, spi_buf, 3, 100);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);		// CSB HIGH
 // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);		// CSB HIGH
}
