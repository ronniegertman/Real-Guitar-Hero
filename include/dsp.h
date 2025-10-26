#pragma once

#define N 2048 // number of samples
#define SAMPLING_RATE 5120 //Hz

// frequencies of notes:
#define C1 32.70
#define C1_SHARP 34.65
#define D_1 36.71
#define D1_SHARP 38.89
#define E1 41.20
#define F1 43.65
#define F1_SHARP 46.25
#define G1 49.00
#define G1_SHARP 51.91
#define A1 55.00
#define A1_SHARP 58.27
#define B_1 61.74
#define C2 65.41
#define C2_SHARP 69.30
#define D_2 73.42
#define D2_SHARP 77.78
#define E2 82.41
#define F2 87.31
#define F2_SHARP 92.50
#define G2 98.00
#define G2_SHARP 103.83
#define A2 110.00
#define A2_SHARP 116.54
#define B2 123.47
#define C3 130.81
#define C3_SHARP 138.59
#define D3 146.83
#define D3_SHARP 155.56
#define E3 164.81
#define F3 174.61
#define F3_SHARP 185.00
#define G3 196.00
#define G3_SHARP 207.65
#define A3 220.00
#define A3_SHARP 233.08
#define B3 246.94
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
#define C5_SHARP 554.37
#define D5 587.33
#define D5_SHARP 622.25
#define E5 659.25
#define F5 698.46
#define F5_SHARP 739.99
#define G5 783.99
#define G5_SHARP 830.61
#define A5 880.00
#define A5_SHARP 932.33
#define B5 987.77
#define C6 1046.50
#define C6_SHARP 1108.73
#define D6 1174.66
#define D6_SHARP 1244.51
#define E6 1318.51

struct Goertzel_item{
    float freq;
    float power;
};

class DSP {
    public:
    static float Goertzel(float* input, float freq ,float s_prev=0, float s_prev_prev=0);
};

class NoteDetector {
    public:
        static constexpr float notes[49] = {
        E2, F2, F2_SHARP, G2, G2_SHARP, A2, A2_SHARP, B2, C3, C3_SHARP,
        D3, D3_SHARP, E3, F3, F3_SHARP, G3, G3_SHARP, A3,
        A3_SHARP, B3, C4, C4_SHARP, D4, D4_SHARP, E4, F4,
        F4_SHARP, G4, G4_SHARP, A4, A4_SHARP, B4, C5, C5_SHARP,
        D5, D5_SHARP, E5, F5, F5_SHARP, G5, G5_SHARP, A5,
        A5_SHARP, B5, C6, C6_SHARP, D6, D6_SHARP, E6
        };

        float detect_note(float* input);
        static void core_freqs(float* input, Goertzel_item* goertzel_powers, int len=36);
        static bool correct_detection(float note_to_play, Goertzel_item* goertzel_powers);

};