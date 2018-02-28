/*
	LSLibrary.h - Library to use the LSMaker sensors.
	Created by Iscle Gil, February, 2018.
	Released under the MIT License.
*/

#include "Arduino.h"
#include "LSLibrary.h"

LSLibraryClass LSLibrary;

uint8_t SPIClass::initialized = 0;

void LsLibraryClass::begin() {
  if (!initialized) {
    pinMode(49, OUTPUT);  // SPI_A0
    pinMode(48, OUTPUT);  // SPI_A1
    pinMode(47, OUTPUT);  // SPI_A2

    digitalWrite(49, HIGH);
    digitalWrite(48, HIGH);
    digitalWrite(47, LOW);
  }
  initialized++;
}

void LSLibraryClass::getMAC (byte *macAddress) {
  digitalWrite (CHIPSELECT, LOW); // Pull CS low
  SPI.transfer(0x03); // Set the EEPROM in read mode
  SPI.transfer(0xFA); // Send the first position to read

  for (byte i = 0; i < 6; i++) {
    macAddress[i] = SPI.transfer(0x00); // Read each position of the MAC address
  }

  digitalWrite (CHIPSELECT, HIGH); // Pull CS high
}
