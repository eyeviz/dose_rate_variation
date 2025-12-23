//==============================================================================
// Bridson.h
//	: header file for Bridson Poisson Disk Sampline
//
//------------------------------------------------------------------------------
//
//				Time-stamp: "2023-04-14 04:31:43 shigeo"
//
//==============================================================================

#ifndef _Bridson_H		// begining of header file
#define _Bridson_H		// notifying that this file is included

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------
#include <vector>

using namespace std;

#include "Mesh.h"

//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------
typedef enum { Default, normDist } RadiusType;


//------------------------------------------------------------------------------
//	Function Declarations
//------------------------------------------------------------------------------
std::vector< P_Point2 > poissonDiskSampling( const interval< double > & rngX,
					     const interval< double > & rngY,
					     const ublas::matrix< double > & radius,
					     // int k = 256,
					     int k = 128,
					     // int k = 32,
					     RadiusType rType = Default );

#endif // _Bridson_H

// end of header file
// Do not add any stuff under this line.
