void scd30_setup() {
  digitalWrite(CO2_SWITCH, HIGH);
  delay(50);
  scd30.begin();
  scd30.setMeasurementInterval(MSMNT_interval);
}
