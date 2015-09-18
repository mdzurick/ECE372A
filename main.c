/* 
 * File:   main.c
 * Author: mdzurick
 * Description: lab 0.
 * Created on August 27, 2015, 10:14 AM
 */

#include <xc.h>
#include <sys/attribs.h>
#include "switch.h"
#include "timer.h"
#include "led.h"
#include "interrupt.h"
#include "config.h"

#define OUTPUT 0
#define INPUT 1

#define PRESSED 0
#define UNPRESSED 1

//DONE: Define states of the state machine

typedef enum stateTypeEnum {
    led1, led2, led3, wait, debouncePress, debounceRelease
} stateType;

//DONE: Use volatile variables that change within interrupts
volatile stateType state = led1;
volatile unsigned int countInterruptTime = 0;

int main() {

    //This function is necessary to use interrupts. 
    enableInterrupts();

    //DONE: Write each initialization function
    initSwitch1();
    initLEDs();
    initTimer2();
    initTimer1();

    unsigned int numbLED;

    while (1) {
        switch (state) {
            /* wait: Used to wait for the button to be released. */
            case wait:
                // while (PORTDbits.RD6 == PRESSED) {}; // Wait until the button is no longer pressed.
                if (PORTDbits.RD6 == UNPRESSED) { // Don't go on until the button is released.
                    T1CONbits.TON = 0; // Turns off the timer. (so that countInterruptTime is stopped).
                    state = debounceRelease;
                }
                break;

            /* led1: State where LED 1 is on and others are off.  Waiting for button press. */
            case led1:
                turnOnLED(1); // Turns on LED 1. Off LED 2, 3;
                numbLED = 1;
                if (PORTDbits.RD6 == PRESSED) { // Since pull-up resistor is used we check for logic low for button press.
                    TMR1 = 0; // Resets timer to 0 value.
                    countInterruptTime = 0; // Resets the counter for seconds to 0.
                    T1CONbits.ON = 1; // Turns on the timer.
                    state = debouncePress;
                }
                break;

            /* led2: State where LED 2 is on and others are off.  Waiting for button press. */
            case led2:
                turnOnLED(2); // Turns on LED 2. Off LED 1, 3.
                numbLED = 2;
                if (PORTDbits.RD6 == PRESSED) { // Check for press.
                    TMR1 = 0; // Resets timer to 0 value.
                    T1CONbits.ON = 1; // Turns on the timer.
                    countInterruptTime = 0; // Resets the counter for seconds to 0.
                    state = debouncePress;
                }
                break;

            /* led3: State where LED 3 is on and others are off.  Waiting for button press. */
            case led3:
                turnOnLED(3); // Turns on LED 3. Off LED 1, 2.
                numbLED = 3;
                if (PORTDbits.RD6 == PRESSED) { // Since pull-up resistor is used we check for logic low for button press.
                    TMR1 = 0; // Resets timer to 0 value.
                    T1CONbits.ON = 1; // Turns on the timer.
                    countInterruptTime = 0; // Resets the counter for seconds to 0.
                    state = debouncePress;
                }
                break;

            /* debouncePress: Debounces the switch with a 50 millisecond delay. */
            case debouncePress:
                delayMs(50); // Delays for 50 milliseconds in order to debounce the switch.
                state = wait;
                break;

            /* debounceRelease: Debounces the switch release with a 50 millisecond delay.  Then reoutes to the next LED. */
            case debounceRelease:
                delayMs(50); // Delays for 50 milliseconds in order to debounce the switch.
                if (numbLED == 1) { // Checks if previous LED was 1.
                    if (countInterruptTime < 2) { // If the held for less than two seconds...
                        state = led2;
                    } else { //  If held for more than two seconds...
                        state = led3;
                    }
                } else if (numbLED == 2) { // Checks if previous LED was 2.
                    if (countInterruptTime < 2) { // If the held for less than two seconds...
                        state = led3;
                    } else { //  If held for more than two seconds...
                        state = led1;
                    }
                } else { // if (numbLED == 3) { // Checks if previous LED was 3.
                    if (countInterruptTime < 2) { // If the held for less than two seconds...
                        state = led1;
                    } else { //  If held for more than two seconds...
                        state = led2;
                    }
                }
                break;
 
        }
    }

    return 0;
}

void __ISR(_TIMER_1_VECTOR, IPL3SRS) _T1Interrupt() {
    IFS0bits.T1IF = 0; // Puts down the interrupt flag.
    countInterruptTime++; // Counts up the number of times that 1 second is reached.
}

/*
void __ISR(_TIMER_2_VECTOR, IPL3SRS) _T2Interrupt(){
    IFS0bits.T2IF = 0; // Puts down the interrupt flag.
}
 */