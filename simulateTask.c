/*
 * File:   simulateTask.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:19 PM
 */


#include <xc.h>
#include "config.h"

#define SPKR LATBbits.LATB4
#define MOTOR LATBbits.LATB5

void lcdWriteDspData(char x);
void lcdCtrl_SetPos(unsigned char row, unsigned char col);
void lcdCtrl_ClearDisplay(void);

void delay_Reoxygenation(unsigned int milisec);
void delay_Motor(unsigned int milisec);

extern unsigned int hrTime;

extern int speaker_cnt;

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
        delay_Reoxygenation(150);
        PORTB = LATB >> 1;
    }
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 0;
    PORTBbits.RB2 = 0;
}

//on Speaker
void simulate_Alarm(void){
//    for (int a = 0; a < 1000; a ++){
//        SPKR = 1;
//        __delay_us(500);
//        SPKR = 0;
//        __delay_us(500);
//    }
    T5CONbits.ON = 1;
    CCP1CONbits.EN = 1;
}

void onSpeaker(void){
    SPKR = 1;
    if(speaker_cnt == 8){
        T5CONbits.ON = 0;
        CCP1CONbits.EN = 0;
        SPKR = 0;
    }
}

//on Motor
void simulate_Motor(void){
    MOTOR = 1;
    delay_Motor(3000);
    MOTOR = 0;
}

//blink Red LED for warning of not suitable pH level and on speaker
void simulate_BlinkAlertLED(void){
    for(int i = 0; i < 10; i++){
        PORTBbits.RB3 = ~PORTBbits.RB3;
        //simulate_Alarm();
    }
}

//display on LCD low pH warning
void simulate_LowpH(void){
    char pH[] = "The pH is low";
    
    lcdCtrl_ClearDisplay();
    lcdCtrl_SetPos(2,1);
    for(int i = 0; pH[i] != 0; i++){
        lcdWriteDspData(pH[i]);
    }
    simulate_Motor();
    lcdCtrl_ClearDisplay();
}

//display on LCD high pH warning
void simulate_HighpH(void){
    char pH[] = "The pH is high";
    
    lcdCtrl_ClearDisplay();
    lcdCtrl_SetPos(2,1);
    for(int i = 0; pH[i] != 0; i++){
        lcdWriteDspData(pH[i]);
    }
    simulate_Motor();
    lcdCtrl_ClearDisplay();
}

void scheduled_Motor(void){
    if(hrTime == 5){
        simulate_Motor();
        hrTime = 0;
    }
}

void simulate_Warning(void){
    //simulate_BlinkAlertLED();
    simulate_Alarm();
}
