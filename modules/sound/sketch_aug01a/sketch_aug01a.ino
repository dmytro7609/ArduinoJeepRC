#include <SoftwareSerial.h>

#define ARDUINO_RX 19//should connect to TX of the Serial MP3 Player module
#define ARDUINO_TX 18//connect to RX of the module
SoftwareSerial myMP3(ARDUINO_RX, ARDUINO_TX);

#define JOYSTICK A0 // керування напрямком руху

float value;

void setup() {
  Serial.begin(9600);
  Serial.println("UARTSerialV1");
  myMP3.begin(9600);
  delay(500);//allow everything to settle down
  //first we need to select the TF Card
  //Manual says the code for this is 7E 03 35 01 EF
  myMP3.write(0x7E); //start of instruction
  myMP3.write(0x03); //number of bytes excluding start and end byte
  myMP3.write(0x35);  //Command
  myMP3.write(0x01);  //data..usually 1 byte
  myMP3.write(0xEF);  //end byte
  delay(20);
  myMP3.write(0x7E); //start of instruction
  myMP3.write(0x03); //number of bytes excluding start and end byte
  myMP3.write(0x31);  //Command
  myMP3.write(0x0F);  //data..usually 1 byte
  myMP3.write(0xEF);  //end byte
  delay(20);
  Serial.println("Play sound 1");
  //Now lets Play the first song 7E 04 41 00 01 EF
  myMP3.write(0x7E);  //start of instruction
  myMP3.write(0x04);  //number of bytes excluding start and end byte
  myMP3.write(0x41);  //command
  //next line is the first byte of data 0x00 will mean file is in root directory
  myMP3.write((byte)0x00);//forces the value to be read as a byte, otherwise error occurs
  myMP3.write(0x01);//will be the first file on the card
  myMP3.write(0xEF); //end byte
  delay(3000);//wait a second
  myMP3.write(0x7E); //start of instruction
  myMP3.write(0x04); //number of bytes excluding start and end byte
  myMP3.write(0x41);  //Command
  myMP3.write((byte)0x00);//forces the value to be read as a byte, otherwise error occurs
  myMP3.write(0x02);  //end byte
  myMP3.write(0xEF);  //end byte
}

void loop() {
  // put your main code here, to run repeatedly:
  value=analogRead(JOYSTICK); // Ortalama değer
  value=map(value, 0, 1023, 0, 90);
  Serial.println(value);
}
