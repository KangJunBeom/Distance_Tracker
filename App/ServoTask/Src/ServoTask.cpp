#include "ServoTask.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"

extern "C"
void ServoMotorTaskHandler(void *argument){
    for(;;)
    {
        osDelay(1);
    }
}