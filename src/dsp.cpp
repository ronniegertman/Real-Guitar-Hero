#include <algorithm>
#include <math.h>
#include <Arduino.h>
#include "dsp.h"

float DSP::Goertzel(float* input, float freq, float s_prev, float s_prev_prev){
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
    int bin = (int)(0.5 + ((N * freq) / SAMPLING_RATE));
    float omega = 2 * M_PI * bin / N;

    // s[n] = x[n] + 2*cos(omega)*s[n-1] - s[n-2]
    float s = 0.0f;
    for (int i=0; i<N; i++){
        s = input[i] + 2*cos(omega)*s_prev - s_prev_prev;
        s_prev_prev = s_prev;
        s_prev = s;
    }

    // Calculate the power - s[N-1]^2 + s[N-2]^2 - 2*cos(2pik/N)*s[N-1]*s[N-2]
    float power = (s_prev_prev * s_prev_prev) + (s_prev * s_prev) - 2*cos(2*omega) * s_prev * s_prev_prev;
    return power;

}


float NoteDetector::detect_note(float* input){
    /// @brief Detects the note in the input signal using the Goertzel algorithm
    /// @param input Pointer to the input signal array 
    /// @return The frequency of the detected note, or 0.0 if no note is detected
    // find the maximum amplitude
    float max_power = 0.0f;
    float max_freq = 0.0f;
    for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++){
        float power = DSP::Goertzel(input, NoteDetector::notes[i]);
        // Serial.printf("Goertzel power for %f: %f\n", notes[i], power);
        if (power > max_power) { // Threshold to detect a note
            max_power = power;
            max_freq = notes[i];
        }
    }
    return max_freq; // Return the detected frequency
}

 bool compareDescending(Goertzel_item &a, Goertzel_item &b) {
    return a.power > b.power; // Descending: larger values come first
}



void NoteDetector::core_freqs(float* input, Goertzel_item* goertzel_powers, int len){
    /// @brief Detects the core frequencies in the input signal using the Goertzel algorithm
    /// @param input Pointer to the input signal array
    /// @param goertzel_powers Pointer to an array of Goertzel_item to store the detected frequencies and their powers
    /// @param len The length of the goertzel_powers array (default is 36)
    /// @return An array of detected core frequencies, or an empty array if no frequencies are detected

    float core_freqs[6] = {0.0f}; 
    for (int i = 0; i < sizeof(NoteDetector::notes) / sizeof(NoteDetector::notes[0]); i++){
        goertzel_powers[i].freq = NoteDetector::notes[i]; 
        goertzel_powers[i].power = DSP::Goertzel(input, NoteDetector::notes[i]);
    }
    std::sort(goertzel_powers, goertzel_powers + len, compareDescending); // Sort the powers in descending order
}



bool NoteDetector::correct_detection(float note_to_play, Goertzel_item* goertzel_powers){
    /// @brief Checks if the detected note is a valid note
    /// @param detected_note The frequency of the detected note
    /// @return True if the detected note is a valid note, false otherwise

    if (goertzel_powers[0].freq == note_to_play || goertzel_powers[1].freq == note_to_play){ 
        return true; 
    }

    // if we didn't find, check for higher octaves of the same note
    if (goertzel_powers[0].freq == note_to_play * 2 || goertzel_powers[1].freq == note_to_play * 2
    || goertzel_powers[0].freq == note_to_play / 2 || goertzel_powers[1].freq == note_to_play / 2){ 
        return true; 
    }

    return false; 
}


