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

void initGPRSComms()
{
  CTCDataAvailable = false;

  connectToUDP();

  /// TODO: If connection with CTC is OK. Send position and wait for ACK from CTC.

  fona.setPWM(1000, 80);
  delay(300);
  fona.setPWM(0, 80);
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

  //  Serial.print(F("RSSI = ")); Serial.print(n); Serial.print(": ");
  if (n == 0) rssi_dBm = -115;
  if (n == 1) rssi_dBm = -111;
  if (n == 31) rssi_dBm = -52;
  if ((n >= 2) && (n <= 30))
  {
    rssi_dBm = map(n, 2, 30, -110, -54);
  }
  //  Serial.print(rssi_dBm); Serial.println(F(" dBm"));
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
  fona.UDPconnect(UDP_IP, UDP_PORT);
}

/**
   Send a given message through the UDP connection
*/
void sendToUDP(String message)
{
  char inputs[BUFSIZE];
  message = headerSeparator + message;
  message = devToCTCHeader + message;
  message.toCharArray(inputs, message.length() + 1);

  fona.UDPsend(inputs, message.length() + 1);
}

/**
   Read messages received from UDP connection and save it in a global variable
*/
void receiveFromUDP()
{
  if (fona.UDPavailable())
  {
    uint8_t recv[21];
    Serial.println("UDP Data available");
    unsigned long lastRead = millis();
    while (millis() - lastRead < 5000)
    {
      while (fona.UDPavailable())
      {
        uint8_t r = fona.UDPread(recv, 20);
        recv[r] = 0;
        //      Serial.write((char *)recv);
        rxBuffer = (char *)recv;
        lastRead = millis();
      }
    }
    CTCMessageRx = String(rxBuffer);
    Serial.println("UDP received: " + CTCMessageRx);
  }
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


