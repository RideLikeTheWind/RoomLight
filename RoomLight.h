#ifndef ROOM_LIGHT_H
#define ROOM_LIGHT_H

//Make life easier for the kids
const int OFF = 0;
const int H8 = 1;
const int H6 = 2;
const int H5 = 3;
const int H4 = 4;
const int ALL = 5;

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

//class AddButton header;
class AddButton {
	public:
		//Constructor
		AddButton(int pin=0, int debounce=100, bool isActive=true);

    int pin_connected;
		int debounce;
		bool isActive;
};

// class Simple Sensor header

// Sensor default types
const int Temperature = 1;
const int Light = 2;
const int Sound = 3;
// Sensor return types
const int Celsius = 1;
const int Fahrenheit = 2;
const int Brightness = 3;

class SimpleSensor {
  public:
    SimpleSensor(int pin=0, int type=Temperature, bool isActive=true, long int debounce=1000);

    //Functions
    // Returns the value of the sensor given the debounce
    // time provided in the initial setup
    // Pass in return type as Celsius, Fahrenheit or Light
    float readSensor(int returnType=0);

    int pin_connected;
    bool isActive;
    int debounce;
    int type;

  private:
    float readTemp(int returnType=Celsius, int p=0);
    float readLight(int p=0);
};

// RoomLight class
class RoomLight {
  public:

	  // Constructor
	  //RoomLight(int dataPin,int clockPin, int latchPin, int clearPin, int num_connected=4);
    RoomLight(int num_connected=4);

	  // Setup pins
	  void TPICBegin();

		void firstRun();
		/*
			This runs on first loop through
			Turns all lights on
			To do: Checks the switch is wired correctly
		*/

		//UI Functions

    void activeBank(int bank, int status);
    /*
	  	Turn on or off a specific bank
	  	bank = constant of names banks (H8/H6/H5/H4)
      status = constant name LOW/HIGH
	  */

    void progressiveLightBanks();
    /*
      Turn on a bank at a time, then reset at end
      Called on event
    */

	  void ledsFlash(int repeats, int speed);
	  /* BLOCKING
	  	Flashes all lights on and off
	  	repeats = number of flashes
	  	speed = delay in ms for lights to remain on (1000 = 1 second)
	  */

	  void ledsDance(int repeats = 10, int speed = 10);
	  /* BLOCKING
	  	Lights roll around quickly
	  	repeats = number of repeats you want (about 1 per second) | default = 10
	  	startPin = pin you want it to start from | default = 0
	  	speed = how fast you want them to dance (higher is slower) | default = 10, max = 5
	  */

		// Specific Circuit Functions
		bool buttonIsPressed();
    /*
      Function to return the value of a button if pressed or not
    */

    void connectButton(int pin=0, int debounce=100, bool isActive=true);
    /*
      Assign parameters for button connection
      pin = availble output pin
      debounce = delay to ensure button is actually pressed
      isActive = set to default as true (on) but can be turned off and on depending on user prefs
      variables are public so are mutable eg: my_button.pin = 1;
    */

		// Utility Functions

		// main write function
	  void writeShift(int bits);

    // Provides utility to higher-level activation functions
    void turnOnLightBanks();

		// Turn the pins off or on
	  void pinDrains(int visibility); //HIGH (ON) - LOW (OFF)

		// Clear all pins of output
	  void pinClear();

		int num_connected;

	private:
		// General
    // Add constants for the IC Pins
    int dataPin = 7;
    int clockPin = 6;
    int latchPin = 5;
    int clearPin = 8;

    // State comparative array
    int previousLightingArray[6];
    int previousLightingArrayLength = 6;
    int activeLightArray[6];
    int lengthOfActiveArray = 0;

    // Button object
    AddButton deviceButton = AddButton(1,100,false);

    // Button controllers
		long int previousMillis = 0;
		long int currentMillis;
		int reading = LOW;
		bool isReset = true;

		// Functions
		void pulsePin();

		// Do not change //
		bool runOnce = true;
    ///////////////////
};



#endif
