/* initialize OLED */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_DC 11
#define OLED_CS 12
#define OLED_CLK 10
#define OLED_MOSI 9
#define OLED_RESET 13

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* initialize air module */
#include <SoftwareSerial.h>
#include "AirModule.h"

SoftwareSerial airSerial(8, 12); // RX, TX
char testCmd[7] = {0x42, 0x4d, 0x01, 0x00, 0x00, 0x00, 0x90};
char testResponse[20];


void draw_splash() {
  
  display.clearDisplay();   // clears the screen and buffer

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(display.width()/3, display.height()/6);
  display.println("HCHO");
  display.setCursor(display.width()/8, display.height()/2);
  display.println("Detector");
  
  display.drawRect(1, 1, display.width()-1, display.height()-1, WHITE);
  display.display();
  
}

void setup() {
  /* oled display */
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  
  draw_splash();
  delay(3000);
  
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
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("HCHO:");  
    display.print(get_read_value(air));
    display.print(" ");
    display.println(get_unit_display(air));
    display.display();
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
  delay(1500);
}
