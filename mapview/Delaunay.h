#ifndef __Delaunay_H_
#define __Delaunay_H_

//#include <cstdio>
//#include <cstdlib>
#include <cmath>
#include <iostream>
//#include <sstream>
#include <vector>
//#include <list>
//#include <cstring>
//#include <fstream>

#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
using namespace boost;
using namespace boost::numeric;
using namespace boost::numeric::ublas;

// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
// #include <CGAL/Triangulation_euclidean_traits_xy_3.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_id_2.h>

#include <CGAL/nearest_neighbor_delaunay_2.h>


//------------------------------------------------------------------------------
//	Macro definitions
//------------------------------------------------------------------------------

#ifndef DEFAULT_VERTEX_ID
#define	DEFAULT_VERTEX_ID	(99999)
#endif	// DEFAULT_VERTEX_ID

//using namespace boost::numeric;

template < class Gt, class Vb = CGAL::Triangulation_vertex_base_2< Gt > >
class My_vertex_base : public Vb
{
private:
    unsigned int	_id;
    double		_lambda; // latitude
    double		_phi;	 // lontitude
    
public:
    typedef typename Vb::Vertex_handle	Vertex_handle;
    // typedef typename Vb::Face_handle	Face_handle;
    typedef typename Vb::Point		Point;

    template< class TDS2 > 
    struct Rebind_TDS {
	typedef typename Vb::template Rebind_TDS< TDS2 >::Other		Vb2;
	typedef My_vertex_base< Gt, Vb2 >				Other;
    };

    My_vertex_base() : Vb() {
	_id	= DEFAULT_VERTEX_ID;
    }

    My_vertex_base( const Point & p )
	: Vb( p ) {}

    const unsigned int & id( void ) const	{ return _id; }
    unsigned int & id( void )			{ return _id; }
    void setID( unsigned int id )		{ _id = id; }

    const double & lambda( void ) const		{ return _lambda; }
    double & lambda( void )			{ return _lambda; }
    void setLambda( const double & __lambda )	{ _lambda = __lambda; }

    const double & phi( void ) const		{ return _phi; }
    double & phi( void )			{ return _phi; }
    void setPhi( const double & __phi )		{ _phi = __phi; }
};


template < class Gt, class Fb = CGAL::Triangulation_face_base_2< Gt > >
class My_face_base : public Fb
{
private:
    unsigned int	_id;

public:
    typedef typename Fb::Face_handle	Face_handle;
    // typedef typename Fb::Vertex_handle	Vertex_handle;
    // typedef typename Vb::Point		Point;

    //template< class TDS2 > 
    //struct Rebind_TDS {
    //typedef typename Fb::template Rebind_TDS< TDS2 >::Other		Fb2;
    //typedef My_face_base< Gt, Fb2 >					Other;
    //};

    My_face_base() : Fb()
    { }
    My_face_base( void* v0, void* v1, void* v2 ) : 
	// CGAL::Triangulation_face_base_2<Gt>(v0,v1,v2) {}
	Fb( v0, v1, v2 )
    { }
    My_face_base( void* v0, void* v1, void* v2, void* n0, void* n1, void* n2 ) : 
	Fb( v0, v1, v2, n0, n1, n2 )
	// CGAL::Triangulation_face_base_2<Gt>(v0,v1,v2,n0,n1,n2) 
    { }

    const unsigned int & id( void ) const	{ return _id; }
    unsigned int & id( void )			{ return _id; }
    void setID( unsigned int id )		{ _id = id; }
};

// Exact_predicates_exact_constructions_kernel
// 幾何学的判定と作図を正確に行う
// Exact_predicates_exact_constructions_kernel_with_sqrt
// 幾何学的判定と作図を正確に行う、さらに、平方根の計算が正確
// Exact_predicates_inexact_constructions_kernel
// 幾何学的判定は正確だが、作図は丸め誤差により不正確かもしれない。しかし、大体のCGALアルゴリズムには十分であり、上の２つよりも計算が早い。
typedef CGAL::Exact_predicates_inexact_constructions_kernel	DTK;
//typedef CGAL::Triangulation_vertex_base_2< DTK >		MyVb;
typedef My_vertex_base< DTK >					MyVb;
typedef My_face_base< DTK >					MyFb;
typedef CGAL::Triangulation_data_structure_2< MyVb, MyFb >	Tds;
typedef CGAL::Delaunay_triangulation_2< DTK, Tds >		Delaunay;

typedef Delaunay::Vertex_handle					D_Vertex_handle;
typedef Delaunay::Face_handle					D_Face_handle;
typedef Delaunay::Finite_vertices_iterator			D_Finite_vertices_iterator;
typedef Delaunay::Finite_faces_iterator				D_Finite_faces_iterator;
typedef Delaunay::Point						D_Point;
typedef DTK::Vector_2						D_Vector2;
typedef DTK::Point_2						D_Point2;

typedef DTK::Triangle_2						D_Triangle2;
typedef DTK::Triangle_3						D_Triangle3;
typedef DTK::Line_2						D_Line2;
typedef DTK::Line_3						D_Line3;
typedef DTK::Segment_2						D_Segment2;
typedef DTK::Segment_3						D_Segment3;
typedef CGAL::Bbox_2						D_Bbox2;
//typedef CGAL::Polygon_2< DTK >				Polygon2;
//typedef CGAL::Aff_transformation_2< DTK >			Transformation2;
//typedef CGAL::Aff_transformation_3< DTK >			Transformation3;



#endif  // __Delaunay_H_
