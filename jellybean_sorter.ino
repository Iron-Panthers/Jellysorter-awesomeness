#include <Wire.h>
#include <Servo.h>

#include <Adafruit_TCS34725.h>

const int SERVO_PIN = 5;

Servo servo;

Adafruit_TCS34725 tcs = Adafruit_TCS34725();

void setup() {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No color sensor found");
  }
  servo.attach(SERVO_PIN);
}

void loop() {
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  Serial.print("Color: ");
  Serial.print(r);
  Serial.print("\t");
  Serial.print(g);
  Serial.print("\t");
  Serial.print(b);
  Serial.print("\t");
  Serial.println(c);

  if (r > 10) {
    
  }
  
  Serial.println();
}
