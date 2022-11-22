void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);


}

void loop() {
  // put your main code here, to run repeatedly:
int val2 = analogRead(A0);
int val3 = analogRead(A1);
int val4 = analogRead(A2);
int val5 = analogRead(A3);
int val6 = analogRead(A4);
int val7 = analogRead(A5);
int waarden[6] = {val2, val3, val4, val5, val6, val7};

for(int i = 0; i < 6; i++)
{
  Serial.println(waarden[i]);
}

Serial.println("\n");

delay(1000);
}
