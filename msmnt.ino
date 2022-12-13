void measure() {
  digitalWrite(LED, HIGH);
  delay(10);
  digitalWrite(LED, LOW);
  
  CO2 = scd30.getCO2();
  T = scd30.getTemperature();
  RH = scd30.getHumidity();
  sd_log_data();

  //Serial.print(CO2);
  //Serial.println();
}
