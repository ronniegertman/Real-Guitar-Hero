// Include I2S driver
#include <driver/i2s.h> 
#include <Arduino.h>
#include "dsp.h"

// Connections to INMP441 I2S microphone
#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32

// Use I2S Processor 0
#define I2S_PORT I2S_NUM_0

// Define input buffer length for I2S reads
#define I2S_BUFFER_LEN 64

// Create DSP object and signal buffers
DSP dsp;
float input_signal[N];  // N is defined in dsp.h as 2048
int sample_count = 0;
Goertzel_item amps[49];

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
  Serial.begin(115200);
  Serial.println("Starting INMP441 + Goertzel Note Detection");

  delay(1000);

  // Set up I2S
  i2s_install();
  i2s_setpin();
  i2s_start(I2S_PORT);

  // Initialize signal buffer
  for (int i = 0; i < N; i++) {
    input_signal[i] = 0.0f;
  }

  delay(500);
  Serial.println("System ready - listening for notes...");
}

void loop() {
  // Buffer for I2S data
  int16_t sBuffer[I2S_BUFFER_LEN];
  size_t bytesIn = 0;
  
  // Get I2S data
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
        float detected_note = dsp.detect_note(input_signal);
        
        if (detected_note > 0) {
          Serial.printf("Note detected: %.2f Hz\n", detected_note);
        }
        
        // Optional: Get detailed frequency analysis
        // Uncomment if you want to see all detected frequencies
        /*
        dsp.core_freqs(input_signal, amps);
        Serial.println("Top 6 frequencies:");
        for (int j = 0; j < 6; j++) {
          Serial.printf("  %d: %.2f Hz (power: %.2f)\n", j+1, amps[j].freq, amps[j].power);
        }
        */
      }
    }
  }
}