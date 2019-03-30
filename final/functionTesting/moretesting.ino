#define test A5
int testing = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(test, INPUT);
  pinMode(testing, INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  int onLine;
  Serial.println(analogRead(test));
  int buffer = analogRead(test);
  if (buffer < 900){
    onLine = 0;
  }
  else{
    onLine = 1;
  }
  //Serial.println(onLine);
  delay(100);
  Serial.println();
}
