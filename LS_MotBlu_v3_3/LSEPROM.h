/*************************************************** 
 * This is a library for the LSMaker Motor Control
 * Designed specifically to work with the LSMaker robot
 * ----> http://www.salleurl.edu
 * These Motor Control TAD use PWM and Encoders IRQs
 * 
 * Written by Joan Camps/Marcos Hervas for LaSalle-URL.  
 * BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef LSEPROM_H
#define LSEPROM_H

#include <SPI.h>

#define SLAVESELECT 53   //ss

#define SPI_A0 49    //aquests 3 fan activar la sortida del decoder corresponent a la EEPROM
#define SPI_A1 48
#define SPI_A2 47

class LSEPROM {
public:
  LSEPROM();
  void lsGetMAC(byte *macAddress);

private:
  void fill_buffer(void);
  byte readSPI(byte A);
};
#endif

