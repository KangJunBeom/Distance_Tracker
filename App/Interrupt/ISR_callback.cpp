#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_uart.h"
#include "cmsis_os2.h"

extern osMessageQueueId_t AngleQueueHandle;

extern "C"
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
  uint32_t angle {0};



  osMessageQueuePut(AngleQueueHandle, &angle, osPriority_t::osPriorityISR, 0);
}

extern "C"
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){

}
