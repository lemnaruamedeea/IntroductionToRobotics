const int inPin[] = {A0, A1, A2};

const int outPin[] = {9, 10, 11};

int inValue[3];
int outValue[3];

int i;
const int nrPin = 3;

void setup() {
  for (i=0; i<nrPin; i++){
    pinMode(inPin[i], INPUT);
    pinMode(outPin[i], OUTPUT);
  }
}

void loop() {
  for (i=0; i<nrPin; i++){
    inValue[i] = analogRead(inPin[i]);
    outValue[i] = map(inValue[i], 0, 1023, 0, 255);
    analogWrite(outPin[i], outValue[i]);
  }
}
