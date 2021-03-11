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

/* Define --------------------------------------------------------------------*/
#define REMOTE_HOST_IP 192.168.1.107
#define SOCKET_HOST    3000 //8086

#define LOCAL_IP       192.168.1.199
#define GATEWAY_IP     192.168.1.1
#define MASCARA_IP     255.255.255.0

#define REDWIFI        "YOUR_WIFI"
#define PSSWWIFI       "YOUR_PASSWORD"

/* Estructuras ----------------------------------------------------------------*/
/**
*******************************************************************************
\struct         eErroresWifi
\brief          Enum con los diferentes errores en el modulo WIFI
******************************************************************************/
typedef enum  
{
    /** devuelve un 0 si ha ido todo bien   */
    SinErrores = 0,
    /** Error producido al configurar la Red Local WIFI */
    Configuracion_RedWifi,
    /** Error al intentar conectar el modulo de WIFI */
    Conexion_ModuloWifi,
    /** Error al intentar enviar datos mayor que esta permitido
     *  Para arreglar este error, modificar el numero de char del arrya 
     * "http_request" o cambiar el nombre del parametro por uno más corto
    */
    Size_Data,
    /** Error al intentar abrir el Socket del servidor*/
    Abrir_Socket,
    /** Error al intentar conectar con el Socket del servidor*/
    Conectar_Socket,
    /** Error al enviar el numero de char por el Socket*/
    Size_Enviar_Socket,
    /** Sin respuesta del servidor  */
    SinRespuesta_Socket,
    /** Error al intentar cerrar el Socket del servidor*/
    Cerrar_Socket,
    
} wifi_faults_t;

/**
 * @brief Enumerado del metodo de envio de los datos
 */
typedef enum 
 {
     POST,
     GET,
     MAX_METHOD
 } http_method_t;

/**
 *  @brief Estructura para enviar los datos al servidor 
 */
typedef struct  
{
    uint8_t            name_data[10];
    int32_t            data;
    http_method_t      http_method;
} http_send_data_t;

/* SFM functions -------------------------------------------------------------*/
wifi_faults_t    init_client_wifi        (void);

#endif

/**
 *  @}
 */

/****************** (C) COPYRIGHT Jesus Nieto****END OF FILE ******************/