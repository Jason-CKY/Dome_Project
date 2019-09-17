#include <Button.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define NEOPIXEL_PIN 10
#define NUMPIXELS      3
#define BUTTON_PIN 2
#define LONG_PRESS_DURATION 1000

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
Button button(BUTTON_PIN);

unsigned long timeNow = 0;
unsigned long buttonTimer = 0;
bool startCounting = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  button.begin();
  pixels.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  //  Serial.print("Button Timer = ");
  //  Serial.println(buttonTimer);
  //  Serial.println("----------------------------------------------------------------------------------");
  //  Serial.print("Time Now = ");
  //  Serial.println(timeNow);
  //  Serial.println("----------------------------------------------------------------------------------");

  
  static int numPixel_lighted = 0;
  if (button.pressed()) // once a button has been pressed (only once)
    timeNow = millis();
  if (button.read() == Button::PRESSED)
    buttonTimer = millis();
  else if (button.read() == Button::RELEASED) {
    timeNow = buttonTimer;
    numPixel_lighted = 0;
    offAllLights();
  }

  if ( (buttonTimer - timeNow >= LONG_PRESS_DURATION)) {
//    Serial.println("1000s of pressing");
//    Serial.println("************************************************************************************");
    timeNow = buttonTimer;
    Serial.println(numPixel_lighted);
    numPixel_lighted = numPixel_lighted < NUMPIXELS ? numPixel_lighted + 1 : numPixel_lighted;
    for (int i = 0; i < numPixel_lighted; i++) {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // Moderately bright green color.
    }
  }

  pixels.show(); // This sends the updated pixel color to the hardware.
}


void offAllLights(){
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }  
}
