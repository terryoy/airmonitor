#include <SoftwareSerial.h>
#include "AirModule.h"

//int led = 13;
SoftwareSerial airSerial(10, 11);
char testCmd[7] = {0x42, 0x4d, 0x01, 0x00, 0x00, 0x00, 0x90};
char testResponse[20];

void setup() {
  Serial.begin(9600);
  airSerial.begin(9600);
//  pinMode(led, OUTPUT);
}

void loop() {
  // request hcho
  for (int i=0;i<7;i++) {
    airSerial.write(testCmd[i]);
  }
  
  // response handle
  int pos = 0;
  for (int i=0;i<20;i++) testResponse[i]=0x00; // clean buffer
  
  while (airSerial.available() && pos < 20) {
    testResponse[pos] = airSerial.read();
//    Serial.print(testResponse[pos], HEX);
//    Serial.print(" ");
    
    if (pos == 0 && testResponse[pos] != 0x42) {
      testResponse[pos] = 0; // clear the buffer
    }
    else if (pos == 1 && testResponse[pos] != 0x4d) {
      testResponse[0] = testResponse[1] = 0;
      pos = 0;
    }
    else {
      pos++;
    }
  }
  
  // parse data
  if (testResponse[0] == 0x42 && testResponse[1] == 0x4d) {
    AirResponse air;
    parse_air_response(&air, testResponse);
    
    Serial.print("read data: ");    
    Serial.print(get_read_value(air));
    Serial.print(" ");
    Serial.println(get_unit_display(air));
  }
  else {
//    Serial.print("N/A (data: ");
//    for (int i=0;i<20;i++) {
//      Serial.print(testResponse[0], HEX);
//      Serial.print(" ");
//    }
//    Serial.println(")");
  }
  
  
  // sleep
  delay(3000);
}
