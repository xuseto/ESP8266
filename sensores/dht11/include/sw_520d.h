/*******************************************************************************
 * @file sw_520d.h
 * @author Jesus Nieto 
 * @version 0.1.0
 * @date Creation: 14/02/2021
 * @date Last modification 14/02/2021 by Jesus Nieto 
 * @brief
 * @par
 *  COPYRIGHT NOTICE: (c) 2021 Jesus Nieto.
 *  All rights reserved
 *******************************************************************************

    @addtogroup SW_520D
    @{
    @defgroup SW_520D 
    @{
    @brief
    @details Sensor inclinometro, modelo SW 520A

*/

#ifndef _SW_520D_H
#define _SW_520D_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* SFM functions -------------------------------------------------------------*/
void     sw_520d_init       (void);
void     sw_520d_config     (struct pin_device_t *gpio);
uint16_t sw_520d_get_status (void);

#endif

/**
 *  @}
 */

/****************** (C) COPYRIGHT Jesus Nieto****END OF FILE ******************/