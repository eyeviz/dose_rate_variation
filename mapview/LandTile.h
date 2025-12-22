//==============================================================================
// LandTile.h
//	: header file for satellite images provided by JAXA for land use types
//
//------------------------------------------------------------------------------
//
//				Time-stamp: "2024-01-08 21:47:26 shigeo"
//
//==============================================================================

#ifndef _LandTile_H		// begining of header file
#define _LandTile_H		// notifying that this file is included

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------
#include <iostream>

using namespace std;

// OpenCV library
#include <opencv2/opencv.hpp>

#include "LandTile.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------
#define		SATELLITE_DIR	"satellite/"




//------------------------------------------------------------------------------
//	Defining Classes
//------------------------------------------------------------------------------

class LandTile {

  private:

  protected:

    unsigned int	_latitude;
    unsigned int	_longitude;

    cv::Mat		_image;

  public:

//------------------------------------------------------------------------------
//	Constructors
//------------------------------------------------------------------------------
    LandTile();			// default constructor
    LandTile( const LandTile & obj );
				// copy constructor

//------------------------------------------------------------------------------
//	Destructor
//------------------------------------------------------------------------------
    ~LandTile();			// destructor

//------------------------------------------------------------------------------
//	Referrring to members
//------------------------------------------------------------------------------
    const unsigned int & latitude( void ) const		{ return _latitude; }
    unsigned int & latitude( void )			{ return _latitude; }
    const unsigned int & longitude( void ) const	{ return _longitude; }
    unsigned int & longitude( void )			{ return _longitude; }

    const cv::Mat & image( void ) const			{ return _image; }
    cv::Mat & image( void )				{ return _image; }

    const int & cols( void )				{ return _image.cols; }
    const int & rows( void )				{ return _image.rows; }
    
//------------------------------------------------------------------------------
//	Loading satellite images
//------------------------------------------------------------------------------
    void loadData( const unsigned & __latitude,
		   const unsigned int & __longitude );

//------------------------------------------------------------------------------
//	Functions for sorting
//------------------------------------------------------------------------------
    friend int compare ( const LandTile * a, const LandTile * b );
    friend int compare ( const LandTile ** a, const LandTile ** b );
				// comparison
//------------------------------------------------------------------------------
//	equality
//------------------------------------------------------------------------------
    friend int operator == ( const LandTile & a, const LandTile & b );
    friend int operator != ( const LandTile & a, const LandTile & b ) {
	return ( ! ( a == b ) );
    }				// equality

//------------------------------------------------------------------------------
//	Assignment opereators
//------------------------------------------------------------------------------
    LandTile & operator = ( const LandTile & obj );
				// assignment

//------------------------------------------------------------------------------
//	I/O functions
//------------------------------------------------------------------------------
    friend ostream & operator << ( ostream & stream, const LandTile & obj );
				// output
    friend istream & operator >> ( istream & stream, LandTile & obj );
				// input

//------------------------------------------------------------------------------
//	Class name
//------------------------------------------------------------------------------
    virtual const char * className( void ) const { return "LandTile"; }
				// class name
};

inline int arrange( const LandTile * a, const LandTile * b ) {
    return compare( a, b );
}


#endif // _LandTile_H

// end of header file
// Do not add any stuff under this line.
