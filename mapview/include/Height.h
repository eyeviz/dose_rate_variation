//==============================================================================
// Height.h
//	: header file for the height fields
//
//------------------------------------------------------------------------------
//
//				Time-stamp: "2025-12-08 20:26:05 shigeo"
//
//==============================================================================

#ifndef _Height_H		// begining of header file
#define _Height_H		// notifying that this file is included

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include "Delaunay.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------

#ifndef LIMIT_EDGE_LENGTH
//#define LIMIT_EDGE_LENGTH	(0.01)
//#define LIMIT_EDGE_LENGTH	(0.02)
//#define LIMIT_EDGE_LENGTH	(0.10)
//#define LIMIT_EDGE_LENGTH	(0.20)
//#define LIMIT_EDGE_LENGTH	(0.50)
#define LIMIT_EDGE_LENGTH	(1.00)
#endif	// LIMIT_EDGE_LENGTH


// longitude difference
// 1 unit = 80km in longitude
// => 0.00125 unit = 100m
//#define Dx	(1.0/800.0)
// => 0.001875 unit = 150m
// #define Dx	(3.0/1600.0) //
// => 0.0025 unit = 200m
#define Dx	(1.0/400.0) //
// => 0.003125 unit = 250m
//#define Dx	(1.0/320.0) //<= This produces better results. 
// => 0.00624  unit = 500m
// #define Dx	(1.0/160.0)
// latitude difference
// 2/3 unit = 80km in longitude
// => 0.00083333 unit = 100m
// #define Dy	(2.0/(3.0*800.0))
// => 0.00125 unit = 150m
// #define Dy	(2.0/1600.0) //
// => 0.00166666 unit = 200m
#define Dy	(2.0/(3.0*400.0)) //
// => 0.00208333 unit = 250m
//#define Dy	(2.0/(3.0*320.0)) //<= This produces better results. 
// => 0.00416666 unit = 500m
// #define Dy	(2.0/(3.0*160.0))



//------------------------------------------------------------------------------
//	Defining Classes
//------------------------------------------------------------------------------

class Height {

  private:

  protected:

    Delaunay						_T;

    unsigned int					_nValues;

    unsigned int					_idP;
    unsigned int					_idQ;
    unsigned int					_idR;
    
#ifdef USING_UBLAS_VECTORS
    ublas::vector< double, std::vector< double > >	_lambda;
    ublas::vector< double, std::vector< double > >	_phi;
    ublas::vector< double, std::vector< double > >	_coordX;
    ublas::vector< double, std::vector< double > >	_coordY;
    ublas::vector< double, std::vector< double > >	_attrH;
    ublas::vector< double, std::vector< double > >	_attrD;
    // ublas::vector< int, std::vector< int > >		_attrLand;
    ublas::vector< double, std::vector< double > >	_attrG;

    ublas::vector< double, std::vector< double > >	_attrPriAve;
    ublas::vector< double, std::vector< double > >	_attrPosAve;
    ublas::vector< double, std::vector< double > >	_attrPriSdv;
    ublas::vector< double, std::vector< double > >	_attrPosSdv;
    ublas::vector< double, std::vector< double > >	_attrRatioF;
    ublas::vector< double, std::vector< double > >	_attrRatioM;
    ublas::vector< double, std::vector< double > >	_attrRatioL;

    ublas::matrix< double >				_attrValue;

    ublas::vector< double, std::vector< double > >	_attrP;
    ublas::vector< double, std::vector< double > >	_attrQ;
    ublas::vector< double, std::vector< double > >	_attrR;
#else	// USING_UBLAS_VECTORS
    std::vector< double >				_lambda;
    std::vector< double >				_phi;
    std::vector< double >				_coordX;
    std::vector< double >				_coordY;
    std::vector< double >				_attrH;
    std::vector< double >				_attrD;
    // std::vector< int >					_attrLand;
    std::vector< double >				_attrG;

    std::vector< double >				_attrPriAve;
    std::vector< double >				_attrPosAve;
    std::vector< double >				_attrPriSdv;
    std::vector< double >				_attrPosSdv;
    std::vector< double >				_attrRatioF;
    std::vector< double >				_attrRatioM;
    std::vector< double >				_attrRatioL;

    std::vector< std::vector< double > >		_attrValue;

    std::vector< double >				_attrP;
    std::vector< double >				_attrQ;
    std::vector< double >				_attrR;

#endif	// USING_UBLAS_VECTORS
    // ublas::matrix< double >				_attrP;
	
    interval< double >					_rngX;
    interval< double >					_rngY;
    interval< double >					_rngH;
    interval< double >					_rngD;
    interval< double >					_rngG;
    interval< double >					_rngPriAve;
    interval< double >					_rngPosAve;
    interval< double >					_rngPriSdv;
    interval< double >					_rngPosSdv;
    interval< double >					_rngRatioF;
    interval< double >					_rngRatioM;
    interval< double >					_rngRatioL;
    interval< double >					_rngP;
    interval< double >					_rngQ;
    interval< double >					_rngR;

    // vector< vector< int > >				_triID;
    std::vector< std::vector< int > > 			_triID;

    void _loadAttrs	( const ublas::matrix< double > & region );
    void _autoNorm	( std::vector< D_Point2 > & sample,
			  interval< double > & rngH,
			  interval< double > & rngD,
			  interval< double > & rngG,
			  interval< double > & rngPriAve,
			  interval< double > & rngPosAve,
			  interval< double > & rngPriSdv,
			  interval< double > & rngPosSdv,
			  interval< double > & rngRatioF,
			  interval< double > & rngRatioM,
			  interval< double > & rngRatioL,
			  interval< double > & rngP,
			  interval< double > & rngQ,
			  interval< double > & rngR );
    void _manualNorm	( std::vector< D_Point2 > & sample,
			  interval< double > & rngH,
			  interval< double > & rngD,
			  interval< double > & rngG,
			  interval< double > & rngPriAve,
			  interval< double > & rngPosAve,
			  interval< double > & rngPriSdv,
			  interval< double > & rngPosSdv,
			  interval< double > & rngRatioF,
			  interval< double > & rngRatioM,
			  interval< double > & rngRatioL,
			  interval< double > & rngP,
			  interval< double > & rngQ,
			  interval< double > & rngR );
    void _triangulate	( const std::vector< D_Point2 > & sample );
    void _retrieve	( void );
    
  public:

//------------------------------------------------------------------------------
//	Constructors
//------------------------------------------------------------------------------
    Height();			// default constructor
    Height( const Height & obj );
				// copy constructor

//------------------------------------------------------------------------------
//	Destructor
//------------------------------------------------------------------------------
    ~Height();			// destructor

//------------------------------------------------------------------------------
//	Referrring to members
//------------------------------------------------------------------------------
    Delaunay &			T( void )		{ return _T; }
    const Delaunay &		T( void ) const		{ return _T; }

    const unsigned int &		nValues( void ) const	{ return _nValues; }
    unsigned int &			nValues( void )		{ return _nValues; }
    void				setNValues( unsigned int __nValues ) {
	_nValues = __nValues;
    };

    const unsigned int &		idP( void ) const	{ return _idP; }
    unsigned int &			idP( void )		{ return _idP; }
    void 				setIDP( const unsigned int & __idP ) {
	_idP = __idP;
    }

    const unsigned int &		idQ( void ) const	{ return _idQ; }
    unsigned int &			idQ( void )		{ return _idQ; }
    void				setIDQ( const unsigned int & __idQ ) {
	_idQ = __idQ;
    }

    const unsigned int &		idR( void ) const	{ return _idR; }
    unsigned int &			idR( void )		{ return _idR; }
    void				setIDR( const unsigned int & __idR ) {
	_idR = __idR;
    }
    
    std::vector< double > &		lambda( void )		{ return _lambda; }
    const std::vector< double > &	lambda( void ) const	{ return _lambda; }
    std::vector< double > &		phi( void )		{ return _phi; }
    const std::vector< double > &	phi( void ) const	{ return _phi; }

    std::vector< double > &		coordX( void )		{ return _coordX; }
    const std::vector< double > &	coordX( void ) const	{ return _coordX; }
    std::vector< double > &		coordY( void )		{ return _coordY; }
    const std::vector< double > &	coordY( void ) const	{ return _coordY; }
    std::vector< double > &		attrH( void )		{ return _attrH; }
    const std::vector< double > &	attrH( void ) const	{ return _attrH; }
    std::vector< double > &		attrD( void )		{ return _attrD; }
    const std::vector< double > &	attrD( void ) const	{ return _attrD; }

    // std::vector< int > &		attrLand( void )	{ return _attrLand; }
    // const std::vector< int > &		attrLand( void ) const	{ return _attrLand; }
    std::vector< double > &		attrG( void )		{ return _attrG; }
    const std::vector< double > &	attrG( void ) const	{ return _attrG; }

    std::vector< double > &		attrPriAve( void )	{ return _attrPriAve; }
    const std::vector< double > &	attrPriAve( void ) const{ return _attrPriAve; }
    std::vector< double > &		attrPosAve( void )	{ return _attrPosAve; }
    const std::vector< double > &	attrPosAve( void ) const{ return _attrPosAve; }
    std::vector< double > &		attrPriSdv( void )	{ return _attrPriSdv; }
    const std::vector< double > &	attrPriSdv( void ) const{ return _attrPriSdv; }
    std::vector< double > &		attrPosSdv( void )	{ return _attrPosSdv; }
    const std::vector< double > &	attrPosSdv( void ) const{ return _attrPosSdv; }

    std::vector< double > &		attrRatioF( void )	{ return _attrRatioF; }
    const std::vector< double > &	attrRatioF( void ) const{ return _attrRatioF; }
    std::vector< double > &		attrRatioM( void )	{ return _attrRatioM; }
    const std::vector< double > &	attrRatioM( void ) const{ return _attrRatioM; }
    std::vector< double > &		attrRatioL( void )	{ return _attrRatioL; }
    const std::vector< double > &	attrRatioL( void ) const{ return _attrRatioL; }

    const std::vector< std::vector< double > > &	attrValue( void ) const {
	return _attrValue;
    }
    std::vector< std::vector< double > > &		attrValue( void ) {
	return _attrValue;
    }
    
    std::vector< double > &		attrP( void )		{ return _attrP; }
    const std::vector< double > &	attrP( void ) const	{ return _attrP; }
    std::vector< double > &		attrQ( void )		{ return _attrQ; }
    const std::vector< double > &	attrQ( void ) const	{ return _attrQ; }
    std::vector< double > &		attrR( void )		{ return _attrR; }
    const std::vector< double > &	attrR( void ) const	{ return _attrR; }

    interval< double > &		rngX( void )		{ return _rngX; }
    const interval< double > &		rngX( void ) const	{ return _rngX; }
    interval< double > &		rngY( void )		{ return _rngY; }
    const interval< double > &		rngY( void ) const	{ return _rngY; }
    interval< double > &		rngH( void )		{ return _rngH; }
    const interval< double > &		rngH( void ) const	{ return _rngH; }
    interval< double > &		rngD( void )		{ return _rngD; }
    const interval< double > &		rngD( void ) const	{ return _rngD; }
    interval< double > &		rngG( void )		{ return _rngG; }
    const interval< double > &		rngG( void ) const	{ return _rngG; }
    interval< double >			rngPriAve( void )	{ return _rngPriAve; }
    const interval< double >		rngPriAve( void ) const	{ return _rngPriAve; }
    interval< double >			rngPosAve( void )	{ return _rngPosAve; }
    const interval< double >		rngPosAve( void ) const	{ return _rngPosAve; }
    interval< double >			rngPriSdv( void )	{ return _rngPriSdv; }
    const interval< double >		rngPriSdv( void ) const	{ return _rngPriSdv; }
    interval< double >			rngPosSdv( void )	{ return _rngPosSdv; }
    const interval< double >		rngPosSdv( void ) const	{ return _rngPosSdv; }
    interval< double >			rngRatioF( void )	{ return _rngRatioF; }
    const interval< double >		rngRatioF( void ) const	{ return _rngRatioF; }
    interval< double >			rngRatioM( void )	{ return _rngRatioM; }
    const interval< double >		rngRatioM( void ) const	{ return _rngRatioM; }
    interval< double >			rngRatioL( void )	{ return _rngRatioL; }
    const interval< double >		rngRatioL( void ) const	{ return _rngRatioL; }

    interval< double >			rngP( void )		{ return _rngP; }
    const interval< double >		rngP( void ) const	{ return _rngP; }
    interval< double >			rngQ( void )		{ return _rngQ; }
    const interval< double >		rngQ( void ) const	{ return _rngQ; }
    interval< double >			rngR( void )		{ return _rngR; }
    const interval< double >		rngR( void ) const	{ return _rngR; }

    std::vector< std::vector< int > >  & triID( void )		{ return _triID; }
    const std::vector< std::vector< int > > & triID( void ) const
								{ return _triID; }

//------------------------------------------------------------------------------
//	Fundamental functions
//------------------------------------------------------------------------------
    void autoTessellate		( const ublas::matrix< double > & region,
				  interval< double > & rngH,
				  interval< double > & rngD,
				  interval< double > & rngG,
				  interval< double > & rngPriAve,
				  interval< double > & rngPosAve,
				  interval< double > & rngPriSdv,
				  interval< double > & rngPosSdv,
				  interval< double > & rngRatioF,
				  interval< double > & rngRatioM,
				  interval< double > & rngRatioL,
				  interval< double > & rngP,
				  interval< double > & rngQ,
				  interval< double > & rngR );
    void manualTessellate	( const ublas::matrix< double > & region,
				  interval< double > & rngH,
				  interval< double > & rngD,
				  interval< double > & rngG,
				  interval< double > & rngPriAve,
				  interval< double > & rngPosAve,
				  interval< double > & rngPriSdv,
				  interval< double > & rngPosSdv,
				  interval< double > & rngRatioF,
				  interval< double > & rngRatioM,
				  interval< double > & rngRatioL,
				  interval< double > & rngP,
				  interval< double > & rngQ,
				  interval< double > & rngR );
    
//------------------------------------------------------------------------------
//	Curvature staff
//------------------------------------------------------------------------------
    double HF		( const double & x, const double & y );
    double HFx		( const double & x, const double & y );
    double HFy		( const double & x, const double & y );
    double HFxx		( const double & x, const double & y );
    double HFxy		( const double & x, const double & y );
    double HFyy		( const double & x, const double & y );

    void firstFund	( const double & x, const double & y,
			  double & E, double & F, double & G );
    void secondFund	( const double & x, const double & y,
			  double & L, double & M, double & N );
    void curvature	( const double & x, const double & y,
			  double & K, double & H );
    void principal	( const double & x, const double & y,
			  double & k1, double & k2 );
    void eigen		( const double & x, const double & y,
			  double & k1, double & k2,
			  std::vector< double > & e1, std::vector< double > & e2 );
    int drainage	( const double & x, const double & y );
    int drainage	( const double & x, const double & y,
			  const double & k1, const double & k2,
			  const std::vector< double > & e1,
			  const std::vector< double > & e2,
			  double & ridgeDegree, double & ravineDegree );

//------------------------------------------------------------------------------
//	Assignment opereators
//------------------------------------------------------------------------------
    Height & operator = ( const Height & obj );
				// assignment

//------------------------------------------------------------------------------
//	I/O functions
//------------------------------------------------------------------------------
#ifdef SKIP
    friend ostream & operator << ( ostream & stream, const Height & obj );
				// output
    friend istream & operator >> ( istream & stream, Height & obj );
				// input
#endif	// SKIP

//------------------------------------------------------------------------------
//	Class name
//------------------------------------------------------------------------------
    virtual const char * className( void ) const { return "Height"; }
				// class name
};



#endif // _Height_H

// end of header file
// Do not add any stuff under this line.
