/*******************************************************************************
 * @file wifi.h
 * @author Jesus Nieto 
 * @version 0.1.0
 * @date Creation: 07/03/2021
 * @date Last modification 07/03/2021 by Jesus Nieto 
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
    @details configuración del servidor o cliente HTTP mediante WIFI

*/

#ifndef WIFI_H
#define WIFI_H

#include "stdint.h"
#include "simba.h"

/* Define --------------------------------------------------------------------*/

/* Estructuras ----------------------------------------------------------------*/
/**
 * @brief Enumerado del metodo de envio de los datos
 */
typedef enum 
 {
     GET,
     POST,
     PUT,
     MAX_METHOD
 } http_method_t;

/**
 *  @brief Estructura para enviar los datos al servidor 
 */
typedef struct  
{
    uint8_t            name_data[10];
    uint32_t           data;
    http_method_t      http_method;
} http_send_data_t;

/**
 *  @brief Estructura para configuración del Wifi del ESP32
 */
typedef struct  sconfig_wifi
{
    struct inet_addr_t          ip_server;
    struct inet_if_ip_info_t    ip_localhost; 
    char                        *ssid;
    char                        *pssw; 
    char                        *name_server;   
} wifi_config_t;

/* SFM functions -------------------------------------------------------------*/
void             wifi_init       (void);
uint16_t         wifi_config     (const wifi_config_t *new_wifi);
wifi_config_t*   get_struct_wifi (void);

#endif

/**
 *  @}
 */

/****************** (C) COPYRIGHT Jesus Nieto****END OF FILE ******************/