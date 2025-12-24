#include "DistanceTask.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"

extern osMessageQueueId_t DistanceStartQueueHandle;
extern osMessageQueueId_t UartTxQueueHandle;

//select falling or rising
uint16_t Rising_flag = 1;

double Distance = 0;
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
  if(htim->Instance == TIM3 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
    if (Rising_flag)
    {
      start_time = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

      //change mode to falling edge
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
      Rising_flag = 0;
    }
    else
    {
      end_time = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
      
      //change mode to rising edge
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
      Rising_flag = 1;

      //calculate distance
      if (end_time > start_time) {
        Distance = (double)(end_time - start_time) / 58;
      } else {
        Distance = (double)((65535 - end_time) + start_time) / 58;
      }
      
      osMessageQueuePut(UartTxQueueHandle, &Distance, 0, 0);
    }
  }
}