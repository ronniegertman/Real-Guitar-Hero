#include <algorithm>
#include <math.h>
#include <Arduino.h>
#include "dsp.h"

class DSP{
    public:
    float notes[49] = {
    E2, F2, F2_SHARP, G2, G2_SHARP, A2, A2_SHARP, B2, C3, C3_SHARP,
    D3, D3_SHARP, E3, F3, F3_SHARP, G3, G3_SHARP, A3,
    A3_SHARP, B3, C4, C4_SHARP, D4, D4_SHARP, E4, F4,
    F4_SHARP, G4, G4_SHARP, A4, A4_SHARP, B4, C5, C5_SHARP,
    D5, D5_SHARP, E5, F5, F5_SHARP, G5, G5_SHARP, A5,
    A5_SHARP, B5, C6, C6_SHARP, D6, D6_SHARP, E6
};

float Goertzel(float* input, float freq ,float s_prev=0, float s_prev_prev=0){
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

float detect_note(float* input){
    /// @brief Detects the note in the input signal using the Goertzel algorithm
    /// @param input Pointer to the input signal array 
    /// @return The frequency of the detected note, or 0.0 if no note is detected
    // find the maximum amplitude
    float max_power = 0.0f;
    float max_freq = 0.0f;
    for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++){
        float power = Goertzel(input, notes[i]);
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

void core_freqs(float* input, Goertzel_item* goertzel_powers){
    /// @brief Detects the core frequencies in the input signal using the Goertzel algorithm
    /// @param input Pointer to the input signal array
    /// @return An array of detected core frequencies, or an empty array if no frequencies are detected

    float core_freqs[6] = {0.0f}; 
    for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++){
        goertzel_powers[i].freq = notes[i]; 
        goertzel_powers[i].power = Goertzel(input, notes[i]); 
    }
    std::sort(goertzel_powers, goertzel_powers + 36, compareDescending); // Sort the powers in descending order
}

bool correct_detection(float note_to_play, Goertzel_item* goertzel_powers){
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
};

