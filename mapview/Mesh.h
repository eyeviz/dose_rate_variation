#ifndef __Mesh_H_
#define __Mesh_H_

// #define ENABLE_GRADIENT
// #define ENABLE_DISTANCE

#define ENABLE_USE_BIT

//#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Origin.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/enum.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/aff_transformation_tags.h>

#include <vector>

using namespace std;

#include <boost/format.hpp>
#include <boost/numeric/interval.hpp>
using namespace boost;
using namespace boost::numeric;

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
// #include <boost/numeric/ublas/io.hpp>
// using namespace boost::numeric::ublas;

#include "Vars.h"


//------------------------------------------------------------------------------
//	Macro definitions for meshes
//------------------------------------------------------------------------------

#define DEFAULT_LABEL		(0)

//------------------------------------------------------------------------------
//	Additional templates from CGAL
//------------------------------------------------------------------------------
class Cloud;

// Definition of My vertex
template < class Refs, class Traits, class Point >
class My_vertex : public CGAL::HalfedgeDS_vertex_max_base_with_id< Refs, Point, std::size_t > {
private:
    unsigned char		_label;
#ifdef OBSOLETE
    double			_lambda; // longitude
    double			_phi;	 // latitude
#endif	// OBSOLETE

    double			_P;
    double			_Q;
    double			_R;

    double			_H;
    double			_D;
    //int				_land;
    double			_G;
    
    double			_priAve;
    double			_posAve;
    double			_priSdv;
    double			_posSdv;
    
    double			_ratioF;
    double			_ratioM;
    double			_ratioL;

    std::vector< double >	_value;
    
    bool			_flag;
public:
    My_vertex() {
	_label		= DEFAULT_LABEL;
#ifdef OBSOLETE
	_lambda		= 0.0;
	_phi		= 0.0;
#endif	// OBSOLETE

	_P		= 0.0;
	_Q		= 0.0;
	_R		= 0.0;

	_H		= 0.0;
	_D		= 0.0;
	//_land		= 0;
	_G		= 0.0;

	_priAve		= 0.0;
	_posAve		= 0.0;
	_priSdv		= 0.0;
	_posSdv		= 0.0;

	_ratioF		= 0.0;
	_ratioM		= 0.0;
	_ratioL		= 0.0;

	_value.clear();
	
	_flag		= false;
    };
    My_vertex( const Point & p ) : CGAL::HalfedgeDS_vertex_max_base_with_id< Refs, Point, std::size_t >( p ) {
	_label		= DEFAULT_LABEL;
#ifdef OBSOLETE
	_lambda		= 0.0;
	_phi		= 0.0;
#endif	// OBSOLETE

	_P		= 0.0;
	_Q		= 0.0;
	_R		= 0.0;

	_H		= 0.0;
	_D		= 0.0;

	//_land		= 0;
	_G		= 0.0;

	_priAve		= 0.0;
	_posAve		= 0.0;
	_priSdv		= 0.0;
	_posSdv		= 0.0;

	_ratioF		= 0.0;
	_ratioM		= 0.0;
	_ratioL		= 0.0;

	_value.clear();
	
	_flag		= false;
    };
    My_vertex( const My_vertex & obj ) : CGAL::HalfedgeDS_vertex_max_base_with_id< Refs, Point, std::size_t >( obj ) {
	_label		= obj._label;
#ifdef OBSOLETE
	_lambda		= obj._lambda;
	_phi		= obj._phi;
#endif	// OBSOLETE

	_P		= obj._P;
	_Q		= obj._Q;
	_R		= obj._R;

	_H		= obj._H;
	_D		= obj._D;

	//_land		= obj._land;
	_G		= obj._G;

	_priAve		= obj._priAve;
	_posAve		= obj._posAve;
	_priSdv		= obj._priSdv;
	_posSdv		= obj._posSdv;

	_ratioF		= obj._ratioF;
	_ratioM		= obj._ratioM;
	_ratioL		= obj._ratioL;

	_value		= obj._value;
	
	_flag		= obj._flag;
    };

    const unsigned char & label( void ) const	{ return _label; }
    unsigned char & label( void )		{ return _label; } 
    void setLabel( const unsigned int & label ) { _label = label; }

#ifdef OBSOLETE
    const double & lambda( void ) const		{ return _lambda; }
    double & lambda( void )			{ return _lambda; }
    void setLambda( const double & __lambda ) 	{ _lambda = __lambda; }

    const double & phi( void ) const		{ return _phi; }
    double & phi( void )			{ return _phi; }
    void setPhi( const double & __phi ) 	{ _phi = __phi; }
#endif	// OBSOLETE

    const double & P( void ) const		{ return _P; }
    double & P( void )				{ return _P; }
    void setP( const double & __P )		{ _P = __P; }

    const double & Q( void ) const		{ return _Q; }
    double & Q( void )				{ return _Q; }
    void setQ( const double & __Q )		{ _Q = __Q; }

    const double & R( void ) const		{ return _R; }
    double & R( void )				{ return _R; }
    void setR( const double & __R )		{ _R = __R; }

    const double & H( void ) const		{ return _H; }
    double & H( void )				{ return _H; } 
    void setH( const double & __H )		{ _H = __H; }

    const double & D( void ) const		{ return _D; }
    double & D( void )				{ return _D; }
    void setD( const double & __D )		{ _D = __D; }
    
    //const int & land( void ) const		{ return _land; }
    //double & land( void )			{ return _land; }
    //void setLand( const int & __land )		{ _land = __land; }

    const double & G( void ) const		{ return _G; }
    double & G( void )				{ return _G; }
    void setG( const double & __G )		{ _G = __G; }

    const double & priAve( void ) const		{ return _priAve; }
    double & priAve( void )			{ return _priAve; } 
    void setPriAve( const double & __priAve )	{ _priAve = __priAve; }

    const double & posAve( void ) const		{ return _posAve; }
    double & posAve( void )			{ return _posAve; } 
    void setPosAve( const double & __posAve )	{ _posAve = __posAve; }

    const double & priSdv( void ) const		{ return _priSdv; }
    double & priSdv( void )			{ return _priSdv; } 
    void setPriSdv( const double & __priSdv )	{ _priSdv = __priSdv; }

    const double & posSdv( void ) const		{ return _posSdv; }
    double & posSdv( void )			{ return _posSdv; } 
    void setPosSdv( const double & __posSdv )	{ _posSdv = __posSdv; }

    const double & ratioF( void ) const		{ return _ratioF; }
    double & ratioF( void )			{ return _ratioF; } 
    void setRatioF( const double & __ratioF )	{ _ratioF = __ratioF; }

    const double & ratioM( void ) const		{ return _ratioM; }
    double & ratioM( void )			{ return _ratioM; } 
    void setRatioM( const double & __ratioM )	{ _ratioM = __ratioM; }

    const double & ratioL( void ) const		{ return _ratioL; }
    double & ratioL( void )			{ return _ratioL; } 
    void setRatioL( const double & __ratioL )	{ _ratioL = __ratioL; }

    const std::vector< double > & value( void ) const
						{ return _value; }
    std::vector< double > & value( void )	{ return _value; }
    void setValue( const std::vector< double > & __value )
						{ _value = __value; }
    
    const bool & flag( void ) const		{ return _flag; }
    bool & flag( void )				{ return _flag; }
    void setFlag( void )			{ _flag = true; }
    void clearFlag( void )			{ _flag = false; }

    My_vertex & operator = ( const My_vertex & obj ) {
	( CGAL::HalfedgeDS_vertex_max_base_with_id< Refs, Point, std::size_t > & )*this = obj;
	if ( this != &obj ) {
	    _label	= obj._label;
#ifdef OBSOLETE
	    _lambda	= obj._lambda;
	    _phi	= obj._phi;
#endif	// OBSOLETE

	    _P		= obj._P;
	    _Q		= obj._Q;
	    _R		= obj._R;

	    _H		= obj._H;
	    _D		= obj._D;

	    // _land	= obj._land;
	    _G		= obj._G;
	    
	    _priAve	= obj._priAve;
	    _posAve	= obj._posAve;
	    _priSdv	= obj._priSdv;
	    _posSdv	= obj._posSdv;
	    
	    _ratioF	= obj._ratioF;
	    _ratioM	= obj._ratioM;
	    _ratioL	= obj._ratioL;

	    _value	= obj._value;

	    _flag	= obj._flag;
	}
	return *this;
    }
};

// Definition of My half edge
template < class Refs, class Traits, class Point >
class My_halfedge : public CGAL::HalfedgeDS_halfedge_base< Refs > {
private:
    unsigned int	_id;		// edge IDs (a half edge and its
					// opposite share the same edge ID.)
    unsigned char	_label;		// for labels of the edge
					// *** If I remember correctly, ...
					// FIXED_EDGE:	 edge is fixed as a cut
					// edge (perfect matching edge)
					// INVALID_EDGE: edge that cannot be
					// changed into a perfect matching edge
    double		_weight;	// weight value (currently unused)

public:
    My_halfedge() : CGAL::HalfedgeDS_halfedge_base< Refs >() {
	_id		= 0;
	_label		= DEFAULT_LABEL;
	_weight		= 0.0;
    }
    My_halfedge( const My_halfedge & obj ) : CGAL::HalfedgeDS_halfedge_base< Refs >( obj ) {
	_id		= obj._id;
	_label		= obj._label;
	_weight		= obj._weight;
    }

    const unsigned int & id( void ) const	{ return _id; }
    unsigned int & id( void )			{ return _id; } 
    void setID( const unsigned int & id )	{ _id = id; }

    const unsigned char & label( void ) const	{ return _label; }
    unsigned char & label( void )		{ return _label; } 

    const double & weight( void ) const		{ return _weight; }
    double & weight( void )			{ return _weight; } 

    bool orient( void ) {
	return ( this->vertex()->id() < this->opposite()->vertex()->id() );
    }

    My_halfedge & operator = ( const My_halfedge & obj ) {
	( CGAL::HalfedgeDS_halfedge_base< Refs > & )*this = obj;
	if ( this != &obj ) {
	    _id		= obj._id;
	    _label	= obj._label;
	    _weight	= obj._weight;
	}
	return *this;
    }
};

// Definition of My face 
template < class Refs, class Traits, class Point, class Plane, class Triangle >
class My_face : public CGAL::HalfedgeDS_face_base< Refs, CGAL::Tag_true, Plane > {
private:
    unsigned int	_id;
    Plane		_faceEq;
    bool		_flag;
#ifdef ENABLE_USE_BIT
    bool		_use;
#endif	// ENABLE_USE_BIT
    bool		_ridge;
    bool		_ravine;
    int			_label;
    double		_dist;
public:
    My_face() : CGAL::HalfedgeDS_face_base< Refs, CGAL::Tag_true, Plane >() {
	_id		= 0;
	_flag		= false;
#ifdef ENABLE_USE_BIT
	_use		= false;
#endif	// ENABLE_USE_BIT
	_ridge		= false;
	_ravine		= false;
	_dist		= 0.0;
    }
    My_face( const My_face & obj ) : CGAL::HalfedgeDS_face_base< Refs, CGAL::Tag_true, Plane >( obj ) {
	_id		= obj._id;
	_faceEq		= obj._faceEq;
	_flag		= obj._flag;
#ifdef ENABLE_USE_BIT
	_use		= obj._use;
#endif	// ENABLE_USE_BIT
	_ridge		= obj._ridge;
	_ravine		= obj._ravine;
	_label		= obj._label;
	_dist		= obj._dist;
    }
    const unsigned int & id( void ) const	{ return _id; }
    unsigned int & id( void )			{ return _id; }
    void setID( const unsigned int & id )	{ _id = id; }

    const Plane & faceEq( void ) const		{ return _faceEq; }
    Plane & faceEq( void )			{ return _faceEq; }
    void setFaceEq( const Plane & faceEq )	{ _faceEq = faceEq; }
    
    const bool & flag( void ) const		{ return _flag; }
    bool & flag( void )				{ return _flag; }
    void setFlag( void )			{ _flag = true; }
    void clearFlag( void )			{ _flag = false; }

#ifdef ENABLE_USE_BIT
    const bool & use( void ) const		{ return _use; }
    bool & use( void )				{ return _use; }
    void setUse( void )				{ _use = true; }
    void clearUse( void )			{ _use = false; }
#endif	// ENABLE_USE_BIT

    const bool & ridge( void ) const		{ return _ridge; }
    bool & ridge( void )			{ return _ridge; }
    void setRidge( void )			{ _ridge = true; }
    void clearRidge( void )			{ _ridge = false; }

    const bool & ravine( void ) const		{ return _ravine; }
    bool & ravine( void )			{ return _ravine; }
    void setRavine( void )			{ _ravine = true; }
    void clearRavine( void )			{ _ravine = false; }

    const int & label( void ) const		{ return _label; }
    int & label( void )		 		{ return _label; }
    void setLabel( const int & __label )	{ _label = __label; }
    
    const double & dist( void ) const		{ return _dist; }
    double & dist( void )			{ return _dist; }
    void setDist( const double & __dist )	{ _dist = __dist; }

    My_face & operator = ( const My_face & obj ) {
	( CGAL::HalfedgeDS_face_base< Refs, CGAL::Tag_true, Plane > & )*this = obj;
	if ( this != &obj ) {
	    _id		= obj._id;
	    _faceEq	= obj._faceEq;
	    _flag	= obj._flag;
#ifdef ENABLE_USE_BIT
	    _use	= obj._use;
#endif	// ENABLE_USE_BIT
	    _ridge	= obj._ridge;
	    _ravine	= obj._ravine;
	    _label	= obj._label;
	    _dist	= obj._dist;
	}
	return *this;
    }
};


struct My_items : public CGAL::Polyhedron_items_with_id_3 {
    
    template < class Refs, class Traits >

    struct Vertex_wrapper {
        typedef typename Traits::Point_3 Point;
	typedef My_vertex< Refs, Traits, Point > Vertex;
    };

    template < class Refs, class Traits >
    struct Halfedge_wrapper {
        typedef typename Traits::Point_3 Point;
	typedef My_halfedge< Refs, Traits, Point > Halfedge;
    };

    template < class Refs, class Traits >
    struct Face_wrapper {
        typedef typename Traits::Point_3 Point;
	typedef typename Traits::Plane_3 Plane;
	typedef typename Traits::Triangle_2 Triangle;
	typedef My_face< Refs, Traits, Point, Plane, Triangle > Face;
    };

};

//------------------------------------------------------------------------------
//	CGAL type definitions
//------------------------------------------------------------------------------
// typedef CGAL::Exact_predicates_exact_constructions_kernel	P_Kernel;
typedef CGAL::Cartesian< double >			P_Kernel;
typedef P_Kernel::Vector_3				P_Vector3;
typedef P_Kernel::Point_3				P_Point3;
typedef P_Kernel::Plane_3				P_Plane3;
typedef CGAL::Polyhedron_3< P_Kernel, My_items >	Polyhedron;

typedef Polyhedron::Vertex				P_Vertex;
typedef Polyhedron::Vertex_handle			P_Vertex_handle;
typedef Polyhedron::Vertex_iterator			P_Vertex_iterator;

typedef Polyhedron::Halfedge_handle			P_Halfedge_handle;
typedef Polyhedron::Halfedge_iterator			P_Halfedge_iterator;
typedef Polyhedron::Halfedge_around_vertex_circulator	P_Halfedge_vertex_circulator;
typedef Polyhedron::Halfedge_around_facet_circulator	P_Halfedge_facet_circulator;

typedef Polyhedron::Facet				P_Facet;
typedef Polyhedron::Facet_handle			P_Facet_handle;
typedef Polyhedron::Facet_iterator			P_Facet_iterator;

typedef P_Kernel::Vector_2				P_Vector2;
typedef P_Kernel::Point_2				P_Point2;


typedef P_Kernel::Triangle_2				P_Triangle2;
typedef P_Kernel::Triangle_3				P_Triangle3;
typedef P_Kernel::Line_2				P_Line2;
typedef P_Kernel::Line_3				P_Line3;
typedef P_Kernel::Segment_2				P_Segment2;
typedef P_Kernel::Segment_3				P_Segment3;
typedef CGAL::Bbox_2					P_Bbox2;
typedef CGAL::Bbox_3					P_Bbox3;
typedef CGAL::Polygon_2< P_Kernel >			P_Polygon2;
typedef CGAL::Aff_transformation_2< P_Kernel >		P_Transformation2;
typedef CGAL::Aff_transformation_3< P_Kernel >		P_Transformation3;

typedef CGAL::Object					Object;
//typedef Kernel::Intersect_3				Intersect3;


typedef Polyhedron::HalfedgeDS				P_HalfedgeDS;


//------------------------------------------------------------------------------
//	Other customized type definitions
//------------------------------------------------------------------------------
// list of face handles
typedef std::vector< P_Facet_handle >			VecFace;
// list of halfedge handles
typedef std::vector< P_Halfedge_handle >		VecHalf;
// set of face handles
typedef std::vector< std::vector< P_Facet_handle > >	SetFace;


//------------------------------------------------------------------------------
//	Additional integer vectors for indexing
//------------------------------------------------------------------------------
// typedef CGAL::Cartesian< unsigned int >			IDT;
typedef CGAL::Cartesian< int >				IDT;
typedef IDT::Point_2					Tuple2;
typedef IDT::Point_3					Tuple3;


//------------------------------------------------------------------------------
//	Functions
//------------------------------------------------------------------------------
// multivariate functions
extern double	(*mapP)		( P_Vertex_handle v );
extern double	(*mapQ)		( P_Vertex_handle v );
extern double	(*mapR)		( P_Vertex_handle v );

extern void	loadMesh	( char * filename, Polyhedron & mesh );

extern void	tessellateDomain
				( Polyhedron & mesh, 
				  std::vector< double > & X, 
				  std::vector< double > & Y, 
				  std::vector< double > & H,
				  std::vector< std::vector< int > > & tris,
				  // Cloud & cloud,
				  std::vector< P_Facet_handle > & ptrFacet );
void assignAttr ( Polyhedron & mesh, 
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
		  const std::vector< double > & R );
#ifdef SKIP
void assignFunc	( Polyhedron & mesh, 
		  const std::vector< double > & P,
		  const std::vector< double > & Q,
		  const std::vector< double > & R );
#endif	// SKIP
#ifdef OBSOLETE
void assignAngles( Polyhedron & mesh, 
		   const std::vector< double > & lambda,
		   const std::vector< double > & phi );
#endif	// OBSOLETE
void calcDistances( Polyhedron & mesh,
		    const double & phi, const double & lambda );
double calcDistHubeny( double lat1, double lon1,
		       double lat2, double lon2 );
#endif	// __Mesh_H_
