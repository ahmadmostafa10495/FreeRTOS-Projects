
/*
 * Keypad.h
 *
 * Created: 1/20/2020 5:15:19 PM
 *  Author: ahmad
 */ 
#ifndef KEYPAD_H_
#define KEYPAD_H_
/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*                                                                                          *
*                                                                                          *
*******************************************************************************************/

#include "common_macros.h"
#include "std_types.h"
#include "retval.h"
#include "DIO.h"


/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*                                                                                          *
*                                                                                          *
*******************************************************************************************/
#define KEYPAD_PORT            GPIOC

#define KEYPAD_ROWS            (3U)
#define KEYPAD_COLS            (3U)

#define KEYPAD_ROW1            BIT2
#define KEYPAD_ROW2            BIT3
#define KEYPAD_ROW3            BIT4

#define KEYPAD_COL1            BIT5
#define KEYPAD_COL2            BIT6
#define KEYPAD_COL3            BIT7

#define KEYPAD_PULL_UP        HIGH
#define KEYPAD_PULL_DOWN    LOW



/******************************************************************************************
*                                                                                         *
*                               TYPEDEF                                                   *
*                                                                                          *
*                                                                                          *
*******************************************************************************************/



/******************************************************************************************
*                                                                                         *
*                               EXTERN VARIABLES                                          *
*                                                                                          *
*                                                                                          *
*******************************************************************************************/





/******************************************************************************************
*                                                                                         *
*                                 PROTOTYPES                                              *
*                                                                                          *
*                                                                                          *
*******************************************************************************************/


/**
 * Input: 
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: This function initializes the keypad module.
 *                             
 */
extern ERROR_STATUS Keypad_init();

/**
 * Input: 
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: This function updates the keypad pressed key.
 *                             
 */
extern void Keypad_update();

/**
 * Input: 
 * Output:
 *          u8_keypadValue: the value of the pressed key.
 * In/Out:            
 * Return: The error status of the function.            
 * Description: This function returns the keypad pressed key.
 *                             
 */
extern ERROR_STATUS Keypad_getPressedKey(uint8 * u8_keypadValue);
#endif