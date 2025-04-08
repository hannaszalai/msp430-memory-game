/******************************************************************************
  FILE: clockConfig.c

  DESCRIPTION:
    This file sets up the system clocks for the MSP430FR6989 microcontroller.
    It configures ACLK (Auxiliary Clock), SMCLK (Sub-Main Clock), and MCLK (Main Clock)
    using the internal DCO and an external crystal oscillator (LFXT).

    The function `initializeClocks()` ensures stable clock configuration using recommended
    settings and timeout safeguards.

  FUNCTIONALITY:
    - Sets external crystal frequencies
    - Waits for LFXT (low-frequency crystal) to stabilize
    - Configures the digitally controlled oscillator (DCO)
    - Assigns clock sources to ACLK, SMCLK, and MCLK

  AUTHORS:
    Hanna Szalai & Haroun Riahi

******************************************************************************/

#include <driverlib.h>
#include <clockConfig.h>
#include <stdbool.h>

// This sets how long we will wait for the crystal oscillator to stabilize
#define CLOCK_TIMEOUT   100000

// These variables will store the frequencies of the different system clocks
uint32_t freqACLK    = 0;
uint32_t freqSMCLK  = 0;
uint32_t freqMCLK    = 0;

// This is used to store whether the external crystal oscillator is working properly or not
bool isStable   = STATUS_FAIL;
uint8_t  xtalStatus  = 0;

// This function sets up the system clocks for the MSP430 (code mainly gotten from official guide)
void initializeClocks(void) {

    // Tell the system the frequencies of the crystals we’re using on the board
    CS_setExternalClockSource(
            LOW_FREQ_CRYSTAL_HZ, // Frequency of low-speed crystal
            HIGH_FREQ_CRYSTAL_HZ // Frequency of high-speed crystal
    );

    // Save the current values of each clock to our variables (before configuration)
    freqACLK = CS_getACLK();
    freqSMCLK = CS_getSMCLK();
    freqMCLK = CS_getMCLK();

    // Try to turn on the low-frequency crystal oscillator (LFXT)
    isStable  = CS_turnOnLFXTWithTimeout(
                  CS_LFXT_DRIVE_0,
                  CLOCK_TIMEOUT
              );

    // If the oscillator didn’t stabilize in time, go into an infinite loop (halt the system)
    if ( isStable == STATUS_FAIL )
    {
        while( 1 );   // Stuck here forever – this means the clock failed to start, something's wrong!
    }


    // Set the main internal oscillator (DCO) to a specific frequency range (we picked medium-fast)
    CS_setDCOFreq(
            CS_DCORSEL_0, // Low frequency range
            CS_DCOFSEL_3 // Set to 4 MHz
    );

    // Set ACLK (Auxiliary Clock) to use the low-frequency external crystal (LFXT)
    CS_initClockSignal(
            CS_ACLK,
            CS_LFXTCLK_SELECT, // Chose the low-frequency crystal as source
            CS_CLOCK_DIVIDER_1 // Don’t divide the clock, use it as-is
    );

    // Set MCLK (Main Clock) to also use the DCO, but don’t divide it
    CS_initClockSignal(
            CS_MCLK,
            CS_DCOCLK_SELECT,
            CS_CLOCK_DIVIDER_1
    );

    // Set SMCLK (Sub-Main Clock) to use the DCO (Digitally Controlled Oscillator)
    CS_initClockSignal( 
            CS_SMCLK,
            CS_DCOCLK_SELECT,
            CS_CLOCK_DIVIDER_2
    );

    // Get and save the updated clock speeds after configuration
    freqACLK        = CS_getACLK();
    freqSMCLK    = CS_getSMCLK();
    freqMCLK         = CS_getMCLK();
}

