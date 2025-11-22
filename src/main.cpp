
// // void setup() {
// //   pinMode(LED_BUILTIN, OUTPUT); // Initialize the built-in LED pin as output
// //   Serial.begin(115200);         // Start serial communication at 115200 baud rate
// // }

// // bool first_time = true;
// // float start_time = 0.0f;
// // float end_time = 0.0f;

// // void loop() {

// //   if (Serial.available() > 0) {
// //     if (first_time){
// //       first_time = false;
// //       start_time = millis() / 1000.0f; // Record the start time in seconds
// //     }
// //     String input = Serial.readStringUntil('\n'); // Read input until newline

// //     if (input == "END"){ 
// //       end_time = millis() / 1000.0f; // Record the end time in seconds
// //       detected_note = dsp.detect_note(input_signal);
// //       Serial.printf("Note detected: %f, data sent in time %f\n", detected_note, (end_time - start_time));
// //       // core_freqs(input_signal, amps);
// //       // Serial.printf("Core frequencies detected: (%f, %f), (%f,%f), (%f, %f), (%f, %f), (%f, %f), (%f, %f)\n", amps[0].freq, amps[0].power, amps[1].freq, amps[1].power, amps[2].freq, amps[2].power, amps[3].freq, amps[3].power, amps[4].freq, amps[4].power, amps[5].freq, amps[5].power);
// //       return; 
// //     }

// //     int comma_index = input.indexOf(',');
// //     String time_str = input.substring(0, comma_index); // Extract time part
// //     String sample_str = input.substring(comma_index + 1); // Extract sample part

// //     float time = time_str.toFloat(); 
// //     float sample = sample_str.toFloat();

// //     input_signal[(int)(time * SAMPLING_RATE) % N] = sample;
// //     time_signal[(int)(time * SAMPLING_RATE) % N] = time;
    
// //   }
// // }






// #include <Arduino.h>
// #include "dsp.h"
// #include <Math.h>
// #include <bits/stdc++.h>
// #include "LEDs.h"
// using namespace std;

// #ifndef LED_BUILTIN
// #define LED_BUILTIN 2
// #endif

// ////////////////////////////////////////////////////////////////////////////////////
// // note detection parameters
// DSP dsp;
// float input_signal[N] = {0}; // Array to hold the input signal samples
// float time_signal[N] = {0};
// float detected_note = 0.0f; // Variable to hold the detected note frequency
// Goertzel_item amps[36];
// ////////////////////////////////////////////////////////////////////////////////////

// //LED and notes parameters
// String line_e = "e|-------------------------------------5-8-5----------------|";
// String line_B = "B|-------3--3-3------------------6----6-----6-5-6-5---------|";
// String line_G = "G|---2-2-2--2-2--5-5-5-7-----7----7--7-------------7--5-7---|";
// String line_D = "D|-0-0-0-0--0-0--5-5-5-7--5-7---7------------------------7--|"; 
// String line_A = "A|-0----------------5---------------------------------------|"; 
// String line_E = "E|----------------------------------------------------------|"; 
// String tabs =""; // String to hold all the tabs

// Guitar guitar; 
// ////////////////////////////////////////////////////////////////////////////////////
// bool first_time = true;

// void setup() {
//   pinMode(LED_BUILTIN, OUTPUT); // Initialize the built-in LED pin as output
//   ws2812b.begin(); // LEDs initialization 
//   Serial.begin(115200);         
//   delay(1000);
  
//   guitar.parseTabLine(line_e.c_str(), parsedE, lenE); // Parse the first line   
//   guitar.parseTabLine(line_B.c_str(), parsedB, lenB); // Parse the second line   
//   guitar.parseTabLine(line_G.c_str(), parsedG, lenG); // Parse the third line   
//   guitar.parseTabLine(line_D.c_str(), parsedD, lenD); // Parse the fourth line   
//   guitar.parseTabLine(line_A.c_str(), parsedA, lenA); // Parse the fifth line   
//   guitar.parseTabLine(line_E.c_str(), parsedLowE, lenLowE); // Parse the sixth line

//   guitar.getSongSteps(parsedLowE, parsedA, parsedD, parsedG, parsedB, parsedE, lenLowE, songNotes);
  
//   Serial.println("Song steps parsed:");
//   Serial.println(line_e); // Print the first line
//   Serial.println(line_B); // Print the second line     
//   Serial.println(line_G); // Print the third line
//   Serial.println(line_D); // Print the fourth line
//   Serial.println(line_A); // Print the fifth line
//   Serial.println(line_E); // Print the sixth line

//   //toggle light
//   digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on (HIGH is the voltage level)
//   delay(2000);                       // Wait for half a second
//   digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage LOW
// }

// void loop(){
//   if(first_time){
//     for (int j=0; j<lenLowE; j++){
//       for (int i=0; i<NUM_STRINGS; i++){
//         if (songNotes[i][j].led_index >= 0 ){
//           Serial.print(songNotes[i][j].freq);
//           Serial.print(",");
//           Serial.print(songNotes[i][j].led_index);
//           Serial.print(" | ");
//         }
//       }
//       Serial.println();
//     }
//     first_time = false;
//   }
//   // static bool lastButtonState = HIGH; // Initialize last button state to HIGH 
//   // bool currentButtonState = digitalRead(BUTTON_PIN); // Read the current button state

//   // if (lastButtonState == HIGH && currentButtonState == LOW){
   

//   // ws2812b.clear(); // Clear all LEDs
//   // for (int i=0; i < NUM_STRINGS; i++) {
//   //   if(songNotes[i][step].led_index >= 0) { // Check if the led_index is valid
//   //     ws2812b.setPixelColor(songNotes[i][step].led_index, ws2812b.Color(255, 0, 0)); // Turn on the LED for the current step
//   //     Serial.println(songNotes[i][step].led_index);
//   //   } else{
//   //   Serial.println("fail");
//   //   }
//   // }
//   // step++; // Increment the step
//   // if (step >= lenLowE) { // If step exceeds the length of the tab
//   //   step = 0; // Reset the step to 0
//   // }
//   // ws2812b.show(); // Update the LED strip
//   // delay(50);
//   // }
//   // lastButtonState = currentButtonState; // Update the last button state


// }
  
  

 