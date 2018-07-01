/// @file polyline.c
/// @brief Point to polyline algorithms implementation
/// @ingroup SEGMENT_NAV
#include "polyline.h"
#include <math.h>
#include <float.h>

/// <summary>
/// Calculates the square of x
/// </summary>
#define SQR(x) ((x) * (x))

/// <summary>
/// Calculates the squared length of a vector from origen to the point p
/// </summary>
#define SQRLEN(p) ((p.utmx) * (p.utmx) + (p.utmy) * (p.utmy))

/// <summary>
///  Subtracts points: c = a - b
/// </summary>
#define SUBPOINT(c, a, b) {(c).utmx = (a).utmx - (b).utmx; (c).utmy = (a).utmy - (b).utmy; }

/// <summary>
/// The sqr_distance() function implements the algorithm described below to obtain:
/// - The minimum squared length of a query point to a polyline.
/// - The point of the polyline Distance from a point to a polyline and
/// - Note that the point on the polyline may be a vertex of the polyline or a point in a segment.
/// </summary>
/// <remarks>
/// Considerations:
/// - A polyline is considered as a chain of connected line segments.
/// - The basic algorithm is to evaluate the distance from a point to a line segment.
/// - Once you have the algorithm, we just try every line segment in the polyline and pick the shortest distance.
/// - The square root in the Euclidean norm consumes some runtime. However, it is unnecessary in the core
///   of the algorithm for distance comparison purposes since the equivalence:
///            d < sqrt(x^2 + y^2) <=> d^2 < (x^2 + y^2)
/// So we will operate on the squared Euclidean norm instead, i.e. D^2 = Dx^2 + Dy^2,
/// and evaluate the square root only once at function return.
///
/// Math:
/// A point p on a line ab is defined as
///       p = a + t.(b – a)
/// When t is between 0 and 1 the point p is situated between a and b on ab.
///
/// There are a number of mathematical ways to find the shortest distance to a line, here is one of them.
/// The point p is defined in terms of the parameter t, subsequently so does the distance from the query point q to the point p.
/// To find the minimum of that distance we differentiate it and set equal to zero:
///      d/dt(d) = d/dt(||p(t)–q||) = 0
///
/// Solving that equation immediately gives
///      t = ((ax – bx)(ax – qx) + (ay – by)(ay – qy)) / l^2
/// where
///      l^2 = (ax – bx)^2 + (ay – by)^2
/// is, obviously, the squared length of the segment ab.
///
/// Substituting t into the equation d^2 = ?p(t)–q?^2 for the distance gives:
///      d^2 = (((ax – bx)(by – qy) – (ay – by)(bx – qx))^2) / l^2
/// which is what we actually want.
///
/// Algorithm:
/// Notice the repeating terms which may be reused in various evaluations.
/// Notice that we take the first point b, calculate and remember the distance to it.
/// Then we consider all the segments one at a time.
///  - If the t value for current segment is less than 0, then we must consider its starting point,
/// which also belongs to the previous segment which we must have been already examined,
/// so we skip this segment.
///  - If 0 <= t <= 1, then we just calculate the distance.
///  - If t > 1, then we calculate the distance to the ending point of this segment.
/// Then we check if the distance to the current segment is shorter than previously remembered value
/// and if so we remember the new distance value.
/// <remarks>

/// <summary>
/// Finds the minimum distance of a query point to a polyline.
/// Also returns the point on the polyline and the Pk of such point.
/// </summary>
/// <param name="polyline">Array of points of the polyline.</param>
/// <param name="count">Number of points in the polyline.</param>
/// <param name="q">Query point</param>
/// <param name="closestPointOnLine">Returns closest point on polyline.
/// Includes utmx, utmy, and pk.</param>
/// <returns>Distance of query point to the closest point on polyline.</returns>
Float64 point_to_polyline_distance(PointFS polyline[], int count, PointFS q, PointFS *closestPointOnLine)
{
  int pnt_idx;
  PointFS b;
  PointFS dbq;
  PointFS pointOnLine;
  Float64 sqr_dist;

  // Initialize the algorithm setting the distance and point of the first vertex
  // of the polyline, at index 0.
  b = polyline[0];
  SUBPOINT(dbq, b, q);
  pointOnLine = b;
  *closestPointOnLine = b;
  sqr_dist = SQRLEN(dbq);

  // We start at index 1 to calculate the first ab segment
  for (pnt_idx = 1; pnt_idx < count; ++pnt_idx)
  {
    PointFS a;
    PointFS daq;
    PointFS dab;
    Float64 inv_sqrlen;
    Float64 t;
    Float64 current_sqr_dist;

    a = b;               // now a is the previous point
    daq = dbq;           // and we reuse dbq
    b = polyline[pnt_idx];     // b is the next point
    SUBPOINT(dbq, b, q);
    SUBPOINT(dab, a, b);
    inv_sqrlen = 1. / SQRLEN(dab);
    t = (dab.utmx * daq.utmx + dab.utmy * daq.utmy) * inv_sqrlen;

    // The t parameter indicates the situation of the point on line in relation to segment ab
    if (t <= 0.)
    {
        // Closest point on line is the first point of the segment; a.
        // However, this point was already evaluated in the previous loop,
        // or at the initial point.
        continue;
    }
    else if (t >= 1.)
    {
        // Closest point on line is the second point of segment; b
        pointOnLine = b;
        current_sqr_dist = SQRLEN(dbq);
    }
    else // (0 < t < 1.)
    {
        // Closest point on line is lying in the segment; between a and b on ab.
        pointOnLine.utmx = a.utmx - t * dab.utmx;
        pointOnLine.utmy = a.utmy - t * dab.utmy;
        // Corresponding PK
        pointOnLine.pk = a.pk + t * (b.pk - a.pk);
        current_sqr_dist = SQR(dab.utmx * dbq.utmy - dab.utmy * dbq.utmx) * inv_sqrlen;
        // The nearest point defines the block/segment 
        if ( t < 0.5)
        {
            pointOnLine.id_canton = a.id_canton;
            pointOnLine.id_segment = a.id_segment;        	
        }
        else
        {
            pointOnLine.id_canton = b.id_canton;
            pointOnLine.id_segment = b.id_segment;        	       	
        }
    }

    if (current_sqr_dist < sqr_dist)
    {
        // Save distance and point
        *closestPointOnLine = pointOnLine;
        sqr_dist = current_sqr_dist;
    }
  }

  return sqrt(sqr_dist);
}

// Search for 
// Different polylines in the point array are defined in the points array by this convention:
// The point array has the structure:
// - Polyline segment points are ordered in the array.
// - The id_point value for each polyline increases, e.g. from 1 to Np,
//   the last point of the polyline.
// - Example of id_point: 1, 2, 3, ..., N, 1, 2, 3, 4, ..., M, 1, 2, ... 
Float64 point_to_polylines(PointFS rtd[], int count, PointFS q, PointFS *closestPointOnLine)
{
	int pnt_idx;
	PointFS pointOnLine;
	Float64 dist = FLT_MAX;
   	int start_idx;
   	Float64 current_dist;
   	
   	// It assumes at least two points per polyline
   	start_idx = 0;
    for (pnt_idx = 1; pnt_idx < count; ++pnt_idx)
    {   
    	// Check for last point in current polyline or in the array
    	// Note that it may be only one polyline in the rtd.
        if (rtd[pnt_idx].id_point < rtd[pnt_idx - 1].id_point || 
        	(pnt_idx + 1) == count)
        {
        	int end_idx;
        	int poly_count;
        	
        	if ((pnt_idx + 1) == count)
        	{
				// Found the ending point of array, ends current polyline        		
            	end_idx = pnt_idx;       		
        	}
        	else
        	{
				// Found the ending point of current polyline        	
				end_idx = pnt_idx - 1;
        	}
			poly_count = end_idx - start_idx + 1;
			
        	// Calculate nearest point to current polyline
     		current_dist = point_to_polyline_distance(
    				                rtd + start_idx,
    				                poly_count,
    				                q,
    				                &pointOnLine);
     		
     	    if (current_dist < dist)
     	    {
     	        // Save distance and point
     	        *closestPointOnLine = pointOnLine;
     	        dist = current_dist;
     	    }
     	    
     	    // Next polyline start point
        	start_idx = pnt_idx;
    	}
        else
        {
        	// A point in the current polyline, see next
        	continue;
        }
    } // end for

    return dist;
}
