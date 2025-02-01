import pyfirmata2
import time

# Настройка порта Arduino и создание объекта доски
board = pyfirmata2.Arduino('COM5')  # Заменить на нужный порт
it = pyfirmata2.util.Iterator(board)
it.start()

# Пины для Echo и Trigger
echoPin = board.get_pin('d:2:i')  # Echo pin, input
trigPin = board.get_pin('d:3:o')  # Trigger pin, output

# Функция для получения расстояния
def get_distance():
    trigPin.write(0)  # Поставить Trigger в LOW
    time.sleep(0.002)  # Задержка 2мкс
    trigPin.write(1)  # Поставить Trigger в HIGH
    time.sleep(0.01)  # Задержка 10мкс
    trigPin.write(0)  # Поставить Trigger в LOW

    duration = echoPin.read()  # Чтение продолжительности импульса от Echo pin
    if duration is None:
        return None

    # Перевод времени в см
    cm = duration * 17150  # Расстояние в см
    cm = round(cm, 2)
    return cm

# Главный цикл
while True:
    distance = get_distance()
    if distance is not None:
        print(f"{distance} cm")
    else:
        print("Error reading distance")

    time.sleep(0.1)  # Пауза 100 мс
