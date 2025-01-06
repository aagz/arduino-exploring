bool butt_flag = false;
bool butt;

bool butt_reset;

bool led_flag;
int LEDcounter = 7;
unsigned long last_press;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT); // вперед
  pinMode(3, OUTPUT); // назад
  
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  digitalWrite(7, 0);
  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  digitalWrite(12, 0);
}

bool butt_last_state = false;

void loop() {
  butt = digitalRead(3);
  butt_reset = digitalRead(2);
  if (butt != butt_last_state) {
    if (butt && !butt_flag && millis() - last_press > 100) {
      butt_flag = 1;
      Serial.println("Button pressed");
      Serial.print("LED: "); Serial.println(LEDcounter);
      digitalWrite(LEDcounter, 1);
      if (LEDcounter > 12) {
        for (int i = 7; i <= 12; i++) {
          digitalWrite(i, 0);
        }
      }
      LEDcounter = LEDcounter > 12 ? 7 : ++LEDcounter;
      
    } else if (!butt && butt_flag) {
      butt_flag = 0;
      Serial.println("Button released");
    }
  }

  if (butt_reset) {
    for (int i = 7; i <= 12; i++) {
      digitalWrite(i, 0);
    }
    LEDcounter = 7;
  }

  // Serial.println(butt_reset);

  butt_last_state = butt;



  // Serial.println(butt_flag);

  // Serial.println(LEDcounter);

  
  // if (butt && !butt_flag) {
  //   LEDcounter++;
  //   digitalWrite(LEDcounter, 1);
  // }


}