#include <Wire.h>

byte x = 0;

void setup() {
  Wire.begin();
}

void loop() {
  Wire.beginTransmission(8);
  Wire.write("x is "); 
  Wire.write(x); 
  Wire.endTransmission();

  x++;

  delay(500);
}
