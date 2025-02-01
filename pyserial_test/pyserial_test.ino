const int trigPin = 9;      // Пин Trig ультразвукового датчика
const int echoPin = 10;     // Пин Echo ультразвукового датчика
const int ledPin = 13;      // Пин светодиода
const int numReadings = 5;  // Количество измерений для медианного фильтра

long duration;              // Время прохождения сигнала
int distance;               // Расстояние, рассчитанное по времени
int readings[numReadings];  // Массив для хранения измерений
String command = "";        // Буфер для хранения команды

void setup() {
  Serial.begin(9600);       // Инициализация Serial-порта
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Выключаем светодиод при старте
}

void loop() {
  // Проверяем, есть ли данные для чтения
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n'); // Читаем команду до символа новой строки
    command.trim(); // Убираем лишние пробелы и символы новой строки

    // Обрабатываем команду "ping"
    if (command == "ping") {
      Serial.println("pong"); // Отправляем "pong" в ответ
      digitalWrite(ledPin, HIGH); // Включаем светодиод
      delay(500);             // Ждем 500 мс
      digitalWrite(ledPin, LOW); // Выключаем светодиод
    }

    // Обрабатываем команду "GET_DATA"
    else if (command == "GET_DATA") {
      // Делаем несколько измерений для медианного фильтра
      for (int i = 0; i < numReadings; i++) {
        // Генерируем импульс на пине Trig
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        // Измеряем время, которое импульс прошел от Trig до Echo
        duration = pulseIn(echoPin, HIGH);

        // Рассчитываем расстояние
        readings[i] = duration * 0.034 / 2; // Преобразуем в см
        delay(10); // Задержка между измерениями
      }

      // Сортируем массив для медианного фильтра
      for (int i = 0; i < numReadings - 1; i++) {
        for (int j = i + 1; j < numReadings; j++) {
          if (readings[i] > readings[j]) {
            int temp = readings[i];
            readings[i] = readings[j];
            readings[j] = temp;
          }
        }
      }

      // Выбираем медиану
      int medianDistance = readings[numReadings / 2];

      // Отправляем медианное значение
      Serial.println(medianDistance);
    }

    // Очищаем команду для следующего ввода
    command = "";
  }

  delay(10); // Небольшая задержка, чтобы не перегружать процессор
}