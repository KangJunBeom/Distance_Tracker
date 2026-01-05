#include "MainTask.h"
#include "cmsis_os2.h"

#include "SG90.h"
#include "HC_SR04.h"

extern osMessageQueueId_t AngleQueueHandle;

extern "C"
void UartRxTaskHandler(void *argument){
  SG90 servoMotor;
  HC_SR04 distanceSensor;
  uint32_t angle;

  for(;;){
    if(osMessageQueueGet(AngleQueueHandle,&angle,nullptr,osWaitForever) == osOK){

    }
  }
}
