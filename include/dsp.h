#pragma once

float Goertzel(float* input, float freq);
float detect_note(float* input);
#define N 512 // number of samples
#define SAMPLING_RATE 10000 //Hz

// frequencies of notes:
#define C1 32.70
#define C1_SHARP 34.65
#define D1 36.71
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
#define D2 73.42
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