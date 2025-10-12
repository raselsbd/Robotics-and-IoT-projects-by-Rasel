#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool oledConnected = false;

// Button pins
const int upButton = 4;
const int downButton = 3;
const int okButton = 2;

// LED pin
const int ledPin = 13;

// Debounce
bool lastUpState = HIGH, lastDownState = HIGH, lastOkState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int currentQuestion = 0;
int currentOption = 0;
bool waitingForInput = true;

// Questions and options
const char* questions[4] = {
  "1. Your favorite fruit?",
  "2. Your hobby?",
  "3. Choose a pet?",
  "4. Best season?"
};

const char* options[4][4] = {
  {"Apple", "Banana", "Mango", "Orange"},
  {"Reading", "Gaming", "Music", "Travel"},
  {"Dog", "Cat", "Bird", "Fish"},
  {"Summer", "Winter", "Rainy", "Spring"}
};

int answers[4] = {-1, -1, -1, -1}; // Store selected answers

void setup() {
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(okButton, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  Serial.begin(9600);

  // Try to initialize OLED
  if(display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    oledConnected = true;
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Survey Starting...");
    display.display();
    delay(1000);
  } else {
    oledConnected = false;
    Serial.println("OLED not found! Running in Serial mode.");
  }

  showQuestion();
}

void loop() {
  bool upState = digitalRead(upButton);
  bool downState = digitalRead(downButton);
  bool okState = digitalRead(okButton);

  bool inputReceived = false;

  if (upState == LOW && lastUpState == HIGH) {
    currentOption = (currentOption - 1 + 4) % 4;
    inputReceived = true;
    delay(debounceDelay);
  }
  if (downState == LOW && lastDownState == HIGH) {
    currentOption = (currentOption + 1) % 4;
    inputReceived = true;
    delay(debounceDelay);
  }
  if (okState == LOW && lastOkState == HIGH) {
    answers[currentQuestion] = currentOption;
    currentQuestion++;
    if (currentQuestion < 4) {
      currentOption = 0;
      inputReceived = true;
    } else {
      // Survey completed
      digitalWrite(ledPin, LOW);
      showThankYou();
      while(1); // Stop here
    }
    delay(debounceDelay);
  }

  if (inputReceived) {
    digitalWrite(ledPin, HIGH);
    showQuestion();
  } else {
    digitalWrite(ledPin, LOW);
  }

  lastUpState = upState;
  lastDownState = downState;
  lastOkState = okState;
}

void showQuestion() {
  if (oledConnected) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(questions[currentQuestion]);
    for(int i=0; i<4; i++) {
      if(i == currentOption)
        display.print("> ");
      else
        display.print("  ");
      display.println(options[currentQuestion][i]);
    }
    display.display();
  } else {
    Serial.println("-------------------------");
    Serial.println(questions[currentQuestion]);
    for(int i=0; i<4; i++) {
      if(i == currentOption)
        Serial.print("> ");
      else
        Serial.print("  ");
      Serial.println(options[currentQuestion][i]);
    }
  }
}

void showThankYou() {
  if (oledConnected) {
    display.clearDisplay();
    display.setCursor(10, 25);
    display.setTextSize(2);
    display.println("Thank You!");
    display.display();
  } else {
    Serial.println("Thank you for participating!");
  }
}