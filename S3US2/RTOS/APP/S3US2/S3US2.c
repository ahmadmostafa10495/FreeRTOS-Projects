
/*
 * S3US2.c
 *
 * Created: 1/30/2020 11:35:02 AM
 *  Author: ahmad
 */ 
#include "FreeRTOS.h"
#include "task.h"
#include "DIO.h"
#include "lcd.h"
#include "pushButton.h"
#include "my_stdlib.h"
#include "queue.h"
#include "led.h"
#include "Keypad.h"
#include "UART.h"


static uint8 UART_dataRecieved[17];
static uint8 UART_data_RecievedIndex = 0;
volatile static uint8 UART_sendFlag = 1;
static QueueHandle_t pushButton0TOLCDQueue;
static QueueHandle_t pushButton0TOUARTQueue;
static QueueHandle_t LED1Queue;
static QueueHandle_t LED2Queue;
static QueueHandle_t keypadTOLCDQueue;
static QueueHandle_t keypadTOUARTQueue;


static void UART_TX_CBK(void);
static void UART_RX_CBK(void);

static void InitTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* pb init */
        pushButton_Init(BTN_0);
        /* leds init */
        LED_cfg_s_t LED0_info = {LED_0};
        LED_cfg_s_t LED1_info = {LED_1};
        Led_Init(&LED0_info);
        Led_Init(&LED1_info);
        /* Keypad init */
        Keypad_init();
        /* UART init */
        UART_cfg_s UART_info = {UART_INTERRUPT, TRANSCEIVER, UART_NO_DOUBLE_SPEED, UART_ONE_STOP_BIT, UART_NO_PARITY, UART_8_BIT, 9600,\
        UART_TX_CBK, UART_RX_CBK, NULL};
        UART_Init(&UART_info);
        /* LCD init */
        LCD_Init();
        vTaskSuspend(NULL);
    }
}

static void pushButtonAndLedsUpdateTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        static uint8 ButtonStatus01 = 0;
        static uint8 ButtonStatus02 = 0;
        static uint8_t LED1Status = LOW;
        static uint8_t LED2Status = LOW;
        static uint8_t LED1Flag = 0;
        static uint8_t LED2Flag = 0;
        /* Checks the LEDS status  */
        xQueuePeek(LED1Queue, &LED1Status, 0);
        xQueuePeek(LED2Queue, &LED2Status, 0);
        if(LED1Status == HIGH || LED1Flag != 0)
        {
            LED1Flag += 1;
            LED1Flag %= 20;
            Led_On(LED_0);
            vTaskDelay(25/portTICK_PERIOD_MS);
            LED1Status = LOW;
            xQueueOverwrite(LED1Queue, &LED1Status);
            Led_Off(LED_0);
        }
        if(LED2Status == HIGH || LED2Flag != 0)
        {
            LED2Flag += 1;
            LED2Flag %= 8;
            Led_On(LED_1);
            vTaskDelay(25/portTICK_PERIOD_MS);
            LED2Status = LOW;
            xQueueOverwrite(LED2Queue, &LED2Status);
            Led_Off(LED_1);
        }
        /* Updates the pb status  */
        pushButton_Update();
        ButtonStatus01 = pushButton_GetStatus(BTN_0);
        ButtonStatus02 = ButtonStatus01;
        /* Sends the pb status  */
        xQueueOverwrite(pushButton0TOLCDQueue, &ButtonStatus01);
        xQueueOverwrite(pushButton0TOUARTQueue, &ButtonStatus02);
        vTaskDelay(25/portTICK_PERIOD_MS);
    }
}


static void Keypad_handleTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint8 ) pvParameters ) == 1 );
    uint8 u8_keyPressed = 0xFF;
    for( ;; )
    {
        /* Keypad update */
        Keypad_update();
        vTaskDelay(25/portTICK_PERIOD_MS);
        Keypad_update();
        Keypad_getPressedKey(&u8_keyPressed);
        if (u8_keyPressed != 0xFF)
        {
        /* Keypad send */
            xQueueSend(keypadTOLCDQueue, &u8_keyPressed, 0);
            xQueueSend(keypadTOUARTQueue, &u8_keyPressed, 0);
        } 
        else
        {
        }

        vTaskDelay(25/portTICK_PERIOD_MS);
    }
}

static void UART_handleTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint8 ) pvParameters ) == 1 );
    uint8 UARTButtonStatus0 = 0;


    uint8 message[16];
    uint8 messageIndex = 0;
    uint8 tempMessage = 0;
    uint8 counter;
    uint8 LEDStatus;
    for( ;; )
    {
        /* Gets the keypad data */
        xQueueReceive(keypadTOUARTQueue, &tempMessage, 0);
        if (tempMessage != 0)
        {
        /* stores the data in an internal buffer*/
            message[messageIndex] = tempMessage;
            tempMessage = 0;
            messageIndex++;
        } 
        else
        {
        }
        /* Gets the pb status */
        xQueuePeek(pushButton0TOUARTQueue, &UARTButtonStatus0, 0);

        if (UARTButtonStatus0 == Prepressed)
        {
        /* when the pb is pressed start sending through UART */
            for (counter = 0; counter<=messageIndex; counter++)
            {
                if (counter == messageIndex)
                {
        /* Checks if the TX complete */
                    if (UART_sendFlag == 1)
                    {
        /* sends a null delimiter to indicate end of message */
                        UART_SendByte(0);
                        UART_sendFlag = 0;
                    }
                    else
                    {
        /* if TX not complete, stay at the same index */
                        counter--;
                    }
                } 
                else
                {
        /* Checks if the TX complete */
                    if (UART_sendFlag == 1)
                    {
        /* sends the message */
                        UART_SendByte(message[counter]);
                    }
                    else
                    {
        /* if TX not complete, stay at the same index */
                        counter--;
                    }
                }

                vTaskDelay(1/portTICK_PERIOD_MS);
            }
            messageIndex = 0;
            LEDStatus = HIGH;
            xQueueOverwrite(LED2Queue, &LEDStatus);
        } 
        else
        {
        }
        vTaskDelay(25/portTICK_PERIOD_MS);
    }
}

static void LCDUpdateTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    uint8 Button0State = 0;
    uint8 u8_keyPressed = 0xFF;
    uint8 LCDcounter = 0;
    uint8 LEDStatus;
    for( ;; )
    {
        /* LCD update display */
        xQueuePeek(pushButton0TOLCDQueue, &Button0State, 0);
        xQueueReceive(keypadTOLCDQueue, &u8_keyPressed, 0);
        if (Button0State == Prepressed)
        {
            LCD_sendString_RowCol(0, 0, (uint8*)"                ");
            LCDcounter = 0;
        }
        else
        {
            if (u8_keyPressed != 0xFF)
            {
        /* prints the pressed key */
                LCD_gotoRowColumn(0, LCDcounter);
                LCDcounter++;
                LCD_displayCharacter(u8_keyPressed);
                u8_keyPressed = 0xFF;
            } 
            else
            {
            }
        }
        if (UART_dataRecieved[UART_data_RecievedIndex - 1] == 0 && UART_data_RecievedIndex != 0)
        {
        /* prints received data */
            LCD_sendString_RowCol(1, 0, UART_dataRecieved);
            LEDStatus = HIGH;
            xQueueOverwrite(LED1Queue, &LEDStatus);
            vTaskDelay(2000/portTICK_PERIOD_MS);
            LCD_sendString_RowCol(1, 0, (uint8*)"                ");
            UART_data_RecievedIndex = 0;
        } 
        else
        {
        }
        vTaskDelay(25/portTICK_PERIOD_MS);

    }
}



static void UART_TX_CBK(void)
{
        /* sets send flag */
    UART_sendFlag = 1;
}
static void UART_RX_CBK(void)
{
        /* receives byte */
    UART_ReceiveByte(&UART_dataRecieved[UART_data_RecievedIndex++]);
}


void S3US2(void)
{
    pushButton0TOLCDQueue = xQueueCreate( 1, sizeof( uint8 ) );
    if (pushButton0TOLCDQueue == NULL)
    {Led_On(LED_3);}
    pushButton0TOUARTQueue = xQueueCreate( 1, sizeof( uint8 ) );
    if (pushButton0TOUARTQueue == NULL)
    {Led_On(LED_3);}
    LED1Queue = xQueueCreate( 1, sizeof( uint8 ) );
    if (LED1Queue == NULL)
    {Led_On(LED_3);}
    LED2Queue = xQueueCreate( 1, sizeof( uint8 ) );
    if (LED2Queue == NULL)
    {Led_On(LED_3);}
    keypadTOLCDQueue = xQueueCreate( 1, sizeof( uint8 ) );
    if (keypadTOLCDQueue == NULL)
    {Led_On(LED_3);}
    keypadTOUARTQueue = xQueueCreate( 1, sizeof( uint8 ) );
    if (keypadTOUARTQueue == NULL)
    {Led_On(LED_3);}
        
    if(xTaskCreate(LCDUpdateTask, "LUT", 100, (void *) 1, 2, NULL) == pdFALSE)
    {Led_On(LED_3);}
    if(xTaskCreate(Keypad_handleTask, "KPAD", 100, (void *) 1, 1, NULL) == pdFALSE)
    {Led_On(LED_3);}
    if(xTaskCreate(UART_handleTask, "UART", 100, (void *) 1, 3, NULL) == pdFALSE)
    {Led_On(LED_3);}
    if(xTaskCreate(InitTask, "Init", 100, (void *) 1, 4, NULL) == pdFALSE)
    {Led_On(LED_3);}
    if(xTaskCreate(pushButtonAndLedsUpdateTask, "PBU", 100, (void *) 1, 1, NULL) == pdFALSE)
    {Led_On(LED_3);}



    /* Start Scheduler */
    vTaskStartScheduler();

 }