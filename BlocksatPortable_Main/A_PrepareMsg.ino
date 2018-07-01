/**
   ----------------------------
   TFM - Blocksat Portable v1.0
   ----------------------------

   Sergio del Horno Torres
   ----------------------------
   A_PrepareMsg
   In this tab are defined the struct of a message to the CTC
   and the required functions to send a message in order that
   the CTC can understand it correctly.
   This code is imported from the Blocksat Project developed
   at SENER Company and is under its rights.
   ----------------------------
   2018
*/

//------------------------------------- STRUCT DEFINITION --------------------------------------

#include <time.h>

/// @brief OBLAU message sent to CTC.
/// @remarks The OBLAU message size is compiler-aligned to
/// 16bits. It means that the "real" message size is not the summary of
/// bytes in the definition but the closer value aligned to word (16 bits).
typedef struct oblau_msg_t
{                        // POS LEN
    short train_id;      //  0 2         
    char padding1[2];    //  2 2            //!< Flag informing if message is encrypted or not.
    
    short len;           //  4 2            //!< Train length (in the INIT message); later on, the segment occuppied.
    
    char msg_type;       //  6 1
    char pad1;           //  7 1
    
    long sequence;       //  8 4
    long acknowledge;    // 12 4

    long pk;             // 16 4
    long pk_assigned;    // 20 4
    short track;         // 24 2
    
    char wom;            // 26 1
    char pad2;           // 27 1
    
    short spd;           // 28 2
    char pad3;           // 30 1
    char pad4;           // 31 1
    
    char stat;           // 32 1            /// DEPRECATED: set to 0 allways   
    char alarm_id;       // 33 1             /// DEPRECATED: set to -1 allways
    char mode;           // 34 1
    char role;           // 35 1
        
    long alarm;          // 36 4            //!<  alarm mask. Each bit identifies an alarm.
    time_t seconds;      // 40 4
    long n_sec;          // 44 4
    long checksum;       // 48 4
                         // 52
} oblau_msg_t;


//------------------------------------- CONVERT BYTES --------------------------------------

/// @brief Short Integer big endian to little endian conversion
/// @param val Short integer to be converted
/// @returns converted integer
short convert_uint16(short val)
{
    short res;

    res = ((val & 0xff00) >> 8) | ((val & 0x00ff) << 8);

    return res;
}

/// @brief Long Integer big endian to little endian conversion
/// @param val Long integer to be converted
/// @returns converted integer
long convert_uint32(long val)
{
    long res;

    res = (val >> 24) & 0xFF;
    res += (val >> 8) & 0xFF00;
    res += (val << 8) & 0xFF0000;
    res += (val << 24) & 0xFF000000;
    return res;
}

//------------------------------------- CRC MANAGEMENT --------------------------------------

static long crc_tab32[256];
#define CRC_SIZE sizeof(long);
#define  P_32   0xEDB88320L

void init_crc(void)
{
    int i, j;
    long crc;

    for (i = 0; i < 256; i++)
    {
        crc = (long) i;

        for (j = 0; j < 8; j++)
        {
            if (crc & 0x00000001L)
            {
                crc = (crc >> 1) ^ P_32;
            }
            else
            {
                crc = crc >> 1;
            }
        }
        crc_tab32[i] = crc;
    }
}

/// @brief Auxiliary CRC32 computing function
/// @param crc checksum variable
/// @param c character summed to the checksum crc
/// @return CRC32 checksum
long crc_32(long crc, char c)
{
    long tmp, long_c;

    long_c = 0x000000ffL & (long) c;

    tmp = crc ^ long_c;
    crc = (crc >> 8) ^ crc_tab32[tmp & 0xff];

    return crc;
}

long get_msg_checksum(char *msg, long msg_size)
{
    long checksum = 0xffffffffL;
    int i = 0;

    msg_size = msg_size - CRC_SIZE;

    for (i = 0; i < msg_size; i++)
    {
        checksum = crc_32(checksum, msg[i]);
    }
    return checksum;
}

