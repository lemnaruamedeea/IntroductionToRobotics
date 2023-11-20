#include <EEPROM.h>

const int EEPROM_SIZE = 100;
const int NUM_READINGS = 10;
const int SENSOR_DATA_SIZE = 2 * sizeof(int);

const int trigPin = 9;
const int echoPin = 10;
const int ledBPin = 3;
const int ledGPin = 5;
const int ledRPin = 6;

int photocellPin = 0;

int interval = 5;

int ultrasonicThreshold = 10;
int LDRThreshold = 300;

long duration = 0;
int distance = 0;

int ultrasonicValue;
int LDRValue;

int userChoice;
int sensorIndex = 0;

int redValue = 125;
int greenValue = 125;
int blueValue = 125;

bool automaticMode = true;
bool continueReading = true;

unsigned long currentTime;
unsigned long previousTime = 0;
unsigned long previousDataTime = 0;

const int millisecond = 1000;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(ledRPin, OUTPUT);
  pinMode(ledGPin, OUTPUT);
  pinMode(ledBPin, OUTPUT);

  pinMode(echoPin, INPUT);
  pinMode(photocellPin, INPUT);

  analogWrite(ledRPin, redValue);
  analogWrite(ledGPin, greenValue);
  analogWrite(ledBPin, blueValue);

  Serial.println(F("\nDefault values: "));
  Serial.println(F("interval = 5;"));
  Serial.println(F("ultrasonicThreshold = 10;"));
  Serial.print(F("LDRThreshold = 300."));

  Serial.begin(9600);
  printMenu();
}

void loop() {
  if (Serial.available()) {
    userChoice = Serial.parseInt();
    menu(userChoice);
  }

  currentTime = millis();

  if (currentTime - previousDataTime >= interval) {
    storeSensorData(sensorIndex);
    previousDataTime = currentTime;
  }

  ultrasonicValue = readUltrasonicSensor();
  LDRValue = readLDRSensor();
  controlLED();
}

void printMenu() {
  Serial.println(F("\nMain Menu:"));
  Serial.println(F("1. Sensor Settings."));
  Serial.println(F("2. Reset Logger Data."));
  Serial.println(F("3. System Status."));
  Serial.print(F("4. RGB LED Control."));
}

void printSubMenuSensorSettings() {
  Serial.println(F("\n1. Sensor Settings:"));
  Serial.println(F("  1.1 Sensors Sampling Interval."));
  Serial.println(F("  1.2 Ultrasonic Alert Threshold."));
  Serial.println(F("  1.3 LDR Alert Threshold."));
  Serial.print(F("  1.4 Back"));
}

void printSubMenuResetLoggerData() {
  Serial.println(F("\n2. Reset Logger Data:"));
  Serial.println(F("  Are you sure you want to continue with this procedure?"));
  Serial.println(F("    2.1 Yes."));
  Serial.print(F("    2.2 No."));
}

void printSubMenuSystemStatus() {
  Serial.println(F("\n3. System Status:"));
  Serial.println(F("  3.1 Current Sensor Readings."));
  Serial.println(F("  3.2 Current Sensor Settings."));
  Serial.println(F("  3.3 Display Logged Data."));
  Serial.print(F("  3.4 Back."));
}

void printSubMenuRGBLedControl() {
  Serial.println(F("\n4. RGB LED Control:"));
  Serial.println(F("  4.1 Manual Color Control."));
  Serial.println(F("  4.2 LED: Toggle Automatic ON/OFF."));
  Serial.print(F("  4.3 Back"));
}

void menu(int option) {
  switch (option) {
    case 1:
      sensorSettings();
      break;
    case 2:
      resetLoggerData();
      break;
    case 3:
      systemStatus();
      break;
    case 4:
      RGBLedControl();
      break;
    default:
      Serial.println(F("\nInvalid option. Please choose 1, 2, 3 or 4"));
      printMenu();
  }
}

void sensorSettings() {
  printSubMenuSensorSettings();
  while (!Serial.available()) {}
  int subOption = Serial.parseInt();
  switch (subOption) {
    case 1:
      setSamplingInterval();
      printMenu();
      break;
    case 2:
      setUltrasonicThreshold();
      printMenu();
      break;
    case 3:
      setLDRThreshold();
      printMenu();
      break;
    case 4:
      printMenu();
      break;
    default:
      Serial.println(F("\nInvalid option. Please choose 1, 2, 3 or 4"));
      sensorSettings();
  }
}

void setSamplingInterval() {
  Serial.println("\nEnter sampling interval (seconds) between 1 and 10:");
  while (!Serial.available()) {}
  interval = Serial.parseInt();

  if (interval >= 1 && interval <= 10) {
    Serial.print(F("Sampling interval set to "));
    Serial.print(interval);
    Serial.println(F(" seconds."));
  }
  
  else {
    Serial.println(F("Invalid interval. Please choose between 1 and 10 seconds."));
    setSamplingInterval();
  }
}

void setUltrasonicThreshold() {
  Serial.println(F("\nSet Ultrasonic Alert Threshold:"));
  Serial.println(F("Enter min value for the Ultrasonic sensor (when an object is too close): "));
  while (!Serial.available()) {}
  int threshold = Serial.parseInt();

  if (threshold >= 0 && threshold <= 15) {
    ultrasonicThreshold = threshold;
    Serial.print(F("Ultrasonic Alert Threshold set to "));
    Serial.println(ultrasonicThreshold);
  }
  
  else {
    Serial.println(F("Invalid threshold value. Enter a value between 0 and 15."));
    setUltrasonicThreshold();
  }
}

void setLDRThreshold() {
  Serial.println(F("\nSet LDR Alert Threshold:"));
  Serial.println(F("Enter min value for the LDR sensor (when is going dark): "));
  while (!Serial.available()) {}
  int threshold = Serial.parseInt();

  if (threshold >= 0 && threshold <= 300) {
    LDRThreshold = threshold;
    Serial.print(F("LDR Alert Threshold set to "));
    Serial.println(LDRThreshold);
  }
  
  else {
    Serial.println(F("Invalid threshold value. Enter a value between 0 and 300."));
    setLDRThreshold();
  }
}

void resetLoggerData() {
  printSubMenuResetLoggerData();
  while (!Serial.available()) {}
  int subOption = Serial.parseInt();
  switch (subOption) {
    case 1:
      for (int i = 0; i < EEPROM_SIZE; ++i) {
        EEPROM.update(i, 0);
      }
      interval = 5;
      ultrasonicThreshold = 10;
      LDRThreshold = 300;
      Serial.println(F("\nLogger data reseted."));
      printMenu();
      break;
    case 2:
      printMenu();
      break;
    default:
      Serial.println(F("\nInvalid option. Please choose 1 or 2"));
      resetLoggerData();
  }
}

void systemStatus() {
  printSubMenuSystemStatus();
  while (!Serial.available()) {}
  int subOption = Serial.parseInt();
  switch (subOption) {
    case 1:
      currentSensorReadings();
      printMenu();
      break;
    case 2:
      currentSensorSettings();
      printMenu();
      break;
    case 3:
      displayLastSensorData(sensorIndex);
      printMenu();
      break;
    case 4:
      printMenu();
      break;
    default:
      Serial.println(F("\nInvalid option. Please choose 1, 2, 3 or 4"));
      systemStatus();
  }
}

void currentSensorReadings() {
  Serial.println(F("\nPress 'x' to stop readings."));
  Serial.println(F("\nCurrent Sensor Readings:"));
  continueReading = true;
  while (continueReading) {
    currentTime = millis();

    if (currentTime - previousTime >= interval * millisecond) {
      previousTime = currentTime;

      Serial.print(F("Ultrasonic Distance: "));
      Serial.print(ultrasonicValue);
      Serial.print(F(" | "));

      Serial.print(F("LDR Sensor Value: "));
      Serial.println(LDRValue);

      if (Serial.available()) {
        char input = Serial.read();

        if (input == 'x') {
          Serial.println(F("Reading stopped."));
          continueReading = false;
        }
      }
    }
  }
}

void currentSensorSettings() {
  Serial.println(F("Press ' ' to stop readings."));

  Serial.print(F("The  sampling  rate: "));
  Serial.println(interval);

  Serial.print(F("Threshold value for Ultrasonic: "));
  Serial.println(ultrasonicThreshold);

  Serial.print(F("Threshold value for LDR: "));
  Serial.println(LDRThreshold);

  continueReading = true;
  while (continueReading) {

    if (Serial.available()) {
      char input = Serial.read();

      if (input == ' ') {
        Serial.println(F("\nBack to main menu."));
        continueReading = false;
      }
    }
  }
}

int readUltrasonicSensor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

int readLDRSensor() {
  return analogRead(photocellPin);
}

void storeSensorData(int sensorIndex) {
  int address = sensorIndex * EEPROM_SIZE;
  int sensorDataAddress = address + (NUM_READINGS - 1) * SENSOR_DATA_SIZE;

  for (int i = 0; i < NUM_READINGS - 1; ++i) {
    int currentAddress = address + i * SENSOR_DATA_SIZE;
    int nextAddress = address + (i + 1) * SENSOR_DATA_SIZE;
    int ultrasonicValue = EEPROM.read(nextAddress);
    int LDRValue = EEPROM.read(nextAddress + sizeof(int));

    EEPROM.update(currentAddress, ultrasonicValue);
    EEPROM.update(currentAddress + sizeof(int), LDRValue);
  }

  int ultrasonicReading = readUltrasonicSensor();
  int LDRReading = readLDRSensor();

  EEPROM.update(sensorDataAddress, ultrasonicReading);
  EEPROM.update(sensorDataAddress + sizeof(int), LDRReading);
}

void displayLastSensorData(int sensorIndex) {
  Serial.println(F("\nPress 'z' to exit."));
  Serial.println(F("\nDisplaying Last 10 Sensor Readings:"));

  int address = sensorIndex * EEPROM_SIZE;

  for (int i = 0; i < NUM_READINGS; ++i) {
    int sensorDataAddress = address + i * SENSOR_DATA_SIZE;

    int ultrasonicReading = EEPROM.read(sensorDataAddress);
    int LDRReading = EEPROM.read(sensorDataAddress + sizeof(int));

    // Display the sensor readings
    Serial.print(F("Reading "));
    Serial.print(i + 1);
    Serial.print(F(": Ultrasonic - "));
    Serial.println(ultrasonicReading);
    Serial.print(F(" | "));
    Serial.print(F("LDR - "));
    Serial.println(LDRReading);
    }
  continueReading = true;
  while (continueReading) {

    if (Serial.available()) {
      char input = Serial.read();

      if (input == 'z') {
        Serial.println(F("\nBack to main menu."));
        continueReading = false;
      }
    }
  }
}

void RGBLedControl() {
  printSubMenuRGBLedControl();

  while (!Serial.available()) {}
  int subOption = Serial.parseInt();
  switch (subOption) {
    case 1:
      manualColorControl();
      printMenu();
      break;
    case 2:
      toggleAutomaticMode();
      printMenu();
      break;
    case 3:
      printMenu();
      break;
    default:
      Serial.println(F("\nInvalid option. Please choose 1, 2 or 3"));
      RGBLedControl();
  }
}

void manualColorControl() {
  Serial.println(F("\nManual Color Control:"));
  Serial.println(F("Enter RGB values (0-255) for the LED."));
  Serial.print(F("\nRed (0-255): "));

  while (!Serial.available()) {}
  redValue = Serial.parseInt();

  if (redValue >= 0 && redValue <= 255) {
    Serial.println(redValue);
  }

  else {
    Serial.println(F("Invalid value. Please choose between 0 and 255."));
    manualColorControl();
  }

  Serial.print(F("Green (0-255): "));

  while (!Serial.available()) {}
  greenValue = Serial.parseInt();

  if (greenValue >= 0 && greenValue <= 255) {
    Serial.println(greenValue);
  }

  else {
    Serial.println(F("Invalid value. Please choose between 0 and 255."));
    manualColorControl();
  }

  Serial.print("Blue (0-255): ");

  while (!Serial.available()) {}
  blueValue = Serial.parseInt();
  if (blueValue >= 0 && blueValue <= 255) {
    Serial.println(blueValue);
  }

  else {
    Serial.println(F("Invalid value. Please choose between 0 and 255."));
    manualColorControl();
  }

  analogWrite(ledRPin, redValue);
  analogWrite(ledGPin, greenValue);
  analogWrite(ledBPin, blueValue);

  Serial.println("LED color updated.");
}

void toggleAutomaticMode() {
  Serial.println(F("\nToggle Automatic Mode:"));
  Serial.println(F("1. Turn Automatic Mode ON"));
  Serial.println(F("2. Turn Automatic Mode OFF"));

  while (!Serial.available()) {}
  int subOption = Serial.parseInt();

  switch (subOption) {
    case 1:
      automaticMode = true;
      controlLED();
      Serial.println(F("Automatic Mode ON"));
      break;
    case 2:
      automaticMode = false;
      controlLED();
      Serial.println(F("Automatic Mode OFF"));
      break;
    default:
      Serial.println(F("Invalid option. Please choose 1 or 2."));
      toggleAutomaticMode();
  }
}

void controlLED() {

  if (automaticMode == true) {

    if (ultrasonicValue < ultrasonicThreshold || LDRValue < LDRThreshold) {
      digitalWrite(ledRPin, HIGH);
      digitalWrite(ledGPin, LOW);
      digitalWrite(ledBPin, LOW);
    }

    else {
      digitalWrite(ledRPin, LOW);
      digitalWrite(ledGPin, HIGH);
      digitalWrite(ledBPin, LOW);
    }
  }

  else {
    analogWrite(ledRPin, redValue);
    analogWrite(ledGPin, greenValue);
    analogWrite(ledBPin, blueValue);
  }
}