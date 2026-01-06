#include "MainTask.h"
#include "cmsis_os2.h"

#include "SG90.h"
#include "HC_SR04.h"

#include "stm32f1xx_hal_tim.h"
#include "tim.h"

extern osMessageQueueId_t AngleQueueHandle;

extern "C"
void MainTaskHandler(void *argument){
  SG90 servoMotor(&htim2,TIM_CHANNEL_1);
  HC_SR04 distanceSensor;
  uint16_t angle;

  for(;;){
    if(osMessageQueueGet(AngleQueueHandle,&angle,nullptr,osWaitForever) == osOK){

    }
  }
}
