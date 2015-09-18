/* 
 * File:   main.c
 * Author: mdzurick
 * Description: lab 0.
 * Created on August 27, 2015, 10:14 AM
 */;

#include <xc.h>
#include <sys/attribs.h>
#include "switch.h"
#include "timer.h"
#include "led.h"
#include "interrupt.h"
#include "config.h"

#define OUTPUT 0
#define INPUT 1

//DONE: Define states of the state machine

typedef enum stateTypeEnum {
    led1, led2, led3, wait, debouncePress, debounceRelease
} stateType;

//TODO: Use volatile variables that change within interrupts
volatile stateType state = led1;
volatile unsigned int countInterruptTime = 0;

int main() {

    //This function is necessary to use interrupts. 
    enableInterrupts();

    //TODO: Write each initialization function
    initSwitch1();
    initLEDs();
    initTimer2();
    initTimer1();

    unsigned int numbLED;

    while (1) {
        switch (state) {
            case wait:
                while (PORTDbits.RD6 == 0) {}; // Use timer ON/OFF in order to control the counter.
                T1CONbits.TON = 0; // Turns off the timer.
                state = debounceRelease;
                break;

            case led1:
                turnOnLED(1); // Turns on LED 1. Off LED 2, 3;
                numbLED = 1;
                if (PORTDbits.RD6 == 0) {
                    TMR1 = 0;
                    countInterruptTime = 0;
                    T1CONbits.ON = 1; // Turns on the timer.
                    state = debouncePress;
                }
                break;

            case led2:
                turnOnLED(2); // Turns on LED 2. Off LED 1, 3.
                numbLED = 2;
                if (PORTDbits.RD6 == 0) {
                    TMR1 = 0;
                    T1CONbits.ON = 1; // Turns on the timer.
                    countInterruptTime = 0;
                    state = debouncePress;
                }
                break;

            case led3:
                turnOnLED(3); // Turns on LED 3. Off LED 1, 2.
                numbLED = 3;
                if (PORTDbits.RD6 == 0) {
                    TMR1 = 0;
                    T1CONbits.ON = 1; // Turns on the timer.
                    countInterruptTime = 0;
                    state = debouncePress;
                }
                break;

            case debouncePress:
                delayMs(50);
                state = wait;
                break;

            case debounceRelease:
                delayMs(50);
                if (numbLED == 1) {
                    if (countInterruptTime < 2) {
                        state = led2;
                    } else {
                        state = led3;
                    }
                } else if (numbLED == 2) {
                    if (countInterruptTime < 2) {
                        state = led3;
                    } else {
                        state = led1;
                    }
                } else { // if (numbLED == 3) {
                    if (countInterruptTime < 2) {
                        state = led1;
                    } else {
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
    countInterruptTime++;
}

/*
void __ISR(_TIMER_2_VECTOR, IPL3SRS) _T2Interrupt(){
    IFS0bits.T2IF = 0; // Puts down the interrupt flag.
}
 */