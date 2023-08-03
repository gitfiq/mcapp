/*
 * File:   sev_seg.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:25 PM
 */


#include <xc.h>
#include "config.h"

#define SEV_SEG_PORT  PORTC

//to store the ADC temp value
unsigned int tempcnt = 0;

const unsigned char segTable[11] = {
    0b11000000, 0b11111001,
    0b10100100, 0b10110000,
    0b10011001, 0b10010010,
    0b10000010, 0b11111000,
    0b10000000, 0b10011000,
    0b11111111 };
    
//display the temperature value on the seven segment
// Time critical task( will be displayed in the isr - Timer 1 )
void sevseg_LSB(void){
    SEV_SEG_PORT = segTable[10];
    int dig0 = tempcnt % 10;
    PORTEbits.RE1 = 0;
    PORTEbits.RE0 = 1;
    SEV_SEG_PORT = segTable[dig0];
}

void sevseg_MSB(void){
    SEV_SEG_PORT = segTable[10];
    int dig1 = tempcnt / 10;
    PORTEbits.RE1 = 1;
    PORTEbits.RE0 = 0;
    SEV_SEG_PORT = segTable[dig1];
}

//Getting the ADC value
void getTemp_count(unsigned int gettempcnt){
    tempcnt = gettempcnt;
}
