/*
 * File:   usrTask.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:17 PM
 */


#define PB7 PORTDbits.RD6
#define PB8 PORTDbits.RD7
#define SW4 PORTAbits.RA4

void usrTask_CheckInputSW0(void);
void usrTask_CheckInputSW1(void);

//Stable pH level
unsigned int pHcnt = 7;
unsigned char emergencyState = 0;

//called in main
void usrTask(void){
    usrTask_CheckInputSW0();
    usrTask_CheckInputSW1();
}

//to check if the decrease pH push button is pressed
void usrTask_CheckInputSW0(void){
    if(PB7 == 0){
        if(PB7 == 0){
            while(PB7 == 0);
            if(pHcnt < 14)
                pHcnt++;
            else
                pHcnt = 14;
               
        }
    }
}

//to check if the increase pH push button is pressed
void usrTask_CheckInputSW1(void){
    if(PB8 == 0){
        if(PB8 == 0){
            while(PB8 == 0);
            if(pHcnt > 0)
                pHcnt--;
            else
                pHcnt = 0;
               
        }
    }
}

//To set the emergencyState depending on the switch. Allows the Reoxygenation and Clearing waste when switch Off. Vice verse
void usrTask_EmergencySwitch(void){
    if(SW4 == 0){
        emergencyState = 1;
    }
    else{
        emergencyState = 0;
    }
    
    PORTEbits.RE2 = emergencyState;
}

//returning the pH count
unsigned int get_pHcnt(void){
    return pHcnt;
}

//returning the emergencyState value
unsigned char get_emergencyState(void){
    return(emergencyState);
}

