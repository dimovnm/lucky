/*
 * File:   main.c
 * Author: Natalie Dimov & Milen Dimov
 *
 * Created on April 9, 2023, 6:20 PM
 */


#define _XTAL_FREQ 4000000

// CONFIG
#pragma config WDTE = OFF       // Watchdog Timer (WDT disabled)
#pragma config CP = OFF         // Code Protect (Code protection off)
#pragma config MCLRE = OFF      // Master Clear Enable (GP3/MCLR pin fuction is digital I/O, MCLR internally tied to VDD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

// #define variables to save data, rather than declaring as parameters of output function
#define numOfBeeps 3            // number of beeps executed
#define numberOfPeriods 500     // count of number of times f2 is executed over half of f1
                                // f1HalfPeriod_us / (f2HalfPeriod_us * 2)
#define f1HalfPeriod_us 500000  // Period of beep / 2 
#define f2HalfPeriod_us 500     // Period of frequency of beep / 2

void filter(uint8_t signalTime);
void generateOutput(void);

void main(void) {

    //Disable FOSC4 function on GP2 
    OSCCAL = 0x1E;

    // Initialization on Timer0 and Watchdog. 
    //OPTION=0xC0;
    OPTION = 0x80;

    // Initialize IOs    
    GPIO = 0x00;

    // GP2 - Output
    //TRISGPIO=0x04;
    TRIS = 0x0A;

    while (1) {
        
        GP2 = 0;
        filter(200);
        _delay((1000000 - (f1HalfPeriod_us + 5))); // microsecond input - initial delay from generateOutput() function
        //__delay_ms();  // millisecond input, larger error margin  
        generateOutput();
    }

}

void filter(uint8_t signalTime){
    uint8_t count = 0;
    // if input is stuck at 1, output will not be triggered
    while (1 == GP3) {
    }
    while (count < signalTime) {
        // resets counter if input is 0
        if (GP3 == 0) {
            count = 0;
        } else {
        // begins count if input is 1
            count++;
        }
        _delay(970);
    }
}

void generateOutput(void)
{
    // duty cycle: 50%
    GP2 = 0;
    _delay(f1HalfPeriod_us);
    
    // loops through number of beeps
    uint8_t i = 0;
    while (numOfBeeps > i){
        ++i;
        uint16_t count = 0;
        
        // outputs frequency based on given period of beeps
        while(numberOfPeriods > count){
            ++count;
            
            GP2 = 1;
            _delay(f2HalfPeriod_us);
            GP2 = 0;
            _delay(f2HalfPeriod_us);
        }
    }
}