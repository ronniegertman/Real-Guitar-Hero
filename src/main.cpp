#include <Arduino.h>
#include "dsp.h"
#include <Math.h>
#include <bits/stdc++.h>
using namespace std;

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

float input_signal[N] = {0}; // Array to hold the input signal samples
float time_signal[N] = {0};
float detected_note = 0.0f; // Variable to hold the detected note frequency
Goertzel_item amps[36];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the built-in LED pin as output
  Serial.begin(115200);         // Start serial communication at 115200 baud rate
  // Generate a sine wave with a frequency of 440 Hz
  // float frequency = F4; // Frequency in Hz
  // float sampleRate = 10000; // Sample rate in Hz
  // int numSamples = 512; // Number of samples to generate
  // float sineWave[numSamples];
  // for (int i = 0; i < numSamples; i++) {
  //   sineWave[i] = sin(2 * M_PI * frequency * i / sampleRate);
  // }
  // delay(1000);
  // Serial.printf("Goertzel power: %f\n", Goertzel(sineWave, frequency));
  // Serial.printf("Goertzel power: %f\n", Goertzel(sineWave, frequency * 2));
  // Serial.printf("Note detected: %f\n", detect_note(sineWave));

}

void loop() {

  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Read input until newline

    if (input == "END"){
      detected_note = detect_note(input_signal);
      Serial.printf("Note detected: %f in time ", detected_note);
      core_freqs(input_signal, amps);
      Serial.printf("Core frequencies detected: (%f, %f), (%f,%f), (%f, %f), (%f, %f), (%f, %f), (%f, %f)\n", amps[0].freq, amps[0].power, amps[1].freq, amps[1].power, amps[2].freq, amps[2].power, amps[3].freq, amps[3].power, amps[4].freq, amps[4].power, amps[5].freq, amps[5].power);
      return; 
    }

    int comma_index = input.indexOf(',');
    String time_str = input.substring(0, comma_index); // Extract time part
    String sample_str = input.substring(comma_index + 1); // Extract sample part

    float time = time_str.toFloat(); 
    float sample = sample_str.toFloat();

    input_signal[(int)(time * SAMPLING_RATE) % N] = sample;
    time_signal[(int)(time * SAMPLING_RATE) % N] = time;
    
  }
}