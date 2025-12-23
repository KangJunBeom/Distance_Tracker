/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "UartTask.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for UartRxTask */
osThreadId_t UartRxTaskHandle;
const osThreadAttr_t UartRxTask_attributes = {
  .name = "UartRxTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UartTxTask */
osThreadId_t UartTxTaskHandle;
const osThreadAttr_t UartTxTask_attributes = {
  .name = "UartTxTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ServoMotorTask */
osThreadId_t ServoMotorTaskHandle;
const osThreadAttr_t ServoMotorTask_attributes = {
  .name = "ServoMotorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for DistanceSensorTask */
osThreadId_t DistanceSensorTaskHandle;
const osThreadAttr_t DistanceSensorTask_attributes = {
  .name = "DistanceSensorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for UartRxQueue */
osMessageQueueId_t UartRxQueueHandle;
const osMessageQueueAttr_t UartRxQueue_attributes = {
  .name = "UartRxQueue"
};
/* Definitions for ServoAngleQueue */
osMessageQueueId_t ServoAngleQueueHandle;
const osMessageQueueAttr_t ServoAngleQueue_attributes = {
  .name = "ServoAngleQueue"
};
/* Definitions for DistanceStartQueue */
osMessageQueueId_t DistanceStartQueueHandle;
const osMessageQueueAttr_t DistanceStartQueue_attributes = {
  .name = "DistanceStartQueue"
};
/* Definitions for UartTxQueue */
osMessageQueueId_t UartTxQueueHandle;
const osMessageQueueAttr_t UartTxQueue_attributes = {
  .name = "UartTxQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void UartRxTaskHandler(void *argument);
extern void UartTxTaskHandler(void *argument);
extern void ServoMotorTaskHandler(void *argument);
extern void DistanceSensorTaskHandler(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of UartRxQueue */
  UartRxQueueHandle = osMessageQueueNew (8, sizeof(Command), &UartRxQueue_attributes);

  /* creation of ServoAngleQueue */
  ServoAngleQueueHandle = osMessageQueueNew (8, sizeof(uint16_t), &ServoAngleQueue_attributes);

  /* creation of DistanceStartQueue */
  DistanceStartQueueHandle = osMessageQueueNew (8, sizeof(uint8_t), &DistanceStartQueue_attributes);

  /* creation of UartTxQueue */
  UartTxQueueHandle = osMessageQueueNew (8, sizeof(Command), &UartTxQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of UartRxTask */
  UartRxTaskHandle = osThreadNew(UartRxTaskHandler, NULL, &UartRxTask_attributes);

  /* creation of UartTxTask */
  UartTxTaskHandle = osThreadNew(UartTxTaskHandler, NULL, &UartTxTask_attributes);

  /* creation of ServoMotorTask */
  ServoMotorTaskHandle = osThreadNew(ServoMotorTaskHandler, NULL, &ServoMotorTask_attributes);

  /* creation of DistanceSensorTask */
  DistanceSensorTaskHandle = osThreadNew(DistanceSensorTaskHandler, NULL, &DistanceSensorTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_UartRxTaskHandler */
/**
  * @brief  Function implementing the UartRxTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_UartRxTaskHandler */
__weak void UartRxTaskHandler(void *argument)
{
  /* USER CODE BEGIN UartRxTaskHandler */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END UartRxTaskHandler */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

