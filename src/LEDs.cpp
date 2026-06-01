#include "LEDs.h"
#include <Adafruit_NeoPixel.h>


Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);
// Parsed fret arrays

int parsedE[MAX_TAB_LENGTH]; int lenE = 0;
int parsedB[MAX_TAB_LENGTH]; int lenB = 0;
int parsedG[MAX_TAB_LENGTH]; int lenG = 0;
int parsedD[MAX_TAB_LENGTH]; int lenD = 0;
int parsedA[MAX_TAB_LENGTH]; int lenA = 0;
int parsedLowE[MAX_TAB_LENGTH]; int lenLowE = 0;

const GuitarTab Guitar::guitar_tabs[6][13] = {
   { {E2, 75}, {F2, 74}, {F2_SHARP, 61}, {G2, 60}, {G2_SHARP, 48}, {A2, 47}, {A2_SHARP, 36}, {B2, 35}, {C3, 24}, {C3_SHARP, 23}, {D3, 12}, {D3_SHARP, 11}, {E3, 0} },
    { {A2, 76}, {A2_SHARP, 73}, {B2, 62}, {C3, 59}, {C3_SHARP, 49}, {D3, 46}, {D3_SHARP, 37}, {E3, 34}, {F3, 25}, {F3_SHARP, 22}, {G3, 13}, {G3_SHARP, 10}, {A3, 1} },
    { {D3, 77}, {D3_SHARP, 72}, {E3, 63}, {F3, 58}, {F3_SHARP, 50}, {G3, 45}, {G3_SHARP, 38}, {A3, 33}, {A3_SHARP, 26}, {B3, 21}, {C4, 14}, {C4_SHARP, 9}, {D4, 2} },
    { {G3, 79}, {G3_SHARP, 70}, {A3, 65}, {A3_SHARP, 56}, {B3, 51}, {C4, 44}, {C4_SHARP, 39}, {D4, 32}, {D4_SHARP, 27}, {E4, 20}, {F4, 15}, {F4_SHARP, 8}, {G4, 3} },
    { {B3, 80}, {C4, 69}, {C4_SHARP, 66}, {D4, 55}, {D4_SHARP, 52}, {E4, 43}, {F4, 40}, {F4_SHARP, 31}, {G4, 28}, {G4_SHARP, 19}, {A4, 16}, {A4_SHARP, 7}, {B4, 4} },
    { {E4, 81}, {F4, 68}, {F4_SHARP, 67}, {G4, 54}, {G4_SHARP, 53}, {A4, 42}, {A4_SHARP, 41}, {B4, 30}, {C5, 29}, {C5_SHARP, 18}, {D5, 17}, {D5_SHARP, 6}, {E5, 5} }
};
GuitarTab songNotes[NUM_STRINGS][MAX_TAB_LENGTH];


int Guitar::matrixIndex(int row, int col) {
  if (col % 2 == 0)
    return col * NUM_ROWS + row;
  else
    return col * NUM_ROWS + (NUM_ROWS - 1 - row);
}

void Guitar::updateLeds() {
  for (int row = 0; row < NUM_ROWS; row++) {
    for (int col = 0; col < NUM_COLS; col++) {
      int idx = matrixIndex(row, col);
      ws2812b.setPixelColor(idx, ledMatrix[row][col]);
    }
  }
  ws2812b.show();
}

void Guitar::parseTabLine(const std::string& line, int* output, int& length) {
    // this takes a given string from loadFullTabFromFile and parses it into an array of integers
    // there a -1 if no note is played in the current time step
  length = 0;
  for (size_t i = 0; i < line.length(); i++) { // using size_t to avoid overflow issues and for better practices
    if (isdigit(line[i])) {
      if (i + size_t (1) < line.length() && isdigit(line[i + 1])) {
        output[length++] = (line[i] - '0') * 10 + (line[i + 1] - '0');
        i++;
      } else {
        output[length++] = line[i] - '0';
      }
    } else {
      output[length++] = -1;
    }
  }
}

void Guitar::displayStep(int current_step) {
  memset(ledMatrix, 0, sizeof(ledMatrix));
  if (current_step < lenLowE && parsedLowE[current_step] >= 0)
    ledMatrix[0][parsedLowE[current_step]] = ws2812b.Color(255, 0, 0);
  if (current_step < lenA && parsedA[current_step] >= 0)
    ledMatrix[1][parsedA[current_step]] = ws2812b.Color(255, 165, 0);
  if (current_step < lenD && parsedD[current_step] >= 0)
    ledMatrix[2][parsedD[current_step]] = ws2812b.Color(0, 255, 0);
  if (current_step < lenG && parsedG[current_step] >= 0)
    ledMatrix[3][parsedG[current_step]] = ws2812b.Color(0, 255, 255);
  if (current_step < lenB && parsedB[current_step] >= 0)
    ledMatrix[4][parsedB[current_step]] = ws2812b.Color(255, 0, 255);
  if (current_step < lenE && parsedE[current_step] >= 0)
    ledMatrix[5][parsedE[current_step]] = ws2812b.Color(255, 255, 255);

  updateLeds();
}



bool Guitar::loadFullTabFromFile(const std::string& filename,
                         std::string& line_e,
                         std::string& line_B,
                         std::string& line_G,
                         std::string& line_D,
                         std::string& line_A,
                         std::string& line_E)
{
    // this takes a file and seperates it into 6 strings pun - intended
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    int line_num = 0;

    while (std::getline(file, line)) {
        int idx = line_num % 6;
        switch (idx) {
            case 0: line_e += line.substr(2); break;
            case 1: line_B += line.substr(2); break;
            case 2: line_G += line.substr(2); break;
            case 3: line_D += line.substr(2); break;
            case 4: line_A += line.substr(2); break;
            case 5: line_E += line.substr(2); break;
        }
        line_num++;
    }

    file.close();

    // Add the string labels back to the beginning
    line_e = "e|" + line_e;
    line_B = "B|" + line_B;
    line_G = "G|" + line_G;
    line_D = "D|" + line_D;
    line_A = "A|" + line_A;
    line_E = "E|" + line_E;

    // Check we actually read complete blocks
    if (line_num % 6 != 0) {
        std::cerr << "Warning: tab file has incomplete block (lines not a multiple of 6)." << std::endl;
    }

    return true;
}




void Guitar::getSongSteps(const int parsedLowE[], const int parsedA[], const int parsedD[], 
                  const int parsedG[], const int parsedB[], const int parsedHiE[], 
                  int length, GuitarTab output[NUM_STRINGS][MAX_TAB_LENGTH]) {
    
    length = std::min(length, MAX_TAB_LENGTH);
    
    // Initialize output array
    for (int s = 0; s < NUM_STRINGS; ++s) {
        for (int i = 0; i < MAX_TAB_LENGTH; ++i) {
            output[s][i].freq = -1;
            output[s][i].led_index = -1;
        }
    }
    
// Array of pointers to parsed arrays - reordered so lowE is at index 1 (bottom)
    const int* parsedArrays[NUM_STRINGS] = {parsedLowE, parsedA,parsedD , parsedG, parsedB, parsedHiE};
    
    int outputIndex = 0; // Track where to write in the output array
    
    for (int i = 0; i < length; ++i) {
        // Check if all strings at position i are -1
        bool allEmpty = true;
        for (int s = 0; s < NUM_STRINGS; ++s) {
            if (parsedArrays[s][i] >= 0) {
                allEmpty = false;
                break;
            }
        }
        
        // If not all empty, copy this step to the output
        if (!allEmpty) {
            for (int s = 0; s < NUM_STRINGS; ++s) {
                if (parsedArrays[s][i] >= 0) {
                    output[s][outputIndex] = guitar_tabs[s][parsedArrays[s][i]];
                }
                // If parsedArrays[s][i] is -1, output[s][outputIndex] remains initialized to -1
            }
            outputIndex++;
            
            // Stop if we've filled the output array
            if (outputIndex >= MAX_TAB_LENGTH) {
                break;
            }
        }
        // If all empty (all -1), skip this step entirely
    }
}

