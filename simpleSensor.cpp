#include "RoomLight.h"

SimpleSensor::SimpleSensor(int pin, int type, bool isActive, long int debounce) {
	this->pin_connected = pin;
	this->isActive = isActive;
	this->debounce = 1000;
	this->type = type;

	pinMode(this->pin_connected, OUTPUT);
}
long unsigned int currentTime = 0;
long unsigned int previousTime = 0;
float previous_value = 0.0;

// Only returns every 1 second
float SimpleSensor::readSensor(int returnType) {
	if(isActive){
		currentTime = millis();
		//Always returns the previous value so
		//things don't reset to 0.0 every time
		//it cycles
		float ret_val = previous_value;

		if(returnType != 0){
			if(currentTime - previousTime > debounce) {
				if (this->type == Temperature) {
					// set ret_val to the new reading
					ret_val = readTemp(returnType, this->pin_connected);
				}
				else if (type == Light) {
					// set ret_val to the new reading
					ret_val = readLight(this->pin_connected);
				}
				// else if (type == Sound){ (V2)
				//
				// }

				// reset the time
				previousTime = currentTime;
			}
		}else{
			// Return an arbritrary value if no return type specified
			ret_val = 9999.99;
		}
		// Store the previous value so we can return it to remove 0.0 returns
		// between debounce calls
		previous_value = ret_val;
		return ret_val;

	}else{
		return false;
	}
}

// Private functions

float SimpleSensor::readLight(int p) {

	int lightValue = analogRead(p);
	lightValue = map(lightValue, 0, 1023, 0, 100);
	float ret_val = (float) lightValue;
	return ret_val;
}


float SimpleSensor::readTemp(int returnType, int p) {
	//Using the Arduino example
	int reading = analogRead(p);

 	// converting that reading to voltage, for 3.3v arduino use 3.3
 	float voltage = reading * 5.0;
 	voltage /= 1024.0;

 	float temperatureC = (voltage - 0.5) * 100 ;
 	//converting from 10 mv per degree wit 500 mV offset
	//to degrees ((voltage - 500mV) times 100)
	// now convert to Fahrenheit
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

#if DEBUG
	Serial.print(voltage); Serial.println(" volts");
 	Serial.print(temperatureC); Serial.println(" degrees C");
 	Serial.print(temperatureF); Serial.println(" degrees F");
#endif

	if(returnType == Celsius){
		return temperatureC;
	}else if (returnType == Fahrenheit) {
		return temperatureF;
	}
	return 0.0;
}
