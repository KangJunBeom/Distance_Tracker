#include "MainTask.h"
#include "cmsis_os2.h"

#include "SG90.h"
#include "HC_SR04.h"

#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_uart.h"
#include "tim.h"
#include "usart.h"

#include <stdio.h>
#include <string.h>

#define RX_BUFFER_SIZE 32

extern uint8_t rxBuffer[RX_BUFFER_SIZE];
extern osMessageQueueId_t AngleQueueHandle;

HC_SR04 distanceSensor(&htim3,GPIOA,GPIO_PIN_12);

extern "C"
void MainTaskHandler(void *argument){
  SG90 servoMotor(&htim2,TIM_CHANNEL_1);
  uint16_t angle;


  char message[32] = "";
  bool bFail = false;

  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rxBuffer, RX_BUFFER_SIZE);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);

  for(;;){
    if(osMessageQueueGet(AngleQueueHandle,&angle,nullptr,osWaitForever) == osOK){

      bFail = false;

      distanceSensor.setTrigger();

      uint32_t startTick = osKernelGetTickCount();

      while(distanceSensor.isDone() == false){
        if(osKernelGetTickCount() > startTick + 500){
          bFail = true;
          break;
        }
      }


      if(bFail){
        sprintf(message,"-1\n");
        HAL_UART_Transmit(&huart2,(uint8_t*)message,strlen(message),HAL_MAX_DELAY);
      }
      else{
        uint32_t dist = distanceSensor.getDistance();

        sprintf(message,"%d %d\n",angle,dist);
        HAL_UART_Transmit(&huart2,(uint8_t*)message,strlen(message),HAL_MAX_DELAY);
      }

    }
  }
}
