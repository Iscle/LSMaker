/***************************************************
  This is our library for the ESP8266 wifi module using
  AT commands
 ****************************************************/

#include "ESP8266_AT.h"
#ifdef __AVR
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif
#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"



int reset_pin;
int enable_pin;

ESP8266::ESP8266(){

	
}

int ESP8266::begin(int reset, int enable, unsigned long baudrate){
	reset_pin = reset;
	enable_pin = enable;
	
	pinMode(reset_pin,OUTPUT);  //Reset
	pinMode(enable_pin,OUTPUT);  //Enable

	digitalWrite(reset_pin,HIGH);
	digitalWrite(enable_pin,HIGH);
	
	Serial3.begin(baudrate);
	
	//Reset inicial per software perquè sinó de vegades es penja
	delay(100);
	digitalWrite(reset,LOW);
	delay(100);
	digitalWrite(reset,HIGH);
	delay(100);
}

void ESP8266::restart(void){
	Serial3.println("AT+RST");
}

void ESP8266::release(void){
	Serial3.println("AT+GMR");
}

/*
	1 : station mode
	2 : softAP mode
	3 : softAP + station mode
*/

void ESP8266::modo(int mode){
	Serial3.println((String)"AT+CWMODE_CUR=" + mode);
	//Serial3.println("AT+CWMODE_CUR=%d",mode);
}

void ESP8266::list(void){
	Serial3.println("AT+CWLAP");
}

void ESP8266::config_AP(char * ssid, char * password, int chl, int mode){
	Serial3.println((String)"AT+CWSAP_CUR=\""+ssid+"\",\""+password+"\","+chl+","+mode);
}
