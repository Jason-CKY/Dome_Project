#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Button.h>
#define TRIG_BUTTON 4
#define RF_COMMAND 'P'

Button triggerButton(TRIG_BUTTON); // Connect your button between TRIG_BUTTON and GND
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  triggerButton.begin();
}
void loop() {
  if (triggerButton.pressed()) {
    char command = RF_COMMAND;
    radio.write(&command, sizeof(command));
    Serial.println("Sent command");
  }
}
