/*
 * lcd.h
 *
 * Created: 24-Jan-20 10:58:47 PM
 *  Author: Omar Zayed
 */ 


#ifndef LCD_H_
#define LCD_H_

/*- INCLUDES -----------------------------------------------*/

/*#include "commonLayer.h"*/
#include "common_macros.h"
#include "interrupt.h"
#include "registers.h"
#include "retval.h"

/*- PRIMITIVE TYPES ----------------------------------------*/

typedef uint8 enu_LCDCommandType;

/*- ENUMS --------------------------------------------------*/

 #define LCD_CMD_CLEAR_DISPLAY                            ((enu_LCDCommandType)0x01)
 #define LCD_CMD_RETURN_HOME                            ((enu_LCDCommandType)0x02)
 #define LCD_CMD_DECREMENT_CURSOR                        ((enu_LCDCommandType)0x04)
 #define LCD_CMD_INCREMENT_CURSOR                        ((enu_LCDCommandType)0x06)
 #define LCD_CMD_SHIFT_DISPLAY_RIGHT                    ((enu_LCDCommandType)0x05)
 #define LCD_CMD_SHIFT_DISPLAY_LEFT                        ((enu_LCDCommandType)0x07)
 #define LCD_CMD_DISPLAY_OFF_CURSOR_OFF                    ((enu_LCDCommandType)0x08)
 #define LCD_CMD_DISPLAY_OFF_CURSOR_ON                    ((enu_LCDCommandType)0x0A)
 #define LCD_CMD_DISPLAY_ON_CURSOR_OFF                    ((enu_LCDCommandType)0x0C)
 #define LCD_CMD_DISPLAY_ON_CURSOR_BLINKING                ((enu_LCDCommandType)0x0E)
 #define LCD_CMD_SHIFT_CURSOR_POSITION_LEFT                ((enu_LCDCommandType)0x10)
 #define LCD_CMD_SHIFT_CURSOR_POSITION_RIGHT            ((enu_LCDCommandType)0x14)
 #define LCD_CMD_SHIFT_ALL_DISPLAY_LEFT                    ((enu_LCDCommandType)0x18)
 #define LCD_CMD_SHIFT_ALL_DISPLAY_RIGHT                ((enu_LCDCommandType)0x1C)
 #define LCD_CMD_FORCE_CURSOR_1STLINE                    ((enu_LCDCommandType)0x80)
 #define LCD_CMD_FORCE_CURSOR_2NDLINE                    ((enu_LCDCommandType)0xC0)
 #define LCD_CMD_2LINES_5X7MATRIX                        ((enu_LCDCommandType)0x28)



/*- FUNCTION DECLARATIONS ----------------------------------*/



/**
 * Input: 
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: Initiates the LCD module.
 *                             
 */
 ERROR_STATUS LCD_Init(void);

 /**
 * Input: The command needed to be sent to the LCD.
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: Sends a command to the LCD.
 *                             
 */
 ERROR_STATUS LCD_sendCommand(enu_LCDCommandType u8_LCDCommand);

 /**
 * Input: The character needed to be displayed by the LCD.
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: Displays a character on the LCD.
 *                             
 */
 ERROR_STATUS LCD_displayCharacter(uint8 u8_LCDCharacter);

 /**
 * Input: A pointer to the string needed to be displayed on the LCD.
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: Displays a string on the LCD.
 *                             
 */
 ERROR_STATUS LCD_displayString(uint8* ptru8_LCDSring);

 /**
 * Input: 
 *            u8_LCDRow: The row of the LCD.
 *            u8_LCDColumn: The column of the LCD.    
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: Lets the LCD write begins at a certain position.
 *                             
 */
 ERROR_STATUS LCD_gotoRowColumn(uint8 u8_LCDRow, uint8 u8_LCDColumn);

 /**
 * Input: 
 *            u8_LCDRow: The row of the LCD.
 *            u8_LCDColumn: The column of the LCD.
 *            ptru8_LCDSring: A pointer to the string needed to be displayed on the LCD.
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: Writes a string beginning from a certain position.
 *                             
 */
 ERROR_STATUS LCD_sendString_RowCol(uint8 u8_LCDRow, uint8 u8_LCDColumn, uint8* ptru8_LCDSring);



#endif /* LCD_H_ */