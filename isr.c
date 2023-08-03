/*
 * File:   isr.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:18 PM
 */


#include <xc.h>
#include "config.h"

void usrTask_EmergencySwitch(void);     //Emergency switch activated through the IOC. To stop any Reoxygenation and changing water due to pH

void sevseg_LSB(void);                  //display the MSB and LSB display functions from sev seg file.
void sevseg_MSB(void);
int sevseg_Flag = 0;                    //sevseg_Flag is used to toggle between the display of MSB and LSB seven segment

extern unsigned int oxyResult;          //oxyResult and tempResult to contain the conversion results
extern unsigned int tempResult;
extern int type_ADC;                    //type_ADC is used to toggle the ADC conversion between the potentiometer and LM35

extern unsigned int toggle_CntReoxygenation;        //description at timer.c file
extern unsigned int delayFlag_Reoxygenation;
extern unsigned int targetdelay_Reoxygenation;

extern unsigned int toggle_CntpHDsp;                //description at timer.c file
extern unsigned int delayFlag_pHDsp;
extern unsigned int targetdelay_pHDsp;

void incr_MinHrTime(void);                //increase min and Hr time accordingly(from timer.c file)

void incr_Motorcnt(void);               //increase motor count
void onMotor(void);                     //on the motor (from simulateTask.c file)
extern int motor_cnt;

void incr_Alarmcnt(void);             //increase alarm count
void simulate_OnWarning(void);        //simulate warnings are from simulateTask.c file
void simulate_OffWarning(void);
void onSpeaker(void);                 //on the speaker
void onAlertLED(void);                //blink red led
void offWarning(void);                //off all warning functions
extern char warnFlag;
extern int alarm_cnt;

void __interrupt() isr(void){
    
    //ADC interrupt (Convert and proportionalize the values)
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
    
    //External Interrupt (To start the alarm for emergency)
    if(PIR0bits.INTF == 1){
        simulate_OnWarning();
        PIR0bits.INTF = 0;
    }
    
    //Interrupt-On-Change (Push Buttons) (To stop the alarm after emergency)
    if(PIR0bits.IOCIF == 1 && IOCAFbits.IOCAF3 == 1) {
        IOCAFbits.IOCAF3 = 0;
        PIR0bits.IOCIF = 0;
        simulate_OffWarning();
    }
    
    //Interrupt-On-Change (Switch) (To turn off the other functions for maintenance)
    if(PIR0bits.IOCIF == 1 && IOCAFbits.IOCAF4 == 1) {
        IOCAFbits.IOCAF4 = 0;
        PIR0bits.IOCIF = 0;
        usrTask_EmergencySwitch();
    }
    
    //Timer 0 interrupt (Timer for the schedule motor)
    if(PIR0bits.TMR0IF == 1){
        TMR0H = 0x0B;
        TMR0L = 0xDC;   //set to 0.5 s
        PIR0bits.TMR0IF = 0;
        incr_MinHrTime();
    }
    
    //Timer 1 interrupt (Timer for the displaying of seven segment)
    if(PIR4bits.TMR1IF == 1){
        TMR1H = 0xEC;
        TMR1L = 0x78;   //set to 5 ms
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
    
    //Timer 2 interrupt [CCP1] (Timer for alarm system)
    if(PIR4bits.TMR2IF == 1){
        T2PR = 0x7C; // set to 15 ms
        PIR4bits.TMR2IF = 0 ;
        incr_Alarmcnt();
        if(warnFlag == 1){
            onSpeaker();
            onAlertLED();
        }
        else{
            alarm_cnt = 1001;
            offWarning();
        }
    }
    
    //Timer 3 interrupt (Timer for Reoxygenation process)
    if(PIR4bits.TMR3IF == 1){
        TMR3H = 0xFC;
        TMR3L = 0x18; //set to 1 ms
        PIR4bits.TMR3IF = 0;
        toggle_CntReoxygenation++;
        if(toggle_CntReoxygenation >= targetdelay_Reoxygenation){
            delayFlag_Reoxygenation = 1;
            toggle_CntReoxygenation = 0;
        }
    }
    
    //Timer 4 interrupt [CCP2] (Timer for turning on the motor)
    if(PIR4bits.TMR4IF == 1){
        T4PR = 0xF9; // set to 30 ms
        PIR4bits.TMR4IF = 0 ;
        incr_Motorcnt();
        onMotor();
    }
    
    //Timer 5 interrupt (Timer for the delay of pH warning display)
    if(PIR4bits.TMR5IF == 1){
        TMR5H = 0xFC;
        TMR5L = 0x18; //set to 1 ms
        PIR4bits.TMR5IF = 0;
        toggle_CntpHDsp++;
        if(toggle_CntpHDsp >= targetdelay_pHDsp){
            delayFlag_pHDsp = 1;
            toggle_CntpHDsp = 0;
        }
     }
}
