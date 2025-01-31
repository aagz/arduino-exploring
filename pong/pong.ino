#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

#define LEFT_STICK A0
#define RIGHT_STICK A1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  // Инициализация дисплея
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Используем SSD1306_SWITCHCAPVCC и адрес 0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  pinMode(LEFT_STICK, INPUT);
  pinMode(RIGHT_STICK, INPUT);
}

int pane_height = 26;
int pane_width = 5;
int playerOneY = SCREEN_HEIGHT / 2 - pane_height / 2;
int playerTwoY = SCREEN_HEIGHT / 2 - pane_height / 2;

int ballStartX = SCREEN_WIDTH / 2 - 1;
int ballStartY = SCREEN_HEIGHT / 2 - 1;
int ballX = ballStartX;
int ballY = ballStartY;

int ballSpeedX = 3; // Начальная скорость по X
int ballSpeedY = 3; // Начальная скорость по Y

void loop() {
  display.clearDisplay();

  // Чтение значений с джойстиков
  int left = analogRead(LEFT_STICK);
  int right = analogRead(RIGHT_STICK);
  Serial.print("LEFT: "); Serial.print(left); Serial.print(" RIGHT: "); Serial.println(right);

  // Управление левой ракеткой (игрок 1)
  if (left < 400) {
    playerOneY = playerOneY - 3 >= 0 ? playerOneY - 3 : 0;
  } else if (left > 650) {
    playerOneY = playerOneY + 3 + pane_height <= SCREEN_HEIGHT ? playerOneY + 3 : SCREEN_HEIGHT - pane_height;
  }
  display.drawRect(10, playerOneY, pane_width, pane_height, SSD1306_WHITE);

  // Управление правой ракеткой (игрок 2)
  if (right < 400) {
    playerTwoY = playerTwoY - 3 >= 0 ? playerTwoY - 3 : 0;
  } else if (right > 650) {
    playerTwoY = playerTwoY + 3 + pane_height <= SCREEN_HEIGHT ? playerTwoY + 3 : SCREEN_HEIGHT - pane_height;
  }
  display.drawRect(SCREEN_WIDTH - 10 - pane_width, playerTwoY, pane_width, pane_height, SSD1306_WHITE);

  // Движение шара
  ballX += ballSpeedX;
  ballY += ballSpeedY;

  // Проверка столкновения с верхней и нижней границами
  if (ballY >= SCREEN_HEIGHT - 4 || ballY <= 4) {
    ballSpeedY *= -1; // Отскок по Y
  }

  // Проверка столкновения с левой ракеткой (игрок 1)
  if (ballX <= 10 + pane_width + 2 && ballX >= 10 &&
      ballY >= playerOneY && ballY <= playerOneY + pane_height) {
    ballSpeedX *= -1; // Отскок по X
    ballX = 10 + pane_width + 2; // Корректировка позиции шара, чтобы избежать залипания
  }

  // Проверка столкновения с правой ракеткой (игрок 2)
  if (ballX >= SCREEN_WIDTH - 10 - pane_width - 2 && ballX <= SCREEN_WIDTH - 10 &&
      ballY >= playerTwoY && ballY <= playerTwoY + pane_height) {
    ballSpeedX *= -1; // Отскок по X
    ballX = SCREEN_WIDTH - 10 - pane_width - 2; // Корректировка позиции шара
  }

  // Проверка выхода шара за границы (гол)
  if (ballX <= 0 || ballX >= SCREEN_WIDTH) {
    // Сброс позиции шара
    ballX = ballStartX;
    ballY = ballStartY;
    ballSpeedX = 3; // Сброс скорости
    ballSpeedY = 3;
  }

  // Отрисовка шара
  display.drawCircle(ballX, ballY, 2, SSD1306_WHITE);

  // Обновление дисплея
  display.display();

  delay(50);
}