volatile bool buttonSPPressed = false;
volatile bool buttonResetPressed = false;
volatile bool buttonLapPressed = false;

volatile unsigned long lastInterruptTime1 = 0;
volatile unsigned long lastInterruptTime2 = 0;

byte buttonStateReset = LOW;
byte readingReset = HIGH;
byte lastReadingReset = HIGH;

bool startMode = false;
bool pauseMode = false;
bool firstTimeResetMode = false;
bool resetMode = false;
bool lapMode = false;
bool saveLapMode = false;

const int buttonPinSP = 2;
const int buttonPinReset = 8;
const int buttonPinLap = 3;

const int latchPin = 11;
const int clockPin = 10;
const int dataPin = 12;

const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

int lap = 0;
int numberOfLaps = 4;

int displayDigits[] = { segD1, segD2, segD3, segD4 };
unsigned long timeOfTheLaps[4] = { 0, 0, 0, 0 };

const int displayCount = 4;
const int encodingsNumber = 10;

byte byteEncodings[encodingsNumber] = {
  //A B C D E F G DP
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
};

unsigned int lastDebounceTime = 0;

unsigned long lastIncrement = 0;
unsigned long delayCount = 50;
unsigned long number = 0;

unsigned int debounceDelayReset = 50;
const unsigned long debounceDelay = 200;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(buttonPinSP, INPUT_PULLUP);
  pinMode(buttonPinReset, INPUT_PULLUP);
  pinMode(buttonPinLap, INPUT_PULLUP);


  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  attachInterrupt(digitalPinToInterrupt(buttonPinSP), handleInterruptSP, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonPinLap), handleInterruptLap, FALLING);

  Serial.begin(9600);
}

void loop() {
  if (buttonSPPressed) {
    startMode = !startMode;
    buttonSPPressed = false;
    resetMode = false;
    lapMode = false;
  }

  readingReset = digitalRead(buttonPinReset);
  if (readingReset != lastReadingReset) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelayReset) {
    if (readingReset != buttonStateReset) {
      buttonStateReset = readingReset;
      if (buttonStateReset == LOW && startMode == false) {
        resetMode = true;
        lapMode = true;
      }
    }
  }
  lastReadingReset = readingReset;

  if (buttonLapPressed && startMode == true && resetMode == false) {
    saveLap();
    buttonLapPressed = false;
  }

  if (buttonLapPressed && startMode == false && lapMode == true) {
    showLap();
    buttonLapPressed = false;
  }

  if (startMode) {

    if (millis() - lastIncrement > delayCount) {
      number++;
      number %= 10000;
      lastIncrement = millis();
    }

    writeNumber(number);
  }

  else if (resetMode == false && startMode == false) {
    writeNumber(number);
  }

  if (resetMode) {
    number = 0;
    reset();
    
  }

}

void writeReg(int digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }

  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(unsigned long number) {
  int currentNumber = number;
  int displayDigit = 4;
  int lastDigit = 0;

  while (displayDigit != 0) {
    lastDigit = currentNumber % 10;
    displayDigit--;
    activateDisplay(displayDigit);
    
    if (displayDigit == 2) {
      byteEncodings[lastDigit] = byteEncodings[lastDigit] | B00000001;
    }
    writeReg(byteEncodings[lastDigit]);
    byteEncodings[lastDigit] = byteEncodings[lastDigit] & B11111110;
    currentNumber /= 10;
    
    writeReg(B00000000);
  }
}

void reset() {
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], LOW);
  }
  writeNumber(number);
}

void showLap() {
  if (lap >= numberOfLaps) {
    lap = 0;
  }
  
  writeReg(B00000000);
  writeNumber(timeOfTheLaps[lap]);
  Serial.println(timeOfTheLaps[lap]);
  lap++;
}

void saveLap() {
  for (int i = numberOfLaps - 1; i >= 0; i--) {
    timeOfTheLaps[i] = timeOfTheLaps[i - 1];
  }
  timeOfTheLaps[0] = number;
  Serial.println(number);
}

void handleInterruptSP() {
  static unsigned long interruptTime1 = 0;
  interruptTime1 = micros();

  if (interruptTime1 - lastInterruptTime1 > debounceDelay * 1000) {
    buttonSPPressed = true;
  }

  lastInterruptTime1 = interruptTime1;
}

void handleInterruptLap() {
  if (startMode == true && resetMode == false || startMode == false && resetMode == true) {
    static unsigned long interruptTime2 = 0;
    interruptTime2 = micros();

    if (interruptTime2 - lastInterruptTime2 > debounceDelay * 1000) {
      buttonLapPressed = true;
    }

    lastInterruptTime2 = interruptTime2;
  }
}