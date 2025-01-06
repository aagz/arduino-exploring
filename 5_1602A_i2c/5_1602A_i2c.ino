//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Button.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

uint8_t runningMan[8] = {  0x06,
  0x06,
  0x0E,
  0x17,
  0x06,
  0x07,
  0x0D,
  0x09
};

  uint8_t runningMan2[8] = {
  0x06,
  0x06,
  0x0E,
  0x0F,
  0x0E,
  0x06,
  0x06,
  0x07
  };

Button jump_button(2);

  
uint8_t last_frame = 6;
  
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.createChar(6, runningMan);
  
  lcd.createChar(7, runningMan2);
  lcd.home();
  
}


void loop()
{
    lcd.clear();
    
    // for (int j=0; j<16; j++) {
    //   lcd.printByte(i+j);
    // }
    last_frame = last_frame == 6 ? 7 : 6;
    if (jump_button.pressed()) {
      lcd.setCursor(1, 1);
      Serial.println("Button pressed");
    } else {
      lcd.setCursor(0, 1);
    }
    lcd.printByte(last_frame);
    // i+=16;
    Serial.println(111);
    delay(400);
}

