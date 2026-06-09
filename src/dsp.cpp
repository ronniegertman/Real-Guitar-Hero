#include <algorithm>
#include <math.h>
#include <Arduino.h>
#include "dsp.h"

#define THRESHOLD 15000 // Threshold for note detection, can be adjusted based on testing

float DSP::Goertzel(float* input, float freq ,float s_prev, float s_prev_prev){
    /// @brief Goertzel algorithm for detecting a specific frequency in a signal
    /// @param input Pointer to the input signal array
    /// @param freq Frequency to detect in the input signal
    /// @param s_prev Previous sample value (optional, default is 0)
    /// @param s_prev_prev Previous previous sample value (optional, default is 0)
    /// @return The Goertzel algorithm power result for the specified frequency

    if (input == nullptr || freq <= 0) {
        return 0.0f; // Invalid input or frequency
    }
    if (freq > SAMPLING_RATE / 2) {
        return 0.0f; // Frequency is above Nyquist limit
    }
    // Calculate the Goertzel algorithm coefficients
       int bin = (int)(0.5f + ((N * freq) / SAMPLING_RATE));
    float omega = 2.0f * M_PI * bin / N;
    float coeff = 2.0f * cosf(omega);  // compute ONCE here, not inside loop

    float s = 0.0f;
    for (int i = 0; i < N; i++) {
        s = input[i] + coeff * s_prev - s_prev_prev;
        s_prev_prev = s_prev;
        s_prev = s;
    }

    float power = (s_prev_prev * s_prev_prev) + (s_prev * s_prev) 
                  - coeff * s_prev * s_prev_prev;  // reuse coeff here too
    return power;
}

float DSP::detect_note(float* input){
    /// @brief Detects the note in the input signal using the Goertzel algorithm
    /// @param input Pointer to the input signal array 
    /// @return The frequency of the detected note, or 0.0 if no note is detected
    // find the maximum amplitude
    float max_power = -1.0f;
    float max_freq = 0.0f;
    for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++){
        float power = Goertzel(input, notes[i]);
        // Serial.printf("Goertzel power for %f: %f\n", notes[i], power);
        if (power > max_power && power > THRESHOLD) { // Threshold to detect a note
            max_power = power;
            max_freq = notes[i];
        }
    }
    Serial.printf("Max power: %f at frequency: %f\n", max_power, max_freq);
    return max_freq; // Return the detected frequency
}

 static bool compareDescending(Goertzel_item &a, Goertzel_item &b) {
    return a.power > b.power; // Descending: larger values come first
}

void DSP::core_freqs(float* input, Goertzel_item* goertzel_powers){
    /// @brief Detects the core frequencies in the input signal using the Goertzel algorithm
    /// @param input Pointer to the input signal array
    /// @return An array of detected core frequencies, or an empty array if no frequencies are detected

    for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++){
        goertzel_powers[i].freq = notes[i]; 
        goertzel_powers[i].power = DSP::Goertzel(input, notes[i]); 
    }
    std::sort(goertzel_powers, goertzel_powers + 49, compareDescending); // Sort the powers in descending order
    Serial.println("top powers:");
    for (int i = 0; i < 5; i++) {
        Serial.printf("Frequency: %f, Power: %f\n", goertzel_powers[i].freq, goertzel_powers[i].power);
    }
}

bool DSP::correct_detection(float note_to_play, Goertzel_item* goertzel_powers){
    float tol = 0.5f; // 0.5 Hz tolerance
    float current_threshold; 
    int search_depth; // New variable to control how deep we look in the array

    // 1. Assign exact thresholds AND search depths based on frequency bands
    
    if (note_to_play >= 230.0f) {

        current_threshold = 70.0f;  // Very forgiving threshold
        search_depth = 5    ;  
        if (note_to_play >= 490.0f) {
        current_threshold = 5.0f;  // Very forgiving threshold
        search_depth = 5    ;           // Deep search: Allows note to hide behind low-freq body thumps
        }          // Deep search: Allows note to hide behind low-freq body thumps
    } 
    else if (note_to_play >= 180.0f && note_to_play < 230.0f) {
        current_threshold = 5000.0f; 
        search_depth = 3;           // Medium strictness
    } 
    else {
        current_threshold = 30000.0f; // Very strict threshold
        search_depth = 2;             // Shallow search: MUST be the dominant sound
    }

    // 2. Master Noise Gate: Prevents algorithm from running in an empty room
    if (goertzel_powers[0].power < 50.0f) {
        return false; 
    }

    // 3. Evaluate using the DYNAMIC search depth
    for (int i = 0; i < search_depth; i++) {
        
        // Only accept if THIS specific bin clears the target string's threshold
        if (goertzel_powers[i].power >= current_threshold) {
            
            // Check fundamental match
            if (fabs(goertzel_powers[i].freq - note_to_play) < tol) {
                return true;
            }
            
            // Check octave matches (harmonics and subharmonics)
            // if (fabs(goertzel_powers[i].freq - (note_to_play * 2.0f)) < tol ||
            //     fabs(goertzel_powers[i].freq - (note_to_play / 2.0f)) < tol) {
            //     return true;
            // }
        }
    }

    return false; 
}


