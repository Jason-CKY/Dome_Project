#include <Adafruit_NeoPixel.h>

#define PIXELSPIN   6
#define NUMPIXELS   100
#define CALIBRATIONTIME 20000

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELSPIN, NEO_GRB + NEO_KHZ800);

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
int currentAnimation = 0;
void setup() {
  currentPixel = 0;

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show(); // This sends the updated pixel color to the hardware.

}

void loop () {
  currentAnimation = currentAnimation > 4? 0:currentAnimation;
  if (currentAnimation == 0) {
    if ((unsigned long)(millis() - colorWipePreviousMillis) >= pixelsInterval) {
      colorWipePreviousMillis = millis();
      colorWipe(pixels.Color(0, 255, 125));
    }
  } else if (currentAnimation == 1) {

    if ((unsigned long)(millis() - theaterChasePreviousMillis) >= pixelsInterval) {
      theaterChasePreviousMillis = millis();
      theaterChase(pixels.Color(127, 127, 127)); // White
    }
  } else if (currentAnimation == 2) {
    if ((unsigned long)(millis() - theaterChaseRainbowPreviousMillis) >= pixelsInterval) {
      theaterChaseRainbowPreviousMillis = millis();
      theaterChaseRainbow();
    }
  } else if (currentAnimation == 3) {
    if ((unsigned long)(millis() - rainbowPreviousMillis) >= pixelsInterval) {
      rainbowPreviousMillis = millis();
      rainbow();
    }
  } else if (currentAnimation == 4) {
    if ((unsigned long)(millis() - rainbowCyclesPreviousMillis) >= pixelsInterval) {
      rainbowCyclesPreviousMillis = millis();
      rainbowCycle();
    }
  }

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c) {
  if (currentPixel == NUMPIXELS) {
    currentPixel = 0;
    currentAnimation ++;
    return;
  }
  pixels.setPixelColor(currentPixel, c);
  pixels.show();
  currentPixel++;

}

void rainbow() {
  if (rainbowCycles >= 256) {
    rainbowCycles = 0;
    currentAnimation ++;
    return;
  }
  for (uint16_t i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, Wheel((i + rainbowCycles) & 255));
  }
  pixels.show();
  rainbowCycles++;
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle() {
  if (rainbowCycleCycles >= 256 * 5) {
    rainbowCycleCycles = 0;
    currentAnimation++;
    return;
  }
  uint16_t i;

  //for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
  for (i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + rainbowCycleCycles) & 255));
  }
  pixels.show();

  rainbowCycleCycles++;

}

//Theatre-style crawling lights.
void theaterChase(uint32_t c) {
  if (theaterChaseQ >= 3) {
    theaterChaseQ = 0;
    currentAnimation++;
    return;
  }
  for (int i = 0; i < pixels.numPixels(); i = i + 3) {
    pixels.setPixelColor(i + theaterChaseQ, c);  //turn every third pixel on
  }
  pixels.show();
  for (int i = 0; i < pixels.numPixels(); i = i + 3) {
    pixels.setPixelColor(i + theaterChaseQ, 0);      //turn every third pixel off
  }
  theaterChaseQ++;

}


//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow() {
  if (theaterChaseRainbowCycles >= 256){
    theaterChaseRainbowCycles = 0;
    currentAnimation++;
    return;
  }
  for (int i = 0; i < pixels.numPixels(); i = i + 3) {
    pixels.setPixelColor(i + theaterChaseRainbowQ, Wheel( (i + theaterChaseRainbowCycles) % 255)); //turn every third pixel on
  }

  pixels.show();
  for (int i = 0; i < pixels.numPixels(); i = i + 3) {
    pixels.setPixelColor(i + theaterChaseRainbowQ, 0);      //turn every third pixel off
  }
  theaterChaseRainbowQ++;
  theaterChaseRainbowCycles++;
  if (theaterChaseRainbowQ >= 3) theaterChaseRainbowQ = 0;

}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
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
