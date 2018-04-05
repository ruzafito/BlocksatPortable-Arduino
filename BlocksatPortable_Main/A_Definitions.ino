/**
   ----------------------------
   TFM - Blocksat Portable v1.0
   ----------------------------

   Sergio del Horno Torres
   ----------------------------
   A_Definitions
   In this tab are defined the constants used in other tabs.
   Definition of PINs, TIMEOUTs & Global Variables
   ----------------------------
   2018
*/

// Baud Rate
#define baudRate        115200

// ----Times definition----
#define WAIT_TIME       1000
#define GPS_UPDATE      3000
#define SEND_UPDATE     5000
#define TIMEOUT_ECHO    25000
#define GPRS_INIT_TIME  10000

//------------------------------------- FONA CONFIGURATION --------------------------------------
// FONA Pins
#define FONA_RX 10
#define FONA_TX 9
#define FONA_RST 6

//----------------------------------- BLUEFRUIT CONFIGURATION -----------------------------------
// COMMON SETTINGS
// ----------------------------------------------------------------------------------------------
// These settings are used in both SW UART, HW UART and SPI mode
// ----------------------------------------------------------------------------------------------
#define BUFSIZE                        128   // Size of the read buffer for incoming data
#define VERBOSE_MODE                   true  // If set to 'true' enables debug output

// HARDWARE UART SETTINGS
// ----------------------------------------------------------------------------------------------
// The following macros declare the HW serial port you are using. Uncomment
// this line if you are connecting the BLE to Leonardo/Micro or Flora
// ----------------------------------------------------------------------------------------------
  #define BLUEFRUIT_HWSERIAL_NAME      Serial1

// SHARED UART SETTINGS
// ----------------------------------------------------------------------------------------------
// The following sets the optional Mode pin, its recommended but not required
// ----------------------------------------------------------------------------------------------
#define BLUEFRUIT_UART_MODE_PIN        -1    // Set to -1 if unused
