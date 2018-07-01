/**
   ----------------------------
   TFM - Blocksat Portable v1.0
   ----------------------------

   Sergio del Horno Torres
   ----------------------------
   C_BLE
   This tab contains the functions related with BLE module communications.
   Include the initialisation of the BLE communications with the mobile phone
   and the functions in charge of receive and send messages to the App.

   Before execute the functions of this tab is needed a correct initialization of
   the BLE System.

   ----------------------------
   2018
*/

#ifndef DEBUG_BLE_COMS
#define DEBUG_BLE_COMS_NO

bool needToSend = true;

/**
   Initializes the communication between the module and the App.
*/
void initBLEComms ()
{
  flagReceived = false;
  flagParsed = false;

#if defined(DEBUG_BLE_COMS)
  Serial.println("Waiting for BLE phone connection...");
#endif

  /* Wait for connection */
  while (! ble.isConnected())
  {
    //wait 1 second to retry connection
    delay(WAIT_TIME);
  }

#if defined(DEBUG_BLE_COMS)
  Serial.println("Sending BLE init to App...");
#endif
  String initMessage = "ini:ok";

  // attempt to init BLE device in App
  // Sending init messsage to App
  while (!sendBLEData (initMessage))
  {
#if defined(DEBUG_BLE_COMS)
    Serial.println("error Sending... Trying again");
#endif
    delay (2 * WAIT_TIME);
  }

  receiveBLEData();
#if defined(DEBUG_BLE_COMS)
  Serial.print("Waiting for initOk response from App...   ");
#endif
  // Waiting for initOk response from App
  while (flagParsed != true && parameter != "ini" && values[0] != "ok" )
  {
    receiveBLEData();
    delay(2 * WAIT_TIME);
#if defined(DEBUG_BLE_COMS)
    Serial.println("Reintentando init...");
#endif
    sendBLEData (initMessage);
  }
#if defined(DEBUG_BLE_COMS)
  Serial.println("OK");
#endif
  delay(WAIT_TIME);
#if defined(DEBUG_BLE_COMS)
  Serial.println("BLE device and App connected and initializated correctly...");
#endif
  needToSend = true;
  flagReceived = false;
  flagParsed = false;

  vibrateDevice(250, 2000);
}

/**
   Function to be executed on the loop().

   First checks if the BLE App is still connected, if not try to reconnect.
   If BLE App is connected, check for incoming messages, if there is an
   incoming message this function parses it and send an ack message, if
   needed.
   Sets the flagReceived to true if there is a correct a message received.
*/
void receiveBLEData()
{
  // Check if the BLE App is still connected to the device
  if (! ble.isConnected())
  {
#if defined(DEBUG_BLE_COMS)
    if (needToSend) {
      Serial.println("BLE App disconnected. Trying to reconnect...");
    }
#endif
    // If BLE App is disconnected, reset System and reconnect:
    //resetStates();
    needToSend = false;
  } else if (!needToSend){
    needToSend = true;
    initBLEComms();
  }
  
  if (needToSend) {
    // Check for incoming characters from Bluefruit
    ble.println("AT+BLEUARTRX");
    ble.readline();
    if (strcmp(ble.buffer, "OK") == 0)
    {
      // no data
      return;
    }
    // Some data was found, its in the buffer
#if defined(DEBUG_BLE_COMS)
    Serial.print(F("[Recv] ")); Serial.println(ble.buffer);
#endif
    char* myStrings[] = {ble.buffer};
    if (ble.waitForOK())
    {
      flagReceived = true;
      // flagReceived is setted to false when is processed by MainMotor
    }

    String textToAdd = myStrings[0];

    // Get the message and save it in a global variable
    message = message + textToAdd;

    int endIndex = message.indexOf("/");

    if (endIndex > 0)
    {
      message.remove(endIndex);
      parseMessage (message);
      message = "";
    }
  }
}

/**
   This method send to the App over BLE interface a message.
   @param message The string message to be sent.
   @return Returns false if the failed to send the message, else
   return true.
*/
bool sendBLEData (String message)
{
  bool sendOk = false;
  char inputs[BUFSIZE + 1];
  message = headerSeparator + message;
  message = devToAppHeader + message;
  message = message + endSeparator;
  message.toCharArray(inputs, message.length() + 1);

  // Send characters to Bluefruit
#if defined(DEBUG_BLE_COMS)
  Serial.print("[Send] ");
  Serial.println(inputs);
#endif

  ble.print("AT+BLEUARTTX=");
  ble.println(inputs);

  // check response stastus
  if (! ble.waitForOK() )
  {
#if defined(DEBUG_BLE_COMS)
    Serial.println(F("Failed to send?"));
#endif
    sendOk = false;
  }
  else
  {
    sendOk = true;
  }
  return sendOk;
}
#endif
