/**
   ----------------------------
   TFM - Blocksat Portable v1.0
   ----------------------------

   Sergio del Horno Torres
   ----------------------------
   C_GPS
   This tab contains the functions in charge of read the GPS data
   from the module FONA, and send it to the App. The GPS data includes
   the position in 2D (latitude & longitude) and the state of the GPS fix.

   ----------------------------
   2018
*/

bool stateGPSFix;
//float longitude, latitude;
float speed_kph, heading, speed_mph, altitude;
String stateGPSToSend;

/**
   Initializes the GPS states.
*/
void initGPS()
{
  stateGPSFix = false;
  stateGPSToSend = "sch";
}

/**
   Implements the read of the GPS data, includes latitude, longitude, speed,
   heading and altitude.
*/
void readGPSData()
{
  // if you ask for an altitude reading, getGPS will return false if there isn't a 3D fix
  stateGPSFix = fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude);
}

/**
   Read the GPS data check if GPS is fix and if data is available
   format and send it to the App and CTC, else send to App that GPS is searching.
*/
void sendGPSData()
{
  readGPSData();

  if (stateGPSFix)
  {
    if (stateGPSToSend != "on")
    {
      stateGPSToSend = "on";
      if (needToSend) {
        sendBLEData("gss:" + stateGPSToSend);
      }
    }
    // GPS Fix. GPS Data available in global variables (latitude, longitude...)

    String lat = String(latitude, 6);
    String lon = String(longitude, 6);
    if (needToSend) {
      // Sending GPS data to App
      sendBLEData("gsp:" + lat + "," + lon);
    }
    // Sending GPS data to CTC
    sendToUDP("gsp:" + lat + "," + lon);
  }
  else
  {
    // GPS No Fix. No GPS Data Available.
    if (stateGPSToSend != "sch")
    {
      stateGPSToSend = "sch";
      if (needToSend) {
        sendBLEData("gss:" + stateGPSToSend);
      }
    }
  }
}
