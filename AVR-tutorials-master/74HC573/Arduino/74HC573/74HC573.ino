#define LatchEnable1 10
#define LatchEnable2 11

void setup(){

  for(int i = 2; i <= 9; ++i){ // set pins 2 thru 9 as output
    pinMode(i, OUTPUT);
  }
}

void loop(){

     //set bit pattern for latch 1

     digitalWrite(2, HIGH);
     digitalWrite(3, LOW);
     digitalWrite(4, HIGH);
     digitalWrite(5, LOW);
     digitalWrite(6, HIGH);
     digitalWrite(7, LOW);
     digitalWrite(8, HIGH);
     digitalWrite(9, LOW);

     digitalWrite(LatchEnable1, HIGH); // send LE pin of first latch high to save pattern

     //set bit pattern for latch 2

     digitalWrite(2, HIGH);
     digitalWrite(3, HIGH);
     digitalWrite(4, LOW);
     digitalWrite(5, LOW);
     digitalWrite(6, HIGH);
     digitalWrite(7, HIGH);
     digitalWrite(8, LOW);
     digitalWrite(9, LOW);

     digitalWrite(LatchEnable2, HIGH); // send LE pin of second latch high to save pattern

}
