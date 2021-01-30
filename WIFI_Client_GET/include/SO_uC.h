////////////////////////////////////////////////////////////////////////////////////////////////
// Fichero: SO_uC.h
//
// Descripcion: Fichero de cabecera de implementacion de un sistema Operativo en la Familia C2000
//
// Fecha: 29-12-2015
////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef __SO_uC_H__
#define __SO_uC_H__
// Declaracion de MACROS

#define NULO  '\0'
#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

////////////////////////////////////////////////////////////////////////////////// Enum de los eventos disponibles
// Modificar al añadir un módulo nuevo
typedef enum {
    Tarea_heartbeat      = 1,
    NUM_TAREAS_ASINCRONAS,
} ID_tareas_SO_e;

ID_tareas_SO_e tipo_evento;

/////////////////////////////////////////////////////////////////////////////
// Definir el tipo puntero a Funciones Sincronas (TAREAS_SINCRONAS).
typedef void (*tarea_t_SINC)(void);

// Definir el tipo puntero a Funciones Asincronas (TAREAS_ASINCRONAS).
typedef void (*tarea_t_Asin)(void);


// A�adir Tareas nuevas a la Cola de tareas ASINCRONAS.
int Add_Tarea_Asincrona (tarea_t_Asin  task, ID_tareas_SO_e tarea);
// A�adir Tareas nuevas a la Cola de tareas SINCRONAS.
int Add_Tarea_Sincrona (tarea_t_SINC  task, int);

////////////////////////////////////////////////
// Iniciar la cola de tareas ASINCRONAS.
void Ini_Tareas(void );

////////////////////////////////////////////////
// Ejecutamos el roker de tareas.
void Run_Tareas(void );

void Add_Evento (ID_tareas_SO_e ID_tarea);


#endif
////////////////////////////////////////////
//   Fin CODIGO del FICHERO....
///////////////////////////////////////////////

