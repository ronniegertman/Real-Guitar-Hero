#include "LEDs.h"
#include <Adafruit_NeoPixel.h>

// Ensure these macros are defined before use, or include the header where they are defined
#ifndef NUM_PIXELS
#define NUM_PIXELS (NUM_COLS*NUM_ROWS)
#endif

#ifndef PIN_WS2812B
#define PIN_WS2812B D1 
#endif

#ifndef NUM_ROWS
#define NUM_ROWS 8 // or your actual number of rows
#endif

#ifndef NUM_COLS
#define NUM_COLS 13 // or your actual number of columns
#endif

Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);
uint32_t ledMatrix[NUM_ROWS][NUM_COLS];

// Parsed fret arrays
int parsedE[MAX_TAB_LENGTH]; int lenE = 0;
int parsedB[MAX_TAB_LENGTH]; int lenB = 0;
int parsedG[MAX_TAB_LENGTH]; int lenG = 0;
int parsedD[MAX_TAB_LENGTH]; int lenD = 0;
int parsedA[MAX_TAB_LENGTH]; int lenA = 0;
int parsedLowE[MAX_TAB_LENGTH]; int lenLowE = 0;
int step = 0;

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
  for (int i = 0; i < line.length(); i++) {
    if (isdigit(line[i])) {
      if (i + 1 < line.length() && isdigit(line[i + 1])) {
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
    
    // Array of pointers to parsed arrays for easier iteration
    const int* parsedArrays[NUM_STRINGS] = {parsedLowE, parsedA, parsedD, parsedG, parsedB, parsedHiE};
    
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


