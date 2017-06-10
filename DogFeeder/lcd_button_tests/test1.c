/*
Array to hold text
Array to hold cursor location
*/
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// constants won't change. They're used here to
// set pin numbers:
const int button_up = 13;     // the number of the pushbutton pin
const int button_down = 10;     // the number of the pushbutton pin
const int button_select = 9;     // the number of the pushbutton pin


//Text for main menu screen
const char * main_menu [] = {"Time", "Options", "Back"};

//Location of cursor in given menu
int cursor_loc = 0;

//Number of rows that can fit on the LCD screen
int rows = 2;

//Number of cols that can fit on the LCD screen
int cols = 13;

const char * selected = "X";
const char * unselected = " ";

int main (void)
{
  init();
  setup();
}

void setup()
{
    //initialize the pushbutton pin as an input:
    pinMode(button_up, INPUT);
    pinMode(button_down, INPUT);
    pinMode(button_select, INPUT);
    
    //set initial LCD screen size
    lcd.begin(cols, rows);

    //display the main menu
    display_text(main_menu, -1);

    control_loop(main_menu);
}

void control_loop (const char * menu[])
{
    //get number of rows
    size_t num_of_rows = sizeof(menu) / sizeof(menu[0]);
    
    int reading_up = LOW;
    int reading_down = LOW;
    int reading_select = LOW;

    int cursor_pos = 0;

    lcd.setCursor(0,cursor_pos);
    lcd.print(selected);
    
    while (1)
    {
        reading_up = digitalRead(button_up);
        reading_down = digitalRead(button_down);
        reading_select = digitalRead(button_select);
            
        if (reading_up == HIGH)
        {
            lcd.setCursor(0,cursor_pos);
            lcd.print(selected);
          
            lcd.setCursor(0,cursor_pos + 1);
            lcd.print(unselected);
        }
        
        if (reading_down == HIGH)
        {
          /*
           * from top to next
           * keep at bottom
           * until last element in the menu has been reached
           * then make the second to last element the top
           * and when down is pressed, make it bottom. 
          */
            cursor_pos = cursor_pos + 1;
            
            if (cursor_pos == 1)
            {
              lcd.setCursor(0, cursor_pos - 1);
              lcd.print(unselected);
          
              lcd.setCursor(0, cursor_pos);
              lcd.print(selected); 
            }   
            else
            {
              display_text(menu, 1);
              cursor_pos = 0;

              lcd.setCursor(0, cursor_pos);
              lcd.print(selected);
            
              lcd.setCursor(0, cursor_pos + 1);
              lcd.print(unselected);            
            }
        }
    };
}

void display_text(const char * text[], int array_pos)
{
    //if no specified row/col, use default (rows = 2, cols = 13)
    if (array_pos == -1)
        array_pos = 0;

    //clear old menu
    //lcd.clear();
    const char * clear_line = "            ";
    
    for (int i = 0; i < rows; i++)
    {
        //set the cursor to begin writing from the beginning of the col
        //and depending on i, which row to start on
        lcd.setCursor(1, i);
        
        //display the text on the i-th row
        lcd.print(clear_line);
    }
    
    //print out the given text for the LCD size
    for (int i = 0; i < rows; i++, array_pos++)
    {
        //set the cursor to begin writing from the beginning of the col
        //and depending on i, which row to start on
        lcd.setCursor(1, i);
        
        //display the text on the i-th row
        lcd.print(text[array_pos]);
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