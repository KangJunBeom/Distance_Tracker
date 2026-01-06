#ifndef __SG90_H__
#define __SG90_H__
#include "stm32f1xx_hal.h"

class SG90{
public:
    SG90(TIM_HandleTypeDef* htim, uint32_t channel);
    uint32_t setAngle(int angle);

private:
    TIM_HandleTypeDef* _htim;
    uint32_t _channel;
    int _currentAngle;
};

#endif // __SG90_H__
