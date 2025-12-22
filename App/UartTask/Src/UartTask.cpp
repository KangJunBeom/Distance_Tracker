#include "UartTask.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"

extern "C"
void UartRxTaskHandler(void *argument){
    for(;;)
    {
        osDelay(1);
    }
}


extern "C"
void UartTxTaskHandler(void *argument){
    for(;;)
    {
        osDelay(1);
    }
}