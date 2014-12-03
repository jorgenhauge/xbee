#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
char c = 'A';

void setup(){ 
    mySerial.begin(9600);
    Serial.begin(9600);
}

void loop(){
    mySerial.print(c);
    Serial.println(c);
    
    delay(1000);               //waiting a second
}

