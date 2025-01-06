
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Определение пинов для подключения к 7-ми сегментному индикатору
// Пины для подключения SN74HC595N
int dataPin = 12;   // DS (Data)
int latchPin = 10;  // ST_CP (Latch)
int clockPin = 11;  // SH_CP (Shift Clock)

// const int dotPin = 5;
const int displayPins[] = {A0, A1, A2, A3};

const int btnPin = 2;
bool lastButtonState = LOW;
bool buttonPress;
// int splittedNumber[4];

int counter = 0;

const uint8_t runningMan[2][8] {
  {0x06, 0x06, 0x0E, 0x17, 0x06, 0x07, 0x0D, 0x09},
  {0x06, 0x06, 0x0E, 0x0F, 0x0E, 0x06, 0x06, 0x07}
};

// Определение символов для каждой цифры (a, b, c, d, e, f, g)
const byte digits[][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

// Массив для цифр 0-9 на 7-сегментном дисплее
byte num[] = { 
  0x3F,  // 0
  0x06,  // 1
  0x5E,  // 2
  0x4E,  // 3
  0x66,  // 4
  0x6D,  // 5
  0x7D,  // 6
  0x07,  // 7
  0x7F,  // 8
  0x6F,  // 9
};

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Serial.begin(9600);

  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);


  // Инициализация для пинов анодов дисплеев
  for (int i = 0; i < 4; i++) {
    pinMode(displayPins[i], OUTPUT);
  }

  // Инициализация всех дисплеев как выключенных
  for (int i = 0; i < 4; i++) {
    digitalWrite(displayPins[i], LOW);
  }

  // Инициализация для сдвигового регистра
  digitalWrite(latchPin, LOW);
  digitalWrite(clockPin, LOW);

  pinMode(btnPin, INPUT_PULLUP);
  // 1602 init
  lcd.init();
  lcd.backlight();

}

void loop() {
  bool btn = !digitalRead(A1);
  Serial.println(btn);

  // Serial.print("Counter: "); Serial.println(counter);


  for (int i = 0; i < 4; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, 0b10101010); // Отправляем цифру
    digitalWrite(displayPins[i], HIGH); // Включаем соответствующий дисплей
    delay(10); // Уменьшаем задержку для быстрого мультиплексирования
    digitalWrite(displayPins[i], LOW); // Выключаем дисплей после показа
    // Serial.println("---");



  }

    Serial.println(digitalRead(10));
    Serial.println(digitalRead(11));
    Serial.println(digitalRead(12));

    Serial.println(analogRead(A0));
    Serial.println(analogRead(A1));
    Serial.println(analogRead(A2));
    Serial.println(analogRead(A3));
}

// void displayNumber(int number) {
//   for (int digit = 3; digit >= 0; digit--) {
//     // Включение нужного разряда
//     for (int i = 0; i < 4; i++) {
//       if (i == digit) {
//         digitalWrite(digitPins[i], LOW); // Включаем разряд
//       } else {
//         digitalWrite(digitPins[i], HIGH); // Выключаем остальные разряды
//       }
//     }
    
//     // Отображение соответствующей цифры на сегментах
//     for (int segment = 0; segment < 7; segment++) {
//       if (digits[number % 10][segment]) {
//         digitalWrite(segmentPins[segment], HIGH); // Включаем сегмент
//       } else {
//         digitalWrite(segmentPins[segment], LOW); // Выключаем сегмент
//       }
//     }

//     // Сдвигаем число вправо для следующей цифры
//     number /= 10;
    
//     // Задержка для отображения цифры
//     delay(3); // Можно увеличить или уменьшить для настройки яркости
//   }
// }
