#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define LED_1_PIN 3
#define LED_2_PIN 4
#define RF_COMMAND_1 '1'
#define RF_COMMAND_2 '2'

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
// int ledState_1 = LOW;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
}
void loop() {
  int ledState_1 = digitalRead(LED_1_PIN);
  int ledState_2 = digitalRead(LED_2_PIN);
  if (radio.available())
  {
    char command;
    radio.read(&command, sizeof(command));
    if(command == RF_COMMAND_1){
      if(ledState_1 == LOW){
        digitalWrite(LED_1_PIN, HIGH);
      }else{
        digitalWrite(LED_1_PIN, LOW);
      }
    }else if(command == RF_COMMAND_2){
      if(ledState_2 == LOW){
        digitalWrite(LED_2_PIN, HIGH);
      }else{
        digitalWrite(LED_2_PIN, LOW);
      }
    }
  }
  
}
