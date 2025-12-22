#ifndef __JCN_H_
#define __JCN_H_

#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/intersections.h>



//------------------------------------------------------------------------------
//	Macro definitions
//------------------------------------------------------------------------------
#define REGULAR_SLAB		(0)



//------------------------------------------------------------------------------
//	Function declaration
//------------------------------------------------------------------------------
extern void aggregate		( unsigned int wholeW, unsigned int wholeH,
				  unsigned int blockW, unsigned int blockH,
				  std::vector< VecFace > & rasFace,
				  std::vector< VecHalf > & rasEdge,
				  std::vector< VecFace > & bufFace,
				  std::vector< VecHalf > & bufEdge,
				  unsigned int nFaces,
				  std::vector< SetUInt > & pixelFace );

extern void groupFaces		( const unsigned int & slabW, const unsigned int & slabH,
				  std::vector< VecFace > & bufFace,
				  std::vector< VecHalf > & bufEdge,
				  std::vector< SetFace > & setFace );
    

extern void constructJCN	( const unsigned int & slabW, const unsigned int & slabH,
				  const P_Bbox2 & box,
				  std::vector< SetFace > & setFace, 
				  std::vector< VecVertex > & vecVertex,
				  Graph & g );

#endif	// __JCN_H_
