#include <avr/sleep.h>
#include <SparkFun_SCD30_Arduino_Library.h>
//#include <Wire.h>
//#include <avr/wdt.h>
#include <SPI.h>
#include <SD.h>

#define RTC_INTERRUPT 2
#define LED 14
#define SD_CS 9
#define SD_SWITCH 8
#define CO2_SWITCH 4
#define CLOCK_Addr 0x68

#define MOTOR_OPEN 6
#define MOTOR_CLOSE 5
#define MOTOR_OPEN_time 1    // ALL TIMES AND INTERVALS IN [SECONDS]
#define MOTOR_CLOSE_time 1

#define CLOSURE_interval 1800
#define PURGE_time 120
#define CLOSURE_time 300
#define MSMNT_interval 10


long startTime;
int cycleTime;
uint16_t CO2;
SCD30 scd30;
File dataFile;
char utfDatetime[20];
float T, RH;
uint8_t i, hh, mm, ss;
byte adcsra_save;

/////////////////////////DEFINITIONS DONE, DO SETUP/////////////////////////////////////////////

void setup() {
  atmega_setup();
  digitalWrite(LED, HIGH);
  //Serial.println(F("restart"));
  rtc_setup();

//  wdt_enable(WDTO_8S);
//  wdt_reset();
  delay(1000);
  digitalWrite(LED, LOW);

  sd_log_note(-9);   //log restart begin

  digitalWrite(MOTOR_CLOSE, HIGH);     //// open close
  delay(1000*MOTOR_CLOSE_time);
  digitalWrite(MOTOR_CLOSE, LOW);
  digitalWrite(MOTOR_OPEN, HIGH);
  delay(1000*MOTOR_OPEN_time);
  digitalWrite(MOTOR_OPEN, LOW);
  

///////////////////////CLOSURE CYCLE/////////////////////////
  while (1) {
    startTime = rtc_get_TOD();                  /////////// BEGIN PRE-PURGE
    sd_log_note(-1);
    //Serial.println(F("opening box"));
    scd30_setup();
    cycleTime = MOTOR_OPEN_time;
    
    while (cycleTime < PURGE_time) {      ////////// PURGE MSMNT CYCLE
      cycleTime += MSMNT_interval;
      rtc_set_alarm(startTime + cycleTime);
      atmega_sleep(RTC_INTERRUPT);
      measure();
    }//end purge msmnts

    //Serial.println(F("closing box"));           /////////// BEGIN CLOSURE
    sd_log_note(-2);
    digitalWrite(MOTOR_CLOSE, HIGH);
    delay(1000*MOTOR_CLOSE_time);
    digitalWrite(MOTOR_CLOSE, LOW);
    cycleTime += MOTOR_CLOSE_time;


    while (cycleTime < (PURGE_time+CLOSURE_time)) {      ///////// CLOSURE MSMNT CYCLE
      cycleTime += MSMNT_interval;
      if (startTime + cycleTime > rtc_get_TOD() + 1) { //?
        rtc_set_alarm(startTime + cycleTime);
        atmega_sleep(RTC_INTERRUPT);
      }//end alarm IF
      measure();
    }//end closure msmnts


    digitalWrite(CO2_SWITCH, LOW);                       ///////// LONG SLEEP
    sd_log_note(-3);
    digitalWrite(MOTOR_OPEN, HIGH);
    delay(1000*MOTOR_OPEN_time);
    digitalWrite(MOTOR_OPEN, LOW);
    //Serial.println(F("sleep"));
    //Serial.flush();
    rtc_set_alarm(startTime + CLOSURE_interval);
    atmega_sleep(RTC_INTERRUPT);
    
//    wdt_enable(WDTO_8S);
//    wdt_disable();
  } //end big loop
} //end all

void loop(){}
