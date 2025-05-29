#include <arduinoFFT.h>

#define SAMPLES         1024          // #bins of frequecies
#define SAMPLING_FREQ   40000  // Sampling frequency in Hz

float vReal[SAMPLES];
float vImag[SAMPLES];

ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, SAMPLES, SAMPLING_FREQ); /* Create FFT object */
