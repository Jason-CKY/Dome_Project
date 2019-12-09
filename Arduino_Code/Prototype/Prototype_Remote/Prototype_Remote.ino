#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Button.h>
#define TRIG_BUTTON_1     2
#define TRIG_BUTTON_2     9
#define RF_COMMAND_1 '1'
#define RF_COMMAND_2 '2'

Button triggerButton_1(TRIG_BUTTON_1); // Connect your button between TRIG_BUTTON and GND
Button triggerButton_2(TRIG_BUTTON_2);
#define RF_CE             7
#define RF_CSN            8
RF24 radio(RF_CE, RF_CSN); // CE, CSN
const byte address[6] = "00001";

/* accelerator initialisation */
#include <Wire.h>
#include <ADXL345.h>


#define SAMPLES           20
#define MAX_THRESHOLD     50 // max change limit to be considered shaking
ADXL345 adxl; //variable adxl is an instance of the ADXL345 library

int x, y, z;
int x_prev, y_prev, z_prev;
int averageAcc_Arr[SAMPLES] = {0};
int currentAcc_Index = 0;

/* vibration motor initialisation */
#define VIBRATION_MOTOR     10
#define VIBRATION_DURATION  500
unsigned long previousMillis = 0;
unsigned long rfPreviousMillis = 0;
unsigned long clickDecayMillis = 0;
int vibrationState = LOW;
int clickSpeed = 0;

/* RGB LED setup */
#define LED_R 3
#define LED_G 5
#define LED_B 6
#define COMMON_ANODE 'A'
#define COMMON_CATHODE 'C'

#define MAX_VIBRATION_VAL 1100
#define CURRENT_PLAYER_NUM 2
void setup() {
  Serial.begin(9600);
  pinMode(VIBRATION_MOTOR, OUTPUT);
  /* RGB LED */
  {
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
  }
  /* RF calibration */
  {
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MAX);
    radio.setChannel(100);
    radio.stopListening();
    triggerButton_1.begin();
    triggerButton_2.begin();
  }

  // Accelerometer calibration:
  {
    adxl.powerOn();
    adxl.setActivityThreshold(75); //62.5mg per increment
    adxl.setInactivityThreshold(75); //62.5mg per increment
    adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?

    //look of activity movement on this axes - 1 == on; 0 == off
    adxl.setActivityX(1);
    adxl.setActivityY(1);
    adxl.setActivityZ(1);

    //look of inactivity movement on this axes - 1 == on; 0 == off
    adxl.setInactivityX(1);
    adxl.setInactivityY(1);
    adxl.setInactivityZ(1);

    adxl.readXYZ(&x_prev, &y_prev, &z_prev);
    adxl.readXYZ(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
    int xDiff = x_prev - x; // finding change in x
    int yDiff = y_prev - y; // finding change in y
    int zDiff = z_prev - z; // finding change in z
    int len = sizeof(averageAcc_Arr, averageAcc_Arr[0]);
    for (int i = 0; i < len; i++) {
      averageAcc_Arr[i] = getShakingIntensity(xDiff, yDiff, zDiff);
    }
  }

}
void loop() {
  int currentIntensity, averageIntensity;
  double roll, pitch;
  
  /* Accelerator calculations */
  {
    adxl.readXYZ(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
    int xDiff = x_prev - x; // finding change in x
    int yDiff = y_prev - y; // finding change in y
    int zDiff = z_prev - z; // finding change in z

    currentIntensity = getShakingIntensity(xDiff, yDiff, zDiff);
    averageIntensity = getMovingAverage(currentIntensity, averageAcc_Arr, &currentAcc_Index);
    Serial.print(averageIntensity); Serial.print("                    ");
    averageIntensity = averageIntensity > MAX_VIBRATION_VAL ? MAX_VIBRATION_VAL : averageIntensity;
    roll = atan(y / sqrt(pow(x, 2) + pow(z, 2))) * 180 / PI;         // rotation around X-Axis
    pitch = atan(-1 * x / sqrt(pow(y, 2) + pow(z, 2))) * 180 / PI;   // rotation around Y-Axis
    //    Serial.print("Intensity: "); Serial.print(averageIntensity); Serial.print(" ");
    //    Serial.print("Roll: "); Serial.print(roll); Serial.print(" ");
    //    Serial.print("Pitch: "); Serial.print(pitch); Serial.print(" ");
    //    Serial.println();

    x_prev = x;
    y_prev = y;
    z_prev = z;
  }
  int mappedIntensity = map(averageIntensity, 0, MAX_VIBRATION_VAL, 0, 9);
  //  Serial.print("mapped value"); Serial.print(mappedIntensity);
  //  Serial.println();

  static bool startTimer = false;
  static int isBtnPressed = 0;
  static int checkSpeed;
  if (triggerButton_1.pressed()) {
    checkSpeed = millis() - previousMillis;
    vibrationState = 1;
    isBtnPressed = 1;
    previousMillis = millis();
    if (checkSpeed < 240) {
      clickSpeed = 9;
    } else if (checkSpeed < 280) {
      clickSpeed = 8;
    } else if (checkSpeed < 380) {
      clickSpeed = 7;
    } else if (checkSpeed < 420) {
      clickSpeed = 6;
    } else if (checkSpeed < 460) {
      clickSpeed = 5;
    } else if (checkSpeed < 500) {
      clickSpeed = 4;
    } else if (checkSpeed < 600) {
      clickSpeed = 3;
    } else if (checkSpeed < 700) {
      clickSpeed = 2;
    } else {
      clickSpeed = 1;
    }
  }
  //  Serial.println(clickSpeed);
  /* Send status of remote to the mega */
  {
    String text = "";
    text += CURRENT_PLAYER_NUM;
    text += clickSpeed;                // button state
    text += mappedIntensity;
    char sendText[3];
    text.toCharArray(sendText, 4);
    Serial.println(sendText);

    if (millis() - rfPreviousMillis >= CURRENT_PLAYER_NUM * 3) {
      radio.write(&sendText, sizeof(sendText));
      rfPreviousMillis = millis();
    }
  }

  switch (CURRENT_PLAYER_NUM) {
    case 1:
      RGB_write(0, 0, 255, COMMON_ANODE);
      break;
    case 2:
      RGB_write(255, 255, 0, COMMON_ANODE);
      break;
    case 3:
      RGB_write(0, 255, 0, COMMON_ANODE);   // due to hardware issues, actual color is (255, 125, 0) instead
      break;
    case 4:
      RGB_write(255, 0, 0, COMMON_ANODE);
      break;
  }

  if (millis() - clickDecayMillis >= 100) {
    clickDecayMillis = millis();
    clickSpeed -= 1;
    clickSpeed = clickSpeed < 0 ? 0 : clickSpeed;
  }
  if (vibrationState == HIGH && millis() - previousMillis > VIBRATION_DURATION) {
    vibrationState = 0;
  }

  if (vibrationState == 1) {
    digitalWrite(VIBRATION_MOTOR, HIGH);
  } else {
    digitalWrite(VIBRATION_MOTOR, LOW);
  }
}

int getShakingIntensity(int xDiff, int yDiff, int zDiff) {
  int intensity = 0;
  intensity += (abs(xDiff) + abs(yDiff) + abs(zDiff));
  return intensity;
}

bool isShaking(int xDiff, int yDiff, int zDiff) {
  return (abs(xDiff) + abs(yDiff) + abs(zDiff)) > MAX_THRESHOLD;
}


int getMovingAverage(int currentVal, int* averageArr, int* currentIndex) {
  int len = SAMPLES;
  averageArr[currentAcc_Index] = currentVal;
  int sum = 0;
  for (int i = 0; i < len; i++) {
    sum += averageArr[i];
  }
  // update currentAcc_Index
  //  Serial.println("****************************************************************");
  //  Serial.print("size of averageArr: "); Serial.print(sizeof(averageArr)); Serial.print(" size of averageArr[0]"); Serial.println(sizeof(averageArr[0]));
  //  Serial.print("len: "); Serial.print(len);  Serial.print(" Current Index = "); Serial.print(*currentIndex); Serial.print(" Sum: "); Serial.print(sum); Serial.print(" Moving ave: "); Serial.println(sum/len);
  //  Serial.println("****************************************************************");
  *currentIndex = *currentIndex >= len ? 0 : *currentIndex + 1;
  return sum / len;
}

void RGB_write(int r, int g, int b, char mode) {
  int rVal = (r < 0) ? 0 : (r > 255) ? 255 : r;
  int gVal = (g < 0) ? 0 : (g > 255) ? 255 : g;
  int bVal = (b < 0) ? 0 : (b > 255) ? 255 : b;
  rVal = (mode == COMMON_ANODE) ? 255 - rVal : rVal;
  gVal = (mode == COMMON_ANODE) ? 255 - gVal : gVal;
  bVal = (mode == COMMON_ANODE) ? 255 - bVal : bVal;
  analogWrite(LED_R, rVal);
  analogWrite(LED_G, gVal);
  analogWrite(LED_B, bVal);
}
