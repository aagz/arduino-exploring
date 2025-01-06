#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_PIN 3

#include <Arduino.h>

bool buttonState = false;
bool lastButtonState = false;
bool isButtonPressed = false;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

bool checkButton() {
  int buttonReading = digitalRead(BUTTON_PIN);

  if (buttonReading != lastButtonState) {
    lastDebounceTime = millis(); 
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonReading != buttonState) {
      buttonState = buttonReading;

      if (buttonState == HIGH && !isButtonPressed) {
        isButtonPressed = true;
        return true;
      }
    }
  }

  if (buttonReading == LOW && isButtonPressed) {
    isButtonPressed = false;
  }

  lastButtonState = buttonReading;
  return false; 
}

#endif // BUTTON_H