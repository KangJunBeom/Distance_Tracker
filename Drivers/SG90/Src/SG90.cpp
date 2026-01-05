#include "SG90.h"
#include <stdlib.h>

SG90::SG90(TIM_HandleTypeDef* htim, uint32_t channel)
    : _htim(htim), _channel(channel), _currentAngle(0) {
    HAL_TIM_PWM_Start(_htim, _channel);
}

uint32_t SG90::setAngle(int angle) {
    if(angle < 0) angle = 0;
    if(angle > 180) angle = 180;

    int pulse_val = 50 + (angle * 200 / 180);
    __HAL_TIM_SET_COMPARE(_htim, _channel, pulse_val);

    // 이동에 필요한 지연 시간(ms) 계산하여 반환
    uint32_t delay_ms = (abs(angle - _currentAngle) * 400) / 180;
    _currentAngle = angle;

    return delay_ms;
}
