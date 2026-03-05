#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "config.h"
#include "nfc.h"
#include "lock.h"
#include "rs485.h"
#include "i2c_master.h"
#include "i2c_scanner.h"


void app_init(void)
{
    printf("\n\n=== INICIALIZANDO SISTEMA ===\n");
    printf("Dock firmware. ST=%d DK=%d\n\n", STATION_ID, DOCK_ID);
    
    lock_init();
    rs485_init();
    
    // IMPORTANTE: Inicializar I2C ANTES de tudo
    printf("\n--- INICIALIZANDO BARRAMENTO I2C ---\n");
    esp_err_t ret = i2c_master_init();
    if (ret != ESP_OK) {
        printf("❌ Falha ao inicializar I2C: %d\n", ret);
    } else {
        printf("✅ I2C inicializado com sucesso\n\n");
    }
    
    vTaskDelay(pdMS_TO_TICKS(200));
    
    // Scanner I2C (debug)
    printf("--- PROCURANDO DISPOSITIVOS I2C ---\n");
    i2c_scanner_scan();
    vTaskDelay(pdMS_TO_TICKS(500));
    
    // Inicializar NFC
    printf("--- INICIALIZANDO NFC/PN532 ---\n");
    nfc_init();
    
    printf("=== SISTEMA PRONTO ===\n\n");
}

void app_loop(void)
{
    nfc_poll_uid();
    vTaskDelay(pdMS_TO_TICKS(500));
}