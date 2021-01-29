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


#include "header.h"

/* Estructuras ---------------------------------------------------------------*/
struct time_t Tick;
struct pin_driver_t led;

/* Variables -----------------------------------------------------------------*/
uint32_t timeout;
uint32_t step_timeout = 1;  //1 sg

/* Funciones Privadas --------------------------------------------------------*/
void heartbeat_loop (void);

/* Funciones Públicas --------------------------------------------------------*/
/**
 * @brief inicialización del LED de vida.
 */
void heartbeat_init ()
{
    time_set(&Tick);

    Tick.seconds = timeout;

    pin_init(&led, &pin_led_dev, PIN_OUTPUT);
    pin_write(&led, 1);

    result = Add_Tarea_Asincrona (heartbeat_loop);
}

/**
 * @brief bucle infinito del heartbeat.
 */
void heartbeat_loop ()
{
    time_get (&Tick);

    if (timeout < Tick.seconds)
    {
        pin_toggle (&led);
        timeout = Tick.seconds + step_timeout;
    }
}

/**
 * @brief Configuración del tiempo del toggle del led de vida.
 * @param sg segundos que se quiere configurar.
 */
void heartbeat_config (uint32_t sg)
{
    step_timeout = sg;
}

/**
 *  @}
 */

/******************** (C) COPYRIGHT xuseto*****END OF FILE ********************/