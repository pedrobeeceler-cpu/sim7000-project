#include "lock.h"
#include "config.h"
#include "driver/gpio.h"

void lock_init(void)
{
    gpio_config_t io = {
        .pin_bit_mask = 1ULL << LOCK_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io);
    lock_close();
}

void lock_open(void)
{
    gpio_set_level(LOCK_GPIO, 1);
}

void lock_close(void)
{
    gpio_set_level(LOCK_GPIO, 0);
}