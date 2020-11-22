 /******************************************************************************
 *
 * Module: SerialLCD
 *
 * File Name: lcd.cpp
 *
 * Description: Source file for the LCD driver
 *
 * Author: Ahmed badra
 *
 *******************************************************************************/
// For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "macros.h"
#include "Serial.h"
#include "lcd.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

LCD::LCD(uint8_t data_pin, uint8_t shift_clock_pin, uint8_t store_clock_pin)
{
  lcd_port = 0x00000000;
  
  stp_lcd.Init(data_pin, shift_clock_pin, store_clock_pin);
  
  sendLcdCommand(FOUR_BITS_DATA_MODE); /* initialize LCD in 4-bit mode */
  sendLcdCommand(TWO_LINE_LCD_Four_BIT_MODE); /* use 2-line lcd + 4-bit Data Mode + 5*7 dot display Mode */
  
  sendLcdCommand(CURSOR_OFF); /* cursor off */
  
  sendLcdCommand(CLEAR_COMMAND); /* clear LCD at the beginning */
}

void LCD::displayCharacter(uint8_t portdata)
{
  SET_BIT(lcd_port,RS); /* Instruction Mode RS=0 */
  CLEAR_BIT(lcd_port,RW); /* write data to LCD so RW=0 */
  
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);
  
  delay(1); /* delay for processing Tas = 50ns */
  
  SET_BIT(lcd_port,E); /* Enable LCD E=1 */
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);

  delay(1); /* delay for processing Tpw - Tdws = 190ns */

  lcd_port = (lcd_port & 0xF0) | ((portdata & 0xF0) >> 4);
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);

  delay(1); /* delay for processing Tdsw = 100ns */
  
  CLEAR_BIT(lcd_port,E); /* disable LCD E=0 */
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);

  delay(1); /* delay for processing Th = 13ns */

  SET_BIT(lcd_port,E); /* Enable LCD E=1 */
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);

  delay(1); /* delay for processing Tpw - Tdws = 190ns */

  lcd_port = (lcd_port & 0xF0) | (portdata & 0x0F);
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);

  delay(1); /* delay for processing Tdsw = 100ns */

  CLEAR_BIT(lcd_port,E); /* disable LCD E=0 */
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);

  delay(1); /* delay for processing Th = 13ns */
}

void LCD::sendLcdCommand(uint8_t portdata)
{ 
  CLEAR_BIT(lcd_port,RS); /* Instruction Mode RS=0 */
  CLEAR_BIT(lcd_port,RW); /* write data to LCD so RW=0 */
  
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);
  
  delay(1); /* delay for processing Tas = 50ns */
  
  SET_BIT(lcd_port,E); /* Enable LCD E=1 */
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);

  delay(1); /* delay for processing Tpw - Tdws = 190ns */

  lcd_port = (lcd_port & 0xF0) | ((portdata & 0xF0) >> 4);
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);

  delay(1); /* delay for processing Tdsw = 100ns */
  
  CLEAR_BIT(lcd_port,E); /* disable LCD E=0 */
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);

  delay(1); /* delay for processing Th = 13ns */

  SET_BIT(lcd_port,E); /* Enable LCD E=1 */
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);

  delay(1); /* delay for processing Tpw - Tdws = 190ns */

  lcd_port = (lcd_port & 0xF0) | (portdata & 0x0F);
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);

  delay(1); /* delay for processing Tdsw = 100ns */

  CLEAR_BIT(lcd_port,E); /* disable LCD E=0 */
  /* send this port value serially */
  stp_lcd.SendSerialData(lcd_port);

  delay(1); /* delay for processing Th = 13ns */
}

void LCD::displayString(const char *Str)
{
  uint8_t i = 0;
  while(Str[i] != '\0')
  {
    displayCharacter(Str[i]);
    i++;
  }
  /***************** Another Method ***********************
  while((*Str) != '\0')
  {
    LCD_displayCharacter(*Str);
    Str++;
  }   
  *********************************************************/
}

void LCD::goToRowColumn(uint8_t row,uint8_t col)
{
  uint8_t Address;
  
  /* first of all calculate the required address */
  switch(row)
  {
    case 0:
        Address=col;
        break;
    case 1:
        Address=col+0x40;
        break;
    case 2:
        Address=col+0x10;
        break;
    case 3:
        Address=col+0x50;
        break;
  }         
  /* to write to a specific address in the LCD 
   * we need to apply the corresponding command 0b10000000+Address */
  sendLcdCommand(Address | SET_CURSOR_LOCATION); 
}

void LCD::displayStringRowColumn(uint8_t row,uint8_t col,const char *Str)
{
  goToRowColumn(row,col); /* go to to the required LCD position */
  displayString(Str); /* display the string */
}

void LCD::intgerToString(int portdata)
{
   char buff[16]; /* String to hold the ascii result */
   itoa(portdata,buff,10); /* 10 for decimal */
   displayString(buff);
}

void LCD::clearScreen(void)
{
  sendLcdCommand(CLEAR_COMMAND); //clear display 
}
