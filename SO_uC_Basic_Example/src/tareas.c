/*******************************************************************************
 * @file tareas.c
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
 *  @addtogroup TASK
 *  @{
 *
 */


/* Includes ------------------------------------------------------------------*/
#include "simba.h"
#include "config.h"
#include "stdint.h"
#include "SO_uC.h"
#include <stdio.h>

#include "tareas.h"
#include "log.h"

/* Estructuras ---------------------------------------------------------------*/
static Datos_eventos_SO_t datos_evento;

/* Variables -----------------------------------------------------------------*/
uint16_t variables ;

/* Decalraciones funciones privadas ------------------------------------------*/
static void add_new_event (void);

/* Funciones privadas --------------------------------------------------------*/
/**
 * @brief Crear nuevo evento al SO del uC.
 */
static void add_new_event ()
{  
    datos_evento.tipo_evento   = Tarea_Log;
    datos_evento.Datos_SO.ID   = (ID_log_e)ID_log_Tarea1;
    itoa (variables, (char *)datos_evento.Datos_SO.ptr_data);
    
    Add_Evento (&datos_evento);

    variables++;
}


/**
 * @brief  Tarea de test sincrona llamada por el SO cada 100 ms
 */
static void task1 (void)
{
    add_new_event ();
}


/**
 * @brief  Tarea de test sincrona llamada por el SO cada 500 ms
 */
static void task2 (void)
{
    char temp[] = "TAREA 2";
    
    datos_evento.tipo_evento   = Tarea_Log;
    datos_evento.Datos_SO.ID   = (ID_log_e)ID_log_Tarea2;
    memcpy (&datos_evento.Datos_SO.ptr_data, &temp, sizeof(temp));
    
    Add_Evento (&datos_evento);
}

/* Funciones públicas --------------------------------------------------------*/
/**
 * @brief  Inicialización de las taeras de test.
 */
void task_init ()
{
     Add_Tarea_Sincrona (task2, SO_TIMER_1);
     Add_Tarea_Sincrona (task1, SO_TIMER_2);
}

/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/