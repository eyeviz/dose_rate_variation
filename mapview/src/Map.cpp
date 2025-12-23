//==============================================================================
// Map.cc
//	: program file for maps
//
//------------------------------------------------------------------------------
//
//				Date: Wed Jun  7 15:12:21 2017
//
//==============================================================================


//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include <ctime>
#include <fstream>
#include <boost/format.hpp>
#include "shapefil.h"
#include "Common.h"
#include "Map.h"

using namespace std;


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Private Functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Protected Functions
//------------------------------------------------------------------------------

//
//  Map::_init --	initialize the network (called once when necessary)
//
//  Inputs
//	none
//
//  Outputs
//	none
//
void Map::_init( void )
{
    // const char theName[] = "void Map::_init : ";
    _border.clear();
}


//
//  Map::_district  --	load the borders of the district
//
//  Inputs
//	_filename	: file name
//
//  Outputs
//	none
//
// #define SHDEBUG
void Map::_district( const char * filename )
{
    // const double border_th = 1.0e-3;	// cutoff threshold for district areas
    const double border_th = 1.0e-8;	// cutoff threshold for district areas
    SHPHandle	hSHP;
    int		nShapeType, nEntities, iPart, nInvalidCount=0;
    const char 	*pszPlus;
    double 	adfMinBound[4], adfMaxBound[4];

//------------------------------------------------------------------------------
//	Open the passed shapefile.
//------------------------------------------------------------------------------
    hSHP = SHPOpen( filename, "rb" );

    if ( hSHP == NULL ) {
	cerr << "Unable to open: " << filename << endl;
	exit( 1 );
    }

//------------------------------------------------------------------------------
//	Print out the file bounds
//------------------------------------------------------------------------------
    SHPGetInfo( hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound );

#ifdef SHDEBUG
    cerr << boost::format( "Shapefile Type: %s   # of Shapes: %d\n\n")
	% SHPTypeName( nShapeType ) % nEntities;
    cerr << boost::format( "File Bounds: (%12.3f,%12.3f,%g,%g)\n"
			   "         to  (%12.3f,%12.3f,%g,%g)\n" )
	% adfMinBound[0] % adfMinBound[1] % adfMinBound[2] % adfMinBound[3]
	% adfMaxBound[0] % adfMaxBound[1] % adfMaxBound[2] % adfMaxBound[3];
#endif	// SHDEBUG

//------------------------------------------------------------------------------
//	Skim over the list of shapes, printing all the vertices.
//------------------------------------------------------------------------------

#ifdef SHDEBUG
    cerr << HERE << " nEntities = " << nEntities << endl;
#endif	// SHDEBUG

    vector< Polyline > newpoly;
    for ( int i = 0; i < nEntities; i++ ) {
        SHPObject	*psShape;

	psShape = SHPReadObject( hSHP, i );

#ifdef SHDEBUG
	cerr << boost::format( "\nShape:%d (%s)  nVertices=%d, nParts=%d\n"
			       "  Bounds:(%12.3f,%12.3f, %g, %g)\n"
			       "      to (%12.3f,%12.3f, %g, %g)\n" )
	    % i % SHPTypeName(psShape->nSHPType)
	    % psShape->nVertices % psShape->nParts
	    % psShape->dfXMin % psShape->dfYMin
	    % psShape->dfZMin % psShape->dfMMin
	    % psShape->dfXMax % psShape->dfYMax
	    % psShape->dfZMax % psShape->dfMMax;
	}
#endif	// SHDEBUG

	newpoly.clear();
	Polyline curpoly;
	curpoly.clear();
	for ( int j = 0, iPart = 1; j < psShape->nVertices; j++ ) {
            const char	*pszPartType = "";

            if ( j == 0 && psShape->nParts > 0 )
                pszPartType = SHPPartTypeName( psShape->panPartType[0] );

	    if ( iPart < psShape->nParts && psShape->panPartStart[iPart] == j ) {
                pszPartType = SHPPartTypeName( psShape->panPartType[iPart] );
		iPart++;
		pszPlus = "+";
		newpoly.push_back( curpoly );
		curpoly.clear();
	    }
	    else {
	        pszPlus = " ";
	    }

#ifdef SHDEBUG
	    cerr << boost::format( "   %s (%12.3f,%12.3f, %g, %g) %s \n" )
		% pszPlus
		% psShape->padfX[j] % psShape->padfY[j] % psShape->padfZ[j] % psShape->padfM[j]
		% pszPartType;
	    }
#endif	// SHDEBUG

	    Coord2 point( psShape->padfX[ j ], psShape->padfY[ j ] );
	    curpoly.push_back( point );
	}

        SHPDestroyObject( psShape );
	newpoly.push_back( curpoly );

	for ( unsigned int k = 0; k < newpoly.size(); ++k ) {
	    if ( newpoly[ k ].size() > 1 ) {
		double area = newpoly[ k ].area();
		if ( area > border_th ) _border.push_back( newpoly[ k ] );
		// else cerr << "S" << ends;
	    }
	}
    }

    SHPClose( hSHP );

    cerr << "No. of polylines = " << _border.size() << "  \tfile = " << filename << endl;

    return;
}


//
//  Map::_flow  --	load the rivers of the district
//
//  Inputs
//	_filename	: file name
//
//  Outputs
//	none
//
void Map::_flow( const char * filename )
{
    char buf[ BUF_SIZE ];
    ifstream ifs( filename );

    if ( ! ifs.is_open() ) {
	cerr << HERE << " Cannot open the data file : " << filename << endl;
    }

    istringstream istr;
    _river.clear();

    int nLines;
    ifs.getline( buf, sizeof( buf ) );
    istr.clear();
    istr.str( buf );
    istr >> nLines;
    cerr << HERE << " Number of lines = " << nLines << endl;
    
    for ( int i = 0; i < nLines; ++i ) {
	int nPoints;
	ifs.getline( buf, sizeof( buf ) );
	istr.clear();
	istr.str( buf );
	istr >> nPoints;
	// cerr << HERE << " Number of sample points = " << nPoints << endl;
    
	Polyline feat;
	for ( int j = 0; j < nPoints; ++j ) {
	    double lon, lat;
	    ifs.getline( buf, sizeof( buf ) );
	    istr.clear();
	    istr.str( buf );
	    istr >> lon >> lat;
	    Coord2 p( lon, lat );
	    feat.push_back( p );
	}

	_river.push_back( feat );
    }
    ifs.close();

    cerr << "No. of polylines = " << _river.size() << "  \tfile = " << filename << endl;

    return;
}


//
//  Map::_stream  --	load the river streams
//
//  Inputs
//	_filename	: file name
//
//  Outputs
//	none
//
// #define SHDEBUG
void Map::_stream( const char * filename )
{
    // const double river_th = 1.0e-12;	// cutoff threshold for district areas
    // const double river_th = 0.0;	// cutoff threshold for district areas
    SHPHandle	hSHP;
    int		nShapeType, nEntities, iPart, nInvalidCount=0;
    const char 	*pszPlus;
    double 	adfMinBound[4], adfMaxBound[4];

//------------------------------------------------------------------------------
//	Open the passed shapefile.
//------------------------------------------------------------------------------
    hSHP = SHPOpen( filename, "rb" );

    if ( hSHP == NULL ) {
	cerr << "Unable to open: " << filename << endl;
	exit( 1 );
    }

//------------------------------------------------------------------------------
//	Print out the file bounds
//------------------------------------------------------------------------------
    SHPGetInfo( hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound );

#ifdef SHDEBUG
    cerr << boost::format( "Shapefile Type: %s   # of Shapes: %d\n\n")
	% SHPTypeName( nShapeType ) % nEntities;
    cerr << boost::format( "File Bounds: (%12.3f,%12.3f,%g,%g)\n"
			   "         to  (%12.3f,%12.3f,%g,%g)\n" )
	% adfMinBound[0] % adfMinBound[1] % adfMinBound[2] % adfMinBound[3]
	% adfMaxBound[0] % adfMaxBound[1] % adfMaxBound[2] % adfMaxBound[3];
#endif	// SHDEBUG

//------------------------------------------------------------------------------
//	Skim over the list of shapes, printing all the vertices.
//------------------------------------------------------------------------------

#ifdef SHDEBUG
    cerr << HERE << " nEntities = " << nEntities << endl;
#endif	// SHDEBUG

    vector< Polyline > newpoly;
    for ( int i = 0; i < nEntities; i++ ) {
        SHPObject	*psShape;

	psShape = SHPReadObject( hSHP, i );

#ifdef SHDEBUG
	cerr << boost::format( "\nShape:%d (%s)  nVertices=%d, nParts=%d\n"
			       "  Bounds:(%12.3f,%12.3f, %g, %g)\n"
			       "      to (%12.3f,%12.3f, %g, %g)\n" )
	    % i % SHPTypeName(psShape->nSHPType)
	    % psShape->nVertices % psShape->nParts
	    % psShape->dfXMin % psShape->dfYMin
	    % psShape->dfZMin % psShape->dfMMin
	    % psShape->dfXMax % psShape->dfYMax
	    % psShape->dfZMax % psShape->dfMMax;
#endif	// SHDEBUG

	newpoly.clear();
	Polyline curpoly;
	curpoly.clear();
	for ( int j = 0, iPart = 1; j < psShape->nVertices; j++ ) {
            const char	*pszPartType = "";

            if ( j == 0 && psShape->nParts > 0 )
                pszPartType = SHPPartTypeName( psShape->panPartType[0] );

	    if ( iPart < psShape->nParts && psShape->panPartStart[iPart] == j ) {
                pszPartType = SHPPartTypeName( psShape->panPartType[iPart] );
		iPart++;
		pszPlus = "+";
		newpoly.push_back( curpoly );
		curpoly.clear();
	    }
	    else {
	        pszPlus = " ";
	    }

#ifdef SHDEBUG
	    cerr << boost::format( "   %s (%12.3f,%12.3f, %g, %g) %s \n" )
		% pszPlus
		% psShape->padfX[j] % psShape->padfY[j] % psShape->padfZ[j] % psShape->padfM[j]
		% pszPartType;
	    
#endif	// SHDEBUG

	    Coord2 point( psShape->padfX[ j ], psShape->padfY[ j ] );
	    curpoly.push_back( point );
	}

        SHPDestroyObject( psShape );
	newpoly.push_back( curpoly );

	for ( unsigned int k = 0; k < newpoly.size(); ++k ) {
	    if ( newpoly[ k ].size() > 1 ) {
		double area = newpoly[ k ].area();
		// if ( area > river_th ) _river.push_back( newpoly[ k ] );
		// else cerr << "S" << ends;
		_river.push_back( newpoly[ k ] );
	    }
	}
    }

    SHPClose( hSHP );

    cerr << "No. of polylines = " << _river.size() << "  \tfile = " << filename << endl;
    
    return;
}


//
//  Map::_network  --	load the road network
//
//  Inputs
//	_filename	: file name
//
//  Outputs
//	none
//
// #define SHDEBUG
void Map::_network( const char * filename )
{
    // const double road_th = 1.0e-12;	// cutoff threshold for district areas
    // const double road_th = 0.0;	// cutoff threshold for district areas
    SHPHandle	hSHP;
    int		nShapeType, nEntities, iPart, nInvalidCount=0;
    const char 	*pszPlus;
    double 	adfMinBound[4], adfMaxBound[4];

//------------------------------------------------------------------------------
//	Open the passed shapefile.
//------------------------------------------------------------------------------
    hSHP = SHPOpen( filename, "rb" );

    if ( hSHP == NULL ) {
	cerr << "Unable to open: " << filename << endl;
	exit( 1 );
    }

//------------------------------------------------------------------------------
//	Print out the file bounds
//------------------------------------------------------------------------------
    SHPGetInfo( hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound );

#ifdef SHDEBUG
    cerr << boost::format( "Shapefile Type: %s   # of Shapes: %d\n\n")
	% SHPTypeName( nShapeType ) % nEntities;
    cerr << boost::format( "File Bounds: (%12.3f,%12.3f,%g,%g)\n"
			   "         to  (%12.3f,%12.3f,%g,%g)\n" )
	% adfMinBound[0] % adfMinBound[1] % adfMinBound[2] % adfMinBound[3]
	% adfMaxBound[0] % adfMaxBound[1] % adfMaxBound[2] % adfMaxBound[3];
#endif	// SHDEBUG

//------------------------------------------------------------------------------
//	Skim over the list of shapes, printing all the vertices.
//------------------------------------------------------------------------------

#ifdef SHDEBUG
    cerr << HERE << " nEntities = " << nEntities << endl;
#endif	// SHDEBUG

    vector< Polyline > newpoly;
    for ( int i = 0; i < nEntities; i++ ) {
        SHPObject	*psShape;

	psShape = SHPReadObject( hSHP, i );

#ifdef SHDEBUG
	cerr << boost::format( "\nShape:%d (%s)  nVertices=%d, nParts=%d\n"
			       "  Bounds:(%12.3f,%12.3f, %g, %g)\n"
			       "      to (%12.3f,%12.3f, %g, %g)\n" )
	    % i % SHPTypeName(psShape->nSHPType)
	    % psShape->nVertices % psShape->nParts
	    % psShape->dfXMin % psShape->dfYMin
	    % psShape->dfZMin % psShape->dfMMin
	    % psShape->dfXMax % psShape->dfYMax
	    % psShape->dfZMax % psShape->dfMMax;
#endif	// SHDEBUG

	newpoly.clear();
	Polyline curpoly;
	curpoly.clear();
	for ( int j = 0, iPart = 1; j < psShape->nVertices; j++ ) {
            const char	*pszPartType = "";

            if ( j == 0 && psShape->nParts > 0 )
                pszPartType = SHPPartTypeName( psShape->panPartType[0] );

	    if ( iPart < psShape->nParts && psShape->panPartStart[iPart] == j ) {
                pszPartType = SHPPartTypeName( psShape->panPartType[iPart] );
		iPart++;
		pszPlus = "+";
		newpoly.push_back( curpoly );
		curpoly.clear();
	    }
	    else {
	        pszPlus = " ";
	    }

#ifdef SHDEBUG
	    cerr << boost::format( "   %s (%12.3f,%12.3f, %g, %g) %s \n" )
		% pszPlus
		% psShape->padfX[j] % psShape->padfY[j] % psShape->padfZ[j] % psShape->padfM[j]
		% pszPartType;
	    
#endif	// SHDEBUG

	    Coord2 point( psShape->padfX[ j ], psShape->padfY[ j ] );
	    curpoly.push_back( point );
	}

        SHPDestroyObject( psShape );
	newpoly.push_back( curpoly );

	for ( unsigned int k = 0; k < newpoly.size(); ++k ) {
	    if ( newpoly[ k ].size() > 1 ) {
		double area = newpoly[ k ].area();
		// if ( area > road_th ) _road.push_back( newpoly[ k ] );
		// else cerr << "S" << ends;
		_road.push_back( newpoly[ k ] );
	    }
	}
    }

    SHPClose( hSHP );

    cerr << "No. of polylines = " << _road.size() << "  \tfile = " << filename << endl;
    
    return;
}


//------------------------------------------------------------------------------
//	Public Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	コンストラクタ
//	Constructors
//------------------------------------------------------------------------------

//
//  Map::Map --	default constructor
//			コンストラクタ(デフォルト)
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Map::Map()
{
    _init();
}


//
//  Map::Map --	copy constructor
//			コピー・コンストラクタ
//
//  Inputs
//	obj	: object of this class
//
//  Outputs
//	none
//
Map::Map( const Map & obj )
{
    _init();
    if ( this != &obj ) {
	_border		= obj._border;
    }
}


//------------------------------------------------------------------------------
//	デストラクタ
//	Destructor
//------------------------------------------------------------------------------

//
//  Map::~Map --	destructor
//			デストラクタ
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Map::~Map()
{
    _border.clear();
}


//------------------------------------------------------------------------------
//	メンバ参照
//	Referrring to members
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Misc. functions
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//	Static functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Assignment opereators
//------------------------------------------------------------------------------

//
//  Map::operator = --	assignement
//
//  Inputs
//	obj	: objects of this class
//
//  Outputs
//	this object
//
Map & Map::operator = ( const Map & obj )
{
    if ( this != &obj ) {
	_border		= obj._border;
    }
    return *this;
}


//------------------------------------------------------------------------------
//	入出力
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
ostream & operator << ( ostream & stream, const Map & obj )
{
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
istream & operator >> ( istream & stream, Map & obj )
{
    return stream;
}


// end of header file
// Do not add any stuff under this line.
