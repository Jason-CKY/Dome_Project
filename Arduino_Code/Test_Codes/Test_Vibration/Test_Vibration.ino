#define VIBRATION_MOTOR 4
unsigned long previousMillis = 0;
int vibrationState = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    char input = Serial.read();
    if(input == 'c'){
      vibrateDuration(previousMillis, 2000, vibrationState);
    }
  }
}

void vibrateDuration(unsigned long prevTime, unsigned long duration, int vibrationState){
  if(vibrationState == HIGH &&  millis() - prevTime >= duration){
    prevTime = millis();
    vibrationState = LOW;
  }else if(vibrationState == LOW){
    vibrationState = HIGH;
  }
  digitalWrite(VIBRATION_MOTOR, vibrationState);
}
