void findLine(){
  //backtrack and find the line code
  int L = analogRead(leftIR);
  int C = analogRead(centreIR);
  int R = analogRead(rightIR);
  bool noLine = true;                                       //check if it detects a line on any of the 3 sensors
  digitalWrite(leftDirection, LOW);                         // set direction backwards
  digitalWrite(rightDirection, LOW);
  do{
    if (L < IRThresh && C < IRThresh && R < IRThresh)       //once it finds the line, exit the loop and execute follow line code
    {
      analogWrite(leftSpeed, forwardSpeedLeft);
      analogWrite(rightSpeed, forwardSpeedRight); 
    }
    else {
      noLine=false;
    }
  }while (noLine);

  followLine();
}
