#include "LEDs.h"
#include <Adafruit_NeoPixel.h>

// Ensure these macros are defined before use, or include the header where they are defined
#ifndef NUM_PIXELS
#define NUM_PIXELS 60 
#endif

#ifndef PIN_WS2812B
#define PIN_WS2812B 6 
#endif

#ifndef NUM_ROWS
#define NUM_ROWS 6 // or your actual number of rows
#endif

#ifndef NUM_COLS
#define NUM_COLS 24 // or your actual number of columns
#endif

Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);
uint32_t ledMatrix[NUM_ROWS][NUM_COLS];
extern int parsedE[], parsedB[], parsedG[], parsedD[], parsedA[], parsedLowE[];
extern int lenE, lenB, lenG, lenD, lenA, lenLowE;

extern int step;
// Parsed fret arrays
int parsedE[MAX_TAB_LENGTH]; int lenE = 0;
int parsedB[MAX_TAB_LENGTH]; int lenB = 0;
int parsedG[MAX_TAB_LENGTH]; int lenG = 0;
int parsedD[MAX_TAB_LENGTH]; int lenD = 0;
int parsedA[MAX_TAB_LENGTH]; int lenA = 0;
int parsedLowE[MAX_TAB_LENGTH]; int lenLowE = 0;
int step = 0;

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

void Guitar::getSongSteps(GuitarTab* output, int length) {
// output should be an array of 6 strings tabs. 
//if there are no notes to play, don't add anything to the array.
    for (int i = 0; i< MAX_TAB_LENGTH; i++){
        if(parsedLowE[i] >= 0 && i < length) {
            int x = 0;
            int y = parsedLowE[i];
            output[i] = guitar_tabs[x][y];
        }
        if(parsedA[i] >= 0 && i < length) {
            int x = 1;
            int y = parsedA[i];
            output[i] = guitar_tabs[x][y];
        }
        if(parsedD[i] >= 0 && i < length) {
            int x = 2;
            int y = parsedD[i];
            output[i] = guitar_tabs[x][y];
        }
        if(parsedG[i] >= 0 && i < length) {
            int x = 3;
            int y = parsedG[i];
            output[i] = guitar_tabs[x][y];
        }
        if(parsedB[i] >= 0 && i < length) {
            int x = 4;
            int y = parsedB[i];
            output[i] = guitar_tabs[x][y];
        }
        if(parsedE[i] >= 0 && i < length) {
            int x = 5;
            int y = parsedE[i];
            output[i] = guitar_tabs[x][y];
        }
    }
}