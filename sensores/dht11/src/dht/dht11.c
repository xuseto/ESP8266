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

/* Funciones privadas --------------------------------------------------------*/


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
}

/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/

