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
 *
 *  @addtogroup SO_uCS
 *  @{
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "SO_uC.h"
#include "simba.h"
#include "string.h"

// Definir el Numero MAXIMO de TAREAS en cada COLA estatica...
#define NUM_TAREAS_SINCRONAS 7
#define NUM_EVENTOS			 4

/* Estructuras ---------------------------------------------------------------*/
static struct event_t event_SO;
Datos_eventos_SO_t Datos_eventos_SO[4];

struct        time_t  timeout_1s;
struct        time_t  timeout_100ms;
struct        time_t  timeout_500ms;
static struct timer_t timer;
static struct timer_t timer1;
static struct timer_t timer2;

/* Definir la estructura de las colas de tareas SINCRONAS. */
struct {
	int numero; // Numero de TAREAS en TOTAL
	int num_T0; // Numero de TAREAS en mediante el Timer-0
	int num_T1; // Numero de TAREAS en mediante el Timer-1
	int num_T2; // Numero de TAREAS en mediante el Timer-2

	int Timer;	// Elegir Timer 0,1 y 2 segun sus tiempos SINCRONOS...
	tarea_t_SINC Cola_T_T0 [NUM_TAREAS_SINCRONAS];
	tarea_t_SINC Cola_T_T1 [NUM_TAREAS_SINCRONAS];
	tarea_t_SINC Cola_T_T2 [NUM_TAREAS_SINCRONAS];
} tareas_Sincronas;

/* Definir la estructura de las colas de tareas ASINCRONAS. */
struct {
	int numero;

	tarea_t_Asin Cola_T [NUM_TAREAS_ASINCRONAS];
} tareas_Asincronas;


/* Variables -----------------------------------------------------------------*/
uint16_t mask_event = 0xAABB;

// Variables que indican que Numero de Tarea se va ejecutar...
unsigned int N_Tarea_ASIN_exe=0;

unsigned int N_Tarea_SIN_T0_exe=0;
unsigned int N_Tarea_SIN_T1_exe=0;
unsigned int N_Tarea_SIN_T2_exe=0;

uint16_t ptr_eventos_recibidos;
uint16_t ptr_eventos_leidos;


/* Decalraciones funciones privadas ------------------------------------------*/
void Ini_Tareas_Asincronas (void );
void Ini_Tareas_Sincronas  (int);


/* Funciones privadas --------------------------------------------------------*/
/**
 * @brief  Iniciar la cola de tareas Asincronas.
*/
void Ini_Tareas_Asincronas(void) 
{
	int i;
	tareas_Asincronas.numero = 0;

	for (i = 0; i < NUM_TAREAS_ASINCRONAS; i++) 
	    tareas_Asincronas.Cola_T[i] = NULO;
}

/**
 * @brief  Iniciar la cola de tareas Sincronas.
 * @param  N_TIMER quiere ser llamada la tarea:
 *             SO_TIMER_0 : evento generado cada sg.
 *             SO_TIMER_1 : evento generado cada 500ms 
 *             SO_TIMER_2 : evento generado cada 100ms
*/
void Ini_Tareas_Sincronas(int N_TIMER) 
{
	int i;
	tareas_Sincronas.numero = 0;
    // Elegir Timer para seleccioinar el tiempo
	tareas_Sincronas.Timer = N_TIMER;  

	switch (N_TIMER)
	{
		case 0:	// INICIALIZAR  TAREAS SINCRONAS con el TIMER0....
			for (i = 0; i < NUM_TAREAS_SINCRONAS; i++) 
			    tareas_Sincronas.Cola_T_T0[i] = NULO;
			tareas_Sincronas.num_T0=0;
			break;

		case 1:	// INICIALIZAR  TAREAS SINCRONAS con el TIMER1....
			for (i = 0; i < NUM_TAREAS_SINCRONAS; i++) 
			    tareas_Sincronas.Cola_T_T1[i] = NULO;
			tareas_Sincronas.num_T1=0;
			break;

		case 2:	// INICIALIZAR  TAREAS SINCRONAS con el TIMER2....
			for (i = 0; i < NUM_TAREAS_SINCRONAS; i++) 
			    tareas_Sincronas.Cola_T_T2[i] = NULO;
			tareas_Sincronas.num_T2=0;
			break;
	} // Fin del SWITCH....


}


/**
 * @brief Gestor de Tareas SINCRONAS cuyo proceso es ir ejecutando las *diferentes tareas de manera secuencial.
 * @param argument
 */
static void Run_tareas_SINCRONAS_T0 (void *arg_p) 
{
	tarea_t_SINC  tpS;

	// Ejecutar las Tareas del Timer-0....
	if ((tareas_Sincronas.num_T0 > 0)
	    &&
		(tareas_Sincronas.Cola_T_T0 [N_Tarea_SIN_T0_exe]!= NULO))
    {

		// SELECCIONAR la tarea_SINCRONA de la COLA
		tpS = tareas_Sincronas.Cola_T_T0 [N_Tarea_SIN_T0_exe++];
		// Ejecuta la tarea_SINCRONA seleccionada de la COLA
		tpS(); 
		// Hemos llegado al Final de la COLA ==> Empezar de nuevo...
		if (N_Tarea_SIN_T0_exe > tareas_Sincronas.num_T0) 
		    N_Tarea_SIN_T0_exe = 0;
	} 
	else 
	    N_Tarea_SIN_T0_exe=0;

} // FIN de Run_tareas_SINCRONAS-T0...

/**
 * @brief Gestor de Tareas SINCRONAS cuyo proceso es ir ejecutando las *diferentes tareas de manera secuencial.
 * @param argument
 */
static void Run_tareas_SINCRONAS_T1 (void *arg_p) 
{
	tarea_t_SINC  tpS;
	
	// Ejecutar las Tareas del Timer-1....
	if ((tareas_Sincronas.num_T1 > 0)
	    &&(tareas_Sincronas.Cola_T_T1 [N_Tarea_SIN_T1_exe]!= NULO))
	{	
		// SELECCIONAR la tarea_SINCRONA de la COLA
		tpS = tareas_Sincronas.Cola_T_T1 [N_Tarea_SIN_T1_exe++];
	    // Ejecuta la tarea_SINCRONA seleccionada de la COLA
		tpS();  
		// Hemos llegado al Final de la COLA ==> Empezar de nuevo...
		if (N_Tarea_SIN_T1_exe > tareas_Sincronas.num_T1)
		    N_Tarea_SIN_T1_exe=0;
	} 
	else 
	    N_Tarea_SIN_T1_exe=0;
} // FIN de Run_tareas_SINCRONAS-T1...

/**
 * @brief Gestor de Tareas SINCRONAS cuyo proceso es ir ejecutando las *diferentes tareas de manera secuencial.
 * @param argument
 */
static void Run_tareas_SINCRONAS_T2 (void *arg_p) 
{
	tarea_t_SINC  tpS;

	// Ejecutar las Tareas del Timer-2....
	if ((tareas_Sincronas.num_T2 > 0)
	    &&
		(tareas_Sincronas.Cola_T_T2 [N_Tarea_SIN_T2_exe]!= NULO))
	{
					
		// SELECCIONAR la tarea_SINCRONA de la COLA
		tpS = tareas_Sincronas.Cola_T_T2 [N_Tarea_SIN_T2_exe++];
		// Ejecuta la tarea_SINCRONA seleccionada de la COLA
		tpS();  
		// Hemos llegado al Final de la COLA ==> Empezar de nuevo...
		if (N_Tarea_SIN_T2_exe > tareas_Sincronas.num_T2) 
		    N_Tarea_SIN_T2_exe=0;
	} 
	else 
	    N_Tarea_SIN_T2_exe=0;

} // FIN de Run_tareas_SINCRONAS-T1...


/* Funciones públicas --------------------------------------------------------*/
/**
 * @brief  Añadir Tareas nuevas a la Cola de tareas ASINCRONAS.
 * @param  \ref tarea_t_Asin nombre la funcion Callback.
 * @param  \ref ID_tareas_SO_e Id de la función donde llamar desde el evento.
 * @return 1 : OK    ;    -1 : ERROR
*/
int Add_Tarea_Asincrona (tarea_t_Asin  task, ID_tareas_SO_e tarea) 
{
	if (NUM_TAREAS_ASINCRONAS <= tarea)
	    return -1;

    tareas_Asincronas.Cola_T[tarea] = task;
	return 1;
	
}

/**
 * @brief  Añadir Tareas nuevas a la Cola de tareas SINCRONAS.
 * @param  \ref tarea_t_SINC nombre la funcion Callback.
 * @param  N_TIMER quiere ser llamada la tarea:
 *             SO_TIMER_0 : evento generado cada sg.
 *             SO_TIMER_1 : evento generado cada 500ms 
 *             SO_TIMER_2 : evento generado cada 100ms
  * @return ptero devuelve la poscion del array guardado, -1 ERROR.
*/
int Add_Tarea_Sincrona (tarea_t_SINC  task, int N_TIMER) 
{
	int ptero0 = tareas_Sincronas.num_T0 ;
	int ptero1 = tareas_Sincronas.num_T1 ;
	int ptero2 = tareas_Sincronas.num_T2 ;
	int ptero = tareas_Sincronas.num_T0 + tareas_Sincronas.num_T1 + tareas_Sincronas.num_T2 ;

	if ((ptero + 1) >= NUM_TAREAS_SINCRONAS) 
	{
		return -1;
	} 
	else
	{
		switch (N_TIMER)
		{
			case 0:	// COLA de TAREAS SINCRONAS con el TIMER0....
				tareas_Sincronas.Cola_T_T0[ptero0] = task;
				tareas_Sincronas.num_T0++;
				break;
		
			case 1:	// COLA de TAREAS SINCRONAS con el TIMER1....
				tareas_Sincronas.Cola_T_T1[ptero1] = task;
				tareas_Sincronas.num_T1++;
				break;
		
			case 2:	// COLA de TAREAS SINCRONAS con el TIMER2....
				tareas_Sincronas.Cola_T_T2[ptero2] = task;
				tareas_Sincronas.num_T2++;
				break;
		} // Fin del SWITCH....

		return ptero;
	}
}

/**
 * @brief  Iniciar todas las estructuras y variables a 0.
*/
void Ini_Tareas()
{
	Ini_Tareas_Sincronas(SO_TIMER_0);
	Ini_Tareas_Sincronas(SO_TIMER_1);
	Ini_Tareas_Sincronas(SO_TIMER_2);

	Ini_Tareas_Asincronas ();

	event_init(&event_SO);

	ptr_eventos_recibidos = 0x00;
	ptr_eventos_leidos    = 0x00;

	memset (&Datos_eventos_SO, 0x00, sizeof (Datos_eventos_SO));

}// FIN de Ini_Tareas

/**
 * @brief  Inicializar los timers, paras las tareas sincrronas y se queda 
 * esperando a un evento para ejecutar la tarea asincrona correpondiente.
*/
void Run_Tareas()
{
	tarea_t_Asin  tp;

    // Iniciar Timer para las tareas sincronas de 1 sg
    timeout_1s.seconds = 1;
    timeout_1s.nanoseconds = 0;
    timer_init(&timer, &timeout_1s, Run_tareas_SINCRONAS_T0, NULL, TIMER_PERIODIC);
	
	// Iniciar Timer para las tareas sincronas de 500 msg
	timeout_500ms.seconds = 0;
    timeout_500ms.nanoseconds = 500000000;
    timer_init(&timer2, &timeout_500ms, Run_tareas_SINCRONAS_T1, NULL, TIMER_PERIODIC);

	// Iniciar Timer para las tareas sincronas de 100 msg
    timeout_100ms.seconds = 0;
    timeout_100ms.nanoseconds = 100000000;
    timer_init(&timer1, &timeout_100ms, Run_tareas_SINCRONAS_T2, NULL, TIMER_PERIODIC);
    
	// Iniciar Timers
    timer_start(&timer);
	timer_start(&timer1);
	timer_start(&timer2);

    for (;;)
	{
       event_read (&event_SO, &mask_event, sizeof(mask_event));
  
       do {
	       tp = tareas_Asincronas.Cola_T [(Datos_eventos_SO[ptr_eventos_leidos].tipo_evento)];
	       // Llamar a la COLA de TAREAS a Ejecutar...
	       tp(&Datos_eventos_SO[ptr_eventos_leidos].Datos_SO); 

	       ptr_eventos_leidos = ((NUM_EVENTOS-1) <= ptr_eventos_leidos) ? 0x00 : ptr_eventos_leidos + 1;
		} while (ptr_eventos_leidos != ptr_eventos_recibidos);
	}
}// FIN de Run_Tareas

/**
 * @brief  Se crea un evento y se añaden los datos para la función asincrona
 * correspondiente.
*/
void Add_Evento (Datos_eventos_SO_t *Datos_eventos)
{
	//Reset datos de las estructura
	memset (&Datos_eventos_SO[ptr_eventos_recibidos], 0x00, sizeof (Datos_eventos_SO[ptr_eventos_recibidos]));

	//Añadimos los nuevos datos
	memcpy (&Datos_eventos_SO[ptr_eventos_recibidos], Datos_eventos, sizeof (Datos_eventos_SO[ptr_eventos_recibidos]));

	ptr_eventos_recibidos = ((NUM_EVENTOS-1) <= ptr_eventos_recibidos) ? 
	0x00 : ptr_eventos_recibidos + 1;

    event_write_isr (&event_SO, &mask_event, sizeof(mask_event));
}

/**
 *  @}
 */

/*****************(C) COPYRIGHT FJGS (UPV)*****END OF FILE ********************/
