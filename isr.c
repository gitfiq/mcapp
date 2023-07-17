/*
 * File:   isr.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:18 PM
 */


#include <xc.h>
#include "config.h"

void seg_DspAll(void);

extern unsigned int oxyResult;
extern unsigned int tempResult;
extern int type_ADC;
extern int sevseg_Flag;

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
    
    if(PIR4bits.TMR1IF == 1){
        if(sevseg_Flag % 2 == 0){
            sevseg_Flag++;
        }
        else{
            sevseg_Flag = 0;
        }
            
        PIR4bits.TMR1IF = 0;
    }
    
}
