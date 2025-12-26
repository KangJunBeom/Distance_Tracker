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
    int target_angle = 0;
    int current_angle = 0;

    for(;;)
    {   
        
        if(osMessageQueueGet(ServoAngleQueueHandle, &target_angle, NULL, osWaitForever) == osOK) {
            
            if(target_angle < 0) target_angle = 0;
            if(target_angle > 180) target_angle = 180;

            if(target_angle==current_angle){
                uint8_t startSignal = 1;
                osMessageQueuePut(DistanceStartQueueHandle,&startSignal,0,0);
                continue;
            }

            int pulse_val = 50 + (target_angle * 200 / 180);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse_val);
            
            int angle_difference = (target_angle > current_angle) ? (target_angle-current_angle) : (current_angle-target_angle);
            uint32_t t = ((angle_difference * 400)/ 180);
            current_angle = target_angle;
            
            osDelay(t); 
            uint8_t startSignal = 1;
            osMessageQueuePut(DistanceStartQueueHandle, &startSignal, 0, 0);

        }
    }
    
}