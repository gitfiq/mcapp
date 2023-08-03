/*
 * File:   dspTask.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:25 PM
 */


#include <xc.h>
#include "config.h"

unsigned int adc_GetTemp(void);                 //Getting the values of Oxygen and Temperature from ADC
unsigned int adc_GetOxy(void);
unsigned int get_pHcnt(void);                   //Getting the value of pH from polling

void getTemp_count(unsigned int gettempcnt);    //Passing the converted temperature value to the sev_seg.c file
void lcd_DspAll(void);                          //Displaying on the LCD 


void simulate_SetLabel(void);                   //Set default labels
void simulate_Reoxygenation(void);              //Function run when oxygen level is Low
void simulate_LowpH(void);                      //Function run when pH level is Low
void simulate_HighpH(void);                     //Function run when pH level is High
unsigned char get_emergencyState(void);

//Storing the converted and polling values in local variables
unsigned int Tempcnt = 0;                 
unsigned int Oxycnt = 0;
unsigned int pHcnt = 0;

//Called in the main to display on LCD
void dspTask_OnLCD(void){
    Oxycnt = adc_GetOxy();
    pHcnt = get_pHcnt();
    
    simulate_SetLabel();
    
    lcd_DspAll();
    if(get_emergencyState() == 0){             //Emergency state from the usrTask( to check if its on or off))
        if(Oxycnt < 5){                        //When the oxygen level is low 
            simulate_Reoxygenation();
        }  
        if(pHcnt < 7){                         //When the pH level is low
            simulate_LowpH();
            pHcnt = 8;                         //Set back to optimum pH
        }
        else if(pHcnt > 9){                    //When the pH level is high
            simulate_HighpH();
            pHcnt = 8;                         //Set back to optimum pH
        }
    }
  
}

//Call in the main to pass converted temperature value to the seven segment to display
void dspTask_OnSeg(void){
    Tempcnt = adc_GetTemp();
    
    getTemp_count(Tempcnt);
}
