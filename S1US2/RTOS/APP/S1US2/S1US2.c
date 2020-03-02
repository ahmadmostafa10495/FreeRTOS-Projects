
/*
 * S1US1.c
 *
 * Created: 1/27/2020 2:47:50 PM
 *  Author: ahmad
 */ 

#include "Keypad.h"
#include "S1US2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "DIO.h"
#include "lcd.h"
/* Task to be created. */
void Keypad_initTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        
        /* DIO Init */
        DIO_Cfg_s d = {GPIOB, FULL_PORT, OUTPUT};
        DIO_init(&d);
        DIO_Write(GPIOB, FULL_PORT,LOW);
        
        /* Keypad Init */
        Keypad_init();

        /* LCD Init */
        LCD_Init();
        vTaskSuspend(NULL);

    }
}
/* Task to be created. */
void Keypad_handleTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( QueueHandle_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Keypad update value */
        uint8 u8_keyPressed = 0xFF;
        Keypad_update();
        vTaskDelay(30/portTICK_PERIOD_MS);
        Keypad_update();
        Keypad_getPressedKey(&u8_keyPressed);
        if (u8_keyPressed != 0xFF)
        {
        /* Keypad data send */
            xQueueSend(pvParameters, &u8_keyPressed, 0/portTICK_PERIOD_MS);
        } 
        else
        {
        }

        vTaskDelay(30/portTICK_PERIOD_MS);
    }
}
/* Task to be created. */
void LCD_handleTask( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( QueueHandle_t ) pvParameters ) == 1 );
    uint8 u8_password[7];
    uint8 u8_realPassword[] = "123456";
    uint8 u8_passIndex = 0;
    uint8 u8_keyPressed = 0xFF;
    uint8 u8_i;
    for( ;; )
    {
        /* Keypad data receive */       
        xQueueReceive(pvParameters, &u8_keyPressed, portMAX_DELAY );
        
        /* LCD update display */
        if (u8_keyPressed == '0')
        {
            DIO_Toggle(GPIOB, PIN0);
            LCD_displayCharacter(u8_keyPressed);
            u8_password[u8_passIndex] = u8_keyPressed;
            u8_passIndex++;
            u8_keyPressed = 0xFF;
        }
        else if (u8_keyPressed == '1')
        {
            DIO_Toggle(GPIOB, PIN1);
            LCD_displayCharacter(u8_keyPressed);
            u8_password[u8_passIndex] = u8_keyPressed;
            u8_passIndex++;
            u8_keyPressed = 0xFF;
        }
        else if (u8_keyPressed == '2')
        {
            DIO_Toggle(GPIOB, PIN2);
            LCD_displayCharacter(u8_keyPressed);
            u8_password[u8_passIndex] = u8_keyPressed;
            u8_passIndex++;
            u8_keyPressed = 0xFF;
        }
        else if (u8_keyPressed == '3')
        {
            DIO_Toggle(GPIOB, PIN3);
            LCD_displayCharacter(u8_keyPressed);
            u8_password[u8_passIndex] = u8_keyPressed;
            u8_passIndex++;
            u8_keyPressed = 0xFF;
        }
        else if (u8_keyPressed == '4')
        {
            DIO_Toggle(GPIOB, PIN4);
            LCD_displayCharacter(u8_keyPressed);
            u8_password[u8_passIndex] = u8_keyPressed;
            u8_passIndex++;
            u8_keyPressed = 0xFF;
        }
        else if (u8_keyPressed == '5')
        {
            DIO_Toggle(GPIOB, PIN5);
            LCD_displayCharacter(u8_keyPressed);
            u8_password[u8_passIndex] = u8_keyPressed;
            u8_passIndex++;
            u8_keyPressed = 0xFF;
        }
        else if (u8_keyPressed == '6')
        {
            DIO_Toggle(GPIOB, PIN6);
            LCD_displayCharacter(u8_keyPressed);
            u8_password[u8_passIndex] = u8_keyPressed;
            u8_passIndex++;
            u8_keyPressed = 0xFF;
        }
        else if (u8_keyPressed == 'c')
        {
            DIO_Toggle(GPIOB, PIN7);
            LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
            u8_passIndex = 0;
            u8_keyPressed = 0xFF;
        }
        else if (u8_keyPressed == 'e')
        {
            DIO_Toggle(GPIOB, PIN7);
            LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
            
            for (u8_i = 0; u8_i<6; u8_i++)
            {
                if (u8_password[u8_i] != u8_realPassword[u8_i])
                {
                    break;
                } 
                else
                {
                }
            }
            if (u8_i == 6)
            {
                LCD_displayString((uint8*)"CORRECT PASSWORD");
                vTaskDelay(2000/portTICK_PERIOD_MS);
            } 
            else
            {
                LCD_displayString((uint8*)"WRONG PASSWORD");
                vTaskDelay(2000/portTICK_PERIOD_MS);    
            }
            LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
            u8_passIndex = 0;
            u8_keyPressed = 0xFF;
        }        

    }
}


void S1US2(void)
{
    QueueHandle_t Queue_keypadToLCDValue;
    Queue_keypadToLCDValue = xQueueCreate( 10, sizeof( uint8 ) );

    xTaskCreate(Keypad_initTask, "init", 100, (void *) 1, 3, NULL);
    xTaskCreate(Keypad_handleTask, "kpad", 100, (void *) Queue_keypadToLCDValue, 1, NULL);
    xTaskCreate(LCD_handleTask, "lcd", 100, (void *) Queue_keypadToLCDValue, 1, NULL);
    /* Start Scheduler */    
    vTaskStartScheduler();
}