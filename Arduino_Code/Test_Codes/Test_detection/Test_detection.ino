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
  lightUpWhenReturn();     // detect if remote is placed in back and light up relevant LED if correct
  Serial.println("loop");
}

void lightUpWhenReturn() {
  int reading1 = analogRead(GREEN_INPUT);
  int reading2 = analogRead(YELLOW_INPUT);
  int reading3 = analogRead(ORANGE_INPUT);
  int reading4 = analogRead(BLUE_INPUT);
  //for the 4 readings
  Serial.println(reading1);
  Serial.println(reading2);
  Serial.println(reading3);
  Serial.println(reading4);

  //create array
  int lightup[4] = {0};

  if(checkInput(reading1) == GREEN){
    lightup[0]= 1;
  } else if (checkInput(reading1) == YELLOW){
    lightup[1] = 1 ;
  }else if (checkInput(reading1) == ORANGE){
    lightup[2] = 1;
  } else if (checkInput(reading1) == BLUE){
    lightup[3] = 1;
  } 

  if(checkInput(reading2) == GREEN){
    lightup[0]= 1;
  } else if (checkInput(reading2) == YELLOW){
    lightup[1] = 1 ;
  }else if (checkInput(reading2) == ORANGE){
    lightup[2] = 1;
  } else if (checkInput(reading2) == BLUE){
    lightup[3] = 1;
  } 

  if(checkInput(reading3) == GREEN){
    lightup[0]= 1;
  } else if (checkInput(reading3) == YELLOW){
    lightup[1] = 1 ;
  }else if (checkInput(reading3) == ORANGE){
    lightup[2] = 1;
  } else if (checkInput(reading3) == BLUE){
    lightup[3] = 1;
  } 

  if(checkInput(reading4) == GREEN){
    lightup[0]= 1;
  } else if (checkInput(reading4) == YELLOW){
    lightup[1] = 1 ;
  }else if (checkInput(reading4) == ORANGE){
    lightup[2] = 1;
  } else if (checkInput(reading4) == BLUE){
    lightup[3] = 1;
  } 

    if (lightup[0] == 1){
      digitalWrite(GREEN, HIGH);
    } else {
      digitalWrite(GREEN, LOW);
    }
    if (lightup[1] == 1) {
      digitalWrite(YELLOW, HIGH);
    } else {
      digitalWrite(YELLOW, LOW);
    }
    if (lightup[2] == 1) {
      digitalWrite(ORANGE, HIGH);
    }  else {
      digitalWrite(ORANGE, LOW);
    }
    if (lightup[3] == 1) {
      digitalWrite(BLUE, HIGH);
    } else {
      digitalWrite(BLUE, LOW);
    }  
}

int checkInput(int reading) {

  if (reading > 260 && reading < 270) {
    return GREEN;
  } else if (reading > 220 && reading < 230) {
    return YELLOW;
  } else if (reading > 20 && reading < 30) {
    return ORANGE;
  } else if (reading > 35 && reading < 45) {
    return BLUE;
  } else {
    return 0;
  }
}
