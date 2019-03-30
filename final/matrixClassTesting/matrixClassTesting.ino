//Functional testing with the Matrix header files and using objects

#include "matrix.h"
#include "robot.h"

void setup(){
//initialize matrix object 
matrix matrixComp = new matrix();
robot myRobot = new robot();
Serial.begin(9600);
//

}

void loop(){
	robot.print();
	delay(500);
}