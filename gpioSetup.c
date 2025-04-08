/******************************************************************************
 *
 * FILE: gpioSetup.c
 *
 * DESCRIPTION:
 *   This file configures all the input/output pins used in the game.
 *   - Sets up the red and green LEDs as outputs.
 *   - Sets up both buttons as inputs.
 *   - Initializes the pins for the low-frequency crystal oscillator.
 *
 * CREATED BY:
 *   Hanna Szalai & Haroun Riahi
 *
 ******************************************************************************/

#include <driverlib.h>
#include "gpioSetup.h"

#define GREEN_ON 0x0080
#define GREEN_OFF 0xFF7F

#define RED_ON   0x0001
#define RED_OFF  0xFFFE

#define ENABLE_PINS  0xFFFE

#define BUTTON_ONE	0x0002
#define BUTTON_TWO	0x0004

// This function prepares all necessary GPIO pins for use
void initializePins(void)
{
    // Set green LED pin (P9.7) to output mode
    P9OUT &= GREEN_OFF;
    P9DIR |= GREEN_ON;

    // Set red LED pin (P1.0) to output mode
    P1OUT &= RED_OFF;
	P1DIR |= RED_ON;

    // Allow GPIO pins to be turned on and off
    PM5CTL0 = ENABLE_PINS;

    // Set Button 1 (P1.1) as input with pull-up resistor
    P1REN = P1REN|BUTTON_ONE;
	P1OUT = P1OUT|BUTTON_ONE;

    // Set Button 2 (P1.2) as input with pull-up resistor
	P1OUT = P1OUT|BUTTON_TWO;
	P1REN = P1REN|BUTTON_TWO;

    // Set the pins for the low-speed crystal oscillator
    // These are used to give accurate timing for delays, etc.
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_PJ,
            GPIO_PIN4 +
            GPIO_PIN5 ,
            GPIO_PRIMARY_MODULE_FUNCTION
    );
}
