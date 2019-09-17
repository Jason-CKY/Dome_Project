#include <Button.h>

Button button1(3); // Connect your button between pin 3 and GND


void setup() {
	button1.begin();
	Serial.begin(9600);
}

void loop() {
	if (button1.read() == Button::PRESSED){
		Serial.println("1");
	}
  else{
    Serial.println("0");
  }
  delay(50);
}
