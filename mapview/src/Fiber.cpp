//==============================================================================
// Fiber.cpp
//	: header file for
//
//------------------------------------------------------------------------------
//
//				Date: Mon Apr 25 16:40:43 2016
//
//==============================================================================

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include <cmath>
#include <climits>
#include <iostream>
#include <fstream>

using namespace std;

#include "Common.h"
#include "Mesh.h"
#include "Fiber.h"
#include "Graph.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	Functions for computing integral of geodesic distances
//------------------------------------------------------------------------------

//return true if no adjacency nodes.
bool isBirthOrDeath( VertexDescriptor & vd0,
		     vector< VertexDescriptor > & VdVector,
		     Graph & g )
{
    unsigned int adjVertex_count = 0;

    for(unsigned int i=0; i<VdVector.size(); ++i){
        VertexDescriptor vd1 = VdVector[i];
        bool isConnected;
        EdgeDescriptor ed;

        tie( ed,isConnected ) = edge( vd0, vd1, g );
        if(isConnected)     ++adjVertex_count;
    }
    if( adjVertex_count == 0 )   return true;
    else     return false;
}

//return true if more tha two adjacency nodes.
bool isMergeOrSplit(VertexDescriptor &vd0,
                    vector< VertexDescriptor > &VdVector,
                    Graph &g
                    ){
    unsigned int adjVertex_count = 0;
    for(unsigned int i=0; i<VdVector.size(); ++i){
        VertexDescriptor vd1 = VdVector[i];
        bool isConnected;
        EdgeDescriptor ed;

        tie( ed,isConnected ) = edge( vd0, vd1, g );
        if(isConnected)     ++adjVertex_count;
    }
    if( adjVertex_count > 1 )   return true;
    else     return false;
}



void fiberDetector( const unsigned int & sizeX, const unsigned int & sizeY,
                    vector< vector< VertexDescriptor > > & buffer_vertex,
                    Graph & g,
                    FiberMap & fiber,
		    Singularity & singular )
{
    fiber.clear();
    fiber.resize( sizeX*sizeY );
    singular.clear();
    singular.resize( sizeX*sizeY );

    assert( buffer_vertex.size() == sizeX*sizeY );

    for ( unsigned int i = 0; i < buffer_vertex.size(); ++i ) {	//All Slabs
        bool isBranch = false;
        bool isEnd = false;
        bool isNode = false;    //no nodes in slab?
	singular[ i ] = 0;
        for ( unsigned int j = 0; j < buffer_vertex[ i ].size(); ++j ) { //All Nodes
	    isNode = true;
	    // UP
	    if ( ( i / sizeX ) >= 1 ) {
		unsigned int upSlab = i - sizeX;
		bool curBra = isMergeOrSplit( buffer_vertex[ i ][ j ],
					      buffer_vertex[ upSlab ], g );
		bool curEnd = isBirthOrDeath( buffer_vertex[ i ][ j ],
					      buffer_vertex[ upSlab ], g );
		if ( curBra ) singular[ i ]++;
		if ( curEnd ) singular[ i ]++;
		isBranch = isBranch || curBra;
		isEnd    = isEnd    || curEnd;
	    }

	    // DOWN
	    if ( ( i / sizeX < sizeY - 1 ) ) {
		// if ( ( i+(int)size ) < (size*size) )
		unsigned int downSlab = i + sizeX;
		bool curBra = isMergeOrSplit( buffer_vertex[ i ][ j ],
					      buffer_vertex[ downSlab ], g );
		bool curEnd = isBirthOrDeath( buffer_vertex[ i ][ j ],
					      buffer_vertex[ downSlab ], g );
		if ( curBra ) singular[ i ]++;
		if ( curEnd ) singular[ i ]++;
		isBranch = isBranch || curBra;
		isEnd    = isEnd    || curEnd;
	    }

	    // LEFT
	    if ( ( i % sizeX ) >= 1 ) {
		unsigned int leftSlab = i - 1;
		bool curBra = isMergeOrSplit( buffer_vertex[ i ][ j ],
					      buffer_vertex[ leftSlab ], g );
		bool curEnd = isBirthOrDeath( buffer_vertex[ i ][ j ],
					      buffer_vertex[ leftSlab ], g );
		if ( curBra ) singular[ i ]++;
		if ( curEnd ) singular[ i ]++;
		isBranch = isBranch || curBra;
		isEnd    = isEnd    || curEnd;
	    }

	    // RIGHT
	    if ( ( i % sizeX ) < sizeX - 1 ) {
		unsigned int rightSlab = i + 1;
		bool curBra = isMergeOrSplit( buffer_vertex[ i ][ j ],
					      buffer_vertex[ rightSlab ], g );
		bool curEnd = isBirthOrDeath( buffer_vertex[ i ][ j ],
					      buffer_vertex[ rightSlab ], g );
		if ( curBra ) singular[ i ]++;
		if ( curEnd ) singular[ i ]++;
		isBranch = isBranch || isMergeOrSplit( buffer_vertex[ i ][ j ],
						       buffer_vertex[ rightSlab ], g );
		isEnd    = isEnd    || isBirthOrDeath( buffer_vertex[ i ][ j ],
						       buffer_vertex[ rightSlab ], g );
	    }
	}

	if ( ! isBranch && ! isEnd ) fiber[ i ] = no_singular;
	// if ( ! isBranch &&   isEnd ) fiber[ i ] = Merge_Split;
	if ( ! isBranch &&   isEnd ) fiber[ i ] = Birth_Death;
	// if(    isBranch && ! isEnd ) fiber[i] = Generate_Disappear;
	if (   isBranch && ! isEnd ) fiber[i] = Merge_Split;
	if (   isBranch &&   isEnd ) fiber[i] = Both;
	if ( ! isNode )              fiber[i] = no_data;
    }
}

#ifdef SKIP
//test function using simple example in Hamish's paper.
void testFiber( void ){
    Graph   testg;
    int counter=0;

    addVertex( testg, counter++, 0, 0 );
    addVertex( testg, counter++, 0, 1 );
    addVertex( testg, counter++, 1, 0 );
    addVertex( testg, counter++, 1, 1 );
    addVertex( testg, counter++, 1, 1 );
    addVertex( testg, counter++, 1, 2 );
    addVertex( testg, counter++, 1, 2 );
    addVertex( testg, counter++, 2, 1 );
    addVertex( testg, counter++, 2, 1 );
    addVertex( testg, counter++, 2, 2 );
    addVertex( testg, counter++, 2, 2 );
    addVertex( testg, counter++, 2, 3 );
    addVertex( testg, counter++, 3, 2 );

    counter = 0;
    addEdge( testg, counter, 0, 2 );
    addEdge( testg, counter, 0, 1 );
    addEdge( testg, counter, 2, 3 );
    addEdge( testg, counter, 1, 3 );
    addEdge( testg, counter, 3, 7 );
    addEdge( testg, counter, 3, 5 );
    addEdge( testg, counter, 7, 9 );
    addEdge( testg, counter, 5, 9 );
    addEdge( testg, counter, 4, 8 );
    addEdge( testg, counter, 4, 6 );
    addEdge( testg, counter, 9, 12 );
    addEdge( testg, counter, 9, 11 );
    addEdge( testg, counter, 8, 10 );
    addEdge( testg, counter, 6, 10 );
    addEdge( testg, counter, 10, 12 );
    addEdge( testg, counter, 10, 11 );

    vector< vector< VertexDescriptor > >		buffer_vertex;
    buffer_vertex.resize( 16 );

    VertexDescriptor vd = vertex(0,testg);
    buffer_vertex[0].push_back( vd );
    vd = vertex(1,testg);
    buffer_vertex[1].push_back( vd );

    vd = vertex(2,testg);
    buffer_vertex[4].push_back( vd );

    vd = vertex(3,testg);
    buffer_vertex[5].push_back( vd );

    vd = vertex(4,testg);
    buffer_vertex[5].push_back( vd );

    vd = vertex(5,testg);
    buffer_vertex[6].push_back( vd );

    vd = vertex(6,testg);
    buffer_vertex[6].push_back( vd );

    vd = vertex(7,testg);
    buffer_vertex[9].push_back( vd );

    vd = vertex(8,testg);
    buffer_vertex[9].push_back( vd );

    vd = vertex(9,testg);
    buffer_vertex[10].push_back( vd );

    vd = vertex(10,testg);
    buffer_vertex[10].push_back( vd );

    vd = vertex(11,testg);
    buffer_vertex[11].push_back( vd );

    vd = vertex(12,testg);
    buffer_vertex[14].push_back( vd );
    FiberMap          fiber;

    unsigned int size = 4;
    fiberDetector( size, size, buffer_vertex, testg, fiber );

    //array of display
    static GLfloat* buf = new GLfloat[ size * size * 3];
    //initialize
    for(unsigned int i=0; i< size * size * 3; ++i){
        buf[ i ] = 0.0f;
    }

    setFiberColor( fiber, buf );

}
#endif	// SKIP


//convert fiber array -> color
void setFiberColor( FiberMap Fiber, GLfloat* buf )
{
    for ( unsigned int i=0; i < Fiber.size(); ++i ) {
        GLubyte color[3];
        if ( Fiber[ i ] == no_data ) {    //black
            color[0] = (GLubyte)0;
            color[1] = (GLubyte)0;
            color[2] = (GLubyte)0;
        }
        if ( Fiber[ i ] == no_singular ) { //gray
            color[0] = (GLubyte)128;
            color[1] = (GLubyte)128;
            color[2] = (GLubyte)128;
        }
        if ( Fiber[ i ] == Merge_Split ) { //blue
            color[0] = (GLubyte)0;
            color[1] = (GLubyte)0;
            color[2] = (GLubyte)255;
        }
        if ( Fiber[ i ] == Birth_Death ) { //red
            color[0] = (GLubyte)255;
            color[1] = (GLubyte)0;
            color[2] = (GLubyte)0;
        }
        if( Fiber[ i ] == Both ){                   //green
            color[0] = (GLubyte)0;
            color[1] = (GLubyte)255;
            color[2] = (GLubyte)0;
        }


        buf[3* i   ] = (GLubyte)color[0]/255.0f;
        buf[3* i +1] = (GLubyte)color[1]/255.0f;
        buf[3* i +2] = (GLubyte)color[2]/255.0f;
    }
}
