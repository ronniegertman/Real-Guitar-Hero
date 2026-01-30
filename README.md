# LED Guitar Tutor (ESP32 + I2S Mic)

An embedded system that teaches guitar **on a real guitar** using LEDs on the fretboard and real-time audio analysis.  
An **ESP32** reads audio from an **I2S microphone**, detects the played note using DSP (Goertzel-style / frequency analysis), and (optionally) drives LEDs to guide the player and give feedback.

---

## Features

- 🎸 **LED guidance**: show where to place fingers (string/fret) and what to play next
- 🎤 **Audio input**: I2S microphone support on ESP32 (planned / in progress)
- 📈 **Note detection**: analyze audio frames and estimate the fundamental note frequency
- 🧪 **Serial test mode**: current `main.cpp` can ingest samples over Serial for offline testing

---

## Hardware

**Required**
- ESP32 (any model with I2S support)
- I2S microphone module (e.g., INMP441 / SPH0645)
- Addressable LEDs (WS2812B / SK6812 recommended) + mounting on fretboard/neck
- 5V power supply for LEDs (share **common ground** with ESP32)

**Recommended**
- Level shifter for LED data (ESP32 3.3V → LEDs 5V)
- 330Ω series resistor on LED data line
- Bulk capacitor near LED strip (e.g., 470–1000µF)

---

## Firmware Overview

Audio is processed in frames of `N` samples at `SAMPLING_RATE`.  
A detector (see `NoteDetector`) analyzes the buffer and returns a detected note frequency.

Typical loop (target behavior):
1. Choose a lesson target (note/chord)
2. Light the correct LED(s) on the fretboard
3. Capture audio frame
4. Detect note
5. Compare to target and provide feedback (LED color, blink, etc.)

---

## Current `main.cpp` (Serial Sample Ingestion)

The current main file supports feeding sample data via Serial.

### Serial input format

Send lines like:
<time>,<sample>\n
css
Copy code

To trigger detection, send:
END\n
csharp
Copy code

### How samples are stored

For each received line:
- Parse `time` (seconds) and `sample` (float amplitude)
- Store into a circular buffer using:
  - `index = (int)(time * SAMPLING_RATE) % N`
  - `input_signal[index] = sample`
  - `time_signal[index] = time`

When `END` is received:
- `detected_note = noteDetector.detect_note(input_signal);`
- Prints: detected frequency + elapsed time between first sample and END

---

## Provided Main File (current)

```cpp
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
NoteDetector noteDetector; // Create an instance of NoteDetector

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
bool first_time = true;
float start_time = 0.0f;
float end_time = 0.0f;
void loop() {

  if (Serial.available() > 0) {
    if (first_time){
      first_time = false;
      start_time = millis() / 1000.0f; // Record the start time in seconds
    }
    String input = Serial.readStringUntil('\n'); // Read input until newline

    if (input == "END"){
      detected_note = noteDetector.detect_note(input_signal);
      Serial.printf("Note detected: %f, data sent in time %f\n", detected_note, (end_time - start_time));
      // core_freqs(input_signal, amps);
      // Serial.printf("Core frequencies detected: (%f, %f), (%f,%f), (%f, %f), (%f, %f), (%f, %f), (%f, %f)\n", amps[0].freq, amps[0].power, amps[1].freq, amps[1].power, amps[2].freq, amps[2].power, amps[3].freq, amps[3].power, amps[4].freq, amps[4].power, amps[5].freq, amps[5].power);
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
Note: end_time is currently never updated in the shown code, so (end_time - start_time) will print an incorrect (likely negative/zero) duration. Consider setting end_time = millis() / 1000.0f; when END is received.
Build & Flash
Arduino IDE
Install ESP32 board support (Boards Manager)
Select your ESP32 board + port
Upload
Open Serial Monitor at 115200
PlatformIO (recommended)
Build: PlatformIO: Build
Upload: PlatformIO: Upload
Monitor: PlatformIO: Monitor (set to 115200)
Next Steps (I2S + LEDs)
I2S capture
Configure ESP32 I2S
Fill input_signal with real mic samples (normalize to float)
Run noteDetector.detect_note(input_signal) each frame
LED guidance
Create mapping: (string, fret) -> led_index
Lesson engine selects target notes/chords and lights LEDs
Compare detected note to target within tolerance (Hz or cents)
Troubleshooting
Wrong/unstable notes: verify SAMPLING_RATE, increase N, improve mic placement, add noise gate / filtering
LED flicker / resets: external 5V supply, common ground, bulk capacitor, avoid powering LEDs from ESP32 5V pin
Serial ingestion issues: ensure newline \n, timestamps in seconds, consistent sample rate vs time * SAMPLING_RATE
Roadmap
 Real I2S audio capture (replace Serial test path)
 LED fretboard mapping + animations
 Practice modes (single notes, scales, chords, songs)
 Calibration mode (mic gain, tuning reference A4)
 Optional BLE/Wi-Fi companion app
License
Add your license here (MIT / Apache-2.0 / GPL / proprietary).
Copy code









Pro


