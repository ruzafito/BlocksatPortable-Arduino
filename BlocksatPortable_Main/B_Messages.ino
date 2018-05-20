/**
   ----------------------------
   TFM - Blocksat Portable v1.0
   ----------------------------

   Sergio del Horno Torres
   ----------------------------
   B_Messages
   This tab contains the functions related with management of messages.
   Includes the parse of the messages received from App or CTC.
   ----------------------------
   2018
*/

#ifndef DEBUG_COMS
#define DEBUG_COMS

bool flagReceived;
bool flagParsed;

String message = "";
String header;
String parameter;
String values[VALUES_LIMIT];

/**
 * Parses a message according the communications protocol of the system
 * and set the flagParsed to true if is correctly parsed.
 * @param message The String message to be parsed.
 */
void parseMessage(String message)
{
  int indexOfHeaderSep = message.indexOf(headerSeparator);
  int indexOfParSep = message.indexOf(paramSeparator);

  header = message.substring(0, indexOfHeaderSep);
  parameter = message.substring(indexOfHeaderSep + 1, indexOfParSep);
  
  String allValues = message.substring(indexOfParSep + 1, message.length());
  int indexOfValSep = allValues.indexOf(valueSeparator);

  while (allValues.length() > 1)
  {
    int i = 0;
    if (indexOfValSep == -1)
    {
      values[i] = allValues;
      break;
    }
    else
    {
      values[i] = allValues.substring(0, indexOfValSep);
      allValues.remove(0, indexOfValSep + 1);
    }
    i++;
    delay(WAIT_TIME);
  }
  
  if (header.length() > 0 && parameter.length() > 0 && values[0].length() > 0)
  {
    flagParsed = true;
    
    // send an ACK reply
    sendBLEData("ack");
  }
  else
  {
    flagParsed = false;

    // send an NACK reply
    sendBLEData("nack");
  }
}

#endif
