#include <SoftwareSerial.h>

//TMP36 Pin Variables
int sensorPin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                   //the resolution is 10 mV / degree centigrade with a
                   //500 mV offset to allow for negative temperatures
 
/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor
  Serial1.begin(9600);  //Start the serial connection with the computer
                       //Starts a serial connection towarddmes RX and TX(0,1)
}

void print_serial(float voltage, float temperatureC, float temperatureF)
{
  Serial.print(voltage); Serial.println(" volts");
  Serial1.print(voltage); Serial1.print(" volts");
  Serial.print(temperatureC); Serial.println(" degrees C");
  Serial1.print(temperatureC); Serial1.print(" degrees C");
  Serial.print(temperatureF); Serial.println(" degrees F");
  Serial1.print(temperatureF); Serial1.print(" degrees F");
}
 
void loop()                     // run over and over again
{
 //getting the voltage reading from the temperature sensor
 int reading = analogRead(sensorPin);  
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 5.0;
 voltage /= 1024.0;
 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
 
 // now convert to Fahrenheit
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 print_serial(voltage, temperatureC, temperatureF);
 
 delay(1000);                                     //waiting a second
}
