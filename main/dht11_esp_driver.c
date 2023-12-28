#include <stdio.h>
#include <string.h>
#include "../components/dht_driver/include/dht_driver.h"

#define DHT_PIN 4

int app_main(void)
{

    while (1) {
        if (dht_init(DHT_PIN) == INIT_ERROR) {
            printf("Init ERROR\n");
            return 1;
        };
        uint8_t status = read_dht();
        
        if(status == TIMEOUT_ERROR) {
            printf("Timeout ERROR\n");
            return 1;
        } else if (status == CHECKSUM_ERROR) {
            printf("Checksum ERROR\n");
            return 1;
        }
        
        printf("Temp: %0.1f\n", get_temperature());
        printf("Hum: %0.2f\n", get_humidity());

        vTaskDelay(pdMS_TO_TICKS(1000));
    } 

}