#include <Arduino.h>
#include "dsp.h"
#include <Math.h>
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the built-in LED pin as output
  Serial.begin(9600);         // Start serial communication at 115200 baud rate
  // Generate a sine wave with a frequency of 440 Hz
  float frequency = 440.0; // Frequency in Hz
  float sampleRate = 10000; // Sample rate in Hz
  int numSamples = 512; // Number of samples to generate
  float sineWave[numSamples];
  for (int i = 0; i < numSamples; i++) {
    sineWave[i] = sin(2 * M_PI * frequency * i / sampleRate);
  }
  delay(1000);
  Serial.printf("Goertzel power: %f\n", Goertzel(sineWave, frequency));
  Serial.printf("Goertzel power: %f\n", Goertzel(sineWave, frequency * 2));
  Serial.printf("Note detected: %f\n", detect_note(sineWave));

}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
  delay(500);                      // Wait for 500 milliseconds
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
  delay(500);                      // Wait for 500 milliseconds
}