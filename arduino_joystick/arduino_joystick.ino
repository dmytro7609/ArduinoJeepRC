#include <ezButton.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>


RF24 radio(9, 10);
int car_value[8];
int orig_speed = 0;
int invert_speed = 0;
int left_side = 0;
int right_side = 0;

#define JOYSTICKX A1 // Speed control
#define JOYSTICKY A0 // Steering control

ezButton BUTTON_FIRST(4);   // Button that activates the left turn signal
ezButton BUTTON_SECOND(2);  // Button that activates the right turn signal
ezButton TOGGLE_SWITCH1(7); // Toggle switch to turn on/off motor sound
ezButton TOGGLE_SWITCH2(8); // Toggle switch for roof light bar
ezButton TOGGLE_SWITCH3(6); // Toggle switch for bumper lights
ezButton TOGGLE_SWITCH4(5); // Toggle switch for markers, low beam and brake lights

void setup() {
  Serial.begin(9600);

  radio.begin();                         // initialize the operation of the NRF24L01 module
  radio.setDataRate (RF24_1MBPS);        // set the data transmission rate
  radio.setPayloadSize(32);              // set the data packet size
  radio.setRetries(5, 15);               // set the number and intervals of retries when sending data (retries are disabled)
  radio.setPALevel(RF24_PA_HIGH);        // set the transmission signal power level (try RF24_PA_MAX)
  radio.openWritingPipe(0x7878787878LL); // set the address for data transmission
  radio.setChannel(0x4C);                // set the channel for wireless communication
  radio.stopListening();                 // switch the module to transmitter mode
}

void loop() {
  BUTTON_FIRST.loop();
  BUTTON_SECOND.loop();
  TOGGLE_SWITCH1.loop();
  TOGGLE_SWITCH2.loop();
  TOGGLE_SWITCH3.loop();
  TOGGLE_SWITCH4.loop();

  orig_speed = analogRead(JOYSTICKX);
  invert_speed = 1023 - orig_speed;
  car_value[0] = analogRead(JOYSTICKY);
  car_value[1] = constrain(invert_speed, 86, 660);

  if (BUTTON_FIRST.isPressed()) {
    left_side = (left_side == 0) ? 1 : 0;
    car_value[2] = left_side;
  }

  if (BUTTON_SECOND.isPressed()) {
    right_side = (right_side == 0) ? 1 : 0;
    car_value[3] = right_side;
  }

  if (TOGGLE_SWITCH1.isPressed()) {
    car_value[4] = 1;
  }
  else if (TOGGLE_SWITCH1.isReleased()) {
    car_value[4] = 0;
  }

  if (TOGGLE_SWITCH2.isPressed()) {
    car_value[5] = 1;
  }
  else if (TOGGLE_SWITCH2.isReleased()) {
    car_value[5] = 0;
  }

  if (TOGGLE_SWITCH3.isPressed()) {
    car_value[6] = 1;
  }
  else if (TOGGLE_SWITCH3.isReleased()) {
    car_value[6] = 0;
  }

  if (TOGGLE_SWITCH4.isPressed()) {
    car_value[7] = 1;
  }
  else if (TOGGLE_SWITCH4.isReleased()) {
    car_value[7] = 0;
  }

  radio.write(&car_value, sizeof(car_value));
}
