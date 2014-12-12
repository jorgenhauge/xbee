/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 2 has an LED connected on most Arduino boards.
// give it a name:
int led = 2;
int in_pin = 4;
int val = 0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(in_pin, INPUT); 
}

// the loop routine runs over and over again forever:
void loop() {
  val = digitalRead(in_pin);
  if (val == HIGH){
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1500);               // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(500);                // wait for a second
  }
}
