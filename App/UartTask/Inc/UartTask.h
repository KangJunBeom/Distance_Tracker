/**
 ******************************************************************************
 * @file    UartTask.h
 * @brief   This File contains all uart device functions and definitions
 * @author  huigwang
 ******************************************************************************
 */

#ifndef __UART_TASK_H__
#define __UART_TASK_H__

#include "main.h"
#include "cmsis_os.h"
#include <stdint.h>
#include <stdbool.h>


#define DEBUG_UART
#ifndef DEBUG_UART
#define UART_STX 0x02
#define UART_ETX 0x03
#else
#define UART_STX '@'
#define UART_ETX '!'
#endif


typedef struct _command {
  uint8_t buf[20];
  uint16_t len;
} Command;


#define ACC_BUF_SIZE 64

typedef uint8_t buf_t;
typedef uint8_t buf_len_t;

typedef struct acc_buf {
  buf_t buf[ACC_BUF_SIZE];
  buf_len_t len;
} acc_buf_t;

bool process_incoming_servo_data(void);

#define RX_BUFFER_SIZE 32
void Handle_UART_Receive_IDLE(UART_HandleTypeDef *huart, uint16_t Size);

int parse_servo_input(const buf_t *data, uint16_t *angle);
#endif
