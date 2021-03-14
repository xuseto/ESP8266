/*******************************************************************************
 * @file wifi.c
 * @author Jesus Nieto
 * @version 0.1.0
 * @date Creation: 07/03/2021
 * @date Last modification 14/03/2021 by Jesus Nieto 
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
#include "wifi/wifi_task.h"

/* Estructuras ---------------------------------------------------------------*/
wifi_config_t wifi;

/* Variables -----------------------------------------------------------------*/

/* Decalraciones funciones privadas ------------------------------------------*/

/* Funciones privadas --------------------------------------------------------*/
static void callback_send_data_http (Datos_SO_t *data)
{
    http_send_data_t new_data_http;
    memcpy (&new_data_http, data->ptr_data, sizeof(new_data_http));

    new_data_http.http_method = data->ID;

    if (send_data_socket (&new_data_http, &wifi))
        std_printf ("FAIL send data \r\n"); 
}
/* Funciones públicas --------------------------------------------------------*/
/**
 * @brief  inicialización del cliente wifi
 */
void wifi_init ()
{    
    //  Iniciar modulos de Wifi y del socket
    esp_wifi_module_init ();
    socket_module_init ();

    // Configuracion e inicio del modulo WIFI
    esp_wifi_set_op_mode(esp_wifi_op_mode_station_softap_t);

    // Registramos la tarea para enviar los datos HTTP
    //wifi_new_task();
    Add_Tarea_Asincrona (callback_send_data_http, (ID_tareas_SO_e)TASK_HTTP); 
}

/**
 * @brief  Configuracion de los parametros para generar al red WiFi
 */
uint16_t wifi_config (const wifi_config_t *new_wifi)
{   
    wifi = *new_wifi;

    // Configuracion de la red local wifi a la cual se va conectar
    if (esp_wifi_station_init(wifi.ssid, wifi.pssw, NULL, NULL) != 0) 
    {
        std_printf (FSTR("Error red local \r\n"));
        std_printf (FSTR("SSID: %s \r\n"),wifi.ssid);
        std_printf (FSTR("PPSW: %s \r\n"),wifi.pssw);
        return 1 ; 
    }

    // Conectar el modulo WIFI del ESP8266
    if (esp_wifi_station_connect () != 0)
    {
        std_printf (FSTR("Conectar el modulo WIFI del ESP8266 \r\n"));
        return 1 ; 
    }
   
    return 0;
}

wifi_config_t*   get_struct_wifi (void)
{
    return &wifi;
}

/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/