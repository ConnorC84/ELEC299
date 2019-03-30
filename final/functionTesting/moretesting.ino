#define test A4
int testing = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(test, INPUT);
  pinMode(testing, INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
 
  Serial.print(analogRead(test));
  delay(100);
  Serial.println();
}
