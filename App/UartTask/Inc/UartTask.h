#ifndef __UART_TASK_H__
#define __UART_TASK_H__

#include <stdint.h>

typedef struct _command {
    uint16_t length;
    char data[32];
} Command;

#endif // __UART_TASK_H__