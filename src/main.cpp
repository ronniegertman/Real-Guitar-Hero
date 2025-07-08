#include <Arduino.h>
#include "dsp.h"
#include <Math.h>
#include <bits/stdc++.h>
#include "LEDs.h"
using namespace std;

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

float input_signal[N] = {0}; // Array to hold the input signal samples
float time_signal[N] = {0};
float detected_note = 0.0f; // Variable to hold the detected note frequency
Goertzel_item amps[36];
NoteDetector noteDetector; // Create an instance of NoteDetector


 // Setup for note detection 
// void setup() {
//   pinMode(LED_BUILTIN, OUTPUT); // Initialize the built-in LED pin as output
//   Serial.begin(115200);         // Start serial communication at 115200 baud rate
//   ws2812b.begin();//leds initialization, probably better put this in the esp32 setup
// }  

// bool first_time = true;
// float start_time = 0.0f;
// float end_time = 0.0f;

// The loop for checking note detection
// void loop() {
//   if (Serial.available() > 0) {
//     if (first_time){
//       first_time = false;
//       start_time = millis() / 1000.0f; // Record the start time in seconds
//     }
//     String input = Serial.readStringUntil('\n'); // Read input until newline

//     if (input == "END"){ 
//       detected_note = noteDetector.detect_note(input_signal);
//       Serial.printf("Note detected: %f, data sent in time %f\n", detected_note, (end_time - start_time));
//       // core_freqs(input_signal, amps);
//       // Serial.printf("Core frequencies detected: (%f, %f), (%f,%f), (%f, %f), (%f, %f), (%f, %f), (%f, %f)\n", amps[0].freq, amps[0].power, amps[1].freq, amps[1].power, amps[2].freq, amps[2].power, amps[3].freq, amps[3].power, amps[4].freq, amps[4].power, amps[5].freq, amps[5].power);
//       return; 
//     }

//     int comma_index = input.indexOf(',');
//     String time_str = input.substring(0, comma_index); // Extract time part
//     String sample_str = input.substring(comma_index + 1); // Extract sample part

//     float time = time_str.toFloat(); 
//     float sample = sample_str.toFloat();

//     input_signal[(int)(time * SAMPLING_RATE) % N] = sample;
//     time_signal[(int)(time * SAMPLING_RATE) % N] = time;
    
//   }
// }

bool messageSent = false; // Flag to check if the message has been sent

Guitar guitar; 
void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the built-in LED pin as output
  ws2812b.begin(); // LEDs initialization, probably better put this in the esp32 setup
  Serial.begin(115200); // Start serial communication at 115200 baud rate
  delay(1000); // Wait for serial connection to establish

  Serial.println("ESP_READY");

  Serial.println("Esp32 setup complete. Waiting for input...");

}



void loop(){
  

}