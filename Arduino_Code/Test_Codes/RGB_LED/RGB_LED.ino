#define LED_R 3
#define LED_G 5
#define LED_B 6
#define COMMON_ANODE 'A'
#define COMMON_CATHODE 'C'

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  for(int i=0; i<255; i++){
  //    RGB_write(i, 250, 255-i, COMMON_ANODE);
  //    delay(20);
  //  }
  //  for (int i=0; i<255; i++){
  //    RGB_write(255-i, 250, i, COMMON_ANODE);
  //    delay(20);
  //  }
  RGB_write(255, 0, 0, COMMON_ANODE);
  delay(1000);
  RGB_write(0, 255, 0, COMMON_ANODE);
  delay(1000);
  RGB_write(0, 0, 255, COMMON_ANODE);
  delay(1000);
}

void RGB_write(int r, int g, int b, char mode){
  int rVal = (r<0) ? 0 : (r>255) ? 255 : r;
  int gVal = (g<0) ? 0 : (g>255) ? 255 : g;
  int bVal = (b<0) ? 0 : (b>255) ? 255 : b;
  rVal = (mode == COMMON_ANODE) ? 255 - rVal : rVal;
  gVal = (mode == COMMON_ANODE) ? 255 - gVal : gVal;
  bVal = (mode == COMMON_ANODE) ? 255 - bVal : bVal;
  analogWrite(LED_R, rVal);
  analogWrite(LED_G, gVal);
  analogWrite(LED_B, bVal);
}
