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

/* Estructuras ---------------------------------------------------------------*/

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
    if (init_client_wifi())
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