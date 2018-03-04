/*
	LSLibrary.h - Library to use the LSMaker sensors.
	Created by Iscle Gil, February, 2018.
	Released under the MIT License.
*/

#ifndef LSLibrary_h
#define LSLibrary_h

#define LEFT 0
#define RIGHT 1
#define FORWARD 0
#define BRAKE 2
#define REVERSE 1

#define BLE 1
#define LCD 2
#define EEPROM 3
#define SDCARD 4

#define CS 53

#include "Arduino.h"

class LSLibrary {
	public:
		LSLibrary ();
		bool touchScreenTouched ();
		void setTFTBacklight (byte backlight);
		void setCS (byte cs);
		void WiFiRestart ();
		void WiFiVersion ();
		void setWiFiMode (byte mode);
		void WiFiList ();
		void setWiFiAP (char *ssid, char *pwd, int chl, int mode);
		void joinWiFi (char *ssid, char *pwd);
		float getVBat ();
		void EEPROMWriteByte (byte address, byte data);
		byte EEPROMReadByte (byte address);
		void getMAC (byte *macAddress);
		void setLEDs (byte brightness);
		void setMotor (byte motor, byte direction, byte pwm);
	private:
		void touchScreenIRQ ();
		static bool initialized;
};

#endif
