/*##########################################*/
//Define false if you want to turn off debug (writes to Serial Monitor)
#ifndef DEBUG
#define DEBUG true
#endif

#define PULSE_DELAY 10
/*##########################################*/

#include "RoomLight.h"

int lightBanks[] = {
  // Int converts to binary on shift-in
	0, //All off
	16, //H8 Bank on 00010000
	32, //H6 Bank on 00100000
	64, //H5 Bank on 01000000
	128, //H4 Bank on 10000000
	240 //ALL ON
};

RoomLight::RoomLight(int dataPin, int clockPin, int latchPin, int clearPin, int num_connected) {
	// Main constructor

	this->dataPin = dataPin;
  this->clockPin = clockPin;
  this->latchPin = latchPin;
  this->clearPin = clearPin;

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

	for (int i = 0; i < this->lengthOfActiveArray; i++){
		Serial.print("activeBank activeLightArray loop = ");
		Serial.println(bank);
		if (this->activeLightArray[i] == bank) {
			isInArray = true;
			positionInArray = i;
			break;
		}
	}

	Serial.print("isInArray = ");
	Serial.println(isInArray);

	if(isInArray && status == LOW) {
		for(int i = positionInArray; i < this->lengthOfActiveArray; i++){
			this->activeLightArray[i] = this->activeLightArray[i+1];
		}
		this->lengthOfActiveArray--;

		turnOnLightBanks();

	}else if(!isInArray && status == HIGH && this->lengthOfActiveArray != 0){

		this->lengthOfActiveArray++;
		Serial.print("lengthOfActiveArray = ");
		Serial.println(this->lengthOfActiveArray);
		this->activeLightArray[this->lengthOfActiveArray-1] = bank;
		Serial.println("Turning on NIA!");

		turnOnLightBanks();
	//} else if() {

	}else if(!isInArray && status == HIGH && this->lengthOfActiveArray == 0){
		this->lengthOfActiveArray++;
		this->activeLightArray[0] = bank;
		Serial.println("Turning on! ActiveArray was 0");
		Serial.print("ActiveArray is now ");
		Serial.println(this->lengthOfActiveArray);

		turnOnLightBanks();
	}

}

void RoomLight::progressiveLightBanks() {

	if(this->lengthOfActiveArray == 0){
		activeBank(H8, HIGH);
	}else if (this->lengthOfActiveArray == num_connected){
		for (int i = 0; i < num_connected; i++) {
			activeBank(i+1, LOW);
		}
	}else{
		Serial.print("Passing to activeBank = ");
		Serial.println(this->activeLightArray[this->lengthOfActiveArray-1] +1);
		activeBank(this->activeLightArray[this->lengthOfActiveArray-1] + 1, HIGH);
	}

		// int banksConnected[num_connected];
		// for (int i = 0; i < num_connected; i++){
		// 	banksConnected[i] = i+1;
		// }
		//
		// for (int i = 0; i < this->lengthOfActiveArray; i++){
		// 	bool isAlreadyActive = false;
		//
		// 	int a = 0;
		// 	for (a; a < num_connected; a++) {
		// 		if(this->activeLightArray[i] == banksConnected[a]){
		// 			Serial.print("Current Active Item = ");
		// 			Serial.println(this->activeLightArray[i]);
		// 			isAlreadyActive = true;
		// 		}
		// 		isAlreadyActive = false;
		// 		Serial.print("banksConnected loop");
		// 		Serial.println(banksConnected[a]);
		// 	}
		// 	if(!isAlreadyActive){
		// 		Serial.print("activeLightArray[i]");
		// 		Serial.println(activeLightArray[i]);
		// 		Serial.print("banksConnected[i]");
		// 		Serial.println(banksConnected[i]);
		// 		activeBank(banksConnected[a], HIGH);
		// 		break;
		// 	}
		//}
		//turnOnLightBanks();
		//}
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
	for (int i = 0; i < repeats; i++){
		activeBank(ALL, HIGH);
		delay(speed);
		activeBank(ALL, LOW);
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
	if (deviceButton.isActive) {
		reading = digitalRead(deviceButton.pin_connected);

		while(reading == HIGH && isReset == true) {

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

		if(reading != HIGH) {
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


// for(int a = 0; a < length; a++) {
// 	this->previousLightingArray[a] = pins[a];
// 	Serial.print("PLA: ");
// 	Serial.print("a : ");
// 	Serial.print(pins[a]);
// 	if(a == length-1){
// 		for(int b = a; b < previousLightingArrayLength; b++){
// 			this->previousLightingArray[b] = 0;
// 			Serial.print("PLA: ");
// 			Serial.print("b : ");
// 			Serial.print(pins[b]);
// 		}
// 	}
// }
