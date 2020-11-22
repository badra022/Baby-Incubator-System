/* init the bar serial pin to parallel port instance */
STP AlarmBar;

/* set the initial value of the parallel port */
uint8_t AlarmBar_PORT = 0x00000000;

/* led connction pins in the parallel port (output of the IC Q0, Q1,...) */
#define TEMPERATURE_LED     Q1
#define NOISE_LED           Q2
#define HUMIDITY_LED        Q3
#define DOOR_LED            Q4
#define SYSTEM_BUZZER       Q5
#define FAN                 Q6
#define HEATER              Q7

void InitAlarmLeds(void)
{
  /* set the 3 pins of the serial to parallel Ic connection */
  AlarmBar.Init(A2, A3 ,A4); //data | shift_clock | store_clock
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);
}

void LaunchTempAlarm(void)
{
  /* set/clear alarm pin in the parallel port */
  SET_BIT(AlarmBar_PORT, TEMPERATURE_LED);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);
}
void StopTempAlarm(void)
{
  /* set/clear alarm pin in the parallel port */
  CLEAR_BIT(AlarmBar_PORT, TEMPERATURE_LED);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);
}
void LaunchHumidityAlarm(void)
{
  /* set/clear alarm pin in the parallel port */
  SET_BIT(AlarmBar_PORT, HUMIDITY_LED);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);
}
void StopHumidityAlarm(void)
{
  /* set/clear alarm pin in the parallel port */
  CLEAR_BIT(AlarmBar_PORT, HUMIDITY_LED);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);
}
void LaunchSoundAlarm(void)
{
  /* set/clear alarm pin in the parallel port */
  SET_BIT(AlarmBar_PORT, NOISE_LED);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);
}
void StopSoundAlarm(void)
{
  /* set/clear alarm pin in the parallel port */
  CLEAR_BIT(AlarmBar_PORT, NOISE_LED);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);
}
void LaunchDoorAlarm(void)
{
  /* set/clear alarm pin in the parallel port */
  SET_BIT(AlarmBar_PORT, DOOR_LED);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);
}
void StopDoorAlarm(void)
{
  /* set/clear alarm pin in the parallel port */
  CLEAR_BIT(AlarmBar_PORT, DOOR_LED);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);  
}
void LaunchSystemAlarm(void)
{
  /* set/clear alarm pin in the parallel port */
  SET_BIT(AlarmBar_PORT, SYSTEM_BUZZER);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);
}
void StopSystemAlarm(void)
{
  /* set/clear alarm pin in the parallel port */
  CLEAR_BIT(AlarmBar_PORT, SYSTEM_BUZZER);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);
}
void StartFan(void)
{
  /* set/clear alarm pin in the parallel port */
  SET_BIT(AlarmBar_PORT, FAN);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);
}
void StopFan(void)
{
  /* set/clear alarm pin in the parallel port */
  CLEAR_BIT(AlarmBar_PORT, FAN);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);  
}
void StartHeater(void)
{
  /* set/clear alarm pin in the parallel port */
  SET_BIT(AlarmBar_PORT, HEATER);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);  
}
void StopHeater(void)
{
  /* set/clear alarm pin in the parallel port */
  CLEAR_BIT(AlarmBar_PORT, HEATER);
  
  /* transmit this port value serially to leds */
  AlarmBar.SendSerialData(AlarmBar_PORT);  
}
