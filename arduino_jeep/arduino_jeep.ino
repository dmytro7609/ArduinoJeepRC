#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <Wire.h>
#include <SPI.h>

RF24 radio(9, 53);

Servo sWheels; // servo motor for turning wheels
Servo esc;     // motor control board

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
  radio.setDataRate (RF24_1MBPS);           // set the data transmission rate
  radio.setPayloadSize(32);                 // set the data packet size
  radio.setPALevel(RF24_PA_HIGH);           // set the transmission signal power level (try RF24_PA_HIGH)
  radio.openReadingPipe(1, 0x7878787878LL); // open the pipe with the transmitter's ID
  radio.setChannel(0x4C);                   // set the channel for wireless communication
  radio.startListening();                   // start listening on the opened pipe
  
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
}


void loop() {
  if (radio.available()) {
    radio.read(&car_value, sizeof(car_value));

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
  }
}

void roofLightBar(int option){
  digitalWrite(ledLight8, option);
  digitalWrite(ledLight9, option);
  digitalWrite(ledLight10, option);
  digitalWrite(ledLight11, option);
}

void bumperLight(int option) {
  digitalWrite(ledLight1, option);
  digitalWrite(ledLight2, option);
}

void dayLight(int option) {
  digitalWrite(ledLight3, option);
  digitalWrite(ledLight12, option);
  digitalWrite(ledLight13, option);
  digitalWrite(ledLight14, option);
  digitalWrite(ledLight15, option);
}

void blinkers(int sideLeft, int sideRight) {
  if (sideLeft == 1) {
    digitalWrite(ledLight16, HIGH);
    digitalWrite(ledLight6, HIGH);
    delay(500);
    digitalWrite(ledLight16, LOW);
    digitalWrite(ledLight6, LOW);
    delay(500);
  } else {
    digitalWrite(ledLight16, LOW);
    digitalWrite(ledLight6, LOW);
  }

  if (sideRight == 1) {
    digitalWrite(ledLight17, HIGH);
    digitalWrite(ledLight7, HIGH);
    delay(500);
    digitalWrite(ledLight17, LOW);
    digitalWrite(ledLight7, LOW);
    delay(500);
  } else {
    digitalWrite(ledLight17, LOW);
    digitalWrite(ledLight7, LOW);
  }
}