# MSP430 Memory Game 🎮

An embedded memory game inspired by *Simon Says*, built using the MSP430FR6989 microcontroller.

### 🎬 Demo
Watch a short demo of the game in action: 
[![Watch the video](https://img.youtube.com/vi/dQw4w9WgXcQ/0.jpg)](https://www.youtube.com/watch?v=dQw4w9WgXcQ)

## 🧑‍💻 Authors
Hanna Szalai
Haroun Riahi

## 🧠 Gameplay Overview
Players must memorize and replicate a growing sequence of red and green LED blinks using two buttons. Each round increases difficulty by extending the sequence.

### Features
- LED feedback (Red & Green)
- LCD screen display (scrolling, number & word output)
- Two-button input with debounce logic
- Random pattern generation using hardware timer
- Up to **50 rounds** of increasing difficulty
- Game over + score display

## 🛠 Built With
- C & DriverLib
- Code Composer Studio (CCS)
- MSP430FR6989 (TI LaunchPad)
- LCD_C Driver
- Timer_A and GPIO interrupts

## 🚀 Getting Started

### Requirements
- TI MSP430FR6989 LaunchPad
- Code Composer Studio
- USB Debug Interface
- External power supply (optional)

### To Run
1. Clone the repo:
   ```bash
   git clone https://github.com/YOUR_USERNAME/msp430-memory-game.git
   ```

2. Download and install **Code Composer Studio (CCS)**:
   - Visit the [TI Code Composer Studio download page](https://www.ti.com/tool/CCSTUDIO).
   - Download the version compatible with your operating system.
   - Follow the installation instructions provided by TI.

3. Open the project in CCS:
   - Launch Code Composer Studio.
   - Go to `File > Import > Code Composer Studio Projects`.
   - Select the cloned repository folder and click `Finish`.

4. Build the project:
   - In the CCS Project Explorer, right-click on the project and select `Build Project`.
   - Ensure there are no build errors.

5. Connect the MSP430FR6989 LaunchPad:
   - Plug in the LaunchPad to your computer using a USB cable.
   - Ensure the drivers are installed (CCS should handle this automatically).

6. Debug and run the program:
   - Click the `Debug` button in CCS (or press `F11`).
   - The program will be loaded onto the MSP430FR6989.
   - Once debugging starts, click `Resume` (or press `F8`) to run the program.

7. Play the game:
   - Follow the LED sequence and use the buttons to replicate it.
   - Enjoy the game!