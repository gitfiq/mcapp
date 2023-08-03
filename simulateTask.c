/*
 * File:   simulateTask.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:19 PM
 */


#include <xc.h>
#include "config.h"

#define ALARMLED LATBbits.LATB3
#define SPKR LATBbits.LATB4
#define MOTOR LATBbits.LATB5


//To write to the LCD display (set labels)
void lcdWriteDspData(char x);
void lcdCtrl_SetPos(unsigned char row, unsigned char col);
void lcdCtrl_ClearDisplay(void);

unsigned char get_emergencyState(void);

void delay_Reoxygenation(unsigned int milisec);     //delay for the Reoxygentaion using timer 3 
void delay_pHDsp(unsigned int milisec);             // delay for the pH display warning using timer 5

extern int alarm_cnt;                               //alarm_cnt from timer.c file (to track the current speaker count)
extern int motor_cnt;                               //motor_cnt from timer.c file (to track the current motor count )

extern unsigned int hrTime;                         //hrTime from timer.c file (to track the current hr time)

extern unsigned int pHcnt;                          //pHcnt from the usrTask.c file (to track the current pH level)

char warnFlag = 0;                                  //Store the warning flag(when equal to 0 warn wont go off, vice versa)

//Initialize labels
void simulate_SetLabel(void){
    char message1[] = "Oxygen:";
    char message2[] = "ppm";
    char message3[] = "pH:";
    char message4[] = "Hr:";
    unsigned int i;
    
    //Setting up the labels
    lcdCtrl_SetPos(1,1);
    for(i = 0; message1[i] != 0; i++){
        lcdWriteDspData(message1[i]);
    }
    lcdCtrl_SetPos(1,12);
    for(i = 0; message2[i] != 0; i++){
        lcdWriteDspData(message2[i]);
    }
    lcdCtrl_SetPos(2,1);
    for(i = 0; message3[i] != 0; i++){
        lcdWriteDspData(message3[i]);
    }
    lcdCtrl_SetPos(2,9);
    for(i = 0; message4[i] != 0; i++){
        lcdWriteDspData(message4[i]);
    }
}

//Simulate Reoxygenation process
void simulate_Reoxygenation(void){
    PORTBbits.RB2 = 1;
    for(int i = 0; i < 3; i++){ 
        delay_Reoxygenation(150);               //delay from the timer3
        PORTB = LATB >> 1;
    }
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 0;
    PORTBbits.RB2 = 0;
}

//on warning functions (called when external interrupt occurs)
void simulate_OnWarning(void){
    T2CONbits.ON = 1;                           //On the timer 3
    CCP1CONbits.EN = 1;                         //Enable the CCP1
    alarm_cnt = 0;                              //Set the alarm count to 0 everytime this function is call
    warnFlag = 1;                               //Set the warn flag to 0 everytime this function is call
}

//off warning function (called when IOC for push buttons occurs)
void simulate_OffWarning(void){
    warnFlag = 0;
}

//Time critical task( will be called in the isr - Timer 2 )(if warn flag = 1)
void offWarning(void){
    SPKR = 0;
    ALARMLED = 0;                               //Will off the warning system
}

//Time critical task( will be called in the isr - Timer 2 [to on Speaker])(if warn flag = 0)
void onSpeaker(void){
    if(alarm_cnt > 1000){
        SPKR = 0;
    }
    else{
        SPKR = 1;
    }
}

//Time critical task( will be called in the isr - Timer 2 [to blink Red LED])(if warn flag = 0)
void onAlertLED(void){
    ALARMLED = 1;
    if(alarm_cnt > 1000){
        ALARMLED = 0;
    }
    else{
        if((alarm_cnt/10) % 2 == 0)
        ALARMLED = ~ALARMLED;
    }
}

//on Motor, will be called in functions that needs the motor eg.scheduled_Motor())
void simulate_Motor(void){
    T4CONbits.ON = 1;
    CCP2CONbits.EN = 1;
    motor_cnt = 0;
}

//Time critical task( will be called in the isr - Timer 4 [to on the motor for 3 seconds])
void onMotor(void){
    if(motor_cnt > 100){
        MOTOR = 0;
    }
    else{
        MOTOR = 1;
    }
}
// call in the main to Continuously call for motor to on at equal periods
void scheduled_Motor(void){
    if(hrTime == 5 && get_emergencyState() == 0){                    // if Hr time is equal to 5 hrs then the scheduled motor will run
        simulate_Motor();
        hrTime = 0;
        pHcnt = 8;
    }
}

//display on LCD low pH warning
void simulate_LowpH(void){
    char pH[] = "The pH is Low";
    
    simulate_Motor();                   // on the motor to clear waste in the fish tank
    lcdCtrl_ClearDisplay();
    lcdCtrl_SetPos(2,1);
    for(int i = 0; pH[i] != 0; i++){
        lcdWriteDspData(pH[i]);
    }
    delay_pHDsp(2700);                  // delay from timer 5 to match the timing of the motor
    lcdCtrl_ClearDisplay();
}

//display on LCD high pH warning
void simulate_HighpH(void){
    char pH[] = "The pH is High";
    
    simulate_Motor();                   // on the motor to clear waste in the fish tank
    lcdCtrl_ClearDisplay();
    lcdCtrl_SetPos(2,1);
    for(int i = 0; pH[i] != 0; i++){
        lcdWriteDspData(pH[i]);
    }
    delay_pHDsp(2700);                   // delay from timer 5 to match the timing of the motor
    lcdCtrl_ClearDisplay();
}
