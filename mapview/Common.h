#ifndef __Common_H_
#define __Common_H_

//#include <iostream>
//#include <fstream>
//#include <algorithm>
#include <vector>
//#include <map>
//#include <cmath>
//#include <cstdlib>

using namespace std;

//------------------------------------------------------------------------------
//  OpenGL environment setting
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Macro switches
//------------------------------------------------------------------------------
#define ZERO_HEIGHT_PROJECTION

//#define BLENDING_RATIO		(0.7)
#define BLENDING_RATIO		(0.3)

// #define ENABLE_MAP_COORD_NORMALIZATION


//------------------------------------------------------------------------------
//  Macro definitions
//------------------------------------------------------------------------------
#ifndef TRUE
#define TRUE	(1)
#endif	// TRUE

#ifndef FALSE
#define FALSE	(0)
#endif	// FALSE

#ifndef MAX2
#define MAX2( x, y )		(( (x)<(y) )? (y) : (x) )
#endif	// MAX2

#ifndef MIN2
#define MIN2( x, y )		(( (x)>(y) )? (y) : (x) )
#endif	// MIN2

#ifndef SQUARE
#define SQUARE( x )		((x)*(x))
#endif	// SQUARE

#ifndef NO_INDEX
#define NO_INDEX        (90001)
#endif	// NO_INDEX

// EPS should be avoided due to conflicts with CGAL
#ifndef EPSILON
//#define EPSILON		(1.0e-8)
#define EPSILON		(1.0e-5)
#endif	// EPSILON

#ifndef MAX_STR
#define	MAX_STR		(256)
#endif	// MAX_STR

#ifndef MAX_INT
#define MAX_INT		(1.0e+8)
#endif	// MAX_INT

#ifndef NO_DIST
#define NO_DIST		(-1)
#endif	// NO_DIST

#ifndef INFINITY
#define INFINITY	(1.0e+8)
#endif	// INFINITY

#ifndef MAX_VALUE
#define MAX_VALUE	(1.0e+8)
#endif	// MAX_VALUE

#ifndef HERE
#define HERE		" " << __FILE__ << ":" << __LINE__ << " "
#endif	// HERE

#ifndef BUF_SIZE
#define BUF_SIZE	(256)
#endif	// BUF_SIZE


#ifndef NUM_LEVELS
// 1, 2, 4, 8, 16, 32, 64, 128, 256
// #define BASE_SIZE		(4)
//#define NUM_LEVELS		(9)
#define NUM_LEVELS		(7)
#endif	// NUM_LEVELS

#ifndef NUM_RESOLUTIONS
//#define NUM_RESOLUTIONS		(1)
//#define MAX_RESOLUTIONS		(1)
#define NUM_RESOLUTIONS		(NUM_LEVELS*NUM_LEVELS)
#define MAX_RESOLUTIONS		(NUM_LEVELS*NUM_LEVELS)
#endif	// NUM_RESOLUTIONS

#ifndef UNITY_SIZE
#define UNITY_SIZE		(1)
#endif	// UNITY_SIZE

#ifndef RASTER_RESOLUTION
#define RASTER_RESOLUTION	(64) // <----
//#define RASTER_RESOLUTION	(128)
//#define RASTER_RESOLUTION	(256) // <==
//#define RASTER_RESOLUTION	(512) // <==
#endif	// RASTER_RESOLUTION

#ifndef LIMIT_NUM_SINGULARITIES
#define LIMIT_NUM_SINGULARITIES		(512.0)
//#define LIMIT_NUM_SINGULARITIES		(256.0)
//#define LIMIT_NUM_SINGULARITIES		(128.0)
#endif	// LIMIT_NUM_SINGULARITIES


#ifndef	SELECTION_SIZE
#define SELECTION_SIZE		(5096)
#endif	// SELECTION_SIZE

//------------------------------------------------------------------------------
//	Type Definitions
//------------------------------------------------------------------------------
// set of non-negative integers
typedef vector< unsigned int >			SetUInt;


//------------------------------------------------------------------------------
//	External declaration of global variables
//------------------------------------------------------------------------------


#endif  // __Common_H_
