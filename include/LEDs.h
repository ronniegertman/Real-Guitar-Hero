// helpers.h
#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <string>
#include <vector>   // for std::vector
#include <cctype>   // for isdigit
#include "dsp.h" 

#include <iostream>
#include <fstream>

#define NUM_ROWS    8
#define NUM_COLS    13
#define NUM_PIXELS  (NUM_ROWS * NUM_COLS)
#define PIN_WS2812B D1
#define MAX_TAB_LENGTH 1000
#define NUM_STRINGS 6

extern uint32_t ledMatrix[NUM_ROWS][NUM_COLS];

extern Adafruit_NeoPixel ws2812b;

extern int parsedE[MAX_TAB_LENGTH]; 
extern int lenE;
extern int parsedB[MAX_TAB_LENGTH]; 
extern int lenB;
extern int parsedG[MAX_TAB_LENGTH]; 
extern int lenG;
extern int parsedD[MAX_TAB_LENGTH]; 
extern int lenD;
extern int parsedA[MAX_TAB_LENGTH]; 
extern int lenA;
extern int parsedLowE[MAX_TAB_LENGTH]; 
extern int lenLowE;

class GuitarTab {
    public:
        double freq;
        int led_index;
};

class Guitar {
    public:
     static const GuitarTab guitar_tabs[6][13];

    static int matrixIndex(int row, int col); // we don't use this anymore
    void updateLeds(); // we don't use this anymore
    bool loadFullTabFromFile(const std::string& filename,
                         std::string& line_e,
                         std::string& line_B,
                         std::string& line_G,
                         std::string& line_D,
                         std::string& line_A,
                         std::string& line_E);
    static void parseTabLine(const std::string& line, int* output, int& length); 
    void displayStep(int current_step); // we don't use this anymore
    void getSongSteps(const int parsedLowE[], const int parsedA[], const int parsedD[], 
                  const int parsedG[], const int parsedB[], const int parsedHiE[], 
                  int length, GuitarTab output[NUM_STRINGS][MAX_TAB_LENGTH]);

};



#endif