void sd_log_data() {
  sd_ON();
  rtc_log_time();
  dataFile.print(CO2);
  dataFile.print(",");
  dataFile.print(T);
  dataFile.print(",");
  dataFile.print(RH);
  dataFile.println();
  dataFile.close();
  delay(10);
  sd_OFF();
}

void sd_log_note(int n) {
  sd_ON();
  rtc_log_time();
  dataFile.print(n);
  dataFile.print(",");
  dataFile.print(n);
  dataFile.print(",");
  dataFile.print(n);
  dataFile.println();
  dataFile.close();
  delay(10);
  sd_OFF();
}

void sd_ON() {
  digitalWrite(SD_SWITCH, HIGH);
  delay(10);
  SD.begin(SD_CS);
  dataFile = SD.open(F("dataFile.csv"), FILE_WRITE);
}

void sd_OFF() {
  digitalWrite(SD_SWITCH, LOW);
  SD.end();
}
