/*
 * File:   ccp.c
 * Author: ahmad
 *
 * Created on 21 July, 2023, 10:39 PM
 */


#include <xc.h>
#include "config.h"

void initCCP(void){
    CCP1CON = 0b00001111; // disable CCP1 & set CCP1 to PMW mode
    CCPTMRS0bits.C1TSEL = 0b01; // allocate the PMW to timer 2 for PMW
    CCPR1H = 0b00000000;
    CCPR1L = 0b11001101; //set to 20%
    RB4PPS = 0x09; // set Speaker to CCP1
    
    CCP2CON = 0b00001111; // disable CCP2 & set CCP2 to PMW mode
    CCPTMRS0bits.C2TSEL = 0b10; // allocate the PMW to timer 4 for PMW
    CCPR2H = 0b00000011;
    CCPR2L = 0b00110100; //set to 80%
    RB5PPS = 0x0A; // set Motor to CCP2
}
