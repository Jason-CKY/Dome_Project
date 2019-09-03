#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Button.h>
#define TRIG_BUTTON 3
#define RF_COMMAND_1 '1'
#define RF_COMMAND_2 '2'

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
    char command = RF_COMMAND_2;
    radio.write(&command, sizeof(command));
    Serial.println("Sent command");
  }
}
