const int pinSW = 2;  // digital pin connected to switch output
const int pinX = A0;
const int pinY = A1;

const int pinSegA = 12;
const int pinSegB = 10;
const int pinSegC = 9;
const int pinSegD = 8;
const int pinSegE = 7;
const int pinSegF = 6;
const int pinSegG = 5;
const int pinSegDP = 4;

bool stateSegA = LOW;
bool stateSegB = LOW;
bool stateSegC = LOW;
bool stateSegD = LOW;
bool stateSegE = LOW;
bool stateSegF = LOW;
bool stateSegG = LOW;
bool stateSegDP = LOW;

const int segSize = 8;
const int fourDirections = 4;
const int up = 0;
const int down = 1;
const int left = 2;
const int right = 3;

const long longPress = 2000;
const long shortPress = 500;
const long blinkInterval = 300;

byte state = HIGH;
byte dpState = LOW;
byte ledState = LOW;

int index = 0;
int xValue = 0;
int yValue = 0;
int minThreshold = 400;
int maxThreshold = 600;
int segment = 7;

unsigned long currentTime;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
unsigned long blinkTime;
unsigned long lastDebounceTime = 0;
unsigned int debounceDelay = 50;

volatile bool possiblePress = false;
volatile bool shouldStartCounter = false;

bool commonAnode = false;
bool joyMoved = false;
bool lastButtonReading = HIGH;
bool buttonReading = HIGH;
bool pressed = false;

int segments[segSize] = {
  pinSegA, pinSegB, pinSegC, pinSegD, pinSegE, pinSegF, pinSegG, pinSegDP
};

byte segState[segSize] = {
  stateSegA, stateSegB, stateSegC, stateSegD, stateSegE, stateSegF, stateSegG, stateSegDP
};

byte directions[segSize][fourDirections] = {
  // up down left right
  { 0, 6, 5, 1 },  // a-0
  { 0, 6, 5, 1 },  // b-1
  { 6, 3, 4, 7 },  // c-2
  { 6, 3, 4, 2 },  // d-3
  { 6, 3, 4, 2 },  // e-4
  { 0, 6, 5, 1 },  // f-5
  { 0, 3, 6, 6 },  // g-6
  { 7, 7, 2, 7 }   // dp-7
};

void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  attachInterrupt(digitalPinToInterrupt(pinSW), handleInterrupt, RISING);
  Serial.begin(9600);
  if (commonAnode == true) {
    state = !state;
  }
}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  if (xValue < minThreshold && joyMoved == false) {
    if (directions[segment][down] != segment) {
      segment = directions[segment][down];
    }
    joyMoved = true;
  }

  if (xValue > maxThreshold && joyMoved == false) {
    if (directions[segment][up] != segment) {
      segment = directions[segment][up];
    }
    joyMoved = true;
  }

  if (yValue < minThreshold && joyMoved == false) {
    if (directions[segment][left] != segment) {
      segment = directions[segment][left];
    }
    joyMoved = true;
  }

  if (yValue > maxThreshold && joyMoved == false) {
    if (directions[segment][right] != segment) {
      segment = directions[segment][right];
    }
    joyMoved = true;
  }

  if (xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }

  if (possiblePress) {

    if (shouldStartCounter) {
      lastDebounceTime = millis();
      shouldStartCounter = false;
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
      buttonReading = digitalRead(pinSW);

      if (buttonReading == LOW) {
        pressedTime = millis();
      }
      // if (pressed == false) {
      //   pressedTime = millis();
      //   pressed = true;
      // }

      if (buttonReading == HIGH) {
        releasedTime = millis();

        if (releasedTime - pressedTime >= longPress) {
          for (int i = 0; i < segSize; i++) {
            segState[i] = LOW;
          }
          segment = 7;

        }
        else {
          segState[segment] = !segState[segment];
        }
      }
      possiblePress = false;
    }
  }

  currentTime = millis();

  if (currentTime - blinkTime >= blinkInterval) {
    ledState = !ledState;
    blinkTime = currentTime;
    digitalWrite(segments[segment], ledState);
  }

  displaySeg(segment);
}

void displaySeg(byte segment) {
  for (int i = 0; i < segSize; i++) {
    if (i != segment)
      digitalWrite(segments[i], segState[i]);
  }
}

void handleInterrupt() {
  possiblePress = true;       // Indicate that a button press might have occurred
  shouldStartCounter = true;  // Indicate the need to start the debounce counter
}
