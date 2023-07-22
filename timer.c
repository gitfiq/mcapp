/*
 * File:   timer.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 5:32 PM
 */


#include <xc.h>
#include "config.h"

//toggle_Reoxygenation is to keep track of timer 3 
//delayFlag_Reoxygenation is triggered to stop the while loop in the delay_150ms
//targetdelay_Reoxygenation is used to store the milisec value from the simulate task file
unsigned int toggle_CntReoxygenation, delayFlag_Reoxygenation, targetdelay_Reoxygenation = 0;

unsigned int toggle_CntMotor, delayFlag_Motor, targetdelay_Motor = 0;

unsigned int minTime = 0;
unsigned int hrTime = 0;

int speaker_cnt = 0;

void initSysTimer(void){
    INTCONbits.GIE = 0;
    INTCONbits.PEIE = 0;
    
    //Timer 0 is set to 0.5 s to on the motor at every scheduled  interval 
    T0CON0 = 0b10010000;
    T0CON1 = 0b01000011;
    PIR0bits.TMR0IF = 0;
    PIE0bits.TMR0IE = 1;
    
    //Timer 1 set to 5 ms to refresh the seven segment display
    T1CLKbits.CS = 0b0001;
    T1CON = 0b00000011;
    PIR4bits.TMR1IF = 0;
    PIE4bits.TMR1IE = 1;
    
    //Timer 3 set to 1 ms to be the base delay for 150 ms 
    T3CLKbits.CS = 0b0001;
    T3CON = 0b00000010;
    PIR4bits.TMR3IF = 0;
    PIE4bits.TMR3IE = 1;
    
    //Timer 5 set to 0.5 s for the Speaker
    T5CLKbits.CS = 0b0001;
    T5CON = 0b00000010;
    PIR4bits.TMR5IF = 0;
    PIE4bits.TMR5IE = 1;
    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

void Incr_minTime(void)
{
    if(minTime < 60){
        minTime++;
    }
    else{
        hrTime++;
        minTime = 0;
    }
}

unsigned int get_hrTime(void)
{
    return(hrTime);
}

void delay_Reoxygenation(unsigned int milisec){
    delayFlag_Reoxygenation = 0;
    targetdelay_Reoxygenation = milisec;
    
    T3CONbits.ON = 1;
    
    while(!delayFlag_Reoxygenation);
    
    T3CONbits.ON = 0;
}

void delay_Motor(unsigned int milisec){
    delayFlag_Motor = 0;
    targetdelay_Motor = milisec;
    
    T3CONbits.ON = 1;
    
    while(!delayFlag_Motor);
    
    T3CONbits.ON = 0;
}

void incr_Speakercnt(void){
    if(speaker_cnt <= 8){
        speaker_cnt++;
    }
    else{
        speaker_cnt = 0;
    }
}

