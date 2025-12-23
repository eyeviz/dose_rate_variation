//==============================================================================
// Level.cc
//	: program file for level sets.
//
//------------------------------------------------------------------------------
//
//				Date: Mon Jun  5 23:20:48 2017
//
//==============================================================================

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include "Level.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Private Functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Protected Functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Public Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	コンストラクタ
//	Constructors
//------------------------------------------------------------------------------

//
//  Level::Level --	default constructor
//			コンストラクタ(デフォルト)
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Level::Level() 
{
    // PQ
    _facePQ.clear();
    _edgePQ.clear();
    _vertexPQ.clear();
    _projPQ.clear();
    _compPQ.clear();
    _netPQ.clear();
    _fiberPQ.clear();
    _singularPQ.clear();
    // PR
    _facePR.clear();
    _edgePR.clear();
    _vertexPR.clear();
    _projPR.clear();
    _compPR.clear();
    _netPR.clear();
    _fiberPR.clear();
    _singularPR.clear();
    // QR
    _faceQR.clear();
    _edgeQR.clear();
    _vertexQR.clear();
    _projQR.clear();
    _compQR.clear();
    _netQR.clear();
    _fiberQR.clear();
    _singularQR.clear();
}


//
//  Level::Level --	copy constructor
//			コピー・コンストラクタ
//
//  Inputs
//	obj	: object of this class
//
//  Outputs
//	none
//
Level::Level( const Level & obj )
{
    _res		= obj._res;
    // PQ
    _facePQ		= obj._facePQ;
    _edgePQ		= obj._edgePQ;
    _vertexPQ		= obj._vertexPQ;
    _projPQ		= obj._projPQ;
    _compPQ		= obj._compPQ;
    _netPQ		= obj._netPQ;
    _fiberPQ		= obj._fiberPQ;
    _singularPQ		= obj._singularPQ;
    // PR
    _facePR		= obj._facePR;
    _edgePR		= obj._edgePR;
    _vertexPR		= obj._vertexPR;
    _projPR		= obj._projPR;
    _compPR		= obj._compPR;
    _netPR		= obj._netPR;
    _fiberPR		= obj._fiberPR;
    _singularPR		= obj._singularPR;
    // QR
    _faceQR		= obj._faceQR;
    _edgeQR		= obj._edgeQR;
    _vertexQR		= obj._vertexQR;
    _projQR		= obj._projQR;
    _compQR		= obj._compQR;
    _netQR		= obj._netQR;
    _fiberQR		= obj._fiberQR;
    _singularQR		= obj._singularQR;
}


//------------------------------------------------------------------------------
//	デストラクタ
//	Destructor
//------------------------------------------------------------------------------

//
//  Level::~Level --	destructor
//			デストラクタ
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Level::~Level()
{
    // PQ
    _facePQ.clear();
    _edgePQ.clear();
    _vertexPQ.clear();
    _projPQ.clear();
    _compPQ.clear();
    _netPQ.clear();
    _fiberPQ.clear();
    _singularPQ.clear();
    // PR
    _facePR.clear();
    _edgePR.clear();
    _vertexPR.clear();
    _projPR.clear();
    _compPR.clear();
    _netPR.clear();
    _fiberPR.clear();
    _singularPR.clear();
    // QR
    _faceQR.clear();
    _edgeQR.clear();
    _vertexQR.clear();
    _projQR.clear();
    _compQR.clear();
    _netQR.clear();
    _fiberQR.clear();
    _singularQR.clear();
}


//------------------------------------------------------------------------------
//	メンバ参照
//	Referrring to members
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//	代入演算子
//	Assignment opereators
//------------------------------------------------------------------------------

//
//  Level::operator = --	assignement
//				代入
//
//  Inputs
//	obj	: objects of this class
//
//  Outputs
//	this object
//
Level & Level::operator = ( const Level & obj )
{
    if ( this != &obj ) {
	_res		= obj._res;
	// PQ
	_facePQ		= obj._facePQ;
	_edgePQ		= obj._edgePQ;
	_vertexPQ	= obj._vertexPQ;
	_projPQ		= obj._projPQ;
	_compPQ		= obj._compPQ;
	_netPQ		= obj._netPQ;
	_fiberPQ	= obj._fiberPQ;
	_singularPQ	= obj._singularPQ;
	// PR
	_facePR		= obj._facePR;
	_edgePR		= obj._edgePR;
	_vertexPR	= obj._vertexPR;
	_projPR		= obj._projPR;
	_compPR		= obj._compPR;
	_netPR		= obj._netPR;
	_fiberPR	= obj._fiberPR;
	_singularPR	= obj._singularPR;
	// QR
	_faceQR		= obj._faceQR;
	_edgeQR		= obj._edgeQR;
	_vertexQR	= obj._vertexQR;
	_projQR		= obj._projQR;
	_compQR		= obj._compQR;
	_netQR		= obj._netQR;
	_fiberQR	= obj._fiberQR;
	_singularQR	= obj._singularQR;
    }
    return *this;
}


// end of header file
// Do not add any stuff under this line.
