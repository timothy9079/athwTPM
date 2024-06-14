#ifndef __CONFIGS_H
#define __CONFIGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"
#include <stdint.h>

/**
 * debug enable
 */

#define ATHW_TPM_TIS_DEBUG
#define ATHW_TPM_DEBUG
#define TPM2_DEBUG
#define _STM32_TPM_

#define ATHW_SYSTEM_VERSION_MAJOR	0x01 	/*! [31:24] major */
#define ATHW_SYSTEM_VERSION_MINOR	0x00	/*! [23:16] minor*/
#define ATHW_SYSTEM_VERSION_BUILD	0x00	/*! [15:8] builid */
#define ATHW_SYSTEM_VERSION_RC		0x00	/*!	[7:0] release condidate */
#define ATHW_SYSTEM_VERSION 		((ATHW_SYSTEM_VERSION_MAJOR << 24)\
									|(ATHW_SYSTEM_VERSION_MINOR << 16)\
									|(ATHW_SYSTEM_VERSION_BUILD << 8)\
									|(ATHW_SYSTEM_VERSION_RC))


/**
 * @addtogroup TPM_I/F
 * Additional TPM Master interfcae
 * @{
 */
#define _ATHW_STM32_TPM_				


#define ATHW_TPM_SPI_PORT				SPI1
#define ATHW_TPM_SPI_MODE				SPI_MODE_MASTER
#define ATHW_TPM_SPI_CS_PORT 			GPIOA
#define ATHW_TPM_SPI_CS_PIN				GPIO_PIN_4

#define ATHW_TPM_SPI_BANK				SPI1
#define ATHW_TPM_SPI_IO_PORT			GPIOA
#define ATHW_TPM_SPI_CS 				GPIO_PIN_4
#define ATHW_TPM_SPI_SCK 				GPIO_PIN_5
#define ATHW_TPM_SPI_MISO				GPIO_PIN_6
#define ATHW_TPM_SPI_MOSI 				GPIO_PIN_7
 
#define ATHW_TPM_SPI_RST 				GPIO_PIN_8

#define ATHW_TPM_SPI_WAIT_RETRY 		50
#define ATHW_TPM_SPI_TMO 				250					/*! TPM SPI timeout*/

#ifndef ATHW_LOCALITY_DEFAULT
#define ATHW_LOCALITY_DEFAULT 			0
#endif

#define ATHW_TPM_SPI_FRAME_SZ			64
/** }   */

#define HOST_SPI_PORT		SPI2
#define HOST_SPI_MODE		SPI_MODE_SLAVE

#define COM_UART_PORT		USART1
#define COM_UART_SPEED		(115200)



/**
 * @brief HOST Timer Interrupt 
 */
#define PKT_TIMx				TIM7
#define PKT_TIM_IRQn			TIM7_IRQn
#define PKT_TIM_CLK_ENABLE		__HAL_RCC_TIM7_CLK_ENABLE()


#ifdef __cplusplus
}
#endif



#endif
