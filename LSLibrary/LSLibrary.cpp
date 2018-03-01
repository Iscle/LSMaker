/*
	LSLibrary.h - Library to use the LSMaker sensors.
	Created by Iscle Gil, February, 2018.
	Released under the MIT License.
*/

#include "Arduino.h"
#include "SPI.h"
#include "LSLibrary.h"

bool LSLibrary::initialized = false;

#define WIFIBAUD 9600

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

// Power pins
#define V_BAT A1

// Setup all the pins and functions requiered by the library
LSLibrary::LSLibrary() {
  if (!initialized) {
    // SPI bus
    pinMode(SPI_A0, OUTPUT);  // SPI_A0 (Decoder LSB)
    pinMode(SPI_A1, OUTPUT);  // SPI_A1
    pinMode(SPI_A2, OUTPUT);  // SPI_A2 (Decoder MSB)
    pinMode(CS, OUTPUT);      // CS_Pin (Decoder enable)

    // Encoder
    pinMode(LE_CLK, INPUT_PULLUP);   // Left Encoder 1 (Interrupt)
    pinMode(LE_DATA, INPUT_PULLUP);  // Left Encoder 2
    pinMode(RE_CLK, INPUT_PULLUP);   // Right Encoder 1 (Interrupt)
    pinMode(RE_DATA, INPUT_PULLUP);  // Right Encoder 2

    // Motors
    pinMode(LM_ENABLE, OUTPUT);     // Left motor enable pin
    pinMode(LM_DIRECTION, OUTPUT);  // Left motor direction pin (Forward/!Reverse)
    pinMode(LM_PWM), OUTPUT);       // Left motor PWM pin
    pinMode(RM_ENABLE, OUTPUT);     // Right motor enable pin
    pinMode(RM_DIRECTION, OUTPUT);  // Right motor direction pin (Forward/!Reverse)
    pinMode(RM_PWM, OUTPUT);        // Right motor PWM pin

    // Power
    pinMode(V_BAT, INPUT); // Battery voltage input pin

    SPI.begin();
    Serial3.begin(WIFIBAUD); // Start serial for ESP8266
  }
  initialized = true;
}

// Sets TFT Backlight (0% - 100%)
void LSLibrary::setTFTBacklight(byte backlight) {
  analogWrite(TFT_BL, map(backlight, 0, 100, 0, 255));
}

// Sets the decoder CS pin output
void LSLibrary::setCS(byte cs) {
  switch (cs) {
    case 0:
      // SPI_0 CS decoder address
      digitalWrite(SPI_A0, LOW);
      digitalWrite(SPI_A1, LOW);
      digitalWrite(SPI_A2, LOW);
      brake;
    case 1:
      // BLE CS decoder address
      digitalWrite(SPI_A0, HIGH);
      digitalWrite(SPI_A1, LOW);
      digitalWrite(SPI_A2, LOW);
      brake;
    case 2:
      // TFT CS decoder address
    	digitalWrite(SPI_A0, LOW);
    	digitalWrite(SPI_A1, HIGH);
    	digitalWrite(SPI_A2, LOW);
      brake;
    case 3:
      // EEPROM CS decoder address
      digitalWrite(SPI_A0, HIGH);
      digitalWrite(SPI_A1, HIGH);
      digitalWrite(SPI_A2, LOW);
      brake;
    case 4:
      // SD CS decoder address
      digitalWrite(SPI_A0, LOW);
      digitalWrite(SPI_A1, LOW);
      digitalWrite(SPI_A2, HIGH);
      brake;
    case 5:
      // SPI_5 CS decoder address
      digitalWrite(SPI_A0, HIGH);
      digitalWrite(SPI_A1, LOW);
      digitalWrite(SPI_A2, HIGH);
      brake;
    case 6:
      // SPI_6 CS decoder address
      digitalWrite(SPI_A0, LOW);
      digitalWrite(SPI_A1, HIGH);
      digitalWrite(SPI_A2, HIGH);
      brake;
    case 7:
      // SPI_7 CS decoder address
      digitalWrite(SPI_A0, HIGH);
      digitalWrite(SPI_A1, HIGH);
      digitalWrite(SPI_A2, HIGH);
      brake;
  }
}

// Restart ESP8266
void LSLibrary::WiFiRestart () {
  Serial3.println("AT+RST");
}

// Get ESP8266 version details
void LSLibrary::WiFiVersion () {
  Serial3.println("AT+GMR");
}

/*  Modes:
    1: Station mode
    2: SoftAP mode
    3: SoftAP + Station mode
*/

// Sets the ESP8266 WiFi mode
void LSLibrary::setWiFiMode(byte mode) {
  Serial3.println("AT+CWMODE_DEF=" + mode);
}

// Lists the available WiFi hotspots seen by the ESP8266
void LSLibrary::WiFiList() {
	Serial3.println("AT+CWLAP");
}

/*  Encryption methods:
    0: OPEN
    2: WPA_PSK
    3: WPA2_PSK
    4: WPA_WPA2_PSK
*/

// Sets the WiFi AP the ESP8266 will connect to
void LSLibrary::setWiFiAP(char *ssid, char *pwd, int chl, int mode) {
	Serial3.println("AT+CWSAP_DEF=\"" + ssid + "\",\"" + pwd + "\"," + chl + "," + mode);
}

// Reads a byte from the EEPROM and returns it
byte LSLibrary::EEPROMReadByte (byte address) {
  byte data;

  setCS(EEPROM);

  digitalWrite (CS, LOW);     // Pull CS low
  SPI.transfer(0x03);         // Set the EEPROM in read mode
  SPI.transfer(address);      // Send the position to read
  data = SPI.transfer(0x00);  // Read the data

  return data;  // Return the received data
}

// Function to get the MAC address stored on the EEPROM
void LSLibrary::getMAC (byte *macAddress) {
  // Set decoder to EEPROM address
  digitalWrite(SPI_A0, HIGH); // SPI_A0
  digitalWrite(SPI_A1, HIGH); // SPI_A1
  digitalWrite(SPI_A2, LOW);  // SPI_A2

  digitalWrite (CS, LOW); // Pull CS low
  SPI.transfer(0x03);     // Set the EEPROM in read mode
  SPI.transfer(0xFA);     // Send the first position to read

  for (byte i = 0; i < 6; i++) {
    macAddress[i] = SPI.transfer(0x00); // Read each position of the MAC address
  }

  digitalWrite (CS, HIGH); // Pull CS high
}

// Sets the motor speed, (LEFT/RIGHT, FORWARD/BRAKE/REVERSE, 0% - 100%)
void LSLibrary::setMotor (byte motor, byte direction, byte speed) {
  // Default to left motor
  int motorEnablePin = LM_ENABLE;
  int motorDirectionPin = LM_DIRECTION;
  int motorPwmPin = LM_PWM;
  byte enable = 1;

  switch (direction) {
    case -1:
      direction = LOW;
      break;
    case 0:
      enable = 0;
      break;
    case 1:
      direction = HIGH;
      break;
  }

  // Change pin variables if we need to use the right motor
  if (motor == 1) {
    motorEnablePin = RM_ENABLE;
    motorDirectionPin = RM_DIRECTION;
    motorPwmPin = RM_PWM;
  }

  analogWrite(motorPwmPin, map(speed, 0, 100, 0, 255));
  digitalWrite(motorDirectionPin, direction);
  digitalWrite(motorEnablePin, enable);
}
