/*******************************************************************************
 * @file wifi.c
 * @author Jesus Nieto
 * @version 0.1.0
 * @date Creation: 07/03/2021
 * @date Last modification 07/03/2021 by Jesus Nieto 
 * @brief
 * @par
 *  COPYRIGHT NOTICE: (c) 2021 Jesus Nieto.
 *  All rights reserved
 *******************************************************************************
 *
 *  @addtogroup WIFI
 *  @{
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "simba.h"
#include "config.h"
#include "string.h"
#include "SO_uC.h"


#include "wifi/wifi.h"

/* Estructuras ---------------------------------------------------------------*/

/**
 *  @brief Estructura para configuración del Wifi del ESP32
 */
typedef struct  sconfig_wifi
{
    struct inet_addr_t          ip_server;
    struct inet_if_ip_info_t    ip_localhost; 
    char                        *ssid;
    char                        *pssw;   
} config_wifi_t;

config_wifi_t wifi;

/* Variables -----------------------------------------------------------------*/
char http_request[200];
char http_post[] =
            "POST / HTTP/1.1\r\n" ;
char http_get[] =
            "GET / HTTP/1.1\r\n" ;
// char http_host[] =
//             "Host: 192.168.1.200:3001\r\n" ;
char http_type[] =
            "Content-Type: application/x-www-form-urlencoded\r\n" ;
// char http_lenght[] =
//             "Content-Length: 15\r\n";
// char http_data[] =
//             "Temperatura=116\r\n";
             
char nextline [] =   "\r\n"; 

struct socket_t tcp;

/* Decalraciones funciones privadas ------------------------------------------*/

/* Funciones privadas --------------------------------------------------------*/

/* Funciones públicas --------------------------------------------------------*/
/**
 * @brief  inicialización del cliente wifi
 */
wifi_faults_t init_client_wifi ()
{    
    //  Iniciar modulos de Wifi y del socket
    esp_wifi_module_init ();
    socket_module_init ();

    // Configuracion e inicio del modulo WIFI
    esp_wifi_set_op_mode(esp_wifi_op_mode_max_t);

    // Configuramos el IP u Puerto del servidor
    // IP del servidor
    inet_aton(STRINGIFY(REMOTE_HOST_IP),  &wifi.ip_server.ip);
    wifi.ip_server.port = SOCKET_HOST;
    // IP de la red WIFI
    inet_aton(STRINGIFY(LOCAL_IP),   &wifi.ip_localhost.address);
    inet_aton(STRINGIFY(GATEWAY_IP), &wifi.ip_localhost.gateway);
    inet_aton(STRINGIFY(MASCARA_IP), &wifi.ip_localhost.netmask);
    // Nombre de la red y contraseña de la red que se quiere conectar
    wifi.ssid = (char *)REDWIFI;
    wifi.pssw = (char *)PSSWWIFI;

    // Configuracion de la red local wifi a la cual se va conectar
    if (esp_wifi_station_init(&wifi.ssid[0],&wifi.pssw[0], NULL, &wifi.ip_localhost) != 0) 
       return 1 ; 

    // Conectar el modulo WIFI del ESP8266
    if (esp_wifi_station_connect () != 0)
        return 1;        
        
    return 0;
}

wifi_faults_t send_data_socket (http_send_data_t *http_data)
{
    uint16_t I, size_sprintf;
    char http_Datos[30];
    char http_TEMP[40];
    char ip[15];

    // Inidicar que se va a realizar una petición POST
    I = 0;
    if (POST == http_data->http_method)
    {
        memcpy (&http_request[I], &http_post, strlen(http_post));
        I += strlen(http_post)+0;
    }
    else
    {
        memcpy (&http_request[I], &http_get, strlen(http_get));
        I += strlen(http_get)+0;
    }
    
    //  Indicar la IP del servidor
    inet_ntoa (&wifi.ip_server.ip, ip);
    size_sprintf = sprintf(http_TEMP,"Host: %s:%d\r\n",ip,wifi.ip_server.port);
    memcpy (&http_request[I], &http_TEMP, size_sprintf);
    I += size_sprintf+0;

    // Indicar el tipo de que contiene los datos
    memcpy (&http_request[I], &http_type, strlen(http_type));
    I += strlen(http_type)+0;
   
    //  Preparamos la cadena de caractes de los datoas a enviar
    size_sprintf = sprintf(http_Datos,"%s=%d\r\n",http_data->name_data,http_data->data);
    // Indicar el tamaño de los datos
    sprintf(http_TEMP,"Content-Length: %d\r\n",(size_sprintf));
    memcpy (&http_request[I], &http_TEMP, strlen(http_TEMP));
    I += strlen(http_TEMP)+0;

    // Separación de la cabecerda de los datos
    memcpy (&http_request[I], &nextline, strlen(nextline));
    I += strlen(nextline)+0;

    // Encolar los datos a transmitir
    memcpy (&http_request[I], &http_Datos, size_sprintf);
    I += size_sprintf+0;

    if (I > sizeof(http_request))
        return Size_Data;
    //
    // Se envia por TCP los datos
    //
    if (socket_open_tcp(&tcp) != 0)  
        return Abrir_Socket; 

    if (socket_connect(&tcp, &wifi.ip_server) != 0) 
        return Conectar_Socket;  

    // Enviamos los datos por el socket
    if (socket_write(&tcp, &http_request,  I) != I)
        return Size_Enviar_Socket;
   
    // Esperar recibir la respuesta del servidor
    // WARNING: NO se trata la respuesta del servidor
    while (!socket_size(&tcp));

    // cerrar socket
    if (socket_close(&tcp) != 0)  
        return Cerrar_Socket;  

    // Todo OK
    return SinErrores;
}

/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/