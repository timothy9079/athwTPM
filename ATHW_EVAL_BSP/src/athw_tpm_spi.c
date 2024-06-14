/********************************************************************************
 * Copyright (C) 2020 by Trustkey                                               * 
 * This file is part of  Project                                                *   
 * This software contains confidential information of TrustKey Co.,Ltd.         *
 * and unauthorized distribution of this software, or any portion of it, are    *
 * prohibited.                                                                  *
 ********************************************************************************/

/**
 * @file athw_tpm_spi.c
 * TPM SPI interface
 *
 * @anchor ANCHOR_SPI
 * @author User, (c) Trustkey
 * @version Draft 
 * @date 011024
 * @bug No known bugs.
 * @note 
 * 
 * @copyright Copyright 2020 Trustkey. All rights reserved*
 */         

#include "configs.h"
#include "athw_tpmio_types.h"
#include "athw_it_types.h"
#include "x_common.h"
#include "athw_eval_it.h"

int athw_tpmiocb_spi(void *handle) 
{
	int ret = EOK;
	SPI_HandleTypeDef *ctx = (SPI_HandleTypeDef *)(((spi_ioctx_t *)handle)->h_io);
	HAL_StatusTypeDef status;
	int timeout = TPM_SPI_WAIT_RETRY;
	
	uint8_t *txbuf = (uint8_t *)(((spi_ioctx_t *)handle)->iobuf.tx);
	uint8_t *rxbuf = (uint8_t *)(((spi_ioctx_t *)handle)->iobuf.tx); 
	uint16_t xfersz = (uint16_t)(((spi_ioctx_t *)handle)->iobuf.txlen);

	X_RET_VAL_IF_FAIL((txbuf != NULL), ERRNGATE(ESNULLP));
	X_RET_VAL_IF_FAIL((rxbuf != NULL), ERRNGATE(ESNULLP)); 
	X_RET_VAL_IF_FAIL((xfersz != 0), ERRNGATE(ESZEROL));

	if(ctx == NULL) {
		ret = ERRNGATE(ESNULLP);
		goto errdone;
	}

	__HAL_SPI_ENABLE(ctx);


	// To SPI start, CS goes active low
	HAL_GPIO_WritePin(ATHW_TPM_SPI_CS_PORT, ATHW_TPM_SPI_CS_PIN, GPIO_PIN_RESET);

	status = HAL_SPI_TransmitReceive(ctx, txbuf, rxbuf, TPM_TIS_HEADER_SZ, TPM_SPI_TIMEOUT);
	if(status != HAL_OK) {
		ret = ERRNGATE(EIOFAIL);
		goto errdone;
	}


	//  check for wait status
	if((rxbuf[TPM_TIS_HEADER_SZ - 1] && TPM_TIS_READY_MASK) == 0) {
		do {
			// Check ready
			status = HAL_SPI_TransmitReceive(ctx, txbuf, rxbuf, 1, TPM_SPI_TIMEOUT);
			if(rxbuf[0] & TPM_TIS_READY_MASK) {
				break;
			}
		} while(status == HAL_OK && --timeout > 0);
	}
#if defined(ATHW_TPM_SPI_DEBUG) || defined(ATHW_DEBUG_ALL)
	printf(" spi ready wait is %d \r\n", TPM_SPI_WAIT_RETRY - timeout);
#endif

	if(timeout <= 0) {
		ret = ERRNGATE(EIOTIMEOUT);
		goto errdone;
	}

	// send reminder payload 
	status = HAL_SPI_TransmitReceive(ctx, 
									 &txbuf[TPM_TIS_HEADER_SZ], 
									 &rxbuf[TPM_TIS_HEADER_SZ], 
									 xfersz - TPM_TIS_HEADER_SZ,
									 TPM_SPI_TIMEOUT);



	if(status != HAL_OK) {
		ret = ERRNGATE(EIOFAIL);
	}

errdone:

#if defined(ATHW_TPM_SPI_DEBUG) || defined(ATHW_DEBUG_ALL)
	printf("ret code : %d , xfer : %d, status : 0x%x \n", ret, xfersz, status);
#endif

	// To SPI stop, CS goes active high
	HAL_GPIO_WritePin(ATHW_TPM_SPI_CS_PORT, ATHW_TPM_SPI_CS_PIN, GPIO_PIN_SET);
	__HAL_SPI_DISABLE(ctx);
	return ret;

}





