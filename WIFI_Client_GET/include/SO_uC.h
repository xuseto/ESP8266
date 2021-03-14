/*******************************************************************************
 * @file SO_uC.c
 * @author FJGS & David Lundback
 * @version 1.1.1
 * @date Creation: 29/12/2014
 * @date Last modification 01/02/2021 by Jesus Nieto 
 * @brief
 * @par
 *  COPYRIGHT NOTICE: (c) 2021 FJGS (UPV).
 *  All rights reserved
 *******************************************************************************

    @addtogroup SO_uC
    @{
    @defgroup SO_uC 
    @{
    @brief
    @details Sistema central donde se gestiona las treas asincronas, donde queda bloqueado hasta que se genera un evento, y tareas sincronas, donde se ejecutas las aciones, de forma secuencial, cada cierto tiempo, 100 ms, 500 ms o 1 s.
 */

#ifndef __SO_uC_H__
#define __SO_uC_H__

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Defines -------------------------------------------------------------------*/
#define NULO          '\0'
#define SO_TIMER_0    0
#define SO_TIMER_1    1
#define SO_TIMER_2    2
#define MAX_LEN_DATA  200 //50


/* Estrucutas ----------------------------------------------------------------*//** @brief Enum de los eventos disponibles. Modificar al añadir un módulo 
 * nuevo.
 */
typedef enum {
    TASK_LOG,
    TASK_HTTP,
    NUM_TASK_ASINCRONAS,
} ID_tareas_SO_e;

/** @brief Estructura de datos para la funcion de destino, seleccionado del
 *  /ref enum ID_tareas_SO_e.
 */
typedef struct 
{
   /* !< int que represena el ID del dato para gestionarlo en la función de destino */
   uint16_t  ID;
   /* !< array de int donde se almacena los datos, se gestiona como arraya para pasar una cadena de caracteres */
   uint16_t  ptr_data[MAX_LEN_DATA];
}Datos_SO_t;


/** @brief Estructura que encapsula las estructuras anteriores 
 * /ref enum ID_tareas_SO_e.
 * /ref struct Datos_SO_t.
 */
typedef struct 
{
   /* !< Indicamos el evento del destion */
   ID_tareas_SO_e tipo_evento;
   /* !< Indicamos los datos que se quiere informar al destino */
   Datos_SO_t     Datos_SO;
}Datos_eventos_SO_t;

/* SFM functions -------------------------------------------------------------*/
// Definir el tipo puntero a Funciones Sincronas (TAREAS_SINCRONAS).
typedef void (*tarea_t_SINC)(void);

// Definir el tipo puntero a Funciones Asincronas (TAREAS_ASINCRONAS).
typedef void (*tarea_t_Asin)(Datos_SO_t *arg);

// A�adir Tareas nuevas a la Cola de tareas ASINCRONAS.
int Add_Tarea_Asincrona (tarea_t_Asin  task, ID_tareas_SO_e tarea);
// A�adir Tareas nuevas a la Cola de tareas SINCRONAS.
int Add_Tarea_Sincrona (tarea_t_SINC  task, int);


// Iniciar la cola de tareas ASINCRONAS.
void Ini_Tareas(void );

// Ejecutamos el roker de tareas.
void Run_Tareas(void );

void Add_Evento (Datos_eventos_SO_t *Datos_eventos);

#endif

/**
 *  @}
 */

/*****************(C) COPYRIGHT FJGS (UPV)*****END OF FILE ********************/

