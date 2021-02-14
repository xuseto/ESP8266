/*******************************************************************************
 * @file sw_520d.c
 * @author Jesus Nieto
 * @version 0.1.0
 * @date Creation: 14/02/2021
 * @date Last modification 14/02/2021 by Jesus Nieto 
 * 
 * @brief Un sensor de inclinación es un dispositivo que proporciona una señal * digital en caso de que su inclinación supere un umbral. Este tipo de sensor * no permite saber el grado de inclinación del dispositivo, simplemente actúa * como un sensor que se cierra a partir de una cierta inclinación.
 *     Sensores Tilt de doble esfera. Se dispone de un cilindro cuya pared 
 *  constituye un contacto eléctrico, mientras que el otro contacto esta 
 * localizado en el centro de la base. Al inclinar lo suficiente el dispositivo
 *  ambas esferas constituyen un puente entre ambos contactos, cerrando el 
 * circuito.
 * 
 * @par
 *  COPYRIGHT NOTICE: (c) 2021 Jesus Nieto.
 *  All rights reserved
 *******************************************************************************
 *
 *  @addtogroup SW_520D
 *  @{
 *
 *      -------------                --------------
 *        sw_520d   |                |   ESP8266    
 *              Vcc |----------------| 3V
 *              GND |----------------| G
 *                  |                |
 *              OUT |----------------| D2 (Para este ejemplo)
 *                  |                |
 *     --------------                --------------
 */

/* Includes ------------------------------------------------------------------*/
#include "simba.h"
#include <board.h>
#include "config.h"
#include "stdint.h"
#include "SO_uC.h"
#include <stdio.h>


#include "sw_520d.h"
#include "log.h"

/* Estructuras ---------------------------------------------------------------*/
struct pin_driver_t sw_520d_gpio;

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
static void sw_520d_read_pin (void)
{
    if (sw520a_status != pin_read (&sw_520d_gpio))
    {
        sw520a_status = pin_read (&sw_520d_gpio);

        event_data.tipo_evento   = Tarea_Log;
        event_data.Datos_SO.ID   = (ID_log_e)ID_SW_520D;
        itoa (sw520a_status, (char *)event_data.Datos_SO.ptr_data);
    
        Add_Evento (&event_data);
    }
}


/* Funciones Públicas --------------------------------------------------------*/
/**
 * @brief incialización del sensor inclinometro SW 520A.
 */
void sw_520d_init ()
{
    Add_Tarea_Sincrona (sw_520d_read_pin, SO_TIMER_1);  
}

/**
 * @brief Configurar el GPIO para la lectura del sensor.
 */
void sw_520d_config (struct pin_device_t *gpio)
{
    pin_init(&sw_520d_gpio, gpio, PIN_INPUT);
}

/**
 * @brief Consulta el valor del sensor.
 */
uint16_t sw_520d_get_status ()
{
    return sw520a_status;
}

/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/