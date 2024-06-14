  
/********************************************************************************
 * Copyright (C) 2020 by Trustkey                                               * 
 * This file is part of  Project                                                *   
 * This software contains confidential information of TrustKey Co.,Ltd.         *
 * and unauthorized distribution of this software, or any portion of it, are    *
 * prohibited.                                                                  *
 ********************************************************************************/

/**
 * @file athw_eval_it.c
 * ATHW Evaluation module interface 
 *
 * @anchor ATHW_EVAL_BSP
 * @author User, (c) Trustkey
 * @version Draft 
 * @date 010924
 * @bug No known bugs.
 * @note 
 * 
 * @copyright Copyright 2020 Trustkey. All rights reserved*
 */         

#include <stdio.h>

#include "configs.h"
#include "athw_errno.h"
#include "athw_system.h"
#include "athw_eval_it.h"
#include "athw_tpmio_types.h"
#include "athw_it_types.h"
//#include "stm32l4xx_hal_rcc.h"

//#include "stm32l4xx_hal_usart.h"

static DMA_HandleTypeDef h_dmarx;

extern void error_handler(void *priv);

athw_if_handle_t *if_hnd; // Interface Handle


/**
 * @fn athw_system_hal_init - Init the low level hardware
 * 
 * @author User (1/9/2024)
 * 
 * @return int 
 */
//static int athw_system_hal_init(void)
//{
//    int ret = EOK;
//    HAL_Init();
//
//    __HAL_RCC_SYSCFG_CLK_ENABLE();
//    __HAL_RCC_PWR_CLK_ENABLE();
//
//    return ret;
//}

/**
 * @fn athw_system_clock_config 
 * @brief System clock configuration 
 * 
 * @author User (1/9/2024)
 * 
 * @retval None
 */
//static void athw_system_clock_config(void)
//{
//    RCC_OscInitTypeDef osc = {0};
//    RCC_ClkInitTypeDef clk = {0};
//
//    // Configure the main internal regulator output voltage
//    if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
//        error_handler(NULL);
//    }
//
//    // Initializes the RCC Oscillators according to the specified paramters
//    osc.OscillatorType = RCC_OSCILLATORTYPE_MSI;
//    osc.MSIState = RCC_MSI_ON;
//    osc.MSICalibrationValue = 0;
//    osc.MSIClockRange = RCC_MSIRANGE_6;
//    osc.PLL.PLLState = RCC_PLL_NONE;
//    if(HAL_RCC_OscConfig(&osc) != HAL_OK) {
//        error_handler(NULL);
//    }
//
//    // Initialize the CPU, AHB and APB buses clocks
//    clk.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
//                    | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
//    clk.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
//    clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
//    clk.APB1CLKDivider = RCC_HCLK_DIV2;
//    clk.APB2CLKDivider = RCC_HCLK_DIV1;
//
//    if(HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_0) != HAL_OK) {
//        error_handler(NULL);
//    }
//
//    return;
//}

static void athw_eval_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};


    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_8, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

    /*Configure GPIO pins : PC13 PC11 PC12 */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : PA3 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PA4 PA8 */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_8, GPIO_PIN_SET);

    /*Configure GPIO pin : PB11 */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : PB12 */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

static void athw_eval_uart_init(UART_HandleTypeDef *com)
{  
    if(com == NULL) {
        error_handler(NULL);
    }

    com->Instance = COM_UART_PORT;
    com->Init.BaudRate = COM_UART_SPEED;
    com->Init.WordLength = UART_WORDLENGTH_8B;
    com->Init.StopBits = UART_STOPBITS_1;
    com->Init.Parity = UART_PARITY_NONE;
    com->Init.Mode = UART_MODE_TX_RX;
    com->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    com->Init.OverSampling = UART_OVERSAMPLING_16;
    com->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    com->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(com) != HAL_OK)
    {
        error_handler(NULL);
    }
    

    // Turn off buffers, so I/O occurs immediately
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

}

static void athw_eval_hostspi_init(SPI_HandleTypeDef *spi) 
{
    //SPI_InitTypeDef 
    if(spi == NULL) {
        error_handler(NULL);
    }

    spi->Instance               = HOST_SPI_BANK;
    spi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2; //SPI_BAUDRATEPRESCALER_32;
    spi->Init.Direction         = SPI_DIRECTION_2LINES;
    spi->Init.CLKPhase          = SPI_PHASE_1EDGE;
    spi->Init.CLKPolarity       = SPI_POLARITY_LOW;
    spi->Init.DataSize          = SPI_DATASIZE_8BIT;
    spi->Init.FirstBit          = SPI_FIRSTBIT_MSB;
    spi->Init.TIMode            = SPI_TIMODE_DISABLE;
    spi->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    spi->Init.CRCPolynomial     = 7;
    spi->Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
    spi->Init.NSS               = SPI_NSS_SOFT;
    spi->Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;

    spi->Init.Mode              = SPI_MODE_SLAVE;

    if(HAL_SPI_Init(spi) != HAL_OK) {
        error_handler(NULL);
    }
}

static void athw_eval_tpm_dev_init(SPI_HandleTypeDef  *h) 
{ 

    // if not required the condition, enter to error_handler
    if(h == NULL) {
        error_handler(NULL);
    }




    h->Instance                 = SPI1;
    h->Init.Mode                = SPI_MODE_MASTER;
    h->Init.Direction           = SPI_DIRECTION_2LINES;
    h->Init.DataSize            = SPI_DATASIZE_8BIT;
    h->Init.CLKPolarity         = SPI_POLARITY_LOW;
    h->Init.CLKPhase            = SPI_PHASE_1EDGE;
    h->Init.NSS                 = SPI_NSS_SOFT;
    h->Init.BaudRatePrescaler   = SPI_BAUDRATEPRESCALER_64;  //SPI_BAUDRATEPRESCALER_64;
    h->Init.FirstBit            = SPI_FIRSTBIT_MSB;
    h->Init.TIMode              = SPI_TIMODE_DISABLE;
    h->Init.CRCCalculation      = SPI_CRCCALCULATION_DISABLE;
    h->Init.CRCPolynomial       = 7;

    if(HAL_SPI_Init(h) != HAL_OK) {
        error_handler(NULL);
    }

    return;

}


/**
 * @fn HAL_SPI_MspInit - SPI MSP Initialization 
 * 
 * @author User (1/9/2024)
 * 
 * @param hi2c   
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef gpio = {0};
    
    if(hspi->Instance == SPI1) {
        // Peripher Clock Enable
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /**
         * SPI1 GPIO Configuration 
         * PA5  ----->  SPI1_SCK 
         * PA6  ----->  SPI1_MISO 
         * PA7  ----->  SPI1_MOSI 
         */

        //gpio.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
        gpio.Pin = ATHW_TPM_SPI_SCK | ATHW_TPM_SPI_MISO | ATHW_TPM_SPI_MOSI;
        gpio.Mode = GPIO_MODE_AF_PP;
        gpio.Pull = GPIO_NOPULL; //GPIO_PULLDOWN;
        gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio.Alternate = GPIO_AF5_SPI1;

        HAL_GPIO_Init(ATHW_TPM_SPI_IO_PORT, &gpio);
    }
    else if(hspi->Instance == SPI2) {
        __HAL_RCC_SPI2_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_DMA1_CLK_ENABLE();

        /**
         * SPI2 GPIO Configuration 
         * PB13  -----> SPI2_SCK 
         * PB14  -----> SPI2_MISO 
         * PB15  -----> SPI2_MOSI
         */

        gpio.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        gpio.Mode = GPIO_MODE_AF_PP;
        gpio.Pull = GPIO_PULLDOWN;
        gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio.Alternate = GPIO_AF5_SPI2;

        HAL_GPIO_Init(GPIOB, &gpio);

//        h_dmarx.Instance                    = DMA1_Channel2;
//        h_dmarx.Init.Direction              = DMA_PERIPH_TO_MEMORY;
//        h_dmarx.Init.PeriphInc              = DMA_PINC_DISABLE;
//        h_dmarx.Init.MemInc                 = DMA_MINC_ENABLE;
//        h_dmarx.Init.PeriphDataAlignment    = DMA_PDATAALIGN_BYTE;
//        h_dmarx.Init.MemDataAlignment       = DMA_MDATAALIGN_BYTE;
//        h_dmarx.Init.Mode                   = DMA_NORMAL;
//        h_dmarx.Init.Priority               = DMA_PRIORITY_HIGH;
//
//        HAL_DMA_Init(&h_dmarx);
//
//        // accocate the initialized dma handle to spi handle
//        __HAL_LINKDMA(hspi, hdmarx, h_dmarx);
//
//        // NVIC configuration for DMA transfer complete interrupt
//        HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 1, 0);
//        HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
    }

}

/**
 * @fn HAL_SPI_MspDeint - De-initalization
 * 
 * @author User (1/9/2024)
 * 
 * @param hspi   - [in] SPI handle address 
 */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef gpio = {0};
    
    if(hspi->Instance == SPI1) {
        // Peripher Clock Enable
        __HAL_RCC_SPI1_CLK_DISABLE();

        /**
         * SPI1 GPIO Configuration 
         * PA5  ----->  SPI1_SCK 
         * PA6  ----->  SPI1_MISO 
         * PA7  ----->  SPI1_MOSI 
         */

        gpio.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
        HAL_GPIO_DeInit(GPIOA, gpio.Pin);
    }
    else if(hspi->Instance == SPI2) {
        __HAL_RCC_SPI2_CLK_DISABLE();

        /**
         * SPI2 GPIO Configuration 
         * PB13  -----> SPI2_SCK 
         * PB14  -----> SPI2_MISO 
         * PB15  -----> SPI2_MOSI
         */

        gpio.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;

        HAL_GPIO_DeInit(GPIOB, gpio.Pin);

    }

}


/**
 * @fn HAL_UART_MspInit - UART MSP Initialization
 * 
 * @author User (1/9/2024)
 * 
 * @param [in]huart  - Uart handle address
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio = {0};
    RCC_PeriphCLKInitTypeDef clk = {0};

    if(huart->Instance == USART1) {
        clk.PeriphClockSelection = RCC_PERIPHCLK_USART1;
        clk.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
        if(HAL_RCCEx_PeriphCLKConfig(&clk) != HAL_OK) {
            error_handler(NULL);
        }

        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /*
            USUART1 GPIO Configuration
            PB6 -----> UAUART1_TX
            PB7 -----> USUART1_RX
        */

        gpio.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        gpio.Mode = GPIO_MODE_AF_PP;
        gpio.Pull = GPIO_NOPULL;
        gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOB, &gpio);
    }

}

/**
 * @fn HAL_UART_MspDeInit - UART MSP De-Initialization
 * 
 * @author User (1/9/2024)
 * 
 * @param [in]huart  - Uart handle address
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio = {0};

    if(huart->Instance == USART1) {
       
        __HAL_RCC_USART1_CLK_DISABLE();

        /*
            USUART1 GPIO Configuration
            PB6 -----> UAUART1_TX
            PB7 -----> USUART1_RX
        */

        gpio.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        HAL_GPIO_DeInit(GPIOB, gpio.Pin);
    }

}


/**
 * @fn int athw_eval_it_init(void *priv)
 * @brief Evaluation Board interface init
 * 
 * @author User (1/9/2024)
 * 
 * @param priv		- [in,out]Interface data structure   
 * 
 * @return On success, rturn EOK, negative on failure 
 */
int athw_eval_it_init(void *priv)
{
    //athw_if_handle_t *hnd = NULL;
    int ret = ERRNGATE(EFAIL);

    if(priv == NULL) {
        ret = ERRNGATE(ESNULLP);
        goto errdone;
    }

    if_hnd = (athw_if_handle_t *)priv;
   

    // Reset of all perpheral
    athw_system_hal_init();

    // Configure the system clock 
    athw_system_clock_config();


    athw_eval_gpio_init();
    athw_eval_uart_init(if_hnd->h_debuguart);
    athw_eval_hostspi_init(if_hnd->h_hostspi);
    athw_eval_tpm_dev_init(if_hnd->h_tpmspi);

    printf("ATHW Evaluation Board (Version : 0x%x) Device Init done !!! \r\n",
           ATHW_SYSTEM_VERSION);

    ret = EOK;
errdone:
    return ret;
}

/**
 * @fn athw_eval_get_handle - Get the handle pointer address for 
 *     each interface handle type.
 * 
 * @author rocke (2024-02-02)
 * 
 * @param[in] hndtype   Specified the handle type 
 * 
 * @return Return the peripheral address that use in ATHW board 
 *         interface
 */
void* athw_eval_get_handle(int hndtype) 
{
#if 0  
    void **ret = NULL;
    switch(hndtype) {
    case ATHW_HNDTYPE_HOST:
        ret = (void **)&if_hnd->h_hostspi;
        break;
    case ATHW_HNDTYPE_TPM:
        ret = (void **)&if_hnd->h_tpmspi;
        break;
    case ATHW_HNDTYPE_DBG:
        ret = (void **)&if_hnd->h_debuguart;
        break;
    default:
        printf("Unknown I/F Handle Type !!! \r\n");
        break;
    }

    return *ret;
#endif
    
    return NULL;
}


