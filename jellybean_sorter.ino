#include <Wire.h>
#include <Servo.h>

#include <Adafruit_TCS34725.h>

#define SERVO_PIN 5
#define LED_PIN 13

enum Direction {
  LEFT, RIGHT
};

const int DELTA_LUX_THRESHOLD = 75;
const int TYPICAL_LUX = 1260;
const int HUE_DISCRIM = 100;
const int COLOR_RANGE = 6000;

const int SERVO_NEUTRAL = 90;
const int SERVO_LEFT = 180;
const int SERVO_RIGHT = 0;

const int COLOR_SENSOR_DELAY = 500;
const int SERVO_DELAY = 500;

Servo servo;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No color sensor found");
    while (true);
  }
  servo.attach(SERVO_PIN);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  uint16_t r, g, b, c, lux;
  int dlux;
  tcs.setInterrupt(false);      // turn on LED
  delay(60);
  tcs.getRawData(&r, &g, &b, &c);
  tcs.setInterrupt(true);  // turn off LED
  //colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  dlux = int(lux - TYPICAL_LUX);
  int h = hue(r, g, b);
  
  Serial.print(r); Serial.print("\t");
  Serial.print(g); Serial.print("\t");
  Serial.print(b); Serial.print("\t");
  Serial.print(lux); Serial.print("\t");
  Serial.print(h); Serial.print("\t");
  Serial.println(dlux);
  if (abs(dlux) > DELTA_LUX_THRESHOLD) {
    Serial.println("Found a jellybean");
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    if (h > HUE_DISCRIM) {
      Serial.println("Is GOOD bean");
      sendServo(LEFT);
    } else {
      Serial.println("Is BAD bean");
      sendServo(RIGHT);
    }
    delay(1000);
    digitalWrite(LED_PIN, LOW);
  }
}

void sendServo(Direction d) {
  switch (d) {
    case LEFT:
      servo.write(SERVO_LEFT);
      break;
    case RIGHT:
      servo.write(SERVO_RIGHT);
  }
  delay(SERVO_DELAY);
  servo.write(SERVO_NEUTRAL);
  delay(SERVO_DELAY);
}

int hue(uint16_t r, uint16_t g, uint16_t b) {
  float R = float(r) / COLOR_RANGE;
  float G = float(g) / COLOR_RANGE;
  float B = float(b) / COLOR_RANGE;
  float _max = max(max(R, G), B);
  float range = _max - min(min(R, G), B);
  float hue;
  if (R == _max) {
    hue = (G - B) / range;
  } else if (G == _max) {
    hue = 2 + (B - R) / range;
  } else {
    hue = 4 + (R - G) / range;
  }
  return 60 * hue;
}

