//==============================================================================
// Polyline.cc
//	: program file for polylines
//
//------------------------------------------------------------------------------
//
//				Date: Thu Jun 11 16:43:01 2015
//
//==============================================================================

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include "Common.h"
#include "Polyline.h"


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
//  Polyline::_bound --	set the bounding box that confines the polyline
//
//  Inputs
//	none
//
//  Outputs
//	none
//
void Polyline::_bound( void )
{
    double minX = MAX_VALUE;
    double minY = MAX_VALUE;
    double maxX = -MAX_VALUE;
    double maxY = -MAX_VALUE;

    for ( unsigned int k = 0; k < size(); ++k ) {
	if ( minX > this->at( k ).x() ) minX = this->at( k ).x();
	if ( minY > this->at( k ).y() ) minY = this->at( k ).y();
	if ( maxX < this->at( k ).x() ) maxX = this->at( k ).x();
	if ( maxY < this->at( k ).y() ) maxY = this->at( k ).y();
    }
    _mincoord = Coord2( minX, minY );
    _maxcoord = Coord2( maxX, maxY );
}


//
//  Polyline::_area --	return the area of the bounding box that confines the polyline
//
//  Inputs
//	none
//
//  Outputs
//	area
//
double Polyline::_area( void )
{
    _bound();
    return ( ( _maxcoord.x() - _mincoord.x() ) * ( _maxcoord.y() - _mincoord.y() ) );
}


//------------------------------------------------------------------------------
//	Public Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	コンストラクタ
//	Constructors
//------------------------------------------------------------------------------

//
//  Polyline::Polyline --	default constructor
//			コンストラクタ(デフォルト)
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Polyline::Polyline() 
    : vector< Coord2 >()  	
{
    _bound();
}


//
//  Polyline::Polyline --	copy constructor
//			コピー・コンストラクタ
//
//  Inputs
//	obj	: object of this class
//
//  Outputs
//	none
//
Polyline::Polyline( const Polyline & obj )
    : vector< Coord2 >( ( const vector< Coord2 > & ) obj )
{
    _mincoord	= obj._mincoord;
    _maxcoord	= obj._maxcoord;
}


//------------------------------------------------------------------------------
//	デストラクタ
//	Destructor
//------------------------------------------------------------------------------

//
//  Polyline::~Polyline --	destructor
//			デストラクタ
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Polyline::~Polyline()
{
}


//------------------------------------------------------------------------------
//	メンバ参照
//	Referrring to members
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	ソートのための関数
//	Functions for sorting
//------------------------------------------------------------------------------

#ifdef SKIP
//
//  compare --	comparison function for sorting
//		ソートのための比較関数
//
//  Inputs
//	a, b	: pointers to objects of this class
//		  このクラスへのオブジェクトへのポインタ
//
//  Outputs
//	a <  b	-1
//	a == b	 0
//	a >  b	 1
//
int compare ( const Polyline * a, const Polyline * b )
{
    int (*cf)( const %%% *, const %%% * );
    cf = compare;
    return ( (*cf)( &(a->%%%), &(b->%%%) ) );

    return compare( ( const %%% * )a, ( const %%% * )b );
}
#endif	// SKIP

//------------------------------------------------------------------------------
//	等号
//	equality
//------------------------------------------------------------------------------

#ifdef SKIP
//
//  operator == --	equality
//			等号
//
//  Inputs
//	a, b	: objects of this class
//
//  Outputs
//	boolean value
//
int operator == ( const Polyline & a, const Polyline & b )
{

    return ( ( const %%% & )a == ( const %%% & )b );
}
#endif	// SKIP


//------------------------------------------------------------------------------
//	代入演算子
//	Assignment opereators
//------------------------------------------------------------------------------

//
//  Polyline::operator = --	assignement
//				代入
//
//  Inputs
//	obj	: objects of this class
//
//  Outputs
//	this object
//
Polyline & Polyline::operator = ( const Polyline & obj )
{
    if ( this != &obj ) {
	( vector< Coord2 > & )*this = obj;
	_mincoord	= obj._mincoord;
	_maxcoord	= obj._maxcoord;
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
ostream & operator << ( ostream & stream, const Polyline & obj )
{
    stream << obj.size() << endl;
    for ( unsigned int i = 0; i < obj.size(); ++i ) {
	stream << obj[ i ] << endl;
    }
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
istream & operator >> ( istream & stream, Polyline & obj )
{
    unsigned int num;
    stream >> num;
    obj.resize( num );
    for ( unsigned int i = 0; i < obj.size(); ++i ) {
	stream >> obj[ i ];
    }
    obj._bound();
    return stream;
}


