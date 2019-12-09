#include <Button.h>

Button button1(2); // Connect your button between pin 3 and GND

unsigned long previousMillis = 0;
void setup() {
  button1.begin();
  Serial.begin(9600);
  pinMode(10, OUTPUT);
}

void loop() {
  int vibrationState;
  if (button1.pressed()) {
    vibrationState = 1;
    previousMillis = millis();
  }
  if (vibrationState == 1 && millis() - previousMillis >= 1000) {
    vibrationState = 0;
  }
  Serial.println(vibrationState);
  if (vibrationState == 1) {
    digitalWrite(10, HIGH);
  }else{
    digitalWrite(10, LOW);
  }

}
