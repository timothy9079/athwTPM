#ifndef __ATHW_TPMIO_TYPES_H
#define __ATHW_TPMIO_TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "configs.h"

#define MAX_SPI_FRAMESIZE			64

#define HOST_SPI_BANK	 			SPI2
#define HOST_SPI_IO_PORT			GPIOB
#define HOST_SPI_CS 	 			GPIO_PIN_12
#define HOST_SPI_CLK				GPIO_PIN_13
#define HOST_SPI_MISO				GPIO_PIN_14
#define HOST_SPI_MOSI 	 			GPIO_PIN_15



#ifndef TPM_TIMEOUT_TRIES
#define TPM_TIMEOUT_TRIES 1000000
#endif

#ifndef TPM_SPI_WAIT_RETRY
#define TPM_SPI_WAIT_RETRY 50
#endif

#ifndef MAX_SYM_BLOCK_SIZE
#define MAX_SYM_BLOCK_SIZE 20
#endif
#ifndef MAX_SYM_KEY_BYTES
#define MAX_SYM_KEY_BYTES 256
#endif
#ifndef LABEL_MAX_BUFFER
#define LABEL_MAX_BUFFER 128
#endif
#ifndef MAX_RSA_KEY_BITS
#define MAX_RSA_KEY_BITS 2048
#endif
#ifndef MAX_RSA_KEY_BYTES
#define MAX_RSA_KEY_BYTES (((MAX_RSA_KEY_BITS+7)/8)*2)
#endif

#ifndef MAX_ECC_KEY_BITS
#define MAX_ECC_KEY_BITS 521
#endif
#ifndef MAX_ECC_KEY_BYTES
#define MAX_ECC_KEY_BYTES (((MAX_ECC_KEY_BITS+7)/8)*2)
#endif

#ifndef MAX_AES_KEY_BITS
#define MAX_AES_KEY_BITS 128
#endif
#ifndef MAX_AES_BLOCK_SIZE_BYTES
#define MAX_AES_BLOCK_SIZE_BYTES 16
#endif
#ifndef MAX_AES_KEY_BYTES
#define MAX_AES_KEY_BYTES (MAX_AES_KEY_BITS/8)
#endif


#define TPM_TIS_HEADER_SZ			4
#define TPM_SPI_TIMEOUT				250
#define TPM_TIS_READ       			0x80
#define TPM_TIS_WRITE      			0x00
#define TPM_TIS_READY_MASK 			0x01

/* Typically only 0-2 wait states are required */
#ifndef TPM_TIS_MAX_WAIT
#define TPM_TIS_MAX_WAIT   3
#endif

#define TPM_XFER_MAX_SZ 			4096


/**
 * \defgroup TPM_Software_IF TPM Software Interface 
 */

/**
 * \addtogroup TPM_Software_IF 
 * \{ 
 */



/** 
 * \brief Definition of interface timeouts 
 */
#define TIMEOUT_A                   750U // milliseconds
#define TIMEOUT_B                   2000U
#define TIMEOUT_C                   TIMEOUT_A
#define TIMEOUT_D                   TIMEOUT_A

/** \}   */


static inline uint32_t byte_revrse_uint32(uint32_t value) 
{
//#if 	defined(__ICCARM__)
//	return (uint32_t)__REV(value);
//#elif 	defined(__GNUC__) && (defined(__thumb__) || defined(__arm__))
//	__asm__ volatile (
//					  "REV %0 %0 \r\n"
//					  : "+r"(value)
//					  :
//					 );
//
//	return value;
//#endif

	return value;

}

#ifdef __cplusplus
}
#endif
#endif
