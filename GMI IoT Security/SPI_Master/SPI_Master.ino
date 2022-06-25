#include <SPI.h>

char str[ ] = "Hello Guys, I'm Arduino Family\n";

void setup(){
  Serial.begin(115200);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  Serial.println("Hello I'm SPI Master Device");
}

void loop (void){
  digitalWrite(SS, LOW);
  // send test string
  for (int i = 0; i < sizeof(str); i++){
    SPI.transfer(str [i]);
  }
  digitalWrite(SS, HIGH); // disable Slave Select
  delay(2000);
}
