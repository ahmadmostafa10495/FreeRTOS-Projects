
/*
 * S1US1.c
 *
 * Created: 1/27/2020 2:47:50 PM
 *  Author: ahmad
 */ 

#include "led.h"
#include "DIO.h"
#include "S1US1.h"
#include "FreeRTOS.h"
#include "task.h"

/* Task to be created. */
static void init_task( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t) pvParameters ) == 1 );

    for( ;; )
    {
        /* Led Init */
        LED_cfg_s_t led_info = {LED_0};
        Led_Init(&led_info);
        Led_On(LED_0);
        vTaskSuspend(NULL);
    }
}
/* Task to be created. */
static void toggle_task( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Led Toggle */

        Led_Toggle(LED_0);
        vTaskDelay(1000/portTICK_PERIOD_MS);

    }
}

void S1US1(void)
{

    xTaskCreate(toggle_task, "LED", 100, (void *) 1, 3, NULL);
    xTaskCreate(init_task, "INIT", 100, (void *) 1, 1, NULL);
    /* Start Scheduler */
    vTaskStartScheduler();

}