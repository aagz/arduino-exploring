#define DATA 6
#define LATCH 8
#define CLOCK 10

int arr[] = {128, 64, 32, 16, 8, 4, 2, 1};

int v = 0b00000000;

int num[10] = {
  0b11111100,
  0b01100000,
  0b11011010,
  0b11110010,
  0b01100110,
  0b10110110,
  0b10111110,
  0b11100000,
  0b11111110,
  0b11110110
};


void setup() {
  // put your setup code here, to run once:
  pinMode(DATA, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int i;
  for (i = 0; i < 10; i++) {
    v = v > 255 ? 0 : v + 1;
    digitalWrite(LATCH, LOW);
    shiftOut(DATA, CLOCK, LSBFIRST, num[i]);
    digitalWrite(LATCH, HIGH);
    delay(200);
  }
}
