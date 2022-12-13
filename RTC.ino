int rtc_setup() {
  // general settings
  _rtc_write(0x0e, 0b00000100);
  _rtc_write(0x0f, 0b00000000);

  // get rid of alarm date
  _rtc_write(0x0a, _dec2bcd(1));
}

void rtc_log_time() {
  sprintf(utfDatetime, "%04d-%02d-%02dT%02d:%02d:%02d",
     _bcd2dec(_rtc_read(0x06)) + 2000,  //YYYY
     _bcd2dec(_rtc_read(0x05)),  //MM
     _bcd2dec(_rtc_read(0x04)),  //DD
     _bcd2dec(_rtc_read(0x02) & 0b00111111),  //hh
     _bcd2dec(_rtc_read(0x01)),  //mm
     _bcd2dec(_rtc_read(0x00))); //ss
  dataFile.write(utfDatetime);
  dataFile.print(F(","));
  dataFile.flush();
  delay(10);
}

// Gets the current Time Of Day in seconds
long rtc_get_TOD() {
  return 3600L*_bcd2dec(_rtc_read(0x02)) + 60L*_bcd2dec(_rtc_read(0x01)) + _bcd2dec(_rtc_read(0x00));
}

// Sets alarm at Time of Alarm (seconds past midnight)
void rtc_set_alarm(long TOA) {
  TOA = TOA%86400L;
  hh = TOA/3600L;
  mm = (TOA - hh*3600L)/60;
  ss = TOA - hh*3600L - mm*60;
  delay(10);

  _rtc_write(0x0f, 0b00000000); // clear alarm flag
  
  // set alarm
  _rtc_write(0x07, _dec2bcd(ss)); // seconds
  _rtc_write(0x08, _dec2bcd(mm));  // minutes
  _rtc_write(0x09, _dec2bcd(hh) | 0b10000000); // hours
  _rtc_write(0x0a, 0b10000000); 
  _rtc_write(0x0e, 0b00000101); // alarm on
  delay(10);
}

// write byte to register
void _rtc_write(byte reg, byte val) {
  Wire.beginTransmission(CLOCK_Addr);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

byte _rtc_read(byte reg) {
  Wire.beginTransmission(CLOCK_Addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(CLOCK_Addr, 1);
  return Wire.read();
}

// Interface functions with clock bit format
static uint8_t _bcd2bin (uint8_t val) {return val - 6 * (val >> 4);}
static uint8_t _bin2bcd (uint8_t val) {return val + 6 * (val / 10);}
byte _dec2bcd(byte val) {return ( (val/10*16) + (val%10) );}
byte _bcd2dec(byte val) {return ( (val/16*10) + (val%16) );}
