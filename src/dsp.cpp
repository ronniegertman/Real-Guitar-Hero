#include <math.h>
#define N 512 // number of samples
#define SAMPLING_RATE 10000 //Hz

// frequencies of notes:
#define C4 261.63
#define C4_SHARP 277.18
#define D4 293.66
#define D4_SHARP 311.13
#define E4 329.63
#define F4 349.23
#define F4_SHARP 369.99
#define G4 392.00
#define G4_SHARP 415.30
#define A4 440.00
#define A4_SHARP 466.16
#define B4 493.88
#define C5 523.25

float notes[] = {
    C4, C4_SHARP, D4, D4_SHARP, E4, F4, F4_SHARP, G4, G4_SHARP, A4, A4_SHARP, B4, C5
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
    int bin = (int)(N * freq / SAMPLING_RATE);
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
    float power = (s_prev_prev * s_prev_prev) + (s_prev * s_prev) - 2*cos(omega) * s_prev * s_prev_prev;
    return power;

}


float detect_note(float* input){
    // find the maximum amplitude
    float max_power = 0.0f;
    float max_freq = 0.0f;
    for (int i = 0; i < 13; i++){
        float power = Goertzel(input, notes[i]);
        if (power > max_power) { // Threshold to detect a note
            max_power = power;
            max_freq = notes[i];
        }
    }
    return max_freq; // Return the detected frequency

}

