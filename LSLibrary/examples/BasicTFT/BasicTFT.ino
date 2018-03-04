/*
 * BasicTFT - A basic sketch to use the TFT display on the LSMaker.
 * Shows battery information and MAC address.
 * 
 * Requires: Adafruit_GFX, Adafruit_ILI9341, LSLibrary.
 * 
 * By Iscle.
 */

// Include the libraries the sketch needs
#include <SPI.h>
#include <LSLibrary.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Define where the TFT is connected (DO NOT MODIFY)
#define TFT_DC 43
#define TFT_CS 53
#define TFT_RST 32

// Create robot and tft instances
LSLibrary robot;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);


// Do this one time
void setup() {
  byte mac[6]; // Where the MAC will be stored
  
  robot.setTFTBacklight(0); // Shut off the TFT BL while it's not initialized
  robot.getMAC(mac);        // Get the MAC address from the EEPROM
  robot.setCS(LCD);         // Set the CS decoder to the LCD pin

  tft.begin();                                  // Initialize the display
  tft.setRotation(3);                           // Set the right rotation
  tft.fillScreen(ILI9341_BLACK);                // Fill the screen with black
  tft.fillRect(0, 229, 320, 11, ILI9341_WHITE); // Create the bottom white rectangle
  tft.setTextColor(ILI9341_BLACK);              // Set the text color to black
  tft.setCursor(187, 231);                      // Set the cursor to x: 187, y: 231
  tft.print("MAC: ");                           // Print "MAC: " to the display

  // Print the MAC address to the display
  for (int i = 0; i < 6; i++) {
    tft.print(mac[i], HEX);

    if (i != 5) {
      tft.print(":");
    }
  }

  robot.setTFTBacklight(255); // Enable the TFT backlight
}

void loop() {
  float vBat = robot.getVBat();                   // Get the current battery voltage
  tft.setCursor(2, 231);                          // Set the cursor to x: 2, y: 231
  tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE); // Set the text color to black, with white background
  tft.print("Battery: ");                         // Print "Battery: " to the display
  tft.print((vBat - 7.2) * 100 / 3.2);            // Print the battery percentage
  tft.print("% (");                               // Print the % symbol
  tft.print(vBat);                                // Print the voltage
  tft.print("v) ");                               // Print the unit (v)
  delay(2500);                                    // Wait 2500ms
}
