#include <printf.h>
#include <nRF24L01.h> // Подключаем файл конфигурации из библиотеки RF24
#include <RF24_config.h>
#include <RF24.h> // Подключаем библиотеку для работа для работы с модулем NRF24L01
#include <SPI.h> // Подключаем библиотеку для работы с SPI-интерфейсом
#include <Wire.h>

RF24 radio(9, 10);
int car_value[8];
int orig_speed = 0;
int invert_speed = 0;
float receivedTemperature;

#define JOYSTICKX A1        // керування швидкістю
#define JOYSTICKY A0        // керування кермом

void setup() {
  Serial.begin(9600);
  radio.begin();                         // Инициализация модуля NRF24L01
  radio.setChannel(5);                   // Обмен данными будет вестись на пятом канале (2,405 ГГц)
  radio.setDataRate (RF24_250KBPS);      // Скорость обмена данными 1 Мбит/сек
  radio.setPayloadSize(32);
  radio.setRetries(0, 0);                // (время между попыткой достучаться, число попыток)
  radio.setPALevel(RF24_PA_MAX);         // Выбираем высокую мощность передатчика (-6dBm)
  radio.openWritingPipe(0x7878787878LL); // Открываем трубу с уникальным ID
  radio.setChannel(0x60);


}

void loop() {
  orig_speed = analogRead(JOYSTICKX);
  invert_speed = 1023 - orig_speed;
  car_value[0] = analogRead(JOYSTICKY);
  car_value[1] = constrain(invert_speed, 86, 660);
  car_value[2] = 1;
  car_value[3] = 1;
  car_value[4] = 1;
  car_value[5] = 1;
  car_value[6] = 1;
  car_value[7] = 1;
  radio.write(&car_value, sizeof(car_value));

  /*if (radio.available()) {
    radio.read(&receivedTemperature, sizeof(receivedTemperature));
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(3);
    display.setCursor(1, 6);
    display.println("Temp:");
    display.println("12.9");
    //display.println(receivedTemperature, 1);
    display.setCursor(73, 30);
    display.setTextSize(1);
    display.print("C");
    display.display();
    }*/
}
