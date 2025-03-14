#include <Servo.h>
#include <printf.h>
#include <SPI.h>

#define JOYSTICK A0 // керування напрямком руху

Servo servoX;

float value;

void setup() {
  Serial.begin(9600);
  servoX.attach(2);
}

void loop() {
  value=analogRead(JOYSTICK); // Ortalama değer
  value=map(value, 0, 1023, 0, 90);
  servoX.write(value);
}
