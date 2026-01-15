#include "cmsis_os2.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>

#include "HC_SR04.h"

#define DEBUG_UART
#ifndef DEBUG_UART
#define UART_STX 0x02
#define UART_ETX 0x03
#else
#define UART_STX '@'
#define UART_ETX '!'
#endif

#define ACC_BUF_SIZE 64
#define RX_BUFFER_SIZE 32

typedef uint8_t buf_t;
typedef uint8_t buf_len_t;

typedef struct _command {
  uint8_t buf[20];
  uint16_t len;
} Command;

typedef struct acc_buf {
  buf_t buf[ACC_BUF_SIZE];
  buf_len_t len;
} acc_buf_t;

uint8_t rxBuffer[RX_BUFFER_SIZE];
uint16_t lastPos;
extern osMessageQueueId_t AngleQueueHandle;

extern HC_SR04 distanceSensor;

bool process_incoming_servo_data(Command *msg);
void Handle_UART_Receive_IDLE(UART_HandleTypeDef *huart, uint16_t Size);
int parse_servo_input(const buf_t *data, uint16_t *angle);
static inline void shift_acc_buf(acc_buf_t *acc_buf, buf_len_t ofs);

extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,
                                           uint16_t Size) {
  Handle_UART_Receive_IDLE(huart, Size);
}

extern "C" void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  distanceSensor.Timer_IC_Handler();
}

/*
 * @author huigwang
 */
static inline void shift_acc_buf(acc_buf_t *acc_buf, buf_len_t ofs) {
  if (ofs == 0)
    return;

  if (ofs >= acc_buf->len) {
    acc_buf->len = 0;
    acc_buf->buf[0] = '\0';
    return;
  }

  memmove(acc_buf->buf, acc_buf->buf + ofs,
          (acc_buf->len - ofs) * sizeof(buf_t));
  acc_buf->len -= ofs;
  acc_buf->buf[acc_buf->len] = '\0';
  return;
}

/**
 * @author huigwang
 * @brief 메시지 큐에서 데이터를 읽어 STX/ETX 기반의 패킷을 파싱하고 처리합니다.
 * @note 인터럽트로부터 들어온 데이터를 누적 버퍼에 저장하고, STX/ETX로 감싸진
 *패킷을 찾아 서보 각도 명령을 추출합니다. 유효한 명령이 발견되면 별도의 큐로
 *전송합니다.
 * @details  입력은 UartRxQueue에서 읽어오며, 출력은 AngleQueue로
 *전송됩니다.
 */
bool process_incoming_servo_data(Command *rx_msg) {
  uint16_t angle;

  static acc_buf_t acc_buf = {{0}, 0};
  buf_len_t *len = &acc_buf.len;
  buf_t *buf = acc_buf.buf;

  // if no space in acc_buf, shift half
  if (*len + rx_msg->len >= ACC_BUF_SIZE - 1) {
    (void)shift_acc_buf(&acc_buf, *len / 2);
    return false;
  }

  memcpy(&buf[*len], rx_msg->buf, rx_msg->len);
  *len += rx_msg->len;
  buf[*len] = '\0';

  // parsing by STX/ETX
  buf_t *stx_ptr = (buf_t *)memchr(buf, UART_STX, *len);
  if (stx_ptr != NULL) {
    // dismiss data before STX
    if (stx_ptr != buf) {
      buf_len_t offset = (buf_len_t)(stx_ptr - buf) / sizeof(buf_t);
      shift_acc_buf(&acc_buf, offset);
    }
    buf_t *etx_ptr = (buf_t *)memchr(buf, UART_ETX, *len);
    /* input length must be 5: STX + 'S' + 3 digit angle + ETX */
    if (etx_ptr != NULL && etx_ptr > stx_ptr && &buf[5] == etx_ptr) {
      // parse data between STX and ETX
      // send to queue if valid
      if (parse_servo_input(buf + 1, &angle)) {
        osMessageQueuePut(AngleQueueHandle, &angle, osPriorityISR, 0);
      }
      // shift buffer after ETX to front
      buf_len_t offset = (buf_len_t)(etx_ptr - buf) / sizeof(buf_t);
      shift_acc_buf(&acc_buf, offset);
      return true;
    }
  } else {
    if (*len > ACC_BUF_SIZE / 2) {
      memset(buf, 0, *len);
      *len = 0;
      return false;
    }
  }
  return false;
}

/*
 * @author huigwang
 * @brief UART DMA 수신 데이터를 처리하고 큐에 전송하는 함수
 * @param huart: UART 핸들러
 * @param Size: 현재 DMA가 수신한 총 데이터 위치 (또는 남은 데이터 양에 따른
 * 계산 값)
 */
void Handle_UART_Receive_IDLE(UART_HandleTypeDef *huart, uint16_t Size) {
  if (huart->Instance == USART2) {
    uint16_t currentPos = Size;

    if (currentPos == lastPos)
      return;

    Command msg;
    memset(&msg, 0, sizeof(msg));

    // 데이터가 버퍼 중간에 연속적으로 쌓임
    if (currentPos > lastPos) {
      uint16_t length = currentPos - lastPos;
      msg.len = length;
      // 버퍼 오버플로우 방지를 위해 크기 체크 권장
      if (length <= sizeof(msg.buf)) {
        memcpy(msg.buf, &rxBuffer[lastPos], length);
      }
    }
    // 상황 2: 버퍼 끝을 찍고 다시 0번지로 돌아옴 (Wrap-around)
    else if (currentPos < lastPos) {
      uint16_t lenToEnd = RX_BUFFER_SIZE - lastPos;
      msg.len = lenToEnd + currentPos;

      // 1. 마지막 위치부터 버퍼 끝까지 복사
      if (lenToEnd <= sizeof(msg.buf)) {
        memcpy(msg.buf, &rxBuffer[lastPos], lenToEnd);
      }
      // 2. 버퍼 시작(0)부터 현재 위치까지 복사
      if (msg.len <= sizeof(msg.buf)) {
        memcpy(&msg.buf[lenToEnd], &rxBuffer[0], currentPos);
      }
    }

    lastPos = currentPos; // 다음 콜백을 위해 위치 업데이트

    //
    (void)process_incoming_servo_data(&msg);
  }
}

/* @author huigwang
 * @brief 수신된 문자열에서 서보 ID와 각도를 파싱하여 추출.
 * @param data 파싱할 데이터 문자열.
 * @return 파싱 실패 시 0 반환.
 */
int parse_servo_input(const buf_t *data, uint16_t *angle) {
  int temp;
  int result = sscanf((char *)data, "%*c%d", &temp);

  if (result != 1) {
    return 0;
  }

  if (temp <= 180) {
    *angle = (uint16_t)temp;
    return 1;
  }

  // 파싱 실패
  return 0;
}
