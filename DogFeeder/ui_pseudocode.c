#include <stdio.h> //i/o
#include <Wire.h> //for I2C comm
#include <TimeLib.h> //For time
#include <DS1307RTC.h> //For RTC. *Might need to change this lib
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//System time variable
time_t system_time = now();

/****************************************************
* Menu set up:
*	Ideal menu sting lengths will not be longer than
*	16 characters long(columns), the max width of 
*	the screen.
*
*	2 rows is the max rows displayed at any time
*****************************************************/
//This struct is used to display the system time 
//as the main menu screen, allowing both time_t
//variables as well as char * vars to be displayed
struct
{
	enum { is_time, is_char_ptr } type;
	union 
	{
		time_t t_val;
		const char * cptr_val;
	} val;
} main_menu[2];

//Set main menu vars
main_menu[0].type = is_time;			//set the type as 'time_t; for storing the system time
main_menu[0].val.t_val = system_time;	//set the value to be stored as the system_time varible
main_menu[1].type = is_char_ptr;		//set the type as const char *; for storing the 2nd menu option
main_menu[1].val.cptr_val = "Options";	//set the value to be stored as a cstring "Options" 

//Options menu array
const char * options_menu [] = {"Feed Amount", "Feed Time", "System Time", "Test", "Back"};

//Feed Data menu array
const char * feed_data_menu [] = {"Morning", "Evening", "Back"};

//Feed Amount submenu array
const char * feed_amount_menu [] = {};

//set button pin numbers:
const int button_pin_up = 6;
const int button_pin_down = 7;
const int button_pin_select = 8;

/*
Struct to store elements of time
*/
struct Time
{
	//Morning feeding time

	//Evening feeing time
} time;

/*-food status struct
	-number of "steps"
	-feeding time
*/
struct Food_Status
{
	int steps;
		
} food_status;

/*-System status struct
	-hour
	-minute
	-daylight savings
*/
/*-Button enum struct
	-pin to up
	-pin to down
	-pin to select
*/

int main (int argc, char argv[])
{

}

/*-setup*/
void setup()
{
	//initialize the push button pins as input
	pinMode(button_pin_up, INPUT);
	pinMode(button_pin_down, INPUT);
	pinMode(button_pin_select, INPUT);
}

/*-user input
	-handle button presses
Purpose: Used to get user input from buttons 
*/
void user_input()
{
	button_state_up = digitalRead(button_pin_up);
	button_state_down = digitalRead(button_pin_down);
	button_state_select = digitalRead(button_pin_select);
	
	//sends states to button_input to have action executed
	button_input();
}

/*-time
	//24 hour clock
	-daylight savings
	-button input up: increase hour/minutes
	-button input down: decrease hour/minutes
*/
void time()
{
	//display menu
	text_display(16, 3, main_time_menu);
}

/*-button input (button enum struct)
	-up
	-down
	-select
Purpose: Used handle user input from buttons
*/
void button_input()
{
	if (button_state_up == HIGH)
	{
		//send message to sceen that the curser has moved up
	}
	else //assuming LOW
	if (button_pin_down == HIGH)
	{
		//send message to screen that the curser has moved down
	}
	else //assuming LOW
	{}
	if (button_pin_select == HIGH)
	{
		//send message to screen that item was selected
		//possible actions:
		//	-New menu to be displayed
		//	-variable has changed and needs to be updated
		// 	-screen needs to be redrawn accordingly, and curser updated
	}
	else //assuming LOW
	{}
}

/*-text display
	-message display
	-update screen
*/
void text_display(int cols, int rows, const char * msgs [])
{
	//get number of rows
	size_t num_of_rows = sizeof(msgs) / sizeof(msgs[0]);

	// set up the LCD's number of columns and rows:
	lcd.begin(cols, rows);
	
	//print out the messages
	for (int i = 0; i < msgs.length; i++)
	{
		//set the cursor to the correct column and row for message
		//(rows and columns are 0 
		lcd.setCursor(0, i);
		// Print a message to the LCD.
		lcd.print(msg[i]);
	}
}

/*-menu
	-dispense amount (food status struct)
		-increase
		-decrease
	-dispense time (food status struct)
		-hour +/-
		-minute +/-
	-change system time (system status struct)
		-hour +/-
		-minute +/-
	-test
		-dispense amount 
		-run
*/
void main_menu()
{
	//switch based on user input
	int input = 0;

	switch:
	//User selects dispense amount menu
	case 0:
		//handle changing food amount;
		time();
		break;
	//User selects dispense time menu
	case 1:
		//handle changing dispense time;
		break;
	//User selects change system time
	case 2: 
		//handle changing system time
		break;
	//User selects test mode
	case 3:
		//handle changing to test menu
		break:
	default:
		//invaild/no input, reprompt.

}

/*-motor
	-direction
	-length of steps
	-on
	-off
Purpose: Should call the motor file to handle motor functions
*/
void motor()
{
	
}

/*-run
	-check time for dispense
	-dispense 
	-be able to be interupted for user input 
*/
void run()
{
	
}

/*-sound?*/
void sound()
{
	
}
