/********************************************************************************
 * Copyright (C) 2020 by Trustkey                                               * 
 * This file is part of  Project                                                *   
 * This software contains confidential information of TrustKey Co.,Ltd.         *
 * and unauthorized distribution of this software, or any portion of it, are    *
 * prohibited.                                                                  *
 ********************************************************************************/

/**
 * @file system_config.c
 * ATHW System configuration and initialize
 *
 * @anchor ANCHOR_SYSTEM
 * @author User, (c) Trustkey
 * @version Draft 
 * @date 011024
 * @bug No known bugs.
 * @note 
 * 
 * @copyright Copyright 2020 Trustkey. All rights reserved*
 */         



#include "main.h"
#include "stm32l4xx.h"
#include "configs.h"
extern void error_handler(void *priv);


/**
 * @fn athw_system_hal_init - Init the low level hardware
 * 
 * @author User (1/9/2024)
 * 
 * @return int 
 */
int athw_system_hal_init(void)
{
    int ret = EOK;
    HAL_Init();

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    return ret;
}

/**
 * @fn athw_system_clock_config 
 * @brief System clock configuration 
 * 
 * @author User (1/9/2024)
 * 
 * @retval None
 */

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void athw_system_clock_config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
  RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

    return;
}


//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
//
//    if(htim->Instance == PKT_TIMx) {
//        // TIM peripheral clock enable
//        PKT_TIM_CLK_ENABLE;
//
//        // Config the NVIC for TIM3
//        HAL_NVIC_SetPriority(PKT_TIM_IRQn, 3, 0);
//
//        // Enable the TIMx Global Interrupt
//        HAL_NVIC_EnableIRQ(PKT_TIM_IRQn);
//    }
//
//
//}

