#include <SoftwareSerial.h>

SoftwareSerial XBee(10, 11); // RX, TX
int sensorPin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                   //the resolution is 10 mV / degree centigrade with a
                   //500 mV offset to allow for negative temperatures
char incoming_byte;
float temperature = 0.0;
char device_id [16];

float get_temp()
{
  int reading = analogRead(sensorPin);
  float voltage = reading * 5.0;
  voltage /= 1024.0;
  /*if (type == "Far"){
    return(temperature * 9.0 / 5.0) + 32.0;
  }*/
  return (voltage - 0.5) * 100;
}

void await_ok(){
  
}


void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  Serial.begin(9600);
  XBee.print("+++"); //Write "+++" open AT mode.
  while (XBee.available() < 3); // Wait until two bytes is read.
  XBee.readBytesUntil('\n', device_id, sizeof(device_id));
  XBee.println("ATSL");
  while (XBee.available() < 3);
  int n = XBee.readBytesUntil('\r', device_id, sizeof(device_id));
  device_id[n] = 0; // Zero terminate the char array
  XBee.read(); // Get rid of the last byte in the serial buffer \n
  
  XBee.println("ATCN"); // Exit AT mode, returns OK
  while (XBee.available() < 3); // Wait until two bytes is read.
  XBee.readStringUntil('\n');
  
  Serial.print("Device Id is: ");
  Serial.print(device_id);
  /*for (size_t i = 0; i < sizeof(device_id); i++){
    Serial.print(" ");
    Serial.print((int)device_id[i]);
  }
  Serial.println(" ");*/
}

void loop()
{
  if (XBee.available())
  { // If data comes in from serial monitor, send it out to XBee
    incoming_byte = XBee.read();
    Serial.println("Got an incoming_byte: ");
    Serial.println(incoming_byte);
    if (incoming_byte == 'T' || incoming_byte == 't'){
      int reading = analogRead(sensorPin);
      //temperature = get_temp();
      //Serial.println(analogRead(sensorPin));
      XBee.print("Device Id: ");
      XBee.print(device_id);
      XBee.print(" data: ");
      XBee.println(reading);
    }
    else{
      XBee.write(incoming_byte);
      Serial.println("Sent:");
      Serial.println(incoming_byte);
      Serial.println("back!!");
    }
  }
}

