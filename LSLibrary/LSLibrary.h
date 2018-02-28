/*
	LSLibrary.h - Library to use the LSMaker sensors.
	Created by Iscle Gil, February, 2018.
	Released under the MIT License.
*/

#ifndef LSLibrary_h
#define LSLibrary_h

#define LEFT 0
#define RIGHT 1
#define FORWARD 1
#define REVERSE 0

#include "Arduino.h"

class LSLibrary {
	public:
		void begin();
		void getMAC(byte *macAddress);
		void setMotor(byte motor, byte direction, byte pwm);
	private:
		static uint8_t initialized;
};

#endif
