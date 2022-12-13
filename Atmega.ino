void atmega_setup() {
  pinMode(RTC_INTERRUPT, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(CO2_SWITCH, OUTPUT);
  digitalWrite(CO2_SWITCH, LOW);
  pinMode(SD_SWITCH, OUTPUT);
//  digitalWrite(SD_SWITCH, LOW);
  pinMode(MOTOR_OPEN, OUTPUT);
  pinMode(MOTOR_CLOSE, OUTPUT);
  pinMode(3, INPUT);
  digitalWrite(3, LOW);
  //Serial.begin(9600);
//  Wire.begin();
  adcsra_save = ADCSRA;  //save ADC status
  ADCSRA = 0; //disable ADC
}


void atmega_sleep(int wakePin) {    
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  // set up interrupts
  noInterrupts();
  attachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT), atmega_wake, LOW);
  interrupts();

  MCUCR = bit (BODS) | bit (BODSE);
  MCUCR = bit (BODS);
  sleep_cpu();

  pinMode(SD_CS, INPUT);
  SPI.begin();
}

void atmega_wake() {
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT));
}


 //function returns bytes of RAM Availabile
 
//  extern int __heap_start, *__brkval; 
//  int v; 
//  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
//}
