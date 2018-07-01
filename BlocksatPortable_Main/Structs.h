/// @file Structs.h
/// @brief Set of system structures definition.
///
/// Global definitions of variables and data types spread around the code.
/// @ingroup DATA_STRUCTURES

#ifndef STRUCTS_H
#define STRUCTS_H

#include "tl_basetypes.h"
#include "Lib/crc.h"
#include <inetLib.h>
#include <time.h>

// ENUMERATIONS
#define NO_PK -1

/// @name Cab identifiers
///
///@{
enum cabin_ids
{
	CABIN_1	= 1,
//!< Flag used to identify the cabin 1.
	CABIN_2	= 0
//!< Flag used to identify the cabin 2.
};
///@}

/// @name Message types
///
/// Define the identifier of the different messages exchanged between the OBLAU 
/// and the CTC.
///@{
#define ACK     0   	//!< Message received accepted.
#define INIT    1   	//!< Initialize OBLAU in the CTC.
#define NAV     2   	//!< OBLAU navigation data sent to CTC.
#define CANCEL  3   	//!< Request of itinerary cancelation. 
#define NACK    4   	//!< Message received not accepted.
#define CONFIRM 5   	//!< Confirm reception of itinerary at OBLAU.
#define DEST_REACH 6	//!< Message sent from OBLAU once arrives to destination.
#define TRAFFIC 7   	//!< OBLAU requests for an itinerary
#define PROCEED 8		
#define INIT_REQ 9
#define NAV_ACK 10
#define CANCEL_TRAIN_REQUEST 11
#define CANCEL_TRAIN_ACK 12
#define CANCEL_TRAIN_REJECTION 13
#define TRAFFIC_PETITION 14
#define CHANGE_HMI_OK 15
#define UNKNOWN_MSG_TYPE 16
///@}

/// @name OBLAU status
/// @deprecated
///@{
typedef Int8 train_status_t; 
//!< OBLAU status type definition.
#define OBLAU_OK    0
//!< Everything is going on properly in the OBLAU. 
#define COMM_LOST   1
//!< Communications lost (between who???) in the OBLAU.
///@}


/// @name Alarms identifiers
/// Identifiers of all alarm types.
///@{
typedef Int8 alarm_t;
//!< OBLAU alarm type definition.

#define NO_ALARM -1
//!< No alarm.

// A ALARMS

#define ALARM_OUT_OF_ITINERARY      0
//!< Out of itinerary identifier.
#define ALARM_NO_AUTHORISATION      1
#define ALARM_NO_ITINERARY          2
#define ALARM_PK_OVERTAKING         3
#define ALARM_SIDING_POINT          4
#define ALARM_WAYOFMOTION           5
#define ALARM_TRAIN_INTEGRITY       6
#define ALARM_GYRO_INFO             7
#define ALARM_WATCHDOG              8
#define ALARM_INIT_FAILED           9
#define ALARM_POSITION_LOST         10
#define ALARM_SEVERE_FAILURE        11

/// B ALARMS
#define ALARM_TRAIN_OVERVELOCITY    12
#define ALARM_GYRO_HEALTH           13
#define ALARM_DATABASE              14
#define ALARM_TN_INCOHERENT         15
#define ALARM_NAV_NOT_ESTIMATE      16
#define ALARM_NAV_INCOHERENCE       17
#define ALARM_HMI_TIMEOUT           18
#define ALARM_ALERT_TIMEOUT         19

/// ALERTS
#define ALERT_TRAIN_OVERVELOCITY    20
#define ALERT_LOW_COVERAGE_GPS      21
#define ALERT_LOW_COVERAGE_GSM      22
#define ALERT_NEW_ITINERARY         23
#define ALERT_LOW_PK_ACCURACY       24
#define ALERT_DISTANCE              25
#define ALERT_PK_REACHED            26
#define ALERT_INTERNAL_COMM_LOST    27
#define ALERT_COMM_LOST             28
#define ALERT_DEGRADED_MODE         29
#define ALERT_SPEED_LOSS            30
#define ALERT_TUNNEL                31
///@}

/// @name OBLAU modes
/// Oblau mode is only describing if the train is SUPERVISED or UNSUPERVISED
///@{
typedef Int8 oblau_mode_t;
//!< OBLAU mode type definition.
#define UNSUPERVISED_MODE    0
//!< OBLAU is not BLOCKSAT supervised.
#define SUPERVISED_MODE      1
//!< OBLAU is BLOCKSAT supervised (and cannot move freely, it needs itinerary).
#define NO_BLOCKING_MODE     2
//!< OBLAU is moving freely in Blocksat.
///@}


/// oblau set (who talks with hmi)
#define PRIMARY         0
#define MONITOR         1

/// @name WoM identifiers
/// Used to define the WoM sense.
///
/// WoM sense is defined from cab1 which is the one that has the odometer
/// installed. Nominal WoM is the sense of the cab1 when moving forward.
///@{
#define NOMINAL         0
//!< Defines the forward sense of cab1.
#define OPPOSITE        1
//!< Defines the reverse sense of cab1.
///@}

typedef Int8 devState;
#define FAIL            -1
#define LOW_COVERAGE    0
#define CORRECT         1

typedef Int8 sysState;
#define FAILURE -1
#define DEGRADED 0
#define OPTIMAL 1

/// <summary>
/// Calculates the square of x
/// </summary>
#ifndef SQR
#define SQR(x) ((x) * (x))
//#define SQR(x) ({ typeof (x) _x = (x); _x * _x; })	
#endif

typedef enum SolStatus
{
    SOL_COMPUTED = 0,
    INSUFFICIENT_OBS = 1,
    NO_CONVERGENCE = 2,
    SINGULARITY = 3,
    COV_TRACE = 4,
    TEST_DIST = 5,
    COLD_START = 6,
    V_H_LIMIT = 7,
    VARIANCE = 8,
    RESIDUALS = 9,
    DELTA_POS = 10,
    NEGATIVE_VAR = 11,
    INTEGRITY_WARNING = 13,
    PENDING = 18,
    INVALID_FIX = 19,
    UNAUTHORIZED = 20
} SolStatus;

typedef enum GpsFixStatus
{
    NO_GNSS_FIX = 0,
    GNSS_FIX = 1
} GpsFixStatus;


typedef enum GpsSetActive
{
	GPS_SET_NONE = 0,
    GPS_SET_1 = 1,
    GPS_SET_2 = 2
} GpsSetActive;

typedef enum PosType
{
    NONE_TYPE = 0,
    FIXEDPOS = 1,
    FIXEDHEIGHT = 2,
    DOPPLER_VELOCITY = 3,
    SINGLE = 16,
    PSRDIFF = 17,
    WAAS = 18,
    PROPAGATED = 19,
    OMNISTAR = 20,
    L1_FLOAT = 32,
    IONOFREE_FLOAT = 33,
    NARROW_FLOAT = 34,
    L1_INT = 48,
    WIDE_INT = 49,
    NARROW_INT = 50,
    RTK_DIRECT_INS = 51,
    INS = 52,
    INS_PSRSP = 53,
    INS_PSRDIFF = 54,
    INS_RTKFLOAT = 55,
    INS_RTKFIXED = 56,
    OMNISTAR_HP = 64,
    OMNISTAR_XP = 65,
    CDGPS = 66
} PosType;

typedef struct gpsInfo
{
    Int32 msg;

    Bool valid_time_flag;
    UInt32 year;
    UInt8 month;
    UInt8 day;

    UInt8 hour;
    UInt8 minute;
    UInt8 second;
    UInt32 mili;

    SolStatus status;
    PosType type;
    SolStatus vstatus;
    PosType vtype;

    Float64 latitude;
    Float64 longitude;
    Float64 altitude;
    Float64 latitude_dev;
    Float64 longitude_dev;
    Float64 altitude_dev;

    Float64 speed;

    Int32 satellites;

    Float32 x_ECEF;
    Float32 y_ECEF;
    Float32 z_ECEF;

    Float32 x_UTM;
    Float32 y_UTM;

    Float32 x_std_UTM;
    Float32 y_std_UTM;

    Float32 vx_ECEF;
    Float32 vy_ECEF;
    Float32 vz_ECEF;
    Float32 vx_std_ECEF;
    Float32 vy_std_ECEF;
    Float32 vz_std_ECEF;

} gpsInfo;

typedef struct gpsControl
{
    Bool gpggartk;
    Int32 gpggartk_ontime;
    Int32 gpggartk_offset;

    Bool bestposa;
    Int32 bestposa_ontime;
    Int32 bestposa_offset;

    Bool bestxyza;
    Int32 bestxyza_ontime;
    Int32 bestxyza_offset;

    Bool bestvela;
    Int32 bestvela_ontime;
    Int32 bestvela_offset;

    Bool diferencial;
} gpsControl;

/// different point status
enum 
{
    POINT_COMPUTED_CORRECTLY = 0,   /// normal value
    POINT_NOT_COMPUTED,             /// initial value
    POINT_TOO_FAR_FROM_RTD,          /// computed correctly by the GPS but far RTD
    POINT_INFO_NOT_UPDATED,         /// computed correctly but not update for a small time
    POINT_LOST                      /// computed but not updated for a configurable time
};

typedef struct PointFS
{
    UInt16 id_point;
    Int16 id_segment;
    UInt16 id_canton;
    UInt16 track_number;
    Float32 utmx;
    Float32 utmy;
    Float32 pk;
    Float32 tang;
    Bool is_station;
} PointFS;

typedef struct LineFS
{
    /// @deprecated to delete
    UInt16 zone;
    char datum[8];
    UInt16 num_segments;
    UInt16 num_turnouts;
    UInt16 num_points;
} LineFS;

typedef struct SegmentFS
{
    UInt16 id_segment;
    UInt16 id_canton;
    Float32 start_pk;
    Float32 end_pk;
    int max_speed;
} SegmentFS;

typedef struct ConfigInfo
{
    Int8 OBLAU_SET;
    //!< Set identifier
    Int16 TRAIN_ID;
    Int16 TRAIN_LENGTH;
    Int16 MAX_TRAIN_MODEL_SPEED;
    Int16 MAX_SPEED_DIFF;
    Int16 MAX_DIST_VALID_PROJECTION;
    //!< Distance to target when the speed is checked to se that the driver is breaking
    Int16 BRAKE_CHECK_DISTANCE;
    Int16 MIN_DISTANCE_PK;
    //!< Maxmimum speed when the train is suposed to be braking to stop at a pk target
    Int16 MAX_SPEED_AT_CHECK_DISTANCE;
    //!< Minimum distance to reach destination
    Int16 ALARM_DISTANCE_TO_DESTINATION;
    //!< Minimum distance to destination to raise alarm
    Int16 MAX_POSITION_ACCURACY;
    Int16 MAX_SPEED_ACCURACY;
    Float32 LONGREFPOINT;
    Float32 LATREFPOINT;

    // GPRS
    Int32 PINCODE;
    Int32 PIN2CODE;
    Int32 PUKCODE;
    Int32 PUK2CODE;
    char APN[20];
    char USER[20];
    char PASSWORD[20];
    char AUTH[40];
    char ATINIT[40];
    char SIMSLOT[10];

    // CTC
    char CTC_IP_ADDR[INET_ADDR_LEN + 1];
    UInt16 CTC_PORT_NUM;

    // Internal Comms
    char INT_IP_SET1[INET_ADDR_LEN+1];
    char INT_IP_SET2[INET_ADDR_LEN+1];
    char EXT_IP_SET1[INET_ADDR_LEN+1];
    char EXT_IP_SET2[INET_ADDR_LEN+1];
    char NFS[INET_ADDR_LEN+1];
    char *INT_IP;
    char *EXT_IP;
    char *ANOTHER_OBLAU_IP;
    UInt16 INTERNAL_COMM_PORT;

    // GPS
    /*
    char COM_DATUM[20];
    Int16 UTM_ZONE;
    */

    // GPS Advanced Navigation Ethernet configuration
    char GPS_ADV_NAV_IP_ADDR[INET_ADDR_LEN+1];
    UInt16 GPS_ADV_NAV_PORT;
    char OBLAU_SERVER_ADV_NAV_IP_ADDR[INET_ADDR_LEN+1];
    UInt16 OBLAU_SERVER_ADV_NAV_PORT;

    // Traces logger
    char LOGGER_ADDR[INET_ADDR_LEN+1];
    UInt16 LOGGER_PORT;
    // Data logger
    char DATA_ADDR[INET_ADDR_LEN+1];
    UInt16 DATA_PORT;

    // HMI
    char HMI_ADDR_CAB_1[INET_ADDR_LEN+1];
    char HMI_ADDR_CAB_2[INET_ADDR_LEN+1];
    UInt16 HMI_PORT;

    // Cabin and train lenght
    Int16 L_LOC;

    Float32 DIST_SET2_CAB_1;
    Float32 DIST_SET2_CAB_2;
    Float32 DIST_SET2_CAB_1_OTHER;
    Float32 DIST_SET2_CAB_2_OTHER;

    Float32 DIST_SET1_CAB_1_25401;
    Float32 DIST_SET1_CAB_2_25401;
    Float32 DIST_SET1_CAB_1_25402;
    Float32 DIST_SET1_CAB_2_25402;
    Float32 DIST_SET1_CAB_1_25403;
    Float32 DIST_SET1_CAB_2_25403;
    Float32 DIST_SET1_CAB_1_OTHER;
    Float32 DIST_SET1_CAB_2_OTHER;
  
    // Other
    Int16 ODOM_TASK_FREQUENCY;
    Int16 ODOM_ALG_CNT_MAX;
    Int16 TDM_AZ_DISTANCE;
    Int16 TDM_CZ_DISTANCE;
    Int16 WD_TIMEOUT;
    
    //!< Train maximum speed in km/h to consider the train is stopped.
    //!< Used in ALERT_PK_REACHED, ALARM_NO_AUTHORISATION, and ALARM_WAYOFMOTION.
    Float32 STOP_SPEED;  
    
    //!< Train maximum speed in km/h to disregard ALARM_WAYOFMOTION alarm   
    Float32 REVERSE_MAX_SPEED;
    Float32 PK_OVERPASSED_MARGIN;
    Float32 MANUAL_DRIVING_SPEED;
    Float32 BRAKE_DEACCELERATION;
} ConfigInfo;

typedef Int16 train_id_t;
typedef Int16 train_len_t;
typedef Int8 oblau_msg_type;
typedef Int32 pk_value_t;
typedef UInt16 track_num_t;
typedef Int16 speed_value_t;
typedef Int8 oblau_role_t;
typedef Int8 wom_t;
typedef unsigned int seq_num_type;
typedef struct timespec oblau_msg_ts_t;

typedef struct traffic_info_t
{
    pk_value_t pk;
    pk_value_t pk_assigned;
    track_num_t track;
    wom_t wom;
    speed_value_t speed;
} traffic_info_t;

/// @brief OBLAU message sent to CTC.
/// @remarks The OBLAU message size is compiler-aligned to
/// 16bits. It means that the "real" message size is not the summary of
/// bytes in the definition but the closer value aligned to word (16 bits).
typedef struct oblau_msg_t
{
    train_id_t train_id;
    char padding1[2];   //!< Flag informing if message is encrypted or not.
    train_len_t len;    //!< Train length (in the INIT message); later on, the segment occuppied.
    oblau_msg_type msg_type;
    seq_num_type sequence;
    seq_num_type acknowledge;
    traffic_info_t traffic_info;
    train_status_t status;          /// DEPRECATED: set to 0 allways
    Int8 alarm_id;                  /// DEPRECATED: set to -1 allways
    oblau_mode_t mode;
    oblau_role_t role;
    Int32 alarm;                   //!<  alarm mask. Each bit identifies an alarm.
    oblau_msg_ts_t time;
    crc_t checksum;
} oblau_msg_t;

/// @brief OBLAU message size (always aligned to 16bits)
#define OBLAU_MSG_SIZE sizeof(oblau_msg_t)

/// @brief Number of bytes at header of the OBLAU message to NOT encrypt never.
#define NUM_BYTES_UNENCRYPTED_PRE 4

/// @brief Number of bytes at bottom of the OBLAU message to NOT encrypt never.
#define NUM_BYTES_UNENCRYPTED_POST 16

typedef struct PVEstData
{
    Float32 heading;
    Float32 GPS_heading;

    Float32 Vxgps_TRS;
    Float32 Vxgps_var_TRS;

    Float32 Vxgps_ENU;
    Float32 Vygps_ENU;
    Float32 Vxgps_var_ENU;
    Float32 Vygps_var_ENU;

    Float32 Vodo;
    Float32 Vodo_var;

    Int8 wom;

    //!< Train speed; in m/s
    Float32 Vest;
    Float32 Vest_var;    
    //!< Previous train speed; in m/s
    Float32 Vest_pr;
    Float32 Vest_var_pr;    
    //!< Train speed; in km/h
    Float32 Vtrain;
    Float32 Vfiltered;

    Float32 PKest;
    Float32 PKest_var;
    Float32 Ppred;
    Float32 Ppred_var;
    Float32 PKtrain;

    Float32 Pxgps_UTM;
    Float32 Pygps_UTM;
    Float32 Pxgps_var_UTM;
    Float32 Pygps_var_UTM;

    Float32 Pxgps_TRS;
    Float32 Pxgps_var_TRS;
    Float32 Pxo_seg;
    Float32 Pyo_seg;

    Float32 Pxyo_PK;

    Float32 PKtarget;

    // Validity Flags
    Bool Pgps_flag;
    Bool Pgps_contloss_flag;

    Bool PVgps_new_flag; //Its allways true
    Bool Vgps_flag;
    Bool Vgps_contloss_flag;
    Bool Vodo_flag;

} PVEstData;

typedef struct IntInfo {
	unsigned int spurious;
	unsigned int pps;
	unsigned int raw;
	unsigned int acc;
	unsigned int total;
} IntInfo;

#endif
