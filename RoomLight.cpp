/*##########################################*/
//Define false if you want to turn off debug (writes to Serial Monitor)
#ifndef DEBUG
#define DEBUG false
#endif
// Change this if the lights don't respond correctly
#define PULSE_DELAY 10
/*##########################################*/

#include "RoomLight.h"

// Bank addresses as Int used to create patterns
int lightBanks[] = {
  // Int converts to binary on shift-in
	0, //All off
	16, //H8 Bank on 00010000
	32, //H6 Bank on 00100000
	64, //H5 Bank on 01000000
	128, //H4 Bank on 10000000
	240 //ALL ON
};

//RoomLight::RoomLight(int dataPin, int clockPin, int latchPin, int clearPin, int num_connected) {
RoomLight::RoomLight(int num_connected) {
	// Main constructor

	this->num_connected = num_connected;
}

void RoomLight::TPICBegin() {
	// Setup
	pinMode(this->dataPin, OUTPUT);
	pinMode(this->clockPin, OUTPUT);
	pinMode(this->latchPin, OUTPUT);
	pinMode(this->clearPin, OUTPUT);

#if DEBUG
	Serial.begin(9600);
#endif
}

void RoomLight::firstRun() {
	if(this->runOnce == true) {
		// Clear all pins
		writeShift(0);
		// Don't run again
	  this->runOnce = false;
	}
}

void RoomLight::writeShift(int b) {
	shiftOut(dataPin, clockPin, LSBFIRST, b);
	pulsePin();
#if DEBUG
	Serial.print("writeShift(int) = ");
	Serial.println(b);
#endif
}

// UI Functions
void RoomLight::activeBank(int bank, int status) {

	bool isInArray = false;
	int positionInArray = 0;
	// Check if in array
	for (int i = 0; i < this->lengthOfActiveArray; i++){
		if (this->activeLightArray[i] == bank) {
			isInArray = true;
			positionInArray = i;
			break;
		}
	}

#if DEBUG
	Serial.print("isInArray = "); Serial.println(isInArray);
#endif

	if(isInArray && status == LOW) {
		// Turn of if it is and status is LOW
		for(int i = positionInArray; i < this->lengthOfActiveArray; i++){
			this->activeLightArray[i] = this->activeLightArray[i+1];
		}
		this->lengthOfActiveArray--;
	}else if(!isInArray && status == HIGH && this->lengthOfActiveArray != 0){
		// Add a light not in array and status is HIGH but active array length is > 0
		this->lengthOfActiveArray++;
		this->activeLightArray[this->lengthOfActiveArray-1] = bank;
	}else if(!isInArray && status == HIGH && this->lengthOfActiveArray == 0){
		// Add a light not in array and status is HIGH but active array length is 0
		this->lengthOfActiveArray++;
		this->activeLightArray[0] = bank;
	}
	// Turn on the light banks
	turnOnLightBanks();
}

void RoomLight::progressiveLightBanks() {
	// Switches on one light per event
	if(this->lengthOfActiveArray == 0){
		activeBank(H8, HIGH);
	}else if (this->lengthOfActiveArray == num_connected){
		for (int i = 0; i < num_connected; i++) {
			activeBank(i+1, LOW);
		}
	}else{
		activeBank(this->activeLightArray[this->lengthOfActiveArray-1] + 1, HIGH);
	}
}

void RoomLight::turnOnLightBanks() {
	int b = 0;
	// Compare the resulting values; saves writng the same value
	for (int i = 0; i < this->lengthOfActiveArray; i++){
		int bankToGet = this->activeLightArray[i];
		b += lightBanks[bankToGet];
	}
	writeShift(b);
}

void RoomLight::ledsFlash(int repeats, int speed) {
	// Flash the pins
	// Dirty hack to work out a cycle for on and off
	for (int i = 0; i < repeats; i++){
		writeShift(lightBanks[OFF]);
		delay(speed);
		writeShift(lightBanks[ALL]);
		delay(speed);
	}
}

void RoomLight::ledsDance(int repeats, int speed) {
	// Quick roll of the pins
	for (int i = 0; i < repeats; i++) {
		writeShift(OFF);
		for (int a = 0; a <= this->num_connected; a++){
			int randomNum = random(-1, 10);
			writeShift(lightBanks[randomNum]);
			delay(speed);
    }
  }
}

void RoomLight::connectButton(int p, int d, bool a) {
	deviceButton.pin_connected = p;
	deviceButton.debounce = d;
	deviceButton.isActive = a;
}

// Button for light activity
bool RoomLight::buttonIsPressed() {

	bool result = false;
	// Only responds if active so it can be programatically deactivated
	if (deviceButton.isActive) {
		reading = digitalRead(deviceButton.pin_connected);

		while(reading == LOW && isReset == true) {

	#if DEBUG
		// Serial.println(previousMillis);
		// Serial.println(currentMillis);
	#endif

			if((millis() - previousMillis) > 20){
				result = true;
				break;
			}
			reading = digitalRead(deviceButton.pin_connected);
		}

		if(reading != LOW) {
			previousMillis = millis();
			isReset = true;
		}else{
			isReset = false;
		}
	}
	return result;
}

// Utility
void RoomLight::pinClear() {
	// Turn all the lights off
	writeShift(0);
}

void RoomLight::pinDrains(int visibility) {
	//Turn the pins off or on

	if (visibility == HIGH){
	    digitalWrite(clearPin, LOW);
	  }else{
	    digitalWrite(clearPin, HIGH);
	  }
}

// Private

void RoomLight::pulsePin() {
	//pulse to transfer bits to the display register
    digitalWrite(latchPin, HIGH);
    delayMicroseconds(PULSE_DELAY);
    digitalWrite(latchPin, LOW);
}
