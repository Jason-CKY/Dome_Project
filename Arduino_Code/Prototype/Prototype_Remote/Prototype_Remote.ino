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
int vibrationState = LOW;


/* RGB LED setup */
#define LED_R 3
#define LED_G 5 
#define LED_B 6


void setup() {
  Serial.begin(9600);
  pinMode(VIBRATION_MOTOR, OUTPUT);
  /* RF calibration */
  {
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
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
  /* Scanning for any received RF Message from mega */
  if(radio.available()){
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    vibrationState = HIGH;
    previousMillis = millis();
  }
  /* Accelerator calculations */
  {
    adxl.readXYZ(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
    int xDiff = x_prev - x; // finding change in x
    int yDiff = y_prev - y; // finding change in y
    int zDiff = z_prev - z; // finding change in z

    int currentIntensity = getShakingIntensity(xDiff, yDiff, zDiff);
    int averageIntensity = getMovingAverage(currentIntensity, averageAcc_Arr, &currentAcc_Index);
    double roll = atan(y / sqrt(pow(x, 2) + pow(z, 2))) * 180 / PI;         // rotation around X-Axis
    double pitch = atan(-1 * x / sqrt(pow(y, 2) + pow(z, 2))) * 180 / PI;   // rotation around Y-Axis
//    Serial.print("Intensity: "); Serial.print(averageIntensity); Serial.print(" ");
//    Serial.print("Roll: "); Serial.print(roll); Serial.print(" ");
//    Serial.print("Pitch: "); Serial.print(pitch); Serial.print(" ");
//    Serial.println();

    x_prev = x;
    y_prev = y;
    z_prev = z;
  }

  /* Send status of remote to the mega */
  if (triggerButton_1.pressed()) {
    radio.stopListening();
    const char text[] = "Hello World";
    radio.write(&text, sizeof(text));
    radio.startListening();
    Serial.print("Sending: "); Serial.println(text);
    vibrationState = HIGH;
    previousMillis = millis();
  }

  if(vibrationState == HIGH && millis() - previousMillis > VIBRATION_DURATION){
    vibrationState = LOW;
  }
  digitalWrite(VIBRATION_MOTOR, vibrationState);
}

//void sendRFMessage(char msg){
//  radio.stopListening();
//  radio.write(&msg, sizeof(msg));
//  Serial.print("sent 0x"); Serial.print(msg, HEX);
//  Serial.println();
//  radio.startListening();
//}

int getShakingIntensity(int xDiff, int yDiff, int zDiff) {
  int intensity = 0;
  intensity += (abs(xDiff) + abs(yDiff) + abs(zDiff));
  return intensity;
}

bool isShaking(int xDiff, int yDiff, int zDiff) {
  return (abs(xDiff) + abs(yDiff) + abs(zDiff)) > MAX_THRESHOLD;
}


int getMovingAverage(int currentVal, int* averageArr, int* currentIndex) {
  int len = sizeof(averageArr) / sizeof(averageArr[0]);
  averageArr[currentAcc_Index] = currentVal;
  int sum = 0;
  for (int i = 0; i < len; i++) {
    sum += averageArr[i];
  }
  // update currentAcc_Index
  *currentIndex = *currentIndex >= len ? 0 : *currentIndex + 1;
  return sum / len;
}
