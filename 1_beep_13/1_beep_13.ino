bool sig;
unsigned long last_time;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(13, 1);
}

void loop() {
  // for (int i = 0; i < 10; i++) {
  //   sig = sig == 0 ? 1 : 0;
  //   digitalWrite(13, sig);
  //   delay(100);
  // }

  if (millis() - last_time > 1000) {
    last_time = millis();
    // sig = sig == 0 ? 1 : 0;
    digitalWrite(13, digitalRead(13) ? 0 : 1);
    Serial.println(digitalRead(13));
  }

  delay(3);
}
