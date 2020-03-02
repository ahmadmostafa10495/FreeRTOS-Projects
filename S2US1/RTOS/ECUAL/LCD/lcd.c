/*
 * lcd.c
 *
 * Created: 24-Jan-20 10:58:38 PM
 *  Author: Omar Zayed
 */ 

 /*- INCLUDES ----------------------------------------------*/

 #include "lcd.h"
 #include "LCD_lcfg.h"
 #include <util/delay.h>
#include "std_types.h"

 /*- LOCAL MACROS ------------------------------------------*/

 //Maximum delay needed to execute any command for system frequency of 270KHz in us.

 #define LCD_4BIT_INIT_A                    ((enu_LCDCommandType)0x33)
 #define LCD_4BIT_INIT_B                    ((enu_LCDCommandType)0x32)

 #define LCD_MAX_COLUMNS                    (16)
 #define LCD_MAX_ROWS                        (2)

 #define LCD_MAX_DELAY_AT_250FQ_US            (1640)
 #define LCD_250FQ                            (250000)

 /*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/

 static ERROR_STATUS LCD_InternalLatch(void);


 /*- GLOBAL STATIC VARIABLES -------------------------------*/

 static const uint32_t u32_LCDDelayNeeded = (LCD_MAX_DELAY_AT_250FQ_US * LCD_250FQ) / F_CPU;
 static boolean_t gbl_LCDInitialized = FALSE;



 /*- APIs IMPLEMENTATION -----------------------------------*/

 /**
 * Input: 
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: Initiates the LCD module.
 *                             
 */
 ERROR_STATUS LCD_Init(void)
 {
    ERROR_STATUS errorStatus = SUCCESS;
    
    if (gbl_LCDInitialized == TRUE)
    {
        //Error if reinitialized
        errorStatus = LCD_BASE_ERR + ALREADY_INITIALIZED_ERR;
    }
    else
    {
        
        //Inits the LCD DIO PINS
        errorStatus = DIO_init(&DIO_Cfg_LCD_RS);
        errorStatus = DIO_init(&DIO_Cfg_LCD_RW);
        errorStatus = DIO_init(&DIO_Cfg_LCD_E);
        errorStatus = DIO_init(&DIO_Cfg_LCD_D4);
        errorStatus = DIO_init(&DIO_Cfg_LCD_D5);
        errorStatus = DIO_init(&DIO_Cfg_LCD_D6);
        errorStatus = DIO_init(&DIO_Cfg_LCD_D7);


        //Delay till power-up
        _delay_ms(20);

        //Sending initialization commands
        gbl_LCDInitialized = TRUE;
        errorStatus = LCD_sendCommand(LCD_4BIT_INIT_A);
        errorStatus = LCD_sendCommand(LCD_4BIT_INIT_B);
        errorStatus = LCD_sendCommand(LCD_CMD_2LINES_5X7MATRIX);
        errorStatus = LCD_sendCommand(LCD_CMD_DISPLAY_ON_CURSOR_BLINKING);
        errorStatus = LCD_sendCommand(LCD_CMD_CLEAR_DISPLAY);
        errorStatus = LCD_sendCommand(LCD_CMD_INCREMENT_CURSOR);
        errorStatus = LCD_sendCommand(LCD_CMD_DISPLAY_ON_CURSOR_OFF);
        gbl_LCDInitialized = FALSE;

        //Delay till power-up
        _delay_ms(10);

    }

    if(errorStatus == SUCCESS)
    {
        gbl_LCDInitialized = TRUE;
    }
    else
    {
        gbl_LCDInitialized = FALSE;
    }

    return errorStatus;
 }

 /**
 * Input: The command needed to be sent to the LCD.
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: Sends a command to the LCD.
 *                             
 */
 ERROR_STATUS LCD_sendCommand(enu_LCDCommandType u8_LCDCommand)
 {
     ERROR_STATUS errorStatus = SUCCESS;

     if(gbl_LCDInitialized == FALSE)
     {
         //Check if initialized
         errorStatus = LCD_BASE_ERR + NOT_INITIALIZED_ERR;
     }
     else
     {

        errorStatus = DIO_Write(DIO_Cfg_LCD_RS.GPIO, DIO_Cfg_LCD_RS.pins, LOW);
        errorStatus = DIO_Write(DIO_Cfg_LCD_RW.GPIO, DIO_Cfg_LCD_RW.pins, LOW);

        //Write high nipple
        errorStatus = DIO_Write(DIO_Cfg_LCD_D7.GPIO, DIO_Cfg_LCD_D7.pins, u8_LCDCommand);
        errorStatus = DIO_Write(DIO_Cfg_LCD_D6.GPIO, DIO_Cfg_LCD_D6.pins, u8_LCDCommand);
        errorStatus = DIO_Write(DIO_Cfg_LCD_D5.GPIO, DIO_Cfg_LCD_D5.pins, u8_LCDCommand);
        errorStatus = DIO_Write(DIO_Cfg_LCD_D4.GPIO, DIO_Cfg_LCD_D4.pins, u8_LCDCommand);

        errorStatus = LCD_InternalLatch();

        //Shift the command
        u8_LCDCommand = u8_LCDCommand << 4;

        //Write low nipple
        errorStatus = DIO_Write(DIO_Cfg_LCD_D7.GPIO, DIO_Cfg_LCD_D7.pins, u8_LCDCommand);
        errorStatus = DIO_Write(DIO_Cfg_LCD_D6.GPIO, DIO_Cfg_LCD_D6.pins, u8_LCDCommand);
        errorStatus = DIO_Write(DIO_Cfg_LCD_D5.GPIO, DIO_Cfg_LCD_D5.pins, u8_LCDCommand);
        errorStatus = DIO_Write(DIO_Cfg_LCD_D4.GPIO, DIO_Cfg_LCD_D4.pins, u8_LCDCommand);

        errorStatus = LCD_InternalLatch();

        _delay_us(u32_LCDDelayNeeded);

    }

    return errorStatus;
 }

 /**
 * Input: The character needed to be displayed by the LCD.
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: Displays a character on the LCD.
 *                             
 */
 ERROR_STATUS LCD_displayCharacter(uint8 u8_LCDCharacter)
 {
     ERROR_STATUS errorStatus = SUCCESS;

     if(gbl_LCDInitialized == FALSE)
     {
         //Check if initialized
         errorStatus = LCD_BASE_ERR + NOT_INITIALIZED_ERR;
     }
     else
     {

        errorStatus = DIO_Write(DIO_Cfg_LCD_RS.GPIO, DIO_Cfg_LCD_RS.pins, HIGH);
        errorStatus = DIO_Write(DIO_Cfg_LCD_RW.GPIO, DIO_Cfg_LCD_RW.pins, LOW);

        //Write high nipple
        errorStatus = DIO_Write(DIO_Cfg_LCD_D7.GPIO, DIO_Cfg_LCD_D7.pins, u8_LCDCharacter);
        errorStatus = DIO_Write(DIO_Cfg_LCD_D6.GPIO, DIO_Cfg_LCD_D6.pins, u8_LCDCharacter);
        errorStatus = DIO_Write(DIO_Cfg_LCD_D5.GPIO, DIO_Cfg_LCD_D5.pins, u8_LCDCharacter);
        errorStatus = DIO_Write(DIO_Cfg_LCD_D4.GPIO, DIO_Cfg_LCD_D4.pins, u8_LCDCharacter);

        errorStatus = LCD_InternalLatch();

        //Shift the char
        u8_LCDCharacter = u8_LCDCharacter << 4;

        //Write low nipple
        errorStatus = DIO_Write(DIO_Cfg_LCD_D7.GPIO, DIO_Cfg_LCD_D7.pins, u8_LCDCharacter);
        errorStatus = DIO_Write(DIO_Cfg_LCD_D6.GPIO, DIO_Cfg_LCD_D6.pins, u8_LCDCharacter);
        errorStatus = DIO_Write(DIO_Cfg_LCD_D5.GPIO, DIO_Cfg_LCD_D5.pins, u8_LCDCharacter);
        errorStatus = DIO_Write(DIO_Cfg_LCD_D4.GPIO, DIO_Cfg_LCD_D4.pins, u8_LCDCharacter);

        errorStatus = LCD_InternalLatch();

        _delay_us(u32_LCDDelayNeeded);

     }

     return errorStatus;
 }

 /**
 * Input: A pointer to the string needed to be displayed on the LCD.
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: Displays a string on the LCD.
 *                             
 */
 ERROR_STATUS LCD_displayString(uint8* ptru8_LCDSring)
 {
    ERROR_STATUS errorStatus = SUCCESS;

     if(gbl_LCDInitialized == FALSE)
     {
         //Check if initialized
         errorStatus = LCD_BASE_ERR + NOT_INITIALIZED_ERR;
     }
     else if(ptru8_LCDSring == NULL)
    {
        //Check if initialized
        errorStatus = LCD_BASE_ERR + NULL_PTR_ERR;
    }
    else
    {
        for(uint8 u8_LCDColumnIndex = 0; ((u8_LCDColumnIndex < LCD_MAX_COLUMNS) && (ptru8_LCDSring[u8_LCDColumnIndex] != '\0')); u8_LCDColumnIndex++)
        {
            errorStatus = LCD_displayCharacter(ptru8_LCDSring[u8_LCDColumnIndex]);
        }

    }

    return errorStatus;
 }

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
 ERROR_STATUS LCD_gotoRowColumn(uint8 u8_LCDRow, uint8 u8_LCDColumn)
 {
    ERROR_STATUS errorStatus = SUCCESS;
    
    if(gbl_LCDInitialized == FALSE)
    {
        //Check if initialized
        errorStatus = LCD_BASE_ERR + NOT_INITIALIZED_ERR;
    }
    else if (u8_LCDColumn > LCD_MAX_COLUMNS)
    {
        errorStatus = LCD_BASE_ERR + INVALID_ARGU_ERR;
    }
    else
    {
        switch(u8_LCDRow)
        {
        case 0 :
            errorStatus = LCD_sendCommand(LCD_CMD_FORCE_CURSOR_1STLINE | u8_LCDColumn);
            break;
        
        case 1 :
            errorStatus = LCD_sendCommand(LCD_CMD_FORCE_CURSOR_2NDLINE | u8_LCDColumn);
            break;

        default :
            errorStatus = LCD_BASE_ERR + INVALID_ARGU_ERR;
            break;
        }
    }

    return errorStatus;

}

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
 ERROR_STATUS LCD_sendString_RowCol(uint8 u8_LCDRow, uint8 u8_LCDColumn, uint8* ptru8_LCDSring)
 {
    ERROR_STATUS errorStatus = SUCCESS;

    if(gbl_LCDInitialized == FALSE)
    {
        //Check if initialized
        errorStatus = LCD_BASE_ERR + NOT_INITIALIZED_ERR;
    }
    else if(ptru8_LCDSring == NULL)
    {
        //Check if initialized
        errorStatus = LCD_BASE_ERR + NULL_PTR_ERR;
    }
    else
    {    
        errorStatus = LCD_gotoRowColumn(u8_LCDRow, u8_LCDColumn);
        for(uint8 u8_LCDColumnIndex = 0; (((u8_LCDColumnIndex + u8_LCDColumn) < LCD_MAX_COLUMNS) && (ptru8_LCDSring[u8_LCDColumnIndex] != '\0')); u8_LCDColumnIndex++)
        {
            errorStatus = LCD_displayCharacter(ptru8_LCDSring[u8_LCDColumnIndex]);
        }

    }
    return errorStatus;
 }

 static ERROR_STATUS LCD_InternalLatch(void)
 {    
    ERROR_STATUS errorStatus = SUCCESS;

    errorStatus = DIO_Write(DIO_Cfg_LCD_E.GPIO, DIO_Cfg_LCD_E.pins, HIGH);

    //Delay needed for internal latch
    _delay_us(1);

    errorStatus = DIO_Write(DIO_Cfg_LCD_E.GPIO, DIO_Cfg_LCD_E.pins, LOW);


    return errorStatus;

 }
