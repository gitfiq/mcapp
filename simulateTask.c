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


//Initialize labels
void simulate_SetLabel(void){
    char message1[] = "Oxygen:";
    char message2[] = "ppm";
    char message3[] = "pH:";
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
}

//Simulate Reoxygenation process
void simulate_Reoxygenation(void){
    int i;
    PORTBbits.RB2 = 1;
    for(i = 0; i < 3; i++){ 
        __delay_ms(150);
        PORTB = LATB >> 1;
    }
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 0;
    PORTBbits.RB2 = 0;
}

//on Speaker
void simulate_alarm(void){
    for (int a = 0; a <1000; a ++){
        SPKR = 1;
        __delay_us(200);
        SPKR = 0;
        __delay_us(200);
    }
}

//blink Red LED for warning of not suitable pH level and on speaker
void simulate_BlinkAlertLED(void){
    for(int i = 0; i <= 9; i++){
        simulate_alarm();
        PORTBbits.RB3 = ~LATBbits.LATB3;
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
    MOTOR = 1;
    simulate_BlinkAlertLED();
    lcdCtrl_ClearDisplay();
    MOTOR = 0;
}

//display on LCD high pH warning
void simulate_HighpH(void){
    char pH[] = "The pH is high";
    
    lcdCtrl_ClearDisplay();
    lcdCtrl_SetPos(2,1);
    for(int i = 0; pH[i] != 0; i++){
        lcdWriteDspData(pH[i]);
    }
    MOTOR = 1;
    simulate_BlinkAlertLED();
    lcdCtrl_ClearDisplay();
    MOTOR = 0;
}