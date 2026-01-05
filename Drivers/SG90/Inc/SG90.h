#ifndef __SG90_H__
#define __SG90_H__
#include "stm32f1xx_hal.h"

class SG90{
public:
    SG90(TIM_HandleTypeDef* htim, uint32_t channel);

    // 플로우차트의 'Servo 모터 각도를 Angle로 설정' 및 '이동할 때까지 기다리기'를 위한 인터페이스
    uint32_t setAngle(int angle);

private:
    TIM_HandleTypeDef* _htim;
    uint32_t _channel;
    int _currentAngle;
};

#endif // __SG90_H__
