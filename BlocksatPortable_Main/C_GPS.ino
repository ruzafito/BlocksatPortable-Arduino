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
float latitude, longitude, speed_kph, heading, speed_mph, altitude;
String stateGPSToSend;

/**
 * Initializes the GPS states.
 */
void initGPS()
{
  stateGPSFix = false;
  stateGPSToSend = "search";
}

/**
 * Implements the read of the GPS data, includes latitude, longitude, speed,
 * heading and altitude.
 */
void readGPSData()
{
  // if you ask for an altitude reading, getGPS will return false if there isn't a 3D fix
  stateGPSFix = fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude);
}

/**
 * Read the GPS data check if GPS is fix and if data is available
 * format and send it to the App, else send to App that GPS is searching.
 */
void sendGPSData()
{
  readGPSData();
  
  if(stateGPSFix)
  {
    if (stateGPSToSend != "on")
    {
      stateGPSToSend = "on";
      sendBLEData("BP_gpssta:" + stateGPSToSend);
    }
    // GPS Fix. GPS Data available in global variables (latitude, longitude...)
    Serial.println("GPS Fix. GPS Data available: ");
    Serial.print("GPS lat:");
    Serial.println(latitude, 6);
    Serial.print("GPS long:");
    Serial.println(longitude, 6);
    Serial.print("GPS speed KPH:");
    Serial.println(speed_kph);
    Serial.print("GPS speed MPH:");
    speed_mph = speed_kph * 0.621371192;
    Serial.println(speed_mph);
    Serial.print("GPS heading:");
    Serial.println(heading);
    Serial.print("GPS altitude:");
    Serial.println(altitude);

    
    String lat = String(latitude);
    String lon = String(longitude);
    lat.remove(5);
    lon.remove(5);
    sendBLEData("BP_gpspos:" + lat + "," + lon);
  }
  else
  {
    // GPS No Fix. No GPS Data Available.
    Serial.println("GPS No Fix. No GPS Data Available.");
    Serial.println("Waiting for FONA GPS 3D fix...");
    if (stateGPSToSend != "search")
    {
      stateGPSToSend = "search";
      sendBLEData("BP_gpssta:" + stateGPSToSend);
    }
  }
}
