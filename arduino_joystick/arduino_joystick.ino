#include <ezButton.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(7, 8);
int car_value[9];
int invert_speed = 0;

#define JOYSTICKY A0 // steering control car_value[0]
#define JOYSTICKX A1 // speed control car_value[1]

ezButton BUTTON_FIRST(4);   // button on the left joystick that activates the left turn signal car_value[2]
ezButton BUTTON_SECOND(2);  // button on the right joystick that activates the right turn signal car_value[3]
ezButton TOGGLE_SWITCH1(7); // toggle switch to turn on/off motor sound car_value[4]
ezButton TOGGLE_SWITCH2(8); // toggle switch for roof light bar car_value[5]
ezButton TOGGLE_SWITCH3(6); // toggle switch for markers, brake lights and license plate light car_value[6]
ezButton TOGGLE_SWITCH4(5); // toggle switch low beam car_value[7]


void setup() {
  Serial.begin(9600);

  radio.begin();                         // initialize the operation of the NRF24L01 module
  radio.setChannel(115);                 // set the channel for wireless communication
  radio.setDataRate(RF24_1MBPS);         // set the data transmission rate
  radio.setPALevel(RF24_PA_HIGH);        // set the transmission signal power level (try RF24_PA_MAX)
  radio.setRetries(5, 15);               // set the number and intervals of retries when sending data (retries are disabled)
  radio.setPayloadSize(18);              // setting the packet size (18 bytes = 9 elements of type int)
  radio.openWritingPipe(0x7878787878LL); // set the address for data transmission
  radio.stopListening();                 // switch the module to transmitter mode
}

void loop() {
  BUTTON_FIRST.loop();
  BUTTON_SECOND.loop();
  TOGGLE_SWITCH1.loop();
  TOGGLE_SWITCH2.loop();
  TOGGLE_SWITCH3.loop();
  TOGGLE_SWITCH4.loop();

  invert_speed = 1023 - analogRead(JOYSTICKX);
  car_value[0] = analogRead(JOYSTICKY);
  car_value[1] = constrain(invert_speed, 86, 660);

  if (BUTTON_FIRST.isPressed()) {
    car_value[2] = (car_value[2] == 0) ? 1 : 0;
  }

  if (BUTTON_SECOND.isPressed()) {
    car_value[3] = (car_value[3] == 0) ? 1 : 0;
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

  // Check the values when reversing and program the logic for turning on the LEDs (car_value[8])

  radio.write(&car_value, sizeof(car_value));
}