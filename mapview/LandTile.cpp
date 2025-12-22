//==============================================================================
// LandTile.cpp
//	: program file for satellite images provided by JAXA for land use types
//
//------------------------------------------------------------------------------
//
//				Time-stamp: "2024-01-08 18:13:59 shigeo"
//
//==============================================================================

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include "LandTile.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Private Functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Protected Functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Public Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	Constructors
//------------------------------------------------------------------------------

//
//  LandTile::LandTile --	default constructor
//
//  Inputs
//	none
//
//  Outputs
//	none
//
LandTile::LandTile() 
{
    _latitude = _longitude = 0;
}


//
//  LandTile::LandTile --	copy constructor
//
//  Inputs
//	obj	: object of this class
//
//  Outputs
//	none
//
LandTile::LandTile( const LandTile & obj )
{
    _latitude	= obj._latitude;
    _longitude	= obj._longitude;
    _image	= obj._image;
}


//------------------------------------------------------------------------------
//	Destructor
//------------------------------------------------------------------------------

//
//  LandTile::~LandTile --	destructor
//
//  Inputs
//	none
//
//  Outputs
//	none
//
LandTile::~LandTile()
{

}


//------------------------------------------------------------------------------
//	Loading satellite images
//------------------------------------------------------------------------------

void LandTile::loadData( const unsigned & __latitude, const unsigned int & __longitude )
{
    _latitude = __latitude;
    _longitude = __longitude;
    // LC_N37E139_50m.tif
    ostringstream namestr;
    namestr << SATELLITE_DIR 
	    << "LC_N" << setw( 2 ) << _latitude
	    << "E" << setw( 3 ) << _longitude << "_50m.tif" << ends;

    // load the satellite image
    _image = cv::imread( namestr.str().c_str() );

    // error handling
    if ( _image.empty() == true ) {
        return;
    }

    // Number of rows
    cerr << "rows: " << _image.rows << endl;
    // Number of columns
    cerr << "cols: " << _image.cols << endl;
    // Number of channels
    cerr << "channels: " << _image.channels() << endl;
}
	

//------------------------------------------------------------------------------
//	functions for sorting
//------------------------------------------------------------------------------

//
//  compare --	comparison function for sorting
//
//  inputs
//	a, b	: pointers to objects of this class
//
//  outputs
//	a <  b	-1
//	a == b	 0
//	a >  b	 1
//
int compare ( const LandTile * a, const LandTile * b )
{
    if ( a->_longitude < b->_longitude ) return (1);
    else if ( a->_longitude > b->_longitude ) return (-1);
    else {
	if ( a->_latitude < b->_latitude ) return (1);
	else if ( a->_latitude > b->_latitude ) return (-1);
    }
    
    return 0;
}


//------------------------------------------------------------------------------
//	equality
//------------------------------------------------------------------------------

//
//  operator == --	equality
//
//  inputs
//	a, b	: objects of this class
//
//  outputs
//	boolean value
//
int operator == ( const LandTile & a, const LandTile & b )
{
    return ( ( a._longitude == b._longitude ) &&
	     ( a._latitude  == b._latitude  ) );
}


//------------------------------------------------------------------------------
//	assignment opereators
//------------------------------------------------------------------------------

//
//  LandTile::operator = --	assignement
//
//  inputs
//	obj	: objects of this class
//
//  outputs
//	this object
//
LandTile & LandTile::operator = ( const LandTile & obj )
{
    if ( this != &obj ) {
	_latitude	= obj._latitude;
	_longitude	= obj._longitude;
	_image		= obj._image;
    }
    return *this;
}


//------------------------------------------------------------------------------
//	I/O functions
//------------------------------------------------------------------------------

//
//  operator << --	output
//
//  Argument
//	stream	: output stream
//	obj	: object of this class
//
//  Outputs
//	output stream
//
ostream & operator << ( ostream & stream, const LandTile & obj )
{
    stream << obj._longitude << "\t" << obj._latitude << endl;
    return stream;
}


//
//  operator >> --	input
//
//  Inputs
//	stream	: input stream
//	obj	: object of this class
//
//  Outputs
//	input stream
//
istream & operator >> ( istream & stream, LandTile & obj )
{
    stream >> obj._longitude >> obj._latitude;
    return stream;
}


// end of header file
// Do not add any stuff under this line.
