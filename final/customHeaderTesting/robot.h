//robot header file

#ifndef robot_h
#define robot_h

class robot {
	public: //Public Interface
		void begin();
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
		
