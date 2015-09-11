/* 
 * File:   led.c
 * Author: mdzurick
 *
 * Created on August 27, 2015, 3:15 PM
 */

/*************************************************/

#include <xc.h>
#include "led.h"

/*************************************************/

#define OUTPUT 0
#define INPUT 1

/*************************************************/

void initLEDs(){
    //TODO: Initialize LEDs
    TRISDbits.TRISD0 = OUTPUT; // Turns on functionality of LED 1.
    TRISDbits.TRISD1 = OUTPUT; // Turns on functionality of LED 2.
    TRISDbits.TRISD2 = OUTPUT; // Turns on functionality of LED 3.
}

void turnOnLED(int led){
    //TODO: You may choose to write this function
    // as a matter of convenience
    if (led == 1) {
        LATDbits.LATD0 = 1; // Writes logic high to LED 1.
        LATDbits.LATD1 = 0; // Writes logic low to LED 2.
        LATDbits.LATD2 = 0; // Writes logic low to LED 3.
    }
    else if (led == 2) {
        LATDbits.LATD0 = 0; // Writes logic low to LED 1.
        LATDbits.LATD1 = 1; // Writes logic high to LED 2.
        LATDbits.LATD2 = 0; // Writes logic low to LED 3.
    }
    else { // (led == 3)
        LATDbits.LATD0 = 0; // Writes logic low to LED 1.
        LATDbits.LATD1 = 0; // Writes logic low to LED 2.
        LATDbits.LATD2 = 1; // Writes logic high to LED 3.
    }
}