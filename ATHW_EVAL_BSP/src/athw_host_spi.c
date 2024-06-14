/********************************************************************************
 * Copyright (C) 2020 by Trustkey                                               * 
 * This file is part of  Project                                                *   
 * This software contains confidential information of TrustKey Co.,Ltd.         *
 * and unauthorized distribution of this software, or any portion of it, are    *
 * prohibited.                                                                  *
 ********************************************************************************/

/**
 * @file athw_host_spi.c
 * HOST command processing
 *
 * @anchor ANCHOR_SPI
 * @author User, (c) Trustkey
 * @version Draft 
 * @date 011224
 * @bug No known bugs.
 * @note 
 * 
 * @copyright Copyright 2020 Trustkey. All rights reserved*
 */         

#include "configs.h"
#include "athw_eval_it.h"
#include "athw_it_types.h"
extern spi_ioctx_t host_spictx; 

extern void _pkt_proc_suspend_tick(void);
extern int athw_pkt_incoming(void *priv);


/**
 * SPI TX/RX complete IRQ callback 
 * 
 * @author rocke (2024-01-26)
 * 
 * @param hspi   SPI Handle
 */
void HAL_SPI_RxCpltCallBack(SPI_HandleTypeDef *hspi) 
{
	_pkt_proc_suspend_tick();
	host_spictx.state = ATHW_IFCB_PROCEDURE_TYPE_RX;
	athw_pkt_incoming(NULL);
}

void HAL_SPI_TxRxCpltCallBack(SPI_HandleTypeDef *hspi) 
{

	athw_pkt_incoming(NULL);

}

/**
 * @brief SPI error callback
 * 
 * @author rocke (2024-01-26)
 * 
 * @param hspi    SPI Handle
 */
void HAL_SPI_ErrorCallBack(SPI_HandleTypeDef *hspi)
{

}

/**
 * @fn SPI2_IRQHandler - Handles HOST SPI interrupt request
 * 
 * @author rocke (2024-02-02)
 * 
 * @param  None
 * @retval None
 */
void SPI2_IRQHandler(void) 
{

	SPI_HandleTypeDef *hnd;
	hnd = (SPI_HandleTypeDef *)athw_eval_get_handle(ATHW_HNDTYPE_HOST);
	HAL_SPI_IRQHandler(hnd);
	//

}

