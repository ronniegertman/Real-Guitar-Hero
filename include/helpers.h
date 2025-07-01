// helpers.h
#ifndef HELPERS_H
#define HELPERS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <string>
#include <vector>   // for std::vector
#include <cctype>   // for isdigit
#include "notes.h"  
#include <iostream>
#include <fstream>

#define NUM_ROWS    8
#define NUM_COLS    13
#define NUM_PIXELS  (NUM_ROWS * NUM_COLS)
#define PIN_WS2812B D1
#define MAX_TAB_LENGTH 1000

extern Adafruit_NeoPixel ws2812b;
extern uint32_t ledMatrix[NUM_ROWS][NUM_COLS];

class GuitarTab{
    public:
    float note_frequency;
    float LED_INDEX;
};

extern GuitarTab guitar_tabs[6][13];
// Global parsed tab data
extern int parsedE[], parsedB[], parsedG[], parsedD[], parsedA[], parsedLowE[];
extern int lenE, lenB, lenG, lenD, lenA, lenLowE;
extern int step;

// Helper function declarations
int matrixIndex(int row, int col);
void updateLeds();
void parseTabLine(const std::string& line, int* output, int& length);
void displayStep(int current_step);
bool loadFullTabFromFile(const std::string& filename,
                         std::string& line_e,
                         std::string& line_B,
                         std::string& line_G,
                         std::string& line_D,
                         std::string& line_A,
                         std::string& line_E);

#endif