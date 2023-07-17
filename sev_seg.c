/*
 * File:   sev_seg.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:25 PM
 */


#include <xc.h>
#include "config.h"

#define SEV_SEG_PORT  PORTC

int sevseg_Flag = 0;

const unsigned char segTable[11] = {
    0b11000000, 0b11111001,
    0b10100100, 0b10110000,
    0b10011001, 0b10010010,
    0b10000010, 0b11111000,
    0b10000000, 0b10011000,
    0b11111111 };
    
//display the temperature value on the seven segment
void seg_DspAll(unsigned int Temp){
    int dig0, dig1;
    
    if(sevseg_Flag % 2 == 0){
        SEV_SEG_PORT = segTable[10];
        PORTEbits.RE1 = 0;
        PORTEbits.RE0 = 1;
        dig0 = Temp % 10;
        SEV_SEG_PORT = segTable[dig0];
    }
    else{
        SEV_SEG_PORT = segTable[10];
        PORTEbits.RE1 = 1 ;
        PORTEbits.RE0 = 0;
        dig1 = Temp / 10;
        SEV_SEG_PORT = segTable[dig1];
    }
    
}

