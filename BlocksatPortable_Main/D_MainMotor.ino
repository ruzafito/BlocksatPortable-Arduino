/**
   ----------------------------
   TFM - Blocksat Portable v1.0
   ----------------------------

   Sergio del Horno Torres
   ----------------------------
   MainMotor
   This tab contains the Main Motor and the derivate functions of this Motor.
   The Main Motor is in charge of handle the received messages and throw the
   functionality that the application is demanding.
   ----------------------------
   2018
*/

int mainState, prevMainState;
long T0_MainMotor;

/**
   Initializes the states and the timers of the Main Motor.
*/
void initMainMotor()
{
  mainState = 0;
  prevMainState = 0;
  T0_MainMotor = 0;
}

/**
   Manage the cases and state of the system.
*/
void MainMotor()
{

  switch (mainState)
  {
    case 0:
      sendStatesToApp();
      T0_MainMotor = millis();
      if (prevMainState == 0)
      {
        prevMainState++;
      }
      mainState = prevMainState;
      break;
    case 1:
      if (flagReceived)
      {
        mainState ++;
        flagReceived = false;
      }
      waitForSendUpdate();
      break;
    case 2:
      if (flagParsed)
      {
        mainState++;
        flagParsed = false;
      }
      waitForSendUpdate();
      break;
    case 3:
      MessageHandler();
      waitForSendUpdate();
      break;
  }
}

/**
   Auxiliary function in charge of wait preconfigured time (SEND_UPDATE) to send
   a new update of the device data to the App.
*/
void waitForSendUpdate()
{
  prevMainState = mainState;
  if ( millis() > T0_MainMotor + SEND_UPDATE)
  {
    mainState = 0;
  }
  else
  {
    mainState = prevMainState;
  }
}

/**
   Send the system states to the App.
   Include the states of the GPS, Battery and GPRS subsystems.
*/
void sendStatesToApp()
{
  sendGPSData();
  sendBatteryData();
  sendGPRSSignal();
  sendGPRSState();
}

/**
   Manage the actions to do depending on the parsed message from App.
*/
void MessageHandler()
{
  if (header == appToDevHeader) // Message from App
  {
    if (parameter == "req")
    {
      if (values[0] == "str" || values[0] == "stp" || values[0] == "pse")
      {
        /// TODO: Send to CTC a request with value
        sendToUDP("");
      }
      else
      {
        // wrong request value
      }
    }
    else if (parameter == "cfg")
    {
      // TODO: change device configuration
    }
    else
    {
      // Message parameter not valid
    }
  }
  else if (header == CTCToDevHeader) //Message from CTC
  {
    
  }
}

/**
   Reset the states of the subsystems.
*/
void resetStates()
{
  initMainMotor();
  initGPS();
  initBLEComms();

}

