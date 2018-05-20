/**
   ----------------------------
   TFM - Blocksat Portable v1.0
   ----------------------------

   Sergio del Horno Torres
   ----------------------------
   B_Init
   This tab contains initialization methods needed
   for the proper run of the whole program.
   ----------------------------
   2018
*/

#ifndef DEBUG_INIT
#define DEBUG_INIT

#if defined(DEBUG_INIT)
// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}
#endif

//------------------------------------- BASIC INIT --------------------------------------
/**
 * Set baud rate of the Serial and print version information.
 */
void initSystem (){
  while (!Serial); // required for Flora & Micro
  delay(500);
  
#if defined(DEBUG_INIT)
  Serial.begin(baudRate);
  Serial.println(F("BLOCKSAT Portable v1.2 02/04/2018"));
  Serial.println(F("---------------------------------------------------"));
  Serial.println();
#endif
  randomSeed(micros());
}

//------------------------------------- BLE INIT --------------------------------------
#include "Adafruit_BluefruitLE_UART.h"

Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/**
 * Initializes the BLE Systems in the device.
 * Factory reset of BLE Module, print BLE module info and set name device
 */
void initBLESystem()
{
  //#if defined(DEBUG_INIT)
  /* Initialise the BLE module */
  Serial.println(F("Initialising the Bluefruit LE module: "));
  //#endif
  
  if ( !ble.begin(VERBOSE_MODE) )
  {
    #if defined(DEBUG_INIT)
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
    #endif
  }
  #if defined(DEBUG_INIT)
  Serial.println( F("OK!") );

  /* Perform a factory reset to make sure everything is in a known state */
  Serial.println(F("Performing a factory reset: "));
  #endif
  if (! ble.factoryReset() )
  {
       #if defined(DEBUG_INIT)
       error(F("Couldn't factory reset."));
       #endif
  }


  /* Disable command echo from Bluefruit */
  ble.echo(false);
  
//#if defined(DEBUG_INIT)
//  Serial.println("Requesting Bluefruit info:");
//  /* Print Bluefruit information */
//  ble.info();
//#endif

  // this line is particularly required for Flora, but is a good idea
  // anyways for the super long lines ahead!
   ble.setInterCharWriteDelay(5); // 5 ms

#if defined(DEBUG_INIT)
  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'Blocksat Portable': "));
#endif

  if (! ble.sendCommandCheckOK(F("AT+GAPDEVNAME=Blocksat Portable")) )
  {
    #if defined(DEBUG_INIT)
    error(F("Could not set device name?"));
    #endif
  }


  Serial.println(F("BLE Module Initializated... OK"));
#if defined(DEBUG_INIT)
  Serial.println();
  Serial.println();
  Serial.println(F("---------------------------------------------------"));
#endif  
  ble.verbose(false);  // debug info is a little annoying after this point!
}

//------------------------------------- FONA INIT --------------------------------------
#include "Adafruit_FONA.h"

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t type;

/**
 * Set FONA Serial baud rate, print FONA module info, set FONA GPRS APN and
 * enable GPRS and GPS subsystems
 */
void initFONASystem()
{
  
  Serial.println(F("Initialising the FONA GSM + GPS module: "));
  #if defined(DEBUG_INIT)
  Serial.println(F("Initializing....(May take 3 seconds)"));
  #endif
  
  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial))
  {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }

//#if defined(DEBUG_INIT)
//  type = fona.type();
//  Serial.println(F("FONA is OK"));
//  Serial.print(F("Found "));
//  switch (type)
//  {
//    case FONA800L:
//      Serial.println(F("FONA 800L")); break;
//    case FONA800H:
//      Serial.println(F("FONA 800H")); break;
//    case FONA808_V1:
//      Serial.println(F("FONA 808 (v1)")); break;
//    case FONA808_V2:
//      Serial.println(F("FONA 808 (v2)")); break;
//    case FONA3G_A:
//      Serial.println(F("FONA 3G (American)")); break;
//    case FONA3G_E:
//      Serial.println(F("FONA 3G (European)")); break;
//    default: 
//      Serial.println(F("???")); break;
//  }
//
//  // Print module IMEI number.
//  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
//  uint8_t imeiLen = fona.getIMEI(imei);
//  if (imeiLen > 0)
//  {
//    Serial.print("Module IMEI: "); Serial.println(imei);
//  }
//#endif

  // Configure a GPRS APN, username, and password.
  // You might need to do this to access your network's GPRS/data
  // network.  Contact your provider for the exact APN, username,
  // and password values.  Username and password are optional and
  // can be removed, but APN is required.
  fona.setGPRSNetworkSettings(F("tuenti.com"), F("tuenti"), F("tuenti"));
  
  Serial.println(F("Turning on GPRS... (May take 10 seconds)"));
  delay(GPRS_INIT_TIME); //Wait for a long time while the module is registering to the network before of enable GPRS
  // turn GPRS on
  if (!fona.enableGPRS(true))
  {
    Serial.println(F("Failed to turn on GPRS"));
  }
  
  delay(200);

  Serial.println(F("Turning on GPS..."));
  // turn GPS on
  if (!fona.enableGPS(true))
  {
    Serial.println(F("Failed to turn on GPS"));
  }

  Serial.println(F("FONA Module Initializated... OK"));
#if defined(DEBUG_INIT)  
  Serial.println(F("---------------------------------------------------"));
  Serial.println();
  Serial.println();
#endif
}

#endif /*DEBUG_INIT*/
