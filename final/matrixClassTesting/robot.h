//robot header file

#ifndef robot.h
#define robot.h

class robot {
	public: //Public Interface
		void SETUP()
		void getStartingPosition();
		void rotate(int direction);
		
		void move(int direction, int bumperFlag);
		void stop();
		void accountForBattery();
		
		//Gripper functions
		void pickUp();
		void drop();
		bool grab(int angleToCheck);
		
		void print();
};

#endif
		