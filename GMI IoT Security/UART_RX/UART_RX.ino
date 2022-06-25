#include <SoftwareSerial.h>

SoftwareSerial uart(6, 7);

void setup() {
  Serial.begin(115200);
  uart.begin(9600);
}

void loop() {
  if(uart.available()){
    String information = uart.readStringUntil('\n');
    Serial.println(information);
  }
}
