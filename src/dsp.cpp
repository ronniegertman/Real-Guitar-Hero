#include <algorithm>
#include <math.h>
#include <Arduino.h>
#include "dsp.h"

float notes[] = {
    C1, C1_SHARP, D1, D1_SHARP, E1, F1, F1_SHARP, G1, G1_SHARP,
    A1, A1_SHARP, B_1, C2, C2_SHARP, D2, D2_SHARP, E2, F2,
    F2_SHARP, G2, G2_SHARP, A2, A2_SHARP, B2, C3, C3_SHARP,
    D3, D3_SHARP, E3, F3, F3_SHARP, G3, G3_SHARP, A3,
    A3_SHARP, B3, C4, C4_SHARP, D4, D4_SHARP, E4, F4,
    F4_SHARP, G4, G4_SHARP, A4, A4_SHARP, B4, C5
};

float Goertzel(float* input, float freq){
    /// @brief Goertzel algorithm for detecting a specific frequency in a signal
    /// @param input Pointer to the input signal array
    /// @param freq Frequency to detect in the input signal
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
    float s_prev = 0.0f;
    float s_prev_prev = 0.0f;
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
    for (int i = 0; i < 48; i++){
        float power = Goertzel(input, notes[i]);
        // Serial.printf("Goertzel power for %f: %f\n", notes[i], power);
        if (power > max_power) { // Threshold to detect a note
            max_power = power;
            max_freq = notes[i];
        }
    }
    return max_freq; // Return the detected frequency
}


bool compareDescending(float a, float b) {
    return a > b; // Descending: larger values come first
}


float* core_freqs(float* input){
    /// @brief Detects the core frequencies in the input signal using the Goertzel algorithm
    /// @param input Pointer to the input signal array
    /// @return An array of detected core frequencies, or an empty array if no frequencies are detected
    float goertzel_powers[48] = {0.0f}; // Array to hold Goertzel powers for each note

    float core_freqs[6] = {0.0f}; 
    for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++){
        goertzel_powers[i] = Goertzel(input, notes[i]);
    }
    std::sort(goertzel_powers, goertzel_powers + 48, compareDescending); // Sort the powers in descending order
    return goertzel_powers;
}

