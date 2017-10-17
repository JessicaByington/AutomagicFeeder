/*
Name:    lcd_menu.ino
Created: 10/5/2017 2:42:26 PM
Author:  Jessica
*/

// Adaption of rkflyer's Adaptaion of the Simple LCD menu system writen by PolePole - Arduino forums

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
// include the library code for the LCD
#include <LiquidCrystal.h>
#include <stdlib.h>
#include "EEPROMAnything.h";
// initialize the interface pins for the LCD
// rs = 12, en = 11, d4 = 4, d5 = 4, d6 = 3, d7 = 2
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

typedef struct dispenser_config
{
  unsigned char am_hour;
  unsigned char am_minute;
  unsigned char pm_hour;
  unsigned char pm_minute;
  unsigned char amount;
} user_settings;
user_settings usr_set;

typedef struct menu_item_def
{
  int type;
  char *mtext;
} menu_item_type;

struct MenuT
{
  int menu_num;
  int num_items;
  menu_item_type *mline;
};

menu_item_type main_menu_items[] = {
  { 1, "Time" },
  { 2, "Feed Amount" },
  { 1, "Feed Time" },
  { 1, "System Time" },
  { -2, "Test" },
};

menu_item_type feed_amount_menu_items[] = {
  { -7, "Current" },
  { -3, "Increase" },
  { -4, "Decrease" },
  { -1, "Back" }
};

menu_item_type time_menu_items[] = {
  { -8, "Current" },
  { 3, "Hour" },
  { 4, "Minute" },
  { -1, "Back" }
};

menu_item_type hour_menu_items[] = {
  { -5, "00" },
  { -5, "01" },
  { -5, "02" },
  { -5, "03" },
  { -5, "04" },
  { -5, "05" },
  { -5, "06" },
  { -5, "07" },
  { -5, "08" },
  { -5, "09" },
  { -5, "10" },
  { -5, "11" },
  { -5, "12" },
  { -5, "13" },
  { -5, "14" },
  { -5, "15" },
  { -5, "16" },
  { -5, "17" },
  { -5, "18" },
  { -5, "19" },
  { -5, "20" },
  { -5, "21" },
  { -5, "22" },
  { -5, "23" },
  { -1, "Back" }
};

menu_item_type minute_menu_items[] = {
  { -6, "00" },
  { -6, "05" },
  { -6, "10" },
  { -6, "15" },
  { -6, "20" },
  { -6, "25" },
  { -6, "30" },
  { -6, "35" },
  { -6, "40" },
  { -6, "45" },
  { -6, "50" },
  { -6, "55" },
  { -1, "Back" }
};

struct MenuT main_menu = {
  0,
  5,
  main_menu_items
};

struct MenuT feed_menu = {
  2,
  4,
  feed_amount_menu_items
};

struct MenuT time_menu = {
  1,
  4,
  time_menu_items
};

struct MenuT hour_menu = {
  3,
  25,
  hour_menu_items
};

struct MenuT minute_menu = {
  4,
  13,
  minute_menu_items
};

//Define Various Variables used throughout
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
int buttonStateUp = 0;        // Initalise ButtonStates
int buttonStateDown = 0;
int buttonState;
tmElements_t tm;          // stores time from RTC

MenuT current = main_menu;      // keeps track of which menu is currently being displayed
MenuT prev = main_menu;       // remembers the last menu that was displayed so it can return to it
boolean feed_or_system_time = false;
boolean am_or_pm = false;

// constants for indicating whether cursor should be redrawn
#define MOVECURSOR 1 
#define MOVELIST 2  
// constants for the backlight pin
#define LCD_LIGHT_PIN A3

// constants for testing time for dispensing
const int HOUR = 20;
const int MIN = 20;

// Main setup routine
void setup()
{

  
  // read stored user settings from EEPROM
  EEPROM_readAnything(0, usr_set);

  // Start the display with the main menu 
  MenuT current_menu = main_menu;
  MenuT prev_menu = main_menu;

  // Setup stuff that needs to be done regardless of whether entering setup mode or not
  // set up the LCD's number of columns and rows: 
  lcd.begin(totalCols, totalRows); 

  // initialize the serial communications port:
  Serial.begin(9600);

  // Set Buttons as input for testing whether to enter setup mode
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(LCD_LIGHT_PIN, OUTPUT);

  // Read the Button States
  buttonStateUp = digitalRead(buttonUp);
  buttonStateDown = digitalRead(buttonDown);

  // start out with the lcd screen backlight turned on
  digitalWrite(LCD_LIGHT_PIN, HIGH);

  // Clear LCD on exit from setup routine
  lcd.clear();

  // Display the menu that the user chooses
  Menu_Select();
}

void Menu_Select()
{
  do
  {
    current = control_loop(current, prev);
    delay(500); /// not sure I need this
  } while (true);
}

struct MenuT control_loop(MenuT m_menu, MenuT & p_menu)
{
  byte topItemDisplayed = 0;  // stores menu item displayed at top of LCD screen
  byte cursorPosition = 0;  // where cursor is on screen, from 0 --> totalRows. 

                // redraw = 0  - don't redraw
                // redraw = 1 - redraw cursor
                // redraw = 2 - redraw list
  byte redraw = MOVELIST;     // triggers whether menu is redrawn after cursor move.
  byte i = 0;         // temp variable for loops.
  byte totalMenuItems = 0;    // a while loop below will set this to the # of menu items.

  // Get list of Menu Items
  menu_item_type * menuItems = m_menu.mline;
  // store how many items are in the list
  totalMenuItems = m_menu.num_items - 1;

  lcd.clear();              // clear the screen so we can paint the menu.

  boolean stillSelecting = true;      // set because user is still selecting.

  timeoutTime = millis() + menuTimeout; // set initial timeout limit. 

  // Run a loop while waiting for user to select menu option.
  do
  {
    // Call any other setup actions required and/or process anything else required whilst in 
    // setup mode as opposed to things setup regardless of setup mode


    // Call read buttons routine which analyzes buttons and gets a response. Default response is 0.  
    switch (read_buttons())
    {
      // Case responses depending on what is returned from read buttons routine
      case 1:  // 'UP' BUTTON PUSHED
      {
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
      }
      break;

      case 2:    // 'DOWN' BUTTON PUSHED
      {
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
      }
      break;

      case 4:  // SELECT BUTTON PUSHED
      {
        timeoutTime = millis() + menuTimeout;  // reset timeout timer
        int menu_pos = topItemDisplayed + cursorPosition;
        int index = m_menu.mline[menu_pos].type;

        Serial.println(index);

        if (m_menu.mline[menu_pos].mtext == "Feed Time")
          feed_or_system_time = true;
        else if (m_menu.mline[menu_pos].mtext == "System Time")
          feed_or_system_time = false;       

        if (index >= 0)
        {
          p_menu = m_menu;
          return (read_selection(index));
        }
        else if (index == -1)
        {
          if ((m_menu.menu_num >= 0) && (m_menu.menu_num <= 2))
          {
            p_menu = main_menu;
          }

          return p_menu;
        }
        else
        {
          switch (index)
          {
            case -2: // run test
            {
              Test();
            }
            break;

            case -3: // increase amount of food dispensed
            {
              Increase();
            }
            break;

            case -4: // decrease amount of food dispensed
            {
              Decrease();
            }
            break;

            case -5: // change the hour
            {
              
              Hour(m_menu.mline[menu_pos].mtext);
            }
            break;

            case -6: // change the minute
            {
              // pass in the text value that is displayed on the screen
              Minute(m_menu.mline[menu_pos].mtext);
            }
            break;

            case -7: // Display feed amount
            {
              Serial.println("Display Feed Amount");
              DisplayFeedAmount();
            }
            break;

            case -8: // Display feed/system time
            {
              Serial.println("Display Feed/System Time");
              DisplayTime();
            }
            break;

            default:
            break;
          }
        }
      }
      break;
    }

    //  checks if menu should be redrawn at all.
    switch (redraw)
    {
      // Only the cursor needs to be moved.
      case MOVECURSOR:
      {
        redraw = false;             // reset flag.
        if (cursorPosition > totalMenuItems)  // keeps cursor from moving beyond menu items.
          cursorPosition = totalMenuItems;
        for (i = 0; i < (totalRows); i++)
        {
          // loop through all of the lines on the LCD
          lcd.setCursor(0, i);
          lcd.print(" ");           // and erase the previously displayed cursor
          lcd.setCursor((totalCols - 1), i);
          lcd.print(" ");
        }

        lcd.setCursor(0, cursorPosition);   // go to LCD line where new cursor should be & 
                          // display it.
        lcd.print(">");
      }
      break;

      // the entire menu needs to be redrawn
      case MOVELIST:
      {
        redraw = MOVECURSOR;  // redraw cursor after clearing LCD and printing menu.
        lcd.clear();      // clear screen so it can be repainted.

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
        {
          // if menu has less items than LCD rows, display all available menu items.
          for (i = 0; i < totalMenuItems + 1; i++)
          {
            lcd.setCursor(1, i);
            lcd.print(menuItems[topItemDisplayed + i].mtext);
          }
        }
      }
      break;
    }

    if (timeoutTime < millis())
    {
      // user hasn't done anything in awhile
      //stillSelecting = false;  // tell loop to bail out.
      lcd.noDisplay();
      digitalWrite(LCD_LIGHT_PIN, LOW);

      // check the RTC to see if it is time for food to be dispensed
      if (CheckTimeLoop())
      {
        Dispense();
      }

    }
    else
    {
      lcd.display();
      digitalWrite(LCD_LIGHT_PIN, HIGH);
    }
  }

  while (stillSelecting == true);
}

boolean CheckTimeLoop()
{
  boolean is_time = false;
  RTC.read(tm);

  if ((tm.Hour == usr_set.am_hour) || (tm.Hour == usr_set.pm_hour))
  {
    if ((tm.Minute == usr_set.am_minute) || (tm.Minute == usr_set.pm_minute))
      is_time = true;
  }

  return is_time;
}

void Dispense()
{
  // dispense food and wait for the minute to be over to continute. 
  // will block user input for now
  Serial.println("Dispensing...");
  delay(60000);
}

void Test()
{
  Serial.println("Test");
}

void Increase()
{
  Serial.println("Increase");
}

void Decrease()
{
  Serial.println("Decrease");
}

void Hour(char * c_hour)
{
  unsigned char m_hour = atoi(c_hour);

  if (feed_or_system_time == true)
  {
    if ((m_hour < 12) && (m_hour >=0))
    {
      // am time change
      usr_set.am_hour = m_hour;
      am_or_pm = true;
    }
    else if ((m_hour >= 12) && (m_hour <= 23))
    {
      // pm time change
      usr_set.pm_hour = m_hour;
      am_or_pm = false;
    }
  }
  else
  {
    RTC.read(tm);   // reads the clock time
    // system time change
    tm.Hour = m_hour;
    RTC.write(tm); // writes and updates the clock time
  }
  
  EEPROM_writeAnything(0, usr_set); // update changes in EEPROM
  
  delay(1000); // prevents changing hour more than once by accident
}

void Minute(char * c_min)
{
  unsigned char m_min = atoi("00");
  Serial.println(m_min);

  if (feed_or_system_time == true)
  {
    if (am_or_pm == true)
    {
      // am time change
      usr_set.am_minute = m_min;
    }
    else if (am_or_pm == false)
    {
      // pm time change
      usr_set.pm_minute = m_min;
    }
  }
  else
  {
    RTC.read(tm);   // reads the clock time
    // system time change
    tm.Minute = m_min;
    RTC.write(tm); // writes and updates the clock time
  }
  
  EEPROM_writeAnything(0, usr_set); // update changes in EEPROM

  Serial.println(m_min);
  
  delay(1000); // prevents changing hour more than once by accident
}

void DisplayTime()
{
  /* 
   *  if feed time, show both feed times, and then return to menu
   *  if system time, show system time and then return to menu
   */
  if (feed_or_system_time == true)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    
    if (usr_set.am_hour < 10)
    {
      lcd.print(0);
      lcd.setCursor(1, 0);
    }
    lcd.print(usr_set.am_hour);
    
    lcd.setCursor(2, 0);
    lcd.print(":");
    lcd.setCursor(3, 0);

    if (usr_set.am_minute < 10)
    {
      lcd.print(0);
      lcd.setCursor(4, 0);
    }
    lcd.print(usr_set.am_minute);

    lcd.setCursor(0, 1);
    if (usr_set.pm_hour < 10)
    {
      lcd.print(0);
      lcd.setCursor(1, 1);
    }
    lcd.print(usr_set.pm_hour);
    lcd.setCursor(2, 1);
    lcd.print(":");
    lcd.setCursor(3, 1);

    if (usr_set.am_minute < 10)
    {
      lcd.print(0);
      lcd.setCursor(4, 1);
    }
    lcd.print(usr_set.pm_minute);
    
    delay(6000);
    lcd.clear();
  }
  else
  {
    RTC.read(tm);   // reads the clock time
    // system time change


    lcd.setCursor(0, 0);
    lcd.print(tm.Hour);
    lcd.setCursor(0, 2);
    lcd.write(':');
    lcd.setCursor(0, 3);
    lcd.print(tm.Minute);
    
    delay(6000);
    lcd.clear();
  }
}

void DisplayFeedAmount()
{
  
}

struct MenuT read_selection(int index)
{
  MenuT returndata;

  if (index == 0)
  {
    returndata = main_menu;
  }
  else if (index == 1)
  {
    returndata = time_menu;
  }
  else if (index == 2)
  {
    returndata = feed_menu;
  }
  else if (index == 3)
  {
    returndata = hour_menu;
  }
  else if (index == 4)
  {
    returndata = minute_menu;
  }

  return returndata;
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
  }

  return returndata; // this spits back to the function that calls it the variable returndata.
}
