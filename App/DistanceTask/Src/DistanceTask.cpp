#include "DistanceTask.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"

extern osMessageQueueId_t DistanceStartQueueHandle;
extern osMessageQueueId_t UartTxQueueHandle;

//select falling or rising
uint16_t RISING_FLAG = 1;

double DISTANCE = 0;
uint32_t start_time = 0;
uint32_t end_time = 0;

extern "C"
void DistanceSensorTaskHandler(void *argument){
    HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);

    uint8_t signal;
    for(;;)
    {
        if(osMessageQueueGet(DistanceStartQueueHandle,&signal,
          0, osWaitForever) == osOK) {
            //activate ultrasound sensor
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
            osDelay(1);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
        }
    }
}

//start measuring distance
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 && RISING_FLAG)
  {
    start_time = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

    //change mode to falling edge
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
    RISING_FLAG = 0;
  }
  else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 && !(RISING_FLAG))
  {
    end_time = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    
    //change mode to rising edge
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
    RISING_FLAG = 1;

    //calculate distance
    if (end_time > start_time) {
      DISTANCE = (end_time - start_time) / 58;
    } else {
      DISTANCE = ((65535 - end_time) + start_time) / 58;
    }
    
    osMessageQueuePut(UartTxQueueHandle, &DISTANCE, 0, 0);
  }
}