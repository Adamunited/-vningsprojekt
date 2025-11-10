/*
* Name: övningsprojekt
* Author: Adam Yousif
* Date: 2025-10-20
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display, 
* Further, it measures temprature with ds3231 and displays a mapped value to a 9g-servo-motor.
*/

const int potPin   = A0;   
const int tempPin  = A1;   
const int leds[]   = {10, 8, 5, 3}; 

#include <U8glib.h> 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);


void setup() {
  for (int i = 0; i < 4; i++) pinMode(leds[i], OUTPUT);
  pinMode(potPin, INPUT);
  pinMode(tempPin, INPUT);
  Serial.begin(9600);
}


void loop() {
  float tempC = getTemp();         // t.ex. 0–50 °C, enkel skalning
  int   thresh = getThresh();      // t.ex. 15–40
  oledDraw(tempC, thresh);         // skriv två rader på skärmen

  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print(" C  |  Thresh: ");
  Serial.println(thresh);

  if (tempC > thresh) {
    alarm();                       // blinka LED:ar en i taget
  } else {
    allLedsOff();
    delay(150);
  }
}

// Förenklad “temperatur” från analog sensor.
// här skalar vi 0–1023 till 0–50 °C.
float getTemp() {
  int raw = analogRead(tempPin);         // 0–1023
  float tempC = map(raw, 0, 1023, 0, 50);
  return tempC;
}

// Tröskel från potentiometer, samma intervall som på skärmen
int getThresh() {
  int raw = analogRead(potPin);          // 0–1023
  int t = map(raw, 0, 1023, 15, 40);     // 15–40
  return t;
}

// Rita två rader: Temp och Thresh
void oledDraw(float tempC, int thresh) {
  char line1[20];
  char line2[20];
  dtostrf(tempC, 5, 1, line1);           // format  xx.x
  sprintf(line1, "Temp: %s C", line1);
  sprintf(line2, "Thresh: %d C", thresh);

  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x13);
    u8g.drawStr(0, 20, line1);
    u8g.drawStr(0, 40, line2);
  } while (u8g.nextPage());
}

// Alarm: fyra LED:ar blinkar en i taget
void alarm() {
  for (int i = 0; i < 4; i++) {
    allLedsOff();
    digitalWrite(leds[i], HIGH);
    delay(120);
  }
}

void allLedsOff() {
  for (int i = 0; i < 4; i++) digitalWrite(leds[i], LOW);
}
