boolean stepclockdot = false;
void uibupdate() {
  if(sensors){
    if(changeflag==true && EEPROM_read(34)==1)lcdb.clear();
    changeflag=false;
    ;
    leds[random(0,5)] = CRGB::Green;
    leds[random(0,5)] = CRGB::Cyan;
    leds[random(0,5)] = CRGB::Yellow;
    leds[random(0,5)] = CRGB::Red;
     FastLED.show();
    Serial.print("offset: ");
    Serial.println((int)sensorsoffset);
    
    
    initBar3();
    
if((0+sensorsoffset)<=3&&(0+sensorsoffset)>=0){
    lcdb.setCursor(0,0+sensorsoffset);
    lcdb.print("D1:");
    lcdb.print(ds_1val);
    
    lcdb.print((char)223);
    lcdb.print("C   ");
    fillBar3(12, 0+sensorsoffset, 8, map(abs(25-ds_1val),0,5,100,0));
}
if((-1+sensorsoffset)<=3&&(-1+sensorsoffset)>=0){
    lcdb.setCursor(0, -1+sensorsoffset);
    lcdb.print("1T:");
    lcdb.print(BMEtemp);
    lcdb.print((char)223);
    lcdb.print("C   ");
    fillBar3(12, -1+sensorsoffset, 8, map(abs(25-BMEtemp),0,5,100,0));
}    
if((-2+sensorsoffset)<=3&&(-2+sensorsoffset)>=0){
    lcdb.setCursor(0, -2+sensorsoffset);
    lcdb.print("1H:");
    lcdb.print(BMEhum);
    lcdb.print("%   ");
    fillBar3(12, -2+sensorsoffset, 8, map(abs(50-BMEhum),0,30,100,0));
}
if((-3+sensorsoffset)<=3&&(-3+sensorsoffset)>=0){  
    lcdb.setCursor(0, -3+sensorsoffset);
    lcdb.print("1P:");
    lcdb.print(BMEpres/133.3);
    lcdb.print("mm   ");
    fillBar3(12, -3+sensorsoffset, 8, map(abs(760-(BMEpres/133.3)),0,40,100,0));  
}
if((-4+sensorsoffset)<=3&&(-4+sensorsoffset)>=0){
    lcdb.setCursor(0, -4+sensorsoffset);
    lcdb.print("V1:");
    lcdb.print(analogRead(A0));
    lcdb.print("m   ");
    fillBar3(12, -4+sensorsoffset, 8, map(analogRead(A0),0,1024,100,0));  


    
}
    }
  else if(settings){
    lcdb.clear();
    lcdb.setCursor(1, 0);
    lcdb.print("configuration menu");

    lcdb.setCursor(0, 1);
    lcdb.print((int)glob);
    lcdb.print("\x7E");

    lcdb.print(EEPROM.read(glob));
    lcdb.print("<"+String(encval)+">");
        
        BacklightUpdate();
        
        if(EEPROM.read(9)==0){
          lcdb.setCursor(0,3);
          lcdb.print("FAQ: vk.cc/a1v0mT");
          }
        if(glob==1){
          lcdb.setCursor(0,2);
          lcdb.print(scan());
          }
        if(glob==2){
          /*
          Serial.print("<COPROLIST>");
          for(word i; i<=255;i++){
            
            Serial.print('|');
            Serial.print((int)i);
            Serial.print("|");
            Serial.print(EEPROM_read(i));
            if(i!=255)Serial.println(",");
            }
            Serial.print("</COPROLIST>");
            */
          }
    }else{
  
  stepclockdot = !stepclockdot;
  if (stepclockdot == true) {
    BMEPull();
    
    
    lcdb.setCursor(0, 0);
    lcdb.print("+4: ");
    lcdb.print(curtime);

    

    //BMEtemp //ds_1val

if(EEPROM.read(33)==0){    
    lcdb.setCursor(0, 2);
    lcdb.write(6);
    lcdb.print("B1:");
    lcdb.print(BMEtemp);
    lcdb.print((char)223);
    

    lcdb.setCursor(0, 1);
    lcdb.write(6);
    lcdb.print("D1:");
    lcdb.print(ds_1val);
    lcdb.print((char)223);
    
}else{
  
    lcdb.setCursor(0, 1);
    lcdb.write(6);
    lcdb.print("Cp.");
    lcdb.print(avgtemp);
    lcdb.print((char)223);
    lcdb.print("C");
  }

    if(EEPROM.read(33)==1)lcdb.setCursor(0, 2);
    if(EEPROM.read(33)==0)lcdb.setCursor(0, 3);
    lcdb.write(7);
    if(EEPROM.read(33)==1)lcdb.print("Cp.");
    if(EEPROM.read(33)==0)lcdb.print("B1:");
    lcdb.print(BMEhum);
    lcdb.print("%");    

    
    
    //fillBar3(11, 3, 9, map(abs(25-abs(BMEtemp)),0,10,100,0));
    lcdb.setCursor(11,1);
    lcdb.write(6);
    fillBar3(12, 1, 8, map(abs(25-avgtemp),0,5,100,0));
    lcdb.setCursor(11,2);
    lcdb.write(7);
    fillBar3(12, 2, 8, map(abs(50-BMEhum),0,30,100,0));
    lcdb.setCursor(11,3);
    lcdb.write('#');
    fillBar3(12, 3, 8, map(abs(760-(BMEpres/133.3)),0,40,100,0));  
    
    

//    lcdb.setCursor(0, 1);
    //lcdb.print(BMEtemp);
//    lcdb.print((char)223);
//    lcdb.print("C    ");
//
    //lcdb.print(BMEhum);
    
    //lcdb.print("% ");

if(BMEhum>45&&BMEhum<60){
     leds[0] = CRGB::Green;
    }else{
     leds[0] = CRGB::Red;
    }
if(BMEtemp>23&&BMEtemp<27){
     leds[1] = CRGB::Green;
    }
    else{
     leds[1] = CRGB::Red;
    }
if(ds_1val>23&&ds_1val<27){
     leds[2] = CRGB::Green;
    }
    else{
     leds[2] = CRGB::Red;
    }
        
if(BMEpres/133.3>720&&BMEpres/133.3<640){
     leds[3] = CRGB::Green;
    }
    else{
     leds[3] = CRGB::Red;
    }    
    FastLED.show();
    //fillBar3(0, 3, 9, BMEhum);
    
    //давление
    
  }
  else {

    lcdb.setCursor(17, 0);
    lcdb.print(":");
    

    }
  }
}


