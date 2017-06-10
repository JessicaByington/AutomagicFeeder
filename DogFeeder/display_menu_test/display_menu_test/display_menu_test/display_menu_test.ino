/*
LiquidCrystal Library - Hello World

Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
library works with all LCD displays that are compatible with the
Hitachi HD44780 driver. There are many of them out there, and you
can usually tell them by the 16-pin interface.

This sketch prints "Hello World!" to the LCD
and shows the time.

The circuit:
* LCD RS pin to digital pin 12
* LCD Enable pin to digital pin 11
* LCD D4 pin to digital pin 5
* LCD D5 pin to digital pin 4
* LCD D6 pin to digital pin 3
* LCD D7 pin to digital pin 2
* LCD R/W pin to ground
* LCD VSS pin to ground
* LCD VCC pin to 5V
* 10K resistor:
* ends to +5V and ground
* wiper to LCD VO pin (pin 3)

Library originally added 18 Apr 2008
by David A. Mellis
library modified 5 Jul 2009
by Limor Fried (http://www.ladyada.net)
example added 9 Jul 2009
by Tom Igoe
modified 22 Nov 2010
by Tom Igoe

This example code is in the public domain.

http://www.arduino.cc/en/Tutorial/LiquidCrystal
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//I guess this is the part where I check the curser position
/*void main(int argc, char * argv[])
{

}*/

int main(void)
{
	init();
	setup();

	printf("This was a trium ph");

	const char * msgs[2] = { "This was a trium", "ph" };

	text_display(16, 2, msgs);
}

void setup() 
{
}

/*-text display
-message display
-update screen
*/
void text_display(int cols, int rows, const char * msgs[])
{
	//get number of rows
	size_t num_of_rows = sizeof(msgs) / sizeof(msgs[0]);

	// set up the LCD's number of columns and rows:
	lcd.begin(cols, rows);

	//print out the messages
	for (int i = 0; i <= num_of_rows; i++)
	{
		//set the cursor to the correct column and row for message
		//(rows and columns are 0 
		lcd.setCursor(0, i);
		// Print a message to the LCD.
		lcd.print(msgs[i]);
	}
}