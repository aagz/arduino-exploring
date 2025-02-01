import serial
import time
import pygame
import sys
from threading import Thread, Lock

# Настройки Arduino
arduino_port = 'COM5'
baudrate = 9600

# Настройки Pygame
width, height = 800, 600
ball_radius = 50
fps = 60

# Экстремальные параметры реакции
response_speed = 18.0  # Сильно увеличиваем (было 12)
damping_factor = 0.85  # Оптимизируем демпфирование (было 0.85)
smoothing = 0.03  # Минимальное сглаживание (было 0.1)
polling_interval = 0.02  # Максимальная частота опроса (было 0.03)

# Инициализация Serial
try:
    arduino = serial.Serial(port=arduino_port, baudrate=baudrate, timeout=1)
    time.sleep(1.5)  # Уменьшаем время ожидания
except serial.SerialException as e:
    print(f"Ошибка подключения: {e}")
    exit()


def send_command(command):
    arduino.write(f"{command}\n".encode())
    time.sleep(0.03)  # Оптимизированная задержка
    return arduino.readline().decode().strip()


# Инициализация Pygame
pygame.init()
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Молниеносный контроль шара")
font = pygame.font.Font(None, 36)
clock = pygame.time.Clock()

# Глобальные переменные
target_y = height - ball_radius
current_y = target_y
velocity = 0
lock = Lock()


def arduino_reader():
    global target_y
    while True:
        response = send_command("GET_DATA")
        if response:
            try:
                distance = max(0, min(20, float(response)))
                with lock:
                    # Минимальное сглаживание цели
                    new_target = height - (distance / 20) * (height - 2 * ball_radius) - ball_radius
                    target_y = new_target * (1 - smoothing) + target_y * smoothing
            except:
                pass
        time.sleep(polling_interval)  # Максимальная частота опроса


Thread(target=arduino_reader, daemon=True).start()

running = True
prev_time = time.time()
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    current_time = time.time()
    dt = current_time - prev_time
    prev_time = current_time

    with lock:
        error = target_y - current_y
        # Агрессивное ускорение
        acceleration = error * response_speed * 1.5  # Дополнительный множитель
        velocity += acceleration * dt * 3.2  # Экстремальное ускорение
        velocity *= damping_factor
        # Нелинейное движение
        current_y += velocity * dt + 0.7 * acceleration * dt ** 2

    # Отрисовка
    screen.fill((255, 255, 255))
    pygame.draw.circle(screen, (255, 0, 0), (width // 2, int(current_y)), ball_radius)

    # Расчет расстояния
    display_distance = 20 * (height - current_y - ball_radius) / (height - 2 * ball_radius)
    text = font.render(f"Расстояние: {display_distance:.1f} см", True, (0, 0, 0))
    screen.blit(text, (10, 10))

    pygame.display.flip()
    clock.tick(fps)

arduino.close()
pygame.quit()