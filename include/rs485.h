#pragma once
#include <stddef.h>

void rs485_init(void);

// envia linha terminada em \n
void rs485_send_line(const char *line);

// tenta receber uma linha (non-block). retorna 1 se recebeu.
int rs485_try_read_line(char *out, size_t out_max);