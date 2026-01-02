
#include <Arduino.h>
#include "dsp.h"
#include <Math.h>
#include <bits/stdc++.h>
#include "LEDs.h"
#include <driver/i2s.h>

using namespace std;

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32
#define I2S_PORT I2S_NUM_0
#define I2S_BUFFER_LEN 64

////////////////////////////////////////////////////////////////////////////////////
// note detection parameters
DSP dsp;
float input_signal[N] = {0}; // Array to hold the input signal samples
float time_signal[N] = {0};
float detected_note = 0.0f; // Variable to hold the detected note frequency
Goertzel_item amps[49];
int sample_count = 0;

////////////////////////////////////////////////////////////////////////////////////

//LED and notes parameters
String line_e = "e|-------5-7-----7-|-8-----8-2-----2-|-0---------0-----|";
String line_B = "B|-----5-----5-----|---5-------3-----|---1---1-----1---|";
String line_G = "G|---5---------5---|-----5-------2---|-----2---------2-|";
String line_D = "D|-7---------------|-----------------|-----------------|"; 
String line_A = "A|-----------------|-----------------|-----------------|"; 
String line_E = "E|-----------------|-----------------|-----------------|"; 
String tabs =""; // String to hold all the tabs

Guitar guitar; 
int step = 0;
////////////////////////////////////////////////////////////////////////////////////
bool first_time = true;


void i2s_install() {
  // Set up I2S Processor configuration
  // Using SAMPLING_RATE from dsp.h (5120 Hz)
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLING_RATE,  // 5120 Hz to match Goertzel algorithm
    .bits_per_sample = i2s_bits_per_sample_t(16),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = I2S_BUFFER_LEN,
    .use_apll = false
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void i2s_setpin() {
  // Set I2S pin configuration
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_set_pin(I2S_PORT, &pin_config);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the built-in LED pin as output
  ws2812b.begin(); // LEDs initialization 
  Serial.begin(115200);
  
  Serial.println("Starting INMP441 + Goertzel Note Detection");
  delay(1000);

  i2s_install();
  i2s_setpin();
  i2s_start(I2S_PORT);

  for (int i = 0; i < N; i++) {
    input_signal[i] = 0.0f;
  }

  delay(500);
  Serial.println("System ready - listening for notes...");
  
  guitar.parseTabLine(line_e.c_str(), parsedE, lenE); // Parse the first line   
  guitar.parseTabLine(line_B.c_str(), parsedB, lenB); // Parse the second line   
  guitar.parseTabLine(line_G.c_str(), parsedG, lenG); // Parse the third line   
  guitar.parseTabLine(line_D.c_str(), parsedD, lenD); // Parse the fourth line   
  guitar.parseTabLine(line_A.c_str(), parsedA, lenA); // Parse the fifth line   
  guitar.parseTabLine(line_E.c_str(), parsedLowE, lenLowE); // Parse the sixth line

  guitar.getSongSteps(parsedLowE, parsedA, parsedD, parsedG, parsedB, parsedE, lenLowE, songNotes);
  
  Serial.println("Song steps parsed:");
  Serial.println(line_e); // Print the first line
  Serial.println(line_B); // Print the second line     
  Serial.println(line_G); // Print the third line
  Serial.println(line_D); // Print the fourth line
  Serial.println(line_A); // Print the fifth line
  Serial.println(line_E); // Print the sixth line

  //toggle light
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on (HIGH is the voltage level)
  delay(2000);                       // Wait for half a second
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage LOW
}
void loop(){


  if(first_time){
    for (int j=0; j<lenLowE; j++){
      for (int i=0; i<NUM_STRINGS; i++){
        if (songNotes[i][j].led_index >= 0 ){
          Serial.print(songNotes[i][j].freq);
          Serial.print(",");
          Serial.print(songNotes[i][j].led_index);
          Serial.print(" | ");
        }
      }
      Serial.println();
    }
    for (int i=0; i<NUM_STRINGS; i++){
      ws2812b.setPixelColor(songNotes[i][step].led_index, ws2812b.Color(255, 0, 0));
    }
    ws2812b.show();
    first_time = false;
  }

  int16_t sBuffer[I2S_BUFFER_LEN];
  size_t bytesIn = 0;
   
  esp_err_t result = i2s_read(I2S_PORT, &sBuffer, I2S_BUFFER_LEN * sizeof(int16_t), &bytesIn, portMAX_DELAY);

  if (result == ESP_OK) {
    // Calculate number of samples read
    int16_t samples_read = bytesIn / sizeof(int16_t);
    
    // Copy samples to input signal buffer
    for (int16_t i = 0; i < samples_read; ++i) {
      // Normalize the 16-bit sample to float (-1.0 to 1.0)
      input_signal[sample_count] = sBuffer[i] / 32768.0f;
      sample_count++;

      // When we have enough samples, process them
      if (sample_count >= N) {
        sample_count = 0;  // Reset counter
        
        // Detect the note
        detected_note = dsp.detect_note(input_signal);
        
        if (detected_note > 0) {
          Serial.printf("Note detected: %.2f Hz\n", detected_note);
        }
      }
    }
  }
    for (int str = 0; str < NUM_STRINGS; str++) {
        if(songNotes[str][step].freq > 0.0f) {
          Serial.printf("expected freq %.2f Hz\n",songNotes[str][step].freq);
        }
        if(fabs(detected_note - songNotes[str][step].freq) < 0.5f) { // Allow a tolerance of 5 Hz
          // Serial.printf("Matched string %d at step %d: freq %.2f Hz\n", str, step, detected_note);
          // step++;
          // Serial.printf("Step incremented to %d\n", step);
          // if (step >= lenLowE) { // If step exceeds the length of the tab
          //     step = 0; // Reset the step to 0
          // }

          ws2812b.clear(); // Clear all LEDs
          for (int i=0; i < NUM_STRINGS; i++) {
              if(songNotes[i][step].led_index >= 0) { // Check if the led_index is valid
              ws2812b.setPixelColor(songNotes[i][step].led_index, ws2812b.Color(255, 0, 0)); // Turn on the LED for the current step
              Serial.println(songNotes[i][step].led_index);
              } 
              else{
              Serial.println("fail");
              }
          }
          step++; // Increment the step
          if (step >= lenLowE) { // If step exceeds the length of the tab
              step = 0; // Reset the step to 0
          }
          ws2812b.show(); // Update the LED strip
          delay(50);
          break; // Exit the loop after processing the matched note
          }
      }
        // else{
        //   Serial.printf("No match for string %d at step %d: detected %.2f Hz, expected %.2f Hz\n", str, step, detected_note, songNotes[str][step].freq);
        //   delay(100);
        // } 
    }

  
  

 