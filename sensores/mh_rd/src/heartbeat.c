/*******************************************************************************
 * @file heartbeat.c
 * @author Jesus Nieto
 * @version 0.1.0
 * @date Creation: 01/02/2021
 * @date Last modification 01/02/2021 by Jesus Nieto 
 * @brief
 * @par
 *  COPYRIGHT NOTICE: (c) 2021 Jesus Nieto.
 *  All rights reserved
 *******************************************************************************
 *
 *  @addtogroup HEARTBEAT
 *  @{
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "simba.h"
#include "config.h"
#include "stdint.h"
#include "SO_uC.h"
#include <stdio.h>


#include "heartbeat.h"
#include "log.h"

/* Estructuras ---------------------------------------------------------------*/
struct pin_driver_t led;

/* Variables -----------------------------------------------------------------*/

/* Decalraciones funciones privadas ------------------------------------------*/

/* Funciones privadas --------------------------------------------------------*/
/**
 * @brief Funcion sincrona, llamada por el SO cada sg.
 */
static void heartbeat_loop  (void)
{
    pin_toggle (&led);
}

/* Funciones Públicas --------------------------------------------------------*/
/**
 * @brief inicialización del LED de vida.
 */
void heartbeat_init ()
{
    pin_init(&led, &pin_led_dev, PIN_OUTPUT);
    pin_write(&led, 1);

    Add_Tarea_Sincrona (heartbeat_loop, SO_TIMER_0);
}


/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/