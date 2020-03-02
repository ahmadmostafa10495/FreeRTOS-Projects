/*
 * S1US3.c
 *
 * Created: 27-Jan-20 5:57:09 PM
 *  Author: Omar Zayed
 */ 

#include "FreeRTOS.h"
#include "task.h"
#include "DIO.h"
#include "lcd.h"
#include "pushButton.h"
#include "my_stdlib.h"
#include "led.h"
#include "queue.h"




/* Task to be created. */
static void InitsTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Led Init */
        LED_cfg_s_t str_LEDInfo = {LED_0};
        Led_Init(&str_LEDInfo);
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
    configASSERT( ( ( QueueHandle_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Task code goes here. */

        static uint32 pushButtonCounter = 0;
        static boolean_t bl_button = FALSE;
        static uint8 pushButtonTime = 0;
        pushButton_Update();

        if(pushButton_GetStatus(BTN_0) == Pressed)
        {
            pushButtonCounter += 25;
            bl_button = TRUE;
        }
        else if((pushButton_GetStatus(BTN_0) == Released) && (bl_button == TRUE))
        {
            /* Calculates the push button on time and sets the toggle time of the leds */

            pushButtonTime = pushButtonCounter / 1000;
            xQueueSend(pvParameters, &pushButtonTime, 0);
            pushButtonCounter = 0;
            bl_button = FALSE;
        }
        vTaskDelay(25/portTICK_PERIOD_MS);
    }
}


/* Task to be created. */
static void LED_Toggle( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( QueueHandle_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Task code goes here. */
        static uint8 pushButtonTime = 0;
        xQueueReceive(pvParameters, &pushButtonTime, 0);
        if(pushButtonTime >= 4)
        {
/* Toggles the leds and sets the toggle time*/
            Led_Toggle(LED_0);
            vTaskDelay(100/portTICK_PERIOD_MS);
        }
        else if(pushButtonTime >= 2)
        {
/* Toggles the leds and sets the toggle time*/
            Led_Toggle(LED_0);
            vTaskDelay(400/portTICK_PERIOD_MS);
        }
        else
        {

            /* Stops the leds if the time below 2 seconds */
            Led_Off(LED_0);
            vTaskDelay(100/portTICK_PERIOD_MS);
        }

    }
}



 void S1US3(void)
 {
    QueueHandle_t Queue_pushbuttonTimeValue;
    Queue_pushbuttonTimeValue = xQueueCreate( 10, sizeof( uint8 ) );
    xTaskCreate(InitsTask, "Inits", 100, (void *) 1, 4, NULL);
    xTaskCreate(pushButtonUpdateTask, "PBU", 100, (void *) Queue_pushbuttonTimeValue, 3, NULL);
    xTaskCreate(LED_Toggle, "PBD", 100, (void *) Queue_pushbuttonTimeValue, 1, NULL);


    /* Start Scheduler */
    vTaskStartScheduler();
    
   
    
 }