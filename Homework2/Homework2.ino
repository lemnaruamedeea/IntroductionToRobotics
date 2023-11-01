const int buttonPinLevel1 = 2;
const int buttonPinLevel2 = 4;
const int buttonPinLevel3 = 5;

const int ledPinLevel1 = 9;
const int ledPinLevel2 = 10;
const int ledPinLevel3 = 12;
const int ledPinElevator = 13;

const int buzzerPin = 6;
const int buzzerToneOpen = 700;
const int buzzerToneMoving = 800;
const int buzzerToneClose = 900;

byte buttonStateLevel1 = LOW;
byte buttonStateLevel2 = LOW;
byte buttonStateLevel3 = LOW;

byte ledStateLevel1 = HIGH;
byte ledStateLevel2 = HIGH;
byte ledStateLevel3 = HIGH;
byte ledStateElevator = HIGH;

byte readingLevel1 = HIGH;
byte readingLevel2 = HIGH;
byte readingLevel3 = HIGH;

byte lastReadingLevel1 = HIGH;
byte lastReadingLevel2 = HIGH;
byte lastReadingLevel3 = HIGH;

unsigned int lastDebounceTime1 = 0;
unsigned int lastDebounceTime2 = 0;
unsigned int lastDebounceTime3 = 0;
unsigned int debounceDelay = 50;

unsigned long currentTime;
unsigned long startTime;
unsigned long movementTime;
unsigned long blinkTime;

const long blinkInterval = 400;
const long soundLength = 500;
const long midLedIntervalStart = 3000;
const long midLedIntervalStop = 4000;

bool movingElevator = false;
bool midLed = false;

int thisFloor = 1;

void setup() {
  pinMode(buttonPinLevel1, INPUT_PULLUP);
  pinMode(buttonPinLevel2, INPUT_PULLUP);
  pinMode(buttonPinLevel3, INPUT_PULLUP);

  pinMode(ledPinLevel1, OUTPUT);
  pinMode(ledPinLevel2, OUTPUT);
  pinMode(ledPinLevel3, OUTPUT);
  pinMode(ledPinElevator, OUTPUT);
  Serial.begin(9600);
}


void loop() {
  currentTime = millis();

  if (!movingElevator) {
    readingLevel1 = digitalRead(buttonPinLevel1);
    readingLevel2 = digitalRead(buttonPinLevel2);
    readingLevel3 = digitalRead(buttonPinLevel3);

    if (readingLevel1 == LOW && thisFloor != 1 && (currentTime-startTime > debounceDelay)){

      if (readingLevel1 != lastReadingLevel1) {
        lastDebounceTime1 = millis();
      }

      if ((millis() - lastDebounceTime1) > debounceDelay) {

        if (readingLevel1 != buttonStateLevel1) {
          buttonStateLevel1 = readingLevel1;

          if (buttonStateLevel1 == LOW) {
            ledStateLevel1 = !ledStateLevel1;
          }
        }
      }

      lastReadingLevel1 = readingLevel1;
      movingElevator = true;

      if (thisFloor == 2){
        movementTime = 3000;
      }

      else if (thisFloor == 3){
        movementTime = 7000;
        midLed = true;
      }

      thisFloor = 1;
      startTime = currentTime;
    }

    if (readingLevel2 == LOW && thisFloor != 2 && (currentTime-startTime > debounceDelay)){

      if (readingLevel2 != lastReadingLevel2) {
        lastDebounceTime2 = millis();
      }

      if ((millis() - lastDebounceTime2) > debounceDelay) {

        if (readingLevel2 != buttonStateLevel2) {
          buttonStateLevel2 = readingLevel2;

          if (buttonStateLevel2 == LOW) {
            ledStateLevel2 = !ledStateLevel2;
          }
        }
      }

      lastReadingLevel2 = readingLevel2;
      movingElevator = true;
      movementTime = 3000;
      thisFloor = 2;
      startTime = currentTime;
    }

    if (readingLevel3 == LOW && thisFloor != 3 && (currentTime-startTime > debounceDelay)){

      if (readingLevel3 != lastReadingLevel3) {
        lastDebounceTime3 = millis();
      }

      if ((millis() - lastDebounceTime3) > debounceDelay) {

        if (readingLevel3 != buttonStateLevel3) {
          buttonStateLevel3 = readingLevel3;

          if (buttonStateLevel3 == LOW) {
            ledStateLevel3 = !ledStateLevel3;
          }
        }
      }

      lastReadingLevel3 = readingLevel3;
      movingElevator = true;
      
      if (thisFloor == 2){
        movementTime = 3000;
      }

      else if (thisFloor == 1){
        movementTime = 7000;
        midLed = true;
      }

      thisFloor = 3;
      startTime = currentTime;
    }
  }

  if (movingElevator) {

    if (currentTime - startTime <= soundLength) {
      tone(buzzerPin, buzzerToneClose, soundLength);
    }

    else if (currentTime - startTime >= movementTime) {
      digitalWrite(ledPinElevator, HIGH);
      tone(buzzerPin, buzzerToneOpen, 500);
      movingElevator = false;
    }

    else{
      tone(buzzerPin, buzzerToneMoving);
    }

    if (currentTime - blinkTime >= blinkInterval) {
      ledStateElevator = !ledStateElevator;
      blinkTime = currentTime;
      digitalWrite(ledPinElevator, ledStateElevator);
    }

    if (midLed) {

      if (currentTime - startTime <= midLedIntervalStart || currentTime - startTime >= midLedIntervalStop) {
          ledStateLevel2 = LOW;
      }

      else {
        ledStateLevel2 = HIGH;
      }

      digitalWrite(ledPinLevel2, ledStateLevel2);
    }
    
    digitalWrite(ledPinLevel1, LOW);
    digitalWrite(ledPinLevel3, LOW);
  }

  else {
    if (thisFloor == 1) { 
      digitalWrite(ledPinLevel1, HIGH);
    }
    
    if (thisFloor == 2) {
      digitalWrite(ledPinLevel2, HIGH);
    }

    if (thisFloor == 3) {
      digitalWrite(ledPinLevel3, HIGH);
    }
  }
}
