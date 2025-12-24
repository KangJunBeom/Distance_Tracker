#include "ServoTask.h"
#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal_uart.h"
#include "tim.h"
#include <cstdint>
#include <stdlib.h>

extern osMessageQueueId_t ServoAngleQueueHandle;
extern osMessageQueueId_t DistanceStartQueueHandle;

extern "C"
void ServoMotorTaskHandler(void *argument){
    
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    int angle = 0;

    for(;;)
    {   
        
        if(osMessageQueueGet(ServoAngleQueueHandle, &angle, NULL, osWaitForever) == osOK) {
            
            if(angle < 0) angle = 0;
            if(angle > 180) angle = 180;

            int pulse_val = 50 + (angle * 200 / 180);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse_val);
            
            uint8_t startSignal = 1;
            uint32_t t = (angle * 320 / 180);
            osDelay(t); 
            osMessageQueuePut(DistanceStartQueueHandle, &startSignal, 0, 0);

        }
    }
    
}