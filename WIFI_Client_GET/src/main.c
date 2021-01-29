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

/* Variables -----------------------------------------------------------------*/

/* Funciones Privadas --------------------------------------------------------*/

/* Funciones Públicas --------------------------------------------------------*/
/**
 * @brief Main. inicio del código
 * @return int
 */
int main()
{
    sys_start();

    Ini_Tareas_Asincronas();
    // Configurar-Inicializar las TAREAS SINCRONAS
	Ini_Tareas_Sincronas(TIMER_0);
	Ini_Tareas_Sincronas(TIMER_1);
	Ini_Tareas_Sincronas(TIMER_2);

    heartbeat_init();

    while (1) {
        //heartbeat_loop ();

        Run_tareas_ASINCRONAS();  // Gestionar las TAREAS ASINCRONAS

    }

    return (0);
}



/**
 *  @}
 */

/******************** (C) COPYRIGHT xuseto*****END OF FILE ********************/