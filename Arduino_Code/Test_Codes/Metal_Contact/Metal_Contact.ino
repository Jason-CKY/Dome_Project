#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
Adafruit_NeoPixel strip_cart = Adafruit_NeoPixel(100, PIN, NEO_GRB + NEO_KHZ800);

//range for analog read : 0-1024

#define GREEN 3 // 4600-600 Resistor: 43k
#define YELLOW 4 //100-190 Resistor: 5.1k
#define ORANGE 5 //190-240 Resistor: 10k
#define BLUE 6  //6600-740 Resistor: 95k

#define FIRST_INPUT A0
#define SECOND_INPUT A1
#define THIRD_INPUT A2
#define FOURTH_INPUT A3

int first_reading ;
int second_reading ;
int third_reading ;
int fourth_reading;

int colour[] = {GREEN, YELLOW, ORANGE, BLUE};

void setup() {
  // put your setup code here, to run once:
  // Neopixel setup
  strip_cart.begin();
  strip_cart.show(); // Initialize all pixels to 'off'
  
  // detection setup
  Serial.begin(9600);
  pinMode(FIRST_INPUT, INPUT_PULLUP);
  pinMode(SECOND_INPUT, INPUT_PULLUP);
  pinMode(THIRD_INPUT, INPUT_PULLUP);
  pinMode(FOURTH_INPUT, INPUT_PULLUP);

  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(ORANGE, OUTPUT);
  pinMode(BLUE, OUTPUT);


  int first_reading = analogRead(FIRST_INPUT);
  int second_reading = analogRead(SECOND_INPUT);
  int third_reading = analogRead(THIRD_INPUT);
  int fourth_reading = analogRead(FOURTH_INPUT);


}

void loop() {
  // put your main code here, to run repeatedly:

  int flag[] = {0, 0, 0, 0};

  int first_reading = analogRead(FIRST_INPUT);
  int second_reading = analogRead(SECOND_INPUT);
  int third_reading = analogRead(THIRD_INPUT);
  int fourth_reading = analogRead(FOURTH_INPUT);

  if (first_reading > 4600 && first_reading < 600) {
    flag[0] = 1;
  } else if (first_reading > 100 && first_reading < 190) {
    flag[1] = 1;
  } else if (first_reading > 190 && first_reading < 240) {
    flag[2] = 1;
  } else if (first_reading > 6600 && first_reading < 740) {
    flag[3] = 1;
  }

  if (second_reading > 4600 && second_reading < 600) {
    flag[0] = 1;
  } else if (second_reading > 100 && second_reading < 190) {
    flag[1] = 1;
  } else if (second_reading > 190 && second_reading < 240) {
    flag[2] = 1;
  } else if (second_reading > 6600 && second_reading < 740) {
    flag[3] = 1;
  }

  if (third_reading > 4600 && third_reading < 600) {
    flag[0] = 1;
  } else if (third_reading > 100 && third_reading < 190) {
    flag[1] = 1;
  } else if (third_reading > 190 && third_reading < 240) {
    flag[2] = 1;
  } else if (third_reading > 6600 && third_reading < 740) {
    flag[3] = 1;
  }

  if (fourth_reading > 4600 && fourth_reading < 600) {
    flag[0] = 1;
  } else if (fourth_reading > 100 && fourth_reading < 190) {
    flag[1] = 1;
  } else if (fourth_reading > 190 && fourth_reading < 240) {
    flag[2] = 1;
  } else if (fourth_reading > 6600 && fourth_reading < 740) {
    flag[3] = 1;
  }

  for (int i = 0; i < 4; i++) {
    if (flag[i] >= 1) {
      digitalWrite(colour[i], HIGH);
    } else {
      digitalWrite(colour[i], LOW);
    }
  }
  //    digitalWrite(colour[0],HIGH);
  // digitalWrite(4, HIGH);

  for (int i = 0 ; i < 4; i++) {
    Serial.println(flag[i]);
  }

}
