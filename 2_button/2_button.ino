void setup() {
  // put your setup code here, to run once:
  pinMode(A5, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  bool button1 = !digitalRead(A5);
  bool button2 = !digitalRead(13);

  if (button1 == 1) digitalWrite(13, 1);
  else digitalWrite(13, 0);

  Serial.print("Button1: "), Serial.print(button1);
  Serial.print(", Button2: "), Serial.println(button2);
}
