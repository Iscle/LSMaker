/***************************************************
 * This is a library for the TouchScreen controller
 * of the AR1021 integrated circuit
 *
 * Developer: 	Joan Camps
 * Data:			07/11/2016
 * Release:		v0
 ****************************************************/
 
#include <Wire.h>

#define LSTOUCH_ADDRESS                             0x4D
#define LSTOUCH_HEADER                              0x55
#define LSTOUCH_I2C_CLOCK                           100000L

// COMMANDS
#define LSTOUCH_CMD_VERSION                         0x10
#define LSTOUCH_CMD_ENABLE                          0x12
#define LSTOUCH_CMD_DISABLE                         0x13
#define LSTOUCH_CMD_CALIBRATE                       0x14
#define LSTOUCH_CMD_REGISTER_READ                   0x20
#define LSTOUCH_CMD_REGISTER_WRITE                  0x21
#define LSTOUCH_CMD_REGISTER_START_ADDRESS_REQUEST  0x22
#define LSTOUCH_CMD_REGISTERS_WRITE_TO_EEPROM       0x23
#define LSTOUCH_CMD_EEPROM_READ                     0x28
#define LSTOUCH_CMD_EEPROM_WRITE                    0x29
#define LSTOUCH_CMD_EEPROM_WRITE_TO_REGISTERS       0x2B

// RESPONSES TO COMMANDS
#define LSTOUCH_RESP_SUCCESS                        0x00
#define LSTOUCH_RESP_BAD_COMMAND                    0x01
#define LSTOUCH_RESP_BAD_HEADER                     0x03
#define LSTOUCH_RESP_TIME_OUT                       0x04
#define LSTOUCH_RESP_CANCEL_CALIBRATION             0xFC
#define LSTOUCH_REG_TOUCH_THRESHOLD                 0x02
#define LSTOUCH_REG_SENSITIVITY_FILTER              0x03
#define LSTOUCH_REG_SAMPLING_FAST                   0x04
#define LSTOUCH_REG_SAMPLING_SLOW                   0x05
#define LSTOUCH_REG_ACCURACY_FILTER_FAST            0x06
#define LSTOUCH_REG_ACCURACY_FILTER_SLOW            0x07
#define LSTOUCH_REG_SPEED_THRESHOLD                 0x08
#define LSTOUCH_REG_SLEEP_DELAY                     0x0A
#define LSTOUCH_REG_PEN_UP_DELAY                    0x0B
#define LSTOUCH_REG_TOUCH_MODE                      0x0C
#define LSTOUCH_REG_TOUCH_OPTIONS                   0x0D
#define LSTOUCH_REG_CALIBRATION_INSET               0x0E
#define LSTOUCH_REG_PEN_STATE_REPORT_DELAY          0x0F
#define LSTOUCH_REG_TOUCH_REPORT_DELAY              0x11

#define DEBUG 1

class LSTouch{
public:
  LSTouch(void);
  void begin(uint8_t i2caddr = LSTOUCH_ADDRESS);
  void myWireFlush(void);
  void commandSending(byte command,byte numbytes,byte *data);
  int confirmReceived(byte command,byte numbytes,byte *data,byte *realnby);
  int disableTouch(void);
  int enableTouch(void);
  int getVersion(byte *p);
  int registerStartAddress(byte *p);
  int registerRead(byte adreg,byte *p);
  int registersRead(byte adreg,byte numreg,byte *p);
  int registerWrite(byte adreg,byte val);
  int registersWrite(byte adreg,byte numreg,byte *val);
  void ResetTouch(void);
  void Motor(void);
  boolean touched(void);
  void getPoint(void);
  int x,y,z;
private:
  void setup(void);
  static void IRQ_Touch(void);
};
