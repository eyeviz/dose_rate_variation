//==============================================================================
// Cloud.cpp
//	: program file for point clouds
//
//------------------------------------------------------------------------------
//
//				Time-stamp: "2021-05-13 01:23:31 shigeo"
//
//==============================================================================

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include <algorithm>

using namespace std;

#include "Common.h"
#include "Cloud.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//	Private Functions
//------------------------------------------------------------------------------
int Cloud::_nDim = 2;
int Cloud::_maxNeighbors = 3;
float Cloud::_maxRadius = 0.2;

//int Cloud::_maxNeighbors = 128;//<==
//float Cloud::_maxRadius = 1.0;//<==


//------------------------------------------------------------------------------
//	Protected Functions
//------------------------------------------------------------------------------
//
//  Cloud::_set --	set samples
//
//  Inputs
//	sample	: array of sample points
//
//  Outputs
//	none
//
void Cloud::_set( const std::vector< D_Point2 > & sample,
		  const std::vector< LandType > & value )
{
    if ( sample.size() != value.size() ) {
	cerr << HERE << "Improper inputs for neighbor search " << endl;
	assert( sample.size() == value.size() );
	return;
    }

    _point = cv::Mat_< float >( 0, _nDim );
    for ( unsigned int k = 0; k < sample.size(); ++k ) {
	cv::Mat row =
	    ( cv::Mat_< float >( 1, _nDim )
	      << ( float )sample[ k ].x(), ( float )sample[ k ].y() );
#ifdef SKIP
	if ( k % 1000 == 0 )
	    cerr << HERE << " k = " << k << " : "
		 << sample[ k ].x() << " , " << sample[ k ].y() << endl;
#endif	// SKIP
	_point.push_back( row );
    }

    // Number of KD-trees = 4
    if ( _flann_index != NULL ) {
	delete _flann_index;
	_flann_index = NULL;
    }
    _flann_index = new cv::flann::Index( _point, cv::flann::KDTreeIndexParams( 4 ) );

    _landtype = value;
}


//
//  Cloud::_nearest --	return the the land use at the nearest sample
//
//  Inputs
//	point
//
//  Outputs
//	none
//
LandType Cloud::_nearest( const double x, const double y )
{
    // cerr << HERE << endl;
    cv::Mat query =
	( cv::Mat_< float >( 1, _nDim ) << ( float )x, ( float )y );
    // cerr << HERE << " query.nRows = " << query.rows << endl;
    // cerr << HERE << " query.nCols = " << query.cols << endl;
    cv::Mat indices, dists;	// neither assume type nor size here!!!
    // search parmeter is set to be 32 by default.
    // We can make more precise our nearest neighbor search as we increase this
    // parameter but it will take more time
    assert( _flann_index != NULL );
    _flann_index->radiusSearch( query, indices, dists, _maxRadius, _maxNeighbors,
				cv::flann::SearchParams( 32 ) );
    int id = indices.at< int >( 0, 0 );
    if ( ( id < 0 ) || ( id >= _landtype.size() ) ) {
	cerr << HERE << " Cannot find the nearest sample!! " << endl;
	cerr << HERE << " id = " << id << " size = " << _landtype.size() << endl;
	id = 0;
    }

#ifdef DEBUG
    cerr << HERE << " Reference : " << x << " , " << y << endl;
    cerr << HERE << " Nearest ID = " << id << endl;
#endif	// DEBUG
    for ( unsigned int k = 0; k < 3; ++k ) {
	int nid = indices.at< int >( 0, k );
	float curDist = dists.at< float >( 0, k );
	if ( curDist > 0.1 ) {
	    cerr << HERE << " **** Too far neighbors " << endl;
	    cerr << HERE << " k = " << setw( 3 ) << k
		 << " id : " << nid
		 << " dists = "
		 <<  dists.at< float >( 0, k )
		 << " => coord :"
		 << _point.at< float >( nid, 0 ) << " , "
		 << _point.at< float >( nid, 1 ) << endl;
	}
    }

#ifdef DEBUG
    LandPlot::printType( _landtype[ id ] );
    cerr << endl;
#endif	// DEBUG
    return _landtype[ id ];
}

//------------------------------------------------------------------------------
//	Public Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	Constructors
//------------------------------------------------------------------------------

//
//  Cloud::Cloud --	default constructor
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Cloud::Cloud() 
{
    _point.release();
    _flann_index = NULL;
}


//
//  Cloud::Cloud --	copy constructor
//
//  Inputs
//	obj	: object of this class
//
//  Outputs
//	none
//
Cloud::Cloud( const Cloud & obj )
{
    _point		= obj._point;
}


//------------------------------------------------------------------------------
//	Destructor
//------------------------------------------------------------------------------

//
//  Cloud::~Cloud --	destructor
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Cloud::~Cloud()
{
    _point.release();
    _flann_index = NULL;
}


//------------------------------------------------------------------------------
//	Referrring to members
//------------------------------------------------------------------------------


//
//  Cloud::operator = --	assignement
//
//  Inputs
//	obj	: objects of this class
//
//  Outputs
//	this object
//
Cloud & Cloud::operator = ( const Cloud & obj )
{
    if ( this != &obj ) {
	_point		= obj._point;
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
ostream & operator << ( ostream & stream, const Cloud & obj )
{
    // stream << ( const %%% & )obj;	
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
istream & operator >> ( istream & stream, Cloud & obj )
{
    // stream >> ( %%% & )obj;
    return stream;
}


// end of header file
// Do not add any stuff under this line.
