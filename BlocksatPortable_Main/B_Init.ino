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

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void initSystem ()
{
  while (!Serial); // required for Flora & Micro
  delay(500);

  boolean success;

  Serial.begin(115200);
  Serial.println(F("BLOCKSAT Portable v1.0 24/03/2018"));
  Serial.println(F("---------------------------------------------------"));

  randomSeed(micros());

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  /* Perform a factory reset to make sure everything is in a known state */
  Serial.println(F("Performing a factory reset: "));
  if (! ble.factoryReset() ){
       error(F("Couldn't factory reset"));
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  // this line is particularly required for Flora, but is a good idea
  // anyways for the super long lines ahead!
   ble.setInterCharWriteDelay(5); // 5 ms

  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'Blocksat Portable': "));

  if (! ble.sendCommandCheckOK(F("AT+GAPDEVNAME=Blocksat Portable")) ) {
    error(F("Could not set device name?"));
  }

  /* Add battery service */
  Serial.println(F("Adding battery service: "));

  if (! ble.sendCommandCheckOK(F("AT+GATTADDSERVICE=UUID=0x180F")) ) {
    error(F("Could not add battery service"));
  }

  /* Add battery measurement characteristic */
  Serial.println(F("Adding battery measurement characteristic: "));

  if (! ble.sendCommandCheckOK(F("AT+GATTADDCHAR=UUID=0x2A19,PROPERTIES=0x10,MIN_LEN=1,VALUE=100")) ) {
    error(F("Could not add battery measurement characteristic"));
  }

  /* Add location service */
  Serial.println(F("Adding location service: "));

  if (! ble.sendCommandCheckOK(F("AT+GATTADDSERVICE=UUID=0x1819")) ) {
    error(F("Could not add location service"));
  }

  /* Add 2D position characteristic */
  Serial.println(F("Adding 2D position characteristic: "));

  if (! ble.sendCommandCheckOK(F("AT+GATTADDCHAR=UUID=0x2A2F,PROPERTIES=0x10,MIN_LEN=1,VALUE=1")) ) {
    error(F("Could not add 2D position characteristic"));
  }

  /* Add tx power service */
  Serial.println(F("Adding tx power service: "));

  if (! ble.sendCommandCheckOK(F("AT+GATTADDSERVICE=UUID=0x1804")) ) {
    error(F("Could not add tx power service"));
  }

  /* Add tx power level characteristic */
  Serial.println(F("Adding tx power level characteristic: "));

  if (! ble.sendCommandCheckOK(F("AT+GATTADDCHAR=UUID=0x2A07,PROPERTIES=0x02,MIN_LEN=1,VALUE=1")) ) {
    error(F("Could not add tx power level characteristic"));
  }

  /* Add custom GSM service */
  Serial.println(F("Adding custom GSM service: "));

  if (! ble.sendCommandCheckOK(F("AT+GATTADDSERVICE=UUID128=00-11-00-11-44-55-66-77-88-99-AA-BB-CC-DD-EE-FF")) ) {
    error(F("Could not add custom GSM service"));
  }

  /* Add custom GSM state characteristic */
  Serial.println(F("Adding custom GSM state characteristic: "));

  if (! ble.sendCommandCheckOK(F("AT+GATTADDCHAR=UUID=0x0002,PROPERTIES=0x02,MIN_LEN=1,VALUE=100")) ) {
    error(F("Could not add custom GSM state characteristic"));
  }

  /* Add custom GSM signal characteristic */
  Serial.println(F("Adding custom GSM signal characteristic: "));

  if (! ble.sendCommandCheckOK(F("AT+GATTADDCHAR=UUID=0x0003,PROPERTIES=0x02,MIN_LEN=1,VALUE=100")) ) {
    error(F("Could not add custom GSM signal characteristic"));
  }

  /* Add custom GSM rx message characteristic */
  Serial.println(F("Adding custom GSM rx message characteristic: "));

  if (! ble.sendCommandCheckOK(F("AT+GATTADDCHAR=UUID=0x0004,PROPERTIES=0x02,MIN_LEN=1,VALUE=100")) ) {
    error(F("Could not add custom GSM rx message characteristic"));
  }

  /* Add custom GSM tx message characteristic */
  Serial.println(F("Adding custom GSM tx message characteristic: "));

  if (! ble.sendCommandCheckOK(F("AT+GATTADDCHAR=UUID=0x0005,PROPERTIES=0x08,MIN_LEN=1,VALUE=100")) ) {
    error(F("Could not add custom GSM tx message characteristic"));
  }

  /* Get a list of all custom GATT services and characteristics on the device */
  Serial.println(F("Getting a list of all custom GATT services and characteristics on the device: "));

  if (! ble.sendCommandCheckOK(F("AT+GATTLIST")) ) {
    error(F("Could not get a list"));
  }

  /* Reset the device for the new service setting changes to take effect */
  Serial.print(F("Performing a SW reset (service changes require a reset): "));
  ble.reset();

  Serial.println();
}
