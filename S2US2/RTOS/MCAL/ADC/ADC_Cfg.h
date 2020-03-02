
/*
 * ADC_Cfg.h
 *
 * Created: 1/23/2020 7:33:10 PM
 *  Author: ahmad
 */ 
#ifndef ADC_CFG_H_
#define ADC_CFG_H_
/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*                                                                                          *
*                                                                                          *
*******************************************************************************************/

#include "common_macros.h"
#include "std_types.h"
#include "retval.h"



/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*                                                                                          *
*                                                                                          *
*******************************************************************************************/


#define ADMUX                        *((reg_type8)(0x27))

#define AREF                        (0x00U)
#define AVCC                        (0x40U)
#define INTERNAL_2_56                (0xC0U)

#define ADC_LEFT_ADJUST                (0x20U)
#define ADC_RIGHT_ADJUST            (0x00U)

#define ADC0                        (0x00U)
#define ADC1                        (0x01U)
#define ADC2                        (0x02U)
#define ADC3                        (0x03U)
#define ADC4                        (0x04U)
#define ADC5                        (0x05U)
#define ADC6                        (0x06U)
#define ADC7                        (0x07U)
#define ADC0_ADC0_10x                (0x08U)
#define ADC1_ADC0_10x                (0x09U)
#define ADC0_ADC0_200x                (0x0AU)
#define ADC1_ADC0_200x                (0x0BU)
#define ADC2_ADC2_10x                (0x0CU)
#define ADC3_ADC2_10x                (0x0DU)
#define ADC2_ADC2_200x                (0x0EU)
#define ADC3_ADC2_200x                (0x0FU)
#define ADC0_ADC1_1x                (0x10U)
#define ADC1_ADC1_1x                (0x11U)
#define ADC2_ADC1_1x                (0x12U)
#define ADC3_ADC1_1x                (0x13U)
#define ADC4_ADC1_1x                (0x14U)
#define ADC5_ADC1_1x                (0x15U)
#define ADC6_ADC1_1x                (0x16U)
#define ADC7_ADC1_1x                (0x17U)
#define ADC0_ADC2_1x                (0x18U)
#define ADC1_ADC2_1x                (0x19U)
#define ADC2_ADC2_1x                (0x1AU)
#define ADC3_ADC2_1x                (0x1BU)
#define ADC4_ADC2_1x                (0x1CU)
#define ADC5_ADC2_1x                (0x1DU)
#define ADC_1_22_V                    (0x1EU)
#define ADC_0_V                        (0x1FU)


#define ADCSRA                        *((reg_type8)(0x26))

#define ADC_ENABLE                    (0x80U)
#define ADSC                        (0x40U)
#define ADATE                        (0x20U)
#define ADIF                        (0x10U)
#define ADIE                        (0x08U)

/*
#define ADC_INTERRUPT_MODE            (0x08U)
#define ADC_POLLING_MODE            (0x00U)

#define ADC_PRESCALER_2                (0x01U)
#define ADC_PRESCALER_4                (0x02U)
#define ADC_PRESCALER_8                (0x03U)
#define ADC_PRESCALER_16            (0x04U)
#define ADC_PRESCALER_32            (0x05U)
#define ADC_PRESCALER_64            (0x06U)
#define ADC_PRESCALER_128            (0x07U)
*/


#define ADCH                        *((reg_type8)(0x25))
#define ADCL                        *((reg_type8)(0x24))
#define ADC                            *((reg_type16)0x24)


#define SFIOR                        *((reg_type8)(0x50))

#define SINGLE_CONVERSION_MODE        (0xFFU)
#define FREE_RUNNING_MODE            (0x00U)
#define ANALOG_COMPARATOR_MODE        (0x20U)
#define EXTERNAL_INTRRUPT_0_MODE    (0x40U)
#define TIMER0_COMP_MATCH_MODE        (0x60U)
#define TIMER0_OVF_MODE                (0x80U)
#define TIMER1_COMP_MATCH_B_MODE    (0xA0U)
#define TIMER1_OVF_MODE                (0xC0U)
#define TIMER1_CAPTURE_MODE            (0xE0U)



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
#endif
