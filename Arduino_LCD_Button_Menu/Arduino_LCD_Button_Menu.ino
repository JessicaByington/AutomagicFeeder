/*
 Name:    lcd_menu.ino
 Created: 10/5/2017 2:42:26 PM
 Author:  Commander
*/


/// Adaption of the Simple LCD menu system writen by PolePole - Arduino forums

//I use the 3 wire LCD driver using an 74HC595 chip - Very good at saving pins http://www.stephenhobley.com/blog/2011/02/07/controlling-an-lcd-with-just-3-pins/
// If you use the conventional LiquidCrystal.h comment out the lines with 595 in them and uncomment the conventional ones, of course setting the pin numbers as required


// include the library code for the LCD
#include <LiquidCrystal.h>
//#include <LiquidCrystal595.h>  

// initialize the interface pins for the LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//LiquidCrystal595 lcd(2,3,4);     // datapin, latchpin, clockpin

typedef struct menu_item_def
{
  char *mtext;
} menu_item_type;

struct MenuT
{
  int num_items;
  menu_item_type *mline;
};

menu_item_type main_menu_items[] = {
  { "Time" },
  { "Options" },
};

menu_item_type option_menu_items[] = {
  { "Feed Amount" },
  { "Feed Time" },
  { "System Time" },
  { "Test" },
  { "Back" }
};

menu_item_type feed_amount_menu_items[] = {
  { "Current: " },
  { "Increase" },
  { "Decrease" },
  { "Back" }
};

menu_item_type time_menu_items[] = {
  { "Current: " },
  { "Hour" },
  { "Minute" },
  { "AM/PM" },
  { "Back" }
};

menu_item_type hour_menu_items[] = {
  { "1" },
  { "2" },
  { "3" },
  { "4" },
  { "5" },
  { "6" },
  { "7" },
  { "8" },
  { "9" },
  { "10" },
  { "11" },
  { "12" },
  { "Back" }
};

menu_item_type minute_menu_items[] = {
  { "00" },
  { "05" },
  { "10" },
  { "15" },
  { "20" },
  { "25" },
  { "30" },
  { "35" },
  { "40" },
  { "45" },
  { "50" },
  { "55" },
  { "Back" }
};

menu_item_type period_menu_items[] = {
  { "AM" },
  { "PM" },
  { "Back" }
};

struct MenuT main_menu = {
  2,
  main_menu_items
};

struct MenuT option_menu = {
  5,
  option_menu_items
};

struct MenuT feed_menu = {
  4,
  feed_amount_menu_items
};

struct MenuT time_menu = {
  5,
  time_menu_items
};

struct MenuT hour_menu = {
  13,
  hour_menu_items
};

struct MenuT minute_menu = {
  13,
  minute_menu_items
};

struct MenuT period_menu = {
  3,
  period_menu_items
};

//Define Various Variables used throughout
int timer = 0;
byte totalRows = 2;         // total rows of LCD
byte totalCols = 16;        // total columns of LCD
int returndata = 0;         // Used for return of button presses
unsigned long timeoutTime = 0;    // this is set and compared to millis to see when the user last did something.
const int menuTimeout = 20000;    // time to timeout in a menu when user doesn't do anything.
unsigned long lastButtonPressed;  // this is when the last button was pressed. It's used to debounce.
const int debounceTime = 150;   // this is the debounce and hold delay. Otherwise, you will FLY through the menu by touching the button. 
const int buttonUp = 13;      // Set pin for UP Button
const int buttonDown = 10;      // Set pin for DOWN Button
const int buttonSelect = 9;     // Set pin for SLELECT Button
                  //const int buttonCancel = 12;       // Set pun for CANCEL Button (No currently used)
int buttonStateUp = 0;        // Initalise ButtonStates
int buttonStateDown = 0;
int buttonState;
int count = 0;            // Temp variable for void demo

// constants for indicating whether cursor should be redrawn
#define MOVECURSOR 1 
#define MOVELIST 2  

// Main setup routine
void setup()
{
  // Setup stuff that needs to be done regardless of whether entering setup mode or not
  // set up the LCD's number of columns and rows: 
  lcd.begin(totalCols, totalRows);

  // Turn on the LCD Backlight
  //lcd.setLED1Pin(1);      

  // initialize the serial communications port:
  Serial.begin(9600);

  // Set Buttons as input for testing whether to enter setup mode
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);


  // Read the Button States
  buttonStateUp = digitalRead(buttonUp);
  buttonStateDown = digitalRead(buttonDown);

  //End of Void Setup() 
  // Clear LCD on exit from setup routine
  lcd.clear();
}

void loop()
{

  // Put your Main code in here


  // If you want your main code to be the menu system then copy the contents of void setup into here
  // If you do copy void setup copy (then delete) it from the section '// Start Setup Mode if both up and down buttons pressed together' onwards
  // Delete the line      'if (buttonStateUp == HIGH && buttonStateDown == HIGH) { '
  // Also make sure you delete the last '}' from the end of the void setup, Labeled 'End of Start Setup mode if'
  // When your routine runs it will now go straight into the main menu function
  // Start Setup Mode if both up and down buttons pressed together

  byte topItemDisplayed = 0;    // stores menu item displayed at top of LCD screen
  byte cursorPosition = 0;    // where cursor is on screen, from 0 --> totalRows. 

                  // redraw = 0  - don't redraw
                  // redraw = 1 - redraw cursor
                  // redraw = 2 - redraw list
  byte redraw = MOVELIST;     // triggers whether menu is redrawn after cursor move.
  byte i = 0;           // temp variable for loops.
  byte totalMenuItems = 0;    //a while loop below will set this to the # of menu items.

  // Create list of Menu Items
  menu_item_type * menuItems = main_menu.mline;
  // store how many items are in the list
  totalMenuItems = main_menu.num_items - 1;
  
  lcd.clear();  // clear the screen so we can paint the menu.

  boolean stillSelecting = true;  // set because user is still selecting.

  timeoutTime = millis() + menuTimeout; // set initial timeout limit. 

  do   // Run a loop while waiting for user to select menu option.
  {
    // Call any other setup actions required and/or process anything else required whilst in 
    // setup mode as opposed to things setup regardless of setup mode


    // Call read buttons routine which analyzes buttons and gets a response. Default response is 0.  
    switch (read_buttons())
    {
      // Case responses depending on what is returned from read buttons routine
      case 1:  // 'UP' BUTTON PUSHED
        timeoutTime = millis() + menuTimeout;  // reset timeout timer
                             //  if cursor is at top and menu is NOT at top
                             //  move menu up one.
        if (cursorPosition == 0 && topItemDisplayed > 0)  //  Cursor is at top of LCD, and there are higher menu items still to be displayed.
        {
          topItemDisplayed--;  // move top menu item displayed up one. 
          redraw = MOVELIST;  // redraw the entire menu
        }

        // if cursor not at top, move it up one.
        if (cursorPosition > 0)
        {
          cursorPosition--;  // move cursor up one.
          redraw = MOVECURSOR;  // redraw just cursor.
        }
        break;

      case 2:    // 'DOWN' BUTTON PUSHED
        timeoutTime = millis() + menuTimeout;  // reset timeout timer
                             // this sees if there are menu items below the bottom of the LCD screen & sees if cursor is at bottom of LCD 
        if ((topItemDisplayed + (totalRows - 1)) < totalMenuItems && cursorPosition == (totalRows - 1))
        {
          topItemDisplayed++;  // move menu down one
          redraw = MOVELIST;  // redraw entire menu
        }

        if (cursorPosition < (totalRows - 1))  // cursor is not at bottom of LCD, so move it down one.
        {
          cursorPosition++;  // move cursor down one
          redraw = MOVECURSOR;  // redraw just cursor.
        }
        break;

      case 4:  // SELECT BUTTON PUSHED
        timeoutTime = millis() + menuTimeout;  // reset timeout timer

        switch (topItemDisplayed + cursorPosition) // adding these values together = where on menuItems cursor is.
        {
          case 0:  // menu item 1 selected          
            lcd.clear();
            lcd.print("Run Item1 code");
            lcd.setCursor(0, 1);
            lcd.print("from here");
            Serial.print("Menu item ");
            Serial.print(topItemDisplayed + cursorPosition);
            Serial.print(" selected - ");
            Serial.println(menuItems[topItemDisplayed + cursorPosition].mtext);
            delay(2000);
            stillSelecting = false;
            break;
        }
        break;

      //case 8:  //  CANCEL BUTTON PUSHED - Not currently used
      //  stillSelecting = false;
      //  Serial.println("Button held for a long time");
      //  break;
    }

    //  checks if menu should be redrawn at all.
    switch (redraw)
    {
      case MOVECURSOR:  // Only the cursor needs to be moved.
        redraw = false;  // reset flag.
        if (cursorPosition > totalMenuItems) // keeps cursor from moving beyond menu items.
          cursorPosition = totalMenuItems;
        for (i = 0; i < (totalRows); i++)
        {  // loop through all of the lines on the LCD
          lcd.setCursor(0, i);
          lcd.print(" ");                      // and erase the previously displayed cursor
          lcd.setCursor((totalCols - 1), i);
          lcd.print(" ");
        }

        lcd.setCursor(0, cursorPosition);      // go to LCD line where new cursor should be & display it.
        lcd.print(">");
        //lcd.setCursor((totalCols - 1), cursorPosition);
        //lcd.print("<");
        break;  // MOVECURSOR break.

      case MOVELIST:  // the entire menu needs to be redrawn
        redraw = MOVECURSOR;  // redraw cursor after clearing LCD and printing menu.
        lcd.clear(); // clear screen so it can be repainted.

        if (totalMenuItems > ((totalRows - 1)))
        {
          // if there are more menu items than LCD rows, then cycle through menu items.
          for (i = 0; i < (totalRows); i++)
          {
            lcd.setCursor(1, i);
            lcd.print(menuItems[topItemDisplayed + i].mtext);
          }
        }
        else
        {  // if menu has less items than LCD rows, display all available menu items.
          for (i = 0; i < totalMenuItems + 1; i++)
          {
            lcd.setCursor(1, i);
            lcd.print(menuItems[topItemDisplayed + i].mtext);
          }
        }

        break;  // MOVELIST break
      }

    if (timeoutTime < millis())
    {
      // user hasn't done anything in awhile
      stillSelecting = false;  // tell loop to bail out.
    }
  }


  while (stillSelecting == true);

  //End of Start Setup mode if

  /*lcd.clear();
  lcd.print("now void loop");
  lcd.setCursor(0, 1);
  lcd.print(count);
  count = count + 1;
  delay(500);*/
}

// This routine reads the buttons, applys the debounce and returns the result to the calling routine
int read_buttons()
{
  // you may need to swap "void" with "int" or "byte"
  int returndata = 0;

  // see if it's time to check the buttons again
  if ((lastButtonPressed + debounceTime) < millis())
  {
    // read Up button
    buttonState = digitalRead(buttonUp);

    if (buttonState == HIGH)
    {
      returndata = returndata + 1;
      lastButtonPressed = millis();
    }

    // read Down button
    buttonState = digitalRead(buttonDown);

    if (buttonState == HIGH)
    {
      returndata = returndata + 2;
      lastButtonPressed = millis();
    }

    // read Select button
    buttonState = digitalRead(buttonSelect);
    if (buttonState == HIGH)
    {
      returndata = returndata + 4;
      lastButtonPressed = millis();
    }

    // read Cancel button - Not used at present
    //buttonState = digitalRead(buttonCancel);
    //if (buttonState == HIGH){
    //  returndata = returndata + 8;
    //  lastButtonPressed = millis();
    //}
  }

  return returndata; // this spits back to the function that calls it the variable returndata.
}

