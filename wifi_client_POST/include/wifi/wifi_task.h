/*******************************************************************************
 * @file wifi.h
 * @author Jesus Nieto 
 * @version 0.1.0
 * @date Creation: 14/04/2021
 * @date Last modification 14/04/2021 by Jesus Nieto 
 * @brief
 * @par
 *  COPYRIGHT NOTICE: (c) 2021 Jesus Nieto.
 *  All rights reserved
 *******************************************************************************

    @addtogroup WIFI
    @{
    @defgroup WIFI 
    @{
    @brief
    @details configuración de las funciones del task del SO_uc

*/

#ifndef WIFI_TASK_H
#define WIFI_TASK_H

#include "stdint.h"
#include "simba.h"
#include "wifi/wifi.h"

/* Define --------------------------------------------------------------------*/

/* Estructuras ----------------------------------------------------------------*/

/* SFM functions -------------------------------------------------------------*/
uint16_t send_data_socket (http_send_data_t *http_data, wifi_config_t   *wifi_task);

#endif

/**
 *  @}
 */

/****************** (C) COPYRIGHT Jesus Nieto****END OF FILE ******************/