#include <LiquidCrystal_I2C.h>
#include "sounds.h"

#define RED_PIN 12
#define YELLOW_PIN 11
#define GREEN_PIN 10

#define BUZZER_PIN 8

#define BUTTON_PIN 3

enum GameState {
  READY,
  PREPARE,
  GAME,
  LOSS
};

enum RunningManStates {
  STOP,
  RUNNING
};

GameState gameState = READY;

const uint8_t runningManFrames[2][8] = {
  { 0x06, 0x06, 0x0E, 0x17, 0x06, 0x07, 0x0D, 0x09 }, 
  { 0x06, 0x06, 0x0E, 0x0F, 0x0E, 0x06, 0x06, 0x07 }
};

byte currentManFrame = 0;

int currentScore = 0;

const uint8_t leftSeparator[8] = {
  0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08
};

bool buttonState = false;
bool lastButtonState = false;
bool isButtonPressed = false;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  Serial.begin(9600);

  // initialize lcd 1602
  lcd.init();
  lcd.backlight();
  
  
  for (int i = 0; i < sizeof(runningManFrames); i++) {
    lcd.createChar(i, runningManFrames[i]);
  }

  lcd.createChar(3, leftSeparator);

  setupSounds(BUZZER_PIN);

  pinMode(BUTTON_PIN, OUTPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {
  int buttonReading = digitalRead(BUTTON_PIN);
  Serial.println(gameState);
  if (gameState == READY) {
    readyGame();

  } else if (gameState == PREPARE) {
    prepareGame();
    
  } else if (gameState == GAME) {
    game();
  } else {
    playLossSound();
  }
  playJumpSoundNonBlocking();
}

bool checkButton() {
  int buttonReading = digitalRead(BUTTON_PIN);

  if (buttonReading != lastButtonState) {
    lastDebounceTime = millis(); 
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonReading != buttonState) {
      buttonState = buttonReading;

      if (buttonState == HIGH && !isButtonPressed) {
        isButtonPressed = true;
        return true;
      }
    }
  }

  if (buttonReading == LOW && isButtonPressed) {
    isButtonPressed = false;
  }

  lastButtonState = buttonReading;
  return false; 
}

void readyGame() {
  lcd.setCursor(5,0);
  lcd.print("Ready?");

  if (checkButton()) {
    gameState = PREPARE;
    lcd.clear();
  }
}

void prepareGame() {
  int leds[3] = {GREEN_PIN, YELLOW_PIN, RED_PIN};

  for (int i = 3; i >= 1; --i) {
    digitalWrite(leds[i-1], HIGH);
    lcd.setCursor(7,0); lcd.print(i);
    lcd.setCursor(8,0); lcd.print(".");
    tone(BUZZER_PIN, 1000);
    delay(100);
    noTone(BUZZER_PIN);
    delay(900);
    digitalWrite(leds[i-1], LOW);
  }

  tone(BUZZER_PIN, 1500);
  for (int i = 0; i < 3; i++) {
    digitalWrite(leds[i], HIGH);
  }    
  delay(200);
  noTone(BUZZER_PIN);
  for (int i = 0; i < 3; i++) {
    digitalWrite(leds[i], LOW);
  }
  gameState = GAME;
  lcd.clear();
}

void game() {
  if (currentScore / 100 != 0) {
    lcd.setCursor(0, 0);
  } else if (currentScore / 10 != 0) {
    lcd.setCursor(1, 0);
  } else {
    lcd.setCursor(2, 0);
  }

  lcd.print(currentScore);

  lcd.setCursor(3, 0);
  lcd.write(3);
  lcd.setCursor(3, 1);
  lcd.write(3);

  if (checkButton()) {
    currentScore++;
    // startJumpSound();
    // playJumpSoundNonBlocking();
  }
}
