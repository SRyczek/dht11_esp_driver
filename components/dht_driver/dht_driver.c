
#include "include/dht_driver.h"

static gpio_num_t dht_gpio;
static uint8_t data[5];
static float humidity, temperature;
static uint16_t clock_ticks;
static uint8_t checksum;

uint8_t dht_init(gpio_num_t gpio_num)
{
    if (GPIO_IS_VALID_GPIO(gpio_num)) {
        dht_gpio = gpio_num;
        return OK;
    }
    printf("Init ERROR\n");
    return INIT_ERROR;
}


static void send_start_signal()
{
    memset(data, 0, sizeof(data));
    gpio_set_direction(dht_gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(dht_gpio, 0);
    esp_rom_delay_us(20 * 1000);
    gpio_set_level(dht_gpio, 1);
    esp_rom_delay_us(30);
    gpio_set_direction(dht_gpio, GPIO_MODE_INPUT);
}

static uint8_t verify_incoming_signal(uint16_t microseconds, int level)
{
    uint16_t ticks = 0;
    while (gpio_get_level(dht_gpio) == level) {
        ticks++;
        if (ticks > microseconds)
            return TIMEOUT_ERROR;
        esp_rom_delay_us(1);
    }
    clock_ticks = ticks;
    return OK;

}

static void set_bit_at_position(int position)
{
    if (position >= 0 && position <= 7) {
        data[0] |= (1 << position);
    }
    else if (position >= 8 && position <= 15) {
        position = position % 8;
        data[1] |= (1 << position);
    }
    else if (position >= 16 && position <= 23) {
        position = position % 8;
        data[2] |= (1 << position);
    }
    else if (position >= 24 && position <= 31) {
        position = position % 8;
        data[3] |= (1 << position);
    }
    else if (position >= 32 && position <= 39) {
        position = position % 8;
        data[4] |= (1 << position);
    }
    else {
        printf("ERROR SET POSITION\n");
    }
}

static float parse_data(uint8_t integral_part, uint8_t decimal_part)
{
    return ((float)integral_part + ((float)decimal_part) / 10);
}

static void convert_data(void)
{
    uint8_t reverse_data;
    for (int data_block = 0; data_block < 5; data_block++) {
        reverse_data = 0;
        for (int bit_num = 0; bit_num < 8; bit_num++) {
            if (data[data_block] & (1 << bit_num)) {
                reverse_data |= (1 << (7 - bit_num));
            }
        }
        data[data_block] = reverse_data;
    }
    temperature = parse_data(data[2], data[3]);
    humidity = parse_data(data[0], data[1]);
    checksum = data[4];
}

uint8_t read_dht(void)
{
    send_start_signal();
    /* verify response part 1 */
    if (verify_incoming_signal(83, 0) == TIMEOUT_ERROR) 
        return TIMEOUT_ERROR;
    /* verify response part 2 */
    if (verify_incoming_signal(87, 1) == TIMEOUT_ERROR) 
        return TIMEOUT_ERROR;
    
    for (int bit_num = 0; bit_num < 40; bit_num++) {
        if (verify_incoming_signal(50, 0) == TIMEOUT_ERROR)
            return TIMEOUT_ERROR;

        if (verify_incoming_signal(70, 1) == TIMEOUT_ERROR || clock_ticks == TIMEOUT_ERROR)
            return TIMEOUT_ERROR;

        if (clock_ticks > 40)
            set_bit_at_position(bit_num);

    }
    convert_data();
    if (checksum != (data[0] + data[1] + data[2] + data[3]))
        return CHECKSUM_ERROR;
    return OK;
}

float get_temperature(void)
{
    return temperature;
}

float get_humidity(void)
{
    return humidity;
}

