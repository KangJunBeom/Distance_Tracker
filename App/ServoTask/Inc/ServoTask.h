#ifndef __SERVO_TASK_H__
#define __SERVO_TASK_H__

#include "stm32f1xx_hal.h"

/* 함수 선언 (Prototype) */
/**
  * @brief 서보모터를 특정 각도로 이동시키는 함수
  * @param htim: PWM 설정이 완료된 타이머 핸들러 (예: htim2)
  * @param angle: 이동할 각도 (0 ~ 180)
  */

extern "C"
void ServoMotorTaskHandler(void *argument);

#endif /* __SERVO_TASK_H__ */