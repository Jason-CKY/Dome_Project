#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define LED_PIN 7
#define RF_COMMAND 'P'

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
int ledState = LOW;
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  pinMode(LED_PIN, OUTPUT);
}
void loop() {
  ledState = digitalRead(LED_PIN);
  if (radio.available())
  {
    char command = '';
    radio.read(&command, sizeof(command));
    if(command == RF_COMMAND){
      if(ledState == LOW){
        digitalWrite(LED_PIN, HIGH);
      }else{
        digitalWrite(LED_PIN, LOW);
      }
    }
  }
  
}
