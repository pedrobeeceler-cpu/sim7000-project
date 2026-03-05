#include "i2c_scanner.h"
#include "i2c_master.h"
#include "driver/i2c.h"
#include "esp_log.h"

#define TAG "I2C_SCANNER"

void i2c_scanner_scan(void)
{
    i2c_port_t port = i2c_master_get_port();
    
    ESP_LOGI(TAG, "Iniciando scan I2C...");
    printf("\n[I2C SCAN] Procurando dispositivos no barramento...\n");
    
    uint8_t devices_found = 0;
    
    for (uint8_t addr = 0x01; addr < 0x7F; addr++) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_READ, true);
        i2c_master_stop(cmd);

        esp_err_t ret = i2c_master_cmd_begin(port, cmd, pdMS_TO_TICKS(20));
        i2c_cmd_link_delete(cmd);

        if (ret == ESP_OK) {
            printf("  [FOUND] Dispositivo em 0x%02X (decimal: %d)\n", addr, addr);
            devices_found++;
        }
    }
    
    printf("[I2C SCAN] Total encontrado: %d dispositivo(s)\n\n", devices_found);
    
    if (devices_found == 0) {
        printf("  ❌ NENHUM dispositivo encontrado!\n");
        printf("  Possíveis problemas:\n");
        printf("    1. Ligações SDA (GPIO21) e SCL (GPIO22) desligadas?\n");
        printf("    2. Faltam pull-ups nas linhas I2C?\n");
        printf("    3. PN532 não tem alimentação?\n");
        printf("    4. PN532 está com endereço configurado diferente?\n\n");
    }
}

