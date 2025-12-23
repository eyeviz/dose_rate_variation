//==============================================================================
// Height.cpp
//	: program file for the height fields
//
//------------------------------------------------------------------------------
//
//				Time-stamp: "2025-12-16 20:56:50 shigeo"
//
//==============================================================================

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

using Eigen::Matrix2d;
using Eigen::Matrix2f;
using Eigen::SelfAdjointEigenSolver;

#include "Vars.h"
#include "Common.h"
#include "Height.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------
// #define DEBUG_FOR_TRACKING_FEATURE_VALUES

#define NUM_STAGES	(100000) // for debug messages




//------------------------------------------------------------------------------
//	Private Functions
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//	Protected Functions
//------------------------------------------------------------------------------

//
//  Height::_loadAttrs --	
//
//  Inputs
//	region : array of attribute values including geographic positions
//
//  Outputs
//	none
//
void Height::_loadAttrs( const ublas::matrix< double > & region )
{
    // const unsigned int nFixed = FIXED_VARS;
#ifdef SKIP
    std::vector< D_Point2 > sample;
#endif	// SKIP
    unsigned sz = region.size1();

    if ( sz == 0 ) {
	cerr << "Input data is empty" << endl;
    }
#ifdef SKIP
    sample.clear();
#endif	// SKIP

    _T.clear();
    _lambda.clear();
    _phi.clear();
    _coordX.clear();
    _coordY.clear();

    _attrH.clear();
    _attrD.clear();
    _attrG.clear();

    _attrPriAve.clear();
    _attrPosAve.clear();
    _attrPriSdv.clear();
    _attrPosSdv.clear();

    _attrValue.clear();
    _attrP.clear();
    _attrQ.clear();
    _attrR.clear();

    if ( ( _idP == NO_INDEX ) || ( _idQ == NO_INDEX ) || ( _idR == NO_INDEX ) ) {
	cerr << HERE
	     << " _idP = " << _idP
	     << " _idQ = " << _idQ
	     << " _idR = " << _idR << endl;
	assert( _idP != NO_INDEX );
	assert( _idQ != NO_INDEX );
	assert( _idR != NO_INDEX );
    }
    
    cerr << HERE << " No of samples = " << sz << endl;
    cerr << HERE << " Loading ..." << endl;
    for ( unsigned int k = 0; k < sz; ++k ) {
	double y, x, H, D, G;
	// int land;
	double priAve, posAve, priSdv, posSdv;
	double ratioF, ratioM, ratioL;
	std::vector< double > value;
	double P, Q, R;
	
	x = region( k, 0 );
	y = region( k, 1 );
	H = region( k, 2 );
	D = region( k, 3 );
	// land = ( int )region( k, 4 );
	G = region( k, 4 );

	priSdv = region( k, 5 );
	posSdv = region( k, 6 );
	priAve = region( k, 7 );
	posAve = region( k, 8 );

	ratioF  = region( k, 9 );
	ratioM  = region( k, 10 );
	ratioL  = region( k, 11 );
	
	value.clear();
	for ( unsigned int i = 0; i < region.size2(); ++i )
	    value.push_back( region( k, i ) );

	P = region( k, _idP );
	Q = region( k, _idQ );
	R = region( k, _idR );

	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	_lambda.push_back( x );
	_phi.push_back( y );
	_coordX.push_back( x );
	_coordY.push_back( y );
	_attrH.push_back( H );
	_attrD.push_back( D );
	// _attrLand.push_back( land );
	_attrG.push_back( G );

	_attrPriAve.push_back( priAve );
	_attrPosAve.push_back( posAve );
	_attrPriSdv.push_back( priSdv );
	_attrPosSdv.push_back( posSdv );
	
	_attrRatioF.push_back( ratioF );
	_attrRatioM.push_back( ratioM );
	_attrRatioL.push_back( ratioL );

	_attrValue.push_back( value );

	_attrP.push_back( P );
	_attrQ.push_back( Q );
	_attrR.push_back( R );

#ifdef DEBUG_FOR_TRACKING_FEATURE_VALUES
	//------------------------------------------------------------------------------
	const double targetX = 140.547040;
	const double targetY = 37.851180;
	const double eps = 0.00001;

	if ( ( abs( x - targetX ) < eps ) && ( abs( y - targetY ) < eps ) ) {
	    cerr << HERE << " k = " << setw( 8 ) << k << endl;
	    cerr << HERE 
		 << " Longitude : " << fixed << setprecision( 8 )
		 << targetX << " is found => "
		 << fixed << setprecision( 8 ) << x << endl;
	    cerr << HERE
		 << " Latitude : "  << fixed << setprecision( 8 )
		 << targetY << " is found => "
		 << fixed << setprecision( 8 ) << y << endl;
	    cerr << HERE
		 << " P, Q, R : "  << fixed << setprecision( 8 )
		 << fixed << setprecision( 8 ) << P << " , "
		 << Q << " , " << R << endl;
	    getchar();
	}
	//------------------------------------------------------------------------------
#endif	// DEBUG_FOR_TRACKING_FEATURE_VALUES

	
	if ( k % NUM_STAGES == 0 ) {
	    cerr << HERE << " No of samples = " << k << endl;
#ifdef SKIP
	    cerr << HERE << " x = " << x << " y = " << y
		 << " H = " << H << " D = " << D << endl;
	    cerr << HERE << " Ave Pri = " << priAve << " Pos = " << posAve << endl;
	    cerr << HERE << " Sdv Pri = " << priSdv << " Pos = " << posSdv << endl;
	    cerr << endl;
#endif	// SKIP
	}
    }
    cerr << " finished" << endl;
}


//
//  Height::_triangulate --	triangulate the domain by plotting the input
//				attribute values
//
//  Inputs
//	region : array of attribute values including geographic positions
//
//  Outputs
//	none
//
void Height::_triangulate( const std::vector< D_Point2 > & sample )
{
    for ( unsigned int i = 0; i < sample.size(); ++i ) {
	D_Vertex_handle vh = _T.push_back( sample[ i ] );
	vh->setID	( i );
	vh->setLambda	( _lambda[ i ] );
	vh->setPhi	( _phi[ i ] );
	if ( i % NUM_STAGES == 0 )
	    cerr << HERE << " Triangulating the domain with sample No. = " << i << endl;
    }
    cerr << HERE << " Triangulating the domain with sample No. = " << sample.size() << endl;
    cerr << HERE << " Domain triangulation ... done" << endl;
}

//
//  Height::_autoNorm --	automatically normalize the data samples
//
//  Inputs
//	none
//
//  Outputs
//	none
//
void Height::_autoNorm( std::vector< D_Point2 > & sample,
			interval< double > & rngH,
			interval< double > & rngD,
			interval< double > & rngG,
			interval< double > & rngPriAve,
			interval< double > & rngPosAve,
			interval< double > & rngPriSdv,
			interval< double > & rngPosSdv,
			interval< double > & rngRatioF,
			interval< double > & rngRatioM,
			interval< double > & rngRatioL,
			interval< double > & rngP,
			interval< double > & rngQ,
			interval< double > & rngR )
{
    assert( _coordX.size() == _coordY.size() );

    unsigned sz = _coordX.size();

    cerr << HERE << " size of the samples = " << sz << endl;

    sample.clear();
    // normalization
    double minX = INFINITY, maxX = -INFINITY;
    double minY = INFINITY, maxY = -INFINITY;
    double minH = INFINITY, maxH = -INFINITY;
    double minD = INFINITY, maxD = -INFINITY;
    double minG = INFINITY, maxG = -INFINITY;
    double minPriAve = INFINITY, maxPriAve = -INFINITY;
    double minPosAve = INFINITY, maxPosAve = -INFINITY;
    double minPriSdv = INFINITY, maxPriSdv = -INFINITY;
    double minPosSdv = INFINITY, maxPosSdv = -INFINITY;
    double minRatioF = INFINITY, maxRatioF = -INFINITY;
    double minRatioM = INFINITY, maxRatioM = -INFINITY;
    double minRatioL = INFINITY, maxRatioL = -INFINITY;
    double minP = INFINITY, maxP = -INFINITY;
    double minQ = INFINITY, maxQ = -INFINITY;
    double minR = INFINITY, maxR = -INFINITY;

    for ( unsigned int k = 0; k < sz; ++k ) {

	if ( _coordX[ k ] < minX ) minX = _coordX[ k ]; 
	if ( _coordX[ k ] > maxX ) maxX = _coordX[ k ]; 

	if ( _coordY[ k ] < minY ) minY = _coordY[ k ]; 
	if ( _coordY[ k ] > maxY ) maxY = _coordY[ k ]; 

	if ( _attrH[ k ] < minH ) minH = _attrH[ k ]; 
	if ( _attrH[ k ] > maxH ) maxH = _attrH[ k ]; 

	if ( _attrD[ k ] < minD ) minD = _attrD[ k ]; 
	if ( _attrD[ k ] > maxD ) maxD = _attrD[ k ]; 

	if ( _attrG[ k ] < minG ) minG = _attrG[ k ]; 
	if ( _attrG[ k ] > maxG ) maxG = _attrG[ k ]; 

	if ( _attrPriAve[ k ] < minPriAve ) minPriAve = _attrPriAve[ k ];
	if ( _attrPriAve[ k ] > maxPriAve ) maxPriAve = _attrPriAve[ k ];

	if ( _attrPosAve[ k ] < minPosAve ) minPosAve = _attrPosAve[ k ];
	if ( _attrPosAve[ k ] > maxPosAve ) maxPosAve = _attrPosAve[ k ];

	if ( _attrPriSdv[ k ] < minPriSdv ) minPriSdv = _attrPriSdv[ k ];
	if ( _attrPriSdv[ k ] > maxPriSdv ) maxPriSdv = _attrPriSdv[ k ];

	if ( _attrPosSdv[ k ] < minPosSdv ) minPosSdv = _attrPosSdv[ k ];
	if ( _attrPosSdv[ k ] > maxPosSdv ) maxPosSdv = _attrPosSdv[ k ];

    	if ( _attrRatioF[ k ] < minRatioF ) minRatioF = _attrRatioF[ k ];
	if ( _attrRatioF[ k ] > maxRatioF ) maxRatioF = _attrRatioF[ k ];

    	if ( _attrRatioM[ k ] < minRatioM ) minRatioM = _attrRatioM[ k ];
	if ( _attrRatioM[ k ] > maxRatioM ) maxRatioM = _attrRatioM[ k ];

    	if ( _attrRatioL[ k ] < minRatioL ) minRatioL = _attrRatioL[ k ];
	if ( _attrRatioL[ k ] > maxRatioL ) maxRatioL = _attrRatioL[ k ];

	if ( _attrP[ k ] < minP ) minP = _attrP[ k ];
	if ( _attrP[ k ] > maxP ) maxP = _attrP[ k ];

	if ( _attrQ[ k ] < minQ ) minQ = _attrQ[ k ];
	if ( _attrQ[ k ] > maxQ ) maxQ = _attrQ[ k ];

	if ( _attrR[ k ] < minR ) minR = _attrR[ k ];
	if ( _attrR[ k ] > maxR ) maxR = _attrR[ k ];
    }

    cerr << HERE << " Before range adjustment in the normalization : " << endl;
    cerr << HERE << " minX = " << minX << " maxX = " << maxX << endl;
    cerr << HERE << " minY = " << minY << " maxY = " << maxY << endl;
    cerr << HERE << " minH = " << minH << " maxH = " << maxH << endl;
    cerr << HERE << " minD = " << minD << " maxD = " << maxD << endl;
    cerr << HERE << " minG = " << minG << " maxG = " << maxG << endl;
    cerr << HERE << " minPriAve = " << minPriAve << " maxPriAve = " << maxPriAve << endl;
    cerr << HERE << " minPosAve = " << minPosAve << " maxPosAve = " << maxPosAve << endl;
    cerr << HERE << " minPriSdv = " << minPriSdv << " maxPriSdv = " << maxPriSdv << endl;
    cerr << HERE << " minPosSdv = " << minPosSdv << " maxPosSdv = " << maxPosSdv << endl;
    cerr << HERE << " minRatioF = " << minRatioF << " maxRatioF = " << maxRatioF << endl;
    cerr << HERE << " minRatioM = " << minRatioM << " maxRatioM = " << maxRatioM << endl;
    cerr << HERE << " minRatioL = " << minRatioL << " maxRatioL = " << maxRatioL << endl;
    cerr << HERE << " minP = " << minP << " maxP = " << maxP << endl;
    cerr << HERE << " minQ = " << minQ << " maxP = " << maxQ << endl;
    cerr << HERE << " minR = " << minR << " maxR = " << maxR << endl;

#ifdef NIIDA_COMBINED_RANGES_FOR_P_AND_Q
    if ( minP < minQ ) minQ = minP;
    if ( minP > minQ ) minP = minQ;
    if ( maxP < maxQ ) maxP = maxQ;
    if ( maxP > maxQ ) maxQ = maxP;
    cerr << HERE << "******************************" << endl;
    cerr << HERE << " Combined range for P and Q in the NIIDA River case" << endl;
    cerr << HERE << " minP = " << minP << " maxP = " << maxP << endl;
    cerr << HERE << " minQ = " << minQ << " maxP = " << maxQ << endl;
    cerr << HERE << "******************************" << endl;
    cerr << getchar();
#endif	// NIIDA_COMBINED_RANGES_FOR_P_AND_Q
    
    if ( _rngX.lower() == _rngX.upper() ) _rngX.assign( minX, maxX );
    else {
	minX = _rngX.lower(); maxX = _rngX.upper();
    }
    if ( _rngY.lower() == _rngY.upper() ) _rngY.assign( minY, maxY );
    else {
	minY = _rngY.lower(); maxY = _rngY.upper();
    }
    if ( _rngH.lower() == _rngH.upper() ) _rngH.assign( minH, maxH );
    else {
	minH = _rngH.lower(); maxH = _rngH.upper();
    }
    if ( _rngD.lower() == _rngD.upper() ) _rngD.assign( minD, maxD );
    else {
	minD = _rngD.lower(); maxD = _rngD.upper();
    }
    if ( _rngG.lower() == _rngG.upper() ) _rngG.assign( minG, maxG );
    else {
	minG = _rngG.lower(); maxG = _rngG.upper();
    }

    if ( _rngPriAve.lower() == _rngPriAve.upper() )
	_rngPriAve.assign( minPriAve, maxPriAve );
    else {
	minPriAve = _rngPriAve.lower(); maxPriAve = _rngPriAve.upper();
    }
    if ( _rngPosAve.lower() == _rngPosAve.upper() )
	_rngPosAve.assign( minPosAve, maxPosAve );
    else {
	minPosAve = _rngPosAve.lower(); maxPosAve = _rngPosAve.upper();
    }

    if ( _rngPriSdv.lower() == _rngPriSdv.upper() )
	_rngPriSdv.assign( minPriSdv, maxPriSdv );
    else {
	minPriSdv = _rngPriSdv.lower(); maxPriSdv = _rngPriSdv.upper();
    }
    if ( _rngPosSdv.lower() == _rngPosSdv.upper() )
	_rngPosSdv.assign( minPosSdv, maxPosSdv );
    else {
	minPosSdv = _rngPosSdv.lower(); maxPosSdv = _rngPosSdv.upper();
    }

    if ( _rngRatioF.lower() == _rngRatioF.upper() )
	_rngRatioF.assign( minRatioF, maxRatioF );
    else {
	minRatioF = _rngRatioF.lower(); maxRatioF = _rngRatioF.upper();
    }
    if ( _rngRatioM.lower() == _rngRatioM.upper() )
	_rngRatioM.assign( minRatioM, maxRatioM );
    else {
	minRatioM = _rngRatioM.lower(); maxRatioM = _rngRatioM.upper();
    }
    if ( _rngRatioL.lower() == _rngRatioL.upper() )
	_rngRatioL.assign( minRatioL, maxRatioL );
    else {
	minRatioL = _rngRatioL.lower(); maxRatioL = _rngRatioL.upper();
    }

    if ( _rngP.lower() == _rngP.upper() )
	_rngP.assign( minP, maxP );
    else {
	minP = _rngP.lower(); maxP = _rngP.upper();
    }
    if ( _rngQ.lower() == _rngQ.upper() )
	_rngQ.assign( minQ, maxQ );
    else {
	minQ = _rngQ.lower(); maxQ = _rngQ.upper();
    }
    if ( _rngR.lower() == _rngR.upper() )
	_rngR.assign( minR, maxR );
    else {
	minR = _rngR.lower(); maxR = _rngR.upper();
    }
    
    cerr << HERE << " After range adjustment : " << endl;
    cerr << HERE << " minX = " << minX << " maxX = " << maxX << endl;
    cerr << HERE << " minY = " << minY << " maxY = " << maxY << endl;
    cerr << HERE << " minH = " << minH << " maxH = " << maxH << endl;
    cerr << HERE << " minD = " << minD << " maxD = " << maxD << endl;
    cerr << HERE << " minG = " << minG << " maxG = " << maxG << endl;

    cerr << HERE << " minPriAve = " << minPriAve << " maxPriAve = " << maxPriAve << endl;
    cerr << HERE << " minPosAve = " << minPosAve << " maxPosAve = " << maxPosAve << endl;
    cerr << HERE << " minPriSdv = " << minPriSdv << " maxPriSdv = " << maxPriSdv << endl;
    cerr << HERE << " minPosSdv = " << minPosSdv << " maxPosSdv = " << maxPosSdv << endl;
    cerr << HERE << " minRatioF = " << minRatioF << " maxRatioF = " << maxRatioF << endl;
    cerr << HERE << " minRatioM = " << minRatioM << " maxRatioM = " << maxRatioM << endl;
    cerr << HERE << " minRatioL = " << minRatioL << " maxRatioL = " << maxRatioL << endl;

    cerr << HERE << " minP = " << minP << " maxP = " << maxP << endl;
    cerr << HERE << " minQ = " << minQ << " maxQ = " << maxQ << endl;
    cerr << HERE << " minR = " << minR << " maxR = " << maxR << endl;

    rngH = interval< double >( minH, maxH );
    rngD = interval< double >( minD, maxD );
    rngG = interval< double >( minG, maxG );

    rngPriAve = interval< double >( minPriAve, maxPriAve );
    rngPosAve = interval< double >( minPosAve, maxPosAve );
    rngPriSdv = interval< double >( minPriSdv, maxPriSdv );
    rngPosSdv = interval< double >( minPosSdv, maxPosSdv );
    rngRatioF = interval< double >( minRatioF, maxRatioF );
    rngRatioM = interval< double >( minRatioM, maxRatioM );
    rngRatioL = interval< double >( minRatioL, maxRatioL  );
    
    rngP = interval< double >( minP, maxP );
    rngQ = interval< double >( minQ, maxQ );
    rngR = interval< double >( minR, maxR );
    
    double widthX = maxX - minX;
    double widthY = maxY - minY;
    double widthH = maxH - minH;
    double widthD = maxD - minD;
    double widthG = maxG - minG;

    double widthPriAve = maxPriAve - minPriAve;
    double widthPosAve = maxPosAve - minPosAve;
    double widthPriSdv = maxPriSdv - minPriSdv;
    double widthPosSdv = maxPosSdv - minPosSdv;

    double widthRatioF = maxRatioF - minRatioF;
    double widthRatioM = maxRatioM - minRatioM;
    double widthRatioL = maxRatioL - minRatioL;

    double widthP = maxP - minP;
    double widthQ = maxQ - minQ;
    double widthR = maxR - minR;

    for ( unsigned int k = 0; k < sz; ++k ) {
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	_coordX[ k ] = 2.0 * ( _coordX[ k ] - minX )/widthX - 1.0;
	_coordY[ k ] = 2.0 * ( _coordY[ k ] - minY )/widthY - 1.0;
	_attrH[ k ]  = 2.0 * ( _attrH[ k ] - minH )/widthH - 1.0;
	_attrD[ k ]  = 2.0 * ( _attrD[ k ] - minD )/widthD - 1.0;
	_attrG[ k ]  = 2.0 * ( _attrG[ k ] - minG )/widthG - 1.0;

	_attrPriAve[ k ]  = 2.0 * ( _attrPriAve[ k ] - minPriAve )/widthPriAve - 1.0;
	_attrPosAve[ k ]  = 2.0 * ( _attrPosAve[ k ] - minPosAve )/widthPosAve - 1.0;
	_attrPriSdv[ k ]  = 2.0 * ( _attrPriSdv[ k ] - minPriSdv )/widthPriSdv - 1.0;
	_attrPosSdv[ k ]  = 2.0 * ( _attrPosSdv[ k ] - minPosSdv )/widthPosSdv - 1.0;

	_attrRatioF[ k ]  = 2.0 * ( _attrRatioF[ k ] - minRatioF )/widthRatioF - 1.0;
	_attrRatioM[ k ]  = 2.0 * ( _attrRatioM[ k ] - minRatioM )/widthRatioM - 1.0;
	_attrRatioL[ k ]  = 2.0 * ( _attrRatioL[ k ] - minRatioL )/widthRatioL - 1.0;

	_attrP[ k ]  = 2.0 * ( _attrP[ k ] - minP )/widthP - 1.0;
	_attrQ[ k ]  = 2.0 * ( _attrQ[ k ] - minQ )/widthQ - 1.0;
	_attrR[ k ]  = 2.0 * ( _attrR[ k ] - minR )/widthR - 1.0;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	_attrH[ k ]  = ( _attrH[ k ] - minH )/widthH;
	_attrD[ k ]  = ( _attrD[ k ] - minD )/widthD;
	_attrG[ k ]  = ( _attrG[ k ] - minG )/widthG;

	_attrPriAve[ k ] = ( _attrPriAve[ k ] - minPriAve ) / widthPriAve;
	_attrPosAve[ k ] = ( _attrPosAve[ k ] - minPosAve ) / widthPosAve;
	_attrPriSdv[ k ] = ( _attrPriSdv[ k ] - minPriSdv ) / widthPriSdv;
	_attrPosSdv[ k ] = ( _attrPosSdv[ k ] - minPosSdv ) / widthPosSdv;

	_attrRatioF[ k ] = ( _attrRatioF[ k ] - minRatioF ) / widthRatioF;
	_attrRatioM[ k ] = ( _attrRatioM[ k ] - minRatioM ) / widthRatioM;
	_attrRatioL[ k ] = ( _attrRatioL[ k ] - minRatioL ) / widthRatioL;

#ifdef DEBUG_FOR_TRACKING_FEATURE_VALUES
	if ( k == 801525 ) {
	    cerr << HERE << " Before "
		 << " P = " << _attrP[ k ]
		 << " Q = " << _attrQ[ k ]
		 << " R = " << _attrR[ k ] << endl;
	}
#endif	// DEBUG_FOR_TRACKING_FEATURE_VALUES

	_attrP[ k ] = ( _attrP[ k ] - minP ) / widthP;
	_attrQ[ k ] = ( _attrQ[ k ] - minQ ) / widthQ;
	_attrR[ k ] = ( _attrR[ k ] - minR ) / widthR;

#ifdef DEBUG_FOR_TRACKING_FEATURE_VALUES
	if ( k == 801525 ) {
	    cerr << HERE << " After "
		 << " P = " << _attrP[ k ]
		 << " Q = " << _attrQ[ k ]
		 << " R = " << _attrR[ k ] << endl;
	    getchar();
	}
#endif	// DEBUG_FOR_TRACKING_FEATURE_VALUES

#endif	// ENABLE_MAP_COORD_NORMALIZATION

	sample.push_back( D_Point2( _coordX[ k ], _coordY[ k ] ) );
    }
}

//
//  Height::_manualNorm --	automatically normalize the data samples
//
//  Inputs
//	none
//
//  Outputs
//	none
//
void Height::_manualNorm( std::vector< D_Point2 > & sample,
			  interval< double > & rngH,
			  interval< double > & rngD,
			  interval< double > & rngG,
			  interval< double > & rngPriAve,
			  interval< double > & rngPosAve,
			  interval< double > & rngPriSdv,
			  interval< double > & rngPosSdv,
			  interval< double > & rngRatioF,
			  interval< double > & rngRatioM,
			  interval< double > & rngRatioL,
			  interval< double > & rngP,
			  interval< double > & rngQ,
			  interval< double > & rngR )
{
    assert( _coordX.size() == _coordY.size() );

    cerr << HERE << " input intervals at manualNorm" << endl;
    cerr << HERE << " rngH = " << rngH;
    cerr << HERE << " rngD = " << rngD;
    cerr << HERE << " rngG = " << rngG;
    cerr << HERE << " rngPriAve = " << rngPriAve;
    cerr << HERE << " rngPosAve = " << rngPosAve;
    cerr << HERE << " rngPriSdv = " << rngPriSdv;
    cerr << HERE << " rngPosSdv = " << rngPosSdv;
    cerr << HERE << " rngRatioF = " << rngRatioF;
    cerr << HERE << " rngRatioM = " << rngRatioM;
    cerr << HERE << " rngRatioL = " << rngRatioL;
    cerr << HERE << " rngP = " << rngP;
    cerr << HERE << " rngQ = " << rngQ;
    cerr << HERE << " rngR = " << rngR;
    
    unsigned sz = _coordX.size();
    sample.clear();
    // normalization
    double minX = INFINITY, maxX = -INFINITY;
    double minY = INFINITY, maxY = -INFINITY;
    double minH = INFINITY, maxH = -INFINITY;
    double minD = INFINITY, maxD = -INFINITY;
    double minG = INFINITY, maxG = -INFINITY;
    double minPriAve = INFINITY, maxPriAve = -INFINITY;
    double minPosAve = INFINITY, maxPosAve = -INFINITY;
    double minPriSdv = INFINITY, maxPriSdv = -INFINITY;
    double minPosSdv = INFINITY, maxPosSdv = -INFINITY;
    double minRatioF = INFINITY, maxRatioF = -INFINITY;
    double minRatioM = INFINITY, maxRatioM = -INFINITY;
    double minRatioL = INFINITY, maxRatioL = -INFINITY;
    double minP = INFINITY, maxP = -INFINITY;
    double minQ = INFINITY, maxQ = -INFINITY;
    double minR = INFINITY, maxR = -INFINITY;

    for ( unsigned int k = 0; k < sz; ++k ) {
	if ( _coordX[ k ] < minX ) minX = _coordX[ k ]; 
	if ( _coordX[ k ] > maxX ) maxX = _coordX[ k ]; 

	if ( _coordY[ k ] < minY ) minY = _coordY[ k ]; 
	if ( _coordY[ k ] > maxY ) maxY = _coordY[ k ]; 

	if ( _attrH[ k ] < minH ) minH = _attrH[ k ];
	if ( _attrH[ k ] > maxH ) maxH = _attrH[ k ];

	if ( _attrD[ k ] < minD ) minD = _attrD[ k ];
	if ( _attrD[ k ] > maxD ) maxD = _attrD[ k ];

	if ( _attrG[ k ] < minG ) minG = _attrG[ k ];
	if ( _attrG[ k ] > maxG ) maxG = _attrG[ k ];
	
	if ( _attrPriAve[ k ] < minPriAve ) minPriAve = _attrPriAve[ k ];
	if ( _attrPriAve[ k ] > maxPriAve ) maxPriAve = _attrPriAve[ k ];
	if ( _attrPosAve[ k ] < minPosAve ) minPosAve = _attrPosAve[ k ];
	if ( _attrPosAve[ k ] > maxPosAve ) maxPosAve = _attrPosAve[ k ];
	if ( _attrPriSdv[ k ] < minPriSdv ) minPriSdv = _attrPriSdv[ k ];
	if ( _attrPriSdv[ k ] > maxPriSdv ) maxPriSdv = _attrPriSdv[ k ];
	if ( _attrPosSdv[ k ] < minPosSdv ) minPosSdv = _attrPosSdv[ k ];
	if ( _attrPosSdv[ k ] > maxPosSdv ) maxPosSdv = _attrPosSdv[ k ];

	if ( _attrRatioF[ k ] < minRatioF ) minRatioF = _attrRatioF[ k ];
	if ( _attrRatioF[ k ] > maxRatioF ) maxRatioF = _attrRatioF[ k ];
	if ( _attrRatioM[ k ] < minRatioM ) minRatioM = _attrRatioM[ k ];
	if ( _attrRatioM[ k ] > maxRatioM ) maxRatioM = _attrRatioM[ k ];
	if ( _attrRatioL[ k ] < minRatioL ) minRatioL = _attrRatioL[ k ];
	if ( _attrRatioL[ k ] > maxRatioL ) maxRatioL = _attrRatioL[ k ];

	if ( _attrP[ k ] < minP ) minP = _attrP[ k ];
	if ( _attrP[ k ] > maxP ) maxP = _attrP[ k ];
	if ( _attrQ[ k ] < minQ ) minQ = _attrQ[ k ];
	if ( _attrQ[ k ] > maxQ ) maxQ = _attrQ[ k ];
	if ( _attrR[ k ] < minR ) minR = _attrR[ k ];
	if ( _attrR[ k ] > maxR ) maxR = _attrR[ k ];
    }

    cerr << HERE << " Computing ranges (just for checking not used in the program)" << endl;
    cerr << HERE << " minX = " << minX << " maxX = " << maxX << endl;
    cerr << HERE << " minY = " << minY << " maxY = " << maxY << endl;
    cerr << HERE << " minH = " << minH << " maxH = " << maxH << endl;
    cerr << HERE << " minD = " << minD << " maxD = " << maxD << endl;
    cerr << HERE << " minG = " << minG << " maxG = " << maxG << endl;
    cerr << HERE << " minPriAve = " << minPriAve << " maxPriAve = " << maxPriAve << endl;
    cerr << HERE << " minPosAve = " << minPosAve << " maxPosAve = " << maxPosAve << endl;
    cerr << HERE << " minPriSdv = " << minPriSdv << " maxPriSdv = " << maxPriSdv << endl;
    cerr << HERE << " minPosSdv = " << minPosSdv << " maxPosSdv = " << maxPosSdv << endl;
    cerr << HERE << " minRatioF = " << minRatioF << " maxRatioF = " << maxRatioF << endl;
    cerr << HERE << " minRatioM = " << minRatioM << " maxRatioM = " << maxRatioM << endl;
    cerr << HERE << " minRatioL = " << minRatioL << " maxRatioL = " << maxRatioL << endl;
    cerr << HERE << " minP = " << minP << " maxP = " << maxP << endl;
    cerr << HERE << " minQ = " << minQ << " maxQ = " << maxQ << endl;
    cerr << HERE << " minR = " << minR << " maxR = " << maxR << endl;

//------------------------------------------------------------------------------
//	Adjust the range of normalization
//------------------------------------------------------------------------------
    for ( unsigned int k = 0; k < sz; ++k ) {
#ifdef SKIP
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	double tX = ( _coordX[ k ] - minX )/( maxX - minX );
	_coordX[ k ] = ( 1.0 - tX )*(-1.0) + tX*(1.0);
	double tY = ( _coordY[ k ] - minY )/( maxY - minY );
	_coordY[ k ] = ( 1.0 - tY )*(-1.0) + tY*(1.0);
#else	// ENABLE_MAP_COORD_NORMALIZATION
	double tX = ( _coordX[ k ] - minX )/( maxX - minX );
	_coordX[ k ] = ( 1.0 - tX )*(0.0) + tX*(1.0);
	double tY = ( _coordY[ k ] - minY )/( maxY - minY );
	_coordY[ k ] = ( 1.0 - tY )*(0.0) + tY*(1.0);
#endif	// ENABLE_MAP_COORD_NORMALIZATION
#endif	// SKIP

	sample.push_back( D_Point2( _coordX[ k ], _coordY[ k ] ) );

	double tH = ( _attrH[ k ] - minH )/( maxH - minH );
	_attrH[ k ] = ( 1.0 - tH )*rngH.lower() + tH*rngH.upper();

	double tD = ( _attrD[ k ] - minD )/( maxD - minD );
	_attrD[ k ] = ( 1.0 - tD )*rngD.lower() + tD*rngD.upper();

	double tG = ( _attrG[ k ] - minG )/( maxG - minG );
	_attrG[ k ] = ( 1.0 - tG )*rngG.lower() + tG*rngG.upper();
	    
	double tPriAve = ( _attrPriAve[ k ] - minPriAve )/( maxPriAve - minPriAve );
	_attrPriAve[ k ] = ( 1.0 - tPriAve )*rngPriAve.lower() + tPriAve*rngPriAve.upper();
	
	double tPosAve = ( _attrPosAve[ k ] - minPosAve )/( maxPosAve - minPosAve );
	_attrPosAve[ k ] = ( 1.0 - tPosAve )*rngPosAve.lower() + tPosAve*rngPosAve.upper();

	double tPriSdv = ( _attrPriSdv[ k ] - minPriSdv )/( maxPriSdv - minPosSdv );
	_attrPriSdv[ k ] = ( 1.0 - tPriSdv )*rngPriSdv.lower() + tPriSdv*rngPriSdv.upper();

	double tPosSdv = ( _attrPosSdv[ k ] - minPosSdv )/( maxPosSdv - minPosSdv );
	_attrPosSdv[ k ] = ( 1.0 - tPosSdv )*rngPosSdv.lower() + tPosSdv*rngPosSdv.upper();

	double tRatioF = ( _attrRatioF[ k ] - minRatioF )/( maxRatioF - minRatioF );
	_attrRatioF[ k ] = ( 1.0 - tRatioF )*rngRatioF.lower() + tRatioF*rngRatioF.upper();

	double tRatioM = ( _attrRatioM[ k ] - minRatioM )/( maxRatioM - minRatioM );
	_attrRatioM[ k ] = ( 1.0 - tRatioM )*rngRatioM.lower() + tRatioM*rngRatioM.upper();

	double tRatioL = ( _attrRatioL[ k ] - minRatioL )/( maxRatioL - minRatioL );
	_attrRatioL[ k ] = ( 1.0 - tRatioL )*rngRatioL.lower() + tRatioL*rngRatioL.upper();

	double tP = ( _attrP[ k ] - minP )/( maxP - minP );
	_attrP[ k ] = ( 1.0 - tP )*rngP.lower() + tP*rngP.upper();

	double tQ = ( _attrQ[ k ] - minQ )/( maxQ - minQ );
	_attrQ[ k ] = ( 1.0 - tQ )*rngQ.lower() + tQ*rngQ.upper();

	double tR = ( _attrR[ k ] - minR )/( maxR - minR );
	_attrR[ k ] = ( 1.0 - tR )*rngR.lower() + tR*rngR.upper();
    }

#ifdef SKIP
	if ( _attrPriAve[ k ] < minPriAve ) minPriAve = _attrPriAve[ k ];
	if ( _attrPriAve[ k ] > maxPriAve ) maxPriAve = _attrPriAve[ k ];
	if ( _attrPosAve[ k ] < minPosAve ) minPosAve = _attrPosAve[ k ];
	if ( _attrPosAve[ k ] > maxPosAve ) maxPosAve = _attrPosAve[ k ];
	if ( _attrPriSdv[ k ] < minPriSdv ) minPriSdv = _attrPriSdv[ k ];
	if ( _attrPriSdv[ k ] > maxPriSdv ) maxPriSdv = _attrPriSdv[ k ];
	if ( _attrPosSdv[ k ] < minPosSdv ) minPosSdv = _attrPosSdv[ k ];
	if ( _attrPosSdv[ k ] > maxPosSdv ) maxPosSdv = _attrPosSdv[ k ];

	if ( _attrRatioF[ k ] < minRatioF ) minRatioF = _attrRatioF[ k ];
	if ( _attrRatioF[ k ] > maxRatioF ) maxRatioF = _attrRatioF[ k ];
	if ( _attrRatioM[ k ] < minRatioM ) minRatioM = _attrRatioM[ k ];
	if ( _attrRatioM[ k ] > maxRatioM ) maxRatioM = _attrRatioM[ k ];
	if ( _attrRatioL[ k ] < minRatioL ) minRatioL = _attrRatioL[ k ];
	if ( _attrRatioL[ k ] > maxRatioL ) maxRatioL = _attrRatioL[ k ];

	if ( _attrP[ k ] < minP ) minP = _attrP[ k ];
	if ( _attrP[ k ] > maxP ) maxP = _attrP[ k ];
	if ( _attrQ[ k ] < minQ ) minQ = _attrQ[ k ];
	if ( _attrQ[ k ] > maxQ ) maxQ = _attrQ[ k ];
	if ( _attrR[ k ] < minR ) minR = _attrR[ k ];
	if ( _attrR[ k ] > maxR ) maxR = _attrR[ k ];
#endif	// SKIP
    
    _rngH = rngH;
    _rngD = rngD;
    _rngG = rngG;
    _rngPriAve = rngPriAve;
    _rngPosAve = rngPosAve;
    _rngPriSdv = rngPriSdv;
    _rngPosSdv = rngPosSdv;
    _rngRatioF = rngRatioF;
    _rngRatioM = rngRatioM;
    _rngRatioL = rngRatioL;
    _rngP = rngP;
    _rngQ = rngQ;
    _rngR = rngR;
    
#ifdef SKIP
    if ( _rngX.lower() == _rngX.upper() ) _rngX.assign( minX, maxX );
    else {
	minX = _rngX.lower(); maxX = _rngX.upper();
    }
    if ( _rngY.lower() == _rngY.upper() ) _rngY.assign( minY, maxY );
    else {
	minY = _rngY.lower(); maxY = _rngY.upper();
    }

    cerr << HERE << " Given intevals for attribute values : " << endl;
    cerr << HERE << " interval H = " << rngH << endl;
    cerr << HERE << " interval D = " << rngD << endl;
    cerr << HERE << " interval G = " << rngG << endl;
    cerr << HERE << " interval priAve = " << rngPriAve << endl;
    cerr << HERE << " interval posAve = " << rngPosAve << endl;
    cerr << HERE << " interval priSdv = " << rngPriSdv << endl;
    cerr << HERE << " interval posSdv = " << rngPosSdv << endl;
    cerr << HERE << " interval ratioF = " << rngRatioF << endl;
    cerr << HERE << " interval ratioM = " << rngRatioM << endl;
    cerr << HERE << " interval ratioL = " << rngRatioL << endl;
    cerr << HERE << " interval P = " << rngP << endl;
    cerr << HERE << " interval Q = " << rngQ << endl;
    cerr << HERE << " interval R = " << rngR << endl;

#ifdef ENABLE_MAP_COORD_NORMALIZATION
    double sideX = maxX - minX;
    double sideY = maxY - minY;
#endif	// ENABLE_MAP_COORD_NORMALIZATION
    double rangeH = width( _rngH );
    double rangeD = width( _rngD );
    double rangeG = width( _rngG );

    double rangePriAve = width( _rngPriAve );
    double rangePosAve = width( _rngPosAve );
    double rangePriSdv = width( _rngPriSdv );
    double rangePosSdv = width( _rngPosSdv );
    double rangeRatioF = width( _rngRatioF );
    double rangeRatioM = width( _rngRatioM );
    double rangeRatioL = width( _rngRatioL );

    double rangeP = width( _rngP );
    double rangeQ = width( _rngQ );
    double rangeR = width( _rngR );
#endif	// SKIP

#ifdef SKIP
    for ( unsigned int k = 0; k < sz; ++k ) {
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	_coordX[ k ] = 2.0 * ( _coordX[ k ] - minX )/sideX - 1.0;
	_coordY[ k ] = 2.0 * ( _coordY[ k ] - minY )/sideY - 1.0;
	_attrH[ k ]  = 2.0 * ( _attrH[ k ] - _rngH.lower() )/rangeH - 1.0;
	_attrD[ k ]  = 2.0 * ( _attrD[ k ] - _rngD.lower() )/rangeD - 1.0;
	_attrG[ k ]  = 2.0 * ( _attrG[ k ] - _rngG.lower() )/rangeG - 1.0;
	_attrPriAve[ k ] =
	    2.0 * ( _attrPriAve[ k ] - _rngPriAve.lower() ) / rangePriAve - 1.0;
	_attrPosAve[ k ] =
	    2.0 * ( _attrPosAve[ k ] - _rngPosAve.lower() ) / rangePosAve - 1.0;
	_attrPriSdv[ k ] =
	    2.0 * ( _attrPriSdv[ k ] - _rngPriSdv.lower() ) / rangePriSdv - 1.0;
	_attrPosSdv[ k ] =
	    2.0 * ( _attrPosSdv[ k ] - _rngPosSdv.lower() ) / rangePosSdv - 1.0;
	_attrRatio [ k ] =
	    2.0 * ( _attrRatio [ k ] - _rngRatio.lower()  ) / rangeRatio  - 1.0;

	_attrP [ k ] =
	    2.0 * ( _attrP [ k ] - _rngP.lower()  ) / rangeP  - 1.0;
	_attrQ [ k ] =
	    2.0 * ( _attrQ [ k ] - _rngQ.lower()  ) / rangeQ  - 1.0;
	_attrR [ k ] =
	    2.0 * ( _attrR [ k ] - _rngR.lower()  ) / rangeR  - 1.0;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	_attrH[ k ]  = ( _attrH[ k ] - _rngH.lower() )/rangeH;
	_attrD[ k ]  = ( _attrD[ k ] - _rngD.lower() )/rangeD;
	_attrG[ k ]  = ( _attrG[ k ] - _rngG.lower() )/rangeG;
	_attrPriAve[ k ] = ( _attrPriAve[ k ] - _rngPriAve.lower() ) / rangePriAve;
	_attrPosAve[ k ] = ( _attrPosAve[ k ] - _rngPosAve.lower() ) / rangePosAve;
	_attrPriSdv[ k ] = ( _attrPriSdv[ k ] - _rngPriSdv.lower() ) / rangePriSdv;
	_attrPosSdv[ k ] = ( _attrPosSdv[ k ] - _rngPosSdv.lower() ) / rangePosSdv;
	// cerr << HERE << _attrRatio[ k ];
	_attrRatioF[ k ] = ( _attrRatioF[ k ] - _rngRatioF.lower() ) / rangeRatioF;
	_attrRatioM[ k ] = ( _attrRatioM[ k ] - _rngRatioM.lower() ) / rangeRatioM;
	_attrRatioL[ k ] = ( _attrRatioL[ k ] - _rngRatioL.lower() ) / rangeRatioL;
	// cerr << " => " << _attrRatio[ k ] << endl;
	_attrP[ k ] = ( _attrP[ k ] - _rngP.lower() ) / rangeP;
	_attrQ[ k ] = ( _attrQ[ k ] - _rngQ.lower() ) / rangeQ;
	_attrR[ k ] = ( _attrR[ k ] - _rngR.lower() ) / rangeR;
#endif	// ENABLE_MAP_COORD_NORMALIZATION

#ifdef SKIP
	if ( k % NUM_STAGES == 0 )
	    cerr << " push coord [ " << setw( 6 ) << k << " ] = "
		 << " ( " << _coordX[ k ] << " , " << _coordY[ k ] << " ) " << endl;
#endif	// SKIP

	sample.push_back( D_Point2( _coordX[ k ], _coordY[ k ] ) );
    }
#endif	// SKIP
}


//
//  Height::_retrieve --	retrieve a set of triangles as corner IDs
//
//  Inputs
//	none
//
//  Outputs
//	none
//
void Height::_retrieve( void )
{
    _triID.clear();
    for ( D_Finite_faces_iterator ffi = _T.finite_faces_begin();
	  ffi != _T.finite_faces_end(); ++ffi ) {
	std::vector< int > cornerID;
	std::vector< D_Point2 > coord;
	cornerID.clear();
	coord.clear();
#ifdef SKIP
	cerr << " Triangle : ";
#endif	// SKIP
	for ( unsigned int k = 0; k < 3; ++k ) {
	    D_Vertex_handle vh = ffi->vertex( k );
	    cornerID.push_back( vh->id() );
	    coord.push_back( vh->point() );
#ifdef SKIP
	    cerr << vh->id() << " ";
#endif	// SKIP
	}
#ifdef SKIP
	cerr << endl;
#endif	// SKIP
	
	// Check the triangle
	bool isBig = false;
	for ( unsigned int k = 0; k < 3; ++k ) {
	    D_Vector2 vec = coord[ (k+1)%3 ] - coord[ k ];
	    double len = sqrt( vec.squared_length() );
#ifdef SKIP
	    cerr << HERE << " len = " << len << " judge = ";
	    if ( len > LIMIT_EDGE_LENGTH ) cerr << " large" << endl;
	    else cerr << "small" << endl;
#endif	// SKIP
	    
	    if ( len > LIMIT_EDGE_LENGTH ) isBig = true;
	}
	if ( ! isBig ) _triID.push_back( cornerID );
    }

}

//------------------------------------------------------------------------------
//	Public Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	Constructors
//------------------------------------------------------------------------------

//
//  Height::Height --	default constructor
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Height::Height() 
{
    _T.clear();

    _nValues	= 0;
    _idP	= NO_INDEX;
    _idQ	= NO_INDEX;
    _idR	= NO_INDEX;
    
    _coordX.clear();
    _coordY.clear();

    _attrH.clear();
    _attrD.clear();
    _attrG.clear();

    _attrPriAve.clear();
    _attrPosAve.clear();
    _attrPriSdv.clear();
    _attrPosSdv.clear();
	
    _attrRatioF.clear();
    _attrRatioM.clear();
    _attrRatioL.clear();

    _attrValue.clear();
    _attrP.clear();
    _attrQ.clear();
    _attrR.clear();
    
    _triID.clear();
}


//
//  Height::Height --	copy constructor
//
//  Inputs
//	obj	: object of this class
//
//  Outputs
//	none
//
Height::Height( const Height & obj )
{
    _T			= obj._T;

    _coordX		= obj._coordX;
    _coordY		= obj._coordY;

    _attrH		= obj._attrH;
    _attrD		= obj._attrD;
    _attrG		= obj._attrG;
    
    _attrPriAve		= obj._attrPriAve;
    _attrPosAve		= obj._attrPosAve;
    _attrPriSdv		= obj._attrPriSdv;
    _attrPosSdv		= obj._attrPosSdv;

    _attrRatioF		= obj._attrRatioF;
    _attrRatioM		= obj._attrRatioM;
    _attrRatioL		= obj._attrRatioL;

    _attrValue		= obj._attrValue;

    _attrP		= obj._attrP;
    _attrQ		= obj._attrQ;
    _attrR		= obj._attrR;
    
    _rngX		= obj._rngX;
    _rngY		= obj._rngY;

    _rngH		= obj._rngH;
    _rngD		= obj._rngD;
    _rngG		= obj._rngG;

    _rngPriAve		= obj._rngPriAve;
    _rngPosAve		= obj._rngPosAve;
    _rngPriSdv		= obj._rngPriSdv;
    _rngPosSdv		= obj._rngPosSdv;

    _rngP		= obj._rngP;
    _rngQ		= obj._rngQ;
    _rngR		= obj._rngR;
    
    _triID		= obj._triID;
}


//------------------------------------------------------------------------------
//	Destructor
//------------------------------------------------------------------------------

//
//  Height::~Height --	destructor
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Height::~Height()
{
    _T.clear();

    _coordX.clear();
    _coordY.clear();
    _attrH.clear();
    _attrD.clear();
    _attrG.clear();

    _attrPriAve.clear();
    _attrPosAve.clear();
    _attrPriSdv.clear();
    _attrPosSdv.clear();

    _attrRatioF.clear();
    _attrRatioM.clear();
    _attrRatioL.clear();
    
    _attrValue.clear();
    _attrP.clear();
    _attrQ.clear();
    _attrR.clear();

    _triID.clear();
}


//------------------------------------------------------------------------------
//	Referrring to members
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Functions functions
//------------------------------------------------------------------------------
//
//  Height::autoTessellate --	tessellate the domain
//
//  Inputs
//	region : array of attribute values including geographic positions
//
//  Outputs
//	none
//
void Height::autoTessellate( const ublas::matrix< double > & region,
			     interval< double > & rngH,
			     interval< double > & rngD,
			     interval< double > & rngG,
			     interval< double > & rngPriAve,
			     interval< double > & rngPosAve,
			     interval< double > & rngPriSdv,
			     interval< double > & rngPosSdv,
			     interval< double > & rngRatioF,
			     interval< double > & rngRatioM,
			     interval< double > & rngRatioL,
			     interval< double > & rngP,
			     interval< double > & rngQ,
			     interval< double > & rngR )
{
    cerr << HERE << "##############################" << endl;
    cerr << HERE << "Height::autoTessellate" << endl;
    cerr << HERE << "##############################" << endl;
    // getchar();
    // cerr << "HERE A" << endl;
    _loadAttrs		( region );
    // cerr << "HERE B" << endl;
    std::vector< D_Point2 > sample;
    _autoNorm		( sample,
			  rngH, rngD, rngG,
			  rngPriAve, rngPosAve,
			  rngPriSdv, rngPosSdv,
			  rngRatioF, rngRatioM, rngRatioL,
			  rngP, rngQ, rngR );
    // cerr << "HERE C" << endl;
    
    _triangulate	( sample );
    // cerr << "HERE D" << endl;
    _retrieve		();
    // cerr << "HERE E" << endl;
}

//
//  Height::manualTessellate --	tessellate the domain
//
//  Inputs
//	region : array of attribute values including geographic positions
//
//  Outputs
//	none
//
void Height::manualTessellate( const ublas::matrix< double > & region,
			       interval< double > & rngH,
			       interval< double > & rngD,
			       interval< double > & rngG,
			       interval< double > & rngPriAve,
			       interval< double > & rngPosAve,
			       interval< double > & rngPriSdv,
			       interval< double > & rngPosSdv,
			       interval< double > & rngRatioF,
			       interval< double > & rngRatioM,
			       interval< double > & rngRatioL,
			       interval< double > & rngP,
			       interval< double > & rngQ,
			       interval< double > & rngR )
{
    cerr << HERE << "##############################" << endl;
    cerr << HERE << "Height::manualTessellate" << endl;
    getchar();
    cerr << HERE << "##############################" << endl;
    // cerr << "HERE A1" << endl;
    _loadAttrs		( region );
    std::vector< D_Point2 > sample;
    _manualNorm		( sample,
			  rngH, rngD, rngG,
			  rngPriAve, rngPosAve,
			  rngPriSdv, rngPosSdv,
			  rngRatioF, rngRatioM, rngRatioL,
			  rngP, rngQ, rngR );
    _triangulate	( sample );
    _retrieve		();
}



//------------------------------------------------------------------------------
//	Curvature staff
//------------------------------------------------------------------------------
double Height::HF( const double & x, const double & y )
{
    const int nNeighbors	= 4;
    
    // retrieve attribute values
    list< D_Vertex_handle > LV;
    D_Point2 p( x, y );
    LV.clear();
    CGAL::nearest_neighbors( _T, p, nNeighbors, std::back_inserter( LV ) );

    double vH = 0.0;
    for ( list< D_Vertex_handle >::iterator it = LV.begin(); it != LV.end(); it++ ) {
	unsigned int curID = (*it)->id(); 
	vH += _attrH[ curID ];
    }
    vH /= ( double )nNeighbors;

    return vH;
}

double Height::HFx( const double & x, const double & y )
{
    const double	dx = Dx;

    return ( HF(x+dx,y) - HF(x-dx,y) )/(2*dx);
}

double Height::HFy( const double & x, const double & y )
{
    const double	dy = Dy;

    return ( HF(x,y+dy) - HF(x,y-dy) )/(2*dy);
}

double Height::HFxx( const double & x, const double & y )
{
    const double	dx = Dx;

    return ( HFx(x+dx,y) - HFx(x-dx,y) )/(2*dx);
}

double Height::HFyy( const double & x, const double & y )
{
    const double	dy = Dy;

    return ( HFy(x,y+dy) - HFy(x,y-dy) )/(2*dy);
}

double Height::HFxy( const double & x, const double & y )
{
    const double	dy = Dy;

    return ( HFx(x,y+dy) - HFx(x,y-dy) )/(2*dy);
}

void Height::firstFund( const double & x, const double & y,
			double & E, double & F, double & G )
{
    double fx = HFx( x, y );
    double fy = HFy( x, y );
#ifdef DEBUG
    cerr << HERE << " x = " << x << " y = " << y << endl;
    cerr << HERE << " fx = " << fx << " fy = " << fy << endl;
#endif	// DEBUG
    E = 1.0 + fx*fx;
    F = fx*fy;
    G = 1.0 + fy*fy;
    return;
}

void Height::secondFund( const double & x, const double & y,
			 double & L, double & M, double & N )
{
    double fx = HFx( x, y );
    double fy = HFy( x, y );
    double fxx = HFxx( x, y );
    double fxy = HFxy( x, y );
    double fyy = HFyy( x, y );
#ifdef DEBUG
    cerr << HERE << " x = " << x << " y = " << y << endl;
    cerr << HERE << " fx = " << fx << " fy = " << fy << endl;
    cerr << HERE << " fxx = " << fxx << " fxy = " << fxy << " fyy = " << fyy << endl;
#endif	// DEBUG
    double denominator = sqrt( 1.0 + fx*fx + fy*fy );
    L = fxx/denominator;
    M = fxy/denominator;
    N = fyy/denominator;
    return;
}

void Height::curvature( const double & x, const double & y,
			double & K, double & H )
{
    double fx = HFx( x, y );
    double fy = HFy( x, y );
    double fxx = HFxx( x, y );
    double fxy = HFxy( x, y );
    double fyy = HFyy( x, y );
    double denominator = 1.0 + fx*fx + fy*fy;

    K = ( fxx*fyy - fxy*fxy ) / (denominator*denominator);
    H = ( (1.0+fy*fy)*fxx - 2.0*fx*fy*fxy + (1.0*fx*fx)*fyy )/
	2.0*sqrt(denominator*denominator*denominator);
}

void Height::principal( const double & x, const double & y,
			double & k1, double & k2 )
{
    double K, H;
    curvature( x, y, K, H );
    double R = H*H - K;
    double r = sqrt( R );
    k1 = H + r; 
    k2 = H - r; 
}

void Height::eigen( const double & x, const double & y,
		    double & k1, double & k2,
		    std::vector< double > & e1, std::vector< double > & e2 )
{
    double E, F, G;
    double L, M, N;
    firstFund	( x, y, E, F, G );
    secondFund	( x, y, L, M, N );
    Matrix2d I, II;
    I << E, F, F, G;
    II << L, M, M, N;
    Matrix2d Mat = I.inverse() * II;
    SelfAdjointEigenSolver<Matrix2d> eigensolver( Mat );
#ifdef DEBUG
    cerr << HERE << " E = " << E << " F = " << F << " G = " << G << endl;
    cerr << HERE << " L = " << L << " M = " << M << " N = " << N << endl;
    cerr << "The eigenvalues of M are:\n" << eigensolver.eigenvalues() << endl;
    cerr << "Here's a matrix whose columns are eigenvectors of Mat \n"
	 << "corresponding to these eigenvalues:\n"
	 << eigensolver.eigenvectors() << endl;
#endif	// DEBUG

    // eigenvectors are originally in ascending order
    k1 = eigensolver.eigenvalues()[ 1 ];
    k2 = eigensolver.eigenvalues()[ 0 ];
    e1.clear();
    e1.push_back( eigensolver.eigenvectors().col( 1 )[ 0 ] );
    e1.push_back( eigensolver.eigenvectors().col( 1 )[ 1 ] );
    e2.clear();
    e2.push_back( eigensolver.eigenvectors().col( 0 )[ 0 ] );
    e2.push_back( eigensolver.eigenvectors().col( 0 )[ 1 ] );
}


int Height::drainage( const double & x, const double & y )
{
    const double	dx = Dx;
    const double	dy = Dy;
    double			k1, k2;
    std::vector< double >	e1, e2;
    bool isRidge = false, isRavine = false;
    eigen( x, y, k1, k2, e1, e2 );

    if ( k1 > 0 ) {
	double			pv1, pv2;
	std::vector< double >	pe1, pe2;
	eigen( x+dx*e1[ 0 ], y+dy*e1[ 1 ],
	       pv1, pv2, pe1, pe2 );
	double			qv1, qv2;
	std::vector< double >	qe1, qe2;
	eigen( x-dx*e1[ 0 ], y-dy*e1[ 1 ],
	       qv1, qv2, qe1, qe2 );
#ifdef DEBUG
	cerr << HERE << "====> pv1 = " << pv1 << " k1 = " << k1 << " qv1 = " << qv1 << endl;
#endif	// DEBUG
	if ( ( pv1 < k1 ) && ( qv1 < k1 ) ) isRavine = true;
    }

    if ( k2 < 0 ) {
	double			pv1, pv2;
	std::vector< double >	pe1, pe2;
	eigen( x+dx*e2[ 0 ], y+dy*e2[ 1 ],
	       pv1, pv2, pe1, pe2 );
	double			qv1, qv2;
	std::vector< double >	qe1, qe2;
	eigen( x-dx*e2[ 0 ], y-dy*e2[ 1 ],
	       qv1, qv2, qe1, qe2 );
#ifdef DEBUG
	cerr << HERE << "====> pv2 = " << pv2 << " k2 = " << k2 << " qv2 = " << qv2 << endl;
#endif	// DEBUG
	if ( ( pv2 > k2 ) && ( qv2 > k2 ) ) isRidge = true;
    }

    if ( isRidge && isRavine ) return 3;
    else if ( isRavine ) return 2;
    else if ( isRidge ) return 1;
    else return 0;
}

int Height::drainage( const double & x, const double & y,
		      const double & k1, const double & k2,
		      const std::vector< double > & e1,
		      const std::vector< double > & e2,
		      double & ridgeDegree, double & ravineDegree )
{
    const double	dx = Dx;
    const double	dy = Dy;
    bool isRidge = false, isRavine = false;
    ridgeDegree = ravineDegree = -100.0;

    if ( k1 > 0 ) {
	double			pv1, pv2;
	std::vector< double >	pe1, pe2;
	eigen( x+dx*e1[ 0 ], y+dy*e1[ 1 ],
	       pv1, pv2, pe1, pe2 );
	double			qv1, qv2;
	std::vector< double >	qe1, qe2;
	eigen( x-dx*e1[ 0 ], y-dy*e1[ 1 ],
	       qv1, qv2, qe1, qe2 );
#ifdef DEBUG
	cerr << HERE << "====> pv1 = " << pv1 << " k1 = " << k1 << " qv1 = " << qv1 << endl;
#endif	// DEBUG
	if ( ( pv1 < k1 ) && ( qv1 < k1 ) ) {
	    isRavine = true;
	    ravineDegree = 0.5 * ( ( k1 - pv1 ) + ( k1 - qv1 ) );
	}
    }

    if ( k2 < 0 ) {
	double			pv1, pv2;
	std::vector< double >	pe1, pe2;
	eigen( x+dx*e2[ 0 ], y+dy*e2[ 1 ],
	       pv1, pv2, pe1, pe2 );
	double			qv1, qv2;
	std::vector< double >	qe1, qe2;
	eigen( x-dx*e2[ 0 ], y-dy*e2[ 1 ],
	       qv1, qv2, qe1, qe2 );
#ifdef DEBUG
	cerr << HERE << "====> pv2 = " << pv2 << " k2 = " << k2 << " qv2 = " << qv2 << endl;
#endif	// DEBUG
	if ( ( pv2 > k2 ) && ( qv2 > k2 ) ) {
	    isRidge = true;
	    ridgeDegree = 0.5 * ( ( pv2 - k2 ) + ( qv2 - k2 ) );
	}
    }

    // avoid both activating ridge and ravine bits
    if ( isRidge && isRavine ) {
	if ( ridgeDegree < ravineDegree ) {
	    isRidge = false;
	    ridgeDegree = -100.0;
	}
	else {
	    isRavine = false;
	    ravineDegree = -100.0;
	}
    }

    assert( !( isRidge && isRavine ) );
    
    if ( isRavine ) return 2;
    else if ( isRidge ) return 1;
    else return 0;
}

//------------------------------------------------------------------------------
//	Assignment opereators
//------------------------------------------------------------------------------

//
//  Height::operator = --	assignement
//
//  Inputs
//	obj	: objects of this class
//
//  Outputs
//	this object
//
Height & Height::operator = ( const Height & obj )
{
    if ( this != &obj ) {
	_T		= obj._T;

	_coordX		= obj._coordX;
	_coordY		= obj._coordY;

	_attrH		= obj._attrH;
	_attrD		= obj._attrD;
	_attrG		= obj._attrG;

	_attrPriAve	= obj._attrPriAve;
	_attrPosAve	= obj._attrPosAve;
	_attrPriSdv	= obj._attrPriSdv;
	_attrPosSdv	= obj._attrPosSdv;

	_attrRatioF	= obj._attrRatioF;
	_attrRatioM	= obj._attrRatioM;
	_attrRatioL	= obj._attrRatioL;

	_attrValue	= obj._attrValue;

	_attrP		= obj._attrP;
	_attrQ		= obj._attrQ;
	_attrR		= obj._attrR;
	
	_rngX		= obj._rngX;
	_rngY		= obj._rngY;

	_rngH		= obj._rngH;
	_rngD		= obj._rngD;
	_rngG		= obj._rngG;

	_rngPriAve	= obj._rngPriAve;
	_rngPosAve	= obj._rngPosAve;
	_rngPriSdv	= obj._rngPriSdv;
	_rngPosSdv	= obj._rngPosSdv;

	_rngP		= obj._rngP;
	_rngQ		= obj._rngQ;
	_rngR		= obj._rngR;
	
	_triID		= obj._triID;
    }
    return *this;
}


//------------------------------------------------------------------------------
//	I/O functions
//------------------------------------------------------------------------------




// end of header file
// Do not add any stuff under this line.
