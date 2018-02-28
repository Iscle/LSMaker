/*
	LSLibrary.h - Library to use the LSMaker sensors.
	Created by Iscle Gil, February, 2018.
	Released under the MIT License.
*/

#ifndef LSLibrary_h
#define LSLibrary_h

#define CHIPSELECT 53

#include "Arduino.h"

class LSLibraryClass {
	public:
		static void begin();
		static void getMAC(byte *macAddress);
	private:
		static uint8_t initialized;
};

extern LSLibraryClass LSLibrary

#endif
