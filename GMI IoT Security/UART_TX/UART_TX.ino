#include <SoftwareSerial.h>

SoftwareSerial uart(6, 7);

void setup() {
  Serial.begin(115200);
  uart.begin(9600);
}

void loop() {
  String information = "Hello";
  
  Serial.println(information);
  uart.println(information);
  
  delay(250);
}
