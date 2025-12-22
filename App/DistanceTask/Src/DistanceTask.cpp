#include "DistanceTask.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"

extern "C"
void DistanceSensorTaskHandler(void *argument){
    for(;;)
    {
        osDelay(1);
    }
}