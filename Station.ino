
/* first encoder*/
#define lcdFirst 0x27

#include "GyverEncoder.h"
#include <iarduino_RTC.h>
#include <EEPROM.h>
#include <FastLED.h>
#include <OneWire.h>
#include <BME280I2C.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
Encoder enc1(EEPROM.read(21), EEPROM.read(22), EEPROM.read(23));
BME280I2C bme;
iarduino_RTC time(RTC_DS3231);
//LiquidCrystal_I2C lcdg(EEPROM.read(12),16,2);
LiquidCrystal_I2C lcdb(lcdFirst, 20, 4);
OneWire ds18b20_1(EEPROM.read(13));
double ds_1val=0;
CRGB leds[15];
char sensorsoffset = 3;
boolean lcdbsel = false;
boolean lcdgsel = false;
boolean settings = false;
boolean sensors = false;
char glob = 1;
int encval = 0;
char submin = 0;
String curtime="";
double avgtemp=0;
boolean changeflag=false;

bool BMEerror = false;
float BMEtemp(NAN), BMEhum(NAN), BMEpres(NAN);

void setup() {

  TimerCreate(EEPROM.read(36),EEPROM.read(37),EEPROM.read(38));
  
  lcdb.init();
  lcdb.backlight();
  lcdb.setCursor(6, 0);
  lcdb.print("Toster OS");
  lcdb.setCursor(0, 1);
  lcdb.print("Loading...");
  delay(1000);
  lcdb.setCursor(0, 1);
  lcdb.print("PUD: --+++++++-+-+-");
  lcdb.setCursor(0, 2);
  lcdb.print("PUD: +---++--");
  lcdb.setCursor(0, 3);
  lcdb.print(scan());
  delay(1000);


  Serial.begin(9600);
  enc1.setType(TYPE2);
  pinMode(EEPROM.read(32), OUTPUT);
  FastLED.addLeds<NEOPIXEL, 8>(leds, 15);
  time.begin();
  //lcdg.init();
  //lcdg.setCursor(0,0);
  //lcdg.print("g");
  
  BMEerror = !bme.begin();
  initBar3();
  
  curtime=time.gettime("d m Y,H i");
  lcdb.init();
  lcdb.setCursor(4, 0);
  lcdb.print("MTZ16 v0.1 DEMO");
  
  for(int i=0; i<map(EEPROM.read(8),0,10,0,254);i++){
  analogWrite(EEPROM.read(6),i);
  delay(15);
  }
   BacklightUpdate();
  
  
  

  lcdb.setCursor(0, 3);
  lcdb.print("finish");
  lcdb.init();
}

ISR(TIMER1_COMPA_vect)
{
    digitalWrite(EEPROM_read(35), !digitalRead(EEPROM_read(35)));
     
}

void loop() {
  enc1.tick();
  digitalWrite(12, HIGH);


  if (millis() % 2000 == 100)lcdb.backlight();
  if (millis() % 500 == 0)uibupdate();
  if (millis() % 2000 == 0)dspush(ds18b20_1);

  if (millis() % 2000 == 1000)ds_1val=dspull(ds18b20_1);
  if (millis() % 2000 == 1200)avgtemp=(ds_1val+BMEtemp)/2;
  if (millis() % 5000 == 0)curtime=time.gettime("d m Y,H i");

  if (enc1.isTurn()) {
    changeflag=true;
    // если был совершён поворот (индикатор поворота в любую сторону)
    digitalWrite(12, LOW);
    tone(2, 500, 20);
    if (settings)uibupdate();
  }
 // enc1.tick();
  if (enc1.isRight()) {
    tone(EEPROM_read(32), 1500, 20);
    Serial.println("R");
    encval++;
    if (sensors)sensorsoffset++;
  }
  if (enc1.isLeft()) {
    tone(EEPROM_read(32), 1300, 20);
    Serial.println("L");
    encval--;
    if (sensors)sensorsoffset--;
  }
  if (enc1.isRightH()) {
    tone(EEPROM_read(32), 500, 40);
    Serial.println("RH");
    glob++;
    if (glob > 250)glob = 250;
  }
  if (enc1.isLeftH()) {
    tone(EEPROM_read(32), 400, 40);
    Serial.println("LH");
    glob--;
    if (glob < 0)glob = 0;
  }
  if (enc1.isPress()) {
    tone(EEPROM_read(32), 2000, 20); Serial.println("P");  if (settings) {
      ;
    }
    else {
      sensors = !sensors;
    }
  }
  if (enc1.isRelease()) {
    tone(EEPROM_read(32), 2200, 20);
    Serial.println("R");
    lcdb.clear();
    if (settings) {
      EEPROM.write(glob, encval);
    }
  }
  if (enc1.isHolded()) {
    tone(EEPROM_read(32), 1500, 60);
    Serial.println("H");
    lcdb.clear();
    settings = !settings;
  }
  //if (enc1.isHold()){ Serial.println("Hold");         }

}
