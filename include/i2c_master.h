#pragma once

#include "driver/i2c.h"
#include "esp_err.h"

#define I2C_SDA_PIN  21
#define I2C_SCL_PIN  22

esp_err_t i2c_master_init(void);
i2c_port_t i2c_master_get_port(void);
