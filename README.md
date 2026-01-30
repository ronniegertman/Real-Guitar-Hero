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
