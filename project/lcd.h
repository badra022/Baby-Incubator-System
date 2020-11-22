 /******************************************************************************
 *
 * Module: SerialLCD
 *
 * File Name: lcd.h
 *
 * Description: Header file for the LCD driver
 *
 * Author: Ahmed Badra
 *
 *******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
/* LCD HW Pins 
  Q0 -> d0
  Q1 -> d1
  Q2 -> d2
  Q3 -> d3
  Q4 -> rs
  Q5 -> enable
  Q6 -> rw not important
*/

#include <inttypes.h>

#define RS Q4
#define RW Q6
#define E  Q5




class LCD{
  public:
  LCD(uint8_t data_pin, uint8_t shift_clock_pin, uint8_t store_clock_pin);
  void displayCharacter(uint8_t portdata);
  void sendLcdCommand(uint8_t portdata);
  void displayString(const char *Str);
  void goToRowColumn(uint8_t row,uint8_t col);
  void displayStringRowColumn(uint8_t row,uint8_t col,const char *Str);
  void intgerToString(int portdata);
  void clearScreen(void);
  private:
  uint8_t lcd_port; 
  STP stp_lcd;
};


/* LCD Commands */
#define CLEAR_COMMAND 0x01
#define FOUR_BITS_DATA_MODE 0x02
#define TWO_LINE_LCD_Four_BIT_MODE 0x28
#define TWO_LINE_LCD_Eight_BIT_MODE 0x38
#define CURSOR_OFF 0x0C
#define CURSOR_ON 0x0E
#define SET_CURSOR_LOCATION 0x80 


#endif /* LCD_H_ */
