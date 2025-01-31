#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

#define ECHO_PIN 2
#define TRIG_PIN 3


void setup() {
  // put your setup code here, to run once:
  // pinMode(RED_PIN, OUTPUT);
  // pinMode(GREEN_PIN, OUTPUT);
  // pinMode(BLUE_PIN, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
    int duration, cm; 
    digitalWrite(TRIG_PIN, LOW); 
    delayMicroseconds(2); 
    digitalWrite(TRIG_PIN, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(TRIG_PIN, LOW); 
    duration = pulseIn(ECHO_PIN, HIGH); 
    cm = duration / 58;
    Serial.print(cm); 
    Serial.println(" cm"); 
    delay(100);



  // put your main code here, to run repeatedly:
  // // analogWrite(RED_PIN, random(0,256));
  // // analogWrite(GREEN_PIN, random(0,256));
  // // analogWrite(BLUE_PIN, random(0,256));
  // // delay(1000);

  // Serial.println((analogRead(0) -520) / 2);
  // int light = (analogRead(0) -520) / 2;
  // light = light < 0 ? 0 : light;
  // // if (analogRead(0) < 700) {
  // //   analogWrite(RED_PIN, 255);
  // //   analogWrite(GREEN_PIN, 255);
  // //   analogWrite(BLUE_PIN, 255);
  // // } else if (analogRead(0) > 750) {
  // //   analogWrite(RED_PIN, 0);
  // //   analogWrite(GREEN_PIN, 0);
  // //   analogWrite(BLUE_PIN, 0);
  // // }

  //   analogWrite(RED_PIN, light);
  //   analogWrite(GREEN_PIN, light);
  //   analogWrite(BLUE_PIN, light);
  // delay(50);
}
