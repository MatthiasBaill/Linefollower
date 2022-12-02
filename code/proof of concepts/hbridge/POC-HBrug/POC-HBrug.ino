#define AIN1 9
#define AIN2 10
#define BIN1 13
#define BIN2 11

void setup() {

}

void loop() {
for(int k = 0; k<255; k+=10)
   {
    analogWrite (AIN1,k);
    analogWrite (BIN1,k);
    delay(100);
   }

  for(int k =255 ; k>= 0; k-=10)
   {
    analogWrite(AIN1,k);
    analogWrite (BIN1,k);
    delay(100);
   }



   for(int k = 0; k<255; k+=10)
   {
    analogWrite (AIN2,k);
    analogWrite (BIN2,k);
    delay(100);
   }

  for(int k =255 ; k>= 0; k-=10)
   {
    analogWrite(AIN2,k);
    analogWrite (BIN2,k);
    delay(100);
   }
  }
