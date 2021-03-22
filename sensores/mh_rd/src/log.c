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

/* Estructuras ---------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Decalraciones funciones privadas ------------------------------------------*/

/* Funciones privadas --------------------------------------------------------*/
/**
 * @brief  Función llamada desde el SO_uC para imprmir en pantalla el log 
 * generado en el código.
 * @param /ref Datos_SO_t
 */
static void log_print (Datos_SO_t *log_data)
{
    std_printf(FSTR("***************************************************\r\n"));
    switch ((ID_log_e)log_data->ID)
    {
    case ID_MH_RD:
        std_printf(FSTR("[MH RD      ]"));
        break;
    
    default:
        std_printf(FSTR("[LOG        ]"));
        break;
    }
    
    std_printf(FSTR(" %s\r\n"),log_data->ptr_data);

    std_printf(FSTR("\r\n"));
}

/* Funciones públicas --------------------------------------------------------*/
/**
 * @brief  Inicialización del módulo LOG.
 */
void log_init ()
{
    Add_Tarea_Asincrona (log_print, (ID_tareas_SO_e)LOG_TASK);
}

/**
 * @brief  Función que transforma un int a una cadena de caracteres.
 * @param  i valor entero que se quiere convertir en ASCII.
 * @param  b[] puntero del array donde guardar el valor int en ASCII.
 */
char* itoa(int i, char b[])
{
    char const digit[] = "0123456789";
    char* p = b;

    if (0 > i)
    {
        *p++ = '-';
        i *= -1;
    }

    int shifter = i;
    
    do { //Move to where representation ends
        ++p;
        shifter = shifter/10;
    } while(shifter);

    *p = '\0';

   //Move back, inserting digits as u go
    do { 
        *--p = digit[i%10];
        i = i/10;
    } while(i);

    return b;
}

/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/

