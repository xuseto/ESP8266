/*******************************************************************************
 * @file log.c
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
 *  @addtogroup LOG
 *  @{
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "simba.h"
#include "config.h"
#include "string.h"
#include "SO_uC.h"

#include "log.h"
#include "dht/dht11_driver.h"
#include "dht/dht11.h"

/* Estructuras ---------------------------------------------------------------*/
typedef struct sdht11
{
    struct dht_driver_t driver;
    float               temperature;
    float               humidty;
}dht11_t;

dht11_t dht11;

/* Variables -----------------------------------------------------------------*/

/* Decalraciones funciones privadas ------------------------------------------*/
static void dht11_task (void);

/* Funciones privadas --------------------------------------------------------*/
static void dht11_task (void)
{
    Datos_eventos_SO_t new_event;
    uint16_t value;

    value =  dht_read (&dht11.driver, &dht11.temperature, &dht11.humidty);

    new_event.tipo_evento = TASK_LOG;
    new_event.Datos_SO.ID = (ID_log_e)ID_DHT11;
    itoa (value, (char *)new_event.Datos_SO.ptr_data);

    Add_Evento (&new_event);

    

    //std_printf ("Temperatura : %ld, Humedad: %ld \r\n",dht11.temperature, dht11.humidty);
    //std_printf ("Temperatura, Humedad:  \r\n");
}

/* Funciones públicas --------------------------------------------------------*/
/**
 * @brief Iniciar el sensor DHT11
 */
void dht11_init ()
{
   if (dht_module_init())
   {
       std_printf ("Error DHT11 \r\n");
   }
   else
   {
       Add_Tarea_Sincrona (dht11_task, SO_TIMER_1);
   }

}

void dht11_config (struct pin_device_t gpio)
{
    dht_init(&dht11.driver, &gpio);
}


/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/

