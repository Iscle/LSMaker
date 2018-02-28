/*
	LSLibrary.h - Library to use the LSMaker sensors.
	Created by Iscle Gil, February, 2018.
	Released under the MIT License.
*/

#ifndef LSLibrary_h
#define LSLibrary_h

#include "Arduino.h"

class LSLibrary {
	public:
		void begin();
		void getMAC(byte *macAddress);
	private:
		static uint8_t initialized;
};

#endif
