/*
 * File:   lcd.c
 * Author: ahmad
 *
 * Created on 17 July, 2023, 4:16 PM
 */


#include <xc.h>
#include "config.h"

#define LCD_DATA_D4 LATDbits.LATD0
#define LCD_DATA_D5 LATDbits.LATD1
#define LCD_DATA_D6 LATDbits.LATD2
#define LCD_DATA_D7 LATDbits.LATD3
#define LCD_E LATDbits.LATD4
#define LCD_RS LATDbits.LATD5

void lcdWriteCtrlWord(char x);
void lcdWriteDspData(char x);
void lcdWriteNibble(char nibble);
void lcdCtrl_SetPos(unsigned char row, unsigned char col);

void lcdCtrl_FunctionSet(void);
void lcdCtrl_OnOffDisplay(char display_state, char cursor_state);
void lcdCtrl_SetEntryMode(void);
void lcdCtrl_ClearDisplay(void);

unsigned int adc_GetOxy(void);
unsigned int get_pHcnt(void);

void initLCD(void)
{
    __delay_ms(15); 
    lcdWriteCtrlWord(0b00000011); 
    __delay_ms(5);
    lcdWriteCtrlWord(0b00000010); 

    lcdCtrl_FunctionSet(); 
    lcdCtrl_OnOffDisplay(1, 0); 
    lcdCtrl_SetEntryMode(); 
    lcdCtrl_ClearDisplay(); 
}

void lcdWriteCtrlWord(char x)
{
    char upper_nibble, lower_nibble;
    
    upper_nibble = (x & 0b11110000) >> 4;
    lower_nibble = x & 0b00001111;
    
    LCD_RS = 0;
    lcdWriteNibble(upper_nibble); 
    lcdWriteNibble(lower_nibble); 
}

void lcdWriteDspData(char x)
{
    char upper_nibble, lower_nibble;
    upper_nibble = (x & 0b11110000) >> 4;
    lower_nibble = x & 0b00001111;
    
    LCD_RS = 1;
    lcdWriteNibble(upper_nibble); 
    lcdWriteNibble(lower_nibble); 
}

void lcdWriteNibble(char nibble)
{
    LCD_DATA_D7 = (nibble & 0b00001000) >> 3;
    LCD_DATA_D6 = (nibble & 0b00000100) >> 2;
    LCD_DATA_D5 = (nibble & 0b00000010) >> 1;
    LCD_DATA_D4 = (nibble & 0b00000001);
    LCD_E = 1;
    __delay_ms(1);
    LCD_E = 0;
    __delay_ms(1);
}

void lcdCtrl_SetPos(unsigned char row, unsigned char col)
{
    unsigned char ramAddr; 
    
    if (row == 1)
        ramAddr = col - 1; 
    else 
        ramAddr = 0x40 + col - 1; 
    
    lcdWriteCtrlWord(0x80 + ramAddr); 
}
    
void lcdCtrl_FunctionSet(void){
    lcdWriteCtrlWord(0b00101000); // Function Set - 4-bit, 2 lines, 5x7
}

void lcdCtrl_OnOffDisplay(char display_state, char cursor_state){
    char pattern = 0b00001000;
    if(display_state == 1)
        pattern |= 0b00000100; // Display on
    if(cursor_state == 1)
        pattern |= 0b00000011; // Cursor on and blinking
    
    lcdWriteCtrlWord(pattern);
}

void lcdCtrl_SetEntryMode(void){
    lcdWriteCtrlWord(0b00000110); // Entry mode - inc addr, no shift
}

void lcdCtrl_ClearDisplay(void){
    lcdWriteCtrlWord(0b00000001); // Clear display & home position
}

void lcd_DspAll(void){
    
    unsigned int Oxycnt, pHcnt = 0;
    unsigned char Oxydig0, Oxydig1, pHdig0, pHdig1;
    
    //Getting oxygen count
    Oxycnt  = adc_GetOxy();
    Oxydig0 = Oxycnt % 10;
    Oxydig1 = Oxycnt / 10 % 10;
        
    //Getting pH count
    pHcnt  = get_pHcnt();
    pHdig0 = pHcnt % 10;
    pHdig1 = pHcnt / 10 % 10;

    //Displaying oxygen count on LCD
    lcdCtrl_SetPos(1,9);
    lcdWriteDspData(Oxydig1 + 0x30);
    lcdWriteDspData(Oxydig0 + 0x30);
        
    //Displaying pH count on LCD
    lcdCtrl_SetPos(2,5);
    lcdWriteDspData(pHdig1 + 0x30);
    lcdWriteDspData(pHdig0 + 0x30);
        
}