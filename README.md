# LED Guitar Tutor (ESP32)

An embedded system that teaches guitar **on a real instrument** using LEDs on the fretboard and real-time audio analysis.  
The system listens to the guitar through an **I2S microphone**, detects the played note using DSP, and provides visual feedback via LEDs.

---

## Features

- 🎸 LED-guided guitar learning on a real fretboard  
- 🎤 Audio capture via **ESP32 I2S microphone**
- 📈 Real-time **note detection** (Goertzel / DSP based)
- 🧪 Serial-based audio input for offline testing and debugging

---

## Hardware

- ESP32 (with I2S support)
- I2S microphone (e.g. INMP441, SPH0645)
- Addressable LEDs (WS2812B / SK6812 recommended)
- External 5V power supply for LEDs (common ground with ESP32)

---

## Firmware Overview

The firmware collects audio samples, processes them in fixed-size buffers, and detects the dominant note frequency.  
Detected notes are later compared against lesson targets and visualized using LEDs on the guitar neck.

Key concepts:
- Fixed buffer size `N`
- Sampling rate `SAMPLING_RATE`
- DSP-based note detection (`NoteDetector`)

---

## Current `main.cpp` Behavior

The current implementation supports **Serial-fed audio samples** for testing.

### Input format
Each line over Serial:
