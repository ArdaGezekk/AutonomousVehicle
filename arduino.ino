const int ActiveLeft = 5;
const int HighLeft = 7;       // LEFT MOTOR
const int LowLeft =6;
const int ActiveRight = 10;
const int HighRight = 8;       //RIGHT MOTOR
const int LowRight =9;
const int D0 = 0;       //Raspberry pin 21
const int D1 = 1;       //Raspberry pin 22
const int D2 = 2;       //Raspberry pin 23
const int D3 = 3;       //Raspberry pin 24
const int D4 = 4;       //Raspberry pin 25
int firstDigit,secondDigit,thirdDigit,fourthDigit,fifthDigit,decimalValue;
void setup() {
  pinMode(ActiveLeft, OUTPUT);
  pinMode(HighLeft, OUTPUT);
  pinMode(LowLeft, OUTPUT);
  pinMode(ActiveRight, OUTPUT);
  pinMode(HighRight, OUTPUT);
  pinMode(LowRight, OUTPUT);
  pinMode(D0, INPUT_PULLUP);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);
}
void Data(){
  firstDigit = digitalRead(D0);
  secondDigit = digitalRead(D1);
  thirdDigit = digitalRead(D2);
  fourthDigit = digitalRead(D3);
  fifthDigit = digitalRead(D4);
  decimalValue = ((16*fifthDigit)+(8*fourthDigit)+(4*thirdDigit)+(2*secondDigit)+firstDigit);
}
void Stop(){
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  analogWrite(ActiveLeft,0);
  digitalWrite(HighRight, LOW);
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveRight,0);
}
void Forward(){
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  analogWrite(ActiveLeft,180);
  digitalWrite(HighRight, LOW);
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveRight,180);
}
void Backward(){
  digitalWrite(HighLeft, HIGH);
  digitalWrite(LowLeft, LOW);
  analogWrite(ActiveLeft,180);
  digitalWrite(HighRight, HIGH);
  digitalWrite(LowRight, LOW);
  analogWrite(ActiveRight,180);
}
void littleLeft(){
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  analogWrite(ActiveLeft,120);
  digitalWrite(HighRight, LOW);
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveRight,190);
}
void mediumLeft(){
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  analogWrite(ActiveLeft,70);
  digitalWrite(HighRight, LOW);
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveRight,190);
}
void hardLeft(){
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  analogWrite(ActiveLeft,20);
  digitalWrite(HighRight, LOW);
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveRight,190);
}
void littleRight(){
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  analogWrite(ActiveLeft,190);
  digitalWrite(HighRight, LOW);
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveRight,120);
}
void mediumRight(){
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  analogWrite(ActiveLeft,190);
  digitalWrite(HighRight, LOW);
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveRight,70);
}
void hardRight(){
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  analogWrite(ActiveLeft,190);
  digitalWrite(HighRight, LOW);
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveRight,20);
}
void TurnU(){
  analogWrite(ActiveLeft, 0);
  analogWrite(ActiveRight, 0);
  delay(800);
  digitalWrite(HighLeft, HIGH);
  digitalWrite(LowLeft, LOW);
  digitalWrite(HighRight, LOW);   //   left
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveLeft, 235);
  analogWrite(ActiveRight, 235);
  delay(750);
  analogWrite(ActiveLeft, 0);
  analogWrite(ActiveRight, 0);
  delay(400);
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  digitalWrite(HighRight, LOW);   // forward
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveLeft, 110);
  analogWrite(ActiveRight, 110);
  delay(1100);
  analogWrite(ActiveLeft, 0);
  analogWrite(ActiveRight, 0);
  delay(400);
  digitalWrite(HighLeft, HIGH);
  digitalWrite(LowLeft, LOW);
  digitalWrite(HighRight, LOW);   //   left
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveLeft, 235);
  analogWrite(ActiveRight, 235);
  delay(750);
  analogWrite(ActiveLeft, 0);
  analogWrite(ActiveRight, 0);
  delay(1000);
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  digitalWrite(HighRight, LOW);
  digitalWrite(LowLeft, HIGH);
  analogWrite(ActiveLeft, 150);
  analogWrite(ActiveRight, 150);
  delay(300);
  analogWrite(ActiveLeft, 0);
  analogWrite(ActiveRight, 0);
  delay(500);
}
void Object(){
  analogWrite(ActiveLeft, 0);
  analogWrite(ActiveRight, 0);
  delay(800);
  digitalWrite(HighLeft, HIGH);
  digitalWrite(LowLeft, LOW);
  digitalWrite(HighRight, LOW);   //   left
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveLeft, 185);
  analogWrite(ActiveRight, 185);
  delay(800);
  analogWrite(ActiveLeft, 0);
  analogWrite(ActiveRight, 0);
  delay(300);
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  digitalWrite(HighRight, LOW);   // forward
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveLeft, 110);
  analogWrite(ActiveRight, 110);
  delay(1000);
  analogWrite(ActiveLeft, 0);
  analogWrite(ActiveRight, 0);
  delay(300);
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  digitalWrite(HighRight, HIGH);   //   right
  digitalWrite(LowRight, LOW);
  analogWrite(ActiveLeft, 185);
  analogWrite(ActiveRight, 185);
  delay(800);
  analogWrite(ActiveLeft, 0);
  analogWrite(ActiveRight, 0);
  delay(300);
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  digitalWrite(HighRight, LOW);   // forward
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveLeft, 110);
  analogWrite(ActiveRight, 110);
  delay(850);
  analogWrite(ActiveLeft, 0);
  analogWrite(ActiveRight, 0);
  delay(300);
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  digitalWrite(HighRight, HIGH);   //   right
  digitalWrite(LowRight, LOW);
  analogWrite(ActiveLeft, 185);
  analogWrite(ActiveRight, 185);
  delay(850);
  analogWrite(ActiveLeft, 0);
  analogWrite(ActiveRight, 0);
  delay(300);
  digitalWrite(HighLeft, LOW);
  digitalWrite(LowLeft, HIGH);
  digitalWrite(HighRight, LOW);   // forward
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveLeft, 110);
  analogWrite(ActiveRight, 110);
  delay(1200);
  analogWrite(ActiveLeft, 0);
  analogWrite(ActiveRight, 0);
  delay(300);
  digitalWrite(HighLeft, HIGH);
  digitalWrite(LowLeft, LOW);
  digitalWrite(HighRight, LOW);   //   left
  digitalWrite(LowRight, HIGH);
  analogWrite(ActiveLeft, 185);
  analogWrite(ActiveRight, 185);
  delay(700);
  analogWrite(ActiveLeft, 0);
  analogWrite(ActiveRight, 0);
  delay(500);
}
void loop(){
  Data();
  if(decimalValue == 1)
  {
    Forward();
  }
  else if(decimalValue == 3)
  {
    littleRight();
  }
  else if(decimalValue == 5)
  {
    mediumRight();
  }
  else if(decimalValue == 7)
  {
    hardRight();
  }
  else if(decimalValue == 9)
  {
    littleLeft();
  }
  else if(decimalValue == 11)
  {
    mediumLeft();
  }
  else if(decimalValue == 13)
  {
    hardLeft();
  }
  else if (decimalValue == 15)
  {
    digitalWrite(HighLeft, LOW);
    digitalWrite(LowLeft, HIGH);
    digitalWrite(HighRight, LOW);   // forward
    digitalWrite(LowRight, HIGH);
    analogWrite(ActiveLeft, 110);
    analogWrite(ActiveRight, 110);
    delay(550);
    TurnU();
  }
  else if (decimalValue == 17)    //Stop Sign
  {
    analogWrite(ActiveLeft, 0);
    analogWrite(ActiveRight, 0);
    delay(5000);
    analogWrite(ActiveLeft, 150);
    analogWrite(ActiveRight, 150);
    delay(1500);
  }
  else if (decimalValue == 19)  // Barrier
  {
    Object();
  }
  else if (decimalValue == 21) //TrafficLigt
  {
    analogWrite(ActiveLeft, 0);
    analogWrite(ActiveRight, 0);
    delay(1000);
  }
}
