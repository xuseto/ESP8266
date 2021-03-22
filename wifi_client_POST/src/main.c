/*******************************************************************************
 * @file main.c
 * @author Jesus Nieto
 * @version 0.1.0
 * @date Creation: 01/02/2021
 * @date Last modification 07/03/2021 by Jesus Nieto 
 * @brief
 * @par
 *  COPYRIGHT NOTICE: (c) 2021 Jesus Nieto.
 *  All rights reserved
 *******************************************************************************
 *
 *  @addtogroup MAIN
 *  @{
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "simba.h"
#include "config.h"
#include "SO_uC.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "heartbeat.h"
#include "log.h"
#include "wifi/wifi.h"

/* Defines -------------------------------------------------------------------*/
#define REMOTE_HOST_IP 192.168.1.107
#define SOCKET_HOST    3333

#define LOCAL_IP       192.168.1.199
#define GATEWAY_IP     192.168.1.1
#define MASCARA_IP     255.255.255.0

#define REDWIFI        "MIWIFI_2G_eS9k"
#define PSSWWIFI       "UeS9kC9N"
#define SERVER_ESP32   "ESP32"

/* Estructuras ---------------------------------------------------------------*/
wifi_config_t wifi_main;

/* Variables -----------------------------------------------------------------*/

/* Decalraciones funciones privadas ------------------------------------------*/

/* Funciones privadas --------------------------------------------------------*/

/* Funciones públicas --------------------------------------------------------*/
/**
 * @brief  Main. inicio del código
 * @return int
 */
int main()
{

    sys_start();
    Ini_Tareas ();

    heartbeat_init ();
    log_init ();
    wifi_init();

    // IP del servidor
    if( inet_aton(STRINGIFY(REMOTE_HOST_IP),  &wifi_main.ip_server.ip) != 0) 
    {
        std_printf(FSTR("Bad ip address '%s'.\r\n"), STRINGIFY(REMOTE_HOST_IP));
        return 1;
    }
    
    wifi_main.ip_server.port = (uint16_t)SOCKET_HOST;
    // IP de la red WIFI
    if( inet_aton(STRINGIFY(LOCAL_IP),   &wifi_main.ip_localhost.address) != 0) 
    {
        std_printf(FSTR("Bad ip address '%s'.\r\n"), STRINGIFY(REMOTE_HOST_IP));
        return 1;
    }
    if( inet_aton(STRINGIFY(GATEWAY_IP), &wifi_main.ip_localhost.gateway) != 0) 
    {
        std_printf(FSTR("Bad gateway '%s'.\r\n"), STRINGIFY(REMOTE_HOST_IP));
        return 1;
    }
    if( inet_aton(STRINGIFY(MASCARA_IP), &wifi_main.ip_localhost.netmask) != 0) 
    {
        std_printf(FSTR("Bad ip mask '%s'.\r\n"), STRINGIFY(REMOTE_HOST_IP));
        return 1;
    }
    // Nombre de la red y contraseña de la red que se quiere conectar
    wifi_main.ssid = (char *)REDWIFI;
    wifi_main.pssw = (char *)PSSWWIFI;
    wifi_main.name_server = (char *)SERVER_ESP32;

    if (wifi_config (&wifi_main))
    {
        std_printf (FSTR("Error al conectar a la red wifi \r\n"));
        while (1);
    }
        
    std_printf (FSTR("Configuracion ESP32 finalizada \r\n"));
    while (1) 
    {
        Run_Tareas ();
    }
    
    return (0);
}

/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/