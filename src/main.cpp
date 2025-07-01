#include "helpers.h"

#define BUTTON_PIN 0  // GPIO0 is usually the flash button


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
std::string line_e, line_B, line_G, line_D, line_A, line_E;

if (loadFullTabFromFile("/tab.txt", line_e, line_B, line_G, line_D, line_A, line_E)) {
    parseTabLine(line_e, parsedE, lenE);
    parseTabLine(line_B, parsedB, lenB);
    parseTabLine(line_G, parsedG, lenG);
    parseTabLine(line_D, parsedD, lenD);
    parseTabLine(line_A, parsedA, lenA);
    parseTabLine(line_E, parsedLowE, lenLowE);
} else {
    Serial.println("Failed to load tab file.");
}

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