/*
 * LM35.c
 *
 * Created: 29-Jan-20 8:12:23 PM
 *  Author: Omar Zayed
 */ 

 #include "LM35.h"
 #include "adc.h"

static void ADCInterrupt_CBK(void);
static uint16 LM35_ConvertReading(uint16 Reading);



 static uint16 gu16_LM35Reading = 0;


 /**
 * Input: 
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module.
 *
 */
extern ERROR_STATUS LM35_Init(void)
{
    ERROR_STATUS errorStatus = SUCCESS;

    str_ADC_cfg_t str_ADC_cfg = {ADC_INTERRUPT_MODE, ADC_SINGLE, ADC_PRESCALER_64, FREERUN_MODE, ADCInterrupt_CBK};
    errorStatus = ADC_Init(&str_ADC_cfg);
    errorStatus = ADC_Enable();

    return errorStatus;
}

/**
 * Input: 
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Starts running the module.
 *
 */
extern ERROR_STATUS LM35_StartConversion(void)
{
    ERROR_STATUS errorStatus = SUCCESS;

    errorStatus = ADC_Start_Interrupt(ADC0_SINGLE);

    return errorStatus;
}


/**
 * Input: 
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Starts running the module.
 *
 */
extern ERROR_STATUS LM35_GetReading(uint16* u16_LM35Temperature)
{
    ERROR_STATUS errorStatus = SUCCESS;
    *u16_LM35Temperature = gu16_LM35Reading;
    return errorStatus;
}


static uint16 LM35_ConvertReading(uint16 Reading)
{
    return (Reading - 2) / 2;
}

static void ADCInterrupt_CBK(void)
{    
    static uint16 LM35_ADCReading = 0;
    ADC_Read(&LM35_ADCReading);
    gu16_LM35Reading = LM35_ConvertReading(LM35_ADCReading);
}
