#include <Button.h>
#define VIBRATION_MOTOR 6
unsigned long previousMillis = 0;
int vibrationState = LOW;
#define TRIG_BUTTON_1 2
Button triggerButton_1(TRIG_BUTTON_1); // Connect your button between TRIG_BUTTON and GND

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(VIBRATION_MOTOR, OUTPUT);
  triggerButton_1.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(triggerButton_1.pressed()){
    vibrateDuration(previousMillis, 2000, vibrationState);
    //digitalWrite(VIBRATION_MOTOR, HIGH);
    vibrationState = HIGH;
    previousMillis = millis();
    
  }
  if(vibrationState == HIGH && millis() - previousMillis > 2000){
    vibrationState = LOW;
  }

  digitalWrite(VIBRATION_MOTOR, vibrationState);
}
