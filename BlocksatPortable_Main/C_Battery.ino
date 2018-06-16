/**
   ----------------------------
   TFM - Blocksat Portable v1.0
   ----------------------------

   Sergio del Horno Torres
   ----------------------------
   C_Battery
   This tab contains the functions in charge of read the battery level
   from the module FONA (where the battery is connected and supplies
   all the device), and send it to the App.

   ----------------------------
   2018
*/

uint16_t vbat;
String batPerc;

/**
   Implements the read of the battery level of the device in percentege.
*/
void readBatteryData()
{
  // read the battery percentage
  if (! fona.getBattPercent(&vbat))
  {
    batPerc = "NA";
  } else
  {
    batPerc = String(vbat);
  }
}

/**
   Read the battery level and send it to the App.
*/
void sendBatteryData()
{
  readBatteryData();
  sendBLEData("blv:" + batPerc);
}

