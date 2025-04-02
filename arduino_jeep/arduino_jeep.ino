#include <RF24.h>
#include <Servo.h>
#include <Wire.h>
#include <SPI.h>

RF24 radio(9, 53);

Servo sWheels; // servo motor for turning wheels
Servo esc;     // motor control board

const int ledLight1 = 19;  // license plate LED
const int ledLight2 = 6;   // brake left light LED
const int ledLight3 = 7;   // brake right light LED
const int ledLight4 = 11;  // turn front left signal LED
const int ledLight5 = 12;  // turn front right signal LED
const int ledLight6 = 13;  // turn rear left signal LED
const int ledLight7 = 14;  // turn rear right signal LED
const int ledLight8 = 15;  // roof light bar 1 (from left) LED
const int ledLight9 = 16;  // roof light bar 2 (from left) LED
const int ledLight10 = 17; // roof light bar 3 (from left) LED
const int ledLight11 = 18; // roof light bar 4 (from left) LED
const int ledLight12 = 8;  // low beam left LED
const int ledLight13 = 10; // low beam right LED
const int ledLight14 = 3;  // marker light left LED
const int ledLight15 = 4;  // marker light right LED
const int ledLight16 = 20; // reverse left LED
const int ledLight17 = 21; // reverse right LED

int car_value[9];

int isReverseOn = 0; // !!! After programming the reverse light logic, change it here (car_value[8])
int isDRLOn = 0;
int isLowBeamOn = 0;
int isHighBeamOn = 0;
int isLeftSignalOn = 0;
int isRightSignalOn = 0;


void setup() {
  Serial.begin(9600);

  radio.begin();                            // initialize the operation of the NRF24L01 module
  radio.setChannel(115);                    // set the channel for wireless communication
  radio.setDataRate(RF24_1MBPS);            // set the data transmission rate
  radio.setPALevel(RF24_PA_HIGH);           // set the transmission signal power level (try RF24_PA_HIGH)
  radio.setPayloadSize(18);                 // setting the packet size (18 bytes = 9 elements of type int)
  radio.openReadingPipe(1, 0x7878787878LL); // open the pipe with the transmitter's ID
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

    if (isDRLOn != car_value[6]){
      isDRLOn = car_value[6];
      setDaytimeLights(isDRLOn);
    }
    if (isLowBeamOn != car_value[7]){
      isLowBeamOn = car_value[7];
      setLowBeam(isLowBeamOn);
    }
    if (isHighBeamOn != car_value[5]){
      isHighBeamOn = car_value[5];
      setHighBeam(isHighBeamOn);
    }
    if (isLeftSignalOn != car_value[2]){
      isLeftSignalOn = car_value[2];
      setLeftTurnSignal(isLeftSignalOn);
    }
    if (isRightSignalOn != car_value[3]){
      isRightSignalOn = car_value[3];
      setRightTurnSignal(isRightSignalOn);
    }
  }
}

void setDaytimeLights(int state){
  digitalWrite(ledLight1, state);
  digitalWrite(ledLight6, state);
  digitalWrite(ledLight7, state);
  digitalWrite(ledLight14, state);
  digitalWrite(ledLight15, state);

}

void setLowBeam(int state){
  digitalWrite(ledLight12, state);
  digitalWrite(ledLight13, state);
}

void setHighBeam(int state){
  digitalWrite(ledLight8, state);
  digitalWrite(ledLight9, state);
  digitalWrite(ledLight10, state);
  digitalWrite(ledLight11, state);
}

void setLeftTurnSignal(int state){
  digitalWrite(ledLight4, state);
  digitalWrite(ledLight6, state);
}

void setRightTurnSignal(int state){
  digitalWrite(ledLight5, state);
  digitalWrite(ledLight7, state);
}