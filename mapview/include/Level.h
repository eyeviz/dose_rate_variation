//==============================================================================
// Level.h
//	: header file for level sets
//
//------------------------------------------------------------------------------
//
//				Date: Tue Jun  6 12:44:32 2017
//
//==============================================================================

#ifndef _Level_H		// begining of header file
#define _Level_H		// notifying that this file is included

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include "Graph.h"
#include "Mesh.h"
#include "Fiber.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Defining Classes
//------------------------------------------------------------------------------

class Level {

  private:

  protected:
    
    Tuple3			_res;

    // PQ
    std::vector< VecFace >	_facePQ;
    std::vector< VecHalf >	_edgePQ;
    std::vector< VecVertex >	_vertexPQ;
    std::vector< SetUInt >	_projPQ;
    std::vector< SetFace >	_compPQ;
    Graph			_netPQ;
    std::vector< FiberType >	_fiberPQ;
    Singularity			_singularPQ;

    // PR
    std::vector< VecFace >	_facePR;
    std::vector< VecHalf >	_edgePR;
    std::vector< VecVertex >	_vertexPR;
    std::vector< SetUInt >	_projPR;
    std::vector< SetFace >	_compPR;
    Graph			_netPR;
    std::vector< FiberType >	_fiberPR;
    Singularity			_singularPR;

    // QR
    std::vector< VecFace >	_faceQR;
    std::vector< VecHalf >	_edgeQR;
    std::vector< VecVertex >	_vertexQR;
    std::vector< SetUInt >	_projQR;
    std::vector< SetFace >	_compQR;
    Graph			_netQR;
    std::vector< FiberType >	_fiberQR;
    Singularity			_singularQR;

  public:

//------------------------------------------------------------------------------
//	コンストラクタ
//	Constructors
//------------------------------------------------------------------------------
    Level();			// default constructor
				// コンストラクタ(デフォルト)	
    Level( const Level & obj );
				// copy constructor
				// コピー・コンストラクタ

//------------------------------------------------------------------------------
//	デストラクタ
//	Destructor
//------------------------------------------------------------------------------
    virtual ~Level();		// destructor
				// デストラクタ

//------------------------------------------------------------------------------
//	メンバ参照
//	Referrring to members
//------------------------------------------------------------------------------
    const Tuple3 &			res( void ) const	{ return _res; }
    Tuple3 &				res( void )		{ return _res; }

    // PQ
    const std::vector< VecFace > &	facePQ( void ) const	{ return _facePQ; }
    std::vector< VecFace > &		facePQ( void )		{ return _facePQ; }
    
    const std::vector< VecHalf > &	edgePQ( void ) const	{ return _edgePQ; }
    std::vector< VecHalf > &		edgePQ( void )		{ return _edgePQ; }

    const std::vector< VecVertex > &	vertexPQ( void ) const	{ return _vertexPQ; }
    std::vector< VecVertex > &		vertexPQ( void )	{ return _vertexPQ; }

    const std::vector< SetUInt > &	projPQ( void ) const	{ return _projPQ; }
    std::vector< SetUInt > &		projPQ( void )		{ return _projPQ; }

    const std::vector< SetFace > &	compPQ( void ) const	{ return _compPQ; }
    std::vector< SetFace > &		compPQ( void )		{ return _compPQ; }

    const Graph &			netPQ( void ) const	{ return _netPQ; }
    Graph &				netPQ( void )		{ return _netPQ; }

    const std::vector< FiberType > &	fiberPQ( void ) const	{ return _fiberPQ; }	
    std::vector< FiberType > &		fiberPQ( void )		{ return _fiberPQ; }	

    const Singularity &			singularPQ( void ) const{ return _singularPQ; }	
    Singularity &			singularPQ( void )	{ return _singularPQ; }	


    // PR
    const std::vector< VecFace > &	facePR( void ) const	{ return _facePR; }
    std::vector< VecFace > &		facePR( void )		{ return _facePR; }

    const std::vector< VecHalf > &	edgePR( void ) const	{ return _edgePR; }
    std::vector< VecHalf > &		edgePR( void )		{ return _edgePR; }

    const std::vector< VecVertex > &	vertexPR( void ) const	{ return _vertexPR; }
    std::vector< VecVertex > &		vertexPR( void )	{ return _vertexPR; }

    const std::vector< SetUInt > &	projPR( void ) const	{ return _projPR; }
    std::vector< SetUInt > &		projPR( void )		{ return _projPR; }

    const std::vector< SetFace > &	compPR( void ) const	{ return _compPR; }
    std::vector< SetFace > &		compPR( void )		{ return _compPR; }

    const Graph &			netPR( void ) const	{ return _netPR; }
    Graph &				netPR( void )		{ return _netPR; }

    const std::vector< FiberType > &		fiberPR( void ) const	{ return _fiberPR; }	
    std::vector< FiberType > &		fiberPR( void )		{ return _fiberPR; }	

    const Singularity &			singularPR( void ) const{ return _singularPR; }	
    Singularity &			singularPR( void )	{ return _singularPR; }	


    // QR
    const std::vector< VecFace > &	faceQR( void ) const	{ return _faceQR; }
    std::vector< VecFace > &		faceQR( void )		{ return _faceQR; }

    const std::vector< VecHalf > &	edgeQR( void ) const	{ return _edgeQR; }
    std::vector< VecHalf > &		edgeQR( void )		{ return _edgeQR; }

    const std::vector< VecVertex > &	vertexQR( void ) const	{ return _vertexQR; }
    std::vector< VecVertex > &		vertexQR( void )	{ return _vertexQR; }

    const std::vector< SetUInt > &	projQR( void ) const	{ return _projQR; }
    std::vector< SetUInt > &		projQR( void )		{ return _projQR; }

    const std::vector< SetFace > &	compQR( void ) const	{ return _compQR; }
    std::vector< SetFace > &		compQR( void )		{ return _compQR; }

    const Graph &			netQR( void ) const	{ return _netQR; }
    Graph &				netQR( void )		{ return _netQR; }

    const std::vector< FiberType > &	fiberQR( void ) const	{ return _fiberQR; }	
    std::vector< FiberType > &		fiberQR( void )		{ return _fiberQR; }	

    const Singularity &			singularQR( void ) const{ return _singularQR; }	
    Singularity &			singularQR( void )	{ return _singularQR; }	

//------------------------------------------------------------------------------
//	代入演算子
//	Assignment opereators
//------------------------------------------------------------------------------
    Level & operator = ( const Level & obj );
				// assignment
				// 代入

//------------------------------------------------------------------------------
//	クラス名
//	Class name
//------------------------------------------------------------------------------
    virtual const char * className( void ) const { return "Level"; }
				// class name
				// クラス名
};



#endif // _Level_H

// end of header file
// Do not add any stuff under this line.
