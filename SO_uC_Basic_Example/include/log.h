
#ifndef LOG_H
#define LOG_H

/* Includes ------------------------------------------------------------------*/

/* Estructuras ---------------------------------------------------------------*/
/** @brief Enum para gestionar los nombres que hay entre [], para identificar  
 * quien a generenad el log.
 */
typedef enum
{
    ID_log_heartbeat,
    ID_log_Tarea1,
    ID_log_Tarea2,

    ID_log_Max
} ID_log_e;

/* SFM functions -------------------------------------------------------------*/
void log_init (void);
char* itoa(int i, char b[]);

#endif

/**
 *  @}
 */

/****************(C) COPYRIGHT Jesus Nieto*****END OF FILE ********************/