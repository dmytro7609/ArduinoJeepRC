#include <Adafruit_BMP280.h> // for BPM280 #1
#include <Adafruit_Sensor.h> // for BPM280 #2
#include <Adafruit_GFX.h> //need to be install
#include <RF24_config.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>
#include <Servo.h>
#include <Wire.h>
#include <SPI.h>

//Розібратися коли вмикати стоп сигнали
//Коли і яки вмикати світодіоди
//Перемикання передач
//Зворотня відправка показника датчика температури
//DELETE ALL OLD CODE

RF24 radio(9, 53); // Create a Radio

Servo sWheels; // Arduino pin connected to Servo motor for turning wheels
Servo esc;     // motor regulator

int car_value[8];

void setup() {
  Serial.begin(9600);         // открываем порт для связи с ПК
  radio.begin();  // Инициализация модуля NRF24L01
  radio.setChannel(5); // Обмен данными будет вестись на пятом канале (2,405 ГГц)
  radio.setDataRate (RF24_250KBPS); // Скорость обмена данными 1 Мбит/сек
  radio.setPayloadSize(32);
  radio.setRetries(0, 0);    // (время между попыткой достучаться, число попыток)
  radio.setPALevel(RF24_PA_MAX); // Выбираем высокую мощность передатчика (-6dBm)
  radio.openReadingPipe(1, 0x7878787878LL); // Открываем трубу ID передатчика
  radio.setChannel(0x60);
  radio.startListening(); // Начинаем прослушивать открываемую трубу

  esc.attach(5);
  sWheels.attach(2);
  sWheels.write(0);
}

void loop() {
  //float temperature = 25.5;
  //float temperature = bmp280.readTemperature();
  //Serial.println(F("Temperature = "));
  //Serial.print(temperature, 1);

  if (radio.available()) {
    radio.read(&car_value, sizeof(car_value)); //read joystick values

    Serial.println(car_value[6]);

    int carSpeed = map(car_value[1], 0, 1023, 0, 180); // керування швидкістю, або 255
    esc.write(carSpeed);
    int wheelsAngle = map(car_value[0], 0, 1023, 0, 180); // керування напрямком руху
    sWheels.write(wheelsAngle);

    //radio.writeAckPayload(1, &temperature, sizeof(temperature));
  }
}
