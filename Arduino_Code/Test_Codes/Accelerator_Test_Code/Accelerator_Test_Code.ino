#include <Wire.h>
#include <ADXL345.h>

ADXL345 adxl; //variable adxl is an instance of the ADXL345 library
int x, y, z;
int x_sample, y_sample, z_sample;


/*Macros*/
#define SAMPLES 50
#define MAX_THRESHOLD 50 // max change limit

void setup() {
  Serial.begin(9600);
  //pinMode(A2, OUTPUT);
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
  }
  for (int i = 0; i < SAMPLES; i++) {
    adxl.readXYZ(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
    x_sample += x;
    y_sample += y;
    z_sample += z;
  }
  x_sample /= SAMPLES;
  y_sample /= SAMPLES;
  z_sample /= SAMPLES;

}

void loop() {
  updateAverage();
  adxl.readXYZ(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
  int xDiff = x_sample - x; // finding change in x
  int yDiff = y_sample - y; // finding change in y
  int zDiff = z_sample - z; // finding change in z

  /*    Printing out x, y, z coordinates value              */
  // Serial.print("X: "); Serial.print(x); Serial.print("  ");
  // Serial.print("Y: "); Serial.print(y); Serial.print("  ");
  // Serial.print("Z: "); Serial.print(z); Serial.print("  ");

  int intensity = shakingIntensity(xDiff, yDiff, zDiff);
  double roll = atan(y / sqrt(pow(x, 2) + pow(z, 2))) * 180 / PI;         // rotation around X-Axis
  double pitch = atan(-1 * x / sqrt(pow(y, 2) + pow(z, 2))) * 180 / PI;   // rotation around Y-Axis
  Serial.print("Intensity: "); Serial.print(intensity); Serial.print(" ");
//  Serial.print("Roll: "); Serial.print(roll); Serial.print(" ");
//  Serial.print("Pitch: "); Serial.print(pitch); Serial.print(" ");
  Serial.println();
  //analogWrite(A2, map(intensity, 0, 1000, 0, 255));
}

void updateAverage() {
  adxl.readXYZ(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
  x_sample += x;
  y_sample += y;
  z_sample += z;
  x_sample /= 2;
  y_sample /= 2;
  z_sample /= 2;
}


int shakingIntensity(int xDiff, int yDiff, int zDiff){
    int intensity = 0;
    if (isShaking)
    {
//      Serial.println("Shaking Detected");
      intensity += (abs(xDiff)+ abs(yDiff) + abs(zDiff));
//      Serial.print("Intensity");
//      Serial.print("\t");
//      Serial.println(intensity);
    }
    return intensity;
}

bool isShaking(int xDiff, int yDiff, int zDiff){
  return (abs(xDiff)+ abs(yDiff) + abs(zDiff)) > MAX_THRESHOLD;
} 
