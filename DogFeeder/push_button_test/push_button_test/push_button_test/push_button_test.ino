/*
Button

Turns on and off a light emitting diode(LED) connected to digital
pin 13, when pressing a pushbutton attached to pin 2.


The circuit:
* LED attached from pin 13 to ground
* pushbutton attached to pin 6 from +5V
* 10K resistor attached to pin 6 from ground

* Note: on most Arduinos there is already an LED on the board
attached to pin 13.


created 2005
by DojoDave <http://www.0j0.org>
modified 30 Aug 2011
by Tom Igoe

This example code is in the public domain.

http://www.arduino.cc/en/Tutorial/Button
*/

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin0 = 6;     // the number of the pushbutton pin
const int buttonPin1 = 7;     // the number of the pushbutton pin
const int buttonPin2 = 8;     // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

							// variables will change:
int ledState = HIGH;
int buttonState0 = 0;         // variable for reading the pushbutton status
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status
int lastButtonState = LOW;

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;	//the last time the ouput pin was toggled
unsigned long debounceDelay = 50;	//the debound time; increase if the output flickers

void setup() {
	// initialize the LED pin as an output:
	pinMode(ledPin, OUTPUT);

	// initialize the pushbutton pin as an input:
	pinMode(buttonPin0, INPUT);
	pinMode(buttonPin1, INPUT);
	pinMode(buttonPin2, INPUT);

	//set initial LED state
	digitalWrite(ledPin, ledState);
}

void loop() {
	// read the state of the pushbutton value:
	int reading = digitalRead(buttonPin0);

	//If the switch changed, due to noise or pressing:
	if (reading != lastButtonState)
	{
		//reset the debounce timer
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay)
	{
		//whatever the reading is at, it's been there for longer
		//than the debounce delay, so take it as the acutal current state:
		
		if (reading == HIGH)
		{
			//turn LED on:
			digitalWrite(ledPin, HIGH);
		}
		else
		{
			//turn LED off:
			digitalWrite(ledPin, LOW);
		}
	}

	//save the reading. next time throught the loop,
	//it'll be the lastButtonState:
	lastButtonState = reading;
}