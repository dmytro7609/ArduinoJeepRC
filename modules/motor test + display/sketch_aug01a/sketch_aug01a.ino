#include <Servo.h>
#include <printf.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define JOYSTICK A0 // керування напрямком руху

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Servo ESC;
float value;

void setup() {
  Serial.begin(9600);
  ESC.attach(5);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

}

void loop() {
  value=analogRead(JOYSTICK); // Ortalama değer
  value=map(value, 0, 1023, 0, 180);
  ESC.write(value);
  Serial.print("Değer= ");
  Serial.println(value);
  Serial.print("Joystick= ");
  Serial.println(analogRead(JOYSTICK));
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  String temp_to_string1 = String(value, 1);
  String temp_to_string2 = String(analogRead(JOYSTICK), 1);
  display.println(temp_to_string1 + " " + analogRead(JOYSTICK));
  display.display();
}
