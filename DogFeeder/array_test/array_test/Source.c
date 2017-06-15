#include <stdio.h>

//Text for main menu screen
const char * main_menu[] = { "Time", "Options", "Back" };

void control_loop(char * menu[]);
void display_text(char * text[], int, int);

int rows = 2;

int main(void)
{
	char * main_menu[] = { "Time", "Options", "Back" };
	control_loop(main_menu);
}

void control_loop(char * menu[])
{
	char input = ' ';

	display_text(menu, 0, 0);

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
			if ((selection >= 0) && (selection < 2))
				selection++;

			int loc = selection;
			display_text(menu, selection, loc);
		}

		if (input == 'u')
		{
			if ((selection > 0) && (selection <= 3))
				selection--;

			int loc = selection;
			display_text(menu, selection, loc);
		}
	};
}

void display_text(char * text[], int selection, int loc)
{
	int index = 0;
	int i = 0;
	int row = rows;

	//int num_of_elem = (sizeof(text) / sizeof(*text));

	if (selection > (rows - 1)) 
	{
		if (selection <= (3 - 1))
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