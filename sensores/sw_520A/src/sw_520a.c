/*******************************************************************************
 * @file sw_520a.c
 * @author Jesus Nieto
 * @version 0.1.0
 * @date Creation: 14/02/2021
 * @date Last modification 14/02/2021 by Jesus Nieto 
 * @brief
 * @par
 *  COPYRIGHT NOTICE: (c) 2021 Jesus Nieto.
 *  All rights reserved
 *******************************************************************************
 *
 *  @addtogroup SW_520A
 *  @{
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "simba.h"
#include <board.h>
#include "config.h"
#include "stdint.h"
#include "SO_uC.h"
#include <stdio.h>


#include "sw_520a.h"
#include "log.h"

/* Estructuras ---------------------------------------------------------------*/
struct pin_driver_t sw_520a_gpio;

static Datos_eventos_SO_t event_data;

/* Variables -----------------------------------------------------------------*/
/**
 *  1: Objeto normal
 *  0: Objeto en el angulo defenido
 */
static uint16_t sw520a_status;

/* Decalraciones funciones privadas ------------------------------------------*/

/* Funciones privadas --------------------------------------------------------*/
/**
 * @brief Interrupción del sensor inclinometro.
 */
static void sw_520a_read_pin (void)
{
    if (sw520a_status != pin_read (&sw_520a_gpio))
    {
        sw520a_status = pin_read (&sw_520a_gpio);

        event_data.tipo_evento   = Tarea_Log;
        event_data.Datos_SO.ID   = (ID_log_e)ID_SW_520A;
        itoa (sw520a_status, (char *)event_data.Datos_SO.ptr_data);
    
        Add_Evento (&event_data);
    }
}


/* Funciones Públicas --------------------------------------------------------*/
/**
 * @brief incialización del sensor inclinometro SW 520A.
 */
void sw_520a_init ()
{
    Add_Tarea_Sincrona (sw_520a_read_pin, SO_TIMER_1);  
}

/**
 * @brief Configurar el GPIO para la lectura del sensor.
 */
void sw_520a_config (struct pin_device_t *gpio)
{
    pin_init(&sw_520a_gpio, gpio, PIN_INPUT);
}

/**
 * @brief Consulta el valor del sensor.
 */
uint16_t sw_520a_get_status ()
{
    return sw520a_status;
}

/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/