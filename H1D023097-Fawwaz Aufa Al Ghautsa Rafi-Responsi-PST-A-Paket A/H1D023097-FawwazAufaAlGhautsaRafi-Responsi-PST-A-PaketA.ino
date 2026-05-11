
const int greenLED = 11;
const int redLED = 12;
const int buttonPin = 3;

const int segmentPins[] = {4, 5, 6, 7, 8, 9, 10};

const byte digits[10][8] = {
  {1,1,1,1,1,1,0,0},
  {0,1,1,0,0,0,0,0},
  {1,1,0,1,1,0,1,0},
  {1,1,1,1,0,0,1,0},
  {0,1,1,0,0,1,1,0},
  {1,0,1,1,0,1,1,0},
  {1,0,1,1,1,1,1,0},
  {1,1,1,0,0,0,0,0},
  {1,1,1,1,1,1,1,0},
  {1,1,1,1,0,1,1,0}
};

int resetDigit = 0;
void displayDigit(int num){
  for(int i=0;i<8;i++){
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); 

  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  resetSystem();
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    startCycle();
  }
}

void startCycle() {
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, HIGH);

  for (int i = 9; i >= 0; i--) {
    displayDigit(i);
    delay(1000);
  }

  displayDigit(resetDigit);
  digitalWrite(redLED, LOW);
  delay(1000);
  resetSystem();
}

void displayDigit(int num) {
  for (int i = 0; i < 7; i++) {
    bool bitVal = bitRead(digits[num], i);
    digitalWrite(segmentPins[i], bitVal);
  }
}


void resetSystem() {
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
  displayDigit(resetDigit);
}