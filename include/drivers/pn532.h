#ifndef PN532_H
#define PN532_H

#include <stdint.h>
#include "esp_err.h"

esp_err_t pn532_init(void);
esp_err_t pn532_read_uid(uint8_t *uid, uint8_t *uid_len);

#endif