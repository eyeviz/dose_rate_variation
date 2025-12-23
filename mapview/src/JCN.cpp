#include <iostream>
#include <iomanip>
#include <bitset>
#include <vector>
#include <algorithm>

#ifdef __LINUX__
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#endif	// __LINUX__

#ifdef __MAC__
#include <GLUT/glut.h>
#endif	// __MAC__

#include "Common.h"
#include "Mesh.h"
#include "Graph.h"
#include "JCN.h"

//------------------------------------------------------------------------------
//	Functions for program debugging
//------------------------------------------------------------------------------
void printVecFace( VecFace & setFace )
{
    for ( unsigned i = 0; i < setFace.size(); ++i ) {
	cerr << setFace[ i ]->id() << " , ";
    }
    cerr << endl;
}

void printFace( P_Facet_handle & fh )
{
    P_Halfedge_facet_circulator hfc = fh->facet_begin();
    do {
	double valueP = mapP( hfc->vertex() );
	double valueQ = mapQ( hfc->vertex() );
	cerr << "(" << valueP << "," << valueQ << ") :";
    } while ( ++hfc != fh->facet_begin() );
    cerr << endl;
}

//------------------------------------------------------------------------------
//	Functions for sorting
//------------------------------------------------------------------------------
bool sortFaces( P_Facet_handle fi, P_Facet_handle fj )
{
    return ( fi->id() < fj->id() );
}

bool sortEdges( P_Halfedge_handle hi, P_Halfedge_handle hj )
{
    return ( hi->id() < hj->id() );
}


//------------------------------------------------------------------------------
//	Functions for downsampling
//------------------------------------------------------------------------------

//#define AGGREGATION_DEBUG

void aggregate( unsigned int pixelW, unsigned int pixelH,
		unsigned int clusterW, unsigned int clusterH,
		vector< VecFace > & rasFace, vector< VecHalf > & rasEdge,
		vector< VecFace > & bufFace, vector< VecHalf > & bufEdge,
		unsigned int nFaces, vector< SetUInt > & pixelFace )
{
    // cerr << HERE << " rasFace.size() = " << rasFace.size() << endl;
    // cerr << HERE << " rasEdge.size() = " << rasEdge.size() << endl;
    // cerr << HERE << " pixelW = " << pixelW << " pixelH = " << pixelH << endl;
    assert( rasFace.size() == pixelW*pixelH );
    assert( rasEdge.size() == pixelW*pixelH );
    assert( rasFace.size() == rasEdge.size() );
    // unsigned int mH = RASTER_RESOLUTION / h;
    // unsigned int mW = RASTER_RESOLUTION / w;

    //cerr << HERE << " downsample in" << endl;
    //getchar();

#ifdef AGGREGATION_DEBUG
    cerr << HERE << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    cerr << HERE << " pixelH = " << pixelH << " pixelW = " << pixelW << endl;
    cerr << HERE << " clusterH = " << clusterH << " clusterW = " << clusterW << endl;
    getchar();
#endif	// AGGREGATION_DEBUG

    bufFace.clear();
    bufFace.resize( clusterW*clusterH );
    bufEdge.clear();
    bufEdge.resize( clusterW*clusterH );
    pixelFace.resize( nFaces );
    for ( unsigned int k = 0; k < nFaces; ++k ) pixelFace[ k ].clear();

    unsigned int intervalH = pixelH/clusterH;
    unsigned int intervalW = pixelW/clusterW;
    
    for ( unsigned int i = 0; i < pixelH; i++ ) {
	unsigned int mi = i / intervalH;
	for ( unsigned int j = 0; j < pixelW; j++ ) {
	    unsigned int mj = j / intervalW;

	    unsigned int pixelID = i * pixelW + j;

	    unsigned int clusterID = mi * clusterW + mj;
	    VecHalf & clusterEdge = bufEdge[ clusterID ];
	    VecFace & clusterFace = bufFace[ clusterID ];

#ifdef AGGREGATION_DEBUG
	    if ( ( mi == 4 ) && ( mj == 0 ) ) {
		cerr << HERE << "##############################" << endl;
		cerr << HERE << " i = " << i << "/" << pixelH << " j = " << j << "/" << pixelW << endl;
		cerr << HERE << " mi = " << mi << "/" << clusterH
		     << " mj = " << mj << "/" << clusterW << endl;
		cerr << HERE << " pixelID = " << pixelID << " clusterID = " << clusterID << endl;
		cerr << HERE << " bufSize = " << bufFace.size() << " rasSize = " << rasFace.size() << endl;
		cerr << HERE << " rasFace[ " << pixelID << " ].size = " << rasFace[ pixelID ].size() << endl;
	    }
#endif	// AGGREGATION_DEBUG

#ifdef SKIP
	    cerr << HERE << " i = " << i << " j = " << j
		 << " msize = " << rasFace[ pixelID ].size() << endl;
#endif	// SKIP

	    for ( unsigned int m = 0; m < rasFace[ pixelID ].size(); ++m ) {
		
		assert( rasFace[ pixelID ][ m ]->id() < pixelFace.size() );

		SetUInt & clusterPixel = pixelFace[ rasFace[ pixelID ][ m ]->id() ];

		// Do not do this!!
		// clusterPixel.clear();

		if ( find( clusterFace.begin(), clusterFace.end(), rasFace[ pixelID ][ m ] )
		     == clusterFace.end() ) {
#ifdef AGGREGATION_DEBUG
		    if ( rasFace[ pixelID ][ m ]->id() == 14123 ) {
			cerr << " inserting Face ID = " << rasFace[ pixelID ][ m ]->id() << endl;
			getchar();
		    }
#endif	// AGGREGATION_DEBUG
		    clusterFace.push_back( rasFace[ pixelID ][ m ] );
		    sort( clusterFace.begin(), clusterFace.end(), sortFaces );
		}

		// Caution: not "pixelID" but "clusterID"
		if ( find( clusterPixel.begin(), clusterPixel.end(), clusterID )
		     == clusterPixel.end() ) {
		    clusterPixel.push_back( clusterID );
		    sort( clusterPixel.begin(), clusterPixel.end() );
		}
	    }

#ifdef AGGREGATION_DEBUG
	    if ( ( mi == 4 ) && ( mj == 0 ) ) {
		cerr << HERE << "***** clusterFace.size = " << clusterFace.size() << endl;
		for ( unsigned int k = 0; k < clusterFace.size(); ++k ) {
		    cerr << clusterFace[ k ]->id() << " ";
		}
		cerr << "<====" << endl;
	    }
#endif	// AGGREGATION_DEBUG

	    for ( unsigned int m = 0; m < rasEdge[ pixelID ].size(); ++m ) {
		if ( find( clusterEdge.begin(), clusterEdge.end(), rasEdge[ pixelID ][ m ] )
		     == clusterEdge.end() ) {
#ifdef AGGREGATION_DEBUG
		    cerr << " inserting Edge ID = " << rasEdge[ pixelID ][ m ]->id() << endl;
		    getchar();
#endif	// AGGREGATION_DEBUG
		    clusterEdge.push_back( rasEdge[ pixelID ][ m ] );
		    sort( clusterEdge.begin(), clusterEdge.end(), sortEdges );
		}
	    }
	}
    }

#ifdef DEBUG
    for ( unsigned int k = 0; k < nFaces; ++k ) {
	cerr << " pixelFace[ " << k << " ]( " << pixelFace[ k ].size() << " ) = " << ends;
	for ( unsigned int i = 0; i < pixelFace[ k ].size(); ++i ) {
	    cerr << pixelFace[ k ][ i ] << " ";
	}
	cerr << endl;
    }
#endif	// DEBUG

#ifdef AGGREGATION_DEBUG
    unsigned int ids[41] = {14123, 14164, 15084, 15254, 15845, 15855, 15857, 15860, 15867, 15935, 16746, 16750, 16774, 16777, 16803, 25921, 26046, 26720, 26748, 26763, 26770, 26772, 26785, 26793, 26794, 26795, 26804, 26815, 26834, 26846, 26860, 26870, 26899, 26901, 26920, 27065, 27707, 27714, 27748, 27785, 27816 };
    for ( unsigned int kk = 0; kk < 41; ++kk ) {
	unsigned int id = ids[ kk ];
	cerr << " pixelFace[ " << id << " ].size = " << pixelFace[ id ].size() << " : " << ends;
	for ( unsigned int ii = 0; ii < pixelFace[ id ].size(); ++ii ) {
	    cerr << pixelFace[ id ][ ii ] << " ";
	}
	cerr << endl;
    }
#endif	// AGGREGATION_DEBUG

}


//------------------------------------------------------------------------------
//	Main Functions for composing JCNs
//------------------------------------------------------------------------------
void groupFaces( const unsigned int & slabW, const unsigned int & slabH,
		 vector< VecFace > & bufFace, vector< VecHalf > & bufEdge,
		 vector< SetFace > & setFace )
{
    assert( bufFace.size() == bufEdge.size() );
    unsigned int w = slabW, h = slabH;

    setFace.clear();
    setFace.resize( bufFace.size() );
    for ( unsigned int i = 0; i < h; i++ ) {
	for ( unsigned int j = 0; j < w; j++ ) {
	    unsigned int index = i * w + j;
	    VecHalf & pixelEdge = bufEdge[ index ];
	    VecFace & pixelFace = bufFace[ index ];
	    SetFace & groupFace = setFace[ index ];
	    groupFace.clear();

	    sort( pixelEdge.begin(), pixelEdge.end(), sortEdges );

	    // initialize the group
	    for ( unsigned int m = 0; m < pixelFace.size(); ++m ) {
		VecFace set;
		set.push_back( pixelFace[ m ] );
		groupFace.push_back( set );
	    }
	    
	    assert( groupFace.size() == pixelFace.size() );
	    for ( unsigned m = 0; m < groupFace.size(); ++m ) {
		sort( groupFace[ m ].begin(), groupFace[ m ].end(), sortFaces );
	    }

#ifdef MY_DEBUG
	    cerr << "##############################" << endl;
	    cerr << HERE << " i = " << i << " j = " << j << endl;
	    cerr << HERE << " current group : " << endl;
	    for ( unsigned m = 0; m < groupFace.size(); ++m ) {
		cerr << "[ " << m << " ] = ";
		printVecFace( groupFace[ m ] );
	    }
#endif	// MY_DEBUG

	    for ( unsigned int k = 0; k < pixelEdge.size(); ++k ) {
		P_Halfedge_handle hh = pixelEdge[ k ];
		P_Facet_handle fL = hh->facet();
		P_Facet_handle fR = hh->opposite()->facet();

#ifdef DEBUG
		cerr << HERE << " Half Edge = " << hh->id()
		     << " Left face = " << fL->id() 
		     << " right face = " << fR->id() << endl;
#endif	// DEBUG

		SetFace::iterator groupL = groupFace.end(), groupR = groupFace.end();
		for ( SetFace::iterator it = groupFace.begin(); it != groupFace.end(); ++it ) {
		    VecFace & each = (*it);
		    VecFace::iterator itL = find( each.begin(), each.end(), fL );
		    if ( itL != each.end() ) groupL = it;
		    VecFace::iterator itR = find( each.begin(), each.end(), fR );
		    if ( itR != each.end() ) groupR = it;
		}

#ifdef DEBUG
		cerr << HERE << " Left group = ";
		if ( groupL == groupFace.end() ) cerr << "NONE" << endl;
		else printVecFace( (*groupL) );
		cerr << HERE << " Right group = ";
		if ( groupR == groupFace.end() ) cerr << "NONE" << endl;
		else printVecFace( (*groupR) );
#endif	// DEBUG
		    
		if ( ( groupL != groupFace.end() ) && ( groupR != groupFace.end() ) ) {
		    if ( groupL == groupR ) {
			; // do nothing
		    }
		    else {
			SetFace::iterator groupF, groupS;
			if ( ( groupL - groupFace.begin() ) < ( groupR - groupFace.begin() ) ) {
			    groupF = groupL;
			    groupS = groupR;
			}
			else {
			    groupF = groupR;
			    groupS = groupL;
			}
			
			for ( unsigned int m = 0; m < groupS->size(); ++m )
			    groupF->push_back( (*groupS)[ m ] );
			
			groupFace.erase( groupS );
			sort( groupF->begin(), groupF->end(), sortFaces );
		    }
		}

#ifdef DEBUG
		cerr << HERE << " intermediate group : " << endl;
		for ( unsigned m = 0; m < groupFace.size(); ++m ) {
		    cerr << "[ " << m << " ] = ";
		    printVecFace( groupFace[ m ] );
		}
#endif	// DEBUG

	    }

#ifdef DEBUG
	    cerr << HERE << " finalized group : " << endl;
	    for ( unsigned m = 0; m < groupFace.size(); ++m ) {
		cerr << "[ " << m << " ] = ";
		printVecFace( groupFace[ m ] );
	    }	    
#endif	// DEBUG
        }
    }
}


void vertexToJCN( const unsigned int & slabW, const unsigned int & slabH,
		  const P_Bbox2 & box,
		  vector< SetFace > & setFace, 
		  vector< VecVertex > & setVertex,
		  Graph & g ) 
{
    // std::cerr<< "   s = " << s << " " <<__FILE__<<" " <<__LINE__<<std::endl;
    unsigned int counter = 0;
    unsigned int w = slabW, h = slabH;
    double minP, maxP, minQ, maxQ;
    double curT, curS;
    // double sumArea = 0.0;
    // VertexAreaMap	vertexArea	= get( vertex_myarea, g );

    setVertex.resize( h*w );
    curT = 0.00000;
    maxQ = ( 1.0 - curT ) * box.ymin() + curT * box.ymax();
    for ( unsigned int j = 0; j < h; j++ ) {
	minQ = maxQ;
	curT = ( double )( j + 1 )/( double )h;
	maxQ = ( 1.0 - curT ) * box.ymin() + curT * box.ymax();

	curS = 0.00000;
	maxP = ( 1.0 - curS ) * box.xmin() + curS * box.xmax();
	for ( unsigned int i = 0; i < w; i++ ) {

	    minP = maxP;
	    curS = ( double )( i + 1 )/( double )w;
	    maxP = ( 1.0 - curS ) * box.xmin() + curS * box.xmax();
	    
	    unsigned int index = j * w + i;
	    
	    SetFace &	groupFace = setFace[ index ];
	    VecVertex & groupVertex = setVertex[ index ];
	    
#ifdef SKIP
	    cerr << HERE << ">>>>>>>>>> i = " << i << " j = " << j
		 << " GFsize = " << groupFace.size() << endl;
#endif	// SKIP
	    
	    for ( unsigned int k = 0; k < groupFace.size(); ++k ) {
		// cerr << HERE << " k = " << k << endl;
		VertexDescriptor vd = add_vertex( g );
		
		put( vertex_myid,	g, vd, counter );
		put( vertex_myslabcoord,	g, vd, P_Point2( ( double )j, ( double )i ) );
		// Point2 randomCoord( ( double )j + 1.0*drand48()/2 - 0.2,
		// ( double )i + 1.0*drand48()/2 - 0.2 );
		P_Point2 coord;
		if ( slabW == 1 ) {
		    coord = P_Point2( ( double)slabH * ( ((double)k+0.5)/(double)groupFace.size() ),
				    ( double )j+drand48()/2.0-0.25 );
#ifdef DEBUG
		    cerr << HERE << "[W=1] k = " << k << " / " << groupFace.size() << " = "
			 << (( double )k+0.5)/( double )(groupFace.size()) << endl;
#endif	// DEBUG
		}
		else if ( slabH == 1 ) {
		    coord = P_Point2( ( double )i+drand48()/2.0-0.25, 
				    ( double )slabW * ( ((double)k+0.5)/(double)groupFace.size() ) );
#ifdef DEBUG
		    cerr << HERE << "[H=1] k = " << k << " / " << groupFace.size() << " = "
			 << (( double )k+0.5)/( double )(groupFace.size()) << endl;
#endif	// DEBUG
		}
		else {
		    coord = P_Point2( ( double )i+drand48()/2.0-0.25, ( double )j+drand48()/2.0-0.25 );
		}
		put( vertex_mycoord,	g, vd, coord );
		put( vertex_mylabel,	g, vd, REGULAR_SLAB );
                P_Bbox2 curBox = P_Bbox2( minP, minQ, maxP, maxQ );
		put( vertex_mybbox,	g, vd, curBox );
		put( vertex_mydepth,	g, vd, 0.0 );
		put( vertex_myfacets,	g, vd, groupFace[ k ] );
		// calculate the projected area
#ifdef SKIP
		double eachArea = 0.0;
		for ( unsigned int m = 0; m < groupFace[ k ].size(); ++m ) {
		    Facet_handle & curFace = groupFace[ k ][ m ];
		    double baseArea = projectedArea( curFace );
		    if ( baseArea > EPSILON ) {
			double cropArea = sharedArea( curFace, curBox );
			double origArea = originalArea( curFace );
			double curArea  = origArea * ( cropArea/baseArea );
			eachArea += curArea;
		    }
		}
		put( vertex_myarea,	g, vd, eachArea );
		sumArea += eachArea;
#endif	// SKIP
		counter++;

		groupVertex.push_back( vd );
	    }
	}
    }

    assert( counter == num_vertices( g ) );

#ifdef SKIP
    BGL_FORALL_VERTICES( vd, g, Graph ) {
	double realArea = vertexArea[ vd ];
	vertexArea[ vd ] = realArea/sumArea;
    }
#endif	// SKIP

#ifdef DEBUG
    cerr << HERE << "%%%%%%%%%% Overall area = " << sumArea << endl;
    cerr << HERE << " Total number of vertices = " << num_vertices( g ) << endl;
#endif	// DEBUG
}


bool connectVertices( Graph & g, 
		      VertexDescriptor & vA, VertexDescriptor & vB,
		      const unsigned int & id )
{
    VertexFacetsMap		vertexFacets	 = get( vertex_myfacets, g );

    VecFace & vecA = vertexFacets[ vA ];
    VecFace & vecB = vertexFacets[ vB ];
    VecFace sharedF( vecA.size() + vecB.size() );
#ifdef DEBUG
    cerr << " vecA : ";
    printVecFace( vecA );
    cerr << " vecB : ";
    printVecFace( vecB );
#endif	// DEBUG

    // If two vertices shared the faces, they should be connected.
    VecFace::iterator itF = set_intersection( vecA.begin(), vecA.end(),
					      vecB.begin(), vecB.end(),
					      sharedF.begin() );
    sharedF.resize( itF - sharedF.begin() );
#ifdef DEBUG
    cerr << " shared faces : ";
    printVecFace( shared );
#endif	// DEBUG

    EdgeDescriptor ed;
    bool isInserted;
    if ( sharedF.size() > 0 ) {
	tie( ed, isInserted ) = add_edge( vA, vB, g );
	if ( isInserted ) {
	    put( edge_myid,	g, ed, id );
	}
	return true;
    }
    return false;
}


void edgeToJCN( const unsigned int & slabW, const unsigned int & slabH,
		vector< VecVertex > & vecVertex, 
		Graph & g ) 
{
    unsigned int	counter = 0;
    unsigned int w = slabW, h = slabH;
    // VertexIDMap		vertexID	 = get( vertex_myid, g );


    // Along the first axis
    for ( unsigned int i = 0; i < h; i++ ) {
	for ( unsigned int j = 0; j < w - 1; j++ ) {
	    unsigned int indexL = i * w +   j;
	    unsigned int indexR = i * w + (j+1);
	    
	    VecVertex & vecL = vecVertex[ indexL ];
	    VecVertex & vecR = vecVertex[ indexR ];

	    for ( unsigned int p = 0; p < vecL.size(); ++p )
		for ( unsigned int q = 0; q < vecR.size(); ++q ) {
		    if ( connectVertices( g,
					  vecL[ p ], vecR[ q ], 
					  counter ) ) {
#ifdef DEBUG
			cerr << HERE << " connecting "
			     << vertexID[ vecL[ p ] ] << " = " 
			     << vertexID[ vecR[ q ] ] << endl;
#endif	// DEBUG
			counter++;
		    }
		}
	}
    }
    // Along the second axis
    for ( unsigned int j = 0; j < w; j++ ) {
	for ( unsigned int i = 0; i < h - 1; i++ ) {
	    unsigned int indexB =   i   * w +   j;
	    unsigned int indexT = (i+1) * w +   j;

	    VecVertex & vecB = vecVertex[ indexB ];
	    VecVertex & vecT = vecVertex[ indexT ];

	    for ( unsigned int p = 0; p < vecB.size(); ++p )
		for ( unsigned int q = 0; q < vecT.size(); ++q ) {
		    if ( connectVertices( g, vecB[ p ], vecT[ q ], 
					  counter ) ) {
#ifdef DEBUG
			cerr << HERE << " connecting "
			     << vertexID[ vecB[ p ] ] << " = " 
			     << vertexID[ vecT[ q ] ] << endl;
#endif	// DEBUG
			counter++;
		    }
		}
	}
    }
    
    assert( counter == num_edges( g ) );
#ifdef DEBUG
    cerr << HERE << " Total number of edges = " << num_edges( g ) << endl;
#endif	// DEBUG
}

void constructJCN( const unsigned int & slabW, const unsigned int & slabH,
		   const P_Bbox2 & box,
		   vector< SetFace > & setFace, 
		   vector< VecVertex > & vecVertex,
		   Graph & g )
{
    vecVertex.clear();
    g.clear();

    vertexToJCN	( slabW, slabH, box, setFace, vecVertex, g );
    edgeToJCN	( slabW, slabH, vecVertex, g );
}


#ifdef SKIP
void constructCT( const unsigned int & slabW, const unsigned int & slabH,
		  const Bbox2 & box,
		  vector< SetFace > & setFace, 
		  vector< VecVertex > & vecVertex,
		  Tree & g )
{
    vecVertex.clear();
    g.clear();

    vertexToCT	( slabW, slabH, box, setFace, vecVertex, g );
    edgeToCT	( slabW, slabH, vecVertex, g );
}
#endif	// SKIP
