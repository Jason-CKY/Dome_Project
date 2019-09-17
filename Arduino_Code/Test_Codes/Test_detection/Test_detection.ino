//When analog pin from mega is connected to
//analog+resistor pin from nano,
//read output and verify
//range for analog read : 0-1024

#define GREEN 3 // 260-270
#define YELLOW 4 //220-230
#define ORANGE 5 //20-30
#define BLUE 6  //35-45

#define GREEN_INPUT A0
#define YELLOW_INPUT A1
#define ORANGE_INPUT A2
#define BLUE_INPUT A3

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(GREEN_INPUT, INPUT_PULLUP);
  pinMode(YELLOW_INPUT, INPUT_PULLUP);
  pinMode(ORANGE_INPUT, INPUT_PULLUP);
  pinMode(BLUE_INPUT, INPUT_PULLUP);

  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(ORANGE, OUTPUT);
  pinMode(BLUE, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  lightUpWhenCorrect();     // detect if remote is placed in correct order and light up relevant LED if correct
}


void lightUpWhenCorrect() {

  int green_reading = analogRead(GREEN_INPUT);
  int yellow_reading = analogRead(YELLOW_INPUT);
  int orange_reading = analogRead(ORANGE_INPUT);
  int blue_reading = analogRead(BLUE_INPUT);
  
  
  // Debug lines
  {
    Serial.print("Green reading = "); Serial.print(green_reading); Serial.print(" ");
    Serial.print("Yellow reading = "); Serial.print(yellow_reading); Serial.print(" ");
    Serial.print("Orange reading = "); Serial.print(orange_reading); Serial.print(" ");
    Serial.print("Blue reading = "); Serial.print(blue_reading); Serial.print(" ");
    Serial.println();
  }
  
  if (isCorrectInput(GREEN, green_reading)) {
    // light up green led
    digitalWrite(GREEN, HIGH);
  } else if (!isCorrectInput(GREEN, green_reading)) {
    // don't light up green led
    digitalWrite(GREEN, LOW);
  }

  if (isCorrectInput(YELLOW, yellow_reading)) {
    // light up yellow led
    digitalWrite(YELLOW, HIGH);
  } else if (!isCorrectInput(YELLOW, yellow_reading)) {
    // don't light up yellow led
    digitalWrite(YELLOW, LOW);
  }

  if (isCorrectInput(ORANGE, orange_reading)) {
    // light up orange led
    digitalWrite(ORANGE, HIGH);
  } else if (!isCorrectInput(ORANGE, orange_reading)) {
    // don't light up orange led
    digitalWrite(ORANGE, LOW);
  }

  if (isCorrectInput(BLUE, blue_reading)) {
    // light up blue led
    digitalWrite(BLUE, HIGH);
  } else if (!isCorrectInput(BLUE, blue_reading)) {
    // don't light up yellow led
    digitalWrite(BLUE, LOW);
  }
  
}

bool isCorrectInput(int colour, int reading) {
  if (colour == GREEN) {
    return reading > 260 && reading < 270;
  } else if (colour == YELLOW) {
    return reading > 220 && reading < 230;
  } else if (colour == ORANGE) {
    return reading > 20 && reading < 30;
  } else if (colour == BLUE) {
    return reading > 35 && reading < 45;
  }
}
