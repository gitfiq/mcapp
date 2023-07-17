/*
 * File:   dspTask.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:25 PM
 */


#include <xc.h>
#include "config.h"

unsigned int adc_GetTemp(void);
unsigned int adc_GetOxy(void);
unsigned int get_pHcnt(void);

void seg_DspAll(unsigned int Tempcnt);
void lcd_DspAll(void);

void simulate_SetLabel(void);
void simulate_Reoxygenation(void);
void simulate_LowpH(void);
void simulate_HighpH(void);

unsigned int Tempcnt = 0;
unsigned int Oxycnt = 0;
unsigned int pHcnt = 0;

void dspTask_OnLCD(void){
    Oxycnt = adc_GetOxy();
    pHcnt = get_pHcnt();
    
    simulate_SetLabel();
    
    lcd_DspAll();
    
    if(Oxycnt < 5){
        simulate_Reoxygenation();
    }  
    if(pHcnt < 6){
        simulate_LowpH();
        pHcnt = 7;
    }
    else if(pHcnt > 8){
        simulate_HighpH();
        pHcnt = 7;
    }
  
}

void dspTask_OnSeg(void){
    Tempcnt = adc_GetTemp();
    
        seg_DspAll(Tempcnt);

}
