import serial
import time
import pygame
import math
import sys
from threading import Thread, Lock

# Настройки Arduino
ARDUINO_PORT = 'COM5'
BAUDRATE = 9600

# Настройки Pygame
WIDTH, HEIGHT = 1920, 1080
FPS = 60
TRIANGLE_SIZE = 30
BASE_SPEED = 3
MAX_ROTATION_SPEED = 6.4  # +15% (было 5.8)

# Инициализация Serial
try:
    arduino = serial.Serial(port=ARDUINO_PORT, baudrate=BAUDRATE, timeout=1)
    time.sleep(1.5)
except serial.SerialException as e:
    print(f"Ошибка подключения: {e}")
    exit()


def send_command(command):
    arduino.write(f"{command}\n".encode())
    time.sleep(0.03)
    return arduino.readline().decode().strip()


# Инициализация Pygame
pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Boids>")
font = pygame.font.Font(None, 36)
clock = pygame.time.Clock()


class Boid:
    def __init__(self):
        self.x = WIDTH // 2
        self.y = HEIGHT // 2
        self.angle = 0.0
        self.rotation_speed = 0.0
        self.smoothed_rotation = 0.0

    def update(self, distance, dt):
        # Инвертированное управление и ограничение диапазона
        if 0 <= distance <= 20:
            # Инверсия направления: <10 см - вправо, >10 см - влево
            target_rotation = (10 - distance) * 0.68  # Новый коэффициент для +15% скорости
        else:
            target_rotation = 0  # Остановка при выходе за диапазон

        # Плавное изменение скорости
        self.smoothed_rotation += (target_rotation - self.smoothed_rotation) * 0.1

        # Ограничение скорости
        self.rotation_speed = max(-MAX_ROTATION_SPEED,
                                  min(MAX_ROTATION_SPEED,
                                      self.smoothed_rotation))

        # Обновление угла
        self.angle += self.rotation_speed * dt * 60

        # Движение
        radians = math.radians(self.angle)
        direction = pygame.Vector2(math.cos(radians), -math.sin(radians))
        self.x += direction.x * BASE_SPEED
        self.y += direction.y * BASE_SPEED

        # Тороидальное пространство
        self.x = self.x % WIDTH
        self.y = self.y % HEIGHT

    def draw(self, surface):
        angle_rad = math.radians(self.angle)
        points = [
            (math.cos(angle_rad) * TRIANGLE_SIZE,
             -math.sin(angle_rad) * TRIANGLE_SIZE),
            (math.cos(angle_rad + 2.3) * TRIANGLE_SIZE / 2,
             -math.sin(angle_rad + 2.3) * TRIANGLE_SIZE / 2),
            (math.cos(angle_rad - 2.3) * TRIANGLE_SIZE / 2,
             -math.sin(angle_rad - 2.3) * TRIANGLE_SIZE / 2)
        ]
        translated_points = [(x + self.x, y + self.y) for x, y in points]
        pygame.draw.polygon(surface, (255, 40, 40), translated_points)


# Глобальные переменные
current_distance = 10.0
lock = Lock()
boid = Boid()


def arduino_reader():
    global current_distance
    while True:
        response = send_command("GET_DATA")
        if response:
            try:
                distance = float(response)
                with lock:
                    current_distance = distance
            except:
                pass
        time.sleep(0.02)


Thread(target=arduino_reader, daemon=True).start()

running = True
last_time = time.time()
warning_color = (255, 0, 0)

while running:
    current_time = time.time()
    dt = current_time - last_time
    last_time = current_time

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    with lock:
        boid.update(current_distance, dt)

    screen.fill((255, 240, 240))
    boid.draw(screen)

    # Визуализация данных
    status_text = [
        f"Расстояние: {current_distance:.1f} см",
        f"Скорость поворота: {boid.rotation_speed:.1f}°/фрейм",
        f"Угол: {boid.angle:.1f}°"
    ]

    # Предупреждение при выходе за диапазон
    if current_distance < 0 or current_distance > 20:
        warn_text = font.render("ВНЕ ДИАПАЗОНА!", True, warning_color)
        screen.blit(warn_text, (WIDTH - 250, 10))

    # Отрисовка текста
    y_offset = 10
    for text_line in status_text:
        text_surface = font.render(text_line, True, (25, 25, 25))
        screen.blit(text_surface, (10, y_offset))
        y_offset += 40

    pygame.display.flip()
    clock.tick(FPS)

arduino.close()
pygame.quit()