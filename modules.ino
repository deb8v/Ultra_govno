void BacklightUpdate() {
  analogWrite(EEPROM_read(6), map(EEPROM_read(8), 0, 10, 0, 254));
  analogWrite(EEPROM_read(5), map(EEPROM_read(7), 0, 10, 0, 254));
}

void BMEPull() {
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);
  bme.read(BMEpres, BMEtemp, BMEhum, tempUnit, presUnit);
}

void dspush(OneWire ds) {
  ds.reset();
  ds.write(0xCC);
  ds.write(0x44);

}

float dspull(OneWire ds) {
  ds.reset();
  ds.write(0xCC);
  ds.write(0xBE);
  byte data[2];
  data[0] = ds.read();
  data[1] = ds.read();
  return ((data[1] << 8) | data[0]) * 0.0625;
}


String scan() {
  String devices = "";
  byte error, address;
  int nDevices;

  nDevices = 0;
  for (address = 8; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print(address);
      devices += String(address) + ";";
      Serial.println(" !");

      nDevices++;
    }
    else if (error == 4) {

      Serial.println(address);
      devices += "!" + String(address) + ";";
    }
  }
  if (nDevices == 0)
    devices = "-";
  else
    Serial.println("done\n");

  return devices;
}
char* i2cr(int from, int len) {
  char* buff;
  Wire.requestFrom(from, len);

  while (Wire.available()) {
    buff += char(Wire.read());
    Serial.println(buff);

  }
  return buff;

}
boolean change_flag = true;
void initBar3() {
  // создано в http://maxpromer.github.io/lcd-Character-Creator/
  byte right_empty[8] = {0b11111,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b11111};
  byte left_empty[8] = {0b11111,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b11111};
  byte bar2[] = {0b11111, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11111};
  byte bar3[] = {  B11111,  B11100,  B11100,  B11100,  B11100,  B11100,  B11100,  B11111};
  byte bar4[] = {  B11111,  B11110,  B11110,  B11110,  B11110,  B11110,  B11110,  B11111};
  byte center_empty[8] = {0b11111, 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};


  byte temp[8] = {
 0b10001,
  0b10101,
  0b10101,
  0b10101,
  0b10001,
  0b10001,
  0b10001,
  0b11111
};
  byte hum[8] = {  4, 4, 14, 14, 14, 29, 31, 14};
  byte pres[8] = {8, 2, 17, 4, 0, 18, 8, 2};
  lcdb.createChar(6, temp);
  lcdb.createChar(7, hum);
  //lcdb.createChar(8, pres);
  lcdb.createChar(0, left_empty);
  lcdb.createChar(1, center_empty);
  lcdb.createChar(2, right_empty);
  lcdb.createChar(3, bar2);
  lcdb.createChar(4, bar3);
  lcdb.createChar(5, bar4);

}
void fillBar3(byte start_pos, byte row, byte bar_length, byte fill_percent) {
  byte infill = bar_length * fill_percent / 10;
  byte fract = infill % 10;
  infill = infill / 10;
  // change_flag - true слева, false справа
  if (infill < bar_length - 1) {
    if (!change_flag) {
      change_flag = true;
      byte bar2[] = {0b11111, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11111};
      byte bar3[] = { B11111, B11100, B11100, B11100, B11100, B11100, B11100, B11111};
      byte bar4[] = { B11111, B11110, B11110, B11110, B11110, B11110, B11110, B11111};
      lcdb.createChar(3, bar2);
      lcdb.createChar(4, bar3);
      lcdb.createChar(5, bar4);
    }
  } else {
    if (change_flag) {
      change_flag = false;
      byte leftbar1[] = { B11111, B10001, B10001, B10001, B10001, B10001, B10001, B11111};
      byte leftbar2[] = { B11111, B11001, B11001, B11001, B11001, B11001, B11001, B11111};
      byte leftbar3[] = { B11111, B11101, B11101, B11101, B11101, B11101, B11101, B11111};
      lcdb.createChar(3, leftbar1);
      lcdb.createChar(4, leftbar2);
      lcdb.createChar(5, leftbar3);
    }
  } lcdb.setCursor(start_pos, row); if (infill == 0) {
    if (fract >= 0 && fract < 2) lcdb.write(0); else if (fract >= 2 && fract < 4) lcdb.write(0); else if (fract >= 4 && fract < 6) lcdb.write(3); else if (fract >= 6 && fract < 8) lcdb.write(4); else if (fract >= 8) lcdb.write(5);
  }
  else lcdb.write(255);
  for (int n = 1; n < bar_length - 1; n++) {
    if (n < infill) lcdb.write(255); if (n == infill) {
      if (fract >= 0 && fract < 2) lcdb.write(1); else if (fract >= 2 && fract < 4) lcdb.write(0); else if (fract >= 4 && fract < 6) lcdb.write(3); else if (fract >= 6 && fract < 8) lcdb.write(4); else if (fract >= 8) lcdb.write(5);
    }
    if (n > infill) lcdb.write(1);
  }
  if (infill == bar_length - 1) {
    if (fract >= 0 && fract < 2) lcdb.write(2); else if (fract >= 2 && fract < 4) lcdb.write(3); else if (fract >= 4 && fract < 6) lcdb.write(4); else if (fract >= 6 && fract < 8) lcdb.write(5); else if (fract >= 8) lcdb.write(255);
  }
  else if (infill == bar_length) lcdb.write(255);
  else lcdb.write(2);
}
void TimerCreate(byte TRT1f12, byte TRT1f11, byte TRT1f10) {
  cli();  // отключить глобальные прерывания
  TCCR1A = 0;   // установить регистры в 0
  TCCR1B = 0;
  OCR1A = 15624; // установка регистра совпадения
  TCCR1B |= (1 << WGM12);
  if (TRT1f10)TCCR1B |= (1 << CS10);
  if (TRT1f11)TCCR1B |= (1 << CS11);
  if (TRT1f12)TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << OCIE1A);
  sei();
  /*
    12 11 10
    0  0  1 /1
    0  1  0 /8   |.0328
    0  1  1 /64  |.2624
    1  0  0 /256
    1  0  1 /1024
    1/(16 ∗ 10^6)~0.0041
  */
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
  while (EECR & (1 << EEPE));
  EEAR = uiAddress;
  EECR |= (1 << EERE);
  return EEDR;
}
void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
  while (EECR & (1 << EEPE));
  EEAR = uiAddress;
  EEDR = ucData;
  EECR |= (1 << EEMPE);
  EECR |= (1 << EEPE);
}

