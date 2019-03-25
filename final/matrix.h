// Path functionality for robot 

//Position Variables
int botCoords[] = {2, 0, 0}; // x and y postion, 0,1,2,3 Direction (N,E,S,W)
int nextTargetCoords[] = {0, 0};

int testpath[3][2] = {{2, 1}, {2, 2}, {1, 2}};  //test path for grip testing 

//int testpath[8][2] = {{0, 1}, {1, 1}, {1, 2}, {2, 2}, {2, 1}, {1, 1}, {1, 0}, {0, 0}};   //two interecting squares 
//int testpath[8][2] = {{0, 1}, {0, 0}, {1, 0}, {0, 0}, {0, -1}, {0, 0}, {-1, 0}, {0, 0}};  // a X 
//int testpath[14][2] = {{0, 1}, {0, 2}, {0, 3}, {1, 3}, {1, 2}, {1, 1}, {1, 0}, {2, 0}, {2, -1}, {1, -1}, {0, -1}, { -1, -1}, { -1, 0}, {0, 0}};  // a dick


//makes robot move to adjecent cordinate specified. 
//--this fucntion handles logic for rotation robot to right orientation (by calling RotateBot apropriatley)
//--calls MoveForwardAlong line to move forward to the next intersection  
//--updates robot position
void moveToCordinate(int cord[]) {
  //find the difrence betweenCordinates
  int cordDifrence[2] = {0, 0};
  cordDifrence[0] = cord[0] - botCoords[0];
  cordDifrence[1] = cord[1] - botCoords[1];
  //first check that the cordinates are adjecent and we can make it there
  if (abs(cordDifrence[0]) > 1 || abs(cordDifrence[1]) > 1) {
    Serial.println("this isnt a valid next cordinate, fatal error");
  }
  if (abs(cordDifrence[0]) > 0 && abs(cordDifrence[1]) > 0) {
    Serial.println("this isnt a valid next cordinate,fatal error");
  }
  int targetOrientation;
  if (cordDifrence[0] == 1) {
    targetOrientation = 1;
  }
  if (cordDifrence[0] == -1) {
    targetOrientation = 3;
  }
  if (cordDifrence[1] == 1) {
    targetOrientation = 0;
  }
  if (cordDifrence[1] == -1) {
    targetOrientation = 2;
  }
  Serial.println("target Oreintation is: " + String(targetOrientation));
  int neededRotation;
  neededRotation = targetOrientation - botCoords[2];
  Serial.println("Needed rotation is: " + String(neededRotation));
  if (neededRotation == -1 || neededRotation == 3) {
    Serial.println("Need to turn left");
    RotateBot(-1);
  }
  if (neededRotation == 1 || neededRotation == -3) {
    Serial.println("Need to turn right");
    RotateBot(1);
  }
  if (neededRotation == 2 || neededRotation == -2) {
    Serial.println("Need to turn 180");
    RotateBot(1);
    RotateBot(1);
  }
  if (neededRotation == 0) {
    Serial.println("Allready Facing the right way");
  }
  MoveForwardAlongLine(cord);
  botCoords[0] = cord[0];
  botCoords[1] = cord[1];
}
