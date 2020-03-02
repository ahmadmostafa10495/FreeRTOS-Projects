/*
 * ADC_lcfg.c
 *
 * Created: 26-Jan-20 3:23:43 AM
 *  Author: Omar Zayed
 */ 


 /* INCLUDES **********************************************************/

 #include "DIO.h"

 /*********************************************************************
 *
 * Please configure the ADC channel on port A.
 *
 * *******************************************************************/

 /* ADC Channel */
 DIO_Cfg_s DIO_Cfg_ADC_Channel = {GPIOA, PIN0, INPUT};


 