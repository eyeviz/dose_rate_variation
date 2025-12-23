//==============================================================================
// Polyline.h
//	: header file for polylines
//
//------------------------------------------------------------------------------
//
//				Date: Thu Jun 11 16:42:48 2015
//
//==============================================================================

#ifndef _Polyline_H		// begining of header file
#define _Polyline_H		// notifying that this file is included

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include <vector>

using namespace std;

#include "Coord2.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Defining Classes
//------------------------------------------------------------------------------

class Polyline : public std::vector< Coord2 > {

  private:

  protected:

    Coord2	_mincoord;
    Coord2	_maxcoord;

    void	_bound( void );		// set the bounding box
    double	_area( void );		// return the area of the bounding box

  public:

//------------------------------------------------------------------------------
//	コンストラクタ
//	Constructors
//------------------------------------------------------------------------------
    Polyline();			// default constructor
				// コンストラクタ(デフォルト)	
    Polyline( const Polyline & obj );
				// copy constructor
				// コピー・コンストラクタ

//------------------------------------------------------------------------------
//	デストラクタ
//	Destructor
//------------------------------------------------------------------------------
    ~Polyline();			// destructor
				// デストラクタ

//------------------------------------------------------------------------------
//	メンバ参照
//	Referrring to members
//------------------------------------------------------------------------------
    const Coord2 & mincoord( void ) const	{ return _mincoord; }
    Coord2 & mincoord( void ) 			{ return _mincoord; }
    const Coord2 & maxcoord( void ) const	{ return _maxcoord; }
    Coord2 & maxcoord( void )			{ return _maxcoord; }

    void bound( void )				{ _bound(); }
    double area( void )				{ return _area(); }

//------------------------------------------------------------------------------
//	ソートのための関数
//	Functions for sorting
//------------------------------------------------------------------------------
#ifdef SKIP
    friend int compare ( const Polyline * a, const Polyline * b );
    friend int compare ( const Polyline ** a, const Polyline ** b );
				// comparison
				//  比較関数
#endif	// SKIP

//------------------------------------------------------------------------------
//	等号
//	equality
//------------------------------------------------------------------------------
#ifdef SKIP
    friend int operator == ( const Polyline & a, const Polyline & b );
    friend int operator != ( const Polyline & a, const Polyline & b ) {
	return ( ! ( a == b ) );
    }				// equality
				// 等号
#endif	// SKIP

//------------------------------------------------------------------------------
//	代入演算子
//	Assignment opereators
//------------------------------------------------------------------------------
    Polyline & operator = ( const Polyline & obj );
				// assignment
				// 代入

//------------------------------------------------------------------------------
//	入出力
//	I/O functions
//------------------------------------------------------------------------------
    friend ostream & operator << ( ostream & stream, const Polyline & obj );
				// output
				// 出力
    friend istream & operator >> ( istream & stream, Polyline & obj );
				// input
				// 入力

//------------------------------------------------------------------------------
//	クラス名
//	Class name
//------------------------------------------------------------------------------
    virtual const char * className( void ) const { return "Polyline"; }
				// class name
				// クラス名
};

#ifdef SKIP
inline int arrange( const Polyline * a, const Polyline * b ) {
    return compare( a, b );
}				// ソートのためのふたつのベクトルの比較
#endif	// SKIP



#endif // _Polyline_H

// end of header file
// Do not add any stuff under this line.
