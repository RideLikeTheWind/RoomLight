
## TPIC6B595 Library

### License
License Copyright: Copyright © 2007 Free Software Foundation, Inc.
License License: "Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed." (www.gnu.org/licenses/gpl-3.0.en.html).
License Contact: Free Software Foundation (fsf.org).

SPDX short identifier: GPL-3.0-only

### Description
This library is designed for the Texas Instruments TPIC6C595 Serial to Parallel Shift Register. It was adapted from a similar library by Derek Chafin to suit a specific need.
It utilises a circuit designed to control a Room Light in a project built by Loreto Kirribilli. The library will not work correctly without that circuit. If you require more information please contact the author.

### Authorship
Author: J Mascorella  
Github: @iridelikethewind
Year: 2020  
Version: 1.0

### Configuration
You will need to supply the following parameters at the top of the main .ino file in Arduino.
num_connected: the number of lights you have connected to the Arduino (up to 4).
buttonPin: If desired to make reallocation of the button easier if altered
_Extensible pins are available but not mapped in this version_
```c
int num_connected = 4;
int buttonPin = 13;
```

### Functions(params)

#### Initialisation
Constructor: Call this before the void setup() and loop() functions in Arduino, but after config variables above.
```c
RoomLight(int num_connected);
```

Setup function
Must run in the void setup() loop in Arduino
```c
void TPICBegin();
```

#### UI functions

Turn on or off a specific bank
bank = constant of names banks (H8/H6/H5/H4)
status = constant name LOW/HIGH
```c
void activeBank(int bank, int status);
```

Turn on a bank at a time, then reset at end
Called on event
```c
void progressiveLightBanks();
```
_Example_
```c
if(buttonIsPressed) {
  my_light.progressiveLightBanks(); //tracks current, next and reset
}
```

**BLOCKING**
Flashes all lights on and off
repeats = number of flashes
speed = delay in ms for lights to remain on (1000 = 1 second)
```c
void ledsFlash(int repeats, int speed);
```

**BLOCKING**
Lights roll around quickly
repeats = number of repeats you want (about 1 per second)
speed = how quickly the lights should change. Slower is a bigger number (> 20 starts getting slow)
```c
void ledsDance(int repeats, int speed);
```

Turn on or off a specific bank
pin = refer to constants: H8 || H6 || H5 || H4 as printed on the board
status = HIGH || LOW for On / Off
```c
void activeBank(int pins, int status);
```

#### Button Functions

Function to return the value of a button if pressed or not
Returns bool true || false
```c
bool buttonIsPressed();
```

Instantiate button and set parameters
pin = availble output pin - default: 0
debounce = delay to ensure button is actually pressed (default = 100ms)
isActive = set to default as true (on) but can be turned off and on depending on user prefs (default = true)
variables are public so are mutable eg: my_button.pin = 1;
```c
void connectButton(int pin=0, int debounce=100, bool isActive=true);
```

#### Utility functions

Main write function
Can be used to override LED displays
Argument is a decimal number that converts to an 8 bit byte (00000000 - 11111111 as 0 - 255)
_Only use if another function is unsuitable_
* 0, //All off
* 16 //H8 Bank on 00010000
* 32, //H6 Bank on 00100000
* 64, //H5 Bank on 01000000
* 128, //H4 Bank on 10000000
* 240 //ALL ON
Logic can be used to add values together to turn on a sequence of banks i.e.
16 + 32 = 48 = H8 and H6 ON
```c
void writeShift(int bits);
```

Turn the pins off or on
HIGH = on
LOW = off
```c
void pinDrains(int visibility);
```

Clear all pins of output
```c
void pinClear();
```

#### Debug mode

Define DEBUG in the RoomLight.h file
```c
#define DEBUG true
```

### To do
* Updates required to provide other UI Functions
* Need to explore improvements to the circuit to which this library is associated
