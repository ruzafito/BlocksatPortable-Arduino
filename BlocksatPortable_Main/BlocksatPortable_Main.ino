/*********************************************************************
 This code contains parts included in Adafruit Libraries and example
 codes from Adafruit, all of them under free software licenses.

 Some pieces of the BLE module code are imported from bleuart_cmdmode
 example of the Adafruit BluefruitLE nRF51 Library, under MIT license.

 Some pieces of the FONA module code are imported from FONAtest example
 of the Adafruit FONA Library, written by Limor Fried/Ladyada for
 Adafruit Industries under BSD license.

 Some pieces of the FONA module code are imported from GPS example
 of the Adafruit FONA Library, written by Todd Treece for Adafruit
 Industries under MIT license.
*********************************************************************/
/**
   ----------------------------
   TFM - Blocksat Portable v1.0
   ----------------------------

   Sergio del Horno Torres
   ----------------------------
   BlocksatPortable_Main
   This is the Main tab.
   Contains setup() and loop() methods.
   This tab just call init methods in setup() and
   motor methods in loop().
   ----------------------------
   2018
*/

void setup()
{
  initSystem();
  initBLESystem();
  initFONASystem();

  initMainMotor();
  initGPS();
  initBLEComms();

  initGPRSComms();

  vibrateDevice(100, 2000);

}

void loop()
{

  receiveBLEData();
  receiveFromUDP();
  MainMotor();
  
}
