/******************************************************************************
 *
 * FILE: mian.c
 *
 * DESCRIPTION:
 *   Entry point and core game loop for the Simon Says-style memory game
 *   implemented on the MSP430FR6989 microcontroller.
 *   This program controls:
 *     - LED pattern generation and playback
 *     - Button-based user input
 *     - LCD display updates (rounds, prompts, and messages)
 *
 *   The game generates a randomized sequence of red and green LED flashes
 *   which the player must reproduce by pressing the corresponding buttons.
 *   The sequence grows longer with each successful round. If the player
 *   makes a mistake or completes all rounds, a Game Over or You Win message
 *   is displayed.
 *
 *   This file handles initialization of all peripherals (GPIO, Clock,
 *   Timers, and LCD), controls the main game loop, and manages transitions
 *   between game states.
 *
 * CREATED BY:
 *   Hanna Szalai & Haroun Riahi
 *
 ******************************************************************************/

#include <driverlib.h>
#include <clockConfig.h>
#include <string.h>
#include <lcdDisplay.h>
#include <gpioSetup.h>
#include <msp430.h>

// RED LED
#define RED_ON  0x0001
#define RED_OFF   0xFFFE

// GREEN LED
#define GREEN_ON  0x0080
#define GREEN_OFF 0xFF7F


// BUTTONS
#define BUTTON_ONE   0x0002
#define BUTTON_TWO  0x0004

// TIMERS
#define TIMER_UP_MODE   0x0010
#define TIMER_CONTINUOUS  0x0020
#define TIMER_ACLK_SRC 0x0100
#define DISABLE_WATCHDOG  0x5A80

#define ENABLE_PINS  0xFFFE
#define BUTTON_DELAY    0xA000
#define TEN_MS_COUNT  400
#define TIMER_SMCLK_SRC   0x0200


void main (void)
{
    void scrollText(char msg[250]);
    void showText(char msg[250]);
    void showNumber(unsigned long int number);
    void showMultiWords(char msg[250]);
    int  delay(int count);
    int  generateRandomPattern(int pattern);
    void playSequence(int pattern, int i);
    int  getUserInput(int pattern, int gameOver, int i);
    void displayGameOver(int points);

    // Game state variables
    int round;
    int points;
    int pattern;
    int gameOver;
    int delayCount;

    // Disable watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Initialize hardware
    initializePins();
    initializeClocks();
    LCD_init();
    PM5CTL0 = ENABLE_PINS;

    // Set up Timer A0 for continuous mode using SMCLK
    TA0CTL   = TA0CTL | (TIMER_SMCLK_SRC + TIMER_CONTINUOUS);
    TA0CCTL0 = CCIE;

    // Set up Timer A1 for up mode using ACLK
    TA1CTL   = TA1CTL | (TIMER_ACLK_SRC  + TIMER_UP_MODE         );
    TA1CCR0  = TEN_MS_COUNT;

    // Enable interrupts globally
    _BIS_SR(GIE);

    while(1)
    {
        // Reset state at beginning of game
        gameOver = 0;
        pattern = 0;
        points    = 0;

        // Show initial LEDs and wait for user to start
        P1OUT = P1OUT | RED_ON;
        P9OUT = P9OUT | GREEN_ON;
        scrollText("BEGIN BY PRESSING S1");

        // Generate RANDOM game sequence
        pattern = generateRandomPattern(pattern);

        // Begin main gameplay loop
        while(!gameOver)
        {
            for(round=0; round<50; round=round+1)
            {
                P1OUT = P1OUT & RED_OFF;
                P9OUT = P9OUT & GREEN_OFF;
                delayCount = 50;
                while(delayCount = delay(delayCount));

                if(!gameOver)
                {
                    showText("LEVEL");
                    delayCount = 50;
                    while(delayCount = delay(delayCount));
                    showNumber(round+1);
                    delayCount = 50;
                    while(delayCount = delay(delayCount));

                    // Show and evaluate input
                    playSequence(pattern,round);
                    gameOver = getUserInput(pattern,gameOver,round);

                    if(!gameOver)
                    {
                        points += 1;
                    }
                }
                else
                {
                    break;
                }
            }
            gameOver = 1;

        }
        // Show game over screen
        displayGameOver(points);
    }
}

// Timer_A0 Interrupt Service Routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
    TA0CTL = TA0CTL & (~TAIFG);
}

// Clears the LCD screen by writing empty spaces (' ') to all 6 visible positions.
void clearDisplay(void)
{
    int positionIndex = 1;
    // Go through each of the 6 LCD slots and set them to ' ' (a blank space)
    while (positionIndex <= 6)
    {
        LCD_showChar(' ', positionIndex);
        positionIndex++;
    }
}

// This function scrolls a message across the 6-character LCD screen one letter at a time.
void scrollText(char msg[250])
{
    // These are helper functions we use inside this function.
    void clearDisplay(void);
    int delay(int count);

    // Finding out how long the message is
    unsigned int msgLen = strlen(msg);
    // This keeps track of how far we've shifted the message
    unsigned int shiftPos = 0;

    unsigned int lcdPos, msgPos;
    char currentChar;
    int wait;

    // Clear the screen so there's nothing showing
    clearDisplay();

    // Keep moving the message until the entire string has scrolled off the screen
    while (shiftPos < msgLen + 6)
    {
        // Start scrolling from this position in the message
        msgPos = shiftPos;

        for (lcdPos = 1; lcdPos <= 6; lcdPos++)
        {
            if (msgPos >= 6 && msgPos < msgLen + 6)
            {
                // Display the character at the current LCD position
                currentChar = msg[msgPos - 6];
                LCD_showChar(currentChar, lcdPos);
            }
            else
            {
                // If there's nothing left to show, just put a blank space
                LCD_showChar(' ', lcdPos);
            }
            msgPos++;
        }

        // Wait a bit so the message scrolls slowly and is readable
        wait = 20; // 20 x 10ms = 200ms delay
        while (wait = delay(wait));
        // Move the scroll one character to the left
        shiftPos++;
    }
    // Clear screen again after message has fully scrolled off
    clearDisplay();
}


// Displays a short word (6 characters max) directly on the LCD screen.
void showText(char msg[250])
{
    // Function to clear the LCD
void clearDisplay(void);

unsigned int i;
unsigned int msgLength;
char currentChar;

clearDisplay();

// Measure how long the message is
msgLength = strlen(msg);

if (msgLength <= 6)
{
    // Loop through the message and show each character
    for (i = 0; i < msgLength; i++)
    {
        currentChar = msg[i];         // Get the character from the message
        LCD_showChar(currentChar, i + 1);  // Display it at position 1-6
    }
}
}

// This is how we are showing numbers on the levels
void showNumber(unsigned long int number)
{
    void clearDisplay(void);  // Clear LCD before showing anything

    clearDisplay();

    number = number % 1000000;

    char buffer[7];            // Buffer to hold up to 6 digits + null terminator
    int numDigits = 0;

    // Convert number to string manually
    do {
        buffer[numDigits++] = (number % 10) + '0';
        number /= 10;
    } while (number > 0 && numDigits < 6);

    // Print characters in reverse order to LCD
    int lcdPos = 6;
    int i;

    // Fill left side with spaces for padding
    for (i = 0; i < 6 - numDigits; i++) {
        LCD_showChar(' ', lcdPos--);
    }

    // Display number digits, right-aligned
    for (i = numDigits - 1; i >= 0; i--) {
        LCD_showChar(buffer[i], lcdPos--);
    }
}


// Display multiple words on the LCD, one at a time, separated by spaces
void showMultiWords(char msg[250])
{
    void clearDisplay(void);
    int delay(int count);

    unsigned int i = 0;  // Index in the message string
    unsigned int lcdPos = 1; // Position on the LCD (1 to 6)
    unsigned int delayCount;
    char ch;

    clearDisplay();

    // Loop until the end of the message
    while (msg[i] != '\0')
    {
        // Reset LCD position for each word
        lcdPos = 1;

        // Display a word until it hit a space or end of message
        while (msg[i] != ' ' && msg[i] != '\0' && lcdPos <= 6)
        {
            ch = msg[i++];
            LCD_showChar(ch, lcdPos++);
        }

        // Wait after displaying the word
        delayCount = 40;                // ~400ms delay between words
        while ((delayCount = delay(delayCount)));

        clearDisplay();

        // If space was found, skip over it to get to the next word
        if (msg[i] == ' ')
            i++;
    }

    // Final delay and cleanup
    delayCount = 40;
    while ((delayCount = delay(delayCount)));
    clearDisplay();
}

int delay(int count)
{
    if (TA1CTL & TAIFG)
    {
        // Reduce remaining wait cycles
        count--;
        // Acknowledge and clear overflow flag
        TA1CTL &= ~TAIFG;
    }

    return count;
}

// Generates a pseudo-random 16-bit seed based on timer value and user input
int generateRandomPattern(int pattern)
{
    while(!pattern)
    {
        if(!(P1IN & BUTTON_ONE))
        {
            pattern = TA0R;
        }
    }
    return pattern;
}

// Plays a sequence of LED flashes for the user to observe.
void playSequence(int pattern, int round)
{
    int delay(int count);
    void showText(char msg[250]);

    int ledBit = 0;       // Current bit from the pattern
    int step = 0;         // Step through the sequence
    int wait = 0;

    while (step <= round)
    {
        showText("WATCH");

        wait = 25;
        while ((wait = delay(wait)));  // Short pause before LED

        ledBit = (pattern >> step) & 0x01;

        if (ledBit == 0)
        {
            P1OUT |= RED_ON;        // Red LED ON
            P9OUT &= GREEN_OFF;     // Green LED OFF
        }
        else
        {
            P9OUT |= GREEN_ON;      // Green LED ON
            P1OUT &= RED_OFF;       // Red LED OFF
        }

        wait = 25;
        while ((wait = delay(wait)));  // Hold LED state

        P1OUT &= RED_OFF;           // Turn off red LED
        P9OUT &= GREEN_OFF;         // Turn off green LED

        step++;                     // Move to next step in pattern
    }
}


// This function waits for the player to replicate the LED pattern by pressing the correct buttons.
int getUserInput(int pattern, int gameOver, int round)
{
    void showText(char msg[250]);
    int delay(int count);

    int btnPressCount = 0;    // How many button presses the user has made
    int delayCount;
    int expectedBit;

    showText("GO");

    // Loop until all button presses are received or user makes a mistake
    while ((btnPressCount <= round) && !gameOver)
    {
        // --- Check if Button 1 (S1) is pressed ---
        if ((BUTTON_ONE & P1IN) == 0)
        {
            // Wait until the button is released (debouncing)
            while ((BUTTON_ONE & P1IN) == 0)
            {
                P1OUT |= RED_ON;          // Light red LED to confirm press
                delayCount = 3;           // Short delay (30ms)
                while ((delayCount = delay(delayCount)));
            }
            P1OUT &= RED_OFF;            // Turn off red LED

            // Get the expected bit from the pattern
            expectedBit = (pattern >> btnPressCount) & 0x01;

            // If expected bit is not 0, player made a mistake
            if (expectedBit != 0)
            {
                gameOver = 1;         // Set game over flag
                break;                  // Exit input loop
            }

            btnPressCount++;         // Move to next expected input
        }

        // --- Check if Button 2 (S2) is pressed ---
        else if ((BUTTON_TWO & P1IN) == 0)
        {
            // Wait until the button is released (debouncing)
            while ((BUTTON_TWO & P1IN) == 0)
            {
                P9OUT |= GREEN_ON;        // Light green LED to confirm press
                delayCount = 3;         // Short delay (30ms)
                while ((delayCount = delay(delayCount)));
            }
            P9OUT &= GREEN_OFF;        // Turn off green LED

            // Get the expected bit from the pattern
            expectedBit = (pattern >> btnPressCount) & 0x01;

            // If expected bit is not 1, player made a mistake
            if (expectedBit == 0)
            {
                gameOver = 1;            // Set game over flag
                break;                     // Exit input loop
            }

            btnPressCount++;         // Move to next expected input
        }
    }

    return gameOver; // Return 1 if player lost, 0 if they got the pattern right
}


// This function handles the game over screen
void displayGameOver(int points)
{
    void scrollText(char msg[250]);
    void showNumber(unsigned long int number);

    // Check if the player won (got all 16 correct)
    if (points == 16)
    {
        scrollText("YOU WIN");
    }
    else
    {
        scrollText("GAME OVER FINAL SCORE");
        showNumber(points);
    }

    // Wait for player to press both buttons (S1 and S2) to restart the game
    while (1)
    {
        if (!(BUTTON_ONE & P1IN) && !(BUTTON_TWO & P1IN))
        {
            break;  // Exit when both buttons are held
        }
    }
}
