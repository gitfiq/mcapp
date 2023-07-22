/*
 * File:   isr.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:18 PM
 */


#include <xc.h>
#include "config.h"

//get the warning function from the simulate task file
void simulate_Warning(void);
//Emergency switch activated through the IOC. To stop any Reoxygenation and changing water due to pH
void usrTask_EmergencySwitch(void);

//get the MSB and LSB display functions from sev seg file.
void sevseg_LSB(void);
void sevseg_MSB(void);
//sevseg_Flag is used to toggle between the MSB to LSB seven segment
int sevseg_Flag = 0;

//oxyResult and tempResult to contain the conversion
extern unsigned int oxyResult;
extern unsigned int tempResult;
//type_ADC is used to toggle the ADC conversion between the potentiometer and LM35
extern int type_ADC;

//
void Incr_minTime(void);

//toggle_CntReoxygenation is to keep track of timer 3 
//delayFlag_Reoxygenation is triggered to stop the while loop in the delay_150ms
//targetdelay_Reoxygenation is used to store the milisec value from the simulate task file
extern unsigned int toggle_CntReoxygenation;
extern unsigned int delayFlag_Reoxygenation;
extern unsigned int targetdelay_Reoxygenation;

extern unsigned int toggle_CntMotor;
extern unsigned int delayFlag_Motor;
extern unsigned int targetdelay_Motor;

void incr_Speakercnt(void);
void onSpeaker(void);

void __interrupt() isr(void){
    if(PIR1bits.ADIF == 1){
        if(type_ADC % 2 == 0){
            oxyResult = (((ADRESH * 256) + ADRESL) / 1023.0) * 10;
            type_ADC++;
        }
        else{
            tempResult = (((ADRESH * 256) + ADRESL) / 1023.0) * 500;
            type_ADC = 0;
        }
        PIR1bits.ADIF = 0;
    }     
    
    if(PIR0bits.INTF == 1){
        simulate_Warning();
        PIR0bits.INTF = 0;
    }
    
    if(PIR0bits.IOCIF == 1 && IOCAFbits.IOCAF4 == 1) {
        IOCAFbits.IOCAF4 = 0;
        PIR0bits.IOCIF = 0;
        usrTask_EmergencySwitch();
    }
    
    if(PIR0bits.TMR0IF == 1){
        TMR0H = 0x0B;
        TMR0L = 0xDC;
        PIR0bits.TMR0IF = 0;
        Incr_minTime();
    }
    
    if(PIR4bits.TMR1IF == 1){
        TMR1H = 0xEC;
        TMR1L = 0x78;
        PIR4bits.TMR1IF = 0;
        if(sevseg_Flag == 0){
            sevseg_LSB();
            sevseg_Flag++;
        }
        else if(sevseg_Flag == 1){
            sevseg_MSB();
            sevseg_Flag = 0;
        }
    }
    
     if(PIR4bits.TMR3IF == 1){
        TMR3H = 0xFC;
        TMR3L = 0x18;
        PIR4bits.TMR3IF = 0;
        toggle_CntReoxygenation++;
        toggle_CntMotor++;
        if(toggle_CntReoxygenation >= targetdelay_Reoxygenation){
            delayFlag_Reoxygenation = 1;
            toggle_CntReoxygenation = 0;
        }
        
        if(toggle_CntMotor >= targetdelay_Motor){
            delayFlag_Motor = 1;
            toggle_CntMotor = 0;
        }
    }
    
    if(PIR4bits.TMR5IF == 1){
        TMR5H = 0x0B;
        TMR5L = 0xDC;
        PIR4bits.TMR5IF = 0;
        incr_Speakercnt();
        onSpeaker();
        
    }
    
}
