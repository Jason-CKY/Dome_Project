#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define NUMPIXELS 100
#define NUMPLAYERS 4
Adafruit_NeoPixel strip_cart = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// neopixel animation without delay
unsigned long pixelsInterval = 50; // the time we need to wait
unsigned long colorWipePreviousMillis = 0;
unsigned long theaterChasePreviousMillis = 0;
unsigned long theaterChaseRainbowPreviousMillis = 0;
unsigned long rainbowPreviousMillis = 0;
unsigned long rainbowCyclesPreviousMillis = 0;

int theaterChaseQ = 0;
int theaterChaseRainbowQ = 0;
int theaterChaseRainbowCycles = 0;
int rainbowCycles = 0;
int rainbowCycleCycles = 0;

uint16_t currentPixel = 0;// what pixel are we operating on

/* detection
  RED:    400-600 Resistor: 43k
  GREEN:  100-190 Resistor: 5.1k
  BLUE:   191-240 Resistor: 10k
  YELLOW: 600-740 Resistor: 95k
*/

int inputs[4] = {A0, A1, A2, A3};
int res_range[4][2] = { {400, 600}, {230, 300}, {90, 120}, {150, 200} };
int led_pos[4][2] = { {43, 48}, {49, 54}, {55, 61}, {62, 67} };
uint32_t player_colors[4];
int flags[4] = {0}; // 0 for not detected, 1 for detected

void setup() {
  // put your setup code here, to run once:
  // Neopixel setup
  strip_cart.begin();
  strip_cart.show(); // Initialize all pixels to 'off'
  //  rainbow(20, strip_cart);
  //  rainbowCycle(20, strip_cart);
  //  theaterChaseRainbow(50, &strip_cart);

  // detection setup
  Serial.begin(9600);
  for (int i = 0; i < NUMPLAYERS; i++) {
    pinMode(inputs[i], INPUT_PULLUP);
  }

  player_colors[0] = strip_cart.Color(255, 0, 0);
  player_colors[1] = strip_cart.Color(0, 255, 0);
  player_colors[2] = strip_cart.Color(0, 0, 255);
  player_colors[3] = strip_cart.Color(255, 255, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  clearArray(flags, NUMPLAYERS);
  if ((unsigned long)(millis() - rainbowPreviousMillis) >= pixelsInterval) {
    rainbowPreviousMillis = millis();
    rainbow(strip_cart);
  }
  for (int pos = 0; pos < NUMPLAYERS; pos++) {
    int reading = analogRead(inputs[pos]);
    // if(pos==0) Serial.println(reading);
    for (int player = 0; player < NUMPLAYERS; player++) {
      if (reading > res_range[player][0] && reading < res_range[player][1]) {
        for (int k = led_pos[pos][0]; k <= led_pos[pos][1]; k++) {
          strip_cart.setPixelColor(k, player_colors[player]);
        }
        flags[player] = 1;
      }
    }
  }
  strip_cart.show();
  Serial.print("Detected Players: "); printArray(flags, NUMPLAYERS);
}

void printArray(int arr[], int _size) {
  Serial.print("[");
  for (int i = 0; i < _size - 1; i++) {
    Serial.print(arr[i]); Serial.print(", ");
  }
  Serial.print(arr[_size - 1]); Serial.println("]");
}

void clearArray(int arr[], int _size) {
  for (int i = 0; i < _size; i++) {
    arr[i] = 0;
  }
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, Adafruit_NeoPixel &pixels) {
  pixels.setPixelColor(currentPixel, c);
  pixels.show();
  currentPixel++;
  if (currentPixel == NUMPIXELS) {
    currentPixel = 0;
  }
}

void rainbow(Adafruit_NeoPixel &pixels) {
  for (uint16_t i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, Wheel((i + rainbowCycles) & 255, pixels));
  }
  //  pixels.show();
  rainbowCycles++;
  if (rainbowCycles >= 256) rainbowCycles = 0;
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(Adafruit_NeoPixel &pixels) {
  uint16_t i;

  //for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
  for (i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + rainbowCycleCycles) & 255, pixels));
  }
  pixels.show();

  rainbowCycleCycles++;
  if (rainbowCycleCycles >= 256 * 5) rainbowCycleCycles = 0;
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, Adafruit_NeoPixel &pixels) {
  for (int i = 0; i < pixels.numPixels(); i = i + 3) {
    pixels.setPixelColor(i + theaterChaseQ, c);  //turn every third pixel on
  }
  pixels.show();
  for (int i = 0; i < pixels.numPixels(); i = i + 3) {
    pixels.setPixelColor(i + theaterChaseQ, 0);      //turn every third pixel off
  }
  theaterChaseQ++;
  if (theaterChaseQ >= 3) theaterChaseQ = 0;
}


//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(Adafruit_NeoPixel &pixels) {
  for (int i = 0; i < pixels.numPixels(); i = i + 3) {
    pixels.setPixelColor(i + theaterChaseRainbowQ, Wheel( (i + theaterChaseRainbowCycles) % 255, pixels)); //turn every third pixel on
  }

  pixels.show();
  for (int i = 0; i < pixels.numPixels(); i = i + 3) {
    pixels.setPixelColor(i + theaterChaseRainbowQ, 0);      //turn every third pixel off
  }
  theaterChaseRainbowQ++;
  theaterChaseRainbowCycles++;
  if (theaterChaseRainbowQ >= 3) theaterChaseRainbowQ = 0;
  if (theaterChaseRainbowCycles >= 256) theaterChaseRainbowCycles = 0;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos, Adafruit_NeoPixel &pixels) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
