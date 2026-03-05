#include "drivers/pn532.h"
#include "i2c_master.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "PN532"

#define PN532_ADDR 0x48  // Endereço I2C do PN532

static i2c_port_t i2c_port;

static esp_err_t pn532_write(uint8_t *data, uint8_t len)
{
    ESP_LOGI(TAG, "I2C Write %d bytes", len);
    return i2c_master_write_to_device(i2c_port, PN532_ADDR, data, len, pdMS_TO_TICKS(1000));
}

// Ler do PN532 (via I2C) - versão com debug simples
static esp_err_t pn532_read(uint8_t *data, uint8_t len)
{
    esp_err_t ret = i2c_master_read_from_device(i2c_port, PN532_ADDR, data, len, pdMS_TO_TICKS(1000));
    
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "I2C Read OK - %d bytes", len);
        printf("Dados: ");
        for (int i = 0; i < 10 && i < len; i++) 
            printf("%02X ", data[i]);
        printf("\n");
    } else {
        ESP_LOGE(TAG, "I2C Read FAIL - ret=%d", ret);
    }
    
    return ret;
}

esp_err_t pn532_init(void)
{
    ESP_LOGI(TAG, "Inicializando PN532...");

    i2c_port = i2c_master_get_port();

    vTaskDelay(pdMS_TO_TICKS(500));  // Maior delay para PN532 acordar

    // comando SAMConfig
    uint8_t sam_config[] = {
        0x00,0x00,0xFF,
        0x05,0xFB,
        0xD4,0x14,0x01,0x14,0x01,
        0x00,
        0x00
    };

    esp_err_t ret = pn532_write(sam_config, sizeof(sam_config));
    ESP_LOGI(TAG, "SAMConfig write: %s", ret == ESP_OK ? "OK" : "FAIL");

    vTaskDelay(pdMS_TO_TICKS(200));

    uint8_t resp[32] = {0};
    vTaskDelay(pdMS_TO_TICKS(100));
    
    ret = pn532_read(resp, sizeof(resp));
    ESP_LOGI(TAG, "SAMConfig read: %s", ret == ESP_OK ? "OK" : "FAIL");
    if (ret == ESP_OK) {
        printf("SAMConfig resp: ");
        for(int i = 0; i < 10; i++) printf("%02X ", resp[i]);
        printf("\n");
    }

    ESP_LOGI(TAG, "PN532 inicializado");

    return ESP_OK;
}

esp_err_t pn532_read_uid(uint8_t *uid, uint8_t *uid_len)
{
    uint8_t cmd[] = {
        0x00,0x00,0xFF,
        0x04,0xFC,
        0xD4,0x4A,0x01,0x00,
        0xE1,
        0x00
    };

    pn532_write(cmd, sizeof(cmd));

    vTaskDelay(pdMS_TO_TICKS(100));  // Espera resposta estar pronta

    uint8_t resp[32] = {0};
    esp_err_t read_ret = pn532_read(resp, sizeof(resp));

    printf("ReadUID: ret=%s\n", read_ret == ESP_OK ? "OK" : "FAIL");
    
    if (read_ret == ESP_OK) {
        printf("Resposta (20 bytes): ");
        for (int i = 0; i < 20; i++) 
            printf("%02X ", resp[i]);
        printf("\n");
    } else {
        return ESP_FAIL;
    }

    // Verificar resposta válida
    if (resp[0] != 0x00 || resp[1] != 0x00 || resp[2] != 0xFF) {
        printf("Erro: frame inválido resp[0:3]=%02X %02X %02X\n", resp[0], resp[1], resp[2]);
        return ESP_FAIL;
    }

    if (resp[6] != 0xD5) {
        printf("Erro: resp[6]=%02X (esperava 0xD5)\n", resp[6]);
        return ESP_FAIL;
    }

    *uid_len = resp[12];
    
    if (*uid_len == 0 || *uid_len > 10) {
        printf("UID length inválido: %d\n", *uid_len);
        return ESP_FAIL;
    }

    printf("UID length OK: %d\n", *uid_len);

    for (int i = 0; i < *uid_len; i++)
        uid[i] = resp[13+i];

    return ESP_OK;
}