// helpers.h
#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <string>
#include <vector>   // for std::vector
#include <cctype>   // for isdigit
#include "dsp.h" 
#include "guitar.h" 
#include <iostream>
#include <fstream>

#define NUM_ROWS    8
#define NUM_COLS    13
#define NUM_PIXELS  (NUM_ROWS * NUM_COLS)
#define PIN_WS2812B D1
#define MAX_TAB_LENGTH 1000

class GuitarTab {
    public:
        int freq;
        int led_index;
};

class Guitar {
    public:
    static const GuitarTab guitar_tabs[6][13] = {
        { {E2, 5}, {F2, 10}, {F2_SHARP, 21}, {G2, 26}, {G2_SHARP, 37}, {A2, 42}, {A2_SHARP, 53}, {B2, 58}, {C3, 69}, {C3_SHARP, 74}, {D3, 85}, {D3_SHARP, 90}, {E3, 101} },
        { {A2, 4}, {A2_SHARP, 11}, {B2, 20}, {C3, 27}, {C3_SHARP, 36}, {D3, 43}, {D3_SHARP, 52}, {E3, 59}, {F3, 68}, {F3_SHARP, 75}, {G3, 84}, {G3_SHARP, 91}, {A3, 100} },
        { {D3, 3}, {D3_SHARP, 12}, {E3, 19}, {F3, 28}, {F3_SHARP, 35}, {G3, 44}, {G3_SHARP, 51}, {A3, 60}, {A3_SHARP, 67}, {B3, 76}, {C4, 83}, {C4_SHARP, 92}, {D4, 99} },
        { {G3, 2}, {G3_SHARP, 13}, {A3, 18}, {A3_SHARP, 29}, {B3, 34}, {C4, 45}, {C4_SHARP, 50}, {D4, 61}, {D4_SHARP, 66}, {E4, 77}, {F4, 82}, {F4_SHARP, 93}, {G4, 98} },
        { {B3, 1}, {C4, 14}, {C4_SHARP, 17}, {D4, 30}, {D4_SHARP, 33}, {E4, 46}, {F4, 49}, {F4_SHARP, 62}, {G4, 65}, {G4_SHARP, 78}, {A4, 81}, {A4_SHARP, 94}, {B4, 97} },
        { {E4, 0}, {F4, 15}, {F4_SHARP, 16}, {G4, 31}, {G4_SHARP, 32}, {A4, 47}, {A4_SHARP, 48}, {B4, 63}, {C5, 64}, {C5_SHARP, 79}, {D5, 80}, {D5_SHARP, 95}, {E5, 96} }
    };

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
    void getSongSteps(GuitarTab* output, int length);

}



#endif