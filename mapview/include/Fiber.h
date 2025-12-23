//==============================================================================
// Fiber.h
//	: header file for
//
//------------------------------------------------------------------------------
//
//				Date: Mon Apr 25 16:34:48 2016
//
//==============================================================================

#ifndef _Fiber_H		// begining of header file
#define _Fiber_H		// notifying that this file is included


//#include <CGAL/Boolean_set_operations_2.h>
//#include <CGAL/intersections.h>

#ifdef __X11__
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/freeglut_ext.h>
#include <opencv2/highgui/highgui.hpp>
#endif	// __X11__

#ifdef __MAC__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#endif	// __MAC__



#include "Common.h"
#include "Mesh.h"
#include "Graph.h"


//------------------------------------------------------------------------------
//	Macro definitions
//------------------------------------------------------------------------------
//#define REGULAR_SLAB		(0)

//------------------------------------------------------------------------------
//	Ohter customized type definitions
//------------------------------------------------------------------------------

// set of contracted Contour Tree
typedef enum { no_data, no_singular, Merge_Split, Birth_Death, Both }   FiberType;
typedef std::vector< FiberType >     FiberMap;
typedef std::vector< unsigned int >	Singularity;

//------------------------------------------------------------------------------
//	Function declaration
//------------------------------------------------------------------------------

extern void fiberDetector( const unsigned int & sizeX,
			   const unsigned int & sizeY,
                           std::vector< std::vector< VertexDescriptor > > & buffer_vertex,
                           Graph & g,
                           FiberMap & fiber,
			   Singularity & singular );
extern void testFiber( void );
extern void setFiberColor( FiberMap fiber, GLfloat * buf );




#endif // _Fiber_H

// end of header file
// Do not add any stuff under this line.



