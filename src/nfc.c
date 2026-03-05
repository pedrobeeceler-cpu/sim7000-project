#include "nfc.h"
#include "drivers/pn532.h"
#include <stdio.h>

void nfc_init(void)
{
    pn532_init();
}

void nfc_poll_uid(void)
{
    uint8_t uid[10];
    uint8_t uid_len;

    if (pn532_read_uid(uid, &uid_len) == ESP_OK)
    {
        printf("✅ UID: ");

        for(int i=0;i<uid_len;i++)
            printf("%02X", uid[i]);

        printf("\n");
    }
}