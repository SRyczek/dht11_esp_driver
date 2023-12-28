
#ifndef DHT11_DRIVER_H
#define DHT11_DRIVER_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h> 

enum dht_status {
    INIT_ERROR,
    TIMEOUT_ERROR,
    OK
};

int dht_init(gpio_num_t gpio_num);
int read_dht(void);
float get_temperature(void);
float get_humidity(void);

#endif