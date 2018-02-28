/*
	LSLibrary.h - Library to use the LSMaker sensors.
	Created by Iscle Gil, February, 2018.
	Released under the MIT License.
*/

#include "Arduino.h"
#include "SPI.h"
#include "LSLibrary.h"

uint8_t LSLibrary::initialized = 0;

// SPI pins
#define SPI_A0 49
#define SPI_A1 48
#define SPI_A2 47
#define CS 53

// Encoder pins
#define LE_CLK 3
#define LE_DATA 27
#define RE_CLK 2
#define RE_DATA 26

// Motor pins
#define LM_ENABLE 24
#define LM_DIRECTION 25
#define LM_PWM 8
#define RM_ENABLE 22
#define RM_DIRECTION 23
#define RM_PWM 7

// Setup all the pins and functions requiered by the library
void LSLibrary::begin() {
  if (!initialized) {
    // SPI bus
    pinMode(SPI_A0, OUTPUT);  // SPI_A0 (Decoder LSB)
    pinMode(SPI_A1, OUTPUT);  // SPI_A1
    pinMode(SPI_A2, OUTPUT);  // SPI_A2 (Decoder MSB)
    pinMode(CS, OUTPUT);  // CS_Pin (Decoder enable)

    // Encoder
    pinMode(LE_CLK, INPUT_PULLUP);   // Left Encoder 1 (Interrupt)
    pinMode(LE_DATA, INPUT_PULLUP);  // Left Encoder 2
    pinMode(RE_CLK, INPUT_PULLUP);   // Right Encoder 1 (Interrupt)
    pinMode(RE_DATA, INPUT_PULLUP);  // Right Encoder 2

    // Motors
    pinMode(LM_ENABLE, OUTPUT); // Left motor enable pin
    pinMode(LM_DIRECTION, OUTPUT); // Left motor direction pin (Forward/!Reverse)
    pinMode(LM_PWM), OUTPUT);  // Left motor PWM pin
    pinMode(RM_ENABLE, OUTPUT); // Right motor enable pin
    pinMode(RM_DIRECTION, OUTPUT); // Right motor direction pin (Forward/!Reverse)
    pinMode(RM_PWM, OUTPUT);  // Right motor PWM pin

    SPI.begin();
  }
  initialized++;
}

// Function to get the MAC address stored on the EEPROM
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

void LSLibrary::setMotorRotation (byte motor, byte direction, byte pwm, byte ) {

}
// Sets the motor speed, (LEFT/RIGHT, FORWARD/BRAKE/REVERSE, 0-255)
void LSLibrary::setMotor (byte motor, byte direction, byte pwm) {
  // Default to left motor
  int motorEnablePin = 24;
  int motorDirectionPin = 25;
  int motorPwmPim = 8;
  byte enable = 1;

  if (direction == 0) {
    enable = 0;
  }

  // Change pin variables if we need to use the right motor
  if (motor == 1) {
    motorEnablePin = 22;
    motorDirectionPin = 23;
    motorPwmPim = 7;
  }

  analogWrite(motorPwmPin, pwm);
  digitalWrite(motorDirectionPin, direction);
  digitalWrite(motorEnablePin, enable);
}
