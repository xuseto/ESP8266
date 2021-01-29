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
// Definir el Numero MAXIMO de TAREAS en cada COLA estatica...
#define NUM_TAREAS_ASINCRONAS 7
#define NUM_TAREAS_SINCRONAS 7

#define NULO  '\0'

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

/////////////////////////////////////////////////////////////////////////////
// Definir el tipo puntero a Funciones Sincronas (TAREAS_SINCRONAS).
typedef void (*tarea_t_SINC)(void);

// Definir el tipo puntero a Funciones Asincronas (TAREAS_ASINCRONAS).
typedef void (*tarea_t_Asin)(void);

///////////////////////////////////////////////////////////////
// Declaracion de Funciones Prototitpo
// Iniciar la cola de tareas SINCRONAS.
void Ini_Tareas_Sincronas(int);
// Añadir Tareas nuevas a la Cola de tareas SINCRONAS.
int Add_Tarea_Sincrona (tarea_t_SINC  task, int);
// Gestor de Tareas SINCRONAS cuyo proceso es ir ejecutando las diferentes tareas de manera secuencial.
void Run_tareas_SINCRONAS_T0 (void);
void Run_tareas_SINCRONAS_T1 (void);
void Run_tareas_SINCRONAS_T2 (void);

////////////////////////////////////////////////
// Iniciar la cola de tareas ASINCRONAS.
void Ini_Tareas_Asincronas(void );
// Añadir Tareas nuevas a la Cola de tareas ASINCRONAS.
int Add_Tarea_Asincrona (tarea_t_Asin  task);
// Gestor de Tareas ASINCRONAS cuyo proceso es ir ejecutando las diferentes tareas de manera secuencial.
void Run_tareas_ASINCRONAS (void);

///////////////////////////////////////////////////////
// Inicializar TAREAS y Añadir Tareas
void Ini_Add_Tareas (void);

//////////////////////////////////////////////////////////////
// Funciones a realizar por el GESTOR de TAREAS SINCRONAS
// Mediante Interupcion del TIMER-2...
// Mediante Polling del Flag _Interrup de TIMER-0 y 1...
void Test_Run_TSK_Timer_0_Sinc (void);
void Test_Run_TSK_Timer_1_Sinc (void);


#endif
////////////////////////////////////////////
//   Fin CODIGO del FICHERO....
///////////////////////////////////////////////

