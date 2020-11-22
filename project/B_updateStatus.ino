/* sensors Input pins */
/* analog */
char TemperatureSensor = A1;  // for the baby
char SoundSensor = A0; 
/* digital I\O */
char HumiditySensor = 12; // with the temperature of the Air
char DoorButton = 3;
char SleepButton = 13;
char MuteButton = 2; // the pin which is connected to interrupt 0, 1
char ViewButton = A5;

/* global system control variables */
bool SystemAlarmsFlag = true;
uint8_t SystemAlarmState = 0;
bool DoorIsClosed = true;

/* measure values for diffirent parameters (to be shown in serial monitor and wireless terminal)*/
double TemperatureAir = 0;
double TemperatureBaby = 0;
double Humidity = 0;
double Noise = 0;

/* initialize the library by associating any needed LCD interface pin
 * with the arduino pin number it is connected to */
const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/* initiating wireless bluetooth Terminal */
SoftwareSerial bluetoothTerminal(11, 10); // RX | TX

/* initiating DHT11 sensor instance to take readings */
DHT dht(HumiditySensor, DHT11);

void InitSystemDirections(void)
{
  /* setting the directions of other input pins */
  pinMode(MuteButton, INPUT_PULLUP);
  pinMode(SleepButton, INPUT_PULLUP);
  pinMode(HumiditySensor, INPUT); // pullup resistance is set in the circuit
  pinMode(DoorButton, INPUT_PULLUP);
  pinMode(ViewButton, INPUT_PULLUP);
  
  /* set system's external interrupts 0 to mute system ISR in pin 2 */
  attachInterrupt(0, INT_MuteSystemAlarm, LOW);
  
}

void INT_MuteSystemAlarm(void)
{
  /* this ISR interrupts the thread and close the system buzzer alarm and disable it for future alarms */
  StopSystemAlarm();
  SystemAlarmsFlag = !SystemAlarmsFlag;
}

void INT_WakeUpSystem(void){
  /* this function interrupts the system and sleep the cpu for power saving */
  
  /* notify the user that the system will wakeUp */
  Serial.println("WokeUp!");

  /* disable sleep mode */
  sleep_disable();

  /* detach previous interrupt 1 for the wakeUp system ISR */
  detachInterrupt(1);
  
  /* set system's external interrupt 1 to mute system ISR in pin 2 */
  attachInterrupt(0, INT_MuteSystemAlarm, LOW); // pin2 now will work as mute button for the system buzzer alarm
}

/* system Monitoring APIs */
void updateTemperatureMonitor(void*)
{
  /* this function 1.reads the measurement of temperature
   *               2.store the measurement value in Temperaturebaby global variable
   *               3.print the Temperature state in the system status moniton (LCD)
   */
  TemperatureBaby = analogRead(TemperatureSensor);
  TemperatureBaby = ( TemperatureBaby/1024.0)*5000;
  TemperatureBaby = TemperatureBaby/10;
  if(TemperatureBaby < 37.0)
  {
    LaunchTempAlarm();
    StartHeater();
    StopFan();
  }
  else if(TemperatureBaby > 39.0)
  {
    LaunchTempAlarm();
    StartFan();
    StopHeater();
  }
  else
  {
    StopTempAlarm();
    StopHeater();
    StopFan();
  }  
}
void updateHumidityMonitor(void*)
{
  /* this function 1.reads the measurement of humidity and temperature
   *               2.store the measurement temperature value in TemperatureAir global variable
   *               3.store the measurement humidity value in Humidity global variable
   *               3.print the Humidity state in the system status moniton (LCD)
   */
  static uint16_t Hcount = 0;
  static uint16_t Tcount = 0;  
  Humidity = dht.readHumidity();
  TemperatureAir = dht.readTemperature();
  if (isnan(TemperatureAir) || isnan(Humidity)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
    if(TemperatureAir < 36.0)
  {
    LaunchTempAlarm();
    StartHeater();
    StopFan();
    Tcount++;
  }
  else if(TemperatureBaby > 38.0)
  {
    LaunchTempAlarm();
    StartFan();
    StopHeater();
    Tcount++;
  }
  else
  {
    StopTempAlarm();
    StopHeater();
    StopFan();
    SystemAlarmState--;
    Tcount++;
  }  
  if(Humidity < 40)
  {
    LaunchHumidityAlarm();
    Hcount++;
  }
  else if(Humidity > 70)
  {
    LaunchHumidityAlarm();
    Hcount++;
  }
  else
  {
    StopHumidityAlarm();
    SystemAlarmState--;
    Hcount = 0;
  }
   if(Hcount == 6)
   {
    SystemAlarmState++;
   }
   if(Tcount == 6)
   {
    SystemAlarmState++;
   }
}
void updateSoundMonitor(void*)
{
  /* this function 1.reads the measurement of Noise in dBs
   *               2.store the measurement sound value in Noise global variable
   *               3.print the Noise state in the system status moniton (LCD)
   */   
   static uint16_t Scount = 0;
   uint16_t Temp = analogRead(SoundSensor);
   Noise = (Temp+83.2073) / 11.003;
   if(Noise > 100)
   {
      LaunchSoundAlarm();
      Scount++;
   }
   else if(Noise > 90)
   {
      LaunchSoundAlarm();
      SystemAlarmState--;
      Scount = 0;   
   }
   else
   {
      StopSoundAlarm();
      SystemAlarmState--;
      Scount = 0;    
   }
   if(Scount == 6)
   {
    SystemAlarmState++;
   }
}
void updateDoorMonitor(void*)
{
  /* this function 1.reads the state of the door (open or closed)
   *               2.if door state is open, led alarm will be launched
   *               3.if open state didn't change for 10 seconds, increase the SystemAlarmState global variable 
   *               4.if door state is closed, stop led alarm and decrease the SystemAlarmState global variable
   */
  static uint16_t Dcount = 0;
  uint8_t value = digitalRead(DoorButton);
  if(value == LOW)
  {
    DoorIsClosed = !DoorIsClosed;
    while(digitalRead(DoorButton) == LOW){}

  }
  if(DoorIsClosed)
  {
    SystemAlarmState--;
    StopDoorAlarm();
    Dcount = 0;
  }
  else
  {
    LaunchDoorAlarm();
    Dcount++;
  }
  /* if the door kept open for some time, launch the sysem buzzer alarm */
  if(Dcount == 60)
  {
    SystemAlarmState++;
  }
}

void updateSystemStatus(void*)
{
  /* this function launchs the system buzzer alarm based on the level 
   *  of SystemAlarmState(HOW DANGER IT IS) global variable */
  
  if(SystemAlarmState > 3 && SystemAlarmsFlag == true)
  {
    LaunchSystemAlarm();
  }
  else
  {
    StopSystemAlarm();
  }
}

void updateSystemMeasurements(void*)
{
  /* this function checks if the user pressed the viewButton or not 
   *  to view the measurements on the terminal */
    if(digitalRead(ViewButton) == LOW)
    {
      Serial.println("hello");
      /* display measurements */
      bluetoothTerminal.print("Temperature of Air = ");
      bluetoothTerminal.print(TemperatureAir);
      bluetoothTerminal.println(" C");
      bluetoothTerminal.print("Temperature of Baby = ");
      bluetoothTerminal.print(TemperatureBaby);
      bluetoothTerminal.println(" C");
      bluetoothTerminal.print("Humidity = ");
      bluetoothTerminal.print(Humidity);
      bluetoothTerminal.println(" %");
      bluetoothTerminal.print("Noise level = ");
      bluetoothTerminal.print(Noise);
      bluetoothTerminal.println(" dB");
      while(digitalRead(ViewButton) == LOW){}
    }
}

void updateSleepState(void*)
{
  if(!digitalRead(SleepButton))
  {
    /* clear the monitor and go to sleep, wait for the wakeup interrupt in pin 2 interrupt */
    lcd.clear();
    lcd.setCursor(0, 0);
    /* viewing sleeping status for 2 seconds */
    lcd.print("Sleeping..!");

    /* enable sleep mode, not starting it! */
    sleep_enable();

    /* detach previous interrupt 0 for the mute system ISR */
    detachInterrupt(0);
    
    /* set system's external interrupts 1 to wakeUp system ISR in pin 2 */
    pinMode(MuteButton, INPUT_PULLUP);
    attachInterrupt(0, INT_WakeUpSystem, LOW); // pin2 now will work as wakeUp button for the system

    /* setting sleep mode as full sleep (power down) */
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    delay(500);
    
    /* go to bed and sleep now */
    sleep_cpu();

    /* when interrupted, notify the user in the Lcd */
    delay(500);
    
  }
  else{
    /* pass */
  }
}

void updateSystemStatusMonitor(void*)
{
  if(TemperatureAir > 39 || TemperatureBaby > 38)
  {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("T: High");
  }
  else if(TemperatureAir < 37 || TemperatureBaby < 36)
  {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("T: Low");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("T: Normal");    
  }
  if(Humidity > 60)
  {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("H: High");
  }
  else if(Humidity < 50)
  {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("H: Low");
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("H: Normal");    
  }
  if(Noise < 90)
  {
    lcd.setCursor(0, 2);
    lcd.print("                ");
    lcd.setCursor(0, 2);
    lcd.print("N: Quiet");
  }
  else if(Noise < 100)
  {
    lcd.setCursor(0, 2);
    lcd.print("                ");
    lcd.setCursor(0, 2);
    lcd.print("N: High");
  }
  else
  {
    lcd.setCursor(0, 2);
    lcd.print("                ");
    lcd.setCursor(0, 2);
    lcd.print("N: Very High");    
  }
  if(DoorIsClosed)
  {
    lcd.setCursor(0, 3);
    lcd.print("                ");
    lcd.setCursor(0, 3);
    lcd.print("Door is Closed");
  }
  else
  {
    lcd.setCursor(0, 3);
    lcd.print("                ");
    lcd.setCursor(0, 3);
    lcd.print("Door is Open");    
  }
}
