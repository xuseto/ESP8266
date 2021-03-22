/*******************************************************************************
 * @file mh_rd.c
 * @author Jesus Nieto
 * @version 0.1.0
 * @date Creation: 22/03/2021
 * @date Last modification 22/03/2021 by Jesus Nieto 
 * 
 * @brief El sensor de lluvia detecta el agua que completa los circuitos en las 
 * pistas impresos de sus tarjetas de sensores. La tarjeta del sensor actúa como
 * una resistencia variable que cambiará de 100kohm cuando está mojado a 
 * 2Mohm cuando está seco. En resumen, cuanto más húmedo sea el tablero, 
 * mayor será la corriente que se llevará a cabo.
 * Leer más:
 *  https://www.microjpm.com/products/mh-rd-rain-detector-module-sensor/
 * 
 * @par
 *  COPYRIGHT NOTICE: (c) 2021 Jesus Nieto.
 *  All rights reserved
 *******************************************************************************
 *
 *  @addtogroup MH_RD
 *  @{
 *
 *      -------------                --------------
 *        MH_RD     |                |   ESP8266    
 *              Vcc |----------------| 3V
 *              GND |----------------| G
 *                  |                |
 *              AO  |----------------| A0 
 *                  |                |
 *     --------------                --------------
 */

/* Includes ------------------------------------------------------------------*/
#include "simba.h"
#include "config.h"
#include "stdint.h"
#include "SO_uC.h"
#include <stdio.h>

#include "mh_rd.h"
#include "log.h"

/* Defines -------------------------------------------------------------------*/
#define DELAY_READ_ADC     5 //sg.

/* Estructuras ---------------------------------------------------------------*/
char state_0[] = "NOT RAINING";
char state_1[] = "RAINING";
char state_2[] = "FLOOD";

struct analog_input_pin_t adc;

/* Variables -----------------------------------------------------------------*/
uint16_t delay;
uint16_t value_adc;

/* Declaraciones funciones privadas ------------------------------------------*/
static void mh_rd_task    (void);
static void add_new_event (void);

/* Funciones privadas --------------------------------------------------------*/
/**
 * @brief Task sincrona cada segundo, pero se comprueba el estado del sensor
 * cada 5 segundos.
 */
static void mh_rd_task ()
{
     if (DELAY_READ_ADC < delay)
     {
         delay = 0x00;
         value_adc = analog_input_pin_read(&adc);
         add_new_event ();
     }
     else
     {
         delay ++;
     }
}

/**
 * @brief Crear nuevo evento al SO del uC.
 */
static void add_new_event ()
{  
    Datos_eventos_SO_t data_event;

    data_event.tipo_evento   = LOG_TASK;
    data_event.Datos_SO.ID   = (ID_log_e)ID_MH_RD;
    if (value_adc >= 900)
       memcpy (&data_event.Datos_SO.ptr_data, &state_0[0], sizeof(state_0)); 
    else if (value_adc > 400)
        memcpy (&data_event.Datos_SO.ptr_data, &state_1[0], sizeof(state_1));
    else
        memcpy (&data_event.Datos_SO.ptr_data, &state_2[0], sizeof(state_2));
    
    Add_Evento (&data_event);
}

/* Funciones Públicas --------------------------------------------------------*/
/**
 * @brief inicialización del sensro de lluvia MH RD.
 */
void mh_rd_init ()
{
    analog_input_pin_module_init();
    if (analog_input_pin_init(&adc, &pin_a0_dev) != 0) {
        std_printf(FSTR("Failed to initialize the analog input pin.\r\n"));
        while (1);
    }


    Add_Tarea_Sincrona (mh_rd_task, SO_TIMER_0);
}


/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/