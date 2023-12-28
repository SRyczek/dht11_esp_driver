#include <stdio.h>
#include <string.h>
#include "../components/dht_driver/include/dht_driver.h"

#define DHT_PIN 4

void app_main(void)
{

    while (1) {
        dht_init(DHT_PIN);
        read_dht();
        printf("Temp: %0.1f\n", get_temperature());
        printf("Hum: %0.2f\n", get_humidity());

        vTaskDelay(pdMS_TO_TICKS(1000));
    } 

}