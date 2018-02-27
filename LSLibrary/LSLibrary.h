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
		Prova(int velocitat);
		void Cosa1();
		void Cosa2();
	private:
		int _velocitat;
};

#endif
