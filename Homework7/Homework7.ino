#include "LedControl.h"  // Include LedControl library for controlling the LED matrix
#include <LiquidCrystal.h>
#include <EEPROM.h>
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int xPin = A0;
const int yPin = A1;
const int pinSW = 2;
const int brightnessPin = 3;
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;

// Create an LedControl object to manage the LED matrix
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);  // DIN, CLK, LOAD, No. DRIVER
// Variables to track the current and previous positions of the joystick-controlled LED
byte xPos = 0;
byte yPos = 0;
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
int numOfBombs = 0;
int option = 0;
int setting = 0;
int lives = 3;
int lcdBrightness = 4;
int matrixBrightness = 4;
int first = 0;
int second = 1;
int third = 2;

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
const int yInitialPos = 0;
const int messageDelay = 500;
const int twoSecondsDelay = 2000;
const int lcdBrightnessAddress = 0;
const int matrixBrightnessAddress = 1;
const int minLvl = 0;
const int maxLvl = 5;
const int startLives = 3;

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
bool gameStarted = false;
bool restartMode = false;
bool gameFinished = false;
bool winner = false;
bool sMessage = true;
bool inMainMenu = true;  //default
bool inGame = false;
bool inSettings = false;
bool inAbout = false;
bool lcdSettings = false;
bool matrixSettings = false;

int lcdBrightnessLevels[] = { 0, 51, 102, 153, 204, 255 };
int matrixBrightnessLevels[] = { 1, 3, 6, 9, 12, 15 };

String const mainMenuOptions[] = { "START", "SETTINGS", "ABOUT" };
String const settingsOptions[] = { "LCD BRIGHTNESS", "MATRIX LIGHT", "EXIT" };

const int firstPos = 0;
const int lastMainPos = sizeof(mainMenuOptions) / sizeof(String) - 1;
const int lastSettingsPos = sizeof(settingsOptions) / sizeof(String) - 1;

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
  { 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte sadFace[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte matrixBomb[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 1, 0, 0, 1, 1, 1, 0 },
  { 1, 1, 0, 1, 0, 0, 0, 1 },
  { 0, 0, 1, 1, 0, 0, 0, 1 },
  { 0, 0, 0, 1, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 1, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte matrixBombExplode[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 1, 1, 0, 1, 1, 0, 1 },
  { 0, 0, 1, 1, 0, 1, 1, 0 },
  { 0, 0, 0, 1, 0, 1, 0, 0 },
  { 0, 0, 1, 0, 1, 0, 1, 1 },
  { 0, 1, 1, 0, 1, 0, 1, 0 },
  { 0, 0, 0, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 1, 0 }
};

byte matrixBulb[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 1, 0, 0 },
  { 1, 0, 1, 1, 0, 0, 1, 1 },
  { 1, 0, 1, 1, 0, 0, 1, 1 },
  { 1, 0, 0, 0, 0, 1, 0, 0 },
  { 0, 1, 1, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte matSettings[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 1, 1 },
  { 0, 0, 0, 0, 0, 1, 1, 1 },
  { 0, 0, 0, 0, 1, 1, 1, 0 },
  { 1, 1, 1, 1, 1, 1, 0, 0 },
  { 1, 1, 0, 1, 1, 0, 0, 0 },
  { 1, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 1, 1, 1, 0, 0, 0 }
};

byte matrixAbout[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 1, 1, 0, 1 },
  { 1, 0, 0, 1, 0, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte full[8] = {
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b00000
};

byte empty[8] = {
  0b00000,
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b00000
};

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

void setup() {
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false);  // turn off power saving, enables display
  lc.clearDisplay(0);     // Clear the matrix display

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  //lcdBrightness = EEPROM.read(lcdBrightnessAddress);
  // matrixBrightness = EEPROM.read(matrixBrightnessAddress);

  analogWrite(brightnessPin, lcdBrightnessLevels[lcdBrightness]);
  lc.setIntensity(0, matrixBrightnessLevels[matrixBrightness]);

  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {

  if (inMainMenu == true && sMessage == false) {
    menu();
  }

  if (inSettings == true) {
    settings();
  }

  if (inMainMenu == true && sMessage == true) {
    startMessage();
    lcd.clear();
    sMessage = false;
  }

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

        if (inMainMenu == true) {
          mainMenu();
        }

        else if (inSettings == true) {
          settingsMenu();
        }

        else if (lcdSettings == true) {
          settings();
          lcdSettings = false;
        }

        else if (matrixSettings == true) {
          settings();
          matrixSettings = false;
        }

        else if (gameStarted == true) {

          if (numOfBombs < 1) {
            placeBomb(xPos, yPos);  // Place the bomb at the current joystick position
          }
        }
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

  if (gameLost == false && gameStarted == true) {
    checkGameWon();
  }

  if (winner == true) {
    gameStarted = false;
    if (gameFinished == true) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You won!");
      delay(twoSecondsDelay);
      lcd.setCursor(0, 0);
      lcd.print("'x' to return");
      lcd.setCursor(0, 1);
      lcd.print("to main menu.");
      clearMatrix();
      gameFinished = false;
    }

    gameWon();
  }

  if (gameLost == true) {
    gameStarted = false;
    if (restartMode == true) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Game over!");
      delay(twoSecondsDelay);
      lcd.setCursor(0, 0);
      lcd.print(F("'x' to return"));
      lcd.setCursor(0, 1);
      lcd.print(F("to main menu."));
      clearMatrix();
      restartMode = false;
    }

    gameOver();
  }

  if (gameStarted == true) {
    
    blinkForPlayer();
  }
}

void menu() {
  inMainMenu = true;
  
  menuAnimation();
  
  lcd.setCursor(1, 0);  //title
  lcd.print(F("MAIN MENU:"));

  lcd.setCursor(1, 1);  //current option
  lcd.print(">");
  lcd.print(mainMenuOptions[option]);
}

void mainMenu() {
  //enter in current selected option
  if (option == first)  // start
  {
    //start the game
    inMainMenu = false;
    gameStarted = true;
    inSettings = false;
    inAbout = false;
    gameLost = false;
    winner = false;
    lives = startLives;
    // lcd.createChar(1, heart);  // heart for lives
    clearMatrix();

    lcd.clear();
    lcd.setCursor(0, 0);  //title
    lcd.print("Lives:");
    for (int i = 0; i< lives; i++){
      lcd.write(1);
    }

    randomSeed(analogRead(A2));
    generateWalls();
    xPos = xInitialPos;
    yPos = yInitialPos;
    matrixChanged == true;

  } else if (option == second)  // settings
  {                        //enter settings
    inMainMenu = false;
    gameStarted = false;
    inSettings = true;
    inAbout = false;
    gameLost = false;
    winner = false;
    lcd.clear();
    clearMatrix();
  } else if (option == third)  // about
  {                        //enter about
    inMainMenu = false;
    gameStarted = false;
    inSettings = false;
    inAbout = true;
    gameLost = false;
    winner = false;
    about();
  }
}

void settings() {
  inSettings = true;
  settingsAnimation();
  lcd.setCursor(1, 0);  //title
  lcd.print("SETTINGS:");

  lcd.setCursor(1, 1);  //current option
  lcd.print(">");
  lcd.print(settingsOptions[setting]);
}

void settingsMenu() {
  //enter in current selected option
  if (setting == first)  // lcd brightness
  {
    //enter lcd brightness settings
    inMainMenu = false;
    gameStarted = false;
    inSettings = false;
    inAbout = false;
    gameLost = false;
    winner = false;
    lcdSettings = true;
    matrixSettings = false;
    lcdBrightnessFunc();
    
  }

  else if (setting == second)  // matrix light
  {                       //enter matrix lightening settings
    clearMatrix();
    inMainMenu = false;
    gameStarted = false;
    inSettings = false;
    inAbout = false;
    gameLost = false;
    winner = false;
    lcdSettings = false;
    matrixSettings = true;
    matrixBrightnessFunc();

  } else if (setting == third)  // exit
  {                         //go back to main menu
    inMainMenu = true;
    gameStarted = false;
    inSettings = false;
    inAbout = false;
    gameLost = false;
    winner = false;
    clearMatrix();
  }
}

void lcdBrightnessFunc() {
  int level = lcdBrightness;
  lcd.clear();
  lcd.setCursor(0, 0);  //title
  lcd.print("LCD BRIGHTNESS");

  lcd.createChar(2, empty);  // empty
  lcd.createChar(3, full);   //full

  lcd.setCursor(4, 1);
  lcd.print("-");
  for (int lvl = 0; lvl < 6; lvl++) {
    if (level >= 0) {
      lcd.write(3);
    } else {
      lcd.write(2);
    }
    level--;
  }
  lcd.print("+");

    analogWrite(brightnessPin, lcdBrightnessLevels[lcdBrightness]);  // make change on lcd

    //update in EEPROM

    EEPROM.update(lcdBrightnessAddress, lcdBrightness);
    lcdBrightness = EEPROM.read(lcdBrightnessAddress);
}

void matrixBrightnessFunc() {
  int level = matrixBrightness;
  brightnessAnimation();
  lcd.clear();
  lcd.setCursor(0, 0);  //title
  lcd.print("MATRIX LIGHT");

  lcd.createChar(4, empty);  // empty
  lcd.createChar(5, full);   //full

  lcd.setCursor(4, 1);
  lcd.print("-");
  for (int lvl = 0; lvl < 6; lvl++) {
    if (level >= 0) {
      lcd.write(5);
    } else {
      lcd.write(4);
    }
    level--;
  }
  lcd.print("+");

  lc.setIntensity(0, matrixBrightnessLevels[matrixBrightness]);

  //update in EEPROM

  EEPROM.update(matrixBrightnessAddress, matrixBrightness);
  matrixBrightness = EEPROM.read(lcdBrightnessAddress);
}

void about() {
  clearMatrix();
  aboutAnimation();
  int scrollDelay = 350;
  int characters = 25;
  int oneSecondDelay = 1000;

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Title: BombMania!");  //title
  lcd.setCursor(1, 1);
  lcd.print("By Lemnaru Amedeea, Git: lemnaruamedeea");

  for (int i = 0; i < characters; i++) {  // autoscroll to left

    lcd.scrollDisplayLeft();

    delay(scrollDelay);
  }

  delay(oneSecondDelay);
  inMainMenu = true;
  inAbout = false;
}

void startMessage() {
  menuAnimation();
  int characters = 21;
  int fourSecondsDelay = 4000;
  String welcomeMessage1 = "BombMania!";
  String welcomeMessage2 = "Let's bomb this!";
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(welcomeMessage1);
  lcd.setCursor(0, 1);
  lcd.print(welcomeMessage2);

  delay(fourSecondsDelay);
}

void placeBomb(int x, int y) {
  // Place the bomb at the specified position
  numOfBombs = 1;
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
          // Player loses a live if the bomb catches him

          lives--;
          lcd.clear();
    lcd.createChar(1, heart);
    lcd.setCursor(0, 0);  //title
    lcd.print("Lives:");
    for (int i = 0; i < lives; i++) {
      lcd.write(1);
    }
          // Player loses the game if he has no more lives
          if (lives == 0) {
            gameLost = true;
          }
          // gameOver();
          exploded = false;
          restartMode = true;
        }
      }
      numOfBombs = 0;
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
    randomRow = random(matrixSize);     // Generate random row index
    randomCol = random(2, matrixSize);  // Generate random column index
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
  if (xValue < minThreshold) {

    if (lcdSettings == true) {  //move the level bar to left

      if (lcdBrightness > minLvl) {
        lcdBrightness--;
        lcdBrightnessFunc();
      }
    }

    else if (matrixSettings == true) {  //move the level bar to left

      if (matrixBrightness > minLvl) {
        matrixBrightness--;
        Serial.println(matrixBrightness);
        matrixBrightnessFunc();
      }
    }

    else if (gameStarted == true) {  //UP

      if (xPos > start && matrix[xPos - one][yPos] == 0) {
        xPos--;
      }

      else if (xPos == start && matrix[matrixSize - one][yPos] == 0) {
        xPos = matrixSize - 1;
      }
    }
  }

  if (xValue > maxThreshold) {

    if (lcdSettings == true) {  //move the level bar to right

      if (lcdBrightness < maxLvl) {
        lcdBrightness++;
        lcdBrightnessFunc();
      }
    }

    else if (matrixSettings == true) {  //move the level bar to right

      if (matrixBrightness < maxLvl) {
        matrixBrightness++;
        Serial.println(matrixBrightness);
        matrixBrightnessFunc();
      }
    }

    else if (gameStarted == true) {  //DOWN

      if (xPos < matrixSize - 1 && matrix[xPos + one][yPos] == 0) {
        xPos++;
      }

      else if (xPos == (matrixSize - 1) && matrix[start][yPos] == 0) {
        xPos = start;
      }
    }
  }

  if (yValue > maxThreshold) {

    if (inMainMenu == true) {  //in main menu

      if (option < lastMainPos) {  //UP
        option++;
        lcd.clear();
        menu();
      }

    }

    else if (inSettings == true) {  //in settings

      if (setting < lastSettingsPos) {  // UP
        setting++;
        lcd.clear();
        settings();
      }

    }

    else if (gameStarted == true) {  //RIGHT

      if (yPos < matrixSize - 1 && matrix[xPos][yPos + one] == 0) {
        yPos++;
      }

      else if (yPos == (matrixSize - 1) && matrix[xPos][start] == 0) {
        yPos = start;
      }
    }
  }

  if (yValue < minThreshold) {

    if (inMainMenu == true) {  //in main menu

      if (option > firstPos) {  //DOWN
        option--;
        lcd.clear();
        menu();
      }

    }

    else if (inSettings == true) {  //in setting

      if (setting > firstPos) {  //DOWN
        setting--;
        lcd.clear();
        settings();
      }

    }

    else if (gameStarted == true) {  // LEFT

      if (yPos > start && matrix[xPos][yPos - one] == 0) {
        yPos--;
      }

      else if (yPos == start && matrix[xPos][matrixSize - 1] == 0) {
        yPos = matrixSize - 1;
      }
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
    gameFinished = true;
    winner = true;
  }
}

void gameWon() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, happyFace[row][col]);
    }
  }

  if (Serial.available()) {
    char key = Serial.read();  // Read the incoming byte
    // Check if the received character matches the trigger key
    if (key == 'x') {
      clearMatrix();
      lives = startLives;
      inMainMenu = true;
      winner = false;
    }
  }
  matrixChanged = true;
}

void gameOver() {

  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, sadFace[row][col]);
    }
  }

  if (Serial.available()) {
    char key = Serial.read();  // Read the incoming byte
    // Check if the received character matches the trigger key
    if (key == 'x') {
      clearMatrix();
      lives = startLives;
      inMainMenu = true;
      gameLost = false;
    }
  }
}

void menuAnimation() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixBomb[row][col]);
    }
  }

}

void aboutAnimation() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixAbout[row][col]);
    }
  }
}

void settingsAnimation() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matSettings[row][col]);
    }
  }
}

void brightnessAnimation() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixBulb[row][col]);
    }
  }
}