#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define NUMPIXELS 100
#define NUMPLAYERS 4
Adafruit_NeoPixel strip_cart = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int theaterChaseQ = 0;
int theaterChaseRainbowQ = 0;
int theaterChaseRainbowCycles = 0;
int rainbowCycles = 0;
int rainbowCycleCycles = 0;

unsigned long theaterChasePreviousMillis = 0;
unsigned long pixelsInterval = 10;
unsigned long rfPreviousMillis = 0;
uint16_t currentPixel = 0;// what pixel are we operating on

// green, red, blue, yellow
int inputs[4] = {A0, A1, A2, A3};
int res_range[4][2] = { {850, 950}, {480, 570}, {40, 120}, {670, 750} };
int led_pos[4][2] = { {42, 45}, {49, 52}, {56, 59}, {62, 65} };
uint32_t player_colors[4];
int flags[4] = {1, 1, 1, 1}; // 0 for not detected, 1 for detected


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
int btnState[4] = {0};
int shakeState[4] = {0};
int id = 0;

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

  player_colors[0] = strip_cart.Color(0, 0, 255);
  player_colors[1] = strip_cart.Color(255, 255, 0);
  player_colors[2] = strip_cart.Color(0, 255, 0);
  player_colors[3] = strip_cart.Color(255, 0, 0);

  radio.begin();
  radio.setChannel(100);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:

  //  Serial.println(analogRead(A0));
  clearArray(flags, NUMPLAYERS, 1);
  if ((unsigned long)(millis() - theaterChasePreviousMillis) >= pixelsInterval) {
    theaterChasePreviousMillis = millis();
    strip_cart.setPixelColor(currentPixel, strip_cart.Color(0, 0, 0));
    currentPixel ++;
    for (int i = currentPixel; i < currentPixel + 3; i++) {
      strip_cart.setPixelColor(i, strip_cart.Color(100, 100, 100));
    }
    currentPixel = currentPixel >= NUMPIXELS ? 0 : currentPixel;
  }
  for (int pos = 0; pos < NUMPLAYERS; pos++) {
    int reading = analogRead(inputs[pos]);
    for (int player = 0; player < NUMPLAYERS; player++) {
      if (reading > res_range[player][0] && reading < res_range[player][1]) {
        for (int k = led_pos[pos][0]; k <= led_pos[pos][1]; k++) {
          strip_cart.setPixelColor(k, player_colors[player]);
        }
        flags[player] = 0;
      }
    }
  }
  strip_cart.show();
  //  Serial.print("Detected Players: "); printArray(flags, NUMPLAYERS);
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
//    Serial.println(text);
    if (text[0] == '1' || text[0] == '2' || text[0] == '3' || text[0] == '4') {
      id = (int)text[0] - 48;
      btnState[id - 1] = (int)text[1] - 48;
      shakeState[id - 1] = (int)text[2] - 48;
    }
  }
  if (millis() - rfPreviousMillis >= 20) {
    String output = "";
    for (int i = 0; i < 4; i++) {
      output += flags[i];
    } for (int j = 0; j < 4; j++) {
      if (btnState[j] >= 0) {
        output += btnState[j];
      } else {
        output += 0;
      }
    } for (int k = 0; k < 4; k++) {
      if (shakeState[k] <= 9 && shakeState[k] >= 0) {
        output += shakeState[k];
      } else {
        output += 0;
      }
    }
    output += 'e';
    Serial.println(output);
    clearArray(btnState, NUMPLAYERS, 0);
    rfPreviousMillis = millis();
  }
  digitalWrite(10, LOW);
}

void printArray(int arr[], int _size) {
  Serial.print("[");
  for (int i = 0; i < _size - 1; i++) {
    Serial.print(arr[i]); Serial.print(", ");
  }
  Serial.print(arr[_size - 1]); Serial.println("]");
}

void clearArray(int arr[], int _size, int default_val) {
  for (int i = 0; i < _size; i++) {
    arr[i] = default_val;
  }
}
