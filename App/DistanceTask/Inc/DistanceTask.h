#ifndef __DISTANCE_TASK_H__
#define __DISTANCE_TASK_H__

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f103xb.h"
#include "tim.h"

extern "C"
void DistanceSensorTaskHandler(void *argument);

#endif // __DISTANCE_TASK_H__