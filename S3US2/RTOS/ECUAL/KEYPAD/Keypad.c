
/*
 * Keypad.c
 *
 * Created: 1/20/2020 5:15:03 PM
 *  Author: ahmad
 */ 

/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*                                                                                          *
*                                                                                          *
*******************************************************************************************/
#include "Keypad.h"
#include "Keypad_Cfg.h"
/*#include "SOS.h"*/



/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*                                                                                          *
*                                                                                          *
*******************************************************************************************/

#define SETROW1                    DIO_Write(KEYPAD_PORT, KEYPAD_ROW1, !KEYPAD_PULL_MODE);DIO_Write(KEYPAD_PORT, KEYPAD_ROW2|KEYPAD_ROW3, KEYPAD_PULL_MODE);
#define SETROW2                    DIO_Write(KEYPAD_PORT, KEYPAD_ROW2, !KEYPAD_PULL_MODE);DIO_Write(KEYPAD_PORT, KEYPAD_ROW1|KEYPAD_ROW3, KEYPAD_PULL_MODE);
#define SETROW3                    DIO_Write(KEYPAD_PORT, KEYPAD_ROW3, !KEYPAD_PULL_MODE);DIO_Write(KEYPAD_PORT, KEYPAD_ROW1|KEYPAD_ROW2, KEYPAD_PULL_MODE);

#define CHECKCOL1(DATA_PTR)        DIO_Read(KEYPAD_PORT, KEYPAD_COL1, DATA_PTR);
#define CHECKCOL2(DATA_PTR)        DIO_Read(KEYPAD_PORT, KEYPAD_COL2, DATA_PTR);
#define CHECKCOL3(DATA_PTR)        DIO_Read(KEYPAD_PORT, KEYPAD_COL3, DATA_PTR);

#define KEYPAD_INIT                (0U)
#define KEYPAD_UNINIT            (1U)

#define KEYPAD_FIRST_READ        (0U)
#define KEYPAD_SECOND_READ        (1U)
/******************************************************************************************
*                                                                                         *
*                               TYPEDEF                                                   *
*                                                                                          *
*                                                                                          *
*******************************************************************************************/



/******************************************************************************************
*                                                                                         *
*                               STATIC VARIABLES                                          *
*                                                                                          *
*                                                                                          *
*******************************************************************************************/
static uint8 gu8_keypadStatus = KEYPAD_UNINIT;
static uint8 gu8_keypadKeyPressed;
static uint8 gu8_firstRead;
static uint8 gu8_secondRead;
/******************************************************************************************
*                                                                                         *
*                                 IMPLEMENTATION                                          *
*                                                                                          *
*                                                                                          *
*******************************************************************************************/

static void keypad_click(uint8* u8_keyPressed);
/**
 * Input: 
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: This function initializes the keypad module.
 *                             
 */
ERROR_STATUS 
Keypad_init()
{
    uint8 u8_retval = KEYPAD_BASE_ERR + SUCCESS;
    if (gu8_keypadStatus == KEYPAD_UNINIT)
    {
        DIO_Cfg_s str_rowInfo = {KEYPAD_PORT, KEYPAD_ROW1|KEYPAD_ROW2|KEYPAD_ROW3, OUTPUT};
        DIO_Cfg_s str_colInfo = {KEYPAD_PORT, KEYPAD_COL1|KEYPAD_COL2|KEYPAD_COL3, INPUT};
        DIO_init(&str_rowInfo);
        DIO_init(&str_colInfo);
        DIO_Write(KEYPAD_PORT, KEYPAD_COL1|KEYPAD_COL2|KEYPAD_COL3, KEYPAD_PULL_MODE);
        gu8_keypadKeyPressed = 0xFF;
        gu8_firstRead = 0xFF;
        gu8_secondRead = 0xFF;
        gu8_keypadStatus = KEYPAD_INIT;
        
    }
    else
    {
        u8_retval = KEYPAD_BASE_ERR + ALREADY_INITIALIZED_ERR;
    }
    return u8_retval;
}

/**
 * Input: 
 * Output:
 * In/Out:            
 * Return: The error status of the function.            
 * Description: This function updates the keypad pressed key.
 *                             
 */
void 
Keypad_update()
{
    static uint8 u8_readCount = KEYPAD_FIRST_READ;
    if (u8_readCount == KEYPAD_FIRST_READ)
    {
        keypad_click(&gu8_firstRead);
        u8_readCount = KEYPAD_SECOND_READ;
    } 
    else if (u8_readCount == KEYPAD_SECOND_READ)
    {
        keypad_click(&gu8_secondRead);
        if (gu8_firstRead == gu8_secondRead)
        {
            gu8_keypadKeyPressed = gu8_firstRead;
        }
        else
        {
        }
        gu8_firstRead = 0xFF;
        gu8_secondRead = 0xFF;
        u8_readCount = KEYPAD_FIRST_READ;
    } 
    else
    {
    }
}

/**
 * Input: 
 * Output:
 *          u8_keypadValue: the value of the pressed key.
 * In/Out:            
 * Return: The error status of the function.            
 * Description: This function returns the keypad pressed key.
 *                             
 */
ERROR_STATUS 
Keypad_getPressedKey(uint8 * u8_keypadValue)
{
    uint8 u8_retval = KEYPAD_BASE_ERR + SUCCESS;
    if (gu8_keypadStatus == KEYPAD_INIT)
    {
        *u8_keypadValue = gu8_keypadKeyPressed;
        gu8_keypadKeyPressed = 0xFF;
            
    }
    else
    {
        u8_retval = KEYPAD_BASE_ERR + NOT_INITIALIZED_ERR;
    }
    return u8_retval;    
}

static void keypad_click(uint8* u8_keyPressed)
{
    uint8 u8_key = 0xFF;
    /* checking for row 1*/
    SETROW1
    CHECKCOL1(&u8_key);
    if (u8_key == !KEYPAD_PULL_MODE)
    {
        *u8_keyPressed = 'h';
    } 
    else
    {
        
    }
    CHECKCOL2(&u8_key);
    if (u8_key == !KEYPAD_PULL_MODE)
    {
        *u8_keyPressed = 'i';
    }
    else
    {
        
    }
    CHECKCOL3(&u8_key);
    if (u8_key == !KEYPAD_PULL_MODE)
    {
        *u8_keyPressed = ' ';
    }
    else
    {
        
    }
    /* checking for row 2*/
    SETROW2
    CHECKCOL1(&u8_key);
    if (u8_key == !KEYPAD_PULL_MODE)
    {
        *u8_keyPressed = 'a';
    }
    else
    {
        
    }
    CHECKCOL2(&u8_key);
    if (u8_key == !KEYPAD_PULL_MODE)
    {
        *u8_keyPressed = 'v';
    }
    else
    {
        
    }
    CHECKCOL3(&u8_key);
    if (u8_key == !KEYPAD_PULL_MODE)
    {
        *u8_keyPressed = 'r';
    }
    else
    {
        
    }
    /* checking for row 3*/
    SETROW3
    CHECKCOL1(&u8_key);
    if (u8_key == !KEYPAD_PULL_MODE)
    {
        *u8_keyPressed = '1';
    }
    else
    {
        
    }
    CHECKCOL2(&u8_key);
    if (u8_key == !KEYPAD_PULL_MODE)
    {
        *u8_keyPressed = '2';
    }
    else
    {
        
    }
    CHECKCOL3(&u8_key);
    if (u8_key == !KEYPAD_PULL_MODE)
    {
        *u8_keyPressed = '!';
    }
    else
    {
        
    }
}