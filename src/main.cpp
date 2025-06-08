#include <Arduino.h>


#include <Adafruit_NeoPixel.h>

#define NUM_ROWS    8
#define NUM_COLS    10
#define NUM_PIXELS  (NUM_ROWS * NUM_COLS)
#define PIN_WS2812B D1
#define BUTTON_PIN 0  // GPIO0 is usually the flash button

Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

// A matrix holding color values for each LED (8 rows × 10 columns)
// row 0 is on the bottom, row 7 is on the top
// column 0 is on the left, column 9 is on the right
uint32_t ledMatrix[NUM_ROWS][NUM_COLS];
void updateLeds();

void setup() {
  ws2812b.begin(); // INITIALIZE WS2812B strip object (REQUIRED)
  ws2812b.clear();      // Turn all LEDs off
  ws2812b.show();       // Push the updates to the strip

  // Initialize the ledMatrix with a color (e.g., red)
  // Step 1: Initialize all LEDs to "off"
  for (int row = 0; row < NUM_ROWS; row++) {
    for (int col = 0; col < NUM_COLS; col++) {
      ledMatrix[row][col] = ws2812b.Color(0, 0, 0); // off
    }
  }

  // Step 2: Set entire row 3 to green
  for (int col = 0; col < NUM_COLS; col++) {
    ledMatrix[3][col] = ws2812b.Color(0, 255, 0); // green
  }

  // Step 3: Set entire column 5 to red
  for (int row = 0; row < NUM_ROWS; row++) {
    ledMatrix[row][5] = ws2812b.Color(255, 0, 0); // red
  }

  //
  updateLeds();
}

void loop() {
//Update the matrix dynamically
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50);  // simple debounce
    updateLeds();  // re-send LED matrix
    while (digitalRead(BUTTON_PIN) == LOW); // wait for release
  }
}

// Zigzag index mapping
int matrixIndex(int row, int col) {
  if (col % 2 == 0) {
    // Even column: bottom → top
    return col * NUM_ROWS + row;
  } else {
    // Odd column: top → bottom
    return col * NUM_ROWS + (NUM_ROWS - 1 - row);
  }
}
// Send ledMatrix color data to the LED strip
void updateLeds() {
  for (int row = 0; row < NUM_ROWS; row++) {
    for (int col = 0; col < NUM_COLS; col++) {
      int idx = matrixIndex(row, col);
      ws2812b.setPixelColor(idx, ledMatrix[row][col]);
    }
  }
  ws2812b.show();
}