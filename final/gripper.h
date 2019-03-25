//ball gripper functions

void pickUpDice() {
  int tempCoords[3];
  tempCoords[0] = botCoords[0];
  tempCoords[1] = botCoords[1];
  tempCoords[2] = botCoords[2];

  //todo react to buttons
  digitalWrite(rightDirection, HIGH);
  digitalWrite(leftDirection, HIGH);
  analogWrite(leftSpeed, forwardSpeed);
  analogWrite(rightSpeed, forwardSpeed);
  while (digitalRead(Bumpers) == 1) {

  }
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
  delay(300);

  digitalWrite(leftDirection, LOW);
  digitalWrite(rightDirection, LOW);
  analogWrite(leftSpeed, forwardSpeed);
  analogWrite(rightSpeed, forwardSpeed);
  delay(300);

  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
  servoArm.write(50); //move arm down

  digitalWrite(rightDirection, HIGH);
  digitalWrite(leftDirection, HIGH);
  analogWrite(leftSpeed, forwardSpeed);
  analogWrite(rightSpeed, forwardSpeed);
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
