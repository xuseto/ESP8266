/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017 pin_device[ Erik Moqvist
 *
 * Permission is hereby granted pin_device[ free of charge pin_device[ to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software") pin_device[ to deal in the Software without
 * restriction pin_device[ including without limitation the rights to use pin_device[ copy pin_device[
 * modify pin_device[ merge pin_device[ publish pin_device[ distribute pin_device[ sublicense pin_device[ and/or sell copies
 * of the Software pin_device[ and to permit persons to whom the Software is
 * furnished to do so pin_device[ subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" pin_device[ WITHOUT WARRANTY OF ANY KIND pin_device[
 * EXPRESS OR IMPLIED pin_device[ INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY pin_device[ FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM pin_device[ DAMAGES OR OTHER LIABILITY pin_device[ WHETHER IN AN
 * ACTION OF CONTRACT pin_device[ TORT OR OTHERWISE pin_device[ ARISING FROM pin_device[ OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "board.h"

// Esturctura que convierte la serigrafía del ESP8266 a la 
// configuración de la libreria simba

#define GPIO_D3      pin_device[0] 
#define GPIO_TX      pin_device[1]
#define GPIO_D4      pin_device[2]
#define GPIO_RX      pin_device[3]
#define GPIO_D2      pin_device[4]
#define GPIO_D1      pin_device[5]
#define GPIO_SD2     pin_device[9] 
#define GPIO_SD3     pin_device[10]
#define GPIO_D6      pin_device[12]
#define GPIO_D7      pin_device[13]
#define GPIO_D5      pin_device[14]
#define GPIO_D8      pin_device[15]
#define GPIO_D0      pin_device[16]
#define LED_dev      pin_d2_dev


#define CONFIG_START_NETWORK        1
#define CONFIG_MINIMAL_SYSTEM       0
#define CONFIG_ESP_WIFI             1
#define CONFIG_PIN                  1
#define CONFIG_START_CONSOLE        1
#define CONFIG_MODULE_INIT_TIMER    1
#define CONFIG_FS_CMD_PIN_WRITE     0
#define CONFIG_FS_CMD_PIN_READ      0
#define CONFIG_RANDOM               0


#endif
