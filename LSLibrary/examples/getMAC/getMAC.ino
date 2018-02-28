#include <SPI.h>
#include <LSLibrary.h>

LSLibrary robot; // Create an instance

void setup () {
  Serial.begin(115200); // Initialise the serial to 115200bps
  
  byte mac[6]; // The MAC will be stored here

  robot.getMAC(mac); // Get the MAC from the EEPROM

  // Print the MAC to the serial interface
  for (int i = 0; i < 6; i++) {
    Serial.print(mac[i]);
    if (i != 5) {
      Serial.print(";");
    }
    else {
      Serial.println();
    }
  }
}

void loop () {
  //This example doesn't need anything here.
}
