/*
  Arduino Wireless Communication Tutorial
        Example 1 - Receiver Code

  by Dejan Nedelkovski, www.HowToMechatronics.com

  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[][6] = {"00001", "00002"};
unsigned long prevTime = 0;
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address[0]);
  radio.openReadingPipe(1, address[1]);
  radio.setPALevel(RF24_PA_MIN);
}
void loop() {
  /* receiving RF from sender */
  radio.startListening();
  if (radio.available()){
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }

  /* Sending RF to sender */
  if(millis() - prevTime >= 1000){
    prevTime = millis();
    radio.stopListening();
    char command[32] = "Hello World from Receiver";
    radio.write(&command, sizeof(command));
    Serial.println("sent");
  }
}
