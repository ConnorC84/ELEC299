/* fail safe for the IR sensor reading that 
  causes the robot to enter the movement sections of the code. The goal 
  is to determine the initial location of the robot by having the robot 
  turn left then right. If the robot is in the first slot it will read false
  then true, if in pos2 it will read true, true, pos3 true false*/


//this array will be used to checkt the position of robot
 bool i,j;
 bool pos[2] = [i, j]; 


//irNotWorkingDefinition
bool irNotWorking(){
    do
    {
        turnLeft(); 
            if(digitalRead(frontIR) == HIGH){ 
                i = true;
            }
            else{
                i = false;
            }
          turnRight();
            if(digitalRead(frontIR) == HIGH){
                j = true; 
            }
            else{
                j = false; 
            }
        return pos;
    }  

    //enter the loop to go 
    if (pos = [true, true]){
        completeRoute(2); //middle start S2
    }
    if(pos = [false, true];){ //left start S1
        completeRoute(1);
    }
    else{
        completeRoute(3); //right start S3
    }
}

void main (){
//calls function if value from IR Sesnsor is not the ASCI character for 
int val;
int count;

   
    
        if (val < 48 || val > 50){ //check for invalid character reading 
            count++;
        }
        if(count == 10){
            irNotWorking(); //if the incorrect value is read more 10 times assume IR is not working 
        }
         while(count < 10 ){
         delay(300);
         }

}
