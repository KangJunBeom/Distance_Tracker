#include "ServoTask.h"
#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

extern "C"
void ServoMotorTaskHandler(void *argument){
    
    int angle = 0; //example angle to move
    int delay = 0.0;
    extern TIM_HandleTypeDef htim2;
    extern osMessageQueueId_t ServoAngleQueueHandle;
    extern osMessageQueueId_t DistanceStartQueueHandle;

    for(;;)
    {
        if(osMessageQueueGet(ServoAngleQueueHandle, &angle, NULL, osWaitForever) == osOK) {

        if(angle < 0) angle = 0;
        if(angle > 180) angle = 180;

        int pulse_val = 50 + (angle * 200 / 180);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse_val);
        
        //delay for servo to reach the position (approx 320ms for 180 degrees)
        delay = (angle*100 / 180) * 320;
        
        //Send signal to Distance Sensor Task to start measurement(dummy data)
        uint8_t startSignal = 1;
        osMessageQueuePut(DistanceStartQueueHandle, &startSignal, 0, delay);
    }
    }
}