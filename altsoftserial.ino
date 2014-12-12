#include <AltSoftSerial.h>

// Uses hard-wired pins, same as leonardo: 13 for RX, 5 for TX.
AltSoftSerial XBeeSerial;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo and Micro only
  }
  Serial.println("AltSoftSerial Test Begin");
  XBeeSerial.begin(9600);
  XBeeSerial.println("Hello World");
}

void loop() {
  char c;

  if (Serial.available()) {
    c = Serial.read();
    XBeeSerial.print(c);
  }
  if (XBeeSerial.available()) {
    c = XBeeSerial.read();
    Serial.print(c);
  }
}
