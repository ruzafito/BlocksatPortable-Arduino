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

   Before execute the functions of this tab is needed a correct initialization of
   the FONA System.
   ----------------------------
   2018
*/

#ifndef DEBUG_GPRS_COMS
#define DEBUG_GPRS_COMS_NO

int8_t rssi_dBm;
String CTCMessageRx;
bool CTCDataAvailable;
char *rxBuffer;

char* CTCIP = UDP_IP;
long CTCPort = UDP_PORT;

void initGPRSComms()
{
  CTCDataAvailable = false;

  init_crc();

  connectToUDP();

}

/**
   Read GPRS State and send it directly to App
*/
void sendGPRSState()
{
  sendBLEData("grs:" + String(fona.GPRSstate()));
}

/**
   Implements the read of the signal level (RSSI) in dBm.
*/
void readGPRSSignal()
{
  //  Serial.println(F("Read the GPRS RSSI: "));
  // read the RSSI
  uint8_t n = fona.getRSSI();

  if (n == 0) rssi_dBm = -115;
  if (n == 1) rssi_dBm = -111;
  if (n == 31) rssi_dBm = -52;
  if ((n >= 2) && (n <= 30))
  {
    rssi_dBm = map(n, 2, 30, -110, -54);
  }
}

/**
   Read the GPRS data and send it to the App.
*/
void sendGPRSSignal()
{
  readGPRSSignal();

  String rssiToSend = String(rssi_dBm);

  sendBLEData("gri:" + rssiToSend);
}

/**
   Establish an UDP connection to the IP and PORT setted in definitions tab.
*/
void connectToUDP()
{
  fona.UDPconnect(CTCIP, CTCPort);
}

/**
   Send a given message through the UDP connection
*/
void sendToUDP(String message)
{
  char inputs[BUFSIZE + 1];
  message = headerSeparator + message;
  message = devToCTCHeader + message;
  message = message + endSeparator;
  message.toCharArray(inputs, message.length() + 1);

  fona.UDPsend(inputs, message.length() + 1);

  Serial.println("Send to CTC: " + message);

#if defined(DEBUG_GPRS_COMS)
  Serial.println("Received in CTC: " + message);
#endif
}

/**
 * Send NAV message to CTC via UDP.
 * This message contains the position of the train in the synoptic.
 */
void sendNAVToUDP()
{
  oblau_msg_t msg;

  //long pk = obtainPK(longitude, latitude);
  long pk = 1145;
  
  msg.train_id = convert_uint16(123);       // short
  msg.padding1[0] = 0;                      // char                 //!< Flag informing if message is encrypted or not.
  msg.padding1[1] = 0;                      // char
  msg.len = convert_uint16(2);              // short                //!< Segment occuppied (train length in the INIT message)
  msg.msg_type = 2;                         // char
  msg.pad1 = 0xB1;                          // char
  msg.sequence = convert_uint32(3);         // long                 // Incrementar en cada mensaje NAV
  msg.acknowledge = convert_uint32(4);      // long
  msg.pk = convert_uint32(5200);            // long
  msg.pk_assigned = convert_uint32(pk);     // long
  msg.track = convert_uint16(0);            // short
  msg.wom = 1;                              // char
  msg.pad2 = 0xB2;                          // char
  msg.spd = convert_uint16(161);            // short
  msg.pad3 = 0xB3;                          // char
  msg.pad4 = 0xB4;                          // char
  msg.stat = 0;                             // char                 /// DEPRECATED: set to 0 allways
  msg.alarm_id = -1;                        // char                 /// DEPRECATED: set to -1 allways
  msg.mode = 0;                             // char
  msg.role = 0;                             // char
  msg.alarm = convert_uint32(1);            // long                 //!<  alarm mask. Each bit identifies an alarm.
  msg.seconds = convert_uint32(2);          // long
  msg.n_sec = convert_uint32(3);            // long
  msg.checksum = convert_uint32(get_msg_checksum((char *) &msg, sizeof(msg)));     // long
  
  fona.UDPsend((char*) &msg, sizeof(msg));
}

/**
   Read messages received from UDP connection and save it in a global variable
*/
void receiveFromUDP()
{

  char inputs[BUFSIZE + 1];

  fona.UDPread(inputs, BUFSIZE + 1);

  CTCMessageRx = String(inputs);

  //if ( getUserInput(inputs, BUFSIZE) )
  if (CTCMessageRx.length() > 4)
  {
#if defined(DEBUG_GPRS_COMS)
    Serial.println("UDP received: " + CTCMessageRx);
#endif

    int endIndex = CTCMessageRx.indexOf("/");

    if (endIndex > 0)
    {
      CTCMessageRx.remove(endIndex);
      parseMessage (CTCMessageRx);
      CTCMessageRx = "";
    }
  }
}

//bool getUserInput(char buffer[], uint8_t maxSize)
//{
//  // timeout in 100 milliseconds
//  TimeoutTimer timeout(100);
//
//  memset(buffer, 0, maxSize);
//  while ( (!Serial.available()) && !timeout.expired() ) {
//    delay(1);
//  }
//
//  if ( timeout.expired() ) return false;
//
//  delay(2);
//  uint8_t count = 0;
//  do
//  {
//    count += Serial.readBytes(buffer + count, maxSize);
//    delay(2);
//  } while ( (count < maxSize) && (Serial.available()) );
//
//  return true;
//}

/**
   Close an open UDP connection
*/
void closeUDPConnection()
{
  fona.UDPclose();
}

/**
   Enables or disables the sleep mode of the UDP connection
*/
void sleepMode(bool onoff)
{
  if (onoff)
  {
    fona.println("AT+CSCLK=1");
  }
  else
  {
    fona.println("AT+CSCLK=0");
  }
}

#endif
