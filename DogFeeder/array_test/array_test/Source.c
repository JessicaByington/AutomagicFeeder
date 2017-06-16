#include <stdio.h>

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

int rows = 2;

int main(void)
{
	Menu_Struct menu_struct = 
	{
		//Text for main menu screen
		.main_menu = {.size = 2,.menu = {"Time", "Options"} },

		//text for option menu
		.option_menu = {.size = 5,.menu = {"Feed amount", "Feed Time", "System Time", "Test", "Back"}},

		//text for feed amount menu
		.feed_amount_menu = {.size = 4,.menu = {"Current: ", "Increase", "Decrease", "Back"}},

		//text for menu for changing feed and system time
		.time_menu = {.size = 5,.menu = {"Current:", "Hour", "Minute", "AM/PM", "Back"}},

		//text for menu to change feed and system time
		.time_change_menu = {.size = 3,.hour_size = 13,.minute_size = 12,.period_size = 2,
							 .hour = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12" },
							 .minute = { "00", "05", "10", "15", "20", "25", "30", "35", "40", "45", "50", "55" },
							 .period = {"AM","PM"}}
	};

	//starts program with the main menu being displayed
	control_loop(menu_struct);
}

void control_loop(Menu_Struct menu_struct)
{
	//Set menu and size variable to main menu
	//will change as different menus are selected
	char * menu = menu_struct.option_menu.menu;
	int menu_size = menu_struct.option_menu.size;

	char input = ' ';
	int selection = 0;

	//init display of main menu when program is first starting up.
	display_text(menu, 0, 0, menu_size);

	//Main control loop to direct the flow of menu selections
	while (1)
	{
		//Get input from user
		input = 0; //clers previous selection
		printf("up or down (u or d): "); //Display text prompt for user
		scanf(" %c", &input); //Read in selection from user

		//select the next menu item below current
		if (input == 'd')
		{
			//Keeps the user from going out of bounds of the menu
			//and increaments where the user is at on the current menu
			if ((selection >= 0) && (selection < (menu_size - 1)))
				selection++;

			int loc = selection;
			//display updated menu with desired item highlighted
			display_text(menu, selection, loc, menu_size);
		}

		//select the next menu item above current
		if (input == 'u')
		{
			//Keeps the user from going out of bounds of the menu
			//and increaments where the user is at on the current menu
			if ((selection > 0) && (selection <= menu_size))
				selection--;

			int loc = selection;
			//display updated menu with desired item highlighted
			display_text(menu, selection, loc, menu_size);
		}
	};
}

void display_text(char * text[], int selection, int loc, int menu_size)
{
	int index = 0;
	int i = 0;
	int row = rows;

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
	if (selection > (rows - 1)) 
	{
		if (selection <= (menu_size - 1))
		{
			index = loc - 1;
			i = loc - 1;
			row = rows + (loc - 1);
		}
	}

	for (; i < row; i++, index++)
	{
		//Print 'X' next to the currently selected menu item
		if (i == selection)
			printf("%c", 'X');
		
		//Display menu text
		printf("%s\n", text[index]);
		
	}
}