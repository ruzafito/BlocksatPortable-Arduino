/**
   ----------------------------
   TFM - Blocksat Portable v1.0
   ----------------------------

   Sergio del Horno Torres
   ----------------------------
   C_Vibration
   This tab contains the function in charge of send an order to the vibration
   motor of the FONA System.

   Before execute the functions of this tab is needed a correct initialization of 
   the FONA System.
   ----------------------------
   2018
*/

/**
   Program the motor to vibrate during vibratingTime with a given period in millis
*/
void vibrateDevice(uint16_t period, uint16_t vibratingTime)
{
  unsigned long lastRead1 = millis();
  unsigned long lastRead2 = millis();
  bool on = true;

  fona.setPWM(1000, 80);

  while (millis() < lastRead1 + vibratingTime)
  {
    if (millis() > lastRead2 + period)
    {
      if (on)
      {
        on = false;
        fona.setPWM(0);
      }
      else
      {
        on = true;
        fona.setPWM(1000, 80);
      }
      lastRead2 = millis();
    }
  }
  on = false;
  fona.setPWM(0);
}

