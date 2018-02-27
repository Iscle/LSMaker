
/*************************************************** 
  This is a library for the LSMaker Motor Control
  Designed specifically to work with the LSMaker robot
  ----> http://www.salleurl.edu
  These Motor Control TAD use PWM and Encoders IRQs
  
  Written by Joan Camps/Marcos Hervas for LaSalle-URL.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "LSEPROM.h"

//variables
//byte macAddress[6];


byte variableBYTE;   

byte eeprom_output_data;
byte eeprom_input_data=0;
byte clr;
int address=0;
//data buffer
char buffer [128];



LSEPROM::LSEPROM(void) {
  SPI.begin();
}



void LSEPROM::fill_buffer(){
  for (int I=0;I<128;I++){
    buffer[I]=I;
  }
}

//void LSEPROM::EEPROM_setup() {
    // start the SPI library:
//  SPI.begin();
//}


void LSEPROM::lsGetMAC(byte *macAddress){
 
 
  //configurar CS (els tres valors A0-A2) que activen la sortida del decoder corresponent a la EEPROM
  pinMode(SPI_A0,OUTPUT);    
  pinMode(SPI_A1,OUTPUT);
  pinMode(SPI_A2, OUTPUT);
  
  digitalWrite(SPI_A0,HIGH);    
  digitalWrite(SPI_A1,HIGH);
  digitalWrite(SPI_A2, LOW);

   macAddress[0]=readSPI(0xFA);
   macAddress[1]=readSPI(0xFB);
   macAddress[2]=readSPI(0xFC);
   macAddress[3]=readSPI(0xFD);
   macAddress[4]=readSPI(0xFE);
   macAddress[5]=readSPI(0xFF);

  

  for(int i = 5; i>0; i--){

  Serial.print(macAddress[i],HEX);  //mostra per pantalla les dues parts de la MAC
  Serial.print(":");  //mostra per pantalla les dues parts de la MAC
  }
  Serial.println(macAddress[0],HEX);
  //Serial.println("");
  

    
}

byte LSEPROM::readSPI(byte A){
   byte dada;
   delay(200);
   digitalWrite(SLAVESELECT,LOW);
   //delay(200);
   SPI.transfer(0x03);
   SPI.transfer(A);
   dada = SPI.transfer(0x00);
   digitalWrite(SLAVESELECT, HIGH);
   delay(200);
   return dada;
}


