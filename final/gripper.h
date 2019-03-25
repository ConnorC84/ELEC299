//ball gripper functions

void pickUpDice() {
  int tempCoords[3];
  tempCoords[0] = botCoords[0];
  tempCoords[1] = botCoords[1];
  tempCoords[2] = botCoords[2];
//
  
  
  //todo react to buttons
  digitalWrite(RMotorDir, HIGH);
  digitalWrite(LMotorDir, HIGH);
  analogWrite(LMotorSpeed, forwardSpeed);
  analogWrite(RMotorSpeed, forwardSpeed);
  while (digitalRead(bumpers) == 1) {

  }
  analogWrite(LMotorSpeed, 0);
  analogWrite(RMotorSpeed, 0);
  delay(300);

  digitalWrite(RMotorDir, LOW);
  digitalWrite(LMotorDir, LOW);
  analogWrite(LMotorSpeed, forwardSpeed);
  analogWrite(RMotorSpeed, forwardSpeed);
  delay(300);

  analogWrite(LMotorSpeed, 0);
  analogWrite(RMotorSpeed, 0);
  servoArm.write(50); //move arm down

  digitalWrite(RMotorDir, HIGH);
  digitalWrite(LMotorDir, HIGH);
  analogWrite(LMotorSpeed, forwardSpeed);
  analogWrite(RMotorSpeed, forwardSpeed);
  delay(300);

  Grip();
  RotateBot(1);
  MoveForwardAlongLine(tempCoords);

  botCoords[2] = (tempCoords[2] + 2) % 4;
}


void Grip() {
  angle = 0;
  servoGrip.write(angle);
  grip = analogRead (gripperPad);

  while (grip < 500) {
    grip = analogRead (gripperPad);
    servoGrip.write(++angle);
    Serial.println(String(grip));
    delay(50);
  }
  return;
}
