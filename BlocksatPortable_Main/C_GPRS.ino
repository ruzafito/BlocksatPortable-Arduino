/**
   ----------------------------
   TFM - Blocksat Portable v1.0
   ----------------------------

   Sergio del Horno Torres
   ----------------------------
   C_GPRS
   This tab contains the functions in charge of read the GPRS data
   from the module FONA, and send it to the App. The GPRS data includes
   the signal level (RSSI) and the state of the network.

   ----------------------------
   2018
*/

int8_t rssi_dBm;

/// TODO: Read GPRS state.

/**
 * Implements the read of the signal level (RSSI) in dBm.
 */
void readGPRSData()
{
  Serial.println(F("Read the GPRS RSSI: "));
  // read the RSSI
  uint8_t n = fona.getRSSI();

  Serial.print(F("RSSI = ")); Serial.print(n); Serial.print(": ");
  if (n == 0) rssi_dBm = -115;
  if (n == 1) rssi_dBm = -111;
  if (n == 31) rssi_dBm = -52;
  if ((n >= 2) && (n <= 30))
  {
    rssi_dBm = map(n, 2, 30, -110, -54);
  }
  Serial.print(rssi_dBm); Serial.println(F(" dBm"));
}

/**
 * Read the GPRS data and send it to the App.
 */
void sendGPRSData()
{
  readGPRSData();

  String rssiToSend = String(rssi_dBm);

  sendBLEData("BP_gprssi:" + rssiToSend);

}
