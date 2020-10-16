#include <Arduino.h>

void setup() {
// initialize digital pin LED_BUILTIN as an output.
Serial.begin(9600);
Serial.println();
Serial.println("Running Setup");
Serial.print("LED_BUILTIN = ");
Serial.println(LED_BUILTIN, DEC);
pinMode(D0, OUTPUT); // GPIO16  --> NodeMCU LED
pinMode(D4, OUTPUT); // GPIO2   --> ESP-12 LED
}

// the loop function runs over and over again forever
void loop() {
digitalWrite(D0, HIGH); // turn the NodeMCU LED off (HIGH is the voltage level)
digitalWrite(D4, HIGH); // turn the ESP-12 LED off (HIGH is the voltage level)
delay(1000); // wait for a second
digitalWrite(D0, LOW); // turn the NodeMCU LED on by making the voltage LOW
digitalWrite(D4, LOW); // turn the ESP-12 LED on by making the voltage LOW
delay(1000); // wait for a second
}

