/*
	LSLibrary.h - Library to use the LSMaker sensors.
	Created by Iscle Gil, February, 2018.
	Released under the MIT License.
*/

#include "Arduino.h"
#include "SPI.h"
#include "LSLibrary.h"

uint8_t LSLibrary::initialized = 0;

void LSLibrary::begin() {
  if (!initialized) {
    // SPI bus
    pinMode(49, OUTPUT);  // SPI_A0 (Decoder LSB)
    pinMode(48, OUTPUT);  // SPI_A1
    pinMode(47, OUTPUT);  // SPI_A2 (Decoder MSB)
    pinMode(53, OUTPUT);  // CS_Pin (Decoder enable)

    // Encoder
    pinMode(3, INPUT_PULLUP);   // Left Encoder 1 (Interrupt)
    pinMode(27, INPUT_PULLUP);  // Left Encoder 2
    pinMode(2, INPUT_PULLUP);   // Right Encoder 1 (Interrupt)
    pinMode(26, INPUT_PULLUP);  // Right Encoder 2

    // Left motor
    pinMode(24, OUTPUT); // Left motor enable pin
    pinMode(25, OUTPUT); // Left motor direction pin (Forward/!Reverse)
    pinMode(8, OUTPUT);  // Left motor PWM pin
    // Right motor
    pinMode(22, OUTPUT); // Right motor enable pin
    pinMode(23, OUTPUT); // Right motor direction pin (Forward/!Reverse)
    pinMode(7, OUTPUT);  // Right motor PWM pin

    SPI.begin();
  }
  initialized++;
}

void LSLibrary::getMAC (byte *macAddress) {
  // Set decoder to EEPROM address
  digitalWrite(49, HIGH); // SPI_A0
  digitalWrite(48, HIGH); // SPI_A1
  digitalWrite(47, LOW);  // SPI_A2

  digitalWrite (53, LOW); // Pull CS low
  SPI.transfer(0x03); // Set the EEPROM in read mode
  SPI.transfer(0xFA); // Send the first position to read

  for (byte i = 0; i < 6; i++) {
    macAddress[i] = SPI.transfer(0x00); // Read each position of the MAC address
  }

  digitalWrite (53, HIGH); // Pull CS high
}
