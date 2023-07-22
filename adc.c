/*
 * File:   adc.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:10 PM
 */


#include <xc.h>
#include "config.h"

unsigned int oxyResult = 0;
unsigned int tempResult = 0;
int type_ADC = 0;

void initADC(void){
    ADREF = 0b00000000;
    ADCLK = 0b00000011;
    ADACQ = 0b00000000;
    ADCON0 = 0b10000100;
    INTCONbits.GIE = 0;
    INTCONbits.PEIE = 1;
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.GIE = 1;
}

void adc_GetConversion(void){
    if(type_ADC % 2 == 0){
        ADPCH = 0b00000001;
    }
    else{
        ADPCH = 0b00000000; 
   }
    __delay_ms(5); 
    ADCON0bits.ADGO = 1;
        
}

unsigned int adc_GetOxy(void){
    return(oxyResult);
}

unsigned int adc_GetTemp(void){
    return(tempResult);
}
