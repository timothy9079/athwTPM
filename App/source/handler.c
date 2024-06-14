/********************************************************************************
 * Copyright (C) 2020 by Trustkey                                               * 
 * This file is part of  Project                                                *   
 * This software contains confidential information of TrustKey Co.,Ltd.         *
 * and unauthorized distribution of this software, or any portion of it, are    *
 * prohibited.                                                                  *
 ********************************************************************************/

/**
 * @file handler.c
 * Exception and callbac handler 
 *
 * @anchor ATHW_SYSTEM
 * @author User, (c) Trustkey
 * @version Draft 
 * @date 010924
 * @bug No known bugs.
 * @note 
 * 
 * @copyright Copyright 2020 Trustkey. All rights reserved*
 */         
#include "stm32l475xx.h"

//#if	defined (__ICCARM__)
//#elif 	defined (__GNUC__)
//#else
//#endif


void error_handler(void *priv)
{
    (void)priv;

	printf(" ~~~ ERROR ~~~ \r\n");

    __disable_irq();
	 while(1) {
        // TODO : describe the error related description
	 }
}



