//==============================================================================
// Bridson.cpp
//	: program file for Bridson Poisson Disk Sampline
//
//------------------------------------------------------------------------------
//
//				Time-stamp: "2024-02-02 13:13:33 shigeo"
//
//==============================================================================

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------
#include <random>
#include <algorithm>

using namespace std;

#include "Common.h"
#include "Bridson.h"

//------------------------------------------------------------------------------
//	Macro Switches
//------------------------------------------------------------------------------
// #define POISSON_DEBUG



//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
// https://code-database.com/knowledges/112
// vector<vector<string>> xy;
// vector<vector<string>> xy(3);
// vector<vector<string>> xy(3, vector<string>(3));
// vector<vector<string>> xy(3, vector<string>(3, "abc"));

Tuple2 getGridCoordinates( const interval< double > & rngX,
			   const interval< double > & rngY,
			   const int & gridW, const int & gridH,
			   P_Point2 coord )
{
    double nmX = ( coord.x() - rngX.lower() ) / width( rngX );
    double nmY = ( coord.y() - rngY.lower() ) / width( rngY );
    Tuple2 grid( std::max( 0, std::min( gridW-1, ( int )floor( gridW * nmX ) ) ),
		 std::max( 0, std::min( gridH-1, ( int )floor( gridH * nmY ) ) ) );
    return grid;
}


std::vector< P_Point2 > poissonDiskSampling( const interval< double > & rngX,
					     const interval< double > & rngY,
					     const ublas::matrix< double > & radius,
					     int k, RadiusType rType )
{
    // Implementation of the Poisson Disk Sampling algorithm.

    // radius: 2d array specifying the minimum sampling radius for each spatial
    // position in the sampling box. The size of the sampling box is given by
    // the size of the radius array.

    // k: Number of iterations to find a new particle in an annulus between
    // radius r and 2r from a sample particle.

    // rType: Method to determine the distance to newly spawned
    // particles. 'default' follows the algorithm of Bridson (2007) and
    // generates particles uniformly in the annulus between radius r and 2r.

    // 'normDist' instead creates new particles at distances drawn from a normal
    // distribution centered around 1.5r with a dispersion of 0.2r.

    // return
    // nParticle: Number of particles in the sampling.
    // particleCoordinates: 2d array containing the coordinates of the created particles.

    vector< P_Point2 > queueCoordinates;
    vector< P_Point2 > particleCoordinates;

    std::random_device seed_gen;
    std::default_random_engine engine( seed_gen() );
    std::uniform_real_distribution<> uniform( 0.0, 1.0 );
    std::normal_distribution<> normal( 1.5, 0.2 );
 
    // Set-up background grid
    if ( radius.size1() == 0 ) return particleCoordinates;

    int gridW = radius.size1();
    int gridH = radius.size2();
    int gridMax = std::max( gridW, gridH );
    cerr << HERE << " gridW = " << gridW << " gridH = " << gridH << endl;
    // ublas::matrix< int > grid( gridH, gridW );
    ublas::matrix< int > grid = ublas::zero_matrix< int >( gridH, gridW );

    
    // Pick initial (active) point
    double minX = rngX.lower();
    double minY = rngY.lower();
    double widthX = width( rngX );
    double widthY = width( rngY );
    double widthMin = std::min( widthX, widthY );
    P_Point2 coord( uniform( engine ) * widthX + minX,
		    uniform( engine ) * widthY + minY );
    Tuple2 idx = getGridCoordinates( rngX, rngY, gridW, gridH, coord );
    int nParticle = 1;

#ifdef SKIP
    cerr << " idx = ( " << idx.x() << " , " << idx.y() << " ) " << endl;
#endif	// SKIP

    grid( idx.x(), idx.y() ) = nParticle;
    
    // Initialise active queue

    // Appending to list is much quicker than to numpy array, if you do it very often
    queueCoordinates.push_back( coord );
    // List containing the exact positions of the final particles
    particleCoordinates.push_back( coord );

    // Continue iteration while there is still points in active list
    while ( queueCoordinates.size() > 0 ) {
	// cerr << "[I] Number of queue coordinates = " << queueCoordinates.size() << endl;

	// Pick random element in active queue
	int idxNext = std::min( ( int )queueCoordinates.size() - 1,
				( int )floor( uniform( engine ) * queueCoordinates.size() ) );
#ifdef POISSON_DEBUG
	cerr << " idxNext = " << idxNext << endl;
#endif	// POISSON_DEBUG

	P_Point2 activeCoord = queueCoordinates[ idxNext ];
	Tuple2 activeGridCoord = getGridCoordinates( rngX, rngY, gridW, gridH,
						     activeCoord );

#ifdef POISSON_DEBUG
	cerr << " activeCoord = ( "
	     << activeCoord[ 0 ] << " , " << activeCoord[ 1 ] << " ) " << endl;
	cerr << " activeGridCoord = ( "
	     << activeGridCoord[ 0 ] << " , " << activeGridCoord[ 1 ] << " ) " << endl;
#endif	// POISSON_DEBUG
	
	bool success = false;
	for ( int i = 0; i < k; ++i ) {

#ifdef POISSON_DEBUG
	    cerr << "############################################################" << endl;
	    cerr << " i = " << i << " / " << k << endl;
	    // getchar();
#endif	// POISSON_DEBUG
	    
	    double newRadius;
            if ( rType == Default ) {
		// Pick radius for new sample particle ranging between 1 and 2
		// times the local radius 
                newRadius =
		    radius( activeGridCoord.x(), activeGridCoord.y() ) *
		    ( uniform( engine ) + 1.0 );
	    }
            else if ( rType == normDist ) {
		// Pick radius for new sample particle from a normal
		// distribution around 1.5 times the local radius
		std::normal_distribution<> distrib( 1.5, 0.2 );
		newRadius =
		    radius( activeGridCoord.x(), activeGridCoord.y() ) *
		    normal( engine );
	    }

#ifdef POISSON_DEBUG
	    cerr << " newRadius = " << newRadius << endl;
#endif	// POISSON_DEBUG
	    
	    // Pick the angle to the sample particle and determine its
	    // coordinates
            double angle = 2.0 * M_PI * uniform( engine );
	    P_Point2 newCoord( activeCoord.x() + newRadius * cos( angle ),
			       activeCoord.y() + newRadius * sin( angle ) );

#ifdef POISSON_DEBUG
	    cerr << " newCoord = ( "
		 << newCoord[ 0 ] << " , " << newCoord[ 1 ] << " ) " << endl;
#endif	// POISSON_DEBUG
	    
	    // Prevent that the new particle is outside of the grid
	    if ( ! ( in( newCoord.x(), rngX ) ) && ( in( newCoord.y(), rngY ) ) ) {
#ifdef POISSON_DEBUG
		cerr << " New sample out of domain" << endl;
#endif	// POISSON_DEBUG
		continue;
	    }

	    // Check that particle is not too close to other particle
            Tuple2 newGridCoord =
		getGridCoordinates( rngX, rngY, gridW, gridH, newCoord );
#ifdef POISSON_DEBUG
	    cerr << " newGridCoord = ( "
		 << newGridCoord[ 0 ] << " , " << newGridCoord[ 1 ] << " ) " << endl;
#endif	// POISSON_DEBUG

	    double curRadius = radius( newGridCoord.x(), newGridCoord.y() );
#ifdef POISSON_DEBUG
	    cerr << " curRadius = " << curRadius << " gridMax = " << gridMax
		 << " widthMin = " << widthMin << endl;
#endif	// POISSON_DEBUG
            int radiusThere =
		( int )ceil( ( double )gridMax * curRadius / widthMin );

#ifdef POISSON_DEBUG
	    cerr << " radiusThere = " << radiusThere << endl;
#endif	// POISSON_DEBUG

	    Tuple2 gridRangeX( std::max( newGridCoord.x() - radiusThere, 0 ),
			       std::min( newGridCoord.x() + radiusThere + 1, gridW ) );
	    Tuple2 gridRangeY( std::max( newGridCoord.y() - radiusThere, 0 ),
			       std::min( newGridCoord.y() + radiusThere + 1, gridH ) );

#ifdef POISSON_DEBUG
	    cerr << " gridRangeX = " << gridRangeX[ 0 ] << " -- " << gridRangeX[ 1 ] << endl;
	    cerr << " gridRangeY = " << gridRangeY[ 0 ] << " -- " << gridRangeY[ 1 ] << endl;
#endif	// POISSON_DEBUG

	    bool conflict = false;
	    for ( int i = gridRangeX[ 0 ]; i < gridRangeX[ 1 ]; ++i ) {
		for ( int j = gridRangeY[ 0 ]; j < gridRangeY[ 1 ]; ++j ) {
		    if ( grid( i, j ) > 0 ) {
#ifdef POISSON_DEBUG
			cerr << HERE << " Conflict at grid ( "
			     << setw( 3 ) << i << " , " << setw( 3 ) << j
			     << " ) => ID " << setw( 5 ) << grid( i, j ) << endl;
#endif	// POISSON_DEBUG
			conflict = true;
		    }
		    if ( conflict ) break;
		}
		if ( conflict ) break;
	    }
		    
            if ( ! conflict ) {
		// No conflicts detected. Create a new particle at this position!
		queueCoordinates.push_back( newCoord );
		particleCoordinates.push_back( newCoord );
                nParticle++;
		grid( newGridCoord.x(), newGridCoord.y() ) = nParticle;
                success = true;
		if ( nParticle % 10000 == 0 ) {
		    cerr << HERE << " number of sampling points = " << setw( 8 )
			 << nParticle << endl;
		}
#ifdef POISSON_DEBUG
		cerr << " New sample passed" << endl;
#endif	// POISSON_DEBUG
	    }
            else {
		// There is a conflict. Do NOT create a new particle at this position!
#ifdef POISSON_DEBUG
		cerr << " New sample declined" << endl;
#endif	// POISSON_DEBUG
                continue;
	    }
	}
	
	if ( ! success ) {
	    // No new particle could be associated to the currently active particle.
	    // Remove current particle from the active queue!
            queueCoordinates.erase( queueCoordinates.begin() + idxNext );
	}

	// cerr << "[F] Number of queue coordinates = " << queueCoordinates.size() << endl;
    }

    return particleCoordinates;
}
// end of header file
// Do not add any stuff under this line.
