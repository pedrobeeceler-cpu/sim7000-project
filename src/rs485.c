#include "rs485.h"
#include "config.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include <string.h>

static char rxbuf[256];
static int rxlen = 0;

void rs485_init(void)
{
    uart_config_t cfg = {
        .baud_rate = RS485_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    uart_driver_install(RS485_UART, 2048, 0, 0, NULL, 0);
    uart_param_config(RS485_UART, &cfg);
    uart_set_pin(RS485_UART, RS485_TX_GPIO, RS485_RX_GPIO, RS485_RTS_GPIO, UART_PIN_NO_CHANGE);

    // RS485 half-duplex mode (usa RTS como DE)
    uart_set_mode(RS485_UART, UART_MODE_RS485_HALF_DUPLEX);
}

void rs485_send_line(const char *line)
{
    uart_write_bytes(RS485_UART, line, strlen(line));
}

int rs485_try_read_line(char *out, size_t out_max)
{
    uint8_t tmp[64];
    int n = uart_read_bytes(RS485_UART, tmp, sizeof(tmp), 0);
    if (n <= 0) return 0;

    for (int i = 0; i < n; i++) {
        if (rxlen < (int)sizeof(rxbuf) - 1) rxbuf[rxlen++] = (char)tmp[i];

        if (tmp[i] == '\n') {
            rxbuf[rxlen] = 0;
            strncpy(out, rxbuf, out_max);
            out[out_max - 1] = 0;
            rxlen = 0;
            return 1;
        }
    }
    return 0;
}