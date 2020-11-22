/************************************************************/
/*********** Author    : Ahmed Mohamed Badra *************/
/*********** Date   : 24/9/2020       *************/
/*********** Version  : V02         *************/
/************************************************************/
#ifndef STP_H_
#define STP_H_

#include <inttypes.h>

class STP{
  public:
  STP(void);
  void Init(uint8_t data_pin, uint8_t shift_clock_pin, uint8_t store_clock_pin);
  void SendSerialData(uint8_t DataToSend);
  private:
  uint8_t _data;
  uint8_t _shift_clock;
  uint8_t _store_clock;
};


#endif
