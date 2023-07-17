/*
 * File:   timer.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 5:32 PM
 */


#include <xc.h>
#include "config.h"

void initSysTimer(void){
    INTCONbits.GIE = 0;
    INTCONbits.PEIE = 0;
    
    //Timer 1
    T1CLKbits.CS = 0b0001;
    T1CON = 0b00110011;
    TMR1H = 0xFD;
    TMR1L = 0x8F;
    PIR4bits.TMR1IF = 0;
    PIE4bits.TMR1IE = 1;
    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}