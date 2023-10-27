#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <Wire.h>
#include <SPI.h>

RF24 radio(9, 53);

Servo sWheels; // servo motor for turning wheels
Servo sGears;  // servo motor for gear shifting
Servo esc;     // motor control board

Adafruit_BMP280 bmp280;

const int ledLight1 = 4;   // Bumber left LED
const int ledLight2 = 6;   // Bumper right LED
const int ledLight3 = 7;   // License plate LED
const int ledLight4 = 8;   // Brake left light LED
const int ledLight5 = 22;  // Brake right light LED
const int ledLight6 = 23;  // Turn signal left rear LED
const int ledLight7 = 24;  // Turn signal right rear LED
const int ledLight8 = 25;  // Roof light bar 1 (left) LED
const int ledLight9 = 26;  // Roof light bar 2 (left) LED
const int ledLight10 = 27; // Roof light bar 3 (left) LED
const int ledLight11 = 28; // Roof light bar 4 (left) LED
const int ledLight12 = 29; // Low beam left LED
const int ledLight13 = 30; // Low beam right LED
const int ledLight14 = 31; // Marker light left LED
const int ledLight15 = 32; // Marker light right LED
const int ledLight16 = 33; // Turn signal left front LED
const int ledLight17 = 34; // Turn signal right front LED

int car_value[8];
int checkLightBar = 0;
int checkBumperLight = 0;
int checkDayLight = 0;


void setup() {
  Serial.begin(9600);
  radio.begin();                            // initialize the operation of the NRF24L01 module
  radio.setDataRate (RF24_2MBPS);           // set the data transmission rate
  radio.setPayloadSize(32);                 // set the data packet size
  radio.enableAckPayload();                 // set the confirmation for received data (to send a response to that data)
  radio.setRetries(0, 0);                   // set the number and intervals of retries when sending data (retries are disabled)
  radio.setPALevel(RF24_PA_MAX);            // set the transmission signal power level (maximum)
  radio.openReadingPipe(1, 0x7878787878LL); // open the pipe with the transmitter's ID
  radio.setChannel(0x60);                   // set the channel for wireless communication
  radio.startListening();                   // start listening on the opened pipe

  if (!bmp280.begin(0x76)) {
    while (1);
  }
  bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,
                     Adafruit_BMP280::SAMPLING_X16,
                     Adafruit_BMP280::SAMPLING_X2,
                     Adafruit_BMP280::FILTER_X16,
                     Adafruit_BMP280::STANDBY_MS_500);
  
  pinMode(ledLight1, OUTPUT);
  pinMode(ledLight2, OUTPUT);
  pinMode(ledLight3, OUTPUT);
  pinMode(ledLight4, OUTPUT);
  pinMode(ledLight5, OUTPUT);
  pinMode(ledLight6, OUTPUT);
  pinMode(ledLight7, OUTPUT);
  pinMode(ledLight8, OUTPUT);
  pinMode(ledLight9, OUTPUT);
  pinMode(ledLight10, OUTPUT);
  pinMode(ledLight11, OUTPUT);
  pinMode(ledLight12, OUTPUT);
  pinMode(ledLight13, OUTPUT);
  pinMode(ledLight14, OUTPUT);
  pinMode(ledLight15, OUTPUT);
  pinMode(ledLight16, OUTPUT);
  pinMode(ledLight17, OUTPUT);

  esc.attach(5);

  sWheels.attach(2);
  sWheels.write(0);

  sGears.attach(3);
  sGears.write(0);
}


void loop() {
  float temperature = bmp280.readTemperature();

  if (radio.available()) {
    radio.read(&car_value, sizeof(car_value));

    if (car_value[4] == 1) {
      int wheelsAngle = map(180, 0, 1023, 0, 180);
      sGears.write(wheelsAngle);
    }
    else if (car_value[4] == 0) {
      int wheelsAngle = map(0, 0, 1023, 0, 180);
      sGears.write(wheelsAngle);
    }

    int wheelsAngle = map(car_value[0], 0, 1023, 0, 180);
    sWheels.write(wheelsAngle);

    int carSpeed = map(car_value[1], 0, 1023, 0, 180);
    esc.write(carSpeed);

    if (checkLightBar != car_value[5]){
      checkLightBar = car_value[5];
      roofLightBar(checkLightBar);
    }
    if (checkBumperLight != car_value[6]){
      checkBumperLight = car_value[6];
      bumperLight(checkBumperLight);
    }
    if (checkDayLight != car_value[7]){
      checkDayLight = car_value[7];
      dayLight(checkDayLight);
    }
    blinkers(car_value[2], car_value[3]);

    ackData[0] = temperature;
    radio.writeAckPayload(1, &ackData, sizeof(ackData));
  }
}

void roofLightBar(int option){
  digitalWrite(ledLight8, option);
  digitalWrite(ledLight9, option);
  digitalWrite(ledLight10, option);
  digitalWrite(ledLight11, option);
}

void bumperLight(int option) {
  digitalWrite(ledBar1, option);
  digitalWrite(ledBar2, option);
}

void dayLight(int option) {
  digitalWrite(ledLight3, option);
  digitalWrite(ledLight12, option);
  digitalWrite(ledLight13, option);
  digitalWrite(ledLight14, option);
  digitalWrite(ledNormal5, option);
}

void blinkers(int sideLeft, int sideRight) {
  digitalWrite(ledLight16, sideLeft == 1 ? HIGH : LOW);
  digitalWrite(ledLight6, sideLeft == 1 ? HIGH : LOW);
  digitalWrite(ledLight17, sideRight == 1 ? HIGH : LOW);
  digitalWrite(ledLight7, sideRight == 1 ? HIGH : LOW);
}