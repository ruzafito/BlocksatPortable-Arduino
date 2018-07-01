/// @file polyline.h
/// @brief
/// @ingroup
#ifndef POLYLINE_H_
#define POLYLINE_H_

#define QT_TEST
#ifdef QT_TEST
    // OBLAU definitions
	#define FALSE (0)
	#define TRUE (1)
    typedef float Float32;                  /// 32 bit floating-point basetype
    typedef double Float64;                 /// 64 bit floating-point basetype
    typedef signed char Int8;               /// 8 bit signed integer basetype
    typedef signed short int Int16;         /// 16 bit signed integer basetype
    typedef signed long int Int32;          /// 32 bit signed integer basetype
    typedef unsigned char UInt8;            /// 8 bit unsigned integer basetype
    typedef unsigned short int UInt16;      /// 16 bit unsigned integer basetype
    typedef unsigned long int UInt32;       /// 32 bit unsigned integer basetype
    typedef void Void;                      /// void basetype
    typedef unsigned char Bool;             /// boolean basetype

    typedef struct PointFS
    {
    UInt16 id_point;
    UInt16 id_segment;
    UInt16 id_canton;
    UInt16 track_number;
    Float32 utmx;
    Float32 utmy;
    Float32 pk;
    Float32 tang;
    Bool is_station;
    } PointFS;

	typedef struct SegmentFS
	{
		UInt16 id_segment;
		UInt16 id_canton;
		Float32 start_pk;
		Float32 end_pk;
		int max_speed;
    } SegmentFS;    
#else
    #include "tl_basetypes.h"
    #include "Structs.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

Float64 point_to_polyline_distance(PointFS polyline[], int pnt_count, PointFS q, PointFS *closestPointOnLine);

Float64 point_to_polylines(PointFS rtd[], int pnt_count, PointFS q, PointFS *closestPointOnLine);

#ifdef __cplusplus
}
#endif
#endif // POLYLINE_H_
