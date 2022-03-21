/*! \file LMX2572.h */

/**
 * @file    LMX2572.h
 * @brief   LMX2572 synthesizer.
 *
 * @addtogroup LMX2572
 * @ingroup ORESAT
 * @{
 */
 
#ifndef _LMX2572_H
#define _LMX2572_H

#include "stm32l4xx_hal.h"
#include "main.h"

#define LMX2572_VERSION                      "1.0.0"
#define LMX2572_MAJOR                        1
#define LMX2572_MINOR                        0
#define LMX2572_PATCH                        0

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
//static uint32_t R[126];

void 		lmx2572_write_reg(SPI_HandleTypeDef  *spip, uint32_t value);
void 		lmx2572_load_regs(SPI_HandleTypeDef  *spip, uint32_t lmx2572Regs[]);
void 		lmx2572_reset(SPI_HandleTypeDef *spip);
uint32_t 	lmx2572_read(SPI_HandleTypeDef *hspi, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif /*_LMX2572_H_ */
//! @} 
