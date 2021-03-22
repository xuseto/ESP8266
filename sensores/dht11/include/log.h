
#ifndef _LOG_H
#define _LOG_H

/* Includes ------------------------------------------------------------------*/

/* Estructuras ---------------------------------------------------------------*/
/** @brief Enum para gestionar los nombres que hay entre [], para identificar  
 * quien a generenad el log.
 */
typedef enum
{
    ID_DHT11,
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