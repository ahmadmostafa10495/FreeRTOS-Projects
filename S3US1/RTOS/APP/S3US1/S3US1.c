/*
 * S3US1.c
 *
 * Created: 30-Jan-20 5:07:03 PM
 *  Author: Omar Zayed
 */ 

#include "FreeRTOS.h"
#include "task.h"
#include "DIO.h"
#include "lcd.h"
#include "UART.h"
#include "LM35.h"
#include "pushButton.h"
#include "my_stdlib.h"
#include "queue.h"

static void UARTtest_cbk(void);


QueueHandle_t pushButtonQueue;
QueueHandle_t LM35Queue;
QueueHandle_t UARTQueue;


/* Task to be created. */
static void InitsTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32 ) pvParameters ) == 1 );

    for( ;; )
    {
        /* UART init */
        UART_cfg_s UART_cfg = {UART_INTERRUPT, RECEIVER, UART_NO_DOUBLE_SPEED, UART_ONE_STOP_BIT, UART_NO_PARITY, UART_8_BIT, 9600, NULL, UARTtest_cbk, NULL};
        UART_Init(&UART_cfg);
        /* pb init */
        pushButton_Init(BTN_0);
        /* LM35 init */
        LM35_Init();
        /* LCD init */
        LCD_Init();
        vTaskSuspend(NULL);
    }
}

static void pushButtonUpdateTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32 ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Updates the pb status */
        static uint8 ButtonStatus = LOW;
        pushButton_Update();

        if(pushButton_GetStatus(BTN_0) == Pressed)
        {
            ButtonStatus = HIGH;
        /* Sends the pb status */
            xQueueOverwrite(pushButtonQueue, &ButtonStatus);
        }
        else 
        {
        /* Sends the pb status */
            ButtonStatus = LOW;
            xQueueOverwrite(pushButtonQueue, &ButtonStatus);
        }
        vTaskDelay(25/portTICK_PERIOD_MS);
    }
}

static void LM35UpdateTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32 ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Trigger the LM35 every 1 second */

        LM35_StartConversion();

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}


/* Task to be created. */
static void LCDUpdateTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32 ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Updates the LCD display */
        static uint8 pushButtonStatus = 0;
        static uint16 LM35Reading = 0;
        static uint8 UARTReading = 0;
        static uint8 charData = 0;
        static uint8 charPos = 0;
        static uint8 gau8_buff1[5];
        xQueuePeek(pushButtonQueue, &pushButtonStatus, 0);

        if(pushButtonStatus == HIGH)
        {
            LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
            LCD_gotoRowColumn(0,0);
        }
        else
        {
            LM35_GetReading(&LM35Reading);
            my_itoa(LM35Reading, gau8_buff1, 10);
            LCD_gotoRowColumn(0, 0);
            LCD_displayString((uint8*)"Temperature : ");
            LCD_displayString(gau8_buff1);
            
        /* Checks if a message is received through UART */
            while(uxQueueMessagesWaiting(UARTQueue) > 0)
            {
                if(charPos == 16)
                {
        /* if the received message is longer than 16 character it clears the LCD */
                    LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
                    my_itoa(LM35Reading, gau8_buff1, 10);
                    LCD_gotoRowColumn(0, 0);
                    LCD_displayString((uint8*)"Temperature : ");
                    LCD_displayString(gau8_buff1);

                    charPos = 0;
                }
                LCD_gotoRowColumn(1, charPos);
                xQueueReceive(UARTQueue, &UARTReading, 0);
                my_itoa(UARTReading, gau8_buff1, 10);
                charData = UARTReading;
                LCD_displayCharacter(charData);
                charPos++;

            }
        }
                
        vTaskDelay(100/portTICK_PERIOD_MS);

    }
}

/* Task to be created. */
static void UARTUpdateTASK( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32 ) pvParameters ) == 1 );

    for( ;; )
    {
          /* Task code goes here. */
            vTaskDelay(25/portTICK_PERIOD_MS);
    }
}



 void S3US1(void)
 {
    pushButtonQueue = xQueueCreate( 1, sizeof( uint8 ) );
    LM35Queue = xQueueCreate( 1, sizeof( uint8 ) );
    UARTQueue = xQueueCreate( 16, sizeof( uint8 ) );
    
    xTaskCreate(InitsTask, "Inits", 100, (void *) 1, 4, NULL);
    xTaskCreate(pushButtonUpdateTask, "PBU", 100, (void *) 1, 3, NULL);
    xTaskCreate(LM35UpdateTask, "LU", 100, (void *) 1, 3, NULL);
    xTaskCreate(LCDUpdateTask, "LCDU", 100, (void *) 1, 2, NULL);
    xTaskCreate(UARTUpdateTASK, "UARTU", 100, (void *) 1, 1, NULL);


    /* Start Scheduler */
    vTaskStartScheduler();
    
    while(1)
    {
        
    }
    
 }
 

 static void UARTtest_cbk(void)
 {
         /* receives byte */
     static uint8 ReceivedByte = 0;
     UART_ReceiveByte(&ReceivedByte);
     xQueueSend(UARTQueue, &ReceivedByte, 0);
 }