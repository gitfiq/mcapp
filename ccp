/*
 * File:   ccp.c
 * Author: ahmad
 *
 * Created on 21 July, 2023, 10:39 PM
 */


#include <xc.h>
#include "config.h"

void initCCP(void){
    CCP1CON = 0b00001111;
    CCPR1H = 0b00000000;
    CCPR1L = 0b01010001;
    RB4PPS = 0x0A;
}
