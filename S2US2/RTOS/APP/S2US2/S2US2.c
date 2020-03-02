/*
 * s2us2.c
 *
 * Created: 28-Jan-20 10:29:41 PM
 *  Author: Omar Zayed
 */ 

#include "FreeRTOS.h"
#include "task.h"
#include "DIO.h"
#include "lcd.h"
#include "pushButton.h"
#include "my_stdlib.h"
#include "queue.h"
#include "led.h"

QueueHandle_t pushButton0Queue;
QueueHandle_t pushButton1Queue;
QueueHandle_t ledQueue;


/* Task to be created. */
static void InitsTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* pb init */
        pushButton_Init(BTN_0);
        pushButton_Init(BTN_1);

        /* led init */
        Led_Init(LED_0);

        /* LCD init */
        LCD_Init();
        vTaskSuspend(NULL);
    }
}

static void pushButton0UpdateTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Gets pb 0 status and sends it to the queue */
        static uint8_t ButtonStatus0 = LOW;
        static uint8_t ButtonStatus1 = LOW;
        xQueuePeek(pushButton1Queue, &ButtonStatus1, 0);
        pushButton_Update();

        /* pb 0 won't send its status if pb 1 is pressed already */
        if((pushButton_GetStatus(BTN_0) == Pressed) && (ButtonStatus1 == LOW))
        {
            ButtonStatus0 = HIGH;
            xQueueOverwrite(pushButton0Queue, &ButtonStatus0);
        }
        else 
        {
            ButtonStatus0 = LOW;
            xQueueOverwrite(pushButton0Queue, &ButtonStatus0);
        }
        vTaskDelay(25/portTICK_PERIOD_MS);
    }
}

static void pushButton1UpdateTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Gets pb 1 status and sends it to the queue */
        static uint8_t ButtonStatus0 = LOW;
        static uint8_t ButtonStatus1 = LOW;
        xQueuePeek(pushButton0Queue, &ButtonStatus0, 0);
        pushButton_Update();

        /* pb 1 won't send its status if pb 0 is pressed already */
        if((pushButton_GetStatus(BTN_1) == Pressed) && (ButtonStatus0 == LOW))
        {
            ButtonStatus1 = HIGH;
            xQueueOverwrite(pushButton1Queue, &ButtonStatus1);
        }
        else 
        {
            ButtonStatus1 = LOW;
            xQueueOverwrite(pushButton1Queue, &ButtonStatus1);
        }
        vTaskDelay(25/portTICK_PERIOD_MS);
    }
}


/* Task to be created. */
static void LCDUpdateTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Updates the LCD display */
        static uint8_t ledStatus = 0;
        xQueuePeek(ledQueue, &ledStatus, 0);

        if(ledStatus == 0)
        {
            LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
            LCD_gotoRowColumn(0,0);
            LCD_displayString((uint8_t*)"LED OFF");
        }
        else if(ledStatus == 1)
        {
            LCD_gotoRowColumn(0,0);
            LCD_displayString((uint8_t*)"LED ON Button 0");
        }
        else
        {
            LCD_gotoRowColumn(0,0);
            LCD_displayString((uint8_t*)"LED ON Button 1");
        }
                
        vTaskDelay(25/portTICK_PERIOD_MS);

    }
}

/* Task to be created. */
static void LEDONTASK( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
          /* Updates the LEDs status and sends the status and the pb activating it to the LCD */
          static uint8_t pushButton0Status = LOW;
          static uint8_t pushButton1Status = LOW;
        static uint8_t LCDUpdateStatus = 0;
        xQueuePeek(pushButton0Queue, &pushButton0Status, 0);
        xQueuePeek(pushButton1Queue, &pushButton1Status, 0);
          if(pushButton0Status == LOW && pushButton1Status == LOW)
          {
            Led_Off(LED_0);
            LCDUpdateStatus = 0;
            xQueueOverwrite(ledQueue, &LCDUpdateStatus);
        }
        else if(pushButton0Status == HIGH)
        {
            Led_On(LED_0);
            LCDUpdateStatus = 1;
            xQueueOverwrite(ledQueue, &LCDUpdateStatus);    
        }
        else
        {
            Led_On(LED_0);
            LCDUpdateStatus = 2;
            xQueueOverwrite(ledQueue, &LCDUpdateStatus);
        }
            vTaskDelay(25/portTICK_PERIOD_MS);
    }
}



 void S2US2(void)
 {
    pushButton0Queue = xQueueCreate( 1, sizeof( uint8_t ) );
    pushButton1Queue = xQueueCreate( 1, sizeof( uint8_t ) );
    ledQueue = xQueueCreate( 1, sizeof( uint8_t ) );


    xTaskCreate(InitsTask, "Inits", 100, (void *) 1, 4, NULL);
    xTaskCreate(pushButton1UpdateTask, "PBU", 100, (void *) 1, 3, NULL);
    xTaskCreate(pushButton0UpdateTask, "PBU", 100, (void *) 1, 3, NULL);
    xTaskCreate(LCDUpdateTask, "LUT", 100, (void *) 1, 2, NULL);
    xTaskCreate(LEDONTASK, "LOT", 100, (void *) 1, 1, NULL);


    /* Start Scheduler */
    vTaskStartScheduler();
    
 
    
 }