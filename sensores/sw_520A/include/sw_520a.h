/*******************************************************************************
 * @file sw_520a.h
 * @author Jesus Nieto 
 * @version 0.1.0
 * @date Creation: 14/02/2021
 * @date Last modification 14/02/2021 by Jesus Nieto 
 * @brief
 * @par
 *  COPYRIGHT NOTICE: (c) 2021 Jesus Nieto.
 *  All rights reserved
 *******************************************************************************

    @addtogroup SW_520A
    @{
    @defgroup SW_520A 
    @{
    @brief
    @details Sensor inclinometro, modelo SW 520A

*/

#ifndef SW_520A_H
#define SW_520A_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* SFM functions -------------------------------------------------------------*/
void     sw_520a_init       (void);
void     sw_520a_config     (struct pin_device_t *gpio);
uint16_t sw_520a_get_status (void);

#endif

/**
 *  @}
 */

/****************** (C) COPYRIGHT Jesus Nieto****END OF FILE ******************/