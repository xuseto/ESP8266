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
#include "log.h"


#define     strlen_EndOfLine    2

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
char http_request[] = "POST / HTTP/1.1\r\nHost: 192.168.1.107:3333\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 8\r\n\r\nsensor=5\r\n";

const char http_post[] =
            "POST / HTTP/1.1\r\n" ;
const char http_get[] =
            "GET / HTTP/1.1\r\n" ;
const char http_put[] =
            "PUT / HTTP/1.1\r\n" ;
const char http_host[] =
            "Host: " ;
const char http_type[] =
            "Content-Type: application/x-www-form-urlencoded\r\n" ;
const char http_lenght[] =
            "Content-Length: ";


const char fail[] = "ERROR al abrir el socket\r\n";

const char http_url[] = "/";
             
const char nextline [] =   "\r\n"; 

struct socket_t tcp;

/* Decalraciones funciones privadas ------------------------------------------*/
// static void        callback_send_data_http (void); //(Datos_SO_t *data);
wifi_faults_t      send_data_socket        (http_send_data_t *http_data);

/* Funciones privadas --------------------------------------------------------*/
static void add_new_event ()
{ 
    static Datos_eventos_SO_t datos_evento;

    datos_evento.tipo_evento   = TASK_LOG;
    datos_evento.Datos_SO.ID   = 0x00;
    memcpy (&datos_evento.Datos_SO.ptr_data, &http_request, sizeof(datos_evento.Datos_SO.ptr_data));
    
    Add_Evento (&datos_evento);

}


static void callback_send_data_http (Datos_SO_t *data)
{
    http_send_data_t new_data_http;
    uint8_t nombre_dato[] = "sensor";

//    memcpy (&new_data_http, data, sizeof(new_data_http));
   new_data_http.data = 0x05;
   memcpy (&new_data_http.name_data, &nombre_dato, sizeof(nombre_dato));
   new_data_http.http_method = POST;
   
    send_data_socket (&new_data_http); 
}

wifi_faults_t send_data_socket (http_send_data_t *http_data)
{
    uint16_t I, size_sprintf,ptr;
    char http_TEMP[40];
    char ip[15];

    // Inidicar que se va a realizar una petición POST
    // I = 0;
    // if (POST == http_data->http_method)
    // {
    //     memcpy (&http_request[I], &http_post, strlen(http_post));
    //     I += strlen(http_post)+0;
    // }
    // else
    // if (PUT == http_data->http_method)
    // {
    //     memcpy (&http_request[I], &http_put, strlen(http_put));
    //     I += strlen(http_put)+0;
    // } 
    // else
    // {
    //     memcpy (&http_request[I], &http_get, strlen(http_get));
    //     I += strlen(http_get)+0;
    // }
    
    // //  Indicar la IP del servidor
    // inet_ntoa (&wifi.ip_server.ip, &ip[0]);
    // memcpy (&http_TEMP, &http_host, sizeof(http_host));
    // size_sprintf = strlen(http_host);
    // memcpy (&http_TEMP[size_sprintf], &ip, sizeof(ip));
    // size_sprintf += strlen(ip);
    // http_TEMP[size_sprintf] = ':';
    // size_sprintf += 1;
    // memset (&ip, 0x00, sizeof(ip));
    // itoa (wifi.ip_server.port, ip);
    // memcpy (&http_TEMP[size_sprintf], &ip, sizeof(ip));
    // ptr = 0x00;
    // while ('\0' != ip[ptr])
    // {
    //     size_sprintf ++;
    //     ptr ++;
    // }
    // // memcpy (&http_TEMP[size_sprintf], &http_url, sizeof(http_url));
    // // size_sprintf += strlen(http_url); 
    // memcpy (&http_TEMP[size_sprintf], &nextline, sizeof(nextline));
    // size_sprintf += strlen(nextline); 
    // memcpy (&http_request[I], &http_TEMP, size_sprintf);
    // I += size_sprintf;

    // // Indicar el tipo de que contiene los datos
    // memcpy (&http_request[I], &http_type, strlen(http_type));
    // I += strlen(http_type)+0;
   
    // // Indicar el tamaño de los datos
    // memcpy (&http_request[I], &http_lenght, sizeof(http_lenght));
    // I += strlen(http_lenght);
    // size_sprintf = 12;
    // //itoa ((size_sprintf-strlen_EndOfLine),ip);
    // itoa ((size_sprintf),ip);
    // memcpy (&http_request[I], &ip, sizeof(ip));
    // ptr = 0x00;
    // while ('\0' != ip[ptr])
    // {
    //     I ++;
    //     ptr ++;
    // } 
    // memcpy (&http_request[I], &nextline, sizeof(nextline));
    // I += strlen(nextline);
   
    // memcpy (&http_request[I], &nextline, sizeof(nextline));
    // I += strlen(nextline);


    // //  Preparamos la cadena de caractes de los datoas a enviar
    // size_sprintf = 0x00; 
    // memcpy (&http_TEMP[size_sprintf], &http_data->name_data[0], sizeof(http_data->name_data));
    // ptr = 0x00;
    // while ('\0' != http_data->name_data[ptr])
    // {
    //     size_sprintf ++;
    //     ptr ++;
    // } 
    // http_TEMP[size_sprintf] = '=';
    // size_sprintf += 1;
    // itoa (http_data->data, ip);
    // memcpy (&http_TEMP[size_sprintf], &ip, sizeof(ip));
    // ptr = 0x00;
    // while ('\0' != ip[ptr])
    // {
    //     size_sprintf ++;
    //     ptr ++;
    // } 
    // memcpy (&http_TEMP[size_sprintf], &nextline, sizeof(nextline));
    // size_sprintf += strlen(nextline);
    // memcpy (&http_request[I], &http_TEMP, size_sprintf);
    // I += size_sprintf;

    

    // // Separación de la cabecerda de los datos
    // memcpy (&http_request[I], &nextline, sizeof(nextline));
    // I += strlen(nextline);
    // // memcpy (&http_request[I], &nextline, sizeof(nextline));
    // // I += strlen(nextline);


    // Se envia por TCP los datos 
    socket_open_tcp(&tcp);
    socket_connect(&tcp, &wifi.ip_server);

    socket_write(&tcp, &http_request,  sizeof(http_request));

    //while (!socket_size(&tcp));
    socket_close(&tcp);


    // if (socket_connect(&tcp, &wifi.ip_server) != 0) 
    //     return Conectar_Socket;  

    // // Enviamos los datos por el socket
    // if (socket_write(&tcp, &http_request,  I) != I)
    //     return Size_Enviar_Socket;
   
    // // Esperar recibir la respuesta del servidor
    // // WARNING: NO se trata la respuesta del servidor
    //while (!socket_size(&tcp));

    // // cerrar socket
    //if (socket_close(&tcp) != 0)  
    //    return Cerrar_Socket;  

    // // Todo OK
    add_new_event ();
    
    return SinErrores;
}


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
    esp_wifi_set_op_mode(esp_wifi_op_mode_station_t);

    // Configuramos el IP u Puerto del servidor
    // IP del servidor
    if( inet_aton(STRINGIFY(REMOTE_HOST_IP),  &wifi.ip_server.ip) != 0) 
    {
        std_printf(FSTR("Bad ip address '%s'.\r\n"), STRINGIFY(REMOTE_HOST_IP));
        return 1;
    }
    
    wifi.ip_server.port = (uint16_t)SOCKET_HOST;
    // IP de la red WIFI
    if( inet_aton(STRINGIFY(LOCAL_IP),   &wifi.ip_localhost.address) != 0) 
    {
        std_printf(FSTR("Bad ip address '%s'.\r\n"), STRINGIFY(REMOTE_HOST_IP));
        return 1;
    }
    if( inet_aton(STRINGIFY(GATEWAY_IP), &wifi.ip_localhost.gateway) != 0) 
    {
        std_printf(FSTR("Bad gateway '%s'.\r\n"), STRINGIFY(REMOTE_HOST_IP));
        return 1;
    }
    if( inet_aton(STRINGIFY(MASCARA_IP), &wifi.ip_localhost.netmask) != 0) 
    {
        std_printf(FSTR("Bad ip mask '%s'.\r\n"), STRINGIFY(REMOTE_HOST_IP));
        return 1;
    }
    // Nombre de la red y contraseña de la red que se quiere conectar
    wifi.ssid = (char *)REDWIFI;
    wifi.pssw = (char *)PSSWWIFI;

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

    // Registramos la tarea para enviar los datos HTTP
    Add_Tarea_Asincrona (callback_send_data_http, (ID_tareas_SO_e)TASK_HTTP);  
        
    return 0;
}


/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/