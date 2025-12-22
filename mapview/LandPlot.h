//==============================================================================
// LandPlot.h
//	: header file for a set of land use plots
//
//------------------------------------------------------------------------------
//
//				Date: Fri Apr  6 00:07:03 2018
//
//==============================================================================

#ifndef _LandPlot_H		// begining of header file
#define _LandPlot_H		// notifying that this file is included

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------
#include <iostream>
using namespace std;

#include "shapefil.h"
#include "LandType.h"
#include "Delaunay.h"

// ************************************************************
// Caution: 
//
// You need to include opencv header files after including cgal/eigen header
// files
// Noted by ST on 2025/06/04
// ************************************************************
#include <opencv2/opencv.hpp>
// #include <opencv/cv.h>
using namespace cv;

//------------------------------------------------------------------------------
//	Defining Classes
//------------------------------------------------------------------------------

class LandPlot {

  private:

    void		_addMeshCode	( const unsigned int iRecord,
					  const unsigned int iField,
					  DBFHandle & hDBF );
    void		_addLandType	( const unsigned int iRecord,
					  const unsigned int iField,
					  DBFHandle & hDBF );

    void		_addString	( const unsigned int iRecord,
					  const unsigned int iField,
					  DBFHandle & hDBF );
    void		_addInteger	( const unsigned int iRecord,
					  const unsigned int iField,
					  DBFHandle & hDBF );
    void		_addDouble	( const unsigned int iRecord,
					  const unsigned int iField,
					  DBFHandle & hDBF );

  protected:

    // array of borders
    std::vector< D_Point2 >	_sample;
    std::vector< string >	_meshcode;	// Record #0
    std::vector< LandType >	_landtype;
    // [0]	0100:	RICE		rice field
    // [1]	0200:	FARM		other farmland
    // [2]	0500:	FOREST		forest
    // [3]	0600:	WASTE		wasteland (abandoned land)
    // [4]	0700:	URBAN		buidling sites
    // [5]	0901:	ROAD		roads
    // [6]	0902:	RAILWAY		railways
    // [7]	1000:	OTHER		other sites
    // [8]	1100:	MARSH		river and lakes
    // [9]	1400:	BEACH		seaside
    // [10]	1500:	OCEAN		ocean
    // [11]	1600:	GOLF		golf courses
    // [12]	xxxx:	OUT		outside the domain
    

    static int		_type2code	( const LandType ut );	
    static LandType	_code2type	( const int code );
    static void		_printType	( const LandType ut );

    void		_init		( void );
    void		_reset		( void );
    void		_normalize	( void );

    void		_append		( const char * fliename );
#ifdef SKIP
    void		_filter		( const Map & map );
#endif	// SKIP


  public:

//------------------------------------------------------------------------------
//	constructor
//	Constructors
//------------------------------------------------------------------------------
    LandPlot();			// default constructor
				// constructor (default)
    LandPlot( const LandPlot & obj );
				// copy constructor
				// copy constructor

//------------------------------------------------------------------------------
//	デストラクタ
//	Destructor
//------------------------------------------------------------------------------
    ~LandPlot();			// destructor
				// デストラクタ

//------------------------------------------------------------------------------
//	メンバ参照
//	Referrring to members
//------------------------------------------------------------------------------
    static string		usenames[ NUM_USES ];

    const std::vector< D_Point2 > &	sample( void ) const	{ return _sample; }
    std::vector< D_Point2 >	 &	sample( void )		{ return _sample; }
    
    const std::vector< LandType > &	landtype( void ) const 	{ return _landtype; }
    std::vector< LandType > &		landtype( void ) 	{ return _landtype; }
    
//------------------------------------------------------------------------------
//	converter
//------------------------------------------------------------------------------
    static int		type2code	( const LandType ut ) {
	return _type2code( ut );
    }
    static LandType	code2type	( const int code ) {
	return _code2type( code );
    }
    static void		printType	( const LandType ut ) {
	_printType( ut );
    }
    
//------------------------------------------------------------------------------
//	Fundamental functions
//------------------------------------------------------------------------------
    void init( void )				{ _init(); _reset(); }
    void reset( void )				{ _reset(); }
    void normalize( void )			{ _normalize(); }

//------------------------------------------------------------------------------
//	file I/O
//------------------------------------------------------------------------------
    void append	( const char * filename ) {
	_append( filename );
    }
#ifdef SKIP
    void filter	( const Map & map ) {
	_filter( map );
    }
#endif	// SKIP

//------------------------------------------------------------------------------
//	ソートのための関数
//	Functions for sorting
//------------------------------------------------------------------------------
#ifdef SKIP
    friend int compare ( const LandPlot * a, const LandPlot * b );
    friend int compare ( const LandPlot ** a, const LandPlot ** b );
				// comparison
				//  比較関数
#endif	// SKIP

//------------------------------------------------------------------------------
//	equality
//	equality
//------------------------------------------------------------------------------
#ifdef SKIP
    friend int operator == ( const LandPlot & a, const LandPlot & b );
    friend int operator != ( const LandPlot & a, const LandPlot & b ) {
	return ( ! ( a == b ) );
    }				// equality
				// equality
#endif	// SKIP

//------------------------------------------------------------------------------
//	assignment演算子
//	Assignment opereators
//------------------------------------------------------------------------------
    LandPlot & operator = ( const LandPlot & obj );
				// assignment
				// assignment

//------------------------------------------------------------------------------
//	入output
//	I/O functions
//------------------------------------------------------------------------------
    friend ostream & operator << ( ostream & stream, const LandPlot & obj );
				// output
				// output
    friend istream & operator >> ( istream & stream, LandPlot & obj );
				// input
				// input

//------------------------------------------------------------------------------
//	クラス名
//	Class name
//------------------------------------------------------------------------------
    virtual const char * className( void ) const { return "LandPlot"; }
				// class name
				// クラス名
};

#endif // _LandPlot_H

// end of header file
// Do not add any stuff under this line.
