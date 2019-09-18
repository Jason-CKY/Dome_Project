#include <Wire.h>
#include <ADXL345.h>

/*Macros*/
#define SAMPLES 20
#define MAX_THRESHOLD 50 // max change limit to be considered shaking
ADXL345 adxl; //variable adxl is an instance of the ADXL345 library

int x, y, z;
int x_prev, y_prev, z_prev;
int averageAcc_Arr[SAMPLES] = {0};
int currentAcc_Index = 0;


void setup() {
  Serial.begin(9600);
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
  adxl.readXYZ(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
  int xDiff = x_prev - x; // finding change in x
  int yDiff = y_prev - y; // finding change in y
  int zDiff = z_prev - z; // finding change in z

  /*    Printing out x, y, z coordinates value              */
  // Serial.print("X: "); Serial.print(x); Serial.print("  ");
  // Serial.print("Y: "); Serial.print(y); Serial.print("  ");
  // Serial.print("Z: "); Serial.print(z); Serial.print("  ");

  int currentIntensity = getShakingIntensity(xDiff, yDiff, zDiff);
  int averageIntensity = getMovingAverage(currentIntensity);
  double roll = atan(y / sqrt(pow(x, 2) + pow(z, 2))) * 180 / PI;         // rotation around X-Axis
  double pitch = atan(-1 * x / sqrt(pow(y, 2) + pow(z, 2))) * 180 / PI;   // rotation around Y-Axis
  Serial.print("Intensity: "); Serial.print(averageIntensity); Serial.print(" ");
  Serial.print("Roll: "); Serial.print(roll); Serial.print(" ");
  Serial.print("Pitch: "); Serial.print(pitch); Serial.print(" ");
  Serial.println();

  x_prev = x;
  y_prev = y;
  z_prev = z;
}

int getShakingIntensity(int xDiff, int yDiff, int zDiff) {
  int intensity = 0;
  intensity += (abs(xDiff) + abs(yDiff) + abs(zDiff));
  return intensity;
}

bool isShaking(int xDiff, int yDiff, int zDiff) {
  return (abs(xDiff) + abs(yDiff) + abs(zDiff)) > MAX_THRESHOLD;
}


int getMovingAverage(int currentIntensity) {
  int len = sizeof(averageAcc_Arr) / sizeof(averageAcc_Arr[0]);
  averageAcc_Arr[currentAcc_Index] = currentIntensity;
  int sum = 0;
  for (int i = 0; i < len; i++) {
    sum += averageAcc_Arr[i];
  }
  // update currentAcc_Index
  currentAcc_Index = currentAcc_Index >= len ? 0 : currentAcc_Index + 1;
  return sum / len;
}
