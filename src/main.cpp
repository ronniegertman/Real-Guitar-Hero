#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);
void lightUpBuiltinLED(); // Function to light up built-in LED

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  lightUpBuiltinLED(); // Light up the built-in LED
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (LOW is ON for ESP8266)
  delay(500);                       // Wait for 500 milliseconds
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off (HIGH is OFF for ESP8266)
  delay(500);     
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

// Function to light up the built-in LED on ESP8266
void lightUpBuiltinLED() {
  pinMode(LED_BUILTIN, OUTPUT); // Set built-in LED pin as output
  digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (LOW is ON for ESP8266)
}