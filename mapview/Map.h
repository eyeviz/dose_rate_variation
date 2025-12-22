//==============================================================================
// Map.h
//	: program file for maps
//
//------------------------------------------------------------------------------
//
//				Date: Wed Jun  7 15:12:27 2017
//
//==============================================================================

#ifndef _Map_H		// begining of header file
#define _Map_H		// notifying that this file is included

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------
#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
// #include <opencv/cv.h>
using namespace cv;

#include "Polyline.h"

//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------
#define KANTO_AREA_MAP

#ifdef WHOLE_JAPAN_MAP
#define MAP_TEXTURE_FILENAME	"japan-2048x1900.jpg"
#define MAP_TEXTURE_MIN_X	(128.485618)
#define MAP_TEXTURE_MAX_X	(146.173606)
#define MAP_TEXTURE_MIN_Y	(30.836343)
#define MAP_TEXTURE_MAX_Y	(45.815345)
#endif	// WHOLE_JAPAN_MAP

#ifdef KANTO_AREA_MAP
#define MAP_TEXTURE_FILENAME	"kanto-800x800.jpg"
//#define MAP_TEXTURE_MIN_X	(138.327211)
//#define MAP_TEXTURE_MIN_X	(138.42)
#define MAP_TEXTURE_MIN_X	(138.36)
//#define MAP_TEXTURE_MAX_X	(141.069673)
//#define MAP_TEXTURE_MAX_X	(141.17)
#define MAP_TEXTURE_MAX_X	(141.20)
//#define MAP_TEXTURE_MIN_Y	(34.857870)
#define MAP_TEXTURE_MIN_Y	(34.90)
//#define MAP_TEXTURE_MAX_Y	(37.175740)
#define MAP_TEXTURE_MAX_Y	(37.10)
#endif	// KANTO_AREA_MAP


//#define LABEL_UNIT_RATIO	(0.0036)
// #define LABEL_UNIT_RATIO	(0.0024)
//#define LABEL_UNIT_RATIO	(0.0020)
//#define LABEL_UNIT_RATIO	(0.0018)
//#define LABEL_UNIT_RATIO	(0.0016)
#define LABEL_UNIT_RATIO	(0.0015)
#define CONFLICT_MARGIN		(0.00)
//#define CONFLICT_MARGIN		(0.20) // <==


#define DEFAULT_WINDOW_SIZE	(1024)


//------------------------------------------------------------------------------
//	Defining Classes
//------------------------------------------------------------------------------

class Map {

  private:

  protected:

    // array of borders
    std::vector< Polyline >	_border;
    // array of rivers
    std::vector< Polyline >	_river;
    // array of roads
    std::vector< Polyline >	_road; 
    
    void		_init		( void );
    void		_district	( const char * filename );
    void		_flow		( const char * filename );
    void		_stream		( const char * filename );
    void		_network	( const char * filename );

  public:

//------------------------------------------------------------------------------
//	constructor
//	Constructors
//------------------------------------------------------------------------------
    Map();			// default constructor
				// constructor (default)
    Map( const Map & obj );
				// copy constructor
				// copy constructor

//------------------------------------------------------------------------------
//	デストラクタ
//	Destructor
//------------------------------------------------------------------------------
    ~Map();			// destructor
				// デストラクタ

//------------------------------------------------------------------------------
//	メンバ参照
//	Referrring to members
//------------------------------------------------------------------------------
    const std::vector< Polyline > &	border( void ) const	{ return _border; }
    std::vector< Polyline >  &		border( void )		{ return _border; }

    const std::vector< Polyline > &	river( void ) const	{ return _river; }
    std::vector< Polyline >  &		river( void )		{ return _river; }

    const std::vector< Polyline > &	road( void ) const	{ return _road; }
    std::vector< Polyline >  &		road( void )		{ return _road; }
    
//------------------------------------------------------------------------------
//	Fundamental functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	file I/O
//------------------------------------------------------------------------------
    void district	( const char * filename )	{ _district( filename ); }
    void flow		( const char * filename )	{ _flow    ( filename ); }
    void stream		( const char * filename )	{ _stream  ( filename ); }
    void network	( const char * filename )	{ _network ( filename ); }


//------------------------------------------------------------------------------
//	assignment opereators
//------------------------------------------------------------------------------
    Map & operator = ( const Map & obj );
				// assignment

//------------------------------------------------------------------------------
//	i/o functions
//------------------------------------------------------------------------------
    friend ostream & operator << ( ostream & stream, const Map & obj );
				// output
    friend istream & operator >> ( istream & stream, Map & obj );
				// input

//------------------------------------------------------------------------------
//	Class name
//------------------------------------------------------------------------------
    virtual const char * className( void ) const { return "Map"; }
				// class name
				// クラス名
};

#endif // _Map_H

// end of header file
// Do not add any stuff under this line.
