//==============================================================================
// Cloud.h,cpp
//	: header file for point clouds
//
//------------------------------------------------------------------------------
//
//				Time-stamp: "2021-05-12 17:24:33 shigeo"
//
//==============================================================================
#ifndef _Cloud_H		// begining of header file
#define _Cloud_H		// notifying that this file is included

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/flann.hpp>

#include "LandPlot.h"

using namespace cv::flann;
using namespace std;

//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Defining Classes
//------------------------------------------------------------------------------

class Cloud {

  private:
    static int			_nDim;
    static int			_maxNeighbors;
    static float		_maxRadius;
    
protected:
    cv::Mat_< float >		_point;
    cv::flann::Index		* _flann_index;
    std::vector< LandType >	_landtype;
    
//------------------------------------------------------------------------------
//	interpolation of scalar field
//------------------------------------------------------------------------------
    void	_set		( const std::vector< D_Point2 > & sample,
				  const std::vector< LandType > & value );
    LandType	_nearest	( const double x, const double y );

  public:

//------------------------------------------------------------------------------
//	Constructors
//------------------------------------------------------------------------------
    Cloud();			// default constructor
    Cloud( const Cloud & obj );
				// copy constructor

//------------------------------------------------------------------------------
//	Destructor
//------------------------------------------------------------------------------
    ~Cloud();			// destructor

//------------------------------------------------------------------------------
//	Referrring to members
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	Sampling land use type on the domain
//------------------------------------------------------------------------------
    
//------------------------------------------------------------------------------
//	interpolation of scalar field
//------------------------------------------------------------------------------
    void	set( const std::vector< D_Point2 > & sample,
		     const std::vector< LandType > & value ) {
	_set( sample, value );
    }
    
    LandType nearest( const double & x, const double & y ) {
	return _nearest( x, y );
    }
    LandType nearest( const D_Point2 & p ) {
	return _nearest( p.x(), p.y() );
    }
    
//------------------------------------------------------------------------------
//	Assignment opereators
//------------------------------------------------------------------------------
    Cloud & operator = ( const Cloud & obj );
				// assignment

//------------------------------------------------------------------------------
//	I/O functions
//------------------------------------------------------------------------------
    friend ostream & operator << ( ostream & stream, const Cloud & obj );
				// output
    friend istream & operator >> ( istream & stream, Cloud & obj );
				// input

//------------------------------------------------------------------------------
//	Class name
//------------------------------------------------------------------------------
    virtual const char * className( void ) const { return "Cloud"; }
				// class name
};



#endif // _Cloud_H

// end of header file
// Do not add any stuff under this line.
