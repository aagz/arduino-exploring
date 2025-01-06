#ifndef SOUNDS_H
#define SOUNDS_H

#include <Arduino.h>

int buzzerPin;

unsigned long lastJumpTime = 0;
int jumpStep = 0;
bool isJumping = false;

void setupSounds(int pin) {
  buzzerPin = pin;
  pinMode(buzzerPin, OUTPUT);
}

void playJumpSoundNonBlocking() {
  if (isJumping) {
    if (millis() - lastJumpTime >= 100) {
      lastJumpTime = millis();

      int freq = 500 + (jumpStep * 100);
      tone(buzzerPin, freq, 100);       
      jumpStep++;

      if (jumpStep > 5) {
        isJumping = false;
        noTone(buzzerPin);
      }
    }
  }
}

void startJumpSound() {
  isJumping = true;
  jumpStep = 0;
}

void playLossSound() {
  for (int freq = 1000; freq >= 200; freq -= 100) {
    tone(buzzerPin, freq, 150);
    delay(150);                
  }
}

#endif // SOUNDS_H
