/*  LSMaker software by Iscle.
 *  February 2018.
 *  
 *  Released under the MIT license.
 *
 *  For use with LSMaker v2 by La Salle URL - BCN
 */

// Configuration

// LCD Pins (Do not change)
#define TFT_DC 43
#define TFT_CS 53

// Battery voltage pin
#define VBAT_PIN 1

// Common libraries
#include <SPI.h>
#include <Wire.h>

// LCD libraries
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// Touchscreen libraries
#include "Adafruit_STMPE610.h"

// Gyroscope libraries (I2C)
#include <Adafruit_Sensor.h>
#include <Adafruit_FXAS21002C.h>

// Accelerometer libraries
#include <FXOS8700CQ.h>

// Create module instances
FXOS8700CQ accel = FXOS8700CQ(0x1F);  // Accelerometer
Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);  // Gyroscope
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);  // TFT Display

void setup() {
  Wire.begin();

  if (!gyro.begin()) {
    Serial.println ("Error! The gyroscope can't be detected.");
    while (1);
  }

  accel.begin();
}

void loop() {

}

void displaySetup () {

}

void gyroscopeCode () {

  sensor_t sensor;
  gyro.getSensor(&sensor);

  /*  Gyroscope (sensor info):
   *  sensor.name
   *  sensor.version
   *  sensor.sensor_id
   *  sensor.max_value
   *  sensor.min_value
   *  sensor.resolution
   */

  // Get a new sensor event
  sensors_event_t event;
  gyro.getEvent(&event);

  /*  Gyroscope (rad/s):
   *  event.gyro.x
   *  event.gyro.y
   *  event.gyro.z
   */
}

void accelerometerCode () {
  // Query acceleromter data
  accel.readAccelData();
  // Query magnetometer data
  accel.readMagData();

  /*  Accelerometer (multiplicar per 9.8, m/s^2):
   *  accel.accelData.x
   *  accel.accelData.y
   *  accel.accelData.z
   */

  /*  Magnetometer:
   *  accel.magData.x
   *  accel.magData.y
   *  accel.magData.z
   */
}

void tftCode () {

}

// Get the battery voltage, averaged to 10 reads
float vBat () {
  float voltage = 0.0;
  
  for (int i = 0; i < 10; i++) {
    voltage = voltage + analogRead(VBAT_PIN);
  }
  
  voltage = voltage / 10.0;
  
  return voltage;
}
