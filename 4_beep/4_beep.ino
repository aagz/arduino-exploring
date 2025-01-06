bool shutdown = false;
int beepTone = 0;
bool beepSide = true;

void setup() {
  // put your setup code here, to run once:
  pinMode(8, OUTPUT);
  pinMode(2, OUTPUT); 
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  shutdown = !digitalRead(2);
  // put your main code here, to run repeatedly:
  Serial.print("2:"); Serial.println(digitalRead(2));
  if (!shutdown) {
    if (beepTone > 5000) {
      beepTone = 5000;
      beepSide = false;
    } else if (beepTone <= 100) {
      beepTone = 100;
      beepSide = true;
    }

    beepTone = beepSide ? beepTone + 100 : beepTone - 100;

    tone(8, beepTone);
    beepLed(beepTone);
    Serial.println(beepTone);

  } else {
    Serial.println("Shutdown!");
    tone(8, 0);
    digitalWrite(8 , 0);
  }
}
  // digitalWrite(8, 1);
//   if (!shutdown) {
//     tone(8,0);
//     // delay(1000);
//     // tone(8,1000);
//     // delay(1000);

//     for (int i = 1; i <= 5000; i += 100) {
//       beepTone = i;
//       tone(8, beepTone);
//       beepLed(i);
//       Serial.println(i);
//     }
//     for (int i = 5000; i >= 0; i -= 100) {
//       tone(8, i);
//       beepLed(i);
//       Serial.println(i);
//     }
//   } else {
//     Serial.println("Shutdown!");
//   }
// }

void beepLed(int tone) {
  if (tone % 100) digitalWrite(10, 1);
  
  if (tone % 300) digitalWrite(11, 1);
  
  if (tone % 200) digitalWrite(12, 1);

  delay(100);
  digitalWrite(10, 0);
  
  digitalWrite(11, 0);
  
  digitalWrite(12, 0);
}
