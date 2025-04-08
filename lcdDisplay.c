/******************************************************************************
 *
 * FILE: lcdDisplay.c
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

#include "driverlib.h"
#include <lcdDisplay.h>
#include "string.h"

// Alphabet segment mappings for A-Z from the previous lab
const char alphabetBig[26][2] =
{
    {0xEF, 0x00},  /* "A" LCD segments a+b+c+e+f+g+m */
    {0xF1, 0x50},  /* "B" */
    {0x9C, 0x00},  /* "C" */
    {0xF0, 0x50},  /* "D" */
    {0x9F, 0x00},  /* "E" */
    {0x8F, 0x00},  /* "F" */
    {0xBD, 0x00},  /* "G" */
    {0x6F, 0x00},  /* "H" */
    {0x90, 0x50},  /* "I" */
    {0x78, 0x00},  /* "J" */
    {0x0E, 0x22},  /* "K" */
    {0x1C, 0x00},  /* "L" */
    {0x6C, 0xA0},  /* "M" */
    {0x6C, 0x82},  /* "N" */
    {0xFC, 0x00},  /* "O" */
    {0xCF, 0x00},  /* "P" */
    {0xFC, 0x02},  /* "Q" */
    {0xCF, 0x02},  /* "R" */
    {0xB7, 0x00},  /* "S" */
    {0x80, 0x50},  /* "T" */
    {0x7C, 0x00},  /* "U" */
    {0x0C, 0x28},  /* "V" */
    {0x6C, 0x0A},  /* "W" */
    {0x00, 0xAA},  /* "X" */
    {0x00, 0xB0},  /* "Y" */
    {0x90, 0x28}   /* "Z" */
};

// Number patterns for 0-9
const char digit[10][2] =
{
    {0xFC, 0x28},  /* "0" LCD segments a+b+c+d+e+f+k+q */
    {0x60, 0x20},  /* "1" */
    {0xDB, 0x00},  /* "2" */
    {0xF3, 0x00},  /* "3" */
    {0x67, 0x00},  /* "4" */
    {0xB7, 0x00},  /* "5" */
    {0xBF, 0x00},  /* "6" */
    {0xE4, 0x00},  /* "7" */
    {0xFF, 0x00},  /* "8" */
    {0xF7, 0x00}   /* "9" */
};

// Segment definitions for display character positions
const uint8_t CharMap[ LCD_NUM_CHAR ][ 4 ] =
{
        { 18, 19, 20, 21 },
        { 10, 11, 12, 13 },
        {  6,  7,  8,  9 },
        { 36, 37, 38, 39 },
        { 28, 29, 30, 31 },
        { 14, 15, 16, 17 }
};


LCD_C_initParam initParams = {
    LCD_C_CLOCKSOURCE_ACLK,
    LCD_C_CLOCKDIVIDER_1,
    LCD_C_CLOCKPRESCALAR_16,
    LCD_C_4_MUX,
    LCD_C_LOW_POWER_WAVEFORMS,
    LCD_C_SEGMENTS_ENABLED
};

// Initializes all LCD settings
void LCD_init()
{
    LCD_C_off( LCD_C_BASE );
    LCD_C_setPinAsLCDFunctionEx( LCD_C_BASE,
            LCD_C_SEGMENT_LINE_0,
            LCD_C_SEGMENT_LINE_21
    );
    LCD_C_setPinAsLCDFunctionEx( LCD_C_BASE,
            LCD_C_SEGMENT_LINE_26,
            LCD_C_SEGMENT_LINE_43
    );
    LCD_C_init( LCD_C_BASE, &initParams );
    LCD_C_setVLCDSource( LCD_C_BASE, LCD_C_VLCD_GENERATED_INTERNALLY,
                         LCD_C_V2V3V4_GENERATED_INTERNALLY_NOT_SWITCHED_TO_PINS,
                         LCD_C_V5_VSS );
    LCD_C_setVLCDVoltage( LCD_C_BASE,
                          LCD_C_CHARGEPUMP_VOLTAGE_3_02V_OR_2_52VREF );
    LCD_C_enableChargePump( LCD_C_BASE );
    LCD_C_selectChargePumpReference( LCD_C_BASE,
                                     LCD_C_INTERNAL_REFERENCE_VOLTAGE );
    LCD_C_configChargePump( LCD_C_BASE, LCD_C_SYNCHRONIZATION_ENABLED, 0 );
    LCD_C_clearMemory( LCD_C_BASE );
    LCD_C_clearBlinkingMemory( LCD_C_BASE );
    LCD_C_selectDisplayMemory( LCD_C_BASE, LCD_C_DISPLAYSOURCE_MEMORY );
    LCD_C_setBlinkingControl( LCD_C_BASE, LCDBLKPRE1, LCDBLKPRE2, LCD_C_BLINK_MODE_DISABLED );
    LCD_C_on( LCD_C_BASE );
}

void LCD_showChar(char c, int Position)
{
    Position -= 1;
    if ( ( Position >= 0 ) && ( Position <= 6 ) )
    {
        if ( c == ' ' )
        {
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][0], 0 );
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][1], 0 );
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][2], 0 );
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][3], 0 );
        }
        else if ( c >= '0' && c <= '9' )
        {
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][0], digit[c-48][0] );
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][1], digit[c-48][0] >> 4 );
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][2], digit[c-48][1] );
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][3], digit[c-48][1] >> 4 );
        }
        else if ( c >= 'A' && c <= 'Z' )
        {
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][0], alphabetBig[c-65][0] );
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][1], alphabetBig[c-65][0] >> 4 );
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][2], alphabetBig[c-65][1] );
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][3], alphabetBig[c-65][1] >> 4 );
        }
        else
        {
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][0], 0xFF );
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][1], 0xFF );
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][2], 0xFF );
            LCD_C_setMemory( LCD_C_BASE, CharMap[Position][3], 0xFF );
        }
    }
}

void LCD_displayNumber( unsigned long val )
{
    unsigned long div[6];
    unsigned long mod[6];
    unsigned long err;

    err = val / 1000000;
    if ( err > 0 )
    {
        LCD_showChar( ' ', 1 );
        LCD_showChar( 'E', 2 );
        LCD_showChar( 'R', 3 );
        LCD_showChar( 'R', 4 );
        LCD_showChar( 'O', 5 );
        LCD_showChar( 'R', 6 );
    }
    else
    {
        div[0] = val / 100000  ;
        mod[0] = val % 100000  ;
        div[1] = mod[0] / 10000;
        mod[1] = mod[0] % 10000;
        div[2] = mod[1] / 1000 ;
        mod[2] = mod[1] % 1000 ;
        div[3] = mod[2] / 100  ;
        mod[3] = mod[2] % 100  ;
        div[4] = mod[3] / 10   ;
        div[5] = mod[3] % 10   ;

        int i = 0;
        int LeadingZeros = 0;

        for ( i = 0; i < ( LCD_NUM_CHAR - 1); i++ )
        {
            if ( ( div[i] == 0 ) && ( !LeadingZeros ) )
            {
                LCD_showChar( ' ', i + 1 );
            }
            else
            {
                LCD_showChar( div[i] + '0', i + 1 );
                LeadingZeros++;
            }
        }

        i = LCD_NUM_CHAR - 1;
        LCD_showChar( div[i] + '0', i + 1 );
    }
}

int LCD_showSymbol( int Operation, int Symbol, int Memory )
{
    int idx = 0;
    int bit = 0;
    int mem = 0;
    int ret = -1;

    if (( initParams.muxRate <= LCD_C_4_MUX ) && ( Memory == 1 ))
        mem = 35-3;                                                             // Writing to LCDBM35 also sets LCDM3 (and so on with the other values)

    switch ( Symbol )
    {
    case LCD_TMR:
        idx = LCD_TIMER_IDX;
        bit = LCD_TIMER_COM;
        break;
    case LCD_HRT:
        idx = LCD_HEART_IDX;
        bit = LCD_HEART_COM;
        break;
    case LCD_REC:
        idx = LCD_REC_IDX;
        bit = LCD_REC_COM;
        break;
    case LCD_EXCLAMATION:
        idx = LCD_EXCLAMATION_IDX;
        bit = LCD_EXCLAMATION_COM;
        break;
    case LCD_BATT:
        idx = LCD_BATT_IDX;
        bit = LCD_BATT_COM;
        break;
    case LCD_BRACKETS:
        idx = LCD_BRACKETS_IDX;
        bit = LCD_BRACKETS_COM;
        break;
    case LCD_B6:
        idx = LCD_B6_IDX;
        bit = LCD_B6_COM;
        break;
    case LCD_B5:
        idx = LCD_B5_IDX;
        bit = LCD_B5_COM;
        break;
    case LCD_B4:
        idx = LCD_B4_IDX;
        bit = LCD_B4_COM;
        break;
    case LCD_B3:
        idx = LCD_B3_IDX;
        bit = LCD_B3_COM;
        break;
    case LCD_B2:
        idx = LCD_B2_IDX;
        bit = LCD_B2_COM;
        break;
    case LCD_B1:
        idx = LCD_B1_IDX;
        bit = LCD_B1_COM;
        break;
    case LCD_ANT:
        idx = LCD_ANT_IDX;
        bit = LCD_ANT_COM;
        break;
    case LCD_TX:
        idx = LCD_TX_IDX;
        bit = LCD_TX_COM;
        break;
    case LCD_RX:
        idx = LCD_RX_IDX;
        bit = LCD_RX_COM;
        break;
    case LCD_NEG:
        idx = LCD_NEG_IDX;
        bit = LCD_NEG_COM;
        break;
    case LCD_DEG:
        idx = LCD_DEG_IDX;
        bit = LCD_DEG_COM;
        break;
    case LCD_A1DP:
        idx = LCD_A1DP_IDX;
        bit = LCD_A1DP_COM;
        break;
    case LCD_A2DP:
        idx = LCD_A2DP_IDX;
        bit = LCD_A2DP_COM;
        break;
    case LCD_A3DP:
        idx = LCD_A3DP_IDX;
        bit = LCD_A3DP_COM;
        break;
    case LCD_A4DP:
        idx = LCD_A4DP_IDX;
        bit = LCD_A4DP_COM;
        break;
    case LCD_A5DP:
        idx = LCD_A5DP_IDX;
        bit = LCD_A5DP_COM;
        break;
    case LCD_A2COL:
        idx = LCD_A2COL_IDX;
        bit = LCD_A2COL_COM;
        break;
    case LCD_A4COL:
        idx = LCD_A4COL_IDX;
        bit = LCD_A4COL_COM;
        break;
    default:
        break;
    }

    switch ( Operation )
    {
    case LCD_UPDATE:
        LCDMEM[ idx - 1 + mem ] |= bit;
        break;
    case LCD_CLEAR:
        LCDMEM[ idx - 1 + mem ] &= ~bit;
        break;
    case LCD_TOGGLE:
        LCDMEM[ idx - 1 + mem ] ^= bit;
        break;
    }

    if ( ( LCDMEM[ idx - 1 + mem ] & bit ) >> 0 )
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }

    return ( ret );
}


void LCD_setMemoryBits( uint8_t LcdMemIndex , uint8_t SegmentMask )
{
    LCDMEM[ LcdMemIndex - 1 ] = SegmentMask;
}

void LCD_setBlinkingMemoryBits( uint8_t LcdMemIndex , uint8_t SegmentMask )
{
    LCDBMEM[ LcdMemIndex - 1 ] = SegmentMask;
}

void LCD_updateMemoryBits( uint8_t LcdMemIndex , uint8_t SegmentMask )
{
    LCDMEM[ LcdMemIndex - 1 ] |= SegmentMask;
}

void LCD_updateBlinkingMemoryBits( uint8_t LcdMemIndex , uint8_t SegmentMask )
{
    LCDBMEM[ LcdMemIndex - 1 ] |= SegmentMask;
}

void LCD_clearMemoryBits( uint8_t LcdMemIndex , uint8_t SegmentMask )
{
    LCDMEM[ LcdMemIndex - 1 ] &= ~SegmentMask;                                  // 'Clear' AND's the inverse of the provided 8-bit mask
}

void LCD_clearBlinkingMemoryBits( uint8_t LcdMemIndex , uint8_t SegmentMask ) // Clear specified SegmentMask bits of LCD Memory
{
    LCDBMEM[ LcdMemIndex - 1 ] &= ~SegmentMask;                                 // 'Clear' AND's the inverse of the provided 8-bit mask
}

void LCD_toggleMemoryBits( uint8_t LcdMemIndex , uint8_t SegmentMask )        // Toggle the bits specified by the 8-bit SegmentMask
{
    LCDMEM[ LcdMemIndex - 1 ] ^= SegmentMask;                                   // 'Toggle' exclusive-OR's the mask bits within the specified LCD memory location
}

void LCD_toggleBlinkingMemoryBits( uint8_t LcdMemIndex , uint8_t SegmentMask ) // Toggle the bits specified by the 8-bit SegmentMask
{
    LCDBMEM[ LcdMemIndex - 1 ] ^= SegmentMask;                                  // 'Toggle' exclusive-OR's the mask bits within the specified LCD memory location
}

uint8_t myLCD_getMemoryBits( uint8_t LcdMemIndex )                              // Get the byte-wide LCD Memory location
{
    return( LCDMEM[ LcdMemIndex - 1 ] );                                        // 'Get' returns the current value of the specified LCD memory location
}

uint8_t LCD_getBlinkingMemoryBits( uint8_t LcdMemIndex )                      // Get the byte-wide LCD Memory location
{
    return( LCDBMEM[ LcdMemIndex - 1 ] );                                       // 'Get' returns the current value of the specified LCD memory location
}
