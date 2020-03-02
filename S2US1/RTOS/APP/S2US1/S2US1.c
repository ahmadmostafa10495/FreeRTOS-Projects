/*
 * S1US3.c
 *
 * Created: 27-Jan-20 5:57:09 PM
 *  Author: ahmed
 */ 

#include "FreeRTOS.h"
#include "task.h"
#include "DIO.h"
#include "lcd.h"
#include "pushButton.h"
#include "my_stdlib.h"
#include "led.h"
#include "queue.h"



static QueueHandle_t Queue_pushbuttonStateValue;
static QueueHandle_t Queue_LCDPrintedString;
/* Task to be created. */
static void InitsTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint8 ) pvParameters ) == 1 );

    for( ;; )
    {
        /* pb Init */
        pushButton_Init(BTN_0);
        /* LCD Init */
        LCD_Init();
        vTaskSuspend(NULL);
    }
}

static void pushButtonUpdateTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint8 ) pvParameters ) == 1 );
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for( ;; )
    {
        /* Update the pb status */

        static uint8 pushButtonstate = 0;
        pushButton_Update();
        pushButtonstate = pushButton_GetStatus(BTN_0);
        xQueueSend(Queue_pushbuttonStateValue, &pushButtonstate, 0);
        vTaskDelayUntil(&xLastWakeTime, 25/portTICK_PERIOD_MS);
    }
}


/* Task to be created. */
static void LCDUpdateTask( void * pvParameters )
{
   
    configASSERT( ( ( uint8 ) pvParameters ) == 1 );
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    uint8 pushButtonState = 0;
    uint8 LCD_String[13];
    for( ;; )
    {
  

          /* Updates the LCD display */

        xQueueReceive(Queue_pushbuttonStateValue, &pushButtonState, 0);
        if(pushButtonState == Pressed)
        {
          /* Display Over-written if pb pressed */
            LCD_sendString_RowCol(1,0,(uint8*) "over-written");
            vTaskDelayUntil(&xLastWakeTime, 25/portTICK_PERIOD_MS);
        }
          /* Receive and display */

        xQueueReceive(Queue_LCDPrintedString, &LCD_String, 0);
            
            LCD_sendString_RowCol(0,0,(uint8*)LCD_String);
            LCD_String[0] = 0;
            vTaskDelayUntil(&xLastWakeTime, 200/portTICK_PERIOD_MS);
            LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);

    }
}

/* Task to be created. */
static void LCDSendMessage( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint8 ) pvParameters ) == 1 );
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for( ;; )
    {
          /* sends periodic data */
        xQueueSend(Queue_LCDPrintedString, (uint8*)"Hello LCD !!", 0);
        vTaskDelayUntil(&xLastWakeTime, 400/portTICK_PERIOD_MS);


    }
}



 void S2US1(void)
 {

    Queue_pushbuttonStateValue = xQueueCreate( 1, sizeof( uint8 ) );
    Queue_LCDPrintedString = xQueueCreate( 1, sizeof( uint8 ) * 13 );
    xTaskCreate(InitsTask, "Inits", 100, (void *) 1, 4, NULL);
    xTaskCreate(pushButtonUpdateTask, "PBU", 100, (void *) 1, 3, NULL);
    xTaskCreate(LCDUpdateTask, "LUT", 100, (void *) 1, 1, NULL);
    xTaskCreate(LCDSendMessage, "PBD", 100, (void *) 1, 2, NULL);


    /* Start Scheduler */
    vTaskStartScheduler();
    
 }