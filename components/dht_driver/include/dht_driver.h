
#ifndef DHT11_DRIVER_H
#define DHT11_DRIVER_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h> 

enum dht_status {
    INIT_ERROR,
    TIMEOUT_ERROR,
    OK,
    CHECKSUM_ERROR
};

/**
 * @brief Initializes the DHT sensor on a specified GPIO.
 * 
 * This function initializes the DHT sensor by assigning a GPIO pin to it. 
 * It checks if the provided GPIO number is valid for the DHT sensor. If the GPIO is valid, 
 * it assigns the GPIO to the DHT sensor and returns OK, indicating successful initialization.
 * If the GPIO is not valid, it prints an error message and returns INIT_ERROR.
 * 
 * @param gpio_num The GPIO number to which the DHT sensor is to be assigned.
 * 
 * @return OK if initialization is successful, otherwise INIT_ERROR.
 */
uint8_t dht_init(gpio_num_t gpio_num);

uint8_t read_dht(void);
float get_temperature(void);
float get_humidity(void);
uint8_t get_checksum(void);
#endif