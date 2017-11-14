#include <Wire.h>
#include <Servo.h>

#include <Adafruit_TCS34725.h>

#define SERVO_PIN 5

const int SERVO_NEUTRAL = 90;
const int SERVO_LEFT = 150;
const int SERVO_RIGHT = 30;

const int COLOR_SENSOR_DELAY = 500;
const int SERVO_DELAY = 500;

Servo servo;

Adafruit_TCS34725 tcs = Adafruit_TCS34725();

void setup() {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No color sensor found");
    while (true) {}
  }
  servo.attach(SERVO_PIN);
}

void loop() {
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  //colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  Serial.print("Color: ");
  Serial.print(r);
  Serial.print("\t");
  Serial.print(g);
  Serial.print("\t");
  Serial.print(b);
  Serial.print("\t");
  Serial.println(c);

  if (lux > 18000) {
    Serial.println("Detected an increase in light");
    delay(COLOR_SENSOR_DELAY);
    if (r > 18000) {
      Serial.println("Detected a left bucket jellybean");
      servo.write(SERVO_LEFT);
      delay(SERVO_DELAY);
    } else {
      Serial.println("Detected a right bucket jellybean");
      servo.write(SERVO_RIGHT);
      delay(SERVO_DELAY);
    }
    servo.write(SERVO_NEUTRAL);
    delay(SERVO_DELAY);
  }
  
  Serial.println();
  delay(10);
}
