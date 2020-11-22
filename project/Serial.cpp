/************************************************************/
/*********** Author    : Ahmed Mohamed Badra *************/
/*********** Date   : 24/9/2020       *************/
/*********** Version  : V02         *************/
/************************************************************/

/************************************************************/
/*              INCLUDES            */
/************************************************************/
// For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "macros.h"
#include "Serial.h"

/************************************************************/
/*            FUNCTION DEFINITIONS        */
/************************************************************/

/*
void STP::Init(void)
{
  digitalMode(STP_SERIAL_DATA, OUTPUT);
  digitalMode(STP_SHIFT_CLOCK, OUTPUT);
  digitalMode(STP_STORE_CLOCK, OUTPUT);
}
*/

void STP::Init(uint8_t data_pin, uint8_t shift_clock_pin, uint8_t store_clock_pin)
{
  _data = data_pin;
  _shift_clock = shift_clock_pin;
  _store_clock = store_clock_pin;
  
  pinMode(data_pin, OUTPUT);
  pinMode(shift_clock_pin, OUTPUT);
  pinMode(store_clock_pin, OUTPUT);
}
STP::STP(void)
{
}


void STP::SendSerialData(uint8_t DataToSend)
{
  int counter;
  uint8_t bitToSend;
  for (counter = 7; counter >=0; counter--)
  {
    /* Send bit by bit starting from Most significant bit MSB */
    bitToSend =  GET_BIT(DataToSend,counter);
    
    digitalWrite(_data, bitToSend);
    
    /* Send Pulse to Shift Clock */
    digitalWrite(_shift_clock, HIGH);
    delayMicroseconds(1);
    digitalWrite(_shift_clock, LOW);
    delayMicroseconds(1);
  }
  
  /* Send Pulse to Shift Clock */
  digitalWrite(_store_clock,HIGH);
  delayMicroseconds(1);
  digitalWrite(_store_clock,LOW);
  delayMicroseconds(1);
}
