/******************************************************************************
 * FILE: clockConfig.h
 *
 * DESCRIPTION:
 *   This header file provides constant values and the function declaration
 *   needed to configure the internal and external clocks of the MSP430FR6989.
 *
 * PURPOSE:
 *   To set up a reliable clock system for the MSP430 so other parts of
 *   the program (like timers or delays) will work accurately.
 *
 * AUTHORS:
 *   Hanna Szalai & Haroun Riahi
 *
 ******************************************************************************/

#ifndef LCD_CLOCKCONFIG_H_
#define LCD_CLOCKCONFIG_H_

// It initializes the internal and external clocks to the desired speeds.
void setupSystemClocks(void);

// These values tell the MSP430 the speed of the crystal we're using
#define HIGH_FREQ_CRYSTAL_HZ 0
#define LOW_FREQ_CRYSTAL_HZ 32768

// Expected output clock speeds after configuration
#define MAIN_CLOCK_HZ 4000000
#define AUX_CLOCK_HZ 32768
#define SUB_MAIN_CLOCK_HZ 2000000

#endif

