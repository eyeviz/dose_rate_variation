//==============================================================================
// Graph.cpp
//	: program file for
//
//------------------------------------------------------------------------------
//
//				Date: Sun Feb  1 16:43:52 2015
//
//==============================================================================
//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------
#include "Common.h"
#include "Mesh.h"
#include "Graph.h"


//------------------------------------------------------------------------------
//	Fundamental Functions
//------------------------------------------------------------------------------
void initAttributes( Graph & g )
{
    BGL_FORALL_VERTICES( vd, g, Graph ) {
	put( vertex_myflag,	g, vd, false );
    }
}

void addVertex( Graph &g, int id, int val1, int val2 ){

    VertexDescriptor vd = add_vertex( g );
    put( vertex_myid,   g, vd, id );
    put( vertex_myarea, g, vd, 1.0);
    put( vertex_myslabcoord, g, vd, P_Point2( ( double )val1, ( double )val2 ) );
    P_Point2 randomCoord( ( double )val1 + 1.0*drand48()/3 - 0.2,
			  ( double )val2 + 1.0*drand48()/3 - 0.2 );
    put( vertex_mycoord,	g, vd, randomCoord );
}

void addEdge(Graph &g, int id, int vertexId1, int vertexId2 ){

    bool is_added;
    VertexDescriptor vd0 = vertex(vertexId1, g);
    VertexDescriptor vd2 = vertex(vertexId2, g);

    EdgeDescriptor ed0;
    tie( ed0, is_added ) = add_edge( vd0, vd2, g );
    put( edge_myid, g, ed0, id );

}
// end of header file
// Do not add any stuff under this line.
