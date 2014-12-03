#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
void setup() 
{
  mySerial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  Serial.begin(9600);
}

void loop() {
        if (mySerial.available())
        {
          char c = mySerial.read();
          Serial.println("Her is what I got: ");
          Serial.println(c);
        }
    delay(1000);               //waiting a second
}
