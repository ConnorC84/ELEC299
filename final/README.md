SETUP

 the Pin layout is in the main file

The IR Beacon uses a low pass filter to recieve a character. This is the indicator of what starting position the robot is in. 
The bumpers is wired with an encoder circuit, allowing both buttons to be read through a voltage divider reducing it to 1 pin. 

Updates to be made before monday:
	- convert the functions in the working code / base code to the headerfiles 
	- update all pins to match our layout across files 
	- see if we can use the bumpers as interrupt pins
	- line tracking functionality , more research into the ArduinoRobot built in library on this ( We can still their function)
	- Build a PID system in to the motors and line tracking function 
	
	- get encoders to work? 