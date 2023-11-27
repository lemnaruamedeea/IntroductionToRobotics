#include "LedControl.h"  // Include LedControl library for controlling the LED matrix
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int xPin = A0;
const int yPin = A1;
const int pinSW = 2;

unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;

// Create an LedControl object to manage the LED matrix
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);  // DIN, CLK, LOAD, No. DRIVER
// Variable to set the brightness level of the matrix
byte matrixBrightness = 2;
// Variables to track the current and previous positions of the joystick-controlled LED
byte xPos = 0;
byte yPos = 7;
byte xLastPos = 0;
byte yLastPos = 0;
byte playerLedState = LOW;
byte bombLedState = LOW;
byte buttonState = LOW;
byte reading = HIGH;
byte lastReading = HIGH;

int totalElements;
int randomRow;
int randomCol;
int checkX;
int checkY;
int bombX;
int bombY;

unsigned long bombTime = 0;                    // Time of the bomb activation
unsigned long explosionTime = 0;               // Time of the explosion
const unsigned long bombDuration = 2000;       // Duration before bomb explodes in milliseconds
const unsigned long explosionDuration = 1000;  // Duration of the explosion

const int start = 0;
const int one = 1;
int numWalls;  // Adjust the number of walls as needed
const int fillPercent = 70;
const int first2Rows = 16;
const int xInitialPos = 0;
const int yInitialPos = 7;

// Thresholds for detecting joystick movement
const int minThreshold = 200;
const int maxThreshold = 600;
const int playerBlink = 400;
const int bombBlink = 100;

const byte moveInterval = 100;  // Timing variable to control the speed of LED movement
const byte matrixSize = 8;      // Size of the LED matrix

unsigned long long lastMoved = 0;        // Tracks the last time the LED moved
unsigned long long lastPlayerBlink = 0;  // Tracks the last time the LED for player blinked
unsigned long long lastBombBlink = 0;    // Tracks the last time the LED for a bomb blinked

bool matrixChanged = true;  // Flag to track if the matrix display needs updating
bool bombPlaced = false;    // Flag to track if the bomb is placed
bool bombActive = false;    // Flag to track if the bomb is active
bool exploded = false;
bool gameLost = false;

// 2D array representing the state of each LED (on/off) in the matrix
byte matrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte happyFace[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte sadFace[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

void setup() {
  Serial.begin(9600);
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false);                 // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness);  // sets brightness (0~15 possible values)
  lc.clearDisplay(0);                    // Clear the matrix display
  pinMode(pinSW, INPUT_PULLUP);
}

void loop() {
  blinkForPlayer();

  if (millis() - lastMoved > moveInterval) {  // Check if it's time to move the LED
    // game logic
    updatePositions();     // Update the position of the LED based on joystick input
    lastMoved = millis();  // Update the time of the last move
  }

  if (matrixChanged == true) {  // Check if the matrix display needs updating
    updateMatrix();             // Update the LED matrix display
    matrixChanged = false;      // Reset the update flag
  }

  reading = digitalRead(pinSW);
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        placeBomb(xPos, yPos);  // Place the bomb at the current joystick position
      }
    }
  }
  lastReading = reading;

  // Check for bomb activation
  if (bombPlaced && !bombActive) {

    if (millis() - bombTime >= bombDuration) {
      bombActive = true;  // Activate the bomb
    }
  }

  // Blink the bomb if it's active
  if (bombActive) {
    blinkForBomb();
  }

  if (gameLost == false) {
    checkGameWon();
  }

  if (gameLost == true) {
    gameOver();
  }

  if (Serial.available() > 0) {
    char key = Serial.read();  // Read the incoming byte

    // Check if the received character matches the trigger key
    if (key == 's') {
      randomSeed(analogRead(A2));
      generateWalls();  // Generate walls if 's' key is pressed (s from start)
      xPos = xInitialPos;
      yPos = yInitialPos;
      gameLost = false;
    }
  }
}

void placeBomb(int x, int y) {
  // Place the bomb at the specified position
  bombX = x;
  bombY = y;
  bombPlaced = true;
  bombTime = millis();  // Start the timer for bomb activation
}

void explode() {
  exploded = true;

  if (bombPlaced) {
    // Explode the walls adjacent to the bomb
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        checkX = bombX + i;
        checkY = bombY + j;

        // Check if the position is within the matrix bounds
        if (checkX >= 0 && checkX < matrixSize && checkY >= 0 && checkY < matrixSize) {

          if (matrix[checkX][checkY] == 1) {
            matrix[checkX][checkY] = 0;  // Destroy the wall
          }
        }

        if (exploded && (checkX == xPos && checkY == yPos)) {
          // Player loses the game if the bomb catches them
          gameLost = true;
          // gameOver();
          exploded = false;
        }
      }
    }

    matrix[bombX][bombY] = 0;  // Clear the bomb position
    bombPlaced = false;        // Reset bomb state

    matrixChanged = true;  // Set the flag to update the matrix display
  }
}

void blinkForPlayer() {

  if (millis() - lastPlayerBlink > playerBlink) {  // Check if it's time to blink for player
    playerLedState = !playerLedState;
    matrix[xPos][yPos] = playerLedState;
    lastPlayerBlink = millis();  // Update the time of the last blink for player
    matrixChanged = true;
  }
}

void blinkForBomb() {

  if (millis() - bombTime < bombDuration) {

    if (millis() - lastBombBlink >= bombBlink) {  // Check if it's time to blink for the bomb
      bombLedState = !bombLedState;
      matrix[bombX][bombY] = bombLedState;
      lastBombBlink = millis();  // Update the time of the last blink for the bomb
      matrixChanged = true;
    }
  }

  else {
    explode();
  }
}

void generateWalls() {
  // Reset the entire matrix
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      matrix[row][col] = 0;  // Set all positions as empty initially
    }
  }

  totalElements = (matrixSize * matrixSize) - first2Rows;
  numWalls = (fillPercent * totalElements) / 100;

  for (int i = 0; i < numWalls; i++) {
    randomRow = random(2, matrixSize);     // Generate random row index
    randomCol = random(matrixSize);  // Generate random column index
    matrix[randomRow][randomCol] = 1;   // Set the position as a wall
  }

  matrixChanged = true;
}

void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);  // set each led individually
    }
  }
}
// Function to read joystick input and update the position of the LED
void updatePositions() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  // Store the last positions of the LED
  xLastPos = xPos;
  yLastPos = yPos;
  // Update xPos based on joystick movement (X-axis)
  if (xValue < minThreshold) { //UP

    if (xPos > start && matrix[xPos - one][yPos] == 0) {
      xPos--;
    }

    else if (xPos == start && matrix[matrixSize - one][yPos] == 0) {
      xPos = matrixSize - 1;
    }
  }

  if (xValue > maxThreshold) { //DOWN

    if (xPos < matrixSize - 1 && matrix[xPos + one][yPos] == 0) {
      xPos++;
    }

    else if (xPos == (matrixSize - 1) && matrix[start][yPos] == 0) {
      xPos = start;
    }
  }

  if (yValue > maxThreshold) { //RIGHT

    if (yPos < matrixSize - 1 && matrix[xPos][yPos + one] == 0) {
      yPos++;
    }

    else if (yPos == (matrixSize - 1) && matrix[xPos][start] == 0) {
      yPos = start;
    }
  }

  if (yValue < minThreshold) { //LEFT

    if (yPos > start && matrix[xPos][yPos - one] == 0) {
      yPos--;
    }

    else if (yPos == start && matrix[xPos][matrixSize - 1] == 0) {
      yPos = matrixSize - 1;
    }
  }
  // Check if the position has changed and update the matrix if necessary
  if (xPos != xLastPos || yPos != yLastPos) {
    matrixChanged = true;
    matrix[xLastPos][yLastPos] = 0;
    matrix[xPos][yPos] = 1;
  }
}

void clearMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      matrix[row][col] = 0;
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

void checkGameWon() {
  int counter = 0;
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (matrix[row][col] == 0) {
        counter++;
      }
    }
    Serial.println();
  }

  if (counter == (matrixSize * matrixSize)) {
    gameWon();
  }
}

void gameWon() {
  clearMatrix();
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, happyFace[row][col]);
    }
  }

  Serial.println("\nEnter s to play again.");
  while (Serial.available()) {
    // Check if the user entered 0 to stop displaying
    char key = Serial.read();  // Read the incoming byte

    // Check if the received character matches the trigger key
    if (key == 's') {
      randomSeed(analogRead(A2));
      generateWalls();  // Generate walls if 's' key is pressed (s from start)
      xPos = xInitialPos;
      yPos = yInitialPos;
    }
  }
}

void gameOver() {
  clearMatrix();
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, sadFace[row][col]);
    }
  }

  Serial.println("\nEnter s to play again.");
  while (Serial.available()) {
    // Check if the user entered 0 to stop displaying
    char key = Serial.read();  // Read the incoming byte

    // Check if the received character matches the trigger key
    if (key == 's') {
      randomSeed(analogRead(A2));
      generateWalls();  // Generate walls if 's' key is pressed (s from start)
      xPos = xInitialPos;
      yPos = yInitialPos;
      gameLost = false;
    }
  }
}
