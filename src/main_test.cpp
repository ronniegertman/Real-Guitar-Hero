// #include <Arduino.h>
// #include "dsp.h"
// #include <Math.h>
// #include <bits/stdc++.h>
// #include "LEDs.h"
// using namespace std;
// // ESP32 Serial Communication Code

// // Define the same Baud Rate as in the Python script
// const int BAUD_RATE = 115200;

// void setup() {
//   // Initialize serial communication
//   Serial.begin(BAUD_RATE);
  
//   // Wait a moment for the Serial Monitor to initialize (optional, good practice)
//   delay(1000); 
  
//   // Send the ready signal so Python knows when to start
//   Serial.println("ESP_READY - Waiting for data...");
// }

// void loop() {
//   // Check if data is available to read from the Python script
//   if (Serial.available()) {
//     // Read the incoming message until a newline character ('\n') is found
//     String incomingMessage = Serial.readStringUntil('\n');
    
//     // Print the message received from Python
//     Serial.print("ESP RECEIVED: ");
//     Serial.println(incomingMessage);
    
//     // Check if the received message is the expected one
//     if (incomingMessage.indexOf("Hello from Python!") != -1) {
//       // Send the response back to the Python script
//       Serial.println("Hello from ESP!");
//       Serial.println("ESP is done with the task.");
//     } else {
//       Serial.println("Received unexpected data.");
//     }
//   }
  
//   // A small delay to keep the loop from running too fast
//   delay(10);
// }