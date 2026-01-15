#include "HC_SR04.h"

HC_SR04::HC_SR04(const TIM_HandleTypeDef* timer, const GPIO_TypeDef* port, const uint16_t pin)
    : _htim(const_cast<TIM_HandleTypeDef*>(timer)), _triggerPort(const_cast<GPIO_TypeDef*>(port)), _triggerPin(pin) {

    // HAL_TIM_IC_Start_IT(_htim, TIM_CHANNEL_1);
}

void HC_SR04::setTrigger() {
    _isDone = false;
    _distance = -1;

    HAL_GPIO_WritePin(_triggerPort, _triggerPin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(_triggerPort, _triggerPin, GPIO_PIN_RESET);
}

volatile bool HC_SR04::isDone() const {
    return _isDone;
}

int32_t HC_SR04::getDistance() const {
    return _distance;
}

void HC_SR04::Timer_IC_Handler() {
    static uint32_t p1 = 0;
    static uint32_t p2 = 0;
    static bool isFirstEdge = true;

    if (isFirstEdge) {
        p1 = HAL_TIM_ReadCapturedValue(_htim, TIM_CHANNEL_1);
        isFirstEdge = false;
        __HAL_TIM_SET_CAPTUREPOLARITY(_htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
    } else {
        p2 = HAL_TIM_ReadCapturedValue(_htim, TIM_CHANNEL_1);
        uint32_t diff = (p2 >= p1) ? (p2 - p1) : ((_htim->Instance->ARR - p1) + p2);
        _distance = diff / 58;
        _isDone = true;
        isFirstEdge = true;

        __HAL_TIM_SET_CAPTUREPOLARITY(_htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
    }
}
