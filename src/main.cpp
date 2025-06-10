#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <string> // Required for std::string
#include <vector> // Often used with std::string for dynamic arrays
#include <cctype>   // For isdigit

#define NUM_ROWS    8
#define NUM_COLS    10
#define NUM_PIXELS  (NUM_ROWS * NUM_COLS)
#define PIN_WS2812B D1
#define BUTTON_PIN 0  // GPIO0 is usually the flash button
#define MAX_TAB_LENGTH 100

Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

// A matrix holding color values for each LED (8 rows × 10 columns)
// row 0 is on the bottom, row 7 is on the top
// column 0 is on the left, column 9 is on the right
uint32_t ledMatrix[NUM_ROWS][NUM_COLS];
void updateLeds();

// Parsed fret arrays
int parsedE[MAX_TAB_LENGTH]; int lenE = 0;
int parsedB[MAX_TAB_LENGTH]; int lenB = 0;
int parsedG[MAX_TAB_LENGTH]; int lenG = 0;
int parsedD[MAX_TAB_LENGTH]; int lenD = 0;
int parsedA[MAX_TAB_LENGTH]; int lenA = 0;
int parsedLowE[MAX_TAB_LENGTH]; int lenLowE = 0;

// Current step (position in tab)
int step = 0;

// ---------------- Helper Functions ----------------

int matrixIndex(int row, int col) {
  if (col % 2 == 0) { 
    return col * NUM_ROWS + row;
  } 
  else { 
    return col * NUM_ROWS + (NUM_ROWS - 1 - row);
  }
}

void updateLeds() {
  for (int row = 0; row < NUM_ROWS; row++) {
    for (int col = 0; col < NUM_COLS; col++) {
      int idx = matrixIndex(row, col);
      ws2812b.setPixelColor(idx, ledMatrix[row][col]);
    }
  }
  ws2812b.show();
}

// Parse one line of tab into an array of frets
void parseTabLine(const std::string& line, int* output, int& length) {
    length = 0;
    for (int i = 0; i < line.length(); i++) {
        if (isdigit(line[i])) {
            if (i + 1 < line.length() && isdigit(line[i + 1])) { // Check bounds for i+1
                output[length++] = (line[i] - '0') * 10 + (line[i + 1] - '0');
                i++; // Skip the next digit as it's already processed
            }
            else {
                output[length++] = line[i] - '0';
            }
        } 
        else {
            output[length++] = -1; // No note at this step
        }
    }
}


// Display notes at a given step
void displayStep(int current_step) { // Renamed 'step' to 'current_step' to avoid confusion with global 'step'
  memset(ledMatrix, 0, sizeof(ledMatrix)); // Clear matrix to all black

  // Map guitar strings to LED matrix rows (0=bottom, 5=top for 6 strings)
  // Low E string is physical bottom, high e string is physical top.
  if (current_step < lenLowE && parsedLowE[current_step] >= 0)
    ledMatrix[0][parsedLowE[current_step]] = ws2812b.Color(255, 0, 0);   // Low E string (Row 0)
  if (current_step < lenA && parsedA[current_step] >= 0)
    ledMatrix[1][parsedA[current_step]] = ws2812b.Color(255, 165, 0); // A string (Row 1)
  if (current_step < lenD && parsedD[current_step] >= 0)
    ledMatrix[2][parsedD[current_step]] = ws2812b.Color(0, 255, 0); // D string (Row 2)
  if (current_step < lenG && parsedG[current_step] >= 0)
    ledMatrix[3][parsedG[current_step]] = ws2812b.Color(0, 255, 255);// G string (Row 3)
  if (current_step < lenB && parsedB[current_step] >= 0)
    ledMatrix[4][parsedB[current_step]] = ws2812b.Color(255, 0, 255); // B string (Row 4)
  if (current_step < lenE && parsedE[current_step] >= 0)
    ledMatrix[5][parsedE[current_step]] = ws2812b.Color(255, 255, 255); // e string (Row 5)

  updateLeds();
}

// ---------------- Setup & Loop ----------------

void setup() {
  ws2812b.begin(); // INITIALIZE WS2812B strip object (REQUIRED)
  ws2812b.clear();      // Turn all LEDs off
  ws2812b.show();       // Push the updates to the strip

  // Step 1: Initialize all LEDs to "off"
  for (int row = 0; row < NUM_ROWS; row++) {
    for (int col = 0; col < NUM_COLS; col++) {
      ledMatrix[row][col] = ws2812b.Color(0, 0, 0); // off
    }
  }
// Sultans of Swing tab data (one bar)
  std::string line_e = "e|-------------------------------------5-8-5----------------|";
  std::string line_B = "B|-------3--3-3------------------6----6-----6-5-6-5---------|";
  std::string line_G = "G|---2-2-2--2-2--5-5-5-7-----7----7--7-------------7--5-7---|";
  std::string line_D = "D|-0-0-0-0--0-0--5-5-5-7--5-7---7------------------------7--|";
  std::string line_A = "A|-0----------------5---------------------------------------|";
  std::string line_E = "E|----------------------------------------------------------|";

// Parse all lines
  parseTabLine(line_e, parsedE, lenE);
  parseTabLine(line_B, parsedB, lenB);
  parseTabLine(line_G, parsedG, lenG);
  parseTabLine(line_D, parsedD, lenD);
  parseTabLine(line_A, parsedA, lenA);
  parseTabLine(line_E, parsedLowE, lenLowE);

  displayStep(step);
}

void loop() {
  static bool lastButtonState = HIGH;
  bool currentState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && currentState == LOW) {
    // Determine the maximum length among all parsed tab lines
    // This defines the "end" of our tab data.
    int maxOverallLength = 0;
    if (lenE > maxOverallLength) maxOverallLength = lenE;
    if (lenB > maxOverallLength) maxOverallLength = lenB;
    if (lenG > maxOverallLength) maxOverallLength = lenG;
    if (lenD > maxOverallLength) maxOverallLength = lenD;
    if (lenA > maxOverallLength) maxOverallLength = lenA;
    if (lenLowE > maxOverallLength) maxOverallLength = lenLowE;

    // Loop to find the next non-blank step
    // The do-while ensures we always try to advance and check at least once.
    do {
      step++; // Try the next step

      // If we go past the end of the longest tab line, wrap back to the start (step 0)
      if (step >= maxOverallLength) {
        step = 0;
      }

      // Check if the current 'step' has at least one valid note (not -1)
      bool hasAnyNote = false;
      if (step < lenE && parsedE[step] >= 0) hasAnyNote = true;
      if (step < lenB && parsedB[step] >= 0) hasAnyNote = true;
      if (step < lenG && parsedG[step] >= 0) hasAnyNote = true;
      if (step < lenD && parsedD[step] >= 0) hasAnyNote = true;
      if (step < lenA && parsedA[step] >= 0) hasAnyNote = true;
      if (step < lenLowE && parsedLowE[step] >= 0) hasAnyNote = true;

      if (hasAnyNote) {
        break; // Found a step with a note, stop skipping
      }

    } while (true); 

    displayStep(step);
  }

  lastButtonState = currentState; // Update button state for next loop iteration
  delay(50); // Small delay to debounce button presses
}