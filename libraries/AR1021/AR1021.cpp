/***************************************************
 * This is a library for the TouchScreen controller
 * of the AR1021 integrated circuit
 *
 * Developer: 	Joan Camps
 * Data:			07/11/2016
 * Release:		v0
 ****************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include "AR1021.h"

const byte interruptPin = 19;
volatile byte state = LOW;

int buttonpressed = 0;
int count = 0;

bool LSHiHaButton = 0;

unsigned char bufRx[2048];
int byRx=0;

/**************************************************************************/
/*!
 @brief  Instantiates a new LSTouch class
 */
/**************************************************************************/
LSTouch::LSTouch() {
}

/* /**************************************************************************/
/*!
 @brief  Setups the HW
 */
/**************************************************************************/
void LSTouch::begin(uint8_t i2caddr) {
  Wire.begin();
  Wire.setClock(LSTOUCH_I2C_CLOCK);
  delay(100);

  myWireFlush();

  Serial.begin(9600);          // comença la comunicació serial a 9600bps
  Serial.println("inici setup");

  setup();

  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), IRQ_Touch, RISING);
  delay(50);
  Motor();
}

void LSTouch::setup() {
  int i;
  byte ret[10],adr;
  int n;
  disableTouch();

  delay(50);
  Serial.println("Version:");
  n = getVersion(ret);
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");

  Serial.println("registerStartAddress:");
  n=registerStartAddress(ret);
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
  adr = ret[0];

  // TOUCH_MODE

  ///////// TOUCH_MODE ////////
  //
  //   xxxxxxxx
  //   |||||||+--- PU.0
  //   ||||||+---- PU.1
  //   |||||+----- PU.2
  //   ||||+------ PM.0
  //   |||+------- PM.1
  //   ||+-------- PD.0
  //   |+--------- PD.1
  //   +---------- PD.2
  //
  //   PD - Pen Down
  //   PM - Pen Movement
  //   PU - Pen Up
  //
  // proves: només PD, resta a 0 --> xxx00000
  // by default:    0xB1 --> 10110001 (PD.P0P1; PM.P1; PU.P0)
  // PROPOSTA:      0x51 --> 01010001 (PD.P1; PM.P1; PU.P0)
  // ALTAERNATIVA:  0x41 --> 01000001 (PD.P1; PM.no; PU.P0)
  Serial.print("registerRead: TOUCH_MODE");

  n=registerRead(adr+LSTOUCH_REG_TOUCH_MODE,ret);
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.println("registerWrite: TOUCH_MODE");
  registerWrite(adr+LSTOUCH_REG_TOUCH_MODE,0x51);   // B1 (10110001) by default
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.println("registerRead: TOUCH_MODE");
  n=registerRead(adr+LSTOUCH_REG_TOUCH_MODE,ret);
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");

  // TOUCH_THRESHOLD
  // by default:  0xC5 --> 11000101 (197)
  // PROPOSTA     0XC5
  Serial.println("registerRead: TOUCH_THRESHOLD");
  n=registerRead(adr+LSTOUCH_REG_TOUCH_THRESHOLD,ret);
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.println("registerWrite: TOUCH_THRESHOLD");
  registerWrite(adr+LSTOUCH_REG_TOUCH_THRESHOLD,0xC5);   // 0XC5 by default
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.println("registerRead: TOUCH_THRESHOLD");
  n=registerRead(adr+LSTOUCH_REG_TOUCH_THRESHOLD,ret);
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");

  // SAMPLING_FAST
  // by default:  0x04 --> 00000100
  // PROPOSTA     0X10 --> valor 16 (un dels recomanats)
  Serial.println("registerRead: SAMPLING_FAST");
  n=registerRead(adr+LSTOUCH_REG_SAMPLING_FAST,ret);
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.println("registerWrite: SAMPLING_FAST");
  registerWrite(adr+LSTOUCH_REG_SAMPLING_FAST,0x10);   // 0X04 (00000100) by default
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.println("registerRead: SAMPLING_FAST");
  n=registerRead(adr+LSTOUCH_REG_SAMPLING_FAST,ret);
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");

  // SAMPLING_SLOW
  // by default:  0x10 --> 00010000
  // PROPOSTA     0X80 --> valor 128 (un dels recomanats)
  Serial.println("registerRead: SAMPLING_SLOW");
  n=registerRead(adr+LSTOUCH_REG_SAMPLING_SLOW,ret);
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.println("registerWrite: SAMPLING_SLOW");
  registerWrite(adr+LSTOUCH_REG_SAMPLING_SLOW,0x80);   // 0X10 (00010000) by default
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.println("registerRead: SAMPLING_SLOW");
  n=registerRead(adr+LSTOUCH_REG_SAMPLING_SLOW,ret);
  if (n>=0) for (i=0;i<n;i++) {
    Serial.print(ret[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
  enableTouch();
  delay(50);
}

void LSTouch::myWireFlush() {
  char c;
  while (Wire.available()) {
    c = Wire.read();
  }
}

void LSTouch::commandSending(byte command,byte numbytes,byte *data) {
  int i=0;
  Wire.beginTransmission(LSTOUCH_ADDRESS);
  Wire.write((byte)0x00);
  Wire.write((byte)LSTOUCH_HEADER);
  Wire.write((byte)numbytes+1);
  Wire.write((byte)command);
  for(i=0;i<numbytes;i++) Wire.write((byte)data[i]);
  Wire.endTransmission();
}

int LSTouch::confirmReceived(byte command,byte numbytes,byte *data,byte *realnby) {
  unsigned char hdr,nby,stat,comm;
  unsigned char ret=0x00;
  int i;
  Wire.requestFrom(LSTOUCH_ADDRESS, 4+numbytes);
  hdr = Wire.read();
  Serial.print("0x");
  Serial.print(hdr,HEX);
  Serial.print("\t");
  if (hdr!=LSTOUCH_HEADER) ret=0x10;
  nby = Wire.read();
  Serial.print("0x");
  Serial.print(nby,HEX);
  Serial.print("\t");
  if (nby!=numbytes+2) ret=0x20;
  stat = Wire.read();
  Serial.print("0x");
  Serial.print(stat,HEX);
  Serial.print("\t");
  if (ret==0) ret=stat;
  comm = Wire.read();
  Serial.print("0x");
  Serial.print(comm,HEX);
  Serial.print("\t");
  if (ret==0 && comm!=command) ret=0x30;
  if (ret!=0x00) myWireFlush();
  else {
    i=0;
    while (i<nby-2) {
      data[i] = Wire.read();
      Serial.print("0x");
      Serial.print(data[i],HEX);
      Serial.print("\t");
      i++;
    }
    *realnby = nby-2;
  }

  bufRx[byRx++] = hdr;
  bufRx[byRx++] = nby;
  bufRx[byRx++] = stat;
  bufRx[byRx++] = comm;
  if (ret==0x00) {
    i=0;
    while (i<nby-2) {
      bufRx[byRx++] = data[i];
      i++;
    }
  }

  return ret;
}

int LSTouch::disableTouch(void) {
  byte n,r;
  byte p[5];
  int i=0,ret=1;
  do {

    commandSending(LSTOUCH_CMD_DISABLE,0,p);
    r = confirmReceived(LSTOUCH_CMD_DISABLE,0,p,&n);
    Serial.println("DisableTouch");
    i++;
    if (r) delay(50);
  }
  while(r!=0x00 && i<69);
  if (r!=0x00) ret=0;
  delay(50);
  return ret;
}

int LSTouch::enableTouch(void) {
  byte n,r;
  byte p[5];
  int i=0,ret=1;

  do {

    commandSending(LSTOUCH_CMD_ENABLE,0,p);
    r = confirmReceived(LSTOUCH_CMD_ENABLE,0,p,&n);
    Serial.println("enableTouch");

    i++;
    if(r!=0x00) delay(50);
  }
  while(r!=0x00 && i<69);
  if (r!=0x00) ret=0;
  delay(50);
  return ret;
}

int LSTouch::getVersion(byte *p) {
  byte n=0,r;
  int i=0,ret=0;
  //disableTouch();
  bufRx[byRx++] = 0xFF;

  do {
    commandSending(LSTOUCH_CMD_VERSION,0,p);
    r = confirmReceived(LSTOUCH_CMD_VERSION,3,p,&n);
    Serial.println("LSTOUCH_CMD_VERSION");
    i++;
    if(r!=0x00) delay(50);
  }
  while(r!=0x00 && i<69);
  bufRx[byRx++] = 0xFF;
  bufRx[byRx++] = 0xFF;
  ret=n;
  if (r!=0x00) ret=-1;
  return ret;
}


int LSTouch::registerStartAddress(byte *p) {
  byte n=0,r;
  int i=0,ret=0;
  bufRx[byRx++] = 0xFF;

  do {
    commandSending(LSTOUCH_CMD_REGISTER_START_ADDRESS_REQUEST,0,p);
    r = confirmReceived(LSTOUCH_CMD_REGISTER_START_ADDRESS_REQUEST,1,p,&n);
    Serial.println("registerStartAddress");
    i++;
    if(r!=0x00) delay(50);
  }
  while(r!=0x00 && i<69);
  Serial.println(i);
  bufRx[byRx++] = 0xFF;
  bufRx[byRx++] = 0xFF;
  ret=n;
  if (r!=0x00) ret=-1;
  return ret;
}

int LSTouch::registerRead(byte adreg,byte *p) {
  byte n=0,r;
  byte arr[3];
  int i=0,ret=0;
  //disableTouch();
  bufRx[byRx++] = 0xFF;
  //delay(50);
  arr[0] = 0x00;
  arr[1] = adreg;
  arr[2] = 0x01;
  do {
    commandSending(LSTOUCH_CMD_REGISTER_READ,3,arr);
    r = confirmReceived(LSTOUCH_CMD_REGISTER_READ,0x01,p,&n);
    Serial.println("registerRead");

    i++;
    if(r!=0x00) delay(50);
  }
  while(r!=0x00 && i<69);
  Serial.println(i);
  bufRx[byRx++] = 0xFF;
  bufRx[byRx++] = 0xFF;
  ret=n;
  if (r!=0x00) ret=-1;
  return ret;
}

int LSTouch::registersRead(byte adreg,byte numreg,byte *p) {
  byte n=0,r;
  byte arr[3];
  int i=0,ret=0;
  bufRx[byRx++] = 0xFF;

  arr[0] = 0x00;
  arr[1] = adreg;
  arr[2] = numreg;
  do {
    commandSending(LSTOUCH_CMD_REGISTER_READ,3,arr);
    r = confirmReceived(LSTOUCH_CMD_REGISTER_READ,numreg,p,&n);
    Serial.println("registersRead");
    i++;
    if(r!=0x00) delay(50);
  }
  while(r!=0x00 && i<69);
  bufRx[byRx++] = 0xFF;
  bufRx[byRx++] = 0xFF;
  ret=n;
  if (r!=0x00) ret=-1;
  return ret;
}

int LSTouch::registerWrite(byte adreg,byte val) {
  byte n=0,r;
  byte arr[4];
  int i=0,ret=0;
  byte numreg = 3, p[10];
  bufRx[byRx++] = 0xFF;
  arr[0] = 0x00;
  arr[1] = adreg;
  arr[2] = 0x01;
  arr[3] = val;
  do {
    commandSending(LSTOUCH_CMD_REGISTER_WRITE,4,arr);
    r = confirmReceived(LSTOUCH_CMD_REGISTER_WRITE,0,p,&n);
    Serial.println("LSTOUCH_CMD_REGISTER_WRITE");

    i++;
    if(r!=0x00) delay(50);
  }
  while(r!=0x00 && i<69);
  bufRx[byRx++] = 0xFF;
  bufRx[byRx++] = 0xFF;
  ret=n;
  if (r!=0x00) ret=-1;
  return ret;
}

int LSTouch::registersWrite(byte adreg,byte numreg,byte *val) {
  byte n=0,r;
  byte arr[20];
  int i=0,ret=0;
  byte p[10];
  bufRx[byRx++] = 0xFF;
  arr[0] = 0x00;
  arr[1] = adreg;
  arr[2] = numreg;
  for(i=0;i<numreg;i++) arr[3+i] = val[i];
  i=0;
  do {

    commandSending(LSTOUCH_CMD_REGISTER_WRITE,3+numreg,arr);
    r = confirmReceived(LSTOUCH_CMD_REGISTER_WRITE,0,p,&n);
    Serial.println("registersWrite");
    i++;
    if(r!=0x00) delay(50);
  }
  while(r!=0x00 && i<69);

  bufRx[byRx++] = 0xFF;
  bufRx[byRx++] = 0xFF;
  ret=n;
  if (r!=0x00) ret=-1;
  return ret;
}

void LSTouch::ResetTouch() {
}

static void LSTouch::IRQ_Touch() {
  int i;

  buttonpressed = 1;
  count++;
}

void LSTouch::Motor() {
  int i;
  unsigned char LSTouch_data[4],c;


  if (buttonpressed == 1) {
    buttonpressed=0;
    Wire.requestFrom(0x4D, 5);  //direcciòn 77, 5 bytes: constante que val 80 si ON y 81 si Off. 2 bytes, dos indican X i dos Y)
    c=Wire.read(); // rep un byte com a caràcter

    if (c==0x80) z = 0; //Serial.println("released");
    if (c==0x81) z = 1;//Serial.println("pressed");

    LSHiHaButton = 1;

    while (Wire.available()) { // l'esclau pot enviar menys del requerit
      LSTouch_data[i]= Wire.read(); // rep un byte com a caràcter
      i++;
    }
    x = (((LSTouch_data[1]<<7)&0x0F80)|((LSTouch_data[0])&0x007F))>>2;
    y = (((LSTouch_data[3]<<7)&0x0F80)|((LSTouch_data[2])&0x007F))>>2;

  }
}

boolean LSTouch::touched(void) {
  return LSHiHaButton;
}

void LSTouch::getPoint(void) {
  LSHiHaButton = 0;
}
