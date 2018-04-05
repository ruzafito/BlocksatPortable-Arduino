/**
   ----------------------------
   TFM - Blocksat Portable v1.0
   ----------------------------

   Sergio del Horno Torres
   ----------------------------
   C_BLECommunications
   This tab contains the functions related with BLE module communications.
   Include the initialisation of the BLE communications with the mobile phone
   and the functions in charge of receive and send messages to the App. This
   functions include the ones that parses the message and for the ACK replies.

   Before execute the function of this tab is needed a correct initialization of 
   the BLE System.

   ----------------------------
   2018
*/

bool flagReceived;
bool flagParsed;
bool flagSendOk;

String message;

/**
 * Initializes the communication between the module and the App.
 */
void initBLEComms ()
{
  flagReceived = false;
  flagSendOk = false;

  Serial.println("Waiting for BLE phone connection...");
  /* Wait for connection */
  while (! ble.isConnected())
  {
      //wait 1 second to retry connection
      delay(WAIT_TIME);
  }

  Serial.println("Sending BLE init to App...");

  // attempt to init BLE device in App
  // Sending init messsage to App
  while (!sendBLEData ("init"))
  {
    delay (2 * WAIT_TIME);
  }

  Serial.print("Waiting for initOk response from App...   ");
  // Waiting for initOk response from App
  while (message != "initOk")
  {
    receiveBLEData();
    delay(WAIT_TIME);
    Serial.println("Reintentando init...");
    sendBLEData ("init");
  }
  Serial.println("OK");
  delay(WAIT_TIME);
  Serial.println("BLE device and App connected and initializated correctly...");
}

/**
 * Function to be executed on the loop().
 * 
 * First checks if the BLE App is still connected, if not try to reconnect.
 * If BLE App is connected, check for incoming messages, if there is an
 * incoming message this function parses it and send an ack message, if
 * needed.
 * Sets the flagReceived to true if there is a correct a message received.
 */
void receiveBLEData()
{
  // Check if the BLE App is still connected to the device
  if(! ble.isConnected())
  {
    Serial.println("BLE App disconnected. Trying to reconnect...");
    // If BLE App is disconnected, reset System and reconnect:
    resetStates();
  }
  // Check for incoming characters from Bluefruit
  ble.println("AT+BLEUARTRX");
  ble.readline();
  if (strcmp(ble.buffer, "OK") == 0)
  {
    // no data
    return;
  }
  // Some data was found, its in the buffer
  Serial.print(F("[Recv] ")); Serial.println(ble.buffer);
  char* myStrings[]={ble.buffer};
  if(ble.waitForOK())
  {
    flagReceived = true;
    // flagReceived is setted to false when is processed by MainMotor
  }

  // Get the message and save it in a global variable
  message = myStrings[0];

  parseMessage (message);

  // send an ACK reply
  replyACKMessage();
}

/**
 * This method send to the App over BLE interface a message.
 * @param message The string message to be sent.
 * @return Returns false if the failed to send the message, else 
 * return true.
 */
bool sendBLEData (String message)
{
  bool sendOk = false;
  char inputs[BUFSIZE + 1];
  message.toCharArray(inputs, message.length() + 1);
  
  // Send characters to Bluefruit
    Serial.print("[Send] ");
    Serial.println(inputs);

    ble.print("AT+BLEUARTTX=");
    ble.println(inputs);

    // check response stastus
    if (! ble.waitForOK() )
    {
      Serial.println(F("Failed to send?"));
      sendOk = false;
    }
    else
    {
      sendOk = true;
    }
    return sendOk;
}

/**
 * Send and ack to the App.
 */
void replyACKMessage()
{
  String ack = "ack";
  // send an ack reply to the connected App
  sendBLEData (ack);
}

/**
 * Parses a message according the communications protocol of the system
 * and set the flagParsed to true if is correctly parsed.
 * @param message The String message to be parsed.
 */
void parseMessage(String message)
{

/// TODO: Parse the message. Create Communications Protocol
  
//  int indexOfAct = message.indexOf("act:") + 4;
//  int indexOfPar = message.indexOf("par:") + 4;
//  int indexOfVal = message.indexOf("val:") + 4;
//
//  activity = message.substring(indexOfAct, message.indexOf("par:") - 1);
//  parameter = message.substring(indexOfPar, message.indexOf("val:") - 1);
//  value = message.substring(indexOfVal, message.length());
  flagParsed = true;

}

