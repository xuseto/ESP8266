/*******************************************************************************
 * @file ldr.c
 * @author Jesus Nieto
 * @version 0.1.0
 * @date Creation: 22/03/2021
 * @date Last modification 22/03/2021 by Jesus Nieto 
 * 
 * @brief El módulo de sensor LDR se utiliza para detectar la intensidad de la 
 * luz. Está asociado con el pin de salida analógica y el pin de salida digital 
 * etiquetados como AO y DO respectivamente en la placa. Cuando hay luz, la 
 * resistencia de LDR se reducirá según la intensidad de la luz. Cuanto mayor 
 * sea la intensidad de la luz, menor será la resistencia del LDR. El sensor 
 * tiene una perilla de potenciómetro que se puede ajustar para cambiar la 
 * sensibilidad de LDR hacia la luz.
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
 *              DO  |----------------| D4
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

#include "ldr.h"
#include "log.h"

/* Defines -------------------------------------------------------------------*/
#define DELAY_READ_ADC     1 //sg.
#define MIN_VALUE          60
#define GPIO_IN            GPIO_D4
         

/* Estructuras ---------------------------------------------------------------*/
struct analog_input_pin_t adc;
struct pin_driver_t       ldr_gpio;

/* Variables -----------------------------------------------------------------*/
char state_0[] = "DAY";
char state_1[] = "NIGHT";

uint16_t delay;
uint16_t value_adc;

/* Declaraciones funciones privadas ------------------------------------------*/
static void ldr_task      (void);
static void add_new_event (void);

/* Funciones privadas --------------------------------------------------------*/
/**
 * @brief Task sincrona cada segundo, pero se comprueba el estado del sensor
 * cada 5 segundos.
 */
static void ldr_task ()
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
    char log[30] = {0};
    Datos_eventos_SO_t data_event;

    data_event.tipo_evento   = LOG_TASK;
    data_event.Datos_SO.ID   = (ID_log_e)ID_MH_RD;

    if (MIN_VALUE > value_adc)
        value_adc = 100;
    else
        value_adc = -0.1162*value_adc + 107;

    std_sprintf (log, "Light to %d ", value_adc);
    if (pin_read(&ldr_gpio))
        std_sprintf (log, "Light to %d%% is %s", value_adc,state_1);
    else
        std_sprintf (log, "Light to %d%% is %s", value_adc,state_0);
    
    memcpy (&data_event.Datos_SO.ptr_data, &log[0], sizeof(log));
    //itoa (value_adc, (char *)data_event.Datos_SO.ptr_data);
    Add_Evento (&data_event);
}

/* Funciones Públicas --------------------------------------------------------*/
/**
 * @brief inicialización del sensro de lluvia MH RD.
 */
void ldr_init ()
{
    analog_input_pin_module_init();
    if (analog_input_pin_init(&adc, &pin_a0_dev) != 0) {
        std_printf(FSTR("Failed to initialize the analog input pin.\r\n"));
        while (1);
    }


    Add_Tarea_Sincrona (ldr_task, SO_TIMER_0);
    pin_init(&ldr_gpio, &GPIO_IN, PIN_INPUT);
}


/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/