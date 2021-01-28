#include "switch.h"
#include <SoftwareSerial.h>

#define led_pin 13

Switch redLED(byte(0x14), 7);
Switch greenLED(byte(0x15), 6);
Switch blueLED(byte(0x16), 5);

SoftwareSerial ble(10, 11);
long previousMillis = 0;

void setup() {
  // put your setup code here, to run once:
  ble.begin(9600);
  
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(led_pin, OUTPUT);
}

void loop() {
  // Receive
  byte serialBuff[2];
  if (ble.available()){
    digitalWrite(led_pin, HIGH);
    
    ble.readBytes(serialBuff, 2);
    byte id = serialBuff[0];
    byte command = serialBuff[1];
    
    if (id == byte(0x00)){
      responceDeviceList();
      
      delay(100);
      digitalWrite(led_pin, LOW);
      return;
    }

    redLED.executeCodeWithId(command, id);
    greenLED.executeCodeWithId(command, id);
    blueLED.executeCodeWithId(command, id);
    
    delay(100);
    digitalWrite(led_pin, LOW);
  }

  // Send
//  if (millis() - previousMillis > 1000){
//    previousMillis = millis();
//  }
}

void responceDeviceList(){
  byte deviceDefineId = byte(0x00);
  
  postData(deviceDefineId, redLED.id);
  postData(deviceDefineId, greenLED.id);
  postData(deviceDefineId, blueLED.id);
}

void postData(byte id, byte value){
  ble.write(id);
  ble.write(value);
}
