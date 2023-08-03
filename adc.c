/*
 * File:   adc.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:10 PM
 */


#include <xc.h>
#include "config.h"

//Store the converted Oxygen value 
unsigned int oxyResult = 0;

//Store the converted Temperature value
unsigned int tempResult = 0;

// Count to toggle between the temperature and oxygen ADC. (Only 1 ADC)
int type_ADC = 0;

void initADC(void){
    ADREF = 0b00000000;
    ADCLK = 0b00000011;
    ADACQ = 0b00000000; // Aquisition time not set
    ADCON0 = 0b10000100;
    INTCONbits.GIE = 0;
    INTCONbits.PEIE = 1;
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.GIE = 1;
}

// Run in the main to start the conversion and get the vale through an ADC interrupt
void adc_GetConversion(void){
    if(type_ADC % 2 == 0){
        ADPCH = 0b00000001;     //(RA1  - Potentiometer)
    }
    else{
        ADPCH = 0b00000000;    //(RA0 - LM35)
   }
    __delay_ms(5); //waiting time set to 5 ms
    ADCON0bits.ADGO = 1;
        
}

//Returning Oxygen count
unsigned int adc_GetOxy(void){
    return(oxyResult);
}

//Returning Temperature count
unsigned int adc_GetTemp(void){
    return(tempResult);
}
