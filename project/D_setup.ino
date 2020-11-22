/* timer instance for setting tasks schedule */
Timer Incubator;

// the setup function runs once when you press reset or power the board
void setup() {

  InitSystemDirections();
  InitAlarmLeds();
  Serial.begin(9600);
  bluetoothTerminal.begin(9600);
  dht.begin();    // begin conversion of the readings inside dht module

  delay(1000);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 4);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  Incubator.every(500, updateDoorMonitor, NULL);
  Incubator.every(1000, updateTemperatureMonitor, NULL);
  Incubator.every(1000, updateHumidityMonitor, NULL);
  Incubator.every(1000, updateSoundMonitor, NULL);
  Incubator.every(2000, updateSystemStatus, NULL);
  Incubator.every(500, updateSystemMeasurements, NULL);
  Incubator.every(1000, updateSleepState, NULL);
  Incubator.every(1000, updateSystemStatusMonitor, NULL);
  
}
