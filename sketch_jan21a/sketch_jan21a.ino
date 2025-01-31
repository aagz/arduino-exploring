#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Инициализация дисплея
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Адрес 0x3C для I2C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay(); // Очистка дисплея
  display.setTextSize(3); // Размер текста
  display.setTextColor(SSD1306_WHITE); // Цвет текста
  display.setCursor(0, 0); // Позиция курсора
  display.println("1234567890"); // Вывод текста
  display.display(); // Обновление дисплея
}

void loop() {
  // Ничего не делаем в loop
}