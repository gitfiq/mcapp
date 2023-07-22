/*
 * File:   main.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:04 PM
 */


#include <xc.h>
#include "config.h"

void initSysPins(void);
void initSysTimer(void);
void initSysIOC(void);
void initExtInt(void);
void initCCP(void);
void initADC(void);
void initLCD(void);

void adc_GetConversion(void);

void dspTask_OnLCD(void);
void dspTask_OnSeg(void);

void scheduled_Motor(void);
void usrTask(void);

void main(void) {
    
    initSysPins();
    initSysTimer();
    initSysIOC();
    initExtInt();
    initCCP();
    initADC();
    initLCD();
    
    while(1){
        adc_GetConversion();
        
        dspTask_OnLCD();
        dspTask_OnSeg();

        usrTask();
        //scheduled_Motor();
    }
    return;
}

void initSysPins(void){
    ANSELA = 0b00000011;
    ANSELB = 0b00000000;
    ANSELC = 0b00000000;
    ANSELD = 0b00000000;
    ANSELE = 0b00000000;
    
    TRISA = 0b11110111;
    TRISB = 0b11000000;
    TRISC = 0b00000000;
    TRISD = 0b11000000;
    TRISE = 0b11111100;
}
