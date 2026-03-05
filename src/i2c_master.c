#include "i2c_master.h"
#include "driver/i2c.h"
#include "esp_log.h"

#define TAG "I2C_MASTER"
#define I2C_PORT I2C_NUM_0
#define I2C_FREQ 100000

static bool i2c_initialized = false;

esp_err_t i2c_master_init(void)
{
    if (i2c_initialized) {
        ESP_LOGW(TAG, "I2C already initialized");
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Inicializando I2C Master no porto 0...");
    
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ
    };

    esp_err_t ret = i2c_param_config(I2C_PORT, &conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "i2c_param_config failed: %d", ret);
        return ret;
    }

    ret = i2c_driver_install(I2C_PORT, conf.mode, 0, 0, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "i2c_driver_install failed: %d", ret);
        return ret;
    }

    i2c_initialized = true;
    ESP_LOGI(TAG, "I2C Master inicializado com sucesso!");
    
    return ESP_OK;
}

i2c_port_t i2c_master_get_port(void)
{
    return I2C_PORT;
}
