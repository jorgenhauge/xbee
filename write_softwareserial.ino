#include <AltSoftSerial.h>

//#include <SoftwareSerial.h>

//SoftwareSerial XBee(10, 11); // RX, TX
AltSoftSerial XBee; //Hardcoded to: TX: pin 5, RX: pin 13 
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

void blink_led(){
	pinMode(13, OUTPUT);
	for (int i = 0; i < 10; i++){
	    digitalWrite(13, HIGH);
	    delay(250);
	    digitalWrite(13, LOW);
	    delay(195);
	}
	digitalWrite(13, HIGH);
}

int readUntil(char terminator, char * buf, int len){
	int i = 0;
	for (;;){
		while (XBee.available() == 0); // Hang here if no characters is on the port
		char c = XBee.read();
		Serial.print((int)c);
                Serial.print(" ");
		if (c == terminator || i == len){
			break;
		}
	buf[i++] = c;
	}
buf[i] = 0;
return i;
}

void setup()
{
	// Set up both ports at 9600 baud. This value is most important
	// for the XBee. Make sure the baud rate matches the config
	// setting of your XBee.
	//blink_led();
	XBee.begin(9600);
	Serial.begin(9600);
        while (!Serial) {
            ; // wait for serial port to connect. Needed for Leonardo and Micro only
        }
	Serial.println("Alive!!");
        Serial.println("Running command +++");
	XBee.print("+++"); //Write "+++" open AT mode.
	//while (XBee.available() < 3); // Wait until two bytes is read.
	readUntil('\r', device_id, sizeof(device_id));
	//XBee.readBytesUntil('\r', device_id, sizeof(device_id));
	Serial.println("Got OK");

        Serial.println("Running command ATSL");
	XBee.println("ATSL");
	//while (XBee.available() < 10);
	//int n = XBee.readBytesUntil('\r', device_id, sizeof(device_id));
	readUntil('\r', device_id, sizeof(device_id));
	Serial.println("Got response on ATSL");
	//device_id[n] = 0; // Zero terminate the char array
	XBee.read(); // Get rid of the last byte in the serial buffer \n

        Serial.println("Running command ATCN");
	XBee.println("ATCN"); // Exit AT mode, returns OK
	//while (XBee.available() < 3); // Wait until two bytes is read.

	//XBee.readStringUntil('\n');
	char garbage[8];
	readUntil('\r', garbage, sizeof(garbage));
	Serial.println("Got OK");

	Serial.print("ID: ");
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
		XBee.print("ID: ");
		XBee.print(device_id);
		XBee.print(" VTMP1: ");
		XBee.println(reading);
    }
    else {
		XBee.write(incoming_byte);
		Serial.println("Sent:");
		Serial.println(incoming_byte);
		Serial.println("back!!");
    }
  }
}

