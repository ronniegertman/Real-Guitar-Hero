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


String line_e = ""; // String to hold the first line of the tab, arduino uses String class Serial.println() works with Arduino's String type
String line_B = ""; // String to hold the second line of the tab 
String line_G = ""; // String to hold the third line of the tab
String line_D = ""; // String to hold the fourth line of the tab
String line_A = ""; // String to hold the fifth line of the tab
String line_E = ""; // String to hold the sixth line of the tab
String tabs =""; // String to hold all the tabs

Guitar guitar; 
void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the built-in LED pin as output
  ws2812b.begin(); // LEDs initialization, probably better put this in the esp32 setup
  Serial.begin(115200); // Start serial communication at 115200 baud rate
  delay(1000); // Wait for serial connection to establish


  Serial.println(" "); // added space so it wont be one piece with the boot message
  Serial.println("ESP_READY"); 

  Serial.println("Waiting for tabs to be loaded...");

  tabs = Serial.readString();
  line_e = tabs.substring(0, tabs.indexOf('\n')); // Extract the first line
  tabs.remove(0, line_e.length() + 1); // Remove the first line
  line_B = tabs.substring(0, tabs.indexOf('\n')); // Extract the second line
  tabs.remove(0, line_B.length() + 1); // Remove the second line
  line_G = tabs.substring(0, tabs.indexOf('\n')); // Extract the third line
  tabs.remove(0, line_G.length() + 1); // Remove the third line
  line_D = tabs.substring(0, tabs.indexOf('\n')); // Extract the fourth
  tabs.remove(0, line_D.length() + 1); // Remove the fourth line
  line_A = tabs.substring(0, tabs.indexOf('\n')); // Extract the fifth line
  tabs.remove(0, line_A.length() + 1); // Remove the fifth line
  line_E = tabs; // The rest of the string is the sixth line, no need to remove it
  line_E.trim(); // Trim any whitespace from the end of the last line

  Serial.println("Tabs loaded successfully! here are the tabs:"); // Notify that tabs are loaded

 // Serial.println(tabs); // Print the loaded tabs
  Serial.println(line_e); // Print the first line
  Serial.println(line_B); // Print the second line     
  Serial.println(line_G); // Print the third line
  Serial.println(line_D); // Print the fourth line
  Serial.println(line_A); // Print the fifth line
  Serial.println(line_E); // Print the sixth line

  guitar.parseTabLine(line_e.c_str(), parsedE, lenE); // Parse the first line   
  guitar.parseTabLine(line_B.c_str(), parsedB, lenB); // Parse the second line   
  guitar.parseTabLine(line_G.c_str(), parsedG, lenG); // Parse the third line   
  guitar.parseTabLine(line_D.c_str(), parsedD, lenD); // Parse the fourth line   
  guitar.parseTabLine(line_A.c_str(), parsedA, lenA); // Parse the fifth line   
  guitar.parseTabLine(line_E.c_str(), parsedLowE, lenLowE); // Parse the sixth line

  guitar.getSongSteps(parsedLowE, parsedA, parsedD, parsedG, parsedB, parsedE, lenLowE, songNotes); // Get the song steps from the parsed lines

//   for (int i = 0; i < NUM_STRINGS; i++) {
//         for (int j = 0; j < MAX_TAB_LENGTH; j++) {
//             Serial.print("songNotes[");
//             Serial.print(i);
//             Serial.print("][");
//             Serial.print(j);
//             Serial.print("] = {freq: ");
//             Serial.print(songNotes[i][j].freq);
//             Serial.print(", led_index: ");
//             Serial.print(songNotes[i][j].led_index);
//             Serial.println("}");
//         }
// }
}

void loop(){
  

}