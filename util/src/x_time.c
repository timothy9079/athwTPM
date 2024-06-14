/********************************************************************************
 * Copyright (C) 2020 by Trustkey                                               * 
 * This file is part of  Project                                                *   
 * This software contains confidential information of TrustKey Co.,Ltd.         *
 * and unauthorized distribution of this software, or any portion of it, are    *
 * prohibited.                                                                  *
 ********************************************************************************/

/**
 * @file x_time.c
 * Time 
 *
 * @anchor ATHW_EVAL
 * @author Leon, (c) Trustkey
 * @version Draft 
 * @date 2024-01-08
 * @bug No known bugs.
 * @note 
 * 
 * @copyright Copyright 2020 Trustkey. All rights reserved*
 */        

#include "x_time.h"
#include "stm32l4xx_hal.h"
#include "x_log.h"


void delay_ms(int ms)
{
    uint32_t tickstart = 0UL;
    uint32_t tickcur = 0UL;

    tickstart = HAL_GetTick();

    
    //tr_log("delay time : %d ms , start : %d", ms, tickstart);
    
    
    do {
        tickcur = HAL_GetTick();
    } while ((tickcur - tickstart) < ms);
    
    //tr_log("%d - %d = %d ms", tickcur, tickstart, tickcur - tickstart);
    
}
