/*
 * LCD_lcfg.c
 *
 * Created: 25-Jan-20 1:01:30 AM
 *  Author: Omar Zayed
 */ 

 /*- INCLUDES -----------------------------------------------*/

  #include "DIO.h"



 /*********************************************************************
 *
 * Please configure the LCD DIO settings.
 *
 * *******************************************************************/

 DIO_Cfg_s DIO_Cfg_LCD_RS = {GPIOA, PIN1, OUTPUT};
 DIO_Cfg_s DIO_Cfg_LCD_RW = {GPIOA, PIN2, OUTPUT};
 DIO_Cfg_s DIO_Cfg_LCD_E = {GPIOA, PIN3, OUTPUT};

 DIO_Cfg_s DIO_Cfg_LCD_D4 = {GPIOA, PIN4, OUTPUT};
 DIO_Cfg_s DIO_Cfg_LCD_D5 = {GPIOA, PIN5, OUTPUT};
 DIO_Cfg_s DIO_Cfg_LCD_D6 = {GPIOA, PIN6, OUTPUT};
 DIO_Cfg_s DIO_Cfg_LCD_D7 = {GPIOA, PIN7, OUTPUT};