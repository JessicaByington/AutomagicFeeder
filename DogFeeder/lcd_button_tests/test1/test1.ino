/*
Array to hold text
Array to hold cursor location
*/
// include the library code:
#include <LiquidCrystal.h>

//struct for main menu
typedef struct
{
	int size;
	char * menu[2];
} Main_Menu;

//struct for option menus
typedef struct
{
	int size;
	char * menu[5];
} Option_Menu;

//struct for feed amount
typedef struct
{
	int size;
	char * menu[4];
} Feed_Amount_Menu;

//struct for time, will be used for changing feed and system time
typedef struct
{
	int size;
	char * menu[5];
} Time_Menu;

//struct for time, will be used for changing feed and system time
typedef struct
{
	int size;
	int hour_size;
	int minute_size;
	int period_size;
	char * hour[13];
	char * minute[12];
	char * period[2];
} Time_Change_Menu;

//struct to hold all other structs 
typedef struct
{
	int size;
	Main_Menu main_menu;
	Option_Menu option_menu;
	Feed_Amount_Menu feed_amount_menu;
	Time_Menu time_menu;
	Time_Change_Menu time_change_menu;
} Menu_Struct;

void control_loop(Menu_Struct menu_struct);
void display_text(char * text[], int, int, int);

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// constants won't change. They're used here to
// set pin numbers:
const int button_up = 13;     // the number of the pushbutton pin
const int button_down = 10;     // the number of the pushbutton pin
const int button_select = 9;     // the number of the pushbutton pin

//Variables for reading the pushbutton status
int lastButtonState_up = LOW;
int lastButtonState_down = LOW;
int lastButtonState_select = LOW;

//Location of cursor in given menu
int cursor_loc = 0;

//Number of ROWS that can fit on the LCD screen
const int ROWS = 2;

//Number of COLS that can fit on the LCD screen
const int COLS = 13;

//the last time the output pin was toggled
unsigned long lastDebounceTime_up = 0;
unsigned long lastDebounceTime_down = 0;
unsigned long lastDebounceTime_select = 0;
//the debounce time; increase if the output flickers
unsigned long debounceDelay = 100;


int main(void)
{
	init();
	setup();

	Menu_Struct menu_struct =
	{
		//Text for main menu screen
		.main_menu = { .size = 2,.menu = { "Time", "Options" } },

		//text for option menu
		.option_menu = { .size = 5,.menu = { "Feed amount", "Feed Time", "System Time", "Test", "Back" } },

		//text for feed amount menu
		.feed_amount_menu = { .size = 4,.menu = { "Current: ", "Increase", "Decrease", "Back" } },

		//text for menu for changing feed and system time
		.time_menu = { .size = 5,.menu = { "Current:", "Hour", "Minute", "AM/PM", "Back" } },

		//text for menu to change feed and system time
		.time_change_menu = { .size = 3,.hour_size = 13,.minute_size = 12,.period_size = 2,
		.hour = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12" },
		.minute = { "00", "05", "10", "15", "20", "25", "30", "35", "40", "45", "50", "55" },
		.period = { "AM","PM" } }
	};

	//starts program with the main menu being displayed
	control_loop(menu_struct);
}

void setup()
{
	//initialize the pushbutton pin as an input:
	pinMode(button_up, INPUT);
	pinMode(button_down, INPUT);
	pinMode(button_select, INPUT);

	////set initial LCD screen size
	//lcd.begin(COLS, ROWS);

	////display the main menu
	//display_text(main_menu, -1);

	//control_loop(main_menu);
}

void control_loop(Menu_Struct menu_struct)
{
	//Set menu and size variable to main menu
	//will change as different menus are selected
	char * menu[menu_struct.option_menu.size] = menu_struct.option_menu.menu[];
	int menu_size = menu_struct.option_menu.size;

	int reading_up = LOW;
	int reading_down = LOW;
	int reading_select = LOW;

	int selection = 0;

	while (1)
	{
		int reading_up = digitalRead(button_up);
		int reading_down = digitalRead(button_down);
		int reading_select = digitalRead(button_select);

		if (reading_up != lastButtonState_up)
		{
			lastDebounceTime_up = millis();
		}

		if (reading_down != lastButtonState_down)
		{
			lastDebounceTime_down = millis();
		}

		if (reading_select != lastButtonState_select)
		{
			lastDebounceTime_select = millis();
		}

		if ((millis() - lastDebounceTime_up) > debounceDelay)
		{
			//whatever the reading is at, it's been there for longer
			//than the debounce delay, so take it as the acutal current state
			if (reading_up == HIGH)
			{
				//Keeps the user from going out of bounds of the menu
				//and increaments where the user is at on the current menu
				if ((selection > 0) && (selection <= menu_size))
					selection--;

				int loc = selection;
				//display updated menu with desired item highlighted
				display_text(menu, selection, loc, menu_size);
			}

		}

		if ((millis() - lastDebounceTime_down) > debounceDelay)
		{
			//whatever the reading is at, it's been there for longer
			//than the debounce delay, so take it as the acutal current state
			if (reading_down == HIGH)
			{
				//Keeps the user from going out of bounds of the menu
				//and increaments where the user is at on the current menu
				if ((selection >= 0) && (selection < (menu_size - 1)))
					selection++;

				int loc = selection;
				//display updated menu with desired item highlighted
				display_text(menu, selection, loc, menu_size);
			}

		}

		lastButtonState_down = reading_down;
		lastButtonState_up = reading_up;
		lastButtonState_select = reading_select;
	};
}

void display_text(char * text[], int selection, int loc, int menu_size)
{
	int index = 0;
	int i = 0;
	int row = ROWS;

	//clear old menu
	//lcd.clear();
	const char * clear_line = "             ";

	//if the selection is not in the range of what the LCD screen can show,
	//rolls the top menu item out, the next menu item up, and print out the 
	//menu item below. 
	//EX: Top: Time, Next: Options, New: Back
	// Before menu roll:
	//  Time
	// XOptions
	//
	// After:
	//  Options
	// XBack
	if (selection > (ROWS - 1))
	{
		if (selection <= (menu_size - 1))
		{
			index = loc - 1;
			i = loc - 1;
			row = ROWS + (loc - 1);
		}
	}

	for (int j = 0; j < row; j++)
	{
		//set the cursor to begin writing from the beginning of the col
		//and depending on i, which row to start on
		lcd.setCursor(0, j);

		//display the text on the i-th row
		lcd.print(clear_line);
	}

	//print out the given text for the LCD size
	for (; i < row; i++, index++)
	{
		if (i == selection)
		{
			lcd.setCursor(0, i);
			lcd.print('X');
		}

		//set the cursor to begin writing from the beginning of the col
		//and depending on i, which row to start on
		lcd.setCursor(1, i);

		//display the text on the i-th row
		lcd.print(text[index]);
	}
}

/*
pass in pointer to array of text
display the first 2
store which two posisons are being displayed
if button up/down, increase/decrease what is displayed within bounds

part of setup show init the main menu to the display

when changing menus, clear out the cursor_loc and text_pos.
if button selects the last pos in the text array
clear cursor_loc and text_pos

*/