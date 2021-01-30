///////////////////////////////////////////////////////////////////////////////////////////////
// Fichero: SO_uC.c
//
// Descripcion: Fichero de codigo de implementacion de un sistema Operativo en la Familia C2000
//
// Fecha: 29-12-2014
////////////////////////////////////////////////////////////////////////////////////////////////
// ***********************************
#include "SO_uC.h"
#include "simba.h"

///////////////////////////////////////////////////////////////
// Declaracion de Funciones Prototitpo
// Iniciar la cola de tareas SINCRONAS.
void Ini_Tareas_Sincronas(int);

// Gestor de Tareas SINCRONAS cuyo proceso es ir ejecutando las diferentes tareas de manera secuencial.
//void Run_tareas_SINCRONAS_T0 (void *arg_p);
void Run_tareas_SINCRONAS_T1 (void);
void Run_tareas_SINCRONAS_T2 (void);

////////////////////////////////////////////////
// Iniciar la cola de tareas ASINCRONAS.
void Ini_Tareas_Asincronas(void );
// Gestor de Tareas ASINCRONAS cuyo proceso es ir ejecutando las diferentes tareas de manera secuencial.
void Run_tareas_ASINCRONAS (void);



struct        time_t timeout;
static struct timer_t timer;

/////////////////////////////////////////////////////////////////////////////
/* Definir la estructura de las colas de tareas SINCRONAS. */
struct {
	int numero;  // Numero de TAREAS en TOTAL
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

///////////////////////////////////////////////////////////////////////
// Variables que indican que Numero de Tarea se va ejecutar...
unsigned int N_Tarea_ASIN_exe=0;

unsigned int N_Tarea_SIN_T0_exe=0;
unsigned int N_Tarea_SIN_T1_exe=0;
unsigned int N_Tarea_SIN_T2_exe=0;
//////////////////////////////////////
unsigned int Cnt_TSK_ASIN=0;
unsigned int Cnt_TSK_T0_SIN=0, Cnt_TSK_T1_SIN=0, Cnt_TSK_T2_SIN=0;

///////////////////////////////////////////////////////////////////////////////////
// Iniciar la cola de tareas Asincronas.
void Ini_Tareas_Asincronas(void) {
	int i;
	tareas_Asincronas.numero = 0;

	for (i = 0; i < NUM_TAREAS_ASINCRONAS; i++) tareas_Asincronas.Cola_T[i] = NULO;
}
///////////////////////////////////////////////////////////////////////////////////
// A�adir Tareas nuevas a la Cola de tareas ASINCRONAS.
int Add_Tarea_Asincrona (tarea_t_Asin  task) {
	int ptero = tareas_Asincronas.numero ;

	if ((ptero + 1) >= NUM_TAREAS_ASINCRONAS) {
			return -1;
	} else {
		tareas_Asincronas.Cola_T[ptero] = task;
		tareas_Asincronas.numero++;
		return ptero;
		}
}
///////////////////////////////////////////////////////////////////////////////////
// Gestor de Tareas ASINCRONAS cuyo proceso es ir ejecutando
// las diferentes tareas de manera secuencial.
void Run_tareas_ASINCRONAS (void) {
	//int i;
	tarea_t_Asin  tp;

	/* for (;;){
		for (i = 0; i < tareas_Asincronas.numero; i++){
			tp = tareas_Asincronas.Cola_T[i];
			tp();
		}
	}*/
	///////////////////////////////////////////
	Cnt_TSK_ASIN++;
	if ((tareas_Asincronas.numero > 0)&& (tareas_Asincronas.Cola_T [N_Tarea_ASIN_exe]!=NULO)){
		tp = tareas_Asincronas.Cola_T [N_Tarea_ASIN_exe++];
		tp(); // Llamar a la COLA de TAREAS a Ejecutar...
		// Hemos llegado al Final de la COLA ==> Empezar de nuevo...
		if (N_Tarea_ASIN_exe > NUM_TAREAS_ASINCRONAS) N_Tarea_ASIN_exe=0;
	} else N_Tarea_ASIN_exe=0;
	/////////////////////////////////////////////////////////
}
//####################################################################################
///////////////////////////////////////////////////////////////////////////////////
// Iniciar la cola de tareas Sincronas.
void Ini_Tareas_Sincronas(int N_TIMER) {
	int i;
	tareas_Sincronas.numero = 0;

	tareas_Sincronas.Timer = N_TIMER;  // Elegir Timer para seleccioinar el tiempo

	switch (N_TIMER){
					case 0:	// INICIALIZAR  TAREAS SINCRONAS con el TIMER0....
						for (i = 0; i < NUM_TAREAS_SINCRONAS; i++) tareas_Sincronas.Cola_T_T0[i] = NULO;
						tareas_Sincronas.num_T0=0;
						break;
					case 1:	// INICIALIZAR  TAREAS SINCRONAS con el TIMER1....
						for (i = 0; i < NUM_TAREAS_SINCRONAS; i++) tareas_Sincronas.Cola_T_T1[i] = NULO;
						tareas_Sincronas.num_T1=0;
						break;
					case 2:	// INICIALIZAR  TAREAS SINCRONAS con el TIMER2....
						for (i = 0; i < NUM_TAREAS_SINCRONAS; i++) tareas_Sincronas.Cola_T_T2[i] = NULO;
						tareas_Sincronas.num_T2=0;
						break;
			} // Fin del SWITCH....


}
///////////////////////////////////////////////////////////////////////////////////
// A�adir Tareas nuevas a la Cola de tareas SINCRONAS.
int Add_Tarea_Sincrona (tarea_t_SINC  task, int N_TIMER) {
		int ptero0 = tareas_Sincronas.num_T0 ;
		int ptero1 = tareas_Sincronas.num_T1 ;
		int ptero2 = tareas_Sincronas.num_T2 ;
		int ptero = tareas_Sincronas.num_T0 + tareas_Sincronas.num_T1 + tareas_Sincronas.num_T2 ;

		if ((ptero + 1) >= NUM_TAREAS_SINCRONAS) {
				return -1;
		} else {
			switch (N_TIMER){
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
///////////////////////////////////////////////////////////////////////////////////
// Gestor de Tareas SINCRONAS cuyo proceso es ir ejecutando
// las diferentes tareas de manera secuencial.
static void Run_tareas_SINCRONAS_T0 (void *arg_p) {
	//int i;
	tarea_t_SINC  tpS;

	//////////////////////////////////////////////////
	/*for (;;){
		for (i = 0; i < tareas_Sincronas.number; i++){
			tp = tareas_Sincronas.Cola_T[i];
			tp();
		}
	} // Fin del for..
	*/
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Ejecutar las Tareas del Timer-0....
	if ((tareas_Sincronas.num_T0 > 0)&&(tareas_Sincronas.Cola_T_T0 [N_Tarea_SIN_T0_exe]!= NULO)){
			Cnt_TSK_T0_SIN++;
			 // SELECCIONAR la tarea_SINCRONA de la COLA
			tpS = tareas_Sincronas.Cola_T_T0 [N_Tarea_SIN_T0_exe++];
			tpS();  // Ejecuta la tarea_SINCRONA seleccionada de la COLA
			// Hemos llegado al Final de la COLA ==> Empezar de nuevo...
			if (N_Tarea_SIN_T0_exe > tareas_Sincronas.num_T0) N_Tarea_SIN_T0_exe=0;
	} else N_Tarea_SIN_T0_exe=0;
		/////////////////////////////////////////////////////////////////////////////////////////////

} // FIN de Run_tareas_SINCRONAS-T0...
/////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
// Gestor de Tareas SINCRONAS cuyo proceso es ir ejecutando
// las diferentes tareas de manera secuencial.
void Run_tareas_SINCRONAS_T1 (void) {
	//int i;
	tarea_t_SINC  tpS;
	//////////////////////////////////////////////////
	/*for (;;){
		for (i = 0; i < tareas_Sincronas.number; i++){
			tp = tareas_Sincronas.Cola_T[i];
			tp();
		}
	} // Fin del for..
	*/
	/////////////////////////////////////////////////////////////////////////////////////////////
	// Ejecutar las Tareas del Timer-1....
	if ((tareas_Sincronas.num_T1 > 0)&&(tareas_Sincronas.Cola_T_T1 [N_Tarea_SIN_T1_exe]!= NULO)){
					Cnt_TSK_T1_SIN++;
					// SELECCIONAR la tarea_SINCRONA de la COLA
					tpS = tareas_Sincronas.Cola_T_T1 [N_Tarea_SIN_T1_exe++];
					tpS();  // Ejecuta la tarea_SINCRONA seleccionada de la COLA
					// Hemos llegado al Final de la COLA ==> Empezar de nuevo...
					if (N_Tarea_SIN_T1_exe > tareas_Sincronas.num_T1) N_Tarea_SIN_T1_exe=0;
	} else N_Tarea_SIN_T1_exe=0;


} // FIN de Run_tareas_SINCRONAS-T1...
///////////////////////////////////////////////////////////////////////
// Gestor de Tareas SINCRONAS cuyo proceso es ir ejecutando
// las diferentes tareas de manera secuencial.
void Run_tareas_SINCRONAS_T2 (void) {
	//int i;
	tarea_t_SINC  tpS;
	//////////////////////////////////////////////////
	/*for (;;){
		for (i = 0; i < tareas_Sincronas.number; i++){
			tp = tareas_Sincronas.Cola_T[i];
			tp();
		}
	} // Fin del for..
	*/
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Ejecutar las Tareas del Timer-2....
	if ((tareas_Sincronas.num_T2 > 0)&&(tareas_Sincronas.Cola_T_T2 [N_Tarea_SIN_T2_exe]!= NULO)){
					Cnt_TSK_T2_SIN++;
					// SELECCIONAR la tarea_SINCRONA de la COLA
					tpS = tareas_Sincronas.Cola_T_T2 [N_Tarea_SIN_T2_exe++];
					tpS();  // Ejecuta la tarea_SINCRONA seleccionada de la COLA
					// Hemos llegado al Final de la COLA ==> Empezar de nuevo...
					if (N_Tarea_SIN_T2_exe > tareas_Sincronas.num_T2) N_Tarea_SIN_T2_exe=0;
	} else N_Tarea_SIN_T2_exe=0;
		/////////////////////////////////////////////////////////

} // FIN de Run_tareas_SINCRONAS-T1...

void Ini_Tareas()
{
	Ini_Tareas_Sincronas(TIMER_0);
	Ini_Tareas_Sincronas(TIMER_1);
	Ini_Tareas_Sincronas(TIMER_2);

    /* Initialize and start a periodic timer. */
    timeout.seconds = 1;
    timeout.nanoseconds = 0;
    timer_init(&timer, &timeout, Run_tareas_SINCRONAS_T0, NULL, TIMER_PERIODIC);
    timer_start(&timer);
}

////////////////////////////////////////////
//   Fin CODIGO del FICHERO....
///////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////////////////
// FIN CODIGO DEL FICHERO
//////////////////////////////////////////////////////////////////////////////////
