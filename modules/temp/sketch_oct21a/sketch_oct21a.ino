#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp280;

void setup() {
  Serial.begin(9600);
  Serial.println(F("BMP280"));

  if(!bmp280.begin(0x76)) {
    while(1);
  }
  bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,
                     Adafruit_BMP280::SAMPLING_X16,
                     Adafruit_BMP280::SAMPLING_X2,
                     Adafruit_BMP280::FILTER_X16,
                     Adafruit_BMP280::STANDBY_MS_500);
}

void loop() {
  float temperature = bmp280.readTemperature();
  
  Serial.print(F("Temperature = "));
  Serial.print(temperature, 1);
  Serial.println(" *C");

  Serial.println();
  delay(10);
}
