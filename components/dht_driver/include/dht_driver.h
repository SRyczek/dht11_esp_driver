
/*
 * MIT License
 * 
 * Copyright (c) 2023 Sebastian Ry
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#ifndef DHT11_DRIVER_H
#define DHT11_DRIVER_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h> 

/**
 * @brief Enumeration for DHT sensor status codes.
 *
 * This enumeration defines various status codes that might result from
 * interacting with the DHT sensor. It is used to indicate the success or
 * type of failure of the operations performed with the DHT sensor such as
 * initialization, reading data, and validating checksum.
 *
 * @enum dht_status
 *
 * Values:
 * INIT_ERROR - Indicates an error occurred during the initialization of the DHT sensor.
 * TIMEOUT_ERROR - Indicates a timeout error occurred while waiting for a response
 *                 from the DHT sensor, usually during data read operations.
 * OK - Indicates that the operation, whether it be initialization, reading or
 *      checksum verification, completed successfully.
 * CHECKSUM_ERROR - Indicates that the data read from the sensor does not match the
 *                  calculated checksum, implying data corruption or communication error.
 */

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

/**
 * @brief Reads data from a DHT sensor.
 * 
 * This function initiates the reading process by sending a start signal to the DHT sensor
 * using the send_start_signal function. It then goes through the process of verifying
 * the sensor's response and reading 40 bits of data, which represent humidity and temperature
 * values along with a checksum for data integrity verification.
 * 
 * The function is divided into several steps:
 * 1. Sending the start signal.
 * 2. Verifying the sensor's response.
 * 3. Reading each of the 40 bits of data.
 * 4. Converting the received bits into meaningful humidity, temperature, and checksum values.
 * 
 * If at any point the function encounters a timeout error during verification or data reading,
 * it returns a TIMEOUT_ERROR. After successfully reading and converting data, it checks
 * the checksum. If the checksum does not match the sum of the data, it returns a CHECKSUM_ERROR.
 * If all steps are successful, it returns OK indicating successful data reading.
 * 
 * @return OK on successful data read and integrity check, TIMEOUT_ERROR if there was an issue
 *         in reading the data, or CHECKSUM_ERROR if the data integrity check fails.
 */
uint8_t read_dht(void);

/**
 * @brief Returns the temperature value read from the DHT sensor.
 * 
 * This function returns the temperature value read from the DHT sensor.
 * 
 * @return The temperature value read from the DHT sensor.
 */
float get_temperature(void);


/**
 * @brief Returns the humidity value read from the DHT sensor.
 * 
 * This function returns the humidity value read from the DHT sensor.
 * 
 * @return The humidity value read from the DHT sensor.
 */
float get_humidity(void);

#endif