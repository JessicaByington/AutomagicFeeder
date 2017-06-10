/*
 Name:		DCMotorTest.ino
 Created:	4/29/2017 3:12:33 PM
 Author:	Jess
*/

#define CW 7 //CW is defined as pin #7//

#define CCW 8 //CCW is defined as pin #8//

void setup() { //Setup runs once//

	pinMode(CW, OUTPUT); //Set CW as an output//

	pinMode(CCW, OUTPUT); //Set CCW as an output//

}

void loop() { //Loop runs forever//

	digitalWrite(CCW, HIGH); //Motor runs clockwise//

	delay(5000); //for 1 second//

	/*digitalWrite(CW, LOW); //Motor stops//

	digitalWrite(CCW, HIGH);//Motor runs counter-clockwise//

	delay(5000); //For 1 second//

	digitalWrite(CCW, LOW); //Motor stops//*/

}