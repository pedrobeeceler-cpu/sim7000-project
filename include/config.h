#pragma once

// Identidade (muda conforme estação/doca)
#define STATION_ID   1
#define DOCK_ID      1

// NFC PN532 (I2C)
#define NFC_I2C_SDA  21
#define NFC_I2C_SCL  22

// LOCK (ajusta ao teu hardware)
#define LOCK_GPIO    26

// RS485 (UART) — AJUSTA aos pinos reais do teu kit
#define RS485_UART           2
#define RS485_TX_GPIO        17
#define RS485_RX_GPIO        16
#define RS485_RTS_GPIO       4     // DE/RE do transceiver RS485 (se tiveres). Se não tiveres, diz-me.
#define RS485_BAUD           115200