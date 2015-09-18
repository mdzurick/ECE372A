/* 
 * File:   timer.c
 * Author: mdzurick
 *
 * Created on August 27, 2015, 3:26 PM
 */

/******************************************/

#include <xc.h>

/******************************************/

#define FREQUENCY 625000
#define TIME_DELAY_TIMER1 2

#define FLAG_DOWN 0

/*****************************************/

void initTimer1(){
    //DONE: Initialize Timer 1 to have a period of
    // 1 second. Enable its interrupt
    unsigned int prValue = (unsigned int)(((FREQUENCY*TIME_DELAY_TIMER1)/256) - 1);
    
    TMR1 = 0; // Enables register for Timer 1.
    PR1 = prValue; // This puts the calculated value into the PR1.
    
    /* Choices of prescalar are 1, 8, 64, 256. */
    T1CONbits.TCKPS = 0x3; // Prescalar enabled with choice of 256.
    
    T1CONbits.TCS = 0; // Configures the oscillator.
    
    IEC0bits.T1IE = 1; // Enable the interrupt.
    IFS0bits.T1IF = FLAG_DOWN; // Interrupt Flag is now down.
    IPC1bits.T1IP = 3; // Set the interrupt priority.
    
    // T1CONbits.TON = 1; // Turns on the timer.
}


void initTimer2(){
    //DONE: Initialize Timer 2.
    TMR2 = 0; // Enables register for Timer 2.
    
    /* Choices of prescalar are 1, 2, 4, 8, 16, 32, 64, 256. */
    T2CONbits.TCKPS = 0x7; // Prescalar enabled with choice of 256.
    
    T2CONbits.TCS = 0; // Configures the oscillator.
    IFS0bits.T2IF = FLAG_DOWN; // Interrupt flag is down.
//    IPC2bits.T2IP = 3; // Set the interrupt priority.
//    IEC0bits.T2IE = 1; // Enable the interrupt.
    
}

void delayMs(int delay){
    //DONE: Using timer 2, create a delay
    // that is delay amount of ms.
    unsigned int prValue = (unsigned int)(((FREQUENCY*delay)/(256*1000)) - 1);
    // unsigned int prValue = (unsigned int)(((FREQUENCY*delay)/256*1000) - 1);
    
    TMR2 = 0;
    PR2 = prValue; // This puts the calculated value into the PR2.
    
    IFS0bits.T2IF = FLAG_DOWN; // Interrupt flag is down.
    T2CONbits.TON = 1; // Turns on Timer 2.
    while (IFS0bits.T2IF == FLAG_DOWN) {}; // Waiting for the interrupt flag to go high.
    
    IFS0bits.T2IF = FLAG_DOWN; // Interrupt flag is down.
    
    T2CONbits.TON = 0; // Turns off Timer 2.
}

