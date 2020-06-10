#include <RoomLight.h>

// Add constants for the pins connected to the Arduino
// from the pinball circuit
const int dataPin = 9;
const int clockPin = 5;
const int latchPin = 6;
const int pinStatus = 7;

// Number of banks connected
int num_connected = 8;

RoomLight my_lamp = RoomLight(dataPin, clockPin, latchPin, pinStatus, num_connected);

void setup() {
  // put your setup code here, to run once:
  my_lamp.TPICBegin();
}

void loop() {
  // put your main code here, to run repeatedly:
  my_lamp.firstRun();

	//Enter the code you want to respond to things as they happen

}