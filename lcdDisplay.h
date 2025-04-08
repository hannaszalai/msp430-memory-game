/******************************************************************************
 *
 * FILE: lcdDisplay.h
 *
 * DESCRIPTION:
 *   Completely customized LCD control and memory manipulation file
 *   for the MSP430FR6989. Contains routines for initializing the
 *   screen, writing letters, digits, and special symbols, as well as
 *   direct memory access functions for blinking and toggling behavior.
 *
 * SOURCE IDEA:
 *   https://github.com/levietduc0712/MSP430FR6989/blob/a4ad3f1b6a1724cc6b35b700
 *   c161a61c9c6d319d/Support%20Document/Firmware/Source/BOOSTXL-BATPAKMKII_
 *   FuelGauge_MSP430FR6989/HAL_LCD.c
 *
 * CREATED BY:
 *   Hanna Szalai & Haroun Riahi
 *
 * ******************************************************************************/


#include <msp430fr6989.h>
#include "stdint.h"

#ifndef LCD_DISPLAY_H_
#define LCD_DISPLAY_H_

void LCD_init(void);
void LCD_showChar( char, int );
void LCD_displayNumber( unsigned long );
int  LCD_showSymbol( int, int, int );

void    LCD_setMemoryBits   ( uint8_t, uint8_t );
void    LCD_updateMemoryBits( uint8_t, uint8_t );
void    LCD_clearMemoryBits ( uint8_t, uint8_t );
void    LCD_toggleMemoryBits( uint8_t, uint8_t );
uint8_t LCD_getMemoryBits   ( uint8_t          );

void    LCD_setBlinkingMemoryBits   ( uint8_t, uint8_t );
void    LCD_updateBlinkingMemoryBits( uint8_t, uint8_t );
void    LCD_clearBlinkingMemoryBits ( uint8_t, uint8_t );
void    LCD_toggleBlinkingMemoryBits( uint8_t, uint8_t );
uint8_t LCD_getBlinkingMemoryBits   ( uint8_t          );


#define LCD_NUM_CHAR                6

#define LCD_HEART_IDX               3
#define LCD_HEART_SEG               LCD_C_SEGMENT_LINE_4
#define LCD_HEART_COM               BIT2

#define LCD_TIMER_IDX               3
#define LCD_TIMER_SEG               LCD_C_SEGMENT_LINE_4
#define LCD_TIMER_COM               BIT3

#define LCD_REC_IDX                 3
#define LCD_REC_COM                 BIT1

#define LCD_EXCLAMATION_IDX         3
#define LCD_EXCLAMATION_COM         BIT0

#define LCD_BRACKETS_IDX            18
#define LCD_BRACKETS_COM            BIT4

#define LCD_BATT_IDX                14
#define LCD_BATT_COM                BIT4

#define LCD_B1_IDX                  18
#define LCD_B1_COM                  BIT5

#define LCD_B2_IDX                  14
#define LCD_B2_COM                  BIT5

#define LCD_B3_IDX                  18
#define LCD_B3_COM                  BIT6

#define LCD_B4_IDX                  14
#define LCD_B4_COM                  BIT6

#define LCD_B5_IDX                  18
#define LCD_B5_COM                  BIT7

#define LCD_B6_IDX                  14
#define LCD_B6_COM                  BIT7

#define LCD_ANT_IDX                 5
#define LCD_ANT_COM                 BIT2

#define LCD_TX_IDX                  9
#define LCD_TX_COM                  BIT2

#define LCD_RX_IDX                  9
#define LCD_RX_COM                  BIT0

#define LCD_NEG_IDX                 11
#define LCD_NEG_COM                 BIT2

#define LCD_DEG_IDX                 16
#define LCD_DEG_COM                 BIT2

#define LCD_A1DP_IDX                11
#define LCD_A1DP_COM                BIT0

#define LCD_A2DP_IDX                7
#define LCD_A2DP_COM                BIT0

#define LCD_A3DP_IDX                5
#define LCD_A3DP_COM                BIT0

#define LCD_A4DP_IDX                20
#define LCD_A4DP_COM                BIT0

#define LCD_A5DP_IDX                16
#define LCD_A5DP_COM                BIT0

#define LCD_A2COL_IDX               7
#define LCD_A2COL_COM               BIT2

#define LCD_A4COL_IDX               20
#define LCD_A4COL_COM               BIT2

#define LCD_UPDATE                  0
#define LCD_CLEAR                   1
#define LCD_TOGGLE                  2
#define LCD_GET                     3

#define LCD_MEMORY_MAIN             0
#define LCD_MEMORY_BLINKING         1

#define LCD_TMR                     0
#define LCD_HRT                     1
#define LCD_REC                     2
#define LCD_EXCLAMATION             3
#define LCD_BRACKETS                4
#define LCD_BATT                    5
#define LCD_B1                      6
#define LCD_B2                      7
#define LCD_B3                      8
#define LCD_B4                      9
#define LCD_B5                      10
#define LCD_B6                      11
#define LCD_ANT                     12
#define LCD_TX                      13
#define LCD_RX                      14
#define LCD_NEG                     15
#define LCD_DEG                     16
#define LCD_A1DP                    17
#define LCD_A2DP                    18
#define LCD_A3DP                    19
#define LCD_A4DP                    20
#define LCD_A5DP                    21
#define LCD_A2COL                   22
#define LCD_A4COL                   23

extern const char digit[10][2];                                                 // Segment values for each number character
extern const char alphabetBig[26][2];                                           // Segment values for each alphabet character


#endif
