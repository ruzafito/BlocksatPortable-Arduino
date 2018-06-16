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

int8_t rssi_dBm;
String CTCMessageRx;
bool CTCDataAvailable;
char *rxBuffer;

char* CTCIP = UDP_IP;
long CTCPort = UDP_PORT;

void initGPRSComms()
{
  CTCDataAvailable = false;

  connectToUDP();

  /// TODO: If connection with CTC is OK. Send position and wait for ACK from CTC.

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
  //char inputs[BUFSIZE];
  message = headerSeparator + message;
  message = devToCTCHeader + message;
  message = message + endSeparator;

  Serial.println("Received in CTC: " + message);
}

/**
   Read messages received from UDP connection and save it in a global variable
*/
void receiveFromUDP()
{

  char inputs[BUFSIZE + 1];

  if ( getUserInput(inputs, BUFSIZE) )
  {
    CTCMessageRx = String(inputs);
    Serial.println("UDP received: " + CTCMessageRx);
    int endIndex = CTCMessageRx.indexOf("/");

    if (endIndex > 0)
    {
      CTCMessageRx.remove(endIndex);
      parseMessage (CTCMessageRx);
      CTCMessageRx = "";
    }
  }
}

bool getUserInput(char buffer[], uint8_t maxSize)
{
  // timeout in 100 milliseconds
  TimeoutTimer timeout(100);

  memset(buffer, 0, maxSize);
  while ( (!Serial.available()) && !timeout.expired() ) {
    delay(1);
  }

  if ( timeout.expired() ) return false;

  delay(2);
  uint8_t count = 0;
  do
  {
    count += Serial.readBytes(buffer + count, maxSize);
    delay(2);
  } while ( (count < maxSize) && (Serial.available()) );

  return true;
}

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


