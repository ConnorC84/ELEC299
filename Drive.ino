int IR1 = A0;
int IR2 = A1;
int IR3 = A2;

int rightDirection = 7;
int leftDirection = 4;
int leftSpeed = 5;
int rightSpeed = 6;

#define IR1Threshold 975 
#define IR2Threshold 975
#define IR3Threshold 975 


void setup() {
  // IR Sensors 
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);

  // Motor Controllers 
  pinMode(rightDirection, OUTPUT);
  pinMode(leftDirection, OUTPUT);
  pinMode(rightSpeed, OUTPUT);
  pinMode(leftSpeed, OUTPUT);
  
  Serial.begin(9600);
  
}

void loop() {

inputs();
delay(500);
while((analogRead(IR2) < IR2Threshold) && (analogRead(IR1) < IR1Threshold) && (analogRead(IR3) > IR3Threshold)){
  driveLine();
}

rotateLeftLine();

}

void inputs(){
  Serial.print(analogRead(IR1));
  Serial.print("\n");
  /*Serial.print(analogRead(IR2));
  Serial.print("\n");
  Serial.print(analogRead(IR3));
  Serial.print("\n");
  */
}

void driveLine(){
  
    digitalWrite(leftDirection, HIGH);
    digitalWrite(rightDirection, HIGH);

    analogWrite(leftSpeed, 200);
    analogWrite(rightSpeed, 200); 
  }


void rotateLeftLine(){
  while(analogRead(IR1) < IR1Threshold){
    digitalWrite(leftDirection, LOW);
    digitalWrite(rightDirection, HIGH);

    analogWrite(leftSpeed, 200);
    analogWrite(rightSpeed, 200);    
  }

    digitalWrite(leftSpeed, LOW);
    digitalWrite(rightSpeed, LOW);
}
