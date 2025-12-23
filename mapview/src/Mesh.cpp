//==============================================================================
// Mesh.h
//	: program file for triangular meshes
//
//------------------------------------------------------------------------------
//
//				Date: Thu Sep 28 22:41:54 2017
//
//==============================================================================
#include <flann/flann.hpp>
#include <flann/io/hdf5.h>
using namespace flann;

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
using namespace std;

#include "Common.h"
#include "Mesh.h"
#include "LandPlot.h"
#include "Cloud.h"

struct Plane_equation {
    template < class Facet >
    typename Facet::Plane_3 operator()( Facet& f ) {
        typename Facet::Halfedge_handle h = f.halfedge();
        typedef typename Facet::Plane_3  Plane;
        return Plane( h->vertex()->point(),
                      h->next()->vertex()->point(),
                      h->next()->next()->vertex()->point());
    }
};

void scalePolyhedron( double s, Polyhedron & mesh  )
{
    P_Transformation3 scale( CGAL::SCALING, s );

    for ( P_Vertex_iterator vi = mesh.vertices_begin();
          vi != mesh.vertices_end(); ++vi ) {

        vi->point() = scale( vi->point() );
    }
}

void loadMesh( char * filename, Polyhedron & mesh )
{
    ifstream ifs( filename );
    if ( ! ifs ) {
        cerr << "Cannot open the file : " << filename << endl;
        exit( 0 );
    }

    ifs >> mesh;

    //scalling
//    scalePolyhedron( -3.0, mesh );

    // assign face IDs in ascending order
    unsigned int counterV = 0;
    for ( P_Vertex_iterator vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi ) {
	vi->id() = counterV++;
    // cerr << " VID = " << vi->id() << endl;
    // vi->setID( counterV++ );
    }

    // assign face IDs in ascending order
    unsigned int counterF = 0;
    for ( P_Facet_iterator fi = mesh.facets_begin(); fi != mesh.facets_end(); ++fi ) {
	fi->setID( counterF++ );
	P_Halfedge_handle hh = fi->halfedge();
	P_Point3 p = hh->prev()->vertex()->point();
	P_Point3 q = hh->vertex()->point();
	P_Point3 r = hh->next()->vertex()->point();
	fi->faceEq() = P_Plane3( p, q, r );
    }

    // assigned the largest value as the halfedge IDs first
    unsigned int nHalfedges = mesh.size_of_halfedges();
    for ( P_Halfedge_iterator hi = mesh.halfedges_begin(); hi != mesh.halfedges_end(); ++hi ) {
	hi->setID( nHalfedges );
    }
    // then assigne halfedge IDs consistenty in the sense that a halfedge and
    // its reversed halfedge has the same IDs.
    unsigned int counterH = 0;
    for ( P_Halfedge_iterator hi = mesh.halfedges_begin(); hi != mesh.halfedges_end(); ++hi ) {
	if ( hi->id() > counterH ) {
	    hi->setID( counterH );
	    hi->opposite()->setID( counterH );
	    counterH++;
	}
    }
    cerr << HERE << " Full number of halfedges = " << nHalfedges << endl;
    cerr << HERE << " Half number of halfedges = " << counterH << endl;

    mesh.normalize_border();

    // Compute normals of all the faces
    transform( mesh.facets_begin(), mesh.facets_end(), mesh.planes_begin(),
	       Plane_equation() );
#ifdef SKIP
    CGAL::set_pretty_mode( std::cout);
    std::copy( P.planes_begin(), P.planes_end(),
               std::ostream_iterator<Plane_3>( std::cout, "\n"));
#endif	// SKIP

    if ( mesh.size_of_border_edges() != 0 ) {
        cerr << "The input object has border edges!!" << endl;
        exit( 0 );
    }
}


//------------------------------------------------------------------------------
//	Triangulate surface
//------------------------------------------------------------------------------
// A modifier creating a triangle with the incremental builder.
template<class HDS>
class polyhedron_builder : public CGAL::Modifier_base< HDS > {
public:
    std::vector< double >		&coordX;
    std::vector< double >		&coordY;
    std::vector< double >		&coordZ;
    std::vector< std::vector< int > >	&tris;
    polyhedron_builder( std::vector<double> &_coordX, 
			std::vector<double> &_coordY, 
			std::vector<double> &_coordZ,
			std::vector< std::vector<int> > &_tris ) : 
	coordX(_coordX), coordY(_coordY), coordZ(_coordZ), tris(_tris) {}

    void operator()( HDS & hds ) {
	typedef typename HDS::Vertex	Vertex;
        typedef typename Vertex::Point	Point;
 
	// create a cgal incremental builder
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true );
        B.begin_surface( coordX.size(), tris.size() );
   
	// add the polyhedron vertices
	for( int i=0; i<(int)coordX.size(); i++ ){
	    B.add_vertex( Point( coordX[i], coordY[i], 0.5*coordZ[i] ) );
	}
	
	// add the polyhedron triangles
	for( int i=0; i<(int)tris.size(); i++ ){
	    B.begin_facet();
	    B.add_vertex_to_facet( tris[i][0] );
	    B.add_vertex_to_facet( tris[i][1] );
	    B.add_vertex_to_facet( tris[i][2] );
	    B.end_facet();
	}
	
	// finish up the surface
        B.end_surface();
    }
};


void tessellateDomain( Polyhedron & mesh, 
		       std::vector< double > & X, 
		       std::vector< double > & Y, 
		       std::vector< double > & H,
		       std::vector< std::vector< int > > & tris,
		       std::vector< P_Facet_handle > & ptrFacet )
{
    // build a polyhedron from the loaded arrays
    polyhedron_builder< P_HalfedgeDS > builder( X, Y, H, tris );
    mesh.delegate( builder );
    unsigned int idV = 0;
    for ( P_Vertex_iterator vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi ) {
        vi->id() = idV;
	idV++;
    }
    cerr << HERE << " Number of vertices in the domain = " << idV << endl;

    ptrFacet.clear();
    unsigned int idF = 0;
    for ( P_Facet_iterator fi = mesh.facets_begin(); fi != mesh.facets_end(); ++fi ) {
	fi->id() = idF;
	// if ( idF % 1000 == 0 )
	// cerr << HERE << " idF = " << setw( 8 ) << idF << endl;
	idF++;
	P_Halfedge_handle hh = fi->halfedge();
	P_Point3 p = hh->prev()->vertex()->point();
	P_Point3 q = hh->vertex()->point();
	P_Point3 r = hh->next()->vertex()->point();
	fi->faceEq() = P_Plane3( p, q, r );
	// P_Facet_handle fh = fi;
	// bary center of the face
	ptrFacet.push_back( ( P_Facet_handle )fi );
    }
    cerr << HERE << " Total number of faces = " << idF << endl;
}


void assignAttr( Polyhedron & mesh, 
		 const std::vector< double > & H,
		 const std::vector< double > & D,
		 // const std::vector< int > & land,
		 const std::vector< double > & G,
		 const std::vector< double > & priAve,
		 const std::vector< double > & posAve,
		 const std::vector< double > & priSdv,
		 const std::vector< double > & posSdv,
		 const std::vector< double > & ratioF,
		 const std::vector< double > & ratioM,
		 const std::vector< double > & ratioL,
		 const std::vector< std::vector< double > > & value,
		 const std::vector< double > & P,
		 const std::vector< double > & Q,
		 const std::vector< double > & R )
{
    unsigned int counter = 0;
    cerr << HERE << " reducation ratios : ";
    for ( P_Vertex_iterator vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi ) {
        vi->setH	( H[ counter ] );
	vi->setD	( D[ counter ] );
	// vi->setLand	( land[ counter ] );
	vi->setG	( G[ counter ] );
	vi->setPriAve	( priAve[ counter ] );
	vi->setPosAve	( posAve[ counter ] );
	vi->setPriSdv	( priSdv[ counter ] );
	vi->setPosSdv	( posSdv[ counter ] );
	vi->setRatioF	( ratioF[ counter ] );
	vi->setRatioM	( ratioM[ counter ] );
	vi->setRatioL	( ratioL[ counter ] );
	// cerr << fixed << setprecision( 6 ) << ratioF[ counter ] << " : ";
	// cerr << fixed << setprecision( 6 ) << ratioM[ counter ] << " : ";
	// cerr << fixed << setprecision( 6 ) << ratioL[ counter ] << " : ";
	vi->setValue	( value[ counter ] );
	vi->setP	( P[ counter ] );
	vi->setQ	( Q[ counter ] );
	vi->setR	( R[ counter ] );

	assert( vi->id() == counter );
	counter++;
    }
    cerr << endl;
}


#ifdef SKIP
void assignFunc( Polyhedron & mesh, 
		 const std::vector< double > & P,
		 const std::vector< double > & Q,
		 const std::vector< double > & R ) 
{
    unsigned int counter = 0;
    for ( P_Vertex_iterator vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi ) {
        vi->setP	( P[ counter ] );
	vi->setQ	( Q[ counter ] );
	vi->setR	( R[ counter ] );

	assert( vi->id() == counter );
	counter++;
    }
}
#endif	// SKIP


#ifdef OBSOLETE
void assignAngles( Polyhedron & mesh, 
		   const std::vector< double > & lambda,
		   const std::vector< double > & phi )
{
    unsigned int counter = 0;
    for ( P_Vertex_iterator vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi ) {
	vi->setLambda	( lambda[ counter ] );
	vi->setPhi	( phi   [ counter ] );
	assert( vi->id() == counter );
	counter++;
    }
}
#endif	// OBSOLETE

//------------------------------------------------------------------------------
//	Calculate distances from the specific position
//------------------------------------------------------------------------------

static double deg2rad( double deg )
{
    return ( deg * M_PI / 180.0 );
}

static double rad2deg( double rad )
{
    return ( rad * 180.0 / M_PI );
}

double calcDistHubeny( double lat1, double lon1,
		       double lat2, double lon2 )
{
    // https://www.trail-note.net/tech/calc_distance/
    // https://komoriss.com/calculate-distance-between-two-points-from-latitude-and-longitude/
#ifdef OLD
    double my = deg2rad( (lat1 + lat2) / 2.0);
    double dy = deg2rad( lat1 - lat2 );
    double dx = deg2rad( lng1 - lng2 );

    double sinMy = sin( my );
    double W = sqrt( 1.0 - e2 * sinMy * sinMy );
    double M = mnum / (W * W * W);
    double N = a / W;
	
    double dym = dy * M;
    double dxncos = dx * N * cos( my );

    // m -> km
    return ( 0.001 * sqrt( dym * dym + dxncos * dxncos ) );
#else	// OLD
    // phi => latituide (IDO)
    // lambda => longitude (KEIDO)
    // d = sqrt((M*Δφ)^2+(N*cos(φ)*Δλ)^2)

    // M = 6334834 / sqrt ((1-0.0066744*sin(φ)^2)^3)
    // N = 6377397 / sqrt (1-0.0066744*sin(φ)^2)
    // φ, Δφ, Δλ are in radian

    // = sqrt ((6334834 / sqrt ((1-0.006674*sin((B1+B3)/2*Pi()/180)^2)^3)*(B1-B3)*Pi()/180)^2 +( 6377397 / sqrt (1-0.006674*sin((B1+B3)/2*Pi()/180)^2)*cos((B1+B3)/2*Pi()/180)*(B2-B4)*Pi()/180)^2)

    const double POLE_RADIUS = 6356752;	   // # 極半径(短半径)
    const double EQUATOR_RADIUS = 6378137; // # 赤道半径(長半径)
    const double E = 0.081819191042815790; // # 離心率
    const double E2= 0.006694380022900788; // # 離心率の２乗
	    
    double phi1 = lat1*M_PI/180.0;
    double phi2 = lat2*M_PI/180.0;
    double lambda1 = lon1*M_PI/180.0;
    double lambda2 = lon2*M_PI/180.0;
#ifdef DEBUG
    cerr << HERE << " phi1 = " << phi1 << " lambda1 = " << lambda1 << endl;
    cerr << HERE << " phi2 = " << phi2 << " lambda2 = " << lambda2 << endl;
#endif	// DEBUG
    
    double m_phi = 0.5 * ( phi1 + phi2 );
    double d_phi = phi1 - phi2;
    double d_lambda = lambda1 - lambda2;
#ifdef DEBUG
    cerr << HERE << " m_phi = " << m_phi << endl;
    cerr << HERE << " d_phi = " << d_phi << endl;
    cerr << HERE << " d_lambda = " << d_lambda << endl;
#endif	// DEBUG

    double W = sqrt( 1.0 - E2 * SQUARE( sin( m_phi ) ) );
    double M = EQUATOR_RADIUS * ( 1 - E2 );
    double N = EQUATOR_RADIUS / W;
#ifdef DEBUG
    cerr << HERE << " M = " << M << endl;
    cerr << HERE << " W = " << W << endl;
    cerr << HERE << " N = " << N << endl;
#endif	// DEBUG

    double dist = sqrt( SQUARE( M * d_phi ) +
			SQUARE( N * d_lambda * cos( m_phi ) ) );
    return ( 0.001 * dist );
#endif	// OLD
}

#ifdef OBSOLETE
void calcDistances( Polyhedron & mesh,
		    const double & phi, const double & lambda )
{
    unsigned int counter = 0;
    for ( P_Vertex_iterator vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi ) {
	double dist = calcDistHubeny( vi->phi(), vi->lambda(),
				      phi, lambda );
	vi->setD	( dist );
	if ( vi->id() % 10000 == 0 )
	    cerr << HERE << " counter = " << counter
		 << " height = " << vi->H() << " dist = " << vi->D() << endl;
	assert( vi->id() == counter );
	counter++;
    }
    for ( P_Facet_iterator fi = mesh.facets_begin(); fi != mesh.facets_end(); ++fi ) {
	P_Halfedge_handle hh = fi->halfedge();
	P_Vertex_handle vhP = hh->prev()->vertex();
	P_Vertex_handle vhQ = hh->vertex();
	P_Vertex_handle vhR = hh->next()->vertex();
	//double minDist = std::min( vhP->D(), std::min( vhQ->D(), vhR->D() ) );
	double minDist = std::min( {vhP->D(), vhQ->D(), vhR->D() } );
	fi->setDist( minDist );
    }
}
#endif	// OBSOLETE
