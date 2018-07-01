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
long TSend = SEND_UPDATE;
String TMinWork = WORK_TIME;
long trainDistance = TRAIN_DISTANCE;

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
  if ( millis() > T0_MainMotor + TSend)
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

  sendNAVToUDP();
  //sendToUDP("Hola");

  if (needToSend) {
    sendBatteryData();
    sendGPRSSignal();
    sendGPRSState();
  }
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
        sendToUDP(parameter + paramSeparator + values[0] + paramSeparator + TMinWork);
        sendBLEData("cts:wai");
      }
      else
      {
        // wrong request value
      }
    }
    else if (parameter == "cfg")
    {
      values[0].toCharArray(CTCIP, 20);
      CTCPort = atol(values[1].c_str());
      trainDistance = atol(values[2].c_str());
      TSend = atol(values[3].c_str()) * 1000;
      TMinWork = values[4];

      vibrateDevice(250, 1000);
    }
    else
    {
      // Message parameter not valid
    }
  }
  else if (header == CTCToDevHeader) //Message from CTC
  {
    if (parameter == "rqr")
    {
      if (values[0] == "str" && values [1] == "ok")
      {
        sendBLEData("cts:grn");
        sendToUDP("ops:wrk");
        sendBLEData("cts:wrk");
      }
      else if (values[0] == "str" && values [1] == "nok")
      {
        sendBLEData("cts:den");
        sendToUDP("ops:wai");
      }
      else if (values[0] == "stp" && values [1] == "ok")
      {
        sendBLEData("cts:end");
        sendToUDP("ops:wai");
      }
      else
      {
        // wrong request value
      }
    }
    else if (parameter == "wng")
    {
      vibrateDevice(100, 2000);
    }
    else if (parameter == "wrk")
    {
      if (values[0] == "end")
      {
        sendBLEData("cts:end");
        sendToUDP("ops:wai");
      }
      vibrateDevice(100, 2000);
    }
    else if (parameter == "trn")
    {
      if (atol(values[3].c_str()) < trainDistance)
      {
        vibrateDevice(100, 4000);
      }
      sendBLEData("ctt:" + values[0] + ";" + values[1] + ";" + values[2] + ";" + values[3]);
    }
    else
    {
      // Message parameter not valid
    }
  }

  header = "";
  parameter = "";
  values[0] = "";
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

