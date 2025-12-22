#ifndef _Graph_H
#define _Graph_H


#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/iteration_macros.hpp>

using namespace boost;

#include "Common.h"
#include "Mesh.h"

//prototype
void graph_vertex_array( std::vector< std::vector < unsigned int > > & buffer_face,
			 std::vector< std::vector < unsigned int > > & buffer_edge ,
			 std::vector< pair< int, int > > adjacencytriangleIDIndex );
void JCNConstruction();


//------------------------------------------------------------------------------
//  Boost customization
//------------------------------------------------------------------------------
// Vertex attributes
// ID
enum vertex_myid_t		{ vertex_myid };
// Positions
enum vertex_mycoord_t		{ vertex_mycoord };
// Slab Coord
enum vertex_myslabcoord_t		{ vertex_myslabcoord };
// Label
enum vertex_mylabel_t		{ vertex_mylabel };
// Bounding box
enum vertex_mybbox_t		{ vertex_mybbox };
// Bounding box
enum vertex_myarea_t		{ vertex_myarea };
// Depth
enum vertex_mydepth_t		{ vertex_mydepth };
// Facets for JCN constuction
enum vertex_myfacets_t		{ vertex_myfacets };
// Flag for matching
enum vertex_myflag_t		{ vertex_myflag };

// Facet of Parent
enum vertex_myparent_t		{ vertex_myparent };
enum vertex_mychild_t		{ vertex_mychild };
// ID of Matching Partner
enum vertex_mypartner_t		{ vertex_mypartner };

// ID of Parent's Matching Partner
enum vertex_myparentpartner_t		{ vertex_myparentpartner };

// Pointer of Vertex of Contour Tree
enum vertex_myctvertex_t		{ vertex_myctvertex };

// Edge attributes
// ID
enum edge_myid_t		{ edge_myid };
// Shared facets
enum edge_mycommon_t		{ edge_mycommon };

namespace boost {
    // vertex properties
    BOOST_INSTALL_PROPERTY( vertex, myid );
    BOOST_INSTALL_PROPERTY( vertex, mycoord );
    BOOST_INSTALL_PROPERTY( vertex, myslabcoord );
    BOOST_INSTALL_PROPERTY( vertex, mylabel );
    BOOST_INSTALL_PROPERTY( vertex, mybbox );
    BOOST_INSTALL_PROPERTY( vertex, myarea );
    BOOST_INSTALL_PROPERTY( vertex, mydepth );
    BOOST_INSTALL_PROPERTY( vertex, myfacets );
    BOOST_INSTALL_PROPERTY( vertex, myparent );
    BOOST_INSTALL_PROPERTY( vertex, mychild );
    BOOST_INSTALL_PROPERTY( vertex, mypartner );
    BOOST_INSTALL_PROPERTY( vertex, myparentpartner );
    BOOST_INSTALL_PROPERTY( vertex, myctvertex );
    BOOST_INSTALL_PROPERTY( vertex, myflag );
    // edge properties
    BOOST_INSTALL_PROPERTY( edge, myid );
    BOOST_INSTALL_PROPERTY( edge, mycommon );
}


//------------------------------------------------------------------------------
//	Customizing vertex properties
//------------------------------------------------------------------------------
// Vertex ID 
typedef property< vertex_myid_t, unsigned int >			MyVID;
// Vertex position
typedef property< vertex_mycoord_t, P_Point2, MyVID >		MyCoord;
// Vertex slabCoord
typedef property< vertex_myslabcoord_t, P_Point2, MyCoord >	MySlabCoord;
// Vertex label
typedef property< vertex_mylabel_t, unsigned int, MySlabCoord >	MyLabel;
// Vertex bounding box
typedef property< vertex_mybbox_t, P_Bbox2, MyLabel >		MyBBox;
// Vertex area
typedef property< vertex_myarea_t, double, MyBBox >		MyArea;
// Vertex depth
typedef property< vertex_mydepth_t, double, MyArea >		MyDepth;
// Associated facets
typedef property< vertex_myfacets_t, VecFace, MyDepth >		MyFacets;

// Vertex Parent
typedef property< vertex_myparent_t, int, MyFacets >		MyParent;
// Vertex Child
typedef property< vertex_mychild_t, int, MyParent >		MyChild;
// Vertex Partner
typedef property< vertex_mypartner_t, int, MyChild >			MyPartner;
// Vertex Partner
typedef property< vertex_myparentpartner_t, int, MyPartner >			MyParentPartner;

// Vertex pointer To CT
typedef property< vertex_myctvertex_t, unsigned int, MyParentPartner >			MyCtVertex;

// Vertex flag
typedef property< vertex_myflag_t, bool, MyCtVertex >		MyVertexProperty;




//------------------------------------------------------------------------------
//	Customizing edge properties
//------------------------------------------------------------------------------
// Edge ID
typedef property< edge_myid_t, unsigned int >	                MyEID;
// faces shared by the two endpoints
typedef property< edge_mycommon_t, VecFace, MyEID >		MyEdgeProperty;


//------------------------------------------------------------------------------
//	Graph definition
//------------------------------------------------------------------------------
typedef adjacency_list< vecS, listS, undirectedS,
			MyVertexProperty, MyEdgeProperty >	Graph;

typedef graph_traits< Graph >					GraphTraits;
typedef graph_traits< Graph >::vertex_descriptor		VertexDescriptor;
typedef graph_traits< Graph >::edge_descriptor			EdgeDescriptor;

typedef graph_traits< Graph >::vertex_iterator			VertexIterator;
typedef graph_traits< Graph >::edge_iterator			EdgeIterator;

// Vertex property map
typedef property_map< Graph, vertex_myid_t >::type      	VertexIDMap;
typedef property_map< Graph, vertex_mycoord_t >::type       VertexCoordMap;
typedef property_map< Graph, vertex_myslabcoord_t >::type	VertexSlabCoordMap;
typedef property_map< Graph, vertex_mybbox_t >::type		VertexBboxMap;
typedef property_map< Graph, vertex_myarea_t >::type		VertexAreaMap;
typedef property_map< Graph, vertex_mydepth_t >::type		VertexDepthMap;
typedef property_map< Graph, vertex_myfacets_t >::type		VertexFacetsMap;
typedef property_map< Graph, vertex_myflag_t >::type		VertexFlagMap;

typedef property_map< Graph, vertex_myparent_t >::type		VertexParentMap;
typedef property_map< Graph, vertex_mychild_t >::type		VertexChildMap;
typedef property_map< Graph, vertex_mypartner_t >::type		VertexPartnerMap;
typedef property_map< Graph, vertex_myparentpartner_t >::type		VertexParentPartnerMap;
typedef property_map< Graph, vertex_myctvertex_t >::type		VertexCtMap;



//typedef property_map< Graph, vertex_index_t >::type		VertexIndexMap;

// Edge property map
typedef property_map< Graph, edge_myid_t >::type		EdgeIDMap;
typedef property_map< Graph, edge_mycommon_t >::type		EdgeCommonMap;
//typedef property_map< Graph, edge_index_t >::type		EdgeIndexMap;


//------------------------------------------------------------------------------
//	Ohter customized type definitions
//------------------------------------------------------------------------------
// set of vertex descriptors
typedef std::vector< VertexDescriptor >	VecVertex;


//------------------------------------------------------------------------------
//	Functions
//------------------------------------------------------------------------------
extern void initAttributes( Graph & g );
extern void addVertex( Graph &g, int id, int val1, int val2 );
extern void addEdge( Graph &g, int id, int vertexId1, int vertexId2 );

#endif  // _Graph_H

