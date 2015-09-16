/* 
 * File:   switch.c
 * Author: mdzurick
 *
 * Created on August 27, 2015, 3:12 PM
 */

#include <xc.h>

#define OUTPUT 0
#define INPUT 1

void initSwitch1(){
    //TODO: Initialize switch 1
    TRISDbits.TRISD6 = INPUT; // Configures switch as an input.
    CNPUDbits.CNPUD6 = 1; // Applies a pull-up resistor to the above switch.
    
    
    CNENDbits.CNIED6 = 1; // Enable CN Interrupt
    IEC1bits.CNDIE = 1; // Enable CN Interrupt (general))
    IFS1bits.CNDIF = 0; // Put the interrupt flag down.
}