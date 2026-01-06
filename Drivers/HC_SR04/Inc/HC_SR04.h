#ifndef __HC_SR04_H__
#define __HC_SR04_H__

#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include <cstdint>

class HC_SR04{
private:
  TIM_HandleTypeDef* const _htim = nullptr;
  GPIO_TypeDef* const _triggerPort;
  const uint16_t _triggerPin;

  bool _isDone = false;
  int32_t _distance = -1;

public:
  HC_SR04(const TIM_HandleTypeDef* timer,const GPIO_TypeDef* port,const uint16_t pin);

  void setTrigger();
  bool isDone() const;
  int32_t getDistance() const;

  void Timer_IC_Handler();
};

#endif // __HC_SR04_H__

