#include "RoomLight.h"

AddButton::AddButton(int pin, int debounce, bool isActive) {
	this->pin_connected = pin;
	this->debounce = debounce;
	this->isActive = isActive;
	pinMode(this->pin_connected, INPUT_PULLUP);
}
