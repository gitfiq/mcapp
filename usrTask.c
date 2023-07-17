/*
 * File:   usrTask.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:17 PM
 */


#include <xc.h>
#include "config.h"

#define SW0 PORTAbits.RA3
#define SW1 PORTAbits.RA4

void usrTask_CheckInputSW0(void);
void usrTask_CheckInputSW1(void);

unsigned pHcnt = 7;

//called in main
void usrTask(void){
    usrTask_CheckInputSW0();
    usrTask_CheckInputSW1();
}

//to check if the decrease pH push button is pressed
void usrTask_CheckInputSW0(void){
    if(SW0 == 0){
        if(SW0 == 0){
            while(SW0 == 0);
            if(pHcnt < 14)
                pHcnt++;
            else
                pHcnt = 14;
               
        }
    }
}

//to check if the increase pH push button is pressed
void usrTask_CheckInputSW1(void){
    if(SW1 == 0){
        if(SW1 == 0){
            while(SW1 == 0);
            if(pHcnt > 0)
                pHcnt--;
            else
                pHcnt = 0;
               
        }
    }
}

//returning the pH count
unsigned int get_pHcnt(void){
    return pHcnt;
}
