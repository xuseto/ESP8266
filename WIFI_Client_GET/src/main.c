/*******************************************************************************
 * @file main.c
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
 *  @addtogroup MAIN
 *  @{
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "simba.h"
#include "config.h"
#include "SO_uC.h"

#include "heartbeat.h"

/* Estructuras ---------------------------------------------------------------*/
static struct timer_t timer;

/* Variables -----------------------------------------------------------------*/

/* Funciones Privadas --------------------------------------------------------*/

/* Funciones Públicas --------------------------------------------------------*/
/**
 * @brief Main. inicio del código
 * @return int
 */
int main()
{
 
    struct time_t timeout;

    sys_start();

    heartbeat_init ();

    /* Initialize and start a periodic timer. */
    timeout.seconds = 1;
    timeout.nanoseconds = 0;
    timer_init(&timer, &timeout, heartbeat_loop, NULL, TIMER_PERIODIC);
    timer_start(&timer);
    
    while (1) {

//add desde branch test_SO
    }
    
    return (0);
}



/**
 *  @}
 */

/******************** (C) COPYRIGHT xuseto*****END OF FILE ********************/