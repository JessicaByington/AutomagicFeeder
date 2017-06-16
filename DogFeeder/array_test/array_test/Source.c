#include <stdio.h>

//template struct for all menus
typedef struct
{
	int size;
	char * menu[];
} Menu;

//Text for main menu screen
Menu main_menu = {.size = 3, .menu = { "Time", "Options", "Back" }};

//Test menu for testing larger menus
Menu test_menu = {.size = 7, .menu = {"one", "two", "three", "four", "five", "six", "seven"}};

void control_loop(struct Menu menu_struct);
void display_text(char * text[], int, int, int);

int rows = 2;

int main(void)
{
	control_loop(test_menu);
}

void control_loop(struct Menu menu_struct)
{
	char * menu = menu_struct.menu;
	int menu_size = menu_struct.menu;

	char input = ' ';

	display_text(menu, 0, 0, menu_size);

	int selection = 0;

	while (1)
	{
		
		input = 0;
		fflush(stdout);
		printf("up or down (u or d): ");
		scanf(" %c", &input);
		fflush(stdout);

		if (input == 'd')
		{
			if ((selection >= 0) && (selection < (menu_size - 1)))
				selection++;

			int loc = selection;
			display_text(menu, selection, loc, menu_size);
		}

		if (input == 'u')
		{
			if ((selection > 0) && (selection <= menu_size))
				selection--;

			int loc = selection;
			display_text(menu, selection, loc, menu_size);
		}
	};
}

void display_text(char * text[], int selection, int loc, int menu_size)
{
	int index = 0;
	int i = 0;
	int row = rows;

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
		if (i == selection)
			printf("%c", 'X');
		
		printf("%s\n", text[index]);
		
	}
}