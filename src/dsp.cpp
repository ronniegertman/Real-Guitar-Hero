#include <math.h>
#define N 512 // number of samples
#define SAMPLING_RATE 10000 //Hz

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