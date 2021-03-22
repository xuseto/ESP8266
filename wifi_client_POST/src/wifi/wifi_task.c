/*******************************************************************************
 * @file wifi_task.c
 * @author Jesus Nieto
 * @version 0.1.0
 * @date Creation: 14/04/2021
 * @date Last modification 14/04/2021 by Jesus Nieto 
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
#include "log.h"

/* Estructuras ---------------------------------------------------------------*/
wifi_config_t   *wifi_task;
struct socket_t tcp_task;

/* Variables -----------------------------------------------------------------*/
char http_post[] = "POST";
char http_put[]  = "PUT";
char http_get[]  = "GET";

/* Decalraciones funciones privadas ------------------------------------------*/
static void   add_new_event           (http_send_data_t *http_data);

/* Funciones privadas --------------------------------------------------------*/

static void add_new_event (http_send_data_t *http_data)
{ 
    Datos_eventos_SO_t datos_evento;
    char string_log[20];

    std_sprintf (string_log, "%s=%lu", http_data->name_data, http_data->data);
    memcpy (&datos_evento.Datos_SO.ptr_data, &string_log, sizeof(string_log));
    datos_evento.tipo_evento   = TASK_LOG;
    datos_evento.Datos_SO.ID   = 0x00;
        
    Add_Evento (&datos_evento);
}

/* Funciones públicas --------------------------------------------------------*/
uint16_t send_data_socket (http_send_data_t *http_data, wifi_config_t *wifi_task)
{
    char http_request[200] = {0};
    char method[4], data[20] = {0};
    char ip[16];
    uint16_t size_data;

    if (GET == http_data->http_method)
        sprintf (method, "%s", http_get);
    else if (PUT == http_data->http_method)
        sprintf (method, "%s", http_put);
    else if (POST == http_data->http_method)
        sprintf (method, "%s", http_post);
    else
        return 1;

    inet_ntoa(&wifi_task->ip_server.ip, &ip[0]);

    size_data = std_sprintf (data,"%s=%lu", http_data->name_data, http_data->data);
    
    std_sprintf (http_request, 
"POST / HTTP/1.1\r\n\
Host: %s:%d\r\n\
Content-Type: application/x-www-form-urlencoded\r\n\
Content-Length: %d\r\n\r\n%s",
                ip,
                &wifi_task->ip_server.port,
                size_data,
                data);

    // Se envia por TCP los datos 
    socket_open_tcp(&tcp_task);
    socket_connect(&tcp_task, &wifi_task->ip_server);

    socket_write(&tcp_task, &http_request,  sizeof(http_request));

    //while (!socket_size(&tcp));
    socket_close(&tcp_task);

    // Todo OK
    add_new_event (http_data); 
    return 0;
}

/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/