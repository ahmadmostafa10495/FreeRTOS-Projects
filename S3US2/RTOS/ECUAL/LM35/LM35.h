/*
 * LM35.h
 *
 * Created: 29-Jan-20 8:13:05 PM
 *  Author: Omar Zayed
 */ 


#ifndef LM35_H_
#define LM35_H_

#include "common_macros.h"
#include "interrupt.h"
#include "retval.h"
#include "std_types.h"


/************************************************************************/
/*                 ADC FUNCTIONS PROTOTYPES                */
/************************************************************************/


/**
 * Input: 
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module.
 *
 */
extern ERROR_STATUS LM35_Init(void);

/**
 * Input: 
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Starts running the module.
 *
 */
extern ERROR_STATUS LM35_StartConversion(void);


/**
 * Input: 
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Starts running the module.
 *
 */
extern ERROR_STATUS LM35_GetReading(uint16* u16_LM35Temperature);



#endif /* LM35_H_ */