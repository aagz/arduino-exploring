#include <LiquidCrystal_I2C.h>
#include "sounds.h"
#include "frames.h"
#include "button.h"

#define RED_PIN 12
#define YELLOW_PIN 11
#define GREEN_PIN 10

#define BUZZER_PIN 8

#define BUTTON_PIN 3

enum GameState {
  READY,
  PREPARE,
  GAME,
  LOSE
};

enum RunningManStates {
  STOP,
  RUNNING
};

enum Entities {
  NONE,
  PLAYER,
  ENEMY
};

Entities startGameField[2][12] = {
  {NONE,NONE,NONE,NONE,NONE,NONE,NONE,ENEMY,NONE,NONE,NONE,NONE},
  {PLAYER,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,ENEMY}
};

Entities gameField[2][12];

GameState gameState = READY;

byte currentManFrame = 0;

int currentScore;

int gameSpeed = 90; // 60 ticks per min
unsigned long lastGameTick;

LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  Serial.begin(9600);

  // initialize lcd 1602
  lcd.init();
  lcd.backlight();
  
  for (int i = 0; i < 2; i++) {
    lcd.createChar(i+1, runningManFrames[i]);
  }

  lcd.createChar(3, leftSeparatorFrame);
  lcd.createChar(4, enemyFrame);
  lcd.createChar(5, emptyFrame);

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
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(currentScore);
    playLossSound();
    
    gameState = READY;
  }
  playJumpSoundNonBlocking();
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

  memcpy(gameField, startGameField, sizeof(startGameField));
  currentScore = 0;

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

  int startField = 4;
  for (int i = 0; i <= 12; i++) {
    for (int j = 0; j < 2; j++) {
      lcd.setCursor(startField + i, j);
      
      if (gameField[j][i] == PLAYER) {
        // Проверяем, нужно ли обновить кадр
        if (millis() - lastPlayerFrameTime >= playerFrameDuration) {
          lastPlayerFrame = (lastPlayerFrame == 1) ? 2 : 1; // Смена кадра
          lastPlayerFrameTime = millis(); // Обновление времени
        }
        lcd.write(lastPlayerFrame); // Отображение текущего кадра
      } 
      else if (gameField[j][i] == ENEMY) {
        lcd.write(4); // Отображение врага
      } 
      else if (gameField[j][i] == NONE) {
        lcd.write(5); // Отображение пустой ячейки
      }
    }
  }

  if (checkButton()) {
    if (gameField[0][0] == PLAYER && gameField[1][0] == NONE) {
      gameField[0][0] = NONE;
      gameField[1][0] = PLAYER;
    } else if (gameField[1][0] == PLAYER && gameField[0][0] == NONE) {
      gameField[1][0] = NONE;
      gameField[0][0] = PLAYER;
    } else {
      gameState = LOSE;
    }
  }


  if (millis() - lastGameTick >= 60000 / gameSpeed) {
    for (int i = 0; i < 12; i++) {
      for (int j = 0; j < 2; j++) {
        if (gameField[j][i] == ENEMY && gameField[j][i-1] == PLAYER) {
          gameState = LOSE;
          Serial.println("LOSE");
        } else if (gameField[j][i] == ENEMY && i > 0) {
          gameField[j][i-1] = ENEMY;
          gameField[j][i] = NONE;
        } else if (gameField[j][i] == ENEMY && i == 0) {
          gameField[j][i] = NONE;
        }
      }
    }

    if (gameField[0][11] == NONE && 
      gameField[1][11] == NONE && 
      gameField[0][10] == NONE && 
      gameField[1][10] == NONE && random(1, 100) < 40 ) {
      gameField[random(0,2)][11] = ENEMY;
    }
    lastGameTick = millis();
    currentScore++;
  }

  if (checkButton()) {
    // currentScore++;
    // startJumpSound();
    // playJumpSoundNonBlocking();
  }

}
