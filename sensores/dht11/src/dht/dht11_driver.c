
/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"
#include "time.h"
#include "dht/dht11_driver.h"

#if CONFIG_DHT == 1

#define FALLING                                     0
#define RISING                                      1

/* Time definitions. */
#define TIMEOUT_US                                  100
#define MINIMUM_RESPONSE_LOW_US                     50
#define MINIMUM_RESPONSE_HIGH_US                    60

/* Data definitions. */
#define DATA_SIZE                                   5

#define HUMID_MSB_INDEX                             0
#define HUMID_LSB_INDEX                             1
#define TEMP_MSB_INDEX                              2
#define TEMP_LSB_INDEX                              3
#define PARITY_INDEX                                4

#define UPDATE_TICK()                               time_get(&module.tick)
#define TIME_MICROS()                               ((float)module.tick.nanoseconds * 0.001)
#define TIME_MICROS_ELAPSED(timer1,timer2)          (timer2 - timer1)

struct module_t {
    int8_t initialized;
#if CONFIG_DHT_COMMAND_READ == 1
    struct fs_command_t cmd_read;
    struct fs_command_t cmd_read_11;
#endif
   struct time_t tick; 
};

struct module_t module;

/**
 * Check if given buffer contains valid data.
 */
static int is_valid(uint8_t *buf_p)
{
    uint8_t actual_parity;
    uint8_t expected_parity;

    actual_parity = (buf_p[HUMID_MSB_INDEX]
                     + buf_p[HUMID_LSB_INDEX]
                     + buf_p[TEMP_MSB_INDEX]
                     + buf_p[TEMP_LSB_INDEX]);
    expected_parity = buf_p[PARITY_INDEX];

    return (actual_parity == expected_parity);
}

/**
 * Wait for a rising or falling edge.
 *
 * @return Wait time in microseconds, or negative error code.
 */
static int wait_for_edge(struct dht_driver_t *self_p,
                         int target_level)
{
    uint16_t start;
    uint16_t stop;
    uint16_t elapsed;

    UPDATE_TICK() ;
    start = TIME_MICROS();

    while (1) {
        stop = TIME_MICROS();
        elapsed = TIME_MICROS_ELAPSED(start, stop);

        if (pin_device_read(self_p->pin_p) == target_level) {
            break;
        }

        if (elapsed > TIMEOUT_US) {
            return (-ETIMEDOUT);
        }
    }

    return (elapsed);
}

/**
 * Read a bit from the device.
 *
 * @return zero(0) or negative error code.
 */
static int read_bit(struct dht_driver_t *self_p,
                    int *value_p)
{
    int res;

    res = wait_for_edge(self_p, RISING);

    if (res < 0) {
        return (res);
    }

    res = wait_for_edge(self_p, FALLING);

    if (res < 0) {
        return (res);
    }

    *value_p = (res > 50);

    return (0);
}

/**
 * Read data from the device by setting the start signal high, wait
 * for the response and then read all 40 bits.
 *
 * @return zero(0), positive number on failure, or negative error code.
 */
static int read_isr(struct dht_driver_t *self_p, uint8_t *buf_p)
{
    int i;
    int res;
    int value;

    pin_device_set_mode(self_p->pin_p, PIN_INPUT);
    time_busy_wait_us(20);

    /* Read the response signal. */
    res = wait_for_edge(self_p, RISING);

    if (res < MINIMUM_RESPONSE_LOW_US) {
        return (res >= 0 ? -EPROTO : res);
    }

    res = wait_for_edge(self_p, FALLING);

    if (res < MINIMUM_RESPONSE_HIGH_US) {
        return (res >= 0 ? -EPROTO : res);
    }

    /* Read temperature, humidty and parity. */
    for (i = 0; i < DATA_SIZE * 8; i++) {
        res = read_bit(self_p, &value);

        if (res != 0) {
            break;
        }

        buf_p[i / 8] <<= 1;
        buf_p[i / 8] |= value;
    }

    return (res);
}

static int read_sensor(struct dht_driver_t *self_p, uint8_t *buf_p)
{
    int res;

    memset(&buf_p[0], 0, DATA_SIZE);

    /* Device communication. Start start signal by setting the pin
       low. */
    pin_device_set_mode(self_p->pin_p, PIN_OUTPUT);
    pin_device_write_low(self_p->pin_p);
    thrd_sleep_ms(1);

    sys_lock();
    res = read_isr(self_p, &buf_p[0]);
    sys_unlock();

    if (res != 0) {
        return (res < 0 ? res : -EPROTO);
    }

    /* Check the parity bits. */
    if (!is_valid(&buf_p[0])) {
        return (-EPROTO);
    }

    return (0);
}

#if CONFIG_DHT_COMMAND_READ == 1

static int cmd_read_sensor(int argc,
                           const char *argv[],
                           void *chout_p,
                           int (*read_p)(struct dht_driver_t *, float *, float *),
                           const char *fn_name_p)
{
    struct dht_driver_t dht;
    int pin;
    int res;
    float temperature;
    float humidity;

    if (argc != 2) {
        std_printf(OSTR("Usage: %s <pin name>\r\n"), fn_name_p);

        return (-EINVAL);
    }

    pin = board_pin_string_to_device_index(argv[1]);

    if (pin < 0) {
        std_printf(OSTR("Invalid pin name: %s.\r\n"), argv[1]);

        return (-EINVAL);
    }

    dht_init(&dht, &pin_device[pin]);
    
    res = read_p(&dht, &temperature, &humidity);

    if (res == 0) {
        std_fprintf(chout_p, OSTR("Temperature: %f, Humidity: %f\r\n"), temperature, humidity);
    } else {
        std_fprintf(chout_p, OSTR("Error: %d (%s)\r\n"), res, errno_as_string(res));
    }

    return (0);
}

static int cmd_read_cb(int argc,
                       const char *argv[],
                       void *chout_p,
                       void *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    return (cmd_read_sensor(argc, argv, chout_p, dht_read, "read"));
}

static int cmd_read_11_cb(int argc,
                          const char *argv[],
                          void *chout_p,
                          void *chin_p,
                          void *arg_p,
                          void *call_arg_p)
{
    return (cmd_read_sensor(argc, argv, chout_p, dht_read_11, "read_11"));
}

#endif

int dht_module_init()
{
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

    FATAL_ASSERT(TIME_MICROS_maximum() > TIMEOUT_US);

#if CONFIG_DHT_COMMAND_READ == 1
    fs_command_init(&module.cmd_read,
                    CSTR("/drivers/dht/read"),
                    cmd_read_cb,
                    NULL);
    fs_command_register(&module.cmd_read);

    fs_command_init(&module.cmd_read_11,
                    CSTR("/drivers/dht/read_11"),
                    cmd_read_11_cb,
                    NULL);
    fs_command_register(&module.cmd_read_11);
#endif

    time_set(&module.tick);

    return (pin_module_init());
}

int dht_init(struct dht_driver_t *self_p,
             struct pin_device_t *pin_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(pin_p != NULL, EINVAL);

    self_p->pin_p = pin_p;

    return (0);
}

int dht_read(struct dht_driver_t *self_p,
             float *temperature_p,
             float *humidty_p)
{
    int res;
    int negative;
    uint8_t buf[DATA_SIZE];

    res = read_sensor(self_p, &buf[0]);

    if (res < 0) {
        return (res);
    }

    /* Temperature and humidty unpacking and convertion. */
    negative = (buf[TEMP_MSB_INDEX] >> 7);
    buf[TEMP_MSB_INDEX] &= 0x7f;
    *temperature_p = ((buf[TEMP_MSB_INDEX] << 8) | buf[TEMP_LSB_INDEX]);
    *temperature_p /= 10.0f;

    if (negative == 1) {
        *temperature_p *= -1.0f;
    }

    *humidty_p = ((buf[HUMID_MSB_INDEX] << 8) | buf[HUMID_LSB_INDEX]);
    *humidty_p /= 10.0f;

    return (0);
}

int dht_read_11(struct dht_driver_t *self_p,
               float *temperature_p,
               float *humidty_p)
{
    int res;
    uint8_t buf[DATA_SIZE];

    res = read_sensor(self_p, &buf[0]);

    if (res < 0) {
        return (res);
    }

    /* Temperature and humidty unpacking and convertion. */
    *temperature_p = (buf[TEMP_MSB_INDEX] + (buf[TEMP_LSB_INDEX] / 10.0f));
    *humidty_p = buf[HUMID_MSB_INDEX];

    return (0);
}

#endif