/*
 * s2us2.c
 *
 * Created: 28-Jan-20 10:29:41 PM
 *  Author: ahmed
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
        /* LCD init */
        LCD_Init();
        vTaskSuspend(NULL);
    }
}

static void pushButtonUpdateTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Updates the pb state */
        static uint8 ButtonStatus0 = 0;
        static uint8 ButtonStatus1 = 0;
        pushButton_Update();
        ButtonStatus0 = pushButton_GetStatus(BTN_0);
        /* Sends the pb state */
        xQueueOverwrite(pushButton0Queue, &ButtonStatus0);
        ButtonStatus1 = pushButton_GetStatus(BTN_1);
        /* Sends the pb state */
        xQueueOverwrite(pushButton1Queue, &ButtonStatus1);
        vTaskDelay(25/portTICK_PERIOD_MS);

    }
}



/* Task to be created. */
static void LCDUpdateTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    uint8 Player0Score = 0;
    uint8 Player1Score = 0;
    uint8 Player0State = 0;
    uint8 Player1State = 0;
    uint8 LCD_Buff[5];
    uint8 counter;
    for( ;; )
    {
        /* Updates the playes' score */
        xQueuePeek(pushButton0Queue, &Player0State, 0);
        
        if(Player0State == Prepressed)
        {
            Player0Score ++;
            if (Player1Score != 0)
            {
                Player1Score --;
            } 
            else
            {
            }
        }
        xQueuePeek(pushButton1Queue, &Player1State, 0);
        if(Player1State == Prepressed)
        {
            Player1Score ++;
            if (Player0Score != 0)
            {
                Player0Score --;
            }
            else
            {
            }
        }    
        /* Displays the score on the LCD */
        LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
        my_itoa(Player0Score, LCD_Buff, 10);
        LCD_sendString_RowCol(0, 0, (uint8*)"Player1: ");
        LCD_sendString_RowCol(0, 10, LCD_Buff);
        my_itoa(Player1Score, LCD_Buff, 10);
        LCD_sendString_RowCol(1, 0, (uint8*)"Player2: ");
        LCD_sendString_RowCol(1, 10, LCD_Buff);
        if (Player0Score >= 50)
        {
            for (counter = 0; counter < 12; counter++)
            {
            /* Display the winning player */
                LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
                LCD_sendString_RowCol(0, 0, (uint8*)"Player1 WINS");
                LCD_sendString_RowCol(1, 0, (uint8*)"CONGRATULATIONS");
                vTaskDelay(200/portTICK_PERIOD_MS);
                LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
                LCD_sendString_RowCol(0, 0, (uint8*)"Player1 WINS");
                vTaskDelay(200/portTICK_PERIOD_MS);
            }
            /* Resets the game */
            Player0Score = 0;
            Player1Score = 0;
            LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
        } 
        else if (Player1Score >= 50)
        {
            for (counter = 0; counter < 12; counter++)
            {
            /* Display the winning player */
                LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
                LCD_sendString_RowCol(0, 0, (uint8*)"Player2 WINS");
                LCD_sendString_RowCol(1, 0, (uint8*)"CONGRATULATIONS");
                vTaskDelay(200/portTICK_PERIOD_MS);
                LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
                LCD_sendString_RowCol(0, 0, (uint8*)"Player2 WINS");
                vTaskDelay(200/portTICK_PERIOD_MS);
            }
            /* Resets the game */
            Player0Score = 0;
            Player1Score = 0;
            LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
        }
        vTaskDelay(25/portTICK_PERIOD_MS);

    }
}




 void S2US3(void)
 {
    pushButton0Queue = xQueueCreate( 1, sizeof( uint8 ) );
    pushButton1Queue = xQueueCreate( 1, sizeof( uint8 ) );



    xTaskCreate(InitsTask, "Inits", 100, (void *) 1, 4, NULL);
    xTaskCreate(pushButtonUpdateTask, "PBU", 100, (void *) 1, 3, NULL);
    xTaskCreate(LCDUpdateTask, "LUT", 100, (void *) 1, 2, NULL);


    /* Start Scheduler */
    vTaskStartScheduler();
    

    
 }