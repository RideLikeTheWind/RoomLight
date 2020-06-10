#include <RoomLight.h>

// Number of banks connected
// Only 1-4 (see Readme)
const int num_connected = 4;

// Pin you have plugged button in to
int buttonPin = 13;

//Instatiate (provide as written)
RoomLight my_lamp = RoomLight(num_connected);

void setup() {
  // Setup call
  my_lamp.TPICBegin();
}

void loop() {
  // Always call this to reset the banks on startup
  my_lamp.firstRun();

  // Turn on a single bank
  my_lamp.activeBank(H6, HIGH);


  // Turn some of the banks on and off
  my_lamp.activeBank(H8, HIGH);
  my_lamp.activeBank(H5, LOW);
  delay(50);

  my_lamp.activeBank(H5, HIGH);
  my_lamp.activeBank(H4, HIGH);
  delay(50);

  my_lamp.activeBank(H4, LOW);
  my_lamp.activeBank(H8, LOW);
  delay(50);

  // Example for switching lights on progressively using the button
  // Won't run simultaneously with above code
  // Attach button
  // my_lamp.connectButton(buttonPin); //provide the pin you have connected the button to

  // if(my_lamp.buttonIsPressed()) {
  //   //Cycle up through the banks until they are all on then resets
  //   my_lamp.progressiveLightBanks();
  //  }

}
