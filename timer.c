/*
 * File:   timer.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 5:32 PM
 */


#include <xc.h>
#include "config.h"

unsigned int toggle_CntReoxygenation, delayFlag_Reoxygenation, targetdelay_Reoxygenation = 0;           //toggle_Reoxygenation is to keep track of timer 3 
                                                                                                        //delayFlag_Reoxygenation is triggered to stop the while loop in the delay_Reoxygenation()
                                                                                                        //targetdelay_Reoxygenation is used to store the milisec value from the simulate task file

unsigned int toggle_CntpHDsp, delayFlag_pHDsp, targetdelay_pHDsp = 0;                                   //toggle_pHDsp is to keep track of timer 5 
                                                                                                        //delayFlag_pHDsp is triggered to stop the while loop in the delay_pHDsp()
                                                                                                        //targetdelay_pHDsp is used to store the milisec value from the simulate task file

unsigned int minTime = 0;               //to track the min time
unsigned int hrTime = 0;                //to track the Hr time (display on LCD)

int alarm_cnt = 0;                      //track the alarm count
int motor_cnt = 0;                      //track the motor count

void initSysTimer(void){
    INTCONbits.GIE = 0;
    INTCONbits.PEIE = 0;
    
    //Timer 0 is set to 0.5 s to on the motor at every scheduled interval ( 5hrs - 2.5 min )
    T0CON0 = 0b10010000;
    T0CON1 = 0b01000011;
    PIR0bits.TMR0IF = 0;
    PIE0bits.TMR0IE = 1;
    
    //Timer 1 set to 5 ms to refresh the seven segment display
    T1CLKbits.CS = 0b0001;
    T1CON = 0b00000011;
    PIR4bits.TMR1IF = 0;
    PIE4bits.TMR1IE = 1;
    
    //Timer 2 set to 15 ms for the speaker (CCP1)
    T2CLKbits.CS = 0b0001;
    T2CON = 0b00111110;         // disable timer 2 ,prescaler value - 8 , postscaler value - 15
    PIR4bits.TMR2IF = 0;
    PIE4bits.TMR2IE = 1;
    
    //Timer 3 set to 1 ms to be the base delay for Reoxygenation 
    T3CLKbits.CS = 0b0001;
    T3CON = 0b00000010;
    PIR4bits.TMR3IF = 0;
    PIE4bits.TMR3IE = 1;
    
    //Timer 4 set 30 ms for motor (CCP2)
    T4CLKbits.CS = 0b0001;
    T4CON = 0b00111110;         // disable timer 4 ,prescaler value - 8 , postscaler value - 15
    PIR4bits.TMR4IF = 0;
    PIE4bits.TMR4IE = 1;
    
    //Timer 5 set to 1 ms for the base delay for pH display
    T5CLKbits.CS = 0b0001;
    T5CON = 0b00000010;
    PIR4bits.TMR5IF = 0;
    PIE4bits.TMR5IE = 1;
    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

//called in isr to increase the min and Hr time accordingly
void incr_MinHrTime(void)
{
    if(minTime < 60){
        minTime++;
    }
    else{
        hrTime++;
        minTime = 0;
    }
}

//returns the Hr time
unsigned int get_hrTime(void)
{
    return(hrTime);
}

//called in simulate task to allow to set the desired delay (Reoxygenation)
void delay_Reoxygenation(unsigned int milisec){
    delayFlag_Reoxygenation = 0;
    targetdelay_Reoxygenation = milisec;
    
    T3CONbits.ON = 1;
    
    while(!delayFlag_Reoxygenation);
    
    T3CONbits.ON = 0;
}

//called in simulate task to allow to set the desired delay (pH warning display)
void delay_pHDsp(unsigned int milisec){
    delayFlag_pHDsp = 0;
    targetdelay_pHDsp = milisec;
    
    T5CONbits.ON = 1;
    
    while(!delayFlag_pHDsp);
    
    T5CONbits.ON = 0;
}

//called in isr to increase alarm count accordingly (timer 2)
void incr_Alarmcnt(void){
    if(alarm_cnt <= 1000){
        alarm_cnt++;
    }
    else{
        CCP1CONbits.EN = 0;         //disable CCP1 
        T2CONbits.ON = 0;           //disable Timer 2 to stop counting
    }
}

//called in isr to increase motor count accordingly (timer 4)
void incr_Motorcnt(void){
    if(motor_cnt <= 100){
        motor_cnt++;
    }
    else{
        CCP2CONbits.EN = 0;         //disable CCP2           
        T4CONbits.ON = 0;           //disable Timer 4 to stop counting
    }
}

