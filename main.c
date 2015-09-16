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
typedef enum stateTypeEnum{
    led1, led2, led3, wait, debouncePress, debounceRelease
} stateType;

//TODO: Use volatile variables that change within interrupts
volatile stateType state = led1;

int main() {
    
    //This function is necessary to use interrupts. 
    enableInterrupts();
    
    //TODO: Write each initialization function
    initSwitch1();
    initLEDs();
    initTimer2();
    initTimer1();
    
    unsigned int numbLED;
    unsigned int countTime;
    
    while(1){
        switch(state) {
            case wait:
                countTime = 0;
                while (PORTDbits.RD6 == 1) {
                    ONE SECOND
                    countTime++;
                }
                state = debounceRelease;
                break;
                
            case led1:
                numbLED = 1;
                turnOnLED(1); // Turns on LED 1. Off LED 2, 3;
                if (PORTDbits.RD6 == 1) {
                    state = debouncePress;
                }
                break;
                
            case led2:
                numbLED = 2;
                turnOnLED(2); // Turns on LED 2. Off LED 1, 3.
                if (PORTDbits.RD6 == 1) {
                    state = debouncePress;
                }
                break;
                
            case led3:
                numbLED = 3;
                turnOnLED(3); // Turns on LED 3. Off LED 1, 2.
                if (PORTDbits.RD6 == 1) {
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
                    if (countTime < 2) {
                        state = led2;
                    }
                    else {
                        state = led3;
                    }
                }
                if (numbLED == 2) {
                    if (countTime < 2) {
                        state = led3;
                    }
                    else {
                        state = led1;
                    }
                }
                if (numbLED == 3) {
                    if (countTime < 2) {
                        state = led1;
                    }
                    else {
                        state = led2;
                    }
                }
                break;
        }
        //TODO: Implement a state machine to create the desired functionality
        
    }
    
    return 0;
}

