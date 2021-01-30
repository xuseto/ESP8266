/*******************************************************************************
 * @file heartbeat.c
 * @author xuseto
 * @version 0.0.0.0.1
 * @date Creation: 29/01/2021
 * @date Last modification 29/01/2021 
 * @brief
 * @par
 *  COPYRIGHT NOTICE: (c) 2021 Xuseto.
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


#include "heartbeat.h"

/* Estructuras ---------------------------------------------------------------*/
struct pin_driver_t led;


/* Variables -----------------------------------------------------------------*/


/* Funciones Privadas --------------------------------------------------------*/
//void heartbeat_loop  (void) ;

/* Funciones Públicas --------------------------------------------------------*/
/**
 * @brief bucle infinito del heartbeat.
 */
void heartbeat_loop  (void *arg_p)
{
    pin_toggle (&led);
}


/**
 * @brief inicialización del LED de vida.
 */
void heartbeat_init ()
{

    pin_init(&led, &pin_led_dev, PIN_OUTPUT);
    pin_write(&led, 1);
}





/**
 *  @}
 */

/******************** (C) COPYRIGHT xuseto*****END OF FILE ********************/