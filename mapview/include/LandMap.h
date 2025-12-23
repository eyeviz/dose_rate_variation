//==============================================================================
// LandMap.h
//	: header file for the land map as a grid of satellite images
//
//------------------------------------------------------------------------------
//
//				Time-stamp: "2024-01-08 21:58:32 shigeo"
//
//==============================================================================

#ifndef _LandMap_H		// begining of header file
#define _LandMap_H		// notifying that this file is included

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include "LandTile.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------

// Fukushima Pref. (36,139),(36,140),(37,139),(37,140),(37,141)
#define	BASE_LATITUDE	(36)
#define	BASE_LONGITUDE	(139)

#define NUM_CATEGORIES	(12)


//------------------------------------------------------------------------------
//	Defining Classes
//------------------------------------------------------------------------------

class LandMap {

  private:

    static unsigned char _category[ NUM_CATEGORIES ][ 3 ];

 protected:
    
    LandTile	_tile[ 2 ][ 3 ];
    // _tile[ 0 ][ 0 ] => (36,139)
    // _tile[ 0 ][ 1 ] => (36,140)
    // No _tile[ 0 ][ 2 ]
    // _tile[ 1 ][ 0 ] => (37,139)
    // _tile[ 1 ][ 1 ] => (37,140)
    // _tile[ 1 ][ 2 ] => (37,141)

    void _init	( void );
    
    void		_pixel	( const double & latitude, const double & longitude,
				  unsigned char & R, unsigned char & G, unsigned char & B );
    unsigned int	_landID	( const unsigned char & R, 
				  const unsigned char & G,
				  const unsigned char & B );
    string		_landuse	( const unsigned int & id );

  public:

//------------------------------------------------------------------------------
//	Constructors
//------------------------------------------------------------------------------
    LandMap();			// default constructor

//------------------------------------------------------------------------------
//	Destructor
//------------------------------------------------------------------------------
    ~LandMap();			// destructor

//------------------------------------------------------------------------------
//	Referrring to members
//------------------------------------------------------------------------------
    unsigned int landID( const double & latitude, const double & longitude ) {
	unsigned char R, G, B;
	_pixel( latitude, longitude, R, G, B );
	return _landID( R, G, B );
    }

    string landtype( const double & latitude, const double & longitude ) {
	unsigned char R, G, B;
	_pixel( latitude, longitude, R, G, B );
	unsigned int id = _landID( R, G, B );
	return _landuse( id );
    }
    
//------------------------------------------------------------------------------
//	Class name
//------------------------------------------------------------------------------
    virtual const char * className( void ) const { return "LandMap"; }
				// class name
};


#endif // _LandMap_H

// end of header file
// Do not add any stuff under this line.
