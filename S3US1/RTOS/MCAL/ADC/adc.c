/*
 * adc.c
 *
 * Created: 26-Jan-20 2:32:43 AM
 *  Author: Omar Zayed
 */ 

 /*- INCLUDES ----------------------------------------------*/

 #include "adc.h"
 #include "ADC_lcfg.h"
 #include "ADC_Cfg.h"
 #include "registers.h"
 #include "common_macros.h"
 #include "interrupt.h"
 #include "retval.h"
 #include "std_types.h"


 /************************************************************************/
 /*                               Defines                                    */
 /************************************************************************/

 #define ADC_RGS_CLR                            0b00000000
 #define ADC_SFIOR_CLR                            0b00011111

 #define ADC_REFV_AVCC                            0b01000000
 #define ADC_REFV_AREF                            0b00000000
 #define ADC_REFV_2_56V                            0b11000000

 #define ADC_RIGHT_ADJ                            0b00000000
 #define ADC_LEFT_ADJ                            0b00100000

 #define ADC_POLLING                            0b00000000
 #define ADC_INTERRUPT                            0b00001000
 
 #define ADC_SINGLE_CONV                        0b00000000
 #define ADC_AUTO_TRIGGER                        0b00100000

 #define ADC_FREE_RUNNING                        0b00000000
 #define ADC_ANALOG_COMP                        0b00000001
 #define ADC_EXTI0                                0b00000010
 #define ADC_TIMER0                                0b00000011

 #define ADC_START_CONV                            0b01000000


 
 /*- GLOBAL STATIC VARIABLES -------------------------------*/
 static boolean gbl_ADCInitialized = FALSE;
 
 static uint8 ADC_InterPoll_Lookup[2] = {ADC_POLLING, ADC_INTERRUPT};
 static uint8 ADC_SingleAutoTrigger_Lookup[2] = {ADC_SINGLE_CONV, ADC_AUTO_TRIGGER};

 gptrfu_UserCbk ADC_CBK_Ptr = NULL;



 /*- APIs IMPLEMENTATION -----------------------------------*/

 /**
 * Input: Pointer to a structure contains the information needed to initialize the ADC.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module, it sets the pre-scaler, Right/Left adjustment, auto trigger, channels to work on.
 *
 */
extern ERROR_STATUS ADC_Init(str_ADC_cfg_t* str_ADC_cfg)
{

    ERROR_STATUS errorStatus = SUCCESS;

    if (gbl_ADCInitialized == TRUE)
    {
        //Error if reinitialized
        errorStatus = ADC_BASE_ERR + ALREADY_INITIALIZED_ERR;
    }
    else if(str_ADC_cfg == NULL)
    {
        errorStatus = ADC_BASE_ERR + NULL_PTR_ERR;
    }
    else
    {
        //Init the DIO
        errorStatus = DIO_init(&DIO_Cfg_ADC_Channel);

        ADMUX = ADC_RGS_CLR;
        ADCSRA = ADC_RGS_CLR;
        SFIOR &= ADC_SFIOR_CLR;
        
        ADMUX |= 
        ADC_REFV_AVCC |                //Voltage reference
        ADC_RIGHT_ADJ;        //Adjustment
        
        ADCSRA |= 
        ADC_SingleAutoTrigger_Lookup[str_ADC_cfg->u8_Single_AutoTrig] |            //Trigger mode
        ADC_InterPoll_Lookup[str_ADC_cfg->u8_Interrupt_Polling] |                //Interrupt
        str_ADC_cfg->u8_PreScaler;                                                //Prescaler

        SFIOR |= str_ADC_cfg->u8_ADC_TrigSource;    //Auto Trigger Source

        ADC_CBK_Ptr = str_ADC_cfg->PtrF_ADC_CBF;

        gbl_ADCInitialized = TRUE;

    }

    return errorStatus;

}

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function enables the ADC 
 **/
extern ERROR_STATUS ADC_Enable(void)
{

    ERROR_STATUS errorStatus = SUCCESS;

    if (gbl_ADCInitialized == FALSE)
    {
        //Error if not initialized
        errorStatus = ADC_BASE_ERR + NOT_INITIALIZED_ERR;
    }
    else
    {
        SET_BIT(ADCSRA, BIT7);
    }

    return errorStatus;

}


/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function starts the ADC conversion one time whenever called
 **/
extern ERROR_STATUS ADC_Start_Polling(uint8 u8_ADC_Ch, uint16 *ADC_Read)
{

    ERROR_STATUS errorStatus = SUCCESS;

    if (gbl_ADCInitialized == FALSE)
    {
        //Error if not initialized
        errorStatus = ADC_BASE_ERR + NOT_INITIALIZED_ERR;
    }
    else if(ADC_Read == NULL)
    { 
        //Error if null pointer
        errorStatus = ADC_BASE_ERR + NULL_PTR_ERR;
    }
    else
    {
        //Choose channel
        ADCSRA |= u8_ADC_Ch;

        ADCSRA |= ADC_START_CONV;
        
        while(GET_BIT(ADCSRA, BIT4) != BHIGH)
        {
            
        }        
        
        *ADC_Read = ADC;

        //Clear Flag
        SET_BIT(ADCSRA, BIT4);

    }

    return errorStatus;


}



/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function starts the ADC conversion one time whenever called 
 * and uses module isr
 **/
extern ERROR_STATUS ADC_Start_Interrupt(uint8 u8_ADC_Ch)
{

    ERROR_STATUS errorStatus = SUCCESS;

    if (gbl_ADCInitialized == FALSE)
    {
        //Error if not initialized
        errorStatus = ADC_BASE_ERR + NOT_INITIALIZED_ERR;
    }
    else
    {
        //Choose channel
        ADCSRA |= u8_ADC_Ch;

        ADCSRA |= ADC_START_CONV;

    }

    return errorStatus;

}

/**
 * Input: Read variable pointer
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function Reads the ADC register
 **/
extern ERROR_STATUS ADC_Read(uint16* ADC_Read)
{
    ERROR_STATUS errorStatus = SUCCESS;

    if (gbl_ADCInitialized == FALSE)
    {
        //Error if not initialized
        errorStatus = ADC_BASE_ERR + NOT_INITIALIZED_ERR;
    }
    else if(ADC_Read == NULL)
    {
        //Error if null pointer
        errorStatus = ADC_BASE_ERR + NULL_PTR_ERR;
    }
    else
    {
        *ADC_Read = ADC;
    }

    return errorStatus;

}


/**
 * Input: Auto Trigger Source for the ADC
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function sets the Auto Trigger Source for the ADC
 **/

extern ERROR_STATUS ADC_Set_AutoTrig(uint8 u8_ADC_Ch, uint8 u8_AutoTrigSource)
{

    ERROR_STATUS errorStatus = SUCCESS;

    if (gbl_ADCInitialized == FALSE)
    {
        //Error if not initialized
        errorStatus = ADC_BASE_ERR + NOT_INITIALIZED_ERR;
    }
    else
    {



    }

    return errorStatus;
}

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the auto triggered ADC conversion
 *
 */

extern ERROR_STATUS ADC_Stop_AutoTrig(void)
{

    ERROR_STATUS errorStatus = SUCCESS;

    if (gbl_ADCInitialized == FALSE)
    {
        //Error if not initialized
        errorStatus = ADC_BASE_ERR + NOT_INITIALIZED_ERR;
    }
    else
    {
        ADCSRA &= ~ADC_AUTO_TRIGGER;
    }

    return errorStatus;
}

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the call of the given function.
 *
 */
extern ERROR_STATUS ADC_Stop(void)
{

    ERROR_STATUS errorStatus = SUCCESS;

    if (gbl_ADCInitialized == FALSE)
    {
        //Error if not initialized
        errorStatus = ADC_BASE_ERR + NOT_INITIALIZED_ERR;
    }
    else
    {




    }

    return errorStatus;
}

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function Disables the ADC conversion one time whenever called
 **/
extern ERROR_STATUS ADC_Disable(void)
{

    ERROR_STATUS errorStatus = SUCCESS;

    if (gbl_ADCInitialized == FALSE)
    {
        //Error if not initialized
        errorStatus = ADC_BASE_ERR + NOT_INITIALIZED_ERR;
    }
    else
    {
        CLR_BIT(ADCSRA, BIT7);
    }

    return errorStatus;

}

ISR(ADC_vect)
{
    if(ADC_CBK_Ptr != NULL)
    {
        ADC_CBK_Ptr();
    }

}